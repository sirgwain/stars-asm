package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestFunctionFactDependentEnumAtEntry verifies configured discriminator facts
// select dependent enums in the entry block, before any predecessor propagates them.
func TestFunctionFactDependentEnumAtEntry(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "FGetBestDefensePart")
	root, ok := ctx.symbolRootByName("part")
	if !ok {
		t.Fatal("part local not found")
	}
	path, ok := appendSymbolFieldPath(root, []string{"hs", "iItem"})
	if !ok {
		t.Fatal("part.hs.iItem path not found")
	}
	fn := Func{
		CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{retForReturnSinkTest(0x21f6)}),
		Blocks: []Block{{ID: 0x21f6, Effects: []Effect{
			&Assign{Dst: &SymbolRef{Path: path}, Src: testConst(9)},
			&Return{Value: testConst(1)},
		}}},
	}
	(&unionContextProcessor{ctx: ctx}).ProcessFunc(newResult(ctx.fs), &fn)
	ctx.SetCurrentBlock(0x21f6)
	resolved, _ := (&resolveEnumsProcessor{ctx: ctx}).ProcessBlock(newResult(ctx.fs), fn, fn.Blocks[0])
	if got := FormatEffect(resolved.Effects[0]); got != "part.hs.iItem = iplanetarySDI" {
		t.Fatalf("entry assignment = %s", got)
	}
}

// TestUnionContextObserverRecordsUsedBlockFact verifies union selections are recorded by block.
func TestUnionContextObserverRecordsUsedBlockFact(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "FTrackXfer")
	global := fx.SDB.GetGlobal("pxfer")
	if global == nil {
		t.Fatal("global pxfer not found")
	}
	root := &symresolve.SymbolRoot{Symbol: global}
	unionType, ok := typeinfo.UnwrapPointer(global.Type)
	if !ok {
		t.Fatal("pxfer is not a pointer")
	}
	rule, ok := fx.SDB.UnionRules.UnionVariantForType(unionType)
	if !ok {
		t.Fatal("no union rule for pxfer")
	}
	value := rule.Enum.GetValue("grobjFleet")
	unionCtx := symresolve.NewUnionContext()
	if !unionCtx.AddAllElements(root, rule, value) {
		t.Fatal("failed to add union selection")
	}
	block := machine.BlockID(0x5def)
	ctx.SetUnionContexts(map[machine.BlockID]*symresolve.UnionContext{block: unionCtx})
	ctx.SetCurrentBlock(block)
	previous := unionCtx.SwapSelectionObserver(func(selection symresolve.UnionSelection) {
		ctx.addUnionSelectionBlockFact(block, selection)
	})
	if _, ok := unionCtx.AllElementsSelectionFor(root, unionType.(*typeinfo.Struct)); !ok {
		t.Fatal("union selection not found")
	}
	unionCtx.SwapSelectionObserver(previous)

	facts := ctx.RecordedUnionBlockPathFacts()[block]
	if len(facts) != 1 {
		t.Fatalf("got %d recorded facts, want 1", len(facts))
	}
	fact := facts[0]
	if fact.Root != "pxfer" || !fact.AllElements || fact.Value.Name != "grobjFleet" {
		t.Fatalf("unexpected fact: %+v", fact)
	}
	analysis := (&Func{}).Analyze(ctx)
	if len(analysis.FunctionPathFacts) != 1 {
		t.Fatalf("analysis did not expose recorded block fact: %+v", analysis.FunctionPathFacts)
	}
	if got := analysis.FunctionPathFacts[0].BlockPathFacts[0].Value; got != "grobjFleet" {
		t.Fatalf("analysis block fact value = %q, want grobjFleet", got)
	}

	resolvedBlock := machine.BlockID(0x5e34)
	ctx.SetUnionContexts(map[machine.BlockID]*symresolve.UnionContext{
		block:         unionCtx,
		resolvedBlock: unionCtx,
	})
	planet := rule.Enum.GetValue("grobjPlanet")
	planetMember, ok := rule.MemberForValue(planet.Value)
	if !ok {
		t.Fatal("XFER planet member not found")
	}
	fn := &Func{Blocks: []Block{{
		ID: resolvedBlock,
		Effects: []Effect{&Assign{
			Dst: &FieldAccess{
				Base: &ArrayIndex{
					Base:     &Global{GlobalVar: global},
					Index:    &Const{TypeInfo: typeinfo.I16, U64: 0},
					TypeInfo: unionType,
				},
				Field: planetMember,
			},
			Src: &Const{TypeInfo: typeinfo.I16, U64: 1},
		}},
	}}}
	analysis = fn.Analyze(ctx)
	if len(analysis.FunctionPathFacts) != 1 || len(analysis.FunctionPathFacts[0].BlockPathFacts) != 5 {
		t.Fatalf("analysis function path facts = %+v, want all five configured blocks", analysis.FunctionPathFacts)
	}
	if got := analysis.FunctionPathFacts[0].BlockPathFacts[0].Block; got != "L_5def" {
		t.Fatalf("first aggregated block = %q, want L_5def", got)
	}
	if got := analysis.FunctionPathFacts[0].BlockPathFacts[1].Block; got != "L_5e34" {
		t.Fatalf("second aggregated block = %q, want L_5e34", got)
	}
	if got := analysis.FunctionPathFacts[0].BlockPathFacts[1].Value; got != "grobjFleet" {
		t.Fatalf("resolved union access block fact value = %q, want grobjFleet", got)
	}
	if got := analysis.FunctionPathFacts[0].BlockPathFacts[4].Block; got != "L_5ecd" {
		t.Fatalf("last configured block = %q, want L_5ecd", got)
	}
}
