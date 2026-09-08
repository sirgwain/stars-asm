package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
)

// machineBitfieldRead describes a physical bit range extracted from a storage load.
type machineBitfieldRead struct {
	Load     *machine.Load
	BitOff   int
	BitWidth int
}

// machineBitfieldWrite describes a physical bit range written to storage.
type machineBitfieldWrite struct {
	Load     *machine.Load
	BitOff   int
	BitWidth int
	Value    machine.Value
}

// recognizeBitfieldRead recognizes a machine mask-and-shift bitfield extraction.
func recognizeBitfieldRead(ctx *FuncContext, value machine.Value) (machineBitfieldRead, bool) {
	load, bitOff, bitWidth, ok := bitfieldExtract(ctx, value)
	if !ok {
		return machineBitfieldRead{}, false
	}
	return machineBitfieldRead{Load: load, BitOff: bitOff, BitWidth: bitWidth}, true
}

// recognizeBitfieldWrite recognizes a destination-preserving machine bitfield write.
func recognizeBitfieldWrite(ctx *FuncContext, mem machine.MemoryAddress, value machine.Value) (machineBitfieldWrite, bool) {
	load, bitOff, bitWidth, stored, ok := bitfieldStore(mem, value, ctx.symbols.sameResolvedStorage)
	if !ok {
		return machineBitfieldWrite{}, false
	}
	return machineBitfieldWrite{Load: load, BitOff: bitOff, BitWidth: bitWidth, Value: stored}, true
}

// symbolFromAddressTermValue resolves a machine bitfield used as an address
// component through semantic projection before trying ordinary symbols.
func (sr *symbolResolver) symbolFromAddressTermValue(value machine.Value) (symresolve.SymbolPath, bool) {
	if bitfield, ok := recognizeBitfieldRead(sr.FuncContext, value); ok {
		converter := &machineConverter{ctx: sr.FuncContext}
		if field, ok := converter.resolveBitfieldLValue(bitfield.Load.Addr, bitfield.BitOff, bitfield.BitWidth); ok {
			if path, ok := symbolPathForExpr(field); ok {
				return path, true
			}
		}
	}
	return sr.symbolFromValue(value)
}

// bitfieldExtract returns the storage load, bit offset, and bit width for a bitfield expression.
func bitfieldExtract(ctx *FuncContext, value machine.Value) (*machine.Load, int, int, bool) {
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
	load, bitOff, ok := shiftedLoad(ctx, source)
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
	stored, ok := unshiftMachineBitfieldSet(mem, set, bitOff, bitWidth, changed, same)
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
func unshiftMachineBitfieldSet(mem machine.MemoryAddress, value machine.Value, bitOff int, bitWidth int, changed uint, same func(machine.MemoryAddress, machine.MemoryAddress) bool) (machine.Value, bool) {
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
	if arithmetic, ok := unshiftMachineBitfieldArithmetic(mem, value, bitOff, bitWidth, changed, same); ok {
		return arithmetic, true
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

// unshiftMachineBitfieldArithmetic recovers a field-relative add or subtract from a masked wide storage calculation.
func unshiftMachineBitfieldArithmetic(mem machine.MemoryAddress, value machine.Value, bitOff int, bitWidth int, changed uint, same func(machine.MemoryAddress, machine.MemoryAddress) bool) (machine.Value, bool) {
	and, ok := unwrapMachineCasts(value).(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return nil, false
	}
	mask, unmasked, ok := constOperand(and.LHS, and.RHS)
	if !ok || mask.Val != changed {
		return nil, false
	}
	arithmetic, ok := unwrapMachineCasts(unmasked).(*machine.Binary)
	if !ok || (arithmetic.Op != machine.ValueOpAdd && arithmetic.Op != machine.ValueOpSub) {
		return nil, false
	}

	field, lhsStorage := unshiftMachineBitfieldStorageOperand(mem, arithmetic.LHS, bitOff, bitWidth, same)
	delta, rhsShifted := unshiftMachineBitfieldShiftedOperand(arithmetic.RHS, bitOff)
	if lhsStorage && rhsShifted {
		return machine.BinaryVal(arithmetic.Op, field, delta), true
	}
	if arithmetic.Op == machine.ValueOpAdd {
		field, rhsStorage := unshiftMachineBitfieldStorageOperand(mem, arithmetic.RHS, bitOff, bitWidth, same)
		delta, lhsShifted := unshiftMachineBitfieldShiftedOperand(arithmetic.LHS, bitOff)
		if rhsStorage && lhsShifted {
			return machine.BinaryVal(machine.ValueOpAdd, field, delta), true
		}
	}
	return nil, false
}

// unshiftMachineBitfieldStorageOperand converts a full storage load to a masked field read.
func unshiftMachineBitfieldStorageOperand(mem machine.MemoryAddress, value machine.Value, bitOff int, bitWidth int, same func(machine.MemoryAddress, machine.MemoryAddress) bool) (machine.Value, bool) {
	load, ok := unwrapMachineCasts(value).(*machine.Load)
	if !ok || !same(mem, load.Addr) {
		return nil, false
	}
	mask, ok := bitMask(bitWidth)
	if !ok {
		return nil, false
	}
	shifted := machine.Value(load)
	if bitOff != 0 {
		shifted = machine.BinaryVal(machine.ValueOpShr, shifted, machine.ConstVal(uint(bitOff)))
	}
	return machine.BinaryVal(machine.ValueOpAnd, shifted, machine.ConstVal(mask)), true
}

// unshiftMachineBitfieldShiftedOperand removes the field-position shift and representation-only integer widening.
func unshiftMachineBitfieldShiftedOperand(value machine.Value, bitOff int) (machine.Value, bool) {
	value = unwrapMachineCasts(value)
	if shift, ok := value.(*machine.Binary); ok && shift.Op == machine.ValueOpShl {
		amount, ok := shift.RHS.(*machine.Const)
		if !ok || int(amount.Val) != bitOff {
			return nil, false
		}
		value = unwrapMachineCasts(shift.LHS)
	} else if bitOff != 0 {
		return nil, false
	}
	if extended, ok := value.(*machine.SignExtendValue); ok {
		value = extended.Parent
	}
	return value, true
}

// unwrapMachineCasts removes representation-only machine casts.
func unwrapMachineCasts(value machine.Value) machine.Value {
	for {
		cast, ok := value.(*machine.Cast)
		if !ok {
			return value
		}
		value = cast.Value
	}
}

// shiftedLoad returns the load and right-shift amount for a bitfield source.
func shiftedLoad(ctx *FuncContext, value machine.Value) (*machine.Load, int, bool) {
	value = unwrapMachineBitfieldValue(value)
	bitOff := 0
	for {
		shift, ok := value.(*machine.Binary)
		if !ok || (shift.Op != machine.ValueOpShr && shift.Op != machine.ValueOpSar) {
			break
		}
		amount, ok := shift.RHS.(*machine.Const)
		if !ok {
			return nil, 0, false
		}
		bitOff += int(amount.Val)
		value = unwrapMachineBitfieldValue(shift.LHS)
	}
	load, ok := machineBitfieldStorageLoad(ctx, value)
	if !ok {
		return nil, 0, false
	}
	return load, bitOff, true
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
func machineBitfieldStorageLoad(ctx *FuncContext, value machine.Value) (*machine.Load, bool) {
	if load, ok := value.(*machine.Load); ok {
		return load, true
	}
	words, ok := value.(*machine.StackWords)
	if !ok || len(words.Words) != 2 {
		return nil, false
	}
	wide, ok := (&wideMachineCollapser{ctx: ctx}).pair(words.Words[1], words.Words[0])
	if !ok {
		return nil, false
	}
	load, ok := wide.(*machine.Load)
	return load, ok
}
