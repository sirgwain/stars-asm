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

	changed := (&branchCallResultProcessor{}).ProcessFunc(nil, fn)
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

// TestBranchCallResultLeavesSingleUseResult verifies single-use call results
// remain available for the ordinary call-result collapse pass.
func TestBranchCallResultLeavesSingleUseResult(t *testing.T) {
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

	changed := (&branchCallResultProcessor{}).ProcessFunc(nil, fn)
	if changed {
		t.Fatal("ProcessFunc changed = true, want false")
	}
}
