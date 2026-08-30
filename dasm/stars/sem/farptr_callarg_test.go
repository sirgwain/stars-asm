package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCollapseTypedFarPointerCallArgCollapsesDefaultDataOffsetArithmetic
// verifies DGROUP far pointer call args collapse even when the source offset is
// pointer arithmetic carried as a raw 16-bit offset.
func TestCollapseTypedFarPointerCallArgCollapsesDefaultDataOffsetArithmetic(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	szWork := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "szWork", Type: &typeinfo.Array{Elem: charType, Count: 256}}}
	cch := &Local{FunctionVar: typeinfo.FunctionVar{Name: "cch", Type: typeinfo.U16, BPOffset: -0x38}}
	offset := &Binary{
		TypeInfo: typeinfo.U16,
		Op:       OpAdd,
		LHS: &ArrayIndex{
			Base:     szWork,
			Index:    cch,
			TypeInfo: charType,
		},
		RHS: &Const{TypeInfo: typeinfo.U16, U64: 0xfffd},
	}
	arg := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Const{TypeInfo: typeinfo.U16, U64: 0x25},
		Offset:  offset,
	}

	got, changed := collapseTypedFarPointerCallArg(&FuncContext{dsReg: machine.ConstVal(0x25)}, arg, &typeinfo.FunctionVar{Type: farCharPtrType})
	if !changed {
		t.Fatal("collapseTypedFarPointerCallArg changed = false, want true")
	}
	if got != offset {
		t.Fatalf("collapsed arg = %#v, want original offset", got)
	}
	if gotText := FormatExpr(got); gotText != "(szWork[cch] + 0xfffd)" {
		t.Fatalf("collapsed arg text = %q, want %q", gotText, "(szWork[cch] + 0xfffd)")
	}
}

// TestCollapseTypedFarPointerCallArgAddressesDefaultDataLValue verifies DGROUP
// far pointer call args collapse to address-of when the offset names source
// storage rather than a pointer-valued expression.
func TestCollapseTypedFarPointerCallArgAddressesDefaultDataLValue(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	rectType := &typeinfo.Struct{Name: "tagRECT", SKind: typeinfo.StructKindStruct, Size: 8}
	rectType.Fields = []typeinfo.StructField{
		{Name: "left", Type: int16Type, Offset: 0, Size: 2, End: 2},
		{Name: "top", Type: int16Type, Offset: 2, Size: 2, End: 4},
		{Name: "right", Type: int16Type, Offset: 4, Size: 2, End: 6},
		{Name: "bottom", Type: int16Type, Offset: 6, Size: 2, End: 8},
	}
	rectPtrType := &typeinfo.Pointer{Elem: rectType, Class: typeinfo.PtrFar}
	field := &rectType.Fields[0]
	prc := &Local{FunctionVar: typeinfo.FunctionVar{Name: "prc", Type: &typeinfo.Pointer{Elem: rectType, Class: typeinfo.PtrNear}, BPOffset: 6}}
	left := &FieldAccess{Base: prc, Field: field}
	arg := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Const{TypeInfo: typeinfo.U16, U64: 0x25},
		Offset:  left,
	}

	got, changed := collapseTypedFarPointerCallArg(&FuncContext{dsReg: machine.ConstVal(0x25)}, arg, &typeinfo.FunctionVar{Type: rectPtrType})
	if !changed {
		t.Fatal("collapseTypedFarPointerCallArg changed = false, want true")
	}
	if gotText := FormatExpr(got); gotText != "&prc->left" {
		t.Fatalf("collapsed arg text = %q, want %q", gotText, "&prc->left")
	}
}

// TestCollapseTypedFarPointerCallArgCollapsesDefaultDataMerge verifies DGROUP
// far pointer call args collapse when the offset is a control-flow merge.
func TestCollapseTypedFarPointerCallArgCollapsesDefaultDataMerge(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	offset := &Merge{Arms: []MergeArm{
		{Block: 0x576d, Value: &Const{TypeInfo: typeinfo.U16, U64: 0x160b}},
		{Block: 0x5775, Value: &Const{TypeInfo: typeinfo.U16, U64: 0x1613}},
	}}
	arg := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Const{TypeInfo: typeinfo.U16, U64: 0x25},
		Offset:  offset,
	}

	got, changed := collapseTypedFarPointerCallArg(&FuncContext{dsReg: machine.ConstVal(0x25)}, arg, &typeinfo.FunctionVar{Type: farCharPtrType})
	if !changed {
		t.Fatal("collapseTypedFarPointerCallArg changed = false, want true")
	}
	if got != offset {
		t.Fatalf("collapsed arg = %#v, want original merge offset", got)
	}
	wantText := "merge(Join: L_0000, (L_576d:0x160b, L_5775:0x1613))"
	if gotText := FormatExpr(got); gotText != wantText {
		t.Fatalf("collapsed arg text = %q, want %q", gotText, wantText)
	}
}

// TestCollapseTypedFarPointerCallArgConvertsResourceIDs verifies annotated
// resource parameters preserve the MAKEINTRESOURCE meaning of 0:offset pointers.
func TestCollapseTypedFarPointerCallArgConvertsResourceIDs(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	param := &typeinfo.FunctionVar{Type: farCharPtrType, Semantic: typeinfo.ParamSemanticResourceNameOrID}
	id := &Const{TypeInfo: typeinfo.U16, U64: 0x7a}
	arg := &FarPointer{
		Part:    machine.FarPointerWhole,
		Segment: &Const{TypeInfo: typeinfo.U16, U64: 0},
		Offset:  id,
	}

	got, changed := collapseTypedFarPointerCallArg(&FuncContext{dsReg: machine.ConstVal(0x25)}, arg, param)
	if !changed {
		t.Fatal("collapseTypedFarPointerCallArg changed = false, want true")
	}
	if gotText := FormatExpr(got); gotText != "MAKEINTRESOURCE(0x7a)" {
		t.Fatalf("collapsed arg text = %q, want %q", gotText, "MAKEINTRESOURCE(0x7a)")
	}
	if gotType := got.ExprType(); !typeinfo.Equals(gotType, farCharPtrType) {
		t.Fatalf("collapsed arg type = %v, want %v", gotType, farCharPtrType)
	}
}

// TestCollapseTypedFarPointerCallArgConvertsResourceIDOffsets verifies resource
// IDs survive dynamic pointer offset normalization.
func TestCollapseTypedFarPointerCallArgConvertsResourceIDOffsets(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	param := &typeinfo.FunctionVar{Type: farCharPtrType, Semantic: typeinfo.ParamSemanticResourceNameOrID}
	i := &Local{FunctionVar: typeinfo.FunctionVar{Name: "i", Type: typeinfo.U16, BPOffset: -0x8}}
	offset := &Binary{
		TypeInfo: typeinfo.U16,
		Op:       OpOr,
		LHS: &Binary{
			TypeInfo: typeinfo.U16,
			Op:       OpAdd,
			LHS:      i,
			RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x1cc},
		},
		RHS: &Const{TypeInfo: typeinfo.U16, U64: 0},
	}
	arg := &PointerOffset{
		Pointer: &FarPointer{
			Part:    machine.FarPointerWhole,
			Segment: &Const{TypeInfo: typeinfo.U16, U64: 0},
			Offset:  &Const{TypeInfo: typeinfo.U16, U64: 0},
		},
		Offset:   offset,
		TypeInfo: farCharPtrType,
	}

	got, changed := collapseTypedFarPointerCallArg(&FuncContext{dsReg: machine.ConstVal(0x25)}, arg, param)
	if !changed {
		t.Fatal("collapseTypedFarPointerCallArg changed = false, want true")
	}
	if gotText := FormatExpr(got); gotText != "MAKEINTRESOURCE(((i + 0x1cc) | 0x0))" {
		t.Fatalf("collapsed arg text = %q, want %q", gotText, "MAKEINTRESOURCE(((i + 0x1cc) | 0x0))")
	}
}

// TestCollapseTypedFarPointerCallArgAddressesStackLValues verifies stack far
// pointer call args collapse to address-of source storage.
func TestCollapseTypedFarPointerCallArgAddressesStackLValues(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	pointType := &typeinfo.Struct{Name: "tagPOINT", SKind: typeinfo.StructKindStruct, Size: 4}
	pointPtrType := &typeinfo.Pointer{Elem: pointType, Class: typeinfo.PtrFar}
	rectType := &typeinfo.Struct{Name: "tagRECT", SKind: typeinfo.StructKindStruct, Size: 8}
	rectType.Fields = []typeinfo.StructField{
		{Name: "left", Type: int16Type, Offset: 0, Size: 2, End: 2},
		{Name: "top", Type: int16Type, Offset: 2, Size: 2, End: 4},
		{Name: "right", Type: int16Type, Offset: 4, Size: 2, End: 6},
		{Name: "bottom", Type: int16Type, Offset: 6, Size: 2, End: 8},
	}
	rc := &Local{FunctionVar: typeinfo.FunctionVar{Name: "rc", Type: rectType, BPOffset: -0x42}}
	right := &FieldAccess{Base: rc, Field: &rectType.Fields[2]}
	rgb := &Local{FunctionVar: typeinfo.FunctionVar{Name: "rgb", Type: &typeinfo.Array{Elem: charType, Count: 256}, BPOffset: -0x100}}
	rgbIndex := &ArrayIndex{Base: rgb, Index: &Const{TypeInfo: typeinfo.U16, U64: 1}, TypeInfo: charType}
	xfer := &Local{FunctionVar: typeinfo.FunctionVar{Name: "xfer", Type: typeinfo.U16, BPOffset: -0x20}}
	xferOffset := &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: xfer, RHS: &Const{TypeInfo: typeinfo.U16, U64: 4}}

	tests := []struct {
		name string
		arg  Expr
		want string
	}{
		{name: "rect", arg: rc, want: "&rc"},
		{name: "field", arg: right, want: "&rc.right"},
		{name: "address of", arg: &AddressOf{Target: rgbIndex, TypeInfo: typeinfo.U16}, want: "&rgb[0x1]"},
		{name: "offset arithmetic", arg: xferOffset, want: "(xfer + 0x4)"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			arg := &FarPointer{
				Part:    machine.FarPointerWhole,
				Segment: &RawValue{Value: machine.ScalarVal("ss"), TypeInfo: typeinfo.U16},
				Offset:  tt.arg,
			}

			got, changed := collapseTypedFarPointerCallArg(&FuncContext{dsReg: machine.ConstVal(0x25)}, arg, &typeinfo.FunctionVar{Type: pointPtrType})
			if !changed {
				t.Fatal("collapseTypedFarPointerCallArg changed = false, want true")
			}
			if gotText := FormatExpr(got); gotText != tt.want {
				t.Fatalf("collapsed arg text = %q, want %q", gotText, tt.want)
			}
		})
	}
}
