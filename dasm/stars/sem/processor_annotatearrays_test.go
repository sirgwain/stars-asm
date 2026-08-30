package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestAnnotateArraysKeepsMachineExpressionShallow verifies array lowering keeps
// semantic expressions rich without annotating the whole machine expression.
func TestAnnotateArraysKeepsMachineExpressionShallow(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("CalcPlanetMaxPop")
	if fn == nil {
		t.Fatal("CalcPlanetMaxPop not found")
	}
	target := fx.SDB.GetFunction("GetRaceStat")
	if target == nil {
		t.Fatal("GetRaceStat not found")
	}

	iplr := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   8,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x70be, Role: machine.OperandSrc},
	})
	rgplr := machine.ConstVal(0x59a2).WithOrigin(&machine.Origin{InstOff: 0x70b9, Role: machine.OperandSrc})
	arg := machine.BinaryVal(
		machine.ValueOpAdd,
		rgplr,
		machine.WordVal(
			machine.BinaryVal(machine.ValueOpMul, machine.ConstVal(0xc0), iplr),
			machine.WordLow,
		),
	)
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x7096,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x7096, InstOff: 0x70be},
						Target:   target,
						Args:     []machine.Value{arg, machine.ConstVal(0xe)},
						Result:   &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x70be},
					},
				},
			},
		},
	}

	semFunc, result, err := Lower(NewFuncContext(fx.Image, fx.SDB, res, fn), effects, nil)
	if err != nil {
		t.Fatalf("LowerMachine: %v", err)
	}
	if len(semFunc.Blocks) != 1 || len(semFunc.Blocks[0].Effects) != 1 {
		t.Fatalf("effects = %#v, want one lowered call effect", semFunc.Blocks)
	}

	gotSem := FormatEffect(semFunc.Blocks[0].Effects[0])
	wantSem := "call GetRaceStat(rgplr[iplr], rsMajorAdv) -> callresult(RaceAttribute)"
	if gotSem != wantSem {
		t.Fatalf("semantic effect = %q, want %q", gotSem, wantSem)
	}

	gotMachine := result.Value(arg)
	wantMachine := "(0x59a2 + loword((0xc0 * load([bp+iplr]))))"
	if gotMachine != wantMachine {
		t.Fatalf("machine annotation = %q, want %q", gotMachine, wantMachine)
	}
}
