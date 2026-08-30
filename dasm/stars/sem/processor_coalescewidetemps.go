package sem

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type coalesceWideTempsProcessor struct {
	ctx *FuncContext
}

type wideTempPart int

const (
	wideTempLow wideTempPart = iota
	wideTempHigh
)

type wideTempDef struct {
	block machine.BlockID
	src   Expr
	index int
}

type wideTempInfo struct {
	temp *Temp
	defs []wideTempDef
}

type wideTempCandidate struct {
	wide *Temp
	low  []wideTempInfo
	high []wideTempInfo
	defs []wideTempDef
}

type wideTempAssignmentKey struct {
	block machine.BlockID
	index int
}

// ProcessFunc coalesces low/high temp pairs into one wide temp.
func (p *coalesceWideTempsProcessor) ProcessFunc(result *Result, f *Func) bool {
	changed := false
	for {
		candidate, ok := p.findCandidate(f)
		if !ok {
			break
		}
		p.applyCandidate(f, candidate)
		changed = true
	}
	return changed
}

// findCandidate finds one group of low/high temp definitions for the same wide values.
func (p *coalesceWideTempsProcessor) findCandidate(f *Func) (wideTempCandidate, bool) {
	infos := collectWideTempInfos(f)
	for _, low := range infos {
		if len(low.defs) == 0 {
			continue
		}
		for _, high := range infos {
			if low.temp.Name == high.temp.Name {
				continue
			}
			defs, typ, ok := reconstructWideTempDefs(p.ctx, low, high, nil)
			if !ok {
				continue
			}

			compatibleLows := []wideTempInfo{low}
			for _, otherLow := range infos {
				if otherLow.temp.Name == low.temp.Name || otherLow.temp.Name == high.temp.Name {
					continue
				}
				otherDefs, _, ok := reconstructWideTempDefs(p.ctx, otherLow, high, typ)
				if ok && sameWideTempDefs(defs, otherDefs) {
					compatibleLows = append(compatibleLows, otherLow)
				}
			}

			compatibleHighs := []wideTempInfo{high}
			for _, otherHigh := range infos {
				if otherHigh.temp.Name == low.temp.Name || otherHigh.temp.Name == high.temp.Name {
					continue
				}
				otherDefs, _, ok := reconstructWideTempDefs(p.ctx, low, otherHigh, typ)
				if ok && sameWideTempDefs(defs, otherDefs) {
					compatibleHighs = append(compatibleHighs, otherHigh)
				}
			}

			return wideTempCandidate{
				wide: &Temp{Name: low.temp.Name + "_wide", TypeInfo: typ},
				low:  compatibleLows,
				high: compatibleHighs,
				defs: defs,
			}, true
		}
	}
	return wideTempCandidate{}, false
}

// collectWideTempInfos returns semantic temp assignment profiles.
func collectWideTempInfos(f *Func) []wideTempInfo {
	defsByTemp := make(map[string]*wideTempInfo)
	for bi, block := range f.Blocks {
		for ei, effect := range block.Effects {
			assign, ok := effect.(*Assign)
			if !ok {
				continue
			}
			temp, ok := assign.Dst.(*Temp)
			if !ok {
				continue
			}
			info := defsByTemp[temp.Name]
			if info == nil {
				next := wideTempInfo{temp: temp}
				info = &next
				defsByTemp[temp.Name] = info
			}
			info.defs = append(info.defs, wideTempDef{block: f.Blocks[bi].ID, src: assign.Src, index: ei})
		}
	}

	names := make([]string, 0, len(defsByTemp))
	for name := range defsByTemp {
		names = append(names, name)
	}
	slices.Sort(names)

	infos := make([]wideTempInfo, 0, len(names))
	for _, name := range names {
		info := defsByTemp[name]
		if info == nil || len(info.defs) == 0 {
			continue
		}
		infos = append(infos, *info)
	}
	return infos
}

// reconstructWideTempDefs reconstructs wide source definitions for low/high temps.
func reconstructWideTempDefs(ctx *FuncContext, low wideTempInfo, high wideTempInfo, expected typeinfo.Type) ([]wideTempDef, typeinfo.Type, bool) {
	if len(low.defs) == 0 || len(low.defs) != len(high.defs) {
		return nil, nil, false
	}
	defs := make([]wideTempDef, len(low.defs))
	var typ typeinfo.Type
	if expected != nil && expected.Bytes() == 4 {
		typ = expected
	}
	for i := range low.defs {
		if low.defs[i].block != high.defs[i].block {
			return nil, nil, false
		}
		src, ok := reconstructWideTempSrc(ctx, high.defs[i].src, low.defs[i].src, typ)
		if !ok {
			return nil, nil, false
		}
		if src.ExprType() != nil && src.ExprType().Bytes() == 4 {
			typ = preferredWideTempType(typ, src.ExprType())
		}
		defs[i] = wideTempDef{block: low.defs[i].block, src: src, index: min(low.defs[i].index, high.defs[i].index)}
	}
	if typ == nil {
		return nil, nil, false
	}
	return defs, typ, true
}

// preferredWideTempType chooses the richer type for a reconstructed wide temp.
func preferredWideTempType(current typeinfo.Type, next typeinfo.Type) typeinfo.Type {
	if current == nil {
		return next
	}
	if typeinfo.IsFarPointer(next) && !typeinfo.IsFarPointer(current) {
		return next
	}
	return current
}

// reconstructWideTempSrc rebuilds one wide source from high and low words.
func reconstructWideTempSrc(ctx *FuncContext, high Expr, low Expr, expected typeinfo.Type) (Expr, bool) {
	if src, ok := collapseWideExprPair(high, low, expected); ok {
		return src, true
	}
	if highParent, highOK := highWordParent(high); highOK {
		if lowParent, lowOK := lowWordParent(low); lowOK && sameExpr(highParent, lowParent) && exprWidth(highParent) == 4 {
			return highParent, true
		}
	}
	if expected != nil && typeinfo.IsFarPointer(expected) {
		if src, ok := collapseFarPointerWords(ctx, &Temp{TypeInfo: expected}, high, low); ok {
			return src, true
		}
	}
	return nil, false
}

// sameWideTempDefs reports whether two reconstructed definition lists match.
func sameWideTempDefs(a []wideTempDef, b []wideTempDef) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i].block != b[i].block || !sameExpr(a[i].src, b[i].src) {
			return false
		}
	}
	return true
}

// applyCandidate rewrites one wide temp group through the function.
func (p *coalesceWideTempsProcessor) applyCandidate(f *Func, candidate wideTempCandidate) {
	replacements := candidate.replacements()
	defsByBlock := make(map[machine.BlockID]wideTempDef, len(candidate.defs))
	for _, def := range candidate.defs {
		defsByBlock[def.block] = def
	}
	removedAssignments := candidate.removedAssignments()

	for bi := range f.Blocks {
		block := &f.Blocks[bi]
		var out []Effect
		inserted := false
		for ei, effect := range block.Effects {
			if def, ok := defsByBlock[block.ID]; ok && ei == def.index && !inserted {
				out = append(out, &Assign{
					MetaInfo: machine.Meta{BlockID: block.ID},
					Dst:      candidate.wide,
					Src:      def.src,
				})
				inserted = true
			}
			if removedAssignments[wideTempAssignmentKey{block: block.ID, index: ei}] {
				continue
			}
			rewritten, _ := rewriteWideTempEffect(effect, replacements, p.ctx)
			out = append(out, rewritten)
		}
		block.Effects = out
	}
}

// replacements returns old half-temp read replacements for the candidate.
func (candidate wideTempCandidate) replacements() map[string]Expr {
	replacements := make(map[string]Expr, len(candidate.low)+len(candidate.high))
	for _, info := range candidate.low {
		replacements[info.temp.Name] = wideTempPartExpr(candidate.wide, wideTempLow)
	}
	for _, info := range candidate.high {
		replacements[info.temp.Name] = wideTempPartExpr(candidate.wide, wideTempHigh)
	}
	return replacements
}

// removedAssignments returns the exact half-temp assignment indexes to remove.
func (candidate wideTempCandidate) removedAssignments() map[wideTempAssignmentKey]bool {
	out := make(map[wideTempAssignmentKey]bool)
	for _, info := range candidate.low {
		for _, def := range info.defs {
			out[wideTempAssignmentKey{block: def.block, index: def.index}] = true
		}
	}
	for _, info := range candidate.high {
		for _, def := range info.defs {
			out[wideTempAssignmentKey{block: def.block, index: def.index}] = true
		}
	}
	return out
}

// wideTempPartExpr returns the selected low/high word of a wide temp.
func wideTempPartExpr(temp *Temp, part wideTempPart) Expr {
	if typeinfo.IsFarPointer(temp.ExprType()) {
		fpPart := machine.FarPointerOffset
		if part == wideTempHigh {
			fpPart = machine.FarPointerSegment
		}
		return &FarPointer{Parent: temp, Part: fpPart, TypeInfo: typeinfo.U16}
	}
	wordPart := machine.WordLow
	if part == wideTempHigh {
		wordPart = machine.WordHigh
	}
	return &Word{Parent: temp, Part: wordPart}
}

// rewriteWideTempEffect replaces old half-temp reads in one effect.
func rewriteWideTempEffect(effect Effect, replacements map[string]Expr, ctx *FuncContext) (Effect, bool) {
	rewriter := &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, changed := w.rewriteExprChildren(expr)
			if temp, ok := next.(*Temp); ok {
				if replacement := replacements[temp.Name]; replacement != nil {
					return replacement, true, true
				}
			}
			if collapsed, ok := collapseWideTempReconstructedExpr(ctx, next); ok {
				return collapsed, true, true
			}
			return next, changed, true
		},
	}
	return rewriter.rewriteEffect(effect)
}

// collapseWideTempReconstructedExpr collapses projections of one wide temp.
func collapseWideTempReconstructedExpr(ctx *FuncContext, expr Expr) (Expr, bool) {
	if words, ok := expr.(*Words); ok {
		return collapseWideWords(words)
	}
	ptr, ok := expr.(*FarPointer)
	if !ok || ptr.Part != machine.FarPointerWhole {
		return nil, false
	}
	segment, segOK := farPointerPartParent(ptr.Segment, machine.FarPointerSegment)
	offset, offOK := farPointerPartParent(ptr.Offset, machine.FarPointerOffset)
	if !segOK || !offOK || !sameExpr(segment, offset) || !typeinfo.IsFarPointer(segment.ExprType()) {
		segment, segOK = highWordParent(ptr.Segment)
		offset, offOK = lowWordParent(ptr.Offset)
		if !segOK || !offOK || !sameExpr(segment, offset) || exprWidth(segment) != 4 || !typeinfo.IsFarPointer(ptr.ExprType()) {
			return nil, false
		}
	}
	if ctx != nil {
		if resolved, ok := ctx.resolveSemanticFarPointer(&FarPointer{
			Part:     machine.FarPointerWhole,
			Offset:   ptr.Offset,
			Segment:  ptr.Segment,
			TypeInfo: segment.ExprType(),
		}); ok {
			return resolved, true
		}
	}
	return segment, true
}
