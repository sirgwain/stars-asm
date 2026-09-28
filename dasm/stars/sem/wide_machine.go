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
	if value, ok := c.pairNeg(low, high); ok {
		return value, true
	}
	return c.pairBinary(low, high)
}

// pairNeg reconstructs a 32-bit negate lowered as NEG low; ADC high, 0;
// NEG high. Unary NEG is represented as Binary(NEG, value, 0), and the ADC
// carries the low-word NEG borrow into the high word.
func (c *wideMachineCollapser) pairNeg(low machine.Value, high machine.Value) (machine.Value, bool) {
	lowNeg, lowOK := low.(*machine.Binary)
	highNeg, highOK := high.(*machine.Binary)
	if !lowOK || !highOK || lowNeg.Op != machine.ValueOpNeg || highNeg.Op != machine.ValueOpNeg {
		return nil, false
	}
	if !machineConstIsZero(lowNeg.RHS) || !machineConstIsZero(highNeg.RHS) {
		return nil, false
	}
	highAdjust, ok := highNeg.LHS.(*machine.Binary)
	if !ok || highAdjust.Op != machine.ValueOpAdd || !machineConstIsZero(highAdjust.RHS) {
		return nil, false
	}
	if !machine.AdjacentWideArithmetic(lowNeg.Producer, highAdjust.Producer, asm.OpNEG, asm.OpADC) ||
		!machine.AdjacentWideArithmetic(highAdjust.Producer, highNeg.Producer, asm.OpADC, asm.OpNEG) {
		return nil, false
	}
	source, ok := c.pair(lowNeg.LHS, highAdjust.LHS)
	if !ok {
		return nil, false
	}
	return machine.BinaryVal(machine.ValueOpNeg, source, machine.ConstVal(0)), true
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
			if lowConst.Fixup != nil || highConst.Fixup != nil {
				// A relocated segment:offset pair names a far symbol such as
				// a window procedure; folding it into one number would drop
				// the fixups that identify it.
				return &machine.StackWords{Words: []machine.Value{highConst, lowConst}}, true
			}
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
	// An OR or XOR with zero folds away on its own word, as in
	// (int32_t)x | 0x80000000 lowered to OR ax, 0 / OR dx, 0x8000; restore
	// the zero so both words carry the same operation.
	if highOK && !lowOK && identityZeroOperand(highBinary) {
		lowBinary, lowOK = &machine.Binary{Op: highBinary.Op, LHS: low, RHS: machine.ConstVal(0)}, true
	} else if lowOK && !highOK && identityZeroOperand(lowBinary) {
		highBinary, highOK = &machine.Binary{Op: lowBinary.Op, LHS: high, RHS: machine.ConstVal(0)}, true
	}
	if !lowOK || !highOK || lowBinary.Op != highBinary.Op {
		return nil, false
	}
	switch lowBinary.Op {
	case machine.ValueOpAnd, machine.ValueOpOr, machine.ValueOpXor, machine.ValueOpNot:
	case machine.ValueOpAdd:
		if !machine.AdjacentWideArithmetic(lowBinary.Producer, highBinary.Producer, asm.OpADD, asm.OpADC) {
			return nil, false
		}
	case machine.ValueOpSub:
		if !machine.AdjacentWideArithmetic(lowBinary.Producer, highBinary.Producer, asm.OpSUB, asm.OpSBB) {
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

// identityZeroOperand reports whether binary is an OR or XOR with a constant
// right operand, whose other word may have folded an identity zero away.
func identityZeroOperand(binary *machine.Binary) bool {
	if binary.Op != machine.ValueOpOr && binary.Op != machine.ValueOpXor {
		return false
	}
	_, ok := binary.RHS.(*machine.Const)
	return ok
}

// scalarTypeAtOffset resolves a fixed storage offset through
// nested structs and arrays and returns only the exact leaf type it selects.
func (ctx *FuncContext) scalarTypeAtOffset(base symresolve.SymbolPath, offset int) (typeinfo.Type, bool) {
	if offset < 0 {
		return nil, false
	}
	path := base
	for {
		typ := path.Type()
		if offset == 0 && typ.Bytes() == 4 && (typ.Kind() == typeinfo.KInt || typ.Kind() == typeinfo.KPointer) {
			return typ, true
		}
		switch aggregate := typ.(type) {
		case *typeinfo.Pointer:
			if aggregate.Elem == nil {
				return nil, false
			}
			path = &symresolve.SymbolDeref{Base: path}
		case *typeinfo.Array:
			if aggregate.Elem == nil || aggregate.Elem.Bytes() <= 0 ||
				offset+4 > aggregate.Bytes() {
				return nil, false
			}
			elemSize := aggregate.Elem.Bytes()
			index := offset / elemSize
			offset %= elemSize
			path = &symresolve.SymbolTerm{
				Base:     path,
				IndexVal: machine.ConstVal(uint(index)),
				Scale:    elemSize,
				Result:   aggregate.Elem,
			}
		case *typeinfo.Struct:
			field, remainder, ok := ctx.res.ResolveContainingFieldPathInContext(path, offset, ctx.unionContext())
			if !ok {
				return nil, false
			}
			path = field
			offset = remainder
		default:
			if _, ok := path.(*symresolve.SymbolDeref); ok && typ.Bytes() > 0 && offset%typ.Bytes() == 0 {
				return typ, true
			}
			return typ, offset == 0
		}
	}
}

// wideStorageScalarType returns the exact four-byte integer or pointer type
// stored at mem, resolving fixed offsets through structs and arrays, e.g.
// pfl->rgwtMin[3] at [bx+0x58]. Narrower fields and aggregates report false.
func (ctx *FuncContext) wideStorageScalarType(mem machine.MemoryAddress) (typeinfo.Type, bool) {
	mem.Width = 4
	addr, ok := ctx.symbols.addressFromMemory(mem, nil)
	if !ok {
		return nil, false
	}
	lane, ok := ctx.symbols.storageLaneFromMemory(mem, addr)
	if !ok {
		return nil, false
	}
	typ, ok := ctx.scalarTypeAtOffset(lane.object, lane.offset)
	if !ok || typ.Bytes() != 4 || (typ.Kind() != typeinfo.KInt && typ.Kind() != typeinfo.KPointer) {
		return nil, false
	}
	return typ, true
}
