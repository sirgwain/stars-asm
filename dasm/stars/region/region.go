package region

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
)

// Func is a function whose IR control flow has been rebuilt as a tree of
// structured nodes.
type Func struct {
	Name   string
	Decl   string
	Locals []ir.Local
	Body   []Node
}

type Node interface{ node() }

// Label marks a goto target.
type Label struct{ Name string }

// node marks Label as a region node.
func (*Label) node() {}

// Basic is a run of straight-line IR statements.
type Basic struct{ Stmts []ir.Stmt }

// node marks Basic as a region node.
func (*Basic) node() {}

// Goto is an unstructured jump to a Label.
type Goto struct{ Label string }

// node marks Goto as a region node.
func (*Goto) node() {}

// If runs Then when Cond is true and Else otherwise.
type If struct {
	Cond       ir.Expr
	Then, Else []Node
}

// node marks If as a region node.
func (*If) node() {}

// LoopKind is the C loop statement a Loop renders as.
type LoopKind int

const (
	// LoopForever is while (1) { Body }.
	LoopForever LoopKind = iota
	// LoopWhile is while (Cond) { Body }.
	LoopWhile
	// LoopDoWhile is do { Body } while (Cond).
	LoopDoWhile
	// LoopFor is for (Init; Cond; Post) { Body }. Init may be nil.
	LoopFor
)

// Loop repeats Body until it breaks, returns, or jumps out. Header is the
// label of the loop's header block, which sits just before the loop.
type Loop struct {
	Kind   LoopKind
	Header string
	Cond   ir.Expr
	Init   *ir.Assign
	Post   *ir.Assign
	// Latch is the label of a for loop's latch block when the block was
	// reduced to Post and removed, so continue now stands for jumps to it.
	Latch string
	Body  []Node
}

// node marks Loop as a region node.
func (*Loop) node() {}

// Break leaves the innermost enclosing Loop.
type Break struct{}

// node marks Break as a region node.
func (*Break) node() {}

// Continue restarts the innermost enclosing Loop.
type Continue struct{}

// node marks Continue as a region node.
func (*Continue) node() {}

// Switch dispatches on Index to the case whose Values contain it.
type Switch struct {
	Index ir.Expr
	Cases []Case
}

// node marks Switch as a region node.
func (*Switch) node() {}

// Case is one Switch arm, entered for any of its Values, or for any value
// no other case lists when Default is set.
type Case struct {
	Values  []ir.Expr
	Default bool
	Body    []Node
}

// Build rebuilds fn's control flow as structured nodes. Flow it cannot
// structure is kept as labels and gotos; it only fails on malformed IR.
func Build(fn ir.Func) (Func, error) {
	g, err := NewGraph(&fn)
	if err != nil {
		return Func{}, err
	}

	out := Func{Name: fn.Name, Decl: fn.Decl, Locals: fn.Locals}
	if g.Opaque {
		out.Body = opaqueBody(fn)
		return out, nil
	}

	fn = threadJumps(fn)
	if g, err = NewGraph(&fn); err != nil {
		return Func{}, err
	}
	fn = recoverSwitches(fn, g)
	if g, err = NewGraph(&fn); err != nil {
		return Func{}, err
	}
	fn = mergeConditions(fn, g)
	if g, err = NewGraph(&fn); err != nil {
		return Func{}, err
	}

	b := newBuilder(g, NewFacts(g))
	body := b.tree(g.Entry)
	for _, id := range b.facts.Dom.Order {
		if id != ExitID && !b.emitted[id] {
			return Func{}, fmt.Errorf("region build %s: reachable block %s was not placed", fn.Name, g.blocks[id].Label)
		}
	}
	for _, block := range fn.Blocks {
		if !b.emitted[block.ID] {
			body = append(body, unreachableBlock(block)...)
		}
	}

	body = simplify(body, nil, nil, "")
	body = shapeLoops(body, gotoCounts(body))
	if err := checkFlow(g, body); err != nil {
		return Func{}, fmt.Errorf("region build %s: %w", fn.Name, err)
	}
	out.Body = duplicateReturns(pruneLabels(body))
	return out, nil
}

// opaqueBody emits every IR block unchanged under its own label. It is used
// when untranslated control flow hides some of the graph's edges.
func opaqueBody(fn ir.Func) []Node {
	var body []Node
	for _, block := range fn.Blocks {
		body = append(body, &Label{Name: block.Label})
		if len(block.Stmts) > 0 {
			body = append(body, &Basic{Stmts: block.Stmts})
		}
	}
	return body
}

// unreachableBlock emits a block that the entry never reaches, keeping its
// jumps as gotos.
func unreachableBlock(block ir.Block) []Node {
	stmts, term := splitTerminator(block.Stmts)
	out := []Node{&Label{Name: block.Label}}
	if len(stmts) > 0 {
		out = append(out, &Basic{Stmts: stmts})
	}
	switch s := term.(type) {
	case *ir.Goto:
		out = append(out, &Goto{Label: s.Label})
	case *ir.IfGoto:
		out = append(out, &If{Cond: s.Cond, Then: []Node{&Goto{Label: s.TrueLabel}}, Else: []Node{&Goto{Label: s.FalseLabel}}})
	case *ir.TableJump:
		index, groups := tableCases(s)
		cases := make([]Case, len(groups))
		for i, g := range groups {
			cases[i] = Case{Values: g.values, Body: []Node{&Goto{Label: g.label}}}
		}
		out = append(out, &Switch{Index: index, Cases: cases})
	}
	return out
}

// splitTerminator separates a block's final jump from its other statements.
// term is nil when the block returns or falls through.
func splitTerminator(stmts []ir.Stmt) (body []ir.Stmt, term ir.Stmt) {
	if n := len(stmts); n > 0 {
		switch stmts[n-1].(type) {
		case *ir.IfGoto, *ir.Goto, *ir.TableJump, *ir.SwitchGoto:
			return stmts[:n-1], stmts[n-1]
		}
	}
	return stmts, nil
}

// caseGroup is one switch destination and the case values that reach it.
type caseGroup struct {
	label     string
	values    []ir.Expr
	isDefault bool
}

// addCase appends value to the group for label, creating the group in order.
func addCase(groups []caseGroup, byLabel map[string]int, label string, value ir.Expr) []caseGroup {
	g, ok := byLabel[label]
	if !ok {
		g = len(groups)
		byLabel[label] = g
		groups = append(groups, caseGroup{label: label})
	}
	groups[g].values = append(groups[g].values, value)
	return groups
}

// switchGotoCases groups a SwitchGoto's values by destination in case order
// and marks the default's group, adding one if no case shares it.
func switchGotoCases(s *ir.SwitchGoto) []caseGroup {
	var groups []caseGroup
	byLabel := map[string]int{}
	for _, c := range s.Cases {
		groups = addCase(groups, byLabel, c.Label, c.Value)
	}
	if g, ok := byLabel[s.Default]; ok {
		groups[g].isDefault = true
	} else {
		groups = append(groups, caseGroup{label: s.Default, isDefault: true})
	}
	return groups
}

// tableCases recovers a switch's index expression and case values from a
// word jump table, grouping the values by destination in table order. The
// table index is a byte offset, index*2; when it is (x - k)*2 or (x + k)*2
// with a numeric k, the switch is on x and the case values are shifted by k.
// An index of another shape is kept as is, with byte-offset case values.
func tableCases(t *ir.TableJump) (ir.Expr, []caseGroup) {
	index, base, step := t.Index, int64(0), int64(2)
	if b, ok := index.(*ir.Binary); ok && b.Op == "*" && intConstValue(b.RHS) == 2 {
		index, step = b.LHS, 1
		if b, ok := index.(*ir.Binary); ok && (b.Op == "-" || b.Op == "+") {
			if k, ok := numericConst(b.RHS); ok {
				index, base = b.LHS, k
				if b.Op == "+" {
					base = -k
				}
			}
		}
	}

	var groups []caseGroup
	byLabel := map[string]int{}
	for i, label := range t.Labels {
		v := base + int64(i)*step
		groups = addCase(groups, byLabel, label, &ir.IntConst{Value: uint64(v), Text: strconv.FormatInt(v, 10)})
	}
	return index, groups
}

// intConstValue returns e's value when it is an integer constant, or -1.
func intConstValue(e ir.Expr) int64 {
	if c, ok := e.(*ir.IntConst); ok {
		return int64(c.Value)
	}
	return -1
}

// numericConst returns the value of an integer constant whose text is a
// number or a character literal. Constants spelled as names, such as enum
// members, are rejected.
func numericConst(e ir.Expr) (int64, bool) {
	c, ok := e.(*ir.IntConst)
	if !ok {
		return 0, false
	}
	if c.Text == "" || strings.HasPrefix(c.Text, "'") {
		return int64(c.Value), true
	}
	v, err := strconv.ParseInt(c.Text, 0, 64)
	return v, err == nil
}

// pruneLabels drops labels that no Goto in body references.
func pruneLabels(body []Node) []Node {
	return filterLabels(body, gotoCounts(body))
}

// gotoCounts returns how many Goto nodes in body target each label.
func gotoCounts(body []Node) map[string]int {
	counts := map[string]int{}
	walkNodes(body, func(n Node) {
		if g, ok := n.(*Goto); ok {
			counts[g.Label]++
		}
	})
	return counts
}

// filterLabels returns nodes without the labels that have no gotos in refs,
// recursing into nested bodies.
func filterLabels(nodes []Node, refs map[string]int) []Node {
	out := nodes[:0]
	for _, n := range nodes {
		switch n := n.(type) {
		case *Label:
			if refs[n.Name] == 0 {
				continue
			}
		case *If:
			n.Then = filterLabels(n.Then, refs)
			n.Else = filterLabels(n.Else, refs)
		case *Loop:
			n.Body = filterLabels(n.Body, refs)
		case *Switch:
			for i := range n.Cases {
				n.Cases[i].Body = filterLabels(n.Cases[i].Body, refs)
			}
		}
		out = append(out, n)
	}
	return out
}

// walkNodes calls visit for every node in nodes, depth first.
func walkNodes(nodes []Node, visit func(Node)) {
	for _, n := range nodes {
		visit(n)
		switch n := n.(type) {
		case *If:
			walkNodes(n.Then, visit)
			walkNodes(n.Else, visit)
		case *Loop:
			walkNodes(n.Body, visit)
		case *Switch:
			for _, c := range n.Cases {
				walkNodes(c.Body, visit)
			}
		}
	}
}
