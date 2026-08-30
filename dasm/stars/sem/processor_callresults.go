package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

type collapseCallResultsProcessor struct {
	ctx *FuncContext
}

// ProcessBlock inlines single-use call results into later effects in the same block.
func (p *collapseCallResultsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	effects := append([]Effect(nil), b.Effects...)
	remove := make([]bool, len(effects))
	changed := false

	for i, effect := range effects {
		callEffect, ok := effect.(*CallEffect)
		if !ok || callEffect.Call == nil {
			continue
		}
		callResult, ok := callEffect.Result.(*CallResult)
		if !ok {
			continue
		}
		if countCallResultUses(effects[i+1:], callResult) != 1 {
			continue
		}

		replacement := &Call{Function: callEffect.Call.Function, Target: callEffect.Call.Target, Args: callEffect.Call.Args}
		rewriter := p.callResultRewriter(callResult, replacement)
		for j := i + 1; j < len(effects); j++ {
			next, ok := rewriter.rewriteEffect(effects[j])
			if !ok {
				continue
			}
			effects[j] = next
			remove[i] = true
			changed = true
			break
		}
	}

	if !changed {
		return b, false
	}
	out := make([]Effect, 0, len(effects))
	for i, effect := range effects {
		if remove[i] {
			continue
		}
		out = append(out, effect)
	}
	b.Effects = out
	return b, true
}

// countCallResultUses counts matching call result expressions in effects.
func countCallResultUses(effects []Effect, target *CallResult) int {
	uses := 0
	walker := &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			if result, ok := expr.(*CallResult); ok && sameExpr(result, target) {
				uses++
			}
			return expr, false, false
		},
	}
	for _, effect := range effects {
		walker.rewriteEffect(effect)
	}
	return uses
}

// callResultRewriter returns the semantic tree rewrite for one call result.
func (p *collapseCallResultsProcessor) callResultRewriter(target *CallResult, replacement Expr) *semRewriter {
	return &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			result, ok := expr.(*CallResult)
			if ok && sameExpr(result, target) {
				return replacement, true, true
			}
			return expr, false, false
		},
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			args, argsChanged := w.rewriteExprs(call.Args)
			next := *call
			next.Args = args
			collapsed, collapsedChanged := collapseTypedFarPointerCallArgs(p.ctx, &next)
			if !argsChanged && !collapsedChanged {
				return call, false, true
			}
			return collapsed, true, true
		},
	}
}
