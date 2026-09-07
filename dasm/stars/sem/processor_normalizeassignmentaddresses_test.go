package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestNormalizeAssignmentAddressesPreProcessorResolvesNearPointerReturn
// verifies the machine pre-pass resolves bare DGROUP offsets returned from
// near-pointer functions.
func TestNormalizeAssignmentAddressesPreProcessorResolvesNearPointerReturn(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(
		fx.Image,
		fx.SDB,
		symresolve.NewResolver(fx.Image, fx.SDB),
		fx.SDB.GetFunction("SzVersion"),
	)
	block := machine.BlockEffects{Effects: []machine.Effect{
		machine.ReturnEffect{Value: machine.ConstVal(0x57a4)},
	}}

	gotBlock, changed := (&normalizeAssignmentAddressesProcessor{ctx: ctx}).ProcessMachineBlock(nil, machine.FuncEffects{}, block)
	if !changed {
		t.Fatal("ProcessMachineBlock changed = false, want true")
	}
	ret, ok := gotBlock.Effects[0].(machine.ReturnEffect)
	if !ok {
		t.Fatalf("effect type = %T, want machine.ReturnEffect", gotBlock.Effects[0])
	}
	got := ret.Value.String()
	want := "addr(ds:[0x57a4])"
	if got != want {
		t.Fatalf("return value = %q, want %q", got, want)
	}
}
