package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// wideMachineCollapser reconstructs logical 32-bit values from paired low and
// high 16-bit machine lanes.
type wideMachineCollapser struct {
	ctx *FuncContext
}

// pair reconstructs a logical wide value from low and high machine lanes.
func (c *wideMachineCollapser) pair(low machine.Value, high machine.Value) (machine.Value, bool) {
	if value, ok := c.pairDirect(low, high); ok {
		return value, true
	}
	if value, ok := c.pairPointerOffset(low, high); ok {
		return value, true
	}
	if value, ok := c.pairPhi(low, high); ok {
		return value, true
	}
	return c.pairBinary(low, high)
}

// pairPointerOffset reconstructs a wide pointer whose low word carries
// additional offset arithmetic while its high word remains unchanged.
func (c *wideMachineCollapser) pairPointerOffset(low machine.Value, high machine.Value) (machine.Value, bool) {
	binary, ok := low.(*machine.Binary)
	if !ok || (binary.Op != machine.ValueOpAdd && binary.Op != machine.ValueOpSub) {
		return nil, false
	}
	if base, ok := c.pairDirect(binary.LHS, high); ok {
		if !c.pointerValue(base) {
			return nil, false
		}
		next := *binary
		next.LHS = base
		return &next, true
	}
	if base, ok := c.pairPointerOffset(binary.LHS, high); ok {
		next := *binary
		next.LHS = base
		return &next, true
	}
	if binary.Op == machine.ValueOpAdd {
		if base, ok := c.pairDirect(binary.RHS, high); ok {
			if !c.pointerValue(base) {
				return nil, false
			}
			next := *binary
			next.RHS = base
			return &next, true
		}
		if base, ok := c.pairPointerOffset(binary.RHS, high); ok {
			next := *binary
			next.RHS = base
			return &next, true
		}
	}
	return nil, false
}

// pointerValue reports whether a reconstructed machine value denotes a declared pointer.
func (c *wideMachineCollapser) pointerValue(value machine.Value) bool {
	if typ := machineValueType(value); typeinfo.IsPointer(typ) {
		return true
	}
	if c.ctx == nil {
		return false
	}
	path, ok := c.ctx.symbols.symbolFromValue(value)
	if ok && typeinfo.IsPointer(path.Type()) {
		return true
	}
	load, ok := value.(*machine.Load)
	if !ok {
		return false
	}
	addr, ok := c.ctx.symbols.addressFromMemory(load.Addr, nil)
	if !ok {
		return false
	}
	_, scratch := addr.base.(*symresolve.SymbolScratch)
	return scratch
}

// pairDirect handles primitive lane representations that directly identify a
// single wide parent value.
func (c *wideMachineCollapser) pairDirect(low machine.Value, high machine.Value) (machine.Value, bool) {
	if lowWord, ok := low.(*machine.WordValue); ok && lowWord.Part == machine.WordLow {
		if highWord, ok := high.(*machine.WordValue); ok &&
			highWord.Part == machine.WordHigh &&
			c.sameParent(lowWord.Parent, highWord.Parent) {
			return lowWord.Parent, true
		}
	}

	if highWord, ok := high.(*machine.WordValue); ok {
		switch highWord.Part {
		case machine.WordHigh:
			if c.sameParent(low, highWord.Parent) {
				return low, true
			}
		case machine.WordSignHigh:
			if machine.ValueEquals(low, highWord.Parent) {
				return machine.SignExtendVal(low, 16, 32), true
			}
		}
	}

	if lowConst, ok := low.(*machine.Const); ok {
		if highConst, ok := high.(*machine.Const); ok {
			return &machine.Const{
				Val:    ((highConst.Val & 0xffff) << 16) | (lowConst.Val & 0xffff),
				Origin: lowConst.Origin,
			}, true
		}
	}

	if highConst, ok := high.(*machine.Const); ok && highConst.Val == 0 {
		return machine.CastVal(low, typeinfo.U32), true
	}

	if lowLoad, ok := low.(*machine.Load); ok {
		if highLoad, ok := high.(*machine.Load); ok {
			return c.pairLoads(lowLoad, highLoad)
		}
	}

	if lowPointer, ok := low.(*machine.FarPointer); ok && lowPointer.Part == machine.FarPointerOffset {
		if highPointer, ok := high.(*machine.FarPointer); ok && highPointer.Part == machine.FarPointerSegment {
			if c.sameParent(lowPointer.Parent, highPointer.Parent) {
				return lowPointer.Parent, true
			}
			if parent, ok := c.pair(lowPointer.Parent, highPointer.Parent); ok {
				return parent, true
			}
		}
	}

	if segment, ok := high.(*machine.Reg); ok && segment.Val.IsSeg() {
		return machine.AddressVal(machine.MemoryAddress{Seg: high, Base: low, Width: 4}), true
	}

	return nil, false
}

// sameParent reports whether two projected parents are structurally equal or
// are resolver-equivalent loads of the same physical storage.
func (c *wideMachineCollapser) sameParent(a machine.Value, b machine.Value) bool {
	if c.ctx == nil {
		return machine.ValueEquals(a, b)
	}
	return c.ctx.symbols.sameResolvedMachineValue(a, b)
}

// pairLoads rebuilds one dword load after the resolver proves that its word
// loads name adjacent lanes of the same normalized storage.
func (c *wideMachineCollapser) pairLoads(low *machine.Load, high *machine.Load) (machine.Value, bool) {
	if c.ctx == nil {
		return nil, false
	}
	if _, ok := c.ctx.symbols.adjacentResolvedStorage(low.Addr, high.Addr); !ok {
		return nil, false
	}
	wide := *low
	wide.Addr.Width = 4
	return &wide, true
}

// pairPhi combines matching low/high merge arms recursively.
func (c *wideMachineCollapser) pairPhi(low machine.Value, high machine.Value) (machine.Value, bool) {
	lowPhi, lowOK := low.(*machine.PhiValue)
	highPhi, highOK := high.(*machine.PhiValue)
	if !lowOK || !highOK || lowPhi.Join != highPhi.Join || len(lowPhi.Arms) != len(highPhi.Arms) {
		return nil, false
	}

	arms := make([]machine.PhiArm, len(lowPhi.Arms))
	for i := range lowPhi.Arms {
		if lowPhi.Arms[i].Block != highPhi.Arms[i].Block {
			return nil, false
		}
		value, ok := c.pair(lowPhi.Arms[i].Value, highPhi.Arms[i].Value)
		if !ok {
			return nil, false
		}
		arms[i] = lowPhi.Arms[i]
		arms[i].Value = value
	}
	return &machine.PhiValue{Join: lowPhi.Join, Arms: arms}, true
}

// pairBinary combines lane-wise bit operations and provenance-proven
// carry-aware arithmetic recursively.
func (c *wideMachineCollapser) pairBinary(low machine.Value, high machine.Value) (machine.Value, bool) {
	lowBinary, lowOK := low.(*machine.Binary)
	highBinary, highOK := high.(*machine.Binary)
	if !lowOK || !highOK || lowBinary.Op != highBinary.Op {
		return nil, false
	}
	switch lowBinary.Op {
	case machine.ValueOpAnd, machine.ValueOpOr, machine.ValueOpXor:
	case machine.ValueOpAdd:
		if !adjacentWideArithmeticInstructions(lowBinary.Producer, highBinary.Producer, asm.OpADD, asm.OpADC) {
			return nil, false
		}
	case machine.ValueOpSub:
		if !adjacentWideArithmeticInstructions(lowBinary.Producer, highBinary.Producer, asm.OpSUB, asm.OpSBB) {
			return nil, false
		}
	default:
		return nil, false
	}

	lhs, ok := c.pair(lowBinary.LHS, highBinary.LHS)
	if !ok {
		return nil, false
	}
	rhs, ok := c.pair(lowBinary.RHS, highBinary.RHS)
	if !ok {
		return nil, false
	}
	return machine.BinaryVal(lowBinary.Op, lhs, rhs), true
}
