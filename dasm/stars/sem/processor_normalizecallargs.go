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
			args, argsChanged := w.rewriteMachineValues(call.Args)
			call.Args = args
			next, normalizedChanged := normalizeMachineVarArgDataFarPointers(p.ctx, call)
			return next, argsChanged || normalizedChanged, true
		},
	}
	effects, changed := rewriter.rewriteMachineEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// ProcessBlock normalizes semantically annotated call arguments in one block.
func (p *normalizeCallArgsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	rewriter := &semRewriter{
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			args, argsChanged := w.rewriteExprs(call.Args)
			next := *call
			next.Args = args
			normalized, normalizedChanged := normalizeSemanticVarArgFarPointers(p.ctx, &next)
			collapsed, collapsedChanged := collapseTypedFarPointerCallArgs(p.ctx, normalized)
			if !argsChanged && !normalizedChanged && !collapsedChanged {
				return call, false, true
			}
			return collapsed, true, true
		},
	}
	effects, changed := rewriter.rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	next := b
	next.Effects = effects
	return next, true
}

// normalizeMachineVarArgDataFarPointers rebuilds DS:offset far pointers split across varargs.
func normalizeMachineVarArgDataFarPointers(ctx *FuncContext, call machine.CallEffect) (machine.CallEffect, bool) {
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
		if i+1 < len(call.Args) && isMachineDataSegmentArg(ctx, call.Args[i+1]) {
			args = append(args, machine.FarPointerWordsVal(call.Args[i], call.Args[i+1]))
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

// normalizeSemanticVarArgFarPointers rebuilds far pointers from adjacent vararg words.
func normalizeSemanticVarArgFarPointers(ctx *FuncContext, call *Call) (*Call, bool) {
	start, ok := callVarArgStart(call.Function)
	if !ok {
		return call, false
	}
	if start >= len(call.Args)-1 {
		return call, false
	}
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	ptrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	changed := false
	args := make([]Expr, 0, len(call.Args))
	args = append(args, call.Args[:start]...)
	for i := start; i < len(call.Args); i++ {
		if resolved, ok := normalizeSemanticVarArgFarPointer(ctx, call.Args[i]); ok {
			args = append(args, resolved)
			changed = true
			continue
		}
		if i+1 < len(call.Args) && isSemanticDataSegmentArg(ctx, call.Args[i+1]) {
			if typeinfo.IsPointer(call.Args[i].ExprType()) {
				args = append(args, call.Args[i])
				i++
				changed = true
				continue
			}
			ptr := &FarPointer{
				Part:     machine.FarPointerWhole,
				Segment:  call.Args[i+1],
				Offset:   call.Args[i],
				TypeInfo: ptrType,
			}
			if resolved, ok := ctx.resolveSemanticFarPointer(ptr); ok {
				args = append(args, resolved)
			} else {
				args = append(args, call.Args[i])
			}
			i++
			changed = true
			continue
		}
		args = append(args, call.Args[i])
	}
	if !changed {
		return call, false
	}
	next := *call
	next.Args = args
	return &next, true
}

// normalizeSemanticVarArgFarPointer collapses a whole far-pointer vararg when possible.
func normalizeSemanticVarArgFarPointer(ctx *FuncContext, arg Expr) (Expr, bool) {
	ptr, ok := arg.(*FarPointer)
	if !ok || ptr.Part != machine.FarPointerWhole {
		return arg, false
	}
	if resolved, ok := ctx.resolveSemanticFarPointer(ptr); ok {
		return resolved, true
	}
	if isSemanticDataSegmentArg(ctx, ptr.Segment) {
		return ptr.Offset, true
	}
	return arg, false
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

// isSemanticDataSegmentArg reports whether expr is the current data-segment register.
func isSemanticDataSegmentArg(ctx *FuncContext, expr Expr) bool {
	return exprMatchesMachineValue(expr, ctx.dsReg)
}

// isMachineDataSegmentArg reports whether value is the current data-segment register.
func isMachineDataSegmentArg(ctx *FuncContext, value machine.Value) bool {
	return machine.ValueEquals(value, ctx.dsReg)
}
