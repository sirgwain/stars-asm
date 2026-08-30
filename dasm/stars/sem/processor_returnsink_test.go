package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestReturnSinkProcessorMovesMergeArmsToPredecessors verifies return merge sinks become predecessor returns.
func TestReturnSinkProcessorMovesMergeArmsToPredecessors(t *testing.T) {
	cfg := cfgForReturnSinkTest(t, []asm.DecodedInst{
		jccForReturnSinkTest(0x1000, 0x1004),
		jmpForReturnSinkTest(0x1002, 0x1006),
		jmpForReturnSinkTest(0x1004, 0x1006),
		retForReturnSinkTest(0x1006),
	})
	fn := &Func{
		CFG: cfg,
		Blocks: []Block{
			{
				ID:      0x1000,
				Effects: []Effect{&Branch{TrueBlock: 0x1004, FalseBlock: 0x1002}},
			},
			{
				ID:      0x1002,
				Effects: []Effect{&Jump{To: 0x1006}},
			},
			{
				ID: 0x1004,
			},
			{
				ID: 0x1006,
				Effects: []Effect{
					&Return{
						MetaInfo: machine.Meta{BlockID: 0x1006, InstOff: 0x1006},
						Value: &Merge{
							TypeInfo: typeinfo.U16,
							Arms: []MergeArm{
								{Block: 0x1002, Value: &Const{TypeInfo: typeinfo.U16, U64: 0x1}},
								{Block: 0x1004, Value: &Const{TypeInfo: typeinfo.U16, U64: 0x0}},
							},
						},
					},
				},
			},
		},
	}

	if changed := (&returnSinkProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := FormatEffect(fn.Blocks[1].Effects[len(fn.Blocks[1].Effects)-1]), "return 0x1"; got != want {
		t.Fatalf("first predecessor tail = %q, want %q", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[2].Effects[len(fn.Blocks[2].Effects)-1]), "return 0x0"; got != want {
		t.Fatalf("second predecessor tail = %q, want %q", got, want)
	}
	if len(fn.Blocks[3].Effects) != 0 {
		t.Fatalf("sink effects = %#v, want none", fn.Blocks[3].Effects)
	}
}

// cfgForReturnSinkTest builds a CFG for return sink processor tests.
func cfgForReturnSinkTest(t *testing.T, instrs []asm.DecodedInst) *machine.CFG {
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

// jccForReturnSinkTest builds a conditional jump instruction.
func jccForReturnSinkTest(off uint32, target uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 2, Op: asm.OpJcc, Mnemonic: "JZ", Target: int32(target)}
}

// jmpForReturnSinkTest builds an unconditional jump instruction.
func jmpForReturnSinkTest(off uint32, target uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: int32(target)}
}

// retForReturnSinkTest builds a return instruction.
func retForReturnSinkTest(off uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 1, Op: asm.OpRET, Mnemonic: "RET"}
}
