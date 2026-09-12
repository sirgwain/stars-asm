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
	// addressValue distinguishes a complete address from its low word.
	addressValue bool
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
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			assign, ok := effect.(*Assign)
			if !ok {
				return effect, false, false
			}
			next := *assign
			next.Src = recoverExpectedValue(assign.Src, assign.Dst.ExprType())
			rewritten, changed := w.rewriteEffectChildren(&next)
			return rewritten, changed || next.Src != assign.Src, true
		},
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			if call.Function == nil {
				return call, false, false
			}
			next := *call
			next.Args = append([]Expr(nil), call.Args...)
			changed := false
			for i, arg := range next.Args {
				if i < len(call.Params) {
					next.Args[i] = recoverExpectedValue(arg, call.Params[i].Type)
					changed = changed || next.Args[i] != arg
				}
			}
			rewritten, childChanged := w.rewriteCallChildren(&next)
			return rewritten, changed || childChanged, true
		},
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			switch value := expr.(type) {
			case *PointerOffset:
				if resolved, ok := p.resolvePointerArithmetic(value); ok {
					next, _ := w.rewriteExprChildren(resolved)
					return next, true, true
				}
			case *Binary:
				next := value
				changed := false

				// In pointer difference expressions, a machine address of an array
				// normally represents the array's ordinary C pointer decay.
				if value.Op == OpSub {
					if typeinfo.IsPointer(value.LHS.ExprType()) {
						if rhs, ok := decayAddressOfArray(value.RHS, value.LHS.ExprType()); ok {
							copy := *value
							copy.RHS = rhs
							next = &copy
							changed = true
						}
					}

					if typeinfo.IsPointer(next.RHS.ExprType()) {
						if lhs, ok := decayAddressOfArray(next.LHS, next.RHS.ExprType()); ok {
							if next == value {
								copy := *value
								next = &copy
							}
							next.LHS = lhs
							changed = true
						}
					}
				}

				// Pointer-typed binaries already use element steps. Machine
				// arithmetic exposed by scratch substitution still uses bytes.
				if !typeinfo.IsPointer(next.ExprType()) &&
					(next.Op == OpAdd || next.Op == OpSub) {

					if resolved, ok := p.resolvePointerArithmetic(next); ok {
						child, _ := w.rewriteExprChildren(resolved)
						return child, true, true
					}
				}

				if changed {
					child, childChanged := w.rewriteExprChildren(next)
					return child, changed || childChanged, true
				}
				// Pointer-typed binaries already use element steps. Machine
				// arithmetic exposed by scratch substitution still uses bytes.
				if !typeinfo.IsPointer(value.ExprType()) && (value.Op == OpAdd || value.Op == OpSub) {
					if resolved, ok := p.resolvePointerArithmetic(value); ok {
						next, _ := w.rewriteExprChildren(resolved)
						return next, true, true
					}
				}
			}
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
			// Pair the segment and offset before rewriting either lane's
			// parent into an array address with a different semantic type.
			if memory, ok := value.(*Memory); ok {
				if resolved, ok := p.resolveMemory(memory); ok {
					next, _ := w.rewriteLValueChildren(resolved)
					return next, true, true
				}
			}
			value, childChanged := w.rewriteLValueChildren(value)
			if part, ok := value.(*Part); ok && part.Width > 0 && part.TypeInfo != nil {
				converter := machineConverter{ctx: p.ctx}
				expected := &typeinfo.Pointer{Elem: part.TypeInfo, Class: typeinfo.PtrNear}
				if target, ok := converter.typedAddressTarget(part.Base, part.ByteOff, expected); ok && target.ExprType().Bytes() == part.Width {
					return target, true, true
				}
			}
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

// resolvePointerArithmetic projects a byte address into a typed subobject
// address without loading the subobject or following a pointer stored in it.
func (p *resolveLateAddressesProcessor) resolvePointerArithmetic(expr Expr) (Expr, bool) {
	if binary, ok := expr.(*Binary); ok && binary.Op == OpSub {
		if semanticAddressValued(binary.LHS) && semanticAddressValued(binary.RHS) {
			return nil, false
		}
	}

	parts := flattenSemanticAddress(expr, 1)
	if parts.invalid || !parts.addressValue || parts.base == nil || parts.offset == 0 && len(parts.terms) == 0 {
		return nil, false
	}
	converter := machineConverter{ctx: p.ctx}
	projected, ok := converter.consumeAddressProjection(AddressExpr{Base: parts.base, Offset: parts.offset, Terms: parts.terms, Deref: parts.deref}, 0)
	if !ok {
		return nil, false
	}
	target, ok := projected.(LValue)
	if !ok {
		return nil, false
	}
	switch target.(type) {
	case *Part, *Deref:
		return nil, false
	}
	if index, ok := target.(*ArrayIndex); ok && typeinfo.IsPointer(index.Base.ExprType()) {
		return objectAddress(index, 0, index.Base.ExprType()), true
	}
	class := typeinfo.PtrNear
	if ptr, ok := parts.base.ExprType().(*typeinfo.Pointer); ok {
		class = ptr.Class
	}
	if typeinfo.IsArray(target.ExprType()) {
		return target, true
	}
	return &AddressOf{Target: target, TypeInfo: &typeinfo.Pointer{Elem: target.ExprType(), Class: class}}, true
}

// resolveAddressOfPart distinguishes pointer byte arithmetic from an address
// of a typed subobject after wide pointer reconstruction.
func (p *resolveLateAddressesProcessor) resolveAddressOfPart(address *AddressOf) (Expr, bool) {
	part, ok := address.Target.(*Part)
	if !ok {
		return nil, false
	}
	converter := machineConverter{ctx: p.ctx}
	if part.Width == 0 && typeinfo.IsPointer(part.Base.ExprType()) {
		ptr := part.Base.ExprType().(*typeinfo.Pointer)
		base := &Deref{Pointer: part.Base, Width: ptr.Elem.Bytes(), TypeInfo: ptr.Elem}
		if target, ok := converter.typedAddressTarget(base, part.ByteOff, address.TypeInfo); ok {
			return objectAddress(target, 0, address.TypeInfo), true
		}
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
	if target, ok := converter.typedAddressTarget(part.Base, part.ByteOff, address.TypeInfo); ok {
		return objectAddress(target, 0, address.TypeInfo), true
	}
	if part.Width != 0 {
		return nil, false
	}
	return objectAddress(part.Base, part.ByteOff, address.TypeInfo), true
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
	case *PointerOffset:
		return mergeSemanticAddressParts(flattenSemanticAddress(value.Pointer, sign), flattenSemanticAddress(value.Offset, sign))
	case *Unary:
		if value.Op == OpNeg {
			return flattenSemanticAddress(value.X, -sign)
		}
	case *Cast:
		return flattenSemanticAddress(value.Value, sign)
	case *Word:
		if value.Part == machine.WordLow {
			parts := flattenSemanticAddress(value.Parent, sign)
			parts.addressValue = false
			return parts
		}
	case *Binary:
		switch value.Op {
		case OpAdd, OpSub:
			lhs := flattenSemanticAddress(value.LHS, sign)
			rhsSign := sign
			if value.Op == OpSub {
				rhsSign = -rhsSign
			}
			rhs := flattenSemanticAddress(value.RHS, rhsSign)
			if ptr, ok := value.ExprType().(*typeinfo.Pointer); ok {
				// Source pointer arithmetic counts elements; only machine
				// integer arithmetic and PointerOffset count bytes.
				delta := &rhs
				if rhs.base != nil {
					delta = &lhs
				}
				delta.offset *= ptr.Elem.Bytes()
				for i := range delta.terms {
					delta.terms[i].Scale *= ptr.Elem.Bytes()
				}
			}
			return mergeSemanticAddressParts(lhs, rhs)
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
			return semanticAddressParts{base: value.Target, addressValue: true}
		}
	default:
		if expr != nil && sign == 1 && typeinfo.IsArray(expr.ExprType()) {
			return semanticAddressParts{base: expr, addressValue: true}
		}
		if expr != nil && sign == 1 && typeinfo.IsPointer(expr.ExprType()) {
			return semanticAddressParts{base: expr, deref: true, addressValue: true}
		}
	}
	if expr == nil {
		return semanticAddressParts{}
	}
	return semanticAddressParts{terms: []ScaledTerm{{Expr: unwrapSemanticAddressWord(expr), Scale: sign}}}
}

func decayAddressOfArray(expr Expr, expected typeinfo.Type) (Expr, bool) {
	address, ok := expr.(*AddressOf)
	if !ok {
		return expr, false
	}

	decayed, ok := decayArrayLValue(address.Target, expected)
	if !ok {
		return expr, false
	}
	return decayed, true
}

// mergeSemanticAddressParts combines normalized semantic address fragments
// while rejecting a second base by retaining it as an ordinary term.
func mergeSemanticAddressParts(a, b semanticAddressParts) semanticAddressParts {
	out := semanticAddressParts{base: a.base, offset: a.offset + b.offset, deref: a.deref, invalid: a.invalid || b.invalid, addressValue: a.addressValue}
	out.terms = append(out.terms, a.terms...)
	if out.base == nil {
		out.base = b.base
		out.deref = b.deref
		out.addressValue = b.addressValue
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

func semanticAddressValued(expr Expr) bool {
	if expr == nil {
		return false
	}
	if typeinfo.IsPointer(expr.ExprType()) || typeinfo.IsArray(expr.ExprType()) {
		return true
	}
	_, ok := expr.(*AddressOf)
	return ok
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
