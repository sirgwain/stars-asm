package machine

import (
	"slices"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestTableJumpPreservesEntriesAndResolvesTrampolines exercises extraction with
// duplicate table entries, a trampoline chain, and both CFG collapse settings.
func TestTableJumpPreservesEntriesAndResolvesTrampolines(t *testing.T) {
	insts := []asm.DecodedInst{
		{Off: 0x1000, Len: 3, Op: asm.OpMOV, Mnemonic: "MOV", Dst: asm.Operand{Kind: asm.OKReg, Reg: asm.RegBX}, Src: asm.Operand{Kind: asm.OKImm, Imm: 2}},
		{Off: 0x1003, Len: 5, Op: asm.OpJMP, Mnemonic: "JMP", Target: -1,
			Src: asm.Operand{Kind: asm.OKMem, Mem: asm.MemRef{Base: asm.RegBX, SegOverride: asm.RegCS, Disp: 0x1008, MemSize: 2}}},
		{Off: 0x1008, Len: 2, Op: asm.OpDW, Mnemonic: "DW", Target: 0x1010},
		{Off: 0x100a, Len: 2, Op: asm.OpDW, Mnemonic: "DW", Target: 0x1014},
		{Off: 0x100c, Len: 2, Op: asm.OpDW, Mnemonic: "DW", Target: 0x1010},
		jmpForGraphTest(0x1010, 0x1012),
		jmpForGraphTest(0x1012, 0x1015),
		retForGraphTest(0x1014),
		retForGraphTest(0x1015),
	}
	for _, collapse := range []bool{false, true} {
		ctx := ctxForGraphTest()
		ctx.fs.Name = "Dispatch"
		ctx.fs.Ret = &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"}
		cfg, err := BuildCFG(ctx, insts, false, CFGOptions{CollapseJumps: collapse})
		if err != nil {
			t.Fatal(err)
		}
		if cfg.Block(0x1008) != nil {
			t.Fatal("table data became an executable block")
		}
		if !slices.Equal(cfg.Jumps[0x1003].TableTargetOffs, []uint32{0x1010, 0x1014, 0x1010}) {
			t.Fatal("physical table entries were changed")
		}
		want := []BlockID{0x1010, 0x1014, 0x1010}
		if collapse {
			want = []BlockID{0x1015, 0x1014, 0x1015}
			if cfg.Block(0x1010) != nil || cfg.Block(0x1012) != nil {
				t.Fatal("table trampoline chain survived collapse")
			}
		}
		effects := Extract(ctx, cfg, ExtractOptions{})
		jump, ok := effects.Blocks[0].Effects[0].(TableJumpEffect)
		if !ok {
			t.Fatalf("effect = %T", effects.Blocks[0].Effects[0])
		}
		index, isConst := jump.Index.(*Const)
		if !slices.Equal(jump.Targets, want) || !isConst || index.Val != 2 {
			t.Fatalf("collapse=%v: jump = %#v", collapse, jump)
		}
		if len(cfg.Successors(0x1000)) != 2 {
			t.Fatal("CFG did not deduplicate table edges")
		}
		for _, target := range jump.Targets {
			if !slices.Contains(cfg.Successors(0x1000), target) {
				t.Fatalf("effect target %s missing from CFG", target)
			}
		}
	}
}

// TestCollapseTrampolineAfterReturn verifies non-fallthrough predecessors do
// not pin an otherwise removable trampoline.
func TestCollapseTrampolineAfterReturn(t *testing.T) {
	cfg := cfgForCollapsedGraphTest(t, []asm.DecodedInst{
		jccForGraphTest(0x1000, "JNZ", 0x1003),
		retForGraphTest(0x1002),
		jmpForGraphTest(0x1003, 0x1006),
		retForGraphTest(0x1005),
		retForGraphTest(0x1006),
	})
	if cfg.Block(0x1003) != nil {
		t.Fatal("trampoline after RET was retained")
	}
	if !slices.Equal(cfg.Successors(0x1000), []BlockID{0x1002, 0x1006}) {
		t.Fatalf("successors = %v", cfg.Successors(0x1000))
	}
}
