package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

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
	if addr, ok := sr.addressFromStackAddressMemory(mem); ok {
		return resolvedAddressWithExpectedType(sr.addressWithExactPath(mem, addr), expected, mem.Width), true
	}
	if seg, ok := mem.Seg.(*machine.Reg); ok && seg.Val.IsSeg() && mem.Base != nil {
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

	if addr, ok := sr.addressFromFarPointerMemory(mem); ok {
		return resolvedAddressWithExpectedType(
			sr.addressWithExactPath(mem, addr),
			expected,
			mem.Width,
		), true
	}

	if addr, ok := sr.addressFromSplitFarPointer(mem.Seg, mem.Base, mem.Disp); ok {
		return resolvedAddressWithExpectedType(
			sr.addressWithExactPath(mem, addr),
			expected,
			mem.Width,
		), true
	}

	if path, ok := sr.exactMemoryPath(mem); ok {
		addr := resolvedAddressFromPath(path)
		addr.exact = path
		return resolvedAddressWithExpectedType(addr, expected, mem.Width), true
	}

	return resolvedAddress{}, false
}

// addressFromFarPointerMemory normalizes memory addressed through the
// segment/offset projections of the same machine far-pointer value.
//
// For example:
//
//	farseg(load(dword [bp-psz])):
//	    [faroff(load(dword [bp-psz])) + disp]
//
// means that the effective address was formed by following psz.  Preserve
// that fact as deref=true; exact typed projection is handled separately.
func (sr *symbolResolver) addressFromFarPointerMemory(mem machine.MemoryAddress) (resolvedAddress, bool) {
	segment, ok := mem.Seg.(*machine.FarPointer)
	if !ok {
		return resolvedAddress{}, false
	}

	parent, ok := commonFarPointerParent(segment, mem.Base)
	if !ok {
		return resolvedAddress{}, false
	}

	addr, ok := sr.addressFromValue(parent, 0)
	if !ok || !resolvedAddressIsPointer(addr) {
		return resolvedAddress{}, false
	}

	// The machine effective address was formed from the pointer's
	// segment:offset value, so exactly one pointer crossing is proven.
	addr.deref = true

	// Base has already been accounted for by parent. Add only the
	// MemoryAddress displacement/index components.
	return sr.addMemoryAddressTerms(addr, mem), true
}

// addressFromStackAddressMemory resolves SS:[&local+offset] through the
// addressed local object instead of preserving it as raw segmented memory.
func (sr *symbolResolver) addressFromStackAddressMemory(mem machine.MemoryAddress) (resolvedAddress, bool) {
	seg, ok := mem.Seg.(*machine.Reg)
	if !ok || seg.Val != asm.RegSS || mem.Base == nil {
		return resolvedAddress{}, false
	}
	addr, ok := sr.addressFromValue(mem.Base, 0)
	if !ok || !addr.hasBase() {
		return resolvedAddress{}, false
	}
	return sr.addMemoryAddressTerms(addr, mem), true
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
