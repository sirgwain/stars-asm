package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestCollapseScratchBitfieldRMWPreservesOldStorageValue verifies a scratch snapshot is fused before the destination clear.
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
	scratchWide := scratchLow
	scratchWide.Width = 4
	delta := machine.BinaryVal(
		machine.ValueOpShl,
		machine.SignExtendVal(cBuilt, 16, 32),
		machine.ConstVal(20),
	)
	highAdd := &machine.Binary{
		Op:       machine.ValueOpAdd,
		LHS:      machine.WordVal(delta, machine.WordHigh),
		RHS:      machine.LoadVal(dstHigh),
		Producer: machine.Meta{BlockID: block, InstOff: 0x2448, InstOp: asm.OpADC},
	}
	effects := []machine.Effect{
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
	if !changed || len(got.Effects) != 1 {
		t.Fatalf("collapsed effects = (%v, %d), want (true, 1)", changed, len(got.Effects))
	}
	store, ok := got.Effects[0].(machine.StoreEffect)
	if !ok {
		t.Fatalf("collapsed effect = %T, want machine.StoreEffect", got.Effects[0])
	}
	converted := (&machineConverter{ctx: ctx, result: &Result{}}).convertEffect(store)
	if formatted := FormatEffect(converted); formatted != "lppl->cFactories = (lppl->cFactories + cBuilt)" {
		t.Fatalf("collapsed semantic store = %q", formatted)
	}
}
