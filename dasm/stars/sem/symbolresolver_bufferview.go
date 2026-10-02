package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// bufferViewStruct returns the record struct a buffer_views rule gives
// variable v in this function: always, or in the current block, where the
// view's discriminator selects it.
func (sr *symbolResolver) bufferViewStruct(v typeinfo.Var) *typeinfo.Struct {
	if strct := sr.sdb.FunctionBufferView(sr.fs.Name, v); strct != nil {
		return strct
	}
	for view, strct := range sr.currentBufferViews {
		if view.Var(v) {
			return strct
		}
	}
	return nil
}

// pointerView returns a pointer variable viewed as the pointer type its
// accesses read through: the record struct a buffer_views rule gives it, or
// the pointer the current block's message carries in a window procedure's
// wParam or lParam. Other paths are returned unchanged.
func (sr *symbolResolver) pointerView(path symresolve.SymbolPath) symresolve.SymbolPath {
	root := path
	// a collapsed pointer load resolves to the pointer at offset zero
	if offset, ok := path.(*symresolve.SymbolOffset); ok && offset.Offset == 0 {
		root = offset.Base
	}
	if root, ok := root.(*symresolve.SymbolRoot); ok {
		if ptr, ok := root.Symbol.VarType().(*typeinfo.Pointer); ok {
			if strct := sr.bufferViewStruct(root.Symbol); strct != nil {
				return &symresolve.SymbolCast{Base: root, To: &typeinfo.Pointer{Elem: strct, Class: ptr.Class}}
			}
		}
	}
	return sr.messagePointerView(path)
}

// arrayBufferView returns a byte array variable viewed as a pointer to the
// record struct a buffer_views rule gives it, as the original's
// ((RTPLANET *)rgbCur), or nil when no rule covers it here.
func (sr *symbolResolver) arrayBufferView(v typeinfo.Var) symresolve.SymbolPath {
	if _, ok := v.VarType().(*typeinfo.Array); !ok {
		return nil
	}
	strct := sr.bufferViewStruct(v)
	if strct == nil {
		return nil
	}
	return &symresolve.SymbolCast{Base: &symresolve.SymbolRoot{Symbol: v}, To: &typeinfo.Pointer{Elem: strct}}
}

// bufferViewAddress rebases an address within a byte buffer that a
// buffer_views rule views as a record struct onto that view, so its offset
// selects the struct's fields and bitfields: an array, or the target of a
// pointer to the buffer.
func (sr *symbolResolver) bufferViewAddress(addr resolvedAddress) resolvedAddress {
	if root, ok := addr.base.(*symresolve.SymbolRoot); ok {
		if view := sr.arrayBufferView(root.Symbol); view != nil {
			addr.base = view
			addr.deref = true
			return addr
		}
	}
	// a pointer to the buffer, dereferenced through the view's pointer type
	if addr.deref && addr.base != nil {
		addr.base = sr.pointerView(addr.base)
	}
	return addr
}

// isBufferViewField reports whether a path resolved through a buffer view
// selects one of the viewed record's fields rather than the record itself.
func isBufferViewField(path symresolve.SymbolPath) bool {
	switch path.(type) {
	case *symresolve.SymbolCast, *symresolve.SymbolDeref:
		return false
	}
	return true
}

// bufferViewWhole returns the buffer variable a buffer view casts when target
// is the viewed record as a whole, as where the original took the buffer's
// address for pb - rgb, rather than one of its fields.
func (c *machineConverter) bufferViewWhole(target LValue) (Expr, bool) {
	path, ok := symbolPathForExpr(target)
	if !ok {
		return nil, false
	}
	if deref, ok := path.(*symresolve.SymbolDeref); ok {
		path = deref.Base
	}
	cast, ok := path.(*symresolve.SymbolCast)
	if !ok {
		return nil, false
	}
	root, ok := cast.Base.(*symresolve.SymbolRoot)
	if !ok || c.ctx.symbols.arrayBufferView(root.Symbol) == nil {
		return nil, false
	}
	return c.convertSymbolPath(root, root.Type())
}

// wholePointerView returns the variable a pointer view casts when path is
// that view as a whole, passed as a pointer value rather than read through,
// and the variable's own pointer type already suits expected, as with
// FreeLp(vlpbAiData, htMisc). Other paths are returned unchanged.
func wholePointerView(path symresolve.SymbolPath, expected typeinfo.Type) symresolve.SymbolPath {
	cast, ok := path.(*symresolve.SymbolCast)
	if !ok {
		return path
	}
	root, ok := cast.Base.(*symresolve.SymbolRoot)
	if !ok {
		return path
	}
	have, haveOK := root.Type().(*typeinfo.Pointer)
	want, wantOK := expected.(*typeinfo.Pointer)
	if !haveOK || !wantOK || have.Elem == nil || want.Elem == nil {
		return path
	}
	// any object pointer converts to void * without a cast
	if want.Elem.Kind() != typeinfo.KVoid && !typeinfo.IsCallCompatible(want.Elem, have.Elem) {
		return path
	}
	return root
}
