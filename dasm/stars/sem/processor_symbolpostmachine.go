package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type symbolDebugPostMachineProcessor struct {
	ctx *FuncContext
}

// ProcessMachineBlock goes through all machine blocks and attempts to resolve symbols
// this should be run after all machine processing, and is jsut for debugging
func (p *symbolDebugPostMachineProcessor) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	p.rewriter(result).rewriteMachineEffects(b.Effects)
	return b, false
}

// rewriter returns the machine tree visitor for storage annotation.
func (p *symbolDebugPostMachineProcessor) rewriter(result *Result) *machineRewriter {
	return &machineRewriter{
		effect: func(mr *machineRewriter, effect machine.Effect) (machine.Effect, bool, bool) {
			switch e := effect.(type) {
			case machine.CallEffect:
				if e.Target == nil {
					// skip CALLF [bp+0x6] style calls
					return effect, false, false
				}
				for i, arg := range e.Args {
					if len(e.Target.Params) <= i {
						// var arg
						break
					}
					_ = arg
					if isSymResolvable(arg) {
						typ := e.Target.Params[i].Type
						if messageType := messageCallArgumentType(p.ctx.sdb, e.Target, e.Args, i); messageType != nil {
							typ = messageType
						}
						if expr, ok := p.resolvedExpr(arg, typ, result); ok {
							p.ctx.log.Debug("resolved expression", "expr", FormatExpr(expr), "arg", arg)
						} else {
							p.ctx.log.Warn("expression failed to resolve", "arg", arg, "typ", typ)
						}
					}
				}
			case machine.ReturnEffect:
				if !typeinfo.IsPointer(p.ctx.fs.Ret) {
					break
				}
				if isSymResolvable(e.Value) {
					if expr, ok := p.resolvedExpr(e.Value, p.ctx.fs.Ret, result); ok {
						p.ctx.log.Debug("resolved expression", "expr", FormatExpr(expr), "ret", e.Value)
					} else {
						p.ctx.log.Warn("expression failed to resolve", "ret", e.Value)
					}
				}
			}
			return effect, false, false
		},
		memory: func(w *machineRewriter, mem machine.MemoryAddress) (machine.MemoryAddress, bool, bool) {
			next, changed := w.rewriteMachineMemoryChildren(mem)
			return next, changed, true
		},
	}
}

// resolvedExpr converts a typed machine value recursively and reports whether
// any raw machine value or unresolved memory remains in the semantic tree.
func (p *symbolDebugPostMachineProcessor) resolvedExpr(value machine.Value, expected typeinfo.Type, result *Result) (Expr, bool) {
	expr := (&machineConverter{ctx: p.ctx, result: result}).convertValueTyped(value, expected)
	resolved := true
	walkExpr(expr, func(candidate Expr) {
		switch candidate.(type) {
		case *Memory, *RawValue:
			resolved = false
		}
	})
	return expr, resolved
}

func isSymResolvable(value machine.Value) bool {
	if _, ok := value.(*machine.CallResult); ok {
		return false
	}
	if _, ok := value.(*machine.PhiValue); ok {
		return false
	}
	return true
}
