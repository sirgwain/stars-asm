package sem

import (
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestFromMachineResolvesFarPointerMemory verifies far-pointer memory is lowered
// from the source pointer without collapsing the machine effect itself.
func TestFromMachineResolvesFarPointerMemory(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("ChgPopFromPlanet")
	if fn == nil {
		t.Fatal("ChgPopFromPlanet not found")
	}
	target := fx.SDB.GetFunction("CalcPlanetMaxPop")
	if target == nil {
		t.Fatal("CalcPlanetMaxPop not found")
	}

	lppl := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  4,
		Origin: machine.Origin{InstOff: 0x7082, Role: machine.OperandSrc},
	})
	idpl := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.FarPointerVal(lppl, machine.FarPointerSegment),
		Base:  machine.FarPointerVal(lppl, machine.FarPointerOffset),
		Width: 2,
	})
	iplr := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   10,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x7084, Role: machine.OperandSrc},
	})
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x7082,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x7082, InstOff: 0x7082},
						Target:   target,
						Args:     []machine.Value{idpl, iplr},
						Result:   &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x7082},
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
	wantSem := "call CalcPlanetMaxPop(lppl->id, fUpdate) -> callresult(int32_t)"
	if gotSem != wantSem {
		t.Fatalf("semantic effect = %q, want %q", gotSem, wantSem)
	}

	gotMachine := result.Value(idpl)
	wantMachine := "load(farseg(load(dword [bp+lppl])):[faroff(load(dword [bp+lppl]))])"
	if gotMachine != wantMachine {
		t.Fatalf("machine annotation = %q, want %q", gotMachine, wantMachine)
	}
}

// TestFromMachineKeepsFarPointerResidualConstants verifies constants in
// recovered far-pointer offsets stay byte displacements, not globals.
func TestFromMachineKeepsFarPointerResidualConstants(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("LComputePower")
	if fn == nil {
		t.Fatal("LComputePower not found")
	}

	lpshdef := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  4,
		Origin: machine.Origin{InstOff: 0x0b89, Role: machine.OperandSrc},
	})
	ihs := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -8,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x0b91, Role: machine.OperandSrc},
	})
	slotBytes := machine.BinaryVal(
		machine.ValueOpShl,
		machine.BinaryVal(machine.ValueOpShl, ihs, machine.ConstVal(1)),
		machine.ConstVal(1),
	)
	rghsOffset := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.BinaryVal(machine.ValueOpAdd, machine.FarPointerVal(lpshdef, machine.FarPointerOffset), machine.ConstVal(0x3a)),
		slotBytes,
	)
	hiword := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.FarPointerVal(lpshdef, machine.FarPointerSegment),
		Base:  rghsOffset,
		Disp:  2,
		Width: 2,
	})

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x0b86,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x0b86, InstOff: 0x0ba6},
						Addr: machine.MemoryAccess{
							Base:   machine.FrameBaseVal(),
							Disp:   -0x24,
							Width:  2,
							Origin: machine.Origin{InstOff: 0x0ba6, Role: machine.OperandDst},
						},
						Src:   hiword,
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
	want := "HIWORD(part.hs) = HIWORD(lpshdef->hul.rghs[ihs])"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

// formatEffectsForTest renders effects for assertion failures.
func formatEffectsForTest(effects []Effect) string {
	lines := make([]string, len(effects))
	for i, effect := range effects {
		lines[i] = FormatEffect(effect)
	}
	return strings.Join(lines, "\n")
}

// TestLowerMachineCollapsesSplitFarPointerLocalStores verifies far-pointer
// local word parts remain offset/segment storage, not pointee fields.
func TestLowerMachineCollapsesSplitFarPointerLocalStores(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("DoAutomitronAiTurn")
	if fn == nil {
		t.Fatal("DoAutomitronAiTurn not found")
	}

	lpfl := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x74,
		Width:  4,
		Origin: machine.Origin{InstOff: 0xdb4, Role: machine.OperandSrc},
	})
	lpflWord := func(disp int) machine.MemoryAccess {
		return machine.MemoryAccess{
			Base:   machine.FrameBaseVal(),
			Disp:   -0x74 + disp,
			Width:  2,
			Origin: machine.Origin{InstOff: 0x0db4, Role: machine.OperandSrc},
		}
	}
	lpflEnemyWord := func(disp int) machine.MemoryAccess {
		return machine.MemoryAccess{
			Base:   machine.FrameBaseVal(),
			Disp:   -0x32 + disp,
			Width:  2,
			Origin: machine.Origin{InstOff: 0x0db4, Role: machine.OperandSrc},
		}
	}
	lpflFieldWord := func(disp int) machine.MemoryAccess {
		return machine.MemoryAccess{
			Seg:   machine.FarPointerVal(lpfl, machine.FarPointerSegment),
			Base:  machine.FarPointerVal(lpfl, machine.FarPointerOffset),
			Disp:  disp,
			Width: 2,
		}
	}
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x0dab,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x0dab, InstOff: 0x0db4},
						Addr:     lpflFieldWord(0x68),
						Src:      machine.LoadVal(lpflEnemyWord(0)),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x0dab, InstOff: 0x0db8},
						Addr:     lpflFieldWord(0x6a),
						Src:      machine.LoadVal(lpflEnemyWord(2)),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x0dab, InstOff: 0x0dc2},
						Addr:     lpflEnemyWord(0),
						Src:      machine.LoadVal(lpflWord(0)),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x0dab, InstOff: 0x0dc5},
						Addr:     lpflEnemyWord(2),
						Src:      machine.LoadVal(lpflWord(2)),
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
	if len(semFunc.Blocks) != 1 || len(semFunc.Blocks[0].Effects) != 2 {
		t.Fatalf("effects = %q, want one block with two collapsed far-pointer stores", formatEffectsForTest(semFunc.Blocks[0].Effects))
	}

	wants := []string{
		"lpfl->lpflNext = lpflEnemy",
		"lpflEnemy = lpfl",
	}
	for i, want := range wants {
		if got := FormatEffect(semFunc.Blocks[0].Effects[i]); got != want {
			t.Fatalf("effect[%d] = %q, want %q", i, got, want)
		}
	}
}

// TestLowerMachineResolvesDirectFarPointerCallArgs verifies direct constant far
// pointers resolve to globals before string literals.
func TestLowerMachineResolvesDirectFarPointerCallArgs(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("SzVersion")
	if fn == nil {
		t.Fatal("SzVersion not found")
	}
	target := fx.SDB.GetFunction("_wsprintf")
	if target == nil {
		t.Fatal("_wsprintf not found")
	}

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x1212,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x1212, InstOff: 0x1212},
						Target:   target,
						Args: []machine.Value{
							machine.FarPointerWordsVal(machine.ConstVal(0x57a4), machine.ConstVal(0x25)),
							machine.FarPointerWordsVal(machine.ConstVal(0x9f0), machine.ConstVal(0x25)),
							machine.ConstVal(2),
						},
						Result: &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x1212},
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
	want := "call _wsprintf(szWork, \"backup.%03d\", 0x2) -> callresult(int16_t)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

// TestLowerMachineResolvesDirectFarPointerGlobalFieldCallArg verifies constant
// far pointers into aggregate globals lower to address-of field expressions.
func TestLowerMachineResolvesDirectFarPointerGlobalFieldCallArg(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("AddMinesToBlockedQueues")
	if fn == nil {
		t.Fatal("AddMinesToBlockedQueues not found")
	}
	target := fx.SDB.GetFunction("PszProductionETA")
	if target == nil {
		t.Fatal("PszProductionETA not found")
	}

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x18c8,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x18c8, InstOff: 0x18f5},
						Target:   target,
						Args: []machine.Value{
							machine.FarPointerWordsVal(machine.ConstVal(0x49ee), machine.ConstVal(fx.SDB.DGroupFrame)),
						},
						Result: &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x18f5},
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
	want := "call PszProductionETA(&sel.pl) -> callresult(char *)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

// TestLowerMachineResolvesNestedCodeSegmentFarPointerCallArg verifies nested
// code-segment globals use CS, not DGROUP, when resolving far-pointer offsets.
func TestLowerMachineResolvesNestedCodeSegmentFarPointerCallArg(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("EnsureMacintiShdefs")
	if fn == nil {
		t.Fatal("EnsureMacintiShdefs not found")
	}
	random := fx.SDB.GetFunction("Random")
	if random == nil {
		t.Fatal("Random not found")
	}
	target := fx.SDB.GetFunction("FCreateAiShdef")
	if target == nil {
		t.Fatal("FCreateAiShdef not found")
	}

	randomResult := &machine.CallResult{Target: random, Type: random.Ret, InstOff: 0x3127}
	ish := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -4,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x314b, Role: machine.OperandSrc},
	})
	shBase := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -0x10,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x314b, Role: machine.OperandSrc},
	})
	ishIndex := machine.BinaryVal(machine.ValueOpAdd, randomResult, shBase)
	ishOffset := machine.BinaryVal(machine.ValueOpAdd,
		machine.ConstVal(0x2a06),
		machine.BinaryVal(machine.ValueOpShl, ishIndex, machine.ConstVal(1)),
	)
	ishLoad := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.RegVal(asm.RegCS),
		Base:  ishOffset,
		Width: 2,
	})
	aipOffset := machine.BinaryVal(machine.ValueOpAdd, machine.ConstVal(0x2a44), ishLoad)

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x3123,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x3123, InstOff: 0x3127},
						Target:   random,
						Args:     []machine.Value{machine.ConstVal(4)},
						Result:   randomResult,
					},
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x3123, InstOff: 0x314b},
						Target:   target,
						Args: []machine.Value{
							ish,
							machine.ConstVal(9),
							machine.FarPointerWordsVal(aipOffset, machine.RegVal(asm.RegCS)),
						},
						Result: &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x314b},
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
		t.Fatalf("effects = %q, want lowered FCreateAiShdef call", formatEffectsForTest(semFunc.Blocks[0].Effects))
	}

	got := FormatEffect(semFunc.Blocks[0].Effects[0])
	want := "call FCreateAiShdef(ish, 0x9, &vrgMacAip[vrgMacIshAip[(Random(0x4) + shBase)]]) -> callresult(int16_t)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

// TestLowerMachineCollapsesDefaultDataFarPointerCallArg verifies DS:near pointer
// call args lower to the source pointer when the callee expects a far pointer.
func TestLowerMachineCollapsesDefaultDataFarPointerCallArg(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("MergeFleetsDlg")
	if fn == nil {
		t.Fatal("MergeFleetsDlg not found")
	}
	target := fx.SDB.GetFunction("WinHelp")
	if target == nil {
		t.Fatal("WinHelp not found")
	}
	helpFile := fx.SDB.GetGlobal("szHelpFile")
	if helpFile == nil {
		t.Fatal("szHelpFile not found")
	}

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x07d1,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x07d1, InstOff: 0x07e7},
						Target:   target,
						Args: []machine.Value{
							machine.ConstVal(0x1234),
							machine.FarPointerWordsVal(
								machine.LoadVal(machine.MemoryAccess{
									Seg:   machine.RegVal(asm.RegDS),
									Disp:  int(helpFile.Addr.Off),
									Width: helpFile.Type.Bytes(),
								}),
								machine.RegVal(asm.RegDS),
							),
							machine.ConstVal(1),
							machine.ConstVal(0x44a),
						},
						Result: &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x07e7},
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
	want := "call WinHelp(0x1234, szHelpFile, 0x1, 0x44a) -> callresult(int16_t)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

// TestLowerMachineCollapsesNullFarPointerCallArg verifies 0:0 far pointer call
// args lower to a source null pointer constant.
func TestLowerMachineCollapsesNullFarPointerCallArg(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("About")
	if fn == nil {
		t.Fatal("About not found")
	}
	target := fx.SDB.GetFunction("SetTimer")
	if target == nil {
		t.Fatal("SetTimer not found")
	}

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x1261,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x1261, InstOff: 0x1291},
						Target:   target,
						Args: []machine.Value{
							machine.ConstVal(0x1234),
							machine.ConstVal(0xe),
							machine.ConstVal(0x32),
							machine.FarPointerWordsVal(machine.ConstVal(0), machine.ConstVal(0)),
						},
						Result: &machine.CallResult{Target: target, Type: target.Ret, InstOff: 0x1291},
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
	want := "call SetTimer(0x1234, 0xe, 0x32, 0x0) -> callresult(uint16_t)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

// TestLowerMachineCollapsesStackFarPointerCallArg verifies SS:local far
// pointer args lower to source address-of expressions.
func TestLowerMachineCollapsesStackFarPointerCallArg(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("About")
	if fn == nil {
		t.Fatal("About not found")
	}
	target := fx.SDB.GetFunction("GetClientRect")
	if target == nil {
		t.Fatal("GetClientRect not found")
	}

	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x12a9,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x12a9, InstOff: 0x12b3},
						Target:   target,
						Args: []machine.Value{
							machine.LoadVal(machine.MemoryAccess{
								Base:   machine.FrameBaseVal(),
								Disp:   0xe,
								Width:  2,
								Origin: machine.Origin{InstOff: 0x12a9, Role: machine.OperandSrc},
							}),
							machine.FarPointerWordsVal(
								machine.AddressVal(machine.MemoryAccess{
									Base:   machine.FrameBaseVal(),
									Disp:   -0xa,
									Width:  2,
									Origin: machine.Origin{InstOff: 0x12ac, Role: machine.OperandSrc},
								}),
								machine.RegVal(asm.RegSS),
							),
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
	want := "call GetClientRect(hwnd, &rc)"
	if got != want {
		t.Fatalf("semantic effect = %q, want %q", got, want)
	}
}

func TestLowerMachineResolvesSelectorFieldFarPointerMemory(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("AddItemToQueue")
	if fn == nil {
		t.Fatal("AddItemToQueue not found")
	}
	sel := fx.SDB.GetGlobal("sel")
	if sel == nil {
		t.Fatal("sel not found")
	}

	lpplprod := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.ConstVal(uint(sel.Addr.Off)),
		Disp:  0xcc,
		Width: 4,
	})
	lpplprodOff := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.ConstVal(uint(sel.Addr.Off)),
		Disp:  0xcc,
		Width: 2,
	})
	lpplprodSeg := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.ConstVal(uint(sel.Addr.Off)),
		Disp:  0xce,
		Width: 2,
	})
	iprodMac := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.FarPointerVal(lpplprod, machine.FarPointerSegment),
		Base:  machine.FarPointerVal(lpplprod, machine.FarPointerOffset),
		Disp:  3,
		Width: 1,
	})
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x3e72,
				Effects: []machine.Effect{
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x3e50, InstOff: 0x3e63},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JNZ", LHS: lpplprodOff, RHS: machine.ConstVal(0)},
						TrueBlock:  0x3e72,
						FalseBlock: 0x3e68,
					},
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x3e68, InstOff: 0x3e6d},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JZ", LHS: lpplprodSeg, RHS: machine.ConstVal(0)},
						TrueBlock:  0x3e88,
						FalseBlock: 0x3e72,
					},
					machine.BranchEffect{
						MetaInfo:   machine.Meta{BlockID: 0x3e72, InstOff: 0x3e80},
						Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JA", LHS: iprodMac, RHS: machine.ConstVal(0xc8)},
						TrueBlock:  0x4254,
						FalseBlock: 0x3e82,
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
		t.Fatalf("effects = %#v, want one block with three lowered branch effects", semFunc.Blocks)
	}

	wants := []string{
		"branch LOWORD(sel.pl.lpplprod) != 0x0 ? L_3e72 : L_3e68",
		"branch HIWORD(sel.pl.lpplprod) == 0x0 ? L_3e88 : L_3e72",
		"branch sel.pl.lpplprod->iprodMac > 0xc8 ? L_4254 : L_3e82",
	}
	for i, want := range wants {
		if got := FormatEffect(semFunc.Blocks[0].Effects[i]); got != want {
			t.Fatalf("effect %d = %q, want %q", i, got, want)
		}
	}
}

func TestLowerMachineResolvesCompatibleCallArgPointerArrayField(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("UninhabitPlanet")
	if fn == nil {
		t.Fatal("UninhabitPlanet not found")
	}
	freePl := fx.SDB.GetFunction("FreePl")
	if freePl == nil {
		t.Fatal("FreePl not found")
	}
	lpPlanets := fx.SDB.GetGlobal("lpPlanets")
	if lpPlanets == nil {
		t.Fatal("lpPlanets not found")
	}

	lppl := machine.LoadVal(machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  4,
		Origin: machine.Origin{InstOff: 0x87da, Role: machine.OperandSrc},
	})
	id := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.FarPointerVal(lppl, machine.FarPointerSegment),
		Base:  machine.FarPointerVal(lppl, machine.FarPointerOffset),
		Width: 2,
	})
	lpPlanetsOff := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.ConstVal(uint(lpPlanets.Addr.Off)),
		Width: 2,
	})
	lpPlanetsSeg := machine.LoadVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.ConstVal(uint(lpPlanets.Addr.Off)),
		Disp:  2,
		Width: 2,
	})
	planetOffset := machine.WordVal(machine.BinaryVal(machine.ValueOpMul, machine.ConstVal(0x38), id), machine.WordLow)
	planetBase := machine.BinaryVal(machine.ValueOpAdd, lpPlanetsOff, planetOffset)
	lpplprodOff := machine.LoadVal(machine.MemoryAccess{
		Seg:   lpPlanetsSeg,
		Base:  planetBase,
		Disp:  0x34,
		Width: 2,
	})
	lpplprodSeg := machine.LoadVal(machine.MemoryAccess{
		Seg:   lpPlanetsSeg,
		Base:  planetBase,
		Disp:  0x36,
		Width: 2,
	})
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x87da,
				Effects: []machine.Effect{
					machine.CallEffect{
						MetaInfo: machine.Meta{BlockID: 0x87da, InstOff: 0x87f7},
						Target:   freePl,
						Args:     []machine.Value{machine.FarPointerWordsVal(lpplprodOff, lpplprodSeg)},
						Result:   &machine.CallResult{Target: freePl, Type: freePl.Ret, InstOff: 0x87f7},
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x87da, InstOff: 0x8814},
						Addr:     lpplprodOff.(*machine.Load).Access,
						Src:      machine.ConstVal(0),
						Width:    2,
					},
					machine.StoreEffect{
						MetaInfo: machine.Meta{BlockID: 0x87da, InstOff: 0x881a},
						Addr:     lpplprodSeg.(*machine.Load).Access,
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
	if len(semFunc.Blocks) != 1 || len(semFunc.Blocks[0].Effects) != 2 {
		t.Fatalf("effects = %#v, want one lowered call effect and one collapsed store", semFunc.Blocks)
	}

	wants := []string{
		"call FreePl(lpPlanets[lppl->id].lpplprod) -> callresult(void)",
		"lpPlanets[lppl->id].lpplprod = 0x0",
	}
	for i, want := range wants {
		if got := FormatEffect(semFunc.Blocks[0].Effects[i]); got != want {
			t.Fatalf("effect %d = %q, want %q", i, got, want)
		}
	}
}
