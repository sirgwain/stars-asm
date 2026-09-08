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
		effects = append(effects, b.Effects[i])
	}
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
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
	if p.scratchReadOutsideWindow(effects, index, wideScratch) {
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
	snapshot, ok := p.collapseScratchSnapshot(low.Src, high.Src, load, changed)
	if !ok || !machineValueMaskedWithin(snapshot, changed) {
		return clear, false
	}

	source := machine.BinaryVal(
		machine.ValueOpOr,
		machine.BinaryVal(machine.ValueOpAnd, load, keep),
		snapshot,
	)
	bitfield, ok := recognizeBitfieldWrite(p.ctx, clear.Addr, source)
	if !ok {
		return clear, false
	}
	field, ok := resolveDeclaredBitfield(p.ctx, clear.Addr, bitfield.BitOff, bitfield.BitWidth)
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
func (p *collapseStorageRMWProcessor) scratchReadOutsideWindow(effects []machine.Effect, index int, scratch machine.MemoryAddress) bool {
	for i, effect := range effects {
		if i >= index && i <= index+3 {
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

// machineValueMaskedWithin reports whether value is explicitly limited to the allowed bit mask.
func machineValueMaskedWithin(value machine.Value, allowed uint) bool {
	and, ok := value.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return false
	}
	mask, _, ok := constOperand(and.LHS, and.RHS)
	return ok && mask.Val&^allowed == 0
}
