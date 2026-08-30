package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCollapseCallResultsInlinesAssignments verifies direct call result
// assignments become source-level call assignments.
func TestCollapseCallResultsInlinesAssignments(t *testing.T) {
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	target := &typeinfo.Function{Name: "CalcPlanetMaxPop", Ret: int32Type}
	local := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lPopMax", Type: int32Type, BPOffset: -10}}
	result := &CallResult{Function: target, TypeInfo: int32Type, InstOff: 0x7248}
	block := Block{Effects: []Effect{
		&CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x7248},
			Call:     &Call{Function: target, Args: []Expr{&Const{TypeInfo: typeinfo.U16, U64: 0x2}}},
			Result:   result,
		},
		&Assign{Dst: local, Src: result},
	}}

	gotBlock, changed := (&collapseCallResultsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lPopMax = CalcPlanetMaxPop(0x2)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseCallResultsInlinesNestedCallArgs verifies call results can be
// folded into later nested call arguments.
func TestCollapseCallResultsInlinesNestedCallArgs(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	charPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	getString := &typeinfo.Function{Name: "PszGetCompressedString", Ret: charPtrType}
	wsprintf := &typeinfo.Function{Name: "_wsprintf", Ret: int16Type}
	result := &CallResult{Function: getString, TypeInfo: charPtrType, InstOff: 0x1212}
	block := Block{Effects: []Effect{
		&CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x1212},
			Call:     &Call{Function: getString, Args: []Expr{&Const{TypeInfo: typeinfo.U16, U64: 0x105}}},
			Result:   result,
		},
		&CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x1220},
			Call: &Call{Function: wsprintf, Args: []Expr{
				&Global{GlobalVar: &typeinfo.GlobalVar{Name: "szWork", Type: charPtrType}},
				&FarPointer{
					Part:    machine.FarPointerWhole,
					Segment: &Const{TypeInfo: typeinfo.U16, U64: 0x25},
					Offset:  result,
				},
			}},
			Result: &CallResult{Function: wsprintf, TypeInfo: int16Type, InstOff: 0x1220},
		},
	}}

	gotBlock, changed := (&collapseCallResultsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "call _wsprintf(szWork, farptr(0x25, PszGetCompressedString(0x105))) -> callresult(int16_t)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseCallResultsCollapsesDefaultDataFarPointerArg verifies far pointer
// artifacts created by call-result inlining are collapsed with call param types.
func TestCollapseCallResultsCollapsesDefaultDataFarPointerArg(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	hwndType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "HWND", Size: 2, Signed: false}
	voidType := &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"}
	getVersion := &typeinfo.Function{Name: "SzVersion", Ret: farCharPtrType}
	setWindowText := &typeinfo.Function{
		Name: "SetWindowText",
		Ret:  voidType,
		Params: []typeinfo.FunctionVar{
			{Name: "arg1", Type: hwndType},
			{Name: "arg2", Type: farCharPtrType},
		},
	}
	hwnd := &Local{FunctionVar: typeinfo.FunctionVar{Name: "hwnd", Type: hwndType, BPOffset: 0xe}}
	result := &CallResult{Function: getVersion, TypeInfo: farCharPtrType, InstOff: 0x127c}
	block := Block{Effects: []Effect{
		&CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x127c},
			Call:     &Call{Function: getVersion},
			Result:   result,
		},
		&CallEffect{
			MetaInfo: machine.Meta{InstOff: 0x1281},
			Call: &Call{Function: setWindowText, Args: []Expr{
				hwnd,
				&FarPointer{
					Part:    machine.FarPointerWhole,
					Segment: &Const{TypeInfo: typeinfo.U16, U64: 0x25},
					Offset:  result,
				},
			}},
		},
	}}

	ctx := &FuncContext{dsReg: machine.ConstVal(0x25)}
	gotBlock, changed := (&collapseCallResultsProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "call SetWindowText(hwnd, SzVersion())"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseCallResultsKeepsMultiUseResults verifies reused call results are
// not duplicated by the processor.
func TestCollapseCallResultsKeepsMultiUseResults(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	target := &typeinfo.Function{Name: "GetRaceStat", Ret: int16Type}
	a := &Local{FunctionVar: typeinfo.FunctionVar{Name: "a", Type: int16Type, BPOffset: -2}}
	b := &Local{FunctionVar: typeinfo.FunctionVar{Name: "b", Type: int16Type, BPOffset: -4}}
	result := &CallResult{Function: target, TypeInfo: int16Type, InstOff: 0x72a0}
	block := Block{Effects: []Effect{
		&CallEffect{Call: &Call{Function: target}, Result: result},
		&Assign{Dst: a, Src: result},
		&Assign{Dst: b, Src: result},
	}}

	_, changed := (&collapseCallResultsProcessor{}).ProcessBlock(nil, Func{}, block)
	if changed {
		t.Fatal("ProcessBlock changed = true, want false")
	}
}
