package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

type AnalyzeResult struct {
	Merges        int `json:"merges,omitempty"`
	Temps         int `json:"temps,omitempty"`
	FarPointers   int `json:"farPointers,omitempty"`
	HiWords       int `json:"hiWords,omitempty"`
	LoWords       int `json:"loWords,omitempty"`
	BranchHiWords int `json:"branchHiWords,omitempty"`
	BranchLoWords int `json:"branchLoWords,omitempty"`
	SegRegRefs    int `json:"segRegRefs,omitempty"`
}

func (fn *Func) Analyze(ctx *FuncContext) AnalyzeResult {
	var result AnalyzeResult
	temps := make(map[string]struct{})

	for _, block := range fn.Blocks {
		for _, effect := range block.Effects {
			// Count unique semantic temp variables.
			walkEffect(effect, func(expr Expr) {
				if temp, ok := expr.(*Temp); ok {
					temps[temp.Name] = struct{}{}
				}
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
	return result
}

func (r *AnalyzeResult) countExpr(ctx *FuncContext, expr Expr, branch bool) {
	switch e := expr.(type) {
	case *Merge:
		r.Merges++

	case *FarPointer:
		// Count actual farptr(...) constructions, not
		// offset()/segment() projections of an existing pointer.
		if e.Part == machine.FarPointerWhole {
			r.FarPointers++
		}

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
