package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// decayArrayLValue returns an array lvalue where C would decay it to a pointer.
// decayArrayLValue returns an array lvalue where C would decay it to a pointer.
func decayArrayLValue(target LValue, expected typeinfo.Type) (Expr, bool) {
	targetType, ok := target.ExprType().(*typeinfo.Array)
	if !ok {
		return target, false
	}

	expectedPtr, ok := expected.(*typeinfo.Pointer)
	if !ok {
		return target, false
	}

	// An array decays to a pointer to its first element, and any object
	// pointer is implicitly convertible to void *.
	if expectedPtr.Elem.Kind() == typeinfo.KVoid {
		return target, true
	}

	if expectedPtr.IsCStringPointer() && targetType.IsCStringArray() {
		return target, true
	}
	if typeinfo.IsCallCompatible(expectedPtr.Elem, targetType.Elem) {
		return target, true
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
