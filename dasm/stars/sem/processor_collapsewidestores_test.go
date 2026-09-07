package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCollapseWideMachineStorePair verifies adjacent 16-bit stores collapse
// into one 32-bit machine store when their addresses and values line up.
func TestCollapseWideMachineStorePair(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	ds := machine.RegVal(asm.RegDS)

	xorFileBufCtx := mustFuncContext(t, fx, res, "XorFileBuf")
	xorResult := &machine.CallResult{
		Target:  &typeinfo.Function{Name: "LGetNextFileXor", Ret: typeinfo.I32},
		Type:    typeinfo.I32,
		InstOff: 0x1cf3,
	}

	pl := frameMemoryAccess(xorFileBufCtx, 0, -0x6, 2)
	plLoad := machine.LoadVal(pl)
	plLowAddr := machine.MemoryAddress{
		Seg:   ds,
		Base:  plLoad,
		Width: 2,
		Origin: machine.Origin{
			InstOff: 0x1cfb,
			Role:    machine.OperandDst,
		},
	}
	plHighAddr := plLowAddr
	plHighAddr.Disp = 0x2
	plHighAddr.Origin = machine.Origin{
		InstOff: 0x1cfd,
		Role:    machine.OperandDst,
	}

	cMaxMinesCtx := mustFuncContext(t, fx, res, "CMaxMines")
	lPopMaxAddr := frameMemoryAccess(cMaxMinesCtx, 0, -0xa, 2)
	lPopMaxHighAddr := frameMemoryAccess(cMaxMinesCtx, 0, -0x8, 2)
	cMaxAddr := frameMemoryAccess(cMaxMinesCtx, 0, -0x6, 2)
	cMaxHighAddr := frameMemoryAccess(cMaxMinesCtx, 0, -0x4, 2)
	cMaxResult := &machine.CallResult{
		Target:  &typeinfo.Function{Name: "__aFldiv", Ret: typeinfo.I32},
		Type:    typeinfo.I32,
		InstOff: 0x729e,
	}

	calcPctSurviveCtx := mustFuncContext(t, fx, res, "CalcPctSurvive")
	cDefensesAddr := frameMemoryAccess(calcPctSurviveCtx, 0x37a-calcPctSurviveCtx.fs.Addr.Off, -0x8, 2)
	cDefensesHighAddr := frameMemoryAccess(calcPctSurviveCtx, 0x37d-calcPctSurviveCtx.fs.Addr.Off, -0x6, 2)
	cMaxLoad := frameLoad(calcPctSurviveCtx, 0x3b9-calcPctSurviveCtx.fs.Addr.Off, -0x16, 2)
	lppl := machine.LoadVal(frameMemoryAccess(calcPctSurviveCtx, 0, 0x6, 4))
	cDefensesSrc := machine.BinaryVal(
		machine.ValueOpAnd,
		machine.LoadVal(machine.MemoryAddress{
			Base:  lppl,
			Disp:  0x18,
			Width: 2,
		}),
		machine.ConstVal(0xfff),
	)

	calcPlayerScoreCtx := mustFuncContext(t, fx, res, "CalcPlayerScore")
	scoreAddr := frameMemoryAccess(calcPlayerScoreCtx, 0x5951-calcPlayerScoreCtx.fs.Addr.Off, -0x26, 2)
	scoreHighAddr := frameMemoryAccess(calcPlayerScoreCtx, 0x5954-calcPlayerScoreCtx.fs.Addr.Off, -0x24, 2)
	lTempAddr := frameMemoryAccess(calcPlayerScoreCtx, 0x594b-calcPlayerScoreCtx.fs.Addr.Off, -0x12, 2)
	lTempHighAddr := frameMemoryAccess(calcPlayerScoreCtx, 0x594e-calcPlayerScoreCtx.fs.Addr.Off, -0x10, 2)
	indexedLowAddr := frameMemoryAccess(calcPlayerScoreCtx, 0x5b94-calcPlayerScoreCtx.fs.Addr.Off, -0xe, 2)
	indexedHighAddr := frameMemoryAccess(calcPlayerScoreCtx, 0x5b98-calcPlayerScoreCtx.fs.Addr.Off, -0xc, 2)
	indexedLowValue := frameLoad(calcPlayerScoreCtx, 0x5b90-calcPlayerScoreCtx.fs.Addr.Off, -0x30, 2)
	indexedHighValue := frameLoad(calcPlayerScoreCtx, 0x5b92-calcPlayerScoreCtx.fs.Addr.Off, -0x30, 2)
	indexedLowAddr.Seg = machine.RegVal(asm.RegSS)
	indexedLowAddr.Index = machine.BinaryVal(
		machine.ValueOpShl,
		machine.BinaryVal(machine.ValueOpShl, indexedLowValue, machine.ConstVal(1)),
		machine.ConstVal(1),
	)
	indexedHighAddr.Seg = machine.RegVal(asm.RegSS)
	indexedHighAddr.Index = machine.BinaryVal(machine.ValueOpMul, indexedHighValue, machine.ConstVal(4))
	wordArrayLowAddr := frameMemoryAccess(calcPlayerScoreCtx, 0x5b90-calcPlayerScoreCtx.fs.Addr.Off, -0x5c, 2)
	wordArrayHighAddr := frameMemoryAccess(calcPlayerScoreCtx, 0x5b92-calcPlayerScoreCtx.fs.Addr.Off, -0x5a, 2)
	wordArrayLowAddr.Seg = machine.RegVal(asm.RegSS)
	wordArrayLowAddr.Index = machine.BinaryVal(machine.ValueOpShl, indexedLowValue, machine.ConstVal(1))
	wordArrayHighAddr.Seg = machine.RegVal(asm.RegSS)
	wordArrayHighAddr.Index = machine.BinaryVal(machine.ValueOpShl, indexedHighValue, machine.ConstVal(1))

	addMinesCtx := mustFuncContext(t, fx, res, "AddMinesToBlockedQueues")
	cResAddr := frameMemoryAccess(addMinesCtx, 0x19a4-addMinesCtx.fs.Addr.Off, -0x1e, 2)
	cResHighAddr := frameMemoryAccess(addMinesCtx, 0x19a7-addMinesCtx.fs.Addr.Off, -0x1c, 2)
	scratchAddr := frameMemoryAccess(addMinesCtx, 0x19eb-addMinesCtx.fs.Addr.Off, -0x136, 2)
	scratchHighAddr := frameMemoryAccess(addMinesCtx, 0x19ef-addMinesCtx.fs.Addr.Off, -0x134, 2)
	cResDelta := &machine.CallResult{
		Target:  &typeinfo.Function{Name: "__aFldiv", Ret: typeinfo.I32},
		Type:    typeinfo.I32,
		InstOff: 0x19a0,
	}

	// lpProc var is in scope 1 so adjust its relative offset from the About function address
	aboutCtx := mustFuncContext(t, fx, res, "About")
	lpProcAddr := frameMemoryAccess(aboutCtx, 0x14A3-aboutCtx.fs.Addr.Off, -0xe, 2)
	lpProcHighAddr := frameMemoryAccess(aboutCtx, 0x14A3-aboutCtx.fs.Addr.Off, -0xc, 2)
	makeProcInstanceResult := &machine.CallResult{
		Target: &typeinfo.Function{
			Name: "MakeProcInstance",
			Ret:  &typeinfo.Pointer{Elem: &typeinfo.Function{Ret: typeinfo.I32}, Class: typeinfo.PtrFar},
		},
		Type:    &typeinfo.Pointer{Elem: &typeinfo.Function{Ret: typeinfo.I32}, Class: typeinfo.PtrFar},
		InstOff: 0x14af,
	}
	phiLeft := &machine.Block{ID: 0x1000}
	phiRight := &machine.Block{ID: 0x1010}

	// adjacent independent locals
	getTechLevelCostCtx := mustFuncContext(t, fx, res, "GetTechLevelCost")
	local1 := frameMemoryAccess(getTechLevelCostCtx, 0, -0xa, 2) // cTech
	local2 := frameMemoryAccess(getTechLevelCostCtx, 0, -0x8, 2) // i

	uninhabitPlanetCtx := mustFuncContext(t, fx, res, "UninhabitPlanet")
	uninhabitLPPL := machine.LoadVal(frameMemoryAccess(uninhabitPlanetCtx, 0, 0x6, 4))
	planetDst := func(disp int) machine.MemoryAddress {
		return machine.MemoryAddress{
			Seg:   machine.FarPointerVal(uninhabitLPPL, machine.FarPointerSegment),
			Base:  machine.FarPointerVal(uninhabitLPPL, machine.FarPointerOffset),
			Disp:  disp,
			Width: 2,
		}
	}
	planetLoad := func(disp int) machine.MemoryAddress {
		return machine.MemoryAddress{Base: uninhabitLPPL, Disp: disp, Width: 2}
	}
	maskedPlanetWord := func(disp int, keep, set uint) machine.Value {
		return machine.BinaryVal(
			machine.ValueOpOr,
			machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(planetLoad(disp)), machine.ConstVal(keep)),
			machine.ConstVal(set),
		)
	}

	tests := []struct {
		name        string
		ctx         *FuncContext
		low         machine.StoreEffect
		high        machine.StoreEffect
		wantSrc     string
		wantAddr    string
		wantWidth   int
		wantChanged bool
	}{
		{
			name: "adjacent word locals should not merge",
			low: machine.StoreEffect{
				Addr:  local1,
				Src:   machine.ConstVal(0),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  local2,
				Src:   machine.ConstVal(0),
				Width: 2,
			},
			ctx:         getTechLevelCostCtx,
			wantChanged: false,
		},
		{
			name: "adjacent word fields should not merge",
			ctx:  uninhabitPlanetCtx,
			low: machine.StoreEffect{
				Addr:  planetDst(0),
				Src:   machine.ConstVal(0),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  planetDst(2),
				Src:   machine.ConstVal(0),
				Width: 2,
			},
			wantChanged: false,
		},
		{
			name: "adjacent word array elements should not merge",
			ctx:  calcPlayerScoreCtx,
			low: machine.StoreEffect{
				Addr:  wordArrayLowAddr,
				Src:   machine.ConstVal(0),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  wordArrayHighAddr,
				Src:   machine.ConstVal(0),
				Width: 2,
			},
			wantChanged: false,
		},
		{
			name: "adjacent scratch words should not merge without a declared root",
			ctx:  addMinesCtx,
			low: machine.StoreEffect{
				Addr:  scratchAddr,
				Src:   machine.WordVal(cResDelta, machine.WordLow),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  scratchHighAddr,
				Src:   machine.WordVal(cResDelta, machine.WordHigh),
				Width: 2,
			},
			wantChanged: false,
		},
		{
			name: "XorFileBuf xor adjacent pointer words",
			ctx:  xorFileBufCtx,
			low: machine.StoreEffect{
				Addr:  plLowAddr,
				Src:   machine.BinaryVal(machine.ValueOpXor, machine.LoadVal(plLowAddr), machine.WordVal(xorResult, machine.WordLow)),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  plHighAddr,
				Src:   machine.BinaryVal(machine.ValueOpXor, machine.LoadVal(plHighAddr), machine.WordVal(xorResult, machine.WordHigh)),
				Width: 2,
			},
			wantSrc:     "(load(dword ds:[load([bp-0x6])]) ^ callresult(int32_t))",
			wantAddr:    "dword ds:[load([bp-0x6])]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "CMaxMines stores lPopMax callresult",
			ctx:  cMaxMinesCtx,
			low: machine.StoreEffect{
				Addr:  lPopMaxAddr,
				Src:   machine.WordVal(cMaxResult, machine.WordLow),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  lPopMaxHighAddr,
				Src:   machine.WordVal(cMaxResult, machine.WordHigh),
				Width: 2,
			},
			wantSrc:     "callresult(int32_t)",
			wantAddr:    "dword [bp-0xa]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "CMaxMines stores cMax callresult",
			ctx:  cMaxMinesCtx,
			low: machine.StoreEffect{
				Addr:  cMaxAddr,
				Src:   machine.WordVal(cMaxResult, machine.WordLow),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  cMaxHighAddr,
				Src:   machine.WordVal(cMaxResult, machine.WordHigh),
				Width: 2,
			},
			wantSrc:     "callresult(int32_t)",
			wantAddr:    "dword [bp-0x6]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "CMaxMines stores cMax constant ten",
			ctx:  cMaxMinesCtx,
			low: machine.StoreEffect{
				Addr:  cMaxAddr,
				Src:   machine.ConstVal(0xa),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  cMaxHighAddr,
				Src:   machine.ConstVal(0x0),
				Width: 2,
			},
			wantSrc:     "0xa",
			wantAddr:    "dword [bp-0x6]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "CMaxMines stores cMax zero",
			ctx:  cMaxMinesCtx,
			low: machine.StoreEffect{
				Addr:  cMaxAddr,
				Src:   machine.ConstVal(0x0),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  cMaxHighAddr,
				Src:   machine.ConstVal(0x0),
				Width: 2,
			},
			wantSrc:     "0x0",
			wantAddr:    "dword [bp-0x6]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "CMaxMines stores recursive wide phi",
			ctx:  cMaxMinesCtx,
			low: machine.StoreEffect{
				Addr: cMaxAddr,
				Src: &machine.PhiValue{Join: 0x2000, Arms: []machine.PhiArm{
					{Block: phiLeft, Value: machine.ConstVal(1)},
					{Block: phiRight, Value: machine.WordVal(cMaxResult, machine.WordLow)},
				}},
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr: cMaxHighAddr,
				Src: &machine.PhiValue{Join: 0x2000, Arms: []machine.PhiArm{
					{Block: phiLeft, Value: machine.ConstVal(0)},
					{Block: phiRight, Value: machine.WordVal(cMaxResult, machine.WordHigh)},
				}},
				Width: 2,
			},
			wantSrc:     "merge(Join: L_2000, (L_1000:0x1, L_1010:callresult(int32_t)))",
			wantAddr:    "dword [bp-0x6]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "CalcPlayerScore stores normalized-equivalent indexed lanes",
			ctx:  calcPlayerScoreCtx,
			low: machine.StoreEffect{
				Addr:  indexedLowAddr,
				Src:   machine.ConstVal(0),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  indexedHighAddr,
				Src:   machine.ConstVal(0),
				Width: 2,
			},
			wantSrc:     "0x0",
			wantAddr:    "dword [bp+((load([bp-0x30]) << 0x1) << 0x1)-0xe]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "CalcPctSurvive stores zero-extended primitive",
			ctx:  calcPctSurviveCtx,
			low: machine.StoreEffect{
				Addr:  cDefensesAddr,
				Src:   cDefensesSrc,
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  cDefensesHighAddr,
				Src:   machine.ConstVal(0x0),
				Width: 2,
			},
			wantSrc:     "(uint32_t)(load([load(dword [bp+0x6])+0x18]) & 0xfff)",
			wantAddr:    "dword [bp-0x8]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "CalcPctSurvive stores sign-extended primitive",
			ctx:  calcPctSurviveCtx,
			low: machine.StoreEffect{
				Addr:  cDefensesAddr,
				Src:   cMaxLoad,
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  cDefensesHighAddr,
				Src:   machine.WordVal(cMaxLoad, machine.WordSignHigh),
				Width: 2,
			},
			wantSrc:     "sext16to32(load([bp-0x16]))",
			wantAddr:    "dword [bp-0x8]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "CalcPlayerScore adds wide local load",
			ctx:  calcPlayerScoreCtx,
			low: machine.StoreEffect{
				MetaInfo: machine.Meta{BlockID: 0x594b, InstOff: 0x5951, InstOp: asm.OpADD, InstLen: 3},
				Addr:     scoreAddr,
				Src: &machine.Binary{
					Op:       machine.ValueOpAdd,
					LHS:      machine.LoadVal(scoreAddr),
					RHS:      machine.LoadVal(lTempAddr),
					Producer: machine.Meta{BlockID: 0x594b, InstOff: 0x5951, InstOp: asm.OpADD, InstLen: 3},
				},
				Width: 2,
			},
			high: machine.StoreEffect{
				MetaInfo: machine.Meta{BlockID: 0x594b, InstOff: 0x5954, InstOp: asm.OpADC, InstLen: 3},
				Addr:     scoreHighAddr,
				Src: &machine.Binary{
					Op:       machine.ValueOpAdd,
					LHS:      machine.LoadVal(scoreHighAddr),
					RHS:      machine.LoadVal(lTempHighAddr),
					Producer: machine.Meta{BlockID: 0x594b, InstOff: 0x5954, InstOp: asm.OpADC, InstLen: 3},
				},
				Width: 2,
			},
			wantSrc:     "(load(dword [bp-0x26]) + load(dword [bp-0x12]))",
			wantAddr:    "dword [bp-0x26]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "AddMinesToBlockedQueues subtracts wide value with borrow",
			ctx:  addMinesCtx,
			low: machine.StoreEffect{
				MetaInfo: machine.Meta{BlockID: 0x1977, InstOff: 0x19a4, InstOp: asm.OpSUB, InstLen: 3},
				Addr:     cResAddr,
				Src: &machine.Binary{
					Op:       machine.ValueOpSub,
					LHS:      machine.LoadVal(cResAddr),
					RHS:      machine.WordVal(cResDelta, machine.WordLow),
					Producer: machine.Meta{BlockID: 0x1977, InstOff: 0x19a4, InstOp: asm.OpSUB, InstLen: 3},
				},
				Width: 2,
			},
			high: machine.StoreEffect{
				MetaInfo: machine.Meta{BlockID: 0x1977, InstOff: 0x19a7, InstOp: asm.OpSBB, InstLen: 3},
				Addr:     cResHighAddr,
				Src: &machine.Binary{
					Op:       machine.ValueOpSub,
					LHS:      machine.LoadVal(cResHighAddr),
					RHS:      machine.WordVal(cResDelta, machine.WordHigh),
					Producer: machine.Meta{BlockID: 0x1977, InstOff: 0x19a7, InstOp: asm.OpSBB, InstLen: 3},
				},
				Width: 2,
			},
			wantSrc:     "(load(dword [bp-0x1e]) - callresult(int32_t))",
			wantAddr:    "dword [bp-0x1e]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "independent word subtractions do not collapse",
			ctx:  addMinesCtx,
			low: machine.StoreEffect{
				MetaInfo: machine.Meta{BlockID: 0x1977, InstOff: 0x19a4, InstOp: asm.OpSUB, InstLen: 3},
				Addr:     cResAddr,
				Src: &machine.Binary{
					Op:       machine.ValueOpSub,
					LHS:      machine.LoadVal(cResAddr),
					RHS:      machine.WordVal(cResDelta, machine.WordLow),
					Producer: machine.Meta{BlockID: 0x1977, InstOff: 0x19a4, InstOp: asm.OpSUB, InstLen: 3},
				},
				Width: 2,
			},
			high: machine.StoreEffect{
				MetaInfo: machine.Meta{BlockID: 0x1977, InstOff: 0x19a7, InstOp: asm.OpSUB, InstLen: 3},
				Addr:     cResHighAddr,
				Src: &machine.Binary{
					Op:       machine.ValueOpSub,
					LHS:      machine.LoadVal(cResHighAddr),
					RHS:      machine.WordVal(cResDelta, machine.WordHigh),
					Producer: machine.Meta{BlockID: 0x1977, InstOff: 0x19a7, InstOp: asm.OpSUB, InstLen: 3},
				},
				Width: 2,
			},
			wantChanged: false,
		},
		{
			name: "About stores lpProc far pointer",
			ctx:  aboutCtx,
			low: machine.StoreEffect{
				Addr:  lpProcAddr,
				Src:   machine.FarPointerVal(makeProcInstanceResult, machine.FarPointerOffset),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  lpProcHighAddr,
				Src:   machine.FarPointerVal(makeProcInstanceResult, machine.FarPointerSegment),
				Width: 2,
			},
			wantSrc:     "callresult(int32_t() *)",
			wantAddr:    "dword [bp-0xe]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "UninhabitPlanet clears high-word bitfield",
			ctx:  uninhabitPlanetCtx,
			low: machine.StoreEffect{
				Addr:  planetDst(0x18),
				Src:   maskedPlanetWord(0x18, 0xffff, 0),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  planetDst(0x1a),
				Src:   maskedPlanetWord(0x1a, 0xff7f, 0),
				Width: 2,
			},
			wantSrc:     "((load(dword [load(dword [bp+0x6])+0x18]) & 0xff7fffff) | 0x0)",
			wantAddr:    "dword farseg(load(dword [bp+0x6])):[faroff(load(dword [bp+0x6]))+0x18]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "UninhabitPlanet clears low-word bitfield",
			ctx:  uninhabitPlanetCtx,
			low: machine.StoreEffect{
				Addr:  planetDst(0x18),
				Src:   maskedPlanetWord(0x18, 0xf000, 0),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  planetDst(0x1a),
				Src:   maskedPlanetWord(0x1a, 0xffff, 0),
				Width: 2,
			},
			wantSrc:     "((load(dword [load(dword [bp+0x6])+0x18]) & 0xfffff000) | 0x0)",
			wantAddr:    "dword farseg(load(dword [bp+0x6])):[faroff(load(dword [bp+0x6]))+0x18]",
			wantWidth:   4,
			wantChanged: true,
		},
		{
			name: "UninhabitPlanet sets cross-word bitfield",
			ctx:  uninhabitPlanetCtx,
			low: machine.StoreEffect{
				Addr:  planetDst(0x18),
				Src:   maskedPlanetWord(0x18, 0x0fff, 0xf000),
				Width: 2,
			},
			high: machine.StoreEffect{
				Addr:  planetDst(0x1a),
				Src:   maskedPlanetWord(0x1a, 0xfffe, 0x1),
				Width: 2,
			},
			wantSrc:     "((load(dword [load(dword [bp+0x6])+0x18]) & 0xfffe0fff) | 0x1f000)",
			wantAddr:    "dword farseg(load(dword [bp+0x6])):[faroff(load(dword [bp+0x6]))+0x18]",
			wantWidth:   4,
			wantChanged: true,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			processor := &collapseWideStoresProcessor{ctx: tt.ctx}
			got, changed := processor.collapseWideMachineStorePair(tt.low, tt.high)
			if changed != tt.wantChanged {
				t.Fatalf("collapseWideMachineStorePair() changed = %v, want %v", changed, tt.wantChanged)
			}
			if changed {
				if got.Width != tt.wantWidth {
					t.Fatalf("collapseWideMachineStorePair() width = %d, want %d", got.Width, tt.wantWidth)
				}
				if got.Addr.String() != tt.wantAddr {
					t.Fatalf("collapseWideMachineStorePair() addr = %v, want %v", got.Addr, tt.wantAddr)
				}
				if got.Src.String() != tt.wantSrc {
					t.Fatalf("collapseWideMachineStorePair() src = %v, want %v", got.Src, tt.wantSrc)
				}
			}
		})
	}
}

// TestCollapseWideReadModifyWriteConstPair reconstructs a wide arithmetic
// source from low/high constant word operands.
func TestCollapseWideReadModifyWriteConstPair(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "CMaxMines")

	lowAddr := frameMemoryAccess(ctx, 0, -0x6, 2)
	highAddr := lowAddr
	highAddr.Disp += 2
	low := machine.StoreEffect{
		MetaInfo: machine.Meta{InstOff: 0x1808, InstOp: asm.OpADD, InstLen: 5},
		Addr:     lowAddr,
		Src: &machine.Binary{
			Op:       machine.ValueOpAdd,
			LHS:      machine.LoadVal(lowAddr),
			RHS:      machine.ConstVal(0xff07),
			Producer: machine.Meta{InstOff: 0x1808, InstOp: asm.OpADD, InstLen: 5},
		},
		Width: 2,
	}
	high := machine.StoreEffect{
		MetaInfo: machine.Meta{InstOff: 0x180d, InstOp: asm.OpADC, InstLen: 3},
		Addr:     highAddr,
		Src: &machine.Binary{
			Op:       machine.ValueOpAdd,
			LHS:      machine.LoadVal(highAddr),
			RHS:      machine.ConstVal(0x7fff),
			Producer: machine.Meta{InstOff: 0x180d, InstOp: asm.OpADC, InstLen: 3},
		},
		Width: 2,
	}

	got, changed := (&collapseWideStoresProcessor{ctx: ctx}).collapseWideMachineStorePair(low, high)
	if !changed {
		t.Fatal("collapseWideMachineStorePair() changed = false, want true")
	}
	if got.Width != 4 || got.Addr.Width != 4 {
		t.Fatalf("collapsed widths = (%d, %d), want (4, 4)", got.Width, got.Addr.Width)
	}
	want := "(load(dword [bp-0x6]) + 0x7fffff07)"
	if got.Src.String() != want {
		t.Fatalf("collapsed source = %q, want %q", got.Src.String(), want)
	}
}

// TestCollapseWideRegisterArithmeticStoresRequireProvenCarry verifies MOV
// stores only collapse when their source values retain SUB/SBB provenance.
func TestCollapseWideRegisterArithmeticStoresRequireProvenCarry(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "Random")

	s1Low := frameMemoryAccess(ctx, 0, -0xa, 2)
	s1High := s1Low
	s1High.Disp += 2
	s2Low := frameMemoryAccess(ctx, 0, -0x12, 2)
	s2High := s2Low
	s2High.Disp += 2
	zLow := frameMemoryAccess(ctx, 0, -0x6, 2)
	zHigh := zLow
	zHigh.Disp += 2

	low := machine.StoreEffect{
		MetaInfo: machine.Meta{InstOff: 0x181e, InstOp: asm.OpMOV, InstLen: 3},
		Addr:     zLow,
		Src:      machine.BinaryVal(machine.ValueOpSub, machine.LoadVal(s1Low), machine.LoadVal(s2Low)),
		Width:    2,
	}
	high := machine.StoreEffect{
		MetaInfo: machine.Meta{InstOff: 0x1821, InstOp: asm.OpMOV, InstLen: 3},
		Addr:     zHigh,
		Src:      machine.BinaryVal(machine.ValueOpSub, machine.LoadVal(s1High), machine.LoadVal(s2High)),
		Width:    2,
	}

	if _, changed := (&collapseWideStoresProcessor{ctx: ctx}).collapseWideMachineStorePair(low, high); changed {
		t.Fatal("MOV stores without SUB/SBB provenance collapsed")
	}

	low.Src.(*machine.Binary).Producer = machine.Meta{InstOff: 0x1818, InstOp: asm.OpSUB, InstLen: 2}
	high.Src.(*machine.Binary).Producer = machine.Meta{InstOff: 0x181a, InstOp: asm.OpSBB, InstLen: 2}
	got, changed := (&collapseWideStoresProcessor{ctx: ctx}).collapseWideMachineStorePair(low, high)
	if !changed {
		t.Fatal("MOV stores with SUB/SBB provenance did not collapse")
	}
	want := "(load(dword [bp-0xa]) - load(dword [bp-0x12]))"
	if got.Src.String() != want {
		t.Fatalf("collapsed source = %q, want %q", got.Src.String(), want)
	}
}

// TestCollapseWideMaskedStoresAllowDeclaredWideInteger verifies independently
// lane-wise bit operations collapse for any proven declared wide object.
func TestCollapseWideMaskedStoresAllowDeclaredWideInteger(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "CMaxMines")
	lowAddr := frameMemoryAccess(ctx, 0, -0xa, 2)
	highAddr := lowAddr
	highAddr.Disp += 2
	maskedWord := func(addr machine.MemoryAddress, keep uint) machine.Value {
		return machine.BinaryVal(
			machine.ValueOpOr,
			machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(addr), machine.ConstVal(keep)),
			machine.ConstVal(0),
		)
	}
	low := machine.StoreEffect{Addr: lowAddr, Src: maskedWord(lowAddr, 0xfffe), Width: 2}
	high := machine.StoreEffect{Addr: highAddr, Src: maskedWord(highAddr, 0xffff), Width: 2}

	got, changed := (&collapseWideStoresProcessor{ctx: ctx}).collapseWideMachineStorePair(low, high)
	if !changed {
		t.Fatal("masked wide integer storage pair did not collapse")
	}
	want := "((load(dword [bp-0xa]) & 0xfffffffe) | 0x0)"
	if got.Src.String() != want {
		t.Fatalf("collapsed source = %q, want %q", got.Src.String(), want)
	}
}
