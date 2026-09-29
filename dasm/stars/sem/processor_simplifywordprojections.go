package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

type simplifyWordProjectionsProcessor struct{}

// ProcessBlock simplifies word projections left once wide pairing is done:
// the low word of a 16-bit IMUL product, a 32-bit value shifted right by 16
// read as its low word, and an identity 0xffff mask on a word.
func (p *simplifyWordProjectionsProcessor) ProcessBlock(_ *Result, _ Func, b Block) (Block, bool) {
	rewriter := &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, childChanged := w.rewriteExprChildren(expr)
			if simplified, ok := simplifyWordProjection(next); ok {
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

// simplifyWordProjection rewrites one word projection:
//
//   - loword(a * b) of two 16-bit operands is C's int product a * b. The
//     machine models IMUL as a 32-bit DX:AX product so a paired high word can
//     rebuild a long multiply, as in LOWORD(11 * dyArial8) * 3.
//   - loword((uint32_t)(l >> 16)) is HIWORD(l), the expansion of the Windows
//     HIWORD macro, as in HIWORD(lParam).
//   - w & 0xffff on a word projection is w.
func simplifyWordProjection(expr Expr) (Expr, bool) {
	if and, ok := expr.(*Binary); ok && and.Op == OpAnd {
		if mask, ok := and.RHS.(*Const); ok && mask.U64 == 0xffff {
			if word, ok := and.LHS.(*Word); ok {
				return word, true
			}
		}
		return nil, false
	}
	word, ok := expr.(*Word)
	if !ok || word.Part != machine.WordLow {
		return nil, false
	}
	parent := word.Parent
	if cast, ok := parent.(*Cast); ok && exprWidth(cast) == 4 {
		parent = cast.Value
	}
	binary, ok := parent.(*Binary)
	if !ok {
		return nil, false
	}
	switch binary.Op {
	case OpMul:
		if exprWidth(binary) == 2 && exprWidth(binary.LHS) <= 2 && exprWidth(binary.RHS) <= 2 {
			return binary, true
		}
	case OpShr, OpSar:
		if shift, ok := binary.RHS.(*Const); ok && shift.U64 == 16 && exprWidth(binary.LHS) == 4 {
			return &Word{Parent: binary.LHS, Part: machine.WordHigh}, true
		}
	}
	return nil, false
}
