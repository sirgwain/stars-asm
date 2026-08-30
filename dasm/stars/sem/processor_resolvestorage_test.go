package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
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
