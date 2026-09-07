package ir

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
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

// TestLowerDeclaresSurvivingScratchSymbols verifies symbolic scratch storage
// that cannot be elided is emitted as a synthetic IR local.
func TestLowerDeclaresSurvivingScratchSymbols(t *testing.T) {
	scratch := &symresolve.SymbolScratch{
		Function:    typeinfo.Addr{Seg: 1, Off: 0x100},
		BPOffset:    -4,
		StorageSize: 2,
		TypeInfo:    typeinfo.U16,
	}
	src := sem.Func{Blocks: []sem.Block{
		{
			ID: machine.BlockID(0x100),
			Effects: []sem.Effect{
				&sem.Assign{
					Dst: &sem.SymbolRef{Path: scratch},
					Src: &sem.Const{TypeInfo: typeinfo.U16, U64: 1},
				},
			},
		},
	}}
	fn := &typeinfo.Function{Name: "ScratchUser", Ret: &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"}}

	got := Lower(src, fn)
	if len(got.Locals) != 1 {
		t.Fatalf("locals = %d, want 1", len(got.Locals))
	}
	if got.Locals[0].Name != "scratch_bp_m4" || got.Locals[0].Type != typeinfo.U16 {
		t.Fatalf("scratch local = %#v, want uint16_t scratch_bp_m4", got.Locals[0])
	}
}
