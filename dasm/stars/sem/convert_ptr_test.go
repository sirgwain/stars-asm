package sem

import (
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestConvertValueResolvesSymbolsInsideComplexExpression verifies symbol
// resolution is recursive and does not require a whole-expression pattern.
func TestConvertValueResolvesSymbolsInsideComplexExpression(t *testing.T) {
	fx := testfixture.Stars(t)
	fn := &typeinfo.Function{
		Name: "ComplexExpression",
		Addr: typeinfo.Addr{Seg: 1, Off: 0x100},
		Len:  0x100,
		Vars: []typeinfo.FunctionVar{
			{Name: "row", Type: typeinfo.I16, BPOffset: -0x40},
			{Name: "column", Type: typeinfo.I16, BPOffset: -0x3e},
			{Name: "depth", Type: typeinfo.I16, BPOffset: -0x3c},
		},
	}
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fn)
	ctx.currentInstOff = 0x110
	ds := machine.RegVal(asm.RegDS)
	globalLoad := func(offset int) machine.Value {
		return machine.LoadVal(machine.MemoryAddress{Seg: ds, Disp: offset, Width: 2})
	}
	localLoad := func(offset int) machine.Value {
		return machine.LoadVal(machine.MemoryAddress{Base: machine.FrameBaseVal(), Disp: offset, Width: 2})
	}
	add := func(lhs, rhs machine.Value) machine.Value {
		return machine.BinaryVal(machine.ValueOpAdd, lhs, rhs)
	}

	value := add(
		add(
			add(
				add(
					add(
						add(globalLoad(0x530a), machine.ConstVal(0x48)),
						machine.WordVal(machine.BinaryVal(machine.ValueOpMul, machine.ConstVal(0xc), globalLoad(0x23fa)), machine.WordLow),
					),
					machine.ConstVal(0x6),
				),
				localLoad(-0x40),
			),
			machine.ConstVal(0x19),
		),
		add(localLoad(-0x3e), localLoad(-0x3c)),
	)

	formatted := FormatExpr((&machineConverter{ctx: ctx}).convertValue(value))
	for _, symbol := range []string{"dyArial10", "dyArial8", "row", "column", "depth"} {
		if !strings.Contains(formatted, symbol) {
			t.Fatalf("expression %q does not contain resolved symbol %q", formatted, symbol)
		}
	}
	if strings.Contains(formatted, "load(") {
		t.Fatalf("expression retained raw storage: %q", formatted)
	}
}

func TestLowerMachineResolvesNearPointerMemory(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("AddBackTrailingSpaces")
	if fn == nil {
		t.Fatal("AddBackTrailingSpaces not found")
	}

	ppch := machine.LoadVal(machine.MemoryAddress{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x2832, Role: machine.OperandSrc},
	})
	ppchTarget := machine.MemoryAddress{
		Seg:    machine.RegVal(asm.RegDS),
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

func TestLowerMachineResolvesIndexedNearBytePointerArray(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("CostOfDevelopingItem")
	if fn == nil {
		t.Fatal("CostOfDevelopingItem not found")
	}

	i := machine.LoadVal(machine.MemoryAddress{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x16,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x6757, Role: machine.OperandSrc},
	})
	pTech := machine.LoadVal(machine.MemoryAddress{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x8,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x675a, Role: machine.OperandSrc},
	})
	source := machine.LoadVal(machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
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
	want := "branch 4 <= sext8to16(pTech[i]) ? L_687f : L_6796"
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

	i := machine.LoadVal(machine.MemoryAddress{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x5b41, Role: machine.OperandSrc},
	})
	pplr := machine.LoadVal(machine.MemoryAddress{
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
						Addr: machine.MemoryAddress{
							Seg:   machine.RegVal(asm.RegDS),
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
	want := "pplr->rgEnvVarMax[i] = -1"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

// TestLowerMachineResolvesConfiguredUnionArrayField verifies normalized
// pointer terms retain the configured all-elements union selection.
func TestLowerMachineResolvesConfiguredUnionArrayField(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("FTrackXfer")
	if fn == nil {
		t.Fatal("FTrackXfer not found")
	}
	ctx := NewFuncContext(fx.Image, fx.SDB, res, fn)
	ctx.SetCurrentBlock(0x5e4d)

	pxferGlobal := fx.SDB.GetGlobal("pxfer")
	if pxferGlobal == nil {
		t.Fatal("pxfer not found")
	}
	pxfer := machine.LoadVal(machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
		Disp:  int(pxferGlobal.Addr.Off),
		Width: 2,
	})
	var indexVar, iValVar *typeinfo.FunctionVar
	for i := range fn.Vars {
		switch fn.Vars[i].Name {
		case "i":
			indexVar = &fn.Vars[i]
		case "iVal":
			iValVar = &fn.Vars[i]
		}
	}
	if indexVar == nil || iValVar == nil {
		t.Fatal("FTrackXfer index locals not found")
	}
	index := frameLoad(ctx, 0, indexVar.BPOffset, 2)
	iVal := frameLoad(ctx, 0, iValVar.BPOffset, 2)
	base := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.BinaryVal(
			machine.ValueOpAdd,
			machine.BinaryVal(machine.ValueOpAdd, pxfer, machine.BinaryVal(machine.ValueOpMul, index, machine.ConstVal(0x80))),
			machine.ConstVal(0x10),
		),
		machine.BinaryVal(machine.ValueOpMul, iVal, machine.ConstVal(2)),
	)
	mem := machine.MemoryAddress{Seg: machine.RegVal(asm.RegDS), Base: base, Width: 2}
	value := &machine.PhiValue{
		Join: 0x5e4d,
		Arms: []machine.PhiArm{{
			Block: &machine.Block{ID: 0x5e34},
			Value: machine.LoadVal(mem),
		}},
	}

	merge, ok := (&machineConverter{ctx: ctx}).convertValue(value).(*Merge)
	if !ok || len(merge.Arms) != 1 {
		t.Fatalf("converted value = %#v, want one-arm merge", merge)
	}
	got := FormatExpr(merge.Arms[0].Value)
	if got != "pxfer[i].fl.rgcsh[iVal]" {
		t.Fatalf("resolved memory = %q, want configured fleet cash field", got)
	}
}

// TestConvertTypedAddressResolvesStringLiteral verifies a segmented address
// value is emitted directly from resolver literal data rather than as raw memory.
func TestConvertTypedAddressResolvesStringLiteral(t *testing.T) {
	fx := testfixture.Stars(t)
	fn := fx.SDB.GetFunction("InitMDIApp")
	if fn == nil {
		t.Fatal("InitMDIApp not found")
	}
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fn)
	value := machine.AddressVal(machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
		Disp:  0x364,
		Width: 4,
	})

	got := FormatExpr((&machineConverter{ctx: ctx}).convertValueTyped(value, typeinfo.LpStr))
	if got != `"StarsMenu"` {
		t.Fatalf("converted literal = %q, want StarsMenu string", got)
	}
}

// TestConvertMemoryPreservesInnerZeroArrayIndex verifies a dword access to
// the first element of a nested array retains the source-level zero index.
func TestConvertMemoryPreservesInnerZeroArrayIndex(t *testing.T) {
	fx := testfixture.Stars(t)
	fn := fx.SDB.GetFunction("PopRandom")
	if fn == nil {
		t.Fatal("PopRandom not found")
	}
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fn)
	cRandStack := fx.SDB.GetGlobal("cRandStack")
	rglRandStack := fx.SDB.GetGlobal("rglRandStack")
	if cRandStack == nil || rglRandStack == nil {
		t.Fatal("random stack globals not found")
	}
	index := machine.LoadVal(machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
		Disp:  int(cRandStack.Addr.Off),
		Width: 2,
	})
	mem := machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
		Base:  machine.BinaryVal(machine.ValueOpMul, index, machine.ConstVal(8)),
		Disp:  int(rglRandStack.Addr.Off),
		Width: 4,
	}

	got := FormatExpr((&machineConverter{ctx: ctx}).convertMemoryLValue(mem, 4))
	if got != "rglRandStack[cRandStack][0x0]" {
		addr, _ := ctx.symbols.addressFromMemory(mem, nil)
		t.Fatalf("converted memory = %q, want nested zero index; address=%#v", got, addr)
	}
}

func TestLowerMachineResolvesIndexedNearPointerParameter(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("PopupMenu")
	if fn == nil {
		t.Fatal("PopupMenu not found")
	}

	i := machine.LoadVal(machine.MemoryAddress{
		Base:   machine.FrameBaseVal(),
		Disp:   -0xc,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x165e, Role: machine.OperandSrc},
	})
	rgsz := machine.LoadVal(machine.MemoryAddress{
		Base:   machine.FrameBaseVal(),
		Disp:   0x10,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x1663, Role: machine.OperandSrc},
	})
	psz := machine.LoadVal(machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
		Base:  machine.BinaryVal(machine.ValueOpAdd, rgsz, machine.BinaryVal(machine.ValueOpMul, i, machine.ConstVal(2))),
		Width: 2,
	})
	ch := machine.LoadVal(machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
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
	want := "branch sext8to16(*rgsz[i]) != -1 ? L_16a9 : L_1675"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}
