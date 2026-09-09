package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestAggregateBufferCopyRecovery verifies whole RTBOF loads are reinterpreted
// at their recorded width while partial copies remain unresolved.
func TestAggregateBufferCopyRecovery(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "FOpenFile")
	rtbof := localNamed(t, ctx.fs, "rtbof")
	buffer := &Global{GlobalVar: fx.SDB.GetGlobal("rgbCur")}
	for _, width := range []int{16, 14} {
		t.Run(intTypeForWidth(width).String(), func(t *testing.T) {
			part := &Part{Base: buffer, Width: width, TypeInfo: intTypeForWidth(width)}
			got := recoverExpectedValue(part, rtbof.ExprType())
			if width != 16 {
				if got != part {
					t.Fatal("partial copy was expanded into a whole RTBOF")
				}
				return
			}
			load, ok := got.(*Deref)
			if !ok || load.Width != 16 || !typeinfo.Equals(load.TypeInfo, rtbof.ExprType()) {
				t.Fatalf("copy = %s, want a 16-byte RTBOF dereference", FormatExpr(got))
			}
			cast, ok := load.Pointer.(*Cast)
			if !ok || cast.Value != buffer {
				t.Fatalf("copy pointer = %s, want a cast of rgbCur", FormatExpr(load.Pointer))
			}
		})
	}
}

// TestAggregateCopyThroughFarPointer verifies FReadFleet's REP MOVSW writes
// the ORDER pointed to by lpord and loads the same aggregate from rgbCur.
func TestAggregateCopyThroughFarPointer(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "FReadFleet")
	lpord := localNamed(t, ctx.fs, "lpord")
	pointer := machine.LoadVal(machine.MemoryAddress{Base: machine.FrameBaseVal(), Disp: lpord.BPOffset, Width: 4})
	buffer := fx.SDB.GetGlobal("rgbCur")
	effect := machine.CopyEffect{
		MetaInfo: machine.Meta{BlockID: 0x3f0b, InstOff: 0x3f14},
		Dst: machine.AddressVal(machine.MemoryAddress{
			Seg: machine.FarPointerVal(pointer, machine.FarPointerSegment), Base: machine.FarPointerVal(pointer, machine.FarPointerOffset), Width: 18,
		}),
		Src:   machine.AddressVal(machine.MemoryAddress{Seg: machine.ConstVal(uint(buffer.Addr.Seg)), Disp: int(buffer.Addr.Off), Width: 18}),
		Width: 18,
	}
	got := (&machineConverter{ctx: ctx}).convertEffect(effect).(*Assign)
	dst, ok := got.Dst.(*Deref)
	if !ok || FormatExpr(dst.Pointer) != "lpord" || dst.Width != 18 {
		t.Fatalf("destination = %s, want *lpord", FormatExpr(got.Dst))
	}
	src, ok := got.Src.(*Deref)
	if !ok || src.Width != 18 || !typeinfo.Equals(src.TypeInfo, dst.TypeInfo) {
		t.Fatalf("source = %s, want an ORDER load", FormatExpr(got.Src))
	}
	previous := *effect.Dst.(*machine.Address)
	previous.Addr.Disp = 0xffee
	loaded, ok := (&machineConverter{ctx: ctx}).convertCopyAddress(&previous, 18)
	if !ok || FormatExpr(loaded) != "*(lpord + neg(1))" {
		t.Fatalf("previous ORDER = %s, want a one-element backward step", FormatExpr(loaded))
	}
	previous.Addr.Seg = machine.LoadVal(machine.MemoryAddress{Base: machine.FrameBaseVal(), Disp: lpord.BPOffset + 2, Width: 2})
	previous.Addr.Base = machine.LoadVal(machine.MemoryAddress{Base: machine.FrameBaseVal(), Disp: lpord.BPOffset, Width: 2})
	effect.Src = &previous
	got = (&machineConverter{ctx: ctx}).convertEffect(effect).(*Assign)
	if FormatExpr(got.Src) != "*(lpord + neg(1))" {
		t.Fatalf("split-word previous ORDER = %s, want a one-element backward step", FormatExpr(got.Src))
	}
}

// TestAddressRecoveryUsesExpectedPointee verifies union addresses select the
// unique typed field and unresolved union addresses retain their byte offset.
func TestAddressRecoveryUsesExpectedPointee(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "FCheckXferWP")
	ord := localNamed(t, ctx.fs, "ord")
	pia := localNamed(t, ctx.fs, "piaCur")
	assign := &Assign{Dst: pia, Src: &AddressOf{
		Target: &Part{Base: ord, ByteOff: 8, Width: 0, TypeInfo: typeinfo.U16}, TypeInfo: typeinfo.U16,
	}}
	got, changed := (&resolveLateAddressesProcessor{ctx: ctx}).rewriter().rewriteEffect(assign)
	if !changed || FormatEffect(got) != "piaCur = ord.txp.rgia" {
		t.Fatalf("address assignment = %s, want piaCur = ord.txp.rgia", FormatEffect(got))
	}
	xferType := fx.SDB.GetStruct("XFER")
	xfer := &Local{FunctionVar: typeinfo.FunctionVar{Name: "xfer", Type: xferType, BPOffset: -xferType.Bytes()}}
	address := &AddressOf{Target: &Part{Base: xfer, ByteOff: 4, Width: 0, TypeInfo: typeinfo.U16}, TypeInfo: &typeinfo.Pointer{Elem: &typeinfo.Primitive{Name: "void", TypeKind: typeinfo.KVoid}, Class: typeinfo.PtrNear}}
	resolved, ok := (&resolveLateAddressesProcessor{ctx: ctx}).resolveAddressOfPart(address)
	if !ok || FormatExpr(resolved) != "ptroff(&xfer, 4)" {
		t.Fatalf("ambiguous union address = %s, want byte offset from &xfer", FormatExpr(resolved))
	}
	buffer := &Global{GlobalVar: fx.SDB.GetGlobal("rgbCur")}
	bytePointer := &typeinfo.Pointer{Elem: buffer.ExprType().(*typeinfo.Array).Elem, Class: typeinfo.PtrNear}
	interior := &AddressOf{Target: &Part{Base: buffer, ByteOff: 3, Width: 0, TypeInfo: typeinfo.U16}, TypeInfo: bytePointer}
	resolved, ok = (&resolveLateAddressesProcessor{ctx: ctx}).resolveAddressOfPart(interior)
	if !ok || FormatExpr(resolved) != "(rgbCur + 0x3)" {
		t.Fatalf("array address = %s, want rgbCur + 3", FormatExpr(resolved))
	}
	field := &AddressOf{Target: &Part{Base: ord, ByteOff: 4, Width: 0, TypeInfo: typeinfo.U16}, TypeInfo: &typeinfo.Pointer{Elem: typeinfo.I16, Class: typeinfo.PtrNear}}
	resolved, ok = (&resolveLateAddressesProcessor{ctx: ctx}).resolveAddressOfPart(field)
	if !ok || FormatExpr(resolved) != "&ord.id" {
		t.Fatalf("field address = %s, want &ord.id", FormatExpr(resolved))
	}
}
