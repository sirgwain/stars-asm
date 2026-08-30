package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// collapseTypedFarPointerCallArgs collapses source-level far pointer artifacts in call args.
func collapseTypedFarPointerCallArgs(ctx *FuncContext, call *Call) (*Call, bool) {
	changed := false
	args := make([]Expr, len(call.Args))
	for i, arg := range call.Args {
		var param *typeinfo.FunctionVar
		if call.Function != nil && i < len(call.Function.Params) {
			param = &call.Function.Params[i]
		}
		next, ok := collapseTypedFarPointerCallArg(ctx, arg, param)
		args[i] = next
		changed = changed || ok
	}
	if !changed {
		return call, false
	}
	next := *call
	next.Args = args
	return &next, true
}

// collapseTypedFarPointerCallArg collapses one typed far pointer call argument.
func collapseTypedFarPointerCallArg(ctx *FuncContext, arg Expr, param *typeinfo.FunctionVar) (Expr, bool) {
	if param == nil {
		return arg, false
	}
	expected := param.Type
	if param.Semantic == typeinfo.ParamSemanticResourceNameOrID {
		if value, ok := resourceIDValue(arg); ok {
			return &ResourceID{Value: value, TypeInfo: expected}, true
		}
	}
	if !typeinfo.IsFarPointer(expected) {
		return arg, false
	}
	ptr, ok := arg.(*FarPointer)
	if !ok || ptr.Part != machine.FarPointerWhole {
		return arg, false
	}
	if constExprEquals(ptr.Segment, 0) && constExprEquals(ptr.Offset, 0) {
		return &Const{TypeInfo: expected, U64: 0}, true
	}
	if exprMatchesMachineValue(ptr.Segment, machine.ScalarVal("ss")) {
		if _, ok := ptr.Offset.(*AddressOf); ok {
			return ptr.Offset, true
		}
		if ptr.Offset.ExprType() == typeinfo.U16 {
			return ptr.Offset, true
		}
		if target, ok := ptr.Offset.(LValue); ok {
			return &AddressOf{Target: target, TypeInfo: expected}, true
		}
		return arg, false
	}
	if ctx == nil || !exprMatchesMachineValue(ptr.Segment, ctx.dsReg) {
		return arg, false
	}
	if !sourcePointerMatchesFarPointer(ptr.Offset.ExprType(), expected) {
		_, expectedOK := expected.(*typeinfo.Pointer)
		if !expectedOK {
			return arg, false
		}
		if ptr.Offset.ExprType() == typeinfo.U16 {
			return ptr.Offset, true
		}
		if _, ok := ptr.Offset.(*Merge); ok {
			return ptr.Offset, true
		}
		if target, ok := ptr.Offset.(LValue); ok {
			return &AddressOf{Target: target, TypeInfo: expected}, true
		}
		return arg, false
	}
	return ptr.Offset, true
}

// resourceIDValue extracts the integer value encoded in a MAKEINTRESOURCE-style expression.
func resourceIDValue(arg Expr) (Expr, bool) {
	switch e := arg.(type) {
	case *FarPointer:
		if e.Part == machine.FarPointerWhole && constExprEquals(e.Segment, 0) {
			return e.Offset, true
		}
	case *PointerOffset:
		ptr, ok := e.Pointer.(*FarPointer)
		if ok &&
			ptr.Part == machine.FarPointerWhole &&
			constExprEquals(ptr.Segment, 0) &&
			constExprEquals(ptr.Offset, 0) {
			return e.Offset, true
		}
	}
	return nil, false
}

// sourcePointerMatchesFarPointer reports whether a source pointer can represent
// a far call arg.
func sourcePointerMatchesFarPointer(source, expected typeinfo.Type) bool {
	sourcePtr, sourceOK := source.(*typeinfo.Pointer)
	expectedPtr, expectedOK := expected.(*typeinfo.Pointer)
	if !sourceOK || !expectedOK {
		return false
	}
	if sourcePtr.IsCStringPointer() && expectedPtr.IsCStringPointer() {
		return true
	}
	return typeinfo.Equals(sourcePtr.Elem, expectedPtr.Elem)
}

// exprMatchesMachineValue reports whether a semantic expression matches a machine value.
func exprMatchesMachineValue(expr Expr, value machine.Value) bool {
	switch e := expr.(type) {
	case *Const:
		v, ok := value.(*machine.Const)
		return ok && uint(e.U64) == v.Val
	case *RawValue:
		return machine.ValueEquals(e.Value, value)
	default:
		return false
	}
}
