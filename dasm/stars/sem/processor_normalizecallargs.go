package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

type normalizeCallArgsProcessor struct {
	ctx *FuncContext
}

// ProcessBlock normalizes semantically annotated call arguments in one block.
func (p *normalizeCallArgsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	rewriter := &semRewriter{
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
	effects, changed := rewriter.rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	next := b
	next.Effects = effects
	return next, true
}
