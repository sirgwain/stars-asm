package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type normalizeAssignmentAddressesProcessor struct {
	ctx *FuncContext
}

// ProcessMachineBlock converts constants used in known near-pointer contexts
// into explicit DS-relative machine addresses.
func (p *normalizeAssignmentAddressesProcessor) ProcessMachineBlock(
	result *Result,
	f machine.FuncEffects,
	b machine.BlockEffects,
) (machine.BlockEffects, bool) {
	rewriter := &machineRewriter{
		effect: func(
			w *machineRewriter,
			effect machine.Effect,
		) (machine.Effect, bool, bool) {
			switch e := effect.(type) {
			case machine.StoreEffect:
				src, changed := p.normalizeStoreAddress(e)
				if !changed {
					return e, false, true
				}

				e.Src = src
				return e, true, true

			case machine.BranchEffect:
				predicate, changed := p.normalizePredicateAddresses(e.Predicate)
				if !changed {
					return e, false, true
				}

				e.Predicate = predicate
				return e, true, true

			case machine.ReturnEffect:
				value, changed := p.normalizeReturnAddress(e)
				if !changed {
					return e, false, true
				}

				e.Value = value
				return e, true, true

			default:
				return nil, false, false
			}
		},
	}

	effects, changed := rewriter.rewriteMachineEffects(b.Effects)
	if !changed {
		return b, false
	}

	b.Effects = effects
	return b, true
}

// ProcessBlock converts constants used in semantic near-pointer contexts into
// resolved source-level address expressions.
func (p *normalizeAssignmentAddressesProcessor) ProcessBlock(
	result *Result,
	f Func,
	b Block,
) (Block, bool) {
	rewriter := &semRewriter{
		effect: func(
			w *semRewriter,
			effect Effect,
		) (Effect, bool, bool) {
			switch e := effect.(type) {
			case *Assign:
				dst, dstChanged := w.rewriteLValue(e.Dst)
				src, srcChanged := w.rewriteExpr(e.Src)
				src, srcNormalized := p.normalizeSemanticAssignSrc(dst, src)
				if !dstChanged && !srcChanged && !srcNormalized {
					return e, false, true
				}

				next := *e
				next.Dst = dst
				next.Src = src
				return &next, true, true

			case *Branch:
				cond, condChanged := w.rewriteExpr(e.Cond)
				cond, condNormalized := p.normalizeSemanticPointerCompare(cond)
				if !condChanged && !condNormalized {
					return e, false, true
				}

				next := *e
				next.Cond = cond
				return &next, true, true

			case *Return:
				value, valueChanged := w.rewriteExpr(e.Value)
				value, valueNormalized := p.normalizeSemanticReturnValue(value)
				if !valueChanged && !valueNormalized {
					return e, false, true
				}

				next := *e
				next.Value = value
				return &next, true, true

			default:
				return nil, false, false
			}
		},
	}

	effects, changed := rewriter.rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}

	next := b
	next.Effects = effects
	return next, true
}

// normalizeStoreAddress recognizes a DGROUP offset stored into near-pointer
// storage and makes its address semantics explicit.
func (p *normalizeAssignmentAddressesProcessor) normalizeStoreAddress(
	store machine.StoreEffect,
) (machine.Value, bool) {
	if store.Width != 2 {
		return store.Src, false
	}

	dst, ok := p.ctx.resolveMachineStorage(store.Addr, store.Width)
	if !ok || !isNearAddressStorage(dst.ExprType(), store.Width) {
		return store.Src, false
	}

	return p.normalizeNearAddressConst(store.Src)
}

// normalizeReturnAddress recognizes a DGROUP offset returned from a
// near-pointer function and makes its address semantics explicit.
func (p *normalizeAssignmentAddressesProcessor) normalizeReturnAddress(
	ret machine.ReturnEffect,
) (machine.Value, bool) {
	retType, ok := p.nearPointerReturnType()
	if !ok || retType.Bytes() != 2 {
		return ret.Value, false
	}

	return p.normalizeNearAddressConst(ret.Value)
}

// normalizeSemanticAssignSrc recognizes a DGROUP offset assigned into
// near-pointer storage and makes its address semantics explicit.
func (p *normalizeAssignmentAddressesProcessor) normalizeSemanticAssignSrc(
	dst LValue,
	src Expr,
) (Expr, bool) {
	if dst == nil {
		return src, false
	}
	dstType := dst.ExprType()
	if dstType == nil || !isNearAddressStorage(dstType, dstType.Bytes()) {
		return src, false
	}

	return p.normalizeSemanticNearAddressConst(src, dstType)
}

// normalizeSemanticReturnValue recognizes a DGROUP offset returned from a
// near-pointer function and makes its address semantics explicit.
func (p *normalizeAssignmentAddressesProcessor) normalizeSemanticReturnValue(
	value Expr,
) (Expr, bool) {
	retType, ok := p.nearPointerReturnType()
	if !ok {
		return value, false
	}

	return p.normalizeSemanticNearAddressConst(value, retType)
}

// nearPointerReturnType returns the current function return type when it is
// represented by a two-byte near pointer.
func (p *normalizeAssignmentAddressesProcessor) nearPointerReturnType() (typeinfo.Type, bool) {
	if p.ctx == nil || p.ctx.fs == nil || p.ctx.fs.Ret == nil {
		return nil, false
	}
	if !isNearAddressStorage(p.ctx.fs.Ret, p.ctx.fs.Ret.Bytes()) {
		return nil, false
	}
	return p.ctx.fs.Ret, true
}

// normalizePredicateAddresses recognizes pointer equality comparisons against
// constants that are actually DGROUP offsets.
func (p *normalizeAssignmentAddressesProcessor) normalizePredicateAddresses(
	predicate *machine.PredicateValue,
) (*machine.PredicateValue, bool) {
	if predicate == nil || predicate.Kind != machine.PredicateCompare {
		return predicate, false
	}

	// Start conservatively with pointer equality comparisons. These are the
	// cases where an immediate DGROUP offset is unambiguously an address.
	switch machine.JccCompareOp(predicate.Op) {
	case "==", "!=":
	default:
		return predicate, false
	}

	if p.isNearAddressValue(predicate.LHS) {
		if rhs, changed := p.normalizeNearAddressConst(predicate.RHS); changed {
			next := *predicate
			next.RHS = rhs
			return &next, true
		}
	}

	if p.isNearAddressValue(predicate.RHS) {
		if lhs, changed := p.normalizeNearAddressConst(predicate.LHS); changed {
			next := *predicate
			next.LHS = lhs
			return &next, true
		}
	}

	return predicate, false
}

// normalizeSemanticPointerCompare recognizes pointer equality comparisons
// against constants that are actually DGROUP offsets.
func (p *normalizeAssignmentAddressesProcessor) normalizeSemanticPointerCompare(
	cond Expr,
) (Expr, bool) {
	compare, ok := cond.(*Compare)
	if !ok {
		return cond, false
	}

	switch compare.Op {
	case CompareEQ, CompareNE:
	default:
		return cond, false
	}

	if compare.LHS != nil {
		if lhsType := compare.LHS.ExprType(); lhsType != nil && isNearAddressStorage(lhsType, lhsType.Bytes()) {
			if rhs, changed := p.normalizeSemanticNearAddressConst(compare.RHS, lhsType); changed {
				next := *compare
				next.RHS = rhs
				return &next, true
			}
		}
	}

	if compare.RHS != nil {
		if rhsType := compare.RHS.ExprType(); rhsType != nil && isNearAddressStorage(rhsType, rhsType.Bytes()) {
			if lhs, changed := p.normalizeSemanticNearAddressConst(compare.LHS, rhsType); changed {
				next := *compare
				next.LHS = lhs
				return &next, true
			}
		}
	}

	return cond, false
}

// isNearAddressValue reports whether a machine value loads storage whose
// declared representation is a two-byte near pointer.
func (p *normalizeAssignmentAddressesProcessor) isNearAddressValue(
	value machine.Value,
) bool {
	load, ok := value.(*machine.Load)
	if !ok || load.Access.Width != 2 {
		return false
	}

	storage, ok := p.ctx.resolveMachineStorage(
		load.Access,
		load.Access.Width,
	)
	return ok &&
		isNearAddressStorage(storage.ExprType(), load.Access.Width)
}

// normalizeNearAddressConst converts a nonzero constant that resolves inside
// DGROUP into an explicit DS-relative address while preserving its origin.
func (p *normalizeAssignmentAddressesProcessor) normalizeNearAddressConst(
	value machine.Value,
) (machine.Value, bool) {
	c, ok := value.(*machine.Const)
	if !ok || c.Val == 0 {
		return value, false
	}

	// Do not reinterpret arbitrary integers as addresses. The constant must
	// actually point into a known DGROUP global.
	if _, ok := p.ctx.res.ResolveGlobal(
		uint16(p.ctx.sdb.DGroupFrame),
		uint32(c.Val),
		0,
	); !ok {
		return value, false
	}

	origin := machine.Origin{}
	if c.Origin != nil {
		origin = *c.Origin
	}

	return machine.AddressVal(machine.MemoryAccess{
		Seg:    p.ctx.dsReg,
		Disp:   int(c.Val),
		Origin: origin,
	}), true
}

// normalizeSemanticNearAddressConst converts a nonzero constant that resolves
// inside DGROUP into a source-level near-pointer address expression.
func (p *normalizeAssignmentAddressesProcessor) normalizeSemanticNearAddressConst(
	value Expr,
	expected typeinfo.Type,
) (Expr, bool) {
	c, ok := value.(*Const)
	if !ok || c.U64 == 0 || c.U64 > 0xffff {
		return value, false
	}

	global, ok := p.ctx.res.ResolveGlobal(
		uint16(p.ctx.sdb.DGroupFrame),
		uint32(c.U64),
		0,
	)
	if !ok {
		return value, false
	}

	base := &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: global.Global}}
	target, ok := (&machineConverter{ctx: p.ctx}).consumeAddressExpr(AddressExpr{
		Base:   base,
		Offset: global.FieldOff,
	}, 0)
	if !ok {
		return value, false
	}

	lvalue, ok := target.(LValue)
	if !ok {
		return value, false
	}
	if decayed, ok := decayArrayLValue(lvalue, expected); ok {
		return decayed, true
	}
	return &AddressOf{Target: lvalue, TypeInfo: expected}, true
}

// isNearAddressStorage reports whether storage is represented by a near
// pointer, including pointer-only wrapper aggregates such as OBJ.
func isNearAddressStorage(typ typeinfo.Type, width int) bool {
	if ptr, ok := typ.(*typeinfo.Pointer); ok {
		return ptr.Class == typeinfo.PtrNear && ptr.Bytes() == width
	}

	strct, ok := typ.(*typeinfo.Struct)
	if !ok || strct.Bytes() != width {
		return false
	}

	fields := strct.FieldsAtOffset(0)
	if len(fields) == 0 {
		return false
	}

	found := false
	for _, field := range fields {
		if field.Bitfield != nil || field.Type.Bytes() != width {
			continue
		}

		ptr, ok := field.Type.(*typeinfo.Pointer)
		if !ok || ptr.Class != typeinfo.PtrNear {
			return false
		}

		found = true
	}

	return found
}
