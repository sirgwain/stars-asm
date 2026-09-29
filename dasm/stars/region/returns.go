package region

import "github.com/sirgwain/stars-asm/dasm/stars/ir"

// duplicateReturns replaces each goto to a label whose block only returns a
// simple value with that return, as in a window procedure whose handlers all
// jump to a shared "return 0". A goto transfers straight to its label, so
// returning at the goto returns the same value. A return block left with no
// gotos and no fallthrough into it, because the node before it always
// jumps, is dropped.
func duplicateReturns(body []Node) []Node {
	returns := map[string]*ir.Return{}
	collectReturnLabels(body, returns)
	if len(returns) == 0 {
		return body
	}
	body = replaceReturnGotos(body, returns)
	body = pruneLabels(body)
	return dropDeadReturns(body)
}

// collectReturnLabels records in returns each label in nodes that starts a
// run of labels ending in a block holding only a duplicable return.
func collectReturnLabels(nodes []Node, returns map[string]*ir.Return) {
	for i, n := range nodes {
		switch n := n.(type) {
		case *Label:
			j := i
			for j < len(nodes) {
				if _, ok := nodes[j].(*Label); !ok {
					break
				}
				j++
			}
			if ret, ok := returnOnly(nodes, j); ok {
				returns[n.Name] = ret
			}
		case *If:
			collectReturnLabels(n.Then, returns)
			collectReturnLabels(n.Else, returns)
		case *Loop:
			collectReturnLabels(n.Body, returns)
		case *Switch:
			for _, c := range n.Cases {
				collectReturnLabels(c.Body, returns)
			}
		}
	}
}

// returnOnly returns the return of nodes[i] when it is a block holding only
// a return of nothing, a constant, or a variable, possibly cast: values
// that read the same wherever the return is placed and stay short when
// repeated.
func returnOnly(nodes []Node, i int) (*ir.Return, bool) {
	if i >= len(nodes) {
		return nil, false
	}
	basic, ok := nodes[i].(*Basic)
	if !ok || len(basic.Stmts) != 1 {
		return nil, false
	}
	ret, ok := basic.Stmts[0].(*ir.Return)
	if !ok {
		return nil, false
	}
	value := ret.Value
	for {
		cast, ok := value.(*ir.Cast)
		if !ok {
			break
		}
		value = cast.Value
	}
	switch value.(type) {
	case nil, *ir.IntConst, *ir.Var:
		return ret, true
	}
	return nil, false
}

// replaceReturnGotos replaces the gotos in nodes to a label in returns with
// that label's return.
func replaceReturnGotos(nodes []Node, returns map[string]*ir.Return) []Node {
	for i, n := range nodes {
		switch n := n.(type) {
		case *Goto:
			if ret, ok := returns[n.Label]; ok {
				nodes[i] = &Basic{Stmts: []ir.Stmt{ret}}
			}
		case *If:
			n.Then = replaceReturnGotos(n.Then, returns)
			n.Else = replaceReturnGotos(n.Else, returns)
		case *Loop:
			n.Body = replaceReturnGotos(n.Body, returns)
		case *Switch:
			for j := range n.Cases {
				n.Cases[j].Body = replaceReturnGotos(n.Cases[j].Body, returns)
			}
		}
	}
	return nodes
}

// dropDeadReturns removes return-only blocks that directly follow a node
// that always jumps: with no label before them, nothing reaches them.
func dropDeadReturns(nodes []Node) []Node {
	out := nodes[:0]
	for i, n := range nodes {
		switch n := n.(type) {
		case *Basic:
			if _, ok := returnOnly(nodes, i); ok && endsInJump(out) {
				continue
			}
		case *If:
			n.Then = dropDeadReturns(n.Then)
			n.Else = dropDeadReturns(n.Else)
		case *Loop:
			n.Body = dropDeadReturns(n.Body)
		case *Switch:
			for j := range n.Cases {
				n.Cases[j].Body = dropDeadReturns(n.Cases[j].Body)
			}
		}
		out = append(out, n)
	}
	return out
}
