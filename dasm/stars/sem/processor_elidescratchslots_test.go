package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestElideScratchSlotsInlinesAliasesWithinBlock(t *testing.T) {
	slot18 := scratchMemory(-0x18, 2)
	slot16 := scratchMemory(-0x16, 2)
	lpflBase := &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: &typeinfo.FunctionVar{Name: "lpfl", Type: typeinfo.U16}}}
	lpflSeg := &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: &typeinfo.FunctionVar{Name: "lpfl_seg", Type: typeinfo.U16}}}

	block := Block{
		ID: 0x9000,
		Effects: []Effect{
			&Assign{Dst: slot18, Src: lpflBase},
			&Assign{Dst: slot16, Src: lpflSeg},
			&Branch{
				Cond: &Compare{
					Op:  CompareNE,
					LHS: &Memory{Seg: slot16, Base: slot18, Disp: 4, Width: 2, TypeInfo: typeinfo.U16},
					RHS: &Const{TypeInfo: typeinfo.U16, U64: 1},
				},
				TrueBlock:  0x9010,
				FalseBlock: 0x9020,
			},
		},
	}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("changed = false, want true")
	}
	if len(got.Effects) != 1 {
		t.Fatalf("effects = %#v, want one branch", got.Effects)
	}
	gotBranch := FormatEffect(got.Effects[0])
	wantBranch := "branch lpfl_seg:[lpfl+0x4] != 0x1 ? L_9010 : L_9020"
	if gotBranch != wantBranch {
		t.Fatalf("branch = %q, want %q", gotBranch, wantBranch)
	}
}

func TestElideScratchSlotsStopsAtAssignBarrier(t *testing.T) {
	slot := scratchMemory(-0x10, 2)
	n := &Local{FunctionVar: typeinfo.FunctionVar{Name: "n", Type: typeinfo.U16, BPOffset: 6}}
	block := Block{
		ID: 0x1000,
		Effects: []Effect{
			&Assign{Dst: slot, Src: n},
			&Assign{Dst: n, Src: &Const{TypeInfo: typeinfo.U16, U64: 0}},
			&Branch{
				Cond:       &Compare{Op: CompareNE, LHS: slot, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0}},
				TrueBlock:  0x1010,
				FalseBlock: 0x1020,
			},
		},
	}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if changed {
		t.Fatal("changed = true, want false")
	}
	if len(got.Effects) != 3 {
		t.Fatalf("effects = %#v, want three preserved effects", got.Effects)
	}
	gotFirst := FormatEffect(got.Effects[0])
	wantFirst := "ss:[bp-0x10] = n"
	if gotFirst != wantFirst {
		t.Fatalf("first effect = %q, want %q", gotFirst, wantFirst)
	}
	gotBranch := FormatEffect(got.Effects[2])
	wantBranch := "branch ss:[bp-0x10] != 0x0 ? L_1010 : L_1020"
	if gotBranch != wantBranch {
		t.Fatalf("branch = %q, want %q", gotBranch, wantBranch)
	}
}

func TestElideScratchSlotsInlinesAliasesAcrossWideStorePair(t *testing.T) {
	slot14 := scratchMemory(-0x14, 2)
	slot12 := scratchMemory(-0x12, 2)
	thingType := &typeinfo.Struct{Name: "THING", Size: 0x12}
	farThingType := &typeinfo.Pointer{Elem: thingType, Class: typeinfo.PtrFar}
	lpThings := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "lpThings", Type: farThingType}}
	cThing := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "cThing", Type: typeinfo.U16}}
	lpthMac := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpthMac", Type: farThingType, BPOffset: -0x10}}

	block := Block{
		ID: 0x2606,
		Effects: []Effect{
			&Assign{Dst: slot14, Src: &Part{Base: lpThings, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}},
			&Assign{Dst: slot12, Src: &Part{Base: lpThings, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}},
			&Assign{
				Dst: &Part{Base: lpthMac, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
				Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: slot14, RHS: &Word{
					Part: machine.WordLow,
					Parent: &Binary{
						TypeInfo: typeinfo.U16,
						Op:       OpMul,
						LHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x12},
						RHS:      cThing,
					},
				}},
			},
			&Assign{
				Dst: &Part{Base: lpthMac, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
				Src: slot12,
			},
		},
	}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("changed = false, want true")
	}
	if len(got.Effects) != 2 {
		t.Fatalf("effects = %#v, want two wide store effects", got.Effects)
	}
	gotLow := FormatEffect(got.Effects[0])
	wantLow := "LOWORD(lpthMac) = (LOWORD(lpThings) + loword((0x12 * cThing)))"
	if gotLow != wantLow {
		t.Fatalf("low effect = %q, want %q", gotLow, wantLow)
	}
	gotHigh := FormatEffect(got.Effects[1])
	wantHigh := "HIWORD(lpthMac) = HIWORD(lpThings)"
	if gotHigh != wantHigh {
		t.Fatalf("high effect = %q, want %q", gotHigh, wantHigh)
	}
}

// TestElideScratchSlotsInlinesAliasesAcrossCallWhenUsedLater verifies scratch
// aliases can feed a later use when a call separates the alias stores and reads.
func TestElideScratchSlotsInlinesAliasesAcrossCallWhenUsedLater(t *testing.T) {
	slot0e := scratchMemory(-0x0e, 2)
	slot0c := scratchMemory(-0x0c, 2)
	bitmapHeaderType := &typeinfo.Struct{Name: "BITMAPINFOHEADER", Size: 0x28}
	farBitmapHeaderType := &typeinfo.Pointer{Elem: bitmapHeaderType, Class: typeinfo.PtrFar}
	lpbi := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpbi", Type: farBitmapHeaderType, BPOffset: -0x0a}}
	pBuf := &Local{FunctionVar: typeinfo.FunctionVar{Name: "pBuf", Type: &typeinfo.Pointer{Elem: typeinfo.U8, Class: typeinfo.PtrFar}, BPOffset: -0x06}}
	paletteSize := &typeinfo.Function{Name: "PaletteSize", Ret: typeinfo.U16, Params: []typeinfo.FunctionVar{{Name: "lpbi", Type: farBitmapHeaderType}}}
	result := &CallResult{Function: paletteSize, TypeInfo: typeinfo.U16, InstOff: 0x4e2d}

	block := Block{
		ID: 0x4e0f,
		Effects: []Effect{
			&Assign{Dst: slot0e, Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Part{Base: lpbi, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, RHS: &Part{Base: lpbi, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}}},
			&Assign{Dst: slot0c, Src: &Part{Base: lpbi, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}},
			&CallEffect{Call: &Call{Function: paletteSize, Args: []Expr{lpbi}}, Result: result},
			&Assign{
				Dst: &Part{Base: pBuf, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
				Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: slot0e, RHS: result},
			},
			&Assign{Dst: &Part{Base: pBuf, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, Src: slot0c},
		},
	}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("changed = false, want true")
	}
	if len(got.Effects) != 3 {
		t.Fatalf("effects = %#v, want call plus two wide store effects", got.Effects)
	}
	gotLow := FormatEffect(got.Effects[1])
	wantLow := "LOWORD(pBuf) = ((LOWORD(lpbi) + LOWORD(lpbi)) + callresult(uint16_t))"
	if gotLow != wantLow {
		t.Fatalf("low effect = %q, want %q", gotLow, wantLow)
	}
	gotHigh := FormatEffect(got.Effects[2])
	wantHigh := "HIWORD(pBuf) = HIWORD(lpbi)"
	if gotHigh != wantHigh {
		t.Fatalf("high effect = %q, want %q", gotHigh, wantHigh)
	}
}

// TestElideScratchSlotsInlinesAliasesAcrossStraightLineBlocks verifies a
// scratch-only predecessor can feed a single-successor block.
func TestElideScratchSlotsInlinesAliasesAcrossStraightLineBlocks(t *testing.T) {
	slot06 := scratchMemory(-0x06, 2)
	slot04 := scratchMemory(-0x04, 2)
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	playerType := &typeinfo.Struct{Name: "PLAYER", Size: 0xc0}
	farPlayerType := &typeinfo.Pointer{Elem: playerType, Class: typeinfo.PtrFar}
	idAi := &Local{FunctionVar: typeinfo.FunctionVar{Name: "idAi", Type: int16Type, BPOffset: 0x6}}
	lvlAi := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lvlAi", Type: int16Type, BPOffset: 0x8}}
	fn := &Func{Blocks: []Block{
		{
			ID: 0xb570,
			Effects: []Effect{
				&Assign{Dst: slot06, Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Const{TypeInfo: typeinfo.U16, U64: 0xa370}, RHS: &Word{
					Part: machine.WordLow,
					Parent: &Binary{
						TypeInfo: typeinfo.U16,
						Op:       OpMul,
						LHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x300},
						RHS:      idAi,
					},
				}}},
				&Assign{Dst: slot04, Src: &Const{TypeInfo: typeinfo.U16, U64: 0x10}},
			},
		},
		{
			ID: 0xb5a1,
			Effects: []Effect{
				&Return{Value: &FarPointer{
					Part:    machine.FarPointerWhole,
					Segment: slot04,
					Offset: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: slot06, RHS: &Word{
						Part: machine.WordLow,
						Parent: &Binary{
							TypeInfo: typeinfo.U16,
							Op:       OpMul,
							LHS:      &Const{TypeInfo: typeinfo.U16, U64: 0xc0},
							RHS:      lvlAi,
						},
					}},
					TypeInfo: farPlayerType,
				}},
			},
		},
	}}

	changed := (&elideScratchSlotsProcessor{}).ProcessFunc(nil, fn)
	if !changed {
		t.Fatal("changed = false, want true")
	}
	if len(fn.Blocks[0].Effects) != 0 {
		t.Fatalf("predecessor effects = %#v, want empty", fn.Blocks[0].Effects)
	}
	got := FormatEffect(fn.Blocks[1].Effects[0])
	want := "return farptr(0x10, ((0xa370 + loword((0x300 * idAi))) + loword((0xc0 * lvlAi))))"
	if got != want {
		t.Fatalf("return = %q, want %q", got, want)
	}
}

// TestElideScratchSlotsInlinesTrailingAliasesAcrossStraightLineBlocks verifies
// tail scratch stores in a non-scratch predecessor can feed its successor.
func TestElideScratchSlotsInlinesTrailingAliasesAcrossStraightLineBlocks(t *testing.T) {
	slot0e := scratchMemory(-0x0e, 2)
	slot0c := scratchMemory(-0x0c, 2)
	dx := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dx", Type: typeinfo.I32, BPOffset: -0x0a}}
	dySquared := &Binary{TypeInfo: typeinfo.U32, Op: OpMul, LHS: &Local{FunctionVar: typeinfo.FunctionVar{Name: "dy", Type: typeinfo.I32, BPOffset: -0x06}}, RHS: &Local{FunctionVar: typeinfo.FunctionVar{Name: "dy", Type: typeinfo.I32, BPOffset: -0x06}}}
	dxSquared := &Binary{TypeInfo: typeinfo.U32, Op: OpMul, LHS: dx, RHS: dx}
	fn := &Func{Blocks: []Block{
		{
			ID: 0x685c,
			Effects: []Effect{
				&Assign{Dst: dx, Src: &Const{TypeInfo: typeinfo.I32, U64: 3}},
				&Assign{Dst: slot0e, Src: &Word{Parent: dySquared, Part: machine.WordLow}},
				&Assign{Dst: slot0c, Src: &Word{Parent: dySquared, Part: machine.WordHigh}},
			},
		},
		{
			ID: 0x68b4,
			Effects: []Effect{
				&Return{Value: &Words{Words: []Expr{
					&Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Word{Parent: dxSquared, Part: machine.WordLow}, RHS: slot0e},
					&Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Word{Parent: dxSquared, Part: machine.WordHigh}, RHS: slot0c},
				}}},
			},
		},
	}}

	changed := (&elideScratchSlotsProcessor{}).ProcessFunc(nil, fn)
	if !changed {
		t.Fatal("changed = false, want true")
	}
	if len(fn.Blocks[0].Effects) != 1 {
		t.Fatalf("predecessor effects = %#v, want one non-scratch effect", fn.Blocks[0].Effects)
	}
	got := FormatEffect(fn.Blocks[1].Effects[0])
	want := "return words((loword((dx * dx)) + loword((dy * dy))), (hiword((dx * dx)) + hiword((dy * dy))))"
	if got != want {
		t.Fatalf("return = %q, want %q", got, want)
	}
}

// TestElideScratchSlotsPreservesUnusedAliasBeforeCall verifies unused scratch
// stores are not moved after an intervening call.
func TestElideScratchSlotsPreservesUnusedAliasBeforeCall(t *testing.T) {
	slot := scratchMemory(-0x10, 2)
	paletteSize := &typeinfo.Function{Name: "PaletteSize", Ret: typeinfo.U16}
	block := Block{
		ID: 0x4000,
		Effects: []Effect{
			&Assign{Dst: slot, Src: &Const{TypeInfo: typeinfo.U16, U64: 7}},
			&CallEffect{Call: &Call{Function: paletteSize}},
		},
	}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if changed {
		t.Fatal("changed = true, want false")
	}
	if len(got.Effects) != 2 {
		t.Fatalf("effects = %#v, want preserved scratch assignment and call", got.Effects)
	}
	gotFirst := FormatEffect(got.Effects[0])
	wantFirst := "ss:[bp-0x10] = 0x7"
	if gotFirst != wantFirst {
		t.Fatalf("first effect = %q, want %q", gotFirst, wantFirst)
	}
}

// TestElideScratchSlotsDoesNotInlineCallResultAliasAcrossCall verifies saved
// call results are not duplicated across another call.
func TestElideScratchSlotsDoesNotInlineCallResultAliasAcrossCall(t *testing.T) {
	slot := scratchMemory(-0x10, 2)
	random := &typeinfo.Function{Name: "Random", Ret: typeinfo.U16, Params: []typeinfo.FunctionVar{{Name: "n", Type: typeinfo.U16}}}
	result := &CallResult{Function: random, TypeInfo: typeinfo.U16, InstOff: 0x2000}
	n := &Local{FunctionVar: typeinfo.FunctionVar{Name: "n", Type: typeinfo.U16, BPOffset: -0x4}}
	block := Block{
		ID: 0x4000,
		Effects: []Effect{
			&Assign{Dst: slot, Src: result},
			&CallEffect{Call: &Call{Function: random, Args: []Expr{&Const{TypeInfo: typeinfo.U16, U64: 0x32}}}, Result: result},
			&Assign{Dst: n, Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: slot, RHS: result}},
		},
	}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if changed {
		t.Fatal("changed = true, want false")
	}
	if len(got.Effects) != 3 {
		t.Fatalf("effects = %#v, want preserved scratch assignment, call, and use", got.Effects)
	}
	gotFirst := FormatEffect(got.Effects[0])
	wantFirst := "ss:[bp-0x10] = callresult(uint16_t)"
	if gotFirst != wantFirst {
		t.Fatalf("first effect = %q, want %q", gotFirst, wantFirst)
	}
}

func TestElideScratchSlotsDropsUnusedAliasesBeforeTerminator(t *testing.T) {
	usedSlot := scratchMemory(-0x18, 2)
	unusedSlot := scratchMemory(-0x1a, 2)
	lpflBase := &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: &typeinfo.FunctionVar{Name: "lpfl", Type: typeinfo.U16}}}
	block := Block{
		ID: 0x9000,
		Effects: []Effect{
			&Assign{Dst: usedSlot, Src: lpflBase},
			&Assign{Dst: unusedSlot, Src: &Const{TypeInfo: typeinfo.U16, U64: 7}},
			&Branch{
				Cond:       &Compare{Op: CompareNE, LHS: usedSlot, RHS: &Const{TypeInfo: typeinfo.U16, U64: 1}},
				TrueBlock:  0x9010,
				FalseBlock: 0x9020,
			},
		},
	}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("changed = false, want true")
	}
	if len(got.Effects) != 1 {
		t.Fatalf("effects = %#v, want one branch", got.Effects)
	}
	gotBranch := FormatEffect(got.Effects[0])
	wantBranch := "branch lpfl != 0x1 ? L_9010 : L_9020"
	if gotBranch != wantBranch {
		t.Fatalf("branch = %q, want %q", gotBranch, wantBranch)
	}
}

func TestElideScratchSlotsInlinesSignedDwordFloatScratchAcrossSqrt(t *testing.T) {
	slot30 := scratchMemory(-0x30, 2)
	slot2e := scratchMemory(-0x2e, 2)
	slot3a := scratchMemory(-0x3a, 10)
	slot42 := scratchMemory(-0x42, 2)
	slot40 := scratchMemory(-0x40, 2)
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	rgrad := &Local{FunctionVar: typeinfo.FunctionVar{Name: "rgrad", Type: int16Type}}
	rad := &Local{FunctionVar: typeinfo.FunctionVar{Name: "rad", Type: int16Type}}
	l := &Local{FunctionVar: typeinfo.FunctionVar{Name: "l", Type: typeinfo.I32}}
	sqrtFn := &typeinfo.Function{Name: "sqrt", Ret: typeinfo.Double, Params: []typeinfo.FunctionVar{{Type: typeinfo.Double}}}
	sqrtResult := &CallResult{Function: sqrtFn, TypeInfo: typeinfo.Double, InstOff: 0x5238}

	block := Block{Effects: []Effect{
		&Assign{Dst: slot30, Src: rgrad},
		&Assign{Dst: slot2e, Src: &Word{Parent: rgrad, Part: machine.WordSignHigh}},
		&Assign{Dst: slot3a, Src: &Cast{Value: scratchMemory(-0x30, 4), To: "double", TypeInfo: typeinfo.Double}},
		&CallEffect{Call: &Call{Function: sqrtFn, Args: []Expr{&Cast{Value: l, To: "double", TypeInfo: typeinfo.Double}}}, Result: sqrtResult},
		&Assign{Dst: slot42, Src: rad},
		&Assign{Dst: slot40, Src: &Word{Parent: rad, Part: machine.WordSignHigh}},
		&Branch{
			Cond: &Compare{
				Op:  CompareLE,
				LHS: &Binary{TypeInfo: typeinfo.Double, Op: OpAdd, LHS: sqrtResult, RHS: &Cast{Value: scratchMemory(-0x42, 4), To: "double", TypeInfo: typeinfo.Double}},
				RHS: slot3a,
			},
			TrueBlock:  0x5405,
			FalseBlock: 0x5266,
		},
	}}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(got.Effects) != 2 {
		t.Fatalf("effects = %#v, want call and branch", got.Effects)
	}
	gotBranch := FormatEffect(got.Effects[1])
	wantBranch := "branch (callresult(double) + (double)sext16to32(rad)) <= (double)sext16to32(rgrad) ? L_5405 : L_5266"
	if gotBranch != wantBranch {
		t.Fatalf("branch = %q, want %q", gotBranch, wantBranch)
	}
}

func TestElideScratchSlotsInlinesConstDwordFloatScratchInSqrtArg(t *testing.T) {
	slot10c := scratchMemory(-0x10c, 2)
	slot10a := scratchMemory(-0x10a, 2)
	sqrtFn := &typeinfo.Function{Name: "sqrt", Ret: typeinfo.Double, Params: []typeinfo.FunctionVar{{Type: typeinfo.Double}}}

	block := Block{Effects: []Effect{
		&Assign{Dst: slot10c, Src: &Const{TypeInfo: typeinfo.U16, U64: 1}},
		&Assign{Dst: slot10a, Src: &Const{TypeInfo: typeinfo.U16, U64: 0}},
		&CallEffect{
			Call: &Call{
				Function: sqrtFn,
				Args: []Expr{
					&Binary{
						TypeInfo: typeinfo.Double,
						Op:       OpAdd,
						LHS:      &Local{FunctionVar: typeinfo.FunctionVar{Name: "m2", Type: typeinfo.Double}},
						RHS:      &Cast{Value: scratchMemory(-0x10c, 4), To: "double", TypeInfo: typeinfo.Double},
					},
				},
			},
			Result: &CallResult{Function: sqrtFn, TypeInfo: typeinfo.Double, InstOff: 0x57ff},
		},
	}}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(got.Effects) != 1 {
		t.Fatalf("effects = %#v, want call", got.Effects)
	}
	gotCall := FormatEffect(got.Effects[0])
	wantCall := "call sqrt((m2 + (double)1)) -> callresult(double)"
	if gotCall != wantCall {
		t.Fatalf("call = %q, want %q", gotCall, wantCall)
	}
}

// scratchMemory creates an unresolved BP-relative semantic stack slot.
func scratchMemory(disp int, width int) *Memory {
	return &Memory{
		Seg:      &Register{Val: asm.RegSS},
		Base:     &RawValue{Value: machine.FrameBaseVal(), TypeInfo: typeinfo.U16},
		Disp:     disp,
		Width:    width,
		TypeInfo: intTypeForWidth(width),
	}
}
