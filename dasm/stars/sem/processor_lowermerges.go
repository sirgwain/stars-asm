package sem

import (
	"fmt"
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

type lowerMergesProcessor struct {
	nextBlockID machine.BlockID
	nextByJoin  map[machine.BlockID]int
}

type mergeEdge struct {
	from machine.BlockID
	to   machine.BlockID
}

// ProcessFunc lowers every semantic merge expression to edge-assigned temps.
func (p *lowerMergesProcessor) ProcessFunc(result *Result, f *Func) bool {
	p.nextBlockID = firstSyntheticMergeBlockID(f)

	changed := false
	for {
		passChanged := p.processFuncOnce(f)
		changed = changed || passChanged
		if !passChanged {
			break
		}
	}

	assertNoMergeExprs(f)
	return changed
}

// processFuncOnce lowers the merge expressions present at the start of one pass.
func (p *lowerMergesProcessor) processFuncOnce(f *Func) bool {
	pending := make(map[mergeEdge][]Effect)
	blockIndexes := blockIndexByID(f.Blocks)
	changed := false

	for i := range f.Blocks {
		rewriter := &semRewriter{
			expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
				merge, ok := expr.(*Merge)
				if !ok {
					return nil, false, false
				}

				arms, _ := w.rewriteMergeArms(merge.Arms)
				join := merge.Join
				temp := p.newMergeTemp(merge)
				for _, arm := range arms {
					pending[mergeEdge{from: arm.Block, to: join}] = append(
						pending[mergeEdge{from: arm.Block, to: join}],
						&Assign{
							MetaInfo: machine.Meta{BlockID: arm.Block},
							Dst:      temp,
							Src:      arm.Value,
						},
					)
				}
				return temp, true, true
			},
		}
		effects, effectsChanged := rewriter.rewriteEffects(f.Blocks[i].Effects)
		if !effectsChanged {
			continue
		}
		f.Blocks[i].Effects = effects
		changed = true
	}

	for _, edge := range sortedMergeEdges(pending) {
		assigns := pending[edge]
		if len(assigns) == 0 {
			continue
		}
		if p.insertEdgeAssignments(f, blockIndexes, edge.from, edge.to, assigns) {
			changed = true
			blockIndexes = blockIndexByID(f.Blocks)
		}
	}

	return changed
}

// sortedMergeEdges returns pending edge keys in deterministic order.
func sortedMergeEdges(pending map[mergeEdge][]Effect) []mergeEdge {
	edges := make([]mergeEdge, 0, len(pending))
	for edge := range pending {
		edges = append(edges, edge)
	}
	slices.SortFunc(edges, func(a mergeEdge, b mergeEdge) int {
		if a.to != b.to {
			return int(a.to) - int(b.to)
		}
		return int(a.from) - int(b.from)
	})
	return edges
}

// newMergeTemp creates a typed temporary for one merge expression.
func (p *lowerMergesProcessor) newMergeTemp(merge *Merge) *Temp {
	if p.nextByJoin == nil {
		p.nextByJoin = make(map[machine.BlockID]int)
	}

	p.nextByJoin[merge.Join]++
	n := p.nextByJoin[merge.Join]

	return &Temp{
		Name: fmt.Sprintf(
			"t_merge_%04x_%04x",
			uint16(merge.Join),
			n,
		),
		TypeInfo: merge.ExprType(),
	}
}

// insertEdgeAssignments inserts assignments on the selected CFG edge.
func (p *lowerMergesProcessor) insertEdgeAssignments(f *Func, blockIndexByID map[machine.BlockID]int, from machine.BlockID, to machine.BlockID, assigns []Effect) bool {
	if f.CFG == nil {
		panic(fmt.Sprintf("lower merge missing CFG for edge %s -> %s", from, to))
	}
	predIndex, ok := blockIndexByID[from]
	if !ok {
		panic(fmt.Sprintf("lower merge missing predecessor block %s", from))
	}
	succs := f.CFG.Successors(from)
	if !hasMergeBlockID(succs, to) {
		panic(fmt.Sprintf("lower merge missing CFG edge %s -> %s", from, to))
	}

	if len(succs) > 1 {
		splitID := p.nextSyntheticBlockID(f)
		if !retargetTerminatorEdge(&f.Blocks[predIndex], to, splitID) {
			panic(fmt.Sprintf("lower merge could not retarget CFG edge %s -> %s", from, to))
		}
		if err := f.CFG.InsertSyntheticBlockOnEdge(from, to, splitID); err != nil {
			panic(err)
		}
		p.insertBlockAfter(f, predIndex, Block{
			ID:      splitID,
			Effects: append(append([]Effect(nil), assigns...), &Jump{To: to}),
		})
		return true
	}

	f.Blocks[predIndex].Effects = insertBeforeTerminator(f.Blocks[predIndex].Effects, assigns)
	return true
}

// hasMergeBlockID reports whether ids contains id.
func hasMergeBlockID(ids []machine.BlockID, id machine.BlockID) bool {
	return slices.Contains(ids, id)
}

// nextSyntheticBlockID returns an unused block ID for an inserted edge block.
func (p *lowerMergesProcessor) nextSyntheticBlockID(f *Func) machine.BlockID {
	for {
		id := p.nextBlockID
		p.nextBlockID++
		if !semFuncHasBlock(f, id) && (f.CFG == nil || f.CFG.Block(id) == nil) {
			return id
		}
	}
}

// insertBlockAfter inserts block after index in semantic block order.
func (p *lowerMergesProcessor) insertBlockAfter(f *Func, index int, block Block) {
	f.Blocks = append(f.Blocks, Block{})
	copy(f.Blocks[index+2:], f.Blocks[index+1:])
	f.Blocks[index+1] = block
}

// retargetTerminatorEdge redirects a block terminator edge from old to next.
func retargetTerminatorEdge(block *Block, old machine.BlockID, next machine.BlockID) bool {
	if len(block.Effects) == 0 {
		return false
	}
	switch term := block.Effects[len(block.Effects)-1].(type) {
	case *Branch:
		retargeted := false
		replacement := *term
		if replacement.TrueBlock == old {
			replacement.TrueBlock = next
			retargeted = true
		}
		if replacement.FalseBlock == old {
			replacement.FalseBlock = next
			retargeted = true
		}
		if retargeted {
			block.Effects[len(block.Effects)-1] = &replacement
		}
		return retargeted

	case *Jump:
		if term.To != old {
			return false
		}
		replacement := *term
		replacement.To = next
		block.Effects[len(block.Effects)-1] = &replacement
		return true

	default:
		return false
	}
}

// insertBeforeTerminator inserts effects before a trailing explicit terminator.
func insertBeforeTerminator(effects []Effect, inserts []Effect) []Effect {
	if len(effects) == 0 {
		return append([]Effect(nil), inserts...)
	}
	switch effects[len(effects)-1].(type) {
	case *Branch, *Jump, *Return:
		out := make([]Effect, 0, len(effects)+len(inserts))
		out = append(out, effects[:len(effects)-1]...)
		out = append(out, inserts...)
		out = append(out, effects[len(effects)-1])
		return out
	default:
		out := append([]Effect(nil), effects...)
		out = append(out, inserts...)
		return out
	}
}

// firstSyntheticMergeBlockID returns the starting ID for merge edge blocks.
func firstSyntheticMergeBlockID(f *Func) machine.BlockID {
	id := machine.BlockID(0xf0000000)
	for semFuncHasBlock(f, id) || f.CFG != nil && f.CFG.Block(id) != nil {
		id++
	}
	return id
}

// semFuncHasBlock reports whether f contains id in its semantic block list.
func semFuncHasBlock(f *Func, id machine.BlockID) bool {
	for _, block := range f.Blocks {
		if block.ID == id {
			return true
		}
	}
	return false
}

// assertNoMergeExprs panics if a merge expression remains after lowering.
func assertNoMergeExprs(f *Func) {
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			walkEffect(effect, func(expr Expr) {
				if _, ok := expr.(*Merge); ok {
					panic(fmt.Sprintf("semantic merge remained after lowering in block %s", block.ID))
				}
			})
		}
	}
}
