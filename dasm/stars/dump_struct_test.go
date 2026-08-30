package stars

import (
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestTopoSortStructsOrdersHardDependencies(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2}
	payload := &typeinfo.Struct{
		Name: "_payload",
		Size: 2,
		Fields: []typeinfo.StructField{
			{Name: "value", Type: int16Type, Offset: 0, Size: 2, End: 2},
		},
	}
	wrapper := &typeinfo.Struct{
		Name: "_wrapper",
		Size: 4,
		Fields: []typeinfo.StructField{
			{Name: "items", Type: &typeinfo.Array{Elem: payload, Count: 2}, Offset: 0, Size: 4, End: 4},
		},
	}
	owner := &typeinfo.Struct{
		Name: "_owner",
		Size: 2,
		Fields: []typeinfo.StructField{
			{Name: "next", Type: &typeinfo.Pointer{Elem: payload}, Offset: 0, Size: 2, End: 2},
		},
	}

	got, err := topoSortStructs([]*typeinfo.Struct{wrapper, owner, payload})
	if err != nil {
		t.Fatalf("topoSortStructs() error = %v", err)
	}

	if got[0] != owner || got[1] != payload || got[2] != wrapper {
		t.Fatalf("topoSortStructs() = %v, want owner, payload, wrapper", structNames(got))
	}
}

func TestTopoSortStructsReportsHardCycle(t *testing.T) {
	a := &typeinfo.Struct{Name: "_a", Size: 2}
	b := &typeinfo.Struct{Name: "_b", Size: 2}
	a.Fields = []typeinfo.StructField{{Name: "b", Type: b, Offset: 0, Size: 2, End: 2}}
	b.Fields = []typeinfo.StructField{{Name: "a", Type: a, Offset: 0, Size: 2, End: 2}}

	_, err := topoSortStructs([]*typeinfo.Struct{a, b})
	if err == nil {
		t.Fatal("topoSortStructs() error = nil, want cycle error")
	}
	if !strings.Contains(err.Error(), "_a") || !strings.Contains(err.Error(), "_b") {
		t.Fatalf("topoSortStructs() error = %q, want both cycle names", err)
	}
}

func structNames(structs []*typeinfo.Struct) []string {
	names := make([]string, 0, len(structs))
	for _, s := range structs {
		names = append(names, s.String())
	}
	return names
}
