package machine

import (
	"reflect"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestCFGGraphStronglyConnectedComponents(t *testing.T) {
	cfg := cfgForGraphTest(t, []asm.DecodedInst{
		jccForGraphTest(0x1000, "", 0x1006),
		nopForGraphTest(0x1002),
		jmpForGraphTest(0x1004, 0x1000),
		retForGraphTest(0x1006),
	})

	if got, want := cfg.Successors(0x1000), []BlockID{0x1002, 0x1006}; !reflect.DeepEqual(got, want) {
		t.Fatalf("Successors(0x1000) = %#v, want %#v", got, want)
	}
	if got, want := cfg.Predecessors(0x1000), []BlockID{0x1002}; !reflect.DeepEqual(got, want) {
		t.Fatalf("Predecessors(0x1000) = %#v, want %#v", got, want)
	}

	want := [][]BlockID{{0x1000, 0x1002}, {0x1006}}
	if got := cfg.SCCs; !reflect.DeepEqual(got, want) {
		t.Fatalf("CFG.SCCs = %#v, want %#v", got, want)
	}
	if got := computeStronglyConnectedComponents(cfg); !reflect.DeepEqual(got, want) {
		t.Fatalf("StronglyConnectedComponents() = %#v, want %#v", got, want)
	}
}

func TestCFGSuccessorsResolveConditionalTargetTrampoline(t *testing.T) {
	cfg := cfgForCollapsedGraphTest(t, []asm.DecodedInst{
		jccForGraphTest(0x1000, "", 0x1010),
		retForGraphTest(0x1002),
		jmpForGraphTest(0x1010, 0x1012),
		retForGraphTest(0x1012),
	})

	if got, want := cfg.Successors(0x1000), []BlockID{0x1002, 0x1012}; !reflect.DeepEqual(got, want) {
		t.Fatalf("Successors(0x1000) = %#v, want %#v", got, want)
	}
	if got, want := cfg.Predecessors(0x1010), []BlockID(nil); !reflect.DeepEqual(got, want) {
		t.Fatalf("Predecessors(0x1010) = %#v, want %#v", got, want)
	}
}

func TestCFGSuccessorsResolveConditionalFallthroughTrampoline(t *testing.T) {
	cfg := cfgForCollapsedGraphTest(t, []asm.DecodedInst{
		jccForGraphTest(0x1000, "", 0x1010),
		jmpForGraphTest(0x1002, 0x1012),
		retForGraphTest(0x1010),
		retForGraphTest(0x1012),
	})

	if got, want := cfg.Successors(0x1000), []BlockID{0x1010, 0x1012}; !reflect.DeepEqual(got, want) {
		t.Fatalf("Successors(0x1000) = %#v, want %#v", got, want)
	}
	if got, want := cfg.Predecessors(0x1002), []BlockID(nil); !reflect.DeepEqual(got, want) {
		t.Fatalf("Predecessors(0x1002) = %#v, want %#v", got, want)
	}
}

func TestCFGResolveJumpTrampolineStopsAtCycle(t *testing.T) {
	cfg := cfgForGraphTest(t, []asm.DecodedInst{
		jmpForGraphTest(0x1000, 0x1002),
		jmpForGraphTest(0x1002, 0x1000),
	})

	if got, want := cfg.resolveJumpTrampoline(0x1000), BlockID(0x1000); got != want {
		t.Fatalf("ResolveJumpTrampoline(0x1000) = %#v, want %#v", got, want)
	}
	if got, want := cfg.Successors(0x1000), []BlockID{0x1002}; !reflect.DeepEqual(got, want) {
		t.Fatalf("Successors(0x1000) = %#v, want %#v", got, want)
	}
}

func TestExtractBranchEffectResolvesTrampolines(t *testing.T) {
	ctx := ctxForGraphTest()
	cfg, err := BuildCFG(ctx, []asm.DecodedInst{
		jccForGraphTest(0x1000, "JNZ", 0x1010),
		jmpForGraphTest(0x1002, 0x1012),
		jmpForGraphTest(0x1010, 0x1014),
		nopForGraphTest(0x1012),
		nopForGraphTest(0x1014),
	}, false, CFGOptions{CollapseJumps: true})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}

	effects := Extract(ctx, cfg, ExtractOptions{})
	if len(effects.Blocks) == 0 || len(effects.Blocks[0].Effects) != 1 {
		t.Fatalf("first block effects = %#v, want one branch effect", effects.Blocks)
	}
	branch, ok := effects.Blocks[0].Effects[0].(BranchEffect)
	if !ok {
		t.Fatalf("first effect = %T, want BranchEffect", effects.Blocks[0].Effects[0])
	}
	if got, want := branch.TrueBlock, BlockID(0x1014); got != want {
		t.Fatalf("BranchEffect.TrueBlock = %#v, want %#v", got, want)
	}
	if got, want := branch.FalseBlock, BlockID(0x1012); got != want {
		t.Fatalf("BranchEffect.FalseBlock = %#v, want %#v", got, want)
	}
}

func TestExtractBranchEffectKeepsRawTrampolinesWhenCollapseDisabled(t *testing.T) {
	ctx := ctxForGraphTest()
	cfg, err := BuildCFG(ctx, []asm.DecodedInst{
		jccForGraphTest(0x1000, "JNZ", 0x1010),
		jmpForGraphTest(0x1002, 0x1012),
		jmpForGraphTest(0x1010, 0x1014),
		nopForGraphTest(0x1012),
		nopForGraphTest(0x1014),
	}, false, CFGOptions{})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}

	effects := Extract(ctx, cfg, ExtractOptions{})
	branch, ok := effects.Blocks[0].Effects[0].(BranchEffect)
	if !ok {
		t.Fatalf("first effect = %T, want BranchEffect", effects.Blocks[0].Effects[0])
	}
	if got, want := branch.TrueBlock, BlockID(0x1010); got != want {
		t.Fatalf("BranchEffect.TrueBlock = %#v, want %#v", got, want)
	}
	if got, want := branch.FalseBlock, BlockID(0x1002); got != want {
		t.Fatalf("BranchEffect.FalseBlock = %#v, want %#v", got, want)
	}
}

func TestCFGCollapseJumpsHidesTrampolinesAtBuildTime(t *testing.T) {
	cfg := cfgForCollapsedGraphTest(t, []asm.DecodedInst{
		jccForGraphTest(0x1000, "", 0x1010),
		jmpForGraphTest(0x1002, 0x1012),
		jmpForGraphTest(0x1010, 0x1014),
		retForGraphTest(0x1012),
		retForGraphTest(0x1014),
	})

	got := make([]BlockID, 0, len(cfg.Blocks))
	for _, block := range cfg.Blocks {
		got = append(got, block.ID)
	}
	want := []BlockID{0x1000, 0x1012, 0x1014}
	if !reflect.DeepEqual(got, want) {
		t.Fatalf("collapsed CFG blocks = %#v, want %#v", got, want)
	}
}

func TestCFGCollapseJumpsRetargetsBranchesAtBuildTime(t *testing.T) {
	cfg := cfgForCollapsedGraphTest(t, []asm.DecodedInst{
		jccForGraphTest(0x1000, "JNZ", 0x1010),
		jmpForGraphTest(0x1002, 0x1012),
		jmpForGraphTest(0x1010, 0x1014),
		retForGraphTest(0x1012),
		retForGraphTest(0x1014),
	})

	instrs := cfg.BlockInstrs(0x1000)
	if len(instrs) != 1 {
		t.Fatalf("BlockInstrs(0x1000) length = %d, want 1", len(instrs))
	}
	jmp := cfg.Jumps[instrs[0].Off]
	if got, want := jmp.TargetOff, uint32(0x1014); got != want {
		t.Fatalf("collapsed TargetOff = 0x%04x, want 0x%04x", got, want)
	}
	if got, want := cfg.InstructionOperands(instrs[0]), "L_1014"; got != want {
		t.Fatalf("collapsed operands = %q, want %q", got, want)
	}
}

func TestCFGCollapseJumpsInvertsJccWhenFallthroughTrampolineRemoved(t *testing.T) {
	cfg := cfgForCollapsedGraphTest(t, []asm.DecodedInst{
		jccForGraphTest(0x1000, "JLE", 0x1004),
		jmpForGraphTest(0x1002, 0x1008),
		jccForGraphTest(0x1004, "JGE", 0x1008),
		jmpForGraphTest(0x1006, 0x100a),
		nopForGraphTest(0x1008),
		nopForGraphTest(0x100a),
	})

	if got, want := graphTestBlockIDs(cfg.Blocks), []BlockID{0x1000, 0x1004, 0x1008, 0x100a}; !reflect.DeepEqual(got, want) {
		t.Fatalf("collapsed CFG blocks = %#v, want %#v", got, want)
	}
	assertJump(t, cfg, cfg.BlockInstrs(0x1000)[0], "JG", 0x1008)
	assertJump(t, cfg, cfg.BlockInstrs(0x1004)[0], "JL", 0x100a)
	if got, want := cfg.Successors(0x1000), []BlockID{0x1004, 0x1008}; !reflect.DeepEqual(got, want) {
		t.Fatalf("Successors(0x1000) = %#v, want %#v", got, want)
	}
	if got, want := cfg.Successors(0x1004), []BlockID{0x1008, 0x100a}; !reflect.DeepEqual(got, want) {
		t.Fatalf("Successors(0x1004) = %#v, want %#v", got, want)
	}
}

func TestCFGCollapseJumpsInvertsCarryAndZeroBranches(t *testing.T) {
	cfg := cfgForCollapsedGraphTest(t, []asm.DecodedInst{
		jccForGraphTest(0x1000, "JC", 0x1004),
		jmpForGraphTest(0x1002, 0x1006),
		nopForGraphTest(0x1004),
		jccForGraphTest(0x1006, "JZ", 0x100a),
		jmpForGraphTest(0x1008, 0x100c),
		nopForGraphTest(0x100a),
		nopForGraphTest(0x100c),
	})

	assertJump(t, cfg, cfg.BlockInstrs(0x1000)[0], "JNC", 0x1006)
	assertJump(t, cfg, cfg.BlockInstrs(0x1006)[0], "JNZ", 0x100c)
}

func TestCFGCollapseJumpsRemovesRedundantJumpToNextBlock(t *testing.T) {
	cfg := cfgForCollapsedGraphTest(t, []asm.DecodedInst{
		{Off: 0x1000, Len: 2, Op: asm.OpMOV, Mnemonic: "MOV"},
		jmpForGraphTest(0x1002, 0x1004),
		retForGraphTest(0x1004),
	})

	instrs := cfg.BlockInstrs(0x1000)
	if got, want := len(instrs), 1; got != want {
		t.Fatalf("BlockInstrs(0x1000) length = %d, want %d", got, want)
	}
	if got, want := instrs[0].Off, uint32(0x1000); got != want {
		t.Fatalf("remaining instruction offset = 0x%04x, want 0x%04x", got, want)
	}
	if got, want := cfg.Successors(0x1000), []BlockID{0x1004}; !reflect.DeepEqual(got, want) {
		t.Fatalf("Successors(0x1000) = %#v, want %#v", got, want)
	}
}

func graphTestBlockIDs(blocks []*Block) []BlockID {
	out := make([]BlockID, 0, len(blocks))
	for _, block := range blocks {
		out = append(out, block.ID)
	}
	return out
}

func assertJump(t *testing.T, cfg *CFG, inst asm.DecodedInst, mnemonic string, target uint32) {
	t.Helper()
	jmp := cfg.Jumps[inst.Off]
	if jmp == nil {
		t.Fatalf("instruction at 0x%04x has no jump metadata", inst.Off)
	}
	if got := inst.Mnemonic; got != mnemonic {
		t.Fatalf("jump mnemonic = %q, want %q", got, mnemonic)
	}
	if got := jmp.TargetOff; got != target {
		t.Fatalf("jump target = 0x%04x, want 0x%04x", got, target)
	}
}

func cfgForGraphTest(t *testing.T, instrs []asm.DecodedInst) *CFG {
	t.Helper()
	ctx := ctxForGraphTest()
	cfg, err := BuildCFG(ctx, instrs, false, CFGOptions{})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}
	return cfg
}

func cfgForCollapsedGraphTest(t *testing.T, instrs []asm.DecodedInst) *CFG {
	t.Helper()
	ctx := ctxForGraphTest()
	cfg, err := BuildCFG(ctx, instrs, false, CFGOptions{CollapseJumps: true})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}
	return cfg
}

// jccForGraphTest builds a conditional jump instruction for graph tests.
func jccForGraphTest(off uint32, mnemonic string, target uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 2, Op: asm.OpJcc, Mnemonic: mnemonic, Target: int32(target)}
}

// jmpForGraphTest builds an unconditional jump instruction for graph tests.
func jmpForGraphTest(off uint32, target uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: int32(target)}
}

// nopForGraphTest builds a NOP instruction for graph tests.
func nopForGraphTest(off uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 1, Op: asm.OpNOP, Mnemonic: "NOP"}
}

// retForGraphTest builds a RET instruction for graph tests.
func retForGraphTest(off uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 1, Op: asm.OpRET, Mnemonic: "RET"}
}

func ctxForGraphTest() *FuncContext {
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	return NewFuncContext(nil, sdb, symresolve.NewResolver(img, sdb), &typeinfo.Function{})
}
