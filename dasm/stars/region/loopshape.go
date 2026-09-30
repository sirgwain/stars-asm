package region

import "github.com/sirgwain/stars-asm/dasm/stars/ir"

// shapeLoops rewrites while (1) loops in nodes into while, do-while, and for
// loops, innermost loops first. refs counts the gotos to each label in the
// whole function; it is updated as gotos become continue.
func shapeLoops(nodes []Node, refs map[string]int) []Node {
	var out []Node
	for _, n := range nodes {
		switch n := n.(type) {
		case *If:
			n.Then = shapeLoops(n.Then, refs)
			n.Else = shapeLoops(n.Else, refs)
		case *Switch:
			for i := range n.Cases {
				n.Cases[i].Body = shapeLoops(n.Cases[i].Body, refs)
			}
		case *Loop:
			n.Body = shapeLoops(n.Body, refs)
			shapeLoop(n, refs)
			if n.Kind == LoopFor {
				out = takeForInit(n, out, refs)
			}
		}
		out = append(out, n)
	}
	return out
}

// shapeLoop picks the loop statement for a while (1) loop. A leading
// "if (c) break;" makes it while (!c). Otherwise a trailing "if (c) break;"
// makes it do-while (!c), but only without continue, which would skip the
// test in a do-while. A while loop may then become a for loop.
func shapeLoop(n *Loop, refs map[string]int) {
	if n.Kind != LoopForever || len(n.Body) == 0 {
		return
	}
	if c := breakCond(n.Body[0]); c != nil {
		n.Kind, n.Cond, n.Body = LoopWhile, Negate(c), n.Body[1:]
		shapeFor(n, refs)
		return
	}
	last := len(n.Body) - 1
	if c := breakCond(n.Body[last]); c != nil && !hasContinue(n.Body) {
		n.Kind, n.Cond, n.Body = LoopDoWhile, Negate(c), n.Body[:last]
	}
}

// breakCond returns c when n is "if (c) break;", or nil.
func breakCond(n Node) ir.Expr {
	i, ok := n.(*If)
	if !ok || len(i.Then) != 1 || len(i.Else) != 0 {
		return nil
	}
	if _, ok := i.Then[0].(*Break); !ok {
		return nil
	}
	return i.Cond
}

// shapeFor turns a while loop whose body ends with an assignment to a
// variable its condition reads into a for loop with that assignment as Post.
// A continue in a for loop runs Post, so the loop must have none of its own.
// When Post was the whole latch block and every goto to the latch's label is
// at this loop's level, those gotos become continue and the label is removed.
// Otherwise the gotos stay and land on the label at the end of the body.
func shapeFor(n *Loop, refs map[string]int) {
	if len(n.Body) == 0 || hasContinue(n.Body) {
		return
	}
	last := len(n.Body) - 1
	basic, ok := n.Body[last].(*Basic)
	if !ok {
		return
	}
	post, ok := basic.Stmts[len(basic.Stmts)-1].(*ir.Assign)
	if !ok {
		return
	}
	v, ok := post.Dst.(*ir.Var)
	if !ok || varRefs(n.Cond, v.Name) == 0 {
		return
	}

	n.Kind, n.Post = LoopFor, post
	if len(basic.Stmts) > 1 {
		n.Body[last] = &Basic{Stmts: basic.Stmts[:len(basic.Stmts)-1]}
		return
	}
	n.Body = n.Body[:last]

	if last == 0 {
		return
	}
	latch, ok := n.Body[last-1].(*Label)
	if !ok || levelGotos(n.Body, latch.Name) != refs[latch.Name] {
		return
	}
	continueGotos(n.Body, latch.Name)
	refs[latch.Name] = 0
	n.Body = n.Body[:last-1]
	n.Latch = latch.Name
}

// takeForInit moves an assignment to the loop variable that runs just before
// a for loop into its Init. Only labels with no gotos may lie between them,
// since a jump to a label before the for statement would rerun Init.
func takeForInit(n *Loop, out []Node, refs map[string]int) []Node {
	v := n.Post.Dst.(*ir.Var)
	for i := len(out) - 1; i >= 0; i-- {
		switch prev := out[i].(type) {
		case *Label:
			if refs[prev.Name] == 0 {
				continue
			}
		case *Basic:
			init, ok := prev.Stmts[len(prev.Stmts)-1].(*ir.Assign)
			if !ok {
				return out
			}
			if dst, ok := init.Dst.(*ir.Var); !ok || dst.Name != v.Name {
				return out
			}
			n.Init = init
			if len(prev.Stmts) == 1 {
				return append(out[:i], out[i+1:]...)
			}
			out[i] = &Basic{Stmts: prev.Stmts[:len(prev.Stmts)-1]}
		}
		return out
	}
	return out
}

// hasContinue reports whether nodes contain a continue for their own loop,
// ignoring nested loops.
func hasContinue(nodes []Node) bool {
	found := false
	walkLoopLevel(nodes, func(n Node) {
		if _, ok := n.(*Continue); ok {
			found = true
		}
	})
	return found
}

// levelGotos counts the gotos to label in nodes, outside nested loops.
func levelGotos(nodes []Node, label string) int {
	count := 0
	walkLoopLevel(nodes, func(n Node) {
		if g, ok := n.(*Goto); ok && g.Label == label {
			count++
		}
	})
	return count
}

// continueGotos replaces the gotos to label in nodes, outside nested loops,
// with continue.
func continueGotos(nodes []Node, label string) {
	var replace func([]Node)
	replace = func(nodes []Node) {
		for i, n := range nodes {
			switch n := n.(type) {
			case *Goto:
				if n.Label == label {
					nodes[i] = &Continue{}
				}
			case *If:
				replace(n.Then)
				replace(n.Else)
			case *Switch:
				for _, c := range n.Cases {
					replace(c.Body)
				}
			}
		}
	}
	replace(nodes)
}

// walkLoopLevel calls visit for every node in nodes that belongs to the same
// loop, skipping the bodies of nested loops.
func walkLoopLevel(nodes []Node, visit func(Node)) {
	for _, n := range nodes {
		visit(n)
		switch n := n.(type) {
		case *If:
			walkLoopLevel(n.Then, visit)
			walkLoopLevel(n.Else, visit)
		case *Switch:
			for _, c := range n.Cases {
				walkLoopLevel(c.Body, visit)
			}
		}
	}
}

// varRefs counts the reads of the variable name in e.
func varRefs(e ir.Expr, name string) int {
	count := 0
	switch e := e.(type) {
	case *ir.Var:
		if e.Name == name {
			count++
		}
	case *ir.Unary:
		count += varRefs(e.X, name)
	case *ir.Binary:
		count += varRefs(e.LHS, name) + varRefs(e.RHS, name)
	case *ir.Cond:
		count += varRefs(e.Cond, name) + varRefs(e.Then, name) + varRefs(e.Else, name)
	case *ir.Cast:
		count += varRefs(e.Value, name)
	case *ir.Index:
		count += varRefs(e.Base, name) + varRefs(e.Index, name)
	case *ir.Field:
		count += varRefs(e.Base, name)
	case *ir.Call:
		for _, a := range e.Args {
			count += varRefs(a, name)
		}
		count += varRefs(e.Target, name)
	case *ir.Macro:
		for _, a := range e.Args {
			count += varRefs(a, name)
		}
	case *ir.AddressOf:
		count += varRefs(e.Target, name)
	case *ir.Deref:
		count += varRefs(e.Pointer, name)
	case *ir.PointerOffset:
		count += varRefs(e.Pointer, name) + varRefs(e.Offset, name)
	}
	return count
}
