package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// inlineScratchAssignments moves a captured call or merge into its sole, immediately
// following assignment. Run after recovery so aliases have already been expanded
// and counting references cannot miss a second use hidden behind another slot.
func inlineScratchAssignments(f *Func, a *scratchAnalysis) bool {
	refs := map[string]int{}
	for _, object := range a.objects {
		if object.disposition == scratchTemp {
			refs[object.storage.(*Temp).Name] = 0
		}
	}
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			walkEffect(effect, func(expr Expr) {
				if temp, ok := expr.(*Temp); ok {
					if _, tracked := refs[temp.Name]; tracked {
						refs[temp.Name]++
					}
				}
			})
		}
	}
	changed := false
	for bi, block := range f.Blocks {
		effects := make([]Effect, 0, len(block.Effects))
		for i := 0; i < len(block.Effects); i++ {
			effect := block.Effects[i]
			var dst, value Expr
			switch definition := effect.(type) {
			case *Assign:
				dst, value = definition.Dst, definition.Src
			case *CallEffect:
				dst, value = definition.Result, definition.Call
			}
			temp, ok := dst.(*Temp)
			// Count the definition as well as its one use; address-taking and
			// partial writes also count, preventing elimination of live storage.
			if !ok || refs[temp.Name] != 2 || i+1 == len(block.Effects) {
				effects = append(effects, effect)
				continue
			}
			use, ok := block.Effects[i+1].(*Assign)
			if !ok || !sameExpr(use.Src, temp) || !scratchAssignmentDestinationStable(use.Dst) {
				effects = append(effects, effect)
				continue
			}
			hasCapturedValue := false
			walkExpr(value, func(expr Expr) {
				switch expr.(type) {
				case *Call, *Merge:
					hasCapturedValue = true
				}
			})
			if !hasCapturedValue {
				effects = append(effects, effect)
				continue
			}
			// Preserve the scratch store's conversion before assigning to a
			// destination whose declared type or width may differ.
			if !typeinfo.Equals(value.ExprType(), temp.TypeInfo) {
				value = &Cast{Value: value, To: temp.TypeInfo.String(), TypeInfo: temp.TypeInfo}
			}
			next := *use
			next.Src = value
			effects = append(effects, &next)
			i++
			changed = true
		}
		f.Blocks[bi].Effects = effects
	}
	return changed
}

// scratchAssignmentDestinationStable excludes pointer and index reads that could
// move ahead of a captured expression when it is assigned directly to the destination.
func scratchAssignmentDestinationStable(dst LValue) bool {
	switch value := dst.(type) {
	case *Local, *Global, *Temp:
		return true
	case *Part:
		return scratchAssignmentDestinationStable(value.Base)
	case *FieldAccess:
		base, ok := value.Base.(LValue)
		return ok && !typeinfo.IsPointer(base.ExprType()) && scratchAssignmentDestinationStable(base)
	case *SymbolRef:
		var base symresolve.SymbolPath
		switch path := value.Path.(type) {
		case *symresolve.SymbolRoot:
			return true
		case *symresolve.SymbolField:
			base = path.Base
		case *symresolve.SymbolBitfield:
			base = path.Base
		case *symresolve.SymbolOffset:
			base = path.Base
		default:
			return false
		}
		return !typeinfo.IsPointer(base.Type()) && scratchAssignmentDestinationStable(&SymbolRef{Path: base})
	default:
		return false
	}
}
