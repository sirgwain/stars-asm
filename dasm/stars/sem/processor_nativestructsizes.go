package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// structSizeFields are the fields a Windows struct carries its own size in.
var structSizeFields = []string{"lStructSize", "cbSize"}

// nativeStructSizesProcessor replaces Win16 element sizes with sizeof for
// the native compile, where pointers and handles, and structs holding them,
// Windows RECT values or Win16 ints, are larger:
//
//   - a byte_count argument that is k times the Win16 size of the struct,
//     pointer or handle the call's pointer arguments point to becomes
//     k * sizeof(T), and n * size becomes n * sizeof(T), as in
//     memset(&ord, 0, 0x12) or fmemmove(&rglpfl[i + 1], &rglpfl[i], n * 4).
//   - pointer arguments that point to different structs leave the count
//     alone.
//   - an allocator's alloc_size is sized by the struct pointer its result is
//     stored in, directly or through a call result temp, as in
//     plf = LocalAlloc(0x40, 0x32) for a LOGFONT *.
//   - a Windows struct's lStructSize or cbSize set to its Win16 size becomes
//     sizeof(T).
//
// Counts that are not a multiple of the struct size, such as a copy of the
// fields before a struct's pointer, are left alone.
type nativeStructSizesProcessor struct{}

// ProcessFunc rewrites struct size constants throughout one function.
func (p *nativeStructSizesProcessor) ProcessFunc(result *Result, f *Func) bool {
	tempTargets := callResultTempTargets(f)
	rewriter := &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			switch e := effect.(type) {
			case *Assign:
				if src, ok := structSizeFieldSrc(e); ok {
					next := *e
					next.Src = src
					return &next, true, true
				}
				call, ok := e.Src.(*Call)
				if !ok {
					return nil, false, false
				}
				target, ok := allocationTarget(e.Dst, tempTargets)
				if !ok {
					return nil, false, false
				}
				sized, ok := elementSizedCall(call, target)
				if !ok {
					return nil, false, false
				}
				next := *e
				next.Src = sized
				rewritten, _ := w.rewriteEffectChildren(&next)
				return rewritten, true, true
			case *CallEffect:
				if e.Call == nil || e.Result == nil {
					return nil, false, false
				}
				target, ok := allocationTarget(e.Result, tempTargets)
				if !ok {
					return nil, false, false
				}
				call, ok := elementSizedCall(e.Call, target)
				if !ok {
					return nil, false, false
				}
				next := *e
				next.Call = call
				rewritten, _ := w.rewriteEffectChildren(&next)
				return rewritten, true, true
			}
			return nil, false, false
		},
		call: func(w *semRewriter, call *Call, _ machine.Meta) (*Call, bool, bool) {
			next, ok := elementSizedCall(call, nil)
			if !ok {
				return nil, false, false
			}
			rewritten, _ := w.rewriteCallChildren(next)
			return rewritten, true, true
		},
	}
	changed := false
	for i := range f.Blocks {
		effects, blockChanged := rewriter.rewriteEffects(f.Blocks[i].Effects)
		if blockChanged {
			f.Blocks[i].Effects = effects
			changed = true
		}
	}
	return changed
}

// elementSizedCall rewrites the byte_count or alloc_size argument of call as a multiple of
// sizeof the element its pointer arguments point to, or when none does, of
// target, the element an allocator's result is stored as.
func elementSizedCall(call *Call, target typeinfo.Type) (*Call, bool) {
	if call.Function == nil {
		return nil, false
	}
	countIdx := -1
	for i, param := range call.Params {
		if param.Semantic == typeinfo.ParamSemanticByteCount || param.Semantic == typeinfo.ParamSemanticAllocSize {
			countIdx = i
		}
	}
	if countIdx < 0 || countIdx >= len(call.Args) {
		return nil, false
	}
	// Pointer arguments that disagree on the element, such as a heap list
	// header advanced to its items, leave the count's element unknown.
	var pointee typeinfo.Type
	for i, arg := range call.Args {
		if i == countIdx || i >= len(call.Params) {
			continue
		}
		if _, ok := call.Params[i].Type.(*typeinfo.Pointer); !ok {
			continue
		}
		elem, ok := sizedPointee(cExprType(arg))
		if !ok {
			continue
		}
		if pointee != nil && !typeinfo.Equals(pointee, elem) {
			return nil, false
		}
		pointee = elem
	}
	if pointee != nil {
		target = pointee
	}
	if target == nil {
		return nil, false
	}
	count, ok := elementSizeMultiple(call.Args[countIdx], target)
	if !ok {
		return nil, false
	}
	next := *call
	next.Args = append([]Expr(nil), call.Args...)
	next.Args[countIdx] = count
	return &next, true
}

// elementSizeMultiple rewrites a byte count of k * size, or n * size, where
// size is the Win16 size of elem, to use sizeof(elem).
func elementSizeMultiple(expr Expr, elem typeinfo.Type) (Expr, bool) {
	size := uint64(elem.Bytes())
	if size == 0 {
		return nil, false
	}
	// A struct ending in a flexible array is a list header, never an array
	// element; a multiple of its size counts the list's items instead.
	header := false
	if s, ok := elem.(*typeinfo.Struct); ok {
		_, _, header = s.FlexibleArrayFieldAt(s.Size)
	}
	switch e := expr.(type) {
	case *Const:
		value := uint64(uint16(e.U64))
		if value == 0 || value%size != 0 {
			return nil, false
		}
		if value == size {
			return &SizeOf{Type: elem}, true
		}
		if header {
			return nil, false
		}
		count := &Const{TypeInfo: typeinfo.I16, U64: value / size}
		return &Binary{TypeInfo: typeinfo.U16, Op: OpMul, LHS: count, RHS: &SizeOf{Type: elem}}, true
	case *Binary:
		if e.Op != OpMul || header {
			return nil, false
		}
		if c, ok := e.RHS.(*Const); ok && uint64(uint16(c.U64)) == size {
			return &Binary{TypeInfo: e.TypeInfo, Op: OpMul, LHS: e.LHS, RHS: &SizeOf{Type: elem}, Producer: e.Producer}, true
		}
		if c, ok := e.LHS.(*Const); ok && uint64(uint16(c.U64)) == size {
			return &Binary{TypeInfo: e.TypeInfo, Op: OpMul, LHS: e.RHS, RHS: &SizeOf{Type: elem}, Producer: e.Producer}, true
		}
	}
	return nil, false
}

// structSizeFieldSrc returns sizeof(T) for an assignment of T's Win16 size
// to T's own size field.
func structSizeFieldSrc(assign *Assign) (Expr, bool) {
	c, ok := assign.Src.(*Const)
	if !ok {
		return nil, false
	}
	for _, name := range structSizeFields {
		s, ok := fieldOwner(assign.Dst, name)
		if ok && uint64(uint16(c.U64)) == uint64(s.Size) {
			return &SizeOf{Type: s}, true
		}
	}
	return nil, false
}

// callResultTempTargets maps each call result temp to the element a pointer
// it is copied into points to, so an allocation stored first in a temp is
// sized by the typed pointer that receives it.
func callResultTempTargets(f *Func) map[string]typeinfo.Type {
	targets := map[string]typeinfo.Type{}
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			assign, ok := effect.(*Assign)
			if !ok {
				continue
			}
			temp, ok := assign.Src.(*Temp)
			if !ok {
				continue
			}
			if elem, ok := sizedPointee(assign.Dst.ExprType()); ok {
				if _, seen := targets[temp.Name]; !seen {
					targets[temp.Name] = elem
				}
			}
		}
	}
	return targets
}

// allocationTarget returns the element a call result is stored as: the
// pointee of the result's own pointer type, or of the pointer a result temp
// is copied into.
func allocationTarget(result Expr, tempTargets map[string]typeinfo.Type) (typeinfo.Type, bool) {
	if elem, ok := sizedPointee(result.ExprType()); ok {
		return elem, true
	}
	temp, ok := result.(*Temp)
	if !ok {
		return nil, false
	}
	elem, ok := tempTargets[temp.Name]
	return elem, ok
}

// sizedPointee returns the element a pointer type points to when a count of
// those elements is sized by its type: a struct, a pointer, or a handle,
// each of which may be larger natively.
func sizedPointee(typ typeinfo.Type) (typeinfo.Type, bool) {
	ptr, ok := typ.(*typeinfo.Pointer)
	if !ok {
		return nil, false
	}
	switch elem := ptr.Elem.(type) {
	case *typeinfo.Struct, *typeinfo.Pointer:
		return elem, true
	case *typeinfo.Primitive:
		return elem, typeinfo.IsNative(elem, typeinfo.NativePointer)
	}
	return nil, false
}

// structPointee returns the struct a pointer type points to.
func structPointee(typ typeinfo.Type) (*typeinfo.Struct, bool) {
	ptr, ok := typ.(*typeinfo.Pointer)
	if !ok {
		return nil, false
	}
	s, ok := ptr.Elem.(*typeinfo.Struct)
	return s, ok
}
