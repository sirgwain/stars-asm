package region

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
)

// threadJumps retargets jumps that land on a trampoline block, one that only
// passes control on: it holds nothing but a goto, or nothing at all and falls
// through to the next block. Each jump goes straight to where the chain of
// trampolines ends. A trampoline is then removed unless the block before it
// falls through into it. The entry block always stays. fn is not modified;
// changed blocks get new statement slices.
func threadJumps(fn ir.Func) ir.Func {
	next := map[string]string{}
	for i, block := range fn.Blocks[1:] {
		switch {
		case len(block.Stmts) == 0 && i+2 < len(fn.Blocks):
			next[block.Label] = fn.Blocks[i+2].Label
		case len(block.Stmts) == 1:
			if g, ok := block.Stmts[0].(*ir.Goto); ok && g.Label != block.Label {
				next[block.Label] = g.Label
			}
		}
	}
	if len(next) == 0 {
		return fn
	}

	resolve := func(label string) string {
		seen := map[string]bool{}
		for !seen[label] {
			seen[label] = true
			to, ok := next[label]
			if !ok {
				return label
			}
			label = to
		}
		// A cycle of trampolines is an empty infinite loop; leave it alone.
		return label
	}

	blocks := slices.Clone(fn.Blocks)
	for i := range blocks {
		b := &blocks[i]
		if len(b.Stmts) == 0 {
			continue
		}
		if t := retargetJump(b.Stmts[len(b.Stmts)-1], resolve); t != nil {
			b.Stmts = append(slices.Clone(b.Stmts[:len(b.Stmts)-1]), t)
		}
	}

	refs := jumpTargets(blocks)
	out := fn
	out.Blocks = nil
	for _, b := range blocks {
		if _, trampoline := next[b.Label]; trampoline && !refs[b.Label] && !fallsThrough(out.Blocks[len(out.Blocks)-1]) {
			continue
		}
		out.Blocks = append(out.Blocks, b)
	}
	return out
}

// retargetJump returns a copy of the jump stmt with each label passed through
// resolve, or nil when stmt is not a jump or no label changes.
func retargetJump(stmt ir.Stmt, resolve func(string) string) ir.Stmt {
	switch s := stmt.(type) {
	case *ir.Goto:
		if to := resolve(s.Label); to != s.Label {
			return &ir.Goto{Label: to}
		}
	case *ir.IfGoto:
		t, f := resolve(s.TrueLabel), resolve(s.FalseLabel)
		if t != s.TrueLabel || f != s.FalseLabel {
			return &ir.IfGoto{Cond: s.Cond, TrueLabel: t, FalseLabel: f}
		}
	case *ir.TableJump:
		labels := make([]string, len(s.Labels))
		changed := false
		for i, label := range s.Labels {
			labels[i] = resolve(label)
			changed = changed || labels[i] != label
		}
		if changed {
			return &ir.TableJump{Index: s.Index, Labels: labels}
		}
	}
	return nil
}

// fallsThrough reports whether control can run off the end of block into the
// block after it.
func fallsThrough(block ir.Block) bool {
	if len(block.Stmts) == 0 {
		return true
	}
	switch block.Stmts[len(block.Stmts)-1].(type) {
	case *ir.Goto, *ir.IfGoto, *ir.TableJump, *ir.SwitchGoto, *ir.Return:
		return false
	}
	return true
}

// jumpTargets returns the labels named by any block's final jump.
func jumpTargets(blocks []ir.Block) map[string]bool {
	refs := map[string]bool{}
	for _, b := range blocks {
		if len(b.Stmts) == 0 {
			continue
		}
		switch s := b.Stmts[len(b.Stmts)-1].(type) {
		case *ir.Goto:
			refs[s.Label] = true
		case *ir.IfGoto:
			refs[s.TrueLabel] = true
			refs[s.FalseLabel] = true
		case *ir.TableJump:
			for _, label := range s.Labels {
				refs[label] = true
			}
		}
	}
	return refs
}
