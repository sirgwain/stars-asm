package sem

import (
	"cmp"
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type AnalyzeResult struct {
	Merges            int                             `json:"merges,omitempty"`
	Temps             int                             `json:"temps,omitempty"`
	FarPointers       int                             `json:"farPointers,omitempty"`
	HiWords           int                             `json:"hiWords,omitempty"`
	LoWords           int                             `json:"loWords,omitempty"`
	BranchHiWords     int                             `json:"branchHiWords,omitempty"`
	BranchLoWords     int                             `json:"branchLoWords,omitempty"`
	SegRegRefs        int                             `json:"segRegRefs,omitempty"`
	FunctionPathFacts []typeinfo.FunctionPathFactJSON `json:"functionPathFacts,omitempty"`
}

func (fn *Func) Analyze(ctx *FuncContext) AnalyzeResult {
	var result AnalyzeResult
	temps := make(map[string]struct{})
	var previousBlock *machine.BlockID
	var previousUnionContext *symresolve.UnionContext
	if ctx != nil {
		previousBlock = ctx.currentBlock
		previousUnionContext = ctx.currentUnionContext
		defer func() {
			ctx.currentBlock = previousBlock
			ctx.currentUnionContext = previousUnionContext
		}()
	}

	for _, block := range fn.Blocks {
		var unionSelections []symresolve.UnionSelection
		if ctx != nil {
			ctx.SetCurrentBlock(block.ID)
			if ctx.unionContext() != nil {
				unionSelections = ctx.unionContext().Selections()
			}
		}
		for _, effect := range block.Effects {
			// Count unique semantic temp variables.
			walkEffect(effect, func(expr Expr) {
				if temp, ok := expr.(*Temp); ok {
					temps[temp.Name] = struct{}{}
				}
				recordUnionBlockPathFacts(ctx, block.ID, unionSelections, expr)
			})

			if branch, ok := effect.(*Branch); ok {
				// Count branch word projections separately.
				walkExpr(branch.Cond, func(expr Expr) {
					result.countExpr(ctx, expr, true)
				})
				continue
			}

			walkEffect(effect, func(expr Expr) {
				result.countExpr(ctx, expr, false)
			})
		}
	}
	result.Temps = len(temps)
	if ctx != nil {
		result.FunctionPathFacts = aggregateUnionBlockPathFacts(ctx.RecordedUnionBlockPathFacts())
	}
	return result
}

// aggregateUnionBlockPathFacts groups block selections into union-loader function fact records.
func aggregateUnionBlockPathFacts(facts map[machine.BlockID][]*typeinfo.UnionBlockPathFact) []typeinfo.FunctionPathFactJSON {
	var ordered []*typeinfo.UnionBlockPathFact
	for _, blockFacts := range facts {
		ordered = append(ordered, blockFacts...)
	}
	slices.SortFunc(ordered, func(left, right *typeinfo.UnionBlockPathFact) int {
		if n := cmp.Compare(left.Func.Name, right.Func.Name); n != 0 {
			return n
		}
		if n := cmp.Compare(left.Root, right.Root); n != 0 {
			return n
		}
		if left.AllElements != right.AllElements {
			if !left.AllElements {
				return -1
			}
			return 1
		}
		if n := cmp.Compare(left.Type.String(), right.Type.String()); n != 0 {
			return n
		}
		if n := slices.Compare(left.Path, right.Path); n != 0 {
			return n
		}
		if n := cmp.Compare(left.Enum.Name, right.Enum.Name); n != 0 {
			return n
		}
		if n := cmp.Compare(left.BlockOff, right.BlockOff); n != 0 {
			return n
		}
		return cmp.Compare(left.Value.Value, right.Value.Value)
	})

	var out []typeinfo.FunctionPathFactJSON
	for _, fact := range ordered {
		if len(out) == 0 || !functionPathFactMatchesBlockFact(out[len(out)-1], fact) {
			out = append(out, typeinfo.FunctionPathFactJSON{
				Func:        fact.Func.Name,
				Root:        fact.Root,
				AllElements: fact.AllElements,
				Type:        fact.Type.String(),
				Path:        append([]string(nil), fact.Path...),
				Enum:        fact.Enum.Name,
			})
		}
		group := &out[len(out)-1]
		group.BlockPathFacts = append(group.BlockPathFacts, typeinfo.BlockPathFactJSON{
			Block: machine.BlockID(fact.BlockOff).String(),
			Value: fact.Value.Name,
		})
	}
	return out
}

// functionPathFactMatchesBlockFact reports whether a block fact belongs to an output fact group.
func functionPathFactMatchesBlockFact(group typeinfo.FunctionPathFactJSON, fact *typeinfo.UnionBlockPathFact) bool {
	return group.Func == fact.Func.Name &&
		group.Root == fact.Root &&
		group.AllElements == fact.AllElements &&
		group.Type == fact.Type.String() &&
		slices.Equal(group.Path, fact.Path) &&
		group.Enum == fact.Enum.Name
}

// recordUnionBlockPathFacts records block context needed by resolved union member accesses.
func recordUnionBlockPathFacts(ctx *FuncContext, block machine.BlockID, selections []symresolve.UnionSelection, expr Expr) {
	if ctx == nil {
		return
	}
	for _, selection := range selections {
		if exprUsesUnionSelection(expr, selection) {
			ctx.addUnionSelectionBlockFact(block, selection)
		}
	}
}

// exprUsesUnionSelection reports whether an expression traverses a member governed by selection.
func exprUsesUnionSelection(expr Expr, selection symresolve.UnionSelection) bool {
	switch e := expr.(type) {
	case *FieldAccess:
		return unionSelectionExprBaseMatches(e.Base, selection) && unionRuleContainsField(selection.Rule, e.Field)
	case *SymbolRef:
		return symbolPathUsesUnionSelection(e.Path, selection)
	default:
		return false
	}
}

// unionSelectionExprBaseMatches reports whether a semantic member base belongs to a selection root.
func unionSelectionExprBaseMatches(base Expr, selection symresolve.UnionSelection) bool {
	if deref, ok := base.(*Deref); ok {
		base = deref.Pointer
	}
	if selection.AllElements {
		index, ok := base.(*ArrayIndex)
		if !ok {
			return false
		}
		base = index.Base
	}
	path, ok := symbolPathForExpr(base)
	return ok && selection.Root != nil && symresolve.Equals(path, selection.Root)
}

// symbolPathUsesUnionSelection reports whether a path traverses a member governed by selection.
func symbolPathUsesUnionSelection(path symresolve.SymbolPath, selection symresolve.UnionSelection) bool {
	switch p := path.(type) {
	case *symresolve.SymbolField:
		if unionSelectionBaseMatches(p.Base, selection) && unionRuleContainsField(selection.Rule, p.Field) {
			return true
		}
		return symbolPathUsesUnionSelection(p.Base, selection)
	case *symresolve.SymbolBitfield:
		if unionSelectionBaseMatches(p.Base, selection) && unionRuleContainsField(selection.Rule, p.Field) {
			return true
		}
		return symbolPathUsesUnionSelection(p.Base, selection)
	case *symresolve.SymbolDeref:
		return symbolPathUsesUnionSelection(p.Base, selection)
	case *symresolve.SymbolOffset:
		return symbolPathUsesUnionSelection(p.Base, selection)
	case *symresolve.SymbolTerm:
		return symbolPathUsesUnionSelection(p.Base, selection)
	default:
		return false
	}
}

// unionSelectionBaseMatches reports whether a member base belongs to a selection root.
func unionSelectionBaseMatches(base symresolve.SymbolPath, selection symresolve.UnionSelection) bool {
	if deref, ok := base.(*symresolve.SymbolDeref); ok {
		base = deref.Base
	}
	if selection.AllElements {
		term, ok := base.(*symresolve.SymbolTerm)
		if !ok {
			return false
		}
		base = term.Base
	}
	if offset, ok := base.(*symresolve.SymbolOffset); ok && offset.Offset == 0 {
		base = offset.Base
	}
	return selection.Root != nil && symresolve.Equals(base, selection.Root)
}

// unionRuleContainsField reports whether a field is a selectable member of a union rule.
func unionRuleContainsField(rule *typeinfo.UnionVariantRule, field *typeinfo.StructField) bool {
	if rule == nil || field == nil {
		return false
	}
	for _, member := range rule.ValueToMember {
		if member == field {
			return true
		}
	}
	return false
}

func (r *AnalyzeResult) countExpr(ctx *FuncContext, expr Expr, branch bool) {
	switch e := expr.(type) {
	case *Merge:
		r.Merges++

	case *FarPointer:
		// FarPointer values represent offset/segment projections only.

	case *Memory:
		if ctx != nil && (exprMatchesMachineValue(e.Seg, ctx.dsReg) || exprMatchesMachineValue(e.Seg, ctx.csReg)) {
			r.SegRegRefs++
		}

	case *Word:
		switch e.Part {
		case machine.WordLow:
			if branch {
				r.BranchLoWords++
			} else {
				r.LoWords++
			}

		case machine.WordHigh:
			if branch {
				r.BranchHiWords++
			} else {
				r.HiWords++
			}
		}

	case *Part:
		// formatPart() renders these as LOWORD/HIWORD too.
		if e.Width != 2 {
			return
		}

		switch e.ByteOff {
		case 0:
			if branch {
				r.BranchLoWords++
			} else {
				r.LoWords++
			}

		case 2:
			if branch {
				r.BranchHiWords++
			} else {
				r.HiWords++
			}
		}
	}
}
