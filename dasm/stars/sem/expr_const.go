package sem

// constExprEquals reports whether expr is a constant with the given value.
func constExprEquals(expr Expr, value uint64) bool {
	c, ok := expr.(*Const)
	return ok && c.U64 == value
}
