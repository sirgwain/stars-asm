package machine

import "testing"

// phiValue builds a materialized phi value for source-visible consumers such
// as calls, stores, branches, and returns.
func phiValue(arms []PhiArm) Value {
	return phiValueWithPrevious(0, arms, nil, nil)
}

func TestPhiValueFlattensAndDedupesNestedArms(t *testing.T) {
	b1 := &Block{ID: 1}
	b2 := &Block{ID: 2}
	b3 := &Block{ID: 3}

	inner := phiValue([]PhiArm{
		{Block: b2, Value: ScalarVal("bx")},
		{Block: b1, Value: ScalarVal("ax")},
	})

	got := phiValue([]PhiArm{
		{Block: b3, Value: ScalarVal("cx")},
		{Block: b2, Value: ScalarVal("bx")},
		{Block: b1, Value: inner},
	})

	merge, ok := got.(*PhiValue)
	if !ok {
		t.Fatalf("phi = %T %[1]s, want PhiValue", got)
	}
	if got, want := merge.String(), "merge(Join: L_0000, (L_0001:ax, L_0002:bx, L_0003:cx))"; got != want {
		t.Fatalf("phi = %q, want %q", got, want)
	}
}

func TestStackWordsValueCombinesMatchingPhiShapesByArm(t *testing.T) {
	b1 := &Block{ID: 1}
	b2 := &Block{ID: 2}

	lo := phiValue([]PhiArm{
		{Block: b1, Value: ScalarVal("ax")},
		{Block: b2, Value: ScalarVal("bx")},
	})
	hi := phiValue([]PhiArm{
		{Block: b1, Value: ScalarVal("dx")},
		{Block: b2, Value: ScalarVal("cx")},
	})

	got := stackWordsValue([]Value{lo, hi})
	merge, ok := got.(*PhiValue)
	if !ok {
		t.Fatalf("wide phi = %T %[1]s, want PhiValue", got)
	}
	if got, want := merge.String(), "merge(Join: L_0000, (L_0001:words(ax, dx), L_0002:words(bx, cx)))"; got != want {
		t.Fatalf("wide phi = %q, want %q", got, want)
	}
}
