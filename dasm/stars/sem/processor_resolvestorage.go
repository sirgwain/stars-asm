package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type resolveStorageProcessor struct {
	ctx     *FuncContext
	instOff uint32
}

// ProcessBlock resolves raw memory references to semantic local/global lvalues.
func (p *resolveStorageProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	effects, changed := p.rewriter(result).rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// resolveCallWithRewriter resolves storage references in call arguments using
// an existing tree rewriter.
func (p *resolveStorageProcessor) resolveCallWithRewriter(w *semRewriter, call *Call) (*Call, bool) {
	args, changed := w.rewriteExprs(call.Args)
	if !changed {
		return call, false
	}
	next := *call
	next.Args = args
	return &next, true
}

// rewriter returns the semantic tree rewrite for storage resolution.
func (p *resolveStorageProcessor) rewriter(result *Result) *semRewriter {
	return &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			prevInstOff := p.instOff
			p.instOff = effect.EffectMeta().InstOff
			next, changed := w.rewriteEffectChildren(effect)
			p.instOff = prevInstOff
			return next, changed, true
		},
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			next, changed := p.resolveCallWithRewriter(w, call)
			return next, changed, true
		},
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, changed := w.rewriteExprChildren(expr)
			if ptr, ok := next.(*FarPointer); ok {
				if resolved, ok := p.ctx.resolveSemanticFarPointer(ptr); ok {
					return resolved, true, true
				}
			}
			return next, changed, true
		},
		lvalue: func(w *semRewriter, value LValue) (LValue, bool, bool) {
			next, changed, handled := p.resolveLValueWithRewriter(w, result, value)
			return next, changed, handled
		},
	}
}

// resolveLValueWithRewriter resolves one lvalue using an existing tree rewriter.
func (p *resolveStorageProcessor) resolveLValueWithRewriter(w *semRewriter, result *Result, value LValue) (LValue, bool, bool) {
	switch v := value.(type) {
	case *RawMemory:
		if storage, ok := p.resolveStorage(v.Access); ok {
			return storage, true, true
		}
		memory, ok := func() (LValue, bool) {
			var mem *Memory = unresolvedMemory(p.ctx, result, v.Access)
			return p.resolveMemoryWithRewriter(w, mem)
		}()
		if !ok {
			return value, false, true
		}
		return memory, true, true
	case *Memory:
		next, changed := p.resolveMemoryWithRewriter(w, v)
		if memory, ok := next.(*Memory); ok {
			if storage, ok := p.ctx.resolveSemanticMemory(memory); ok {
				return storage, true, true
			}
		}
		return next, changed, true
	case *SymbolRef:
		return value, false, true
	case *Part:
		next, changed := w.rewriteLValueChildren(v)
		part, ok := next.(*Part)
		if !ok {
			return next, true, true
		}
		if resolved, ok := p.resolveSymbolPart(part); ok {
			return resolved, true, true
		}
		return next, changed, true
	case *Deref:
		next, changed := w.rewriteLValueChildren(v)
		deref, ok := next.(*Deref)
		if !ok {
			return next, true, true
		}
		if resolved, ok := p.resolveSymbolDeref(deref); ok {
			return resolved, true, true
		}
		return next, changed, true
	default:
		return value, false, false
	}
}

// resolveSymbolPart resolves a byte range from an already symbolic base path.
func (p *resolveStorageProcessor) resolveSymbolPart(part *Part) (LValue, bool) {
	base, ok := part.Base.(*SymbolRef)
	if !ok {
		return nil, false
	}
	if typeinfo.IsPointer(base.Path.Type()) && part.ByteOff < base.Path.Type().Bytes() {
		return nil, false
	}
	if path, ok := p.ctx.res.ResolveFieldPathLoadInContext(base.Path, part.ByteOff, part.Width, p.ctx.unionContext()); ok {
		return &SymbolRef{Path: path}, true
	}
	path, offLeft, ok := p.ctx.res.ResolveFieldPathInContext(base.Path, part.ByteOff, p.ctx.unionContext())
	if !ok {
		return nil, false
	}
	if offLeft == 0 && part.Width == path.Type().Bytes() {
		return &SymbolRef{Path: path}, true
	}
	if offLeft+part.Width > path.Type().Bytes() {
		return nil, false
	}
	return &Part{
		Base:     &SymbolRef{Path: path},
		ByteOff:  offLeft,
		Width:    part.Width,
		TypeInfo: part.TypeInfo,
	}, true
}

// resolveSymbolDeref resolves a byte range dereferenced from a symbolic pointer.
func (p *resolveStorageProcessor) resolveSymbolDeref(deref *Deref) (LValue, bool) {
	path, ok := symbolPathForExpr(deref.Pointer)
	if !ok {
		return nil, false
	}
	if field, ok := p.ctx.res.ResolveFieldPathLoadInContext(path, deref.ByteOff, deref.Width, p.ctx.unionContext()); ok {
		return &SymbolRef{Path: field}, true
	}
	field, offLeft, ok := p.ctx.res.ResolveFieldPathInContext(path, deref.ByteOff, p.ctx.unionContext())
	if !ok {
		return nil, false
	}
	if offLeft == 0 && deref.Width == field.Type().Bytes() {
		return &SymbolRef{Path: field}, true
	}
	if offLeft+deref.Width > field.Type().Bytes() {
		return nil, false
	}
	return &Part{
		Base:     &SymbolRef{Path: field},
		ByteOff:  offLeft,
		Width:    deref.Width,
		TypeInfo: deref.TypeInfo,
	}, true
}

// resolveMemoryWithRewriter resolves memory address expressions with an
// existing tree rewriter.
func (p *resolveStorageProcessor) resolveMemoryWithRewriter(w *semRewriter, mem *Memory) (LValue, bool) {
	seg, segChanged := w.rewriteExpr(mem.Seg)
	base, baseChanged := w.rewriteExpr(mem.Base)
	index, indexChanged := w.rewriteExpr(mem.Index)
	if !segChanged && !baseChanged && !indexChanged {
		return mem, false
	}
	next := *mem
	next.Seg = seg
	next.Base = base
	next.Index = index
	return &next, true
}

// resolveStorage resolves direct raw memory to local or global storage.
func (p *resolveStorageProcessor) resolveStorage(mem machine.MemoryAccess) (LValue, bool) {
	lvalue, ok := p.ctx.resolveMachineStorage(mem, mem.Width)
	if !ok {
		return nil, false
	}
	return lvalue, true
}

// lvalueForLocalAccess returns the best semantic lvalue for a local access.
func lvalueForLocalAccess(access symresolve.LocalAccess, width int) LValue {
	base := &Local{FunctionVar: access.Local}
	if access.FieldOff == 0 && width == access.Local.Type.Bytes() {
		return base
	}
	return &Part{Base: base, ByteOff: access.FieldOff, Width: width, TypeInfo: intTypeForWidth(width)}
}

// lvalueForGlobalAccess returns the best semantic lvalue for a global access.
func lvalueForGlobalAccess(access symresolve.GlobalAccess, width int) LValue {
	base := &Global{GlobalVar: access.Global}
	if access.FieldOff == 0 && width == access.Global.Type.Bytes() {
		return base
	}
	return &Part{Base: base, ByteOff: access.FieldOff, Width: width, TypeInfo: intTypeForWidth(width)}
}

// isAggregateType reports whether a type is storage containing subobjects.
func isAggregateType(typ typeinfo.Type) bool {
	if typ == nil {
		return false
	}
	switch typ.Kind() {
	case typeinfo.KStruct, typeinfo.KUnion, typeinfo.KArray:
		return true
	default:
		return false
	}
}
