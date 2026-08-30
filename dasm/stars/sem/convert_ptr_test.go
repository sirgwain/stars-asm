package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

func TestLowerMachineResolvesNearPointerMemory(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("AddBackTrailingSpaces")
	if fn == nil {
		t.Fatal("AddBackTrailingSpaces not found")
	}

	ppch := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x2832, Role: machine.OperandSrc},
	})
	ppchTarget := machine.MemoryAccess{
		Seg:    machine.ConstVal(fx.SDB.DGroupFrame),
		Base:   ppch,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x2835, Role: machine.OperandDst},
	}
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x2832,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x2832, InstOff: 0x2835},
						Addr:     ppchTarget,
						Src:      machine.BinaryVal(machine.ValueOpAdd, machine.LoadVal(ppchTarget), machine.ConstVal(1)),
						Width:    2,
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
		t.Fatalf("effects = %#v, want one lowered store effect", semFunc.Blocks)
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "*ppch = (*ppch + 0x1)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

func TestLowerMachineResolvesIndexedLocalPointerArray(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("FleetTransferCargoBalance")
	if fn == nil {
		t.Fatal("FleetTransferCargoBalance not found")
	}

	i := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x90,
		Width:  2,
		Origin: machine.Origin{InstOff: 0xaea2, Role: machine.OperandSrc},
	})
	pointer := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x8a,
		Width:  2,
		Index:  machine.BinaryVal(machine.ValueOpShl, i, machine.ConstVal(1)),
		Origin: machine.Origin{InstOff: 0xaeae, Role: machine.OperandSrc},
	})
	field := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  pointer,
		Disp:  4,
		Width: 2,
	})
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0xaea2,
				Effects: []machine.Effect{
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0xaea2, InstOff: 0xaebe},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JZ", LHS: field, RHS: machine.ConstVal(0)},
						TrueBlock:  0xaefc,
						FalseBlock: 0xaec3,
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
		t.Fatalf("effects = %#v, want one lowered branch effect", semFunc.Blocks)
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "branch *(rgpflNew[i]+0x4) == 0x0 ? L_aefc : L_aec3"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

func TestLowerMachineResolvesIndexedNearBytePointerArray(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("CostOfDevelopingItem")
	if fn == nil {
		t.Fatal("CostOfDevelopingItem not found")
	}

	i := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x16,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x6757, Role: machine.OperandSrc},
	})
	pTech := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x8,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x675a, Role: machine.OperandSrc},
	})
	source := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.BinaryVal(machine.ValueOpAdd, pTech, i),
		Width: 1,
	})
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x6757,
				Effects: []machine.Effect{
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x6757, InstOff: 0x6791},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JLE", LHS: machine.ConstVal(4), RHS: &machine.SignExtendValue{Parent: source, FromBits: 8, ToBits: 16}},
						TrueBlock:  0x687f,
						FalseBlock: 0x6796,
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
		t.Fatalf("effects = %#v, want one lowered branch effect", semFunc.Blocks)
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "branch 0x4 <= sext8to16(pTech[i]) ? L_687f : L_6796"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

func TestLowerMachineResolvesIndexedNearPointerByteArrayFields(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("CreateRandomRace")
	if fn == nil {
		t.Fatal("CreateRandomRace not found")
	}

	i := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x5b41, Role: machine.OperandSrc},
	})
	pplr := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x5b3c, Role: machine.OperandSrc},
	})
	envMax := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.BinaryVal(machine.ValueOpAdd, pplr, machine.ConstVal(0x16)),
		i,
	)
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x5b39,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x5b39, InstOff: 0x5b46},
						Addr: machine.MemoryAccess{
							Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
							Base:  envMax,
							Width: 1,
						},
						Src:   machine.ConstVal(0xff),
						Width: 1,
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
		t.Fatalf("effects = %#v, want one lowered store effect", semFunc.Blocks)
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "pplr->rgEnvVarMax[i] = 0xff"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

func TestLowerMachineResolvesIndexedNearPointerParameter(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("PopupMenu")
	if fn == nil {
		t.Fatal("PopupMenu not found")
	}

	i := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0xc,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x165e, Role: machine.OperandSrc},
	})
	rgsz := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   0x10,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x1663, Role: machine.OperandSrc},
	})
	psz := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.BinaryVal(machine.ValueOpAdd, rgsz, machine.BinaryVal(machine.ValueOpMul, i, machine.ConstVal(2))),
		Width: 2,
	})
	ch := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  psz,
		Width: 1,
	})
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x165e,
				Effects: []machine.Effect{
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x165e, InstOff: 0x1670},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JNZ", LHS: machine.SignExtendVal(ch, 8, 16), RHS: machine.ConstVal(0xffff)},
						TrueBlock:  0x16a9,
						FalseBlock: 0x1675,
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
		t.Fatalf("effects = %#v, want one lowered branch effect", semFunc.Blocks)
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "branch sext8to16(*rgsz[i]) != 0xffff ? L_16a9 : L_1675"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}
