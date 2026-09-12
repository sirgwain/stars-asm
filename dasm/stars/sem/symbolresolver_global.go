package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// decompose decomposes a binary value into path parts
func (sr *symbolResolver) decompose(segNum uint16, width int, baseVal machine.Value, disp int) (symresolve.SymbolPath, bool) {
	var fixed = disp
	var other machine.Value

	// Flatten the complete additive address before resolving its absolute
	// portion. In particular, an address such as
	// 0x59a2 + (0xc0 * i) + 0xa0 must resolve 0x5a42 as an address within
	// rgplr, rather than resolving 0xa0 as an interior address in game.
	for _, term := range collectAddTerms(baseVal) {
		if c, ok := term.(*machine.Const); ok {
			fixed += int(c.Val)
			continue
		}
		if other != nil {
			return sr.decomposePointerBase(width, baseVal, disp)
		}
		other = term
	}

	// resolve base from either the binary const, or the mem.Disp if this wasn't a binary add index
	base, ok := sr.globalSymbol(segNum, uint32(fixed), width)
	if !ok && segNum == sr.segFromRegister(asm.RegCS) {
		base, ok = sr.globalSymbolByOffset(uint32(fixed), width)
	}
	if !ok {
		return sr.decomposePointerBase(width, baseVal, disp)
	}

	// figure out the root global and its offset (i.e. rglpfl+0x2)
	root, rootOff, _ := symbolOffsetRoot(base)

	// Keep the inner base normalized to offset 0.
	base = &symresolve.SymbolOffset{
		Base:   root,
		Offset: 0,
		Result: base.Type(),
	}

	other = stripLowWord(other)

	if index, scale := sr.decomposeTerm(other); index != nil {
		var term symresolve.SymbolPath

		if indexSymbol, ok := sr.symbolFromAddressTermValue(index); ok {
			term = &symresolve.SymbolTerm{
				Base:   base,
				Index:  indexSymbol,
				Scale:  scale,
				Result: indexedTermResult(base.Type(), scale),
			}
		} else {
			term = &symresolve.SymbolTerm{
				Base:     base,
				IndexVal: index,
				Scale:    scale,
				Result:   indexedTermResult(base.Type(), scale),
			}
		}

		return &symresolve.SymbolOffset{
			Base:   term,
			Offset: rootOff,
			Result: term.Type(),
		}, true
	}

	if other == nil {
		// For non-indexed globals, preserve the original resolved form.
		return &symresolve.SymbolOffset{
			Base:   root,
			Offset: rootOff,
			Result: base.Type(),
		}, true
	}

	term := &symresolve.SymbolTerm{
		Base:     base,
		IndexVal: other,
		Scale:    1,
		Result:   indexedTermResult(base.Type(), 1),
	}

	return &symresolve.SymbolOffset{
		Base:   term,
		Offset: rootOff,
		Result: term.Type(),
	}, true
}

// decomposePointerBase decomposes DS-relative memory that starts from a typed
// near-pointer value, such as load([bp-ptile]) + i*sizeof(TILE) + field.
func (sr *symbolResolver) decomposePointerBase(width int, baseVal machine.Value, disp int) (symresolve.SymbolPath, bool) {
	fixed := disp
	terms := collectAddTerms(baseVal)

	var base symresolve.SymbolPath
	var index machine.Value

	for _, term := range terms {
		if c, ok := term.(*machine.Const); ok {
			fixed += int(c.Val)
			continue
		}
		if sym, ok := sr.symbolFromValue(term); ok && pointerIndexElementType(sym.Type()) != nil {
			if base != nil {
				return nil, false
			}
			base = sym
			continue
		}
		if index != nil {
			return nil, false
		}
		index = term
	}

	if base == nil {
		return nil, false
	}

	if index != nil {
		indexVal := stripLowWord(index)
		indexBase, scale := sr.decomposeTerm(indexVal)
		if indexBase == nil {
			indexBase = indexVal
			scale = 1
		}

		// Only resolve an indexed field when we're describing an actual
		// memory access. An address-valued expression must preserve the
		// addressed object for later typed projection.
		if width > 0 {
			if term, ok := sr.symbolFromIndexedField(base, fixed, width, indexBase, scale); ok {
				return term, true
			}
		}

		term := &symresolve.SymbolTerm{
			Base:   base,
			Scale:  scale,
			Result: indexedTermResult(base.Type(), scale),
		}
		if indexSymbol, ok := sr.symbolFromAddressTermValue(indexBase); ok {
			term.Index = indexSymbol
		} else {
			term.IndexVal = indexBase
		}
		base = term
	}

	// width == 0 means this is an address value, not a load from that
	// address. Preserve the complete addressed object and residual byte
	// offset; typed call conversion can choose a subobject later.
	if width == 0 {
		if fixed == 0 {
			return base, true
		}
		return &symresolve.SymbolOffset{Base: base, Offset: fixed, Result: base.Type()}, true
	}

	if typeinfo.IsPointer(base.Type()) && fixed+width <= base.Type().Bytes() {
		return &symresolve.SymbolOffset{Base: base, Offset: fixed, Result: base.Type()}, true
	}

	if path, ok := sr.res.ResolveFieldPathLoadInContext(base, fixed, width, sr.unionContext()); ok {
		return path, true
	}

	if path, offLeft, ok := sr.res.ResolveFieldPathInContext(base, fixed, sr.unionContext()); ok {
		return &symresolve.SymbolOffset{Base: path, Offset: offLeft, Result: path.Type()}, true
	}

	return &symresolve.SymbolOffset{Base: base, Offset: fixed, Result: base.Type()}, true
}

func (sr *symbolResolver) decomposeTerm(value machine.Value) (machine.Value, int) {
	v, ok := value.(*machine.Binary)
	if !ok {
		return nil, 0
	}

	switch v.Op {
	case machine.ValueOpMul:
		if c, ok := v.LHS.(*machine.Const); ok {
			return v.RHS, int(c.Val)
		}
		if c, ok := v.RHS.(*machine.Const); ok {
			return v.LHS, int(c.Val)
		}
	case machine.ValueOpShl:
		shift, ok := v.RHS.(*machine.Const)
		if !ok || shift.Val >= 16 {
			return nil, 0
		}
		index, scale := sr.decomposeTerm(v.LHS)
		if index == nil {
			index = v.LHS
			scale = 1
		}
		return index, scale << shift.Val
	}
	return nil, 0
}

// globalSymbol resolves a seg/off to a global SymbolOffset
func (sr *symbolResolver) globalSymbol(segNum uint16, offset uint32, width int) (symresolve.SymbolPath, bool) {
	// we found a constant for this global, resolve it
	g, ok := sr.res.ResolveGlobal(segNum, offset)
	if !ok {
		return nil, false
	}

	return sr.symbolFromVarAccess(g.Global, g.FieldOff, width)
}

// globalAddressBase resolves a global address to its variable root and residual byte offset.
func (sr *symbolResolver) globalAddressBase(segNum uint16, offset uint32) (symresolve.SymbolPath, int, bool) {
	g, ok := sr.res.ResolveGlobal(segNum, offset)
	if !ok {
		return nil, 0, false
	}
	return &symresolve.SymbolRoot{Symbol: g.Global}, g.FieldOff, true
}

// flexibleGlobalAddressBase resolves an indexed address into the nearest
// preceding zero-length array global without treating unrelated constants as
// addresses into that open-ended storage.
func (sr *symbolResolver) flexibleGlobalAddressBase(segNum uint16, offset uint32) (symresolve.SymbolPath, int, bool) {
	var match *typeinfo.GlobalVar
	for _, global := range sr.sdb.Globals {
		if global == nil || global.Addr.Seg != segNum || global.Addr.Off > offset {
			continue
		}
		if match == nil || global.Addr.Off > match.Addr.Off {
			match = global
		}
	}
	if match == nil || !isZeroOrOneLengthArray(match.Type) {
		return nil, 0, false
	}
	return &symresolve.SymbolRoot{Symbol: match}, int(offset - match.Addr.Off), true
}

// globalAccessFromMemory resolves a direct machine memory address as a global access.
func (sr *symbolResolver) globalAccessFromMemory(mem machine.MemoryAddress) (symresolve.GlobalAccess, bool) {
	if mem.Index != nil {
		return symresolve.GlobalAccess{}, false
	}
	var seg uint16
	if segReg, ok := mem.Seg.(*machine.Reg); ok {
		seg = sr.segFromRegister(segReg.Val)
	}
	if seg == 0 {
		segConst, ok := mem.Seg.(*machine.Const)
		if !ok {
			return symresolve.GlobalAccess{}, false
		}
		seg = uint16(segConst.Val)
		if fx := segConst.Fixup; fx != nil &&
			fx.Source == asm.FixupSourceSegment &&
			fx.Target == asm.FixupTargetInternalRef {

			seg = fx.TargetSegNum
		}
	}
	off := int64(mem.Disp)
	if mem.Base != nil {
		base, ok := mem.Base.(*machine.Const)
		if !ok {
			return symresolve.GlobalAccess{}, false
		}
		off += int64(base.Val)
	}
	if off < 0 || off > int64(^uint32(0)) {
		return symresolve.GlobalAccess{}, false
	}
	return sr.res.ResolveGlobal(seg, uint32(off))
}

// varAccessFromMemory resolves direct local or global machine storage.
func (sr *symbolResolver) varAccessFromMemory(mem machine.MemoryAddress) (symresolve.VarAccess, bool) {
	if mem.Index != nil {
		return nil, false
	}
	if _, ok := mem.Base.(*machine.FrameBase); ok {
		local, ok := sr.res.ResolveLocal(sr.fs, sr.memoryInstOff(mem), mem.Disp)
		if !ok {
			return nil, false
		}
		return &local, true
	}
	global, ok := sr.globalAccessFromMemory(mem)
	if !ok {
		return nil, false
	}
	return &global, true
}

// addressBaseFromMemory resolves direct storage to its variable root and residual byte offset.
func (sr *symbolResolver) addressBaseFromMemory(mem machine.MemoryAddress) (symresolve.SymbolPath, int, bool) {
	access, ok := sr.varAccessFromMemory(mem)
	if !ok {
		return nil, 0, false
	}
	switch v := access.(type) {
	case *symresolve.GlobalAccess:
		return &symresolve.SymbolRoot{Symbol: v.Global}, v.FieldOff, true
	case *symresolve.LocalAccess:
		return &symresolve.SymbolRoot{Symbol: &v.Local}, v.FieldOff, true
	default:
		return nil, 0, false
	}
}

// globalSymbolByOffset resolves a unique global by offset when CS-relative data
// references carry the code segment instead of the recorded data segment.
func (sr *symbolResolver) globalSymbolByOffset(offset uint32, width int) (symresolve.SymbolPath, bool) {
	var match *typeinfo.GlobalVar
	var fieldOff int
	for _, global := range sr.sdb.Globals {
		if global == nil || global.Type == nil {
			continue
		}
		size := global.Type.Bytes()
		if size <= 0 {
			continue
		}
		start := global.Addr.Off
		end := start + uint32(size)
		if offset < start || offset >= end {
			continue
		}
		if match != nil {
			return nil, false
		}
		match = global
		fieldOff = int(offset - start)
	}
	if match == nil {
		return nil, false
	}
	return sr.symbolFromVarAccess(match, fieldOff, width)
}
