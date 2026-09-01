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
			next, normalizedChanged := normalizeMachineVarArgDataFarPointers(call)
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
			structCollapsed, structCollapsedChanged := collapseTypedStructCallArgs(collapsed)
			if !argsChanged && !normalizedChanged && !collapsedChanged && !structCollapsedChanged {
				return call, false, true
			}
			return structCollapsed, true, true
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

// collapseTypedStructCallArgs rebuilds by-value struct arguments split into stack words.
func collapseTypedStructCallArgs(call *Call) (*Call, bool) {
	if call == nil || call.Function == nil || len(call.Args) == 0 {
		return call, false
	}
	changed := false
	args := make([]Expr, len(call.Args))
	for i, arg := range call.Args {
		args[i] = arg
		if i >= len(call.Function.Params) {
			continue
		}
		next, ok := collapseTypedStructCallArg(arg, call.Function.Params[i].Type)
		if !ok {
			continue
		}
		args[i] = next
		changed = true
	}
	if !changed {
		return call, false
	}
	next := *call
	next.Args = args
	return &next, true
}

// collapseTypedStructCallArg rebuilds one by-value struct argument from its word fields.
func collapseTypedStructCallArg(arg Expr, expected typeinfo.Type) (Expr, bool) {
	strct, ok := expected.(*typeinfo.Struct)
	if !ok || strct.Size <= 2 || strct.Size%2 != 0 {
		return nil, false
	}
	words, ok := arg.(*Words)
	if !ok || len(words.Words)*2 != strct.Size {
		return nil, false
	}
	var root Expr
	slots := make(map[int]bool, len(words.Words))
	for _, word := range words.Words {
		partRoot, off, ok := structWordArgPart(word)
		if !ok || off < 0 || off+2 > strct.Size || off%2 != 0 || slots[off] {
			return nil, false
		}
		if root == nil {
			root = partRoot
			if !typeinfo.IsCallCompatible(expected, root.ExprType()) {
				return nil, false
			}
		} else if !sameExpr(root, partRoot) {
			return nil, false
		}
		slots[off] = true
	}
	for off := 0; off < strct.Size; off += 2 {
		if !slots[off] {
			return nil, false
		}
	}
	return root, true
}

// structWordArgPart returns the aggregate root and byte offset for a word field.
func structWordArgPart(expr Expr) (Expr, int, bool) {
	switch e := expr.(type) {
	case *FieldAccess:
		if e.Field == nil || e.Field.Type == nil || e.Field.Type.Bytes() != 2 {
			return nil, 0, false
		}
		return e.Base, e.Field.Offset, true
	case *SymbolRef:
		root, fields, ok := splitSymbolFieldPath(e.Path)
		if !ok || len(fields) == 0 {
			return nil, 0, false
		}
		field := fields[len(fields)-1]
		if field == nil || field.Type == nil || field.Type.Bytes() != 2 {
			return nil, 0, false
		}
		return &SymbolRef{Path: rebuildSymbolFieldPath(root, fields[:len(fields)-1])}, field.Offset, true
	case *Part:
		if e.Width != 2 {
			return nil, 0, false
		}
		return e.Base, e.ByteOff, true
	default:
		return nil, 0, false
	}
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
				args = append(args, machine.FarPointerWordsVal(call.Args[i], call.Args[i+1]))
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
		if i+1 < len(call.Args) {
			if resolved, ok := normalizeSemanticVarArgFarPointerParts(call.Args[i], call.Args[i+1]); ok {
				args = append(args, resolved)
				i++
				changed = true
				continue
			}
		}
		if i+1 < len(call.Args) {
			if semanticSegmentExpr(call.Args[i+1]) {
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
					args = append(args, ptr)
				}
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
	next := *call
	next.Args = args
	return &next, true
}

// normalizeSemanticVarArgFarPointerParts collapses adjacent faroff/farseg
// projections of the same pointer-valued expression.
func normalizeSemanticVarArgFarPointerParts(offset Expr, segment Expr) (Expr, bool) {
	offsetParent, offsetOK := farPointerPartParent(offset, machine.FarPointerOffset)
	segmentParent, segmentOK := farPointerPartParent(segment, machine.FarPointerSegment)
	if !offsetOK || !segmentOK || !sameExpr(offsetParent, segmentParent) || !typeinfo.IsFarPointer(offsetParent.ExprType()) {
		return nil, false
	}
	return offsetParent, true
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
	if _, ok := ptr.Segment.(*Register); ok {
		return ptr.Offset, true
	}
	return arg, false
}

// semanticSegmentExpr reports whether expr has the shape of a far-pointer segment word.
func semanticSegmentExpr(expr Expr) bool {
	switch e := expr.(type) {
	case *Register:
		return true
	case *Merge:
		if len(e.Arms) == 0 {
			return false
		}
		for _, arm := range e.Arms {
			if !semanticSegmentExpr(arm.Value) {
				return false
			}
		}
		return true
	default:
		return false
	}
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
