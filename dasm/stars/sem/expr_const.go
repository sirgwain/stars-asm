package sem

import "github.com/sirgwain/stars-asm/dasm/stars/asm"

// constExprOperand splits one constant and one non-constant semantic expression.
func constExprOperand(a, b Expr) (*Const, Expr, bool) {
	if c, ok := a.(*Const); ok {
		return c, b, true
	}
	if c, ok := b.(*Const); ok {
		return c, a, true
	}
	return nil, nil, false
}

// constExprEquals reports whether expr is a constant with the given value.
func constExprEquals(expr Expr, value uint64) bool {
	c, ok := expr.(*Const)
	return ok && c.U64 == value
}

// regExprEquals reports whether expr is a register with the given value
func regExprEquals(expr Expr, value asm.Reg) bool {
	c, ok := expr.(*Register)
	return ok && c.Val == value
}
