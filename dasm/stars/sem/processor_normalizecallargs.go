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
			next, normalizedChanged := normalizeMachineVarArgDataFarPointers(call)
			return next, accessChanged || argsChanged || indirectChanged || normalizedChanged, true
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
func normalizeMachineVarArgDataFarPointers(call machine.CallEffect) (machine.CallEffect, bool) {
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
		if i+1 < len(call.Args) {
			if _, ok := call.Args[i+1].(*machine.Reg); ok {
				args = append(args, &machine.StackWords{Words: []machine.Value{call.Args[i+1], call.Args[i]}})
				i++
				changed = true
				continue
			}
		}
		args = append(args, call.Args[i])
	}
	if !changed {
		return call, false
	}
	call.Args = args
	return call, true
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
