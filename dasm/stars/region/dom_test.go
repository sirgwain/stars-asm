package region

import (
	"slices"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// testBlock builds an IR block whose label is derived from id.
func testBlock(id machine.BlockID, stmts ...ir.Stmt) ir.Block {
	return ir.Block{ID: id, Label: id.String(), StartOff: uint32(id), EndOff: uint32(id) + 0x10, Stmts: stmts}
}

// testIfGoto builds a conditional jump on the named variable.
func testIfGoto(cond string, t, f machine.BlockID) *ir.IfGoto {
	return &ir.IfGoto{Cond: &ir.Var{Name: cond}, TrueLabel: t.String(), FalseLabel: f.String()}
}

// testIncrement builds v = v + 1.
func testIncrement(v string) *ir.Assign {
	return &ir.Assign{Dst: &ir.Var{Name: v}, Src: &ir.Binary{Op: "+", LHS: &ir.Var{Name: v}, RHS: &ir.IntConst{Value: 1}}}
}

// wantIDoms checks every expected immediate dominator in tree.
func wantIDoms(t *testing.T, name string, tree *DomTree, want map[machine.BlockID]machine.BlockID) {
	t.Helper()
	for id, wantDom := range want {
		got, ok := tree.IDom(id)
		if !ok || got != wantDom {
			t.Errorf("%s(%s) = %s (ok=%v), want %s", name, id, got, ok, wantDom)
		}
	}
}

// TestDiamondThenLoop verifies graph edges, dominators, and post-dominators
// for an if/else diamond that merges into a while loop.
func TestDiamondThenLoop(t *testing.T) {
	const a, b, c, d, e, f machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60
	fn := ir.Func{Name: "Diamond", Blocks: []ir.Block{
		testBlock(a, testIfGoto("x", c, b)),
		testBlock(b, &ir.Assign{Dst: &ir.Var{Name: "y"}, Src: &ir.IntConst{Value: 1}}, &ir.Goto{Label: d.String()}),
		testBlock(c, &ir.Assign{Dst: &ir.Var{Name: "y"}, Src: &ir.IntConst{Value: 2}}),
		testBlock(d, testIfGoto("more", e, f)),
		testBlock(e, testIncrement("i"), &ir.Goto{Label: d.String()}),
		testBlock(f, &ir.Return{}),
	}}

	g, err := NewGraph(&fn)
	if err != nil {
		t.Fatal(err)
	}
	for id, want := range map[machine.BlockID][]machine.BlockID{
		a: {c, b}, b: {d}, c: {d}, d: {e, f}, e: {d}, f: {ExitID},
	} {
		if got := g.Successors(id); !slices.Equal(got, want) {
			t.Errorf("Successors(%s) = %v, want %v", id, got, want)
		}
	}
	if got, want := g.Predecessors(d), []machine.BlockID{b, c, e}; !slices.Equal(got, want) {
		t.Errorf("Predecessors(%s) = %v, want %v", d, got, want)
	}

	dom := Dominators(g)
	wantIDoms(t, "idom", dom, map[machine.BlockID]machine.BlockID{b: a, c: a, d: a, e: d, f: d})
	if !dom.Dominates(d, e) || dom.Dominates(b, d) || !dom.Dominates(a, a) {
		t.Errorf("Dominates gave wrong answers for d→e, b→d, a→a")
	}

	pdom := PostDominators(g)
	wantIDoms(t, "ipdom", pdom, map[machine.BlockID]machine.BlockID{a: d, b: d, c: d, e: d, d: f, f: ExitID})
}

// TestNestedLoopsAndInfiniteLoop verifies dominators across nested loops and
// that a block that never reaches the exit has no post-dominator.
func TestNestedLoopsAndInfiniteLoop(t *testing.T) {
	const a, h1, h2, body, latch, x, spin, done machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80
	fn := ir.Func{Name: "Nested", Blocks: []ir.Block{
		testBlock(a, &ir.Assign{Dst: &ir.Var{Name: "i"}, Src: &ir.IntConst{Value: 0}}),
		testBlock(h1, testIfGoto("outer", h2, x)),
		testBlock(h2, testIfGoto("inner", body, latch)),
		testBlock(body, testIncrement("j"), &ir.Goto{Label: h2.String()}),
		testBlock(latch, testIncrement("i"), &ir.Goto{Label: h1.String()}),
		testBlock(x, testIfGoto("hang", spin, done)),
		testBlock(spin, &ir.Goto{Label: spin.String()}),
		testBlock(done, &ir.Return{}),
	}}

	g, err := NewGraph(&fn)
	if err != nil {
		t.Fatal(err)
	}

	dom := Dominators(g)
	wantIDoms(t, "idom", dom, map[machine.BlockID]machine.BlockID{
		h1: a, h2: h1, body: h2, latch: h2, x: h1, spin: x, done: x,
	})

	pdom := PostDominators(g)
	wantIDoms(t, "ipdom", pdom, map[machine.BlockID]machine.BlockID{
		a: h1, h1: x, h2: latch, body: h2, latch: h1, x: done, done: ExitID,
	})
	if pdom.Contains(spin) {
		t.Errorf("post-dominator tree contains infinite loop block %s", spin)
	}
}
