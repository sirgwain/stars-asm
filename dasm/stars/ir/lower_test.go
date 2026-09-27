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

// TestLowerByteReplacement verifies partial-register updates remain
// expressible after semantic lowering.
func TestLowerByteReplacement(t *testing.T) {
	dst := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "word", Type: typeinfo.U16}}
	src := &sem.Byte{
		Parent:   dst,
		Part:     machine.ByteHigh,
		Value:    &sem.Const{TypeInfo: typeinfo.U16, U64: 3},
		TypeInfo: typeinfo.U16,
	}
	fn := Lower(sem.Func{Blocks: []sem.Block{{
		ID:      0x100,
		Effects: []sem.Effect{&sem.Assign{Dst: dst, Src: src}},
	}}}, &typeinfo.Function{Name: "ReplaceByte", Ret: typeinfo.U16})

	assign, ok := fn.Blocks[0].Stmts[0].(*Assign)
	if !ok {
		t.Fatalf("stmt type = %T, want *Assign", fn.Blocks[0].Stmts[0])
	}
	replacement, ok := assign.Src.(*Binary)
	if !ok || replacement.Op != "|" {
		t.Fatalf("replacement = %#v, want bitwise-or expression", assign.Src)
	}
	insert, ok := replacement.RHS.(*Binary)
	if !ok || insert.Op != "<<" {
		t.Fatalf("insert = %#v, want shifted high-byte expression", replacement.RHS)
	}
}

// TestLowerRecoveredStorage emits recovered locals and byte/word views without
// dropping partial scratch writes or explicit two-word reconstructions.
func TestLowerRecoveredStorage(t *testing.T) {
	local := &typeinfo.FunctionVar{Name: "t_buffer", Type: &typeinfo.Array{Elem: typeinfo.U8, Count: 16}, BPOffset: -32}
	tmp := &sem.Temp{Name: "t_scratch_m4", TypeInfo: typeinfo.U32}
	fn := Lower(sem.Func{RecoveredLocals: []*typeinfo.FunctionVar{local}, Blocks: []sem.Block{{ID: 1, Effects: []sem.Effect{
		&sem.Assign{Dst: tmp, Src: &sem.Words{Words: []sem.Expr{&sem.Const{TypeInfo: typeinfo.U16, U64: 2}, &sem.Const{TypeInfo: typeinfo.U16, U64: 1}}}},
		&sem.Assign{Dst: &sem.Part{Base: tmp, ByteOff: 3, Width: 1, TypeInfo: typeinfo.U8}, Src: &sem.Const{TypeInfo: typeinfo.U8, U64: 7}},
		&sem.Return{Value: tmp},
	}}}}, &typeinfo.Function{Name: "RecoveredStorage", Ret: typeinfo.U32})
	if fn.Analyze().Untranslated != 0 || len(fn.Locals) != 2 {
		t.Fatal("recovered storage did not lower completely")
	}
	if fn.Locals[0].Name != local.Name || fn.Locals[0].Type != local.Type {
		t.Fatal("recovered local declaration lost its type")
	}
	assign := fn.Blocks[0].Stmts[1].(*Assign)
	if assign.Dst.(*Var).Name != tmp.Name {
		t.Fatal("partial write did not target containing scalar")
	}
	replacement := assign.Src.(*Binary)
	if replacement.LHS.(*Binary).RHS.(*IntConst).Value != 0x00ffffff {
		t.Fatal("wrong preserve mask")
	}

}

// TestAnalyzeReportsUnsupportedNodePath verifies untranslated metrics describe
// the semantic node that actually prevented lowering.
func TestAnalyzeReportsUnsupportedNodePath(t *testing.T) {
	value := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "value", Type: typeinfo.U32}}
	dst := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "word", Type: typeinfo.U16}}
	fn := Lower(sem.Func{Blocks: []sem.Block{{
		ID: 0x100,
		Effects: []sem.Effect{&sem.Assign{
			Dst: dst,
			Src: &sem.Part{Base: value, ByteOff: 4, Width: 2, TypeInfo: typeinfo.U16},
		}},
	}}}, &typeinfo.Function{Name: "UnsupportedPart", Ret: typeinfo.U16})

	got := fn.Analyze()
	if got.Untranslated != 1 || got.UntranslatedAssign != 1 || got.UntranslatedPart != 1 {
		t.Fatalf("analysis counts = %#v", got)
	}
	if count := got.UntranslatedFailures["assign.src:part"]; count != 1 {
		t.Fatalf("assign.src:part count = %d, want 1", count)
	}
}

// TestLowerTableJumpPreservesDispatch verifies table jumps lower into explicit
// IR control flow with their index and repeated destinations intact.
func TestLowerTableJumpPreservesDispatch(t *testing.T) {
	index := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "index", Type: typeinfo.U16}}
	src := sem.Func{Blocks: []sem.Block{
		{ID: 0x1000, Effects: []sem.Effect{&sem.TableJump{Index: index, Targets: []machine.BlockID{0x1010, 0x1020, 0x1010}}}},
		{ID: 0x1010, Effects: []sem.Effect{&sem.Return{}}},
		{ID: 0x1020, Effects: []sem.Effect{&sem.Return{}}},
	}}
	fn := &typeinfo.Function{Name: "Dispatch", Ret: &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"}, Vars: []typeinfo.FunctionVar{index.FunctionVar}}
	got := Lower(src, fn)
	jump, ok := got.Blocks[0].Stmts[0].(*TableJump)
	if !ok {
		t.Fatalf("statement = %T", got.Blocks[0].Stmts[0])
	}
	if jump.Index.(*Var).Name != "index" || len(jump.Labels) != 3 || jump.Labels[0] != "L_1010" || jump.Labels[1] != "L_1020" || jump.Labels[2] != "L_1010" {
		t.Fatalf("table jump = %#v", jump)
	}
}

// TestLowerScalarArraySlices lowers word and dword ranges of integer arrays as
// element stores when a constant covers aligned elements, and as raw typed
// accesses otherwise.
func TestLowerScalarArraySlices(t *testing.T) {
	rgb := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "rgb", Type: &typeinfo.Array{Elem: typeinfo.U8, Count: 8}}}
	rgi := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "rgi", Type: &typeinfo.Array{Elem: typeinfo.I16, Count: 4}}}
	lSerial := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "lSerial", Type: typeinfo.I32}}
	fn := Lower(sem.Func{Blocks: []sem.Block{{ID: 1, Effects: []sem.Effect{
		&sem.Assign{Dst: &sem.Part{Base: rgi, ByteOff: 2, Width: 4, TypeInfo: typeinfo.U32}, Src: &sem.Const{TypeInfo: typeinfo.U32, U64: 0x1f40064}},
		&sem.Assign{Dst: &sem.Part{Base: rgb, ByteOff: 0, Width: 4, TypeInfo: typeinfo.U32}, Src: lSerial},
		&sem.Assign{Dst: lSerial, Src: &sem.Part{Base: rgb, ByteOff: 4, Width: 4, TypeInfo: typeinfo.U32}},
	}}}}, &typeinfo.Function{Name: "Slices", Ret: typeinfo.U16, Vars: []typeinfo.FunctionVar{rgb.FunctionVar, rgi.FunctionVar, lSerial.FunctionVar}})
	stmts := fn.Blocks[0].Stmts
	if len(stmts) != 4 {
		t.Fatalf("statements = %d, want 4", len(stmts))
	}
	for i, want := range []struct{ index, value uint64 }{{1, 100}, {2, 500}} {
		assign := stmts[i].(*Assign)
		index := assign.Dst.(*Index)
		if index.Base.(*Var).Name != "rgi" || index.Index.(*IntConst).Value != want.index || assign.Src.(*IntConst).Value != want.value {
			t.Fatalf("element store %d = %#v", i, assign)
		}
	}
	store := stmts[2].(*Assign).Dst.(*Deref)
	if store.Pointer.(*Var).Name != "rgb" || store.ByteOff != 0 || !typeinfo.Equals(store.Type, typeinfo.U32) {
		t.Fatalf("raw store = %#v", store)
	}
	load := stmts[3].(*Assign).Src.(*Deref)
	if load.Pointer.(*Var).Name != "rgb" || load.ByteOff != 4 || !typeinfo.Equals(load.Type, typeinfo.U32) {
		t.Fatalf("raw load = %#v", load)
	}
	if fn.Analyze().Untranslated != 0 {
		t.Fatal("scalar array slices left untranslated")
	}
}

// TestLowerRejectsUnresolvedWrites keeps aggregate and pointer fragments, invalid
// nested ranges, and symbolic arithmetic out of emitted assignment destinations.
func TestLowerRejectsUnresolvedWrites(t *testing.T) {
	word := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "word", Type: typeinfo.U32}}
	point := &typeinfo.Struct{Name: "POINT", SKind: typeinfo.StructKindStruct, Size: 4}
	array := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "rgpt", Type: &typeinfo.Array{Elem: point, Count: 2}}}
	pointer := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "pointer", Type: &typeinfo.Pointer{Elem: typeinfo.U16, Class: typeinfo.PtrFar}}}
	tests := []struct {
		name    string
		dst     sem.LValue
		failure string
	}{
		{"aggregate", &sem.Part{Base: array, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, "aggregate-slice"},
		{"pointer", &sem.Part{Base: pointer, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, "pointer-fragment"},
		{"nested range", &sem.Part{Base: &sem.Part{Base: word, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, ByteOff: 1, Width: 2, TypeInfo: typeinfo.U16}, "part"},
		{"symbolic offset", &sem.SymbolRef{Path: &symresolve.SymbolOffset{Base: &symresolve.SymbolRoot{Symbol: &word.FunctionVar}, Offset: 1, Result: typeinfo.U8}}, "invalid-destination"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			fn := Lower(sem.Func{Blocks: []sem.Block{{ID: 1, Effects: []sem.Effect{&sem.Assign{Dst: tt.dst, Src: &sem.Const{TypeInfo: typeinfo.U16, U64: 0x103}}}}}}, &typeinfo.Function{Name: "InvalidWrite", Ret: typeinfo.U16, Vars: []typeinfo.FunctionVar{word.FunctionVar, array.FunctionVar, pointer.FunctionVar}})
			if _, ok := fn.Blocks[0].Stmts[0].(*Comment); !ok {
				t.Fatal("invalid destination emitted as a write")
			}
			if fn.Analyze().UntranslatedFailures["assign.dst:"+tt.failure] == 0 {
				t.Fatalf("missing %s diagnostic", tt.failure)
			}
		})
	}
}
