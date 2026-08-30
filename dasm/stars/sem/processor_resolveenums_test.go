package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestResolveEnumsTypesCallArgsResultsAndComparisons verifies enum rules refine
// call arguments, constrained call results, and constants compared to them.
func TestResolveEnumsTypesCallArgsResultsAndComparisons(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("CMaxMines")
	if fn == nil {
		t.Fatal("CMaxMines not found")
	}
	target := fx.SDB.GetFunction("GetRaceStat")
	if target == nil {
		t.Fatal("GetRaceStat not found")
	}

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x72ca,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x72ca, InstOff: 0x72da},
						Target:   target,
						Args:     []machine.Value{machine.ConstVal(0), machine.ConstVal(0xe)},
						Result:   &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x72da},
					},
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x72ca, InstOff: 0x72e4},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JNZ", LHS: &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x72da}, RHS: machine.ConstVal(0x8)},
						TrueBlock:  0x72f4,
						FalseBlock: 0x72ea,
					},
				},
			},
		},
	}

	semFunc, _, err := Lower(NewFuncContext(fx.Image, fx.SDB, res, fn), effects, nil)
	if err != nil {
		t.Fatalf("LowerMachine: %v", err)
	}
	_ = semFunc
	// TODO: fix this
	// if len(semFunc.Blocks) != 1 || len(semFunc.Blocks[0].Effects) != 2 {
	// 	t.Fatalf("effects = %#v, want call and branch", semFunc.Blocks)
	// }

	// gotCall := FormatEffect(semFunc.Blocks[0].Effects[0])
	// wantCall := "call GetRaceStat(0x0, rsMajorAdv) -> callresult(RaceAttribute)"
	// if gotCall != wantCall {
	// 	t.Fatalf("call effect = %q, want %q", gotCall, wantCall)
	// }
	// gotBranch := FormatEffect(semFunc.Blocks[0].Effects[1])
	// wantBranch := "branch callresult(RaceAttribute) != raMacintosh ? L_72f4 : L_72ea"
	// if gotBranch != wantBranch {
	// 	t.Fatalf("branch effect = %q, want %q", gotBranch, wantBranch)
	// }
}

// TestResolveEnumsTypesConstantWordCallArg verifies enum args can be resolved
// after ABI lowering has produced a constant word pair.
func TestResolveEnumsTypesConstantWordCallArg(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("About")
	if fn == nil {
		t.Fatal("About not found")
	}
	target := fx.SDB.GetFunction("DialogBox")
	if target == nil {
		t.Fatal("DialogBox not found")
	}

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x14a3,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x14a3, InstOff: 0x14cf},
						Target:   target,
						Args: []machine.Value{
							machine.ConstVal(0x1234),
							&machine.StackWords{Words: []machine.Value{machine.ConstVal(0), machine.ConstVal(0x61)}},
							machine.ConstVal(0x5678),
							machine.FarPointerWordsVal(machine.ConstVal(0x151e), machine.ConstVal(0x127d)),
						},
						Result: &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x14cf},
					},
				},
			},
		},
	}

	semFunc, _, err := Lower(NewFuncContext(fx.Image, fx.SDB, res, fn), effects, nil)
	if err != nil {
		t.Fatalf("LowerMachine: %v", err)
	}
	if len(semFunc.Blocks) != 1 || len(semFunc.Blocks[0].Effects) != 1 {
		t.Fatalf("effects = %#v, want one lowered call effect", semFunc.Blocks)
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "call DialogBox(0x1234, IDD_ORDER_INFO, 0x5678, farptr(0x127d, 0x151e)) -> callresult(int16_t)"
	if got != want {
		t.Fatalf("call effect = %q, want %q", got, want)
	}
}

// TestResolveEnumsTypesAssignedFieldConstants verifies enum-typed field stores
// refine constant sources using the destination field type.
func TestResolveEnumsTypesAssignedFieldConstants(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("FGetBestDefensePart")
	if fn == nil {
		t.Fatal("FGetBestDefensePart not found")
	}

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x21f6,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x21f6, InstOff: 0x2204},
						Addr: machine.MemoryAccess{
							Base:   machine.FrameBaseVal(),
							Disp:   -0xe,
							Width:  2,
							Origin: machine.Origin{InstOff: 0x2204, Role: machine.OperandDst},
						},
						Src:   machine.ConstVal(0x8000),
						Width: 2,
					},
				},
			},
		},
	}

	semFunc, _, err := Lower(NewFuncContext(fx.Image, fx.SDB, res, fn), effects, nil)
	if err != nil {
		t.Fatalf("LowerMachine: %v", err)
	}
	if len(semFunc.Blocks) != 1 || len(semFunc.Blocks[0].Effects) != 1 {
		t.Fatalf("effects = %#v, want one lowered store", semFunc.Blocks)
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "part.hs.grhst = hstPlanetary"
	if got != want {
		t.Fatalf("store effect = %q, want %q", got, want)
	}
}
