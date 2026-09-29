package region

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
)

// mergeConditions folds short-circuit condition chains into single branches.
// A block B that holds only an IfGoto and whose only predecessor is A is
// merged into A's IfGoto when the two branches share a target:
//
//	A: if (a) T else B;  B: if (b) T else F  →  A: if (a || b) T else F
//	A: if (a) B else F;  B: if (b) T else F  →  A: if (a && b) T else F
//	A: if (a) T else B;  B: if (b) X else T  →  A: if (!a && b) X else T
//	A: if (a) B else F;  B: if (b) F else Y  →  A: if (!a || b) F else Y
//
// B's condition only runs on the path where A branched to it, which is the
// short-circuit order of && and ||, so side effects keep their order. B is
// removed. fn is not modified; merged blocks get new statement slices.
func mergeConditions(fn ir.Func, g *Graph) ir.Func {
	preds := map[string]map[string]bool{}
	index := map[string]int{}
	for i, block := range fn.Blocks {
		index[block.Label] = i
		preds[block.Label] = map[string]bool{}
		for _, p := range g.Predecessors(block.ID) {
			preds[block.Label][g.blocks[p].Label] = true
		}
	}

	blocks := slices.Clone(fn.Blocks)
	removed := map[string]bool{}
	for changed := true; changed; {
		changed = false
		for i := range blocks {
			a := &blocks[i]
			if removed[a.Label] {
				continue
			}
			for mergeNextCondition(a, blocks, index, preds, removed) {
				changed = true
			}
		}
	}

	out := fn
	out.Blocks = slices.DeleteFunc(blocks, func(b ir.Block) bool { return removed[b.Label] })
	return out
}

// mergeNextCondition merges one condition-only successor into a's final
// IfGoto, updating preds and removed. It reports whether a merge happened.
func mergeNextCondition(a *ir.Block, blocks []ir.Block, index map[string]int, preds map[string]map[string]bool, removed map[string]bool) bool {
	if len(a.Stmts) == 0 {
		return false
	}
	ifA, ok := a.Stmts[len(a.Stmts)-1].(*ir.IfGoto)
	if !ok || ifA.TrueLabel == ifA.FalseLabel {
		return false
	}

	for _, label := range []string{ifA.FalseLabel, ifA.TrueLabel} {
		// The entry block stays even when a loop jumps back to it.
		if label == a.Label || removed[label] || index[label] == 0 {
			continue
		}
		b := &blocks[index[label]]
		if len(b.Stmts) != 1 || len(preds[label]) != 1 || !preds[label][a.Label] {
			continue
		}
		ifB, ok := b.Stmts[0].(*ir.IfGoto)
		if !ok {
			continue
		}
		merged := mergeIfGotos(ifA, ifB, label)
		if merged == nil {
			continue
		}

		a.Stmts = append(slices.Clone(a.Stmts[:len(a.Stmts)-1]), merged)
		removed[label] = true
		delete(preds, label)
		for _, t := range []string{ifB.TrueLabel, ifB.FalseLabel} {
			if p := preds[t]; p != nil {
				delete(p, label)
				p[a.Label] = true
			}
		}
		return true
	}
	return false
}

// mergeIfGotos combines A's branch with the branch of the block bLabel that A
// jumps to, or returns nil when the two branches share no target.
func mergeIfGotos(ifA, ifB *ir.IfGoto, bLabel string) *ir.IfGoto {
	a, b := ifA.Cond, ifB.Cond
	switch {
	case ifA.FalseLabel == bLabel && ifA.TrueLabel == ifB.TrueLabel:
		return &ir.IfGoto{Cond: &ir.Binary{Op: "||", LHS: a, RHS: b}, TrueLabel: ifB.TrueLabel, FalseLabel: ifB.FalseLabel}
	case ifA.TrueLabel == bLabel && ifA.FalseLabel == ifB.FalseLabel:
		return &ir.IfGoto{Cond: &ir.Binary{Op: "&&", LHS: a, RHS: b}, TrueLabel: ifB.TrueLabel, FalseLabel: ifB.FalseLabel}
	case ifA.FalseLabel == bLabel && ifA.TrueLabel == ifB.FalseLabel:
		return &ir.IfGoto{Cond: &ir.Binary{Op: "&&", LHS: Negate(a), RHS: b}, TrueLabel: ifB.TrueLabel, FalseLabel: ifB.FalseLabel}
	case ifA.TrueLabel == bLabel && ifA.FalseLabel == ifB.TrueLabel:
		return &ir.IfGoto{Cond: &ir.Binary{Op: "||", LHS: Negate(a), RHS: b}, TrueLabel: ifB.TrueLabel, FalseLabel: ifB.FalseLabel}
	}
	return nil
}
