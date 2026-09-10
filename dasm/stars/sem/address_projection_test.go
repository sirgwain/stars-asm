package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestConvertIndexedArrayThroughUnion verifies cargo accesses select the
// nested mineral array by its byte stride without relying on an exact fallback.
func TestConvertIndexedArrayThroughUnion(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	tests := []struct {
		function string
		relOff   uint32
		pointer  int
		index    int
		want     string
	}{
		{function: "ChgCargo", relOff: 0x1e9, pointer: -0x4, index: 0xa, want: "pth->thp.rgwtMin[iSupply]"},
		{function: "ChgCargo", relOff: 0x254, pointer: -0x4, index: -0x86, want: "pth->thp.rgwtMin[i]"},
		{function: "LogChangeThing", relOff: 0x7f, pointer: 0xa, index: -0x4, want: "pthNew->thp.rgwtMin[i]"},
	}
	for _, tt := range tests {
		t.Run(tt.want, func(t *testing.T) {
			ctx := mustFuncContext(t, fx, res, tt.function)
			mem := machine.MemoryAddress{
				Seg: machine.RegVal(asm.RegDS),
				Base: machine.BinaryVal(machine.ValueOpAdd,
					machine.BinaryVal(machine.ValueOpAdd, frameLoad(ctx, tt.relOff, tt.pointer, 2), machine.ConstVal(8)),
					machine.BinaryVal(machine.ValueOpMul, frameLoad(ctx, tt.relOff, tt.index, 2), machine.ConstVal(2))),
				Width: 2,
			}
			converter := &machineConverter{ctx: ctx}
			resolved, ok := ctx.symbols.addressFromMemory(mem, nil)
			if !ok {
				t.Fatal("cargo address did not resolve")
			}
			addr, ok := converter.semanticResolvedAddress(resolved)
			if !ok {
				t.Fatal("cargo address did not convert")
			}
			field, ok := converter.consumeAddress(addr, mem.Width)
			if !ok {
				t.Fatal("cargo address required an exact fallback")
			}
			if got := FormatExpr(field); got != tt.want {
				t.Fatalf("converted cargo address = %q, want %q", got, tt.want)
			}
		})
	}
}

// TestIndexedUnionArraySelectionRejectsConflicts verifies nested array
// projection neither overrides a known union member nor guesses between arrays.
func TestIndexedUnionArraySelectionRejectsConflicts(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "ChgCargo")
	converter := &machineConverter{ctx: ctx}
	root, ok := ctx.symbolRootByName("pth")
	if !ok {
		t.Fatal("pth not found")
	}
	base, ok := converter.convertSymbolPath(root, root.Type())
	if !ok {
		t.Fatal("pth did not convert")
	}
	addr := AddressExpr{Base: base, Offset: 8, Deref: true, Terms: []ScaledTerm{{Expr: &Const{TypeInfo: typeinfo.I16, U64: 1}, Scale: 2}}}
	thing := fx.SDB.GetStruct("THING")
	rule, ok := fx.SDB.UnionRules.UnionVariantForType(thing)
	if !ok {
		t.Fatal("THING union rule not found")
	}
	unionCtx := symresolve.NewUnionContext()
	if !unionCtx.Add(root, rule, rule.Enum.GetValue("ithMinefield")) {
		t.Fatal("minefield selection did not apply")
	}
	block := machine.BlockID(0x6202)
	ctx.SetUnionContexts(map[machine.BlockID]*symresolve.UnionContext{block: unionCtx})
	ctx.SetCurrentBlock(block)
	if field, ok := converter.consumeAddress(addr, 2); ok {
		t.Fatalf("cargo array overrode minefield selection: %s", FormatExpr(field))
	}

	ctx.SetCurrentBlock(0)
	ambiguous := *thing
	ambiguous.Name = "AMBIGUOUS_THING"
	ambiguous.Fields = append([]typeinfo.StructField(nil), thing.Fields...)
	ambiguous.Fields = append(ambiguous.Fields, typeinfo.StructField{Name: "otherPacket", Offset: 6, Type: fx.SDB.GetStruct("THPACK")})
	ambiguous.FinalizeLayout()
	pointer := *base.ExprType().(*typeinfo.Pointer)
	pointer.Elem = &ambiguous
	local := *base.(*Local)
	local.Type = &pointer
	addr.Base = &local
	if field, ok := converter.consumeAddress(addr, 2); ok {
		t.Fatalf("ambiguous cargo arrays unexpectedly projected: %s", FormatExpr(field))
	}
}

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
			field, ok := converter.resolveBitfieldLValue(storage, BitfieldAccess{
				StorageWidth: storage.Width,
				BitOff:       tt.bitOff,
				BitWidth:     tt.bitWidth,
			})
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

// TestNegativePointerOffsets verifies pointer-relative byte displacements are
// rebased through typed pointees without losing legitimate raw void-pointer
// accesses immediately before the user-visible allocation.
func TestNegativePointerOffsets(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	t.Run("previous typed element", func(t *testing.T) {
		ctx := mustFuncContext(t, fx, res, "FAddWayPoint")
		converter := &machineConverter{ctx: ctx}
		root, ok := ctx.symbolRootByName("lpord")
		if !ok {
			t.Fatal("lpord not found")
		}
		base, ok := converter.convertSymbolPath(root, root.Type())
		if !ok {
			t.Fatal("lpord did not convert")
		}

		field, ok := converter.consumeBitfieldAddress(AddressExpr{
			Base:   base,
			Offset: -0xc,
			Deref:  true,
		}, BitfieldAccess{StorageWidth: 2, BitOff: 0, BitWidth: 4})
		if !ok {
			t.Fatal("negative ORDER offset did not project")
		}
		access, ok := field.(*FieldAccess)
		if !ok || access.Field == nil || access.Field.Name != "grTask" {
			t.Fatalf("negative ORDER offset = %s, want previous ORDER grTask", FormatExpr(field))
		}
		index, ok := access.Base.(*ArrayIndex)
		if !ok {
			t.Fatalf("negative ORDER offset base = %T, want ArrayIndex", access.Base)
		}
		neg, ok := index.Index.(*Unary)
		if !ok || neg.Op != OpNeg {
			t.Fatalf("negative ORDER index = %s, want -1", FormatExpr(index.Index))
		}
		one, ok := neg.X.(*Const)
		if !ok || one.U64 != 1 {
			t.Fatalf("negative ORDER index = %s, want -1", FormatExpr(index.Index))
		}
	})

	t.Run("char pointer", func(t *testing.T) {
		ctx := mustFuncContext(t, fx, res, "GenNewGameFromFile")
		converter := &machineConverter{ctx: ctx}
		root, ok := ctx.symbolRootByName("lpb")
		if !ok {
			t.Fatal("lpb not found")
		}
		base, ok := converter.convertSymbolPath(root, root.Type())
		if !ok {
			t.Fatal("lpb did not convert")
		}
		value, ok := converter.consumeAddress(AddressExpr{Base: base, Offset: -2, Deref: true}, 1)
		if !ok {
			t.Fatal("negative char pointer offset did not project")
		}
		index, ok := value.(*ArrayIndex)
		if !ok {
			t.Fatalf("negative char pointer offset = %T, want ArrayIndex", value)
		}
		if got := FormatExpr(index.Index); got != "neg(2)" {
			t.Fatalf("negative char pointer index = %q, want neg(2)", got)
		}
	})

	t.Run("void pointer", func(t *testing.T) {
		ctx := mustFuncContext(t, fx, res, "FreeLp")
		converter := &machineConverter{ctx: ctx}
		root, ok := ctx.symbolRootByName("lp")
		if !ok {
			t.Fatal("lp not found")
		}
		base, ok := converter.convertSymbolPath(root, root.Type())
		if !ok {
			t.Fatal("lp did not convert")
		}
		value, ok := converter.consumeAddress(AddressExpr{Base: base, Offset: -2, Deref: true}, 2)
		if !ok {
			t.Fatal("negative void pointer offset did not remain a raw dereference")
		}
		deref, ok := value.(*Deref)
		if !ok || deref.ByteOff != -2 || deref.Width != 2 {
			t.Fatalf("negative void pointer offset = %s, want raw dereference at -2", FormatExpr(value))
		}
	})
}
