package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestResolveLateFieldMaskedLowByte(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "SlotDlg")
	global := fx.SDB.GetGlobal("shdefBuild")
	if global == nil {
		t.Fatal("shdefBuild global not found")
	}
	shdef, ok := global.Type.(*typeinfo.Struct)
	if !ok {
		t.Fatalf("shdefBuild type = %T, want struct", global.Type)
	}
	hulField := testStructFieldNamed(t, shdef, "hul")
	hul := &FieldAccess{Base: &Global{GlobalVar: global}, Field: hulField}
	part := &Part{Base: hul, ByteOff: 0x7a, Width: 2, TypeInfo: typeinfo.U16}
	expr := &Binary{
		TypeInfo: typeinfo.U16,
		Op:       OpAnd,
		LHS:      part,
		RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0xff},
	}

	got, changed := (&resolveLateFieldsProcessor{ctx: ctx}).rewriter().rewriteExpr(expr)
	if !changed {
		t.Fatal("masked low-byte field did not resolve")
	}
	if formatted := FormatExpr(got); formatted != "shdefBuild.hul.chs" {
		t.Fatalf("resolved field = %q, want shdefBuild.hul.chs", formatted)
	}
}

func TestResolveLateFieldSignExtendedByte(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "RaceCreationWizard")
	global := fx.SDB.GetGlobal("vplr")
	if global == nil {
		t.Fatal("vplr global not found")
	}
	part := &Part{Base: &Global{GlobalVar: global}, ByteOff: 0x19, Width: 2, TypeInfo: typeinfo.I16}
	expr := &SignExtend{Parent: part, FromBits: 8, ToBits: 16, TypeInfo: typeinfo.I16}

	got, changed := (&resolveLateFieldsProcessor{ctx: ctx}).rewriter().rewriteExpr(expr)
	if !changed {
		t.Fatal("sign-extended byte field did not resolve")
	}
	if formatted := FormatExpr(got); formatted != "sext8to16(vplr.pctIdealGrowth)" {
		t.Fatalf("resolved expression = %q, want sext8to16(vplr.pctIdealGrowth)", formatted)
	}
}

func TestResolveLateFieldNarrowWrite(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "RaceCreationWizard")
	global := fx.SDB.GetGlobal("vplr")
	if global == nil {
		t.Fatal("vplr global not found")
	}
	assign := &Assign{
		Dst: &Part{Base: &Global{GlobalVar: global}, ByteOff: 0x19, Width: 2, TypeInfo: typeinfo.I16},
		Src: &Const{TypeInfo: typeinfo.U8, U64: 17},
	}

	got, changed := (&resolveLateFieldsProcessor{ctx: ctx}).rewriter().rewriteEffect(assign)
	if !changed {
		t.Fatal("narrow field write did not resolve")
	}
	resolved := got.(*Assign)
	if formatted := FormatExpr(resolved.Dst); formatted != "vplr.pctIdealGrowth" {
		t.Fatalf("resolved destination = %q, want vplr.pctIdealGrowth", formatted)
	}
}

func TestResolveLateFieldDoesNotSelectBitfield(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "RaceCreationWizard")
	global := fx.SDB.GetGlobal("vplr")
	if global == nil {
		t.Fatal("vplr global not found")
	}

	// PLAYER + 0x04 is packed bitfield storage. This pass must leave it for
	// resolveLateBitfieldsProcessor rather than selecting one field by width.
	part := &Part{Base: &Global{GlobalVar: global}, ByteOff: 0x04, Width: 2, TypeInfo: typeinfo.U16}
	expr := &Binary{TypeInfo: typeinfo.U16, Op: OpAnd, LHS: part, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0xff}}
	if got, changed := (&resolveLateFieldsProcessor{ctx: ctx}).rewriter().rewriteExpr(expr); changed {
		t.Fatalf("ordinary-field pass selected packed storage: %s", FormatExpr(got))
	}
}

func testStructFieldNamed(t *testing.T, strct *typeinfo.Struct, name string) *typeinfo.StructField {
	t.Helper()
	for i := range strct.Fields {
		if strct.Fields[i].Name == name {
			return &strct.Fields[i]
		}
	}
	t.Fatalf("field %s.%s not found", strct.Name, name)
	return nil
}
