package machine

import "github.com/sirgwain/stars-asm/dasm/typeinfo"

// convergenceMemoryAccessEqualsDepth reports whether two memory accesses are
// equivalent for bounded dataflow convergence.
func convergenceMemoryAccessEqualsDepth(a, b MemoryAccess, depth int) bool {
	return a.Disp == b.Disp &&
		a.Width == b.Width &&
		a.Scale == b.Scale &&
		a.Origin == b.Origin &&
		convergenceValueEqualsDepth(a.Seg, b.Seg, depth+1) &&
		convergenceValueEqualsDepth(a.Base, b.Base, depth+1) &&
		convergenceValueEqualsDepth(a.Index, b.Index, depth+1)
}

// convergenceValueEquals reports whether two values are equivalent for
// dataflow convergence.
func convergenceValueEquals(a, b Value) bool {
	return convergenceValueEqualsDepth(a, b, 0)
}

// convergenceValueEqualsDepth reports whether two symbolic machine values have
// the same bounded shape for dataflow convergence.
func convergenceValueEqualsDepth(a, b Value, depth int) bool {
	if a == nil || b == nil {
		return a == nil && b == nil
	}
	if depth > 4 {
		return valueKeyDepth(a, depth) == valueKeyDepth(b, depth)
	}

	switch av := a.(type) {
	case *Unknown:
		bv, ok := b.(*Unknown)
		return ok && av.Desc == bv.Desc
	case *Const:
		bv, ok := b.(*Const)
		return ok && av.Val == bv.Val && originEquals(av.Origin, bv.Origin)
	case *Reg:
		bv, ok := b.(*Reg)
		return ok && av.Val == bv.Val
	case *FrameBase:
		_, ok := b.(*FrameBase)
		return ok
	case *FloatConst:
		bv, ok := b.(*FloatConst)
		return ok && av.Val == bv.Val
	case *CallResult:
		bv, ok := b.(*CallResult)
		return ok && callResultsEqual(av, bv)
	case *PredicateValue:
		bv, ok := b.(*PredicateValue)
		return ok && av.Kind == bv.Kind &&
			av.Op == bv.Op &&
			convergenceValueEqualsDepth(av.LHS, bv.LHS, depth+1) &&
			convergenceValueEqualsDepth(av.RHS, bv.RHS, depth+1)
	case *WordValue:
		bv, ok := b.(*WordValue)
		return ok && av.Part == bv.Part && convergenceValueEqualsDepth(av.Parent, bv.Parent, depth+1)
	case *FarPointer:
		bv, ok := b.(*FarPointer)
		return ok && av.Part == bv.Part &&
			convergenceValueEqualsDepth(av.Parent, bv.Parent, depth+1) &&
			convergenceValueEqualsDepth(av.Offset, bv.Offset, depth+1) &&
			convergenceValueEqualsDepth(av.Segment, bv.Segment, depth+1)
	case *SignExtendValue:
		bv, ok := b.(*SignExtendValue)
		return ok && av.FromBits == bv.FromBits &&
			av.ToBits == bv.ToBits &&
			convergenceValueEqualsDepth(av.Parent, bv.Parent, depth+1)
	case *StackWords:
		bv, ok := b.(*StackWords)
		return ok && convergenceValuesEqualDepth(av.Words, bv.Words, depth+1)
	case *ByteValue:
		bv, ok := b.(*ByteValue)
		return ok && convergenceByteValuesEqualDepth(av, bv, depth+1)
	case *Cast:
		bv, ok := b.(*Cast)
		return ok && typeinfo.Equals(av.To, bv.To) && convergenceValueEqualsDepth(av.Value, bv.Value, depth+1)
	case *Binary:
		bv, ok := b.(*Binary)
		return ok && av.Op == bv.Op &&
			convergenceValueEqualsDepth(av.LHS, bv.LHS, depth+1) &&
			convergenceValueEqualsDepth(av.RHS, bv.RHS, depth+1)
	case *Load:
		bv, ok := b.(*Load)
		return ok && av.ID == bv.ID && convergenceMemoryAccessEqualsDepth(av.Access, bv.Access, depth+1)
	case *Address:
		bv, ok := b.(*Address)
		return ok && convergenceMemoryAccessEqualsDepth(av.Access, bv.Access, depth+1)
	case *PhiValue:
		bv, ok := b.(*PhiValue)
		return ok && convergencePhiValuesEqualDepth(av, bv, depth+1)
	default:
		return false
	}
}

// convergenceValuesEqualDepth reports whether two value slices have the same
// bounded shape for dataflow convergence.
func convergenceValuesEqualDepth(a, b []Value, depth int) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if !convergenceValueEqualsDepth(a[i], b[i], depth+1) {
			return false
		}
	}
	return true
}

// convergenceByteValuesEqualDepth reports whether two byte projections or
// writes have the same bounded shape for dataflow convergence.
func convergenceByteValuesEqualDepth(a, b *ByteValue, depth int) bool {
	if a.Part != b.Part || !convergenceValueEqualsDepth(a.Parent, b.Parent, depth+1) {
		return false
	}
	if a.Value == nil || b.Value == nil {
		return a.Value == b.Value
	}
	return convergenceValueEqualsDepth(*a.Value, *b.Value, depth+1)
}

// convergencePhiValuesEqualDepth reports whether two phi values have the
// same bounded predecessor arms for dataflow convergence.
func convergencePhiValuesEqualDepth(a, b *PhiValue, depth int) bool {
	if a.Join != b.Join || len(a.Arms) != len(b.Arms) {
		return false
	}
	for i, aArm := range a.Arms {
		bArm := b.Arms[i]
		if phiArmBlockID(aArm) != phiArmBlockID(bArm) ||
			!convergenceValueEqualsDepth(aArm.Value, bArm.Value, depth+1) {
			return false
		}
	}
	return true
}
