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
	rewrite := p.rewriter(result)
	effects := append([]Effect(nil), b.Effects...)
	changed := false
	prevUnionContext := p.ctx.currentUnionContext
	localUnionContext := prevUnionContext
	if localUnionContext != nil {
		localUnionContext = localUnionContext.Clone()
	}
	defer func() {
		p.ctx.currentUnionContext = prevUnionContext
	}()
	for i, effect := range effects {
		p.ctx.currentUnionContext = localUnionContext
		next, effectChanged := rewrite.rewriteEffect(effect)
		if effectChanged {
			effects[i] = next
			changed = true
		}
		if updated := p.unionContextAfterEffect(localUnionContext, effects[i]); updated != localUnionContext {
			localUnionContext = updated
			p.ctx.currentUnionContext = localUnionContext
		}
	}
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// unionContextAfterEffect updates straight-line union selections produced by one effect.
func (p *resolveStorageProcessor) unionContextAfterEffect(ctx *symresolve.UnionContext, effect Effect) *symresolve.UnionContext {
	assign, ok := effect.(*Assign)
	if !ok {
		return ctx
	}
	root, rule, value, ok := p.unionDiscriminatorAssign(assign)
	if !ok {
		return ctx
	}
	if ctx == nil {
		ctx = symresolve.NewUnionContext()
	}
	ctx.Add(root, rule, value)
	return ctx
}

// unionDiscriminatorAssign returns the union selection implied by a discriminator assignment.
func (p *resolveStorageProcessor) unionDiscriminatorAssign(assign *Assign) (symresolve.SymbolPath, *typeinfo.UnionVariantRule, typeinfo.EnumValue, bool) {
	value, ok := enumConstValue(assign.Src)
	if !ok || p.ctx == nil || p.ctx.sdb == nil || p.ctx.sdb.UnionRules == nil {
		return nil, nil, typeinfo.EnumValue{}, false
	}
	dst, ok := symbolPathForExpr(assign.Dst)
	if !ok {
		return nil, nil, typeinfo.EnumValue{}, false
	}
	root, fields, ok := splitSymbolFieldPath(dst)
	if !ok || len(fields) == 0 {
		return nil, nil, typeinfo.EnumValue{}, false
	}
	for i := 0; i < len(fields); i++ {
		candidate := rebuildSymbolFieldPath(root, fields[:i])
		rule, ok := p.ctx.sdb.UnionRules.UnionVariantForType(candidate.Type())
		if !ok || !fieldNamesMatch(fields[i:], rule.Discriminator) {
			continue
		}
		if _, ok := rule.MemberForValue(value.Value); !ok {
			return nil, nil, typeinfo.EnumValue{}, false
		}
		return candidate, rule, value, true
	}
	return nil, nil, typeinfo.EnumValue{}, false
}

// splitSymbolFieldPath separates a symbolic field path into its root and fields.
func splitSymbolFieldPath(path symresolve.SymbolPath) (symresolve.SymbolPath, []*typeinfo.StructField, bool) {
	field, ok := path.(*symresolve.SymbolField)
	if !ok {
		if _, ok := path.(*symresolve.SymbolRoot); ok {
			return path, nil, true
		}
		return nil, nil, false
	}
	root, fields, ok := splitSymbolFieldPath(field.Base)
	if !ok {
		return nil, nil, false
	}
	return root, append(fields, field.Field), true
}

// rebuildSymbolFieldPath rebuilds a symbolic path from root and a field prefix.
func rebuildSymbolFieldPath(root symresolve.SymbolPath, fields []*typeinfo.StructField) symresolve.SymbolPath {
	path := root
	for _, field := range fields {
		path = &symresolve.SymbolField{Base: path, Field: field}
	}
	return path
}

// fieldNamesMatch reports whether fields exactly match a configured path.
func fieldNamesMatch(fields []*typeinfo.StructField, names []string) bool {
	if len(fields) != len(names) {
		return false
	}
	for i, field := range fields {
		if field == nil || field.Name != names[i] {
			return false
		}
	}
	return true
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
			if addr, ok := expr.(*AddressOf); ok {
				if target, ok := p.resolveAddressOfTarget(addr.Target); ok {
					next := *addr
					next.Target = target
					return &next, true, true
				}
			}
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

// resolveAddressOfTarget resolves a symbolic byte range as an address target.
func (p *resolveStorageProcessor) resolveAddressOfTarget(target LValue) (LValue, bool) {
	part, ok := target.(*Part)
	if !ok {
		return nil, false
	}
	base, ok := symbolPathForExpr(part.Base)
	if !ok {
		return nil, false
	}
	if typeinfo.IsPointer(base.Type()) && part.ByteOff < base.Type().Bytes() {
		return nil, false
	}
	path, offLeft, ok := p.ctx.res.ResolveContainingFieldPathInContext(base, part.ByteOff, p.ctx.unionContext())
	if !ok || offLeft != 0 || part.Width > path.Type().Bytes() {
		return nil, false
	}
	return &SymbolRef{Path: path}, true
}

// resolveSymbolPart resolves a byte range from an already symbolic base path.
func (p *resolveStorageProcessor) resolveSymbolPart(part *Part) (LValue, bool) {
	base, ok := symbolPathForExpr(part.Base)
	if !ok {
		return nil, false
	}
	if typeinfo.IsPointer(base.Type()) && part.ByteOff < base.Type().Bytes() {
		return nil, false
	}
	if path, ok := p.ctx.res.ResolveFieldPathLoadInContext(base, part.ByteOff, part.Width, p.ctx.unionContext()); ok {
		return &SymbolRef{Path: path}, true
	}
	path, offLeft, ok := p.ctx.res.ResolveFieldPathInContext(base, part.ByteOff, p.ctx.unionContext())
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
