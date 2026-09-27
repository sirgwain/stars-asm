package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type normalizeCallArgsProcessor struct {
	ctx *FuncContext
}

// ProcessMachineBlock normalizes call arguments before semantic conversion.
func (p *normalizeCallArgsProcessor) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	rewriter := &machineRewriter{
		effect: func(w *machineRewriter, effect machine.Effect) (machine.Effect, bool, bool) {
			call, ok := effect.(machine.CallEffect)
			if !ok {
				return nil, false, false
			}
			access, accessChanged := w.rewriteMachineMemory(call.MemoryAccess)
			args, argsChanged := w.rewriteMachineValues(call.Args)
			call.MemoryAccess = access
			call.Args = args
			next, indirectChanged := p.normalizeMachineIndirectCallArgs(call)
			call = next
			next, normalizedChanged := p.normalizeMachineVarArgDataFarPointers(call)
			call = next
			next, phiChanged := distributeMachineFarPointerPhis(call)
			return next, accessChanged || argsChanged || indirectChanged || normalizedChanged || phiChanged, true
		},
	}
	effects, changed := rewriter.rewriteMachineEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// normalizeMachineIndirectCallArgs groups raw stack words for indirect calls
// once a function pointer target gives us the callee signature.
func (p *normalizeCallArgsProcessor) normalizeMachineIndirectCallArgs(call machine.CallEffect) (machine.CallEffect, bool) {
	if call.Target != nil || emptyCallMemoryAccess(call.MemoryAccess) {
		return call, false
	}
	fn, ok := p.resolveMachineIndirectCallTargetFunction(call)
	if !ok {
		return call, false
	}
	args := indirectMachineCallArgs(fn, call.Args)
	if machineValueSlicesEqual(args, call.Args) {
		return call, false
	}
	call.Args = args
	return call, true
}

// resolveMachineIndirectCallTargetFunction resolves a memory call target to a
// function pointer signature using machine-level symbol resolution.
func (p *normalizeCallArgsProcessor) resolveMachineIndirectCallTargetFunction(call machine.CallEffect) (*typeinfo.Function, bool) {
	if p.ctx == nil {
		return nil, false
	}
	access := call.MemoryAccess
	access.Width = 4
	resolved, ok := p.ctx.symbols.addressFromMemory(access, nil)
	if !ok {
		return nil, false
	}
	path, ok := resolved.path()
	if !ok {
		return nil, false
	}
	return functionFromCallTargetType(path.Type(), access.Width)
}

// machineValueSlicesEqual reports whether two machine value slices have the
// same structural values.
func machineValueSlicesEqual(a []machine.Value, b []machine.Value) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if !machine.ValueEquals(a[i], b[i]) {
			return false
		}
	}
	return true
}

// indirectMachineCallArgs groups raw indirect-call stack words by the inferred
// signature.
func indirectMachineCallArgs(fn *typeinfo.Function, rawWords []machine.Value) []machine.Value {
	if fn.Conv == typeinfo.CCPascal {
		return leftToRightIndirectMachineCallArgs(fn, rawWords)
	}
	return rightToLeftIndirectMachineCallArgs(fn, rawWords)
}

// leftToRightIndirectMachineCallArgs groups Pascal stack words in parameter order.
func leftToRightIndirectMachineCallArgs(fn *typeinfo.Function, rawWords []machine.Value) []machine.Value {
	args := make([]machine.Value, 0, len(fn.Params))
	idx := 0
	for _, param := range fn.Params {
		n := param.Words()
		if idx+n > len(rawWords) {
			break
		}
		args = append(args, indirectMachineStackWordsValue(rawWords[idx:idx+n]))
		idx += n
	}
	args = append(args, rawWords[idx:]...)
	return args
}

// rightToLeftIndirectMachineCallArgs groups cdecl/stdcall stack words in parameter order.
func rightToLeftIndirectMachineCallArgs(fn *typeinfo.Function, rawWords []machine.Value) []machine.Value {
	args := make([]machine.Value, 0, len(fn.Params))
	idx := len(rawWords)
	for _, param := range fn.Params {
		n := param.Words()
		if idx-n < 0 {
			break
		}
		args = append(args, indirectMachineStackWordsValue(rawWords[idx-n:idx]))
		idx -= n
	}
	for i := idx - 1; i >= 0; i-- {
		args = append(args, rawWords[i])
	}
	return args
}

// indirectMachineStackWordsValue preserves single-word args and wraps wider raw args.
func indirectMachineStackWordsValue(words []machine.Value) machine.Value {
	if len(words) == 0 {
		return machine.UnknownVal("stack")
	}
	if len(words) == 1 {
		return words[0]
	}
	return &machine.StackWords{Words: append([]machine.Value(nil), words...)}
}

// normalizeMachineVarArgDataFarPointers rebuilds DS:offset far pointers split across varargs.
func (p *normalizeCallArgsProcessor) normalizeMachineVarArgDataFarPointers(call machine.CallEffect) (machine.CallEffect, bool) {
	start, ok := callVarArgStart(call.Target)
	if !ok {
		return call, false
	}
	if start >= len(call.Args)-1 {
		return call, false
	}

	changed := false
	args := make([]machine.Value, 0, len(call.Args))
	args = append(args, call.Args[:start]...)
	for i := start; i < len(call.Args); i++ {
		if i+1 < len(call.Args) && (machineSegmentWord(call.Args[i+1]) || p.splitFarPointerWords(call.Args[i+1], call.Args[i])) {
			args = append(args, machineFarPointerWords(call.Args[i+1], call.Args[i]))
			i++
			changed = true
			continue
		}
		args = append(args, call.Args[i])
	}
	if !changed {
		return call, false
	}
	call.Args = args
	return call, true
}

// distributeMachineFarPointerPhis pushes segment words into phi-selected
// offsets for grouped far-pointer args, e.g. words(ds, merge(0x160b, 0x1613))
// becomes merge(words(ds, 0x160b), words(ds, 0x1613)).
func distributeMachineFarPointerPhis(call machine.CallEffect) (machine.CallEffect, bool) {
	var args []machine.Value
	for i, arg := range call.Args {
		words, ok := arg.(*machine.StackWords)
		if !ok || len(words.Words) != 2 || !machineSegmentWord(words.Words[0]) {
			continue
		}
		if _, ok := words.Words[1].(*machine.PhiValue); !ok {
			continue
		}
		if args == nil {
			args = append([]machine.Value(nil), call.Args...)
		}
		args[i] = machineFarPointerWords(words.Words[0], words.Words[1])
	}
	if args == nil {
		return call, false
	}
	call.Args = args
	return call, true
}

// machineSegmentWord reports whether a vararg stack word is a segment
// register, or a phi selecting only segment registers.
func machineSegmentWord(value machine.Value) bool {
	switch v := value.(type) {
	case *machine.Reg:
		return true
	case *machine.PhiValue:
		for _, arm := range v.Arms {
			if !machineSegmentReg(arm.Value) {
				return false
			}
		}
		return len(v.Arms) > 0
	default:
		return false
	}
}

// splitFarPointerWords reports whether segment and offset are the high and
// low words of one 32-bit scalar or pointer in storage, with an optional
// constant offset added to the low word, e.g. load([bp+0xa]),
// load([bp+0x8])+0x8. Adjacent 16-bit objects such as x, y do not pair.
func (p *normalizeCallArgsProcessor) splitFarPointerWords(segment, offset machine.Value) bool {
	high, ok := segment.(*machine.Load)
	if !ok || high.Addr.Width != 2 {
		return false
	}
	if binary, ok := offset.(*machine.Binary); ok && binary.Op == machine.ValueOpAdd {
		if _, ok := binary.RHS.(*machine.Const); !ok {
			return false
		}
		offset = binary.LHS
	}
	low, ok := offset.(*machine.Load)
	if !ok || low.Addr.Width != 2 {
		return false
	}
	if low.Addr.Disp+2 != high.Addr.Disp ||
		!machine.ValueEquals(low.Addr.Seg, high.Addr.Seg) ||
		!machine.ValueEquals(low.Addr.Base, high.Addr.Base) ||
		!machine.ValueEquals(low.Addr.Index, high.Addr.Index) {
		return false
	}
	access := low.Addr
	access.Width = 4
	resolved, ok := p.ctx.symbols.addressFromMemory(access, nil)
	if !ok {
		return false
	}
	path, ok := resolved.path()
	if !ok {
		return false
	}
	typ := path.Type()
	return typ.Bytes() == 4 && (typ.Kind() == typeinfo.KInt || typ.Kind() == typeinfo.KPointer)
}

// machineSegmentReg reports whether value is a segment register.
func machineSegmentReg(value machine.Value) bool {
	reg, ok := value.(*machine.Reg)
	return ok && reg.Val.IsSeg()
}

// machineVarArgFarPointer reports whether a normalized vararg is a
// segment:offset pair, or a phi selecting only such pairs.
func machineVarArgFarPointer(value machine.Value) bool {
	switch v := value.(type) {
	case *machine.StackWords:
		return len(v.Words) == 2 && machineSegmentReg(v.Words[0])
	case *machine.Address:
		// collapse-widevalues folds words(seg, offset) into a far address.
		return v.Addr.Width == 4 && machineSegmentReg(v.Addr.Seg)
	case *machine.PhiValue:
		for _, arm := range v.Arms {
			if !machineVarArgFarPointer(arm.Value) {
				return false
			}
		}
		return len(v.Arms) > 0
	default:
		return false
	}
}

// machineFarPointerWords pairs a segment and offset word into one far-pointer
// argument. Phi words are distributed so each arm carries a complete
// segment:offset pair, e.g. (ds, phi(0xc85, 0xc86)) becomes
// phi(words(ds, 0xc85), words(ds, 0xc86)).
func machineFarPointerWords(segment, offset machine.Value) machine.Value {
	segPhi, segIsPhi := segment.(*machine.PhiValue)
	offPhi, offIsPhi := offset.(*machine.PhiValue)
	switch {
	case offIsPhi && !segIsPhi:
		arms := make([]machine.PhiArm, len(offPhi.Arms))
		for i, arm := range offPhi.Arms {
			arms[i] = machine.PhiArm{Block: arm.Block, Value: machineFarPointerWords(segment, arm.Value)}
		}
		return &machine.PhiValue{Join: offPhi.Join, Arms: arms}
	case segIsPhi && offIsPhi && machinePhiArmsAligned(segPhi, offPhi):
		arms := make([]machine.PhiArm, len(offPhi.Arms))
		for i, arm := range offPhi.Arms {
			arms[i] = machine.PhiArm{Block: arm.Block, Value: machineFarPointerWords(segPhi.Arms[i].Value, arm.Value)}
		}
		return &machine.PhiValue{Join: offPhi.Join, Arms: arms}
	}
	return &machine.StackWords{Words: []machine.Value{segment, offset}}
}

// machinePhiArmsAligned reports whether two phis join at the same block with
// arms from the same predecessors in the same order.
func machinePhiArmsAligned(a, b *machine.PhiValue) bool {
	if a.Join != b.Join || len(a.Arms) != len(b.Arms) {
		return false
	}
	for i := range a.Arms {
		if a.Arms[i].Block == nil || b.Arms[i].Block == nil || a.Arms[i].Block.ID != b.Arms[i].Block.ID {
			return false
		}
	}
	return true
}

// callVarArgStart returns the first variadic argument index for known vararg calls.
func callVarArgStart(function *typeinfo.Function) (int, bool) {
	if function == nil {
		return 0, false
	}
	if function.VarArgs {
		return len(function.Params), true
	}
	if function.Name == "_wsprintf" {
		return 2, true
	}
	return 0, false
}
