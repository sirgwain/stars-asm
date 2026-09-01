package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
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

// collapseTypedFarPointerCallArg collapses one typed pointer call argument.
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

	if !typeinfo.IsPointer(expected) {
		return arg, false
	}

	// A bare 16-bit value in pointer context may be a near pointer into DS.
	//
	// This catches cases such as:
	//
	//     strcpy(0x57a4, psz)
	//
	// where 0x57a4 is actually the address of a known DS global.
	_, argIsFarPointer := arg.(*FarPointer)
	if !argIsFarPointer && arg.ExprType() == typeinfo.U16 && !constExprEquals(arg, 0) {
		ptr := &FarPointer{
			Segment:  &Register{Val: asm.RegDS, SegNum: ctx.segFromRegister(asm.RegDS)},
			Offset:   arg,
			Part:     machine.FarPointerWhole,
			TypeInfo: expected,
		}
		if resolved, ok := ctx.resolveSemanticFarPointer(ptr); ok {
			if decayed, ok := decayArrayAddress(resolved, expected); ok {
				return decayed, true
			}
			return resolved, true
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

	if exprMatchesMachineValue(ptr.Segment, machine.RegVal(asm.RegSS)) {
		if decayed, ok := decayArrayAddress(ptr.Offset, expected); ok {
			return decayed, true
		}
		if _, ok := ptr.Offset.(*AddressOf); ok {
			return ptr.Offset, true
		}
		if ptr.Offset.ExprType() == typeinfo.U16 {
			return ptr.Offset, true
		}
		if target, ok := ptr.Offset.(LValue); ok {
			if decayed, ok := decayArrayLValue(target, expected); ok {
				return decayed, true
			}
			return &AddressOf{Target: target, TypeInfo: expected}, true
		}
		return arg, false
	}

	if _, ok := ptr.Segment.(*Register); !ok {
		return arg, false
	}

	if !sourcePointerMatchesFarPointer(ptr.Offset.ExprType(), expected) {
		if _, ok := expected.(*typeinfo.Pointer); !ok {
			return arg, false
		}

		if ptr.Offset.ExprType() == typeinfo.U16 {
			return ptr.Offset, true
		}
		if _, ok := ptr.Offset.(*Merge); ok {
			return ptr.Offset, true
		}
		if target, ok := ptr.Offset.(LValue); ok {
			if decayed, ok := decayArrayLValue(target, expected); ok {
				return decayed, true
			}
			return &AddressOf{Target: target, TypeInfo: expected}, true
		}

		return arg, false
	}

	return ptr.Offset, true
}

// decayArrayAddress converts &array or &array[0] to array in pointer context.
func decayArrayAddress(expr Expr, expected typeinfo.Type) (Expr, bool) {
	addr, ok := expr.(*AddressOf)
	if !ok {
		return expr, false
	}
	return decayArrayLValue(addr.Target, expected)
}

// decayArrayLValue returns an array lvalue where C would decay it to a pointer.
func decayArrayLValue(target LValue, expected typeinfo.Type) (Expr, bool) {
	arrayExpr := Expr(target)
	targetType, ok := target.ExprType().(*typeinfo.Array)
	if !ok {
		if index, indexOK := target.(*ArrayIndex); indexOK && constExprEquals(index.Index, 0) {
			if array, arrayOK := index.Base.ExprType().(*typeinfo.Array); arrayOK {
				arrayExpr = index.Base
				targetType = array
				ok = true
			}
		}
	}
	if !ok {
		return target, false
	}
	expectedPtr, ok := expected.(*typeinfo.Pointer)
	if !ok {
		return target, false
	}
	if expectedPtr.IsCStringPointer() && targetType.IsCStringArray() {
		return arrayExpr, true
	}
	if typeinfo.IsCallCompatible(expectedPtr.Elem, targetType.Elem) {
		return arrayExpr, true
	}
	return target, false
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
	case *Register:
		v, ok := value.(*machine.Reg)
		return ok && e.Val == v.Val
	case *RawValue:
		return machine.ValueEquals(e.Value, value)
	default:
		return false
	}
}
