package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// recoverScratchWideDefinitions reconstructs adjacent word stores once, when
// their shared object or matching source projections prove a wide value.
func recoverScratchWideDefinitions(f *Func, a *scratchAnalysis) bool {
	changed := false
	for bi, b := range f.Blocks {
		effects := make([]Effect, 0, len(b.Effects))
		for i := 0; i < len(b.Effects); i++ {
			lo, lok := b.Effects[i].(*Assign)
			if !lok || i+1 == len(b.Effects) {
				effects = append(effects, b.Effects[i])
				continue
			}
			hi, hok := b.Effects[i+1].(*Assign)
			off, lw, lok := scratchSlotRangeExpr(lo.Dst)
			if !hok || !lok || lw != 2 {
				effects = append(effects, lo)
				continue
			}
			hoff, hw, hok := scratchSlotRangeExpr(hi.Dst)
			if !hok || hw != 2 || hoff != off+2 {
				effects = append(effects, lo)
				continue
			}
			o := a.object(ScratchRange{off, 2})
			if o.disposition == scratchLocal {
				effects = append(effects, lo)
				continue
			}
			value, matched := collapseWideExprPair(hi.Src, lo.Src, nil)
			if parent, ok := wordPartParent(hi.Src, machine.WordSignHigh); ok && sameExpr(parent, lo.Src) {
				value = &SignExtend{Parent: lo.Src, FromBits: 16, ToBits: 32, TypeInfo: typeinfo.I32}
				matched = true
			}
			_, lowWord := lo.Src.(*Word)
			_, lowPart := lo.Src.(*Part)
			_, lowPointer := lo.Src.(*FarPointer)
			if !o.region.Contains(ScratchRange{off, 4}) && (!matched || !(lowWord || lowPart || lowPointer)) {
				effects = append(effects, lo)
				continue
			}
			// Merge values are captured on incoming edges, so matching word
			// projections can be combined here without moving their evaluation.
			// Calls and opaque values still require separate evaluations, and
			// the high store must not depend on bytes written by the low store.
			safe := true
			for lane, src := range []Expr{lo.Src, hi.Src} {
				walkExpr(src, func(e Expr) {
					switch e.(type) {
					case *Call, *RawValue, *RawMemory:
						safe = false
					}
					if d, w, ok := scratchSlotRangeExpr(e); lane == 1 && ok && (ScratchRange{d, w}).Overlaps(ScratchRange{off, 2}) {
						safe = false
					}
				})
			}
			if !safe {
				effects = append(effects, lo)
				continue
			}
			if !matched {
				value = &Words{Words: []Expr{hi.Src, lo.Src}}
			}
			var dst LValue
			switch original := lo.Dst.(type) {
			case *SymbolRef:
				root, ok := original.Path.(*symresolve.SymbolScratch)
				if !ok {
					effects = append(effects, lo)
					continue
				}
				next := *root
				next.StorageSize = 4
				next.TypeInfo = value.ExprType()
				dst = &SymbolRef{Path: &next}
			case *Memory:
				next := *original
				next.Width = 4
				next.TypeInfo = value.ExprType()
				dst = &next
			default:
				effects = append(effects, lo)
				continue
			}
			effects = append(effects, &Assign{MetaInfo: hi.MetaInfo, Dst: dst, Src: value})
			i++
			changed = true
		}
		f.Blocks[bi].Effects = effects
	}
	return changed
}
