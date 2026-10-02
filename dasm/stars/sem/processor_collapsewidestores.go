package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type collapseWideStoresProcessor struct {
	ctx *FuncContext
}

// ProcessMachineBlock coalesces adjacent contiguous copy effects in one machine block.
func (p *collapseWideStoresProcessor) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	changed := false
	effects := make([]machine.Effect, 0, len(b.Effects))
	for i := 0; i < len(b.Effects); i++ {
		effect, ok := b.Effects[i].(machine.StoreEffect)
		if !ok || effect.Width <= 0 {
			effects = append(effects, b.Effects[i])
			continue
		}
		if i+1 < len(b.Effects) {
			if next, ok := b.Effects[i+1].(machine.StoreEffect); ok && next.Width > 0 {
				if collapsed, ok := p.collapseWideMachineStorePair(effect, next); ok {
					effects = append(effects, collapsed)
					changed = true
					i++
					continue
				}
			}
		}
		if widened, ok := p.widenPointerStore(effect); ok {
			effects = append(effects, widened)
			changed = true
			continue
		}
		effects = append(effects, effect)
	}
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// widenPointerStore restores a whole far-pointer assignment represented by a
// single near-pointer offset store into typed far-pointer storage.
func (p *collapseWideStoresProcessor) widenPointerStore(store machine.StoreEffect) (machine.StoreEffect, bool) {
	if store.Width != 2 || p.ctx == nil {
		return store, false
	}
	withoutIndex := store.Addr
	withoutIndex.Index = nil
	dst, fieldOff, ok := p.ctx.symbols.addressBaseFromMemory(withoutIndex)
	if !ok || fieldOff != 0 || !typeinfo.IsFarPointer(dst.Type()) {
		return store, false
	}
	switch src := store.Src.(type) {
	case *machine.Address:
		if _, ok := p.ctx.symbols.symbolFromAddressAddress(src.Addr); !ok {
			return store, false
		}
	case *machine.Load:
		path, ok := p.ctx.symbols.symbolFromValue(src)
		if !ok || !typeinfo.IsPointer(path.Type()) {
			return store, false
		}
	case *machine.CallResult:
		if !typeinfo.IsPointer(src.Type) {
			return store, false
		}
	case *machine.Binary:
		normalized, ok := pointerOffsetStoreSource(p.ctx, store.Addr, src)
		if !ok {
			return store, false
		}
		store.Src = normalized
	default:
		return store, false
	}
	store.Width = dst.Type().Bytes()
	store.Addr.Width = store.Width
	return store, true
}

// pointerOffsetStoreSource widens the retained pointer load in a word-sized
// offset update so the source still represents the complete pointer value.
func pointerOffsetStoreSource(ctx *FuncContext, dst machine.MemoryAddress, src *machine.Binary) (*machine.Binary, bool) {
	if src.Op != machine.ValueOpAdd && src.Op != machine.ValueOpSub {
		return nil, false
	}
	if load, ok := src.LHS.(*machine.Load); ok && ctx.symbols.sameResolvedStorage(dst, load.Addr) {
		next := *src
		wide := *load
		wide.Addr.Width = 4
		next.LHS = &wide
		return &next, true
	}
	if src.Op == machine.ValueOpAdd {
		if load, ok := src.RHS.(*machine.Load); ok && ctx.symbols.sameResolvedStorage(dst, load.Addr) {
			next := *src
			wide := *load
			wide.Addr.Width = 4
			next.RHS = &wide
			return &next, true
		}
	}
	return nil, false
}

// collapseWideMachineStorePair coalesces adjacent low/high word stores into
// one 32-bit machine store when their destinations describe the same object.
func (p *collapseWideStoresProcessor) collapseWideMachineStorePair(low machine.StoreEffect, high machine.StoreEffect) (machine.StoreEffect, bool) {
	// Only word lanes form a dword; byte stores two apart, as in
	// szForm[1] = 9; szForm[3] = 0, are unrelated.
	if low.Width != 2 || high.Width != 2 {
		return low, false
	}
	if collapsed, ok := p.collapseHugePointerStore(low, high); ok {
		return collapsed, true
	}

	if _, adjacent := p.ctx.symbols.adjacentResolvedStorage(low.Addr, high.Addr); adjacent {
		if src, ok := p.collapseWideMaskedStoreSource(low, high); ok {
			low.Addr.Width = 4
			low.Src = src
			low.Width = 4
			return low, true
		}
	}

	// Compiler-lowered struct/union copy. Generic wideStorageDestination
	// deliberately rejects aggregates, so prove both sides independently.
	if collapsed, ok := p.collapseWideAggregateCopy(low, high); ok {
		return collapsed, true
	}
	if collapsed, ok := p.collapseWideAggregateReturn(low, high); ok {
		return collapsed, true
	}

	wideAddress, ok := p.ctx.symbols.wideStorageDestination(low.Addr, high.Addr)
	if !ok {
		return low, false
	}

	src, ok := p.collapseWideArithmeticSource(low, high)
	if !ok {
		src, ok = p.collapseWidePointerOffsetStoreSource(low, high)
	}
	if !ok {
		src, ok = (&wideMachineCollapser{ctx: p.ctx}).pair(low.Src, high.Src)
	}
	if !ok {
		// A word load plus a constant high lane constructs a dword with no
		// second source read. Convert through U16 before widening: a
		// signed source's bit 15 must not fill the constant high lane.
		load, lowOK := low.Src.(*machine.Load)
		constant, highOK := high.Src.(*machine.Const)
		dst, dstOK := p.ctx.symbols.memoryPath(wideAddress)
		if lowOK && highOK && load.Addr.Width == 2 && constant.Fixup == nil &&
			dstOK && dst.Type().Kind() == typeinfo.KInt && dst.Type().Bytes() == 4 {
			src = machine.BinaryVal(machine.ValueOpOr,
				machine.CastVal(machine.CastVal(load, typeinfo.U16), typeinfo.U32),
				machine.ConstVal((constant.Val&0xffff)<<16))
			ok = true
		}
	}
	if !ok {
		return low, false
	}

	low.Addr = wideAddress
	low.Src = src
	low.Width = 4
	return low, true
}

// collapseHugePointerStore folds the selector update of Win16 huge-pointer
// arithmetic into the pointer's offset update. MSC lowers p += n for a huge
// pointer as
//
//	ADD ax, [p]      ; offset += LOWORD(n)
//	ADC dx, 0        ; dx = HIWORD(n) + carry
//	MOV cx, __AHSHIFT
//	SHL dx, cx       ; segments to selector units
//	ADD dx, [p+2]    ; selector += dx << __AHSHIFT
//
// which reaches here as a pointer offset store followed by a high-word store
// of (hiword(n) << __AHSHIFT) + HIWORD(p). The pair is the single pointer
// update p + n.
func (p *collapseWideStoresProcessor) collapseHugePointerStore(low, high machine.StoreEffect) (machine.StoreEffect, bool) {
	offset, ok := low.Src.(*machine.Binary)
	if !ok || offset.Op != machine.ValueOpAdd {
		return low, false
	}
	selector, ok := high.Src.(*machine.Binary)
	if !ok || selector.Op != machine.ValueOpAdd {
		return low, false
	}
	shifted, base := selector.LHS, selector.RHS
	if _, ok := shifted.(*machine.Binary); !ok {
		shifted, base = base, shifted
	}
	load, ok := base.(*machine.Load)
	if !ok || !p.ctx.symbols.sameResolvedStorage(high.Addr, load.Addr) {
		return low, false
	}
	shift, ok := shifted.(*machine.Binary)
	if !ok || shift.Op != machine.ValueOpShl || !hugeShiftConst(shift.RHS) {
		return low, false
	}
	delta := offset.LHS
	if pointer, ok := delta.(*machine.Load); ok && p.ctx.symbols.sameResolvedStorage(low.Addr, pointer.Addr) {
		delta = offset.RHS
	}
	if !hugeHighDelta(shift.LHS, delta) {
		return low, false
	}
	return p.widenPointerStore(low)
}

// hugeShiftConst reports whether value is the loader-supplied __AHSHIFT
// selector shift imported from KERNEL.
func hugeShiftConst(value machine.Value) bool {
	c, ok := value.(*machine.ImportConst)
	return ok && c.Is("KERNEL", "__AHSHIFT")
}

// hugeHighDelta reports whether value is the high word of the 32-bit
// increment whose low word is delta, allowing the ADC's carry-in as + 0.
func hugeHighDelta(value, delta machine.Value) bool {
	if binary, ok := value.(*machine.Binary); ok && binary.Op == machine.ValueOpAdd {
		if c, ok := binary.RHS.(*machine.Const); ok && c.Val == 0 {
			value = binary.LHS
		}
	}
	switch v := value.(type) {
	case *machine.WordValue:
		return v.Part == machine.WordSignHigh && machine.ValueEquals(v.Parent, delta)
	case *machine.Const:
		return v.Val == 0
	}
	return false
}

// collapseWideArithmeticSource reconstructs carry-aware 32-bit addition or
// subtraction from an adjacent ADD/ADC or SUB/SBB store pair.
func (p *collapseWideStoresProcessor) collapseWideArithmeticSource(low machine.StoreEffect, high machine.StoreEffect) (machine.Value, bool) {
	lowBinary, lowOK := low.Src.(*machine.Binary)
	highBinary, highOK := high.Src.(*machine.Binary)
	if !lowOK || !highOK || lowBinary.Op != highBinary.Op {
		return nil, false
	}

	switch lowBinary.Op {
	case machine.ValueOpAdd:
		if !machine.AdjacentWideArithmetic(low.MetaInfo, high.MetaInfo, asm.OpADD, asm.OpADC) &&
			!machine.AdjacentWideArithmetic(lowBinary.Producer, highBinary.Producer, asm.OpADD, asm.OpADC) {
			return nil, false
		}
	case machine.ValueOpSub:
		if !machine.AdjacentWideArithmetic(low.MetaInfo, high.MetaInfo, asm.OpSUB, asm.OpSBB) &&
			!machine.AdjacentWideArithmetic(lowBinary.Producer, highBinary.Producer, asm.OpSUB, asm.OpSBB) {
			return nil, false
		}
	default:
		return nil, false
	}

	collapser := wideMachineCollapser{ctx: p.ctx}
	lhs, ok := collapser.pair(lowBinary.LHS, highBinary.LHS)
	if !ok {
		return nil, false
	}
	rhs, ok := collapser.pair(lowBinary.RHS, highBinary.RHS)
	if !ok {
		return nil, false
	}
	return machine.BinaryVal(lowBinary.Op, lhs, rhs), true
}

// collapseWidePointerOffsetStoreSource reconstructs a far-pointer base plus
// a low-word byte delta when the high pointer word is copied unchanged.
func (p *collapseWideStoresProcessor) collapseWidePointerOffsetStoreSource(low, high machine.StoreEffect) (machine.Value, bool) {
	wideDestination := low.Addr
	wideDestination.Width = 4
	resolved, ok := p.ctx.symbols.addressFromMemory(wideDestination, nil)
	if !ok {
		return nil, false
	}
	destination, ok := resolved.path()
	if !ok || !typeinfo.IsFarPointer(destination.Type()) {
		return nil, false
	}
	binary, ok := low.Src.(*machine.Binary)
	if !ok || (binary.Op != machine.ValueOpAdd && binary.Op != machine.ValueOpSub) {
		return nil, false
	}
	collapser := wideMachineCollapser{ctx: p.ctx}
	if base, ok := collapser.pair(binary.LHS, high.Src); ok {
		if _, segmentedAddress := base.(*machine.Address); !segmentedAddress {
			return machine.BinaryVal(binary.Op, base, binary.RHS), true
		}
	}
	if binary.Op == machine.ValueOpAdd {
		if base, ok := collapser.pair(binary.RHS, high.Src); ok {
			if _, segmentedAddress := base.(*machine.Address); !segmentedAddress {
				return machine.BinaryVal(binary.Op, base, binary.LHS), true
			}
		}
	}
	return nil, false
}

// collapseWideMaskedStoreSource reconstructs paired masked word writes as
// one 32-bit destination-preserving masked write.
func (p *collapseWideStoresProcessor) collapseWideMaskedStoreSource(low, high machine.StoreEffect) (machine.Value, bool) {
	lowLoad, lowKeep, lowSet, ok := p.maskedMachineWordStore(low.Addr, low.Src)
	if !ok {
		return nil, false
	}
	_, highKeep, highSet, ok := p.maskedMachineWordStore(high.Addr, high.Src)
	if !ok {
		return nil, false
	}

	wideLoad := *lowLoad
	wideLoad.Addr.Width = 4
	keep := ((highKeep & 0xffff) << 16) | (lowKeep & 0xffff)
	set, ok := (&wideMachineCollapser{ctx: p.ctx}).pair(lowSet, highSet)
	if !ok {
		return nil, false
	}
	wideSource := machine.BinaryVal(
		machine.ValueOpOr,
		machine.BinaryVal(machine.ValueOpAnd, &wideLoad, machine.ConstVal(keep)),
		set,
	)
	wideDestination := low.Addr
	wideDestination.Width = 4
	bitfield, ok := recognizeBitfieldWrite(p.ctx, wideDestination, wideSource)
	if !ok {
		return nil, false
	}
	field, ok := resolveDeclaredBitfield(p.ctx, wideDestination, bitfield.Access)
	if !ok || field.Bitfield.StorageSize != wideDestination.Width {
		return nil, false
	}
	return wideSource, true
}

// maskedMachineWordStore extracts the source load plus keep and set masks
// from one 16-bit destination-preserving write.
func (p *collapseWideStoresProcessor) maskedMachineWordStore(dst machine.MemoryAddress, value machine.Value) (*machine.Load, uint, machine.Value, bool) {
	source := value
	set := machine.Value(machine.ConstVal(0))
	if or, ok := value.(*machine.Binary); ok && or.Op == machine.ValueOpOr {
		if _, _, ok := p.maskedMachineWordKeep(dst, or.LHS); ok {
			source = or.LHS
			set = or.RHS
		} else if _, _, ok := p.maskedMachineWordKeep(dst, or.RHS); ok {
			source = or.RHS
			set = or.LHS
		} else {
			return nil, 0, nil, false
		}
	}
	load, keep, ok := p.maskedMachineWordKeep(dst, source)
	return load, keep, set, ok
}

// maskedMachineWordKeep extracts the retained destination load and keep mask
// from one half of a paired masked store, treating an unmasked load as 0xffff.
func (p *collapseWideStoresProcessor) maskedMachineWordKeep(dst machine.MemoryAddress, source machine.Value) (*machine.Load, uint, bool) {
	if load, ok := source.(*machine.Load); ok {
		if p.ctx.sameResolvedStorage(dst, load.Addr) {
			return load, 0xffff, true
		}
		return nil, 0, false
	}
	and, ok := source.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return nil, 0, false
	}
	keep, keptSource, ok := constOperand(and.LHS, and.RHS)
	if !ok {
		return nil, 0, false
	}
	load, ok := keptSource.(*machine.Load)
	if !ok || !p.ctx.sameResolvedStorage(dst, load.Addr) {
		return nil, 0, false
	}
	return load, keep.Val, true
}

// collapseWideAggregateCopy reconstructs a compiler-lowered four-byte
// struct/union assignment from its adjacent word loads and stores.
func (p *collapseWideStoresProcessor) collapseWideAggregateCopy(low machine.StoreEffect, high machine.StoreEffect) (machine.StoreEffect, bool) {
	lowLoad, lowOK := low.Src.(*machine.Load)
	highLoad, highOK := high.Src.(*machine.Load)
	if !lowOK || !highOK {
		return low, false
	}

	wideDst, dstType, ok := p.ctx.symbols.wideAggregateStorage(low.Addr, high.Addr)
	if !ok {
		return low, false
	}

	wideSrc, srcType, ok := p.ctx.symbols.wideAggregateStorage(lowLoad.Addr, highLoad.Addr)
	if !ok || !typeinfo.Equals(dstType, srcType) {
		return low, false
	}

	src := *lowLoad
	src.Addr = wideSrc

	low.Addr = wideDst
	low.Src = &src
	low.Width = 4
	return low, true
}

// collapseWideAggregateReturn restores the assignment of a four-byte struct
// returned in DX:AX. The compiler stores loword and hiword of the call result
// into the destination's two words, as with pt = PtDisplayPlanetStateInfo(...).
func (p *collapseWideStoresProcessor) collapseWideAggregateReturn(low machine.StoreEffect, high machine.StoreEffect) (machine.StoreEffect, bool) {
	lowWord, lowOK := low.Src.(*machine.WordValue)
	highWord, highOK := high.Src.(*machine.WordValue)
	if !lowOK || !highOK || lowWord.Part != machine.WordLow || highWord.Part != machine.WordHigh {
		return low, false
	}
	result, ok := lowWord.Parent.(*machine.CallResult)
	if !ok || !machine.ValueEquals(result, highWord.Parent) {
		return low, false
	}

	wideDst, dstType, ok := p.ctx.symbols.wideAggregateStorage(low.Addr, high.Addr)
	if !ok || !typeinfo.Equals(dstType, result.Type) {
		return low, false
	}

	low.Addr = wideDst
	low.Src = result
	low.Width = 4
	return low, true
}
