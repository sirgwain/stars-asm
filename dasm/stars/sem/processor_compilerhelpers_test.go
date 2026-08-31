package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCompilerHelpersProcessorLowersPureCallResultUses verifies pure helper
// calls are removed and later result uses point at the lowered expression.
func TestCompilerHelpersProcessorLowersPureCallResultUses(t *testing.T) {
	helper := compilerHelperFunction("__aFulmul", typeinfo.U32)
	result := &machine.CallResult{Target: helper, Type: helper.Ret, InstOff: 0x1000}
	block := machine.BlockEffects{Effects: []machine.Effect{
		machine.CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x1000},
			Target:   helper,
			Args:     []machine.Value{machine.ConstVal(2), machine.ConstVal(3)},
			Result:   result,
		},
		machine.StoreEffect{
			MetaInfo: machine.Meta{InstOff: 0x1005},
			Addr:     machine.MemoryAccess{Base: machine.FrameBaseVal(), Disp: -2, Width: 2},
			Src:      machine.WordVal(result, machine.WordLow),
			Width:    2,
		},
	}}

	gotBlock, changed := (&compilerHelpersProcessors{}).ProcessMachineBlock(nil, machine.FuncEffects{}, block)
	if !changed {
		t.Fatal("ProcessMachineBlock changed = false, want true")
	}
	if got, want := len(gotBlock.Effects), 1; got != want {
		t.Fatalf("effects = %d, want %d", got, want)
	}
	got := gotBlock.Effects[0].(machine.StoreEffect)
	if got, want := got.Src.String(), "loword((uint32_t)0x6)"; got != want {
		t.Fatalf("store source = %s, want %s", got, want)
	}
}

// TestCompilerHelpersProcessorRewritesLaterCallArgs verifies helper results
// are substituted into later calls in the same machine block.
func TestCompilerHelpersProcessorRewritesLaterCallArgs(t *testing.T) {
	helper := compilerHelperFunction("__aFulmul", typeinfo.U32)
	result := &machine.CallResult{Target: helper, Type: helper.Ret, InstOff: 0x1000}
	consumer := &typeinfo.Function{
		Name: "UseLong",
		Ret:  typeinfo.U16,
		Params: []typeinfo.FunctionVar{
			{Type: typeinfo.U32},
		},
	}
	block := machine.BlockEffects{Effects: []machine.Effect{
		machine.CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x1000},
			Target:   helper,
			Args:     []machine.Value{machine.ConstVal(2), machine.ConstVal(3)},
			Result:   result,
		},
		machine.CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x1005},
			Target:   consumer,
			Args:     []machine.Value{result},
			Result:   &machine.CallResult{Target: consumer, Type: consumer.Ret, InstOff: 0x1005},
		},
	}}

	gotBlock, changed := (&compilerHelpersProcessors{}).ProcessMachineBlock(nil, machine.FuncEffects{}, block)
	if !changed {
		t.Fatal("ProcessMachineBlock changed = false, want true")
	}
	if got, want := len(gotBlock.Effects), 1; got != want {
		t.Fatalf("effects = %d, want %d", got, want)
	}
	got := gotBlock.Effects[0].(machine.CallEffect)
	if got, want := got.Args[0].String(), "(uint32_t)0x6"; got != want {
		t.Fatalf("consumer arg = %s, want %s", got, want)
	}
}

// TestCompilerHelpersProcessorCarriesResultsAcrossBlocks verifies helper result
// substitutions are still available in branch-split successor blocks.
func TestCompilerHelpersProcessorCarriesResultsAcrossBlocks(t *testing.T) {
	helper := compilerHelperFunction("__aFulshr", typeinfo.U32)
	result := &machine.CallResult{Target: helper, Type: helper.Ret, InstOff: 0x181a}
	callBlock := machine.BlockEffects{Effects: []machine.Effect{
		machine.CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x181a},
			Target:   helper,
			Args:     []machine.Value{machine.ConstVal(0x80000), machine.ConstVal(0x11)},
			Result:   result,
		},
	}}
	branchBlock := machine.BlockEffects{Effects: []machine.Effect{
		machine.BranchEffect{
			MetaInfo:   machine.Meta{InstOff: 0x1830},
			Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JNZ", LHS: machine.BinaryVal(machine.ValueOpAnd, machine.WordVal(result, machine.WordHigh), machine.ConstVal(0)), RHS: machine.ConstVal(0)},
			TrueBlock:  0x18c8,
			FalseBlock: 0x1835,
		},
	}}

	processor := &compilerHelpersProcessors{}
	gotCallBlock, changed := processor.ProcessMachineBlock(nil, machine.FuncEffects{}, callBlock)
	if !changed {
		t.Fatal("call block changed = false, want true")
	}
	if got, want := len(gotCallBlock.Effects), 0; got != want {
		t.Fatalf("call block effects = %d, want %d", got, want)
	}
	gotBlock, changed := processor.ProcessMachineBlock(nil, machine.FuncEffects{}, branchBlock)
	if !changed {
		t.Fatal("branch block changed = false, want true")
	}
	if got, want := len(gotBlock.Effects), 1; got != want {
		t.Fatalf("branch block effects = %d, want %d", got, want)
	}
	got := gotBlock.Effects[0].(machine.BranchEffect)
	if got, want := got.Predicate.LHS.String(), "0x0"; got != want {
		t.Fatalf("branch lhs = %s, want %s", got, want)
	}
}

// TestCompilerHelpersProcessorKeepsFlagOnlyHelper verifies flag-only helpers
// are not removed until their predicate lowering is modeled.
func TestCompilerHelpersProcessorKeepsFlagOnlyHelper(t *testing.T) {
	helper := &typeinfo.Function{Name: "__aFfcompp", Ret: &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"}}
	result := &machine.CallResult{Target: helper, Type: helper.Ret, InstOff: 0x1000}
	block := machine.BlockEffects{Effects: []machine.Effect{
		machine.CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x1000},
			Target:   helper,
			Result:   result,
		},
	}}

	gotBlock, changed := (&compilerHelpersProcessors{}).ProcessMachineBlock(nil, machine.FuncEffects{}, block)
	if changed {
		t.Fatal("ProcessMachineBlock changed = true, want false")
	}
	if got, want := len(gotBlock.Effects), 1; got != want {
		t.Fatalf("effects = %d, want %d", got, want)
	}
}

// compilerHelperFunction builds a realistic two-argument helper signature.
func compilerHelperFunction(name string, ret typeinfo.Type) *typeinfo.Function {
	return &typeinfo.Function{
		Name: name,
		Ret:  ret,
		Params: []typeinfo.FunctionVar{
			{Type: ret},
			{Type: ret},
		},
	}
}
