package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

func TestCollapseWideMachineStorePairNestedIndexedField(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "GenerateWorld")
	ctx.SetCurrentBlock(0x16d6)

	rel := uint32(0x170b) - ctx.fs.Addr.Off

	i := frameLoad(ctx, rel, -0xac, 2)
	j := frameLoad(ctx, rel, -0x102, 2)

	playerOffset := machine.BinaryVal(
		machine.ValueOpMul,
		machine.ConstVal(0xc0),
		i,
	)
	itemOffset := machine.BinaryVal(
		machine.ValueOpMul,
		j,
		machine.ConstVal(4),
	)

	base := machine.BinaryVal(
		machine.ValueOpAdd,
		machine.ConstVal(0x59a2),
		machine.WordVal(playerOffset, machine.WordLow),
	)
	base = machine.BinaryVal(
		machine.ValueOpAdd,
		base,
		machine.ConstVal(0x20),
	)
	base = machine.BinaryVal(
		machine.ValueOpAdd,
		base,
		itemOffset,
	)

	lowAddr := machine.MemoryAddress{
		Seg:   machine.RegVal(asm.RegDS),
		Base:  base,
		Width: 2,
	}
	highAddr := lowAddr
	highAddr.Disp = 2

	got, changed := (&collapseWideStoresProcessor{ctx: ctx}).
		collapseWideMachineStorePair(
			machine.StoreEffect{
				Addr:  lowAddr,
				Src:   machine.ConstVal(0),
				Width: 2,
			},
			machine.StoreEffect{
				Addr:  highAddr,
				Src:   machine.ConstVal(0),
				Width: 2,
			},
		)

	if !changed {
		t.Fatal("nested indexed wide store did not collapse")
	}
	if got.Width != 4 || got.Addr.Width != 4 {
		t.Fatalf("collapsed widths = (%d, %d), want (4, 4)",
			got.Width, got.Addr.Width)
	}

	converted := (&machineConverter{ctx: ctx}).convertEffect(got)
	assign, ok := converted.(*Assign)
	if !ok {
		t.Fatalf("converted effect = %T, want *Assign", converted)
	}
	if want, have := "rgplr[i].rgResSpent[j]", FormatExpr(assign.Dst); have != want {
		t.Fatalf("destination = %q, want %q", have, want)
	}
}
