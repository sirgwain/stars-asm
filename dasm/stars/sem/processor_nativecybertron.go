package sem

import (
	"fmt"
	"math"
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
	overlayCyberOrderFrame(f)
	return true
}

// overlayCyberOrderFrame gives shdef, rgRecycleSBShdef, and ord the one
// storage their blocks shared in the Win16 frame. The mine-laying order never
// sets its task union, so it moves whatever those locals last left there:
// zeros from the late-game recycle clears, which make the fleet stop laying
// on arrival, or bytes of a scrapped design. Native locals are separate, so
// the order would carry unrelated stack bytes instead.
func overlayCyberOrderFrame(f *Func) {
	locals := map[string]*Local{"shdef": nil, "rgRecycleSBShdef": nil, "ord": nil}
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			walkEffect(effect, func(expr Expr) {
				if v, ok := expr.(*Local); ok {
					if _, want := locals[v.Name]; want {
						locals[v.Name] = v
					}
				}
			})
		}
	}
	base, end := math.MaxInt, math.MinInt
	for name, v := range locals {
		if v == nil {
			panic(fmt.Sprintf("native-cybertron: overlaid local %s not found", name))
		}
		base = min(base, v.BPOffset)
		end = max(end, v.BPOffset+v.Type.Bytes())
	}
	frame := &Local{FunctionVar: typeinfo.FunctionVar{Name: "rgbOrdFrame", Type: &typeinfo.Array{Elem: typeinfo.U8, Count: end - base}}}
	f.RecoveredLocals = append(f.RecoveredLocals, &frame.FunctionVar)
	at := func(v *Local) Expr {
		if v.BPOffset == base {
			return frame
		}
		return &AddressOf{
			Target:   &ArrayIndex{Base: frame, Index: &Const{TypeInfo: typeinfo.I16, U64: uint64(v.BPOffset - base)}, TypeInfo: typeinfo.U8},
			TypeInfo: &typeinfo.Pointer{Elem: typeinfo.U8},
		}
	}
	recycle := locals["rgRecycleSBShdef"]
	w := semRewriter{
		lvalue: func(_ *semRewriter, value LValue) (LValue, bool, bool) {
			switch v := value.(type) {
			case *ArrayIndex:
				array, ok := v.Base.(*Local)
				if !ok || array.Name != recycle.Name {
					return nil, false, false
				}
				index, ok := v.Index.(*Const)
				if !ok {
					panic(fmt.Sprintf("native-cybertron: unexpected recycle index %s", FormatExpr(v)))
				}
				return &ArrayIndex{Base: frame, Index: &Const{TypeInfo: index.TypeInfo, U64: index.U64 + uint64(recycle.BPOffset-base)}, TypeInfo: v.TypeInfo}, true, true
			case *Local:
				if v.Name != "shdef" && v.Name != "ord" {
					return nil, false, false
				}
				pointer := &typeinfo.Pointer{Elem: v.Type}
				return &Deref{
					Pointer:  &Cast{Value: at(v), To: typeinfo.TypeDecl(pointer, ""), TypeInfo: pointer},
					Width:    v.Type.Bytes(),
					TypeInfo: v.Type,
				}, true, true
			}
			return nil, false, false
		},
		expr: func(_ *semRewriter, e Expr) (Expr, bool, bool) {
			if v, ok := e.(*Local); ok && v.Name == recycle.Name {
				return at(v), true, true
			}
			return nil, false, false
		},
	}
	for bi := range f.Blocks {
		f.Blocks[bi].Effects, _ = w.rewriteEffects(f.Blocks[bi].Effects)
	}
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
