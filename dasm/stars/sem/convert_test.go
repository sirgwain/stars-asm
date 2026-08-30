package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestLowerMachinePreservesStaleLoadAfterMemoryWrite(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("NthValidShdef")
	if fn == nil {
		t.Fatal("NthValidShdef not found")
	}

	nAtMov := machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x5c4f, Role: machine.OperandSrc},
	}
	nAtSub := machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x5c52, Role: machine.OperandDst},
	}
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x5c4f,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x5c4f, InstOff: 0x5c52},
						Addr:     nAtSub,
						Src:      machine.BinaryVal(machine.ValueOpSub, machine.LoadVal(nAtSub), machine.ConstVal(1)),
						Width:    2,
					},
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x5c4f, InstOff: 0x5c59},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JNZ", LHS: machine.LoadVal(nAtMov), RHS: machine.ConstVal(0)},
						TrueBlock:  0x5c81,
						FalseBlock: 0x5c5e,
					},
				},
			},
		},
	}

	semFunc, _, err := Lower(NewFuncContext(fx.Image, fx.SDB, res, fn), effects, nil)
	if err != nil {
		t.Fatalf("LowerMachine: %v", err)
	}
	if len(semFunc.Blocks) != 1 || len(semFunc.Blocks[0].Effects) != 3 {
		t.Fatalf("effects = %#v, want one block with three effects", semFunc.Blocks)
	}

	gotTemp := FormatEffect(semFunc.Blocks[0].Effects[0])
	wantTemp := "t_5c4f = n"
	if gotTemp != wantTemp {
		t.Fatalf("temp = %q, want %q", gotTemp, wantTemp)
	}
	gotStore := FormatEffect(semFunc.Blocks[0].Effects[1])
	wantStore := "n = (n - 0x1)"
	if gotStore != wantStore {
		t.Fatalf("store = %q, want %q", gotStore, wantStore)
	}
	gotBranch := FormatEffect(semFunc.Blocks[0].Effects[2])
	wantBranch := "branch t_5c4f != 0x0 ? L_5c81 : L_5c5e"
	if gotBranch != wantBranch {
		t.Fatalf("branch = %q, want %q", gotBranch, wantBranch)
	}
}

func TestLowerMachineResolvesByteValueInsideMerge(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("ChFromNybble")
	if fn == nil {
		t.Fatal("ChFromNybble not found")
	}

	nyb := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x49f7, Role: machine.OperandSrc},
	})
	global := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Disp:  0x13fe,
		Width: 2,
	})
	pred := &machine.Block{ID: 0x49fc}
	effects := &machine.FuncEffects{
		CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
			jmpForReturnSinkTest(0x49fc, 0x4a93),
			retForReturnSinkTest(0x4a93),
		}),
		Blocks: []machine.BlockEffects{
			{Block: 0x49fc},
			{
				Block: 0x4a93,
				Effects: []machine.Effect{
					machine.ReturnEffect{
						MetaInfo: machine.Meta{BlockID: 0x4a93, InstOff: 0x4a98},
						Value: &machine.PhiValue{
							Join: 0x4a93,
							Arms: []machine.PhiArm{
								{Block: pred, Value: machine.ByteWriteVal(nyb, machine.ByteLow, global)},
							},
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
		t.Fatalf("effects = %#v, want one sunk return effect", semFunc.Blocks)
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "return setlobyte(nyb, rgchcompstrlower)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

func TestLowerMachineResolvesConstBaseGlobalMemory(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("ClickInShipOrders")
	if fn == nil {
		t.Fatal("ClickInShipOrders not found")
	}

	global := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.ConstVal(0x497a),
		Width: 2,
	})
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x8999,
				Effects: []machine.Effect{
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x8999, InstOff: 0x89af},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JNZ", LHS: global, RHS: machine.ConstVal(0)},
						TrueBlock:  0x89c0,
						FalseBlock: 0x89b4,
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
	want := "branch sel.fl.pt.x != 0x0 ? L_89c0 : L_89b4"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

func TestLowerMachineResolvesIndexedGlobalStructByteArrayField(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("CalcPlayerScore")
	if fn == nil {
		t.Fatal("CalcPlayerScore not found")
	}

	iPlr := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x5a21, Role: machine.OperandSrc},
	})
	i := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x30,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x5a2e, Role: machine.OperandSrc},
	})
	playerBase := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.ConstVal(0x59a2),
		&machine.WordValue{
			Parent: machine.BinaryVal(machine.ValueOpMul, machine.ConstVal(0xc0), iPlr),
			Part:   machine.WordLow,
		},
	)
	rgTechByte := machine.LoadVal(machine.MemoryAccess{
		Seg: machine.ConstVal(fx.SDB.DGroupFrame),
		Base: machine.BinaryVal(
			machine.ValueOpAdd,
			machine.BinaryVal(machine.ValueOpAdd, playerBase, machine.ConstVal(0x1a)),
			i,
		),
		Width: 1,
	})
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x5a1e,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x5a1e, InstOff: 0x5a36},
						Addr: machine.MemoryAccess{
							Base:   machine.FrameBaseVal(),
							Disp:   -0x38,
							Width:  2,
							Origin: machine.Origin{InstOff: 0x5a36, Role: machine.OperandDst},
						},
						Src:   &machine.SignExtendValue{Parent: rgTechByte, FromBits: 8, ToBits: 16},
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
		t.Fatalf("effects = %#v, want one lowered store effect", semFunc.Blocks)
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "iTech = sext8to16(rgplr[iPlr].rgTech[i])"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

func TestLowerMachineResolvesBitfieldExtract(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("BringUpHostDlg")
	if fn == nil {
		t.Fatal("BringUpHostDlg not found")
	}
	gd := fx.SDB.GetGlobal("gd")
	if gd == nil {
		t.Fatal("gd not found")
	}

	flags := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.ConstVal(uint(gd.Addr.Off)),
		Disp:  2,
		Width: 2,
	})
	fProgressTxt := machine.BinaryVal(
		machine.ValueOpAnd,
		machine.BinaryVal(machine.ValueOpShr, flags, machine.ConstVal(10)),
		machine.ConstVal(1),
	)
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x6120,
				Effects: []machine.Effect{
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x6120, InstOff: 0x612e},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JZ", LHS: fProgressTxt, RHS: machine.ConstVal(0)},
						TrueBlock:  0x6138,
						FalseBlock: 0x6133,
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
	want := "branch gd.fProgressTxt == 0x0 ? L_6138 : L_6133"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

func TestLowerMachineCollapsesWideAggregateBitfieldRead(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("AddMinesToBlockedQueues")
	if fn == nil {
		t.Fatal("AddMinesToBlockedQueues not found")
	}

	lppl := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x16,
		Width:  4,
		Origin: machine.Origin{InstOff: 0x180b, Role: machine.OperandSrc},
	})
	lpplprodOff := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.FarPointerVal(lppl, machine.FarPointerSegment),
		Base:  machine.FarPointerVal(lppl, machine.FarPointerOffset),
		Disp:  0x34,
		Width: 2,
	})
	lpplprodSeg := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.FarPointerVal(lppl, machine.FarPointerSegment),
		Base:  machine.FarPointerVal(lppl, machine.FarPointerOffset),
		Disp:  0x36,
		Width: 2,
	})
	prodWordAt := func(instOff uint32, disp int) machine.MemoryAccess {
		return machine.MemoryAccess{
			Base:   machine.FrameBaseVal(),
			Disp:   -0x6 + disp,
			Width:  2,
			Origin: machine.Origin{InstOff: instOff, Role: machine.OperandSrc},
		}
	}
	prodWord := func(disp int) machine.MemoryAccess {
		return prodWordAt(0x180b, disp)
	}
	rgprodWord := func(disp int) machine.MemoryAccess {
		return machine.MemoryAccess{
			Seg:   lpplprodSeg,
			Base:  lpplprodOff,
			Disp:  0x4 + disp,
			Width: 2,
		}
	}
	prodWords := &machine.StackWords{Words: []machine.Value{
		machine.LoadVal(prodWordAt(0x1828, 0)),
		machine.LoadVal(prodWordAt(0x1828, 2)),
	}}
	grobj := machine.BinaryVal(
		machine.ValueOpAnd,
		machine.WordVal(
			machine.CastVal(
				machine.BinaryVal(machine.ValueOpShr, prodWords, machine.ConstVal(0x11)),
				typeinfo.U32,
			),
			machine.WordLow,
		),
		machine.ConstVal(0x7),
	)
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x17fc,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x17fc, InstOff: 0x180b},
						Addr:     prodWord(0),
						Src:      machine.LoadVal(rgprodWord(0)),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x17fc, InstOff: 0x180e},
						Addr:     prodWord(2),
						Src:      machine.LoadVal(rgprodWord(2)),
						Width:    2,
					},
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x17fc, InstOff: 0x1828},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JNZ", LHS: grobj, RHS: machine.ConstVal(1)},
						TrueBlock:  0x18c8,
						FalseBlock: 0x182d,
					},
				},
			},
		},
	}

	semFunc, _, err := Lower(NewFuncContext(fx.Image, fx.SDB, res, fn), effects, nil)
	if err != nil {
		t.Fatalf("LowerMachine: %v", err)
	}
	if len(semFunc.Blocks) != 1 || len(semFunc.Blocks[0].Effects) != 2 {
		t.Fatalf("effects = %q, want one block with two effects", formatEffectsForTest(semFunc.Blocks[0].Effects))
	}

	wants := []string{
		"prod = lppl->lpplprod->rgprod[0x0]",
		"branch prod.grobj != 0x1 ? L_18c8 : L_182d",
	}
	for i, want := range wants {
		if got := FormatEffect(semFunc.Blocks[0].Effects[i]); got != want {
			t.Fatalf("effect[%d] = %q, want %q", i, got, want)
		}
	}
}

func TestLowerMachineResolvesWideBitfieldStores(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("UninhabitPlanet")
	if fn == nil {
		t.Fatal("UninhabitPlanet not found")
	}

	lppl := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  4,
		Origin: machine.Origin{InstOff: 0x8832, Role: machine.OperandSrc},
	})
	pl := func(disp int) machine.MemoryAccess {
		return machine.MemoryAccess{
			Seg:   machine.FarPointerVal(lppl, machine.FarPointerSegment),
			Base:  machine.FarPointerVal(lppl, machine.FarPointerOffset),
			Disp:  disp,
			Width: 2,
		}
	}
	clear := func(mem machine.MemoryAccess, mask uint) machine.Value {
		return machine.BinaryVal(
			machine.ValueOpOr,
			machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(mem), machine.ConstVal(mask)),
			machine.ConstVal(0),
		)
	}
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x882f,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x882f, InstOff: 0x884e},
						Addr:     pl(0x1a),
						Src:      clear(pl(0x1a), 0xff7f),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x882f, InstOff: 0x8880},
						Addr:     pl(0x18),
						Src:      clear(pl(0x18), 0xf000),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x882f, InstOff: 0x88a2},
						Addr:     pl(0x18),
						Src:      machine.BinaryVal(machine.ValueOpOr, machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(pl(0x18)), machine.ConstVal(0x0fff)), machine.ConstVal(0xf000)),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x882f, InstOff: 0x88a6},
						Addr:     pl(0x1a),
						Src:      machine.BinaryVal(machine.ValueOpOr, machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(pl(0x1a)), machine.ConstVal(0xfffe)), machine.ConstVal(0x1)),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x882f, InstOff: 0x88ad},
						Addr:     pl(0x2c),
						Src:      machine.ConstVal(0),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x882f, InstOff: 0x88b3},
						Addr:     pl(0x2e),
						Src:      machine.ConstVal(0),
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
	if len(semFunc.Blocks) != 1 || len(semFunc.Blocks[0].Effects) != 4 {
		t.Fatalf("effects = %#v, want one block with four lowered store effects", semFunc.Blocks)
	}

	wants := []string{
		"lppl->fNoResearch = 0x0",
		"lppl->cDefenses = 0x0",
		"lppl->iScanner = 0x1f",
		"lppl->lStarbase = 0x0",
	}
	for i, want := range wants {
		if got := FormatEffect(semFunc.Blocks[0].Effects[i]); got != want {
			t.Fatalf("effect %d = %q, want %q", i, got, want)
		}
	}
}

func TestLowerMachineResolvesStorageInsideCast(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("ChangeScanSel")
	if fn == nil {
		t.Fatal("ChangeScanSel not found")
	}

	value := machine.CastVal(machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Disp:  0x4970,
		Width: 2,
	}), typeinfo.Double)
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x8d78,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x8d78, InstOff: 0x8d9c},
						Target:   fx.SDB.GetFunction("sqrt"),
						Args:     []machine.Value{value},
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
	want := "call sqrt((double)sel.scan.ith)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}
