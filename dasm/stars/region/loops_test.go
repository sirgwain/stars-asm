package region

import (
	"maps"
	"slices"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// TestNestedLoopFacts verifies loop bodies, latches, exits, nesting, and merge
// points for a for-loop nested in a while-loop with an early return.
func TestNestedLoopFacts(t *testing.T) {
	const a, h1, h2, body, skip, cont, latch, x machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80
	fn := ir.Func{Name: "Nested", Blocks: []ir.Block{
		testBlock(a, &ir.Assign{Dst: &ir.Var{Name: "i"}, Src: &ir.IntConst{Value: 0}}),
		testBlock(h1, testIfGoto("outer", h2, x)),
		testBlock(h2, testIfGoto("inner", body, latch)),
		testBlock(body, testIfGoto("found", skip, cont)),
		testBlock(skip, &ir.Return{}),
		testBlock(cont, testIncrement("j"), &ir.Goto{Label: h2.String()}),
		testBlock(latch, testIncrement("i"), &ir.Goto{Label: h1.String()}),
		testBlock(x, &ir.Return{}),
	}}

	g, err := NewGraph(&fn)
	if err != nil {
		t.Fatal(err)
	}
	f := NewFacts(g)

	if len(f.Loops) != 2 || f.Loops[0].Header != h1 || f.Loops[1].Header != h2 {
		t.Fatalf("loops = %v, want outer %s then inner %s", loopHeaders(f.Loops), h1, h2)
	}
	outer, inner := f.Loops[0], f.Loops[1]

	checkLoop := func(loop *NaturalLoop, parent *NaturalLoop, body, latches, exits []machine.BlockID) {
		t.Helper()
		if got := slices.Sorted(maps.Keys(loop.Body)); !slices.Equal(got, body) {
			t.Errorf("loop %s body = %v, want %v", loop.Header, got, body)
		}
		if !slices.Equal(loop.Latches, latches) {
			t.Errorf("loop %s latches = %v, want %v", loop.Header, loop.Latches, latches)
		}
		if !slices.Equal(loop.Exits, exits) {
			t.Errorf("loop %s exits = %v, want %v", loop.Header, loop.Exits, exits)
		}
		if loop.Parent != parent {
			t.Errorf("loop %s parent = %v, want %v", loop.Header, loop.Parent, parent)
		}
	}
	checkLoop(outer, nil, []machine.BlockID{h1, h2, body, cont, latch}, []machine.BlockID{latch}, []machine.BlockID{x, skip})
	checkLoop(inner, outer, []machine.BlockID{h2, body, cont}, []machine.BlockID{cont}, []machine.BlockID{latch, skip})

	for id, want := range map[machine.BlockID]*NaturalLoop{h1: outer, latch: outer, h2: inner, cont: inner} {
		if got := f.Innermost[id]; got != want {
			t.Errorf("Innermost(%s) = %v, want loop %s", id, got, want.Header)
		}
	}
	if f.Innermost[x] != nil || f.Innermost[skip] != nil {
		t.Errorf("blocks outside every loop have an innermost loop")
	}

	if got := slices.Sorted(maps.Keys(f.Merges)); len(got) != 0 {
		t.Errorf("merges = %v, want none", got)
	}
	if len(f.Irreducible) != 0 {
		t.Errorf("irreducible = %v, want none", f.Irreducible)
	}
}

// TestMergeAndIrreducibleFacts verifies merge points after an if/else and
// detection of a cycle that can be entered at two blocks.
func TestMergeAndIrreducibleFacts(t *testing.T) {
	const a, b, c, d, e, x machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60
	fn := ir.Func{Name: "Irreducible", Blocks: []ir.Block{
		testBlock(a, testIfGoto("x", b, c)),
		testBlock(b, testIncrement("y"), &ir.Goto{Label: d.String()}),
		testBlock(c, testIncrement("z")),
		testBlock(d, testIfGoto("left", e, x)),
		testBlock(e, testIfGoto("again", d, x)),
		testBlock(x, &ir.Return{}),
	}}
	// d and e form a reducible loop headed by d; a second graph below enters
	// a cycle at both of its blocks.
	g, err := NewGraph(&fn)
	if err != nil {
		t.Fatal(err)
	}
	f := NewFacts(g)
	if got := slices.Sorted(maps.Keys(f.Merges)); !slices.Equal(got, []machine.BlockID{d, x}) {
		t.Errorf("merges = %v, want [%s %s]", got, d, x)
	}
	if len(f.Loops) != 1 || f.Loops[0].Header != d || len(f.Irreducible) != 0 {
		t.Errorf("loops = %v irreducible = %v, want one loop at %s", loopHeaders(f.Loops), f.Irreducible, d)
	}

	fn = ir.Func{Name: "TwoEntries", Blocks: []ir.Block{
		testBlock(a, testIfGoto("x", b, c)),
		testBlock(b, testIfGoto("y", c, x)),
		testBlock(c, testIfGoto("z", b, x)),
		testBlock(x, &ir.Return{}),
	}}
	g, err = NewGraph(&fn)
	if err != nil {
		t.Fatal(err)
	}
	f = NewFacts(g)
	if len(f.Loops) != 0 || len(f.Irreducible) != 1 {
		t.Errorf("loops = %v irreducible = %v, want no loops and one irreducible edge", loopHeaders(f.Loops), f.Irreducible)
	}
}

// loopHeaders returns the header of each loop for failure messages.
func loopHeaders(loops []*NaturalLoop) []machine.BlockID {
	out := make([]machine.BlockID, len(loops))
	for i, loop := range loops {
		out[i] = loop.Header
	}
	return out
}
