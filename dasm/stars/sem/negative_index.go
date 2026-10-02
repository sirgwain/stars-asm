package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// NegativeIndexSite is an array element selected through a local that may
// hold -1 there, with no guard on the path. Win16 code that indexed with -1
// read or wrote whatever its memory layout put before the array.
type NegativeIndexSite struct {
	Block  machine.BlockID
	Effect int
	Index  *Local
	Access *ArrayIndex
}

// NegativeIndexFacts is the -1 dataflow result for one function: the sites
// that may index with -1, whether a returned value may be -1, and the
// pointer parameters a -1 may be stored through.
type NegativeIndexFacts struct {
	Sites       []NegativeIndexSite
	ReturnsNeg  bool
	StoresNegTo []int
}

// localKey identifies a local across the separate Local values the passes build.
type localKey struct {
	name     string
	bpOffset int
	register typeinfo.Register
}

// negSet is the set of locals that may hold -1 at a program point.
type negSet map[localKey]bool

// keyOf returns the identity of a local.
func keyOf(v *Local) localKey {
	return localKey{name: v.Name, bpOffset: v.BPOffset, register: v.Register}
}

// clone copies the set.
func (s negSet) clone() negSet {
	out := make(negSet, len(s))
	for k := range s {
		out[k] = true
	}
	return out
}

// AnalyzeNegativeIndexes runs a forward dataflow tracking which locals may
// hold -1. A local becomes -1 from a -1 constant, a copy of such a local, a
// call to a function whose return semantic is may_be_minus_one, or a call
// that receives its address in a parameter with that semantic. A branch on
// the local refines each edge, so `if (i != -1)` and bounds checks guard the
// code they dominate, and any other assignment clears it.
func AnalyzeNegativeIndexes(f *Func, fs *typeinfo.Function) NegativeIndexFacts {
	var facts NegativeIndexFacts
	if len(f.Blocks) == 0 {
		return facts
	}
	params := map[localKey]int{}
	for i := range fs.Params {
		params[keyOf(&Local{FunctionVar: fs.Params[i]})] = i
	}
	in := map[machine.BlockID]negSet{f.Blocks[0].ID: {}}
	for changed := true; changed; {
		changed = false
		for _, block := range f.Blocks {
			state, ok := in[block.ID]
			if !ok {
				continue
			}
			state = state.clone()
			for i, effect := range block.Effects {
				if i == len(block.Effects)-1 {
					if branch, ok := effect.(*Branch); ok {
						for _, edge := range []struct {
							to    machine.BlockID
							taken bool
						}{{branch.TrueBlock, true}, {branch.FalseBlock, false}} {
							out := state.clone()
							refineOnEdge(out, branch.Cond, edge.taken)
							changed = mergeInto(in, edge.to, out) || changed
						}
						continue
					}
				}
				transferNeg(state, effect)
			}
			if last := lastEffect(block); last != nil {
				if _, ok := last.(*Branch); ok {
					continue
				}
			}
			for _, succ := range f.CFG.Successors(block.ID) {
				changed = mergeInto(in, succ, state) || changed
			}
		}
	}
	stores := map[int]bool{}
	for _, block := range f.Blocks {
		state, ok := in[block.ID]
		if !ok {
			continue
		}
		state = state.clone()
		for i, effect := range block.Effects {
			walkEffect(effect, func(expr Expr) {
				access, ok := expr.(*ArrayIndex)
				if !ok {
					return
				}
				if index := indexLocal(access.Index); index != nil && state[keyOf(index)] {
					facts.Sites = append(facts.Sites, NegativeIndexSite{Block: block.ID, Effect: i, Index: index, Access: access})
				}
			})
			switch e := effect.(type) {
			case *Return:
				facts.ReturnsNeg = facts.ReturnsNeg || mayBeNeg(state, e.Value)
			case *Assign:
				if deref, ok := e.Dst.(*Deref); ok && deref.ByteOff == 0 && mayBeNeg(state, e.Src) {
					if p := indexLocal(deref.Pointer); p != nil {
						if idx, ok := params[keyOf(p)]; ok {
							stores[idx] = true
						}
					}
				}
			}
			transferNeg(state, effect)
		}
	}
	for i := range fs.Params {
		if stores[i] {
			facts.StoresNegTo = append(facts.StoresNegTo, i)
		}
	}
	return facts
}

// lastEffect returns a block's final effect, or nil for an empty block.
func lastEffect(block Block) Effect {
	if len(block.Effects) == 0 {
		return nil
	}
	return block.Effects[len(block.Effects)-1]
}

// mergeInto unions state into the entry state of a block and reports growth.
func mergeInto(in map[machine.BlockID]negSet, id machine.BlockID, state negSet) bool {
	cur, ok := in[id]
	if !ok {
		in[id] = state.clone()
		return true
	}
	grown := false
	for k := range state {
		if !cur[k] {
			cur[k] = true
			grown = true
		}
	}
	return grown
}

// transferNeg applies one effect's local definitions to the -1 set.
func transferNeg(state negSet, effect Effect) {
	var calls []*Call
	walkEffect(effect, func(expr Expr) {
		if call, ok := expr.(*Call); ok {
			calls = append(calls, call)
		}
	})
	if e, ok := effect.(*CallEffect); ok && e.Call != nil {
		calls = append(calls, e.Call)
	}
	for _, call := range calls {
		for i, arg := range call.Args {
			addr, ok := arg.(*AddressOf)
			if !ok {
				continue
			}
			target, ok := addr.Target.(*Local)
			if !ok {
				continue
			}
			if call.Function != nil && i < len(call.Function.Params) && call.Function.Params[i].Semantic == typeinfo.SemanticMayBeMinusOne {
				state[keyOf(target)] = true
			} else {
				delete(state, keyOf(target))
			}
		}
	}
	if e, ok := effect.(*Assign); ok {
		if dst, ok := e.Dst.(*Local); ok {
			if mayBeNeg(state, e.Src) {
				state[keyOf(dst)] = true
			} else {
				delete(state, keyOf(dst))
			}
		}
	}
	if e, ok := effect.(*CallEffect); ok {
		if dst, ok := e.Result.(*Local); ok {
			if e.Call != nil && e.Call.Function != nil && e.Call.Function.RetSemantic == typeinfo.SemanticMayBeMinusOne {
				state[keyOf(dst)] = true
			} else {
				delete(state, keyOf(dst))
			}
		}
	}
}

// mayBeNeg reports whether an expression may evaluate to -1.
func mayBeNeg(state negSet, expr Expr) bool {
	switch e := expr.(type) {
	case *Const:
		return isMinusOne(e)
	case *Local:
		return state[keyOf(e)]
	case *Cast:
		return mayBeNeg(state, e.Value)
	case *SignExtend:
		return mayBeNeg(state, e.Parent)
	case *Cond:
		return mayBeNeg(state, e.Then) || mayBeNeg(state, e.Else)
	case *Call:
		return e.Function != nil && e.Function.RetSemantic == typeinfo.SemanticMayBeMinusOne
	case *CallResult:
		return e.Function != nil && e.Function.RetSemantic == typeinfo.SemanticMayBeMinusOne
	}
	return false
}

// isMinusOne reports whether a constant is all ones in its type's width.
func isMinusOne(c *Const) bool {
	width := 2
	if c.TypeInfo != nil && c.TypeInfo.Bytes() > 0 {
		width = c.TypeInfo.Bytes()
	}
	if width >= 8 {
		return c.U64 == ^uint64(0)
	}
	return c.U64 == (uint64(1)<<(8*width))-1
}

// indexLocal returns the local an index or pointer expression reads, looking
// through casts and sign extensions, or nil.
func indexLocal(expr Expr) *Local {
	switch e := expr.(type) {
	case *Local:
		return e
	case *Cast:
		return indexLocal(e.Value)
	case *SignExtend:
		return indexLocal(e.Parent)
	}
	return nil
}

// refineOnEdge clears a local from the -1 set on a branch edge where the
// comparison proves it is not -1: an equality with another constant, a
// not-equal -1 test, a signed non-negative test, or an unsigned upper bound.
func refineOnEdge(state negSet, cond Expr, taken bool) {
	cmp, ok := cond.(*Compare)
	if !ok {
		return
	}
	op, lhs, rhs := cmp.Op, cmp.LHS, cmp.RHS
	if indexLocal(lhs) == nil {
		op, lhs, rhs = swapCompare(op), rhs, lhs
	}
	v := indexLocal(lhs)
	c, ok := rhs.(*Const)
	if v == nil || !ok {
		return
	}
	if !taken {
		op = negateCompare(op)
	}
	value := int64(int16(c.U64))
	var excludesMinusOne bool
	switch op {
	case CompareEQ:
		excludesMinusOne = !isMinusOne(c)
	case CompareNE:
		excludesMinusOne = isMinusOne(c)
	case CompareGE:
		excludesMinusOne = value >= 0
	case CompareGT:
		excludesMinusOne = value >= -1
	case CompareULT, CompareULE:
		// -1 is the largest unsigned value, so it fails any upper bound below it.
		excludesMinusOne = !isMinusOne(c)
	}
	if excludesMinusOne {
		delete(state, keyOf(v))
	}
}

// swapCompare returns the operator for the comparison with operands swapped.
func swapCompare(op CompareOp) CompareOp {
	switch op {
	case CompareLT:
		return CompareGT
	case CompareLE:
		return CompareGE
	case CompareGT:
		return CompareLT
	case CompareGE:
		return CompareLE
	case CompareULT:
		return CompareUGT
	case CompareULE:
		return CompareUGE
	case CompareUGT:
		return CompareULT
	case CompareUGE:
		return CompareULE
	}
	return op
}

// negateCompare returns the operator that holds when op does not.
func negateCompare(op CompareOp) CompareOp {
	switch op {
	case CompareEQ:
		return CompareNE
	case CompareNE:
		return CompareEQ
	case CompareLT:
		return CompareGE
	case CompareLE:
		return CompareGT
	case CompareGT:
		return CompareLE
	case CompareGE:
		return CompareLT
	case CompareULT:
		return CompareUGE
	case CompareULE:
		return CompareUGT
	case CompareUGT:
		return CompareULE
	case CompareUGE:
		return CompareULT
	}
	return op
}
