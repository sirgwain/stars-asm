package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestAggregateCopyRecovery verifies complete PROD copies and the boundaries
// that prohibit coalescing partial, incompatible, padded, or interrupted copies.
func TestAggregateCopyRecovery(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "ProdCommandHandler")
	prod := fx.SDB.GetStruct("PROD")
	dst := testLocal("prod", prod)
	src := &ArrayIndex{Base: testLocal("items", &typeinfo.Array{Elem: prod, Count: 4}), Index: testLocal("index", typeinfo.I16), TypeInfo: prod}
	low := &Assign{Dst: &Part{Base: dst, Width: 2, TypeInfo: typeinfo.U16}, Src: &Word{Parent: src, Part: machine.WordLow}}
	high := &Assign{Dst: &Part{Base: dst, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, Src: &Word{Parent: src, Part: machine.WordHigh}}
	copy, ok := coalesceAggregateCopy(low, high)
	if !ok || !sameExpr(copy.Dst, dst) || !sameExpr(copy.Src, src) {
		t.Fatal("complete PROD copy did not coalesce")
	}
	p := resolveLateBitfieldsProcessor{ctx: ctx}
	block := Block{ID: 1, Effects: []Effect{low, &CallEffect{Call: &Call{Function: &typeinfo.Function{Name: "observe", Ret: typeinfo.U16}, Args: []Expr{&AddressOf{Target: dst, TypeInfo: &typeinfo.Pointer{Elem: prod}}}}}, high}}
	got, _ := p.ProcessBlock(newResult(ctx.fs), Func{Blocks: []Block{block}}, block)
	for _, effect := range got.Effects {
		if a, ok := effect.(*Assign); ok && sameExpr(a.Dst, dst) {
			t.Fatal("copy crossed an intervening call")
		}
	}
	other := *src
	other.TypeInfo = fx.SDB.GetStruct("HS")
	mismatch := *high
	mismatch.Src = &Word{Parent: &other, Part: machine.WordHigh}
	if _, ok := coalesceAggregateCopy(low, &mismatch); ok {
		t.Fatal("incompatible copy coalesced")
	}
	padded := &typeinfo.Struct{Name: "padded", Size: 4, Fields: []typeinfo.StructField{{Name: "first", Type: typeinfo.U8, Offset: 0, Size: 1, End: 1}, {Name: "last", Type: typeinfo.U16, Offset: 2, Size: 2, End: 4}}}
	if _, ok := padded.ScalarBitPartition(0, 32); ok {
		t.Fatal("padding treated as a field")
	}
	pointer := testLocal("ptr", &typeinfo.Pointer{Elem: prod, Class: typeinfo.PtrFar})
	if _, _, _, _, ok := aggregateStorageRange(&Part{Base: pointer, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}); ok {
		t.Fatal("pointer half treated as pointee storage")
	}
}

// TestAggregateWritesKeepUnresolvedSources ensures splitting does not turn one
// unresolved source into multiple writes through undefined value temporaries.
func TestAggregateWritesKeepUnresolvedSources(t *testing.T) {
	fx := testfixture.Stars(t)
	prod := fx.SDB.GetStruct("PROD")
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "ProdCommandHandler")
	p := resolveLateBitfieldsProcessor{ctx: ctx, names: map[string]bool{}}
	a := &Assign{Dst: &Part{Base: testLocal("prod", prod), Width: 2, TypeInfo: typeinfo.U16}, Src: &Register{Val: asm.RegAX}}
	if _, ok := p.expandAggregateWrite(a); ok {
		t.Fatal("unresolved register source was split")
	}
}
