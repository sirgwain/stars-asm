package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

func TestFarPointerMemoryPreservesPointerDereference(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "PszGetLine")

	psz := localNamed(t, ctx.fs, "psz")

	pointer := frameLoad(
		ctx,
		0xb5, // 0x696f - 0x68ba
		psz.BPOffset,
		4,
	)
	mem := machine.MemoryAddress{
		Seg: machine.FarPointerVal(
			pointer,
			machine.FarPointerSegment,
		),
		Base: machine.FarPointerVal(
			pointer,
			machine.FarPointerOffset,
		),
		Width: 1,
		Origin: machine.Origin{
			InstOff: 0x6972,
			Role:    machine.OperandDst,
		},
	}

	resolved, ok := ctx.symbols.addressFromMemory(mem, nil)
	if !ok {
		t.Fatal("far-pointer memory did not resolve")
	}
	if !resolved.deref {
		t.Fatalf("resolved address = %#v, want deref=true", resolved)
	}

	got, ok := (&machineConverter{ctx: ctx}).resolveAddressLValue(
		mem,
		1,
		nil,
	)
	if !ok {
		t.Fatal("far-pointer lvalue did not resolve")
	}

	if formatted := FormatExpr(got); formatted != "*psz" {
		t.Fatalf("resolved lvalue = %q, want *psz", formatted)
	}
}
