package typeinfo

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/nb09"
)

func TestPrimitiveType(t *testing.T) {
	p := Primitive{TypeKind: KInt, Name: "int16", Size: 2, Signed: true}

	if p.Kind() != KInt {
		t.Fatalf("Kind() = %v, want %v", p.Kind(), KInt)
	}
	if p.Bytes() != 2 {
		t.Fatalf("Bytes() = %d, want 2", p.Bytes())
	}
}

func TestPrimitiveTypeFromCodeViewTypind(t *testing.T) {
	tests := []struct {
		typind uint16
		kind   Kind
		name   string
		size   int
		signed bool
	}{
		{typind: 0x0000, kind: KInvalid, name: "notype"},
		{typind: 0x0001, kind: KInvalid, name: "notype"},
		{typind: 0x0002, kind: KInvalid, name: "notype"},
		{typind: 0x0003, kind: KVoid, name: "void"},
		{typind: 0x0010, kind: KInt, name: "int8_t", size: 1, signed: true},
		{typind: 0x0020, kind: KInt, name: "uint8_t", size: 1},
		{typind: 0x0070, kind: KInt, name: "char", size: 1, signed: true},
		{typind: 0x0071, kind: KInt, name: "uint16_t", size: 2},
		{typind: 0x0011, kind: KInt, name: "int16_t", size: 2, signed: true},
		{typind: 0x0021, kind: KInt, name: "uint16_t", size: 2},
		{typind: 0x0012, kind: KInt, name: "int32_t", size: 4, signed: true},
		{typind: 0x0022, kind: KInt, name: "uint32_t", size: 4},
		{typind: 0x0072, kind: KInt, name: "int16_t", size: 2, signed: true},
		{typind: 0x0073, kind: KInt, name: "uint16_t", size: 2},
		{typind: 0x0074, kind: KInt, name: "int32_t", size: 4, signed: true},
		{typind: 0x0075, kind: KInt, name: "uint32_t", size: 4},
		{typind: 0x0040, kind: KFloat, name: "float", size: 4},
		{typind: 0x0041, kind: KFloat, name: "double", size: 8},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			typ, ok := primitiveType(tt.typind).(*Primitive)
			if !ok {
				t.Fatalf("primitiveType(0x%04x) = %T, want *Primitive", tt.typind, primitiveType(tt.typind))
			}
			if typ.Kind() != tt.kind {
				t.Fatalf("Kind() = %v, want %v", typ.Kind(), tt.kind)
			}
			if typ.Name != tt.name {
				t.Fatalf("Name = %q, want %q", typ.Name, tt.name)
			}
			if typ.Bytes() != tt.size {
				t.Fatalf("Bytes() = %d, want %d", typ.Bytes(), tt.size)
			}
			if typ.Signed != tt.signed {
				t.Fatalf("Signed = %v, want %v", typ.Signed, tt.signed)
			}
		})
	}
}

func TestPrimitiveTypePointerMode(t *testing.T) {
	tests := []struct {
		typind uint16
		class  PtrClass
		bytes  int
	}{
		{typind: 0x0172, class: PtrNear, bytes: 2},
		{typind: 0x0272, class: PtrFar, bytes: 4},
		{typind: 0x0372, class: PtrHuge, bytes: 4},
		{typind: 0x0472, class: PtrFar, bytes: 4},
		{typind: 0x0572, class: PtrFar, bytes: 4},
	}

	for _, tt := range tests {
		ptr, ok := primitiveType(tt.typind).(*Pointer)
		if !ok {
			t.Fatalf("primitiveType(0x%04x) = %T, want *Pointer", tt.typind, primitiveType(tt.typind))
		}
		if ptr.Class != tt.class {
			t.Fatalf("Class = %v, want %v", ptr.Class, tt.class)
		}
		if ptr.Bytes() != tt.bytes {
			t.Fatalf("Bytes() = %d, want %d", ptr.Bytes(), tt.bytes)
		}
		elem, ok := ptr.Elem.(*Primitive)
		if !ok {
			t.Fatalf("Elem = %T, want *Primitive", ptr.Elem)
		}
		if elem.Name != "int16_t" {
			t.Fatalf("Elem name = %q, want int16_t", elem.Name)
		}
	}
}

func TestPointerBytes(t *testing.T) {
	tests := []struct {
		name string
		ptr  Pointer
		want int
	}{
		{name: "default", ptr: Pointer{}, want: 2},
		{name: "near", ptr: Pointer{Class: PtrNear}, want: 2},
		{name: "far", ptr: Pointer{Class: PtrFar}, want: 4},
		{name: "huge", ptr: Pointer{Class: PtrHuge}, want: 4},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if tt.ptr.Kind() != KPointer {
				t.Fatalf("Kind() = %v, want %v", tt.ptr.Kind(), KPointer)
			}
			if tt.ptr.Bytes() != tt.want {
				t.Fatalf("Bytes() = %d, want %d", tt.ptr.Bytes(), tt.want)
			}
		})
	}
}

func TestTypeStrings(t *testing.T) {
	int16Type := &Primitive{TypeKind: KInt, Name: "int16_t", Size: 2, Signed: true}
	uint16Type := &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}
	rectType := &Struct{Name: "tagRECT", SKind: StructKindStruct, Size: 8}

	tests := []struct {
		name string
		typ  Type
		want string
	}{
		{name: "primitive", typ: int16Type, want: "int16_t"},
		{name: "pointer", typ: &Pointer{Class: PtrFar, Elem: rectType}, want: "tagRECT *"},
		{name: "array", typ: &Array{Elem: uint16Type, Count: 4}, want: "uint16_t[4]"},
		{name: "function", typ: &Function{
			Conv: CCCdecl,
			Ret:  int16Type,
			Params: []FunctionVar{
				{Name: "hwnd", Type: uint16Type},
				{Name: "rect", Type: &Pointer{Class: PtrNear, Elem: rectType}},
			},
		}, want: "int16_t(uint16_t hwnd, tagRECT *rect)"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := tt.typ.String(); got != tt.want {
				t.Fatalf("String() = %q, want %q", got, tt.want)
			}
		})
	}
}

func TestTypeDeclFunctionPointers(t *testing.T) {
	uint16Type := &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}
	tileType := &Struct{Name: "_tile", Typedef: "TILE", SKind: StructKindStruct, Size: 16}
	objType := &Struct{Name: "_obj", Typedef: "OBJ", SKind: StructKindStruct, Size: 2}
	fn := &Function{
		Ret: &Primitive{TypeKind: KVoid, Name: "void"},
		Params: []FunctionVar{
			{Type: uint16Type},
			{Type: &Pointer{Elem: tileType}},
			{Type: objType},
		},
	}

	tests := []struct {
		name string
		typ  Type
		want string
	}{
		{name: "function", typ: fn, want: "void pfn(uint16_t, TILE *, OBJ)"},
		{name: "function pointer", typ: &Pointer{Elem: fn}, want: "void (**pfn)(uint16_t, TILE *, OBJ)"},
		{name: "function pointer pointer", typ: &Pointer{Elem: &Pointer{Elem: fn, Class: PtrFar}, Class: PtrFar}, want: "void (***pfn)(uint16_t, TILE *, OBJ)"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := TypeDecl(tt.typ, "pfn"); got != tt.want {
				t.Fatalf("TypeDecl() = %q, want %q", got, tt.want)
			}
		})
	}
}

func TestArrayBytes(t *testing.T) {
	elem := &Primitive{TypeKind: KInt, Size: 2}
	a := Array{Elem: elem, Count: 3}

	if a.Kind() != KArray {
		t.Fatalf("Kind() = %v, want %v", a.Kind(), KArray)
	}
	if a.Bytes() != 6 {
		t.Fatalf("Bytes() = %d, want 6", a.Bytes())
	}

	if got := (&Array{Count: 3}).Bytes(); got != 0 {
		t.Fatalf("nil element Bytes() = %d, want 0", got)
	}
}

// TestEquals verifies type equality across the concrete typeinfo implementations.
func TestEquals(t *testing.T) {
	int16Type := &Primitive{TypeKind: KInt, Name: "int16_t", Size: 2, Signed: true}
	uint16Type := &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}
	rectType := &Struct{Name: "tagRECT", SKind: StructKindStruct, Size: 8}
	otherRectType := &Struct{Name: "tagRECT", SKind: StructKindStruct, Size: 8}
	enumType := &Enum{
		Name:     "MessageBoxFlags",
		EnumKind: EnumFlags,
		Values: []EnumValue{
			{Name: "MB_OK", Value: 0},
			{Name: "MB_YESNO", Value: 4},
		},
	}
	fn := &Function{
		Conv: CCCdecl,
		Ret:  int16Type,
		Params: []FunctionVar{
			{Name: "hwnd", Type: &Pointer{Class: PtrNear, Elem: rectType}, BPOffset: 4},
			{Name: "flags", Type: enumType, BPOffset: 6},
		},
	}

	tests := []struct {
		name string
		a    Type
		b    Type
		want bool
	}{
		{name: "nil", want: true},
		{name: "primitive", a: int16Type, b: &Primitive{TypeKind: KInt, Name: "int16_t", Size: 2, Signed: true}, want: true},
		{name: "primitive signed differs", a: int16Type, b: uint16Type, want: false},
		{name: "pointer", a: &Pointer{Class: PtrNear, Elem: rectType}, b: &Pointer{Class: PtrNear, Elem: otherRectType}, want: true},
		{name: "pointer class differs", a: &Pointer{Class: PtrNear, Elem: rectType}, b: &Pointer{Class: PtrFar, Elem: otherRectType}, want: false},
		{name: "array", a: &Array{Elem: uint16Type, Count: 4}, b: &Array{Elem: &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}, Count: 4}, want: true},
		{name: "array count differs", a: &Array{Elem: uint16Type, Count: 4}, b: &Array{Elem: uint16Type, Count: 5}, want: false},
		{name: "struct nominal", a: rectType, b: otherRectType, want: true},
		{name: "struct size differs", a: rectType, b: &Struct{Name: "tagRECT", SKind: StructKindStruct, Size: 16}, want: false},
		{name: "enum", a: enumType, b: &Enum{Name: "MessageBoxFlags", EnumKind: EnumFlags, Values: []EnumValue{{Name: "MB_OK", Value: 0}, {Name: "MB_YESNO", Value: 4}}}, want: true},
		{name: "enum size differs", a: enumType, b: &Enum{Name: "MessageBoxFlags", EnumKind: EnumFlags, Size: 4, Values: []EnumValue{{Name: "MB_OK", Value: 0}, {Name: "MB_YESNO", Value: 4}}}, want: false},
		{name: "enum values differ", a: enumType, b: &Enum{Name: "MessageBoxFlags", EnumKind: EnumFlags, Values: []EnumValue{{Name: "MB_OK", Value: 0}}}, want: false},
		{name: "function ignores param metadata", a: fn, b: &Function{Conv: CCCdecl, Ret: int16Type, Params: []FunctionVar{{Name: "rect", Type: &Pointer{Class: PtrNear, Elem: otherRectType}, BPOffset: 8}, {Name: "options", Type: enumType, BPOffset: 10}}}, want: true},
		{name: "function varargs differ", a: fn, b: &Function{Conv: CCCdecl, Ret: int16Type, VarArgs: true, Params: fn.Params}, want: false},
		{name: "different concrete types", a: int16Type, b: &Pointer{Elem: int16Type}, want: false},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := Equals(tt.a, tt.b); got != tt.want {
				t.Fatalf("Equals() = %v, want %v", got, tt.want)
			}
			if got := Equals(tt.b, tt.a); got != tt.want {
				t.Fatalf("Equals() reverse = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestTypeResolverFromNB09TypeStream(t *testing.T) {
	ts := nb09.TypeStream{
		BaseIndex: 0x1000,
		Records: []nb09.TypeRecord{
			{
				Leaf: nb09.LF_ARGLIST,
				Parsed: &nb09.LFArgList{
					Args: []uint16{0x0072, 0x1002},
				},
			},
			{
				Leaf: nb09.LF_PROCEDURE,
				Parsed: &nb09.LFProcedure{
					RetType: 0x0003,
					ArgList: 0x1000,
				},
			},
			{
				Leaf: nb09.LF_POINTER,
				Parsed: &nb09.LFPointer{
					UType: 0x0073,
					Attr:  0x0001,
				},
			},
			{
				Leaf: nb09.LF_ARRAY,
				Parsed: &nb09.LFArray{
					ElemType: 0x0072,
					Length:   nb09.NumericLeaf{Value: 6},
				},
			},
			{
				Leaf: nb09.LF_FIELDLIST,
				Parsed: &nb09.LFFieldList{
					Fields: []nb09.FieldEntry{
						{
							Leaf: nb09.LF_MEMBER,
							Parsed: &nb09.LFMember{
								Type:   0x0072,
								Offset: nb09.NumericLeaf{Value: 0},
								Name:   "x",
							},
						},
					},
				},
			},
			{
				Leaf: nb09.LF_STRUCTURE,
				Parsed: &nb09.LFStruct{
					FieldList: 0x1004,
					Size:      nb09.NumericLeaf{Value: 2},
					Name:      "POINT",
				},
			},
		},
	}

	resolver := newTypeResolver(ts)

	for i := range ts.Records {
		typind := ts.BaseIndex + uint16(i)
		switch ts.Records[i].Leaf {
		case nb09.LF_ARGLIST, nb09.LF_FIELDLIST:
			continue
		}
		if _, ok := resolver.types[typind]; !ok {
			t.Fatalf("typind 0x%04x was not pre-resolved", typind)
		}
	}

	fn, ok := resolver.getType(0x1001).(*Function)
	if !ok {
		t.Fatalf("procedure type = %T, want *Function", resolver.getType(0x1001))
	}
	if fn.Ret.Kind() != KVoid {
		t.Fatalf("function ret kind = %v, want %v", fn.Ret.Kind(), KVoid)
	}
	if len(fn.Params) != 2 {
		t.Fatalf("function params len = %d, want 2", len(fn.Params))
	}
	if fn.Params[1].Type.Kind() != KPointer {
		t.Fatalf("second param kind = %v, want %v", fn.Params[1].Type.Kind(), KPointer)
	}

	array, ok := resolver.getType(0x1003).(*Array)
	if !ok {
		t.Fatalf("array type = %T, want *Array", resolver.getType(0x1003))
	}
	if array.Count != 3 {
		t.Fatalf("array Count = %d, want 3", array.Count)
	}
	if array.Bytes() != 6 {
		t.Fatalf("array Bytes() = %d, want 6", array.Bytes())
	}

	strct, ok := resolver.getType(0x1005).(*Struct)
	if !ok {
		t.Fatalf("struct type = %T, want *Struct", resolver.getType(0x1005))
	}
	if strct.Kind() != KStruct {
		t.Fatalf("struct Kind() = %v, want %v", strct.Kind(), KStruct)
	}
	if len(strct.Fields) != 1 || strct.Fields[0].Name != "x" {
		t.Fatalf("struct fields = %#v, want field x", strct.Fields)
	}
}
