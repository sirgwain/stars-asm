package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type elideScratchSlotsProcessor struct{}

type scratchAlias struct {
	assign *Assign
	value  Expr
	used   bool
}

// ProcessFunc carries scratch aliases across straight-line block boundaries.
func (p *elideScratchSlotsProcessor) ProcessFunc(result *Result, f *Func) bool {
	blocksByID := semBlockIndexByID(f.Blocks)
	changed := false
	for i := range f.Blocks {
		aliases, aliasOrder, ok := scratchAliasesForWholeBlock(f.Blocks[i])
		if !ok || len(aliases) == 0 {
			continue
		}
		succID, ok := singleScratchPropagationSuccessor(f, i)
		if !ok {
			continue
		}
		succIndex, ok := blocksByID[succID]
		if !ok {
			continue
		}
		rewriter := scratchAliasRewriter(aliases)
		effects, effectChanged := rewriter.rewriteEffects(f.Blocks[succIndex].Effects)
		if !effectChanged {
			continue
		}
		preserved, skipped := appendPendingScratchAssignments(nil, aliases, aliasOrder, true)
		f.Blocks[i].Effects = preserved
		f.Blocks[succIndex].Effects = effects
		changed = changed || skipped
	}
	return changed
}

// ProcessBlock inlines simple unresolved BP-relative compiler scratch slots.
func (p *elideScratchSlotsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	aliases := make(map[string]*scratchAlias)
	aliasOrder := make([]string, 0)
	effects := make([]Effect, 0, len(b.Effects))
	changed := false

	for i := 0; i < len(b.Effects); i++ {
		effect := b.Effects[i]
		assign, ok := effect.(*Assign)
		if ok {
			if key, scratch := scratchSlotKeyExpr(assign.Dst); scratch {
				rewriter := scratchAliasRewriter(aliases)
				value, valueChanged := rewriter.rewriteExpr(assign.Src)
				next := *assign
				next.Src = value
				if _, exists := aliases[key]; exists {
					changed = true
				} else {
					aliasOrder = append(aliasOrder, key)
				}
				aliases[key] = &scratchAlias{assign: &next, value: value}
				changed = changed || valueChanged
				continue
			}
		}

		if i+1 < len(b.Effects) && startsWideWordAssignPair(effect, b.Effects[i+1]) {
			rewriter := scratchAliasRewriter(aliases)
			lo, loChanged := rewriter.rewriteEffect(effect)
			hi, hiChanged := rewriter.rewriteEffect(b.Effects[i+1])
			changed = changed || loChanged || hiChanged
			var skipped bool
			effects, skipped = appendPendingScratchAssignments(effects, aliases, aliasOrder, true)
			changed = changed || skipped
			aliases = make(map[string]*scratchAlias)
			aliasOrder = aliasOrder[:0]
			effects = append(effects, lo, hi)
			i++
			continue
		}

		rewriter := scratchAliasRewriter(aliases)
		next, effectChanged := rewriter.rewriteEffect(effect)
		changed = changed || effectChanged
		if _, ok := next.(*CallEffect); ok && scratchAliasesSafeAcrossCall(aliases) && futureScratchAliasUse(b.Effects[i+1:], aliases) {
			effects = append(effects, next)
			continue
		}
		var skipped bool
		effects, skipped = appendPendingScratchAssignments(effects, aliases, aliasOrder, preserveUnusedScratchAliases(next))
		changed = changed || skipped
		aliases = make(map[string]*scratchAlias)
		aliasOrder = aliasOrder[:0]
		effects = append(effects, next)
	}
	var skipped bool
	effects, skipped = appendPendingScratchAssignments(effects, aliases, aliasOrder, true)
	changed = changed || skipped

	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// scratchAliasesForWholeBlock returns aliases when every effect writes a scratch slot.
func scratchAliasesForWholeBlock(block Block) (map[string]*scratchAlias, []string, bool) {
	aliases := make(map[string]*scratchAlias)
	aliasOrder := make([]string, 0, len(block.Effects))
	for _, effect := range block.Effects {
		assign, ok := effect.(*Assign)
		if !ok {
			return nil, nil, false
		}
		key, scratch := scratchSlotKeyExpr(assign.Dst)
		if !scratch {
			return nil, nil, false
		}
		rewriter := scratchAliasRewriter(aliases)
		value, _ := rewriter.rewriteExpr(assign.Src)
		next := *assign
		next.Src = value
		if _, exists := aliases[key]; !exists {
			aliasOrder = append(aliasOrder, key)
		}
		aliases[key] = &scratchAlias{assign: &next, value: value}
	}
	return aliases, aliasOrder, true
}

// singleScratchPropagationSuccessor returns the unique successor for straight-line propagation.
func singleScratchPropagationSuccessor(f *Func, blockIndex int) (machine.BlockID, bool) {
	if blockIndex < 0 || blockIndex >= len(f.Blocks) {
		return 0, false
	}
	id := f.Blocks[blockIndex].ID
	if f.CFG == nil {
		if blockIndex+1 >= len(f.Blocks) {
			return 0, false
		}
		return f.Blocks[blockIndex+1].ID, true
	}
	succs := f.CFG.Successors(id)
	if len(succs) != 1 {
		return 0, false
	}
	preds := f.CFG.Predecessors(succs[0])
	if len(preds) != 1 || preds[0] != id {
		return 0, false
	}
	return succs[0], true
}

// semBlockIndexByID maps semantic block IDs to their slice index.
func semBlockIndexByID(blocks []Block) map[machine.BlockID]int {
	out := make(map[machine.BlockID]int, len(blocks))
	for i, block := range blocks {
		out[block.ID] = i
	}
	return out
}

// scratchAliasesSafeAcrossCall reports whether pending aliases are pure far-pointer words.
func scratchAliasesSafeAcrossCall(aliases map[string]*scratchAlias) bool {
	for _, alias := range aliases {
		if alias == nil || !exprSafeAcrossCall(alias.value) {
			return false
		}
	}
	return true
}

// exprSafeAcrossCall reports whether expr can be substituted after a call.
func exprSafeAcrossCall(expr Expr) bool {
	switch e := expr.(type) {
	case *Const:
		return true
	case *Part:
		return e.Width == 2 && (e.ByteOff == 0 || e.ByteOff == 2) && typeinfo.IsFarPointer(e.Base.ExprType())
	case *Word:
		return (e.Part == machine.WordLow || e.Part == machine.WordHigh) && typeinfo.IsFarPointer(e.Parent.ExprType())
	case *Binary:
		switch e.Op {
		case OpAdd, OpSub:
			return exprSafeAcrossCall(e.LHS) && exprSafeAcrossCall(e.RHS)
		default:
			return false
		}
	default:
		return false
	}
}

// futureScratchAliasUse reports whether pending scratch aliases are read later.
func futureScratchAliasUse(effects []Effect, aliases map[string]*scratchAlias) bool {
	pending := make(map[string]bool, len(aliases))
	for key := range aliases {
		pending[key] = true
	}
	for _, effect := range effects {
		if effectReadsScratchAlias(effect, pending) {
			return true
		}
		if assign, ok := effect.(*Assign); ok {
			if key, scratch := scratchSlotKeyExpr(assign.Dst); scratch {
				delete(pending, key)
				if len(pending) == 0 {
					return false
				}
			}
		}
	}
	return false
}

// effectReadsScratchAlias reports whether effect reads one of the scratch slots.
func effectReadsScratchAlias(effect Effect, aliases map[string]bool) bool {
	switch e := effect.(type) {
	case *Assign:
		return exprReadsScratchAlias(e.Src, aliases)
	case *CallEffect:
		if e.Call != nil && callReadsScratchAlias(e.Call, aliases) {
			return true
		}
		return exprReadsScratchAlias(e.Result, aliases)
	case *Branch:
		return exprReadsScratchAlias(e.Cond, aliases)
	case *Return:
		return exprReadsScratchAlias(e.Value, aliases)
	default:
		return false
	}
}

// callReadsScratchAlias reports whether call reads one of the scratch slots.
func callReadsScratchAlias(call *Call, aliases map[string]bool) bool {
	if exprReadsScratchAlias(call.Target, aliases) {
		return true
	}
	for _, arg := range call.Args {
		if exprReadsScratchAlias(arg, aliases) {
			return true
		}
	}
	return false
}

// exprReadsScratchAlias reports whether expr contains one of the scratch slots.
func exprReadsScratchAlias(expr Expr, aliases map[string]bool) bool {
	found := false
	walkExpr(expr, func(expr Expr) {
		if found {
			return
		}
		key, scratch := scratchSlotKeyExpr(expr)
		found = scratch && aliases[key]
	})
	return found
}

// startsWideWordAssignPair reports whether two effects assign adjacent low/high words.
func startsWideWordAssignPair(loEffect, hiEffect Effect) bool {
	lo, ok := loEffect.(*Assign)
	if !ok {
		return false
	}
	hi, ok := hiEffect.(*Assign)
	if !ok {
		return false
	}
	loDst, ok := lo.Dst.(*Part)
	if !ok || loDst.ByteOff != 0 || loDst.Width != 2 || exprWidth(loDst.Base) != 4 {
		return false
	}
	hiDst, ok := hi.Dst.(*Part)
	if !ok || hiDst.ByteOff != 2 || hiDst.Width != 2 {
		return false
	}
	return sameLValue(loDst.Base, hiDst.Base)
}

// scratchAliasRewriter creates a semantic rewriter for scratch alias reads.
func scratchAliasRewriter(aliases map[string]*scratchAlias) *semRewriter {
	return &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			key, ok := scratchSlotKeyExpr(expr)
			if !ok {
				return nil, false, false
			}
			alias := aliases[key]
			if alias == nil {
				return expr, false, true
			}
			alias.used = true
			return alias.value, true, true
		},
	}
}

// appendPendingScratchAssignments preserves scratch writes that were never consumed.
func appendPendingScratchAssignments(effects []Effect, aliases map[string]*scratchAlias, order []string, preserveUnused bool) ([]Effect, bool) {
	skipped := false
	for _, key := range order {
		alias := aliases[key]
		if alias == nil {
			continue
		}
		if alias.used || !preserveUnused {
			skipped = true
			continue
		}
		effects = append(effects, alias.assign)
	}
	return effects, skipped
}

// preserveUnusedScratchAliases reports whether unused aliases must remain before effect.
func preserveUnusedScratchAliases(effect Effect) bool {
	switch effect.(type) {
	case *Branch, *Jump, *Return:
		return false
	default:
		return true
	}
}

// scratchSlotKeyExpr returns the canonical key for an unresolved stack scratch expression.
func scratchSlotKeyExpr(expr Expr) (string, bool) {
	mem, ok := expr.(*Memory)
	if !ok || mem.Index != nil || mem.Scale != 0 && mem.Scale != 1 {
		return "", false
	}
	if !scratchStackSegment(mem.Seg) || !scratchFrameBase(mem.Base) {
		return "", false
	}
	return fmt.Sprintf("%d:%d", mem.Disp, mem.Width), true
}

// scratchStackSegment reports whether expr names the stack segment or omits it.
func scratchStackSegment(expr Expr) bool {
	if expr == nil {
		return true
	}
	raw, ok := expr.(*RawValue)
	if !ok {
		return false
	}
	scalar, ok := raw.Value.(*machine.Scalar)
	return ok && scalar.Name == "ss"
}

// scratchFrameBase reports whether expr names the frame base pointer.
func scratchFrameBase(expr Expr) bool {
	raw, ok := expr.(*RawValue)
	if !ok {
		return false
	}
	_, ok = raw.Value.(*machine.FrameBase)
	return ok
}
