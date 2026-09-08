package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCollapseWideMachineValue verifies representative 16-bit word-pair
// shapes collapse into their native wide machine values.
func TestCollapseWideMachineValue(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	cMaxMinesCtx := mustFuncContext(t, fx, res, "CMaxMines")
	aboutCtx := mustFuncContext(t, fx, res, "About")

	ds := machine.RegVal(asm.RegDS)
	cs := machine.RegVal(asm.RegCS)
	ss := machine.RegVal(asm.RegSS)
	lPopMax := frameMemoryAccess(cMaxMinesCtx, 0, -0xa, 2)
	lPopMaxHigh := frameMemoryAccess(cMaxMinesCtx, 0, -0x8, 2)
	iEff := frameLoad(cMaxMinesCtx, 0, -0xc, 2)
	lppl := frameMemoryAccess(cMaxMinesCtx, 0, 0x6, 4)
	lpplLoad := machine.LoadVal(lppl)
	rc := frameMemoryAccess(aboutCtx, 0, -0xa, 4)
	szVersion := &machine.CallResult{
		Target:  &typeinfo.Function{Name: "SzVersion", Ret: typeinfo.LpStr},
		Type:    typeinfo.LpStr,
		InstOff: aboutCtx.fs.Addr.Off,
	}
	uint32Result := &machine.CallResult{
		Target:  &typeinfo.Function{Name: "WideResult", Ret: typeinfo.U32},
		Type:    typeinfo.U32,
		InstOff: cMaxMinesCtx.fs.Addr.Off,
	}
	lPopMaxWords := &machine.StackWords{Words: []machine.Value{
		machine.LoadVal(lPopMaxHigh),
		machine.LoadVal(lPopMax),
	}}
	iEffWords := &machine.StackWords{Words: []machine.Value{
		machine.WordVal(iEff, machine.WordSignHigh),
		iEff,
	}}
	cMaxProduct := machine.CastVal(
		machine.BinaryVal(machine.ValueOpMul, lPopMaxWords, iEffWords),
		typeinfo.U32,
	)

	tests := []struct {
		name        string
		ctx         *FuncContext
		value       machine.Value
		want        string
		wantChanged bool
	}{
		{
			name: "CMaxMines adjacent local dword loads",
			ctx:  cMaxMinesCtx,
			value: &machine.StackWords{Words: []machine.Value{
				machine.LoadVal(lPopMaxHigh),
				machine.LoadVal(lPopMax),
			}},
			want:        "load(dword [bp-0xa])",
			wantChanged: true,
		},
		{
			name: "callresult hiword loword",
			value: &machine.StackWords{Words: []machine.Value{
				machine.WordVal(uint32Result, machine.WordHigh),
				machine.WordVal(uint32Result, machine.WordLow),
			}},
			want:        "callresult(uint32_t)",
			wantChanged: true,
		},
		{
			name: "nested wide expression parent",
			ctx:  cMaxMinesCtx,
			value: &machine.StackWords{Words: []machine.Value{
				machine.WordVal(cMaxProduct, machine.WordHigh),
				machine.WordVal(cMaxProduct, machine.WordLow),
			}},
			want:        "(uint32_t)(load(dword [bp-0xa]) * sext16to32(load([bp-0xc])))",
			wantChanged: true,
		},
		{
			name: "constant words",
			value: &machine.StackWords{Words: []machine.Value{
				machine.ConstVal(0x0),
				machine.ConstVal(0x64),
			}},
			want:        "0x64",
			wantChanged: true,
		},
		{
			name: "far pointer memory load",
			ctx:  cMaxMinesCtx,
			value: machine.LoadVal(machine.MemoryAddress{
				Seg:   machine.FarPointerVal(lpplLoad, machine.FarPointerSegment),
				Base:  machine.FarPointerVal(lpplLoad, machine.FarPointerOffset),
				Width: 2,
			}),
			want:        "load([load(dword [bp+0x6])])",
			wantChanged: true,
		},
		{
			name: "far pointer memory high word load",
			ctx:  cMaxMinesCtx,
			value: machine.LoadVal(machine.MemoryAddress{
				Seg:   machine.FarPointerVal(lpplLoad, machine.FarPointerSegment),
				Base:  machine.FarPointerVal(lpplLoad, machine.FarPointerOffset),
				Disp:  2,
				Width: 2,
			}),
			want:        "load([load(dword [bp+0x6])+0x2])",
			wantChanged: true,
		},
		{
			name: "sign extended word",
			value: &machine.StackWords{Words: []machine.Value{
				machine.WordVal(iEff, machine.WordSignHigh),
				iEff,
			}},
			want:        "sext16to32(load([bp-0xc]))",
			wantChanged: true,
		},
		{
			name: "About stack address with ss segment",
			value: &machine.StackWords{Words: []machine.Value{
				ss,
				machine.AddressVal(rc),
			}},
			want:        "addr(dword [bp-0xa])",
			wantChanged: true,
		},
		{
			name: "About ds callresult",
			value: &machine.StackWords{Words: []machine.Value{
				ds,
				szVersion,
			}},
			want:        "callresult(char *)",
			wantChanged: true,
		},
		{
			name: "DoCyberAiTurn cs null address",
			value: &machine.StackWords{Words: []machine.Value{
				cs,
				machine.ConstVal(0x0),
			}},
			want:        "addr(dword cs:[0x0000])",
			wantChanged: true,
		},
		{
			name: "global ds address",
			value: &machine.StackWords{Words: []machine.Value{
				ds,
				machine.ConstVal(0x1234),
			}},
			want:        "addr(dword ds:[0x1234])",
			wantChanged: true,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, changed, handled := collapseWideMachineValue(tt.ctx, tt.value)
			if !handled {
				t.Fatal("collapseWideMachineValue() handled = false, want true")
			}
			if changed != tt.wantChanged {
				t.Fatalf("collapseWideMachineValue() changed = %v, want %v", changed, tt.wantChanged)
			}
			if got.String() != tt.want {
				t.Fatalf("collapseWideMachineValue() = %v, want %v", got, tt.want)
			}
		})
	}
}

// TestCollapseWideMachineValuePreservesIndexedStackAddress verifies an SS
// word paired with local-address arithmetic becomes a typed interior pointer.
func TestCollapseWideMachineValuePreservesIndexedStackAddress(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "CreateChildWindows")
	strlen := fx.SDB.GetFunction("strlen")
	if strlen == nil {
		t.Fatal("strlen not found")
	}
	strlenResult := &machine.CallResult{
		Target:  strlen,
		Type:    typeinfo.U16,
		InstOff: ctx.fs.Addr.Off + 0x90,
	}
	value := &machine.StackWords{Words: []machine.Value{
		machine.RegVal(asm.RegSS),
		machine.BinaryVal(
			machine.ValueOpAdd,
			machine.AddressVal(frameMemoryAccess(ctx, 0x5b, -0x7c, 1)),
			strlenResult,
		),
	}}

	collapsed, changed, handled := collapseWideMachineValue(ctx, value)
	if !handled || !changed {
		t.Fatalf("collapseWideMachineValue() handled = %v, changed = %v; want both true", handled, changed)
	}
	if got := collapsed.String(); got != "(addr(dword [bp-0x7c]) + callresult(uint16_t))" {
		t.Fatalf("collapseWideMachineValue() = %q", got)
	}
	converted := (&machineConverter{ctx: ctx}).convertValueTyped(collapsed, typeinfo.LpStr)
	if got := FormatExpr(converted); got != "&szGame[callresult(uint16_t)]" {
		t.Fatalf("converted stack address = %q, want %q", got, "&szGame[callresult(uint16_t)]")
	}
}

// TestCollapseWideMachineValueUsesNativeBitfield verifies the wide-value pass
// collapses word storage only after the complete expression resolves to a
// declared bitfield.
func TestCollapseWideMachineValueUsesNativeBitfield(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "AddMinesToBlockedQueues")
	relOff := uint32(0x1828) - ctx.fs.Addr.Off
	low := machine.LoadVal(frameMemoryAccess(ctx, relOff, -0x6, 2))
	high := machine.LoadVal(frameMemoryAccess(ctx, relOff, -0x4, 2))
	value := machine.BinaryVal(
		machine.ValueOpAnd,
		machine.WordVal(
			machine.CastVal(
				machine.BinaryVal(
					machine.ValueOpShr,
					&machine.StackWords{Words: []machine.Value{high, low}},
					machine.ConstVal(17),
				),
				typeinfo.U32,
			),
			machine.WordLow,
		),
		machine.ConstVal(7),
	)

	got, changed, handled := collapseWideMachineValue(ctx, value)
	if !handled || !changed {
		t.Fatalf("collapseWideMachineValue() handled = %v, changed = %v; want both true", handled, changed)
	}
	bitfield, ok := recognizeBitfieldRead(ctx, got)
	if !ok {
		t.Fatalf("collapsed value %q is no longer recognized as a bitfield", got.String())
	}
	dst, ok := (&machineConverter{ctx: ctx}).resolveBitfieldLValue(bitfield.Load.Addr, bitfield.BitOff, bitfield.BitWidth)
	if !ok {
		t.Fatalf("collapsed value %q no longer projects as a bitfield", got.String())
	}
	if formatted := FormatExpr(dst); formatted != "prod.grobj" {
		t.Fatalf("collapsed bitfield = %q, want prod.grobj", formatted)
	}
}

// TestCollapseWideMachineValuePairsFarPointerStructLanes verifies separately
// loaded words from one far-pointer aggregate storage become a dword load.
func TestCollapseWideMachineValuePairsFarPointerStructLanes(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "DoBombing")
	planetWord := func(relOff uint32, disp int) machine.Value {
		lppl := frameLoad(ctx, relOff, -0x30, 4)
		return machine.LoadVal(machine.MemoryAddress{
			Seg:   machine.FarPointerVal(lppl, machine.FarPointerSegment),
			Base:  machine.FarPointerVal(lppl, machine.FarPointerOffset),
			Disp:  disp,
			Width: 2,
		})
	}
	words := &machine.StackWords{Words: []machine.Value{
		planetWord(0x26c, 0x16),
		planetWord(0x268, 0x14),
	}}

	got, changed, handled := collapseWideMachineValue(ctx, words)
	if !handled || !changed {
		t.Fatalf("collapseWideMachineValue() handled = %v, changed = %v; want both true", handled, changed)
	}
	load, ok := got.(*machine.Load)
	if !ok || load.Addr.Width != 4 {
		t.Fatalf("collapseWideMachineValue() = %v, want dword load", got)
	}
}

// TestWideMachinePairPointerOffset verifies low-word pointer arithmetic is
// recombined with an unchanged high word before semantic conversion.
func TestWideMachinePairPointerOffset(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "FLoadGame")
	vrgsz := fx.SDB.GetGlobal("vrgszMRU")
	if vrgsz == nil {
		t.Fatal("vrgszMRU global not found")
	}
	lowAddr := machine.MemoryAddress{Seg: machine.RegVal(asm.RegDS), Disp: int(vrgsz.Addr.Off), Width: 2}
	highAddr := lowAddr
	highAddr.Disp += 2
	delta := machine.BinaryVal(machine.ValueOpShl, frameLoad(ctx, 0x30c0-ctx.fs.Addr.Off, -0x18, 2), machine.ConstVal(8))
	low := machine.BinaryVal(machine.ValueOpAdd, machine.LoadVal(lowAddr), delta)
	high := machine.LoadVal(highAddr)

	got, ok := (&wideMachineCollapser{ctx: ctx}).pair(low, high)
	if !ok {
		t.Fatal("pair() did not reconstruct pointer offset")
	}
	addition, ok := got.(*machine.Binary)
	if !ok || addition.Op != machine.ValueOpAdd {
		t.Fatalf("pair() = %v, want pointer addition", got)
	}
	base, ok := addition.LHS.(*machine.Load)
	if !ok || base.Addr.Width != 4 {
		t.Fatalf("pair() base = %v, want dword vrgszMRU load", addition.LHS)
	}
	if !machine.ValueEquals(addition.RHS, delta) {
		t.Fatalf("pair() delta = %v, want %v", addition.RHS, delta)
	}
}
