package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestResolveStorageCollapsesFarPointerFromCallResult verifies matching
// farseg/faroff projections of one call result collapse back to that result.
func TestResolveStorageCollapsesFarPointerFromCallResult(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	fleetType := &typeinfo.Struct{Name: "FLEET", Size: 0x6a}
	farFleetType := &typeinfo.Pointer{Elem: fleetType, Class: typeinfo.PtrFar}
	lpflFromID := &typeinfo.Function{Name: "LpflFromId", Ret: farFleetType}
	checkCargo := &typeinfo.Function{Name: "FCheckCargo", Ret: int16Type, Params: []typeinfo.FunctionVar{
		{Name: "lpfl", Type: farFleetType},
	}}
	result := &CallResult{Function: lpflFromID, TypeInfo: farFleetType, InstOff: 0x183d}

	block := Block{Effects: []Effect{
		&CallEffect{
			Call: &Call{Function: checkCargo, Args: []Expr{
				&FarPointer{
					Part:     machine.FarPointerWhole,
					Segment:  &FarPointer{Parent: result, Part: machine.FarPointerSegment},
					Offset:   &FarPointer{Parent: result, Part: machine.FarPointerOffset},
					TypeInfo: farFleetType,
				},
			}},
		},
	}}

	gotBlock, changed := (&resolveStorageProcessor{ctx: &FuncContext{}}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "call FCheckCargo(callresult(FLEET *))"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestResolveStorageCollapsesDataSegmentNearPointerOffset verifies DS plus
// near-pointer arithmetic resolves to source pointer arithmetic.
func TestResolveStorageCollapsesDataSegmentNearPointerOffset(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	nearCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrNear}
	pszExt := &Local{FunctionVar: typeinfo.FunctionVar{Name: "pszExt", Type: nearCharPtrType, BPOffset: 8}}
	ctx := &FuncContext{dsReg: machine.RegVal(asm.RegDS)}
	ptr := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Register{Val: asm.RegDS, SegNum: 0x25},
		Offset: &Binary{
			TypeInfo: typeinfo.U16,
			Op:       OpAdd,
			LHS:      pszExt,
			RHS:      &Const{TypeInfo: typeinfo.U16, U64: 1},
		},
	}

	got, ok := ctx.resolveSemanticFarPointer(ptr)
	if !ok {
		t.Fatal("resolveSemanticFarPointer ok = false, want true")
	}
	gotText := FormatExpr(got)
	want := "ptroff(pszExt, 0x1)"
	if gotText != want {
		t.Fatalf("expr = %q, want %q", gotText, want)
	}
}

// TestResolveStorageCollapsesDataSegmentArrayFieldAddress verifies DS plus a
// zero-width array field address resolves to the array expression.
func TestResolveStorageCollapsesDataSegmentArrayFieldAddress(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	hulType := &typeinfo.Struct{
		Name: "HUL",
		Size: 0x28,
		Fields: []typeinfo.StructField{
			{Name: "id", Type: typeinfo.U16, Offset: 0, Size: 2, End: 2},
			{Name: "szName", Type: &typeinfo.Array{Elem: charType, Count: 32}, Offset: 8, Size: 32, End: 0x28},
		},
	}
	hulType.FinalizeLayout()
	hul := &Local{FunctionVar: typeinfo.FunctionVar{Name: "hul", Type: hulType, BPOffset: -0x28}}
	ctx := &FuncContext{dsReg: machine.RegVal(asm.RegDS)}
	ptr := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Register{Val: asm.RegDS, SegNum: 0x25},
		Offset: &Part{
			Base:     hul,
			ByteOff:  8,
			Width:    0,
			TypeInfo: intTypeForWidth(0),
		},
	}

	got, ok := ctx.resolveSemanticFarPointer(ptr)
	if !ok {
		t.Fatal("resolveSemanticFarPointer ok = false, want true")
	}
	gotText := FormatExpr(got)
	want := "hul.szName"
	if gotText != want {
		t.Fatalf("expr = %q, want %q", gotText, want)
	}
}

func TestResolveStorageResolvesNestedPartThroughUnionContext(t *testing.T) {
	fx := testfixture.Stars(t)
	partType := fx.SDB.GetStruct("PART")
	part := &Local{FunctionVar: typeinfo.FunctionVar{Name: "part", Type: partType}}
	rule, ok := fx.SDB.UnionRules.UnionVariantForType(partType)
	if !ok {
		t.Fatal("PART union rule not found")
	}
	unionCtx := symresolve.NewUnionContext()
	unionCtx.Add(&symresolve.SymbolRoot{Symbol: &part.FunctionVar}, rule, typeinfo.EnumValue{Name: "hstPlanetary", Value: 0x8000})
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("CalcPctSurvive"))
	ctx.currentUnionContext = unionCtx
	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Local{FunctionVar: typeinfo.FunctionVar{Name: "dst", Type: typeinfo.I32}},
			Src: &SignExtend{
				Parent: &Part{
					Base: &Part{
						Base:     part,
						ByteOff:  4,
						Width:    4,
						TypeInfo: typeinfo.U32,
					},
					ByteOff:  52,
					Width:    2,
					TypeInfo: typeinfo.U16,
				},
				FromBits: 16,
				ToBits:   32,
				TypeInfo: typeinfo.I32,
			},
		},
	}}

	gotBlock, changed := (&resolveStorageProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "dst = sext16to32(part.pplanetary->grAbility)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestResolveStorageUsesSameBlockGlobalDiscriminatorAssign verifies global
// discriminator writes select the following union-field accesses in the block.
func TestResolveStorageUsesSameBlockGlobalDiscriminatorAssign(t *testing.T) {
	fx := testfixture.Stars(t)
	globalPD := fx.SDB.GetGlobal("GlobalPD")
	if globalPD == nil {
		t.Fatal("GlobalPD not found")
	}
	enumType := fx.SDB.GetEnum("GrPopupType")
	if enumType == nil {
		t.Fatal("GrPopupType not found")
	}
	grPopupPath, ok := appendSymbolFieldPath(&symresolve.SymbolRoot{Symbol: globalPD}, []string{"grPopup"})
	if !ok {
		t.Fatal("GlobalPD.grPopup not resolved")
	}
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("ShipCommandProc"))
	block := Block{Effects: []Effect{
		&Assign{
			Dst: &SymbolRef{Path: grPopupPath},
			Src: &Const{TypeInfo: enumType, U64: 11},
		},
		&Assign{
			Dst: &Part{
				Base:     &Global{GlobalVar: globalPD},
				ByteOff:  6,
				Width:    2,
				TypeInfo: typeinfo.U16,
			},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 1},
		},
	}}

	gotBlock, changed := (&resolveStorageProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	got := FormatEffect(gotBlock.Effects[1])
	want := "GlobalPD.fShowDamage = 0x1"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestConsumeAddressExprResolvesZeroWidthExactFieldAddress verifies address
// expressions at a field boundary name the field instead of a zero-width part.
func TestConsumeAddressExprResolvesZeroWidthExactFieldAddress(t *testing.T) {
	pointType := &typeinfo.Struct{Name: "POINT", Size: 4}
	fleetType := &typeinfo.Struct{Name: "FLEET", Size: 0x7c}
	selType := &typeinfo.Struct{
		Name: "SEL",
		Size: 0xd0,
		Fields: []typeinfo.StructField{
			{Name: "pt", Type: pointType, Offset: 0, Size: 4, End: 4},
			{Name: "fl", Type: fleetType, Offset: 0x1c, Size: 0x7c, End: 0x98},
		},
	}
	selType.FinalizeLayout()
	sel := &Local{FunctionVar: typeinfo.FunctionVar{Name: "sel", Type: selType}}

	got, ok := (&machineConverter{}).consumeAddressExpr(AddressExpr{Base: sel, Offset: 0x1c}, 0)
	if !ok {
		t.Fatal("consumeAddressExpr ok = false, want true")
	}
	gotText := FormatExpr(got)
	want := "sel.fl"
	if gotText != want {
		t.Fatalf("expr = %q, want %q", gotText, want)
	}

	got, ok = (&machineConverter{}).consumeAddressExpr(AddressExpr{Base: sel, Offset: 0}, 0)
	if !ok {
		t.Fatal("consumeAddressExpr offset zero ok = false, want true")
	}
	gotText = FormatExpr(got)
	want = "sel"
	if gotText != want {
		t.Fatalf("offset zero expr = %q, want %q", gotText, want)
	}
}

// TestResolveStorageCollapsesMergedFarPointerOffsets verifies direct DS
// offsets are resolved before merge lowering hides each arm's address.
func TestResolveStorageCollapsesMergedFarPointerOffsets(t *testing.T) {
	sdb := &typeinfo.SymbolDB{Globals: []*typeinfo.GlobalVar{
		{Name: "gOne", Addr: typeinfo.Addr{Seg: 0x25, Off: 0xc85}, Type: typeinfo.U16},
		{Name: "gTwo", Addr: typeinfo.Addr{Seg: 0x25, Off: 0xc86}, Type: typeinfo.U16},
	}}
	ctx := &FuncContext{dsReg: machine.RegVal(asm.RegDS), res: symresolve.NewResolver(nil, sdb)}
	ptr := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Register{Val: asm.RegDS, SegNum: 0x25},
		Offset: &Merge{
			TypeInfo: typeinfo.U16,
			Join:     0x20,
			Arms: []MergeArm{
				{Block: 0x10, Value: &Const{TypeInfo: typeinfo.U16, U64: 0xc85}},
				{Block: 0x11, Value: &Const{TypeInfo: typeinfo.U16, U64: 0xc86}},
			},
		},
		TypeInfo: &typeinfo.Pointer{Elem: typeinfo.U16, Class: typeinfo.PtrFar},
	}

	got, ok := ctx.resolveSemanticFarPointer(ptr)
	if !ok {
		t.Fatal("resolveSemanticFarPointer ok = false, want true")
	}
	gotText := FormatExpr(got)
	want := "merge(Join: L_0020, (L_0010:&gOne, L_0011:&gTwo))"
	if gotText != want {
		t.Fatalf("expr = %q, want %q", gotText, want)
	}
}

// TestResolveStorageKeepsUnknownConstSegmentPointerOffset verifies unknown
// static segment bases keep their scaled residual offset.
func TestResolveStorageKeepsUnknownConstSegmentPointerOffset(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	sdb := &typeinfo.SymbolDB{}
	ctx := &FuncContext{res: symresolve.NewResolver(nil, sdb)}
	charPtrType := &typeinfo.Pointer{Elem: typeinfo.U8, Class: typeinfo.PtrFar}
	iChunk := &Local{FunctionVar: typeinfo.FunctionVar{Name: "iChunk", Type: int16Type, BPOffset: -0x1a}}
	ptr := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Const{TypeInfo: typeinfo.U16, U64: 0x7},
		Offset: &Binary{
			TypeInfo: typeinfo.U16,
			Op:       OpAdd,
			LHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x5934},
			RHS:      &Binary{TypeInfo: typeinfo.U16, Op: OpMul, LHS: iChunk, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x40}},
		},
		TypeInfo: charPtrType,
	}

	got, ok := ctx.resolveSemanticFarPointer(ptr)
	if !ok {
		t.Fatal("resolveSemanticFarPointer ok = false, want true")
	}
	gotText := FormatExpr(got)
	want := "ptroff(farptr(0x7, 0x5934), (0x40 * iChunk))"
	if gotText != want {
		t.Fatalf("expr = %q, want %q", gotText, want)
	}
}

// TestResolveStorageCollapsesKnownConstSegmentByteArrayOffset verifies static
// segment arithmetic resolves through a known byte-array global.
func TestResolveStorageCollapsesKnownConstSegmentByteArrayOffset(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	sdb := &typeinfo.SymbolDB{Globals: []*typeinfo.GlobalVar{
		{
			Name: "acTUT",
			Addr: typeinfo.Addr{Seg: 0x21, Off: 0x5734},
			Type: &typeinfo.Array{Elem: typeinfo.U8, Count: 640},
		},
	}}
	ctx := &FuncContext{res: symresolve.NewResolver(nil, sdb)}
	charPtrType := &typeinfo.Pointer{Elem: typeinfo.U8, Class: typeinfo.PtrFar}
	iChunk := &Local{FunctionVar: typeinfo.FunctionVar{Name: "iChunk", Type: int16Type, BPOffset: -0x8}}
	ptr := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Const{TypeInfo: typeinfo.U16, U64: 0x21},
		Offset: &Binary{
			TypeInfo: typeinfo.U16,
			Op:       OpAdd,
			LHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x5734},
			RHS:      &Binary{TypeInfo: typeinfo.U16, Op: OpMul, LHS: &Const{TypeInfo: typeinfo.U16, U64: 0x40}, RHS: iChunk},
		},
		TypeInfo: charPtrType,
	}

	got, ok := ctx.resolveSemanticFarPointer(ptr)
	if !ok {
		t.Fatal("resolveSemanticFarPointer ok = false, want true")
	}
	gotText := FormatExpr(got)
	want := "&acTUT[(0x40 * iChunk)]"
	if gotText != want {
		t.Fatalf("expr = %q, want %q", gotText, want)
	}
}

// TestConsumeAddressExprFoldsByteArrayIndexOffsets verifies byte table indexes
// keep residual byte terms in the source-level array index.
func TestConsumeAddressExprFoldsByteArrayIndexOffsets(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	table := &typeinfo.GlobalVar{
		Name: "rgbrcStart",
		Type: &typeinfo.Array{Elem: typeinfo.U8, Count: 136},
	}
	base := &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: table}}
	i := &Local{FunctionVar: typeinfo.FunctionVar{Name: "i", Type: int16Type, BPOffset: -0xa}}
	ibrc := &Local{FunctionVar: typeinfo.FunctionVar{Name: "ibrc", Type: int16Type, BPOffset: 0xa}}

	tests := []struct {
		name string
		addr AddressExpr
		want string
	}{
		{
			name: "signed byte residual",
			addr: AddressExpr{
				Base:   base,
				Offset: -1,
				Terms:  []ScaledTerm{{Expr: i, Scale: 1}},
			},
			want: "rgbrcStart[(i - 0x1)]",
		},
		{
			name: "multiple byte terms",
			addr: AddressExpr{
				Base:  base,
				Terms: []ScaledTerm{{Expr: i, Scale: 1}, {Expr: ibrc, Scale: 1}},
			},
			want: "rgbrcStart[(i + ibrc)]",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := (&machineConverter{}).consumeAddressExpr(tt.addr, 1)
			if !ok {
				t.Fatal("consumeAddressExpr ok = false, want true")
			}
			if gotText := FormatExpr(got); gotText != tt.want {
				t.Fatalf("expr = %q, want %q", gotText, tt.want)
			}
		})
	}
}

// TestResolveStorageCollapsesHighConstSegmentArrayOffset verifies direct
// far-pointer offsets above 0x8000 are treated as unsigned segment offsets.
func TestResolveStorageCollapsesHighConstSegmentArrayOffset(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	playerType := &typeinfo.Struct{Name: "PLAYER", Size: 0xc0}
	playerRowType := &typeinfo.Array{Elem: playerType, Count: 4}
	vrgplrComp := &typeinfo.GlobalVar{
		Name: "vrgplrComp",
		Addr: typeinfo.Addr{Seg: 0x10, Off: 0xa370},
		Type: &typeinfo.Array{Elem: playerRowType, Count: 6},
	}
	ctx := &FuncContext{res: symresolve.NewResolver(nil, &typeinfo.SymbolDB{Globals: []*typeinfo.GlobalVar{vrgplrComp}})}
	farPlayerType := &typeinfo.Pointer{Elem: playerType, Class: typeinfo.PtrFar}
	idAi := &Local{FunctionVar: typeinfo.FunctionVar{Name: "idAi", Type: int16Type, BPOffset: 0x6}}
	lvlAi := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lvlAi", Type: int16Type, BPOffset: 0x8}}
	ptr := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Const{TypeInfo: typeinfo.U16, U64: 0x10},
		Offset: &Binary{
			TypeInfo: typeinfo.U16,
			Op:       OpAdd,
			LHS: &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpAdd,
				LHS:      &Const{TypeInfo: typeinfo.U16, U64: 0xa370},
				RHS:      &Binary{TypeInfo: typeinfo.U16, Op: OpMul, LHS: &Const{TypeInfo: typeinfo.U16, U64: 0x300}, RHS: idAi},
			},
			RHS: &Binary{TypeInfo: typeinfo.U16, Op: OpMul, LHS: &Const{TypeInfo: typeinfo.U16, U64: 0xc0}, RHS: lvlAi},
		},
		TypeInfo: farPlayerType,
	}

	got, ok := ctx.resolveSemanticFarPointer(ptr)
	if !ok {
		t.Fatal("resolveSemanticFarPointer ok = false, want true")
	}
	gotText := FormatExpr(got)
	want := "&vrgplrComp[idAi][lvlAi]"
	if gotText != want {
		t.Fatalf("expr = %q, want %q", gotText, want)
	}
}
