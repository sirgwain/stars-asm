package ir

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestLowerAddressOfArrayDecaysToPointer verifies typed address-of array
// expressions lower as the array base in pointer assignment context.
func TestLowerAddressOfArrayDecaysToPointer(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	charArrayType := &typeinfo.Array{Elem: charType, Count: 96}
	charPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	szT := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "szT", Type: charArrayType}}
	tmp := &sem.Temp{Name: "t_merge_2766_0001_wide", TypeInfo: charPtrType}
	src := sem.Func{Blocks: []sem.Block{
		{
			ID: machine.BlockID(0x2746),
			Effects: []sem.Effect{
				&sem.Assign{
					Dst: tmp,
					Src: &sem.AddressOf{Target: szT, TypeInfo: charPtrType},
				},
			},
		},
	}}
	fn := &typeinfo.Function{Name: "DrawVCR", Ret: &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"}}

	got := Lower(src, fn)
	assign, ok := got.Blocks[0].Stmts[0].(*Assign)
	if !ok {
		t.Fatalf("stmt type = %T, want *Assign", got.Blocks[0].Stmts[0])
	}
	v, ok := assign.Src.(*Var)
	if !ok {
		t.Fatalf("src type = %T, want *Var", assign.Src)
	}
	if v.Name != "szT" {
		t.Fatalf("src var = %q, want %q", v.Name, "szT")
	}
}
