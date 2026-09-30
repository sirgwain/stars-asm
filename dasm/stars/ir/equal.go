package ir

import "github.com/sirgwain/stars-asm/dasm/typeinfo"

// ExprEqual reports whether two IR expressions have the same structure and
// values.
func ExprEqual(a, b Expr) bool {
	switch a := a.(type) {
	case *Var:
		b, ok := b.(*Var)
		return ok && a.Name == b.Name
	case *IntConst:
		b, ok := b.(*IntConst)
		return ok && a.Value == b.Value && a.Text == b.Text
	case *FloatConst:
		b, ok := b.(*FloatConst)
		return ok && a.Value == b.Value
	case *SizeOf:
		b, ok := b.(*SizeOf)
		return ok && a.Type == b.Type
	case *StringConst:
		b, ok := b.(*StringConst)
		return ok && a.Value == b.Value
	case *Unary:
		b, ok := b.(*Unary)
		return ok && a.Op == b.Op && a.Postfix == b.Postfix && ExprEqual(a.X, b.X)
	case *Binary:
		b, ok := b.(*Binary)
		return ok && a.Op == b.Op && ExprEqual(a.LHS, b.LHS) && ExprEqual(a.RHS, b.RHS)
	case *Cond:
		b, ok := b.(*Cond)
		return ok && ExprEqual(a.Cond, b.Cond) && ExprEqual(a.Then, b.Then) && ExprEqual(a.Else, b.Else)
	case *Cast:
		b, ok := b.(*Cast)
		return ok && a.Type == b.Type && ExprEqual(a.Value, b.Value)
	case *Index:
		b, ok := b.(*Index)
		return ok && ExprEqual(a.Base, b.Base) && ExprEqual(a.Index, b.Index)
	case *Field:
		b, ok := b.(*Field)
		return ok && a.Name == b.Name && a.Pointer == b.Pointer && ExprEqual(a.Base, b.Base)
	case *Call:
		b, ok := b.(*Call)
		return ok && ExprEqual(a.Target, b.Target) && ExprsEqual(a.Args, b.Args)
	case *Macro:
		b, ok := b.(*Macro)
		return ok && a.Name == b.Name && ExprsEqual(a.Args, b.Args)
	case *AddressOf:
		b, ok := b.(*AddressOf)
		return ok && ExprEqual(a.Target, b.Target)
	case *Deref:
		b, ok := b.(*Deref)
		return ok && a.ByteOff == b.ByteOff && typeinfo.Equals(a.Type, b.Type) && ExprEqual(a.Pointer, b.Pointer)
	case *PointerOffset:
		b, ok := b.(*PointerOffset)
		return ok && typeinfo.Equals(a.Type, b.Type) && ExprEqual(a.Pointer, b.Pointer) && ExprEqual(a.Offset, b.Offset)
	}
	return false
}

// ExprsEqual reports whether two expression lists are pairwise equal.
func ExprsEqual(a, b []Expr) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if !ExprEqual(a[i], b[i]) {
			return false
		}
	}
	return true
}
