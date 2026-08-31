package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type collapseWideStoresProcessor struct {
	ctx *FuncContext
}

type pendingWideTempAlias struct {
	assign *Assign
	value  Expr
}

// ProcessBlock collapses adjacent word-part assignments into wide assignments.
func (p *collapseWideStoresProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	changed := false
	effects := make([]Effect, 0, len(b.Effects))
	rewriter := collapseWideRewriter(p.ctx)
	pendingTemps := make(map[string]pendingWideTempAlias)
	pendingTempOrder := make([]string, 0)
	for i := 0; i < len(b.Effects); i++ {
		effect, effectChanged := rewriter.rewriteEffect(b.Effects[i])
		changed = changed || effectChanged

		if assign, ok := effect.(*Assign); ok {
			if temp, ok := assign.Dst.(*Temp); ok {
				if _, exists := pendingTemps[temp.Name]; !exists {
					pendingTempOrder = append(pendingTempOrder, temp.Name)
				}
				pendingTemps[temp.Name] = pendingWideTempAlias{assign: assign, value: assign.Src}
				continue
			}
		}

		lo, ok := effect.(*Assign)
		if !ok {
			effects = appendPendingWideTempAssigns(effects, pendingTemps, pendingTempOrder, nil)
			pendingTemps = make(map[string]pendingWideTempAlias)
			pendingTempOrder = pendingTempOrder[:0]
			effects = append(effects, effect)
			continue
		}
		if i+1 >= len(b.Effects) {
			effects = appendPendingWideTempAssigns(effects, pendingTemps, pendingTempOrder, nil)
			pendingTemps = make(map[string]pendingWideTempAlias)
			pendingTempOrder = pendingTempOrder[:0]
			if dst, src, ok := collapseFarPointerOffsetAssign(lo); ok {
				effects = append(effects, &Assign{MetaInfo: lo.MetaInfo, Dst: dst, Src: src})
				changed = true
				continue
			}
			effects = append(effects, effect)
			continue
		}
		nextEffect, _ := rewriter.rewriteEffect(b.Effects[i+1])
		hi, ok := nextEffect.(*Assign)
		if !ok {
			effects = appendPendingWideTempAssigns(effects, pendingTemps, pendingTempOrder, nil)
			pendingTemps = make(map[string]pendingWideTempAlias)
			pendingTempOrder = pendingTempOrder[:0]
			if dst, src, ok := collapseFarPointerOffsetAssign(lo); ok {
				effects = append(effects, &Assign{MetaInfo: lo.MetaInfo, Dst: dst, Src: src})
				changed = true
				continue
			}
			effects = append(effects, effect)
			continue
		}
		dst, src, ok := collapseWideAssign(p.ctx, lo, hi)
		usedTemps := map[string]bool(nil)
		if !ok && len(pendingTemps) > 0 {
			tempLo, loUsed := rewritePendingWideTempAssign(lo, pendingTemps)
			tempHi, hiUsed := rewritePendingWideTempAssign(hi, pendingTemps)
			if len(loUsed) > 0 || len(hiUsed) > 0 {
				if nextLo, changed := rewriter.rewriteEffect(tempLo); changed {
					tempLo, _ = nextLo.(*Assign)
				}
				if nextHi, changed := rewriter.rewriteEffect(tempHi); changed {
					tempHi, _ = nextHi.(*Assign)
				}
				if tempDst, tempSrc, tempOK := collapseWideAssign(p.ctx, tempLo, tempHi); tempOK {
					dst, src, ok = tempDst, tempSrc, true
					usedTemps = loUsed
					for name := range hiUsed {
						if usedTemps == nil {
							usedTemps = make(map[string]bool)
						}
						usedTemps[name] = true
					}
				}
			}
		}
		if ok {
			effects = appendPendingWideTempAssigns(effects, pendingTemps, pendingTempOrder, usedTemps)
			pendingTemps = make(map[string]pendingWideTempAlias)
			pendingTempOrder = pendingTempOrder[:0]
			effects = append(effects, &Assign{MetaInfo: lo.MetaInfo, Dst: dst, Src: src})
			i++
			changed = true
			continue
		}
		if p.ctx != nil {
			dst, src, ok := collapseWideFieldAssign(p.ctx, lo, hi)
			if ok {
				effects = appendPendingWideTempAssigns(effects, pendingTemps, pendingTempOrder, nil)
				pendingTemps = make(map[string]pendingWideTempAlias)
				pendingTempOrder = pendingTempOrder[:0]
				effects = append(effects, &Assign{MetaInfo: lo.MetaInfo, Dst: dst, Src: src})
				i++
				changed = true
				continue
			}
			dst, src, ok = collapseWideBitfieldAssign(p.ctx, lo, hi)
			if ok {
				effects = appendPendingWideTempAssigns(effects, pendingTemps, pendingTempOrder, nil)
				pendingTemps = make(map[string]pendingWideTempAlias)
				pendingTempOrder = pendingTempOrder[:0]
				effects = append(effects, &Assign{MetaInfo: lo.MetaInfo, Dst: dst, Src: src})
				i++
				changed = true
				continue
			}
		}
		effects = appendPendingWideTempAssigns(effects, pendingTemps, pendingTempOrder, nil)
		pendingTemps = make(map[string]pendingWideTempAlias)
		pendingTempOrder = pendingTempOrder[:0]
		if dst, src, ok := collapseFarPointerOffsetAssign(lo); ok {
			effects = append(effects, &Assign{MetaInfo: lo.MetaInfo, Dst: dst, Src: src})
			changed = true
			continue
		}
		effects = append(effects, effect)
	}
	effects = appendPendingWideTempAssigns(effects, pendingTemps, pendingTempOrder, nil)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// appendPendingWideTempAssigns flushes pending temp assignments except those
// consumed by a successful temp-aware wide-store collapse.
func appendPendingWideTempAssigns(effects []Effect, aliases map[string]pendingWideTempAlias, order []string, used map[string]bool) []Effect {
	for _, name := range order {
		alias, ok := aliases[name]
		if !ok || used[name] {
			continue
		}
		effects = append(effects, alias.assign)
	}
	return effects
}

// rewritePendingWideTempAssign replaces temp reads in one assignment source
// with their pending source expressions.
func rewritePendingWideTempAssign(assign *Assign, aliases map[string]pendingWideTempAlias) (*Assign, map[string]bool) {
	used := make(map[string]bool)
	rewriter := &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, changed := w.rewriteExprChildren(expr)
			temp, ok := next.(*Temp)
			if !ok {
				return next, changed, true
			}
			alias, ok := aliases[temp.Name]
			if !ok {
				return next, changed, true
			}
			used[temp.Name] = true
			return alias.value, true, true
		},
	}
	src, changed := rewriter.rewriteExpr(assign.Src)
	if !changed {
		return assign, used
	}
	next := *assign
	next.Src = src
	return &next, used
}

// collapseWideRewriter returns the semantic tree rewrite for wide word aggregates.
func collapseWideRewriter(ctx *FuncContext) *semRewriter {
	return &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			next, changed := w.rewriteEffectChildren(effect)
			if assign, ok := next.(*Assign); ok {
				if dst, src, ok := collapseSemanticBitfieldAssign(ctx, assign); ok {
					return &Assign{MetaInfo: assign.MetaInfo, Dst: dst, Src: src}, true, true
				}
			}
			return next, changed, true
		},
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, changed := w.rewriteExprChildren(expr)
			if resolved, ok := collapseSemanticBitfieldExtract(ctx, next); ok {
				return resolved, true, true
			}
			words, ok := next.(*Words)
			if !ok {
				return next, changed, true
			}
			collapsed, ok := collapseWideWords(words)
			if ok {
				return collapsed, true, true
			}
			return next, changed, true
		},
	}
}

// collapseSemanticBitfieldAssign resolves masked semantic stores to bitfield assignments.
func collapseSemanticBitfieldAssign(ctx *FuncContext, assign *Assign) (LValue, Expr, bool) {
	if ctx == nil {
		return nil, nil, false
	}
	keep, set, ok := semanticMaskedStore(assign.Dst, assign.Src)
	if !ok {
		return nil, nil, false
	}
	path, fieldOff, storageWidth, ok := bitfieldSourcePath(assign.Dst)
	if !ok {
		return nil, nil, false
	}
	fullMask, ok := bitMask(storageWidth * 8)
	if !ok {
		return nil, nil, false
	}
	changed := (^uint(keep.U64)) & fullMask
	bitOff, bitWidth, ok := contiguousMaskRange(changed)
	if !ok {
		return nil, nil, false
	}
	src, ok := unshiftSemanticBitfieldSet(set, bitOff, bitWidth, changed)
	if !ok {
		return nil, nil, false
	}
	field, ok := ctx.res.ResolveBitfieldPathLoadInContext(path, fieldOff, storageWidth, bitOff, bitWidth, ctx.unionContext())
	if !ok {
		return nil, nil, false
	}
	return &SymbolRef{Path: field}, src, true
}

// semanticMaskedStore splits a destination-preserving masked store.
func semanticMaskedStore(dst LValue, src Expr) (*Const, Expr, bool) {
	or, ok := src.(*Binary)
	if !ok || or.Op != OpOr {
		return nil, nil, false
	}
	if keep, ok := semanticKeepMask(dst, or.LHS); ok {
		return keep, or.RHS, true
	}
	if keep, ok := semanticKeepMask(dst, or.RHS); ok {
		return keep, or.LHS, true
	}
	return nil, nil, false
}

// semanticKeepMask returns the keep mask from a destination-preserving AND.
func semanticKeepMask(dst LValue, expr Expr) (*Const, bool) {
	and, ok := expr.(*Binary)
	if !ok || and.Op != OpAnd {
		return nil, false
	}
	keep, kept, ok := constExprOperand(and.LHS, and.RHS)
	if !ok {
		return nil, false
	}
	lvalue, ok := kept.(LValue)
	return keep, ok && sameLValue(dst, lvalue)
}

// unshiftSemanticBitfieldSet returns the value shifted into a bitfield store.
func unshiftSemanticBitfieldSet(expr Expr, bitOff int, bitWidth int, changed uint) (Expr, bool) {
	if c, ok := expr.(*Const); ok {
		if uint(c.U64)&^changed != 0 {
			return nil, false
		}
		fieldMask, ok := bitMask(bitWidth)
		if !ok {
			return nil, false
		}
		next := *c
		next.U64 = (c.U64 >> bitOff) & uint64(fieldMask)
		return &next, true
	}
	shift, ok := expr.(*Binary)
	if bitOff == 0 {
		if ok && shift.Op == OpShl && constExprEquals(shift.RHS, 0) {
			return stripSemanticBitfieldSourceMask(shift.LHS, bitWidth), true
		}
		return stripSemanticBitfieldSourceMask(expr, bitWidth), true
	}
	if !ok || shift.Op != OpShl || !constExprEquals(shift.RHS, uint64(bitOff)) {
		return nil, false
	}
	return stripSemanticBitfieldSourceMask(shift.LHS, bitWidth), true
}

// stripSemanticBitfieldSourceMask removes a low mask implied by the destination bitfield width.
func stripSemanticBitfieldSourceMask(expr Expr, bitWidth int) Expr {
	and, ok := expr.(*Binary)
	if !ok || and.Op != OpAnd {
		return expr
	}
	mask, source, ok := constExprOperand(and.LHS, and.RHS)
	if !ok {
		return expr
	}
	width, ok := lowBitMaskWidth(uint(mask.U64))
	if !ok || width != bitWidth {
		return expr
	}
	return source
}

// collapseSemanticBitfieldExtract resolves shifted-and-masked wide expressions to bitfield reads.
func collapseSemanticBitfieldExtract(ctx *FuncContext, expr Expr) (Expr, bool) {
	if ctx == nil {
		return nil, false
	}
	and, ok := expr.(*Binary)
	if !ok || and.Op != OpAnd {
		return nil, false
	}
	mask, source, ok := constExprOperand(and.LHS, and.RHS)
	if !ok {
		return nil, false
	}
	bitWidth, ok := lowBitMaskWidth(uint(mask.U64))
	if !ok {
		return nil, false
	}
	shift, ok := unwrapBitfieldExtractWrapper(source).(*Binary)
	if !ok || shift.Op != OpShr {
		return nil, false
	}
	amount, value, ok := constExprOperand(shift.LHS, shift.RHS)
	if !ok || value != shift.LHS {
		return nil, false
	}
	base := unwrapBitfieldExtractWrapper(value)
	path, fieldOff, storageWidth, ok := bitfieldSourcePath(base)
	if !ok {
		return nil, false
	}
	field, ok := ctx.res.ResolveBitfieldPathLoadInContext(path, fieldOff, storageWidth, int(amount.U64), bitWidth, ctx.unionContext())
	if !ok {
		return nil, false
	}
	return &SymbolRef{Path: field}, true
}

// bitfieldSourcePath resolves the source storage path for a semantic bitfield extraction.
func bitfieldSourcePath(expr Expr) (symresolve.SymbolPath, int, int, bool) {
	path, ok := symbolPathForExpr(expr)
	if ok {
		width := exprWidth(expr)
		return path, 0, width, width > 0
	}
	if part, ok := expr.(*Part); ok {
		path, ok := symbolPathForExpr(part.Base)
		return path, part.ByteOff, part.Width, ok && part.Width > 0
	}
	if words, ok := expr.(*Words); ok {
		path, off, width, ok := bitfieldWordsSourcePath(words)
		return path, off, width, ok
	}
	return nil, 0, 0, false
}

// bitfieldWordsSourcePath resolves contiguous part words as one storage range.
func bitfieldWordsSourcePath(words *Words) (symresolve.SymbolPath, int, int, bool) {
	if len(words.Words) == 0 {
		return nil, 0, 0, false
	}
	first, ok := words.Words[0].(*Part)
	if !ok || first.Width <= 0 {
		return nil, 0, 0, false
	}
	path, ok := symbolPathForExpr(first.Base)
	if !ok {
		return nil, 0, 0, false
	}
	minOff := first.ByteOff
	maxOff := first.ByteOff + first.Width
	seen := map[int]bool{first.ByteOff: true}
	for _, word := range words.Words[1:] {
		part, ok := word.(*Part)
		if !ok || part.Width != first.Width || seen[part.ByteOff] || !sameLValue(first.Base, part.Base) {
			return nil, 0, 0, false
		}
		seen[part.ByteOff] = true
		if part.ByteOff < minOff {
			minOff = part.ByteOff
		}
		if part.ByteOff+part.Width > maxOff {
			maxOff = part.ByteOff + part.Width
		}
	}
	width := maxOff - minOff
	if width != len(words.Words)*first.Width {
		return nil, 0, 0, false
	}
	return path, minOff, width, true
}

// unwrapBitfieldExtractWrapper removes casts and low-word projections around a wide bitfield expression.
func unwrapBitfieldExtractWrapper(expr Expr) Expr {
	for {
		switch e := expr.(type) {
		case *Cast:
			expr = e.Value
		case *Word:
			if e.Part != machine.WordLow {
				return expr
			}
			expr = e.Parent
		default:
			return expr
		}
	}
}

// collapseWideAssign matches low/high word stores to the same larger lvalue.
func collapseWideAssign(ctx *FuncContext, lo, hi *Assign) (LValue, Expr, bool) {
	// Recognize compiler-generated far-pointer arithmetic before the generic
	// word-pair collapse turns it back into a raw farptr(segment, offset).
	if dst, src, ok := collapseFarPointerOffsetPair(lo, hi); ok {
		return dst, src, true
	}

	loDst, ok := lo.Dst.(*Part)
	if !ok || loDst.ByteOff != 0 || loDst.Width != 2 {
		return nil, nil, false
	}
	hiDst, ok := hi.Dst.(*Part)
	if !ok || hiDst.ByteOff != 2 || hiDst.Width != 2 {
		return nil, nil, false
	}
	if !sameLValue(loDst.Base, hiDst.Base) {
		return nil, nil, false
	}
	if loDst.Base.ExprType() == nil || loDst.Base.ExprType().Bytes() != 4 {
		return nil, nil, false
	}

	loParent, ok := lowWordParent(lo.Src)
	if !ok {
		if src, ok := collapseFarPointerWords(ctx, loDst.Base, hi.Src, lo.Src); ok {
			return loDst.Base, src, true
		}
		if src, ok := collapseWideExprPair(hi.Src, lo.Src, loDst.Base.ExprType()); ok {
			return loDst.Base, src, true
		}
		return nil, nil, false
	}
	hiParent, ok := highWordParent(hi.Src)
	if !ok {
		if src, ok := collapseFarPointerWords(ctx, loDst.Base, hi.Src, lo.Src); ok {
			return loDst.Base, src, true
		}
		if src, ok := collapseWideConstWords(hi.Src, lo.Src); ok {
			return loDst.Base, src, true
		}
		if src, ok := collapseWideExprPair(hi.Src, lo.Src, loDst.Base.ExprType()); ok {
			return loDst.Base, src, true
		}
		return nil, nil, false
	}
	if !sameExpr(loParent, hiParent) {
		if src, ok := collapseFarPointerWords(ctx, loDst.Base, hi.Src, lo.Src); ok {
			return loDst.Base, src, true
		}
		if src, ok := collapseWideExprPair(hi.Src, lo.Src, loDst.Base.ExprType()); ok {
			return loDst.Base, src, true
		}
		return nil, nil, false
	}
	if src, ok := collapseWideConstWords(hi.Src, lo.Src); ok {
		return loDst.Base, src, true
	}
	if loParent.ExprType() == nil || loDst.Base.ExprType() == nil {
		return nil, nil, false
	}
	if loParent.ExprType().Bytes() != loDst.Base.ExprType().Bytes() {
		return nil, nil, false
	}
	return loDst.Base, loParent, true
}

// collapseFarPointerOffsetPair collapses paired low/high word updates that
// advance a far pointer by the same signed byte offset.
func collapseFarPointerOffsetPair(lo, hi *Assign) (LValue, Expr, bool) {
	loDst, ok := lo.Dst.(*Part)
	if !ok || loDst.ByteOff != 0 || loDst.Width != 2 {
		return nil, nil, false
	}

	hiDst, ok := hi.Dst.(*Part)
	if !ok || hiDst.ByteOff != 2 || hiDst.Width != 2 {
		return nil, nil, false
	}

	if !sameLValue(loDst.Base, hiDst.Base) ||
		!typeinfo.IsFarPointer(loDst.Base.ExprType()) {
		return nil, nil, false
	}

	delta, ok := farPointerOffsetAssignOffset(loDst.Base, lo.Src)
	if !ok {
		return nil, nil, false
	}

	if !matchesFarPointerHighAdjustment(loDst.Base, delta, hi.Src) {
		return nil, nil, false
	}

	if src, ok := pointerOffsetExpr(loDst.Base, delta); ok {
		return loDst.Base, src, true
	}

	return loDst.Base, &PointerOffset{
		Pointer:  loDst.Base,
		Offset:   delta,
		TypeInfo: loDst.Base.ExprType(),
	}, true
}

// matchesFarPointerHighAdjustment matches the compiler-generated high-word
// adjustment for adding a signed 16-bit byte offset to a far pointer:
//
//	(((signhiword(delta) + 0) << 0xffff) + HIWORD(ptr))
//
// The zero add and outer add are treated as commutative where harmless.
func matchesFarPointerHighAdjustment(ptr Expr, delta Expr, high Expr) bool {
	add, ok := high.(*Binary)
	if !ok || add.Op != OpAdd {
		return false
	}

	// One side must be HIWORD(ptr); the other must be the segment adjustment.
	var adjust Expr
	if parent, ok := highWordParent(add.LHS); ok && sameExpr(parent, ptr) {
		adjust = add.RHS
	} else if parent, ok := highWordParent(add.RHS); ok && sameExpr(parent, ptr) {
		adjust = add.LHS
	} else {
		return false
	}

	shl, ok := adjust.(*Binary)
	if !ok || shl.Op != OpShl {
		return false
	}

	// Match the exact normalized shift count currently produced for this
	// compiler idiom.
	shift, ok := shl.RHS.(*Const)
	if !ok || shift.U64&0xffff != 0xffff {
		return false
	}

	signHigh := stripAddZero(shl.LHS)

	parent, ok := wordPartParent(signHigh, machine.WordSignHigh)
	if !ok {
		return false
	}

	return sameExpr(parent, delta)
}

// stripAddZero removes a single harmless "+ 0" wrapper.
func stripAddZero(expr Expr) Expr {
	add, ok := expr.(*Binary)
	if !ok || add.Op != OpAdd {
		return expr
	}

	if constExprEquals(add.LHS, 0) {
		return add.RHS
	}
	if constExprEquals(add.RHS, 0) {
		return add.LHS
	}

	return expr
}

// collapseFarPointerOffsetAssign rewrites low-word far pointer arithmetic as a
// whole-pointer assignment.
func collapseFarPointerOffsetAssign(assign *Assign) (LValue, Expr, bool) {
	dst, ok := assign.Dst.(*Part)
	if !ok || dst.ByteOff != 0 || dst.Width != 2 || !typeinfo.IsFarPointer(dst.Base.ExprType()) {
		return nil, nil, false
	}
	offset, ok := farPointerOffsetAssignOffset(dst.Base, assign.Src)
	if !ok {
		return nil, nil, false
	}
	if src, ok := pointerOffsetExpr(dst.Base, offset); ok {
		return dst.Base, src, true
	}
	return dst.Base, &PointerOffset{
		Pointer:  dst.Base,
		Offset:   offset,
		TypeInfo: dst.Base.ExprType(),
	}, true
}

// pointerOffsetExpr converts exact byte deltas to source-level pointer steps.
func pointerOffsetExpr(pointer Expr, offset Expr) (Expr, bool) {
	ptr, ok := pointer.ExprType().(*typeinfo.Pointer)
	if !ok || ptr.Elem == nil || ptr.Elem.Bytes() <= 0 {
		return nil, false
	}
	negative := false
	if neg, ok := offset.(*Unary); ok && neg.Op == OpNeg {
		offset = neg.X
		negative = true
	}
	c, ok := offset.(*Const)
	if !ok || c.U64%uint64(ptr.Elem.Bytes()) != 0 {
		return nil, false
	}
	steps := c.U64 / uint64(ptr.Elem.Bytes())
	if steps == 0 {
		return pointer, true
	}
	op := OpAdd
	if negative {
		op = OpSub
	}
	return &Binary{
		TypeInfo: pointer.ExprType(),
		Op:       op,
		LHS:      pointer,
		RHS:      &Const{TypeInfo: typeinfo.U16, U64: steps},
	}, true
}

// farPointerOffsetAssignOffset extracts the byte delta from a low-word pointer
// assignment such as LOWORD(ptr) = LOWORD(ptr) + n.
func farPointerOffsetAssignOffset(pointer LValue, src Expr) (Expr, bool) {
	binary, ok := src.(*Binary)
	if !ok {
		return nil, false
	}
	switch binary.Op {
	case OpAdd:
		if sameFarPointerOffsetPart(pointer, binary.LHS) {
			return binary.RHS, true
		}
		if sameFarPointerOffsetPart(pointer, binary.RHS) {
			return binary.LHS, true
		}
	case OpSub:
		if sameFarPointerOffsetPart(pointer, binary.LHS) {
			return &Unary{TypeInfo: typeinfo.U16, Op: OpNeg, X: binary.RHS}, true
		}
	}
	return nil, false
}

// sameFarPointerOffsetPart reports whether expr is the low word of pointer.
func sameFarPointerOffsetPart(pointer LValue, expr Expr) bool {
	parent, ok := lowWordParent(expr)
	return ok && sameExpr(parent, pointer)
}

// collapseWideExprPair reconstructs a wide expression from matching high and
// low word expression trees.
func collapseWideExprPair(high, low Expr, expected typeinfo.Type) (Expr, bool) {
	if value, ok := collapseWideConstWords(high, low); ok {
		return value, true
	}
	if value, ok := collapseWideWordPair(high, low); ok {
		return value, true
	}
	hiBinary, hiOK := high.(*Binary)
	loBinary, loOK := low.(*Binary)
	if hiOK && loOK && hiBinary.Op == loBinary.Op {
		switch hiBinary.Op {
		case OpAdd, OpSub:
		default:
			return nil, false
		}
		lhs, lhsOK := collapseWideExprPair(hiBinary.LHS, loBinary.LHS, nil)
		rhs, rhsOK := collapseWideExprPair(hiBinary.RHS, loBinary.RHS, nil)
		if lhsOK && rhsOK {
			return &Binary{
				TypeInfo: collapseWideExprType(expected, lhs, rhs),
				Op:       hiBinary.Op,
				LHS:      lhs,
				RHS:      rhs,
			}, true
		}
	}
	return nil, false
}

// collapseWideExprType chooses the type for a rebuilt wide expression.
func collapseWideExprType(expected typeinfo.Type, exprs ...Expr) typeinfo.Type {
	if expected != nil && expected.Bytes() == 4 {
		return expected
	}
	for _, expr := range exprs {
		if expr != nil && expr.ExprType() != nil && expr.ExprType().Bytes() == 4 {
			return expr.ExprType()
		}
	}
	return intTypeForWidth(4)
}

// collapseFarPointerWords combines pointer low/high word stores into one far pointer expression.
func collapseFarPointerWords(ctx *FuncContext, dst LValue, high, low Expr) (Expr, bool) {
	if !typeinfo.IsFarPointer(dst.ExprType()) || exprWidth(high) != 2 {
		return nil, false
	}
	if ctx != nil {
		if src, ok := collapseDataSegmentPointerWords(ctx, dst.ExprType(), high, low); ok {
			return src, true
		}
	}
	ptr := &FarPointer{
		Part:     machine.FarPointerWhole,
		Offset:   low,
		Segment:  high,
		TypeInfo: dst.ExprType(),
	}
	if ctx != nil {
		if resolved, ok := ctx.resolveFarPointerWithRecoveredOffset(ptr); ok {
			if decayed, ok := decayArrayAddress(resolved, dst.ExprType()); ok {
				return decayed, true
			}
			return resolved, true
		}
		if resolved, ok := ctx.resolveSemanticFarPointer(ptr); ok {
			if decayed, ok := decayArrayAddress(resolved, dst.ExprType()); ok {
				return decayed, true
			}
			return resolved, true
		}
	}
	if exprWidth(low) != 2 {
		return nil, false
	}
	if resolved, ok := collapseConstSegmentFarPointerOffset(ptr); ok {
		return resolved, true
	}
	return ptr, true
}

// collapseDataSegmentPointerWords collapses DS plus a pointer-valued low word.
func collapseDataSegmentPointerWords(ctx *FuncContext, expected typeinfo.Type, high Expr, low Expr) (Expr, bool) {
	if !exprMatchesMachineValue(high, ctx.dsReg) {
		return nil, false
	}
	if !sourcePointerMatchesFarPointer(low.ExprType(), expected) {
		return nil, false
	}
	return low, true
}

// collapseWideFieldAssign resolves paired word deref stores to one field assignment.
func collapseWideFieldAssign(ctx *FuncContext, lo, hi *Assign) (LValue, Expr, bool) {
	pointer, off, ok := derefWordStorePair(lo.Dst, hi.Dst)
	if !ok {
		return nil, nil, false
	}
	src, ok := collapseWideConstWords(hi.Src, lo.Src)
	if !ok {
		return nil, nil, false
	}
	path, ok := symbolPathForExpr(pointer)
	if !ok {
		return nil, nil, false
	}
	field, ok := ctx.res.ResolveFieldPathLoad(path, off, 4)
	if !ok {
		return nil, nil, false
	}
	return &SymbolRef{Path: field}, src, true
}

// collapseWideBitfieldAssign resolves paired word writes to one wide bitfield assignment.
func collapseWideBitfieldAssign(ctx *FuncContext, lo, hi *Assign) (LValue, Expr, bool) {
	pointer, off, ok := derefWordStorePair(lo.Dst, hi.Dst)
	if !ok {
		return nil, nil, false
	}
	loKeep, loSet, ok := maskedWordStore(lo.Dst, lo.Src)
	if !ok {
		return nil, nil, false
	}
	hiKeep, hiSet, ok := maskedWordStore(hi.Dst, hi.Src)
	if !ok {
		return nil, nil, false
	}
	keep := (hiKeep << 16) | loKeep
	set := (hiSet << 16) | loSet
	fullMaskValue, ok := bitMask(32)
	if !ok {
		return nil, nil, false
	}
	fullMask := uint64(fullMaskValue)
	changed := (^keep) & fullMask
	bitOff, bitWidth, ok := contiguousMaskRange(uint(changed))
	if !ok || set&^changed != 0 {
		return nil, nil, false
	}
	path, ok := symbolPathForExpr(pointer)
	if !ok {
		return nil, nil, false
	}
	field, ok := ctx.res.ResolveBitfieldPathLoad(path, off, 4, bitOff, bitWidth)
	if !ok {
		return nil, nil, false
	}
	fieldMaskValue, ok := bitMask(bitWidth)
	if !ok {
		return nil, nil, false
	}
	fieldMask := uint64(fieldMaskValue)
	return &SymbolRef{Path: field}, &Const{TypeInfo: field.Type(), U64: (set >> bitOff) & fieldMask}, true
}

// derefWordStorePair returns the pointer and low byte offset for paired deref stores.
func derefWordStorePair(lo, hi LValue) (Expr, int, bool) {
	loDst, ok := lo.(*Deref)
	if !ok || loDst.Width != 2 {
		return nil, 0, false
	}
	hiDst, ok := hi.(*Deref)
	if !ok || hiDst.Width != 2 || hiDst.ByteOff != loDst.ByteOff+2 {
		return nil, 0, false
	}
	if !sameExpr(loDst.Pointer, hiDst.Pointer) {
		return nil, 0, false
	}
	return loDst.Pointer, loDst.ByteOff, true
}

// maskedWordStore returns keep and set masks for a word-sized masked write.
func maskedWordStore(dst LValue, src Expr) (uint64, uint64, bool) {
	source, set := src, uint64(0)
	if or, ok := src.(*Binary); ok && or.Op == OpOr {
		setConst, nextSource, ok := constExprOperand(or.LHS, or.RHS)
		if !ok {
			return 0, 0, false
		}
		set = setConst.U64
		source = nextSource
	}
	and, ok := source.(*Binary)
	if !ok || and.Op != OpAnd {
		return 0, 0, false
	}
	keepConst, keptSource, ok := constExprOperand(and.LHS, and.RHS)
	if !ok {
		return 0, 0, false
	}
	keptLValue, ok := keptSource.(LValue)
	if !ok || !sameLValue(dst, keptLValue) {
		return 0, 0, false
	}
	return keepConst.U64 & 0xffff, set & 0xffff, true
}

// collapseWideWords matches two word args that reconstruct one wide expression.
func collapseWideWords(words *Words) (Expr, bool) {
	if len(words.Words) != 2 {
		return nil, false
	}
	if value, ok := collapseWideConstWords(words.Words[0], words.Words[1]); ok {
		return value, true
	}
	if parent, ok := collapseWideWordPair(words.Words[0], words.Words[1]); ok {
		return parent, true
	}
	return collapseWideWordPair(words.Words[1], words.Words[0])
}

// collapseWideConstWords matches a high and low const word to one 32-bit const.
func collapseWideConstWords(high, low Expr) (Expr, bool) {
	hiConst, hiOK := high.(*Const)
	loConst, loOK := low.(*Const)
	if !hiOK || !loOK || exprWidth(high) != 2 || exprWidth(low) != 2 {
		return nil, false
	}
	return &Const{TypeInfo: intTypeForWidth(4), U64: ((hiConst.U64 & 0xffff) << 16) | (loConst.U64 & 0xffff)}, true
}

// collapseWideWordPair matches one high word and one low word from the same value.
func collapseWideWordPair(high, low Expr) (Expr, bool) {
	hiParent, hiOK := highWordParent(high)
	loParent, loOK := lowWordParent(low)
	if hiOK && loOK && sameExpr(hiParent, loParent) && exprWidth(hiParent) == 4 {
		return hiParent, true
	}

	hiParent, hiOK = wordPartParent(high, machine.WordSignHigh)
	loParent, loOK = lowWordParent(low)
	if hiOK && loOK && sameExpr(hiParent, loParent) && exprWidth(loParent) == 2 {
		return &SignExtend{Parent: loParent, FromBits: 16, ToBits: 32, TypeInfo: intTypeForWidth(4)}, true
	}
	return nil, false
}

// highWordParent returns the value represented by a high word expression.
func highWordParent(expr Expr) (Expr, bool) {
	if parent, ok := wordPartParent(expr, machine.WordHigh); ok {
		return parent, true
	}
	if parent, ok := farPointerPartParent(expr, machine.FarPointerSegment); ok {
		return parent, true
	}
	if deref, ok := expr.(*Deref); ok && deref.ByteOff == 2 && deref.Width == 2 && exprWidth(deref.Pointer) == 4 {
		return deref.Pointer, true
	}
	part, ok := expr.(*Part)
	if !ok || part.ByteOff != 2 || part.Width != 2 {
		return nil, false
	}
	return part.Base, true
}

// wordPartParent returns the parent of a matching word projection.
func wordPartParent(expr Expr, part machine.WordPart) (Expr, bool) {
	word, ok := expr.(*Word)
	if !ok || word.Part != part {
		return nil, false
	}
	return word.Parent, true
}

// lowWordParent returns the value represented by a low word expression.
func lowWordParent(expr Expr) (Expr, bool) {
	if parent, ok := wordPartParent(expr, machine.WordLow); ok {
		return parent, true
	}
	if parent, ok := farPointerPartParent(expr, machine.FarPointerOffset); ok {
		return parent, true
	}
	if part, ok := expr.(*Part); ok && part.ByteOff == 0 && part.Width == 2 {
		return part.Base, true
	}
	if exprWidth(expr) == 2 {
		return expr, true
	}
	return nil, false
}

// farPointerPartParent returns the parent of a matching far-pointer word projection.
func farPointerPartParent(expr Expr, part machine.FarPointerPart) (Expr, bool) {
	ptr, ok := expr.(*FarPointer)
	if !ok || ptr.Part != part || ptr.Parent == nil {
		return nil, false
	}
	return ptr.Parent, true
}

// exprWidth returns the expression width in bytes when it is known.
func exprWidth(expr Expr) int {
	if expr == nil || expr.ExprType() == nil {
		return 0
	}
	return expr.ExprType().Bytes()
}
