package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestBitfieldArithmeticFoldedConstants verifies machine and late semantic
// recovery of aligned arithmetic constants while rejecting possible low-bit carries.
func TestBitfieldArithmeticFoldedConstants(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "GenerateWorld")
	ctx.SetCurrentBlock(0x2c9d)
	players := fx.SDB.GetGlobal("rgplr")
	storage := machine.MemoryAddress{Seg: machine.ConstVal(fx.SDB.DGroupFrame), Base: machine.ConstVal(uint(players.Addr.Off)), Disp: 4, Width: 2}
	converter := machineConverter{ctx: ctx, result: newResult(ctx.fs)}
	for _, tc := range []struct {
		name  string
		op    machine.ValueOp
		delta uint
		want  string
	}{
		{name: "increment", op: machine.ValueOpAdd, delta: 0x1000, want: "rgplr[0].cshdefSB = (rgplr[0].cshdefSB + 0x1)"},
		{name: "decrement", op: machine.ValueOpSub, delta: 0x1000, want: "rgplr[0].cshdefSB = (rgplr[0].cshdefSB - 0x1)"},
		{name: "toggle", op: machine.ValueOpXor, delta: 0x2000, want: "rgplr[0].cshdefSB = (rgplr[0].cshdefSB ^ 0x2)"},
		{name: "unaligned constant", op: machine.ValueOpAdd, delta: 0x1001},
	} {
		t.Run(tc.name, func(t *testing.T) {
			value := machine.BinaryVal(machine.ValueOpOr,
				machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(storage), machine.ConstVal(0xfff)),
				machine.BinaryVal(machine.ValueOpAnd,
					machine.BinaryVal(tc.op, machine.LoadVal(storage), machine.ConstVal(tc.delta)), machine.ConstVal(0xf000)))
			_, recognized := recognizeBitfieldWrite(ctx, storage, value)
			if recognized != (tc.want != "") {
				t.Fatalf("machine recognition = %v", recognized)
			}
			if recognized {
				got := converter.convertEffect(machine.StoreEffect{Addr: storage, Width: 2, Src: value})
				if text := FormatEffect(got); text != tc.want {
					t.Fatalf("machine update = %s, want %s", text, tc.want)
				}
			}
			assign := &Assign{Dst: converter.convertMemoryLValue(storage, 2), Src: converter.convertValueWithoutBitfields(value)}
			block := Block{ID: 0x2c9d, Effects: []Effect{assign}}
			got, changed := (&resolveLateBitfieldsProcessor{ctx: ctx}).ProcessBlock(newResult(ctx.fs), Func{Blocks: []Block{block}}, block)
			if tc.want != "" && (!changed || FormatEffect(got.Effects[0]) != tc.want) {
				t.Fatalf("semantic update = %s, want %s", FormatEffect(got.Effects[0]), tc.want)
			}
			if tc.want == "" {
				if _, recognized := semanticBitfieldStore(assign.Dst, assign.Src); recognized {
					t.Fatal("semantic recovery accepted an unaligned constant")
				}
			}
		})
	}
}

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
			name: "shifted mask before shift",
			value: machine.BinaryVal(
				machine.ValueOpShr,
				machine.BinaryVal(machine.ValueOpAnd, planetWord(0x1a), machine.ConstVal(0x80)),
				machine.ConstVal(7),
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
	dst, ok := (&machineConverter{ctx: ctx}).resolveBitfieldLValue(flags, bitfield.Access)
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

// TestBitfieldStoreDisjointAdd verifies ADD is accepted only when the inserted
// value is proven confined to bits cleared from the destination.
func TestBitfieldStoreDisjointAdd(t *testing.T) {
	mem := machine.MemoryAddress{Base: machine.FrameBaseVal(), Disp: -2, Width: 2}
	source := machine.RegVal(asm.RegAX)
	inserted := machine.BinaryVal(
		machine.ValueOpShl,
		machine.BinaryVal(machine.ValueOpAnd, source, machine.ConstVal(0x7)),
		machine.ConstVal(1),
	)
	stored := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(mem), machine.ConstVal(0xfff1)),
		inserted,
	)

	_, bitOff, bitWidth, got, ok := bitfieldStore(mem, stored, sameStorage)
	if !ok {
		t.Fatal("disjoint ADD bitfield store was not recognized")
	}
	if bitOff != 1 || bitWidth != 3 || !machine.ValueEquals(got, source) {
		t.Fatalf("bitfield store = (off %d, width %d, value %v), want (1, 3, AX)", bitOff, bitWidth, got)
	}

	unsafe := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(mem), machine.ConstVal(0xfff1)),
		source,
	)
	if _, _, _, _, ok := bitfieldStore(mem, unsafe, sameStorage); ok {
		t.Fatal("unmasked ADD bitfield store was recognized")
	}
}

// TestBitfieldStoreMultiplyShift verifies multiply-by-a-power-of-two encoding
// is treated like a left shift during bitfield recognition.
func TestBitfieldStoreMultiplyShift(t *testing.T) {
	mem := machine.MemoryAddress{Base: machine.FrameBaseVal(), Disp: -2, Width: 2}
	source := machine.RegVal(asm.RegAX)
	multiply := machine.BinaryVal(
		machine.ValueOpMul,
		machine.BinaryVal(machine.ValueOpAnd, source, machine.ConstVal(0x7)),
		machine.ConstVal(2),
	)
	stored := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(mem), machine.ConstVal(0xfff1)),
		multiply,
	)

	_, bitOff, bitWidth, got, ok := bitfieldStore(mem, stored, sameStorage)
	if !ok {
		t.Fatal("multiply-shift bitfield store was not recognized")
	}
	if bitOff != 1 || bitWidth != 3 || !machine.ValueEquals(got, source) {
		t.Fatalf("bitfield store = (off %d, width %d, value %v), want (1, 3, AX)", bitOff, bitWidth, got)
	}
	if !machineValueMaskedWithin(multiply, 0x000e) {
		t.Fatal("multiply-shift value was not proven within its destination mask")
	}
	if machineValueMaskedWithin(multiply, 0x0007) {
		t.Fatal("multiply-shift value exceeded an incompatible destination mask")
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

func TestRecognizeBitfieldReadSignedShiftPair(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "FTrackXfer")

	load := &machine.Load{
		Addr: machine.MemoryAddress{
			Width: 2,
		},
	}

	value := machine.BinaryVal(
		machine.ValueOpSar,
		machine.BinaryVal(
			machine.ValueOpShl,
			load,
			machine.ConstVal(11),
		),
		machine.ConstVal(11),
	)

	got, ok := recognizeBitfieldRead(ctx, value)
	if !ok {
		t.Fatal("recognizeBitfieldRead() = false")
	}

	if got.Load != load {
		t.Fatalf("Load = %#v, want %#v", got.Load, load)
	}
	if got.Access.BitOff != 0 {
		t.Fatalf("BitOff = %d, want 0", got.Access.BitOff)
	}
	if got.Access.BitWidth != 5 {
		t.Fatalf("BitWidth = %d, want 5", got.Access.BitWidth)
	}
	if got.Access.Signedness != BitfieldSigned {
		t.Fatalf("Signedness = %v, want signed", got.Access.Signedness)
	}
}

func TestRecognizeBitfieldReadSignedShiftPairNonzeroOffset(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "FTrackXfer")

	load := &machine.Load{
		Addr: machine.MemoryAddress{
			Width: 2,
		},
	}

	// 16-bit storage:
	//
	//     (storage << 7) SAR 12
	//
	// selects original bits [5:9):
	//
	//     BitOff   = 12 - 7 = 5
	//     BitWidth = 16 - 12 = 4
	value := machine.BinaryVal(
		machine.ValueOpSar,
		machine.BinaryVal(
			machine.ValueOpShl,
			load,
			machine.ConstVal(7),
		),
		machine.ConstVal(12),
	)

	got, ok := recognizeBitfieldRead(ctx, value)
	if !ok {
		t.Fatal("recognizeBitfieldRead() = false")
	}

	if got.Access.BitOff != 5 {
		t.Fatalf("BitOff = %d, want 5", got.Access.BitOff)
	}
	if got.Access.BitWidth != 4 {
		t.Fatalf("BitWidth = %d, want 4", got.Access.BitWidth)
	}
	if got.Access.Signedness != BitfieldSigned {
		t.Fatalf("Signedness = %v, want signed", got.Access.Signedness)
	}
}
