package region

import (
	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// exprEqual reports whether two IR expressions have the same structure and
// values.
func exprEqual(a, b ir.Expr) bool {
	switch a := a.(type) {
	case *ir.Var:
		b, ok := b.(*ir.Var)
		return ok && a.Name == b.Name
	case *ir.IntConst:
		b, ok := b.(*ir.IntConst)
		return ok && a.Value == b.Value && a.Text == b.Text
	case *ir.FloatConst:
		b, ok := b.(*ir.FloatConst)
		return ok && a.Value == b.Value
	case *ir.SizeOf:
		b, ok := b.(*ir.SizeOf)
		return ok && a.Type == b.Type
	case *ir.StringConst:
		b, ok := b.(*ir.StringConst)
		return ok && a.Value == b.Value
	case *ir.Unary:
		b, ok := b.(*ir.Unary)
		return ok && a.Op == b.Op && a.Functional == b.Functional && exprEqual(a.X, b.X)
	case *ir.Binary:
		b, ok := b.(*ir.Binary)
		return ok && a.Op == b.Op && exprEqual(a.LHS, b.LHS) && exprEqual(a.RHS, b.RHS)
	case *ir.Cond:
		b, ok := b.(*ir.Cond)
		return ok && exprEqual(a.Cond, b.Cond) && exprEqual(a.Then, b.Then) && exprEqual(a.Else, b.Else)
	case *ir.Cast:
		b, ok := b.(*ir.Cast)
		return ok && a.Type == b.Type && exprEqual(a.Value, b.Value)
	case *ir.Index:
		b, ok := b.(*ir.Index)
		return ok && exprEqual(a.Base, b.Base) && exprEqual(a.Index, b.Index)
	case *ir.Field:
		b, ok := b.(*ir.Field)
		return ok && a.Name == b.Name && a.Pointer == b.Pointer && exprEqual(a.Base, b.Base)
	case *ir.Call:
		b, ok := b.(*ir.Call)
		return ok && exprEqual(a.Target, b.Target) && exprsEqual(a.Args, b.Args)
	case *ir.Macro:
		b, ok := b.(*ir.Macro)
		return ok && a.Name == b.Name && exprsEqual(a.Args, b.Args)
	case *ir.AddressOf:
		b, ok := b.(*ir.AddressOf)
		return ok && exprEqual(a.Target, b.Target)
	case *ir.Deref:
		b, ok := b.(*ir.Deref)
		return ok && a.ByteOff == b.ByteOff && typeinfo.Equals(a.Type, b.Type) && exprEqual(a.Pointer, b.Pointer)
	case *ir.PointerOffset:
		b, ok := b.(*ir.PointerOffset)
		return ok && typeinfo.Equals(a.Type, b.Type) && exprEqual(a.Pointer, b.Pointer) && exprEqual(a.Offset, b.Offset)
	}
	return false
}

// exprsEqual reports whether two expression lists are pairwise equal.
func exprsEqual(a, b []ir.Expr) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if !exprEqual(a[i], b[i]) {
			return false
		}
	}
	return true
}

// hasCall reports whether e contains a function call anywhere.
func hasCall(e ir.Expr) bool {
	switch e := e.(type) {
	case *ir.Call:
		return true
	case *ir.Unary:
		return hasCall(e.X)
	case *ir.Binary:
		return hasCall(e.LHS) || hasCall(e.RHS)
	case *ir.Cond:
		return hasCall(e.Cond) || hasCall(e.Then) || hasCall(e.Else)
	case *ir.Cast:
		return hasCall(e.Value)
	case *ir.Index:
		return hasCall(e.Base) || hasCall(e.Index)
	case *ir.Field:
		return hasCall(e.Base)
	case *ir.Macro:
		for _, a := range e.Args {
			if hasCall(a) {
				return true
			}
		}
	case *ir.AddressOf:
		return hasCall(e.Target)
	case *ir.Deref:
		return hasCall(e.Pointer)
	case *ir.PointerOffset:
		return hasCall(e.Pointer) || hasCall(e.Offset)
	}
	return false
}
