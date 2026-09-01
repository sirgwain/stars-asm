package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCoalesceWideTempsCollapsesFarPointerTempHalves verifies far pointer temp pairs collapse.
func TestCoalesceWideTempsCollapsesFarPointerTempHalves(t *testing.T) {
	planetType := &typeinfo.Struct{Name: "PLANET", Size: 0x38}
	planetPtrType := &typeinfo.Pointer{Elem: planetType, Class: typeinfo.PtrFar}
	lpfl := testLocal("lpfl", &typeinfo.Pointer{Elem: &typeinfo.Struct{Name: "FLEET"}, Class: typeinfo.PtrFar})
	lppl := testLocal("lppl", planetPtrType)
	lpplHome := testLocal("lpplHome", planetPtrType)
	lo := &Temp{Name: "t_merge_160c_0001", TypeInfo: typeinfo.U16}
	hi := &Temp{Name: "t_merge_160c_0002", TypeInfo: typeinfo.U16}
	target := &typeinfo.Function{Name: "IdTargetFreighter", Ret: typeinfo.U16, Params: []typeinfo.FunctionVar{
		{Name: "lpfl", Type: lpfl.ExprType()},
		{Name: "lppl", Type: planetPtrType},
	}}

	fn := &Func{Blocks: []Block{
		{
			ID: 0x15fd,
			Effects: []Effect{
				&Assign{Dst: lo, Src: &FarPointer{Parent: lppl, Part: machine.FarPointerOffset, TypeInfo: typeinfo.U16}},
				&Assign{Dst: hi, Src: &FarPointer{Parent: lppl, Part: machine.FarPointerSegment, TypeInfo: typeinfo.U16}},
				&Jump{To: 0x160c},
			},
		},
		{
			ID: 0x1606,
			Effects: []Effect{
				&Assign{Dst: lo, Src: &FarPointer{Parent: lpplHome, Part: machine.FarPointerOffset, TypeInfo: typeinfo.U16}},
				&Assign{Dst: hi, Src: &FarPointer{Parent: lpplHome, Part: machine.FarPointerSegment, TypeInfo: typeinfo.U16}},
			},
		},
		{
			ID: 0x160c,
			Effects: []Effect{
				&CallEffect{Call: &Call{Function: target, Args: []Expr{
					lpfl,
					&FarPointer{Part: machine.FarPointerWhole, Segment: hi, Offset: lo, TypeInfo: planetPtrType},
				}}},
			},
		},
	}}

	if changed := (&coalesceWideTempsProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := formatEffects(fn.Blocks[0].Effects), []string{"t_merge_160c_0001_wide = lppl", "goto L_160c"}; !equalStrings(got, want) {
		t.Fatalf("block 15fd effects = %#v, want %#v", got, want)
	}
	if got, want := formatEffects(fn.Blocks[1].Effects), []string{"t_merge_160c_0001_wide = lpplHome"}; !equalStrings(got, want) {
		t.Fatalf("block 1606 effects = %#v, want %#v", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[2].Effects[0]), "call IdTargetFreighter(lpfl, t_merge_160c_0001_wide)"; got != want {
		t.Fatalf("call = %q, want %q", got, want)
	}
}

// TestCoalesceWideTempsInfersCStringPointerFromSegmentOffsetTemps verifies
// SS:array and DS:literal word pairs coalesce into one far C string pointer
// temp.
func TestCoalesceWideTempsInfersCStringPointerFromSegmentOffsetTemps(t *testing.T) {
	fx := testfixture.Stars(t)
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	szT := &Local{FunctionVar: typeinfo.FunctionVar{Name: "szT", Type: &typeinfo.Array{Elem: charType, Count: 96}, BPOffset: -0x190}}
	lo := &Temp{Name: "t_merge_2766_0001", TypeInfo: typeinfo.U16}
	hi := &Temp{Name: "t_merge_2766_0002", TypeInfo: typeinfo.U16}
	target := &typeinfo.Function{Name: "_wsprintf", Ret: typeinfo.U16, VarArgs: true, Params: []typeinfo.FunctionVar{
		{Name: "lpszout", Type: farCharPtrType},
		{Name: "lpszfmt", Type: farCharPtrType},
	}}
	fn := &Func{Blocks: []Block{
		{
			ID: 0x2746,
			Effects: []Effect{
				&Assign{Dst: lo, Src: szT},
				&Assign{Dst: hi, Src: &Register{Val: asm.RegSS}},
				&Jump{To: 0x2766},
			},
		},
		{
			ID: 0x2759,
			Effects: []Effect{
				&Assign{Dst: lo, Src: &Const{TypeInfo: typeinfo.U16, U64: 0x1420}},
				&Assign{Dst: hi, Src: &Register{Val: asm.RegDS, SegNum: 0x25}},
				&Jump{To: 0x2766},
			},
		},
		{
			ID: 0x2761,
			Effects: []Effect{
				&Assign{Dst: lo, Src: &Const{TypeInfo: typeinfo.U16, U64: 0x1422}},
				&Assign{Dst: hi, Src: &Register{Val: asm.RegDS, SegNum: 0x25}},
			},
		},
		{
			ID: 0x2766,
			Effects: []Effect{
				&CallEffect{Call: &Call{Function: target, Args: []Expr{
					&Global{GlobalVar: &typeinfo.GlobalVar{Name: "szWork", Type: farCharPtrType}},
					&Global{GlobalVar: &typeinfo.GlobalVar{Name: "pszFmt", Type: farCharPtrType}},
					&FarPointer{Part: machine.FarPointerWhole, Segment: hi, Offset: lo, TypeInfo: farCharPtrType},
				}}},
			},
		},
	}}
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("DrawVCR"))

	if changed := (&coalesceWideTempsProcessor{ctx: ctx}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := formatEffects(fn.Blocks[0].Effects), []string{"t_merge_2766_0001_wide = szT", "goto L_2766"}; !equalStrings(got, want) {
		t.Fatalf("block 2746 effects = %#v, want %#v", got, want)
	}
	if got, want := formatEffects(fn.Blocks[1].Effects), []string{"t_merge_2766_0001_wide = \",\"", "goto L_2766"}; !equalStrings(got, want) {
		t.Fatalf("block 2759 effects = %#v, want %#v", got, want)
	}
	if got, want := formatEffects(fn.Blocks[2].Effects), []string{"t_merge_2766_0001_wide = \".\""}; !equalStrings(got, want) {
		t.Fatalf("block 2761 effects = %#v, want %#v", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[3].Effects[0]), "call _wsprintf(szWork, pszFmt, t_merge_2766_0001_wide)"; got != want {
		t.Fatalf("call = %q, want %q", got, want)
	}
}

// TestCoalesceWideTempsPrefersFarPointerTypeOverNullArms verifies null paths keep pointer type.
func TestCoalesceWideTempsPrefersFarPointerTypeOverNullArms(t *testing.T) {
	btlDataType := &typeinfo.Struct{Name: "BTLDATA", Size: 0x20}
	btlDataPtrType := &typeinfo.Pointer{Elem: btlDataType, Class: typeinfo.PtrFar}
	lpbd := testLocal("lpbd", btlDataPtrType)
	lo := &Temp{Name: "t_merge_0449_0001", TypeInfo: typeinfo.U16}
	hi := &Temp{Name: "t_merge_0449_0002", TypeInfo: typeinfo.U16}

	fn := &Func{Blocks: []Block{
		{
			ID: 0x038a,
			Effects: []Effect{
				&Assign{Dst: lo, Src: &Const{TypeInfo: typeinfo.U16, U64: 0x0}},
				&Assign{Dst: hi, Src: &Const{TypeInfo: typeinfo.U16, U64: 0x0}},
				&Jump{To: 0x0449},
			},
		},
		{
			ID: 0x03e3,
			Effects: []Effect{
				&Assign{Dst: lo, Src: &Const{TypeInfo: typeinfo.U16, U64: 0x0}},
				&Assign{Dst: hi, Src: &Const{TypeInfo: typeinfo.U16, U64: 0x0}},
				&Jump{To: 0x0449},
			},
		},
		{
			ID: 0x0428,
			Effects: []Effect{
				&Assign{Dst: lo, Src: &FarPointer{Parent: lpbd, Part: machine.FarPointerOffset, TypeInfo: typeinfo.U16}},
				&Assign{Dst: hi, Src: &FarPointer{Parent: lpbd, Part: machine.FarPointerSegment, TypeInfo: typeinfo.U16}},
				&Jump{To: 0x0449},
			},
		},
		{
			ID: 0x0449,
			Effects: []Effect{
				&Return{Value: &FarPointer{
					TypeInfo: btlDataPtrType,
					Part:     machine.FarPointerWhole,
					Segment:  hi,
					Offset:   lo,
				}},
			},
		},
	}}

	if changed := (&coalesceWideTempsProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := formatEffects(fn.Blocks[0].Effects), []string{"t_merge_0449_0001_wide = 0x0", "goto L_0449"}; !equalStrings(got, want) {
		t.Fatalf("block 038a effects = %#v, want %#v", got, want)
	}
	if got, want := formatEffects(fn.Blocks[2].Effects), []string{"t_merge_0449_0001_wide = lpbd", "goto L_0449"}; !equalStrings(got, want) {
		t.Fatalf("block 0428 effects = %#v, want %#v", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[3].Effects[0]), "return t_merge_0449_0001_wide"; got != want {
		t.Fatalf("return = %q, want %q", got, want)
	}
	if got, want := fn.Blocks[0].Effects[0].(*Assign).Dst.ExprType(), btlDataPtrType; got != want {
		t.Fatalf("wide temp type = %v, want %v", got, want)
	}
}

// TestCoalesceWideTempsCollapsesDuplicateHighWordTemps verifies repeated high parts share one wide temp.
func TestCoalesceWideTempsCollapsesDuplicateHighWordTemps(t *testing.T) {
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	dChg := testLocal("dChg", int32Type)
	getFuel := &typeinfo.Function{Name: "GetFuelFree", Ret: int32Type}
	getCargo := &typeinfo.Function{Name: "GetCargoFree", Ret: int32Type}
	fuelResult := &CallResult{Function: getFuel, TypeInfo: int32Type, InstOff: 0x640a}
	cargoResult := &CallResult{Function: getCargo, TypeInfo: int32Type, InstOff: 0x641d}
	hi1 := &Temp{Name: "t_merge_6425_0001", TypeInfo: typeinfo.U16}
	hi2 := &Temp{Name: "t_merge_6425_0002", TypeInfo: typeinfo.U16}
	lo := &Temp{Name: "t_merge_6425_0003", TypeInfo: typeinfo.U16}

	fn := &Func{Blocks: []Block{
		{
			ID: 0x6402,
			Effects: []Effect{
				&CallEffect{Call: &Call{Function: getFuel}, Result: fuelResult},
				&Assign{Dst: hi1, Src: &Word{Parent: fuelResult, Part: machine.WordHigh}},
				&Assign{Dst: hi2, Src: &Word{Parent: fuelResult, Part: machine.WordHigh}},
				&Assign{Dst: lo, Src: &Word{Parent: fuelResult, Part: machine.WordLow}},
				&Jump{To: 0x6425},
			},
		},
		{
			ID: 0x6415,
			Effects: []Effect{
				&CallEffect{Call: &Call{Function: getCargo}, Result: cargoResult},
				&Assign{Dst: hi1, Src: &Word{Parent: cargoResult, Part: machine.WordHigh}},
				&Assign{Dst: hi2, Src: &Word{Parent: cargoResult, Part: machine.WordHigh}},
				&Assign{Dst: lo, Src: &Word{Parent: cargoResult, Part: machine.WordLow}},
			},
		},
		{
			ID: 0x6425,
			Effects: []Effect{
				&Branch{
					Cond:       &Compare{Op: CompareGT, LHS: &Part{Base: dChg, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, RHS: hi1},
					TrueBlock:  0x6443,
					FalseBlock: 0x642d,
				},
			},
		},
		{
			ID: 0x642d,
			Effects: []Effect{
				&Branch{
					Cond:       &Compare{Op: CompareLT, LHS: &Part{Base: dChg, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, RHS: hi2},
					TrueBlock:  0x643a,
					FalseBlock: 0x6432,
				},
			},
		},
		{
			ID: 0x6432,
			Effects: []Effect{
				&Branch{
					Cond:       &Compare{Op: CompareGE, LHS: &Part{Base: dChg, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, RHS: lo},
					TrueBlock:  0x6443,
					FalseBlock: 0x643a,
				},
			},
		},
	}}

	if changed := (&coalesceWideTempsProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := formatEffects(fn.Blocks[0].Effects), []string{"call GetFuelFree() -> callresult(int32_t)", "t_merge_6425_0003_wide = callresult(int32_t)", "goto L_6425"}; !equalStrings(got, want) {
		t.Fatalf("block 6402 effects = %#v, want %#v", got, want)
	}
	if got, want := formatEffects(fn.Blocks[1].Effects), []string{"call GetCargoFree() -> callresult(int32_t)", "t_merge_6425_0003_wide = callresult(int32_t)"}; !equalStrings(got, want) {
		t.Fatalf("block 6415 effects = %#v, want %#v", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[2].Effects[0]), "branch HIWORD(dChg) > hiword(t_merge_6425_0003_wide) ? L_6443 : L_642d"; got != want {
		t.Fatalf("first branch = %q, want %q", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[3].Effects[0]), "branch HIWORD(dChg) < hiword(t_merge_6425_0003_wide) ? L_643a : L_6432"; got != want {
		t.Fatalf("second branch = %q, want %q", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[4].Effects[0]), "branch LOWORD(dChg) >= loword(t_merge_6425_0003_wide) ? L_6443 : L_643a"; got != want {
		t.Fatalf("third branch = %q, want %q", got, want)
	}
}

// TestCoalesceWideTempsRebuildsExpressionDefinitions preserves all incoming paths.
func TestCoalesceWideTempsRebuildsExpressionDefinitions(t *testing.T) {
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	pctT := testLocal("pctT", int32Type)
	pctTooBig := testLocal("pctTooBig", int32Type)
	lo := &Temp{Name: "t_merge_126a_0001", TypeInfo: typeinfo.U16}
	hi := &Temp{Name: "t_merge_126a_0002", TypeInfo: typeinfo.U16}

	fn := &Func{Blocks: []Block{
		{
			ID: 0x1255,
			Effects: []Effect{
				&Assign{Dst: lo, Src: &Part{Base: pctT, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}},
				&Assign{Dst: hi, Src: &Part{Base: pctT, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}},
				&Jump{To: 0x126a},
			},
		},
		{
			ID: 0x125e,
			Effects: []Effect{
				&Assign{
					Dst: lo,
					Src: &Binary{
						TypeInfo: typeinfo.U16,
						Op:       OpAdd,
						LHS:      &Part{Base: pctTooBig, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
						RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0xffff},
					},
				},
				&Assign{
					Dst: hi,
					Src: &Binary{
						TypeInfo: typeinfo.U16,
						Op:       OpAdd,
						LHS:      &Part{Base: pctTooBig, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
						RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0xffff},
					},
				},
			},
		},
		{ID: 0x126a, Effects: []Effect{&Assign{Dst: pctT, Src: &Words{Words: []Expr{hi, lo}}}}},
	}}

	if changed := (&coalesceWideTempsProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if got, want := formatEffects(fn.Blocks[0].Effects), []string{"t_merge_126a_0001_wide = pctT", "goto L_126a"}; !equalStrings(got, want) {
		t.Fatalf("block 1255 effects = %#v, want %#v", got, want)
	}
	if got, want := formatEffects(fn.Blocks[1].Effects), []string{"t_merge_126a_0001_wide = (pctTooBig + 0xffffffff)"}; !equalStrings(got, want) {
		t.Fatalf("block 125e effects = %#v, want %#v", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[2].Effects[0]), "pctT = t_merge_126a_0001_wide"; got != want {
		t.Fatalf("join effect = %q, want %q", got, want)
	}
}
