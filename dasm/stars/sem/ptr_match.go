package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

// nearPointerMemoryLoad returns the pointer load behind a DS:offset memory address.
func nearPointerMemoryLoad(ds machine.Value, mem machine.MemoryAddress) (*machine.Load, bool) {
	if !machine.ValueEquals(mem.Seg, ds) || mem.Index != nil {
		return nil, false
	}
	load, ok := mem.Base.(*machine.Load)
	return load, ok
}

// farPointerMemoryLoad returns the source load behind a segment:offset memory address.
func farPointerMemoryLoad(mem machine.MemoryAddress) (*machine.Load, bool) {
	seg, ok := farPointerLoad(mem.Seg, machine.FarPointerSegment)
	if !ok {
		return nil, false
	}
	off, ok := farPointerLoad(mem.Base, machine.FarPointerOffset)
	if !ok {
		return nil, false
	}
	if !seg.Addr.Equals(off.Addr) {
		return nil, false
	}
	return seg, true
}

// farPointerLoad returns the load under a far pointer projection when it matches part.
func farPointerLoad(value machine.Value, part machine.FarPointerPart) (*machine.Load, bool) {
	ptr, ok := value.(*machine.FarPointer)
	if !ok || ptr.Part != part {
		return nil, false
	}
	load, ok := ptr.Parent.(*machine.Load)
	return load, ok
}
