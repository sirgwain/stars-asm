package sem

import "github.com/sirgwain/stars-asm/dasm/typeinfo"

type charArithmeticProcessor struct{}

// ProcessBlock writes the printable constants of character arithmetic as
// character literals, as the original did: *lpT - '0', szWork[0] - 'A',
// and sz[1] = 'A' + i stored into a char. Only how the constant prints
// changes; its type and value stay those of the arithmetic.
func (p *charArithmeticProcessor) ProcessBlock(_ *Result, _ Func, b Block) (Block, bool) {
	rewriter := &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, childChanged := w.rewriteExprChildren(expr)
			if binary, ok := next.(*Binary); ok && charValued(binary) {
				if marked, ok := markCharOperand(binary); ok {
					return marked, true, true
				}
			}
			return next, childChanged, true
		},
	}
	effects, changed := rewriter.rewriteEffects(b.Effects)
	for i, effect := range effects {
		assign, ok := effect.(*Assign)
		if !ok || !isCharType(assign.Dst.ExprType()) {
			continue
		}
		if binary, ok := assign.Src.(*Binary); ok {
			if marked, ok := markCharOperand(binary); ok {
				next := *assign
				next.Src = marked
				effects[i] = &next
				changed = true
			}
		}
	}
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// charValued reports whether a sum or difference has a character operand:
// a char value, possibly promoted, or another such sum, as in
// 10 * i + *lpT - 48.
func charValued(expr Expr) bool {
	if _, ok := promotedChar(expr); ok {
		return true
	}
	binary, ok := expr.(*Binary)
	if !ok || binary.Op != OpAdd && binary.Op != OpSub {
		return false
	}
	return charValued(binary.LHS) || charValued(binary.RHS)
}

// markCharOperand marks the printable constant operand of a sum or
// difference as a character literal.
func markCharOperand(binary *Binary) (*Binary, bool) {
	if binary.Op != OpAdd && binary.Op != OpSub {
		return nil, false
	}
	for _, side := range []*Expr{&binary.RHS, &binary.LHS} {
		c, ok := (*side).(*Const)
		if !ok || c.Char || c.Fixup != nil || !IsCharLiteralValue(c.U64) {
			continue
		}
		if _, enum := c.TypeInfo.(*typeinfo.Enum); enum {
			continue
		}
		marked := *c
		marked.Char = true
		next := *binary
		if side == &binary.RHS {
			next.RHS = &marked
		} else {
			next.LHS = &marked
		}
		return &next, true
	}
	return nil, false
}

// isCharType reports whether typ is C's char, as text buffers are declared,
// rather than a uint8_t or int8_t byte.
func isCharType(typ typeinfo.Type) bool {
	prim, ok := typ.(*typeinfo.Primitive)
	return ok && prim.TypeKind == typeinfo.KInt && prim.Name == "char"
}
