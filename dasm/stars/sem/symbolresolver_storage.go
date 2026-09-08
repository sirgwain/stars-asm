package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// resolvedStorageLane identifies the declared object containing one resolved
// machine lane and the lane's byte offset within that object.
type resolvedStorageLane struct {
	object symresolve.SymbolPath
	offset int
	size   int
}

// adjacentResolvedStorage reports whether high is the next 16-bit lane after
// low within the same normalized storage address.
func (sr *symbolResolver) adjacentResolvedStorage(low machine.MemoryAddress, high machine.MemoryAddress) (resolvedAddress, bool) {
	if low.Width != 2 || high.Width != 2 {
		return resolvedAddress{}, false
	}

	lo, ok := sr.addressFromMemory(low, nil)
	if !ok {
		return resolvedAddress{}, false
	}
	hi, ok := sr.addressFromMemory(high, nil)
	if !ok {
		return resolvedAddress{}, false
	}
	lo = physicalResolvedAddress(lo)
	hi = physicalResolvedAddress(hi)
	if !sr.sameResolvedAddressBase(lo, hi) || resolvedAddressPhysicalOffset(hi) != resolvedAddressPhysicalOffset(lo)+2 {
		return resolvedAddress{}, false
	}
	return lo, true
}

// physicalResolvedAddress folds typed field projections into the fixed byte
// offset used for storage comparison without changing the address's exact path.
func physicalResolvedAddress(addr resolvedAddress) resolvedAddress {
	for {
		switch base := addr.base.(type) {
		case *symresolve.SymbolOffset:
			addr.offset += base.Offset
			addr.base = base.Base
		case *symresolve.SymbolField:
			if base.Field == nil {
				return addr
			}
			addr.offset += base.Field.Offset
			addr.base = base.Base
		case *symresolve.SymbolBitfield:
			if base.Field == nil {
				return addr
			}
			addr.offset += base.Field.Offset
			addr.base = base.Base
		default:
			return addr
		}
	}
}

// storageLaneFromResolvedAddress retains field and indexed-element identity
// while peeling only byte offsets within that declared storage object.
func (sr *symbolResolver) storageLaneFromResolvedAddress(addr resolvedAddress) (resolvedStorageLane, bool) {
	path := addr.exact
	offset := 0
	for path != nil {
		part, ok := path.(*symresolve.SymbolOffset)
		if !ok {
			break
		}
		offset += part.Offset
		path = part.Base
	}
	if len(addr.terms) != 0 && !symbolPathContainsTerm(path) {
		path = nil
		offset = 0
	}
	if path == nil {
		path = addr.base
		offset = addr.offset
		if path == nil {
			return resolvedStorageLane{}, false
		}
		if len(addr.terms) == 1 {
			term := addr.terms[0]
			result := indexedTermResult(path.Type(), term.scale)
			if result == nil {
				return resolvedStorageLane{}, false
			}
			indexed := &symresolve.SymbolTerm{Base: path, Scale: term.scale, Result: result}
			if index, ok := sr.symbolFromAddressTermValue(term.value); ok {
				indexed.Index = index
			} else {
				indexed.IndexVal = term.value
			}
			path = indexed
		} else if len(addr.terms) != 0 {
			return resolvedStorageLane{}, false
		}
	}
	for path != nil {
		part, ok := path.(*symresolve.SymbolOffset)
		if !ok {
			break
		}
		offset += part.Offset
		path = part.Base
	}
	if deref, ok := path.(*symresolve.SymbolDeref); ok && offset >= 0 {
		if elem, ok := typeinfo.UnwrapPointer(deref.Base.Type()); ok && elem.Bytes() > 0 && offset >= elem.Bytes() {
			path = &symresolve.SymbolTerm{
				Base:     deref.Base,
				IndexVal: machine.ConstVal(uint(offset / elem.Bytes())),
				Scale:    elem.Bytes(),
				Result:   elem,
			}
			offset %= elem.Bytes()
		}
	}
	typ := path.Type()
	if typ == nil || typ.Bytes() <= 0 {
		return resolvedStorageLane{}, false
	}
	return resolvedStorageLane{object: path, offset: offset, size: typ.Bytes()}, true
}

// storageLaneFromMemory recovers the pointed-to object for split far-pointer
// addresses before deriving the typed lane from the normalized address.
func (sr *symbolResolver) storageLaneFromMemory(mem machine.MemoryAddress, addr resolvedAddress) (resolvedStorageLane, bool) {
	if path, ok := sr.memoryPath(mem); ok {
		projected := addr
		projected.exact = path
		if lane, ok := sr.storageLaneFromResolvedAddress(projected); ok {
			_, pointerRoot := lane.object.(*symresolve.SymbolRoot)
			if !pointerRoot || !typeinfo.IsPointer(lane.object.Type()) {
				return lane, true
			}
		}
	}

	lane, laneOK := sr.storageLaneFromResolvedAddress(addr)
	if laneOK {
		_, pointerRoot := lane.object.(*symresolve.SymbolRoot)
		if !pointerRoot || !typeinfo.IsPointer(lane.object.Type()) {
			return lane, true
		}
	}
	segment, segmentOK := mem.Seg.(*machine.FarPointer)
	offset, offsetOK := mem.Base.(*machine.FarPointer)
	if segmentOK && offsetOK &&
		segment.Part == machine.FarPointerSegment && offset.Part == machine.FarPointerOffset &&
		sr.sameResolvedMachineValue(segment.Parent, offset.Parent) {
		base, ok := sr.symbolFromValue(offset.Parent)
		if ok && typeinfo.IsPointer(base.Type()) {
			resolved := resolvedAddress{
				base:   &symresolve.SymbolDeref{Base: base},
				offset: mem.Disp,
				deref:  true,
			}
			return sr.storageLaneFromResolvedAddress(resolved)
		}
	}
	return lane, laneOK
}

// sameResolvedStorageObject compares typed storage paths while allowing
// independently loaded indexes that resolve to the same symbolic value.
func (sr *symbolResolver) sameResolvedStorageObject(a symresolve.SymbolPath, b symresolve.SymbolPath) bool {
	if symresolve.Equals(a, b) {
		return true
	}
	aTerm, aOK := a.(*symresolve.SymbolTerm)
	bTerm, bOK := b.(*symresolve.SymbolTerm)
	if !aOK || !bOK || aTerm.Scale != bTerm.Scale || !symresolve.Equals(aTerm.Base, bTerm.Base) {
		return false
	}
	if aTerm.Index != nil || bTerm.Index != nil {
		return aTerm.Index != nil && bTerm.Index != nil && symresolve.Equals(aTerm.Index, bTerm.Index)
	}
	return sr.sameResolvedAddressTermValue(aTerm.IndexVal, bTerm.IndexVal)
}

// sameResolvedAddressBase reports whether two normalized addresses differ
// only in their fixed byte offsets and typed exact projections.
func (sr *symbolResolver) sameResolvedAddressBase(a resolvedAddress, b resolvedAddress) bool {
	if a.deref != b.deref {
		return false
	}

	switch {
	case a.base != nil || b.base != nil:
		if a.base == nil || b.base == nil {
			return false
		}
		aScratch, aIsScratch := a.base.(*symresolve.SymbolScratch)
		bScratch, bIsScratch := b.base.(*symresolve.SymbolScratch)
		if aIsScratch || bIsScratch {
			if !aIsScratch || !bIsScratch || aScratch.Function != bScratch.Function {
				return false
			}
		} else if !symresolve.Equals(a.base, b.base) {
			return false
		}
	case a.baseValue != nil || b.baseValue != nil:
		if a.baseValue == nil || b.baseValue == nil || !machine.ValueEquals(a.baseValue, b.baseValue) {
			return false
		}
	}

	return sr.sameResolvedAddressTerms(a.terms, b.terms)
}

// resolvedAddressPhysicalOffset includes a scratch symbol's BP displacement
// in the normalized byte offset used for lane adjacency.
func resolvedAddressPhysicalOffset(addr resolvedAddress) int {
	if scratch, ok := addr.base.(*symresolve.SymbolScratch); ok {
		return addr.offset + scratch.BPOffset
	}
	return addr.offset
}

// sameResolvedAddressTerms compares normalized dynamic address terms without
// depending on the source expression's commutative addition order.
func (sr *symbolResolver) sameResolvedAddressTerms(a []resolvedAddressTerm, b []resolvedAddressTerm) bool {
	if len(a) != len(b) {
		return false
	}
	matched := make([]bool, len(b))
	for _, aTerm := range a {
		found := false
		for i, bTerm := range b {
			if !matched[i] && aTerm.scale == bTerm.scale && sr.sameResolvedAddressTermValue(aTerm.value, bTerm.value) {
				matched[i] = true
				found = true
				break
			}
		}
		if !found {
			return false
		}
	}
	return true
}

// sameResolvedAddressTermValue compares dynamic address terms structurally or,
// for distinct loads, by their resolver-proven symbolic storage identity.
func (sr *symbolResolver) sameResolvedAddressTermValue(a machine.Value, b machine.Value) bool {
	if machine.ValueEquals(a, b) {
		return true
	}
	aLoad, aOK := a.(*machine.Load)
	bLoad, bOK := b.(*machine.Load)
	if !aOK || !bOK || aLoad.Addr.Width != bLoad.Addr.Width {
		return false
	}
	aPath, aOK := sr.symbolFromValue(aLoad)
	bPath, bOK := sr.symbolFromValue(bLoad)
	return aOK && bOK && symresolve.Equals(aPath, bPath)
}

// sameResolvedMachineValue compares values structurally or treats distinct
// loads as equal when the resolver proves they read the same storage.
func (sr *symbolResolver) sameResolvedMachineValue(a machine.Value, b machine.Value) bool {
	if machine.ValueEquals(a, b) {
		return true
	}
	aLoad, aOK := a.(*machine.Load)
	bLoad, bOK := b.(*machine.Load)
	return aOK && bOK &&
		aLoad.Addr.Width == bLoad.Addr.Width &&
		sr.sameResolvedStorage(aLoad.Addr, bLoad.Addr)
}

// wideStorageDestination proves that adjacent word stores describe one
// declared object wide enough to receive a 32-bit access.
func (sr *symbolResolver) wideStorageDestination(low machine.MemoryAddress, high machine.MemoryAddress) (machine.MemoryAddress, bool) {
	lo, ok := sr.addressFromMemory(low, nil)
	if !ok {
		return machine.MemoryAddress{}, false
	}
	hi, ok := sr.addressFromMemory(high, nil)
	if !ok {
		return machine.MemoryAddress{}, false
	}
	physicalLo := physicalResolvedAddress(lo)
	physicalHi := physicalResolvedAddress(hi)
	if !sr.sameResolvedAddressBase(physicalLo, physicalHi) ||
		resolvedAddressPhysicalOffset(physicalHi) != resolvedAddressPhysicalOffset(physicalLo)+2 {
		return machine.MemoryAddress{}, false
	}
	loLane, ok := sr.storageLaneFromMemory(low, lo)
	typedObject := ok
	hiLane, ok := sr.storageLaneFromMemory(high, hi)
	directLow := low
	directLow.Index = nil
	_, directStorage := sr.varAccessFromMemory(directLow)
	_, pointerRoot := loLane.object.(*symresolve.SymbolRoot)
	typedObject = typedObject && ok &&
		sr.sameResolvedStorageObject(loLane.object, hiLane.object) &&
		hiLane.offset == loLane.offset+2 &&
		loLane.offset >= 0 && loLane.offset+4 <= loLane.size &&
		symbolPathHasDeclaredRoot(loLane.object) &&
		!(pointerRoot && typeinfo.IsPointer(loLane.object.Type()) && !directStorage)

	wide := low
	wide.Width = 4
	if !typedObject {
		resolved, ok := sr.addressFromMemory(wide, nil)
		if !ok {
			return machine.MemoryAddress{}, false
		}
		alias, ok := sr.storageLaneFromMemory(wide, resolved)
		if !ok || alias.offset != 0 || !symbolPathHasDeclaredRoot(alias.object) ||
			alias.object.Type() == nil || alias.object.Type().Bytes() != 4 ||
			isAggregateType(alias.object.Type()) ||
			(typeinfo.IsPointer(alias.object.Type()) && !directStorage) {
			return machine.MemoryAddress{}, false
		}
	}
	return wide, true
}

// sameResolvedStorage reports whether two memory accesses name the same physical storage.
func (sr *symbolResolver) sameResolvedStorage(a machine.MemoryAddress, b machine.MemoryAddress) bool {
	if sameStorage(a, b) {
		return true
	}
	aPath, aOK := sr.memoryPath(a)
	if !aOK {
		return false
	}
	bPath, bOK := sr.memoryPath(b)
	return bOK && symresolve.Equals(aPath, bPath)
}
