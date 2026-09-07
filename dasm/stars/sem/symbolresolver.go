package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type symbolResolver struct {
	*FuncContext
}

// resolvedAddress is a normalized machine address awaiting semantic projection.
type resolvedAddress struct {
	base      symresolve.SymbolPath
	baseValue machine.Value
	offset    int
	terms     []resolvedAddressTerm
	deref     bool
	exact     symresolve.SymbolPath
}

// resolvedAddressTerm is one dynamic byte term in a normalized address.
type resolvedAddressTerm struct {
	value machine.Value
	scale int
}

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
			if index, ok := sr.symbolFromValue(term.value); ok {
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

func newSymbolResolver(ctx *FuncContext) *symbolResolver {
	return &symbolResolver{FuncContext: ctx}
}

// addressFromMemory resolves the symbolic base and residual terms of a machine memory address.
func (sr *symbolResolver) addressFromMemory(mem machine.MemoryAddress, expected typeinfo.Type) (resolvedAddress, bool) {
	withoutIndex := mem
	withoutIndex.Index = nil
	if path, fieldOff, ok := sr.addressBaseFromMemory(withoutIndex); ok {
		addr := resolvedAddress{base: path, offset: fieldOff - mem.Disp}
		return resolvedAddressWithExpectedType(sr.addressWithExactPath(mem, sr.addMemoryAddressTerms(addr, mem)), expected, mem.Width), true
	}
	if addr, ok := sr.addressFromNativePointerMemory(mem); ok {
		return resolvedAddressWithExpectedType(sr.addressWithExactPath(mem, addr), expected, mem.Width), true
	}
	if seg, ok := mem.Seg.(*machine.Reg); ok && (seg.Val == asm.RegDS || seg.Val == asm.RegCS) && mem.Base != nil {
		addr, ok := sr.addressFromValue(mem.Base, sr.segFromRegister(seg.Val))
		if ok {
			addr.deref = resolvedAddressIsPointer(addr)
			addr = sr.addMemoryAddressTerms(addr, mem)
			if addr.base != nil || addr.baseValue != nil {
				return resolvedAddressWithExpectedType(sr.addressWithExactPath(mem, addr), expected, mem.Width), true
			}
			path, fieldOff, ok := sr.globalAddressBase(sr.segFromRegister(seg.Val), uint32(uint16(addr.offset)))
			if !ok && len(addr.terms) > 0 {
				path, fieldOff, ok = sr.flexibleGlobalAddressBase(sr.segFromRegister(seg.Val), uint32(uint16(addr.offset)))
			}
			if ok {
				addr.base = path
				addr.offset = fieldOff
				return resolvedAddressWithExpectedType(sr.addressWithExactPath(mem, addr), expected, mem.Width), true
			}
		}
	}
	if path, ok := sr.exactMemoryPath(mem); ok {
		addr := resolvedAddressFromPath(path)
		addr.exact = path
		return resolvedAddressWithExpectedType(addr, expected, mem.Width), true
	}

	return resolvedAddress{}, false
}

// resolvedAddressWithExpectedType preserves the contextual type of direct
// scratch storage while leaving resolved locals, globals, and projections intact.
func resolvedAddressWithExpectedType(addr resolvedAddress, expected typeinfo.Type, width int) resolvedAddress {
	if expected == nil || expected.Bytes() != width {
		return addr
	}
	retype := func(path symresolve.SymbolPath) symresolve.SymbolPath {
		scratch, ok := path.(*symresolve.SymbolScratch)
		if !ok {
			return path
		}
		copy := *scratch
		copy.TypeInfo = expected
		return &copy
	}
	addr.base = retype(addr.base)
	addr.exact = retype(addr.exact)
	return addr
}

// addressWithExactPath attaches the resolver's exact typed interpretation for
// ambiguities that cannot be represented solely by residual address terms.
func (sr *symbolResolver) addressWithExactPath(mem machine.MemoryAddress, addr resolvedAddress) resolvedAddress {
	if path, ok := sr.exactMemoryPath(mem); ok {
		addr.exact = path
	}
	return addr
}

// path returns the resolver's finished symbolic path when one is available.
func (addr resolvedAddress) path() (symresolve.SymbolPath, bool) {
	if addr.exact != nil {
		return addr.exact, true
	}
	if addr.base != nil && addr.offset == 0 && len(addr.terms) == 0 {
		return addr.base, true
	}
	return nil, false
}

// memoryPath resolves a machine memory access through the normalized address interface.
func (sr *symbolResolver) memoryPath(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {
	addr, ok := sr.addressFromMemory(mem, nil)
	if !ok {
		return nil, false
	}
	return addr.path()
}

// resolvedAddressIsPointer reports whether a normalized base is a pointer value
// whose use in a memory address proves one dereference.
func resolvedAddressIsPointer(addr resolvedAddress) bool {
	if addr.base != nil {
		return typeinfo.IsPointer(addr.base.Type())
	}
	return addr.baseValue != nil && typeinfo.IsPointer(machineValueType(addr.baseValue))
}

// resolvedAddressFromPath separates unresolved byte offsets from a finished
// symbol path while preserving the typed field path used for projection.
func resolvedAddressFromPath(path symresolve.SymbolPath) resolvedAddress {
	addr := resolvedAddress{}
	for {
		offset, ok := path.(*symresolve.SymbolOffset)
		if !ok {
			addr.base = path
			return addr
		}
		addr.offset += offset.Offset
		path = offset.Base
	}
}

// addressFromNativePointerMemory resolves memory reached through a pointer-valued machine value.
func (sr *symbolResolver) addressFromNativePointerMemory(mem machine.MemoryAddress) (resolvedAddress, bool) {
	if mem.Base == nil {
		return resolvedAddress{}, false
	}
	if mem.Seg != nil {
		seg, ok := mem.Seg.(*machine.Reg)
		if !ok || seg.Val != asm.RegDS {
			return resolvedAddress{}, false
		}
	}
	if path, ok := sr.symbolFromValue(mem.Base); ok && typeinfo.IsPointer(path.Type()) {
		addr := sr.addMemoryAddressTerms(resolvedAddress{base: path, deref: true}, mem)
		// The base itself has already been consumed.
		if len(addr.terms) > 0 && machine.ValueEquals(addr.terms[0].value, mem.Base) {
			addr.terms = addr.terms[1:]
		}
		return addr, true
	}
	if typeinfo.IsPointer(machineValueType(mem.Base)) {
		addr := resolvedAddress{baseValue: mem.Base, offset: mem.Disp, deref: true}
		if mem.Index != nil {
			addr.terms = append(addr.terms, resolvedAddressTerm{value: mem.Index, scale: 1})
		}
		return addr, true
	}
	return resolvedAddress{}, false
}

// addressFromValue decomposes machine pointer arithmetic into a symbolic base and residual byte terms.
func (sr *symbolResolver) addressFromValue(value machine.Value, segNum uint16) (resolvedAddress, bool) {
	value = unwrapAddressWord(value)
	switch v := value.(type) {
	case nil:
		return resolvedAddress{}, false
	case *machine.Const:
		if segNum != 0 {
			if path, fieldOff, ok := sr.globalAddressBase(segNum, uint32(v.Val)); ok {
				return resolvedAddress{base: path, offset: fieldOff}, true
			}
		}
		return resolvedAddress{offset: signedWordOffset(v.Val)}, true
	case *machine.Address:
		path, ok := sr.symbolFromAddressAddress(v.Addr)
		if !ok {
			return resolvedAddress{}, false
		}
		return resolvedAddress{base: path}, true
	case *machine.Load:
		if path, ok := sr.symbolFromValue(v); ok && typeinfo.IsPointer(path.Type()) {
			return resolvedAddress{base: path}, true
		}
		return resolvedAddress{terms: []resolvedAddressTerm{{value: value, scale: 1}}}, true
	case *machine.CallResult:
		if typeinfo.IsPointer(v.Type) {
			return resolvedAddress{baseValue: v}, true
		}
		return resolvedAddress{terms: []resolvedAddressTerm{{value: value, scale: 1}}}, true
	case *machine.Binary:
		return sr.addressFromBinary(v, segNum)
	default:
		return resolvedAddress{terms: []resolvedAddressTerm{{value: value, scale: 1}}}, true
	}
}

// addressOffsetFromValue decomposes residual arithmetic without interpreting constants as globals.
func (sr *symbolResolver) addressOffsetFromValue(value machine.Value) resolvedAddress {
	value = unwrapAddressWord(value)
	switch v := value.(type) {
	case nil:
		return resolvedAddress{}
	case *machine.Const:
		return resolvedAddress{offset: signedWordOffset(v.Val)}
	case *machine.Binary:
		switch v.Op {
		case machine.ValueOpAdd, machine.ValueOpSub:
			lhs := sr.addressOffsetFromValue(v.LHS)
			rhs := sr.addressOffsetFromValue(v.RHS)
			if v.Op == machine.ValueOpSub {
				rhs.negate()
			}
			return mergeResolvedAddress(lhs, rhs)
		case machine.ValueOpMul:
			if factor, value, ok := constOperand(v.LHS, v.RHS); ok {
				return scaleResolvedAddress(sr.addressOffsetFromValue(value), int(factor.Val))
			}
		case machine.ValueOpShl:
			if shift, ok := v.RHS.(*machine.Const); ok && shift.Val < 16 {
				return scaleResolvedAddress(sr.addressOffsetFromValue(v.LHS), 1<<shift.Val)
			}
		}
	}
	return resolvedAddress{terms: []resolvedAddressTerm{{value: value, scale: 1}}}
}

// scaleResolvedAddress multiplies every fixed and dynamic component of a
// residual address expression by one constant factor.
func scaleResolvedAddress(addr resolvedAddress, factor int) resolvedAddress {
	addr.offset *= factor
	for i := range addr.terms {
		addr.terms[i].scale *= factor
	}
	return addr
}

// addressFromBinary decomposes one machine binary address expression.
func (sr *symbolResolver) addressFromBinary(value *machine.Binary, segNum uint16) (resolvedAddress, bool) {
	switch value.Op {
	case machine.ValueOpAdd, machine.ValueOpSub:
		lhs, lhsOK := sr.addressFromValue(value.LHS, segNum)
		rhs, rhsOK := sr.addressFromValue(value.RHS, segNum)
		if !lhsOK || !rhsOK {
			return resolvedAddress{}, false
		}

		// Once either side supplies a resolved base, constants on the other
		// side are displacements rather than alternate absolute addresses.
		if _, ok := value.RHS.(*machine.Const); ok && (lhs.hasBase() || value.Op == machine.ValueOpSub) {
			rhs = sr.addressOffsetFromValue(value.RHS)
		}
		if _, ok := value.LHS.(*machine.Const); ok && rhs.hasBase() && value.Op == machine.ValueOpAdd {
			lhs = sr.addressOffsetFromValue(value.LHS)
		}
		if lhs.hasBase() && rhs.hasBase() {
			return resolvedAddress{}, false
		}
		if value.Op == machine.ValueOpSub {
			rhs.negate()
		}
		return mergeResolvedAddress(lhs, rhs), true
	case machine.ValueOpMul:
		if term, ok := resolvedAddressTermFromMul(value.LHS, value.RHS); ok {
			return resolvedAddress{terms: []resolvedAddressTerm{term}}, true
		}
	case machine.ValueOpShl:
		if shift, ok := value.RHS.(*machine.Const); ok {
			return resolvedAddress{terms: []resolvedAddressTerm{{value: value.LHS, scale: 1 << shift.Val}}}, true
		}
	}
	return resolvedAddress{terms: []resolvedAddressTerm{{value: value, scale: 1}}}, true
}

// addMemoryAddressTerms adds displacement and index components to an address.
func (sr *symbolResolver) addMemoryAddressTerms(addr resolvedAddress, mem machine.MemoryAddress) resolvedAddress {
	addr.offset += mem.Disp
	if mem.Index != nil {
		index := sr.addressOffsetFromValue(mem.Index)
		addr.offset += index.offset
		addr.terms = append(addr.terms, index.terms...)
	}
	return addr
}

// hasBase reports whether an address already has a symbolic or value base.
func (addr resolvedAddress) hasBase() bool {
	return addr.base != nil || addr.baseValue != nil
}

// negate reverses the sign of every residual address component.
func (addr *resolvedAddress) negate() {
	addr.offset = -addr.offset
	for i := range addr.terms {
		addr.terms[i].scale = -addr.terms[i].scale
	}
}

// mergeResolvedAddress combines two normalized partial addresses.
func mergeResolvedAddress(a, b resolvedAddress) resolvedAddress {
	out := resolvedAddress{base: a.base, baseValue: a.baseValue, offset: a.offset + b.offset, deref: a.deref || b.deref}
	if !out.hasBase() {
		out.base = b.base
		out.baseValue = b.baseValue
	}
	out.terms = append(out.terms, a.terms...)
	out.terms = append(out.terms, b.terms...)
	return out
}

// resolvedAddressTermFromMul extracts x*N or N*x as a scaled byte term.
func resolvedAddressTermFromMul(a, b machine.Value) (resolvedAddressTerm, bool) {
	if constant, ok := a.(*machine.Const); ok {
		return resolvedAddressTerm{value: b, scale: int(constant.Val)}, true
	}
	if constant, ok := b.(*machine.Const); ok {
		return resolvedAddressTerm{value: a, scale: int(constant.Val)}, true
	}
	return resolvedAddressTerm{}, false
}

// unwrapAddressWord removes low-word projections around address arithmetic.
func unwrapAddressWord(value machine.Value) machine.Value {
	for {
		if word, ok := value.(*machine.WordValue); ok && word.Part == machine.WordLow {
			value = word.Parent
			continue
		}
		if cast, ok := value.(*machine.Cast); ok && cast.To != nil && cast.To.Bytes() > 2 {
			value = cast.Value
			continue
		}
		return value
	}
}

// signedWordOffset interprets word-sized constants as signed residual offsets.
func signedWordOffset(value uint) int {
	if value <= 0xffff && value&0x8000 != 0 {
		return int(int16(uint16(value)))
	}
	return int(value)
}

// symbolFromValueTyped resolves symbols from values based on their type
// i.e. (0x59a2 + loword((0xc0 * load([bp+iplr]) for PLAYER* will resolve to rgplr
func (sr *symbolResolver) symbolFromValueTyped(value machine.Value, expected typeinfo.Type) (symresolve.SymbolPath, bool) {
	ds := sr.segFromRegister(asm.RegDS)
	switch v := value.(type) {

	case *machine.StackWords:
		if sym, ok := sr.symbolFromFarPointerWords(v, expected); ok {
			return sym, true
		}
		// StackWords stores the high word first and low word second.
		// Combine two constant 16-bit words into one 32-bit value.
		// looks like
		// words(0x0, 0x61) for a 32 bit value
		// or a wide arg
		// words(load(ds:[0x22bc]), load(ds:[0x22ba])) typ=COLORREF -> crButtonFace

		high, highOk := sr.symbolFromValueTyped(v.Words[0], expected)
		low, lowOk := sr.symbolFromValueTyped(v.Words[1], expected)

		if highOk && lowOk {
			cHigh, highConst := high.(*symresolve.SymbolConst)
			cLow, lowConst := low.(*symresolve.SymbolConst)
			if highConst && lowConst {
				val := ((cHigh.Const.Val & 0xffff) << 16) |
					(cLow.Const.Val & 0xffff)

				return &symresolve.SymbolConst{
					Const: machine.ConstVal(val),
					Typ:   expected,
				}, true
			}

			// check for shared global/local
			if symLow, ok := low.(*symresolve.SymbolOffset); ok {
				if symHigh, ok := high.(*symresolve.SymbolOffset); ok &&
					symresolve.Equals(symLow.Base, symHigh.Base) &&
					symHigh.Offset-symLow.Offset == 2 {
					return low, true
				}
			}
		}

	case *machine.Const:
		// A typed pointer constant may itself be a resolvable address.
		if typeinfo.IsNearPointer(expected) {
			if g, ok := sr.globalSymbol(ds, uint32(v.Val), 2); ok {
				return g, true
			}
		}
		return &symresolve.SymbolConst{Const: v, Typ: expected}, true
	case *machine.Load:
		if addr, ok := sr.addressFromMemory(v.Addr, expected); ok {
			if path, ok := addr.path(); ok {
				return path, true
			}
		}
		return sr.symbolFromValue(v)
	case *machine.Address:
		return sr.symbolFromValue(v)
	case *machine.Binary:
		if typeinfo.IsNearPointer(expected) {
			// A resolved pointer-valued term is authoritative. Constants beside
			// it are byte displacements, even when their numeric value happens
			// to fall inside a DGROUP global.
			if path, ok := sr.decomposePointerBase(2, v, 0); ok {
				return path, true
			}
			return sr.decompose(ds, 2, v, 0)
		}
	}

	return nil, false
}

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
	if sym, ok := sr.symbolFromSplitFarPointerValue(segment, offset, expected); ok {
		return sym, true
	}
	seg, segOK := sr.symbolFromValue(segment)
	off, offOK := sr.symbolFromValue(offset)
	if segOK && offOK && commonSymbolRoot(seg, off) {
		return off, true
	}
	return nil, false
}

// symbolFromSplitFarPointerValue resolves a far-pointer value whose segment
// and offset words are loaded separately from the same pointer storage.
func (sr *symbolResolver) symbolFromSplitFarPointerValue(segment machine.Value, offset machine.Value, expected typeinfo.Type) (symresolve.SymbolPath, bool) {
	seg, ok := sr.symbolFromValue(segment)
	if !ok {
		return nil, false
	}
	root, rootOff, ok := symbolOffsetRoot(seg)
	if !ok || rootOff != 2 {
		return nil, false
	}

	base, fieldOff, ok := sr.symbolFromSplitFarPointerOffset(root, offset, 0)
	if !ok {
		return nil, false
	}
	if fieldOff == 0 {
		return base, true
	}

	ptr, ok := expected.(*typeinfo.Pointer)
	if !ok || ptr.Elem == nil {
		return nil, false
	}
	if field, ok := sr.symbolFromResolvedAccess(base, fieldOff, ptr.Elem.Bytes()); ok {
		return field, true
	}
	return nil, false
}

// symbolFromAddressAddress resolves an address expression without treating it as
// a load from that address.
func (sr *symbolResolver) symbolFromAddressAddress(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {
	if _, ok := mem.Base.(*machine.FrameBase); ok {
		l, ok := sr.res.ResolveLocal(sr.fs, sr.memoryInstOff(mem), mem.Disp)
		if !ok {
			return sr.symbolFromScratchMemoryAddress(mem, nil)
		}
		return sr.symbolFromAddressedVar(&l.Local, l.FieldOff)
	}

	g, ok := sr.globalAccessFromMemory(mem)
	if !ok {
		return nil, false
	}
	return sr.symbolFromAddressedVar(g.Global, g.FieldOff)
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

// exactMemoryPath resolves a memory access to its most specific typed path,
// including union selections and split far-pointer representations.
func (sr *symbolResolver) exactMemoryPath(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {

	if _, ok := mem.Base.(*machine.FrameBase); ok {
		// local bp+var
		l, ok := sr.res.ResolveLocal(sr.fs, sr.memoryInstOff(mem), mem.Disp)
		if !ok {
			return sr.symbolFromScratchMemoryAddress(mem, nil)
		}

		return sr.symbolFromVarAccess(&l.Local, l.FieldOff, mem.Width)
	}

	if ptr, ok := mem.Seg.(*machine.FarPointer); ok {
		// farseg(load(dword [bp+0x6])):[faroff(load(dword [bp+0x6]))+0x64]
		return sr.symbolFromFarPointer(ptr, mem.Base, mem.Disp, mem.Width)
	}

	// load(ds:[vrglpplAi+0x2]):[(load(ds:[vrglpplAi]) + (load([ipl]) * 0x4))]
	if sym, ok := sr.symbolFromSplitFarPointerMemory(mem); ok {
		return sym, true
	}
	if sym, ok := sr.symbolFromNativePointerMemory(mem); ok {
		return sym, true
	}
	if global, ok := sr.globalAccessFromMemory(mem); ok {
		return sr.symbolFromVarAccess(global.Global, global.FieldOff, mem.Width)
	}

	segReg, ok := mem.Seg.(*machine.Reg)
	if !ok {
		return nil, false
	}

	segNum := sr.segFromRegister(segReg.Val)
	if segNum != 0 {
		if mem.Base == nil {
			// some globals come as ds:[0x1234] where 0x1234 is the Disp
			// direct MOV ax, [0x1234] is like this
			if segReg.Val == asm.RegDS {
				if literal, ok := sr.res.ResolveLiteral(segNum, uint32(mem.Disp)); ok {
					return &symresolve.SymbolLiteral{Literal: literal, Typ: typeinfo.LpStr}, true
				}
			}
		}
		switch v := mem.Base.(type) {
		case *machine.Const:
			// some globals come as ds:[0x1234] or ds:[0x1234+0x2] where 0x1234 is a Const
			// copy coalescing is like this, as well as some [sel+0x2] style loads
			globalOff := uint32(v.Val + uint(mem.Disp))
			if segReg.Val == asm.RegDS {
				if literal, ok := sr.res.ResolveLiteral(segNum, globalOff); ok {
					return &symresolve.SymbolLiteral{Literal: literal, Typ: typeinfo.LpStr}, true
				}
			}
		case *machine.Load:
			// handle ds:[load([bp-pl])] or ds:[load([bp-pl])+0x2]
			if segReg.Val == asm.RegDS {
				if sym, ok := sr.symbolFromValue(mem.Base); ok {
					if !typeinfo.IsPointer(sym.Type()) {
						return nil, false
					}
					if path, ok := sr.symbolFromResolvedAccess(sym, mem.Disp, mem.Width); ok {
						return path, true
					}
					deref := &symresolve.SymbolDeref{
						Base: sym,
					}
					return &symresolve.SymbolOffset{
						Base:   deref,
						Offset: mem.Disp,
						Result: deref.Type(),
					}, true

				}
			}
		case *machine.Binary:
			return sr.decompose(segNum, mem.Width, v, mem.Disp)
		}

	}

	return nil, false
}

// memoryInstOff returns the instruction offset governing a memory access.
func (sr *symbolResolver) memoryInstOff(mem machine.MemoryAddress) uint32 {
	if mem.Origin.InstOff != 0 {
		return mem.Origin.InstOff
	}
	return sr.currentInstOff
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

// scratchTypeForWidth returns the neutral integer view for an untyped scratch access.
func scratchTypeForWidth(width int) typeinfo.Type {
	switch width {
	case 1:
		return typeinfo.U8
	case 2:
		return typeinfo.U16
	case 4:
		return typeinfo.U32
	default:
		return &typeinfo.Primitive{
			TypeKind: typeinfo.KInt,
			Name:     fmt.Sprintf("uint%d_t", width*8),
			Size:     width,
		}
	}
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

// symbolFromSplitFarPointerMemory resolves memory whose segment and offset
// words are represented as separate symbolic loads from the same far pointer.
func (sr *symbolResolver) symbolFromSplitFarPointerMemory(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {
	// Segment word: expect a load from the high word of a far pointer symbol,
	// like load([vrgtok+0x2]).
	seg, ok := sr.symbolFromValue(mem.Seg)
	if !ok {
		return nil, false
	}

	// Far pointer root: the segment word must resolve to root+2, leaving the
	// low word at root for the offset expression to match.
	root, rootOff, ok := symbolOffsetRoot(seg)
	if !ok || rootOff != 2 {
		return nil, false
	}

	// Offset word: expect root plus optional scaled index and memory
	// displacement, like load([vrgtok]) + loword(0x1d*viVCRFocus) + 0xf.
	base, off, ok := sr.symbolFromSplitFarPointerOffset(root, mem.Base, mem.Disp)
	if !ok {
		return nil, false
	}

	// Pointer storage: if the indexed base is itself a far pointer value, an
	// access within its 4-byte representation is loading pointer words rather
	// than dereferencing the pointed-to struct.
	if typeinfo.IsPointer(base.Type()) && off+mem.Width <= base.Type().Bytes() {
		return &symresolve.SymbolOffset{Base: base, Offset: off, Result: base.Type()}, true
	}

	// Target field: resolve the dereferenced pointer/index base and remaining
	// byte offset into a field path when the pointed-to type is known.
	if field, ok := sr.symbolFromResolvedAccess(base, off, mem.Width); ok {
		return field, true
	}
	return &symresolve.SymbolOffset{Base: base, Offset: off, Result: base.Type()}, true
}

// symbolFromSplitFarPointerOffset resolves the offset half of a split far
// pointer memory access and returns the typed aggregate base plus field offset.
func (sr *symbolResolver) symbolFromSplitFarPointerOffset(root symresolve.SymbolPath, value machine.Value, disp int) (symresolve.SymbolPath, int, bool) {
	// Pattern: split far pointer offset expressions look like
	// load([root]) + loword(scale*index) + fieldDisp.
	// Flatten the additions so each term can be classified independently.
	fixed := disp
	terms := collectAddTerms(value)
	remaining := make([]machine.Value, 0, len(terms))
	foundLowWord := false
	for _, term := range terms {
		// Constant terms are part of the final field byte offset.
		if c, ok := term.(*machine.Const); ok {
			fixed += int(c.Val)
			continue
		}

		// Low-word term: expect one term to resolve to the same root at +0,
		// pairing with the caller's segment word root+2.
		sym, ok := sr.symbolFromValue(term)
		if ok {
			termRoot, termOff, ok := symbolOffsetRoot(sym)
			if ok && termOff == 0 && symresolve.Equals(root, termRoot) {
				foundLowWord = true
				continue
			}
		}
		remaining = append(remaining, term)
	}
	if !foundLowWord {
		return nil, 0, false
	}

	// Direct dereference: root plus constants only, like load([root])+0xf.
	if len(remaining) == 0 {
		return root, fixed, true
	}

	// Indexed dereference: after removing root and constants, only one dynamic
	// scaled-index term is currently expected.
	if len(remaining) != 1 {
		return nil, 0, false
	}

	// Index term: accept loword(scale*index) or a raw index expression.
	indexVal := stripLowWord(remaining[0])
	index, scale := sr.decomposeTerm(indexVal)
	if index == nil {
		index = indexVal
		scale = 1
	}

	// Indexed base: build root[index] when the scale matches the pointed-to
	// element size, preserving the field byte offset separately.
	term := &symresolve.SymbolTerm{
		Base:   root,
		Scale:  scale,
		Result: indexedTermResult(root.Type(), scale),
	}
	if indexSymbol, ok := sr.symbolFromValue(index); ok {
		term.Index = indexSymbol
	} else {
		term.IndexVal = index
	}
	return term, fixed, true
}

// collectAddTerms recursively flattens additive machine values.
func collectAddTerms(value machine.Value) []machine.Value {
	if bin, ok := value.(*machine.Binary); ok && bin.Op == machine.ValueOpAdd {
		terms := collectAddTerms(bin.LHS)
		return append(terms, collectAddTerms(bin.RHS)...)
	}
	return []machine.Value{value}
}

// stripLowWord removes a low-word projection when matching address terms.
func stripLowWord(value machine.Value) machine.Value {
	if wv, ok := value.(*machine.WordValue); ok && wv.Part == machine.WordLow {
		return wv.Parent
	}
	return value
}

// symbolOffsetRoot returns the root and byte offset for a symbol offset path.
func symbolOffsetRoot(path symresolve.SymbolPath) (symresolve.SymbolPath, int, bool) {
	if off, ok := path.(*symresolve.SymbolOffset); ok {
		return off.Base, off.Offset, true
	}
	return path, 0, true
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

func (sr *symbolResolver) symbolFromValue(value machine.Value) (symresolve.SymbolPath, bool) {
	if bitfield, ok := sr.symbolFromBitfieldValue(value); ok {
		return bitfield, true
	}

	switch v := value.(type) {

	case *machine.Load:
		base, ok := sr.memoryPath(v.Addr)
		if !ok {
			return nil, false
		}

		return base, true
	case *machine.Address:
		return sr.symbolFromAddressAddress(v.Addr)
	}
	return nil, false
}

// symbolFromBitfieldValue resolves a machine mask and shift to a logical bitfield path.
func (sr *symbolResolver) symbolFromBitfieldValue(value machine.Value) (*symresolve.SymbolBitfield, bool) {
	load, bitOff, bitWidth, ok := sr.bitfieldExtract(value)
	if !ok {
		return nil, false
	}
	return sr.symbolFromBitfieldMemory(load.Addr, bitOff, bitWidth)
}

// symbolFromBitfieldStore resolves a destination-preserving masked write to a
// logical bitfield path and its unshifted source value.
func (sr *symbolResolver) symbolFromBitfieldStore(mem machine.MemoryAddress, value machine.Value) (*symresolve.SymbolBitfield, machine.Value, bool) {
	_, bitOff, bitWidth, stored, ok := bitfieldStore(mem, value, sr.sameResolvedStorage)
	if !ok {
		return nil, nil, false
	}
	field, ok := sr.symbolFromBitfieldMemory(mem, bitOff, bitWidth)
	if !ok {
		return nil, nil, false
	}
	return field, stored, true
}

// bitfieldExtract returns the storage load, bit offset, and bit width for a bitfield expression.
func (sr *symbolResolver) bitfieldExtract(value machine.Value) (*machine.Load, int, int, bool) {
	value = unwrapMachineBitfieldValue(value)
	and, ok := value.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return nil, 0, 0, false
	}
	mask, source, ok := constOperand(and.LHS, and.RHS)
	if !ok {
		return nil, 0, 0, false
	}
	bitWidth, ok := lowBitMaskWidth(mask.Val)
	if !ok {
		return nil, 0, 0, false
	}
	load, bitOff, ok := sr.shiftedLoad(source)
	if !ok {
		return nil, 0, 0, false
	}
	return load, bitOff, bitWidth, true
}

// bitfieldStore returns a bitfield write and its unshifted source using the
// supplied storage equivalence predicate for destination/source-load matching.
func bitfieldStore(mem machine.MemoryAddress, value machine.Value, same func(machine.MemoryAddress, machine.MemoryAddress) bool) (*machine.Load, int, int, machine.Value, bool) {
	load, keep, set, ok := bitfieldStoreParts(mem, value, same)
	if !ok {
		return nil, 0, 0, nil, false
	}
	fullMask, ok := bitMask(mem.Width * 8)
	if !ok {
		return nil, 0, 0, nil, false
	}
	changed := (^keep.Val) & fullMask
	bitOff, bitWidth, ok := contiguousMaskRange(changed)
	if !ok {
		return nil, 0, 0, nil, false
	}
	stored, ok := unshiftMachineBitfieldSet(set, bitOff, bitWidth, changed)
	if !ok {
		return nil, 0, 0, nil, false
	}
	return load, bitOff, bitWidth, stored, true
}

// bitfieldStoreParts separates the preserved destination load, keep mask, and
// inserted value from a compiler-generated read-modify-write expression.
func bitfieldStoreParts(mem machine.MemoryAddress, value machine.Value, same func(machine.MemoryAddress, machine.MemoryAddress) bool) (*machine.Load, *machine.Const, machine.Value, bool) {
	if or, ok := value.(*machine.Binary); ok && or.Op == machine.ValueOpOr {
		if load, keep, ok := bitfieldKeepMask(mem, or.LHS, same); ok {
			return load, keep, or.RHS, true
		}
		if load, keep, ok := bitfieldKeepMask(mem, or.RHS, same); ok {
			return load, keep, or.LHS, true
		}
		return nil, nil, nil, false
	}
	load, keep, ok := bitfieldKeepMask(mem, value, same)
	if !ok {
		return nil, nil, nil, false
	}
	return load, keep, machine.ConstVal(0), true
}

// bitfieldKeepMask returns the destination load and constant keep mask from an AND expression.
func bitfieldKeepMask(mem machine.MemoryAddress, value machine.Value, same func(machine.MemoryAddress, machine.MemoryAddress) bool) (*machine.Load, *machine.Const, bool) {
	and, ok := value.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return nil, nil, false
	}
	keep, keptSource, ok := constOperand(and.LHS, and.RHS)
	if !ok {
		return nil, nil, false
	}
	load, ok := keptSource.(*machine.Load)
	if !ok || !same(mem, load.Addr) {
		return nil, nil, false
	}
	return load, keep, true
}

// unshiftMachineBitfieldSet validates and removes the destination bit shift
// from the inserted portion of a machine bitfield store.
func unshiftMachineBitfieldSet(value machine.Value, bitOff int, bitWidth int, changed uint) (machine.Value, bool) {
	if c, ok := value.(*machine.Const); ok {
		if c.Val&^changed != 0 {
			return nil, false
		}
		fieldMask, ok := bitMask(bitWidth)
		if !ok {
			return nil, false
		}
		next := *c
		next.Val = (c.Val >> bitOff) & fieldMask
		return &next, true
	}
	for {
		cast, ok := value.(*machine.Cast)
		if !ok {
			break
		}
		value = cast.Value
	}

	source := value
	if bitOff != 0 {
		shift, ok := value.(*machine.Binary)
		if !ok || shift.Op != machine.ValueOpShl {
			return nil, false
		}
		amount, ok := shift.RHS.(*machine.Const)
		if !ok || int(amount.Val) != bitOff {
			return nil, false
		}
		source = shift.LHS
	}
	source = unwrapMachineBitfieldValue(source)
	if words, ok := source.(*machine.StackWords); ok && len(words.Words) == 2 {
		if high, ok := words.Words[0].(*machine.Const); ok && high.Val == 0 {
			source = words.Words[1]
		}
	}

	and, ok := source.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return nil, false
	}
	mask, unmasked, ok := constOperand(and.LHS, and.RHS)
	if !ok {
		return nil, false
	}
	width, ok := lowBitMaskWidth(mask.Val)
	if !ok || width != bitWidth {
		return nil, false
	}
	return unmasked, true
}

// shiftedLoad returns the load and right-shift amount for a bitfield source.
func (sr *symbolResolver) shiftedLoad(value machine.Value) (*machine.Load, int, bool) {
	value = unwrapMachineBitfieldValue(value)
	shift, ok := value.(*machine.Binary)
	if !ok || shift.Op != machine.ValueOpShr {
		load, ok := sr.machineBitfieldStorageLoad(value)
		return load, 0, ok
	}
	amount, source, ok := constOperand(shift.LHS, shift.RHS)
	if !ok || source != shift.LHS {
		return nil, 0, false
	}
	load, ok := sr.machineBitfieldStorageLoad(unwrapMachineBitfieldValue(source))
	if !ok {
		return nil, 0, false
	}
	return load, int(amount.Val), true
}

// unwrapMachineBitfieldValue removes representation-only wrappers around a
// machine bitfield expression or its backing storage value.
func unwrapMachineBitfieldValue(value machine.Value) machine.Value {
	for {
		switch v := value.(type) {
		case *machine.Cast:
			value = v.Value
		case *machine.WordValue:
			if v.Part != machine.WordLow {
				return value
			}
			value = v.Parent
		default:
			return value
		}
	}
}

// machineBitfieldStorageLoad returns the physical load selected by a
// bitfield expression, including an uncollapsed high/low word pair.
func (sr *symbolResolver) machineBitfieldStorageLoad(value machine.Value) (*machine.Load, bool) {
	if load, ok := value.(*machine.Load); ok {
		return load, true
	}
	words, ok := value.(*machine.StackWords)
	if !ok || len(words.Words) != 2 {
		return nil, false
	}
	wide, ok := (&wideMachineCollapser{ctx: sr.FuncContext}).pair(words.Words[1], words.Words[0])
	if !ok {
		return nil, false
	}
	load, ok := wide.(*machine.Load)
	return load, ok
}

// symbolFromBitfieldMemory resolves a physical memory bit range to its logical field.
func (sr *symbolResolver) symbolFromBitfieldMemory(mem machine.MemoryAddress, bitOff int, bitWidth int) (*symresolve.SymbolBitfield, bool) {
	base, fieldOff, ok := sr.bitfieldMemoryBase(mem)
	if !ok {
		return nil, false
	}
	field, ok := sr.res.ResolveBitfieldPathLoadInContext(base, fieldOff, mem.Width, bitOff, bitWidth, sr.unionContext())
	if !ok {
		return nil, false
	}
	bitfield, ok := field.(*symresolve.SymbolBitfield)
	return bitfield, ok
}

// bitfieldMemoryBase resolves machine memory to its containing aggregate and
// physical byte offset without first selecting an overlapping storage field.
func (sr *symbolResolver) bitfieldMemoryBase(mem machine.MemoryAddress) (symresolve.SymbolPath, int, bool) {
	if mem.Index == nil {
		if _, ok := mem.Base.(*machine.FrameBase); ok {
			local, ok := sr.res.ResolveLocal(sr.fs, sr.memoryInstOff(mem), mem.Disp)
			if !ok {
				return nil, 0, false
			}
			root := &symresolve.SymbolRoot{Symbol: &local.Local}
			return sr.bitfieldAggregateAtOffset(root, local.FieldOff, mem.Width)
		}
		if global, ok := sr.globalAccessFromMemory(mem); ok {
			root := &symresolve.SymbolRoot{Symbol: global.Global}
			return sr.bitfieldAggregateAtOffset(root, global.FieldOff, mem.Width)
		}
		if seg, ok := mem.Seg.(*machine.FarPointer); ok {
			parent, ok := commonFarPointerParent(seg, mem.Base)
			if !ok {
				return nil, 0, false
			}
			base, ok := sr.symbolFromValue(parent)
			if !ok {
				return nil, 0, false
			}
			return sr.bitfieldAggregateAtOffset(base, mem.Disp, mem.Width)
		}
		if mem.Seg == nil {
			base, ok := sr.symbolFromValue(mem.Base)
			if ok && typeinfo.IsPointer(base.Type()) {
				return sr.bitfieldAggregateAtOffset(base, mem.Disp, mem.Width)
			}
		}
		if base, ok := sr.resolveBitfieldPointerMemoryPath(mem); ok {
			return sr.bitfieldAggregateAtOffset(base, mem.Disp, mem.Width)
		}
	}

	path, ok := sr.memoryPath(mem)
	if !ok {
		return nil, 0, false
	}
	return physicalAggregateBase(path)
}

// bitfieldAggregateAtOffset preserves an indexed flexible-array element as
// the aggregate base before matching a bitfield within that element.
func (sr *symbolResolver) bitfieldAggregateAtOffset(base symresolve.SymbolPath, off int, width int) (symresolve.SymbolPath, int, bool) {
	if path, ok := sr.symbolFromFlexibleArrayAccess(base, off, width); ok {
		return physicalAggregateBase(path)
	}
	return base, off, true
}

// physicalAggregateBase peels resolved members and byte offsets back to the
// nearest root, dereference, or indexed element that denotes an aggregate.
func physicalAggregateBase(path symresolve.SymbolPath) (symresolve.SymbolPath, int, bool) {
	off := 0
	for {
		switch p := path.(type) {
		case *symresolve.SymbolOffset:
			off += p.Offset
			path = p.Base
		case *symresolve.SymbolField:
			if p.Field == nil {
				return nil, 0, false
			}
			off += p.Field.Offset
			path = p.Base
		case *symresolve.SymbolBitfield:
			if p.Field == nil || p.Field.Bitfield == nil {
				return nil, 0, false
			}
			off += p.Field.Offset
			path = p.Base
		default:
			typ, _ := typeinfo.UnwrapPointer(path.Type())
			_, ok := typ.(*typeinfo.Struct)
			return path, off, ok
		}
	}
}

// resolveBitfieldPointerMemoryPath returns the symbolic pointer used by indirect memory.
func (sr *symbolResolver) resolveBitfieldPointerMemoryPath(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {
	if load, ok := nearPointerMemoryLoad(sr.dsReg, mem); ok {
		return sr.resolveBitfieldMemoryPath(load.Addr)
	}
	if load, ok := farPointerMemoryLoad(mem); ok {
		return sr.resolveBitfieldMemoryPath(load.Addr)
	}
	return nil, false
}

// resolveBitfieldMemoryPath returns an exact direct local or global storage path.
func (sr *symbolResolver) resolveBitfieldMemoryPath(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {
	path, ok := sr.memoryPath(mem)
	if !ok {
		return nil, false
	}
	root, off, ok := symbolOffsetRoot(path)
	if !ok || off != 0 {
		return nil, false
	}
	return root, true
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

// decompose decomposes a binary value into path parts
func (sr *symbolResolver) decompose(segNum uint16, width int, baseVal machine.Value, disp int) (symresolve.SymbolPath, bool) {
	var fixed = disp
	var other = baseVal

	// if this is a simple 0x1234 + (index*scale)
	// parse it now
	if bin, ok := baseVal.(*machine.Binary); ok && bin.Op == machine.ValueOpAdd {
		switch {
		case isConst(bin.LHS):
			c := bin.LHS.(*machine.Const)
			fixed += int(c.Val)
			other = bin.RHS

		case isConst(bin.RHS):
			c := bin.RHS.(*machine.Const)
			fixed += int(c.Val)
			other = bin.LHS
		}
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

		if indexSymbol, ok := sr.symbolFromValue(index); ok {
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
		if term, ok := sr.symbolFromIndexedField(base, fixed, width, indexBase, scale); ok {
			return term, true
		}
		term := &symresolve.SymbolTerm{
			Base:   base,
			Scale:  scale,
			Result: indexedTermResult(base.Type(), scale),
		}
		if indexSymbol, ok := sr.symbolFromValue(indexBase); ok {
			term.Index = indexSymbol
		} else {
			term.IndexVal = indexBase
		}
		base = term
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
	if indexSymbol, ok := sr.symbolFromValue(index); ok {
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

func isConst(value machine.Value) bool {
	_, ok := value.(*machine.Const)
	return ok
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
	if match == nil || !isZeroLengthArray(match.Type) {
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
