package sem

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type postIncrementsProcessor struct{}

// ProcessFunc folds a temp that saves a variable before the variable is
// stepped by one back into the temp's single use as a postfix increment or
// decrement:
//
//	t = x; x = x + 1; ...; use(t)   becomes   ...; use(x++)
//
// The increment moves down to the use, so nothing between the increment
// and the use, nor the rest of the use, may read or write x: C leaves a
// read of x unsequenced with x++ in the same expression. The use must also
// always evaluate the temp, so it cannot sit in a conditional arm. x is a
// local whose address is never taken, or a global, field or element stored
// in a variable, whose use must then follow the increment directly, not
// name that variable again, and call nothing before the step.
func (p *postIncrementsProcessor) ProcessFunc(_ *Result, f *Func) bool {
	addressed := addressTakenLocals(f)
	changed := false
	for bi := range f.Blocks {
		for i := 0; i+2 < len(f.Blocks[bi].Effects); i++ {
			if next, ok := foldPostIncrement(f, f.Blocks[bi].Effects, i, addressed); ok {
				f.Blocks[bi].Effects = next
				changed = true
				// The next save may now start at i.
				i--
			}
		}
	}
	return changed
}

// foldPostIncrement folds the save at effects[index] and the step after it
// into the temp's use, or reports false when the pattern or its conditions
// do not hold.
func foldPostIncrement(f *Func, effects []Effect, index int, addressed map[string]bool) ([]Effect, bool) {
	save, ok := effects[index].(*Assign)
	if !ok {
		return nil, false
	}
	temp, ok := save.Dst.(*Temp)
	if !ok {
		return nil, false
	}
	target := save.Src
	root := storageRoot(target)
	local, isLocal := target.(*Local)
	switch {
	case isLocal:
		if addressed[local.Name] {
			return nil, false
		}
	case root == nil || hasSideEffects(target) || stepIndexesName(target, root):
		return nil, false
	}
	if !typeinfo.Equals(temp.TypeInfo, cExprType(target)) {
		return nil, false
	}
	op, ok := stepByOne(effects[index+1], target)
	if !ok {
		return nil, false
	}

	isTemp := func(expr Expr) bool { return sameExpr(expr, temp) }
	isTarget := func(expr Expr) bool { return sameExpr(expr, target) }
	if !isLocal {
		// Only the variable itself is known not to overlap other storage.
		isTarget = func(expr Expr) bool { return sameExpr(expr, root) }
	}
	use := -1
	for k := index + 2; k < len(effects); k++ {
		if countTempRefs(effects[k:k+1], temp) > 0 {
			use = k
			break
		}
		if !isLocal || effectRefersTo(effects[k], isTarget) {
			return nil, false
		}
	}
	if use < 0 || effectRefersTo(effects[use], isTarget) || tempInConditionalArm(effects[use], isTemp) {
		return nil, false
	}
	// A call can read or write a variable other than a local, so each call
	// in the use must run after the step, taking it among its arguments.
	if !isLocal && !callsEnclose(effects[use], isTemp) {
		return nil, false
	}
	total := 0
	for _, block := range f.Blocks {
		total += countTempRefs(block.Effects, temp)
	}
	// The save and the one read.
	if total != 2 || countTempRefs(effects[use:use+1], temp) != 1 {
		return nil, false
	}

	step := &Unary{TypeInfo: cExprType(target), Op: op, X: target}
	rewriter := &semRewriter{
		expr: func(_ *semRewriter, expr Expr) (Expr, bool, bool) {
			if isTemp(expr) {
				return step, true, true
			}
			return nil, false, false
		},
	}
	rewritten, changed := rewriter.rewriteEffect(effects[use])
	if !changed {
		return nil, false
	}
	out := slices.Clone(effects[:index])
	out = append(out, effects[index+2:use]...)
	out = append(out, rewritten)
	return append(out, effects[use+1:]...), true
}

// stepIndexesName reports whether an index within the lvalue target names
// root, the variable target's storage belongs to, so that stepping target
// could change which element it names.
func stepIndexesName(target, root Expr) bool {
	found := false
	walkExpr(target, func(expr Expr) {
		if index, ok := expr.(*ArrayIndex); ok && containsExpr(index.Index, func(e Expr) bool { return sameExpr(e, root) }) {
			found = true
		}
	})
	return found
}

// stepByOne returns the postfix operator of an effect that adds or subtracts
// one from target: target = target + 1, target = target - 1, or, for a
// pointer, target = &target[1], which C defines as target + 1.
func stepByOne(effect Effect, target Expr) (Op, bool) {
	assign, ok := effect.(*Assign)
	if !ok || !sameExpr(assign.Dst, target) {
		return OpUnknown, false
	}
	typ := cExprType(target)
	switch src := assign.Src.(type) {
	case *Binary:
		// An integer only: the semantic sum of a pointer may count bytes.
		if _, _, ok := cIntRange(typ); !ok || !sameExpr(src.LHS, target) || !isConstOne(src.RHS) {
			return OpUnknown, false
		}
		switch src.Op {
		case OpAdd:
			return OpPostInc, true
		case OpSub:
			return OpPostDec, true
		}
	case *AddressOf:
		index, ok := src.Target.(*ArrayIndex)
		if ok && typeinfo.IsPointer(typ) && sameExpr(index.Base, target) && isConstOne(index.Index) {
			return OpPostInc, true
		}
	}
	return OpUnknown, false
}

// isConstOne reports whether expr is the integer constant 1.
func isConstOne(expr Expr) bool {
	c, ok := expr.(*Const)
	return ok && c.U64 == 1
}

// effectRefersTo reports whether any expression in effect matches match.
func effectRefersTo(effect Effect, match func(Expr) bool) bool {
	found := false
	walkEffect(effect, func(expr Expr) {
		found = found || match(expr)
	})
	return found
}

// tempInConditionalArm reports whether a match sits in an arm of a
// conditional expression in effect, where it is not always evaluated.
func tempInConditionalArm(effect Effect, match func(Expr) bool) bool {
	found := false
	walkEffect(effect, func(expr Expr) {
		if cond, ok := expr.(*Cond); ok && (containsExpr(cond.Then, match) || containsExpr(cond.Else, match)) {
			found = true
		}
	})
	return found
}
