package sem

import (
	"slices"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestStorageLaneProjectsDynamicPointerFieldIndex verifies a residual runtime
// index is applied after selecting an array field through a pointer.
func TestStorageLaneProjectsDynamicPointerFieldIndex(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "DoMacintiAiTurn")
	index := slices.IndexFunc(ctx.fs.Vars, func(v typeinfo.FunctionVar) bool { return v.Name == "lppl" })
	if index < 0 {
		t.Fatal("lppl local not found")
	}

	addr := resolvedAddress{
		base:   &symresolve.SymbolRoot{Symbol: &ctx.fs.Vars[index]},
		offset: 0x1c,
		terms: []resolvedAddressTerm{{
			value: machine.BinaryVal(machine.ValueOpMod, machine.SignExtendVal(machine.RegVal(asm.RegAX), 16, 32), machine.ConstVal(3)),
			scale: 4,
		}},
		deref: true,
	}
	lane, ok := ctx.symbols.storageLaneFromResolvedAddress(addr)
	if !ok {
		t.Fatal("dynamic pointer field lane did not resolve")
	}
	term, ok := lane.object.(*symresolve.SymbolTerm)
	if !ok {
		t.Fatalf("lane object = %T, want *symresolve.SymbolTerm", lane.object)
	}
	field, ok := term.Base.(*symresolve.SymbolField)
	if !ok || field.Field.Name != "rgwtMin" {
		t.Fatalf("indexed base = %v, want lppl->rgwtMin", term.Base)
	}
	if lane.offset != 0 || lane.size != 4 {
		t.Fatalf("lane range = (%d, %d), want (0, 4)", lane.offset, lane.size)
	}
}

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
