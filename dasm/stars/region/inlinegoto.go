package region

import (
	"maps"
	"slices"
	"strings"
)

// inlineSingleGotos moves code that exactly one goto reaches to that goto:
//
//	if (x) goto L; ... return a;  L: F   becomes   if (x) { F } ... return a;
//
// F runs from L to the next label a goto reaches. Nothing may fall into L,
// F must always jump away so nothing falls out of it, and F must be movable:
// no other goto reaches inside it and no break or continue in it belongs to
// the statement around it. F then runs exactly when the goto did. Only
// generated L_ labels are inlined; a named label is a goto the source wrote.
// Each round first turns ifs whose arms jump past the code after them into
// if-elses, which can leave more labels with a single goto. refs counts the
// gotos to each label and is kept up to date.
func inlineSingleGotos(body []Node, refs map[string]int) []Node {
	for changed := true; changed; {
		body, changed = joinTailGotos(body, refs)
		for _, label := range slices.Sorted(maps.Keys(refs)) {
			if refs[label] != 1 || !strings.HasPrefix(label, "L_") {
				continue
			}
			rest, code, ok := extractLabel(body, label, refs)
			if !ok {
				continue
			}
			body = replaceGoto(rest, label, code)
			refs[label] = 0
			changed = true
		}
	}
	return body
}

// extractLabel finds label in the tree and, when the code it starts can move
// as inlineSingleGotos describes, returns the tree without that code and the
// code, starting with the label.
func extractLabel(nodes []Node, label string, refs map[string]int) ([]Node, []Node, bool) {
	for j, n := range nodes {
		switch n := n.(type) {
		case *Label:
			if n.Name != label {
				continue
			}
			end := j + 1
			for end < len(nodes) {
				if l, isLabel := nodes[end].(*Label); isLabel && refs[l.Name] > 0 {
					break
				}
				end++
			}
			// The label moves with its code, which checkFlow needs to find the
			// block; pruning drops it once no goto reaches it.
			code := slices.Clone(nodes[j:end])
			if j == 0 || !alwaysJumps(nodes[:j]) || !alwaysJumps(code) || !movable(code[1:], refs) || gotosTo(code, label) > 0 {
				return nil, nil, false
			}
			return append(slices.Clone(nodes[:j]), nodes[end:]...), code, true
		case *If:
			if then, code, ok := extractLabel(n.Then, label, refs); ok {
				n.Then = then
				return nodes, code, true
			}
			if els, code, ok := extractLabel(n.Else, label, refs); ok {
				n.Else = els
				return nodes, code, true
			}
		case *Loop:
			if body, code, ok := extractLabel(n.Body, label, refs); ok {
				n.Body = body
				return nodes, code, true
			}
		case *Switch:
			for c := range n.Cases {
				if body, code, ok := extractLabel(n.Cases[c].Body, label, refs); ok {
					n.Cases[c].Body = body
					return nodes, code, true
				}
			}
		}
	}
	return nil, nil, false
}

// gotosTo counts the gotos to label in nodes, including nested ones.
func gotosTo(nodes []Node, label string) int {
	count := 0
	walkNodes(nodes, func(n Node) {
		if g, ok := n.(*Goto); ok && g.Label == label {
			count++
		}
	})
	return count
}

// replaceGoto returns nodes with the goto to label replaced by code.
func replaceGoto(nodes []Node, label string, code []Node) []Node {
	var out []Node
	for _, n := range nodes {
		switch n := n.(type) {
		case *Goto:
			if n.Label == label {
				out = append(out, code...)
				continue
			}
		case *If:
			n.Then = replaceGoto(n.Then, label, code)
			n.Else = replaceGoto(n.Else, label, code)
		case *Loop:
			n.Body = replaceGoto(n.Body, label, code)
		case *Switch:
			for c := range n.Cases {
				n.Cases[c].Body = replaceGoto(n.Cases[c].Body, label, code)
			}
		}
		out = append(out, n)
	}
	return out
}

// joinTailGotos turns an If whose Then always jumps, some paths to the label
// after the code that follows it, into an if-else:
//
//	if (c) { A; goto L; } B  L:   becomes   if (c) { A } else { B }  L:
//
// Paths through A that jumped to L now fall out of the If onto L instead,
// and paths that jumped elsewhere still do; B still runs exactly when c is
// false. B must hold no label another goto reaches. Only generated L_ labels
// are rewritten. It reports whether it changed anything; refs is kept up to
// date.
func joinTailGotos(nodes []Node, refs map[string]int) ([]Node, bool) {
	changed := false
	for i := 0; i < len(nodes); i++ {
		switch n := nodes[i].(type) {
		case *If:
			var c1, c2 bool
			n.Then, c1 = joinTailGotos(n.Then, refs)
			n.Else, c2 = joinTailGotos(n.Else, refs)
			changed = changed || c1 || c2
		case *Loop:
			var c bool
			n.Body, c = joinTailGotos(n.Body, refs)
			changed = changed || c
		case *Switch:
			for k := range n.Cases {
				var c bool
				n.Cases[k].Body, c = joinTailGotos(n.Cases[k].Body, refs)
				changed = changed || c
			}
		}
	}
	for i := len(nodes) - 1; i >= 0; i-- {
		n, ok := nodes[i].(*If)
		if !ok || len(n.Else) != 0 || !alwaysJumps(n.Then) {
			continue
		}
		end := slices.IndexFunc(nodes[i+1:], func(x Node) bool {
			l, isLabel := x.(*Label)
			return isLabel && refs[l.Name] > 0
		})
		if end < 0 {
			continue
		}
		label := nodes[i+1+end].(*Label).Name
		if !strings.HasPrefix(label, "L_") || tailGotos(n.Then, label) == 0 {
			continue
		}
		n.Then = dropTailGotos(n.Then, label, refs)
		n.Else = slices.Clone(nodes[i+1 : i+1+end])
		n.Laid = false
		if len(n.Then) == 0 {
			n.Cond, n.Then, n.Else = Negate(n.Cond), n.Else, nil
		}
		nodes = append(slices.Clone(nodes[:i+1]), nodes[i+1+end:]...)
		changed = true
	}
	return nodes, changed
}

// tailGotos counts the gotos to label that end a path through nodes: the
// last node, or the last node of either arm of a last If.
func tailGotos(nodes []Node, label string) int {
	if len(nodes) == 0 {
		return 0
	}
	switch n := nodes[len(nodes)-1].(type) {
	case *Goto:
		if n.Label == label {
			return 1
		}
	case *If:
		return tailGotos(n.Then, label) + tailGotos(n.Else, label)
	}
	return 0
}

// dropTailGotos removes the gotos tailGotos counts, so those paths fall off
// the end of nodes instead.
func dropTailGotos(nodes []Node, label string, refs map[string]int) []Node {
	if len(nodes) == 0 {
		return nodes
	}
	switch n := nodes[len(nodes)-1].(type) {
	case *Goto:
		if n.Label == label {
			refs[label]--
			return nodes[:len(nodes)-1]
		}
	case *If:
		n.Then = dropTailGotos(n.Then, label, refs)
		n.Else = dropTailGotos(n.Else, label, refs)
	}
	return nodes
}
