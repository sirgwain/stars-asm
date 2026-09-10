package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type resolveLateBitfieldsProcessor struct {
	ctx *FuncContext
}

// semanticBitRange describes a physical bit range extracted from semantic storage.
type semanticBitRange struct {
	Address AddressExpr
	Access  BitfieldAccess
}

// semanticBitfieldWrite describes a destination-preserving semantic bitfield write.
type semanticBitfieldWrite struct {
	Address AddressExpr
	Access  BitfieldAccess
	Value   Expr
}

// ProcessBlock resolves bitfield extracts revealed by semantic scratch-slot substitution.
func (p *resolveLateBitfieldsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	effects, changed := p.rewriter().rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// rewriter returns the semantic tree rewrite for late bitfield resolution.
func (p *resolveLateBitfieldsProcessor) rewriter() *semRewriter {
	return &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			assign, ok := effect.(*Assign)
			if !ok {
				return effect, false, false
			}

			write, ok := semanticBitfieldStore(assign.Dst, assign.Src)
			if !ok {
				return effect, false, false
			}

			converter := machineConverter{ctx: p.ctx}
			dst, ok := converter.consumeBitfieldAddress(write.Address, write.Access)
			if !ok {
				return effect, false, false
			}

			// The recovered value can itself contain a field-relative read created
			// while undoing a compiler read/modify/write sequence. Run the normal
			// expression rewrite over it before installing the assignment.
			src, _ := w.rewriteExpr(write.Value)

			next := *assign
			next.Dst = dst
			next.Src = src
			return &next, true, true
		},
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			if field, ok := p.resolve(expr); ok {
				return field, true, true
			}
			return expr, false, false
		},
	}
}

// resolve matches a semantic bit extraction against a declared bitfield.
func (p *resolveLateBitfieldsProcessor) resolve(expr Expr) (LValue, bool) {
	bitfield, ok := semanticBitfieldExtract(expr)
	if !ok {
		return nil, false
	}

	converter := machineConverter{ctx: p.ctx}
	return converter.consumeBitfieldAddress(
		bitfield.Address,
		bitfield.Access,
	)
}

// semanticBitfieldStore recognizes compiler-generated semantic read/modify/write
// forms and recovers the source-level field assignment.
func semanticBitfieldStore(dst LValue, value Expr) (semanticBitfieldWrite, bool) {
	address, storageWidth, ok := semanticBitfieldStorage(dst)
	if !ok {
		return semanticBitfieldWrite{}, false
	}

	_, keep, set, ok := semanticBitfieldStoreParts(address, storageWidth, value)
	if !ok {
		return semanticBitfieldWrite{}, false
	}

	fullMask, ok := bitMask(storageWidth * 8)
	if !ok {
		return semanticBitfieldWrite{}, false
	}
	changed := (^uint(keep.U64)) & fullMask
	bitOff, bitWidth, ok := contiguousMaskRange(changed)
	if !ok {
		return semanticBitfieldWrite{}, false
	}

	stored, ok := unshiftSemanticBitfieldSet(address, storageWidth, set, bitOff, bitWidth, changed)
	if !ok {
		return semanticBitfieldWrite{}, false
	}

	return semanticBitfieldWrite{
		Address: address,
		Access: BitfieldAccess{
			StorageWidth: storageWidth,
			BitOff:       bitOff,
			BitWidth:     bitWidth,
			Signedness:   BitfieldSignednessUnknown,
		},
		Value: stored,
	}, true
}

// semanticBitfieldStoreParts separates the preserved destination storage, keep
// mask, and inserted field value from a semantic read/modify/write expression.
func semanticBitfieldStoreParts(address AddressExpr, storageWidth int, value Expr) (Expr, *Const, Expr, bool) {
	value = unwrapSemanticBitfieldValue(value)
	if binary, ok := value.(*Binary); ok && (binary.Op == OpOr || binary.Op == OpAdd) {
		if kept, keep, ok := semanticBitfieldKeepMask(address, storageWidth, binary.LHS); ok {
			if binary.Op == OpOr || semanticDisjointBitfieldInsert(storageWidth, keep, binary.RHS) {
				return kept, keep, binary.RHS, true
			}
		}
		if kept, keep, ok := semanticBitfieldKeepMask(address, storageWidth, binary.RHS); ok {
			if binary.Op == OpOr || semanticDisjointBitfieldInsert(storageWidth, keep, binary.LHS) {
				return kept, keep, binary.LHS, true
			}
		}
		return nil, nil, nil, false
	}

	kept, keep, ok := semanticBitfieldKeepMask(address, storageWidth, value)
	if !ok {
		return nil, nil, nil, false
	}
	return kept, keep, &Const{TypeInfo: value.ExprType(), U64: 0}, true
}

// semanticBitfieldKeepMask returns the preserved storage expression and keep
// mask from: storage & keepMask.
func semanticBitfieldKeepMask(address AddressExpr, storageWidth int, value Expr) (Expr, *Const, bool) {
	and, ok := unwrapSemanticBitfieldValue(value).(*Binary)
	if !ok || and.Op != OpAnd {
		return nil, nil, false
	}
	keep, source, ok := semanticConstOperand(and.LHS, and.RHS)
	if !ok {
		return nil, nil, false
	}
	sourceAddress, sourceWidth, ok := semanticBitfieldStorage(source)
	if !ok || sourceWidth != storageWidth || !sameSemanticBitfieldAddress(address, sourceAddress) {
		return nil, nil, false
	}
	return source, keep, true
}

// semanticDisjointBitfieldInsert is the ADD counterpart of OR safety: the
// inserted value must be provably confined to bits cleared by the keep mask.
func semanticDisjointBitfieldInsert(storageWidth int, keep *Const, inserted Expr) bool {
	fullMask, ok := bitMask(storageWidth * 8)
	if !ok {
		return false
	}
	changed := (^uint(keep.U64)) & fullMask
	return semanticValueMaskedWithin(inserted, changed)
}

// semanticValueMaskedWithin reports whether value is provably confined to allowed.
func semanticValueMaskedWithin(value Expr, allowed uint) bool {
	value = unwrapSemanticBitfieldValue(value)
	if constant, ok := value.(*Const); ok {
		return uint(constant.U64)&^allowed == 0
	}
	if and, ok := value.(*Binary); ok && and.Op == OpAnd {
		mask, _, ok := semanticConstOperand(and.LHS, and.RHS)
		return ok && uint(mask.U64)&^allowed == 0
	}
	if shift, ok := value.(*Binary); ok && shift.Op == OpShl {
		amount, ok := semanticShiftAmount(shift.RHS)
		return ok && amount >= 0 && amount < 64 && semanticValueMaskedWithin(shift.LHS, allowed>>amount)
	}
	return false
}

// unshiftSemanticBitfieldSet validates the inserted bits and converts them back
// to the field-relative source value.
func unshiftSemanticBitfieldSet(address AddressExpr, storageWidth int, value Expr, bitOff int, bitWidth int, changed uint) (Expr, bool) {
	value = unwrapSemanticBitfieldValue(value)
	if constant, ok := value.(*Const); ok {
		if uint(constant.U64)&^changed != 0 {
			return nil, false
		}
		fieldMask, ok := bitMask(bitWidth)
		if !ok {
			return nil, false
		}
		next := *constant
		next.U64 = (constant.U64 >> bitOff) & uint64(fieldMask)
		return &next, true
	}

	if arithmetic, ok := unshiftSemanticBitfieldArithmetic(address, storageWidth, value, bitOff, bitWidth, changed); ok {
		return arithmetic, true
	}

	source := value
	if shift, ok := source.(*Binary); ok && shift.Op == OpShl {
		amount, ok := semanticShiftAmount(shift.RHS)
		if !ok || amount != bitOff {
			return nil, false
		}
		source = unwrapSemanticBitfieldValue(shift.LHS)
	} else if bitOff != 0 {
		return nil, false
	}

	and, ok := source.(*Binary)
	if !ok || and.Op != OpAnd {
		return nil, false
	}
	mask, unmasked, ok := semanticConstOperand(and.LHS, and.RHS)
	if !ok {
		return nil, false
	}
	width, ok := lowBitMaskWidth(uint(mask.U64))
	if !ok || width != bitWidth {
		return nil, false
	}
	return unmasked, true
}

// unshiftSemanticBitfieldArithmetic recovers field-relative + / - operations
// such as ((storage + (delta << bitOff)) & changedMask).
func unshiftSemanticBitfieldArithmetic(address AddressExpr, storageWidth int, value Expr, bitOff int, bitWidth int, changed uint) (Expr, bool) {
	and, ok := unwrapSemanticBitfieldValue(value).(*Binary)
	if !ok || and.Op != OpAnd {
		return nil, false
	}
	mask, unmasked, ok := semanticConstOperand(and.LHS, and.RHS)
	if !ok || uint(mask.U64) != changed {
		return nil, false
	}
	arithmetic, ok := unwrapSemanticBitfieldValue(unmasked).(*Binary)
	if !ok || (arithmetic.Op != OpAdd && arithmetic.Op != OpSub) {
		return nil, false
	}

	field, lhsStorage := unshiftSemanticBitfieldStorageOperand(address, storageWidth, arithmetic.LHS, bitOff, bitWidth)
	delta, rhsShifted := unshiftSemanticBitfieldShiftedOperand(arithmetic.RHS, bitOff)
	if lhsStorage && rhsShifted {
		return &Binary{TypeInfo: arithmetic.TypeInfo, Op: arithmetic.Op, LHS: field, RHS: delta}, true
	}
	if arithmetic.Op == OpAdd {
		field, rhsStorage := unshiftSemanticBitfieldStorageOperand(address, storageWidth, arithmetic.RHS, bitOff, bitWidth)
		delta, lhsShifted := unshiftSemanticBitfieldShiftedOperand(arithmetic.LHS, bitOff)
		if rhsStorage && lhsShifted {
			return &Binary{TypeInfo: arithmetic.TypeInfo, Op: OpAdd, LHS: field, RHS: delta}, true
		}
	}
	return nil, false
}

// unshiftSemanticBitfieldStorageOperand converts the full backing storage to a
// normal semantic bitfield extraction so the expression rewrite can resolve it.
func unshiftSemanticBitfieldStorageOperand(address AddressExpr, storageWidth int, value Expr, bitOff int, bitWidth int) (Expr, bool) {
	valueAddress, valueWidth, ok := semanticBitfieldStorage(value)
	if !ok || valueWidth != storageWidth || !sameSemanticBitfieldAddress(address, valueAddress) {
		return nil, false
	}
	mask, ok := bitMask(bitWidth)
	if !ok {
		return nil, false
	}
	field := value
	if bitOff != 0 {
		field = &Binary{TypeInfo: value.ExprType(), Op: OpShr, LHS: field, RHS: &Const{TypeInfo: typeinfo.U16, U64: uint64(bitOff)}}
	}
	return &Binary{TypeInfo: value.ExprType(), Op: OpAnd, LHS: field, RHS: &Const{TypeInfo: value.ExprType(), U64: uint64(mask)}}, true
}

// unshiftSemanticBitfieldShiftedOperand removes a field-position shift from an
// arithmetic delta.
func unshiftSemanticBitfieldShiftedOperand(value Expr, bitOff int) (Expr, bool) {
	value = unwrapSemanticBitfieldValue(value)
	if shift, ok := value.(*Binary); ok && shift.Op == OpShl {
		amount, ok := semanticShiftAmount(shift.RHS)
		if !ok || amount != bitOff {
			return nil, false
		}
		value = unwrapSemanticBitfieldValue(shift.LHS)
	} else if multiply, ok := value.(*Binary); ok && multiply.Op == OpMul {
		factor, source, ok := semanticConstOperand(multiply.LHS, multiply.RHS)
		if !ok || bitOff < 0 || bitOff >= 64 || factor.U64 != uint64(1)<<bitOff {
			return nil, false
		}
		value = unwrapSemanticBitfieldValue(source)
	} else if bitOff != 0 {
		return nil, false
	}
	if extended, ok := value.(*SignExtend); ok {
		value = extended.Parent
	}
	return value, true
}

// sameSemanticBitfieldAddress compares normalized physical semantic addresses.
func sameSemanticBitfieldAddress(a, b AddressExpr) bool {
	if a.Offset != b.Offset || a.Deref != b.Deref || len(a.Terms) != len(b.Terms) || !sameExpr(a.Base, b.Base) {
		return false
	}
	for i := range a.Terms {
		if a.Terms[i].Scale != b.Terms[i].Scale || !sameExpr(a.Terms[i].Expr, b.Terms[i].Expr) {
			return false
		}
	}
	return true
}

// semanticBitfieldExtract recognizes the semantic forms used by the compiler
// to extract a physical bit range from integer storage.
func semanticBitfieldExtract(expr Expr) (semanticBitRange, bool) {
	if bitfield, ok := semanticMaskedBitfieldExtract(expr); ok {
		return bitfield, true
	}
	if bitfield, ok := semanticShiftPairBitfieldExtract(expr); ok {
		return bitfield, true
	}
	return semanticBitRange{}, false
}

// semanticMaskedBitfieldExtract recognizes the equivalent masked extraction forms:
//
//	(storage >> bitOff) & lowMask
//	(storage & shiftedMask) >> bitOff
func semanticMaskedBitfieldExtract(expr Expr) (semanticBitRange, bool) {
	value := unwrapSemanticBitfieldValue(expr)
	if bitfield, ok := semanticMaskAfterShiftBitfieldExtract(value); ok {
		return bitfield, true
	}
	return semanticMaskBeforeShiftBitfieldExtract(value)
}

// semanticMaskAfterShiftBitfieldExtract recognizes a low mask applied after
// a right shift. SAR is accepted because the mask removes sign extension.
func semanticMaskAfterShiftBitfieldExtract(value Expr) (semanticBitRange, bool) {
	and, ok := value.(*Binary)
	if !ok || and.Op != OpAnd {
		return semanticBitRange{}, false
	}

	mask, source, ok := semanticConstOperand(and.LHS, and.RHS)
	if !ok {
		return semanticBitRange{}, false
	}

	bitWidth, ok := lowBitMaskWidth(uint(mask.U64))
	if !ok {
		return semanticBitRange{}, false
	}

	address, storageWidth, bitOff, ok := shiftedSemanticStorage(source)
	if !ok {
		return semanticBitRange{}, false
	}

	if !validSemanticBitRange(storageWidth, bitOff, bitWidth) {
		return semanticBitRange{}, false
	}

	return semanticBitRange{
		Address: address,
		Access: BitfieldAccess{
			StorageWidth: storageWidth,
			BitOff:       bitOff,
			BitWidth:     bitWidth,
			Signedness:   BitfieldSignednessUnknown,
		},
	}, true
}

// semanticMaskBeforeShiftBitfieldExtract recognizes a contiguous shifted mask
// applied before a logical right shift.
func semanticMaskBeforeShiftBitfieldExtract(value Expr) (semanticBitRange, bool) {
	shift, ok := value.(*Binary)
	if !ok || shift.Op != OpShr {
		return semanticBitRange{}, false
	}
	bitOff, ok := semanticShiftAmount(shift.RHS)
	if !ok {
		return semanticBitRange{}, false
	}
	and, ok := unwrapSemanticBitfieldValue(shift.LHS).(*Binary)
	if !ok || and.Op != OpAnd {
		return semanticBitRange{}, false
	}
	mask, source, ok := semanticConstOperand(and.LHS, and.RHS)
	if !ok {
		return semanticBitRange{}, false
	}
	maskOff, bitWidth, ok := contiguousMaskRange(uint(mask.U64))
	if !ok || maskOff != bitOff {
		return semanticBitRange{}, false
	}
	address, storageWidth, ok := semanticBitfieldStorage(source)
	if !ok || !validSemanticBitRange(storageWidth, bitOff, bitWidth) {
		return semanticBitRange{}, false
	}
	return semanticBitRange{
		Address: address,
		Access: BitfieldAccess{
			StorageWidth: storageWidth,
			BitOff:       bitOff,
			BitWidth:     bitWidth,
			Signedness:   BitfieldSignednessUnknown,
		},
	}, true
}

// semanticShiftPairBitfieldExtract recognizes the compiler idiom:
//
//	(storage << left) >> right
//	(storage << left) SAR right
//
// For N-bit storage:
//
//	bitOff   = right - left
//	bitWidth = N - right
//
// The SAR form is a signed bitfield extraction because the selected field's
// high bit is moved into the storage sign position before arithmetic shifting.
func semanticShiftPairBitfieldExtract(expr Expr) (semanticBitRange, bool) {
	value := unwrapSemanticBitfieldValue(expr)

	rightShift, ok := value.(*Binary)
	if !ok || (rightShift.Op != OpShr && rightShift.Op != OpSar) {
		return semanticBitRange{}, false
	}

	right, ok := semanticShiftAmount(rightShift.RHS)
	if !ok {
		return semanticBitRange{}, false
	}

	leftShift, ok := unwrapSemanticBitfieldValue(rightShift.LHS).(*Binary)
	if !ok || leftShift.Op != OpShl {
		return semanticBitRange{}, false
	}

	left, ok := semanticShiftAmount(leftShift.RHS)
	if !ok {
		return semanticBitRange{}, false
	}

	address, storageWidth, ok := semanticBitfieldStorage(leftShift.LHS)
	if !ok {
		return semanticBitRange{}, false
	}

	storageBits := storageWidth * 8
	if storageBits <= 0 ||
		left < 0 ||
		right < 0 ||
		left >= storageBits ||
		right >= storageBits ||
		right < left {
		return semanticBitRange{}, false
	}

	bitOff := right - left
	bitWidth := storageBits - right

	if !validSemanticBitRange(storageWidth, bitOff, bitWidth) {
		return semanticBitRange{}, false
	}

	signedness := BitfieldUnsigned
	if rightShift.Op == OpSar {
		signedness = BitfieldSigned
	}
	return semanticBitRange{
		Address: address,
		Access: BitfieldAccess{
			StorageWidth: storageWidth,
			BitOff:       bitOff,
			BitWidth:     bitWidth,
			Signedness:   signedness,
		},
	}, true
}

// semanticShiftAmount returns a constant semantic shift count.
func semanticShiftAmount(expr Expr) (int, bool) {
	value := unwrapSemanticBitfieldValue(expr)

	constant, ok := value.(*Const)
	if !ok {
		return 0, false
	}

	return int(constant.U64), true
}

// validSemanticBitRange verifies that a physical bit range fits inside its storage.
func validSemanticBitRange(storageWidth int, bitOff int, bitWidth int) bool {
	if storageWidth <= 0 || bitOff < 0 || bitWidth <= 0 {
		return false
	}

	storageBits := storageWidth * 8
	return bitOff+bitWidth <= storageBits
}

// semanticConstOperand separates a constant mask from the other binary operand.
func semanticConstOperand(lhs, rhs Expr) (*Const, Expr, bool) {
	if constant, ok := lhs.(*Const); ok {
		return constant, rhs, true
	}
	if constant, ok := rhs.(*Const); ok {
		return constant, lhs, true
	}
	return nil, nil, false
}

// shiftedSemanticStorage returns a semantic storage address and its accumulated right shift.
func shiftedSemanticStorage(expr Expr) (AddressExpr, int, int, bool) {
	value := unwrapSemanticBitfieldValue(expr)
	bitOff := 0

	for {
		shift, ok := value.(*Binary)
		if !ok || (shift.Op != OpShr && shift.Op != OpSar) {
			break
		}

		amount, ok := semanticShiftAmount(shift.RHS)
		if !ok {
			return AddressExpr{}, 0, 0, false
		}

		bitOff += amount
		value = unwrapSemanticBitfieldValue(shift.LHS)
	}

	address, storageWidth, ok := semanticBitfieldStorage(value)
	return address, storageWidth, bitOff, ok
}

// semanticBitfieldStorage converts semantic physical storage into the address
// form consumed by typed bitfield projection. Late scratch/address recovery may
// leave the backing word either as a dereference or as a Part of a typed object.
func semanticBitfieldStorage(expr Expr) (AddressExpr, int, bool) {
	value := unwrapSemanticBitfieldValue(expr)

	switch storage := value.(type) {
	case *Deref:
		if storage.Width <= 0 {
			return AddressExpr{}, 0, false
		}
		return AddressExpr{
			Base:   storage.Pointer,
			Offset: storage.ByteOff,
			Deref:  true,
		}, storage.Width, true

	case *Part:
		if storage.Width <= 0 || storage.Base == nil {
			return AddressExpr{}, 0, false
		}
		deref := false
		if pointer, ok := storage.Base.ExprType().(*typeinfo.Pointer); ok {
			// LOWORD/HIWORD Parts inside the pointer representation itself are
			// not pointee accesses. Larger offsets on a pointer-typed base are
			// failed typed projections into the pointed-to object.
			deref = storage.ByteOff >= pointer.Bytes()
		}
		return AddressExpr{
			Base:   storage.Base,
			Offset: storage.ByteOff,
			Deref:  deref,
		}, storage.Width, true

	default:
		return AddressExpr{}, 0, false
	}
}

// unwrapSemanticBitfieldValue removes representation-only wrappers from a semantic bitfield value.
func unwrapSemanticBitfieldValue(expr Expr) Expr {
	for {
		switch value := expr.(type) {
		case *Cast:
			expr = value.Value

		case *Word:
			if value.Part != machine.WordLow {
				return expr
			}
			expr = value.Parent

		default:
			return expr
		}
	}
}
