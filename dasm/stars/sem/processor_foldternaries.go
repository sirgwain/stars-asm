package sem

import (
	"fmt"
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

type foldTernariesProcessor struct{}

// ternaryArm is one side of a diamond whose only effect assigns the merge
// temp before reaching the join.
type ternaryArm struct {
	block machine.BlockID
	temp  *Temp
	value Expr
	join  machine.BlockID
}

// ProcessFunc folds lowered two-arm merge diamonds into conditional
// expressions:
//
//	A: branch c ? T : F
//	T: t = x; goto J
//	F: t = y
//	J: ... t ...
//
// becomes A: t = c ? x : y, and J is appended to A when A is its only
// predecessor and it follows A in layout. In a merged block the temp is then
// forwarded into its single use, and a call whose result is consumed by the
// next effect is nested back into it, as conversion does within one block.
func (p *foldTernariesProcessor) ProcessFunc(_ *Result, f *Func) bool {
	changed := false
	for i := 0; i < len(f.Blocks); {
		if p.foldAt(f, i) {
			changed = true
			continue
		}
		i++
	}
	return changed
}

// foldAt folds the diamond rooted at f.Blocks[index], if there is one.
func (p *foldTernariesProcessor) foldAt(f *Func, index int) bool {
	root := &f.Blocks[index]
	if len(root.Effects) == 0 {
		return false
	}
	branch, ok := root.Effects[len(root.Effects)-1].(*Branch)
	if !ok || branch.TrueBlock == branch.FalseBlock {
		return false
	}
	then, ok := p.matchArm(f, branch.TrueBlock, root.ID)
	if !ok {
		return false
	}
	els, ok := p.matchArm(f, branch.FalseBlock, root.ID)
	if !ok || then.join != els.join || then.join == root.ID || !sameExpr(then.temp, els.temp) {
		return false
	}
	join := then.join
	preds := f.CFG.Predecessors(join)
	if len(preds) != 2 || !slices.Contains(preds, then.block) || !slices.Contains(preds, els.block) || countTempAssigns(f, then.temp) != 2 {
		return false
	}

	cond, thenValue, elseValue := branch.Cond, then.value, els.value
	if compare, ok := cond.(*Compare); ok && compare.Op == CompareNE {
		// Prefer the positive test the source most likely used.
		cond = &Compare{Op: CompareEQ, LHS: compare.LHS, RHS: compare.RHS}
		thenValue, elseValue = elseValue, thenValue
	}
	root.Effects = append(root.Effects[:len(root.Effects)-1:len(root.Effects)-1], &Assign{
		MetaInfo: branch.MetaInfo,
		Dst:      then.temp,
		Src: &Cond{
			TypeInfo: then.temp.TypeInfo,
			Cond:     cond,
			Then:     thenValue,
			Else:     elseValue,
		},
	})
	rootID := root.ID
	f.Blocks = slices.DeleteFunc(f.Blocks, func(b Block) bool { return b.ID == then.block || b.ID == els.block })
	p.rewriteEdges(f, map[machine.BlockID][]machine.BlockID{rootID: {join}, then.block: nil, els.block: nil})

	index = slices.IndexFunc(f.Blocks, func(b Block) bool { return b.ID == rootID })
	if index+1 >= len(f.Blocks) || f.Blocks[index+1].ID != join {
		f.Blocks[index].Effects = append(f.Blocks[index].Effects, &Jump{MetaInfo: branch.MetaInfo, To: join})
		return true
	}

	assign := len(f.Blocks[index].Effects) - 1
	f.Blocks[index].Effects = append(f.Blocks[index].Effects, f.Blocks[index+1].Effects...)
	p.rewriteEdges(f, map[machine.BlockID][]machine.BlockID{rootID: f.CFG.Successors(join), join: nil})
	f.Blocks = slices.Delete(f.Blocks, index+1, index+2)

	block := &f.Blocks[index]
	block.Effects = forwardTempToNextEffect(f, block.Effects, assign, then.temp)
	block.Effects = nestNextEffectCallResults(f, block.Effects)
	return true
}

// matchArm returns the arm of a ternary diamond rooted at root that starts at
// id: a block reached only from root holding one temp assignment and at most
// a jump to its single successor.
func (p *foldTernariesProcessor) matchArm(f *Func, id, root machine.BlockID) (ternaryArm, bool) {
	index := slices.IndexFunc(f.Blocks, func(b Block) bool { return b.ID == id })
	if index < 0 || !slices.Equal(f.CFG.Predecessors(id), []machine.BlockID{root}) {
		return ternaryArm{}, false
	}
	succs := f.CFG.Successors(id)
	if len(succs) != 1 || succs[0] == id {
		return ternaryArm{}, false
	}
	effects := f.Blocks[index].Effects
	switch len(effects) {
	case 1:
		if index+1 >= len(f.Blocks) || f.Blocks[index+1].ID != succs[0] {
			return ternaryArm{}, false
		}
	case 2:
		if jump, ok := effects[1].(*Jump); !ok || jump.To != succs[0] {
			return ternaryArm{}, false
		}
	default:
		return ternaryArm{}, false
	}
	assign, ok := effects[0].(*Assign)
	if !ok {
		return ternaryArm{}, false
	}
	temp, ok := assign.Dst.(*Temp)
	if !ok {
		return ternaryArm{}, false
	}
	return ternaryArm{block: id, temp: temp, value: assign.Src, join: succs[0]}, true
}

// rewriteEdges applies CFG edge replacements for a fold.
func (p *foldTernariesProcessor) rewriteEdges(f *Func, replacements map[machine.BlockID][]machine.BlockID) {
	if err := f.CFG.RewriteOutgoingEdges(replacements); err != nil {
		panic(fmt.Errorf("fold ternary: %w", err))
	}
}

// countTempAssigns counts the assignments to temp anywhere in f.
func countTempAssigns(f *Func, temp *Temp) int {
	count := 0
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			if assign, ok := effect.(*Assign); ok && sameExpr(assign.Dst, temp) {
				count++
			}
		}
	}
	return count
}

// countTempRefs counts the reads and writes of temp in effects.
func countTempRefs(effects []Effect, temp *Temp) int {
	count := 0
	for _, effect := range effects {
		walkEffect(effect, func(expr Expr) {
			if sameExpr(expr, temp) {
				count++
			}
		})
	}
	return count
}

// forwardTempToNextEffect replaces the single read of temp, when it is in the
// effect right after its assignment at index, with the assigned value and
// drops the assignment.
func forwardTempToNextEffect(f *Func, effects []Effect, index int, temp *Temp) []Effect {
	if index+1 >= len(effects) || countTempRefs(effects[index+1:index+2], temp) != 1 {
		return effects
	}
	total := 0
	for _, block := range f.Blocks {
		total += countTempRefs(block.Effects, temp)
	}
	// The assignment and the one read.
	if total != 2 || !callsEnclose(effects[index+1], func(expr Expr) bool { return sameExpr(expr, temp) }) {
		return effects
	}
	value := effects[index].(*Assign).Src
	rewriter := &semRewriter{
		expr: func(_ *semRewriter, expr Expr) (Expr, bool, bool) {
			if sameExpr(expr, temp) {
				return value, true, true
			}
			return nil, false, false
		},
	}
	use, _ := rewriter.rewriteEffect(effects[index+1])
	out := slices.Clone(effects[:index])
	out = append(out, use)
	return append(out, effects[index+2:]...)
}

// nestNextEffectCallResults nests each call whose result has a single use in
// the effect right after it into that use, as conversion does for call
// results confined to one machine block.
func nestNextEffectCallResults(f *Func, effects []Effect) []Effect {
	stats := collectCallResultStats(f)
	for k := len(effects) - 2; k >= 0; k-- {
		def, ok := effects[k].(*CallEffect)
		if !ok {
			continue
		}
		result, ok := def.Result.(*CallResult)
		if !ok {
			continue
		}
		key := keyForCallResult(result)
		isResult := func(expr Expr) bool {
			use, ok := expr.(*CallResult)
			return ok && keyForCallResult(use) == key
		}
		if stats[key].totalUses != 1 || !callsEnclose(effects[k+1], isResult) {
			continue
		}
		rewriter := &semRewriter{
			expr: func(_ *semRewriter, expr Expr) (Expr, bool, bool) {
				if isResult(expr) {
					return def.Call, true, true
				}
				return nil, false, false
			},
		}
		use, changed := rewriter.rewriteEffect(effects[k+1])
		if !changed {
			continue
		}
		effects = slices.Delete(slices.Clone(effects), k, k+1)
		effects[k] = use
	}
	return effects
}

// callsEnclose reports whether every call in effect has an expression
// matching match among its arguments, so no call in effect can run before
// the matched expression is evaluated. Moving a value into such a use keeps
// it ordered before every call it was computed ahead of.
func callsEnclose(effect Effect, match func(Expr) bool) bool {
	ok := true
	walkEffect(effect, func(expr Expr) {
		call, isCall := expr.(*Call)
		if !isCall || !ok {
			return
		}
		found := false
		walkCall(call, func(arg Expr) {
			found = found || match(arg)
		})
		ok = found
	})
	return ok
}
