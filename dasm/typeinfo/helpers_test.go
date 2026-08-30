package typeinfo

import "testing"

func TestIsFarPointer(t *testing.T) {
	int16Type := &Primitive{TypeKind: KInt, Name: "int16_t", Size: 2, Signed: true}

	tests := []struct {
		name string
		typ  Type
		want bool
	}{
		{name: "near pointer", typ: &Pointer{Class: PtrNear, Elem: int16Type}, want: false},
		{name: "far pointer", typ: &Pointer{Class: PtrFar, Elem: int16Type}, want: true},
		{name: "huge pointer", typ: &Pointer{Class: PtrHuge, Elem: int16Type}, want: true},
		{name: "primitive", typ: int16Type, want: false},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := IsFarPointer(tt.typ); got != tt.want {
				t.Fatalf("IsFarPointer() = %v, want %v", got, tt.want)
			}
		})
	}
}
