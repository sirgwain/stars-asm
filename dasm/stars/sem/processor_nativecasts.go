package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

// nativeCastsProcessor adds the casts a native Win32 compile needs where a
// value passes into an argument, assignment or return of a type it only
// converted to implicitly under the original compiler. It runs after
// addresses, fields and constants have their final types.
type nativeCastsProcessor struct {
	ctx *FuncContext
}

// ProcessBlock adds native casts to one semantic block.
func (p *nativeCastsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	effects, changed := p.rewriter().rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// rewriter casts assignment sources to their destination's type, return
// values to the function's return type, and call arguments to their
// parameter types.
func (p *nativeCastsProcessor) rewriter() *semRewriter {
	return &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			next, changed := w.rewriteEffectChildren(effect)
			switch e := next.(type) {
			case *Assign:
				if src := nativeAssignCast(e.Src, e.Dst.ExprType()); src != e.Src {
					cast := *e
					cast.Src = src
					return &cast, true, true
				}
			case *Return:
				if e.Value == nil || p.ctx.fs.Ret == nil {
					break
				}
				if value := nativeAssignCast(e.Value, p.ctx.fs.Ret); value != e.Value {
					cast := *e
					cast.Value = value
					return &cast, true, true
				}
			}
			return next, changed, true
		},
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			next, changed := w.rewriteCallChildren(call)
			if next.Function == nil {
				return next, changed, true
			}
			var args []Expr
			for i, arg := range next.Args {
				if i >= len(next.Params) {
					break
				}
				if cast := nativeArgCast(arg, &next.Params[i]); cast != arg {
					if args == nil {
						args = append([]Expr(nil), next.Args...)
					}
					args[i] = cast
				}
			}
			if args == nil {
				return next, changed, true
			}
			cast := *next
			cast.Args = args
			return &cast, true, true
		},
	}
}
