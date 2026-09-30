package region

import "slices"

// flipGuards un-nests chains of inverted tests, putting the short arm of a
// test first so the long path stays at the outer level. When an If with no
// Else is followed by the rest S of its statement list and its Then always
// jumps away, the two can trade places under the negated test:
//
//	if (c) { T } S   becomes   if (!c) { S } T        when S always jumps
//	if (c) { T } S   becomes   if (!c) { S } else { T }   otherwise
//
// T still runs exactly when c holds and never falls through, and S still
// runs otherwise and ends where it did. The flip is made only when S is
// smaller than T, and, when T is the arm the compiler laid out first and so
// the source's own order, only when T is a large body worth un-nesting. A
// rest holding a label that a goto reaches stays put,
// so no jump lands inside the new If. An If whose Then is a single If and
// whose Else is smaller swaps its arms under the negated test, so a chain of
// "not this case" tests reads as an else-if chain. refs counts the gotos to
// each label.
func flipGuards(nodes []Node, refs map[string]int) []Node {
	for i := 0; i < len(nodes); i++ {
		switch n := nodes[i].(type) {
		case *If:
			n.Then = flipGuards(n.Then, refs)
			n.Else = flipGuards(n.Else, refs)
			if len(n.Else) != 0 {
				// Labels nothing jumps to are pruned before the chain is printed.
				then := slices.DeleteFunc(slices.Clone(n.Then), func(x Node) bool {
					l, isLabel := x.(*Label)
					return isLabel && refs[l.Name] == 0
				})
				if len(then) == 1 && nodeSize(n.Else) < nodeSize(n.Then) {
					if _, chain := then[0].(*If); chain {
						n.Cond, n.Then, n.Else = Negate(n.Cond), n.Else, n.Then
					}
				}
				continue
			}
			rest := nodes[i+1:]
			// A laid-out Then is the arm the source wrote first; it only moves
			// when that un-nests a large body, as after a switch's compare chain.
			if (n.Laid && nodeSize(n.Then) < 30) || len(rest) == 0 || !alwaysJumps(n.Then) || hasJumpTarget(rest, refs) || nodeSize(rest) >= nodeSize(n.Then) {
				continue
			}
			guard := &If{Cond: Negate(n.Cond), Then: flipGuards(slices.Clone(rest), refs)}
			if !alwaysJumps(rest) {
				guard.Else = n.Then
				nodes = append(slices.Clone(nodes[:i]), guard)
				continue
			}
			nodes = append(append(slices.Clone(nodes[:i]), guard), n.Then...)
		case *Loop:
			n.Body = flipGuards(n.Body, refs)
		case *Switch:
			for c := range n.Cases {
				n.Cases[c].Body = flipGuards(n.Cases[c].Body, refs)
			}
		}
	}
	return nodes
}

// alwaysJumps reports whether control never falls off the end of nodes: the
// last node jumps or returns, is an If both of whose arms always jump, or is
// a loop with no condition and no break of its own.
func alwaysJumps(nodes []Node) bool {
	if len(nodes) == 0 {
		return false
	}
	switch n := nodes[len(nodes)-1].(type) {
	case *If:
		return alwaysJumps(n.Then) && alwaysJumps(n.Else)
	case *Loop:
		return n.Kind == LoopForever && loopBreaks(n.Body) == 0
	}
	return endsInJump(nodes)
}

// hasJumpTarget reports whether nodes hold a label that a goto reaches.
func hasJumpTarget(nodes []Node, refs map[string]int) bool {
	found := false
	walkNodes(nodes, func(n Node) {
		if l, ok := n.(*Label); ok && refs[l.Name] > 0 {
			found = true
		}
	})
	return found
}

// nodeSize counts the statements in nodes, including nested ones.
func nodeSize(nodes []Node) int {
	size := 0
	for _, n := range nodes {
		switch n := n.(type) {
		case *Basic:
			size += len(n.Stmts)
		case *Goto, *Break, *Continue:
			size++
		case *If:
			size += 1 + nodeSize(n.Then) + nodeSize(n.Else)
		case *Loop:
			size += 1 + nodeSize(n.Body)
		case *Switch:
			size++
			for _, c := range n.Cases {
				size += nodeSize(c.Body)
			}
		}
	}
	return size
}
