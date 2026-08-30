package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// sameLValue reports whether two lvalues name the same semantic storage.
func sameLValue(a, b LValue) bool {
	switch av := a.(type) {
	case *Local:
		bv, ok := b.(*Local)
		return ok && av.Name == bv.Name && av.BPOffset == bv.BPOffset && av.ScopeID == bv.ScopeID
	case *Global:
		bv, ok := b.(*Global)
		return ok && av.GlobalVar == bv.GlobalVar
	case *Temp:
		bv, ok := b.(*Temp)
		return ok && av.ID == bv.ID && av.Name == bv.Name
	case *Part:
		bv, ok := b.(*Part)
		return ok && av.ByteOff == bv.ByteOff && av.Width == bv.Width && sameLValue(av.Base, bv.Base)
	case *ArrayIndex:
		bv, ok := b.(*ArrayIndex)
		return ok && sameExpr(av.Base, bv.Base) && sameExpr(av.Index, bv.Index)
	case *FieldAccess:
		bv, ok := b.(*FieldAccess)
		return ok && sameStructField(av.Field, bv.Field) && sameExpr(av.Base, bv.Base)
	case *Deref:
		bv, ok := b.(*Deref)
		return ok && av.ByteOff == bv.ByteOff && av.Width == bv.Width && sameExpr(av.Pointer, bv.Pointer)
	case *SymbolRef:
		bv, ok := b.(*SymbolRef)
		// TODO: evaluate this. It' s a string compare...
		return ok && av.Path.CDecl() == bv.Path.CDecl()
	case *RawMemory:
		bv, ok := b.(*RawMemory)
		return ok && av.Access.Equals(bv.Access)
	default:
		return false
	}
}

// sameStructField reports whether two struct field descriptors name the same layout field.
func sameStructField(a, b *typeinfo.StructField) bool {
	if a == nil || b == nil {
		return a == nil && b == nil
	}
	if a == b {
		return true
	}
	return a.Name == b.Name &&
		a.Offset == b.Offset &&
		a.Size == b.Size &&
		a.End == b.End &&
		typeinfo.Equals(a.Type, b.Type)
}

// sameExpr reports whether two expressions are the same limited lowering value.
func sameExpr(a, b Expr) bool {
	if a == nil || b == nil {
		return a == nil && b == nil
	}
	switch av := a.(type) {
	case *CallResult:
		bv, ok := b.(*CallResult)
		return ok && av.Function == bv.Function && av.InstOff == bv.InstOff
	case *FunctionRef:
		bv, ok := b.(*FunctionRef)
		return ok && av.Function == bv.Function && typeinfo.Equals(av.TypeInfo, bv.TypeInfo)
	case *Const:
		bv, ok := b.(*Const)
		return ok && av.U64 == bv.U64
	case *Unary:
		bv, ok := b.(*Unary)
		return ok && av.Op == bv.Op && sameExpr(av.X, bv.X)
	case *Binary:
		bv, ok := b.(*Binary)
		return ok && av.Op == bv.Op && sameExpr(av.LHS, bv.LHS) && sameExpr(av.RHS, bv.RHS)
	case *Byte:
		bv, ok := b.(*Byte)
		return ok && av.Part == bv.Part && sameExpr(av.Parent, bv.Parent) && sameExpr(av.Value, bv.Value)
	case *Cast:
		bv, ok := b.(*Cast)
		return ok && av.To == bv.To && sameExpr(av.Value, bv.Value)
	case *ResourceID:
		bv, ok := b.(*ResourceID)
		return ok && typeinfo.Equals(av.TypeInfo, bv.TypeInfo) && sameExpr(av.Value, bv.Value)
	case *Compare:
		bv, ok := b.(*Compare)
		return ok && av.Op == bv.Op && sameExpr(av.LHS, bv.LHS) && sameExpr(av.RHS, bv.RHS)
	case *SignExtend:
		bv, ok := b.(*SignExtend)
		return ok && av.FromBits == bv.FromBits && av.ToBits == bv.ToBits && sameExpr(av.Parent, bv.Parent)
	case *Word:
		bv, ok := b.(*Word)
		return ok && av.Part == bv.Part && sameExpr(av.Parent, bv.Parent)
	case *FarPointer:
		bv, ok := b.(*FarPointer)
		return ok && av.Part == bv.Part &&
			sameExpr(av.Parent, bv.Parent) &&
			sameExpr(av.Offset, bv.Offset) &&
			sameExpr(av.Segment, bv.Segment)
	case *PointerOffset:
		bv, ok := b.(*PointerOffset)
		return ok && sameExpr(av.Pointer, bv.Pointer) && sameExpr(av.Offset, bv.Offset)
	case *Words:
		bv, ok := b.(*Words)
		return ok && sameExprs(av.Words, bv.Words)
	case *RawValue:
		bv, ok := b.(*RawValue)
		return ok && machine.ValueEquals(av.Value, bv.Value)
	case *Temp:
		bv, ok := b.(*Temp)
		return ok && av.ID == bv.ID && av.Name == bv.Name
	case LValue:
		bv, ok := b.(LValue)
		return ok && sameLValue(av, bv)
	default:
		return false
	}
}

// sameExprs reports whether two expression slices have the same shape.
func sameExprs(a, b []Expr) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if !sameExpr(a[i], b[i]) {
			return false
		}
	}
	return true
}
