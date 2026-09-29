package region

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
)

// simplify rewrites the gotos in nodes into structured control flow. next
// holds the labels control reaches when it falls off the end of nodes; brk
// holds the labels a break reaches, and cont is the label a continue reaches.
//
// A goto to a fallthrough label is dropped, a goto to a break label becomes
// Break, and a goto to the loop header becomes Continue. An If whose Then
// ends in a jump has its Else hoisted after it. A loop whose exits all go to
// one label elsewhere breaks to a single goto placed after it. In a switch,
// a case falls through to the next case and break leaves the switch.
func simplify(nodes []Node, next, brk []string, cont string) []Node {
	var out []Node
	for i, n := range nodes {
		fall := fallLabels(nodes, i, next)
		switch n := n.(type) {
		case *Goto:
			if j := jumpNode(n, fall, brk, cont); j != nil {
				out = append(out, j)
			}
			continue
		case *If:
			n.Then = simplify(n.Then, fall, brk, cont)
			n.Else = simplify(n.Else, fall, brk, cont)
			out = append(out, shapeIf(n)...)
			continue
		case *Loop:
			if exit, ok := soleLoopExit(n, fall); ok {
				n.Body = simplify(n.Body, []string{n.Header}, []string{exit}, n.Header)
				out = append(out, n)
				if j := jumpNode(&Goto{Label: exit}, fall, brk, cont); j != nil {
					out = append(out, j)
				}
				continue
			}
			n.Body = simplify(n.Body, []string{n.Header}, fall, n.Header)
		case *Switch:
			for j := range n.Cases {
				caseNext := fall
				if j+1 < len(n.Cases) {
					caseNext = fallLabels(n.Cases[j+1].Body, -1, nil)
				}
				n.Cases[j].Body = simplify(n.Cases[j].Body, caseNext, fall, cont)
			}
		}
		out = append(out, n)
	}
	return out
}

// jumpNode returns what a goto becomes: nil when it falls through to its
// label anyway, Break or Continue when its label is where those go, and the
// goto itself otherwise.
func jumpNode(g *Goto, fall, brk []string, cont string) Node {
	switch {
	case slices.Contains(fall, g.Label):
		return nil
	case slices.Contains(brk, g.Label):
		return &Break{}
	case g.Label == cont:
		return &Continue{}
	}
	return g
}

// soleLoopExit reports the label that every exit from loop jumps to, when
// there is exactly one, it is not where control lands after the loop anyway,
// and at least one of those jumps can become break. The loop's breaks then
// land after it, where a single goto to the label continues.
func soleLoopExit(loop *Loop, fall []string) (string, bool) {
	inside := map[string]bool{loop.Header: true}
	walkNodes(loop.Body, func(n Node) {
		if l, ok := n.(*Label); ok {
			inside[l.Name] = true
		}
	})
	exit := ""
	sole := true
	walkNodes(loop.Body, func(n Node) {
		if g, ok := n.(*Goto); ok && !inside[g.Label] {
			if exit != "" && exit != g.Label {
				sole = false
			}
			exit = g.Label
		}
	})
	if !sole || exit == "" || slices.Contains(fall, exit) {
		return "", false
	}
	return exit, breakableGotos(loop.Body, exit) > 0
}

// breakableGotos counts the gotos to label in nodes that could become break
// for the loop holding nodes: those not inside a nested loop or switch.
func breakableGotos(nodes []Node, label string) int {
	count := 0
	for _, n := range nodes {
		switch n := n.(type) {
		case *Goto:
			if n.Label == label {
				count++
			}
		case *If:
			count += breakableGotos(n.Then, label) + breakableGotos(n.Else, label)
		}
	}
	return count
}

// fallLabels returns the labels control reaches after nodes[i]: the run of
// labels that follows it, plus next when that run reaches the end of nodes.
// When a goto directly follows nodes[i], falling off nodes[i] reaches the
// goto's target instead. A goto after a label is left alone, since the label
// starts another block.
func fallLabels(nodes []Node, i int, next []string) []string {
	if i+1 < len(nodes) {
		if g, ok := nodes[i+1].(*Goto); ok {
			return []string{g.Label}
		}
	}
	var out []string
	for _, n := range nodes[i+1:] {
		label, ok := n.(*Label)
		if !ok {
			return out
		}
		out = append(out, label.Name)
	}
	return append(out, next...)
}

// shapeIf puts a lone arm in Then and, when one arm ends in a jump, puts that
// arm in Then and hoists the other arm after the If.
func shapeIf(n *If) []Node {
	if len(n.Else) > 0 && (len(n.Then) == 0 || (!endsInJump(n.Then) && endsInJump(n.Else))) {
		n.Cond = Negate(n.Cond)
		n.Then, n.Else = n.Else, n.Then
	}
	if len(n.Else) > 0 && endsInJump(n.Then) {
		rest := n.Else
		n.Else = nil
		return append([]Node{n}, rest...)
	}
	return []Node{n}
}

// endsInJump reports whether control never falls off the end of nodes
// because its last node jumps or returns.
func endsInJump(nodes []Node) bool {
	if len(nodes) == 0 {
		return false
	}
	switch n := nodes[len(nodes)-1].(type) {
	case *Goto, *Break, *Continue:
		return true
	case *Basic:
		_, ok := n.Stmts[len(n.Stmts)-1].(*ir.Return)
		return ok
	}
	return false
}
