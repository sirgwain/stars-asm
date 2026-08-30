package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

type normalizeShiftsProcessor struct {
	bitwiseContext bool
}

// ProcessMachineBlock normalizes shift expressions in every value-bearing effect.
func (p *normalizeShiftsProcessor) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	effects, changed := p.rewriter().rewriteMachineEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// rewriter returns the machine tree rewrite for shift normalization.
func (p *normalizeShiftsProcessor) rewriter() *machineRewriter {
	return &machineRewriter{
		value: func(w *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
			switch v := value.(type) {
			case *machine.Binary:
				next, changed := p.normalizeShiftBinary(w, v)
				return next, changed, true
			case *machine.FarPointer:
				next, changed := p.normalizeShiftFarPointer(w, v)
				return next, changed, true
			default:
				return value, false, false
			}
		},
		memory: func(w *machineRewriter, mem machine.MemoryAccess) (machine.MemoryAccess, bool, bool) {
			next, changed := p.withMemoryBitwiseContext(false, func() (machine.MemoryAccess, bool) {
				return w.rewriteMachineMemoryChildren(mem)
			})
			return next, changed, true
		},
	}
}

// normalizeShiftBinary normalizes one binary expression and its children.
func (p *normalizeShiftsProcessor) normalizeShiftBinary(w *machineRewriter, v *machine.Binary) (machine.Value, bool) {
	bitwiseContext := p.bitwiseContext
	childBitwiseContext := p.bitwiseContext || bitwiseValueOp(v.Op) || shiftValueOp(v.Op)
	lhs, lhsChanged := p.withValueBitwiseContext(childBitwiseContext, func() (machine.Value, bool) {
		return w.rewriteMachineValue(v.LHS)
	})
	rhs, rhsChanged := p.withValueBitwiseContext(childBitwiseContext, func() (machine.Value, bool) {
		return w.rewriteMachineValue(v.RHS)
	})
	next := &machine.Binary{Op: v.Op, LHS: lhs, RHS: rhs}
	changed := lhsChanged || rhsChanged

	if combined, ok := combineNestedShift(next); ok {
		next = combined
		changed = true
	}
	if next.Op == machine.ValueOpShl && !bitwiseContext {
		if multiplier, ok := shiftMultiplier(next.RHS); ok {
			return machine.BinaryVal(machine.ValueOpMul, next.LHS, multiplier), true
		}
	}
	if !changed {
		return v, false
	}
	return next, true
}

// normalizeShiftFarPointer normalizes shift expressions inside a far pointer.
func (p *normalizeShiftsProcessor) normalizeShiftFarPointer(w *machineRewriter, v *machine.FarPointer) (machine.Value, bool) {
	parent, parentChanged := w.rewriteMachineValue(v.Parent)
	offset, offsetChanged := w.rewriteMachineValue(v.Offset)
	segment, segmentChanged := w.rewriteMachineValue(v.Segment)
	if !parentChanged && !offsetChanged && !segmentChanged {
		return v, false
	}
	if v.Part == machine.FarPointerWhole {
		return machine.FarPointerWordsVal(offset, segment), true
	}
	return machine.FarPointerVal(parent, v.Part), true
}

// withValueBitwiseContext runs a value rewrite with a temporary bitwise context.
func (p *normalizeShiftsProcessor) withValueBitwiseContext(bitwiseContext bool, fn func() (machine.Value, bool)) (machine.Value, bool) {
	prev := p.bitwiseContext
	p.bitwiseContext = bitwiseContext
	value, changed := fn()
	p.bitwiseContext = prev
	return value, changed
}

// withMemoryBitwiseContext runs a memory rewrite with a temporary bitwise context.
func (p *normalizeShiftsProcessor) withMemoryBitwiseContext(bitwiseContext bool, fn func() (machine.MemoryAccess, bool)) (machine.MemoryAccess, bool) {
	prev := p.bitwiseContext
	p.bitwiseContext = bitwiseContext
	value, changed := fn()
	p.bitwiseContext = prev
	return value, changed
}

// combineNestedShift folds repeated constant shifts in the same direction.
func combineNestedShift(v *machine.Binary) (*machine.Binary, bool) {
	if !shiftValueOp(v.Op) {
		return nil, false
	}
	rhs, ok := v.RHS.(*machine.Const)
	if !ok {
		return nil, false
	}
	lhs, ok := v.LHS.(*machine.Binary)
	if !ok || lhs.Op != v.Op {
		return nil, false
	}
	lhsRHS, ok := lhs.RHS.(*machine.Const)
	if !ok {
		return nil, false
	}
	return &machine.Binary{Op: v.Op, LHS: lhs.LHS, RHS: machine.ConstVal(lhsRHS.Val + rhs.Val)}, true
}

// shiftMultiplier converts a left shift count into a multiplication constant.
func shiftMultiplier(value machine.Value) (*machine.Const, bool) {
	count, ok := value.(*machine.Const)
	if !ok || count.Val >= 64 {
		return nil, false
	}
	return machine.ConstVal(uint(1) << count.Val), true
}

// shiftValueOp reports whether op is any shift operation.
func shiftValueOp(op machine.ValueOp) bool {
	return op == machine.ValueOpShl || op == machine.ValueOpShr || op == machine.ValueOpSar
}

// bitwiseValueOp reports whether op preserves bitwise context for child shifts.
func bitwiseValueOp(op machine.ValueOp) bool {
	return op == machine.ValueOpAnd || op == machine.ValueOpOr || op == machine.ValueOpXor
}
