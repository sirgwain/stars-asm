package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

type semEffectRewrite func(*semRewriter, Effect) (Effect, bool, bool)
type semExprRewrite func(*semRewriter, Expr) (Expr, bool, bool)
type semLValueRewrite func(*semRewriter, LValue) (LValue, bool, bool)
type semCallRewrite func(*semRewriter, *Call, machine.Meta) (*Call, bool, bool)

type semRewriter struct {
	effect semEffectRewrite
	expr   semExprRewrite
	lvalue semLValueRewrite
	call   semCallRewrite
}

// rewriteEffects rewrites a slice of semantic effects.
func (w *semRewriter) rewriteEffects(effects []Effect) ([]Effect, bool) {
	var out []Effect
	for i, effect := range effects {
		next, effectChanged := w.rewriteEffect(effect)
		if !effectChanged {
			if out != nil {
				out[i] = effect
			}
			continue
		}
		if out == nil {
			out = append([]Effect(nil), effects...)
		}
		out[i] = next
	}
	if out == nil {
		return effects, false
	}
	return out, true
}

// rewriteEffect rewrites one semantic effect.
func (w *semRewriter) rewriteEffect(effect Effect) (Effect, bool) {
	if w.effect != nil {
		if next, changed, handled := w.effect(w, effect); handled {
			return next, changed
		}
	}
	return w.rewriteEffectChildren(effect)
}

// rewriteEffectChildren rewrites only the direct children of an effect.
func (w *semRewriter) rewriteEffectChildren(effect Effect) (Effect, bool) {
	switch e := effect.(type) {
	case *Assign:
		dst, dstChanged := w.rewriteLValue(e.Dst)
		src, srcChanged := w.rewriteExpr(e.Src)
		if !dstChanged && !srcChanged {
			return effect, false
		}
		next := *e
		next.Dst = dst
		next.Src = src
		return &next, true
	case *CallEffect:
		call, callChanged := e.Call, false
		if e.Call != nil {
			call, callChanged = w.rewriteCall(e.Call, e.EffectMeta())
		}
		result, resultChanged := w.rewriteExpr(e.Result)
		if !callChanged && !resultChanged {
			return effect, false
		}
		next := *e
		next.Call = call
		next.Result = result
		return &next, true
	case *Branch:
		cond, changed := w.rewriteExpr(e.Cond)
		if !changed {
			return effect, false
		}
		next := *e
		next.Cond = cond
		return &next, true
	case *Return:
		value, changed := w.rewriteExpr(e.Value)
		if !changed {
			return effect, false
		}
		next := *e
		next.Value = value
		return &next, true
	default:
		return effect, false
	}
}

// rewriteCall rewrites one semantic call expression.
func (w *semRewriter) rewriteCall(call *Call, meta machine.Meta) (*Call, bool) {
	if w.call != nil {
		if next, changed, handled := w.call(w, call, meta); handled {
			return next, changed
		}
	}
	return w.rewriteCallChildren(call)
}

// rewriteCallChildren rewrites only the direct argument children of a call.
func (w *semRewriter) rewriteCallChildren(call *Call) (*Call, bool) {
	args, changed := w.rewriteExprs(call.Args)
	if !changed {
		return call, false
	}
	next := *call
	next.Args = args
	return &next, true
}

// rewriteExprs rewrites a slice of semantic expressions.
func (w *semRewriter) rewriteExprs(exprs []Expr) ([]Expr, bool) {
	var out []Expr
	for i, expr := range exprs {
		next, exprChanged := w.rewriteExpr(expr)
		if !exprChanged {
			if out != nil {
				out[i] = expr
			}
			continue
		}
		if out == nil {
			out = append([]Expr(nil), exprs...)
		}
		out[i] = next
	}
	if out == nil {
		return exprs, false
	}
	return out, true
}

// rewriteExpr rewrites one semantic expression.
func (w *semRewriter) rewriteExpr(expr Expr) (Expr, bool) {
	if w.expr != nil {
		if next, changed, handled := w.expr(w, expr); handled {
			return next, changed
		}
	}
	return w.rewriteExprChildren(expr)
}

// rewriteExprChildren rewrites only the direct children of an expression.
func (w *semRewriter) rewriteExprChildren(expr Expr) (Expr, bool) {
	switch e := expr.(type) {
	case nil:
		return nil, false
	case LValue:
		return w.rewriteLValue(e)
	case *Unary:
		x, changed := w.rewriteExpr(e.X)
		if !changed {
			return expr, false
		}
		next := *e
		next.X = x
		return &next, true
	case *Binary:
		lhs, lhsChanged := w.rewriteExpr(e.LHS)
		rhs, rhsChanged := w.rewriteExpr(e.RHS)
		if !lhsChanged && !rhsChanged {
			return expr, false
		}
		next := *e
		next.LHS = lhs
		next.RHS = rhs
		return &next, true
	case *Byte:
		parent, parentChanged := w.rewriteExpr(e.Parent)
		value, valueChanged := w.rewriteExpr(e.Value)
		if !parentChanged && !valueChanged {
			return expr, false
		}
		next := *e
		next.Parent = parent
		next.Value = value
		return &next, true
	case *Cast:
		value, changed := w.rewriteExpr(e.Value)
		if !changed {
			return expr, false
		}
		next := *e
		next.Value = value
		return &next, true
	case *ResourceID:
		value, changed := w.rewriteExpr(e.Value)
		if !changed {
			return expr, false
		}
		next := *e
		next.Value = value
		return &next, true
	case *Compare:
		lhs, lhsChanged := w.rewriteExpr(e.LHS)
		rhs, rhsChanged := w.rewriteExpr(e.RHS)
		if !lhsChanged && !rhsChanged {
			return expr, false
		}
		next := *e
		next.LHS = lhs
		next.RHS = rhs
		return &next, true
	case *SignExtend:
		parent, changed := w.rewriteExpr(e.Parent)
		if !changed {
			return expr, false
		}
		next := *e
		next.Parent = parent
		return &next, true
	case *Call:
		return w.rewriteCall(e, machine.Meta{})
	case *Word:
		parent, changed := w.rewriteExpr(e.Parent)
		if !changed {
			return expr, false
		}
		next := *e
		next.Parent = parent
		return &next, true
	case *FarPointer:
		parent, parentChanged := w.rewriteExpr(e.Parent)
		offset, offsetChanged := w.rewriteExpr(e.Offset)
		segment, segmentChanged := w.rewriteExpr(e.Segment)
		if !parentChanged && !offsetChanged && !segmentChanged {
			return expr, false
		}
		next := *e
		next.Parent = parent
		next.Offset = offset
		next.Segment = segment
		return &next, true
	case *PointerOffset:
		pointer, pointerChanged := w.rewriteExpr(e.Pointer)
		offset, offsetChanged := w.rewriteExpr(e.Offset)
		if !pointerChanged && !offsetChanged {
			return expr, false
		}
		next := *e
		next.Pointer = pointer
		next.Offset = offset
		return &next, true
	case *Words:
		words, changed := w.rewriteExprs(e.Words)
		if !changed {
			return expr, false
		}
		next := *e
		next.Words = words
		return &next, true
	case *AddressOf:
		target, changed := w.rewriteLValue(e.Target)
		if !changed {
			return expr, false
		}
		next := *e
		next.Target = target
		return &next, true
	case *Merge:
		arms, changed := w.rewriteMergeArms(e.Arms)
		if !changed {
			return expr, false
		}
		next := *e
		next.Arms = arms
		return &next, true
	default:
		return expr, false
	}
}

// rewriteMergeArms rewrites values in merge arms.
func (w *semRewriter) rewriteMergeArms(arms []MergeArm) ([]MergeArm, bool) {
	var out []MergeArm
	for i, arm := range arms {
		value, valueChanged := w.rewriteExpr(arm.Value)
		if !valueChanged {
			if out != nil {
				out[i] = arm
			}
			continue
		}
		if out == nil {
			out = append([]MergeArm(nil), arms...)
		}
		out[i] = arm
		out[i].Value = value
	}
	if out == nil {
		return arms, false
	}
	return out, true
}

// rewriteLValue rewrites one semantic lvalue.
func (w *semRewriter) rewriteLValue(value LValue) (LValue, bool) {
	if w.lvalue != nil {
		if next, changed, handled := w.lvalue(w, value); handled {
			return next, changed
		}
	}
	return w.rewriteLValueChildren(value)
}

// rewriteLValueChildren rewrites only the direct children of an lvalue.
func (w *semRewriter) rewriteLValueChildren(value LValue) (LValue, bool) {
	switch v := value.(type) {
	case *ArrayIndex:
		base, baseChanged := w.rewriteExpr(v.Base)
		index, indexChanged := w.rewriteExpr(v.Index)
		if !baseChanged && !indexChanged {
			return value, false
		}
		next := *v
		next.Base = base
		next.Index = index
		return &next, true
	case *FieldAccess:
		base, changed := w.rewriteExpr(v.Base)
		if !changed {
			return value, false
		}
		next := *v
		next.Base = base
		return &next, true
	case *Part:
		base, changed := w.rewriteLValue(v.Base)
		if !changed {
			return value, false
		}
		next := *v
		next.Base = base
		return &next, true
	case *Deref:
		pointer, changed := w.rewriteExpr(v.Pointer)
		if !changed {
			return value, false
		}
		next := *v
		next.Pointer = pointer
		return &next, true
	case *Memory:
		seg, segChanged := w.rewriteExpr(v.Seg)
		base, baseChanged := w.rewriteExpr(v.Base)
		index, indexChanged := w.rewriteExpr(v.Index)
		if !segChanged && !baseChanged && !indexChanged {
			return value, false
		}
		next := *v
		next.Seg = seg
		next.Base = base
		next.Index = index
		return &next, true
	default:
		return value, false
	}
}

type exprVisitor func(Expr)

func walkEffect(effect Effect, visit exprVisitor) {
	switch e := effect.(type) {
	case *Assign:
		walkExpr(e.Dst, visit)
		walkExpr(e.Src, visit)

	case *CallEffect:
		walkCall(e.Call, visit)
		walkExpr(e.Result, visit)

	case *Branch:
		walkExpr(e.Cond, visit)

	case *Return:
		walkExpr(e.Value, visit)
	}
}

func walkCall(call *Call, visit exprVisitor) {
	if call == nil {
		return
	}

	// Your rewriter currently doesn't walk Target, but for a general
	// semantic visitor I think it should.
	walkExpr(call.Target, visit)

	for _, arg := range call.Args {
		walkExpr(arg, visit)
	}
}

func walkExpr(expr Expr, visit exprVisitor) {
	if expr == nil {
		return
	}

	// Pre-order visitation means a Merge counts once, then its arms
	// are recursively inspected for other interesting expressions.
	visit(expr)

	switch e := expr.(type) {
	case *Unary:
		walkExpr(e.X, visit)

	case *Binary:
		walkExpr(e.LHS, visit)
		walkExpr(e.RHS, visit)

	case *Byte:
		walkExpr(e.Parent, visit)
		walkExpr(e.Value, visit)

	case *Cast:
		walkExpr(e.Value, visit)

	case *ResourceID:
		walkExpr(e.Value, visit)

	case *ArrayIndex:
		walkExpr(e.Base, visit)
		walkExpr(e.Index, visit)

	case *FieldAccess:
		walkExpr(e.Base, visit)

	case *Compare:
		walkExpr(e.LHS, visit)
		walkExpr(e.RHS, visit)

	case *SignExtend:
		walkExpr(e.Parent, visit)

	case *Call:
		walkCall(e, visit)

	case *Word:
		walkExpr(e.Parent, visit)

	case *FarPointer:
		walkExpr(e.Parent, visit)
		walkExpr(e.Offset, visit)
		walkExpr(e.Segment, visit)

	case *PointerOffset:
		walkExpr(e.Pointer, visit)
		walkExpr(e.Offset, visit)

	case *Deref:
		walkExpr(e.Pointer, visit)

	case *Words:
		for _, word := range e.Words {
			walkExpr(word, visit)
		}

	case *Part:
		walkExpr(e.Base, visit)

	case *AddressOf:
		walkExpr(e.Target, visit)

	case *Merge:
		for _, arm := range e.Arms {
			walkExpr(arm.Value, visit)
		}

	case *Memory:
		walkExpr(e.Seg, visit)
		walkExpr(e.Base, visit)
		walkExpr(e.Index, visit)
	}
}
