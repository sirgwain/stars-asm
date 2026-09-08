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
			param := &fn.Params[i]
			expected = param.Type
			if param.Semantic == typeinfo.ParamSemanticResourceNameOrID {
				out[i] = arg
				continue
			}
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
			if _, ok := recognizeBitfieldRead(ctx, v); ok {
				if collapsed, changed := collapseWideBitfieldStorage(ctx, v); changed {
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

		collapser := wideMachineCollapser{ctx: ctx}
		if value, ok := collapser.pair(v.Words[1], v.Words[0]); ok {
			value, _ = collapseWideMachineValueTree(ctx, value)
			return value, true, true
		}
	}
	// next, changed, handled
	return v, false, false
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
func collapseWideBitfieldStorage(ctx *FuncContext, value machine.Value) (machine.Value, bool) {
	collapser := wideMachineCollapser{ctx: ctx}
	rewriter := &machineRewriter{
		value: func(mr *machineRewriter, candidate machine.Value) (machine.Value, bool, bool) {
			words, ok := candidate.(*machine.StackWords)
			if !ok {
				return candidate, false, false
			}
			if len(words.Words) != 2 {
				return candidate, false, true
			}
			wide, ok := collapser.pair(words.Words[1], words.Words[0])
			if !ok {
				return candidate, false, true
			}
			return wide, true, true
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
