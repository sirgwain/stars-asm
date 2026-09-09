package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

type resolveLateBitfieldsProcessor struct {
	ctx *FuncContext
}

// semanticBitRange describes a physical bit range extracted from semantic storage.
type semanticBitRange struct {
	Storage *Deref
	Access  BitfieldAccess
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

	addr := AddressExpr{
		Base:   bitfield.Storage.Pointer,
		Offset: bitfield.Storage.ByteOff,
		Deref:  true,
	}

	converter := machineConverter{ctx: p.ctx}
	return converter.consumeBitfieldAddress(
		addr,
		bitfield.Access,
	)
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

	storage, bitOff, ok := shiftedSemanticStorage(source)
	if !ok {
		return semanticBitRange{}, false
	}

	if !validSemanticBitRange(storage, bitOff, bitWidth) {
		return semanticBitRange{}, false
	}

	return semanticBitRange{
		Storage: storage,
		Access: BitfieldAccess{
			StorageWidth: storage.Width,
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
	storage, ok := unwrapSemanticBitfieldValue(source).(*Deref)
	if !ok || !validSemanticBitRange(storage, bitOff, bitWidth) {
		return semanticBitRange{}, false
	}
	return semanticBitRange{
		Storage: storage,
		Access: BitfieldAccess{
			StorageWidth: storage.Width,
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

	storage, ok := unwrapSemanticBitfieldValue(leftShift.LHS).(*Deref)
	if !ok {
		return semanticBitRange{}, false
	}

	storageBits := storage.Width * 8
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

	if !validSemanticBitRange(storage, bitOff, bitWidth) {
		return semanticBitRange{}, false
	}

	signedness := BitfieldUnsigned
	if rightShift.Op == OpSar {
		signedness = BitfieldSigned
	}
	return semanticBitRange{
		Storage: storage,
		Access: BitfieldAccess{
			StorageWidth: storage.Width,
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
func validSemanticBitRange(storage *Deref, bitOff int, bitWidth int) bool {
	if storage == nil || storage.Width <= 0 || bitOff < 0 || bitWidth <= 0 {
		return false
	}

	storageBits := storage.Width * 8
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

// shiftedSemanticStorage returns a dereference and its accumulated right shift.
func shiftedSemanticStorage(expr Expr) (*Deref, int, bool) {
	value := unwrapSemanticBitfieldValue(expr)
	bitOff := 0

	for {
		shift, ok := value.(*Binary)
		if !ok || (shift.Op != OpShr && shift.Op != OpSar) {
			break
		}

		amount, ok := semanticShiftAmount(shift.RHS)
		if !ok {
			return nil, 0, false
		}

		bitOff += amount
		value = unwrapSemanticBitfieldValue(shift.LHS)
	}

	storage, ok := value.(*Deref)
	return storage, bitOff, ok
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
