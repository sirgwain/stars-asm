package region

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

// DomTree is an immediate-dominator tree over the vertices reachable from
// Root. For a post-dominator tree Root is ExitID and edges are reversed.
type DomTree struct {
	Root machine.BlockID
	// Order holds the reachable vertices in reverse postorder from Root.
	Order []machine.BlockID

	idom map[machine.BlockID]machine.BlockID
	rpo  map[machine.BlockID]int
}

// Dominators computes the dominator tree of g from its entry block.
func Dominators(g *Graph) *DomTree {
	return newDomTree(g.Entry, g.Successors, g.Predecessors)
}

// PostDominators computes the post-dominator tree of g from its virtual exit.
// Blocks that cannot reach the exit, such as those inside an infinite loop,
// are not in the tree.
func PostDominators(g *Graph) *DomTree {
	return newDomTree(ExitID, g.Predecessors, g.Successors)
}

// newDomTree computes immediate dominators with the Cooper–Harvey–Kennedy
// iterative algorithm, walking edges forward with succs and backward with preds.
func newDomTree(root machine.BlockID, succs, preds func(machine.BlockID) []machine.BlockID) *DomTree {
	t := &DomTree{
		Root:  root,
		Order: reversePostorder(root, succs),
		idom:  map[machine.BlockID]machine.BlockID{root: root},
	}
	t.rpo = make(map[machine.BlockID]int, len(t.Order))
	for i, id := range t.Order {
		t.rpo[id] = i
	}

	for changed := true; changed; {
		changed = false
		for _, id := range t.Order[1:] {
			newIDom, found := machine.BlockID(0), false
			for _, p := range preds(id) {
				if _, processed := t.idom[p]; !processed {
					continue
				}
				if !found {
					newIDom, found = p, true
					continue
				}
				newIDom = t.intersect(p, newIDom)
			}
			if cur, ok := t.idom[id]; found && (!ok || cur != newIDom) {
				t.idom[id] = newIDom
				changed = true
			}
		}
	}
	return t
}

// intersect returns the nearest common dominator of a and b.
func (t *DomTree) intersect(a, b machine.BlockID) machine.BlockID {
	for a != b {
		for t.rpo[a] > t.rpo[b] {
			a = t.idom[a]
		}
		for t.rpo[b] > t.rpo[a] {
			b = t.idom[b]
		}
	}
	return a
}

// reversePostorder returns the vertices reachable from root in reverse
// postorder, visiting successors in the order succs returns them.
func reversePostorder(root machine.BlockID, succs func(machine.BlockID) []machine.BlockID) []machine.BlockID {
	var post []machine.BlockID
	seen := map[machine.BlockID]bool{}
	var visit func(machine.BlockID)
	visit = func(id machine.BlockID) {
		seen[id] = true
		for _, s := range succs(id) {
			if !seen[s] {
				visit(s)
			}
		}
		post = append(post, id)
	}
	visit(root)

	for i, j := 0, len(post)-1; i < j; i, j = i+1, j-1 {
		post[i], post[j] = post[j], post[i]
	}
	return post
}

// Contains reports whether id is reachable from the tree's root.
func (t *DomTree) Contains(id machine.BlockID) bool {
	_, ok := t.rpo[id]
	return ok
}

// IDom returns the immediate dominator of id. It reports false for the root
// and for vertices outside the tree.
func (t *DomTree) IDom(id machine.BlockID) (machine.BlockID, bool) {
	if id == t.Root {
		return 0, false
	}
	d, ok := t.idom[id]
	return d, ok
}

// Dominates reports whether a dominates b. Every vertex dominates itself.
func (t *DomTree) Dominates(a, b machine.BlockID) bool {
	if !t.Contains(a) || !t.Contains(b) {
		return false
	}
	for {
		if a == b {
			return true
		}
		if b == t.Root {
			return false
		}
		b = t.idom[b]
	}
}
