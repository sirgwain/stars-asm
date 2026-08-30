package typeinfo

import "testing"

// TestIsCallCompatible verifies ABI-shaped call compatibility without weakening equality.
func TestIsCallCompatible(t *testing.T) {
	int8Type := &Primitive{TypeKind: KInt, Name: "int8_t", Size: 1, Signed: true}
	charType := &Primitive{TypeKind: KInt, Name: "char", Size: 1, Signed: true}
	uint8Type := &Primitive{TypeKind: KInt, Name: "uint8_t", Size: 1, Signed: false}

	plType := plHeaderStruct("PL", "iMax", "iMac", uint8Type, uint8Type)
	plprodType := plHeaderStruct("PLPROD", "iprodMax", "iprodMac", uint8Type, uint8Type)
	signedCountHeader := plHeaderStruct("PLSIGNED", "iMax", "iMac", int8Type, uint8Type)

	tests := []struct {
		name   string
		formal Type
		actual Type
		want   bool
	}{
		{name: "exact", formal: int8Type, actual: int8Type, want: true},
		{name: "char int8", formal: charType, actual: int8Type, want: true},
		{name: "signed differs", formal: charType, actual: uint8Type, want: false},
		{name: "pointer to compatible header", formal: &Pointer{Elem: plType}, actual: &Pointer{Elem: plprodType}, want: true},
		{name: "pointer class differs", formal: &Pointer{Class: PtrNear, Elem: plType}, actual: &Pointer{Class: PtrFar, Elem: plprodType}, want: false},
		{name: "header field signedness differs", formal: &Pointer{Elem: plType}, actual: &Pointer{Elem: signedCountHeader}, want: false},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := IsCallCompatible(tt.formal, tt.actual); got != tt.want {
				t.Fatalf("IsCallCompatible() = %v, want %v", got, tt.want)
			}
		})
	}
}

// plHeaderStruct builds a realistic PL-like header fixture with a typed flexible array.
func plHeaderStruct(name, maxName, macName string, countType, flexElem Type) *Struct {
	uint16Type := &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}
	return &Struct{
		Name:  name,
		SKind: StructKindStruct,
		Size:  4,
		Fields: []StructField{
			{Name: "cbItem", Type: uint16Type, Offset: 0, Size: 2, End: 2, Bitfield: &Bitfield{BaseType: uint16Type, StorageSize: 2, BitOffset: 0, BitWidth: 8}},
			{Name: "fMark", Type: uint16Type, Offset: 0, Size: 2, End: 2, Bitfield: &Bitfield{BaseType: uint16Type, StorageSize: 2, BitOffset: 8, BitWidth: 1}},
			{Name: "ht", Type: uint16Type, Offset: 0, Size: 2, End: 2, Bitfield: &Bitfield{BaseType: uint16Type, StorageSize: 2, BitOffset: 9, BitWidth: 3}},
			{Name: "cAlloc", Type: uint16Type, Offset: 0, Size: 2, End: 2, Bitfield: &Bitfield{BaseType: uint16Type, StorageSize: 2, BitOffset: 12, BitWidth: 4}},
			{Name: maxName, Type: countType, Offset: 2, Size: countType.Bytes(), End: 2 + countType.Bytes()},
			{Name: macName, Type: countType, Offset: 3, Size: countType.Bytes(), End: 3 + countType.Bytes()},
			{Name: "rg", Type: &Array{Elem: flexElem, Count: 0}, Offset: 4, Size: 0, End: 4},
		},
	}
}
