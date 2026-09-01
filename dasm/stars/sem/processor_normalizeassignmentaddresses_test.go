package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestNormalizeAssignmentAddressesProcessorResolvesSemanticNearPointerConst
// verifies a bare DGROUP offset assigned to near-pointer storage resolves to
// the source-level global address.
func TestNormalizeAssignmentAddressesProcessorResolvesSemanticNearPointerConst(t *testing.T) {
	fx := testfixture.Stars(t)
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	nearCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrNear}
	ctx := NewFuncContext(
		fx.Image,
		fx.SDB,
		symresolve.NewResolver(fx.Image, fx.SDB),
		fx.SDB.GetFunction("ClickInPlanetOrders"),
	)
	block := Block{Effects: []Effect{
		&Assign{
			Dst: &Local{FunctionVar: typeinfo.FunctionVar{Name: "psz", Type: nearCharPtrType}},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 0x4a6c},
		},
	}}

	gotBlock, changed := (&normalizeAssignmentAddressesProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "psz = szPopupBuffer"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestNormalizeAssignmentAddressesProcessorResolvesSemanticNearPointerReturn
// verifies a bare DGROUP offset returned from a near-pointer function resolves
// to the source-level global address.
func TestNormalizeAssignmentAddressesProcessorResolvesSemanticNearPointerReturn(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(
		fx.Image,
		fx.SDB,
		symresolve.NewResolver(fx.Image, fx.SDB),
		fx.SDB.GetFunction("SzVersion"),
	)
	block := Block{Effects: []Effect{
		&Return{Value: &Const{TypeInfo: typeinfo.U16, U64: 0x57a4}},
	}}

	gotBlock, changed := (&normalizeAssignmentAddressesProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "return szWork"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestNormalizeAssignmentAddressesPreProcessorResolvesNearPointerReturn
// verifies the machine pre-pass also resolves bare DGROUP offsets returned from
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

// TestNormalizeAssignmentAddressesProcessorResolvesSunkNearPointerReturn
// verifies returns created by return-sink are normalized by the following
// semantic pass.
func TestNormalizeAssignmentAddressesProcessorResolvesSunkNearPointerReturn(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := NewFuncContext(
		fx.Image,
		fx.SDB,
		symresolve.NewResolver(fx.Image, fx.SDB),
		fx.SDB.GetFunction("SzVersion"),
	)
	fn := &Func{
		CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
			jccForReturnSinkTest(0x1000, 0x1004),
			jmpForReturnSinkTest(0x1002, 0x1006),
			jmpForReturnSinkTest(0x1004, 0x1006),
			retForReturnSinkTest(0x1006),
		}),
		Blocks: []Block{
			{ID: 0x1000, Effects: []Effect{&Branch{TrueBlock: 0x1004, FalseBlock: 0x1002}}},
			{ID: 0x1002, Effects: []Effect{&Jump{To: 0x1006}}},
			{ID: 0x1004},
			{
				ID: 0x1006,
				Effects: []Effect{
					&Return{
						MetaInfo: machine.Meta{BlockID: 0x1006, InstOff: 0x1006},
						Value: &Merge{
							TypeInfo: typeinfo.U16,
							Arms: []MergeArm{
								{Block: 0x1002, Value: &Const{TypeInfo: typeinfo.U16, U64: 0x57a4}},
								{Block: 0x1004, Value: &Const{TypeInfo: typeinfo.U16, U64: 0x0}},
							},
						},
					},
				},
			},
		},
	}

	if changed := (&returnSinkProcessor{}).ProcessFunc(nil, fn); !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	processor := &normalizeAssignmentAddressesProcessor{ctx: ctx}
	for i := range fn.Blocks {
		next, changed := processor.ProcessBlock(nil, *fn, fn.Blocks[i])
		if changed {
			fn.Blocks[i] = next
		}
	}

	if got, want := FormatEffect(fn.Blocks[1].Effects[len(fn.Blocks[1].Effects)-1]), "return szWork"; got != want {
		t.Fatalf("first predecessor tail = %q, want %q", got, want)
	}
	if got, want := FormatEffect(fn.Blocks[2].Effects[len(fn.Blocks[2].Effects)-1]), "return 0x0"; got != want {
		t.Fatalf("second predecessor tail = %q, want %q", got, want)
	}
}
