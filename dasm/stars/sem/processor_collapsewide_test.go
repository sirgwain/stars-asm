package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCollapseWideCallArgsCollapsesWordAggregates verifies wide call arguments
// are reconstructed from their high and low word expressions.
func TestCollapseWideCallArgsCollapsesWordAggregates(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	lPopMax := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lPopMax", Type: int32Type, BPOffset: -4}}
	iEff := &Local{FunctionVar: typeinfo.FunctionVar{Name: "iEff", Type: int16Type, BPOffset: -6}}
	mulTarget := &typeinfo.Function{Name: "__aFulmul", Ret: int32Type, Params: []typeinfo.FunctionVar{
		{Name: "lhs", Type: int32Type},
		{Name: "rhs", Type: int32Type},
	}}
	divTarget := &typeinfo.Function{Name: "__aFldiv", Ret: int32Type, Params: []typeinfo.FunctionVar{
		{Name: "lhs", Type: int32Type},
		{Name: "rhs", Type: int32Type},
	}}

	block := Block{Effects: []Effect{
		&CallEffect{
			Call: &Call{Function: mulTarget, Args: []Expr{
				&Words{Words: []Expr{
					&Part{Base: lPopMax, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
					&Part{Base: lPopMax, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
				}},
				&Words{Words: []Expr{
					&Word{Parent: iEff, Part: machine.WordSignHigh},
					iEff,
				}},
			}},
		},
		&CallEffect{
			Call: &Call{Function: divTarget, Args: []Expr{
				&CallResult{Function: mulTarget, TypeInfo: int32Type, InstOff: 0x1000},
				&Words{Words: []Expr{
					&Const{TypeInfo: typeinfo.U16, U64: 0x0},
					&Const{TypeInfo: typeinfo.U16, U64: 0x64},
				}},
			}},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 2 {
		t.Fatalf("effects = %d, want 2", len(gotBlock.Effects))
	}
	wants := []string{
		"call __aFulmul(lPopMax, sext16to32(iEff))",
		"call __aFldiv(callresult(int32_t), 0x64)",
	}
	for i, want := range wants {
		got := FormatEffect(gotBlock.Effects[i])
		if got != want {
			t.Fatalf("effect[%d] = %q, want %q", i, got, want)
		}
	}
}

// TestCollapseWideStoresCollapsesConstParts verifies adjacent constant word
// stores are reconstructed into one wide assignment.
func TestCollapseWideStoresCollapsesConstParts(t *testing.T) {
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	cMax := &Local{FunctionVar: typeinfo.FunctionVar{Name: "cMax", Type: int32Type, BPOffset: -6}}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: cMax, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0x0},
		},
		&Assign{
			Dst: &Part{Base: cMax, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0x0},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "cMax = 0x0"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesBinaryExpressionParts verifies matching high
// and low word arithmetic stores are reconstructed into one wide assignment.
func TestCollapseWideStoresCollapsesBinaryExpressionParts(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	int32ArrayType := &typeinfo.Array{Elem: int32Type, Count: 4}
	i := &Local{FunctionVar: typeinfo.FunctionVar{Name: "i", Type: int16Type, BPOffset: -4}}
	rgRes := &Local{FunctionVar: typeinfo.FunctionVar{Name: "rgRes", Type: int32ArrayType, BPOffset: -0x14}}
	rgCost := &Local{FunctionVar: typeinfo.FunctionVar{Name: "rgCost", Type: int32ArrayType, BPOffset: -0x24}}
	rgCostPaid := &Local{FunctionVar: typeinfo.FunctionVar{Name: "rgCostPaid", Type: int32ArrayType, BPOffset: -0x34}}
	res := &ArrayIndex{Base: rgRes, Index: i, TypeInfo: int32Type}
	cost := &ArrayIndex{Base: rgCost, Index: i, TypeInfo: int32Type}
	paid := &ArrayIndex{Base: rgCostPaid, Index: i, TypeInfo: int32Type}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: res, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpSub,
				LHS:      &Part{Base: res, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
				RHS: &Binary{
					TypeInfo: typeinfo.U16,
					Op:       OpSub,
					LHS:      &Part{Base: cost, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
					RHS:      &Part{Base: paid, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
				},
			},
		},
		&Assign{
			Dst: &Part{Base: res, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpSub,
				LHS:      &Part{Base: res, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
				RHS: &Binary{
					TypeInfo: typeinfo.U16,
					Op:       OpSub,
					LHS:      &Part{Base: cost, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
					RHS:      &Part{Base: paid, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
				},
			},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "rgRes[i] = (rgRes[i] - (rgCost[i] - rgCostPaid[i]))"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesPairUsingPendingTemps verifies stale-load
// temps feeding a high-word source do not block a wide store collapse.
func TestCollapseWideStoresCollapsesPairUsingPendingTemps(t *testing.T) {
	uint32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "uint32_t", Size: 4}
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	lSquare := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lSquare", Type: int32Type, BPOffset: -0x20}}
	t40de := &Temp{Name: "t_40de", TypeInfo: typeinfo.U16}
	t40d8 := &Temp{Name: "t_40d8", TypeInfo: typeinfo.U16}
	product := &Cast{
		To:       "uint32_t",
		TypeInfo: uint32Type,
		Value: &Binary{
			TypeInfo: uint32Type,
			Op:       OpMul,
			LHS:      lSquare,
			RHS:      lSquare,
		},
	}
	tempProduct := &Cast{
		To:       "uint32_t",
		TypeInfo: uint32Type,
		Value: &Binary{
			TypeInfo: uint32Type,
			Op:       OpMul,
			LHS: &Words{Words: []Expr{
				&Word{Parent: lSquare, Part: machine.WordHigh},
				t40de,
			}},
			RHS: &Words{Words: []Expr{
				&Word{Parent: lSquare, Part: machine.WordHigh},
				t40d8,
			}},
		},
	}

	block := Block{Effects: []Effect{
		&Assign{Dst: t40de, Src: &Word{Parent: lSquare, Part: machine.WordLow}},
		&Assign{Dst: t40d8, Src: &Word{Parent: lSquare, Part: machine.WordLow}},
		&Assign{
			Dst: &Part{Base: lSquare, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Word{Parent: product, Part: machine.WordLow},
		},
		&Assign{
			Dst: &Part{Base: lSquare, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Word{Parent: tempProduct, Part: machine.WordHigh},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lSquare = (uint32_t)(lSquare * lSquare)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresResolvesConstFarFunctionPointer verifies constant
// far-pointer halves keep segment fixups when collapsed into a function pointer.
func TestCollapseWideStoresResolvesConstFarFunctionPointer(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("InitMDIApp"))
	fnType := &typeinfo.Function{Ret: typeinfo.I32}
	wndProcType := &typeinfo.Pointer{Elem: fnType, Class: typeinfo.PtrFar}
	lpfnWndProc := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpfnWndProc", Type: wndProcType, BPOffset: -0x1a}}
	segFixup := &asm.Fixup{
		Source:       asm.FixupSourceSegment,
		Target:       asm.FixupTargetInternalRef,
		TargetSegNum: 0x7,
		ResolvedSeg:  0x7,
		ResolvedOff:  0x5c92,
	}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: lpfnWndProc, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0x5c92},
		},
		&Assign{
			Dst: &Part{Base: lpfnWndProc, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0xffff, Fixup: segFixup},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpfnWndProc = MessageWndProc"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresResolvesConstFarGlobal verifies constant far
// char-pointer halves prefer globals after wide-store collapse.
func TestCollapseWideStoresResolvesConstFarGlobal(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("InitMDIApp"))
	charPtrType := &typeinfo.Pointer{Elem: typeinfo.U8.WithName("char"), Class: typeinfo.PtrFar}
	lpszClassName := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpszClassName", Type: charPtrType, BPOffset: -0x6}}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: lpszClassName, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0x202},
		},
		&Assign{
			Dst: &Part{Base: lpszClassName, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0x25},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpszClassName = szMessage"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesCallResultFarPointerParts verifies split
// proc-instance stores are reconstructed into one function pointer assignment.
func TestCollapseWideStoresCollapsesCallResultFarPointerParts(t *testing.T) {
	fnType := &typeinfo.Function{Ret: typeinfo.I32}
	farProcType := &typeinfo.Pointer{Elem: fnType, Class: typeinfo.PtrFar}
	makeProcInstance := &typeinfo.Function{Name: "MakeProcInstance", Ret: farProcType}
	lpProc := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpProc", Type: farProcType, BPOffset: -0xe}}
	result := &CallResult{Function: makeProcInstance, TypeInfo: farProcType, InstOff: 0x14af}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: lpProc, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &FarPointer{Parent: result, Part: machine.FarPointerOffset},
		},
		&Assign{
			Dst: &Part{Base: lpProc, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &FarPointer{Parent: result, Part: machine.FarPointerSegment},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpProc = callresult(int32_t() *)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesDataSegmentNearPointerResult verifies DS plus
// a pointer-valued low word collapses to the source pointer.
func TestCollapseWideStoresCollapsesDataSegmentNearPointerResult(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	getString := &typeinfo.Function{Name: "PszGetCompressedString", Ret: farCharPtrType}
	lpsz := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpsz", Type: farCharPtrType, BPOffset: -0x6}}
	result := &CallResult{Function: getString, TypeInfo: farCharPtrType, InstOff: 0x2ba7}

	block := Block{Effects: []Effect{
		&Assign{Dst: &Part{Base: lpsz, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, Src: result},
		&Assign{Dst: &Part{Base: lpsz, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, Src: &Register{Val: asm.RegDS, SegNum: 0x25}},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: &FuncContext{dsReg: machine.RegVal(asm.RegDS)}}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpsz = callresult(char *)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesFarPointerArrayElementParts verifies split
// far pointer array element loads are reconstructed into one pointer assignment.
func TestCollapseWideStoresCollapsesFarPointerArrayElementParts(t *testing.T) {
	fleetType := &typeinfo.Struct{Name: "FLEET", Size: 0x6a}
	farFleetType := &typeinfo.Pointer{Elem: fleetType, Class: typeinfo.PtrFar}
	rglpflType := &typeinfo.Array{Elem: farFleetType, Count: 16}
	rglpfl := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "rglpfl", Type: rglpflType}}
	iType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	i := &Local{FunctionVar: typeinfo.FunctionVar{Name: "i", Type: iType, BPOffset: -8}}
	lpfl := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpfl", Type: farFleetType, BPOffset: -12}}
	element := &ArrayIndex{Base: rglpfl, Index: i, TypeInfo: farFleetType}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: lpfl, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Part{Base: element, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
		},
		&Assign{
			Dst: &Part{Base: lpfl, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Deref{Pointer: element, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpfl = rglpfl[i]"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesFarPointerArrayElementByteParts verifies split
// byte parts inside a far pointer array collapse to the indexed element.
func TestCollapseWideStoresCollapsesFarPointerArrayElementByteParts(t *testing.T) {
	hbType := &typeinfo.Struct{Name: "HB", Size: 0x1a}
	farHBType := &typeinfo.Pointer{Elem: hbType, Class: typeinfo.PtrFar}
	rglphb := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "rglphb", Type: &typeinfo.Array{Elem: farHBType, Count: 12}}}
	lphb := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lphb", Type: farHBType, BPOffset: -0x1e}}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: lphb, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Part{Base: rglphb, ByteOff: 0x2c, Width: 2, TypeInfo: typeinfo.U16},
		},
		&Assign{
			Dst: &Part{Base: lphb, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Part{Base: rglphb, ByteOff: 0x2e, Width: 2, TypeInfo: typeinfo.U16},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: &FuncContext{}}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lphb = rglphb[0xb]"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesFarPointerOffsetArithmetic verifies split far
// pointer stores are reconstructed when the offset half has byte arithmetic.
func TestCollapseWideStoresCollapsesFarPointerOffsetArithmetic(t *testing.T) {
	thingType := &typeinfo.Struct{Name: "THING", Size: 0x12}
	farThingType := &typeinfo.Pointer{Elem: thingType, Class: typeinfo.PtrFar}
	lpThings := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "lpThings", Type: farThingType}}
	cThing := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "cThing", Type: typeinfo.U16}}
	lpthMac := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpthMac", Type: farThingType, BPOffset: -0x10}}
	offset := &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Part{Base: lpThings, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, RHS: &Word{
		Part: machine.WordLow,
		Parent: &Binary{
			TypeInfo: typeinfo.U16,
			Op:       OpMul,
			LHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x12},
			RHS:      cThing,
		},
	}}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: lpthMac, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: offset,
		},
		&Assign{
			Dst: &Part{Base: lpthMac, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Part{Base: lpThings, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: &FuncContext{}}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpthMac = &lpThings[cThing]"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesFarPointerByteOffsetFallback verifies split
// far pointer stores with dynamic byte offsets do not remain raw farptrs.
func TestCollapseWideStoresCollapsesFarPointerByteOffsetFallback(t *testing.T) {
	btlDataType := &typeinfo.Struct{Name: "BTLDATA", Size: 0xe, Fields: []typeinfo.StructField{
		{Name: "cbData", Offset: 0x6, Size: 2, End: 0x8, Type: typeinfo.U16},
	}}
	farBtlDataType := &typeinfo.Pointer{Elem: btlDataType, Class: typeinfo.PtrFar}
	lpbd := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpbd", Type: farBtlDataType, BPOffset: -0x6}}
	cbData := &FieldAccess{Base: lpbd, Field: &btlDataType.Fields[0]}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: lpbd, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Part{Base: lpbd, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, RHS: cbData},
		},
		&Assign{
			Dst: &Part{Base: lpbd, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Part{Base: lpbd, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: &FuncContext{}}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpbd = ptroff(lpbd, lpbd->cbData)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesSingleFarPointerOffsetAssign verifies a
// lone low-word far pointer increment is rendered as whole-pointer arithmetic.
func TestCollapseWideStoresCollapsesSingleFarPointerOffsetAssign(t *testing.T) {
	orderType := &typeinfo.Struct{Name: "ORDER", Size: 0x12}
	farOrderType := &typeinfo.Pointer{Elem: orderType, Class: typeinfo.PtrFar}
	lpord := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpord", Type: farOrderType, BPOffset: -0x10}}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: lpord, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpAdd,
				LHS:      &Part{Base: lpord, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
				RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x12},
			},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: &FuncContext{}}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpord = (lpord + 0x1)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesFarPointerSubOffsetArithmetic verifies split
// far pointer stores recover a root through additive and subtractive terms.
func TestCollapseWideStoresCollapsesFarPointerSubOffsetArithmetic(t *testing.T) {
	logType := &typeinfo.Array{Elem: typeinfo.U8, Count: 0x1000}
	rtxferType := &typeinfo.Struct{Name: "RTXFER", Size: 0x10}
	farLogType := &typeinfo.Pointer{Elem: logType, Class: typeinfo.PtrFar}
	farRTXFERType := &typeinfo.Pointer{Elem: rtxferType, Class: typeinfo.PtrFar}
	lpLog := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "lpLog", Type: farLogType}}
	imemLogCur := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "imemLogCur", Type: typeinfo.U16}}
	hdrPrev := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "hdrPrev", Type: typeinfo.U16}}
	prt := &Local{FunctionVar: typeinfo.FunctionVar{Name: "prt", Type: farRTXFERType, BPOffset: -0x1a}}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: prt, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpSub,
				LHS:      &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Part{Base: lpLog, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, RHS: imemLogCur},
				RHS:      hdrPrev,
			},
		},
		&Assign{
			Dst: &Part{Base: prt, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Part{Base: lpLog, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: &FuncContext{}}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "prt = ptroff(lpLog, (imemLogCur - hdrPrev))"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesFarPointerArrayRootOffset verifies far pointer
// array roots are normalized before typed offset indexing.
func TestCollapseWideStoresCollapsesFarPointerArrayRootOffset(t *testing.T) {
	btlPlanType := &typeinfo.Struct{Name: "BTLPLAN", Size: 0x24}
	farBtlPlanType := &typeinfo.Pointer{Elem: btlPlanType, Class: typeinfo.PtrFar}
	rglpbtlplan := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "rglpbtlplan", Type: &typeinfo.Array{Elem: farBtlPlanType, Count: 16}}}
	iType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	ibp := &Local{FunctionVar: typeinfo.FunctionVar{Name: "ibp", Type: iType, BPOffset: 0x6}}
	lpbtlplan := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpbtlplan", Type: farBtlPlanType, BPOffset: -0x6}}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: lpbtlplan, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Part{Base: rglpbtlplan, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, RHS: &Word{
				Part: machine.WordLow,
				Parent: &Binary{
					TypeInfo: typeinfo.U16,
					Op:       OpMul,
					LHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x24},
					RHS:      ibp,
				},
			}},
		},
		&Assign{
			Dst: &Part{Base: lpbtlplan, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Part{Base: rglpbtlplan, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: &FuncContext{}}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpbtlplan = &rglpbtlplan[0x0][ibp]"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesConstSegmentPointerOffset verifies a
// context-free static segment plus dynamic offset becomes pointer arithmetic.
func TestCollapseWideStoresCollapsesConstSegmentPointerOffset(t *testing.T) {
	charPtrType := &typeinfo.Pointer{Elem: typeinfo.U8, Class: typeinfo.PtrFar}
	iType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	iChunk := &Local{FunctionVar: typeinfo.FunctionVar{Name: "iChunk", Type: iType, BPOffset: -0x1a}}
	pchLen := &Local{FunctionVar: typeinfo.FunctionVar{Name: "pchLen", Type: charPtrType, BPOffset: -0x0e}}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: pchLen, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Const{TypeInfo: typeinfo.U16, U64: 0x5934}, RHS: &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpMul,
				LHS:      iChunk,
				RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x40},
			}},
		},
		&Assign{
			Dst: &Part{Base: pchLen, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0x7},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "pchLen = ptroff(farptr(0x7, 0x5934), (0x40 * iChunk))"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresRecoversPrematureResolvedOffset verifies far pointer
// pairs recover when the offset half was resolved before seeing the segment.
func TestCollapseWideStoresRecoversPrematureResolvedOffset(t *testing.T) {
	charPtrType := &typeinfo.Pointer{Elem: typeinfo.U8, Class: typeinfo.PtrFar}
	iType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	acTUT := &typeinfo.GlobalVar{
		Name: "acTUT",
		Addr: typeinfo.Addr{Seg: 0x21, Off: 0x5734},
		Type: &typeinfo.Array{Elem: typeinfo.U8, Count: 640},
	}
	szBase := &typeinfo.GlobalVar{
		Name: "szBase",
		Addr: typeinfo.Addr{Seg: 0x25, Off: 0x56a2},
		Type: &typeinfo.Array{Elem: typeinfo.U8, Count: 256},
	}
	ctx := &FuncContext{res: symresolve.NewResolver(nil, &typeinfo.SymbolDB{Globals: []*typeinfo.GlobalVar{acTUT, szBase}})}
	iChunk := &Local{FunctionVar: typeinfo.FunctionVar{Name: "iChunk", Type: iType, BPOffset: -0x8}}
	pchLen := &Local{FunctionVar: typeinfo.FunctionVar{Name: "pchLen", Type: charPtrType, BPOffset: -0x0c}}
	low := &Part{
		Base: &ArrayIndex{
			Base:     &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: szBase}},
			Index:    &Binary{TypeInfo: typeinfo.U16, Op: OpMul, LHS: &Const{TypeInfo: typeinfo.U16, U64: 0x40}, RHS: iChunk},
			TypeInfo: typeinfo.U8,
		},
		ByteOff:  0x92,
		Width:    0,
		TypeInfo: intTypeForWidth(0),
	}

	block := Block{Effects: []Effect{
		&Assign{Dst: &Part{Base: pchLen, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, Src: low},
		&Assign{Dst: &Part{Base: pchLen, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, Src: &Const{TypeInfo: typeinfo.U16, U64: 0x21}},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "pchLen = &acTUT[(0x40 * iChunk)]"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesNestedHelperParts verifies split stores from
// nested lowered helper expressions are reconstructed into one wide assignment.
func TestCollapseWideStoresCollapsesNestedHelperParts(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	int32Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int32_t", Size: 4, Signed: true}
	lPopMax := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lPopMax", Type: int32Type, BPOffset: -10}}
	iEff := &Local{FunctionVar: typeinfo.FunctionVar{Name: "iEff", Type: int16Type, BPOffset: -12}}
	cMax := &Local{FunctionVar: typeinfo.FunctionVar{Name: "cMax", Type: int32Type, BPOffset: -6}}

	product := &Cast{
		To:       "uint32_t",
		TypeInfo: typeinfo.U32,
		Value: &Binary{
			TypeInfo: typeinfo.U32,
			Op:       OpMul,
			LHS:      lPopMax,
			RHS:      &SignExtend{Parent: iEff, FromBits: 16, ToBits: 32, TypeInfo: int32Type},
		},
	}
	quotient := &Cast{
		To:       "int32_t",
		TypeInfo: int32Type,
		Value: &Binary{
			TypeInfo: int32Type,
			Op:       OpDiv,
			LHS: &Words{Words: []Expr{
				&Word{Parent: product, Part: machine.WordHigh},
				&Word{Parent: product, Part: machine.WordLow},
			}},
			RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x64},
		},
	}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Part{Base: cMax, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Word{Parent: quotient, Part: machine.WordLow},
		},
		&Assign{
			Dst: &Part{Base: cMax, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Word{Parent: quotient, Part: machine.WordHigh},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "cMax = (int32_t)((uint32_t)(lPopMax * sext16to32(iEff)) / 0x64)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesBitfieldParts verifies split word masks can
// reconstruct one wide bitfield assignment.
func TestCollapseWideStoresCollapsesBitfieldParts(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("UninhabitPlanet"))
	lppl := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lppl", Type: &typeinfo.Pointer{Elem: fx.SDB.GetStruct("PLANET"), Class: typeinfo.PtrFar}, BPOffset: 6}}
	lo := &Deref{Pointer: lppl, ByteOff: 0x18, Width: 2, TypeInfo: typeinfo.U16}
	hi := &Deref{Pointer: lppl, ByteOff: 0x1a, Width: 2, TypeInfo: typeinfo.U16}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: lo,
			Src: &Binary{TypeInfo: typeinfo.U16, Op: OpOr, LHS: &Binary{TypeInfo: typeinfo.U16, Op: OpAnd, LHS: lo, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x0fff}}, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0xf000}},
		},
		&Assign{
			Dst: hi,
			Src: &Binary{TypeInfo: typeinfo.U16, Op: OpOr, LHS: &Binary{TypeInfo: typeinfo.U16, Op: OpAnd, LHS: hi, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0xfffe}}, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x1}},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lppl->iScanner = 0x1f"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresResolvesNestedPartBitfieldExtract verifies bitfield
// reads from contiguous parts of a nested struct field are named.
func TestCollapseWideStoresResolvesNestedPartBitfieldExtract(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("AddMinesToBlockedQueues"))
	sel := fx.SDB.GetGlobal("sel")
	if sel == nil {
		t.Fatal("sel not found")
	}
	selPlanetField, ok := ctx.res.ResolveFieldLoad(sel, 0x98, fx.SDB.GetStruct("PLANET").Bytes())
	if !ok {
		t.Fatal("sel.pl not resolved")
	}
	selPlanet := &SymbolRef{Path: selPlanetField}
	planetBits := &Words{Words: []Expr{
		&Part{Base: selPlanet, ByteOff: 0x18, Width: 2, TypeInfo: typeinfo.U16},
		&Part{Base: selPlanet, ByteOff: 0x1a, Width: 2, TypeInfo: typeinfo.U16},
	}}
	cond := &Binary{
		TypeInfo: typeinfo.U16,
		Op:       OpAnd,
		LHS: &Word{
			Parent: &Cast{
				Value: &Binary{
					TypeInfo: typeinfo.U32,
					Op:       OpShr,
					LHS:      planetBits,
					RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x17},
				},
				To:       "uint32_t",
				TypeInfo: typeinfo.U32,
			},
			Part: machine.WordLow,
		},
		RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x1},
	}

	block := Block{Effects: []Effect{
		&Branch{
			Cond:       &Compare{Op: CompareNE, LHS: cond, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x0}},
			TrueBlock:  0x19aa,
			FalseBlock: 0x196f,
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "branch sel.pl.fNoResearch != 0x0 ? L_19aa : L_196f"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresResolvesUnshiftedBitfieldExtract verifies low-mask
// branch conditions resolve to bitfield names without an explicit shift.
func TestCollapseWideStoresResolvesUnshiftedBitfieldExtract(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("FBuildObject"))
	player := fx.SDB.GetStruct("PLAYER")
	if player == nil {
		t.Fatal("PLAYER not found")
	}
	rgplr := fx.SDB.GetGlobal("rgplr")
	if rgplr == nil {
		t.Fatal("rgplr not found")
	}
	planet := fx.SDB.GetStruct("PLANET")
	if planet == nil {
		t.Fatal("PLANET not found")
	}
	lppl := &Local{FunctionVar: typeinfo.FunctionVar{
		Name:     "lppl",
		Type:     &typeinfo.Pointer{Elem: planet, Class: typeinfo.PtrFar},
		BPOffset: 6,
	}}
	iPlayerField, ok := ctx.res.ResolveFieldPathLoad(&symresolve.SymbolRoot{Symbol: &lppl.FunctionVar}, 0x2, 2)
	if !ok {
		t.Fatal("lppl->iPlayer not resolved")
	}
	playerExpr := &ArrayIndex{
		Base:     &Global{GlobalVar: rgplr},
		Index:    &SymbolRef{Path: iPlayerField},
		TypeInfo: player,
	}
	cond := &Binary{
		TypeInfo: typeinfo.U16,
		Op:       OpAnd,
		LHS:      &Part{Base: playerExpr, ByteOff: 0x4, Width: 2, TypeInfo: typeinfo.U16},
		RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0xfff},
	}

	block := Block{Effects: []Effect{
		&Branch{
			Cond:       &Compare{Op: CompareNE, LHS: cond, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x200}},
			TrueBlock:  0x219c,
			FalseBlock: 0x1d3a,
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "branch rgplr[lppl->iPlayer].cFleet != 0x200 ? L_219c : L_1d3a"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresResolvesDynamicBitfieldAssign verifies masked stores
// from one bitfield into another are represented as bitfield assignments.
func TestCollapseWideStoresResolvesDynamicBitfieldAssign(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("InitProduction"))
	gd := fx.SDB.GetGlobal("gd")
	if gd == nil {
		t.Fatal("gd not found")
	}
	sel := fx.SDB.GetGlobal("sel")
	if sel == nil {
		t.Fatal("sel not found")
	}
	selPlanetField, ok := ctx.res.ResolveFieldLoad(sel, 0x98, fx.SDB.GetStruct("PLANET").Bytes())
	if !ok {
		t.Fatal("sel.pl not resolved")
	}
	fNoResearch, ok := ctx.res.ResolveBitfieldPathLoad(selPlanetField, 0x18, 4, 0x17, 1)
	if !ok {
		t.Fatal("sel.pl.fNoResearch not resolved")
	}
	dst := &Part{Base: &Global{GlobalVar: gd}, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: dst,
			Src: &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpOr,
				LHS: &Binary{
					TypeInfo: typeinfo.U16,
					Op:       OpAnd,
					LHS:      dst,
					RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0xffdf},
				},
				RHS: &Binary{
					TypeInfo: typeinfo.U16,
					Op:       OpShl,
					LHS: &Binary{
						TypeInfo: typeinfo.U16,
						Op:       OpAnd,
						LHS:      &SymbolRef{Path: fNoResearch},
						RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x1},
					},
					RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x5},
				},
			},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "gd.fNoResearchSav = sel.pl.fNoResearch"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresResolvesShiftedConstBitfieldAssign verifies constant
// set masks are shifted down to the target bitfield value.
func TestCollapseWideStoresResolvesShiftedConstBitfieldAssign(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("DropSalvage"))
	lpth := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lpth", Type: &typeinfo.Pointer{Elem: fx.SDB.GetStruct("THING"), Class: typeinfo.PtrFar}, BPOffset: -0x10}}
	dst := &Part{
		Base:     &SymbolRef{Path: &symresolve.SymbolField{Base: &symresolve.SymbolRoot{Symbol: &lpth.FunctionVar}, Field: fieldByName(fx.SDB.GetStruct("THING"), "thp")}},
		ByteOff:  0,
		Width:    2,
		TypeInfo: typeinfo.U16,
	}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: dst,
			Src: &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpOr,
				LHS: &Binary{
					TypeInfo: typeinfo.U16,
					Op:       OpAnd,
					LHS:      dst,
					RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0xc3ff},
				},
				RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x0},
			},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lpth->thp.iWarp = 0x0"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesDerefFieldParts verifies split word derefs can
// reconstruct one named field assignment.
func TestCollapseWideStoresCollapsesDerefFieldParts(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("UninhabitPlanet"))
	lppl := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lppl", Type: &typeinfo.Pointer{Elem: fx.SDB.GetStruct("PLANET"), Class: typeinfo.PtrFar}, BPOffset: 6}}

	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Deref{Pointer: lppl, ByteOff: 0x2c, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0x0},
		},
		&Assign{
			Dst: &Deref{Pointer: lppl, ByteOff: 0x2e, Width: 2, TypeInfo: typeinfo.U16},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0x0},
		},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "lppl->lStarbase = 0x0"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestCollapseWideStoresCollapsesReturnWordSum verifies return values rebuilt
// from matching low/high additive word trees collapse to one wide expression.
func TestCollapseWideStoresCollapsesReturnWordSum(t *testing.T) {
	dx := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dx", Type: typeinfo.I32, BPOffset: -0x0a}}
	dy := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dy", Type: typeinfo.I32, BPOffset: -0x06}}
	dxSquared := &Binary{TypeInfo: typeinfo.U32, Op: OpMul, LHS: dx, RHS: dx}
	dySquared := &Binary{TypeInfo: typeinfo.U32, Op: OpMul, LHS: dy, RHS: dy}
	block := Block{Effects: []Effect{
		&Return{Value: &Words{Words: []Expr{
			&Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Word{Parent: dxSquared, Part: machine.WordLow}, RHS: &Word{Parent: dySquared, Part: machine.WordLow}},
			&Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Word{Parent: dxSquared, Part: machine.WordHigh}, RHS: &Word{Parent: dySquared, Part: machine.WordHigh}},
		}}},
	}}

	gotBlock, changed := (&collapseWideStoresProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(gotBlock.Effects))
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "return ((dx * dx) + (dy * dy))"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}
