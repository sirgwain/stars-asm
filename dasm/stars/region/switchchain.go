package region

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
)

// minSwitchCases is the fewest equality tests a chain needs to become a switch.
const minSwitchCases = 3

// recoverSwitches replaces chains of equality tests on one expression with a
// SwitchGoto. A chain starts at a block whose final IfGoto tests E == k or
// E != k. Its not-equal edge leads to the next block when that block holds
// only another such test on the same E, has no other predecessor, and tests
// a value not seen yet. The last not-equal target becomes the default.
//
// Only equality is used, so the signedness of E never matters, and E must
// contain no call, because the switch evaluates it once instead of once per
// test. Heads are tried in reverse postorder, so a chain is found from its
// first test. fn is not modified.
func recoverSwitches(fn ir.Func, g *Graph) ir.Func {
	preds := map[string]int{}
	index := map[string]int{}
	for i, block := range fn.Blocks {
		index[block.Label] = i
		preds[block.Label] = len(g.Predecessors(block.ID))
	}

	blocks := slices.Clone(fn.Blocks)
	removed := map[string]bool{}
	for _, id := range Dominators(g).Order {
		if id == ExitID {
			continue
		}
		head := &blocks[g.layout[id]]
		if removed[head.Label] || len(head.Stmts) == 0 {
			continue
		}
		sw, used := switchChain(head, blocks, index, preds, removed)
		if sw == nil {
			continue
		}
		head.Stmts = append(slices.Clone(head.Stmts[:len(head.Stmts)-1]), sw)
		for _, label := range used {
			removed[label] = true
		}
	}

	out := fn
	out.Blocks = slices.DeleteFunc(blocks, func(b ir.Block) bool { return removed[b.Label] })
	return out
}

// switchChain follows the chain of equality tests that starts with head's
// final IfGoto. It returns the SwitchGoto for the chain and the labels of the
// blocks it absorbs, or nil when the chain is too short.
func switchChain(head *ir.Block, blocks []ir.Block, index map[string]int, preds map[string]int, removed map[string]bool) (*ir.SwitchGoto, []string) {
	test, ok := head.Stmts[len(head.Stmts)-1].(*ir.IfGoto)
	if !ok {
		return nil, nil
	}
	e, k, eq, next, ok := equalityTest(test)
	if !ok {
		return nil, nil
	}

	cases := []ir.SwitchCase{{Value: k, Label: eq}}
	seen := map[uint64]bool{k.Value: true}
	var used []string
	for {
		i, ok := index[next]
		if !ok || i == 0 || next == head.Label || removed[next] || preds[next] != 1 {
			break
		}
		b := &blocks[i]
		if len(b.Stmts) != 1 {
			break
		}
		test, ok := b.Stmts[0].(*ir.IfGoto)
		if !ok {
			break
		}
		e2, k2, eq2, next2, ok := equalityTest(test)
		if !ok || !ir.ExprEqual(e, e2) || seen[k2.Value] {
			break
		}
		seen[k2.Value] = true
		cases = append(cases, ir.SwitchCase{Value: k2, Label: eq2})
		used = append(used, next)
		next = next2
	}

	if len(cases) < minSwitchCases {
		return nil, nil
	}
	return &ir.SwitchGoto{Index: e, Cases: cases, Default: next}, used
}

// equalityTest matches an IfGoto on E == k or E != k, where k is an integer
// constant and E contains no call. It returns E, k, the label taken when
// they are equal, and the label taken when they are not.
func equalityTest(t *ir.IfGoto) (e ir.Expr, k *ir.IntConst, eq, ne string, ok bool) {
	b, ok := t.Cond.(*ir.Binary)
	if !ok || (b.Op != "==" && b.Op != "!=") || hasSideEffects(b.LHS) {
		return nil, nil, "", "", false
	}
	k, ok = b.RHS.(*ir.IntConst)
	if !ok {
		return nil, nil, "", "", false
	}
	if b.Op == "==" {
		return b.LHS, k, t.TrueLabel, t.FalseLabel, true
	}
	return b.LHS, k, t.FalseLabel, t.TrueLabel, true
}
