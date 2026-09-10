package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// mustFuncContext returns a fully initialized function context for fixture
// tests.
func mustFuncContext(t *testing.T, fx *testfixture.StarsFixture, res *symresolve.Resolver, name string) *FuncContext {
	t.Helper()
	fn := fx.SDB.GetFunction(name)
	if fn == nil {
		t.Fatalf("function %s not found", name)
	}
	return NewFuncContext(fx.Image, fx.SDB, res, fn)
}

// frameLoad returns a BP-relative load at the supplied function-relative
// instruction offset.
func frameLoad(ctx *FuncContext, relOff uint32, disp int, width int) machine.Value {
	return machine.LoadVal(frameMemoryAccess(ctx, relOff, disp, width))
}

// frameMemoryAccess returns a BP-relative MemoryAccess at the supplied function-relative
// instruction offset
func frameMemoryAccess(ctx *FuncContext, relOff uint32, disp int, width int) machine.MemoryAddress {
	return machine.MemoryAddress{
		Base:   machine.FrameBaseVal(),
		Disp:   disp,
		Width:  width,
		Origin: machine.Origin{InstOff: ctx.fs.Addr.Off + relOff, Role: machine.OperandSrc},
	}
}

func Test_symbolResolver_symbolFromMemoryAccess(t *testing.T) {

	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	cs := machine.RegVal(asm.RegCS)
	ds := machine.RegVal(asm.RegDS)
	ss := machine.RegVal(asm.RegSS)
	fb := machine.FrameBaseVal()

	cMaxMinesCtx := mustFuncContext(t, fx, res, "CMaxMines")

	pushRandomCtx := mustFuncContext(t, fx, res, "PushRandom")

	ensureCyberAiShdefsCtx := mustFuncContext(t, fx, res, "EnsureCyberAiShdefs")

	getTechLevelCostCtx := mustFuncContext(t, fx, res, "GetTechLevelCost")
	iLevel := frameLoad(getTechLevelCostCtx, 0, 0x8, 2)

	drawBuildSelHullCtx := mustFuncContext(t, fx, res, "DrawBuildSelHull")
	viVCRFocus := machine.LoadVal(machine.MemoryAddress{Seg: ds, Disp: 0x5478, Width: 2})

	addMinesToBlockedQueuesCtx := mustFuncContext(t, fx, res, "AddMinesToBlockedQueues")
	ipl := frameLoad(addMinesToBlockedQueuesCtx, 0, -0x20, 2)

	fLookupPartCtx := mustFuncContext(t, fx, res, "FLookupPart")

	tests := []struct {
		name   string
		ctx    *FuncContext
		mem    machine.MemoryAddress
		want   string
		wantOk bool
	}{
		// func params
		{
			name:   "bp+iplr",
			ctx:    cMaxMinesCtx,
			mem:    machine.MemoryAddress{Seg: ss, Base: fb, Disp: 0xa, Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "iplr",
			wantOk: true,
		},
		{
			name:   "bp+lppl",
			ctx:    cMaxMinesCtx,
			mem:    machine.MemoryAddress{Seg: ss, Base: fb, Disp: 0x6, Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "lppl",
			wantOk: true,
		},
		{
			name:   "bp+lppl+0x2",
			ctx:    cMaxMinesCtx,
			mem:    machine.MemoryAddress{Seg: ss, Base: fb, Disp: 0x8, Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "lppl+0x2",
			wantOk: true,
		},
		// func vars
		{
			name:   "bp+lPopMax+0x2",
			ctx:    cMaxMinesCtx,
			mem:    machine.MemoryAddress{Seg: ss, Base: fb, Disp: -0x8, Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "lPopMax+0x2",
			wantOk: true,
		},
		// direct global
		{
			name:   "global hbrButtonFace",
			ctx:    cMaxMinesCtx,
			mem:    machine.MemoryAddress{Seg: ds, Base: machine.ConstVal(0x0010), Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "hbrButtonFace",
			wantOk: true,
		},
		{
			name:   "global gd",
			ctx:    cMaxMinesCtx,
			mem:    machine.MemoryAddress{Seg: ds, Base: machine.ConstVal(0x07ca), Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "gd",
			wantOk: true,
		},
		{
			name:   "global gd field",
			ctx:    cMaxMinesCtx,
			mem:    machine.MemoryAddress{Seg: ds, Base: machine.ConstVal(0x07ce), Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "gd+0x4",
			wantOk: true,
		},
		{
			name:   "global sel point x",
			ctx:    cMaxMinesCtx,
			mem:    machine.MemoryAddress{Seg: ds, Base: machine.ConstVal(0x4956), Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "sel.pt.x",
			wantOk: true,
		},
		{
			name:   "global sel point y with displacement",
			ctx:    cMaxMinesCtx,
			mem:    machine.MemoryAddress{Seg: ds, Base: machine.ConstVal(0x4956), Disp: 0x2, Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "sel.pt.y",
			wantOk: true,
		},
		{
			name: "near pointer first field",
			ctx:  fLookupPartCtx,
			mem: machine.MemoryAddress{
				Seg:   ds,
				Base:  frameLoad(fLookupPartCtx, 0, 0x6, 2),
				Width: 2,
			},
			want:   "ppart->hs.grhst",
			wantOk: true,
		},
		// direct cs globals
		{
			name:   "cs global vrgCyberIshAip",
			ctx:    ensureCyberAiShdefsCtx,
			mem:    machine.MemoryAddress{Seg: cs, Base: machine.ConstVal(0x46b0), Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "vrgCyberIshAip",
			wantOk: true,
		},
		{
			name:   "cs global vrgCyberIshAip indexed",
			ctx:    ensureCyberAiShdefsCtx,
			mem:    machine.MemoryAddress{Seg: cs, Base: machine.ConstVal(0x46c8), Width: 2, Origin: machine.Origin{InstOff: cMaxMinesCtx.fs.Addr.Off}},
			want:   "vrgCyberIshAip+0x18",
			wantOk: true,
		},
		// indexed memory
		{
			name: "cs global rglTechCost indexed with binary op",
			ctx:  getTechLevelCostCtx,
			mem: machine.MemoryAddress{
				Seg: cs,
				Base: machine.BinaryVal(
					machine.ValueOpAdd,
					machine.BinaryVal(machine.ValueOpMul, iLevel, machine.ConstVal(4)),
					machine.ConstVal(0x1d4e),
				),
				Width: 2,
			},
			want:   "rglTechCost[iLevel]",
			wantOk: true,
		},
		{
			// ds:[(load(ds:[0x1378]) * 0x8)+0x5238]
			name: "nested index global rgl",
			ctx:  pushRandomCtx,
			mem: machine.MemoryAddress{
				Seg:  ds,
				Disp: 0x5238,
				Base: machine.BinaryVal(
					machine.ValueOpMul,
					machine.LoadVal(machine.MemoryAddress{
						Seg:   ds,
						Disp:  0x1378,
						Width: 2,
					}),
					machine.ConstVal(0x8),
				),
				Width: 2,
			},
			want:   "rglRandStack[cRandStack]",
			wantOk: true,
		},
		{
			// ds:[(load(ds:[0x1378]) * 0x8)+0x523a]
			name: "nested index global rgl+0x2",
			ctx:  pushRandomCtx,
			mem: machine.MemoryAddress{
				Seg:  ds,
				Disp: 0x523a,
				Base: machine.BinaryVal(
					machine.ValueOpMul,
					machine.LoadVal(machine.MemoryAddress{
						Seg:   ds,
						Disp:  0x1378,
						Width: 2,
					}),
					machine.ConstVal(0x8),
				),
				Width: 2,
			},
			want:   "rglRandStack[cRandStack]+0x2",
			wantOk: true,
		},
		{
			// load([vrgtok+0x2]):[(load([vrgtok]) + loword((0x1d * load([viVCRFocus]))))+0xf]
			name: "split far pointer global indexed field",
			ctx:  drawBuildSelHullCtx,
			mem: machine.MemoryAddress{
				Seg: machine.LoadVal(machine.MemoryAddress{
					Seg:   ds,
					Disp:  0x4b82, // vrgtok+0x2
					Width: 2,
				}),
				Base: machine.BinaryVal(
					machine.ValueOpAdd,
					machine.LoadVal(machine.MemoryAddress{
						Seg:   ds,
						Disp:  0x4b80, // vrgtok
						Width: 2,
					}),
					machine.WordVal(
						machine.BinaryVal(machine.ValueOpMul, machine.ConstVal(0x1d), viVCRFocus),
						machine.WordLow,
					),
				),
				Disp:  0xf,
				Width: 2,
			},
			want:   "vrgtok[viVCRFocus].wt",
			wantOk: true,
		},
		{
			// load([vrglpplAi+0x2]):[(load([vrglpplAi]) + (load([bp-ipl]) * 0x4))]
			name: "split far pointer indexed pointer value",
			ctx:  addMinesToBlockedQueuesCtx,
			mem: machine.MemoryAddress{
				Seg: machine.LoadVal(machine.MemoryAddress{
					Seg:   ds,
					Disp:  0x144e, // vrglpplAi+0x2
					Width: 2,
				}),
				Base: machine.BinaryVal(
					machine.ValueOpAdd,
					machine.LoadVal(machine.MemoryAddress{
						Seg:   ds,
						Disp:  0x144c, // vrglpplAi
						Width: 2,
					}),
					machine.BinaryVal(machine.ValueOpMul, ipl, machine.ConstVal(0x4)),
				),
				Width: 2,
			},
			want:   "vrglpplAi[ipl]",
			wantOk: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			sr := newSymbolResolver(tt.ctx)
			got, ok := sr.memoryPath(tt.mem)
			if tt.wantOk != ok {
				t.Errorf("symbolFromMemoryAccess() = %v, want %v", ok, tt.wantOk)
				t.FailNow()
			}
			if got.String() != tt.want {
				t.Errorf("symbolFromMemoryAccess() = %v, want %v", got.String(), tt.want)
			}
		})
	}
}

// TestSymbolResolverUsesConfiguredBlockUnionContext verifies generated block facts are available before semantic passes run.
func TestSymbolResolverUsesConfiguredBlockUnionContext(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "FLookupPart")
	access := machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
		Base:  frameLoad(ctx, 0, 0x6, 2),
		Disp:  4,
		Width: 4,
	}

	ctx.SetCurrentBlock(0x52a1)
	got, ok := ctx.symbols.memoryPath(access)
	if !ok || got.String() != "ppart->pengine" {
		t.Fatalf("configured block access = %v, %v; want ppart->pengine", got, ok)
	}

	// A flow-derived context may add other selections, but it must not replace
	// the sparse authoritative fact installed for this exact block.
	facts := fx.SDB.UnionRules.BlockFactsFor(ctx.fs, 0x52a1)
	if len(facts) != 1 {
		t.Fatalf("configured block facts = %d, want 1", len(facts))
	}
	root, ok := ctx.symbolRootByName(facts[0].Root)
	if !ok {
		t.Fatal("configured block root did not resolve")
	}
	derived := symresolve.NewUnionContext()
	if !derived.Add(root, facts[0].Rule, facts[0].Rule.Enum.GetValue("hstHull")) {
		t.Fatal("failed to add conflicting derived selection")
	}
	ctx.SetUnionContexts(map[machine.BlockID]*symresolve.UnionContext{0x52a1: derived})
	ctx.SetCurrentBlock(0x52a1)
	got, ok = ctx.symbols.memoryPath(access)
	if !ok || got.String() != "ppart->pengine" {
		t.Fatalf("merged block access = %v, %v; want configured ppart->pengine", got, ok)
	}

	ctx.SetCurrentBlock(0x524e)
	got, ok = ctx.symbols.memoryPath(access)
	if !ok || got.String() != "ppart->pcom" {
		t.Fatalf("default block access = %v, %v; want ppart->pcom", got, ok)
	}

	ctx.SetCurrentBlock(0x5676)
	got, ok = ctx.symbols.memoryPath(access)
	if !ok || got.String() != "ppart->phul" {
		t.Fatalf("configured hull block access = %v, %v; want ppart->phul", got, ok)
	}

	facts = ctx.RecordedUnionBlockPathFacts()[0x52a1]
	if len(facts) != 1 || facts[0].Root != "ppart" || facts[0].Value.Name != "hstEngine" {
		t.Fatalf("recorded configured facts = %+v; want ppart=hstEngine", facts)
	}
	if facts := ctx.RecordedUnionBlockPathFacts()[0x5676]; len(facts) != 1 || facts[0].Value.Name != "hstHull" {
		t.Fatalf("recorded configured hull facts = %+v; want hstHull", facts)
	}
}

// TestSymbolResolverDoesNotDerefNonPointerLoad verifies scalar indexed
// addressing is not represented as a pointer dereference.
func TestSymbolResolverDoesNotDerefNonPointerLoad(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "BattlePlansDlg")
	sr := newSymbolResolver(ctx)
	ds := machine.RegVal(asm.RegDS)

	mem := machine.MemoryAddress{
		Seg: ds,
		Base: machine.LoadVal(machine.MemoryAddress{
			Seg:   ds,
			Disp:  0x018c,
			Width: 2,
		}),
		Disp:  0x4830,
		Width: 1,
	}

	if path, ok := sr.memoryPath(mem); ok {
		t.Fatalf("symbolFromMemoryAccess() = %v, %v; want unresolved non-pointer access", path, ok)
	}
}

func Test_symbolResolver_symbolFromValueTyped(t *testing.T) {

	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	getRaceStatFunc := fx.SDB.GetFunction("GetRaceStat")
	cMaxMinesCtx := mustFuncContext(t, fx, res, "CMaxMines")
	iplr := frameLoad(cMaxMinesCtx, cMaxMinesCtx.fs.Addr.Off, 0xa, 2)
	lpplType := cMaxMinesCtx.fs.Params[0].Type

	aboutCtx := mustFuncContext(t, fx, res, "About")
	rc := frameMemoryAccess(aboutCtx, 0, -0xa, 2)
	ss := machine.RegVal(asm.RegSS)
	battlePlansCtx := mustFuncContext(t, fx, res, "BattlePlansDlg")
	idPlayer := machine.LoadVal(machine.MemoryAddress{Seg: machine.RegVal(asm.RegDS), Disp: 0x018c, Width: 2})
	playerIndex := machine.BinaryVal(
		machine.ValueOpShl,
		machine.BinaryVal(machine.ValueOpShl, idPlayer, machine.ConstVal(1)),
		machine.ConstVal(1),
	)
	planPointerOffset := machine.LoadVal(machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
		Base:  playerIndex,
		Disp:  0x5938,
		Width: 2,
	})
	planPointerSegment := machine.LoadVal(machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
		Base:  playerIndex,
		Disp:  0x593a,
		Width: 2,
	})
	i := frameLoad(battlePlansCtx, 0xba, -0xa, 2)
	planNameOffset := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.BinaryVal(
			machine.ValueOpAdd,
			planPointerOffset,
			machine.WordVal(machine.BinaryVal(machine.ValueOpMul, machine.ConstVal(0x24), i), machine.WordLow),
		),
		machine.ConstVal(4),
	)
	addStringType := messageCallArgumentType(fx.SDB, fx.SDB.GetFunction("SendMessage"), []machine.Value{
		machine.ConstVal(0),
		machine.ConstVal(0x0403),
		machine.ConstVal(0),
		planNameOffset,
	}, 3)
	if addStringType == nil {
		t.Fatal("CB_ADDSTRING LPARAM message type not selected")
	}

	fTrackSlotCtx := mustFuncContext(t, fx, res, "FTrackSlot")
	ds := machine.RegVal(asm.RegDS)

	tests := []struct {
		name     string
		ctx      *FuncContext
		value    machine.Value
		expected typeinfo.Type
		want     string
		wantOk   bool
	}{
		{
			name:     "simple constant",
			ctx:      cMaxMinesCtx,
			value:    machine.ConstVal(0x8),
			expected: getRaceStatFunc.Params[1].Type,
			want:     "0x8",
			wantOk:   true,
		},
		{
			// (0x59a2 + loword((0xc0 * load([bp+iplr])))
			name: "global rgplr indexed with lppl load",
			ctx:  cMaxMinesCtx,
			value: machine.BinaryVal(
				machine.ValueOpAdd,
				machine.BinaryVal(machine.ValueOpMul, iplr, machine.ConstVal(0xc0)),
				machine.ConstVal(0x59a2),
			),
			expected: getRaceStatFunc.Params[0].Type,
			want:     "rgplr[iplr]",
			wantOk:   true,
		},
		{
			// farptr(0x0, 0x0)
			name:     "null pointer",
			ctx:      cMaxMinesCtx,
			value:    &machine.StackWords{Words: []machine.Value{machine.ConstVal(0x0), machine.ConstVal(0x0)}},
			expected: lpplType,
			want:     "0x0",
			wantOk:   true,
		},
		{
			// farptr(ss, addr([bp-rc])
			name:     "ss farpointer for local var address",
			ctx:      aboutCtx,
			value:    &machine.StackWords{Words: []machine.Value{ss, machine.AddressVal(rc)}},
			expected: &typeinfo.Pointer{Elem: fx.SDB.GetStruct("RECT"), Class: typeinfo.PtrFar},
			want:     "rc",
			wantOk:   true,
		},
		{
			name: "split far string pointer with indexed struct field",
			ctx:  battlePlansCtx,
			value: &machine.StackWords{Words: []machine.Value{
				planPointerSegment,
				planNameOffset,
			}},
			expected: addStringType,
			want:     "rglpbtlplan[idPlayer][i].szName",
			wantOk:   true,
		},
		{
			name: "ds farpointer to indexed global struct index 0",
			ctx:  fTrackSlotCtx,
			value: &machine.StackWords{Words: []machine.Value{
				ds,
				machine.ConstVal(0x5926),
			}},
			expected: &typeinfo.Pointer{
				Elem:  fx.SDB.GetStruct("RECT"),
				Class: typeinfo.PtrFar,
			},
			want:   "rgrcBuildSpin",
			wantOk: true,
		},
				{
			name: "ds farpointer to indexed global struct index 1",
			ctx:  fTrackSlotCtx,
			value: &machine.StackWords{Words: []machine.Value{
				ds,
				machine.ConstVal(0x592e),
			}},
			expected: &typeinfo.Pointer{
				Elem:  fx.SDB.GetStruct("RECT"),
				Class: typeinfo.PtrFar,
			},
			want:   "rgrcBuildSpin[0x1]",
			wantOk: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			sr := newSymbolResolver(tt.ctx)
			got, ok := sr.symbolFromValueTyped(tt.value, tt.expected)
			if tt.wantOk != ok {
				t.Errorf("symbolFromValueTyped() = %v, want %v", ok, tt.wantOk)
				t.FailNow()
			}
			if got.String() != tt.want {
				t.Errorf("symbolFromValueTyped() = %v, want %v", got.String(), tt.want)
			}
		})
	}
}
