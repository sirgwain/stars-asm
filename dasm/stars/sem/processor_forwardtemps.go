package sem

import (
	"math"
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type forwardTempsProcessor struct {
	// fs is the function being lowered.
	fs *typeinfo.Function
	// writes returns what a call of a function may store to.
	writes func(*typeinfo.Function) machine.Writes
}

// ProcessFunc removes temps that only relay a value to the next effect:
//
//	t = x; use(t)          becomes use(x)
//	t = x; v = t; use(t)   becomes v = x; use(v)
//
// Each rewrite must keep the program's behavior, not only its value in
// the common case: see ForwardableValue for the conversion the temp
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
			if forwardTempUse(p.fs, p.writes, f, bi, i, temp, value, addressed) {
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

// maxForwardDistance is the most effects a temp's value may move past to
// reach its use.
const maxForwardDistance = 8

// forwardTempUse replaces the single read of the temp defined at
// f.Blocks[bi].Effects[index], when it is in one of the next effects to
// run, with the assigned value and drops the definition. The value then
// runs after the effects between, which must allow it; see forwardsPast.
// fs is the function being lowered, whose return type converts a returned
// temp, and writes returns what a call may store to.
func forwardTempUse(fs *typeinfo.Function, writes func(*typeinfo.Function) machine.Writes, f *Func, bi, index int, temp *Temp, value Expr, addressed map[string]bool) bool {
	total := 0
	for _, block := range f.Blocks {
		total += countTempRefs(block.Effects, temp)
	}
	if total != 2 {
		return false
	}
	var between []Effect
	ui, uj := bi, index
	for {
		var ok bool
		if ui, uj, ok = nextEffect(f, ui, uj); !ok || len(between) > maxForwardDistance {
			return false
		}
		if countTempRefs(f.Blocks[ui].Effects[uj:uj+1], temp) > 0 {
			break
		}
		between = append(between, f.Blocks[ui].Effects[uj])
	}
	use := f.Blocks[ui].Effects[uj]
	if !forwardsPast(value, between, addressed, writes) {
		return false
	}
	if !ForwardableValue(value, temp.TypeInfo) && !convertedOnUse(fs, value, use, temp) || !forwardOrderSafe(value, use, temp, addressed, writes) {
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

// forwardsPast reports whether value can be evaluated after the effects
// between instead of before them: none of them stores to a variable or
// memory value reads, and the calls in value store to nothing they read or
// store to.
func forwardsPast(value Expr, between []Effect, addressed map[string]bool, writes func(*typeinfo.Function) machine.Writes) bool {
	v := evalAccesses{addressed: addressed, writes: writes}
	v.add(value, nil)
	reads := map[string]bool{}
	walkExpr(value, func(e Expr) {
		switch e := e.(type) {
		case *Local:
			reads[e.Name] = true
		case *Temp:
			reads[e.Name] = true
		}
	})
	for _, effect := range between {
		e := evalAccesses{addressed: addressed, writes: writes}
		var dst Expr
		switch x := effect.(type) {
		case *Assign:
			e.addAddress(x.Dst, nil)
			e.add(x.Src, nil)
			dst = x.Dst
		case *CallEffect:
			e.add(x.Call, nil)
			if x.Result != nil {
				e.addAddress(x.Result, nil)
				dst = x.Result
			}
		default:
			return false
		}
		switch root := storageRoot(dst).(type) {
		case nil:
			if temp, ok := dst.(*Temp); ok {
				if reads[temp.Name] {
					return false
				}
			} else if dst != nil {
				// A store through a pointer, which counts as reading the
				// memory too, so a call in value storing there conflicts.
				e.stores.Any = true
				e.pointers = true
			}
		case *Local:
			if reads[root.Name] {
				return false
			}
			if addressed[root.Name] {
				e.stores.Any = true
				e.addressedLocal = true
			}
		case *Global:
			e.stores.Globals = append(e.stores.Globals, root.GlobalVar)
			e.globals = append(e.globals, root.GlobalVar)
		}
		if v.observes(e.stores) || e.observes(v.stores) {
			return false
		}
	}
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

// ForwardableValue reports whether storing value in a temp of type want and
// reading it back yields the value itself in every C expression: each
// possible result already fits want, and for types that do not promote to
// int, the C type of value is want's C type. A conditional is judged by its
// arms, whose C types decide the type of the whole expression.
func ForwardableValue(value Expr, want typeinfo.Type) bool {
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
		return !unsignedArm
	}
	// A 32-bit temp keeps its signedness; the value must have the same one.
	return unsignedArm == !isSignedInt(want)
}

// convertedOnUse reports whether the temp's one read in use converts it at
// once to an integer type no wider than the temp: the whole source of an
// assignment, a prototyped call argument, the returned value, or a cast.
// Converting an integer to a narrower or equal width keeps only its low
// bits, so converting value through the temp first changes nothing, even
// when value does not fit the temp. fs is the function being lowered.
func convertedOnUse(fs *typeinfo.Function, value Expr, use Effect, temp *Temp) bool {
	tempInt, ok := cIntType(temp.TypeInfo)
	if !ok || !integerValue(value) {
		return false
	}
	target, ok := tempConversionTarget(fs, use, temp)
	if !ok {
		return false
	}
	targetInt, ok := cIntType(target)
	return ok && targetInt.Size <= tempInt.Size
}

// tempConversionTarget returns the type C converts the temp's one read in
// use to, when the read is the whole operand of that conversion: the source
// of an assignment or of a stored call result, an argument matched to a
// declared parameter, the returned value, or the operand of a cast.
func tempConversionTarget(fs *typeinfo.Function, use Effect, temp *Temp) (typeinfo.Type, bool) {
	isTemp := func(expr Expr) bool { return sameExpr(expr, temp) }
	switch e := use.(type) {
	case *Assign:
		if isTemp(e.Src) {
			return cExprType(e.Dst), true
		}
	case *Return:
		if isTemp(e.Value) && fs.NativeDecl == "" {
			return fs.Ret, true
		}
	}
	var target typeinfo.Type
	visit := func(expr Expr) {
		switch e := expr.(type) {
		case *Cast:
			if isTemp(e.Value) {
				target = e.TypeInfo
			}
		case *Call:
			for i, arg := range e.Args {
				if isTemp(arg) {
					target = declaredParamType(e, i)
				}
			}
		}
	}
	if call, ok := use.(*CallEffect); ok {
		// The walk visits the arguments of the effect's own call, not the call.
		visit(call.Call)
	}
	walkEffect(use, visit)
	return target, target != nil
}

// declaredParamType returns the type the C prototype of call declares for
// argument i, or nil when the prototype the native compile uses may differ:
// a variadic argument, an unprototyped function, a native declaration, or a
// Win32 or runtime function whose parameter is spelled as a fixed-width
// integer standing in for the header's own type.
func declaredParamType(call *Call, i int) typeinfo.Type {
	fn := call.Function
	if fn == nil || fn.Macro || fn.NativeDecl != "" || len(fn.CallParams) != 0 || i >= len(fn.Params) {
		return nil
	}
	typ := fn.Params[i].Type
	if fn.IsOverride() && fixedWidthInt(typ) {
		return nil
	}
	return typ
}

// fixedWidthInt reports whether typ is spelled as a C fixed-width integer,
// such as int16_t, rather than a named type such as HWND or COLORREF.
func fixedWidthInt(typ typeinfo.Type) bool {
	p, ok := typ.(*typeinfo.Primitive)
	if !ok || p.TypeKind != typeinfo.KInt {
		return false
	}
	switch p.Name {
	case "int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t", "uint32_t":
		return true
	}
	return false
}

// integerValue reports whether value has an integer C type: its semantic
// type is an integer, or for a conditional, both arms are.
func integerValue(value Expr) bool {
	if cond, ok := value.(*Cond); ok {
		return integerValue(cond.Then) && integerValue(cond.Else)
	}
	if _, ok := value.(*Const); ok {
		return true
	}
	_, _, ok := cIntRange(cExprType(value))
	return ok
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
// a leaf's is its type's, or its width's for a bitfield; a word or byte of
// a value, or a value sign-extended from a width, holds that width; and
// arithmetic on such values holds what its operands' ranges allow, in the
// int or unsigned int C evaluates it in.
func armIntRange(arm Expr) (lo, hi int64, unsigned, ok bool) {
	switch e := arm.(type) {
	case *Const:
		v := int64(e.U64)
		if i64, signed := e.Int64(); signed {
			v = i64
		}
		// A literal above INT_MAX has an unsigned or wider C type.
		return v, v, false, v <= math.MaxInt32
	case *FieldAccess:
		if bf := e.Field.Bitfield; bf != nil && bf.BitWidth < 32 {
			// A bitfield narrower than int promotes to int.
			bits := uint(bf.BitWidth)
			if bf.Signed() {
				return -(1 << (bits - 1)), 1<<(bits-1) - 1, false, true
			}
			return 0, 1<<bits - 1, false, true
		}
	case *Word:
		// LOWORD and HIWORD are WORDs, which promote to int.
		return 0, math.MaxUint16, false, e.Part == machine.WordLow || e.Part == machine.WordHigh
	case *Byte:
		// LOBYTE and HIBYTE are BYTEs; a byte replaced in a value is not.
		return 0, math.MaxUint8, false, e.Value == nil
	case *SignExtend:
		// The value is read as a signed integer of its source width, then
		// widened to a signed int or promoted to one.
		bits := uint(e.FromBits)
		return -(1 << (bits - 1)), 1<<(bits-1) - 1, false, (bits == 8 || bits == 16) && isSignedInt(e.TypeInfo) && e.TypeInfo.Bytes() <= 4
	case *Cond:
		thenMin, thenMax, thenUnsigned, thenOK := armIntRange(e.Then)
		elseMin, elseMax, elseUnsigned, elseOK := armIntRange(e.Else)
		return min(thenMin, elseMin), max(thenMax, elseMax), thenUnsigned || elseUnsigned, thenOK && elseOK
	case *Compare:
		return 0, 1, false, true
	case *Binary:
		return binaryIntRange(e)
	}
	if !forwardLeaf(arm) {
		return 0, 0, false, false
	}
	typ := cExprType(arm)
	lo, hi, ok = cIntRange(typ)
	return lo, hi, ok && typ.Bytes() == 4 && !isSignedInt(typ), ok
}

// binaryIntRange returns the range of values integer arithmetic e can hold
// in C and whether C evaluates it in unsigned int, as armIntRange does. An
// operation whose range the operands do not bound holds any value of that
// C type.
func binaryIntRange(e *Binary) (lo, hi int64, unsigned, ok bool) {
	lMin, lMax, lUnsigned, ok := armIntRange(e.LHS)
	if !ok {
		return 0, 0, false, false
	}
	rMin, rMax, rUnsigned, ok := armIntRange(e.RHS)
	if !ok {
		return 0, 0, false, false
	}
	unsigned = lUnsigned || rUnsigned
	switch e.Op {
	case OpShl, OpShr, OpSar:
		// A shift has the type of its promoted left operand.
		unsigned = lUnsigned
	}
	full := func() (int64, int64, bool, bool) {
		if unsigned {
			return 0, math.MaxUint32, true, true
		}
		return math.MinInt32, math.MaxInt32, false, true
	}
	switch e.Op {
	case OpAnd:
		// Masking with a nonnegative value keeps at most its bits.
		switch {
		case lMin >= 0 && rMin >= 0:
			return 0, min(lMax, rMax), unsigned, true
		case lMin >= 0:
			return 0, lMax, unsigned, true
		case rMin >= 0:
			return 0, rMax, unsigned, true
		}
	case OpShl:
		if lMin >= 0 && rMin >= 0 && rMax < 31 && lMax<<rMax <= math.MaxInt32 {
			return lMin << rMin, lMax << rMax, unsigned, true
		}
	case OpShr, OpSar:
		if lMin >= 0 && rMin >= 0 && rMax < 32 {
			return lMin >> rMax, lMax >> rMin, unsigned, true
		}
	case OpAdd, OpSub, OpMul:
		if unsigned && e.Op == OpMul {
			// The product of two unsigned ranges may not fit an int64.
			break
		}
		var candidates []int64
		switch e.Op {
		case OpAdd:
			candidates = []int64{lMin + rMin, lMax + rMax}
		case OpSub:
			candidates = []int64{lMin - rMax, lMax - rMin}
		case OpMul:
			candidates = []int64{lMin * rMin, lMin * rMax, lMax * rMin, lMax * rMax}
		}
		lo, hi = slices.Min(candidates), slices.Max(candidates)
		fullMin, fullMax, _, _ := full()
		if lo >= fullMin && hi <= fullMax {
			return lo, hi, unsigned, true
		}
	}
	return full()
}

// forwardLeaf reports whether expr's C type is its semantic type: a
// variable, field, element, dereference, cast, or call of a function whose
// C prototype returns its semantic return type. That is a generated
// prototype, or a Win32 or runtime function or a message cracker macro
// whose return type is named as the headers name it, such as HWND, rather
// than a fixed-width integer standing in for the header's int. Arithmetic
// is not a leaf, since C evaluates it in at least int whatever its semantic
// type.
func forwardLeaf(expr Expr) bool {
	switch e := expr.(type) {
	case *Local, *Global, *Temp, *FieldAccess, *ArrayIndex, *Deref, *Cast:
		return true
	case *Call:
		fn := e.Function
		return fn != nil && fn.NativeDecl == "" && (!fn.IsOverride() && !fn.Macro || !fixedWidthInt(fn.Ret))
	}
	return false
}

// cIntRange returns the range of the fixed-width C integer type typ is
// declared as, or false for any other type.
func cIntRange(typ typeinfo.Type) (min, max int64, ok bool) {
	p, ok := cIntType(typ)
	if !ok {
		return 0, 0, false
	}
	bits := uint(p.Size * 8)
	if p.Signed {
		return -(1 << (bits - 1)), 1<<(bits-1) - 1, true
	}
	return 0, 1<<bits - 1, true
}

// cIntType returns the fixed-width C integer type typ is declared as: the
// type itself, or for an enum, the typedef its name is declared as or the
// storage its use is declared with. It is false for any other type.
func cIntType(typ typeinfo.Type) (*typeinfo.Primitive, bool) {
	if enum, isEnum := typ.(*typeinfo.Enum); isEnum {
		if enum.Typedef != nil && enum.String() == enum.Name {
			return cIntType(enum.Typedef)
		}
		if enum.Storage == nil {
			return nil, false
		}
		return cIntType(enum.Storage)
	}
	p, isPrim := typ.(*typeinfo.Primitive)
	if !isPrim || p.TypeKind != typeinfo.KInt || p.Native != typeinfo.NativeInt || p.Size < 1 || p.Size > 4 {
		return nil, false
	}
	return p, true
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
// among its arguments, unless the value reads nothing a call can change. A
// value with a call must still be evaluated exactly once, so not in a
// conditional arm, and nothing else use evaluates may read or write memory
// the call can change. Either way, what the calls store to, as writes
// reports, may show that the value and the rest of use cannot observe each
// other; see independentOrder.
func forwardOrderSafe(value Expr, use Effect, temp *Temp, addressed map[string]bool, writes func(*typeinfo.Function) machine.Writes) bool {
	isTemp := func(expr Expr) bool { return sameExpr(expr, temp) }
	iso := callIsolation{temp: temp, addressed: addressed}
	if !hasSideEffects(value) {
		return iso.value(value) || callsEnclose(use, isTemp) || independentOrder(value, use, temp, addressed, writes)
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
	isolated := false
	switch e := use.(type) {
	case *Assign:
		isolated = iso.address(e.Dst) && iso.value(e.Src)
	case *CallEffect:
		isolated = (e.Result == nil || iso.address(e.Result)) && iso.call(e.Call)
	case *Branch:
		isolated = iso.value(e.Cond)
	case *Return:
		isolated = e.Value == nil || iso.value(e.Value)
	case *TableJump:
		isolated = iso.value(e.Index)
	}
	return isolated || independentOrder(value, use, temp, addressed, writes)
}

// independentOrder reports whether value and the rest of use, which C may
// evaluate in either order once value replaces the temp, cannot observe
// each other: neither calls a function that may store to memory the other
// reads, and a call reads anything, so at most one of them calls a
// function that stores at all. A call in use that takes the temp among its
// arguments runs after value either way, so only its other arguments count.
// The store use makes runs after both, as it did.
func independentOrder(value Expr, use Effect, temp *Temp, addressed map[string]bool, writes func(*typeinfo.Function) machine.Writes) bool {
	a := evalAccesses{addressed: addressed, writes: writes}
	a.add(value, nil)
	b := evalAccesses{addressed: addressed, writes: writes}
	takesTemp := func(call *Call) bool {
		found := false
		walkCall(call, func(x Expr) {
			found = found || sameExpr(x, temp)
		})
		return found
	}
	switch e := use.(type) {
	case *Assign:
		b.addAddress(e.Dst, takesTemp)
		b.add(e.Src, takesTemp)
	case *CallEffect:
		if e.Result != nil {
			b.addAddress(e.Result, takesTemp)
		}
		b.add(e.Call, takesTemp)
	case *Branch:
		b.add(e.Cond, takesTemp)
	case *Return:
		b.add(e.Value, takesTemp)
	case *TableJump:
		b.add(e.Index, takesTemp)
	default:
		return false
	}
	return !a.observes(b.stores) && !b.observes(a.stores)
}

// evalAccesses collects what evaluating expressions reads, and what the
// functions they call may store to.
type evalAccesses struct {
	addressed map[string]bool
	writes    func(*typeinfo.Function) machine.Writes

	// stores is what the calls may store to.
	stores machine.Writes
	// calls is set when a function is called, which may read any memory.
	calls bool
	// globals are the globals read.
	globals []*typeinfo.GlobalVar
	// addressedLocal is set when a local whose address is taken is read.
	addressedLocal bool
	// pointers is set when memory is read through a pointer.
	pointers bool
}

// add collects the evaluation of expr. A call skip reports true for runs
// after the forwarded value either way, so only its arguments are collected.
func (a *evalAccesses) add(expr Expr, skip func(*Call) bool) {
	walkExpr(expr, func(e Expr) {
		switch e := e.(type) {
		case *Call:
			if e.Function != nil && e.Function.Macro || skip != nil && skip(e) {
				return
			}
			a.calls = true
			if _, direct := e.Target.(*FunctionRef); !direct || e.Function == nil {
				a.stores.Any = true
				return
			}
			a.stores.Union(a.writes(e.Function))
		case *Unary:
			if e.Op == OpPostInc || e.Op == OpPostDec {
				a.calls = true
				a.stores.Any = true
			}
		case *Global:
			a.globals = append(a.globals, e.GlobalVar)
		case *Local:
			a.addressedLocal = a.addressedLocal || a.addressed[e.Name]
		case *Deref, *Memory:
			a.pointers = true
		case *FieldAccess:
			a.pointers = a.pointers || isCPointer(e.Base.ExprType())
		case *ArrayIndex:
			_, array := e.Base.ExprType().(*typeinfo.Array)
			a.pointers = a.pointers || !array
		}
	})
}

// addAddress collects computing the address of lvalue, without reading the
// storage it names.
func (a *evalAccesses) addAddress(lvalue Expr, skip func(*Call) bool) {
	switch e := lvalue.(type) {
	case *Local, *Global, *Temp:
	case *FieldAccess:
		if isCPointer(e.Base.ExprType()) {
			a.add(e.Base, skip)
			return
		}
		a.addAddress(e.Base, skip)
	case *ArrayIndex:
		a.add(e.Index, skip)
		if _, array := e.Base.ExprType().(*typeinfo.Array); array {
			a.addAddress(e.Base, skip)
			return
		}
		a.add(e.Base, skip)
	case *Deref:
		a.add(e.Pointer, skip)
	default:
		a.add(lvalue, skip)
	}
}

// observes reports whether stores could change what a reads, or, for the
// storage a counts as read, what it stores to.
func (a evalAccesses) observes(stores machine.Writes) bool {
	switch {
	case stores.None():
		return false
	case a.calls || a.pointers:
		return true
	case stores.Any:
		return a.addressedLocal || len(a.globals) > 0
	}
	for _, g := range a.globals {
		if slices.Contains(stores.Globals, g) {
			return true
		}
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
					if local, ok := storageRoot(addr.Target).(*Local); ok {
						out[local.Name] = true
					}
				}
			})
		}
	}
	return out
}

// storageRoot returns the variable, a *Local or a *Global, an lvalue's
// storage belongs to through fields and array elements, or nil when it is
// reached through a pointer.
func storageRoot(expr Expr) Expr {
	switch e := expr.(type) {
	case *Local, *Global:
		return e
	case *FieldAccess:
		if isCPointer(e.Base.ExprType()) {
			return nil
		}
		return storageRoot(e.Base)
	case *ArrayIndex:
		if _, array := e.Base.ExprType().(*typeinfo.Array); array {
			return storageRoot(e.Base)
		}
	}
	return nil
}
