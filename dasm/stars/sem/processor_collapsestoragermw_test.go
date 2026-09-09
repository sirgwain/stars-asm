package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCollapseScratchBitfieldCopy recovers PROD updates assembled in scratch
// words while preserving live scratch storage and unrelated destination bits.
func TestCollapseScratchBitfieldCopy(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "AddMinesToBlockedQueues")
	blockID := machine.BlockID(0x1c8f)
	ctx.SetCurrentBlock(blockID)
	// sel.pl.lpplprod points to PLPROD; its PROD array starts at byte four.
	lpplprod := machine.LoadVal(machine.MemoryAddress{Seg: machine.RegVal(asm.RegDS), Disp: 0x4a22, Width: 4})
	dst := machine.MemoryAddress{
		Seg:  machine.FarPointerVal(lpplprod, machine.FarPointerSegment),
		Base: machine.FarPointerVal(lpplprod, machine.FarPointerOffset), Disp: 4, Width: 4,
	}
	low := frameMemoryAccess(ctx, 0x1ccd-ctx.fs.Addr.Off, -0x13a, 2)
	high := frameMemoryAccess(ctx, 0x1cd1-ctx.fs.Addr.Off, -0x138, 2)
	wide := low
	wide.Width = 4
	value := frameLoad(ctx, 0x1c92-ctx.fs.Addr.Off, -0x10, 2)
	for _, tc := range []struct {
		name                           string
		offset, width                  uint
		field                          string
		liveHere, liveNext, addressUse bool
		wrongDestination, dependent    bool
	}{
		{name: "low word with explicit zero shift", width: 10, field: "cItem"},
		{name: "field spanning both words", offset: 10, width: 7, field: "iItem"},
		{name: "high word field", offset: 20, width: 7, field: "pct"},
		{name: "live in same block", width: 10, liveHere: true},
		{name: "live in another block", width: 10, liveNext: true},
		{name: "address taken", width: 10, addressUse: true},
		{name: "different retained destination", width: 10, wrongDestination: true},
		{name: "high word depends on scratch", width: 10, dependent: true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			mask := (uint(1) << tc.width) - 1
			keep := uint(0xffffffff) ^ (mask << tc.offset)
			insertedValue := machine.Value(value)
			if tc.dependent {
				insertedValue = machine.LoadVal(low)
			}
			inserted := machine.CastVal(machine.BinaryVal(machine.ValueOpShl,
				machine.CastVal(machine.BinaryVal(machine.ValueOpAnd, insertedValue, machine.ConstVal(mask)), typeinfo.U32),
				machine.ConstVal(tc.offset)), typeinfo.I32)
			retainedLow := dst
			retainedLow.Width = 2
			if tc.wrongDestination {
				retainedLow.Disp += 4
			}
			retainedHigh := retainedLow
			retainedHigh.Disp += 2
			effects := []machine.Effect{
				machine.StoreEffect{Addr: low, Width: 2, Src: machine.BinaryVal(machine.ValueOpOr,
					machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(retainedLow), machine.ConstVal(keep&0xffff)),
					machine.WordVal(inserted, machine.WordLow))},
				machine.StoreEffect{Addr: high, Width: 2, Src: machine.BinaryVal(machine.ValueOpOr,
					machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(retainedHigh), machine.ConstVal(keep>>16)),
					machine.WordVal(inserted, machine.WordHigh))},
				machine.StoreEffect{Addr: dst, Width: 4, Src: machine.LoadVal(wide)},
			}
			otherUse := machine.StoreEffect{Addr: frameMemoryAccess(ctx, 0, -0x10, 4), Width: 4, Src: machine.LoadVal(wide)}
			if tc.addressUse {
				otherUse.Src = machine.AddressVal(wide)
			}
			if tc.liveHere || tc.addressUse {
				effects = append(effects, otherUse)
			}
			block := machine.BlockEffects{Block: blockID, Effects: effects}
			fn := machine.FuncEffects{Blocks: []machine.BlockEffects{block}}
			if tc.liveNext {
				fn.Blocks = append(fn.Blocks, machine.BlockEffects{Block: 0x17a3, Effects: []machine.Effect{otherUse}})
			}
			got, changed := (&collapseStorageRMWProcessor{ctx: ctx}).ProcessMachineBlock(nil, fn, block)
			if changed != (tc.field != "") {
				t.Fatalf("changed = %v, want %v", changed, tc.field != "")
			}
			if !changed {
				if len(got.Effects) != len(effects) {
					t.Fatal("unmatched copy lost an effect")
				}
				return
			}
			if len(got.Effects) != 1 {
				t.Fatalf("got %d effects, want one bitfield store", len(got.Effects))
			}
			converted := (&machineConverter{ctx: ctx, result: newResult(ctx.fs)}).convertEffect(got.Effects[0])
			want := "sel.pl.lpplprod->rgprod[0]." + tc.field + " = LOWORD(cBuild)"
			if text := FormatEffect(converted); text != want {
				t.Fatalf("field update = %s, want %s", text, want)
			}
		})
	}
}

// TestCollapseScratchWordBitfieldRMW recovers a captured HS.iItem increment and
// preserves scratch definitions when they have other uses or the masks disagree.
func TestCollapseScratchWordBitfieldRMW(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "FGetBestDefensePart")
	blockID := machine.BlockID(0x221d)
	ctx.SetCurrentBlock(blockID)
	// PART.hs is at -0xe; HS.iItem occupies the low byte at hs+2.
	dst := frameMemoryAccess(ctx, 0x2239-ctx.fs.Addr.Off, -0xc, 2)
	scratch := frameMemoryAccess(ctx, 0x2236-ctx.fs.Addr.Off, -0x12, 2)
	for _, tc := range []struct {
		name                          string
		liveHere, liveNext, interrupt bool
		keep                          uint
		want                          bool
	}{
		{name: "word increment", keep: 0xff00, want: true},
		{name: "live in same block", keep: 0xff00, liveHere: true},
		{name: "live in another block", keep: 0xff00, liveNext: true},
		{name: "intervening effect", keep: 0xff00, interrupt: true},
		{name: "incompatible mask", keep: 0xfff0},
	} {
		t.Run(tc.name, func(t *testing.T) {
			effects := []machine.Effect{
				machine.StoreEffect{Addr: scratch, Width: 2, Src: machine.BinaryVal(machine.ValueOpAnd,
					machine.BinaryVal(machine.ValueOpAdd, machine.LoadVal(dst), machine.ConstVal(1)), machine.ConstVal(0xff))},
				machine.StoreEffect{Addr: dst, Width: 2, Src: machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(dst), machine.ConstVal(tc.keep))},
				machine.StoreEffect{Addr: dst, Width: 2, Src: machine.BinaryVal(machine.ValueOpOr, machine.LoadVal(dst), machine.LoadVal(scratch))},
			}
			otherUse := machine.StoreEffect{Addr: frameMemoryAccess(ctx, 0, -6, 2), Width: 2, Src: machine.LoadVal(scratch)}
			if tc.liveHere {
				effects = append(effects, otherUse)
			}
			if tc.interrupt {
				effects = append(effects[:1], append([]machine.Effect{otherUse}, effects[1:]...)...)
			}
			block := machine.BlockEffects{Block: blockID, Effects: effects}
			fn := machine.FuncEffects{Blocks: []machine.BlockEffects{block}}
			if tc.liveNext {
				fn.Blocks = append(fn.Blocks, machine.BlockEffects{Block: 0x2250, Effects: []machine.Effect{otherUse}})
			}
			got, changed := (&collapseStorageRMWProcessor{ctx: ctx}).ProcessMachineBlock(nil, fn, block)
			if changed != tc.want {
				t.Fatalf("changed = %v, want %v", changed, tc.want)
			}
			if tc.want {
				if len(got.Effects) != 1 {
					t.Fatalf("got %d effects, want one field update", len(got.Effects))
				}
				converted := (&machineConverter{ctx: ctx, result: newResult(ctx.fs)}).convertEffect(got.Effects[0])
				if text := FormatEffect(converted); text != "part.hs.iItem = (part.hs.iItem + 0x1)" {
					t.Fatalf("field update = %s", text)
				}
			} else if len(got.Effects) != len(effects) {
				t.Fatal("unmatched update lost an effect")
			}
		})
	}
}

// TestCollapseScratchBitfieldRMWPreservesOldStorageValue verifies an
// intermediate wide scratch value is substituted before the destination clear.
func TestCollapseScratchBitfieldRMWPreservesOldStorageValue(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "FBuildObject")
	block := machine.BlockID(0x2435)
	ctx.SetCurrentBlock(block)

	lppl := machine.LoadVal(frameMemoryAccess(ctx, 0x2435-ctx.fs.Addr.Off, 0x6, 4))
	cBuilt := machine.LoadVal(frameMemoryAccess(ctx, 0x2435-ctx.fs.Addr.Off, 0xe, 2))
	dst := machine.MemoryAddress{
		Seg:   machine.FarPointerVal(lppl, machine.FarPointerSegment),
		Base:  machine.FarPointerVal(lppl, machine.FarPointerOffset),
		Disp:  0x14,
		Width: 4,
	}
	dstHigh := dst
	dstHigh.Disp += 2
	dstHigh.Width = 2
	scratchLow := frameMemoryAccess(ctx, 0x2452-ctx.fs.Addr.Off, -0x18, 2)
	scratchHigh := frameMemoryAccess(ctx, 0x2455-ctx.fs.Addr.Off, -0x16, 2)
	deltaLow := frameMemoryAccess(ctx, 0x243c-ctx.fs.Addr.Off, -0x1c, 2)
	deltaHigh := frameMemoryAccess(ctx, 0x2440-ctx.fs.Addr.Off, -0x1a, 2)
	scratchWide := scratchLow
	scratchWide.Width = 4
	delta := machine.BinaryVal(
		machine.ValueOpMul,
		machine.SignExtendVal(cBuilt, 16, 32),
		machine.ConstVal(1<<20),
	)
	highAdd := &machine.Binary{
		Op:       machine.ValueOpAdd,
		LHS:      machine.LoadVal(deltaHigh),
		RHS:      machine.LoadVal(dstHigh),
		Producer: machine.Meta{BlockID: block, InstOff: 0x2448, InstOp: asm.OpADC},
	}
	effects := []machine.Effect{
		machine.StoreEffect{Addr: deltaLow, Src: machine.WordVal(delta, machine.WordLow), Width: 2},
		machine.StoreEffect{Addr: deltaHigh, Src: machine.WordVal(delta, machine.WordHigh), Width: 2},
		machine.StoreEffect{Addr: scratchLow, Src: machine.ConstVal(0), Width: 2},
		machine.StoreEffect{
			Addr:  scratchHigh,
			Src:   machine.BinaryVal(machine.ValueOpAnd, highAdd, machine.ConstVal(0xfff0)),
			Width: 2,
		},
		machine.StoreEffect{
			Addr: dst,
			Src: machine.BinaryVal(
				machine.ValueOpOr,
				machine.BinaryVal(machine.ValueOpAnd, machine.LoadVal(dst), machine.ConstVal(0x000fffff)),
				machine.ConstVal(0),
			),
			Width: 4,
		},
		machine.StoreEffect{
			Addr:  dst,
			Src:   machine.BinaryVal(machine.ValueOpOr, machine.LoadVal(dst), machine.LoadVal(scratchWide)),
			Width: 4,
		},
	}

	got, changed := (&collapseStorageRMWProcessor{ctx: ctx}).ProcessMachineBlock(nil, machine.FuncEffects{}, machine.BlockEffects{Block: block, Effects: effects})
	if !changed || len(got.Effects) != 3 {
		t.Fatalf("collapsed effects = (%v, %d), want (true, 3)", changed, len(got.Effects))
	}
	store, ok := got.Effects[2].(machine.StoreEffect)
	if !ok {
		t.Fatalf("collapsed effect = %T, want machine.StoreEffect", got.Effects[2])
	}
	converted := (&machineConverter{ctx: ctx, result: &Result{}}).convertEffect(store)
	if formatted := FormatEffect(converted); formatted != "lppl->cFactories = (lppl->cFactories + cBuilt)" {
		t.Fatalf("collapsed semantic store = %q", formatted)
	}
}
