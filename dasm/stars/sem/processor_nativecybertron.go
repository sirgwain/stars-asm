package sem

import (
	"fmt"
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// nativeCybertronProcessor repairs Cybertron behavior that depends on the
// original Win16 stack layout.
type nativeCybertronProcessor struct {
	ctx *FuncContext
}

// ProcessFunc applies the native repairs for the affected Cybertron functions.
func (p *nativeCybertronProcessor) ProcessFunc(_ *Result, f *Func) bool {
	if p.ctx.fs.Name != "DoCyberAiTurn" {
		return false
	}
	// In Win16, index -1 overwrote a byte of a dead lppl pointer. Native
	// locals differ, so skip absent designs while preserving index choices.
	counts := map[string]int{}
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			if index := cyberRecycleIndex(effect); index != nil {
				counts[index.Name]++
			}
		}
	}
	if counts["iLatestDestroyer"] != 2 || counts["iLatestCargo"] != 1 {
		panic(fmt.Sprintf("native-cybertron: expected two destroyer stores and one cargo store, got %v", counts))
	}
	for bi := 0; bi < len(f.Blocks); bi++ {
		block := f.Blocks[bi]
		for ei, effect := range block.Effects {
			index := cyberRecycleIndex(effect)
			if index == nil {
				continue
			}
			successors := f.CFG.Successors(block.ID)
			if len(successors) != 1 {
				panic(fmt.Sprintf("native-cybertron: expected one successor for %s", block.ID))
			}
			join := firstSyntheticBlockID(f)
			if err := f.CFG.InsertSyntheticBlockOnEdge(block.ID, successors[0], join); err != nil {
				panic(err)
			}
			store := firstSyntheticBlockID(f)
			if err := f.CFG.InsertSyntheticBlockOnEdge(block.ID, join, store); err != nil {
				panic(err)
			}
			if err := f.CFG.RewriteOutgoingEdges(map[machine.BlockID][]machine.BlockID{block.ID: {store, join}}); err != nil {
				panic(err)
			}
			meta := effect.EffectMeta()
			f.Blocks[bi].Effects = append(slices.Clone(block.Effects[:ei]), &Branch{
				MetaInfo: meta,
				Cond: &Compare{Op: CompareNE, LHS: index,
					RHS: &Const{TypeInfo: typeinfo.I16, U64: 0xffff}},
				TrueBlock: store, FalseBlock: join,
			})
			f.Blocks = slices.Insert(f.Blocks, bi+1,
				Block{ID: store, Effects: []Effect{effect, &Jump{MetaInfo: meta, To: join}}},
				Block{ID: join, Effects: slices.Clone(block.Effects[ei+1:])})
			bi++ // Skip the guarded store; scan the remaining effects in the join next.
			break
		}
	}
	return true
}

// cyberRecycleIndex matches the zero stores through latest-design locals that
// need the absent-design guard in DoCyberAiTurn.
func cyberRecycleIndex(effect Effect) *Local {
	assign, ok := effect.(*Assign)
	if !ok {
		return nil
	}
	zero, ok := assign.Src.(*Const)
	if !ok || zero.U64 != 0 {
		return nil
	}
	array, ok := assign.Dst.(*ArrayIndex)
	if !ok {
		return nil
	}
	base, ok := array.Base.(*Local)
	if !ok || base.Name != "rgRecycleShdef" {
		return nil
	}
	index, ok := array.Index.(*Local)
	if !ok || (index.Name != "iLatestDestroyer" && index.Name != "iLatestCargo") {
		return nil
	}
	return index
}
