package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type callResultProcessor struct{}

type materializedCallResultKey struct {
	function *typeinfo.Function
	instOff  uint32
}

type callResultStats struct {
	result    *CallResult
	defined   bool
	totalUses int
}

// ProcessFunc materializes every non-inline call result that has a later use.
func (p *callResultProcessor) ProcessFunc(result *Result, f *Func) bool {
	stats := collectCallResultStats(f)
	if len(stats) == 0 {
		return false
	}

	temps := make(map[materializedCallResultKey]*Temp)
	for key, stat := range stats {
		if stat.result == nil || !stat.defined || stat.totalUses == 0 {
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

// collectCallResultStats counts call-result uses while skipping call definitions.
func collectCallResultStats(f *Func) map[materializedCallResultKey]*callResultStats {
	stats := make(map[materializedCallResultKey]*callResultStats)
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
					stat.defined = true
				}
				countCallResultUsesInCall(stats, callEffect.Call)
				continue
			}
			countCallResultUsesInEffect(stats, effect)
		}
	}

	return stats
}

// countCallResultUsesInEffect counts all call-result expressions in one effect.
func countCallResultUsesInEffect(stats map[materializedCallResultKey]*callResultStats, effect Effect) {
	walkEffect(effect, func(expr Expr) {
		countCallResultUse(stats, expr)
	})
}

// countCallResultUsesInCall counts call-result expressions inside one call expression.
func countCallResultUsesInCall(stats map[materializedCallResultKey]*callResultStats, call *Call) {
	walkCall(call, func(expr Expr) {
		countCallResultUse(stats, expr)
	})
}

// countCallResultUse records one call-result expression when expr is a call result.
func countCallResultUse(stats map[materializedCallResultKey]*callResultStats, expr Expr) {
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
}

// rewriteCallResultsToTemps replaces selected call-result definitions and uses.
func rewriteCallResultsToTemps(effects []Effect, temps map[materializedCallResultKey]*Temp) ([]Effect, bool) {
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
func keyForCallResult(result *CallResult) materializedCallResultKey {
	return materializedCallResultKey{function: result.Function, instOff: result.InstOff}
}

// newCallResultTemp creates the semantic temp used to preserve a call result.
func newCallResultTemp(result *CallResult) *Temp {
	return &Temp{
		Name:     fmt.Sprintf("t_call_%04x", result.InstOff),
		ID:       machine.ValueID{InstOff: result.InstOff},
		TypeInfo: result.ExprType(),
	}
}
