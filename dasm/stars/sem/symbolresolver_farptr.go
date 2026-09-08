package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// symbolFromFarPointerWords resolves a segment/offset word pair without
// reconstructing a whole far-pointer value.
func (sr *symbolResolver) symbolFromFarPointerWords(words *machine.StackWords, expected typeinfo.Type) (symresolve.SymbolPath, bool) {
	if !typeinfo.IsFarPointer(expected) || words == nil || len(words.Words) != 2 {
		return nil, false
	}
	return sr.symbolFromFarPointerWordPair(words.Words[0], words.Words[1], expected)
}

// symbolFromFarPointerWordPair resolves a segment/offset typed far pointer value.
func (sr *symbolResolver) symbolFromFarPointerWordPair(segment machine.Value, offset machine.Value, expected typeinfo.Type) (symresolve.SymbolPath, bool) {
	if off, ok := offset.(*machine.Const); ok && off.Val == 0 {
		if seg, ok := segment.(*machine.Const); ok && seg.Val == 0 {
			return &symresolve.SymbolConst{Const: off, Typ: expected}, true
		}
	}
	if seg, ok := segment.(*machine.Reg); ok {
		segNum := sr.segFromRegister(seg.Val)
		if seg.Val == asm.RegSS {
			if addr, ok := offset.(*machine.Address); ok {
				return sr.symbolFromAddressAddress(addr.Addr)
			}
			return sr.symbolFromValueTyped(offset, expected)
		}
		if seg.Val == asm.RegDS || seg.Val == asm.RegCS {
			if off, ok := offset.(*machine.Const); ok {
				if sym, ok := sr.globalSymbol(segNum, uint32(off.Val), 4); ok {
					return sym, true
				}
				if seg.Val == asm.RegDS {
					if literal, ok := sr.res.ResolveLiteral(segNum, uint32(off.Val)); ok {
						return &symresolve.SymbolLiteral{Literal: literal, Typ: expected}, true
					}
				}
			}
			if sym, ok := sr.symbolFromValue(offset); ok {
				expectedPtr, expectedOK := expected.(*typeinfo.Pointer)
				symbolPtr, symbolOK := sym.Type().(*typeinfo.Pointer)
				if expectedOK && symbolOK && expectedPtr.Elem != nil && symbolPtr.Elem != nil &&
					typeinfo.IsCallCompatible(expectedPtr.Elem, symbolPtr.Elem) {
					return sym, true
				}
			}
			if ptr, ok := expected.(*typeinfo.Pointer); ok && ptr.Elem != nil {
				if sym, ok := sr.decompose(segNum, ptr.Elem.Bytes(), offset, 0); ok {
					return sym, true
				}
			}
		}
	}
	if seg, ok := segment.(*machine.Const); ok {
		if off, ok := offset.(*machine.Const); ok && typeinfo.IsFunctionPointer(expected) {
			segNum := uint16(seg.Val)
			if fx := seg.Fixup; fx != nil && fx.Source == asm.FixupSourceSegment {
				segNum = seg.Fixup.TargetSegNum
			}
			if fn := sr.sdb.GetFunctionByAddr(typeinfo.Addr{Seg: uint16(segNum), Off: uint32(off.Val)}); fn != nil {
				return &symresolve.SymbolLiteral{Literal: fn, Typ: expected}, true
			}
		}
	}
	if addr, ok := sr.addressFromSplitFarPointer(segment, offset, 0); ok {
		if sym, ok := sr.symbolPathFromAddressValue(addr, expected); ok {
			return sym, true
		}
	}
	seg, segOK := sr.symbolFromValue(segment)
	off, offOK := sr.symbolFromValue(offset)
	if segOK && offOK && commonSymbolRoot(seg, off) {
		return off, true
	}
	return nil, false
}

// symbolFromNativePointerMemory resolves memory addressed by a collapsed
// machine pointer load, such as [load(dword [bp+lppl])+field].
func (sr *symbolResolver) symbolFromNativePointerMemory(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {
	if mem.Seg != nil || mem.Index != nil {
		return nil, false
	}
	base, ok := sr.symbolFromValue(mem.Base)
	if !ok || !typeinfo.IsPointer(base.Type()) {
		return nil, false
	}
	if path, ok := sr.symbolFromResolvedAccess(base, mem.Disp, mem.Width); ok {
		return path, true
	}
	if mem.Disp != 0 {
		return &symresolve.SymbolOffset{Base: base, Offset: mem.Disp, Result: base.Type()}, true
	}
	return nil, false
}

// addressFromSplitFarPointer normalizes an address whose segment and offset
// words are represented as separate symbolic loads from the same far pointer.
func (sr *symbolResolver) addressFromSplitFarPointer(segment machine.Value, offset machine.Value, disp int) (resolvedAddress, bool) {
	seg, ok := sr.symbolFromValue(segment)
	if !ok {
		return resolvedAddress{}, false
	}

	root, rootOff, ok := symbolOffsetRoot(seg)
	if !ok || rootOff != 2 {
		return resolvedAddress{}, false
	}

	fixed, terms, ok := sr.splitFarPointerOffset(root, offset, disp)
	if !ok {
		return resolvedAddress{}, false
	}

	return resolvedAddress{
		base:   root,
		offset: fixed,
		terms:  terms,
		deref:  true,
	}, true
}

// splitFarPointerOffset separates the pointer storage's low word from fixed
// and dynamic byte offsets applied to the pointed-to address.
func (sr *symbolResolver) splitFarPointerOffset(
	root symresolve.SymbolPath,
	value machine.Value,
	disp int,
) (int, []resolvedAddressTerm, bool) {
	fixed := disp
	addTerms := collectAddTerms(value)

	var remaining []machine.Value
	foundLowWord := false

	for _, term := range addTerms {
		if c, ok := term.(*machine.Const); ok {
			fixed += int(c.Val)
			continue
		}

		sym, ok := sr.symbolFromValue(term)
		if ok {
			termRoot, termOff, ok := symbolOffsetRoot(sym)
			if ok &&
				termOff == 0 &&
				symresolve.Equals(root, termRoot) {

				foundLowWord = true
				continue
			}
		}

		remaining = append(remaining, term)
	}

	if !foundLowWord {
		return 0, nil, false
	}

	terms := make([]resolvedAddressTerm, 0, len(remaining))

	for _, value := range remaining {
		value = stripLowWord(value)

		index, scale := sr.decomposeTerm(value)
		if index == nil {
			index = value
			scale = 1
		}

		terms = append(terms, resolvedAddressTerm{
			value: index,
			scale: scale,
		})
	}

	return fixed, terms, true
}

// symbolOffsetRoot returns the root and byte offset for a symbol offset path.
func symbolOffsetRoot(path symresolve.SymbolPath) (symresolve.SymbolPath, int, bool) {
	if off, ok := path.(*symresolve.SymbolOffset); ok {
		return off.Base, off.Offset, true
	}
	return path, 0, true
}

// symbolFromFarPointer builds a symbol from a far ptr. Simple farptrs are like
// farseg(load(dword [bp+0x6])):[faroff(load(dword [bp+0x6]))+0x64].
func (sr *symbolResolver) symbolFromFarPointer(seg *machine.FarPointer, off machine.Value, disp int, width int) (symresolve.SymbolPath, bool) {
	parent, ok := commonFarPointerParent(seg, off)
	if !ok {
		return nil, false
	}

	base, ok := sr.symbolFromValue(parent)
	if !ok {
		return nil, false
	}

	if disp == 0 {
		return base, true
	}
	if path, ok := sr.symbolFromResolvedAccess(base, disp, width); ok {
		return path, true
	}

	return &symresolve.SymbolOffset{Base: base, Offset: disp, Result: base.Type()}, true
}

func commonFarPointerParent(seg *machine.FarPointer, off machine.Value) (machine.Value, bool) {
	if seg.Part != machine.FarPointerSegment {
		return nil, false
	}

	offPtr, ok := off.(*machine.FarPointer)
	if !ok || offPtr.Part != machine.FarPointerOffset {
		return nil, false
	}

	if !machine.ValueEquals(seg.Parent, offPtr.Parent) {
		return nil, false
	}

	return seg.Parent, true
}

// commonSymbolRoot returns true if high and low both have the same SymbolOffset
// root and high is +0x2 offset
func commonSymbolRoot(high, low symresolve.SymbolPath) bool {
	symLow, ok := low.(*symresolve.SymbolOffset)
	if !ok {
		return false
	}

	symHigh, ok := high.(*symresolve.SymbolOffset)
	if !ok {
		return false
	}

	return symresolve.Equals(symLow.Base, symHigh.Base) &&
		symHigh.Offset-symLow.Offset == 2
}
