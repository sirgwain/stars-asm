package region

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
)

// foldTableGuards absorbs the range check MSC emits before a jump table into
// the switch. A switch on x whose cases run from k to k+N compiles to
//
//	if ((uint16_t)(x - k) > N) goto D;
//	switch ((x - k) * 2) ...
//
// where D is where values without a case go. The check becomes the switch's
// default, so the source switch (x) { ... default: D } replaces both: every
// value outside the table reaches D either way. The table block must be
// reached only from the check. fn is not modified.
func foldTableGuards(fn ir.Func, g *Graph) ir.Func {
	index := map[string]int{}
	for i, block := range fn.Blocks {
		index[block.Label] = i
	}
	blocks := slices.Clone(fn.Blocks)
	removed := map[string]bool{}
	for i := range blocks {
		guard := &blocks[i]
		if len(guard.Stmts) == 0 {
			continue
		}
		test, ok := guard.Stmts[len(guard.Stmts)-1].(*ir.IfGoto)
		if !ok {
			continue
		}
		value, n, inRange, outOfRange, ok := tableRangeCheck(test)
		if !ok {
			continue
		}
		j, ok := index[inRange]
		if !ok || removed[inRange] || len(g.Predecessors(blocks[j].ID)) != 1 || len(blocks[j].Stmts) != 1 {
			continue
		}
		table, ok := blocks[j].Stmts[0].(*ir.TableJump)
		if !ok || int64(len(table.Labels)) != n+1 || !tableIndexes(table, value) {
			continue
		}
		sw := &ir.SwitchGoto{Default: outOfRange}
		var groups []caseGroup
		sw.Index, groups = tableCases(table)
		for _, group := range groups {
			if group.label == outOfRange {
				continue
			}
			for _, v := range group.values {
				sw.Cases = append(sw.Cases, ir.SwitchCase{Value: v, Label: group.label})
			}
		}
		guard.Stmts = append(slices.Clone(guard.Stmts[:len(guard.Stmts)-1]), sw)
		removed[inRange] = true
	}
	out := fn
	out.Blocks = slices.DeleteFunc(blocks, func(b ir.Block) bool { return removed[b.Label] })
	return out
}

// tableRangeCheck matches a jump table's range check, (uint16_t)v > n or
// (uint16_t)v <= n, returning v, n, the label taken when v is in the table's
// range, and the label taken when it is not.
func tableRangeCheck(test *ir.IfGoto) (value ir.Expr, n int64, inRange, outOfRange string, ok bool) {
	b, ok := test.Cond.(*ir.Binary)
	if !ok {
		return nil, 0, "", "", false
	}
	cast, ok := b.LHS.(*ir.Cast)
	if !ok || cast.Type != "uint16_t" {
		return nil, 0, "", "", false
	}
	n, ok = numericConst(b.RHS)
	if !ok {
		return nil, 0, "", "", false
	}
	switch b.Op {
	case ">":
		return cast.Value, n, test.FalseLabel, test.TrueLabel, true
	case "<=":
		return cast.Value, n, test.TrueLabel, test.FalseLabel, true
	}
	return nil, 0, "", "", false
}

// tableIndexes reports whether table's byte-offset index is value * 2, the
// word table entry of the value the range check tested.
func tableIndexes(table *ir.TableJump, value ir.Expr) bool {
	b, ok := table.Index.(*ir.Binary)
	return ok && b.Op == "*" && intConstValue(b.RHS) == 2 && ir.ExprEqual(b.LHS, value)
}
