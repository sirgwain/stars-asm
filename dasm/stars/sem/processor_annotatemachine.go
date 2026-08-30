package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

type annotateMachineStorageProcessor struct {
	ctx *FuncContext
}

// ProcessMachineBlock records direct local/global annotations for machine memory.
func (p *annotateMachineStorageProcessor) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	p.rewriter(result).rewriteMachineEffects(b.Effects)
	return b, false
}

// rewriter returns the machine tree visitor for storage annotation.
func (p *annotateMachineStorageProcessor) rewriter(result *Result) *machineRewriter {
	return &machineRewriter{
		memory: func(w *machineRewriter, mem machine.MemoryAccess) (machine.MemoryAccess, bool, bool) {
			p.annotateMemory(result, mem)
			next, changed := w.rewriteMachineMemoryChildren(mem)
			return next, changed, true
		},
	}
}

// annotateMemory records an annotation for direct BP locals and direct globals.
func (p *annotateMachineStorageProcessor) annotateMemory(result *Result, mem machine.MemoryAccess) bool {
	if mem.Index != nil {
		return false
	}
	if _, ok := mem.Base.(*machine.FrameBase); !ok {
		if global, ok := p.ctx.resolveGlobal(mem); ok {
			return result.annotateMemory(mem, globalAnnotation(global))
		}
		return false
	}

	local, ok := p.ctx.res.ResolveLocal(p.ctx.fs, mem.Origin.InstOff, mem.Disp)
	if !ok {
		return false
	}
	return result.annotateMemory(mem, localAnnotation(local))
}
