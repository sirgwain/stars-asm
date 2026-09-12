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

// TestDirectUnionMembersKeepRegionsIndependent verifies collection choices,
// exact overrides, and context merges preserve independent overlap regions.
func TestDirectUnionMembersKeepRegionsIndependent(t *testing.T) {
	fx := testfixture.Stars(t)
	fn := fx.SDB.GetFunction("FIsAiAttack")
	facts := fx.SDB.UnionRules.BlockMemberFactsFor(fn, 0x4ad0)
	if len(facts) != 1 {
		t.Fatalf("configured member facts = %d, want 1", len(facts))
	}
	power := facts[0]
	shdef := power.Type
	flags := *power
	visible := *power
	for _, match := range shdef.FieldsContainingOffset(0x7b) {
		if match.Field.Name == "wFlags" {
			flags.Member = match.Field
		}
	}
	for i := range shdef.OverlapRegions {
		if shdef.OverlapRegions[i].Start == 0x7b {
			flags.Region = &shdef.OverlapRegions[i]
		}
	}
	for _, match := range shdef.FieldsContainingOffset(0x87) {
		if match.Field.Name == "lVisible" {
			visible.Member = match.Field
		}
	}
	if flags.Member.Name != "wFlags" || flags.Region == power.Region || visible.Member.Name != "lVisible" {
		t.Fatal("SHDEF union layout did not resolve")
	}
	root := &symresolve.SymbolRoot{Symbol: fx.SDB.GetGlobal("rglpshdef")}
	player := &symresolve.SymbolTerm{
		Base: root, Index: &symresolve.SymbolLiteral{Literal: 1, Typ: typeinfo.I16},
		Scale: 4, Result: root.Type().(*typeinfo.Array).Elem,
	}
	element := &symresolve.SymbolTerm{
		Base: player, Index: &symresolve.SymbolLiteral{Literal: 2, Typ: typeinfo.I16},
		Scale: shdef.Bytes(), Result: shdef,
	}
	ctx := symresolve.NewUnionContext()
	ctx.AddMember(root, power)
	ctx.AddMember(root, &flags)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	for _, access := range []struct {
		offset int
		width  int
		member *typeinfo.StructField
	}{{0x87, 4, power.Member}, {0x7b, 2, flags.Member}} {
		path, ok := res.ResolveFieldPathLoadInContext(element, access.offset, access.width, ctx)
		if !ok || path.(*symresolve.SymbolField).Field != access.member {
			t.Fatalf("offset %x did not select %s: %v", access.offset, access.member.Name, path)
		}
	}
	configured := symresolve.NewUnionContext()
	configured.AddMember(root, &visible)
	merged := symresolve.MergeUnionContexts(ctx, configured)
	if merged.Equal(ctx) {
		t.Fatal("different direct member choices compared equal")
	}
	if member, ok := merged.MemberFor(element, shdef, 0x89, false); !ok || member != visible.Member {
		t.Fatal("configured member did not override derived choice")
	}
	shared := symresolve.IntersectUnionContexts([]*symresolve.UnionContext{ctx, merged})
	if _, ok := shared.MemberFor(element, shdef, 0x89, false); ok {
		t.Fatal("intersection retained conflicting power/visibility choices")
	}
	if member, ok := shared.MemberFor(element, shdef, 0x7b, false); !ok || member != flags.Member {
		t.Fatal("intersection lost the independent flags choice")
	}
	exact := *power
	exact.AllElements = false
	merged.AddMember(element, &exact)
	if member, ok := merged.MemberFor(element, shdef, 0x89, false); !ok || member != power.Member {
		t.Fatal("exact selection did not take precedence over the collection")
	}
	if member, ok := ctx.MemberFor(element, shdef, 0x89, false); !ok || member != power.Member {
		t.Fatal("merging mutated the original context")
	}
}
