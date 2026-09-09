package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestConvertStackAddressMemoryProjectsLocalFields verifies SS:[&local+off]
// is treated as an access to the addressed local aggregate.
func TestConvertStackAddressMemoryProjectsLocalFields(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "ScannerWndProc")
	pt := machine.AddressVal(frameMemoryAccess(ctx, 0x78f, -0x8, 2))
	converter := &machineConverter{ctx: ctx}

	tests := []struct {
		disp int
		want string
	}{
		{disp: 0, want: "pt.x"},
		{disp: 2, want: "pt.y"},
	}
	for _, tt := range tests {
		mem := machine.MemoryAddress{Seg: machine.RegVal(asm.RegSS), Base: pt, Disp: tt.disp, Width: 2}
		if got := FormatExpr(converter.convertMemoryLValue(mem, 2)); got != tt.want {
			t.Fatalf("converted SS address at +%d = %q, want %q", tt.disp, got, tt.want)
		}
	}
}

// TestConvertIndexedStackArrayPreservesIndex verifies a BP-relative local
// root cannot erase a separate dynamic machine index.
func TestConvertIndexedStackArrayPreservesIndex(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "PickANameAndBmp")
	index := machine.BinaryVal(machine.ValueOpShl, frameLoad(ctx, 0xbe, -0x4, 2), machine.ConstVal(1))
	mem := frameMemoryAccess(ctx, 0xbe, -0xc, 2)
	mem.Seg = machine.RegVal(asm.RegSS)
	mem.Index = index

	if _, ok := ctx.symbols.exactMemoryPath(mem); ok {
		t.Fatal("indexed frame address incorrectly produced an exact fallback path")
	}
	if got := FormatExpr((&machineConverter{ctx: ctx}).convertMemoryLValue(mem, 2)); got != "rgfBmpUsed[i]" {
		t.Fatalf("converted indexed stack array = %q, want rgfBmpUsed[i]", got)
	}
}

// TestConvertBitfieldThroughFixedArray verifies bitfields use the shared
// pointer, struct, and array address projector before terminal selection.
func TestConvertBitfieldThroughFixedArray(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "LCalcFuelGainFromRamScoops")
	lpshdef := machine.LoadVal(frameMemoryAccess(ctx, 0, -0xc, 4))
	load := machine.LoadVal(machine.MemoryAddress{Base: lpshdef, Disp: 0x3c, Width: 2})
	value := machine.BinaryVal(
		machine.ValueOpAnd,
		machine.BinaryVal(machine.ValueOpShr, load, machine.ConstVal(8)),
		machine.ConstVal(0xff),
	)
	if got := FormatExpr((&machineConverter{ctx: ctx}).convertValue(value)); got != "lpshdef->hul.rghs[0x0].cItem" {
		t.Fatalf("converted bitfield = %q, want lpshdef->hul.rghs[0x0].cItem", got)
	}
}

// TestConvertBitfieldThroughFlexibleArray verifies a trailing array member is
// selected before equal-sized parent-struct pointer arithmetic.
func TestConvertBitfieldThroughFlexibleArray(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "PszProductionETA")
	const relOff = 0x78
	lpplprodOff := frameLoad(ctx, relOff, 0xa, 2)
	lpplprodSeg := frameLoad(ctx, relOff, 0xc, 2)
	iItem := frameLoad(ctx, relOff, 0xe, 2)
	itemOffset := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.ConstVal(4),
		machine.BinaryVal(machine.ValueOpShl, iItem, machine.ConstVal(2)),
	)
	storage := machine.MemoryAddress{Seg: lpplprodSeg, Base: machine.BinaryVal(machine.ValueOpAdd, lpplprodOff, itemOffset), Width: 4}
	converter := &machineConverter{ctx: ctx}

	tests := []struct {
		name     string
		bitOff   int
		bitWidth int
		want     string
	}{
		{name: "object class", bitOff: 17, bitWidth: 3, want: "lpplprod->rgprod[iItem].grobj"},
		{name: "item type", bitOff: 10, bitWidth: 7, want: "lpplprod->rgprod[iItem].iItem"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			field, ok := converter.resolveBitfieldLValue(storage, tt.bitOff, tt.bitWidth)
			if !ok {
				t.Fatal("flexible-array bitfield did not project")
			}
			if got := FormatExpr(field); got != tt.want {
				t.Fatalf("converted bitfield = %q, want %q", got, tt.want)
			}
		})
	}
}

// TestConvertBitfieldStoreThroughUnion verifies destination-preserving writes
// can select the unique bitfield nested in an overlapping aggregate.
func TestConvertBitfieldStoreThroughUnion(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "DropSalvage")
	pointer := machine.LoadVal(frameMemoryAccess(ctx, 0x209, -0x10, 4))
	dst := machine.MemoryAddress{
		Seg:   machine.FarPointerVal(pointer, machine.FarPointerSegment),
		Base:  machine.FarPointerVal(pointer, machine.FarPointerOffset),
		Disp:  0xe,
		Width: 2,
	}
	src := machine.BinaryVal(
		machine.ValueOpOr,
		machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(dst), machine.ConstVal(0xc000)),
		machine.ConstVal(0),
	)
	effect := (&machineConverter{ctx: ctx}).convertEffect(machine.StoreEffect{Addr: dst, Src: src, Width: 2})

	if got := FormatEffect(effect); got != "lpth->thp.wtMax = 0x0" {
		t.Fatalf("converted store = %q, want lpth->thp.wtMax = 0x0", got)
	}
}
