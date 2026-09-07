package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
)

type annotateProcessor struct {
	ctx *FuncContext
}

// ProcessMachineBlock records direct local/global annotations for machine memory.
func (p *annotateProcessor) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	p.rewriter(result).rewriteMachineEffects(b.Effects)
	return b, false
}

// rewriter returns the machine tree visitor for storage annotation.
func (p *annotateProcessor) rewriter(result *Result) *machineRewriter {
	return &machineRewriter{
		memory: func(w *machineRewriter, mem machine.MemoryAddress) (machine.MemoryAddress, bool, bool) {
			p.annotateMemory(result, mem)
			next, changed := w.rewriteMachineMemoryChildren(mem)
			return next, changed, true
		},
	}
}

// annotateMemory records an annotation for direct BP locals and direct globals.
func (p *annotateProcessor) annotateMemory(result *Result, mem machine.MemoryAddress) bool {
	access, ok := p.ctx.symbols.varAccessFromMemory(mem)
	if !ok {
		return false
	}
	switch v := access.(type) {
	case *symresolve.GlobalAccess:
		return result.annotateMemory(mem, globalAnnotation(*v))
	case *symresolve.LocalAccess:
		return result.annotateMemory(mem, localAnnotation(*v))
	default:
		return false
	}
}

// // annotationForAddressRoot creates an assembly annotation from root resolution.
// localAnnotation creates an assembly annotation for a resolved local or parameter.
func localAnnotation(l symresolve.LocalAccess) Annotation {
	local := l.Local
	path := symresolve.SymbolPath(&symresolve.SymbolRoot{Symbol: &local})
	if l.FieldOff != 0 {
		path = &symresolve.SymbolOffset{Base: path, Offset: l.FieldOff, Result: local.Type}
	}
	return Annotation{Kind: AnnotationLocal, Path: path, Var: &l, Text: l.String()}
}

// globalAnnotation creates an assembly annotation for a resolved global.
func globalAnnotation(g symresolve.GlobalAccess) Annotation {
	path := symresolve.SymbolPath(&symresolve.SymbolRoot{Symbol: g.Global})
	if g.FieldOff != 0 {
		path = &symresolve.SymbolOffset{Base: path, Offset: g.FieldOff, Result: g.Global.Type}
	}
	return Annotation{Kind: AnnotationGlobal, Path: path, Var: &g, Text: g.String()}
}

// sameAnnotation reports whether two annotations have the same render-relevant fields.
func (a Annotation) sameAnnotation(b Annotation) bool {
	return a.Kind == b.Kind && a.Text == b.Text
}
