package symresolve_test

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestUnionContextAllElementsSelection(t *testing.T) {
	fx := testfixture.Stars(t)
	xfer := fx.SDB.GetStruct("XFER")
	rule, ok := fx.SDB.UnionRules.UnionVariantForType(xfer)
	if !ok {
		t.Fatal("XFER union variant rule not found")
	}
	root := &symresolve.SymbolRoot{Symbol: fx.SDB.GetGlobal("pxfer")}
	element := &symresolve.SymbolTerm{
		Base:   root,
		Index:  &symresolve.SymbolLiteral{Literal: 1, Typ: typeinfo.I16},
		Scale:  xfer.Bytes(),
		Result: xfer,
	}
	ctx := symresolve.NewUnionContext()
	grobjFleet := rule.Enum.GetValue("grobjFleet")
	grobjPlanet := rule.Enum.GetValue("grobjPlanet")

	if !ctx.AddAllElements(root, rule, grobjFleet) {
		t.Fatal("AddAllElements did not add the fleet selection")
	}
	selection, ok := ctx.SelectionFor(element, xfer)
	if !ok || selection.Member.Name != "fl" {
		t.Fatalf("element selection = %#v, %v; want fl", selection, ok)
	}
	if !ctx.Add(element, rule, grobjPlanet) {
		t.Fatal("Add did not add the exact planet selection")
	}
	selection, ok = ctx.SelectionFor(element, xfer)
	if !ok || selection.Member.Name != "pl" {
		t.Fatalf("exact selection = %#v, %v; want pl", selection, ok)
	}
	if !ctx.Add(root, rule, grobjFleet) {
		t.Fatal("Add did not add the direct fleet selection")
	}
	selection, ok = ctx.SelectionFor(&symresolve.SymbolDeref{Base: root}, xfer)
	if !ok || selection.Member.Name != "fl" {
		t.Fatalf("dereferenced-root selection = %#v, %v; want fl", selection, ok)
	}
}

func TestUnionBlockPathFactsAreSparse(t *testing.T) {
	fx := testfixture.Stars(t)
	fn := fx.SDB.GetFunction("FTrackXfer")
	facts := fx.SDB.UnionRules.BlockFactsFor(fn, 0x5def)
	if len(facts) != 1 {
		t.Fatalf("L_5def facts = %d, want 1", len(facts))
	}
	if facts[0].Root != "pxfer" || !facts[0].AllElements || facts[0].Value.Name != "grobjFleet" {
		t.Fatalf("L_5def fact = %#v", facts[0])
	}
	if facts := fx.SDB.UnionRules.BlockFactsFor(fn, 0x5dcb); len(facts) != 0 {
		t.Fatalf("L_5dcb facts = %d, want 0", len(facts))
	}
}
