package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestBranchCallResultMaterializesWideMultiUseResult verifies wide compare
// chains reuse a temp instead of anonymous call-result placeholders.
func TestBranchCallResultMaterializesWideMultiUseResult(t *testing.T) {
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	target := &typeinfo.Function{Name: "ChgPopFromPlanet", Ret: int32Type}
	result := &CallResult{Function: target, TypeInfo: int32Type, InstOff: 0x0ea9}
	zero := &Const{TypeInfo: typeinfo.U16, U64: 0}
	fn := &Func{Blocks: []Block{
		{
			ID: 0x0e9f,
			Effects: []Effect{
				&CallEffect{
					MetaInfo: machine.Meta{InstOff: 0x0ea9},
					Call:     &Call{Function: target},
					Result:   result,
				},
				&Branch{
					Cond:       &Compare{Op: CompareLT, LHS: &Word{Parent: result, Part: machine.WordHigh}, RHS: zero},
					TrueBlock:  0x0f9f,
					FalseBlock: 0x0eb9,
				},
			},
		},
		{
			ID: 0x0eb9,
			Effects: []Effect{
				&Branch{
					Cond:       &Compare{Op: CompareGT, LHS: &Word{Parent: result, Part: machine.WordHigh}, RHS: zero},
					TrueBlock:  0x0ec6,
					FalseBlock: 0x0ebe,
				},
			},
		},
		{
			ID: 0x0ebe,
			Effects: []Effect{
				&Branch{
					Cond:       &Compare{Op: CompareLT, LHS: &Word{Parent: result, Part: machine.WordLow}, RHS: zero},
					TrueBlock:  0x0f9f,
					FalseBlock: 0x0ec6,
				},
			},
		},
	}}

	changed := (&callResultProcessor{}).ProcessFunc(nil, fn)
	if !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}

	gotCall := FormatEffect(fn.Blocks[0].Effects[0])
	wantCall := "call ChgPopFromPlanet() -> t_call_0ea9"
	if gotCall != wantCall {
		t.Fatalf("call effect = %q, want %q", gotCall, wantCall)
	}

	gotBranch := FormatEffect(fn.Blocks[2].Effects[0])
	wantBranch := "branch loword(t_call_0ea9) < 0x0 ? L_0f9f : L_0ec6"
	if gotBranch != wantBranch {
		t.Fatalf("branch effect = %q, want %q", gotBranch, wantBranch)
	}
}

// TestBranchCallResultMaterializesSingleSurvivingUse verifies a call result
// already present in semantic IR is made explicit even when it has one use.
func TestBranchCallResultMaterializesSingleSurvivingUse(t *testing.T) {
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	target := &typeinfo.Function{Name: "Calc", Ret: int32Type}
	result := &CallResult{Function: target, TypeInfo: int32Type, InstOff: 0x1000}
	fn := &Func{Blocks: []Block{
		{
			ID: 0x1000,
			Effects: []Effect{
				&CallEffect{Call: &Call{Function: target}, Result: result},
				&Branch{
					Cond:       &Compare{Op: CompareLT, LHS: &Word{Parent: result, Part: machine.WordHigh}, RHS: &Const{TypeInfo: typeinfo.U16}},
					TrueBlock:  0x1010,
					FalseBlock: 0x1005,
				},
			},
		},
	}}

	changed := (&callResultProcessor{}).ProcessFunc(nil, fn)
	if !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := FormatEffect(fn.Blocks[0].Effects[0]), "call Calc() -> t_call_1000"; got != want {
		t.Fatalf("call effect = %q, want %q", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[0].Effects[1]), "branch hiword(t_call_1000) < 0x0 ? L_1010 : L_1005"; got != want {
		t.Fatalf("branch effect = %q, want %q", got, want)
	}
}
