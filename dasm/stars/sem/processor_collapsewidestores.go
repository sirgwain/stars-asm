package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
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
	if !wideStoreDestinationsMatch(p.ctx, low.Addr, high.Addr) {
		return low, false
	}

	if src, ok := p.collapseWideStoreSource(low.Src, high.Src); ok {
		low.Src = src
		low.Width = 4
		low.Addr.Width = 4
		return low, true
	}

	if src, ok := p.collapseWidePointerOffsetStoreSource(low, high); ok {
		low.Src = src
		low.Width = 4
		low.Addr.Width = 4
		return low, true
	}

	if src, ok := p.collapseWideMaskedStoreSource(low, high); ok {
		low.Src = src
		low.Width = 4
		low.Addr.Width = 4
		return low, true
	}

	if src, ok := p.collapseWideBinaryStoreSource(low, high); ok {
		low.Src = src
		low.Width = 4
		low.Addr.Width = 4
		return low, true
	}

	if src, ok := p.collapseWideReadModifyWriteSource(low, high); ok {
		low.Src = src
		low.Width = 4
		low.Addr.Width = 4
		return low, true
	}

	return low, false
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
	if base, ok := collapseWidePointerBase(binary.LHS, high.Src); ok {
		return machine.BinaryVal(binary.Op, base, binary.RHS), true
	}
	if binary.Op == machine.ValueOpAdd {
		if base, ok := collapseWidePointerBase(binary.RHS, high.Src); ok {
			return machine.BinaryVal(binary.Op, base, binary.LHS), true
		}
	}
	return nil, false
}

// collapseWidePointerBase combines the low and high words of one pointer base.
func collapseWidePointerBase(low, high machine.Value) (machine.Value, bool) {
	if lowLoad, ok := low.(*machine.Load); ok {
		if highLoad, ok := high.(*machine.Load); ok && contiguousMachineMemoryPair(lowLoad.Addr, highLoad.Addr) {
			wide := *lowLoad
			wide.Addr.Width = 4
			return &wide, true
		}
	}
	lowWord, lowOK := low.(*machine.WordValue)
	highWord, highOK := high.(*machine.WordValue)
	if highOK && highWord.Part == machine.WordHigh && machine.ValueEquals(low, highWord.Parent) {
		return low, true
	}
	if lowOK && highOK && lowWord.Part == machine.WordLow && highWord.Part == machine.WordHigh && machine.ValueEquals(lowWord.Parent, highWord.Parent) {
		return lowWord.Parent, true
	}
	return nil, false
}

// collapseWideBinaryStoreSource reconstructs one wide binary source from
// low/high binary stores whose operands each describe contiguous word pairs.
func (p *collapseWideStoresProcessor) collapseWideBinaryStoreSource(low, high machine.StoreEffect) (machine.Value, bool) {
	lowBin, ok := low.Src.(*machine.Binary)
	if !ok {
		return nil, false
	}
	highBin, ok := high.Src.(*machine.Binary)
	if !ok || highBin.Op != lowBin.Op || !wideStoreBinaryOp(low, high, lowBin.Op) {
		return nil, false
	}
	wideLHS, ok := collapseWideReadModifyWritePart(lowBin.LHS, highBin.LHS)
	if !ok {
		return nil, false
	}
	wideRHS, ok := collapseWideReadModifyWritePart(lowBin.RHS, highBin.RHS)
	if !ok {
		return nil, false
	}
	return machine.BinaryVal(lowBin.Op, wideLHS, wideRHS), true
}

// wideStoreDestinationsMatch reports whether two word stores target the low
// and high halves of the same storage location.
func wideStoreDestinationsMatch(ctx *FuncContext, low machine.MemoryAddress, high machine.MemoryAddress) bool {
	if !contiguousMachineMemoryPair(low, high) {
		return false
	}
	wide := low
	wide.Width = 4
	resolved, ok := ctx.symbols.addressFromMemory(wide, nil)
	if !ok {
		return false
	}
	path, ok := resolved.path()
	return ok && symbolPathHasDeclaredRoot(path) && path.Type() != nil && path.Type().Bytes() >= 4
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

// contiguousMachineMemoryPair reports whether high is the next word after low,
// comparing machine address shape without origin or access width.
func contiguousMachineMemoryPair(low machine.MemoryAddress, high machine.MemoryAddress) bool {
	return low.Width == 2 &&
		high.Width == 2 &&
		high.Disp-low.Disp == 2 &&
		machine.ValueEquals(low.Seg, high.Seg) &&
		machine.ValueEquals(low.Base, high.Base) &&
		machine.ValueEquals(low.Index, high.Index)
}

// collapseWideStoreSource reconstructs a 32-bit source from simple paired
// low/high word source values.
func (p *collapseWideStoresProcessor) collapseWideStoreSource(low machine.Value, high machine.Value) (machine.Value, bool) {
	if phi, ok := collapseWideMachinePhiPair(high, low); ok {
		return phi, true
	}
	if hiword, ok := high.(*machine.Reg); ok && hiword.Val.IsSeg() {
		addr := machine.AddressVal(machine.MemoryAddress{
			Seg:   high,
			Base:  low,
			Width: 4,
		})
		// p.ctx.log.Debug("addr", "addr", addr, "dump", machine.DumpValue(addr))
		return addr, true
	}
	if loword, ok := low.(*machine.WordValue); ok && loword.Part == machine.WordLow {
		if hiword, ok := high.(*machine.WordValue); ok && hiword.Part == machine.WordHigh && machine.ValueEquals(loword.Parent, hiword.Parent) {
			return loword.Parent, true
		}
	}
	if loword, ok := low.(*machine.Const); ok {
		if hiword, ok := high.(*machine.Const); ok {
			return machine.ConstVal(hiword.Val<<16 | loword.Val), true
		}
	}
	if hiword, ok := high.(*machine.Const); ok && hiword.Val == 0 {
		return machine.CastVal(low, typeinfo.U32), true
	}
	if signHigh, ok := high.(*machine.WordValue); ok && signHigh.Part == machine.WordSignHigh && machine.ValueEquals(signHigh.Parent, low) {
		return machine.SignExtendVal(low, 16, 32), true
	}
	if faroff, ok := low.(*machine.FarPointer); ok && faroff.Part == machine.FarPointerOffset {
		if farseg, ok := high.(*machine.FarPointer); ok && farseg.Part == machine.FarPointerSegment {
			// single far pointer store
			return faroff.Parent, true
		}
	}
	if lowLoad, ok := low.(*machine.Load); ok {
		if highLoad, ok := high.(*machine.Load); ok && contiguousMachineMemoryPair(lowLoad.Addr, highLoad.Addr) {
			// single far pointer store
			wide := *lowLoad
			wide.Addr.Width = 4
			return &wide, true
		}
	}
	return nil, false
}

// collapseWideMachinePhiPair reconstructs one wide merge from matching high
// and low merge arms before semantic merge temporaries are introduced.
func collapseWideMachinePhiPair(high, low machine.Value) (machine.Value, bool) {
	highPhi, highOK := high.(*machine.PhiValue)
	lowPhi, lowOK := low.(*machine.PhiValue)
	if !highOK || !lowOK || highPhi.Join != lowPhi.Join || len(highPhi.Arms) != len(lowPhi.Arms) {
		return nil, false
	}
	arms := make([]machine.PhiArm, len(lowPhi.Arms))
	for i := range lowPhi.Arms {
		if lowPhi.Arms[i].Block != highPhi.Arms[i].Block {
			return nil, false
		}
		value, ok := collapseWideMachinePair(highPhi.Arms[i].Value, lowPhi.Arms[i].Value)
		if !ok {
			return nil, false
		}
		arms[i] = lowPhi.Arms[i]
		arms[i].Value = value
	}
	return &machine.PhiValue{Join: lowPhi.Join, Arms: arms}, true
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
	set, ok := collapseWideReadModifyWritePart(lowSet, highSet)
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
	field, _, ok := p.ctx.symbols.symbolFromBitfieldStore(wideDestination, wideSource)
	if !ok || field.Field.Bitfield.StorageSize != wideDestination.Width {
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
// from one half of a paired masked store.
func (p *collapseWideStoresProcessor) maskedMachineWordKeep(dst machine.MemoryAddress, source machine.Value) (*machine.Load, uint, bool) {
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

// collapseWideReadModifyWriteSource reconstructs a wide binary store such as
// store A = load(A) ^ loword(x); store A+2 = load(A+2) ^ hiword(x).
func (p *collapseWideStoresProcessor) collapseWideReadModifyWriteSource(low machine.StoreEffect, high machine.StoreEffect) (machine.Value, bool) {
	lowBin, ok := low.Src.(*machine.Binary)
	if !ok {
		return nil, false
	}
	highBin, ok := high.Src.(*machine.Binary)
	if !ok || highBin.Op != lowBin.Op || !wideStoreBinaryOp(low, high, lowBin.Op) {
		return nil, false
	}

	lowLoad, lowPart, ok := splitReadModifyWriteWord(lowBin, low.Addr, machine.WordLow)
	if !ok {
		return nil, false
	}
	highLoad, highPart, ok := splitReadModifyWriteWord(highBin, high.Addr, machine.WordHigh)
	if !ok || !contiguousMachineMemoryPair(lowLoad.Addr, highLoad.Addr) {
		return nil, false
	}

	widePart, ok := collapseWideReadModifyWritePart(lowPart, highPart)
	if !ok {
		return nil, false
	}

	wideLoad := *lowLoad
	wideLoad.Addr.Width = 4
	return machine.BinaryVal(lowBin.Op, &wideLoad, widePart), true
}

// splitReadModifyWriteWord returns the load and added word-level value from
// one half of a read-modify-write binary expression.
func splitReadModifyWriteWord(bin *machine.Binary, mem machine.MemoryAddress, part machine.WordPart) (*machine.Load, machine.Value, bool) {
	if load, word, ok := readModifyWriteOperands(bin.LHS, bin.RHS, mem, part); ok {
		return load, word, true
	}
	return readModifyWriteOperands(bin.RHS, bin.LHS, mem, part)
}

// readModifyWriteOperands matches load(addr) op a word-level source value.
func readModifyWriteOperands(loadValue machine.Value, wordValue machine.Value, mem machine.MemoryAddress, part machine.WordPart) (*machine.Load, machine.Value, bool) {
	load, ok := loadValue.(*machine.Load)
	if !ok || !sameMachineMemoryAccess(load.Addr, mem) {
		return nil, nil, false
	}
	if !isReadModifyWriteWordSource(wordValue, part) {
		return nil, nil, false
	}
	return load, wordValue, true
}

// isReadModifyWriteWordSource reports whether a value can represent one
// 16-bit half of a wide read-modify-write source.
func isReadModifyWriteWordSource(value machine.Value, part machine.WordPart) bool {
	switch value := value.(type) {
	case *machine.WordValue:
		return value.Part == part || (part == machine.WordHigh && value.Part == machine.WordSignHigh)
	case *machine.Load:
		return value.Addr.Width == 2
	case *machine.Const:
		return true
	case *machine.CallResult, *machine.Binary, *machine.Cast, *machine.SignExtendValue:
		return part == machine.WordLow
	default:
		return false
	}
}

// collapseWideReadModifyWritePart rebuilds a wide source from matching
// low/high constants, word projections, or contiguous low/high loads.
func collapseWideReadModifyWritePart(low, high machine.Value) (machine.Value, bool) {
	if lowConst, ok := low.(*machine.Const); ok {
		if highConst, ok := high.(*machine.Const); ok {
			return machine.ConstVal((highConst.Val&0xffff)<<16 | lowConst.Val&0xffff), true
		}
	}

	if lowWord, ok := low.(*machine.WordValue); ok {
		highWord, highOK := high.(*machine.WordValue)
		if highOK && lowWord.Part == machine.WordLow && highWord.Part == machine.WordHigh && machine.ValueEquals(lowWord.Parent, highWord.Parent) {
			return lowWord.Parent, true
		}
	}
	if highWord, ok := high.(*machine.WordValue); ok && highWord.Part == machine.WordSignHigh && machine.ValueEquals(low, highWord.Parent) {
		return machine.SignExtendVal(low, 16, 32), true
	}

	lowLoad, lowOK := low.(*machine.Load)
	highLoad, highOK := high.(*machine.Load)
	if !lowOK || !highOK || !contiguousMachineMemoryPair(lowLoad.Addr, highLoad.Addr) {
		return nil, false
	}

	wideLoad := *lowLoad
	wideLoad.Addr.Width = 4
	return &wideLoad, true
}

// sameMachineMemoryAccess compares memory access shape while ignoring origin.
func sameMachineMemoryAccess(a machine.MemoryAddress, b machine.MemoryAddress) bool {
	return a.Width == b.Width &&
		a.Disp == b.Disp &&
		machine.ValueEquals(a.Seg, b.Seg) &&
		machine.ValueEquals(a.Base, b.Base) &&
		machine.ValueEquals(a.Index, b.Index)
}

// wideStoreBinaryOp reports whether a binary op can be reconstructed from
// paired word-level read-modify-write halves.
func wideStoreBinaryOp(low, high machine.StoreEffect, op machine.ValueOp) bool {
	switch op {
	case machine.ValueOpAnd, machine.ValueOpOr, machine.ValueOpXor:
		return true
	case machine.ValueOpAdd:
		return adjacentWideArithmeticInstructions(low.MetaInfo, high.MetaInfo, asm.OpADD, asm.OpADC) ||
			pairedRegisterArithmeticSources(low, high)
	case machine.ValueOpSub:
		return adjacentWideArithmeticInstructions(low.MetaInfo, high.MetaInfo, asm.OpSUB, asm.OpSBB) ||
			pairedRegisterArithmeticSources(low, high)
	default:
		return false
	}
}

// pairedRegisterArithmeticSources reports whether two MOV stores capture the
// low/high results of a register-level wide arithmetic operation. The store
// metadata names the MOV instructions, so the arithmetic producer must be
// identified from the two contiguous load pairs in the source expressions.
func pairedRegisterArithmeticSources(low, high machine.StoreEffect) bool {
	if low.MetaInfo.InstOp != asm.OpMOV || high.MetaInfo.InstOp != asm.OpMOV {
		return false
	}
	lowBin, ok := low.Src.(*machine.Binary)
	if !ok {
		return false
	}
	highBin, ok := high.Src.(*machine.Binary)
	if !ok || highBin.Op != lowBin.Op {
		return false
	}
	_, lhsOK := collapseWideMachinePair(highBin.LHS, lowBin.LHS)
	_, rhsOK := collapseWideMachinePair(highBin.RHS, lowBin.RHS)
	return lhsOK && rhsOK
}

// adjacentWideArithmeticInstructions reports whether two effects came from an
// adjacent low-word operation and its carry-consuming high-word operation.
func adjacentWideArithmeticInstructions(low, high machine.Meta, lowOp, highOp asm.Op) bool {
	return low.InstLen > 0 &&
		low.InstOff+uint32(low.InstLen) == high.InstOff &&
		low.InstOp == lowOp &&
		high.InstOp == highOp
}
