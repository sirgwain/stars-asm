package region

import (
	"github.com/sirgwain/stars-asm/dasm/stars/ir"
)

// hasSideEffects reports whether evaluating e can change state: it contains
// a function call or a postfix increment or decrement.
func hasSideEffects(e ir.Expr) bool {
	switch e := e.(type) {
	case *ir.Call:
		return true
	case *ir.Unary:
		return e.Postfix || hasSideEffects(e.X)
	case *ir.Binary:
		return hasSideEffects(e.LHS) || hasSideEffects(e.RHS)
	case *ir.Cond:
		return hasSideEffects(e.Cond) || hasSideEffects(e.Then) || hasSideEffects(e.Else)
	case *ir.Cast:
		return hasSideEffects(e.Value)
	case *ir.Index:
		return hasSideEffects(e.Base) || hasSideEffects(e.Index)
	case *ir.Field:
		return hasSideEffects(e.Base)
	case *ir.Macro:
		for _, a := range e.Args {
			if hasSideEffects(a) {
				return true
			}
		}
	case *ir.AddressOf:
		return hasSideEffects(e.Target)
	case *ir.Deref:
		return hasSideEffects(e.Pointer)
	case *ir.PointerOffset:
		return hasSideEffects(e.Pointer) || hasSideEffects(e.Offset)
	}
	return false
}
