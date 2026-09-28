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
			next.Src = (&machineConverter{ctx: p.ctx}).recoverExpectedValue(assign.Src, assign.Dst.ExprType())
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
					next.Args[i] = (&machineConverter{ctx: p.ctx}).recoverExpectedValue(arg, call.Params[i].Type)
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
			case *Words:
				if resolved, ok := p.resolveSegmentWords(value); ok {
					next, _ := w.rewriteExprChildren(resolved)
					return next, true, true
				}
			case *Binary:
				next, changed := p.resolveSegmentWordOperands(value)

				// In pointer difference expressions, a machine address of an array
				// normally represents the array's ordinary C pointer decay.
				if next.Op == OpSub {
					if typeinfo.IsPointer(next.LHS.ExprType()) {
						if rhs, ok := decayAddressOfArray(next.RHS, next.LHS.ExprType()); ok {
							copy := *next
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

				// Pointer arithmetic exposed by scratch substitution uses bytes.
				if next.Op == OpAdd || next.Op == OpSub {
					if resolved, ok := p.resolvePointerArithmetic(next); ok {
						child, _ := w.rewriteExprChildren(resolved)
						return child, true, true
					}
				}

				if changed {
					child, childChanged := w.rewriteExprChildren(next)
					return child, changed || childChanged, true
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
	class := typeinfo.PtrNear
	if ptr, ok := parts.base.ExprType().(*typeinfo.Pointer); ok {
		class = ptr.Class
	}
	return p.projectAddress(AddressExpr{Base: parts.base, Offset: parts.offset, Terms: parts.terms, Deref: parts.deref}, class)
}

// projectAddress converts a byte address into the address of the typed
// subobject it names, decaying arrays and rejecting untyped byte views.
func (p *resolveLateAddressesProcessor) projectAddress(addr AddressExpr, class typeinfo.PtrClass) (Expr, bool) {
	converter := machineConverter{ctx: p.ctx}
	projected, ok := converter.consumeAddressProjection(addr, 0)
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
	if typeinfo.IsArray(target.ExprType()) {
		return target, true
	}
	return &AddressOf{Target: target, TypeInfo: &typeinfo.Pointer{Elem: target.ExprType(), Class: class}}, true
}

// resolveSegmentWords resolves a far address built from a segment register
// and an offset, such as words(cs, 0xa370 + 768*i) for a table in a code
// segment, to the address of the global subobject it names.
func (p *resolveLateAddressesProcessor) resolveSegmentWords(expr Expr) (Expr, bool) {
	words, ok := expr.(*Words)
	if !ok || len(words.Words) != 2 {
		return nil, false
	}
	seg, ok := words.Words[0].(*Register)
	if !ok || seg.Val != asm.RegDS && seg.Val != asm.RegCS {
		return nil, false
	}
	addr, ok := p.addressFromMemory(&Memory{Seg: seg, Base: words.Words[1]})
	if !ok {
		return nil, false
	}
	return p.projectAddress(addr, typeinfo.PtrFar)
}

// resolveSegmentWordOperands replaces segment-register far addresses used
// directly as operands of pointer arithmetic with their typed addresses.
func (p *resolveLateAddressesProcessor) resolveSegmentWordOperands(binary *Binary) (*Binary, bool) {
	lhs, lhsOK := p.resolveSegmentWords(binary.LHS)
	rhs, rhsOK := p.resolveSegmentWords(binary.RHS)
	if !lhsOK && !rhsOK {
		return binary, false
	}
	next := *binary
	if lhsOK {
		next.LHS = lhs
	}
	if rhsOK {
		next.RHS = rhs
	}
	return &next, true
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
		if typed, ok := p.constPointerAddress(part.Base, part.ByteOff, address.TypeInfo); ok {
			return typed, true
		}
		return &PointerOffset{Pointer: part.Base, Offset: signedIndexConst(part.ByteOff), TypeInfo: address.TypeInfo}, true
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
			// Semantic pointer arithmetic always counts bytes; element steps
			// are represented as &pointer[index].
			rhs := flattenSemanticAddress(value.RHS, rhsSign)
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
		// A subtracted address makes this a pointer difference, such as
		// (pch - szFile) + 1, not an address with a negative index.
		return semanticAddressParts{invalid: true}
	default:
		if expr != nil && (typeinfo.IsArray(expr.ExprType()) || typeinfo.IsPointer(expr.ExprType())) && sign != 1 {
			return semanticAddressParts{invalid: true}
		}
		if expr != nil && typeinfo.IsArray(expr.ExprType()) {
			return semanticAddressParts{base: expr, addressValue: true}
		}
		if expr != nil && typeinfo.IsPointer(expr.ExprType()) {
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

// ProcessFunc projects addresses, types the remaining byte displacements,
// propagates proven array element types through single-definition pointer
// temporaries, then projects their uses again.
func (p *resolveLateAddressesProcessor) ProcessFunc(result *Result, f *Func) bool {
	changed := false
	for i, b := range f.Blocks {
		p.ctx.SetCurrentBlock(b.ID)
		next, didChange := p.ProcessBlock(result, *f, b)
		if didChange {
			f.Blocks[i] = next
			changed = true
		}
	}
	for i, b := range f.Blocks {
		if effects, didChange := p.byteAddressRewriter().rewriteEffects(b.Effects); didChange {
			f.Blocks[i].Effects = effects
			changed = true
		}
	}
	types := make(map[string]*typeinfo.Pointer)
	definitions := make(map[string]int)
	for _, b := range f.Blocks {
		for _, effect := range b.Effects {
			if a, ok := effect.(*Assign); ok {
				if temp, ok := a.Dst.(*Temp); ok {
					definitions[temp.Name]++
					ptr, pok := temp.ExprType().(*typeinfo.Pointer)
					array, aok := a.Src.ExprType().(*typeinfo.Array)
					if pok && aok && ptr.Elem.Bytes() == array.Elem.Bytes() && !typeinfo.Equals(ptr.Elem, array.Elem) {
						types[temp.Name] = &typeinfo.Pointer{Elem: array.Elem, Class: ptr.Class}
					}
				}
			}
			if c, ok := effect.(*CallEffect); ok {
				if temp, ok := c.Result.(*Temp); ok {
					definitions[temp.Name]++
				}
			}
		}
	}
	for name := range types {
		if definitions[name] != 1 {
			delete(types, name)
		}
	}
	if len(types) > 0 {
		w := semRewriter{lvalue: func(w *semRewriter, value LValue) (LValue, bool, bool) {
			if temp, ok := value.(*Temp); ok {
				if typ, ok := types[temp.Name]; ok {
					next := *temp
					next.TypeInfo = typ
					return &next, true, true
				}
			}
			value, childChanged := w.rewriteLValueChildren(value)
			if index, ok := value.(*ArrayIndex); ok && childChanged {
				next := *index
				next.TypeInfo = indexElementType(index.Base.ExprType())
				return &next, true, true
			}
			return value, childChanged, true
		}}
		for i, b := range f.Blocks {
			effects, didChange := w.rewriteEffects(b.Effects)
			if didChange {
				b.Effects = effects
				changed = true
			}
			p.ctx.SetCurrentBlock(b.ID)
			next, _ := p.ProcessBlock(result, *f, b)
			f.Blocks[i] = next
		}
	}
	p.ctx.ClearCurrentBlock()
	return changed
}

// byteAddressRewriter types byte-displaced pointer arithmetic that address
// projection left unresolved. An assignment, parameter or return type selects
// the addressed subobject; anything else stays an explicit byte offset, so C
// pointer arithmetic never scales an original Win16 byte displacement.
func (p *resolveLateAddressesProcessor) byteAddressRewriter() *semRewriter {
	var w *semRewriter
	// typed rewrites nested displacements without context, then the top
	// expression with the declared pointer type trusted for projection and
	// the (possibly derived) cast type used for an explicit byte offset.
	typed := func(expr Expr, trusted, cast typeinfo.Type) (Expr, bool) {
		next, changed := w.rewriteExprChildren(expr)
		if address, ok := p.typeByteAddress(next, trusted, cast); ok {
			return address, true
		}
		return next, changed
	}
	w = &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			switch e := effect.(type) {
			case *Assign:
				trusted := e.Dst.ExprType()
				if _, temp := e.Dst.(*Temp); temp {
					trusted = nil
				}
				dst, dstChanged := w.rewriteLValue(e.Dst)
				src, srcChanged := typed(e.Src, trusted, e.Dst.ExprType())
				if !dstChanged && !srcChanged {
					return effect, false, true
				}
				next := *e
				next.Dst, next.Src = dst, src
				return &next, true, true
			case *Return:
				if e.Value == nil {
					return effect, false, true
				}
				value, changed := typed(e.Value, p.ctx.fs.Ret, p.ctx.fs.Ret)
				if !changed {
					return effect, false, true
				}
				next := *e
				next.Value = value
				return &next, true, true
			}
			return effect, false, false
		},
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			if call.Function == nil {
				return call, false, false
			}
			next := *call
			next.Args = append([]Expr(nil), call.Args...)
			changed := false
			for i, arg := range call.Args {
				var param typeinfo.Type
				if i < len(call.Params) {
					param = call.Params[i].Type
				}
				var argChanged bool
				next.Args[i], argChanged = typed(arg, param, param)
				changed = changed || argChanged
			}
			return &next, changed, true
		},
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, changed := w.rewriteExprChildren(expr)
			if address, ok := p.typeByteAddress(next, nil, nil); ok {
				return address, true, true
			}
			return next, changed, true
		},
	}
	return w
}

// typeByteAddress rewrites one byte displacement from a typed pointer. A
// constant offset inside the pointee selects the unique subobject compatible
// with the trusted type, and an element-aligned offset becomes pointer steps.
// Any other displacement becomes a byte PointerOffset typed as cast, or as a
// byte pointer without context.
func (p *resolveLateAddressesProcessor) typeByteAddress(expr Expr, trusted, cast typeinfo.Type) (Expr, bool) {
	pointer, offset, ok := byteDisplacement(expr)
	if !ok {
		return nil, false
	}
	ptr := pointer.ExprType().(*typeinfo.Pointer)
	var expected typeinfo.Type
	if want, ok := trusted.(*typeinfo.Pointer); ok {
		expected = want
	}
	if off, ok := byteOffsetConst(offset); ok {
		if typed, ok := p.constPointerAddress(pointer, off, expected); ok {
			return typed, true
		}
	}
	if ptr.Elem.Bytes() == 1 && (expected == nil || typeinfo.Equals(expected, ptr)) {
		if _, raw := expr.(*Binary); raw {
			// Byte-sized pointees already step by bytes in C.
			return nil, false
		}
	}
	typ, ok := cast.(*typeinfo.Pointer)
	if !ok {
		typ = &typeinfo.Pointer{Elem: typeinfo.U8, Class: ptr.Class}
	}
	if current, ok := expr.(*PointerOffset); ok && typeinfo.Equals(current.TypeInfo, typ) {
		return nil, false
	}
	return &PointerOffset{Pointer: pointer, Offset: offset, TypeInfo: typ}, true
}

// constPointerAddress addresses a constant byte offset from a typed pointer
// without byte arithmetic: a trailing flexible array, the unique subobject
// compatible with the expected type, the pointer itself, or whole element
// steps. Without an expected type only aggregate subobjects are selected. It
// fails when only an explicit byte offset can express the address.
func (p *resolveLateAddressesProcessor) constPointerAddress(pointer Expr, off int, expected typeinfo.Type) (Expr, bool) {
	ptr := pointer.ExprType().(*typeinfo.Pointer)
	if !addressLayoutKnown(pointer, ptr.Elem) {
		return nil, false
	}
	want, _ := expected.(*typeinfo.Pointer)
	size := ptr.Elem.Bytes()
	base := pointeeLValue(pointer, ptr.Elem)
	if strct, ok := ptr.Elem.(*typeinfo.Struct); ok && off >= size {
		if field, fieldOff, ok := strct.FlexibleArrayFieldAt(off); ok && fieldOff == 0 {
			return objectAddress(bitfieldFieldAccess(base, field), 0, expected), true
		}
	}
	if size > 0 && off >= 0 && off < size {
		converter := machineConverter{ctx: p.ctx}
		if target, ok := converter.typedAddressTarget(base, off, expected); ok && (expected != nil || isAggregateType(target.ExprType())) {
			return objectAddress(target, 0, expected), true
		}
	}
	if off == 0 {
		return castAddress(pointer, want, ptr.Class), true
	}
	// Whole steps past the address of one object leave that object.
	if _, single := pointer.(*AddressOf); !single && size > 1 && off%size == 0 {
		if stepped, ok := projectPointerAddress(pointer, off, nil); ok {
			return castAddress(stepped, want, ptr.Class), true
		}
	}
	return nil, false
}

// addressLayoutKnown reports whether a pointer's element type describes the
// storage it addresses. An address of a target typed differently, or of an
// array element typed differently from its array, does not describe the
// layout, so only explicit byte offsets are safe from it.
func addressLayoutKnown(pointer Expr, elem typeinfo.Type) bool {
	address, ok := pointer.(*AddressOf)
	if !ok {
		return true
	}
	if !typeinfo.Equals(elem, address.Target.ExprType()) {
		return false
	}
	index, ok := address.Target.(*ArrayIndex)
	return !ok || typeinfo.Equals(index.TypeInfo, indexElementType(index.Base.ExprType()))
}

// byteDisplacement splits byte pointer arithmetic into its pointer and signed
// byte offset.
func byteDisplacement(expr Expr) (Expr, Expr, bool) {
	switch e := expr.(type) {
	case *PointerOffset:
		if typeinfo.IsPointer(e.Pointer.ExprType()) {
			return e.Pointer, e.Offset, true
		}
	case *Binary:
		if e.Op != OpAdd && e.Op != OpSub {
			return nil, nil, false
		}
		lhsPointer, rhsPointer := typeinfo.IsPointer(e.LHS.ExprType()), typeinfo.IsPointer(e.RHS.ExprType())
		switch {
		case lhsPointer && e.RHS.ExprType().Kind() == typeinfo.KInt:
			if e.Op == OpSub {
				return e.LHS, &Unary{TypeInfo: e.RHS.ExprType(), Op: OpNeg, X: e.RHS}, true
			}
			return e.LHS, e.RHS, true
		case rhsPointer && e.Op == OpAdd && e.LHS.ExprType().Kind() == typeinfo.KInt:
			return e.RHS, e.LHS, true
		}
	}
	return nil, nil, false
}

// pointeeLValue names the object a pointer addresses.
func pointeeLValue(pointer Expr, elem typeinfo.Type) LValue {
	if address, ok := pointer.(*AddressOf); ok {
		return address.Target
	}
	return &Deref{Pointer: pointer, Width: elem.Bytes(), TypeInfo: elem}
}

// castAddress converts an address to a required non-void pointer type. An
// array address decays to a pointer of the given class before comparison.
func castAddress(address Expr, want *typeinfo.Pointer, class typeinfo.PtrClass) Expr {
	if want == nil || want.Elem.Kind() == typeinfo.KVoid {
		return address
	}
	actual := address.ExprType()
	if array, ok := actual.(*typeinfo.Array); ok {
		actual = &typeinfo.Pointer{Elem: array.Elem, Class: class}
	}
	if typeinfo.Equals(want, actual) {
		return address
	}
	return &Cast{To: want.String(), TypeInfo: want, Value: address}
}
