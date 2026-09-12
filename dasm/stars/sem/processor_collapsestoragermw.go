package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
)

type collapseStorageRMWProcessor struct {
	ctx *FuncContext
}

type storageRMWRewrite struct {
	start   int
	end     int
	replace machine.Effect
	remove  map[int]bool
}

// ProcessMachineBlock collapses scratch-backed bitfield updates in one machine block.
func (p *collapseStorageRMWProcessor) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	// A register may retain the value just stored and feed the next bitfield
	// update. Recover that value as current storage before stale-load capture.
	original := b.Effects
	changed := false
	for i := 1; i < len(original); i++ {
		previous, previousOK := original[i-1].(machine.StoreEffect)
		store, storeOK := original[i].(machine.StoreEffect)
		if !previousOK || !storeOK {
			continue
		}
		if recovered, ok := p.recoverStoredBitfieldRMW(previous, store); ok {
			if !changed {
				b.Effects = append([]machine.Effect(nil), original...)
			}
			b.Effects[i] = recovered
			changed = true
		}
	}
	rewrites := make([]storageRMWRewrite, 0)
	occupied := make([]bool, len(b.Effects))
	for i := 0; i+2 < len(b.Effects); i++ {
		if occupied[i] || occupied[i+1] || occupied[i+2] {
			continue
		}
		low, lowOK := b.Effects[i].(machine.StoreEffect)
		high, highOK := b.Effects[i+1].(machine.StoreEffect)
		store, storeOK := b.Effects[i+2].(machine.StoreEffect)
		if !lowOK || !highOK || !storeOK {
			continue
		}
		if collapsed, ok := p.collapseScratchBitfieldCopy(f, b, i, low, high, store); ok {
			rewrites = append(rewrites, storageRMWRewrite{start: i, end: i + 2, replace: collapsed})
			occupied[i], occupied[i+1], occupied[i+2] = true, true, true
		}
	}
	for i := 0; i+1 < len(b.Effects); i++ {
		if occupied[i] || occupied[i+1] {
			continue
		}
		clear, clearOK := b.Effects[i].(machine.StoreEffect)
		insert, insertOK := b.Effects[i+1].(machine.StoreEffect)
		if !clearOK || !insertOK {
			continue
		}
		collapsed, remove, ok := p.collapseScratchBitfieldRMW(f, b, i, clear, insert)
		if !ok {
			collapsed, remove, ok = p.collapseScratchWordBitfieldRMW(f, b, i, clear, insert)
		}
		if ok {
			removed := make(map[int]bool, len(remove))
			for _, index := range remove {
				removed[index] = true
			}
			rewrites = append(rewrites, storageRMWRewrite{start: i, end: i + 1, replace: collapsed, remove: removed})
			occupied[i], occupied[i+1] = true, true
		}
	}
	if len(rewrites) == 0 {
		return b, changed
	}

	replaceAt := make(map[int]storageRMWRewrite, len(rewrites))
	remove := make(map[int]bool)
	for _, rewrite := range rewrites {
		replaceAt[rewrite.start] = rewrite
		for index := rewrite.start; index <= rewrite.end; index++ {
			occupied[index] = true
		}
		for index := range rewrite.remove {
			remove[index] = true
		}
	}
	effects := make([]machine.Effect, 0, len(b.Effects)-len(remove))
	for i := 0; i < len(b.Effects); i++ {
		if rewrite, ok := replaceAt[i]; ok {
			effects = append(effects, rewrite.replace)
			i = rewrite.end
			continue
		}
		if remove[i] {
			continue
		}
		effects = append(effects, b.Effects[i])
	}
	b.Effects = effects
	return b, true
}

// recoverStoredBitfieldRMW replaces a retained expression from the immediately
// preceding store with a fresh load, then validates the resulting declared field
// update. Adjacency and load identities prevent crossing mutations or confusing
// equal-looking expressions computed from different versions of storage.
func (p *collapseStorageRMWProcessor) recoverStoredBitfieldRMW(previous, store machine.StoreEffect) (machine.StoreEffect, bool) {
	if previous.Width != store.Width || !p.ctx.sameResolvedStorage(previous.Addr, store.Addr) {
		return store, false
	}
	if _, ok := previous.Src.(*machine.Binary); !ok {
		return store, false
	}
	address := store.Addr
	address.Origin = machine.Origin{InstOff: store.MetaInfo.InstOff, Role: machine.OperandSrc}
	rewriter := &machineRewriter{
		value: func(w *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
			if machine.ValueEquals(value, previous.Src) {
				return machine.LoadVal(address), true, true
			}
			return value, false, false
		},
	}
	source, changed := rewriter.rewriteMachineValue(store.Src)
	if !changed {
		return store, false
	}
	bitfield, ok := recognizeBitfieldWrite(p.ctx, store.Addr, source)
	if !ok {
		return store, false
	}
	if _, ok := resolveDeclaredBitfield(p.ctx, store.Addr, bitfield.Access); !ok {
		return store, false
	}
	store.Src = source
	return store, true
}

// collapseScratchBitfieldCopy reconstructs a masked update assembled in two
// scratch words and then copied back to the original wide storage.
func (p *collapseStorageRMWProcessor) collapseScratchBitfieldCopy(f machine.FuncEffects, b machine.BlockEffects, index int, low, high, store machine.StoreEffect) (machine.StoreEffect, bool) {
	if low.Width != 2 || high.Width != 2 || store.Width != 4 || !p.scratchWordPair(low.Addr, high.Addr) {
		return store, false
	}
	scratch := low.Addr
	scratch.Width = 4
	load, ok := store.Src.(*machine.Load)
	if !ok || !p.sameScratchRange(scratch, load.Addr) {
		return store, false
	}
	// Only the final copy may read these scratch bytes. This also rejects
	// dependencies between the word definitions that would prevent fusion.
	if p.scratchReadOutsideWindow(b.Effects, index+2, index+2, scratch) {
		return store, false
	}
	for _, other := range f.Blocks {
		if other.Block != b.Block && p.scratchReadOutsideWindow(other.Effects, -1, -1, scratch) {
			return store, false
		}
	}
	// Match the retained loads against the copy destination, not the scratch
	// addresses, so ordinary wide bitfield validation proves the update.
	low.Addr = store.Addr
	low.Addr.Width = 2
	high.Addr = low.Addr
	high.Addr.Disp += 2
	source, ok := (&collapseWideStoresProcessor{ctx: p.ctx}).collapseWideMaskedStoreSource(low, high)
	if !ok {
		return store, false
	}
	store.Src = source
	return store, true
}

// collapseScratchWordBitfieldRMW reconstructs a word-sized captured field update
// from the reaching definition of the scratch value consumed by the insert.
func (p *collapseStorageRMWProcessor) collapseScratchWordBitfieldRMW(f machine.FuncEffects, b machine.BlockEffects, clearIndex int, clear, insert machine.StoreEffect) (machine.StoreEffect, []int, bool) {
	if clear.Width != 2 || insert.Width != 2 || !p.ctx.sameResolvedStorage(clear.Addr, insert.Addr) {
		return clear, nil, false
	}
	scratch, ok := p.insertedScratchLoad(insert)
	if !ok || scratch.Width != clear.Width {
		return clear, nil, false
	}
	load, keep, set, ok := bitfieldStoreParts(clear.Addr, clear.Src, p.ctx.sameResolvedStorage)
	if !ok || !machineConstIsZero(set) {
		return clear, nil, false
	}
	snapshot, snapshotIndices, ok := p.scratchValueAtWithIndices(b.Effects, clearIndex+1, scratch)
	if !ok || len(snapshotIndices) == 0 || scratchDefinitionAfter(snapshotIndices, clearIndex) {
		return clear, nil, false
	}
	if !machineValueMaskedWithin(snapshot, (^keep.Val)&0xffff) {
		return clear, nil, false
	}
	collapsed, ok := p.bitfieldRMWStore(clear, load, keep, snapshot)
	if !ok {
		return clear, nil, false
	}
	insertIndex := clearIndex + 1
	if !p.scratchSnapshotRemovable(f, b, scratch, snapshotIndices, insertIndex) {
		return clear, nil, false
	}
	return collapsed, snapshotIndices, true
}

// collapseScratchBitfieldRMW combines a captured wide scratch value with a subsequent clear-and-OR bitfield update.
func (p *collapseStorageRMWProcessor) collapseScratchBitfieldRMW(f machine.FuncEffects, b machine.BlockEffects, clearIndex int, clear, insert machine.StoreEffect) (machine.StoreEffect, []int, bool) {
	if p.ctx == nil || clear.Width != 4 || insert.Width != 4 || !p.ctx.sameResolvedStorage(clear.Addr, insert.Addr) {
		return clear, nil, false
	}
	scratch, ok := p.insertedScratchLoad(insert)
	if !ok || scratch.Width != clear.Width {
		return clear, nil, false
	}
	load, keep, set, ok := bitfieldStoreParts(clear.Addr, clear.Src, p.ctx.sameResolvedStorage)
	if !ok || !machineConstIsZero(set) {
		return clear, nil, false
	}
	fullMask, ok := bitMask(clear.Width * 8)
	if !ok {
		return clear, nil, false
	}
	changed := (^keep.Val) & fullMask
	snapshot, snapshotIndices, ok := p.scratchRMWValueAt(b.Effects, clearIndex+1, scratch, load, changed)
	if !ok || len(snapshotIndices) == 0 || scratchDefinitionAfter(snapshotIndices, clearIndex) {
		return clear, nil, false
	}
	collapsed, ok := p.bitfieldRMWStore(clear, load, keep, snapshot)
	if !ok {
		return clear, nil, false
	}
	insertIndex := clearIndex + 1
	if !p.scratchSnapshotRemovable(f, b, scratch, snapshotIndices, insertIndex) {
		return clear, nil, false
	}
	return collapsed, snapshotIndices, true
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
	if machineConstIsZero(low) && changed&0xffff == 0 {
		if snapshot, ok := p.collapseSingleScratchLane(high, storage, changed, 1); ok {
			return snapshot, true
		}
	}
	if machineConstIsZero(high) && changed>>16 == 0 {
		if snapshot, ok := p.collapseSingleScratchLane(low, storage, changed, 0); ok {
			return snapshot, true
		}
	}
	if snapshot, ok := (&wideMachineCollapser{ctx: p.ctx}).pair(low, high); ok && machineValueMaskedWithin(snapshot, changed) {
		return snapshot, true
	}
	return nil, false
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
	value, _, ok := p.scratchValueAtWithIndices(effects, before, addr)
	return value, ok
}

// scratchRMWValueAt returns a storage-aware scratch snapshot for a bitfield RMW.
func (p *collapseStorageRMWProcessor) scratchRMWValueAt(effects []machine.Effect, before int, scratch machine.MemoryAddress, storage *machine.Load, changed uint) (machine.Value, []int, bool) {
	if !p.syntheticScratchAccess(scratch) {
		return nil, nil, false
	}
	if store, index, ok := p.reachingScratchStore(effects, before, scratch); ok {

		value := p.resolveScratchValue(effects, index, store.Src)

		// collapse-wide-stores commonly represents a low-word value with a
		// zero high word as a uint32 cast. Recover its relationship to the
		// destination storage before treating it as an ordinary wide value.
		if changed>>16 == 0 {
			laneValue := unwrapMachineCasts(value)

			if snapshot, ok := p.collapseSingleScratchLane(laneValue, storage, changed, 0); ok {

				return snapshot, []int{index}, true
			}
		}

		if machineValueMaskedWithin(value, changed) {
			return value, []int{index}, true
		}

		return nil, nil, false
	}
	if scratch.Width != 4 {
		return nil, nil, false
	}
	lowAddr := scratch
	lowAddr.Width = 2
	highAddr := lowAddr
	highAddr.Disp += 2
	low, high, lowIndex, highIndex, ok := p.scratchWordValuesAt(effects, before, lowAddr, highAddr)
	if !ok {
		return nil, nil, false
	}
	snapshot, ok := p.collapseScratchSnapshot(low, high, storage, changed)
	if ok {
		resolved := p.resolveScratchValue(effects, before, snapshot)
		if machineValueMaskedWithin(resolved, changed) {
			snapshot = resolved
		}
	} else {
		resolvedLow := p.resolveScratchValue(effects, lowIndex, low)
		resolvedHigh := p.resolveScratchValue(effects, highIndex, high)

		snapshot, ok = p.collapseScratchSnapshot(resolvedLow, resolvedHigh, storage, changed)
	}

	if !ok {
		return nil, nil, false
	}
	return snapshot, []int{lowIndex, highIndex}, true
}

// scratchValueAtWithIndices returns a reaching scratch value and its defining stores.
func (p *collapseStorageRMWProcessor) scratchValueAtWithIndices(effects []machine.Effect, before int, addr machine.MemoryAddress) (machine.Value, []int, bool) {
	if addr.Width != 4 && !p.syntheticScratchAddress(addr) {
		return nil, nil, false
	}
	if addr.Width == 4 && !p.syntheticScratchAddress(addr) {
		lowAddr := addr
		lowAddr.Width = 2
		highAddr := lowAddr
		highAddr.Disp += 2
		if !p.scratchWordPair(lowAddr, highAddr) {
			return nil, nil, false
		}
	}
	if store, index, ok := p.reachingScratchStore(effects, before, addr); ok {
		return p.resolveScratchValue(effects, index, store.Src), []int{index}, true
	}
	if addr.Width != 4 {
		return nil, nil, false
	}
	lowAddr := addr
	lowAddr.Width = 2
	highAddr := lowAddr
	highAddr.Disp += 2
	lowValue, highValue, lowIndex, highIndex, ok := p.scratchWordValuesAt(effects, before, lowAddr, highAddr)
	if !ok {
		return nil, nil, false
	}
	lowValue = p.resolveScratchValue(effects, lowIndex, lowValue)
	highValue = p.resolveScratchValue(effects, highIndex, highValue)
	value, ok := (&wideMachineCollapser{ctx: p.ctx}).pair(lowValue, highValue)
	if !ok {
		return nil, []int{lowIndex, highIndex}, false
	}
	return value, []int{lowIndex, highIndex}, true
}

// scratchWordValuesAt returns the reaching values and definition indices for adjacent scratch words.
func (p *collapseStorageRMWProcessor) scratchWordValuesAt(effects []machine.Effect, before int, lowAddr, highAddr machine.MemoryAddress) (machine.Value, machine.Value, int, int, bool) {
	low, lowIndex, lowOK := p.reachingScratchStore(effects, before, lowAddr)
	high, highIndex, highOK := p.reachingScratchStore(effects, before, highAddr)
	if !lowOK || !highOK {
		return nil, nil, 0, 0, false
	}
	return low.Src, high.Src,
		lowIndex, highIndex, true
}

// scratchDefinitionAfter reports whether any reaching scratch definition is not before the RMW clear.
func scratchDefinitionAfter(indices []int, clearIndex int) bool {
	for _, index := range indices {
		if index >= clearIndex {
			return true
		}
	}
	return false
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

// collapseSingleScratchLane rebuilds a masked ADD/ADC or SUB/SBB lane as a wide arithmetic update before applying the field mask.
func (p *collapseStorageRMWProcessor) collapseSingleScratchLane(value machine.Value, storage *machine.Load, changed uint, lane int) (machine.Value, bool) {
	and, ok := value.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return nil, false
	}

	mask, unmasked, ok := constOperand(and.LHS, and.RHS)
	if !ok || mask.Val != (changed>>(lane*16))&0xffff {
		return nil, false
	}

	arithmetic, ok := unwrapMachineCasts(unmasked).(*machine.Binary)
	if !ok {
		return nil, false
	}

	var wantPart machine.WordPart
	switch lane {
	case 0:
		wantPart = machine.WordLow

	case 1:
		wantPart = machine.WordHigh

	default:
		return nil, false
	}

	// Low-word-only updates do not need instruction provenance: there is no
	// carry/borrow-consuming lane participating in the field. High-word
	// updates must prove ADC/SBB semantics.
	switch arithmetic.Op {
	case machine.ValueOpAdd:
		if lane == 1 && arithmetic.Producer.InstOp != asm.OpADC {
			return nil, false
		}

	case machine.ValueOpSub:
		if lane == 1 && arithmetic.Producer.InstOp != asm.OpSBB {
			return nil, false
		}

	default:
		return nil, false
	}

	laneAddress := storage.Addr
	laneAddress.Width = 2
	laneAddress.Disp += lane * 2

	delta, ok := p.scratchLaneArithmeticOperands(laneAddress, arithmetic.LHS, arithmetic.RHS, wantPart)

	// Addition is commutative, so the storage load may be either operand.
	if !ok && arithmetic.Op == machine.ValueOpAdd {
		delta, ok = p.scratchLaneArithmeticOperands(laneAddress, arithmetic.RHS, arithmetic.LHS, wantPart)
	}

	if !ok {
		return nil, false
	}

	return machine.BinaryVal(machine.ValueOpAnd,
			machine.BinaryVal(arithmetic.Op, storage, delta),
			machine.ConstVal(changed)),
		true
}

// scratchLaneArithmeticOperands returns the wide delta parent when an arithmetic lane uses the matching storage word.
func (p *collapseStorageRMWProcessor) scratchLaneArithmeticOperands(storage machine.MemoryAddress, storageValue, deltaValue machine.Value, part machine.WordPart) (machine.Value, bool) {
	load, ok := storageValue.(*machine.Load)
	if !ok || !p.ctx.sameResolvedStorage(storage, load.Addr) {
		return nil, false
	}

	if word, ok := deltaValue.(*machine.WordValue); ok {
		if word.Part != part {
			return nil, false
		}
		return word.Parent, true
	}

	// A field wholly contained in the low word can be updated directly
	// from a word-sized operand; there is no high lane to reconstruct.
	if part == machine.WordLow {
		switch deltaValue.(type) {
		case *machine.Load,
			*machine.Const,
			*machine.CallResult,
			*machine.Reg:
			return deltaValue, true
		}
	}

	return nil, false
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

// insertedScratchLoad returns the synthetic scratch address consumed by an OR store.
func (p *collapseStorageRMWProcessor) insertedScratchLoad(insert machine.StoreEffect) (machine.MemoryAddress, bool) {
	or, ok := insert.Src.(*machine.Binary)
	if !ok || or.Op != machine.ValueOpOr {
		return machine.MemoryAddress{}, false
	}
	if scratch, ok := p.destinationScratchOperand(insert.Addr, or.LHS, or.RHS); ok {
		return scratch, true
	}
	return p.destinationScratchOperand(insert.Addr, or.RHS, or.LHS)
}

// destinationScratchOperand matches the destination and synthetic scratch loads of an OR store.
func (p *collapseStorageRMWProcessor) destinationScratchOperand(dst machine.MemoryAddress, dstValue, scratchValue machine.Value) (machine.MemoryAddress, bool) {
	dstLoad, dstOK := dstValue.(*machine.Load)
	if !dstOK || !p.ctx.sameResolvedStorage(dst, dstLoad.Addr) {
		return machine.MemoryAddress{}, false
	}
	scratchLoad, ok := scratchValue.(*machine.Load)
	if !ok || !p.syntheticScratchAccess(scratchLoad.Addr) {
		return machine.MemoryAddress{}, false
	}
	return scratchLoad.Addr, true
}

// syntheticScratchAccess reports whether a word or adjacent-word access is compiler scratch storage.
func (p *collapseStorageRMWProcessor) syntheticScratchAccess(addr machine.MemoryAddress) bool {
	if p.syntheticScratchAddress(addr) {
		return true
	}
	if addr.Width != 4 {
		return false
	}
	low := addr
	low.Width = 2
	high := low
	high.Disp += 2
	return p.scratchWordPair(low, high)
}

// scratchSnapshotRemovable proves that deleting the reaching scratch definitions
// cannot remove a value observed anywhere else in the same scratch lifetime.
func (p *collapseStorageRMWProcessor) scratchSnapshotRemovable(f machine.FuncEffects, b machine.BlockEffects, scratch machine.MemoryAddress, definitions []int, consumer int) bool {
	if len(definitions) == 0 || consumer < 0 || consumer >= len(b.Effects) {
		return false
	}

	firstDefinition := definitions[0]
	ignored := make(map[int]bool, len(definitions)+1)
	for _, index := range definitions {
		if index < 0 || index >= len(b.Effects) || index >= consumer {
			return false
		}
		ignored[index] = true
		if index < firstDefinition {
			firstDefinition = index
		}
	}
	ignored[consumer] = true

	// The value may only be consumed by the RMW insert between its reaching
	// definitions and the intended consumer.
	if p.scratchReadBetweenExcept(b.Effects, scratch, firstDefinition, consumer, ignored) {
		return false
	}

	// Continue liveness from immediately after the consumer.  A later read is
	// relevant only while it can still observe bytes from this lifetime.  Once
	// all bytes have been overwritten, later uses belong to another lifetime.
	written := uint(0)
	if consumer+1 < len(b.Effects) {
		read, nextWritten := p.scratchReadBeforeOverwriteState(b.Effects[consumer+1:], scratch, written)
		if read {
			return false
		}
		written = nextWritten
	}

	full, ok := scratchFullByteMask(scratch.Width)
	if !ok {
		return false
	}
	if written == full {
		return true
	}

	return !p.scratchReadAcrossSuccessorsBeforeOverwrite(f, b.Block, scratch, written)
}

// scratchReadAcrossSuccessorsBeforeOverwrite follows control flow from a block
// while any bytes from the current scratch lifetime remain observable.
func (p *collapseStorageRMWProcessor) scratchReadAcrossSuccessorsBeforeOverwrite(f machine.FuncEffects, from machine.BlockID, scratch machine.MemoryAddress, written uint) bool {
	full, ok := scratchFullByteMask(scratch.Width)
	if !ok {
		return true
	}
	if written == full {
		return false
	}

	blocks := make(map[machine.BlockID]machine.BlockEffects, len(f.Blocks))
	for _, block := range f.Blocks {
		blocks[block.Block] = block
	}

	// Some unit tests build FuncEffects without a CFG.  Preserve conservative
	// behavior there, but still honor overwrites within each candidate block.
	if f.CFG == nil {
		for _, block := range f.Blocks {
			if block.Block == from {
				continue
			}
			read, _ := p.scratchReadBeforeOverwriteState(block.Effects, scratch, written)
			if read {
				return true
			}
		}
		return false
	}

	type state struct {
		block   machine.BlockID
		written uint
	}
	queue := make([]state, 0)
	for _, next := range f.CFG.Successors(from) {
		queue = append(queue, state{block: next, written: written})
	}
	seen := make(map[state]bool)

	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]
		if seen[current] {
			continue
		}
		seen[current] = true

		block, ok := blocks[current.block]
		if !ok {
			// Missing block effects make it unsafe to prove the lifetime dead.
			return true
		}

		read, nextWritten := p.scratchReadBeforeOverwriteState(block.Effects, scratch, current.written)
		if read {
			return true
		}
		if nextWritten == full {
			continue
		}

		for _, next := range f.CFG.Successors(current.block) {
			queue = append(queue, state{block: next, written: nextWritten})
		}
	}

	return false
}

// scratchReadBeforeOverwriteState reports a read of bytes from the current
// scratch lifetime and returns the cumulative set of bytes overwritten.
func (p *collapseStorageRMWProcessor) scratchReadBeforeOverwriteState(effects []machine.Effect, scratch machine.MemoryAddress, written uint) (bool, uint) {
	full, ok := scratchFullByteMask(scratch.Width)
	if !ok {
		return true, written
	}

	for _, effect := range effects {
		if written == full {
			return false, written
		}

		found := false
		rewriter := machineRewriter{
			value: func(w *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
				switch value := value.(type) {
				case *machine.Load:
					found = found || p.scratchReadNeedsValue(scratch, value.Addr, written)
				case *machine.Address:
					found = found || p.scratchReadNeedsValue(scratch, value.Addr, written)
				}
				return nil, false, false
			},
		}
		rewriter.rewriteMachineEffect(effect)
		if found {
			return true, written
		}

		if store, ok := effect.(machine.StoreEffect); ok {
			if mask, ok := p.scratchByteMask(scratch, store.Addr); ok {
				written |= mask
			}
		}
	}

	return false, written
}

// scratchFullByteMask returns the byte mask covering an entire scratch access.
func scratchFullByteMask(width int) (uint, bool) {
	if width <= 0 || width >= 64 {
		return 0, false
	}
	return (uint(1) << width) - 1, true
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
				switch value := value.(type) {
				case *machine.Load:
					found = found || p.scratchStorageOverlaps(scratch, value.Addr)
				case *machine.Address:
					found = found || p.scratchStorageOverlaps(scratch, value.Addr)
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

// scratchReadNeedsValue reports whether a scratch read includes a byte not yet overwritten.
func (p *collapseStorageRMWProcessor) scratchReadNeedsValue(scratch, read machine.MemoryAddress, written uint) bool {
	mask, ok := p.scratchByteMask(scratch, read)
	return ok && mask&^written != 0
}

// scratchByteMask returns the bytes of a scratch range touched by an access.
func (p *collapseStorageRMWProcessor) scratchByteMask(scratch, access machine.MemoryAddress) (uint, bool) {
	if scratch.Width <= 0 || access.Width <= 0 {
		return 0, false
	}
	scratchResolved, ok := p.ctx.symbols.addressFromMemory(scratch, nil)
	if !ok {
		return 0, false
	}
	accessResolved, ok := p.ctx.symbols.addressFromMemory(access, nil)
	if !ok || !p.ctx.symbols.sameResolvedAddressBase(scratchResolved, accessResolved) {
		return 0, false
	}
	if _, ok := scratchResolved.base.(*symresolve.SymbolScratch); !ok {
		return 0, false
	}
	if _, ok := accessResolved.base.(*symresolve.SymbolScratch); !ok {
		return 0, false
	}
	scratchStart := resolvedAddressPhysicalOffset(scratchResolved)
	accessStart := resolvedAddressPhysicalOffset(accessResolved)
	mask := uint(0)
	for i := 0; i < access.Width; i++ {
		offset := accessStart + i - scratchStart
		if offset >= 0 && offset < scratch.Width {
			mask |= 1 << offset
		}
	}
	return mask, mask != 0
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

// scratchReadBetweenExcept reports whether scratch is read in [start, end],
// excluding explicitly ignored effects.
func (p *collapseStorageRMWProcessor) scratchReadBetweenExcept(effects []machine.Effect, scratch machine.MemoryAddress, start, end int, ignored map[int]bool) bool {
	if start < 0 {
		start = 0
	}
	if end >= len(effects) {
		end = len(effects) - 1
	}

	for i := start; i <= end; i++ {
		if ignored[i] {
			continue
		}

		found := false
		rewriter := machineRewriter{
			value: func(w *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
				switch value := value.(type) {
				case *machine.Load:
					found = found || p.scratchStorageOverlaps(scratch, value.Addr)

				case *machine.Address:
					found = found || p.scratchStorageOverlaps(scratch, value.Addr)
				}

				return nil, false, false
			},
		}

		rewriter.rewriteMachineEffect(effects[i])

		if found {
			return true
		}
	}

	return false
}

// machineConstIsZero reports whether a machine value is the constant zero.
func machineConstIsZero(value machine.Value) bool {
	constant, ok := value.(*machine.Const)
	return ok && constant.Val == 0
}
