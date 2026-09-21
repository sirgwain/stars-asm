package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type collapseWideArithmeticProcessor struct{}

// ProcessBlock reconstructs carry-aware wide arithmetic expressions exposed
// after compiler scratch storage has been substituted.
func (p *collapseWideArithmeticProcessor) ProcessBlock(_ *Result, _ Func, b Block) (Block, bool) {
	effects, changed := p.rewriter().rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// rewriter returns the semantic expression rewriter for wide arithmetic.
func (p *collapseWideArithmeticProcessor) rewriter() *semRewriter {
	return &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, childChanged := w.rewriteExprChildren(expr)
			words, ok := next.(*Words)
			if !ok {
				return next, childChanged, true
			}
			collapsed, ok := collapseCarryAwareWideArithmetic(words)
			if !ok {
				return next, childChanged, true
			}
			return collapsed, true, true
		},
	}
}

// collapseCarryAwareWideArithmetic reconstructs a two-word ADD/ADC or SUB/SBB
// expression only when instruction provenance proves the carry relationship.
func collapseCarryAwareWideArithmetic(words *Words) (Expr, bool) {
	if len(words.Words) != 2 {
		return nil, false
	}
	high, highOK := words.Words[0].(*Binary)
	low, lowOK := words.Words[1].(*Binary)
	if !highOK || !lowOK || high.Op != low.Op {
		return nil, false
	}

	switch low.Op {
	case OpAdd:
		if !adjacentWideArithmeticInstructions(low.Producer, high.Producer, asm.OpADD, asm.OpADC) {
			return nil, false
		}
	case OpSub:
		if !adjacentWideArithmeticInstructions(low.Producer, high.Producer, asm.OpSUB, asm.OpSBB) {
			return nil, false
		}
	default:
		return nil, false
	}

	value, ok := collapseWideExprPair(high, low, words.ExprType())
	if !ok {
		return nil, false
	}
	arithmetic, ok := value.(*Binary)
	if !ok || arithmetic.LHS.ExprType() == nil || arithmetic.RHS.ExprType() == nil ||
		arithmetic.LHS.ExprType().Kind() != typeinfo.KInt || arithmetic.RHS.ExprType().Kind() != typeinfo.KInt ||
		arithmetic.LHS.ExprType().Bytes() != 4 || arithmetic.RHS.ExprType().Bytes() != 4 {
		return nil, false
	}
	return value, true
}
