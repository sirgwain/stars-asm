package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestFieldCopies verifies recovery of the expanded HS copy and rejects
// partial copies, interrupted stores, and snapshots read outside the copy.
func TestFieldCopies(t *testing.T) {
	fx := testfixture.Stars(t)
	hs := fx.SDB.GetStruct("HS")
	part := fx.SDB.GetStruct("PART")
	var hsField *typeinfo.StructField
	for i := range part.Fields {
		if part.Fields[i].Name == "hs" {
			hsField = &part.Fields[i]
		}
	}
	dst := &FieldAccess{Base: testLocal("part", part), Field: hsField}
	src := testLocal("lphs", &typeinfo.Pointer{Elem: hs, Class: typeinfo.PtrFar})
	address := &Temp{Name: "t_fields_1", TypeInfo: &typeinfo.Pointer{Elem: hs, Class: typeinfo.PtrNear}}
	value := &Temp{Name: "t_fields_3", TypeInfo: typeinfo.U32}
	field := func(base Expr, name string) *FieldAccess {
		for i := range hs.Fields {
			if hs.Fields[i].Name == name {
				return &FieldAccess{Base: base, Field: &hs.Fields[i]}
			}
		}
		t.Fatalf("missing HS field %s", name)
		return nil
	}
	copy := []Effect{
		&Assign{Dst: field(dst, "grhst"), Src: field(src, "grhst")},
		&Assign{Dst: address, Src: &AddressOf{Target: dst, TypeInfo: address.TypeInfo}},
		&Assign{Dst: value, Src: field(src, "cItem")},
		&Assign{Dst: field(address, "iItem"), Src: field(src, "iItem")},
		&Assign{Dst: field(address, "cItem"), Src: value},
	}
	observe := &typeinfo.Function{Name: "Observe", Ret: typeinfo.I16}
	call := &CallEffect{Call: &Call{Function: observe, Target: &FunctionRef{Function: observe}}}
	for _, tc := range []struct {
		name    string
		effects []Effect
		want    bool
	}{
		{"complete", copy, true},
		{"partial", copy[:4], false},
		{"interrupted", append(append([]Effect{}, copy[:3]...), append([]Effect{call}, copy[3:]...)...), false},
		{"extra snapshot use", append(append([]Effect{}, copy...), &Return{Value: value}), false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			f := &Func{Blocks: []Block{{ID: 1, Effects: append([]Effect{}, tc.effects...)}}}
			got := (&fieldCopiesProcessor{}).ProcessFunc(nil, f)
			if got != tc.want {
				t.Fatalf("recovered = %v, want %v", got, tc.want)
			}
			if got && (len(f.Blocks[0].Effects) != 1 || !sameExpr(f.Blocks[0].Effects[0].(*Assign).Dst, dst)) {
				t.Fatal("copy did not target complete HS object")
			}
		})
	}
}
