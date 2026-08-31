package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

type machineEffectRewrite func(*machineRewriter, machine.Effect) (machine.Effect, bool, bool)
type machineValueRewrite func(*machineRewriter, machine.Value) (machine.Value, bool, bool)
type machineMemoryRewrite func(*machineRewriter, machine.MemoryAccess) (machine.MemoryAccess, bool, bool)

type machineRewriter struct {
	effect machineEffectRewrite
	value  machineValueRewrite
	memory machineMemoryRewrite
}

// rewriteMachineEffects rewrites a slice of machine effects.
func (w *machineRewriter) rewriteMachineEffects(effects []machine.Effect) ([]machine.Effect, bool) {
	var out []machine.Effect
	for i, effect := range effects {
		next, effectChanged := w.rewriteMachineEffect(effect)
		if !effectChanged {
			if out != nil {
				out[i] = effect
			}
			continue
		}
		if out == nil {
			out = append([]machine.Effect(nil), effects...)
		}
		out[i] = next
	}
	if out == nil {
		return effects, false
	}
	return out, true
}

// rewriteMachineEffect rewrites one machine effect.
func (w *machineRewriter) rewriteMachineEffect(effect machine.Effect) (machine.Effect, bool) {
	if w.effect != nil {
		if next, changed, handled := w.effect(w, effect); handled {
			return next, changed
		}
	}
	return w.rewriteMachineEffectChildren(effect)
}

// rewriteMachineEffectChildren rewrites only the direct children of an effect.
func (w *machineRewriter) rewriteMachineEffectChildren(effect machine.Effect) (machine.Effect, bool) {
	switch e := effect.(type) {
	case machine.StoreEffect:
		addr, addrChanged := w.rewriteMachineMemory(e.Addr)
		src, srcChanged := w.rewriteMachineValue(e.Src)
		if !addrChanged && !srcChanged {
			return effect, false
		}
		e.Addr = addr
		e.Src = src
		return e, true
	case machine.CopyEffect:
		dst, dstChanged := w.rewriteMachineValue(e.Dst)
		src, srcChanged := w.rewriteMachineValue(e.Src)
		if !dstChanged && !srcChanged {
			return effect, false
		}
		e.Dst = dst
		e.Src = src
		return e, true
	case machine.CallEffect:
		addr, addrChanged := w.rewriteMachineMemory(e.MemoryAccess)
		args, argsChanged := w.rewriteMachineValues(e.Args)
		result, resultChanged := w.rewriteMachineValue(e.Result)
		if !addrChanged && !argsChanged && !resultChanged {
			return effect, false
		}
		e.MemoryAccess = addr
		e.Args = args
		e.Result = result
		return e, true
	case machine.BranchEffect:
		predicate, changed := w.rewriteMachinePredicate(e.Predicate)
		if !changed {
			return effect, false
		}
		e.Predicate = predicate
		return e, true
	case machine.ReturnEffect:
		value, changed := w.rewriteMachineValue(e.Value)
		if !changed {
			return effect, false
		}
		e.Value = value
		return e, true
	default:
		return effect, false
	}
}

// rewriteMachineValue rewrites one machine value.
func (w *machineRewriter) rewriteMachineValue(value machine.Value) (machine.Value, bool) {
	if w.value != nil {
		if next, changed, handled := w.value(w, value); handled {
			return next, changed
		}
	}
	return w.rewriteMachineValueChildren(value)
}

// rewriteMachineValueChildren rewrites only the direct children of a value.
func (w *machineRewriter) rewriteMachineValueChildren(value machine.Value) (machine.Value, bool) {
	switch v := value.(type) {
	case nil:
		return nil, false
	case *machine.PredicateValue:
		return w.rewriteMachinePredicate(v)
	case *machine.WordValue:
		parent, changed := w.rewriteMachineValue(v.Parent)
		if !changed {
			return value, false
		}
		return machine.WordVal(parent, v.Part), true
	case *machine.FarPointer:
		parent, parentChanged := w.rewriteMachineValue(v.Parent)
		offset, offsetChanged := w.rewriteMachineValue(v.Offset)
		segment, segmentChanged := w.rewriteMachineValue(v.Segment)
		if !parentChanged && !offsetChanged && !segmentChanged {
			return value, false
		}
		next := *v
		next.Parent = parent
		next.Offset = offset
		next.Segment = segment
		return &next, true
	case *machine.SignExtendValue:
		parent, changed := w.rewriteMachineValue(v.Parent)
		if !changed {
			return value, false
		}
		return machine.SignExtendVal(parent, v.FromBits, v.ToBits), true
	case *machine.StackWords:
		words, changed := w.rewriteMachineValues(v.Words)
		if !changed {
			return value, false
		}
		return &machine.StackWords{Words: words}, true
	case *machine.ByteValue:
		parent, parentChanged := w.rewriteMachineValue(v.Parent)
		var byteChanged bool
		var byteValue *machine.Value
		if v.Value != nil {
			next, changed := w.rewriteMachineValue(*v.Value)
			byteChanged = changed
			byteValue = &next
		}
		if !parentChanged && !byteChanged {
			return value, false
		}
		next := *v
		next.Parent = parent
		next.Value = byteValue
		return &next, true
	case *machine.Cast:
		castValue, changed := w.rewriteMachineValue(v.Value)
		if !changed {
			return value, false
		}
		return machine.CastVal(castValue, v.To), true
	case *machine.Binary:
		lhs, lhsChanged := w.rewriteMachineValue(v.LHS)
		rhs, rhsChanged := w.rewriteMachineValue(v.RHS)
		if !lhsChanged && !rhsChanged {
			return value, false
		}
		return machine.BinaryResult(v.Op, lhs, rhs), true
	case *machine.Load:
		access, changed := w.rewriteMachineMemory(v.Access)
		if !changed {
			return value, false
		}
		next := *v
		next.Access = access
		return &next, true
	case *machine.Address:
		access, changed := w.rewriteMachineMemory(v.Access)
		if !changed {
			return value, false
		}
		return machine.AddressVal(access), true
	case *machine.PhiValue:
		changed := false
		arms := make([]machine.PhiArm, len(v.Arms))
		for i, arm := range v.Arms {
			armValue, armChanged := w.rewriteMachineValue(arm.Value)
			arms[i] = arm
			arms[i].Value = armValue
			changed = changed || armChanged
		}
		if !changed {
			return value, false
		}
		next := *v
		next.Arms = arms
		return &next, true
	default:
		return value, false
	}
}

// rewriteMachineValues rewrites a slice of machine values.
func (w *machineRewriter) rewriteMachineValues(values []machine.Value) ([]machine.Value, bool) {
	var out []machine.Value
	for i, value := range values {
		next, valueChanged := w.rewriteMachineValue(value)
		if !valueChanged {
			if out != nil {
				out[i] = value
			}
			continue
		}
		if out == nil {
			out = append([]machine.Value(nil), values...)
		}
		out[i] = next
	}
	if out == nil {
		return values, false
	}
	return out, true
}

// rewriteMachinePredicate rewrites a predicate value.
func (w *machineRewriter) rewriteMachinePredicate(predicate *machine.PredicateValue) (*machine.PredicateValue, bool) {
	if predicate == nil {
		return nil, false
	}
	lhs, lhsChanged := w.rewriteMachineValue(predicate.LHS)
	rhs, rhsChanged := w.rewriteMachineValue(predicate.RHS)
	if !lhsChanged && !rhsChanged {
		return predicate, false
	}
	next := *predicate
	next.LHS = lhs
	next.RHS = rhs
	return &next, true
}

// rewriteMachineMemory rewrites a machine memory access.
func (w *machineRewriter) rewriteMachineMemory(mem machine.MemoryAccess) (machine.MemoryAccess, bool) {
	if w.memory != nil {
		if next, changed, handled := w.memory(w, mem); handled {
			return next, changed
		}
	}
	return w.rewriteMachineMemoryChildren(mem)
}

// rewriteMachineMemoryChildren rewrites value-bearing memory fields.
func (w *machineRewriter) rewriteMachineMemoryChildren(mem machine.MemoryAccess) (machine.MemoryAccess, bool) {
	changed := false
	if next, ok := w.rewriteMachineValue(mem.Seg); ok {
		mem.Seg = next
		changed = true
	}
	if next, ok := w.rewriteMachineValue(mem.Base); ok {
		mem.Base = next
		changed = true
	}
	if next, ok := w.rewriteMachineValue(mem.Index); ok {
		mem.Index = next
		changed = true
	}
	return mem, changed
}
