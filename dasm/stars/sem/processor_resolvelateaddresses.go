package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type resolveLateAddressesProcessor struct {
	ctx *FuncContext
}

type semanticAddressParts struct {
	base    Expr
	offset  int
	terms   []ScaledTerm
	deref   bool
	invalid bool
}

// ProcessBlock projects raw semantic memory after scratch substitution has
// exposed the effective address expression.
func (p *resolveLateAddressesProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	effects, changed := p.rewriter().rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// rewriter returns the lvalue rewrite used for late typed address recovery.
func (p *resolveLateAddressesProcessor) rewriter() *semRewriter {
	return &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			address, ok := expr.(*AddressOf)
			if !ok {
				return expr, false, false
			}
			next, childChanged := w.rewriteExprChildren(address)
			address = next.(*AddressOf)
			resolved, ok := p.resolveAddressOfPart(address)
			if !ok {
				return address, childChanged, true
			}
			return resolved, true, true
		},
		lvalue: func(w *semRewriter, value LValue) (LValue, bool, bool) {
			value, childChanged := w.rewriteLValueChildren(value)
			memory, ok := value.(*Memory)
			if !ok {
				return value, childChanged, true
			}
			resolved, ok := p.resolveMemory(memory)
			if !ok {
				return value, childChanged, true
			}
			return resolved, true, true
		},
	}
}

// resolveAddressOfPart distinguishes pointer byte arithmetic from an address
// of a typed subobject after wide pointer reconstruction.
func (p *resolveLateAddressesProcessor) resolveAddressOfPart(address *AddressOf) (Expr, bool) {
	part, ok := address.Target.(*Part)
	if !ok || part.Width != 0 {
		return nil, false
	}
	if typeinfo.IsPointer(part.Base.ExprType()) {
		if projected, ok := projectPointerAddress(part.Base, part.ByteOff, nil); ok {
			return projected, true
		}
		magnitude := part.ByteOff
		negative := magnitude < 0
		if negative {
			magnitude = -magnitude
		}
		var offset Expr = &Const{TypeInfo: typeinfo.I16, U64: uint64(magnitude)}
		if negative {
			offset = &Unary{TypeInfo: typeinfo.I16, Op: OpNeg, X: offset}
		}
		return &PointerOffset{Pointer: part.Base, Offset: offset, TypeInfo: address.TypeInfo}, true
	}
	converter := machineConverter{ctx: p.ctx}
	projected, ok := converter.consumeAddressProjection(AddressExpr{Base: part.Base, Offset: part.ByteOff}, 0)
	if !ok {
		return nil, false
	}
	target, ok := projected.(LValue)
	if !ok {
		return nil, false
	}
	return &AddressOf{Target: target, TypeInfo: address.TypeInfo}, true
}

// resolveMemory normalizes one semantic effective address and projects it
// through the declared local, global, pointer, array, and struct types.
func (p *resolveLateAddressesProcessor) resolveMemory(memory *Memory) (LValue, bool) {
	addr, ok := p.addressFromMemory(memory)
	if !ok {
		return nil, false
	}
	converter := machineConverter{ctx: p.ctx}
	return converter.consumeAddress(addr, memory.Width)
}

// addressFromMemory recovers the source base represented by a lowered
// segmented memory expression.
func (p *resolveLateAddressesProcessor) addressFromMemory(memory *Memory) (AddressExpr, bool) {
	if parent, ok := semanticFarSegmentParent(memory.Seg); ok && typeinfo.IsPointer(parent.ExprType()) {
		offset, terms, found := splitSemanticFarOffset(parent, memory.Base)
		if !found {
			return AddressExpr{}, false
		}
		addr := AddressExpr{Base: parent, Offset: offset + memory.Disp, Terms: terms, Deref: true}
		return addSemanticMemoryIndex(addr, memory), true
	}

	seg, segmented := memory.Seg.(*Register)
	if memory.Seg != nil && !segmented {
		return AddressExpr{}, false
	}
	if segmented && seg.Val != asm.RegSS && seg.Val != asm.RegDS && seg.Val != asm.RegCS {
		return AddressExpr{}, false
	}

	parts := flattenSemanticAddress(memory.Base, 1)
	if parts.invalid {
		return AddressExpr{}, false
	}
	parts.offset += memory.Disp
	if memory.Index != nil {
		scale := memory.Scale
		if scale == 0 {
			scale = 1
		}
		parts.terms = append(parts.terms, ScaledTerm{Expr: memory.Index, Scale: scale})
	}
	if parts.base != nil {
		return AddressExpr{Base: parts.base, Offset: parts.offset, Terms: parts.terms, Deref: parts.deref}, true
	}
	if !segmented || (seg.Val != asm.RegDS && seg.Val != asm.RegCS) {
		return AddressExpr{}, false
	}

	segNum := p.ctx.segFromRegister(seg.Val)
	path, fieldOff, ok := p.ctx.symbols.globalAddressBase(segNum, uint32(uint16(parts.offset)))
	if !ok && len(parts.terms) > 0 {
		path, fieldOff, ok = p.ctx.symbols.flexibleGlobalAddressBase(segNum, uint32(uint16(parts.offset)))
	}
	if !ok {
		return AddressExpr{}, false
	}
	base, ok := (&machineConverter{ctx: p.ctx}).convertSymbolPath(path, path.Type())
	if !ok {
		return AddressExpr{}, false
	}
	return AddressExpr{Base: base, Offset: fieldOff, Terms: parts.terms}, true
}

// flattenSemanticAddress separates a semantic additive expression into one
// typed base, a fixed byte displacement, and dynamic scaled byte terms.
func flattenSemanticAddress(expr Expr, sign int) semanticAddressParts {
	switch value := expr.(type) {
	case *Cast:
		return flattenSemanticAddress(value.Value, sign)
	case *Word:
		if value.Part == machine.WordLow {
			return flattenSemanticAddress(value.Parent, sign)
		}
	case *Binary:
		switch value.Op {
		case OpAdd, OpSub:
			lhs := flattenSemanticAddress(value.LHS, sign)
			rhsSign := sign
			if value.Op == OpSub {
				rhsSign = -rhsSign
			}
			return mergeSemanticAddressParts(lhs, flattenSemanticAddress(value.RHS, rhsSign))
		case OpMul:
			if constant, other, ok := semanticConstOperand(value.LHS, value.RHS); ok {
				return semanticAddressParts{terms: []ScaledTerm{{Expr: unwrapSemanticAddressWord(other), Scale: sign * signedWordOffset(uint(constant.U64))}}}
			}
		case OpShl:
			if shift, ok := value.RHS.(*Const); ok && shift.U64 < 16 {
				return semanticAddressParts{terms: []ScaledTerm{{Expr: unwrapSemanticAddressWord(value.LHS), Scale: sign * (1 << shift.U64)}}}
			}
		}
	case *Const:
		return semanticAddressParts{offset: sign * signedWordOffset(uint(value.U64))}
	case *AddressOf:
		if sign == 1 {
			return semanticAddressParts{base: value.Target}
		}
	default:
		if expr != nil && sign == 1 && typeinfo.IsPointer(expr.ExprType()) {
			return semanticAddressParts{base: expr, deref: true}
		}
	}
	if expr == nil {
		return semanticAddressParts{}
	}
	return semanticAddressParts{terms: []ScaledTerm{{Expr: unwrapSemanticAddressWord(expr), Scale: sign}}}
}

// mergeSemanticAddressParts combines normalized semantic address fragments
// while rejecting a second base by retaining it as an ordinary term.
func mergeSemanticAddressParts(a, b semanticAddressParts) semanticAddressParts {
	out := semanticAddressParts{base: a.base, offset: a.offset + b.offset, deref: a.deref, invalid: a.invalid || b.invalid}
	out.terms = append(out.terms, a.terms...)
	if out.base == nil {
		out.base = b.base
		out.deref = b.deref
	} else if b.base != nil {
		out.invalid = true
	}
	out.terms = append(out.terms, b.terms...)
	return out
}

// unwrapSemanticAddressWord removes representation-only low-word wrappers
// from dynamic address terms.
func unwrapSemanticAddressWord(expr Expr) Expr {
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

// semanticFarSegmentParent returns the pointer represented by the high word
// used as a segmented memory selector.
func semanticFarSegmentParent(expr Expr) (Expr, bool) {
	switch value := expr.(type) {
	case *Part:
		if value.ByteOff == 2 && value.Width == 2 {
			return value.Base, true
		}
	case *Word:
		if value.Part == machine.WordHigh {
			return value.Parent, true
		}
	case *FarPointer:
		if value.Part == machine.FarPointerSegment {
			return value.Parent, true
		}
	}
	return nil, false
}

// splitSemanticFarOffset removes the matching low pointer word and returns
// all fixed and dynamic byte offsets applied to it.
func splitSemanticFarOffset(parent Expr, expr Expr) (int, []ScaledTerm, bool) {
	if semanticFarOffsetMatches(parent, expr) {
		return 0, nil, true
	}
	switch value := expr.(type) {
	case *Cast:
		return splitSemanticFarOffset(parent, value.Value)
	case *Binary:
		if value.Op == OpAdd || value.Op == OpSub {
			lo, lt, lok := splitSemanticFarOffset(parent, value.LHS)
			ro, rt, rok := splitSemanticFarOffset(parent, value.RHS)
			if lok && rok {
				return 0, nil, false
			}
			if value.Op == OpSub {
				ro = -ro
				for i := range rt {
					rt[i].Scale = -rt[i].Scale
				}
			}
			return lo + ro, append(lt, rt...), lok || rok
		}
	}
	parts := flattenSemanticAddress(expr, 1)
	if parts.invalid || parts.base != nil {
		return 0, nil, false
	}
	return parts.offset, parts.terms, false
}

// semanticFarOffsetMatches reports whether expr is the low word paired with
// the supplied far-pointer parent.
func semanticFarOffsetMatches(parent Expr, expr Expr) bool {
	switch value := expr.(type) {
	case *Part:
		return value.ByteOff == 0 && value.Width == 2 && sameExpr(parent, value.Base)
	case *Word:
		return value.Part == machine.WordLow && sameExpr(parent, value.Parent)
	case *FarPointer:
		return value.Part == machine.FarPointerOffset && sameExpr(parent, value.Parent)
	default:
		return false
	}
}

// addSemanticMemoryIndex appends an explicit indexed addressing component.
func addSemanticMemoryIndex(addr AddressExpr, memory *Memory) AddressExpr {
	if memory.Index == nil {
		return addr
	}
	scale := memory.Scale
	if scale == 0 {
		scale = 1
	}
	addr.Terms = append(addr.Terms, ScaledTerm{Expr: memory.Index, Scale: scale})
	return addr
}
