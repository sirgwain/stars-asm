package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
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

	i := machine.LoadVal(machine.MemoryAddress{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x12,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x100b, Role: machine.OperandSrc},
	})
	ptile := machine.LoadVal(machine.MemoryAddress{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x1a,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x1016, Role: machine.OperandSrc},
	})
	target := machine.MemoryAddress{
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

// TestConvertTypedIndexedGlobalAddresses preserves all constants in a
// multi-term address before projecting it through the PLAYER array.
func TestConvertTypedIndexedGlobalAddresses(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "CMaxMines")
	iplr := frameLoad(ctx, ctx.fs.Addr.Off, 0xa, 2)
	index := machine.WordVal(
		machine.BinaryVal(machine.ValueOpMul, machine.ConstVal(0xc0), iplr),
		machine.WordLow,
	)
	address := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.BinaryVal(
			machine.ValueOpAdd,
			machine.ConstVal(0x59a2),
			index,
		),
		machine.ConstVal(0x80),
	)
	converter := &machineConverter{ctx: ctx}
	cch := fx.SDB.GetFunction("CchGetString")
	if got := FormatExpr(converter.convertValueTyped(address, cch.Params[1].Type)); got != "rgplr[iplr].szName" {
		t.Fatalf("near indexed global address = %q, want rgplr[iplr].szName", got)
	}

	wsprintf := fx.SDB.GetFunction("_wsprintf")
	destination := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.BinaryVal(machine.ValueOpAdd, machine.ConstVal(0x59a2), index),
		machine.ConstVal(0xa0),
	)
	farAddress := &machine.StackWords{Words: []machine.Value{
		machine.RegVal(asm.RegDS),
		destination,
	}}
	if got := FormatExpr(converter.convertValueTyped(farAddress, wsprintf.Params[0].Type)); got != "rgplr[iplr].szNames" {
		t.Fatalf("far indexed global address = %q, want rgplr[iplr].szNames", got)
	}

	if _, ok := wsprintf.Params[0].Type.(*typeinfo.Pointer); !ok {
		t.Fatal("_wsprintf destination is not a pointer type")
	}
}
