package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestConvertMachineBitfieldValues verifies physical recognition and semantic
// projection across direct loads and an uncollapsed 32-bit word pair.
func TestConvertMachineBitfieldValues(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "UninhabitPlanet")
	lppl := machine.LoadVal(frameMemoryAccess(ctx, 0, 0x6, 4))
	planetWord := func(disp int) machine.Value {
		return machine.LoadVal(machine.MemoryAddress{Base: lppl, Disp: disp, Width: 2})
	}

	tests := []struct {
		name           string
		value          machine.Value
		want           string
		wantRecognized bool
	}{
		{
			name:           "unshifted 16-bit field",
			value:          machine.BinaryVal(machine.ValueOpAnd, planetWord(0x4), machine.ConstVal(0xff)),
			want:           "lppl->det",
			wantRecognized: true,
		},
		{
			name: "high word of 32-bit storage",
			value: machine.BinaryVal(
				machine.ValueOpAnd,
				machine.BinaryVal(machine.ValueOpShr, planetWord(0x1a), machine.ConstVal(7)),
				machine.ConstVal(1),
			),
			want:           "lppl->fNoResearch",
			wantRecognized: true,
		},
		{
			name: "wrapped uncollapsed 32-bit storage",
			value: machine.BinaryVal(
				machine.ValueOpAnd,
				machine.WordVal(
					machine.BinaryVal(
						machine.ValueOpShr,
						machine.CastVal(&machine.StackWords{Words: []machine.Value{planetWord(0x1a), planetWord(0x18)}}, typeinfo.U32),
						machine.ConstVal(23),
					),
					machine.WordLow,
				),
				machine.ConstVal(1),
			),
			want:           "lppl->fNoResearch",
			wantRecognized: true,
		},
		{
			name:  "non-contiguous mask",
			value: machine.BinaryVal(machine.ValueOpAnd, planetWord(0x4), machine.ConstVal(0x5)),
		},
		{
			name: "partial 32-bit field",
			value: machine.BinaryVal(
				machine.ValueOpAnd,
				machine.BinaryVal(machine.ValueOpShr, planetWord(0x18), machine.ConstVal(12)),
				machine.ConstVal(0xf),
			),
			wantRecognized: true,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			_, ok := recognizeBitfieldRead(ctx, tt.value)
			if ok != tt.wantRecognized {
				t.Fatalf("recognizeBitfieldRead() ok = %v, want %v", ok, tt.wantRecognized)
			}
			if tt.want != "" {
				if got := FormatExpr((&machineConverter{ctx: ctx}).convertValue(tt.value)); got != tt.want {
					t.Fatalf("converted bitfield = %q, want %q", got, tt.want)
				}
			}
		})
	}
}

// TestConvertDynamicBitfieldStore verifies a shifted dynamic
// source resolves while a preserved load from different storage is rejected.
func TestConvertDynamicBitfieldStore(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "WinMain")
	ini := fx.SDB.GetGlobal("ini")
	if ini == nil {
		t.Fatal("ini not found")
	}
	flags := machine.MemoryAddress{
		Seg:   machine.ConstVal(fx.SDB.DGroupFrame),
		Base:  machine.ConstVal(uint(ini.Addr.Off)),
		Disp:  0xa,
		Width: 2,
	}
	source := frameLoad(ctx, 0x3ec, -0x1e, 2)
	stored := machine.BinaryVal(
		machine.ValueOpOr,
		machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(flags), machine.ConstVal(0xfffd)),
		machine.BinaryVal(
			machine.ValueOpShl,
			machine.BinaryVal(machine.ValueOpAnd, source, machine.ConstVal(1)),
			machine.ConstVal(1),
		),
	)

	bitfield, ok := recognizeBitfieldWrite(ctx, flags, stored)
	if !ok {
		t.Fatal("dynamic fCmdLine store was not recognized")
	}
	dst, ok := (&machineConverter{ctx: ctx}).resolveBitfieldLValue(flags, bitfield.BitOff, bitfield.BitWidth)
	if !ok {
		t.Fatal("dynamic fCmdLine store did not project")
	}
	if got := FormatExpr(dst); got != "ini.fCmdLine" {
		t.Fatalf("store path = %q, want ini.fCmdLine", got)
	}
	if !machine.ValueEquals(bitfield.Value, source) {
		t.Fatalf("store value = %v, want %v", bitfield.Value, source)
	}

	other := flags
	other.Disp += 2
	if _, ok := recognizeBitfieldWrite(ctx, other, stored); ok {
		t.Fatal("mismatched storage was recognized as a bitfield write")
	}
	if !ctx.maskedStorageWrite(flags, stored) {
		t.Fatal("dynamic masked write was not preserved as raw storage fallback")
	}
	if ctx.maskedStorageWrite(other, stored) {
		t.Fatal("mismatched dynamic masked write was classified as destination-preserving")
	}
}

// TestConvertChainedShiftBitfield verifies address-index bitfields retain
// compiler-emitted successive right shifts during machine recognition.
func TestConvertChainedShiftBitfield(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "FTrackXfer")
	load := machine.LoadVal(frameMemoryAccess(ctx, 0x1d5, -0x32, 2))
	value := machine.BinaryVal(
		machine.ValueOpAnd,
		machine.BinaryVal(machine.ValueOpShr,
			machine.BinaryVal(machine.ValueOpShr, load, machine.ConstVal(1)),
			machine.ConstVal(1),
		),
		machine.ConstVal(3),
	)

	if got := FormatExpr((&machineConverter{ctx: ctx}).convertValue(value)); got != "btn.iSide" {
		t.Fatalf("converted bitfield = %q, want btn.iSide", got)
	}
}
