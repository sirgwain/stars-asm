package sem

import (
	"fmt"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
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
			if p.propagateTrailingScratchAliases(f, blocksByID, i) {
				changed = true
			}
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

// propagateTrailingScratchAliases carries a block's final scratch writes into its single successor.
func (p *elideScratchSlotsProcessor) propagateTrailingScratchAliases(f *Func, blocksByID map[machine.BlockID]int, blockIndex int) bool {
	prefix, aliases, aliasOrder, ok := trailingScratchAliases(f.Blocks[blockIndex])
	if !ok || len(aliases) == 0 {
		return false
	}
	succID, ok := singleScratchPropagationSuccessor(f, blockIndex)
	if !ok {
		return false
	}
	succIndex, ok := blocksByID[succID]
	if !ok {
		return false
	}

	rewriter := scratchAliasRewriter(aliases)
	effects, effectChanged := rewriter.rewriteEffects(f.Blocks[succIndex].Effects)
	if !effectChanged {
		return false
	}
	preserved, skipped := appendPendingScratchAssignments(nil, aliases, aliasOrder, true)
	nextEffects := append([]Effect(nil), f.Blocks[blockIndex].Effects[:prefix]...)
	nextEffects = append(nextEffects, preserved...)
	f.Blocks[blockIndex].Effects = nextEffects
	f.Blocks[succIndex].Effects = effects
	return effectChanged || skipped
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
		if call, ok := next.(*CallEffect); ok && scratchAliasesPreservedAcrossCall(call, aliases) && futureScratchAliasUse(b.Effects[i+1:], aliases) {
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

// trailingScratchAliases returns the final contiguous scratch writes in a block.
func trailingScratchAliases(block Block) (int, map[string]*scratchAlias, []string, bool) {
	if len(block.Effects) == 0 {
		return 0, nil, nil, false
	}
	start := len(block.Effects)
	for start > 0 {
		assign, ok := block.Effects[start-1].(*Assign)
		if !ok {
			break
		}
		if _, scratch := scratchSlotKeyExpr(assign.Dst); !scratch {
			break
		}
		start--
	}
	if start == len(block.Effects) {
		return 0, nil, nil, false
	}
	aliases := make(map[string]*scratchAlias)
	aliasOrder := make([]string, 0, len(block.Effects)-start)
	for _, effect := range block.Effects[start:] {
		assign := effect.(*Assign)
		key, _ := scratchSlotKeyExpr(assign.Dst)
		rewriter := scratchAliasRewriter(aliases)
		value, _ := rewriter.rewriteExpr(assign.Src)
		next := *assign
		next.Src = value
		if _, exists := aliases[key]; !exists {
			aliasOrder = append(aliasOrder, key)
		}
		aliases[key] = &scratchAlias{assign: &next, value: value}
	}
	return start, aliases, aliasOrder, true
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

// scratchAliasesPreservedAcrossCall reports whether pending aliases can be
// substituted after call without changing memory-observation order.
func scratchAliasesPreservedAcrossCall(call *CallEffect, aliases map[string]*scratchAlias) bool {
	if scratchAliasesSafeAcrossCall(aliases) {
		return true
	}
	return callPreservesScratchAliases(call)
}

// callPreservesScratchAliases reports whether call cannot mutate values used
// by pending scratch aliases.
func callPreservesScratchAliases(call *CallEffect) bool {
	return call != nil && call.Call != nil && call.Call.Function != nil && strings.EqualFold(call.Call.Function.Name, "sqrt")
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
	case *Register:
		return true
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
			if value, ok := scratchAliasWideValue(expr, aliases); ok {
				return value, true, true
			}
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

// scratchAliasWideValue reconstructs a dword value from adjacent scratch word
// aliases when an explicit dword read proves they form one value.
func scratchAliasWideValue(expr Expr, aliases map[string]*scratchAlias) (Expr, bool) {
	disp, width, ok := scratchSlotRangeExpr(expr)
	if !ok || width != 4 {
		return nil, false
	}
	lo := aliases[fmt.Sprintf("%d:%d", disp, 2)]
	hi := aliases[fmt.Sprintf("%d:%d", disp+2, 2)]
	if lo == nil || hi == nil {
		return nil, false
	}
	if value, ok := scratchAliasWideConst(lo, hi); ok {
		return value, true
	}
	parent, ok := wordPartParent(hi.value, machine.WordSignHigh)
	if ok && sameExpr(parent, lo.value) {
		lo.used = true
		hi.used = true
		return &SignExtend{Parent: lo.value, FromBits: 16, ToBits: 32, TypeInfo: typeinfo.I32}, true
	}
	if value, ok := collapseWideExprPair(hi.value, lo.value, typeinfo.I32); ok {
		lo.used = true
		hi.used = true
		return value, true
	}
	lo.used = true
	hi.used = true
	return &Words{Words: []Expr{hi.value, lo.value}}, true
}

// scratchAliasWideConst synthesizes a dword constant from adjacent scratch word
// aliases.
func scratchAliasWideConst(lo, hi *scratchAlias) (Expr, bool) {
	loConst, loOK := lo.value.(*Const)
	hiConst, hiOK := hi.value.(*Const)
	if !loOK || !hiOK {
		return nil, false
	}
	lo.used = true
	hi.used = true
	return &Const{TypeInfo: typeinfo.I32, U64: ((hiConst.U64 & 0xffff) << 16) | (loConst.U64 & 0xffff)}, true
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

// scratchSlotKeyExpr returns the canonical key for a raw or symbolic stack scratch expression.
func scratchSlotKeyExpr(expr Expr) (string, bool) {
	disp, width, ok := scratchSlotRangeExpr(expr)
	if !ok {
		return "", false
	}
	return fmt.Sprintf("%d:%d", disp, width), true
}

// scratchSlotRangeExpr returns the BP displacement and width named by a raw
// memory expression or a symbolic scratch path.
func scratchSlotRangeExpr(expr Expr) (int, int, bool) {
	switch e := expr.(type) {
	case *Memory:
		if e.Index != nil || e.Scale != 0 && e.Scale != 1 {
			return 0, 0, false
		}
		if !scratchStackSegment(e.Seg) || !scratchFrameBase(e.Base) {
			return 0, 0, false
		}
		return e.Disp, e.Width, e.Width > 0
	case *SymbolRef:
		return scratchSymbolPathRange(e.Path)
	case *Part:
		disp, _, ok := scratchSlotRangeExpr(e.Base)
		if !ok || e.Width <= 0 {
			return 0, 0, false
		}
		return disp + e.ByteOff, e.Width, true
	default:
		return 0, 0, false
	}
}

// scratchSymbolPathRange returns the BP displacement and access width for a
// path rooted at synthetic scratch storage.
func scratchSymbolPathRange(path symresolve.SymbolPath) (int, int, bool) {
	switch p := path.(type) {
	case *symresolve.SymbolScratch:
		return p.BPOffset, p.StorageSize, p.StorageSize > 0
	case *symresolve.SymbolOffset:
		disp, _, ok := scratchSymbolPathRange(p.Base)
		if !ok || p.Type() == nil || p.Type().Bytes() <= 0 {
			return 0, 0, false
		}
		return disp + p.Offset, p.Type().Bytes(), true
	case *symresolve.SymbolField:
		disp, _, ok := scratchSymbolPathRange(p.Base)
		if !ok || p.Field == nil || p.Field.Type == nil {
			return 0, 0, false
		}
		return disp + p.Field.Offset, p.Field.Type.Bytes(), true
	case *symresolve.SymbolBitfield:
		disp, _, ok := scratchSymbolPathRange(p.Base)
		if !ok || p.Field == nil || p.Field.Bitfield == nil {
			return 0, 0, false
		}
		return disp + p.Field.Offset, p.Field.Bitfield.StorageSize, true
	default:
		return 0, 0, false
	}
}

// scratchStackSegment reports whether expr names the stack segment or omits it.
func scratchStackSegment(expr Expr) bool {
	if expr == nil {
		return true
	}
	seg, ok := expr.(*Register)

	return ok && seg.Val == asm.RegSS
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
