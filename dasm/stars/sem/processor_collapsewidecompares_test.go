package sem

import (
	"reflect"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCollapseWideCompareOrdering verifies a signed-high/unsigned-low ladder
// becomes one wide predicate and that its scaffold is isolated in the CFG.
func TestCollapseWideCompareOrdering(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JL", 0x1100),
		jccForWideCompareTest(0x1002, "JG", 0x1006),
		jccForWideCompareTest(0x1004, "JB", 0x1100),
		retForWideCompareTest(0x1006),
		retForWideCompareTest(0x1100),
	})
	lhs := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9000}
	rhs := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9002}
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JL", machine.WordVal(lhs, machine.WordHigh), machine.WordVal(rhs, machine.WordHigh), 0x1100, 0x1002),
		compareBlock(0x1002, "JG", machine.WordVal(lhs, machine.WordHigh), machine.WordVal(rhs, machine.WordHigh), 0x1006, 0x1004),
		compareBlock(0x1004, "JB", machine.WordVal(lhs, machine.WordLow), machine.WordVal(rhs, machine.WordLow), 0x1100, 0x1006),
		{Block: 0x1006},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{}).ProcessMachineFunc(nil, &fn); !changed {
		t.Fatal("ProcessMachineFunc changed = false, want true")
	}
	branch := fn.Blocks[0].Effects[0].(machine.BranchEffect)
	if got, want := branch.Predicate.Op, "JL"; got != want {
		t.Fatalf("wide predicate op = %q, want %q", got, want)
	}
	if !machine.ValueEquals(branch.Predicate.LHS, lhs) || !machine.ValueEquals(branch.Predicate.RHS, rhs) {
		t.Fatalf("wide predicate = %s, want original wide operands", branch.Predicate)
	}
	if got, want := []machine.BlockID{branch.TrueBlock, branch.FalseBlock}, []machine.BlockID{0x1100, 0x1006}; !reflect.DeepEqual(got, want) {
		t.Fatalf("wide targets = %#v, want %#v", got, want)
	}
	if got, want := cfg.Successors(0x1000), []machine.BlockID{0x1006, 0x1100}; !reflect.DeepEqual(got, want) {
		t.Fatalf("root successors = %#v, want %#v", got, want)
	}
	for _, id := range []machine.BlockID{0x1002, 0x1004} {
		if len(cfg.Successors(id)) != 0 || len(cfg.Predecessors(id)) != 0 {
			t.Fatalf("scaffold %s remains connected: pred=%v succ=%v", id, cfg.Predecessors(id), cfg.Successors(id))
		}
	}
}

// TestCollapseWideCompareEquality verifies an inverted low/high inequality tree
// is proved by outcomes rather than by one fixed branch arrangement.
func TestCollapseWideCompareEquality(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JNE", 0x1100),
		jccForWideCompareTest(0x1002, "JNE", 0x1100),
		retForWideCompareTest(0x1004),
		retForWideCompareTest(0x1100),
	})
	value := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9000}
	zero := machine.ConstVal(0)
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JNE", machine.WordVal(value, machine.WordLow), zero, 0x1100, 0x1002),
		compareBlock(0x1002, "JNE", machine.WordVal(value, machine.WordHigh), zero, 0x1100, 0x1004),
		{Block: 0x1004},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{}).ProcessMachineFunc(nil, &fn); !changed {
		t.Fatal("ProcessMachineFunc changed = false, want true")
	}
	branch := fn.Blocks[0].Effects[0].(machine.BranchEffect)
	if got, want := branch.Predicate.Op, "JNE"; got != want {
		t.Fatalf("wide predicate op = %q, want %q", got, want)
	}
	if !machine.ValueEquals(branch.Predicate.LHS, value) {
		t.Fatalf("wide lhs = %s, want %s", branch.Predicate.LHS, value)
	}
	if constant, ok := branch.Predicate.RHS.(*machine.Const); !ok || constant.Val != 0 {
		t.Fatalf("wide rhs = %#v, want dword zero", branch.Predicate.RHS)
	}
}

// TestCollapseWideCompareRejectsSideEffectingScaffold verifies source-level
// work in an intermediate block prevents control-flow removal.
func TestCollapseWideCompareRejectsSideEffectingScaffold(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JNE", 0x1100),
		jccForWideCompareTest(0x1002, "JNE", 0x1100),
		retForWideCompareTest(0x1004),
		retForWideCompareTest(0x1100),
	})
	value := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9000}
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JNE", machine.WordVal(value, machine.WordLow), machine.ConstVal(0), 0x1100, 0x1002),
		{
			Block: 0x1002,
			Effects: []machine.Effect{
				machine.CallEffect{MetaInfo: machine.Meta{BlockID: 0x1002}},
				wideCompareBranch(0x1002, "JNE", machine.WordVal(value, machine.WordHigh), machine.ConstVal(0), 0x1100, 0x1004),
			},
		},
		{Block: 0x1004},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{}).ProcessMachineFunc(nil, &fn); changed {
		t.Fatal("ProcessMachineFunc changed = true, want conservative rejection")
	}
}

// compareBlock builds a single-effect comparison block.
func compareBlock(id machine.BlockID, op string, lhs, rhs machine.Value, trueBlock, falseBlock machine.BlockID) machine.BlockEffects {
	return machine.BlockEffects{Block: id, Effects: []machine.Effect{wideCompareBranch(id, op, lhs, rhs, trueBlock, falseBlock)}}
}

// wideCompareBranch builds a machine comparison branch for processor tests.
func wideCompareBranch(id machine.BlockID, op string, lhs, rhs machine.Value, trueBlock, falseBlock machine.BlockID) machine.BranchEffect {
	return machine.BranchEffect{
		MetaInfo:   machine.Meta{BlockID: id},
		Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: op, LHS: lhs, RHS: rhs},
		TrueBlock:  trueBlock,
		FalseBlock: falseBlock,
	}
}

// cfgForWideCompareTest builds a real machine CFG for wide compare tests.
func cfgForWideCompareTest(t *testing.T, instrs []asm.DecodedInst) *machine.CFG {
	t.Helper()
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	fs := &typeinfo.Function{Ret: &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"}}
	ctx := machine.NewFuncContext(img, sdb, symresolve.NewResolver(img, sdb), fs)
	cfg, err := machine.BuildCFG(ctx, instrs, false, machine.CFGOptions{})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}
	return cfg
}

// jccForWideCompareTest builds a conditional jump instruction.
func jccForWideCompareTest(off uint32, mnemonic string, target uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 2, Op: asm.OpJcc, Mnemonic: mnemonic, Target: int32(target)}
}

// retForWideCompareTest builds a return instruction.
func retForWideCompareTest(off uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 1, Op: asm.OpRET, Mnemonic: "RET"}
}
