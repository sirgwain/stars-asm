package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type branchCallResultProcessor struct{}

type branchCallResultKey struct {
	function *typeinfo.Function
	instOff  uint32
}

type callResultStats struct {
	result     *CallResult
	totalUses  int
	branchUses int
}

// ProcessFunc materializes wide call results that feed multi-effect branch comparisons.
func (p *branchCallResultProcessor) ProcessFunc(result *Result, f *Func) bool {
	stats := collectBranchCallResultStats(f)
	if len(stats) == 0 {
		return false
	}

	temps := make(map[branchCallResultKey]*Temp)
	for key, stat := range stats {
		if stat.result == nil || stat.totalUses <= 1 || stat.branchUses == 0 || !wideCallResult(stat.result) {
			continue
		}
		temps[key] = newCallResultTemp(stat.result)
	}
	if len(temps) == 0 {
		return false
	}

	changed := false
	for i := range f.Blocks {
		effects, blockChanged := rewriteCallResultsToTemps(f.Blocks[i].Effects, temps)
		if blockChanged {
			f.Blocks[i].Effects = effects
			changed = true
		}
	}
	return changed
}

// collectBranchCallResultStats counts call-result uses while skipping call definitions.
func collectBranchCallResultStats(f *Func) map[branchCallResultKey]*callResultStats {
	stats := make(map[branchCallResultKey]*callResultStats)
	if f == nil {
		return stats
	}

	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			if callEffect, ok := effect.(*CallEffect); ok {
				if result, ok := callEffect.Result.(*CallResult); ok {
					key := keyForCallResult(result)
					stat := stats[key]
					if stat == nil {
						stat = &callResultStats{}
						stats[key] = stat
					}
					stat.result = result
				}
				countCallResultUsesInCall(stats, callEffect.Call, false)
				continue
			}

			branch, isBranch := effect.(*Branch)
			if isBranch {
				countCallResultUsesInExpr(stats, branch.Cond, true)
				continue
			}
			countCallResultUsesInEffect(stats, effect, false)
		}
	}

	return stats
}

// countCallResultUsesInEffect counts all call-result expressions in one effect.
func countCallResultUsesInEffect(stats map[branchCallResultKey]*callResultStats, effect Effect, branch bool) {
	walkEffect(effect, func(expr Expr) {
		countCallResultUse(stats, expr, branch)
	})
}

// countCallResultUsesInCall counts call-result expressions inside one call expression.
func countCallResultUsesInCall(stats map[branchCallResultKey]*callResultStats, call *Call, branch bool) {
	walkCall(call, func(expr Expr) {
		countCallResultUse(stats, expr, branch)
	})
}

// countCallResultUsesInExpr counts call-result expressions inside one expression.
func countCallResultUsesInExpr(stats map[branchCallResultKey]*callResultStats, expr Expr, branch bool) {
	walkExpr(expr, func(expr Expr) {
		countCallResultUse(stats, expr, branch)
	})
}

// countCallResultUse records one call-result expression when expr is a call result.
func countCallResultUse(stats map[branchCallResultKey]*callResultStats, expr Expr, branch bool) {
	result, ok := expr.(*CallResult)
	if !ok {
		return
	}
	key := keyForCallResult(result)
	stat := stats[key]
	if stat == nil {
		stat = &callResultStats{result: result}
		stats[key] = stat
	}
	if stat.result == nil {
		stat.result = result
	}
	stat.totalUses++
	if branch {
		stat.branchUses++
	}
}

// rewriteCallResultsToTemps replaces selected call-result definitions and uses.
func rewriteCallResultsToTemps(effects []Effect, temps map[branchCallResultKey]*Temp) ([]Effect, bool) {
	rewriter := &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			result, ok := expr.(*CallResult)
			if !ok {
				return expr, false, false
			}
			temp := temps[keyForCallResult(result)]
			if temp == nil {
				return expr, false, false
			}
			return temp, true, true
		},
	}

	out := append([]Effect(nil), effects...)
	changed := false
	for i, effect := range effects {
		if callEffect, ok := effect.(*CallEffect); ok {
			if result, ok := callEffect.Result.(*CallResult); ok {
				if temp := temps[keyForCallResult(result)]; temp != nil {
					next := *callEffect
					next.Result = temp
					out[i] = &next
					changed = true
					continue
				}
			}
		}
		next, effectChanged := rewriter.rewriteEffect(effect)
		if effectChanged {
			out[i] = next
			changed = true
		}
	}
	if !changed {
		return effects, false
	}
	return out, true
}

// keyForCallResult returns the stable identity for a call result.
func keyForCallResult(result *CallResult) branchCallResultKey {
	return branchCallResultKey{function: result.Function, instOff: result.InstOff}
}

// newCallResultTemp creates the semantic temp used to preserve a call result.
func newCallResultTemp(result *CallResult) *Temp {
	return &Temp{
		Name:     fmt.Sprintf("t_call_%04x", result.InstOff),
		ID:       machine.ValueID{InstOff: result.InstOff},
		TypeInfo: result.ExprType(),
	}
}

// wideCallResult reports whether result has a return type wider than one word.
func wideCallResult(result *CallResult) bool {
	typ := result.ExprType()
	return typ != nil && typ.Bytes() > 2
}
