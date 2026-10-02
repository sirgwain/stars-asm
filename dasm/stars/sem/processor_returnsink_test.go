package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestReturnTempsRequiresCompleteDefinitions verifies direct returns preserve
// exact values and reject missing edge definitions or narrowing conversions.
func TestReturnTempsRequiresCompleteDefinitions(t *testing.T) {
	for _, mode := range []string{"complete", "missing edge", "narrowing"} {
		t.Run(mode, func(t *testing.T) {
			cfg := cfgForReturnSinkTest(t, []asm.DecodedInst{
				jccForReturnSinkTest(0x1000, 0x1004),
				jmpForReturnSinkTest(0x1002, 0x1006),
				jmpForReturnSinkTest(0x1004, 0x1006),
				retForReturnSinkTest(0x1006),
			})
			temp := &Temp{Name: "t_merge", TypeInfo: typeinfo.I16}
			value := testLocal("result", typeinfo.I16)
			flag := testLocal("flag", typeinfo.I16)
			f := &Func{CFG: cfg, Blocks: []Block{
				{ID: 0x1000, Effects: []Effect{&Branch{Cond: &Compare{Op: CompareNE, LHS: flag, RHS: &Const{TypeInfo: typeinfo.I16}}, TrueBlock: 0x1004, FalseBlock: 0x1002}}},
				{ID: 0x1002, Effects: []Effect{&Assign{Dst: temp, Src: &Const{TypeInfo: typeinfo.I16}}, &Jump{To: 0x1006}}},
				{ID: 0x1004, Effects: []Effect{&Assign{Dst: temp, Src: value}, &Jump{To: 0x1006}}},
				{ID: 0x1006, Effects: []Effect{&Return{Value: temp}}},
			}}
			if mode == "missing edge" {
				f.Blocks[2].Effects = f.Blocks[2].Effects[1:]
			}
			if mode == "narrowing" {
				f.Blocks[2].Effects[0].(*Assign).Src = &Const{TypeInfo: typeinfo.U32, U64: 65535}
			}
			p := returnTempsProcessor{fs: &typeinfo.Function{Name: "Check", Ret: typeinfo.I16}}
			if got := p.ProcessFunc(nil, f); got != (mode == "complete") {
				t.Fatalf("recovered = %v for %s", got, mode)
			}
			if mode == "complete" && !sameExpr(f.Blocks[2].Effects[0].(*Return).Value, value) {
				t.Fatal("return result normalized instead of preserved")
			}
		})
	}
}

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
	if got, want := FormatEffect(fn.Blocks[1].Effects[len(fn.Blocks[1].Effects)-1]), "return 1"; got != want {
		t.Fatalf("first predecessor tail = %q, want %q", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[2].Effects[len(fn.Blocks[2].Effects)-1]), "return 0"; got != want {
		t.Fatalf("second predecessor tail = %q, want %q", got, want)
	}
	if len(fn.Blocks[3].Effects) != 0 {
		t.Fatalf("sink effects = %#v, want none", fn.Blocks[3].Effects)
	}
}

// TestReturnSinkProcessorFoldsImmediatelyReturnedCall verifies a call result
// used only by the following return becomes the return expression itself.
func TestReturnSinkProcessorFoldsImmediatelyReturnedCall(t *testing.T) {
	callee := &typeinfo.Function{Name: "MessageBox", Ret: typeinfo.I16}
	result := &CallResult{Function: callee, TypeInfo: typeinfo.I16, InstOff: 0x220f}
	fn := &Func{Blocks: []Block{{
		ID: 0x21f7,
		Effects: []Effect{
			&CallEffect{
				Call:   &Call{Function: callee},
				Result: result,
			},
			&Return{Value: &CallResult{Function: callee, TypeInfo: typeinfo.I16, InstOff: 0x220f}},
		},
	}}}

	if changed := (&returnSinkProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if len(fn.Blocks[0].Effects) != 1 {
		t.Fatalf("effects = %#v, want one return", fn.Blocks[0].Effects)
	}
	if got, want := FormatEffect(fn.Blocks[0].Effects[0]), "return MessageBox()"; got != want {
		t.Fatalf("folded return = %q, want %q", got, want)
	}
}

// TestReturnSinkProcessorFoldsCallAfterMergeSink verifies a call result moved
// from a shared merge return is folded after it reaches its predecessor block.
func TestReturnSinkProcessorFoldsCallAfterMergeSink(t *testing.T) {
	cfg := cfgForReturnSinkTest(t, []asm.DecodedInst{
		jccForReturnSinkTest(0x1000, 0x1004),
		jmpForReturnSinkTest(0x1002, 0x1006),
		jmpForReturnSinkTest(0x1004, 0x1006),
		retForReturnSinkTest(0x1006),
	})
	callee := &typeinfo.Function{Name: "MessageBox", Ret: typeinfo.I16}
	callResult := &CallResult{Function: callee, TypeInfo: typeinfo.I16, InstOff: 0x1002}
	fn := &Func{
		CFG: cfg,
		Blocks: []Block{
			{ID: 0x1000, Effects: []Effect{&Branch{TrueBlock: 0x1004, FalseBlock: 0x1002}}},
			{ID: 0x1002, Effects: []Effect{
				&CallEffect{Call: &Call{Function: callee}, Result: callResult},
				&Jump{To: 0x1006},
			}},
			{ID: 0x1004},
			{ID: 0x1006, Effects: []Effect{&Return{Value: &Merge{
				TypeInfo: typeinfo.I16,
				Arms: []MergeArm{
					{Block: 0x1002, Value: &CallResult{Function: callee, TypeInfo: typeinfo.I16, InstOff: 0x1002}},
					{Block: 0x1004, Value: &Const{TypeInfo: typeinfo.I16, U64: 0}},
				},
			}}}},
		},
	}

	if changed := (&returnSinkProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := FormatEffect(fn.Blocks[1].Effects[len(fn.Blocks[1].Effects)-1]), "return MessageBox()"; got != want {
		t.Fatalf("call predecessor tail = %q, want %q", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[2].Effects[len(fn.Blocks[2].Effects)-1]), "return 0"; got != want {
		t.Fatalf("constant predecessor tail = %q, want %q", got, want)
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
