package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestLowerMachineResolvesIndexedStructFunctionPointerCall verifies indirect
// CALLF targets resolve through indexed struct fields.
func TestLowerMachineResolvesIndexedStructFunctionPointerCall(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("DrawPlanShip")
	if fn == nil {
		t.Fatal("DrawPlanShip not found")
	}

	i := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x12,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x100b, Role: machine.OperandSrc},
	})
	ptile := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x1a,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x1016, Role: machine.OperandSrc},
	})
	target := machine.MemoryAccess{
		Seg:    machine.RegVal(asm.RegDS),
		Base:   machine.BinaryVal(machine.ValueOpAdd, ptile, machine.BinaryVal(machine.ValueOpShl, machine.BinaryVal(machine.ValueOpShl, machine.BinaryVal(machine.ValueOpShl, machine.BinaryVal(machine.ValueOpShl, i, machine.ConstVal(1)), machine.ConstVal(1)), machine.ConstVal(1)), machine.ConstVal(1))),
		Disp:   0x6,
		Width:  4,
		Origin: machine.Origin{InstOff: 0x102f, Role: machine.OperandDst},
	}
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x100a,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo:     machine.Meta{BlockID: 0x100a, InstOff: 0x102f},
						MemoryAccess: target,
						Args: []machine.Value{
							machine.ConstVal(0x1111),
							machine.ConstVal(0x2222),
							machine.ConstVal(0x3333),
						},
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
	want := "call ptile[i].pfn(0x3333, 0x2222, 0x1111)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}
