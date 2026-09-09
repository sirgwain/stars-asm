package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// symbolPathFromAddressValue projects a normalized address-valued expression
// into a typed symbolic path.
func (sr *symbolResolver) symbolPathFromAddressValue(addr resolvedAddress, expected typeinfo.Type) (symresolve.SymbolPath, bool) {
	converter := &machineConverter{ctx: sr.FuncContext}
	semantic, ok := converter.semanticResolvedAddress(addr)
	if !ok {
		return nil, false
	}
	width := 0
	if ptr, ok := expected.(*typeinfo.Pointer); ok && ptr.Elem != nil {
		width = ptr.Elem.Bytes()
	}
	projected, ok := converter.consumeAddressProjection(semantic, width)
	if !ok {
		return nil, false
	}
	return symbolPathForExpr(projected)
}

// symbolFromAddressedVar resolves the symbol denoted by a variable-relative
// address, preserving the aggregate itself for offset zero.
func (sr *symbolResolver) symbolFromAddressedVar(v typeinfo.Var, fieldOff int) (symresolve.SymbolPath, bool) {
	root := symresolve.SymbolPath(&symresolve.SymbolRoot{Symbol: v})
	if fieldOff == 0 {
		return root, true
	}
	if path, offLeft, ok := sr.res.ResolveContainingFieldPathInContext(root, fieldOff, sr.unionContext()); ok {
		if offLeft == 0 {
			return path, true
		}
		return &symresolve.SymbolOffset{Base: path, Offset: offLeft, Result: path.Type()}, true
	}
	if path, _, ok := sr.res.ResolveFieldPathInContext(root, fieldOff, sr.unionContext()); ok {
		if field, ok := rootFieldPath(path); ok {
			offLeft := fieldOff - field.Field.Offset
			if offLeft == 0 {
				return field, true
			}
			return &symresolve.SymbolOffset{Base: field, Offset: offLeft, Result: field.Type()}, true
		}
	}
	return &symresolve.SymbolOffset{Base: root, Offset: fieldOff, Result: v.VarType()}, true
}

// rootFieldPath returns the first field selected below a root symbol path.
func rootFieldPath(path symresolve.SymbolPath) (*symresolve.SymbolField, bool) {
	for {
		field, ok := path.(*symresolve.SymbolField)
		if !ok {
			return nil, false
		}
		if _, ok := field.Base.(*symresolve.SymbolRoot); ok {
			return field, true
		}
		path = field.Base
	}
}

// symbolFromScratchMemoryAddress resolves an unresolved SS:BP access as a
// synthetic scratch symbol without classifying non-frame memory as scratch.
func (sr *symbolResolver) symbolFromScratchMemoryAddress(mem machine.MemoryAddress, expected typeinfo.Type) (symresolve.SymbolPath, bool) {
	if _, ok := mem.Base.(*machine.FrameBase); !ok || mem.Index != nil || mem.Width <= 0 {
		return nil, false
	}
	if mem.Seg != nil {
		seg, ok := mem.Seg.(*machine.Reg)
		if !ok || seg.Val != asm.RegSS {
			return nil, false
		}
	}

	typ := expected
	if typ == nil || typ.Bytes() != mem.Width {
		typ = scratchTypeForWidth(mem.Width)
	}
	return &symresolve.SymbolScratch{
		Function:    sr.fs.Addr,
		BPOffset:    mem.Disp,
		StorageSize: mem.Width,
		TypeInfo:    typ,
	}, true
}

// scratchTypeForWidth returns a neutral storage view, including x87 extended real.
func scratchTypeForWidth(width int) typeinfo.Type {
	switch width {
	case 1:
		return typeinfo.U8
	case 2:
		return typeinfo.U16
	case 4:
		return typeinfo.U32
	case 10:
		return typeinfo.F80
	default:
		return &typeinfo.Primitive{
			TypeKind: typeinfo.KInt,
			Name:     fmt.Sprintf("uint%d_t", width*8),
			Size:     width,
		}
	}
}

// symbolFromVarAccess resolves a byte range within a local or global variable
// into the most specific symbolic field path available.
func (sr *symbolResolver) symbolFromVarAccess(v typeinfo.Var, fieldOff int, width int) (symresolve.SymbolPath, bool) {
	root := symresolve.SymbolPath(&symresolve.SymbolRoot{Symbol: v})
	typ := v.VarType()
	if typeinfo.IsPointer(typ) && fieldOff+width <= typ.Bytes() {
		return &symresolve.SymbolOffset{Base: root, Offset: fieldOff, Result: typ}, true
	}
	if path, ok := sr.res.ResolveFieldPathLoadInContext(root, fieldOff, width, sr.unionContext()); ok {
		return path, true
	}
	if path, offLeft, ok := sr.res.ResolveFieldPathInContext(root, fieldOff, sr.unionContext()); ok {
		return &symresolve.SymbolOffset{Base: path, Offset: offLeft, Result: path.Type()}, true
	}
	return &symresolve.SymbolOffset{Base: root, Offset: fieldOff, Result: typ}, true
}

// symbolFromResolvedAccess resolves a byte range from an existing symbolic base
// as either a field load or an offset within pointer storage.
func (sr *symbolResolver) symbolFromResolvedAccess(base symresolve.SymbolPath, off int, width int) (symresolve.SymbolPath, bool) {
	if path, ok := sr.symbolFromFlexibleArrayAccess(base, off, width); ok {
		return path, true
	}
	if path, offLeft, ok := sr.res.ResolveFieldPathInContext(base, off, sr.unionContext()); ok {
		if typeinfo.IsPointer(path.Type()) && offLeft+width <= path.Type().Bytes() {
			return &symresolve.SymbolOffset{Base: path, Offset: offLeft, Result: path.Type()}, true
		}
		if offLeft == 0 {
			if field, ok := sr.res.ResolveFieldPathLoadInContext(base, off, width, sr.unionContext()); ok {
				return field, true
			}
			if path.Type().Bytes() == width {
				return path, true
			}
		}
		return &symresolve.SymbolOffset{Base: path, Offset: offLeft, Result: path.Type()}, true
	}
	if field, ok := sr.res.ResolveFieldPathLoadInContext(base, off, width, sr.unionContext()); ok {
		return field, true
	}
	return nil, false
}

// symbolFromFlexibleArrayAccess resolves accesses into trailing zero-length
// array fields such as PLPROD.rgprod[0].
func (sr *symbolResolver) symbolFromFlexibleArrayAccess(base symresolve.SymbolPath, off int, width int) (symresolve.SymbolPath, bool) {
	typ, _ := typeinfo.UnwrapPointer(base.Type())
	strct, ok := typ.(*typeinfo.Struct)
	if !ok {
		return nil, false
	}
	for i := range strct.Fields {
		field := &strct.Fields[i]
		array, ok := field.Type.(*typeinfo.Array)
		if !ok || array.Count != 0 || array.Elem == nil || off < field.Offset {
			continue
		}
		elemSize := array.Elem.Bytes()
		if elemSize <= 0 {
			continue
		}
		arrayOff := off - field.Offset
		elemOff := arrayOff % elemSize
		index := arrayOff / elemSize
		term := &symresolve.SymbolTerm{
			Base:   &symresolve.SymbolField{Base: base, Field: field},
			Index:  &symresolve.SymbolLiteral{Literal: index, Typ: typeinfo.I16},
			Scale:  elemSize,
			Result: array.Elem,
		}
		if elemOff == 0 && width == elemSize {
			return term, true
		}
		if path, ok := sr.symbolFromResolvedAccess(term, elemOff, width); ok {
			return path, true
		}
		return &symresolve.SymbolOffset{Base: term, Offset: elemOff, Result: array.Elem}, true
	}
	return nil, false
}

// symbolFromIndexedField resolves pointer + field offset + scaled index when
// the field itself is an indexed array.
func (sr *symbolResolver) symbolFromIndexedField(base symresolve.SymbolPath, fixed int, width int, index machine.Value, scale int) (symresolve.SymbolPath, bool) {
	field, offLeft, ok := sr.res.ResolveFieldPathInContext(base, fixed, sr.unionContext())
	if !ok || offLeft != 0 {
		return nil, false
	}
	array, ok := field.Type().(*typeinfo.Array)
	if !ok || array.Elem == nil || array.Elem.Bytes() != scale {
		return nil, false
	}
	term := &symresolve.SymbolTerm{
		Base:   field,
		Scale:  scale,
		Result: array.Elem,
	}
	if indexSymbol, ok := sr.symbolFromAddressTermValue(index); ok {
		term.Index = indexSymbol
	} else {
		term.IndexVal = index
	}
	if width == array.Elem.Bytes() {
		return term, true
	}
	return &symresolve.SymbolOffset{Base: term, Offset: 0, Result: array.Elem}, true
}

// pointerIndexElementType returns the element type reached by indexing a
// pointer or array symbol type.
func pointerIndexElementType(typ typeinfo.Type) typeinfo.Type {
	switch t := typ.(type) {
	case *typeinfo.Pointer:
		return t.Elem
	case *typeinfo.Array:
		return t.Elem
	default:
		return nil
	}
}

// indexedTermResult returns the element type produced by a scaled index.
func indexedTermResult(typ typeinfo.Type, scale int) typeinfo.Type {
	switch t := typ.(type) {
	case *typeinfo.Pointer:
		if t.Elem != nil && t.Elem.Bytes() == scale {
			return t.Elem
		}
	case *typeinfo.Array:
		if t.Elem != nil && t.Elem.Bytes() == scale {
			return t.Elem
		}
	}
	return nil
}

// symbolPathContainsTerm reports whether a typed path retains the indexed
// element that distinguishes it from its containing array.
func symbolPathContainsTerm(path symresolve.SymbolPath) bool {
	switch path := path.(type) {
	case *symresolve.SymbolTerm:
		return true
	case *symresolve.SymbolOffset:
		return symbolPathContainsTerm(path.Base)
	case *symresolve.SymbolField:
		return symbolPathContainsTerm(path.Base)
	case *symresolve.SymbolBitfield:
		return symbolPathContainsTerm(path.Base)
	case *symresolve.SymbolDeref:
		return symbolPathContainsTerm(path.Base)
	default:
		return false
	}
}

// symbolPathHasDeclaredRoot reports whether a path ultimately refers to a
// declared local or global rather than synthetic scratch storage.
func symbolPathHasDeclaredRoot(path symresolve.SymbolPath) bool {
	switch path := path.(type) {
	case *symresolve.SymbolRoot:
		return true
	case *symresolve.SymbolField:
		return symbolPathHasDeclaredRoot(path.Base)
	case *symresolve.SymbolBitfield:
		return symbolPathHasDeclaredRoot(path.Base)
	case *symresolve.SymbolDeref:
		return symbolPathHasDeclaredRoot(path.Base)
	case *symresolve.SymbolOffset:
		return symbolPathHasDeclaredRoot(path.Base)
	case *symresolve.SymbolTerm:
		return symbolPathHasDeclaredRoot(path.Base)
	default:
		return false
	}
}
