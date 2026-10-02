package sem

import "github.com/sirgwain/stars-asm/dasm/typeinfo"

type simplifyBooleansProcessor struct{}

// ProcessBlock writes boolean values left as ternaries as the comparisons
// they are: c ? 1 : 0 is c and c ? 0 : 1 is c inverted, where c is a
// comparison, which C already evaluates to 0 or 1. A comparison of such a
// value with 0 is likewise the value or its inverse, so
// ((fSet == 0 ? 1 : 0) == 0 ? 0 : 1) is fSet == 0.
func (p *simplifyBooleansProcessor) ProcessBlock(_ *Result, _ Func, b Block) (Block, bool) {
	rewriter := &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, childChanged := w.rewriteExprChildren(expr)
			if simplified, ok := simplifyBoolean(next); ok {
				return simplified, true, true
			}
			return next, childChanged, true
		},
	}
	effects, changed := rewriter.rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// simplifyBoolean rewrites one boolean ternary, or one comparison of a
// boolean with 0, to the boolean or its inverse.
func simplifyBoolean(expr Expr) (Expr, bool) {
	switch e := expr.(type) {
	case *Cond:
		cond, ok := e.Cond.(*Compare)
		if !ok {
			return nil, false
		}
		switch {
		case isBooleanConst(e.Then, 1) && isBooleanConst(e.Else, 0):
			return cond, true
		case isBooleanConst(e.Then, 0) && isBooleanConst(e.Else, 1):
			return invertCompare(cond)
		}
	case *Compare:
		value, ok := e.LHS.(*Compare)
		if !ok || !isBooleanConst(e.RHS, 0) {
			return nil, false
		}
		switch e.Op {
		case CompareNE:
			return value, true
		case CompareEQ:
			return invertCompare(value)
		}
	}
	return nil, false
}

// isBooleanConst reports whether expr is the plain integer constant value,
// not a named enum member that only happens to equal it.
func isBooleanConst(expr Expr, value uint64) bool {
	c, ok := expr.(*Const)
	if !ok || c.U64 != value {
		return false
	}
	_, enum := c.TypeInfo.(*typeinfo.Enum)
	return !enum
}

// invertCompare returns the comparison that holds when cmp does not. A
// floating comparison is not inverted: with a NaN operand, neither a < b nor
// a >= b holds.
func invertCompare(cmp *Compare) (Expr, bool) {
	for _, operand := range []Expr{cmp.LHS, cmp.RHS} {
		if typ := operand.ExprType(); typ != nil && typ.Kind() == typeinfo.KFloat {
			return nil, false
		}
	}
	next := *cmp
	next.Op = negateCompare(cmp.Op)
	return &next, true
}
