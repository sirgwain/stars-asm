package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestLowerMergesProcessorInsertsAssignmentsOnIncomingEdges verifies straight edge lowering.
func TestLowerMergesProcessorInsertsAssignmentsOnIncomingEdges(t *testing.T) {
	fn := &Func{
		CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
			jccForReturnSinkTest(0x1000, 0x1004),
			jmpForReturnSinkTest(0x1002, 0x1006),
			jmpForReturnSinkTest(0x1004, 0x1006),
			retForReturnSinkTest(0x1006),
		}),
		Blocks: []Block{
			{ID: 0x1000, Effects: []Effect{&Branch{TrueBlock: 0x1004, FalseBlock: 0x1002}}},
			{ID: 0x1002, Effects: []Effect{&Jump{To: 0x1006}}},
			{ID: 0x1004, Effects: []Effect{&Jump{To: 0x1006}}},
			{
				ID: 0x1006,
				Effects: []Effect{
					&Assign{
						Dst: testLocal("dst", typeinfo.U16),
						Src: &Merge{
							TypeInfo: typeinfo.U16,
							Join:     0x1006,
							Arms: []MergeArm{
								{Block: 0x1002, Value: testConst(0x1)},
								{Block: 0x1004, Value: testConst(0x2)},
							},
						},
					},
				},
			},
		},
	}

	if changed := (&lowerMergesProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := formatEffects(fn.Blocks[1].Effects), []string{"t_merge_1006_0001 = 0x1", "goto L_1006"}; !equalStrings(got, want) {
		t.Fatalf("block 1002 effects = %#v, want %#v", got, want)
	}
	if got, want := formatEffects(fn.Blocks[2].Effects), []string{"t_merge_1006_0001 = 0x2", "goto L_1006"}; !equalStrings(got, want) {
		t.Fatalf("block 1004 effects = %#v, want %#v", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[3].Effects[0]), "dst = t_merge_1006_0001"; got != want {
		t.Fatalf("join effect = %q, want %q", got, want)
	}
	assertMergeAnalysisZero(t, fn)
}

// TestLowerMergesProcessorHandlesMultipleMergesInBlock verifies per-merge temp creation and edge ordering.
func TestLowerMergesProcessorHandlesMultipleMergesInBlock(t *testing.T) {
	fn := &Func{
		CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
			jccForReturnSinkTest(0x1000, 0x1004),
			jmpForReturnSinkTest(0x1002, 0x1006),
			jmpForReturnSinkTest(0x1004, 0x1006),
			retForReturnSinkTest(0x1006),
		}),
		Blocks: []Block{
			{ID: 0x1000, Effects: []Effect{&Branch{TrueBlock: 0x1004, FalseBlock: 0x1002}}},
			{ID: 0x1002, Effects: []Effect{&Jump{To: 0x1006}}},
			{ID: 0x1004, Effects: []Effect{&Jump{To: 0x1006}}},
			{
				ID: 0x1006,
				Effects: []Effect{
					&Assign{
						Dst: testLocal("dst", typeinfo.U16),
						Src: &Binary{
							TypeInfo: typeinfo.U16,
							Op:       OpAdd,
							LHS: &Merge{
								TypeInfo: typeinfo.U16,
								Join:     0x1006,
								Arms: []MergeArm{
									{Block: 0x1002, Value: testConst(0x1)},
									{Block: 0x1004, Value: testConst(0x2)},
								},
							},
							RHS: &Merge{
								TypeInfo: typeinfo.U16,
								Join:     0x1006,
								Arms: []MergeArm{
									{Block: 0x1002, Value: testConst(0x10)},
									{Block: 0x1004, Value: testConst(0x20)},
								},
							},
						},
					},
				},
			},
		},
	}

	if changed := (&lowerMergesProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := formatEffects(fn.Blocks[1].Effects), []string{"t_merge_1006_0001 = 0x1", "t_merge_1006_0002 = 0x10", "goto L_1006"}; !equalStrings(got, want) {
		t.Fatalf("block 1002 effects = %#v, want %#v", got, want)
	}
	if got, want := formatEffects(fn.Blocks[2].Effects), []string{"t_merge_1006_0001 = 0x2", "t_merge_1006_0002 = 0x20", "goto L_1006"}; !equalStrings(got, want) {
		t.Fatalf("block 1004 effects = %#v, want %#v", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[3].Effects[0]), "dst = (t_merge_1006_0001 + t_merge_1006_0002)"; got != want {
		t.Fatalf("join effect = %q, want %q", got, want)
	}
	assertMergeAnalysisZero(t, fn)
}

// TestLowerMergesProcessorSplitsBranchEdge verifies assignments can target one outgoing edge.
func TestLowerMergesProcessorSplitsBranchEdge(t *testing.T) {
	fn := &Func{
		CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
			jccForReturnSinkTest(0x1000, 0x1004),
			jmpForReturnSinkTest(0x1002, 0x1004),
			retForReturnSinkTest(0x1004),
		}),
		Blocks: []Block{
			{ID: 0x1000, Effects: []Effect{&Branch{TrueBlock: 0x1004, FalseBlock: 0x1002}}},
			{ID: 0x1002, Effects: []Effect{&Jump{To: 0x1004}}},
			{
				ID: 0x1004,
				Effects: []Effect{
					&Return{
						Value: &Merge{
							TypeInfo: typeinfo.U16,
							Join:     0x1004,
							Arms: []MergeArm{
								{Block: 0x1000, Value: testConst(0x1)},
								{Block: 0x1002, Value: testConst(0x2)},
							},
						},
					},
				},
			},
		},
	}

	if changed := (&lowerMergesProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := FormatEffect(fn.Blocks[0].Effects[0]), "branch <nil> ? L_f0000000 : L_1002"; got != want {
		t.Fatalf("branch = %q, want %q", got, want)
	}
	if got, want := formatEffects(fn.Blocks[1].Effects), []string{"t_merge_1004_0001 = 0x1", "goto L_1004"}; !equalStrings(got, want) {
		t.Fatalf("split effects = %#v, want %#v", got, want)
	}
	if got, want := fn.CFG.Successors(0x1000), []machine.BlockID{0x1002, 0xf0000000}; !equalBlockIDs(got, want) {
		t.Fatalf("successors 1000 = %#v, want %#v", got, want)
	}
	if got, want := fn.CFG.Predecessors(0x1004), []machine.BlockID{0x1002, 0xf0000000}; !equalBlockIDs(got, want) {
		t.Fatalf("predecessors 1004 = %#v, want %#v", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[3].Effects[0]), "return t_merge_1004_0001"; got != want {
		t.Fatalf("join effect = %q, want %q", got, want)
	}
	assertMergeAnalysisZero(t, fn)
}

// TestLowerMergesProcessorPlacesAssignmentsAtJoinForLaterUse verifies join-based placement.
func TestLowerMergesProcessorPlacesAssignmentsAtJoinForLaterUse(t *testing.T) {
	fn := &Func{
		CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
			jccForReturnSinkTest(0x1000, 0x1004),
			jmpForReturnSinkTest(0x1002, 0x1006),
			jmpForReturnSinkTest(0x1004, 0x1006),
			jccForReturnSinkTest(0x1006, 0x1010),
			retForReturnSinkTest(0x1008),
			retForReturnSinkTest(0x1010),
		}),
		Blocks: []Block{
			{ID: 0x1000, Effects: []Effect{&Branch{TrueBlock: 0x1004, FalseBlock: 0x1002}}},
			{ID: 0x1002, Effects: []Effect{&Jump{To: 0x1006}}},
			{ID: 0x1004, Effects: []Effect{&Jump{To: 0x1006}}},
			{ID: 0x1006, Effects: []Effect{&Branch{TrueBlock: 0x1010, FalseBlock: 0x1008}}},
			{
				ID: 0x1008,
				Effects: []Effect{
					&Return{
						Value: &Merge{
							TypeInfo: typeinfo.U16,
							Join:     0x1006,
							Arms: []MergeArm{
								{Block: 0x1002, Value: testConst(0x1)},
								{Block: 0x1004, Value: testConst(0x2)},
							},
						},
					},
				},
			},
			{ID: 0x1010, Effects: []Effect{&Return{Value: testConst(0x0)}}},
		},
	}

	if changed := (&lowerMergesProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := formatEffects(fn.Blocks[1].Effects), []string{"t_merge_1006_0001 = 0x1", "goto L_1006"}; !equalStrings(got, want) {
		t.Fatalf("block 1002 effects = %#v, want %#v", got, want)
	}
	if got, want := formatEffects(fn.Blocks[2].Effects), []string{"t_merge_1006_0001 = 0x2", "goto L_1006"}; !equalStrings(got, want) {
		t.Fatalf("block 1004 effects = %#v, want %#v", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[4].Effects[0]), "return t_merge_1006_0001"; got != want {
		t.Fatalf("later use = %q, want %q", got, want)
	}
	assertMergeAnalysisZero(t, fn)
}

// testConst builds a uint16 constant for merge tests.
func testConst(value uint64) *Const {
	return &Const{TypeInfo: typeinfo.U16, U64: value}
}

// testLocal builds a local lvalue for merge tests.
func testLocal(name string, typ typeinfo.Type) *Local {
	return &Local{FunctionVar: typeinfo.FunctionVar{Name: name, Type: typ}}
}

// formatEffects renders a slice of semantic effects.
func formatEffects(effects []Effect) []string {
	out := make([]string, len(effects))
	for i, effect := range effects {
		out[i] = FormatEffect(effect)
	}
	return out
}

// assertMergeAnalysisZero checks that no merge expressions remain.
func assertMergeAnalysisZero(t *testing.T, fn *Func) {
	t.Helper()
	if got := fn.Analyze(nil).Merges; got != 0 {
		t.Fatalf("merge count = %d, want 0", got)
	}
}

// equalStrings compares string slices without reflection.
func equalStrings(a []string, b []string) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}

// equalBlockIDs compares block ID slices without reflection.
func equalBlockIDs(a []machine.BlockID, b []machine.BlockID) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}
