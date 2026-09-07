package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

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
