package region

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// builder places every reachable block once by walking the dominator tree.
// All jumps it emits are gotos; simplify later turns them into fallthrough,
// break, and continue.
type builder struct {
	g     *Graph
	facts *Facts

	// children maps a block to the dominator-tree children emitted after its
	// own code, in reverse postorder.
	children map[machine.BlockID][]machine.BlockID
	// afterLoop maps a loop to the blocks emitted right after it: blocks
	// outside the loop whose immediate dominator is inside it.
	afterLoop map[*NaturalLoop][]machine.BlockID
	emitted   map[machine.BlockID]bool
}

// newBuilder decides where each reachable block is emitted. A block normally
// follows its immediate dominator. When the dominator is inside loops that
// the block is not, the block follows the outermost such loop instead.
func newBuilder(g *Graph, facts *Facts) *builder {
	b := &builder{
		g:         g,
		facts:     facts,
		children:  map[machine.BlockID][]machine.BlockID{},
		afterLoop: map[*NaturalLoop][]machine.BlockID{},
		emitted:   map[machine.BlockID]bool{},
	}
	for _, id := range facts.Dom.Order[1:] {
		if id == ExitID {
			continue
		}
		d, _ := facts.Dom.IDom(id)
		var home *NaturalLoop
		for l := facts.Innermost[d]; l != nil && !l.Body[id]; l = l.Parent {
			home = l
		}
		if home != nil {
			b.afterLoop[home] = append(b.afterLoop[home], id)
		} else {
			b.children[d] = append(b.children[d], id)
		}
	}
	return b
}

// tree emits x under its label, wrapped in a Loop when x is a loop header,
// followed by the blocks placed after it.
func (b *builder) tree(x machine.BlockID) []Node {
	b.emitted[x] = true
	label := b.g.blocks[x].Label
	out := []Node{&Label{Name: label}}

	loop := b.facts.LoopByHeader[x]
	if loop == nil {
		return append(out, b.within(x)...)
	}
	out = append(out, &Loop{Header: label, Body: b.within(x)})
	for _, id := range b.afterLoop[loop] {
		if !b.emitted[id] {
			out = append(out, b.tree(id)...)
		}
	}
	return out
}

// within emits x's own code followed by its dominator-tree children that the
// code did not already inline.
func (b *builder) within(x machine.BlockID) []Node {
	out := b.blockCode(x)
	for _, id := range b.children[x] {
		if !b.emitted[id] {
			out = append(out, b.tree(id)...)
		}
	}
	return out
}

// blockCode emits x's statements and converts its final jump into branches.
func (b *builder) blockCode(x machine.BlockID) []Node {
	stmts, term := splitTerminator(b.g.blocks[x].Stmts)
	var out []Node
	if len(stmts) > 0 {
		out = append(out, &Basic{Stmts: stmts})
	}

	switch t := term.(type) {
	case *ir.IfGoto:
		tID, fID := b.g.byLabel[t.TrueLabel], b.g.byLabel[t.FalseLabel]
		if tID == fID {
			out = append(out, &If{Cond: t.Cond})
			return append(out, b.branch(x, tID)...)
		}
		// The compiler lays out "if (c) A else B" as a jump on !c to B with A
		// falling through, so the fall-through block is the source's Then.
		// When the jump lands on code other paths reach too, the jump itself
		// was the Then, as in "if (c) break;", and no Then block was laid out.
		if owner, ok := b.facts.Dom.IDom(tID); ok && owner == x && b.g.layout[fID] == b.g.layout[x]+1 {
			return append(out, &If{Cond: Negate(t.Cond), Then: b.branch(x, fID), Else: b.branch(x, tID), Laid: true})
		}
		return append(out, &If{Cond: t.Cond, Then: b.branch(x, tID), Else: b.branch(x, fID)})
	case *ir.Goto:
		return append(out, b.branch(x, b.g.byLabel[t.Label])...)
	case *ir.TableJump:
		index, groups := tableCases(t)
		return append(out, b.switchNode(x, index, groups)...)
	case *ir.SwitchGoto:
		return append(out, b.switchNode(x, t.Index, switchGotoCases(t))...)
	}

	if succs := b.g.Successors(x); len(succs) == 1 && succs[0] != ExitID {
		out = append(out, b.branch(x, succs[0])...)
	}
	return out
}

// switchNode builds the Switch that ends x from its case groups. Cases are
// ordered by their destination's position in the code layout, which is the
// order the compiler emitted them in, so fall-through between cases lines up.
// A destination that x dominates is emitted as its case's body, even when other cases also
// jump to it; any other destination is a goto. The block where every path
// from the switch joins is emitted after it, and a goto to it follows the
// switch so that simplify can turn jumps to it into break.
func (b *builder) switchNode(x machine.BlockID, index ir.Expr, groups []caseGroup) []Node {
	slices.SortStableFunc(groups, func(g1, g2 caseGroup) int {
		return b.g.layout[b.g.byLabel[g1.label]] - b.g.layout[b.g.byLabel[g2.label]]
	})

	join, hasJoin := b.facts.PostDom.IDom(x)
	if continuation, ok := b.switchContinuation(x, groups); ok {
		join, hasJoin = continuation, true
	}
	cases := make([]Case, len(groups))
	for i, g := range groups {
		dst := b.g.byLabel[g.label]
		body := []Node{&Goto{Label: g.label}}
		if dst != join && !b.emitted[dst] && slices.Contains(b.children[x], dst) {
			body = b.tree(dst)
		}
		cases[i] = Case{Values: g.values, Default: g.isDefault, Body: body}
	}
	out := []Node{&Switch{Index: index, Cases: cases}}
	if hasJoin && join != ExitID {
		out = append(out, &Goto{Label: b.g.blocks[join].Label})
	}
	return out
}

// switchContinuation finds the common continuation of the ordinary cases
// when the default only returns. It remains a single block after the switch;
// the returning default prevents unmatched values from falling into it.
func (b *builder) switchContinuation(x machine.BlockID, groups []caseGroup) (machine.BlockID, bool) {
	var starts []machine.BlockID
	defaultID := ExitID
	for _, group := range groups {
		id := b.g.byLabel[group.label]
		if group.isDefault {
			defaultID = id
			stmts := b.g.blocks[id].Stmts
			if len(stmts) != 1 {
				return 0, false
			}
			if _, ok := stmts[0].(*ir.Return); !ok {
				return 0, false
			}
		} else {
			starts = append(starts, id)
		}
	}
	if defaultID == ExitID || len(starts) < 2 {
		return 0, false
	}
	for candidate, ok := b.facts.PostDom.IDom(starts[0]); ok && candidate != ExitID; candidate, ok = b.facts.PostDom.IDom(candidate) {
		if candidate == defaultID || !slices.Contains(b.children[x], candidate) {
			continue
		}
		if slices.ContainsFunc(starts, func(id machine.BlockID) bool { return !b.facts.PostDom.Dominates(candidate, id) }) {
			continue
		}
		return candidate, true
	}
	return 0, false
}

// branch emits the jump from src to dst. dst is inlined when src is its only
// way in; otherwise the jump is a goto to dst's label.
func (b *builder) branch(src, dst machine.BlockID) []Node {
	if !b.emitted[dst] && b.inlineInto(src, dst) {
		return b.tree(dst)
	}
	return []Node{&Goto{Label: b.g.blocks[dst].Label}}
}

// inlineInto reports whether dst belongs directly under src: dst is a
// dominator-tree child emitted after src, and not a merge point.
func (b *builder) inlineInto(src, dst machine.BlockID) bool {
	return !b.facts.Merges[dst] && slices.Contains(b.children[src], dst)
}
