package sem

import (
	"math"
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type forwardTempsProcessor struct{}

// ProcessFunc removes temps that only relay a value to the next effect:
//
//	t = x; use(t)          becomes use(x)
//	t = x; v = t; use(t)   becomes v = x; use(v)
//
// Each rewrite must keep the program's behavior, not only its value in
// the common case: see forwardableValue for the conversion the temp
// performed, and forwardOrderSafe for the order the value and the rest of
// its use are evaluated in. The copy form keeps the assignment, so it only
// needs v to hold the temp's value wherever the temp was read. A merge temp
// assigned on each incoming edge and only copied is first replaced by stores
// to its copy on those edges; see sinkMergeCopy.
func (p *forwardTempsProcessor) ProcessFunc(_ *Result, f *Func) bool {
	addressed := addressTakenLocals(f)
	changed := false
	for sinkMergeCopy(f, addressed) {
		changed = true
	}
	for bi := range f.Blocks {
		for i := 0; i < len(f.Blocks[bi].Effects); i++ {
			effects := f.Blocks[bi].Effects
			temp, value, ok := tempDefinition(effects[i])
			if !ok || countTempDefs(f, temp) != 1 {
				continue
			}
			if next, ok := dropUnreadTemp(f, effects, i, temp, value); ok {
				f.Blocks[bi].Effects = next
				changed = true
				i--
				continue
			}
			if next, ok := forwardTempCopy(f, effects, i, temp, addressed); ok {
				f.Blocks[bi].Effects = next
				changed = true
				continue
			}
			if forwardTempUse(f, bi, i, temp, value, addressed) {
				changed = true
				i--
			}
		}
	}
	return changed
}

// sinkMergeCopy assigns the values of one copied merge temp to its copy
// directly, and reports whether it found one:
//
//	P1: t = a; goto J   P2: t = b   J: v = t   becomes   P1: v = a; goto J   P2: v = b   J:
//
// Every predecessor of J must end by assigning t and lead only to J, t must
// be read only by the copy, and v must be a local of t's type whose address
// is never taken, so each store converts the same way the copy did. Effects
// in J before the copy must not use v or t, so storing v on the incoming
// edges instead is not observed.
func sinkMergeCopy(f *Func, addressed map[string]bool) bool {
	for ji := range f.Blocks {
		join := f.Blocks[ji].Effects
		for ci, effect := range join {
			cp, ok := effect.(*Assign)
			if !ok {
				continue
			}
			temp, isTemp := cp.Src.(*Temp)
			local, isLocal := cp.Dst.(*Local)
			if !isTemp || !isLocal || addressed[local.Name] || !typeinfo.Equals(local.Type, temp.TypeInfo) {
				continue
			}
			uses := func(x Expr) bool { return sameExpr(x, local) || sameExpr(x, temp) }
			if slices.ContainsFunc(join[:ci], func(e Effect) bool { return effectRefersTo(e, uses) }) {
				continue
			}
			defs, ok := edgeDefinitions(f, f.Blocks[ji].ID, temp)
			total := 0
			for _, block := range f.Blocks {
				total += countTempRefs(block.Effects, temp)
			}
			if !ok || total != len(defs)+1 {
				continue
			}
			for _, d := range defs {
				effects := slices.Clone(f.Blocks[d.block].Effects)
				switch def := effects[d.index].(type) {
				case *Assign:
					if sameExpr(def.Src, local) {
						// v = v stores nothing.
						effects = slices.Delete(effects, d.index, d.index+1)
						break
					}
					next := *def
					next.Dst = local
					next.Merge = true
					effects[d.index] = &next
				case *CallEffect:
					next := *def
					next.Result = local
					effects[d.index] = &next
				}
				f.Blocks[d.block].Effects = effects
			}
			f.Blocks[ji].Effects = slices.Delete(slices.Clone(join), ci, ci+1)
			return true
		}
	}
	return false
}

// edgeDefinition locates an effect by block index and effect index.
type edgeDefinition struct {
	block, index int
}

// edgeDefinitions returns where each predecessor of join assigns temp as its
// last effect before reaching join, or false unless every predecessor does
// and leads nowhere else.
func edgeDefinitions(f *Func, join machine.BlockID, temp *Temp) ([]edgeDefinition, bool) {
	preds := f.CFG.Predecessors(join)
	if len(preds) == 0 {
		return nil, false
	}
	defs := make([]edgeDefinition, 0, len(preds))
	for _, pred := range preds {
		bi := slices.IndexFunc(f.Blocks, func(b Block) bool { return b.ID == pred })
		if pred == join || bi < 0 || !slices.Equal(f.CFG.Successors(pred), []machine.BlockID{join}) {
			return nil, false
		}
		effects := f.Blocks[bi].Effects
		last := len(effects) - 1
		if last >= 0 {
			if _, jump := effects[last].(*Jump); jump {
				last--
			}
		}
		if last < 0 {
			return nil, false
		}
		if t, _, ok := tempDefinition(effects[last]); !ok || !sameExpr(t, temp) {
			return nil, false
		}
		defs = append(defs, edgeDefinition{block: bi, index: last})
	}
	return defs, true
}

// tempDefinition returns the temp an effect assigns and the value assigned,
// for t = x and for t = f(...).
func tempDefinition(effect Effect) (*Temp, Expr, bool) {
	switch e := effect.(type) {
	case *Assign:
		temp, ok := e.Dst.(*Temp)
		return temp, e.Src, ok
	case *CallEffect:
		temp, ok := e.Result.(*Temp)
		return temp, e.Call, ok && e.Call != nil
	}
	return nil, nil, false
}

// countTempDefs counts the effects in f that assign temp.
func countTempDefs(f *Func, temp *Temp) int {
	count := 0
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			if t, _, ok := tempDefinition(effect); ok && sameExpr(t, temp) {
				count++
			}
		}
	}
	return count
}

// dropUnreadTemp removes the definition at effects[index] of a temp that is
// never read: a call keeps running without storing its result, and any
// other value, which has no side effects, is not computed at all.
func dropUnreadTemp(f *Func, effects []Effect, index int, temp *Temp, value Expr) ([]Effect, bool) {
	total := 0
	for _, block := range f.Blocks {
		total += countTempRefs(block.Effects, temp)
	}
	if total != 1 {
		return nil, false
	}
	if call, ok := effects[index].(*CallEffect); ok {
		next := *call
		next.Result = nil
		out := slices.Clone(effects)
		out[index] = &next
		return out, true
	}
	if hasSideEffects(value) {
		return nil, false
	}
	return slices.Delete(slices.Clone(effects), index, index+1), true
}

// forwardTempCopy rewrites t = x; v = t; ... t ... into v = x; ... v ...
// when v is a local of the temp's type whose address is never taken, every
// other read of the temp follows in the same block, and v is not assigned
// again before the last of them.
func forwardTempCopy(f *Func, effects []Effect, index int, temp *Temp, addressed map[string]bool) ([]Effect, bool) {
	if index+1 >= len(effects) {
		return nil, false
	}
	cp, ok := effects[index+1].(*Assign)
	if !ok || !sameExpr(cp.Src, temp) {
		return nil, false
	}
	local, ok := cp.Dst.(*Local)
	if !ok || addressed[local.Name] || !typeinfo.Equals(local.Type, temp.TypeInfo) {
		return nil, false
	}
	rest := effects[index+2:]
	last := -1
	for k, effect := range rest {
		if countTempRefs(rest[k:k+1], temp) > 0 {
			last = k
		}
		if assignsLocal(effect, local) && last < k {
			break
		}
	}
	total := 0
	for _, block := range f.Blocks {
		total += countTempRefs(block.Effects, temp)
	}
	// The definition, the copy, and the reads before v changes.
	if total != 2+countTempRefs(rest[:last+1], temp) {
		return nil, false
	}
	for _, effect := range rest[:last+1] {
		if assignsLocal(effect, local) {
			return nil, false
		}
	}

	rewriter := &semRewriter{
		expr: func(_ *semRewriter, expr Expr) (Expr, bool, bool) {
			if sameExpr(expr, temp) {
				return local, true, true
			}
			return nil, false, false
		},
	}
	out := slices.Clone(effects[:index])
	switch def := effects[index].(type) {
	case *Assign:
		next := *def
		next.Dst = local
		out = append(out, &next)
	case *CallEffect:
		next := *def
		next.Result = local
		out = append(out, &next)
	}
	for _, effect := range rest[:last+1] {
		next, _ := rewriter.rewriteEffect(effect)
		out = append(out, next)
	}
	return append(out, rest[last+1:]...), true
}

// assignsLocal reports whether effect stores to local.
func assignsLocal(effect Effect, local *Local) bool {
	switch e := effect.(type) {
	case *Assign:
		return sameExpr(e.Dst, local)
	case *CallEffect:
		return sameExpr(e.Result, local)
	}
	return false
}

// forwardTempUse replaces the single read of the temp defined at
// f.Blocks[bi].Effects[index], when it is in the next effect to run, with
// the assigned value and drops the definition.
func forwardTempUse(f *Func, bi, index int, temp *Temp, value Expr, addressed map[string]bool) bool {
	ui, uj, ok := nextEffect(f, bi, index)
	if !ok {
		return false
	}
	use := f.Blocks[ui].Effects[uj]
	if countTempRefs([]Effect{use}, temp) != 1 {
		return false
	}
	total := 0
	for _, block := range f.Blocks {
		total += countTempRefs(block.Effects, temp)
	}
	if total != 2 || !forwardableValue(value, temp.TypeInfo) || !forwardOrderSafe(value, use, temp, addressed) {
		return false
	}
	rewriter := &semRewriter{
		expr: func(_ *semRewriter, expr Expr) (Expr, bool, bool) {
			if sameExpr(expr, temp) {
				return value, true, true
			}
			return nil, false, false
		},
	}
	next, changed := rewriter.rewriteEffect(use)
	if !changed {
		return false
	}
	f.Blocks[ui].Effects = slices.Clone(f.Blocks[ui].Effects)
	f.Blocks[ui].Effects[uj] = next
	f.Blocks[bi].Effects = slices.Delete(slices.Clone(f.Blocks[bi].Effects), index, index+1)
	return true
}

// nextEffect locates the effect that always runs right after
// f.Blocks[bi].Effects[index]: the following effect in the block, or the
// first effect of the block's only successor when the block ends there,
// possibly with a jump, and nothing else reaches that successor.
func nextEffect(f *Func, bi, index int) (int, int, bool) {
	effects := f.Blocks[bi].Effects
	if index+1 < len(effects) {
		if _, jump := effects[index+1].(*Jump); !jump {
			return bi, index + 1, true
		}
		if index+2 != len(effects) {
			return 0, 0, false
		}
	}
	id := f.Blocks[bi].ID
	succs := f.CFG.Successors(id)
	if len(succs) != 1 || succs[0] == id || !slices.Equal(f.CFG.Predecessors(succs[0]), []machine.BlockID{id}) {
		return 0, 0, false
	}
	next := slices.IndexFunc(f.Blocks, func(b Block) bool { return b.ID == succs[0] })
	if next < 0 || len(f.Blocks[next].Effects) == 0 {
		return 0, 0, false
	}
	return next, 0, true
}

// forwardableValue reports whether storing value in a temp of type want and
// reading it back yields the value itself in every C expression: each
// possible result already fits want, and for types that do not promote to
// int, the C type of value is want's C type. A conditional is judged by its
// arms, whose C types decide the type of the whole expression.
func forwardableValue(value Expr, want typeinfo.Type) bool {
	arms := []Expr{value}
	if cond, ok := value.(*Cond); ok {
		arms = []Expr{cond.Then, cond.Else}
	}

	if isCPointer(want) {
		for _, arm := range arms {
			if !pointerArmFits(arm, want) {
				return false
			}
		}
		return true
	}

	wantMin, wantMax, ok := cIntRange(want)
	if !ok {
		// Floats, structs and the like: only an exact C type match.
		return len(arms) == 1 && forwardLeaf(value) && typeinfo.Equals(cExprType(value), want)
	}
	unsignedArm := false
	for _, arm := range arms {
		min, max, unsigned, ok := armIntRange(arm)
		if !ok || min < wantMin || max > wantMax {
			return false
		}
		unsignedArm = unsignedArm || unsigned
	}
	if want.Bytes() < 4 {
		// Both the temp and the value promote to int.
		return true
	}
	// A 32-bit temp keeps its signedness; the value must have the same one.
	return unsignedArm == !isSignedInt(want)
}

// pointerArmFits reports whether arm has the C pointer type want: a null
// constant, a string literal for a char pointer, or a leaf of that type.
func pointerArmFits(arm Expr, want typeinfo.Type) bool {
	switch a := arm.(type) {
	case *Const:
		return a.U64 == 0
	case *StringLiteral:
		ptr, ok := want.(*typeinfo.Pointer)
		return ok && ptr.IsCStringPointer()
	}
	return forwardLeaf(arm) && typeinfo.Equals(cExprType(arm), want)
}

// armIntRange returns the range of values arm can hold in C and whether its
// C type is a 32-bit unsigned int. A constant's range is its printed value;
// any other arm must be a leaf whose type is a fixed-width integer.
func armIntRange(arm Expr) (min, max int64, unsigned, ok bool) {
	if c, ok := arm.(*Const); ok {
		v := int64(c.U64)
		if i64, signed := c.Int64(); signed {
			v = i64
		}
		// A literal above INT_MAX has an unsigned or wider C type.
		return v, v, false, v <= math.MaxInt32
	}
	if !forwardLeaf(arm) {
		return 0, 0, false, false
	}
	typ := cExprType(arm)
	min, max, ok = cIntRange(typ)
	return min, max, ok && typ.Bytes() == 4 && !isSignedInt(typ), ok
}

// forwardLeaf reports whether expr's C type is its semantic type: a
// variable, field, element, dereference, cast, or call of a function whose
// generated prototype returns its semantic return type. Arithmetic is not a
// leaf, since C evaluates it in at least int whatever its semantic type.
func forwardLeaf(expr Expr) bool {
	switch e := expr.(type) {
	case *Local, *Global, *Temp, *FieldAccess, *ArrayIndex, *Deref, *Cast:
		return true
	case *Call:
		return e.Function != nil && !e.Function.Macro && !e.Function.IsOverride() && e.Function.NativeDecl == ""
	}
	return false
}

// cIntRange returns the range of the fixed-width C integer type typ is
// declared as, or false for any other type.
func cIntRange(typ typeinfo.Type) (min, max int64, ok bool) {
	if enum, isEnum := typ.(*typeinfo.Enum); isEnum {
		if enum.Typedef != nil && enum.String() == enum.Name {
			return cIntRange(enum.Typedef)
		}
		if enum.Storage == nil {
			return 0, 0, false
		}
		return cIntRange(enum.Storage)
	}
	p, isPrim := typ.(*typeinfo.Primitive)
	if !isPrim || p.TypeKind != typeinfo.KInt || p.Native != typeinfo.NativeInt || p.Size < 1 || p.Size > 4 {
		return 0, 0, false
	}
	bits := uint(p.Size * 8)
	if p.Signed {
		return -(1 << (bits - 1)), 1<<(bits-1) - 1, true
	}
	return 0, 1<<bits - 1, true
}

// isSignedInt reports whether typ is declared as a signed integer.
func isSignedInt(typ typeinfo.Type) bool {
	if enum, ok := typ.(*typeinfo.Enum); ok {
		if enum.Typedef != nil && enum.String() == enum.Name {
			return isSignedInt(enum.Typedef)
		}
		return enum.Storage != nil && isSignedInt(enum.Storage)
	}
	p, ok := typ.(*typeinfo.Primitive)
	return ok && p.Signed
}

// isCPointer reports whether typ is a C pointer: a pointer or a handle.
func isCPointer(typ typeinfo.Type) bool {
	return typeinfo.IsPointer(typ) || typeinfo.IsNative(typ, typeinfo.NativePointer)
}

// forwardOrderSafe reports whether moving value into use keeps the order of
// everything that can observe it. A value without calls only reads memory,
// so no call in use may run before it: every call there must take the temp
// among its arguments. A value with a call must still be evaluated exactly
// once, so not in a conditional arm, and nothing else use evaluates may
// read or write memory the call can change.
func forwardOrderSafe(value Expr, use Effect, temp *Temp, addressed map[string]bool) bool {
	isTemp := func(expr Expr) bool { return sameExpr(expr, temp) }
	if !hasSideEffects(value) {
		return callsEnclose(use, isTemp)
	}
	inArm := false
	walkEffect(use, func(expr Expr) {
		if cond, ok := expr.(*Cond); ok && (containsExpr(cond.Then, isTemp) || containsExpr(cond.Else, isTemp)) {
			inArm = true
		}
	})
	if inArm {
		return false
	}
	iso := callIsolation{temp: temp, addressed: addressed}
	switch e := use.(type) {
	case *Assign:
		return iso.address(e.Dst) && iso.value(e.Src)
	case *CallEffect:
		return (e.Result == nil || iso.address(e.Result)) && iso.call(e.Call)
	case *Branch:
		return iso.value(e.Cond)
	case *Return:
		return e.Value == nil || iso.value(e.Value)
	}
	return false
}

// hasSideEffects reports whether expr calls a function other than a macro,
// or increments or decrements a variable.
func hasSideEffects(expr Expr) bool {
	return containsExpr(expr, func(e Expr) bool {
		switch e := e.(type) {
		case *Call:
			return e.Function == nil || !e.Function.Macro
		case *Unary:
			return e.Op == OpPostInc || e.Op == OpPostDec
		}
		return false
	})
}

// containsExpr reports whether any expression within expr matches match.
func containsExpr(expr Expr, match func(Expr) bool) bool {
	found := false
	walkExpr(expr, func(e Expr) {
		found = found || match(e)
	})
	return found
}

// callIsolation decides whether the parts of a use a forwarded call is
// unsequenced with are unaffected by it: they read only constants, temps,
// and locals whose address is never taken, and call nothing except calls
// that take the forwarded temp as an argument and so run after it.
type callIsolation struct {
	temp      *Temp
	addressed map[string]bool
}

// value reports whether evaluating expr is isolated from the call.
func (c callIsolation) value(expr Expr) bool {
	switch e := expr.(type) {
	case nil, *Const, *StringLiteral, *FloatConst, *SizeOf, *FunctionRef:
		return true
	case *Temp:
		return true
	case *Local:
		return !c.addressed[e.Name]
	case *Unary:
		return c.value(e.X)
	case *Binary:
		return c.value(e.LHS) && c.value(e.RHS)
	case *Compare:
		return c.value(e.LHS) && c.value(e.RHS)
	case *Cast:
		return c.value(e.Value)
	case *Word:
		return c.value(e.Parent)
	case *SignExtend:
		return c.value(e.Parent)
	case *Cond:
		return c.value(e.Cond) && c.value(e.Then) && c.value(e.Else)
	case *AddressOf:
		return c.address(e.Target)
	case *Call:
		if e.Function != nil && e.Function.Macro {
			return c.call(e)
		}
		return containsExpr(e, func(x Expr) bool { return sameExpr(x, c.temp) }) && c.call(e)
	case *FieldAccess, *ArrayIndex, *Deref:
		return c.storage(e)
	}
	return false
}

// storage reports whether reading the storage expr names is isolated: it
// belongs to a local whose address is never taken, or it is reached from the
// forwarded temp, so it can only be read once the call has returned.
func (c callIsolation) storage(expr Expr) bool {
	if c.derived(expr) {
		return true
	}
	switch e := expr.(type) {
	case *Local:
		return !c.addressed[e.Name]
	case *FieldAccess:
		return !isCPointer(e.Base.ExprType()) && c.storage(e.Base)
	case *ArrayIndex:
		_, array := e.Base.ExprType().(*typeinfo.Array)
		return array && c.value(e.Index) && c.storage(e.Base)
	}
	return false
}

// derived reports whether expr is the forwarded temp or storage reached from
// it through fields, elements with isolated indexes, and dereferences.
func (c callIsolation) derived(expr Expr) bool {
	switch e := expr.(type) {
	case *Temp:
		return sameExpr(e, c.temp)
	case *Cast:
		return c.derived(e.Value)
	case *FieldAccess:
		return c.derived(e.Base)
	case *ArrayIndex:
		return c.value(e.Index) && c.derived(e.Base)
	case *Deref:
		return c.derived(e.Pointer)
	}
	return false
}

// call reports whether a call's target and arguments are isolated.
func (c callIsolation) call(call *Call) bool {
	if !c.value(call.Target) {
		return false
	}
	for _, arg := range call.Args {
		if !c.value(arg) {
			return false
		}
	}
	return true
}

// address reports whether computing the address of lvalue is isolated: it
// is a variable, or it is reached from one through array elements with
// isolated indexes and through pointers that are isolated values.
func (c callIsolation) address(lvalue Expr) bool {
	switch e := lvalue.(type) {
	case *Local, *Global, *Temp:
		return true
	case *FieldAccess:
		if isCPointer(e.Base.ExprType()) {
			return c.value(e.Base)
		}
		return c.address(e.Base)
	case *ArrayIndex:
		if !c.value(e.Index) {
			return false
		}
		if _, array := e.Base.ExprType().(*typeinfo.Array); array {
			return c.address(e.Base)
		}
		return c.value(e.Base)
	case *Deref:
		return c.value(e.Pointer)
	}
	return false
}

// addressTakenLocals returns the names of locals whose address f takes, which
// a call can then change.
func addressTakenLocals(f *Func) map[string]bool {
	out := map[string]bool{}
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			walkEffect(effect, func(expr Expr) {
				if addr, ok := expr.(*AddressOf); ok {
					if local := rootLocal(addr.Target); local != nil {
						out[local.Name] = true
					}
				}
			})
		}
	}
	return out
}

// rootLocal returns the local an lvalue's storage belongs to, or nil when
// it is not part of a local.
func rootLocal(expr Expr) *Local {
	switch e := expr.(type) {
	case *Local:
		return e
	case *FieldAccess:
		if isCPointer(e.Base.ExprType()) {
			return nil
		}
		return rootLocal(e.Base)
	case *ArrayIndex:
		if _, array := e.Base.ExprType().(*typeinfo.Array); array {
			return rootLocal(e.Base)
		}
	}
	return nil
}
