package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// scratchExpressionPure excludes evaluation with side effects or unknown semantics.
func scratchExpressionPure(expr Expr) bool {
	pure := true
	walkExpr(expr, func(e Expr) {
		switch e.(type) {
		case *Call, *RawValue, *RawMemory, *Merge:
			pure = false
		}
	})
	return pure
}

// scratchSameStorage compares direct variables consistently across semantic
// Local/Global nodes and equivalent symbolic roots.
func scratchSameStorage(a, b LValue) bool {
	values := [2]LValue{a, b}
	for i, value := range values {
		if ref, ok := value.(*SymbolRef); ok {
			if root, ok := ref.Path.(*symresolve.SymbolRoot); ok {
				switch variable := root.Symbol.(type) {
				case *typeinfo.FunctionVar:
					values[i] = &Local{FunctionVar: *variable}
				case *typeinfo.GlobalVar:
					values[i] = &Global{GlobalVar: variable}
				}
			}
		}
	}
	return sameLValue(values[0], values[1])
}

// scratchStorageExprsOverlap reports whether two expressions name overlapping
// physical compiler-scratch bytes. Non-scratch expressions never overlap scratch.
func scratchStorageExprsOverlap(a, b Expr) bool {
	aOff, aSize, aOK := scratchSlotRangeExpr(a)
	bOff, bSize, bOK := scratchSlotRangeExpr(b)
	if !aOK || !bOK {
		return false
	}

	aRange := ScratchRange{Offset: aOff, Size: aSize}
	bRange := ScratchRange{Offset: bOff, Size: bSize}
	return aRange.Overlaps(bRange)
}

// scratchAssignmentMayClobber checks physical and promoted dependencies before
// allowing a store to independent, unescaped scratch storage. Addressable or
// unresolved scratch retains the ordinary conservative memory-alias behavior.
func scratchAssignmentMayClobber(assign *Assign, object *scratchObject, dependencies []LValue, memory, register bool) bool {
	if _, _, dstScratch := scratchSlotRangeExpr(assign.Dst); dstScratch {
		if register || memory && (object.disposition == scratchLocal || object.disposition == scratchUnresolved) {
			return true
		}
		for _, dep := range dependencies {
			if scratchStorageExprsOverlap(assign.Dst, dep) || scratchSameStorage(object.storage, dep) {
				return true
			}
		}
		return false
	}

	if memory || register {
		return true
	}
	for _, dep := range dependencies {
		if scratchSameStorage(dep, assign.Dst) {
			return true
		}

		// Stores through pointers may alias locals. Temp objects have no address,
		// but scratch definitions may be rewritten to that same temp later.
		switch assign.Dst.(type) {
		case *Local:
		case *Temp:
		case *SymbolRef:
			ref := assign.Dst.(*SymbolRef)
			if _, ok := ref.Path.(*symresolve.SymbolRoot); !ok {
				return true
			}
		default:
			return true
		}
	}
	return false
}

// scratchUseHasEarlierCall distinguishes enclosing calls, which execute after
// their arguments, from sibling calls that may execute before a scratch read.
func scratchUseHasEarlierCall(effect Effect, region ScratchRange) bool {
	reads := 0
	w := scratchAccessRewriter(func(expr Expr, kind scratchAccessKind, typ typeinfo.Type) Expr {
		if off, size, ok := scratchSlotRangeExpr(expr); ok && kind == scratchRead && (ScratchRange{off, size}) == region {
			reads++
		}
		return expr
	})
	w.rewriteEffect(effect)
	total := reads
	hazard := false
	walkEffect(effect, func(expr Expr) {
		if call, ok := expr.(*Call); ok {
			reads = 0
			w.rewriteExpr(call)
			if reads < total {
				hazard = true
			}
		}
	})
	return hazard
}

// scratchCanMove checks every path from a definition to its use for ordering
// barriers. Constants cross calls; mutable values must survive all intervening
// stores and calls. Register nodes are conservatively treated as mutable.
func scratchCanMove(f *Func, a *scratchAnalysis, value Expr, definition int, use scratchUseKey) bool {
	from := a.defs[definition].point
	to := use.point
	if !scratchExpressionPure(value) {
		return false
	}
	var dependencies []LValue
	memory, register := false, false
	walkExpr(value, func(e Expr) {
		switch v := e.(type) {
		case *Register:
			register = true
		case *Global, *Deref, *Memory, *ArrayIndex, *FieldAccess:
			memory = true
		case *SymbolRef:
			root, ok := v.Path.(*symresolve.SymbolRoot)
			if !ok {
				memory = true
			} else if _, ok := root.Symbol.(*typeinfo.FunctionVar); !ok {
				memory = true
			}
		}
		if lv, ok := e.(LValue); ok {
			dependencies = append(dependencies, lv)
		}
	})
	if len(dependencies) == 0 && !register {
		return true
	}
	callMutable := memory || register
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			walkEffect(effect, func(e Expr) {
				addr, ok := e.(*AddressOf)
				if !ok {
					return
				}
				walkExpr(addr.Target, func(target Expr) {
					if lv, ok := target.(LValue); ok {
						for _, dep := range dependencies {
							if scratchSameStorage(lv, dep) {
								callMutable = true
							}
						}
					}
				})
			})
		}
	}
	if callMutable && scratchUseHasEarlierCall(f.Blocks[to.block].Effects[to.effect], use.region) {
		return false
	}
	seen := map[scratchPoint]bool{}
	var check func(scratchPoint) bool
	check = func(end scratchPoint) bool {
		if seen[end] {
			return true
		}
		seen[end] = true
		for ei := end.effect - 1; ei >= 0; ei-- {
			point := scratchPoint{end.block, ei}
			if point == from {
				return true
			}
			effect := f.Blocks[end.block].Effects[ei]
			barrier := false
			walkEffect(effect, func(e Expr) {
				if _, ok := e.(*Call); ok {
					barrier = callMutable
				}
			})
			if _, ok := effect.(*CallEffect); ok {
				barrier = callMutable
				if result, ok := effect.(*CallEffect).Result.(LValue); ok {
					for _, dep := range dependencies {
						barrier = barrier || scratchSameStorage(result, dep)
					}
				}
			}
			if _, ok := effect.(*RawEffect); ok {
				barrier = true
			}
			if assign, ok := effect.(*Assign); ok {
				var object *scratchObject
				if id, ok := a.writes[point]; ok {
					object = a.defs[id].object
				}
				barrier = barrier || scratchAssignmentMayClobber(assign, object, dependencies, memory, register)
			}
			if barrier {
				return false
			}
		}
		if a.entries[end.block] {
			return false
		}
		for _, pred := range a.predecessors[end.block] {
			// At a join, check only paths carrying this definition. Other
			// reaching definitions receive their own motion/equivalence check.
			if !a.exits[pred][use.region.Offset][definition] {
				continue
			}
			if !check(scratchPoint{pred, len(f.Blocks[pred].Effects)}) {
				return false
			}
		}
		return true
	}
	return check(to)
}
