package region

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// NaturalLoop is a natural loop: its header plus every block that reaches a latch
// without passing through the header.
type NaturalLoop struct {
	Header machine.BlockID
	// Latches are the sources of back edges to Header, in reverse postorder.
	Latches []machine.BlockID
	// Body holds every block in the loop, including Header.
	Body map[machine.BlockID]bool
	// Exits are the blocks outside Body that the loop jumps to, in reverse
	// postorder. Returns are statements, so ExitID is never an exit.
	Exits []machine.BlockID
	// Parent is the innermost enclosing loop, or nil for an outermost loop.
	Parent *NaturalLoop
}

// Edge is a directed edge between two blocks.
type Edge struct{ From, To machine.BlockID }

// Facts are the control-flow facts that structuring walks: dominator trees,
// loops, and merge points.
type Facts struct {
	Dom     *DomTree
	PostDom *DomTree
	// Loops holds every natural loop, outer loops before the loops they contain.
	Loops []*NaturalLoop
	// LoopByHeader maps a loop header to its loop.
	LoopByHeader map[machine.BlockID]*NaturalLoop
	// Innermost maps each block inside a loop to the innermost loop containing it.
	Innermost map[machine.BlockID]*NaturalLoop
	// Merges holds blocks with more than one forward (non-back-edge) predecessor.
	Merges map[machine.BlockID]bool
	// Irreducible holds retreating edges whose target does not dominate their
	// source, meaning a loop that can be entered at more than one block.
	Irreducible []Edge
}

// NewFacts computes the dominator trees, natural loops, merge points, and
// irreducible edges of g. Blocks unreachable from the entry are ignored.
func NewFacts(g *Graph) *Facts {
	f := &Facts{
		Dom:          Dominators(g),
		PostDom:      PostDominators(g),
		LoopByHeader: map[machine.BlockID]*NaturalLoop{},
		Innermost:    map[machine.BlockID]*NaturalLoop{},
		Merges:       map[machine.BlockID]bool{},
	}

	forwardPreds := map[machine.BlockID]int{}
	for _, from := range f.Dom.Order {
		for _, to := range g.Successors(from) {
			switch {
			case f.Dom.Dominates(to, from):
				loop := f.LoopByHeader[to]
				if loop == nil {
					loop = &NaturalLoop{Header: to, Body: map[machine.BlockID]bool{to: true}}
					f.LoopByHeader[to] = loop
					f.Loops = append(f.Loops, loop)
				}
				loop.Latches = append(loop.Latches, from)
			case f.Dom.rpo[to] <= f.Dom.rpo[from]:
				f.Irreducible = append(f.Irreducible, Edge{From: from, To: to})
			default:
				forwardPreds[to]++
			}
		}
	}
	for id, n := range forwardPreds {
		if n > 1 && id != ExitID {
			f.Merges[id] = true
		}
	}

	for _, loop := range f.Loops {
		f.fillLoopBody(g, loop)
	}
	f.nestLoops()
	return f
}

// fillLoopBody collects the blocks of loop by walking predecessors back from
// its latches to its header, then records the blocks the loop exits to.
func (f *Facts) fillLoopBody(g *Graph, loop *NaturalLoop) {
	work := slices.Clone(loop.Latches)
	for len(work) > 0 {
		id := work[len(work)-1]
		work = work[:len(work)-1]
		if loop.Body[id] || !f.Dom.Contains(id) {
			continue
		}
		loop.Body[id] = true
		work = append(work, g.Predecessors(id)...)
	}

	seen := map[machine.BlockID]bool{}
	for _, id := range f.Dom.Order {
		if !loop.Body[id] {
			continue
		}
		for _, s := range g.Successors(id) {
			if !loop.Body[s] && s != ExitID && !seen[s] {
				seen[s] = true
				loop.Exits = append(loop.Exits, s)
			}
		}
	}
	slices.SortFunc(loop.Exits, func(a, b machine.BlockID) int { return f.Dom.rpo[a] - f.Dom.rpo[b] })
}

// nestLoops orders Loops from outermost to innermost and sets each loop's
// Parent and each block's innermost loop. In a reducible graph two natural
// loops are either nested or disjoint, so visiting larger bodies first leaves
// every block assigned to the smallest loop containing it.
func (f *Facts) nestLoops() {
	slices.SortStableFunc(f.Loops, func(a, b *NaturalLoop) int {
		if len(a.Body) != len(b.Body) {
			return len(b.Body) - len(a.Body)
		}
		return f.Dom.rpo[a.Header] - f.Dom.rpo[b.Header]
	})
	for _, loop := range f.Loops {
		loop.Parent = f.Innermost[loop.Header]
		for id := range loop.Body {
			f.Innermost[id] = loop
		}
	}
}
