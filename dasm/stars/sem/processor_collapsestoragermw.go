package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
)

type collapseStorageRMWProcessor struct {
	ctx *FuncContext
}

// ProcessMachineBlock collapses scratch-backed clear-and-OR storage updates in one machine block.
func (p *collapseStorageRMWProcessor) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	effects := make([]machine.Effect, 0, len(b.Effects))
	changed := false
	for i := 0; i < len(b.Effects); i++ {
		if i+3 < len(b.Effects) {
			low, lowOK := b.Effects[i].(machine.StoreEffect)
			high, highOK := b.Effects[i+1].(machine.StoreEffect)
			clear, clearOK := b.Effects[i+2].(machine.StoreEffect)
			insert, insertOK := b.Effects[i+3].(machine.StoreEffect)
			if lowOK && highOK && clearOK && insertOK {
				if collapsed, ok := p.collapseScratchBitfieldRMW(b.Effects, i, low, high, clear, insert); ok {
					effects = append(effects, collapsed)
					changed = true
					i += 3
					continue
				}
			}
		}
		if i+2 < len(b.Effects) {
			snapshot, snapshotOK := b.Effects[i].(machine.StoreEffect)
			clear, clearOK := b.Effects[i+1].(machine.StoreEffect)
			insert, insertOK := b.Effects[i+2].(machine.StoreEffect)
			if snapshotOK && clearOK && insertOK {
				if collapsed, ok := p.collapseScratchWordBitfieldRMW(f, b, i, snapshot, clear, insert); ok {
					effects = append(effects, collapsed)
					changed = true
					i += 2
					continue
				}
			}
		}
		effects = append(effects, b.Effects[i])
	}
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// collapseScratchWordBitfieldRMW reconstructs a word-sized captured field update
// before the temporary clear can be mistaken for a separate field assignment.
func (p *collapseStorageRMWProcessor) collapseScratchWordBitfieldRMW(f machine.FuncEffects, b machine.BlockEffects, index int, snapshot, clear, insert machine.StoreEffect) (machine.StoreEffect, bool) {
	if snapshot.Width != 2 || clear.Width != 2 || insert.Width != 2 ||
		!p.syntheticScratchAddress(snapshot.Addr) || !p.ctx.sameResolvedStorage(clear.Addr, insert.Addr) ||
		!p.insertUsesScratchSnapshot(insert, snapshot.Addr) {
		return clear, false
	}
	if p.scratchReadOutsideWindow(b.Effects, index, index+2, snapshot.Addr) {
		return clear, false
	}
	for _, other := range f.Blocks {
		if other.Block != b.Block && p.scratchReadOutsideWindow(other.Effects, -1, -1, snapshot.Addr) {
			return clear, false
		}
	}
	load, keep, set, ok := bitfieldStoreParts(clear.Addr, clear.Src, p.ctx.sameResolvedStorage)
	if !ok || !machineConstIsZero(set) || !machineValueMaskedWithin(snapshot.Src, (^keep.Val)&0xffff) {
		return clear, false
	}
	return p.bitfieldRMWStore(clear, load, keep, snapshot.Src)
}

// collapseScratchBitfieldRMW combines a captured wide scratch value with a subsequent clear-and-OR bitfield update.
func (p *collapseStorageRMWProcessor) collapseScratchBitfieldRMW(effects []machine.Effect, index int, low, high, clear, insert machine.StoreEffect) (machine.StoreEffect, bool) {
	if p.ctx == nil || low.Width != 2 || high.Width != 2 || clear.Width != 4 || insert.Width != 4 {
		return clear, false
	}
	if !p.scratchWordPair(low.Addr, high.Addr) || !p.ctx.sameResolvedStorage(clear.Addr, insert.Addr) {
		return clear, false
	}

	wideScratch := low.Addr
	wideScratch.Width = 4
	if !p.insertUsesScratchSnapshot(insert, wideScratch) {
		return clear, false
	}
	if p.scratchReadOutsideWindow(effects, index, index+3, wideScratch) {
		return clear, false
	}

	load, keep, set, ok := bitfieldStoreParts(clear.Addr, clear.Src, p.ctx.sameResolvedStorage)
	if !ok || !machineConstIsZero(set) {
		return clear, false
	}
	fullMask, ok := bitMask(clear.Width * 8)
	if !ok {
		return clear, false
	}
	changed := (^keep.Val) & fullMask
	if snapshot, ok := p.collapseScratchSnapshot(low.Src, high.Src, load, changed); ok {
		if collapsed, ok := p.bitfieldRMWStore(clear, load, keep, snapshot); ok {
			return collapsed, true
		}
	}
	snapshot, ok := p.collapseResolvedScratchSnapshot(effects, index, low.Src, high.Src, load, changed)
	if !ok {
		return clear, false
	}
	return p.bitfieldRMWStore(clear, load, keep, snapshot)
}

// bitfieldRMWStore validates a reconstructed scratch snapshot as a declared
// bitfield update and returns the fused destination store.
func (p *collapseStorageRMWProcessor) bitfieldRMWStore(clear machine.StoreEffect, load *machine.Load, keep *machine.Const, snapshot machine.Value) (machine.StoreEffect, bool) {
	source := machine.BinaryVal(
		machine.ValueOpOr,
		machine.BinaryVal(machine.ValueOpAnd, load, keep),
		snapshot,
	)
	bitfield, ok := recognizeBitfieldWrite(p.ctx, clear.Addr, source)
	if !ok {
		return clear, false
	}
	field, ok := resolveDeclaredBitfield(p.ctx, clear.Addr, bitfield.Access)
	if !ok || field.Bitfield.StorageSize != clear.Width {
		return clear, false
	}

	clear.Src = source
	return clear, true
}

// collapseScratchSnapshot reconstructs the captured dword, including a field wholly contained in one word lane.
func (p *collapseStorageRMWProcessor) collapseScratchSnapshot(low, high machine.Value, storage *machine.Load, changed uint) (machine.Value, bool) {
	if snapshot, ok := (&wideMachineCollapser{ctx: p.ctx}).pair(low, high); ok && machineValueMaskedWithin(snapshot, changed) {
		return snapshot, true
	}
	if machineConstIsZero(low) && changed&0xffff == 0 {
		return p.collapseSingleScratchLane(high, storage, changed, 1)
	}
	if machineConstIsZero(high) && changed>>16 == 0 {
		return p.collapseSingleScratchLane(low, storage, changed, 0)
	}
	return nil, false
}

// collapseResolvedScratchSnapshot retries snapshot reconstruction after
// substituting reaching definitions for intermediate scratch loads.
func (p *collapseStorageRMWProcessor) collapseResolvedScratchSnapshot(effects []machine.Effect, index int, low, high machine.Value, storage *machine.Load, changed uint) (machine.Value, bool) {
	if snapshot, ok := (&wideMachineCollapser{ctx: p.ctx}).pair(low, high); ok {
		snapshot = p.resolveScratchValue(effects, index, snapshot)
		if machineValueMaskedWithin(snapshot, changed) {
			return snapshot, true
		}
	}
	low = p.resolveScratchValue(effects, index, low)
	high = p.resolveScratchValue(effects, index, high)
	return p.collapseScratchSnapshot(low, high, storage, changed)
}

// resolveScratchValue substitutes reaching synthetic scratch definitions into
// a machine value before wide arithmetic matching.
func (p *collapseStorageRMWProcessor) resolveScratchValue(effects []machine.Effect, before int, value machine.Value) machine.Value {
	rewriter := &machineRewriter{
		value: func(mr *machineRewriter, candidate machine.Value) (machine.Value, bool, bool) {
			load, ok := candidate.(*machine.Load)
			if !ok {
				return candidate, false, false
			}
			resolved, ok := p.scratchValueAt(effects, before, load.Addr)
			if !ok {
				return candidate, false, true
			}
			return resolved, true, true
		},
	}
	resolved, _ := rewriter.rewriteMachineValue(value)
	return resolved
}

// scratchValueAt returns the reaching value for an exact synthetic scratch
// load, reconstructing adjacent word definitions for a dword read.
func (p *collapseStorageRMWProcessor) scratchValueAt(effects []machine.Effect, before int, addr machine.MemoryAddress) (machine.Value, bool) {
	if !p.syntheticScratchAddress(addr) {
		return nil, false
	}
	if store, index, ok := p.reachingScratchStore(effects, before, addr); ok {
		return p.resolveScratchValue(effects, index, store.Src), true
	}
	if addr.Width != 4 {
		return nil, false
	}
	lowAddr := addr
	lowAddr.Width = 2
	highAddr := lowAddr
	highAddr.Disp += 2
	low, lowIndex, lowOK := p.reachingScratchStore(effects, before, lowAddr)
	high, highIndex, highOK := p.reachingScratchStore(effects, before, highAddr)
	if !lowOK || !highOK {
		return nil, false
	}
	lowValue := p.resolveScratchValue(effects, lowIndex, low.Src)
	highValue := p.resolveScratchValue(effects, highIndex, high.Src)
	return (&wideMachineCollapser{ctx: p.ctx}).pair(lowValue, highValue)
}

// reachingScratchStore finds the latest exact definition of a scratch range;
// an intervening overlapping write kills the lookup.
func (p *collapseStorageRMWProcessor) reachingScratchStore(effects []machine.Effect, before int, addr machine.MemoryAddress) (machine.StoreEffect, int, bool) {
	for i := before - 1; i >= 0; i-- {
		store, ok := effects[i].(machine.StoreEffect)
		if !ok || !p.scratchStorageOverlaps(addr, store.Addr) {
			continue
		}
		if p.sameScratchRange(addr, store.Addr) {
			return store, i, true
		}
		return machine.StoreEffect{}, 0, false
	}
	return machine.StoreEffect{}, 0, false
}

// syntheticScratchAddress reports whether an address is rooted at compiler
// scratch storage rather than a declared local.
func (p *collapseStorageRMWProcessor) syntheticScratchAddress(addr machine.MemoryAddress) bool {
	resolved, ok := p.ctx.symbols.addressFromMemory(addr, nil)
	if !ok {
		return false
	}
	_, ok = resolved.base.(*symresolve.SymbolScratch)
	return ok
}

// sameScratchRange reports whether two scratch accesses cover identical bytes.
func (p *collapseStorageRMWProcessor) sameScratchRange(a, b machine.MemoryAddress) bool {
	if a.Width != b.Width || !p.scratchStorageOverlaps(a, b) {
		return false
	}
	aResolved, aOK := p.ctx.symbols.addressFromMemory(a, nil)
	bResolved, bOK := p.ctx.symbols.addressFromMemory(b, nil)
	return aOK && bOK && resolvedAddressPhysicalOffset(aResolved) == resolvedAddressPhysicalOffset(bResolved)
}

// collapseSingleScratchLane rebuilds a masked ADD/ADC lane as a wide add before applying the field mask.
func (p *collapseStorageRMWProcessor) collapseSingleScratchLane(value machine.Value, storage *machine.Load, changed uint, lane int) (machine.Value, bool) {
	and, ok := value.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return nil, false
	}
	mask, unmasked, ok := constOperand(and.LHS, and.RHS)
	if !ok || mask.Val != (changed>>(lane*16))&0xffff {
		return nil, false
	}
	add, ok := unmasked.(*machine.Binary)
	wantOp := asm.OpADD
	wantPart := machine.WordLow
	if lane == 1 {
		wantOp = asm.OpADC
		wantPart = machine.WordHigh
	}
	if !ok || add.Op != machine.ValueOpAdd || add.Producer.InstOp != wantOp {
		return nil, false
	}
	laneAddress := storage.Addr
	laneAddress.Width = 2
	laneAddress.Disp += lane * 2
	delta, ok := p.scratchLaneAddOperands(laneAddress, add.LHS, add.RHS, wantPart)
	if !ok {
		delta, ok = p.scratchLaneAddOperands(laneAddress, add.RHS, add.LHS, wantPart)
	}
	if !ok {
		return nil, false
	}
	return machine.BinaryVal(
		machine.ValueOpAnd,
		machine.BinaryVal(machine.ValueOpAdd, storage, delta),
		machine.ConstVal(changed),
	), true
}

// scratchLaneAddOperands returns the wide delta parent when an arithmetic lane adds the matching storage word.
func (p *collapseStorageRMWProcessor) scratchLaneAddOperands(storage machine.MemoryAddress, storageValue, deltaValue machine.Value, part machine.WordPart) (machine.Value, bool) {
	load, ok := storageValue.(*machine.Load)
	if !ok || !p.ctx.sameResolvedStorage(storage, load.Addr) {
		return nil, false
	}
	word, ok := deltaValue.(*machine.WordValue)
	if !ok || word.Part != part {
		return nil, false
	}
	return word.Parent, true
}

// scratchWordPair reports whether two adjacent word addresses are synthetic stack scratch storage.
func (p *collapseStorageRMWProcessor) scratchWordPair(low, high machine.MemoryAddress) bool {
	lo, ok := p.ctx.symbols.addressFromMemory(low, nil)
	if !ok {
		return false
	}
	hi, ok := p.ctx.symbols.addressFromMemory(high, nil)
	if !ok || !p.ctx.symbols.sameResolvedAddressBase(lo, hi) {
		return false
	}
	_, loScratch := lo.base.(*symresolve.SymbolScratch)
	_, hiScratch := hi.base.(*symresolve.SymbolScratch)
	return loScratch && hiScratch && resolvedAddressPhysicalOffset(hi) == resolvedAddressPhysicalOffset(lo)+2
}

// insertUsesScratchSnapshot reports whether an OR store combines its current destination with the captured scratch dword.
func (p *collapseStorageRMWProcessor) insertUsesScratchSnapshot(insert machine.StoreEffect, scratch machine.MemoryAddress) bool {
	or, ok := insert.Src.(*machine.Binary)
	if !ok || or.Op != machine.ValueOpOr {
		return false
	}
	return p.destinationAndScratchLoads(insert.Addr, scratch, or.LHS, or.RHS) ||
		p.destinationAndScratchLoads(insert.Addr, scratch, or.RHS, or.LHS)
}

// destinationAndScratchLoads matches the two load operands of the final OR store.
func (p *collapseStorageRMWProcessor) destinationAndScratchLoads(dst, scratch machine.MemoryAddress, dstValue, scratchValue machine.Value) bool {
	dstLoad, dstOK := dstValue.(*machine.Load)
	scratchLoad, scratchOK := scratchValue.(*machine.Load)
	return dstOK && scratchOK &&
		p.ctx.sameResolvedStorage(dst, dstLoad.Addr) &&
		p.ctx.sameResolvedStorage(scratch, scratchLoad.Addr)
}

// scratchReadOutsideWindow reports whether removing the scratch definitions would discard another use.
func (p *collapseStorageRMWProcessor) scratchReadOutsideWindow(effects []machine.Effect, start, end int, scratch machine.MemoryAddress) bool {
	for i, effect := range effects {
		if i >= start && i <= end {
			continue
		}
		found := false
		rewriter := machineRewriter{
			value: func(w *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
				load, ok := value.(*machine.Load)
				if ok && p.scratchStorageOverlaps(scratch, load.Addr) {
					found = true
				}
				return nil, false, false
			},
		}
		rewriter.rewriteMachineEffect(effect)
		if found {
			return true
		}
	}
	return false
}

// scratchStorageOverlaps reports whether two accesses overlap the same synthetic scratch bytes.
func (p *collapseStorageRMWProcessor) scratchStorageOverlaps(a, b machine.MemoryAddress) bool {
	if a.Width <= 0 || b.Width <= 0 {
		return false
	}
	aResolved, aOK := p.ctx.symbols.addressFromMemory(a, nil)
	bResolved, bOK := p.ctx.symbols.addressFromMemory(b, nil)
	if !aOK || !bOK || !p.ctx.symbols.sameResolvedAddressBase(aResolved, bResolved) {
		return false
	}
	_, aScratch := aResolved.base.(*symresolve.SymbolScratch)
	_, bScratch := bResolved.base.(*symresolve.SymbolScratch)
	if !aScratch || !bScratch {
		return false
	}
	aStart := resolvedAddressPhysicalOffset(aResolved)
	bStart := resolvedAddressPhysicalOffset(bResolved)
	return aStart < bStart+b.Width && bStart < aStart+a.Width
}

// machineConstIsZero reports whether a machine value is the constant zero.
func machineConstIsZero(value machine.Value) bool {
	constant, ok := value.(*machine.Const)
	return ok && constant.Val == 0
}
