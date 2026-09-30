package region

import "slices"

// hoistSearchExits reshapes search loops so shapeLoop can give them a loop
// condition. A search loop leaves through a leading test that jumps away
// and through a single break to the code that handles what it found:
//
//	while (1) { if (c) goto L; ... if (x) break; ... }  F
//
// F, which only that break reaches, moves to the break, and the leading test
// becomes the loop's break with its jump placed after the loop:
//
//	while (1) { if (c) break; ... if (x) { F } ... }  goto L;
//
// Control flow is unchanged: F still runs exactly when the break did, and
// the only way out of the loop left is the leading test. refs counts the
// gotos to each label and is kept up to date.
func hoistSearchExits(nodes []Node, next []string, refs map[string]int) []Node {
	var out []Node
	for i := 0; i < len(nodes); i++ {
		fall := fallLabels(nodes, i, next)
		switch n := nodes[i].(type) {
		case *If:
			n.Then = hoistSearchExits(n.Then, fall, refs)
			n.Else = hoistSearchExits(n.Else, fall, refs)
		case *Switch:
			for c := range n.Cases {
				caseNext := fall
				if c+1 < len(n.Cases) {
					caseNext = fallLabels(n.Cases[c+1].Body, -1, nil)
				}
				n.Cases[c].Body = hoistSearchExits(n.Cases[c].Body, caseNext, refs)
			}
		case *Loop:
			n.Body = hoistSearchExits(n.Body, []string{n.Header}, refs)
			if after, consumed, ok := hoistSearchExit(n, nodes[i+1:], next, refs); ok {
				out = append(out, n)
				out = append(out, after...)
				i += consumed
				continue
			}
		}
		out = append(out, nodes[i])
	}
	return out
}

// hoistSearchExit rewrites loop n, followed by the nodes after it and then
// by the labels in next, as hoistSearchExits describes. It returns the nodes
// to place right after the loop and how many nodes after it moved into the
// loop, or false when n is not a search loop it can reshape. When nothing
// follows the loop before the leading test's own target, the test simply
// becomes the break.
func hoistSearchExit(n *Loop, after []Node, next []string, refs map[string]int) ([]Node, int, bool) {
	if n.Kind != LoopForever || len(n.Body) < 2 {
		return nil, 0, false
	}
	lead, ok := n.Body[0].(*If)
	if !ok || len(lead.Else) != 0 || !endsInJump(lead.Then) || !movable(lead.Then, refs) {
		return nil, 0, false
	}
	exit := lead.Then
	// jumpsTo reports whether the leading test only jumps to one of labels.
	jumpsTo := func(labels []string) bool {
		g, isGoto := exit[0].(*Goto)
		return len(exit) == 1 && isGoto && slices.Contains(labels, g.Label)
	}

	// With no break of its own the loop is only left through the leading
	// test, so the test can be its break, with its jump placed after the loop.
	if loopBreaks(n.Body[1:]) == 0 {
		n.Body[0] = &If{Cond: lead.Cond, Then: []Node{&Break{}}}
		if jumpsTo(fallLabels(after, -1, next)) {
			refs[exit[0].(*Goto).Label]--
			return nil, 0, true
		}
		return exit, 0, true
	}

	// F runs from the loop to the next label that something jumps to.
	end := 0
	for end < len(after) {
		if l, isLabel := after[end].(*Label); isLabel && refs[l.Name] > 0 {
			break
		}
		end++
	}
	fallAfter := fallLabels(after, end-1, next)
	if end == 0 {
		if !jumpsTo(fallAfter) {
			return nil, 0, false
		}
		refs[exit[0].(*Goto).Label]--
		n.Body[0] = &If{Cond: lead.Cond, Then: []Node{&Break{}}}
		breakGotos(n.Body, fallAfter, refs)
		return nil, 0, true
	}
	if loopBreaks(n.Body[1:]) != 1 || !movable(after[:end], refs) {
		return nil, 0, false
	}

	found := slices.Clone(after[:end])
	if !endsInJump(found) {
		// F falls through, so the loop's new break must land where F did,
		// which it does only if the leading test jumped there too.
		if !jumpsTo(fallAfter) {
			return nil, 0, false
		}
		found = append(found, &Break{})
	}
	if jumpsTo(fallAfter) {
		refs[exit[0].(*Goto).Label]--
		exit = nil
	}

	body := replaceLoopBreak(n.Body[1:], hoistSearchExits(found, nil, refs))
	n.Body = append([]Node{&If{Cond: lead.Cond, Then: []Node{&Break{}}}}, body...)
	// The break now lands on the leading test's target, or on F's labels.
	// An exit that is code rather than one goto has no label to land on.
	target := fallAfter
	if exit != nil {
		target = nil
		if g, isGoto := exit[0].(*Goto); isGoto && len(exit) == 1 {
			target = []string{g.Label}
		}
	}
	breakGotos(n.Body, target, refs)
	return exit, end, true
}

// breakGotos turns the gotos in a loop's body that jump to where its break
// lands, one of labels, into breaks. Gotos in nested loops and switches keep
// jumping, since a break there leaves the inner statement instead.
func breakGotos(nodes []Node, labels []string, refs map[string]int) {
	for i, n := range nodes {
		switch n := n.(type) {
		case *Goto:
			if slices.Contains(labels, n.Label) {
				refs[n.Label]--
				nodes[i] = &Break{}
			}
		case *If:
			breakGotos(n.Then, labels, refs)
			breakGotos(n.Else, labels, refs)
		}
	}
}

// movable reports whether nodes can run from another place: no goto
// reaches a label among them, and no break or continue in them leaves an
// enclosing loop or switch.
func movable(nodes []Node, refs map[string]int) bool {
	ok := true
	walkNodes(nodes, func(n Node) {
		if l, isLabel := n.(*Label); isLabel && refs[l.Name] > 0 {
			ok = false
		}
	})
	return ok && loopBreaks(nodes) == 0 && !hasContinue(nodes)
}

// loopBreaks counts the breaks in nodes that leave the loop or switch the
// nodes sit in: those outside nested loops and switches.
func loopBreaks(nodes []Node) int {
	count := 0
	for _, n := range nodes {
		switch n := n.(type) {
		case *Break:
			count++
		case *If:
			count += loopBreaks(n.Then) + loopBreaks(n.Else)
		}
	}
	return count
}

// replaceLoopBreak returns nodes with the break that leaves their loop, as
// loopBreaks counts it, replaced by body.
func replaceLoopBreak(nodes []Node, body []Node) []Node {
	var out []Node
	for _, n := range nodes {
		switch n := n.(type) {
		case *Break:
			out = append(out, body...)
			continue
		case *If:
			n.Then = replaceLoopBreak(n.Then, body)
			n.Else = replaceLoopBreak(n.Else, body)
		}
		out = append(out, n)
	}
	return out
}
