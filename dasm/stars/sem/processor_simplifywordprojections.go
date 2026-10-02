package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
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
//   - loword((uint32_t)x) of a value no wider than a word is x, as an
//     unsigned word: LOWORD((uint32_t)pplNew->idRoute) is pplNew->idRoute,
//     and a signed x is (uint16_t)x. A bitfield of at most 16 bits counts
//     as no wider than a word whatever its storage.
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
		if word, ok := wordSizedValue(parent); ok {
			return word, true
		}
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

// wordSizedValue returns x as the unsigned word LOWORD makes of it when x
// fits in a word: an integer no wider than 16 bits, or a bitfield of at
// most 16 bits. A signed x is cast to uint16_t.
func wordSizedValue(x Expr) (Expr, bool) {
	typ := x.ExprType()
	if field, ok := x.(*FieldAccess); ok && field.Field != nil && field.Field.Bitfield != nil {
		if field.Field.Bitfield.BitWidth > 16 {
			return nil, false
		}
		typ = field.Field.Bitfield.BaseType
	} else if width := exprWidth(x); width <= 0 || width > 2 {
		return nil, false
	}
	if !isSignedInt(typ) {
		return x, true
	}
	return &Cast{Value: x, To: typeinfo.U16.String(), TypeInfo: typeinfo.U16}, true
}
