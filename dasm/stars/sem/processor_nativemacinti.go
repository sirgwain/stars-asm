package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// nativeMacintiProcessor repairs DoMacintiAiTurn's late-game fleet splits,
// which the original copied from the 16-entry ship design code onto the
// 10-entry starbase recycle array.
type nativeMacintiProcessor struct {
	ctx *FuncContext
}

// ProcessFunc gives the late-game SplitOutShdefs calls a 16-entry scratch
// array. The original stored the miner (14, 15) and freighter (10, 11) marks
// past rgRecycleSBShdef, into the bytes of l and fTonsOfMinerals that follow
// it in the Win16 frame, cleared only the first 10 entries between calls,
// and let SplitOutShdefs read all 16. Each call now marks only the designs
// set before it; the stale bytes the original read cannot be reproduced.
func (p *nativeMacintiProcessor) ProcessFunc(_ *Result, f *Func) bool {
	if p.ctx.fs.Name != "DoMacintiAiTurn" {
		return false
	}
	var recycle *Local
	bi := -1
	for i, block := range f.Blocks {
		for _, effect := range block.Effects {
			if assign, ok := effect.(*Assign); ok {
				if part, ok := assign.Dst.(*Part); ok {
					if _, ok := part.Base.(*Local); ok && bi != i {
						if bi != -1 {
							panic("native-macinti: aliased recycle stores span more than one block")
						}
						bi = i
					}
				}
			}
			walkEffect(effect, func(expr Expr) {
				if v, ok := expr.(*Local); ok && v.Name == "rgRecycleSBShdef" {
					recycle = v
				}
			})
		}
	}
	if bi == -1 || recycle == nil {
		panic("native-macinti: late-game recycle splits not found")
	}
	split := &Local{FunctionVar: typeinfo.FunctionVar{Name: "rgSplitShdef", Type: &typeinfo.Array{Elem: typeinfo.U8, Count: 16}}}
	f.RecoveredLocals = append(f.RecoveredLocals, &split.FunctionVar)
	parts := 0
	w := semRewriter{
		lvalue: func(_ *semRewriter, v LValue) (LValue, bool, bool) {
			part, ok := v.(*Part)
			if !ok {
				return nil, false, false
			}
			base, ok := part.Base.(*Local)
			if !ok || part.Width != 1 {
				panic(fmt.Sprintf("native-macinti: unexpected aliased store %s", FormatExpr(part)))
			}
			index := base.BPOffset + part.ByteOff - recycle.BPOffset
			if index < 10 || index >= 16 {
				panic(fmt.Sprintf("native-macinti: %s is entry %d of the recycle array", FormatExpr(part), index))
			}
			parts++
			return &ArrayIndex{Base: split, Index: &Const{TypeInfo: typeinfo.I16, U64: uint64(index)}, TypeInfo: typeinfo.U8}, true, true
		},
		expr: func(_ *semRewriter, e Expr) (Expr, bool, bool) {
			if v, ok := e.(*Local); ok && v.Name == recycle.Name {
				return split, true, true
			}
			return nil, false, false
		},
		call: func(w *semRewriter, call *Call, _ machine.Meta) (*Call, bool, bool) {
			if call.Function == nil || call.Function.Name != "memset" {
				return nil, false, false
			}
			next, _ := w.rewriteCallChildren(call)
			size, ok := next.Args[2].(*Const)
			if !ok || size.U64 != 10 {
				panic(fmt.Sprintf("native-macinti: unexpected recycle memset %s", FormatExpr(call)))
			}
			next.Args[2] = &Const{TypeInfo: size.TypeInfo, U64: 16}
			return next, true, true
		},
	}
	effects, _ := w.rewriteEffects(f.Blocks[bi].Effects)
	if parts != 4 {
		panic(fmt.Sprintf("native-macinti: expected 4 aliased recycle stores, got %d", parts))
	}
	f.Blocks[bi].Effects = effects
	return true
}
