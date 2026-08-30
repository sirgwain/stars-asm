package symresolve

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// Resolver owns symbolic query behavior over the loaded type/symbol facts.
// SymbolDB remains the source of truth for what exists; Resolver answers what
// a particular typed access or symbolic use means.
type Resolver struct {
	sdb *typeinfo.SymbolDB
	img *asm.ImageNE
}

type VarAccess interface {
	varAccess()
	String() string
}

// GlobalAccess defines an acces to a global
type GlobalAccess struct {
	Global   *typeinfo.GlobalVar
	Addr     typeinfo.Addr
	FieldOff int
}

func (g *GlobalAccess) varAccess() {}

func (g *GlobalAccess) String() string {
	if g.FieldOff == 0 {
		return g.Global.Name
	}
	return fmt.Sprintf("%s+0x%x", g.Global.Name, g.FieldOff)
}

type LocalAccess struct {
	Local    typeinfo.FunctionVar
	FieldOff int
}

func (g *LocalAccess) varAccess() {}

func (l *LocalAccess) String() string {
	if l.FieldOff == 0 {
		return l.Local.Name
	}
	return fmt.Sprintf("%s+0x%x", l.Local.Name, l.FieldOff)
}

func (ga GlobalAccess) Exact() bool { return ga.FieldOff == 0 }

func NewResolver(img *asm.ImageNE, sdb *typeinfo.SymbolDB) *Resolver {
	return &Resolver{sdb: sdb, img: img}
}

func (r *Resolver) ResolveGlobal(seg uint16, off uint32, width int) (GlobalAccess, bool) {
	addr := typeinfo.Addr{Seg: seg, Off: off}
	if global, fieldOff, ok := r.sdb.GetGlobalContaining(typeinfo.Addr{Seg: seg, Off: off}); ok {
		return GlobalAccess{Global: global, Addr: addr, FieldOff: fieldOff}, true
	}
	return GlobalAccess{}, false
}

func (r *Resolver) ResolveLiteral(seg uint16, off uint32) (string, bool) {
	return r.img.ReadCStringAt(seg, off)
}

func (r *Resolver) ResolveLocal(f *typeinfo.Function, off uint32, bpOff int) (LocalAccess, bool) {
	if off > 0 {
		// PLANET* lppl [BP+6] - 4 bytes wide is bp+6 to bp+10
		for _, p := range f.Params {
			if bpOff >= p.BPOffset && bpOff < p.BPOffset+p.Type.Bytes() {
				return LocalAccess{Local: p, FieldOff: bpOff - p.BPOffset}, true
			}
		}
	}

	// int32_t lPopOld [BP-32] - 4 bytes wide is bp-32 to bp-28
	for _, v := range f.Vars {
		if !f.InScope(v, off) {
			continue
		}
		if bpOff >= v.BPOffset && bpOff < v.BPOffset+v.Type.Bytes() {
			return LocalAccess{Local: v, FieldOff: bpOff - v.BPOffset}, true
		}
	}
	return LocalAccess{}, false
}

// ResolveFunction resolves a function from a fixup
// InternalRef CALLF 0x000a:0x7096 -> CalcPlanetMaxPop
// Ordinal CALLF KERNEL:FREEPROCINSTANCE -> CalcPlanetMaxPop
func (r *Resolver) ResolveFunction(fixup *asm.Fixup) (*typeinfo.Function, bool) {
	if fixup == nil {
		return nil, false
	}
	switch fixup.Target {
	case asm.FixupTargetInternalRef:
		f := r.sdb.GetFunctionByAddr(typeinfo.Addr{Seg: fixup.ResolvedSeg, Off: uint32(fixup.ResolvedOff)})
		return f, f != nil
	case asm.FixupTargetImportOrdinal:
		f := r.sdb.GetFunction(fixup.FuncName)
		return f, f != nil
	}
	return nil, false
}

// ResolveField resolves an offset and a target type from a struct returning the symbol path and any
// offset within that field
func (r *Resolver) ResolveField(v typeinfo.Var, off int) (SymbolPath, int, bool) {
	return r.ResolveFieldInContext(v, off, nil)
}

// ResolveFieldInContext resolves an offset using path-sensitive union context.
func (r *Resolver) ResolveFieldInContext(v typeinfo.Var, off int, ctx *UnionContext) (SymbolPath, int, bool) {
	t, _ := typeinfo.UnwrapPointer(v.VarType())
	s, ok := t.(*typeinfo.Struct)
	if !ok {
		return nil, 0, false
	}

	// see if we find a sub path
	root := &SymbolRoot{Symbol: v}
	f, offLeft := r.resolveField(root, s, off, ctx)
	if root == f {
		return nil, 0, false
	}
	return f, offLeft, true
}

// resolveField recursively builds field paths
// TODO: accessWidth support? How do we distinguish between pt and pt.x? Separate functions?
//
// we also have this 2 byte load that must be an int16_t, so we can find the type (or type equivalent?)
// LES       bx, [bp+lppl]             ; bx, [bp+0x6]
// PUSH      es:[bx]
// CALLF     CalcPlanetMaxPop          ; int32_t CalcPlanetMaxPop(int16_t idpl, int16_t iplr)
//
// but other times we don't know the type, just the accessWidth for compare
// LES       bx, [bp-lppl]             ; bx, [bp-0x1c]
// CMP       es:[bx+0x2], ax
func (r *Resolver) resolveField(base SymbolPath, s *typeinfo.Struct, off int, ctx *UnionContext) (SymbolPath, int) {
	matches := r.unionContextMatches(base, s, s.FieldsContainingOffset(off), ctx, true)
	if len(matches) == 1 {
		match := matches[0]
		field := &SymbolField{
			Base:  base,
			Field: match.Field,
		}

		// we found an exact match, return the field path
		if match.Off == 0 {
			return field, 0
		}

		// we have more offset to consume, check for child types like sel.pt.x
		childType, ok := match.Field.Type.(*typeinfo.Struct)
		if match.Off == 0 || !ok {
			return field, match.Off
		}

		child, childOff := r.resolveField(field, childType, match.Off, ctx)
		if child != field {
			return child, childOff
		}
	}

	return base, off
}

// ResolveFieldLoad resolves a MemoryAccess for a field load with a given accessWidth
// we don't know the type, just the accessWidth for compare
// LES       bx, [bp-lppl]             ; bx, [bp-0x1c]
// CMP       es:[bx+0x2], ax
//
// or a single byte load
// LES       bx, [vlpbdVCR]            ; bx, [0x531c]
// MOV       al, es:[bx+0x3]
func (r *Resolver) ResolveFieldLoad(v typeinfo.Var, off int, accessWidth int) (SymbolPath, bool) {
	return r.ResolveFieldLoadInContext(v, off, accessWidth, nil)
}

// ResolveFieldLoadInContext resolves a field load using path-sensitive union context.
func (r *Resolver) ResolveFieldLoadInContext(v typeinfo.Var, off int, accessWidth int, ctx *UnionContext) (SymbolPath, bool) {
	root := &SymbolRoot{Symbol: v}
	t, _ := typeinfo.UnwrapPointer(root.Type())
	s, ok := t.(*typeinfo.Struct)
	if !ok {
		return nil, false
	}
	return r.resolveFieldLoad(root, s, off, accessWidth, ctx)
}

// ResolveFieldPathLoad resolves a field load rooted at an existing symbol path.
func (r *Resolver) ResolveFieldPathLoad(base SymbolPath, off int, accessWidth int) (SymbolPath, bool) {
	return r.ResolveFieldPathLoadInContext(base, off, accessWidth, nil)
}

// ResolveFieldPathLoadInContext resolves a field path load using path-sensitive union context.
func (r *Resolver) ResolveFieldPathLoadInContext(base SymbolPath, off int, accessWidth int, ctx *UnionContext) (SymbolPath, bool) {
	t, _ := typeinfo.UnwrapPointer(base.Type())
	s, ok := t.(*typeinfo.Struct)
	if !ok {
		return nil, false
	}
	return r.resolveFieldLoad(base, s, off, accessWidth, ctx)
}

// ResolveFieldPath resolves an offset from an existing symbol path.
func (r *Resolver) ResolveFieldPath(base SymbolPath, off int) (SymbolPath, int, bool) {
	return r.ResolveFieldPathInContext(base, off, nil)
}

// ResolveFieldPathInContext resolves an offset from an existing path using union context.
func (r *Resolver) ResolveFieldPathInContext(base SymbolPath, off int, ctx *UnionContext) (SymbolPath, int, bool) {
	t, _ := typeinfo.UnwrapPointer(base.Type())
	s, ok := t.(*typeinfo.Struct)
	if !ok {
		return nil, 0, false
	}
	field, offLeft := r.resolveField(base, s, off, ctx)
	if field == base {
		return nil, 0, false
	}
	return field, offLeft, true
}

// ResolveBitfieldLoad resolves a shifted and masked field load from a root symbol.
func (r *Resolver) ResolveBitfieldLoad(v typeinfo.Var, off int, storageWidth int, bitOff int, bitWidth int) (SymbolPath, bool) {
	return r.ResolveBitfieldLoadInContext(v, off, storageWidth, bitOff, bitWidth, nil)
}

// ResolveBitfieldLoadInContext resolves a bitfield load using path-sensitive union context.
func (r *Resolver) ResolveBitfieldLoadInContext(v typeinfo.Var, off int, storageWidth int, bitOff int, bitWidth int, ctx *UnionContext) (SymbolPath, bool) {
	root := &SymbolRoot{Symbol: v}
	t, _ := typeinfo.UnwrapPointer(root.Type())
	s, ok := t.(*typeinfo.Struct)
	if !ok {
		return nil, false
	}
	return r.resolveBitfieldLoad(root, s, off, storageWidth, bitOff, bitWidth, ctx)
}

// ResolveBitfieldPathLoad resolves a shifted and masked field load from a symbol path.
func (r *Resolver) ResolveBitfieldPathLoad(base SymbolPath, off int, storageWidth int, bitOff int, bitWidth int) (SymbolPath, bool) {
	return r.ResolveBitfieldPathLoadInContext(base, off, storageWidth, bitOff, bitWidth, nil)
}

// ResolveBitfieldPathLoadInContext resolves a bitfield path load using union context.
func (r *Resolver) ResolveBitfieldPathLoadInContext(base SymbolPath, off int, storageWidth int, bitOff int, bitWidth int, ctx *UnionContext) (SymbolPath, bool) {
	t, _ := typeinfo.UnwrapPointer(base.Type())
	s, ok := t.(*typeinfo.Struct)
	if !ok {
		return nil, false
	}
	return r.resolveBitfieldLoad(base, s, off, storageWidth, bitOff, bitWidth, ctx)
}

func (r *Resolver) resolveFieldLoad(base SymbolPath, s *typeinfo.Struct, off int, accessWidth int, ctx *UnionContext) (SymbolPath, bool) {
	matches := r.unionContextMatches(base, s, s.FieldsContainingOffset(off), ctx, true)
	if field, ok := exactFieldLoad(base, matches, accessWidth); ok {
		return field, true
	}
	if len(matches) == 1 {
		match := matches[0]
		field := &SymbolField{
			Base:  base,
			Field: match.Field,
		}

		// we found an exact match, return the field path
		if match.Off == 0 && match.Field.Type.Bytes() == accessWidth {
			return field, true
		}

		// we have more offset to consume, check for child types like sel.pt.x
		childType, ok := match.Field.Type.(*typeinfo.Struct)
		if !ok {
			return nil, false
		}

		child, ok := r.resolveFieldLoad(field, childType, match.Off, accessWidth, ctx)
		if ok && child != field {
			return child, true
		}
	}

	return nil, false
}

// exactFieldLoad returns a unique exact non-bitfield field matching the access width.
func exactFieldLoad(base SymbolPath, matches []typeinfo.StructFieldMatch, accessWidth int) (SymbolPath, bool) {
	var out SymbolPath
	for _, match := range matches {
		if match.Off != 0 || match.Field.Bitfield != nil || match.Field.Type.Bytes() != accessWidth {
			continue
		}
		if out != nil {
			return nil, false
		}
		out = &SymbolField{Base: base, Field: match.Field}
	}
	return out, out != nil
}

// resolveBitfieldLoad recursively matches a bitfield by storage and bit range.
func (r *Resolver) resolveBitfieldLoad(base SymbolPath, s *typeinfo.Struct, off int, storageWidth int, bitOff int, bitWidth int, ctx *UnionContext) (SymbolPath, bool) {
	var out SymbolPath
	for _, match := range r.unionContextMatches(base, s, s.FieldsContainingOffset(off), ctx, true) {
		field := &SymbolField{
			Base:  base,
			Field: match.Field,
		}
		if match.Field.Bitfield != nil {
			fieldBitOff := match.Off*8 + bitOff
			if bitOff+bitWidth <= storageWidth*8 &&
				match.Off+storageWidth <= match.Field.Bitfield.StorageSize &&
				fieldBitOff == match.Field.Bitfield.BitOffset &&
				match.Field.Bitfield.BitWidth == bitWidth {
				if out != nil {
					return nil, false
				}
				out = field
				continue
			}
		}
		childType, ok := match.Field.Type.(*typeinfo.Struct)
		if match.Off == 0 || !ok {
			continue
		}
		child, ok := r.resolveBitfieldLoad(field, childType, match.Off, storageWidth, bitOff, bitWidth, ctx)
		if !ok {
			continue
		}
		if out != nil {
			return nil, false
		}
		out = child
	}
	return out, out != nil
}

// unionContextMatches narrows ambiguous union field matches using context.
func (r *Resolver) unionContextMatches(base SymbolPath, strct *typeinfo.Struct, matches []typeinfo.StructFieldMatch, ctx *UnionContext, useDefault bool) []typeinfo.StructFieldMatch {
	if len(matches) <= 1 {
		return matches
	}
	if ctx != nil {
		selection, ok := ctx.SelectionFor(base, strct)
		if ok {
			return selectUnionMemberMatch(matches, selection.Member)
		}
	}
	if !useDefault || ctx == nil {
		return matches
	}
	rule, ok := r.sdb.UnionRules.UnionVariantForType(strct)
	if !ok || rule.DefaultMember == nil {
		return matches
	}
	return selectUnionMemberMatch(matches, rule.DefaultMember)
}

// selectUnionMemberMatch narrows matches to the requested union member.
func selectUnionMemberMatch(matches []typeinfo.StructFieldMatch, member *typeinfo.StructField) []typeinfo.StructFieldMatch {
	for _, match := range matches {
		if match.Field == member {
			return []typeinfo.StructFieldMatch{match}
		}
	}
	return matches
}
