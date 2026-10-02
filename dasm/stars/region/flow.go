package region

import (
	"fmt"
	"maps"
	"slices"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
)

// exitMark stands for the virtual exit in reached-label sets.
const exitMark = "<exit>"

// flowCtx says where control goes when it leaves a node sequence: by falling
// off its end, by Break, or by Continue (to the loop header label cont).
type flowCtx struct {
	fall func(out map[string]bool)
	brk  func(out map[string]bool)
	cont string
}

// flowChecker traces control through a structured tree from each block's
// label to the labels it reaches next.
type flowChecker struct {
	starts map[string]func(out map[string]bool)
	err    error
}

// checkFlow verifies that body, before unused labels are pruned, sends each
// block to exactly the successors it has in g. Every block starts at its own
// Label, so running from that label until the next label, jump, or return
// gives the block's successors in the structured tree.
func checkFlow(g *Graph, body []Node) error {
	c := &flowChecker{starts: map[string]func(map[string]bool){}}
	c.index(body, flowCtx{
		fall: func(out map[string]bool) { out[exitMark] = true },
		brk:  func(map[string]bool) { c.fail("break outside a loop") },
	})
	if c.err != nil {
		return c.err
	}

	for _, id := range g.Order {
		if id == ExitID {
			continue
		}
		label := g.blocks[id].Label
		start, ok := c.starts[label]
		if !ok {
			return fmt.Errorf("block %s is missing from the structured tree", label)
		}

		want := map[string]bool{}
		for _, s := range g.Successors(id) {
			if s == ExitID {
				want[exitMark] = true
			} else {
				want[g.blocks[s].Label] = true
			}
		}
		got := map[string]bool{}
		start(got)
		if c.err != nil {
			return c.err
		}
		if !maps.Equal(got, want) {
			return fmt.Errorf("block %s reaches %s in the structured tree, want %s", label, formatMarks(got), formatMarks(want))
		}
	}
	return nil
}

// fail records the first structural error the checker finds.
func (c *flowChecker) fail(format string, args ...any) {
	if c.err == nil {
		c.err = fmt.Errorf(format, args...)
	}
}

// index records, for every label in nodes, how to run from just after it.
func (c *flowChecker) index(nodes []Node, ctx flowCtx) {
	for i, n := range nodes {
		switch n := n.(type) {
		case *Label:
			if _, dup := c.starts[n.Name]; dup {
				c.fail("label %s appears more than once", n.Name)
			}
			c.starts[n.Name] = func(out map[string]bool) { c.run(nodes, i+1, ctx, out) }
		case *If:
			after := c.after(nodes, i, ctx)
			c.index(n.Then, after)
			c.index(n.Else, after)
		case *Loop:
			// A for loop's removed latch block only ran Post, then the header.
			if n.Latch != "" {
				if _, dup := c.starts[n.Latch]; dup {
					c.fail("label %s appears more than once", n.Latch)
				}
				c.starts[n.Latch] = func(out map[string]bool) { out[n.Header] = true }
			}
			c.index(latchlessBody(n), c.loopCtx(n, nodes, i, ctx))
		case *Switch:
			for j, cs := range n.Cases {
				c.index(cs.Body, c.caseCtx(n, j, nodes, i, ctx))
			}
		}
	}
}

// latchlessBody returns loop n's body without the latch label kept at its end
// for gotos from nested loops. Falling off the shortened body enters the
// latch, just as reaching the label did.
func latchlessBody(n *Loop) []Node {
	last := len(n.Body) - 1
	if n.Latch == "" || last < 0 {
		return n.Body
	}
	if l, ok := n.Body[last].(*Label); ok && l.Name == n.Latch {
		return n.Body[:last]
	}
	return n.Body
}

// run adds to out the labels control reaches from nodes[i] onward, stopping
// at the first label, jump, or return on each path.
func (c *flowChecker) run(nodes []Node, i int, ctx flowCtx, out map[string]bool) {
	for ; i < len(nodes); i++ {
		switch n := nodes[i].(type) {
		case *Label:
			out[n.Name] = true
			return
		case *Basic:
			if slices.ContainsFunc(n.Stmts, func(s ir.Stmt) bool { _, ok := s.(*ir.Return); return ok }) {
				out[exitMark] = true
				return
			}
		case *Goto:
			out[n.Label] = true
			return
		case *Break:
			ctx.brk(out)
			return
		case *Continue:
			if ctx.cont == "" {
				c.fail("continue outside a loop")
				return
			}
			out[ctx.cont] = true
			return
		case *If:
			after := c.after(nodes, i, ctx)
			c.run(n.Then, 0, after, out)
			c.run(n.Else, 0, after, out)
			return
		case *Loop:
			loop := c.loopCtx(n, nodes, i, ctx)
			c.run(n.Body, 0, loop, out)
			// while and for test their condition before the first pass.
			if n.Kind == LoopWhile || n.Kind == LoopFor {
				loop.brk(out)
			}
			return
		case *Switch:
			for j, cs := range n.Cases {
				c.run(cs.Body, 0, c.caseCtx(n, j, nodes, i, ctx), out)
			}
			return
		}
	}
	ctx.fall(out)
}

// after returns the context of a nested sequence that falls through to the
// node following nodes[i].
func (c *flowChecker) after(nodes []Node, i int, ctx flowCtx) flowCtx {
	return flowCtx{
		fall: func(out map[string]bool) { c.run(nodes, i+1, ctx, out) },
		brk:  ctx.brk,
		cont: ctx.cont,
	}
}

// loopCtx returns the context of a loop body. Break leaves the loop. Falling
// off the end returns to the header, whose code is the condition; a do-while
// tests its condition there, so it may also leave. In a for loop whose latch
// block was removed, falling off the end and continue both enter the latch.
// Otherwise continue returns to the header; a do-while has none.
func (c *flowChecker) loopCtx(loop *Loop, nodes []Node, i int, ctx flowCtx) flowCtx {
	brk := func(out map[string]bool) { c.run(nodes, i+1, ctx, out) }
	lc := flowCtx{
		fall: func(out map[string]bool) { out[loop.Header] = true },
		brk:  brk,
		cont: loop.Header,
	}
	switch loop.Kind {
	case LoopDoWhile:
		lc.fall = func(out map[string]bool) {
			out[loop.Header] = true
			brk(out)
		}
		lc.cont = ""
	case LoopFor:
		if loop.Latch != "" {
			lc.fall = func(out map[string]bool) { out[loop.Latch] = true }
			lc.cont = loop.Latch
		}
	}
	return lc
}

// caseCtx returns the context of case j: falling off its end runs the next
// case, and break leaves the switch.
func (c *flowChecker) caseCtx(sw *Switch, j int, nodes []Node, i int, ctx flowCtx) flowCtx {
	leave := func(out map[string]bool) { c.run(nodes, i+1, ctx, out) }
	fall := leave
	if j+1 < len(sw.Cases) {
		fall = func(out map[string]bool) { c.run(sw.Cases[j+1].Body, 0, c.caseCtx(sw, j+1, nodes, i, ctx), out) }
	}
	return flowCtx{fall: fall, brk: leave, cont: ctx.cont}
}

// formatMarks renders a reached-label set in sorted order.
func formatMarks(marks map[string]bool) string {
	return "{" + strings.Join(slices.Sorted(maps.Keys(marks)), ", ") + "}"
}
