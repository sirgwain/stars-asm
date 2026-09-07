package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type collapseWideValues struct {
	ctx *FuncContext
}

// ProcessMachineFunc collapses wide values throughout a machine function.
func (p *collapseWideValues) ProcessMachineFunc(result *Result, f *machine.FuncEffects) bool {
	changed := false
	for i := range f.Blocks {
		p.ctx.SetCurrentBlock(f.Blocks[i].Block)
		next, blockChanged := p.ProcessMachineBlock(result, *f, f.Blocks[i])
		if blockChanged {
			f.Blocks[i] = next
			changed = true
		}
	}
	p.ctx.ClearCurrentBlock()
	return changed
}

// ProcessMachineBlock coalesces adjacent contiguous copy effects in one machine block.
func (p *collapseWideValues) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	effects, changed := p.rewriter().rewriteMachineEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true

}

// rewriter returns the machine tree visitor for storage annotation.
func (p *collapseWideValues) rewriter() *machineRewriter {
	return &machineRewriter{
		effect: func(mr *machineRewriter, effect machine.Effect) (machine.Effect, bool, bool) {
			switch e := effect.(type) {
			case machine.CallEffect:
				args, changed := p.collapseCallArgs(e.Target, e.Args)
				if !changed {
					return effect, false, true
				}
				e.Args = args
				return e, true, true
			case machine.ReturnEffect:
				var expected typeinfo.Type
				if p.ctx != nil && p.ctx.fs != nil {
					expected = p.ctx.fs.Ret
				}
				value, changed := collapseWideMachineValueTyped(p.ctx, e.Value, expected)
				if !changed {
					return effect, false, true
				}
				e.Value = value
				return e, true, true
			default:
				return effect, false, false
			}
		},
		value: func(mr *machineRewriter, v machine.Value) (machine.Value, bool, bool) {
			return collapseWideMachineValue(p.ctx, v)
		},
	}
}

// collapseCallArgs collapses machine call arguments using callee parameter
// types to avoid flattening resolvable far pointers into integers.
func (p *collapseWideValues) collapseCallArgs(fn *typeinfo.Function, args []machine.Value) ([]machine.Value, bool) {
	if len(args) == 0 {
		return args, false
	}
	changed := false
	out := make([]machine.Value, len(args))
	for i, arg := range args {
		var expected typeinfo.Type
		if fn != nil && i < len(fn.Params) {
			expected = fn.Params[i].Type
		}
		if messageType := messageCallArgumentType(p.ctx.sdb, fn, args, i); messageType != nil {
			expected = messageType
		}
		next, argChanged := collapseWideMachineValueTyped(p.ctx, arg, expected)
		out[i] = next
		changed = changed || argChanged
	}
	if !changed {
		return args, false
	}
	return out, true
}

// collapseWideMachineValueTyped collapses one machine value while preserving
// typed symbolic far pointers for semantic conversion.
func collapseWideMachineValueTyped(ctx *FuncContext, v machine.Value, expected typeinfo.Type) (machine.Value, bool) {
	if phi, ok := v.(*machine.PhiValue); ok {
		var changed bool
		arms := make([]machine.PhiArm, len(phi.Arms))
		for i, arm := range phi.Arms {
			armValue, armChanged := collapseWideMachineValueTyped(ctx, arm.Value, expected)
			arms[i] = arm
			arms[i].Value = armValue
			changed = changed || armChanged
		}
		if !changed {
			return v, false
		}
		next := *phi
		next.Arms = arms
		return &next, true
	}

	if ctx != nil && expected != nil && typeinfo.IsPointer(expected) {
		if _, ok := ctx.symbols.symbolFromValueTyped(v, expected); ok {
			return v, false
		}
	}
	return collapseWideMachineValueTree(ctx, v)
}

// collapseWideMachineValue reconstructs native wide machine values from
// paired 16-bit word representations.
func collapseWideMachineValue(ctx *FuncContext, v machine.Value) (machine.Value, bool, bool) {
	switch v := v.(type) {
	case *machine.Binary:
		if ctx != nil {
			if _, ok := ctx.symbols.symbolFromBitfieldValue(v); ok {
				if collapsed, changed := collapseWideBitfieldStorage(v); changed {
					return collapsed, true, true
				}
			}
		}
	case *machine.Load:
		seg, segOK := v.Addr.Seg.(*machine.FarPointer)
		off, offOK := v.Addr.Base.(*machine.FarPointer)
		if !segOK || !offOK || ctx == nil {
			break
		}
		if machine.ValueEquals(seg.Parent, off.Parent) && typeinfo.IsFarPointer(machineValueType(off.Parent)) {
			addr := v.Addr
			addr.Seg = nil
			addr.Base = off.Parent
			result := *v
			result.Addr = addr
			return &result, true, true
		}

		_, ok := ctx.symbols.symbolFromFarPointer(seg, off, v.Addr.Disp, v.Addr.Width)
		if !ok {
			break
		}
		loadOff, ok := off.Parent.(*machine.Load)
		if !ok {
			break
		}

		// The parent load is the native 32-bit pointer value.
		ptrLoad := *loadOff
		ptrLoad.Addr.Width = 4

		// Replace segmented seg:off addressing with native pointer addressing.
		addr := v.Addr
		addr.Seg = nil
		addr.Base = &ptrLoad

		// for standard farseg/faroff shapes
		// farseg(load(dword [bp-lppl])):[faroff(load(dword [bp-lppl]))+0x34]
		// addr.Disp remains +0x34

		result := *v
		result.Addr = addr

		// ctx.log.Debug(
		// 	"farpointer",
		// 	"mem", v.Addr,
		// 	"native", result.Addr,
		// )

		return &result, true, true
	case *machine.StackWords:
		if len(v.Words) != 2 {
			break
		}

		// check various reg + off words
		if reg, ok := v.Words[0].(*machine.Reg); ok {
			// words(ss, addr(local)+offset) or any address with a seg arg
			if addr, ok := collapseSegmentedMachineAddress(reg, v.Words[1]); ok {
				return addr, true, true
			}
			// words(ds, load(local))
			if load, ok := v.Words[1].(*machine.Load); ok && (reg.Val == asm.RegDS || reg.Val == asm.RegCS) {
				return load, true, true
			}

			// words(ds, 0x1234) const addresses
			if cAddr, ok := v.Words[1].(*machine.Const); ok && (reg.Val == asm.RegDS || reg.Val == asm.RegCS) {
				origin := machine.Origin{}
				if cAddr.Origin != nil {
					origin = *cAddr.Origin
				}
				return machine.AddressVal(machine.MemoryAddress{
					Seg:    v.Words[0],
					Disp:   int(cAddr.Val),
					Width:  4,
					Origin: origin,
				}), true, true
			}

			// words(ds, SzVersion()) style callresult args
			if callresult, ok := v.Words[1].(*machine.CallResult); ok && reg.Val == asm.RegDS {
				return callresult, true, true
			}
		}

		// check for two consts, words(0,0x64)
		if high, ok := v.Words[0].(*machine.Const); ok {
			if low, ok := v.Words[1].(*machine.Const); ok {
				if high.Fixup != nil || low.Fixup != nil {
					break
				}
				return &machine.Const{Val: high.Val<<16 | low.Val, Origin: low.Origin}, true, true
			}
		}
		// check for two contiguous loads, words(load([bp-x+0x2]), load([bp-x])).
		if load, ok := collapseAdjacentMachineLoads(v.Words[0], v.Words[1]); ok {
			return load, true, true
		}

		// collapse hiword/loword if they are parts of the same wide parent.
		if high, ok := v.Words[0].(*machine.WordValue); ok && high.Part == machine.WordHigh {
			if low, ok := v.Words[1].(*machine.WordValue); ok && low.Part == machine.WordLow && machine.ValueEquals(high.Parent, low.Parent) {
				parent, _ := collapseWideMachineValueTree(ctx, low.Parent)
				return parent, true, true
			}
		}
		if high, ok := v.Words[0].(*machine.WordValue); ok && high.Part == machine.WordSignHigh {
			if machine.ValueEquals(high.Parent, v.Words[1]) {
				parent, _ := collapseWideMachineValueTree(ctx, v.Words[1])
				return machine.SignExtendVal(parent, 16, 32), true, true
			}
		}

		// collapse farseg/faroff if they are the same base.
		if farseg, ok := v.Words[0].(*machine.FarPointer); ok && farseg.Part == machine.FarPointerSegment {
			if faroff, ok := v.Words[1].(*machine.FarPointer); ok && faroff.Part == machine.FarPointerOffset {
				if machine.ValueEquals(farseg.Parent, faroff.Parent) && typeinfo.IsFarPointer(machineValueType(faroff.Parent)) {
					parent, _ := collapseWideMachineValueTree(ctx, faroff.Parent)
					return parent, true, true
				}
				if segLoad, ok := farseg.Parent.(*machine.Load); ok {
					if offLoad, ok := faroff.Parent.(*machine.Load); ok && machine.ValueEquals(segLoad.Addr.Base, offLoad.Addr.Base) {
						// far pointer with the same base
						offLoad.Addr.Width = 4
						return offLoad, true, true
					}
				}
			}
		}

		if value, ok := collapseWideMachinePair(v.Words[0], v.Words[1]); ok {
			return value, true, true
		}
	}
	// next, changed, handled
	return v, false, false
}

// collapseWideMachinePair reconstructs a wide value from recursively matching
// high and low machine expression trees.
func collapseWideMachinePair(high, low machine.Value) (machine.Value, bool) {
	if highConst, ok := high.(*machine.Const); ok {
		if lowConst, ok := low.(*machine.Const); ok {
			return machine.ConstVal((highConst.Val&0xffff)<<16 | lowConst.Val&0xffff), true
		}
	}
	if highWord, ok := high.(*machine.WordValue); ok {
		switch highWord.Part {
		case machine.WordHigh:
			if machine.ValueEquals(highWord.Parent, low) {
				return low, true
			}
			if lowWord, ok := low.(*machine.WordValue); ok && lowWord.Part == machine.WordLow && machine.ValueEquals(highWord.Parent, lowWord.Parent) {
				return lowWord.Parent, true
			}
		case machine.WordSignHigh:
			if machine.ValueEquals(highWord.Parent, low) {
				return machine.SignExtendVal(low, 16, 32), true
			}
		}
	}
	if load, ok := collapseAdjacentMachineLoads(high, low); ok {
		return load, true
	}
	highBinary, highOK := high.(*machine.Binary)
	lowBinary, lowOK := low.(*machine.Binary)
	if !highOK || !lowOK || highBinary.Op != lowBinary.Op {
		return nil, false
	}
	switch highBinary.Op {
	case machine.ValueOpAdd, machine.ValueOpSub:
	default:
		return nil, false
	}
	lhs, lhsOK := collapseWideMachinePair(highBinary.LHS, lowBinary.LHS)
	rhs, rhsOK := collapseWideMachinePair(highBinary.RHS, lowBinary.RHS)
	if !lhsOK || !rhsOK {
		return nil, false
	}
	return machine.BinaryVal(highBinary.Op, lhs, rhs), true
}

// machineValueType returns the declared type carried directly by a machine value.
func machineValueType(value machine.Value) typeinfo.Type {
	switch v := value.(type) {
	case *machine.CallResult:
		return v.Type
	case *machine.Cast:
		return v.To
	default:
		return nil
	}
}

// collapseWideBitfieldStorage replaces an uncollapsed high/low storage pair
// inside a proven native bitfield expression with one physical dword load.
func collapseWideBitfieldStorage(value machine.Value) (machine.Value, bool) {
	rewriter := &machineRewriter{
		value: func(mr *machineRewriter, candidate machine.Value) (machine.Value, bool, bool) {
			words, ok := candidate.(*machine.StackWords)
			if !ok {
				return candidate, false, false
			}
			load, ok := machineBitfieldStorageLoad(words)
			if !ok {
				return candidate, false, true
			}
			return load, true, true
		},
	}
	return rewriter.rewriteMachineValue(value)
}

// collapseSegmentedMachineAddress applies a segment word to the address root
// of offset arithmetic and returns the reconstructed wide address value.
func collapseSegmentedMachineAddress(segment machine.Value, offset machine.Value) (machine.Value, bool) {
	switch v := offset.(type) {
	case *machine.Address:
		next := *v
		next.Addr.Seg = segment
		next.Addr.Width = 4
		return &next, true
	case *machine.Binary:
		if v.Op != machine.ValueOpAdd && v.Op != machine.ValueOpSub {
			return nil, false
		}
		lhs, lhsOK := collapseSegmentedMachineAddress(segment, v.LHS)
		rhs, rhsOK := collapseSegmentedMachineAddress(segment, v.RHS)
		if v.Op == machine.ValueOpSub {
			if !lhsOK || rhsOK {
				return nil, false
			}
			next := *v
			next.LHS = lhs
			return &next, true
		}
		if lhsOK == rhsOK {
			return nil, false
		}
		next := *v
		if lhsOK {
			next.LHS = lhs
		} else {
			next.RHS = rhs
		}
		return &next, true
	default:
		return nil, false
	}
}

// collapseWideMachineValueTree recursively collapses wide values within a
// recovered parent expression before that parent is returned to the caller.
func collapseWideMachineValueTree(ctx *FuncContext, v machine.Value) (machine.Value, bool) {
	rewriter := &machineRewriter{
		value: func(mr *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
			return collapseWideMachineValue(ctx, value)
		},
	}
	return rewriter.rewriteMachineValue(v)
}

// collapseAdjacentMachineLoads rebuilds one dword load from high/low
// contiguous word loads.
func collapseAdjacentMachineLoads(highValue machine.Value, lowValue machine.Value) (machine.Value, bool) {
	high, ok := highValue.(*machine.Load)
	if !ok {
		return nil, false
	}
	low, ok := lowValue.(*machine.Load)
	if !ok {
		return nil, false
	}
	if !sameMachineMemoryBase(high.Addr, low.Addr) || high.Addr.Disp-low.Addr.Disp != 2 {
		return nil, false
	}
	wide := *low
	wide.Addr.Width = 4
	return &wide, true
}

// sameMachineMemoryBase compares memory base shape while ignoring displacement,
// width, and origin.
func sameMachineMemoryBase(a machine.MemoryAddress, b machine.MemoryAddress) bool {
	return machine.ValueEquals(a.Seg, b.Seg) &&
		machine.ValueEquals(a.Base, b.Base) &&
		machine.ValueEquals(a.Index, b.Index)
}
