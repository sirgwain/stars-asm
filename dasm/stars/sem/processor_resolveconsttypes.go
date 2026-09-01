package sem

import (
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// resolveConstTypesProcessor assigns semantic types to integer constants once
// expression recovery has established the source-level types around them.
type resolveConstTypesProcessor struct{}

func (p *resolveConstTypesProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	changed := false
	effects := make([]Effect, len(b.Effects))

	for i, effect := range b.Effects {
		next, effectChanged := resolveConstTypesEffect(effect)
		effects[i] = next
		changed = changed || effectChanged
	}

	if !changed {
		return b, false
	}

	b.Effects = effects
	return b, true
}

func resolveConstTypesEffect(effect Effect) (Effect, bool) {
	switch e := effect.(type) {
	case *Assign:
		dst, dstChanged := resolveConstTypesLValue(e.Dst, false)
		src, srcChanged := resolveConstTypesExpr(e.Src, dst.ExprType(), false)
		if !dstChanged && !srcChanged {
			return effect, false
		}

		next := *e
		next.Dst = dst
		next.Src = src
		return &next, true

	default:
		// Rewrite all contained expressions without an outer expected type.
		//
		// This handles branches, call args, returns, etc. Peer expressions
		// inside those trees still provide enough information to type their
		// constants.
		rewriter := &semRewriter{
			expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
				next, changed := w.rewriteExprChildren(expr)

				resolved, resolvedChanged := resolveConstTypesExpr(
					next,
					nil,
					false,
				)
				return resolved, changed || resolvedChanged, true
			},
		}

		return rewriter.rewriteEffect(effect)
	}
}

// resolveConstTypesLValue rewrites constants contained inside assignable
// storage expressions.
func resolveConstTypesLValue(value LValue, bitwise bool) (LValue, bool) {
	switch v := value.(type) {
	case *ArrayIndex:
		base, baseChanged := resolveConstTypesExpr(v.Base, nil, bitwise)
		index, indexChanged := resolveConstTypesExpr(v.Index, arrayIndexConstType(), bitwise)
		if !baseChanged && !indexChanged {
			return value, false
		}

		next := *v
		next.Base = base
		next.Index = index
		return &next, true

	default:
		rewriter := &semRewriter{
			expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
				next, changed := resolveConstTypesExpr(expr, nil, bitwise)
				return next, changed, true
			},
		}
		return rewriter.rewriteLValue(value)
	}
}

// resolveConstTypesExpr rewrites constants using semantic context.
//
// expected is the type imposed by the enclosing construct, such as the
// destination type of an assignment.
//
// bitwise is true when the expression occurs under a bitwise operation. In
// that context constants retain their original unsigned machine type.
func resolveConstTypesExpr(expr Expr, expected typeinfo.Type, bitwise bool) (Expr, bool) {
	if expr == nil {
		return nil, false
	}
	expected = constTypeExpected(expected)

	switch e := expr.(type) {
	case *Const:
		if bitwise || expected == nil || sameConstType(e.TypeInfo, expected) {
			return expr, false
		}

		next := *e
		next.TypeInfo = expected
		return &next, true

	case *Cast:
		// A widening/promotion cast should not hide the semantic type of the
		// value beneath it. The cast itself remains unchanged.
		value, changed := resolveConstTypesExpr(e.Value, nil, bitwise)
		if !changed {
			return expr, false
		}

		next := *e
		next.Value = value
		return &next, true

	case *Binary:
		return resolveConstTypesBinary(e, expected, bitwise)
	case *Compare:
		return resolveConstTypesCompare(e, expected)
	case *Call:
		changed := false
		for i, arg := range e.Args {
			if value, argChanged := resolveConstTypesExpr(arg, arg.ExprType(), bitwise); argChanged {
				e.Args[i] = value
				changed = true
			}
		}
		return e, changed
	case *ArrayIndex:
		base, baseChanged := resolveConstTypesExpr(e.Base, nil, bitwise)
		index, indexChanged := resolveConstTypesExpr(e.Index, arrayIndexConstType(), bitwise)
		if !baseChanged && !indexChanged {
			return expr, false
		}

		next := *e
		next.Base = base
		next.Index = index
		return &next, true
	case *Unary:
		x, changed := resolveConstTypesExpr(e.X, expected, bitwise)
		if !changed {
			return expr, false
		}

		next := *e
		next.X = x
		return &next, true

	default:
		return resolveConstTypesChildren(expr, bitwise)
	}
}

// constTypeExpected returns the semantic type that can be safely applied to an
// integer constant by this processor.
func constTypeExpected(expected typeinfo.Type) typeinfo.Type {
	if !typeinfo.IsIntLike(expected) {
		return nil
	}
	return expected
}

// arrayIndexConstType returns the source integer type for literal indexes.
func arrayIndexConstType() typeinfo.Type {
	return &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
}

func resolveConstTypesBinary(binary *Binary, expected typeinfo.Type, bitwise bool) (Expr, bool) {
	if isBitwiseBinaryOp(binary.Op) {
		// Bitwise expressions intentionally retain their unsigned machine
		// representation. This includes masks and shift counts.
		lhs, lhsChanged := resolveConstTypesExpr(binary.LHS, nil, true)
		rhs, rhsChanged := resolveConstTypesExpr(binary.RHS, nil, true)

		if !lhsChanged && !rhsChanged {
			return binary, false
		}

		next := *binary
		next.LHS = lhs
		next.RHS = rhs
		return &next, true
	}

	lhsExpected := expected
	rhsExpected := expected

	// When one side is a constant, the non-constant peer is stronger evidence
	// than the enclosing expression type.
	if _, ok := binary.LHS.(*Const); ok {
		if typ := binaryConstPeerType(binary.Op, binary.RHS); typ != nil {
			lhsExpected = typ
		}
	}

	if _, ok := binary.RHS.(*Const); ok {
		if typ := binaryConstPeerType(binary.Op, binary.LHS); typ != nil {
			rhsExpected = typ
		}
	}

	// A comparison against a bitwise expression should keep the constant
	// unsigned rather than inheriting the source object's signed type.
	lhsBitwise := containsBitwiseExpr(binary.LHS)
	rhsBitwise := containsBitwiseExpr(binary.RHS)

	lhs, lhsChanged := resolveConstTypesExpr(
		binary.LHS,
		lhsExpected,
		bitwise || rhsBitwise,
	)
	rhs, rhsChanged := resolveConstTypesExpr(
		binary.RHS,
		rhsExpected,
		bitwise || lhsBitwise,
	)

	next := *binary
	next.LHS = lhs
	next.RHS = rhs

	// Once constant typing is resolved, normalize addition/subtraction of a
	// negative RHS:
	//
	//     x + -1  -> x - 1
	//     x - -1  -> x + 1
	//
	// Do not apply this to the other operators supported by Op.Invert().
	if next.Op == OpAdd || next.Op == OpSub {
		if c, ok := next.RHS.(*Const); ok {
			if value, ok := c.Int64(); ok && value < 0 {
				inverted := next.Op.Invert()
				if inverted != OpUnknown {
					positive := *c
					positive.U64 = uint64(-value)

					next.Op = inverted
					next.RHS = &positive

					return &next, true
				}
			}
		}
	}

	if !lhsChanged && !rhsChanged {
		return binary, false
	}

	return &next, true
}

// binaryConstPeerType returns the type a constant should take from its binary
// peer.
func binaryConstPeerType(op Op, peer Expr) typeinfo.Type {
	if typ := semanticPeerType(peer); typ != nil {
		return typ
	}
	if op != OpAdd && op != OpSub {
		return nil
	}
	if _, ok := peer.ExprType().(*typeinfo.Enum); !ok {
		return nil
	}
	return arrayIndexConstType()
}

func resolveConstTypesCompare(compare *Compare, expected typeinfo.Type) (Expr, bool) {
	lhsExpected := expected
	rhsExpected := expected

	// When one side is a constant, the non-constant peer is stronger evidence
	// than the enclosing expression type.
	if _, ok := compare.LHS.(*Const); ok {
		if typ := semanticPeerType(compare.RHS); typ != nil {
			lhsExpected = typ
		}
	}

	if _, ok := compare.RHS.(*Const); ok {
		if typ := semanticPeerType(compare.LHS); typ != nil {
			rhsExpected = typ
		}
	}

	lhs, lhsChanged := resolveConstTypesExpr(
		compare.LHS,
		lhsExpected,
		false,
	)
	rhs, rhsChanged := resolveConstTypesExpr(
		compare.RHS,
		rhsExpected,
		false,
	)

	next := *compare
	next.LHS = lhs
	next.RHS = rhs

	if !lhsChanged && !rhsChanged {
		return compare, false
	}

	return &next, true
}

// semanticPeerType returns the source-level scalar type represented by expr.
//
// Casts introduced by integer promotion are deliberately ignored. For:
//
//	(uint16_t)(*lpT)
//
// where lpT is char *, this therefore returns char rather than uint16_t.
func semanticPeerType(expr Expr) typeinfo.Type {
	for expr != nil {
		switch e := expr.(type) {
		case *Cast:
			expr = e.Value
			continue

		case *SignExtend:
			expr = e.Parent
			continue
		}

		if containsBitwiseExpr(expr) {
			return nil
		}

		typ := expr.ExprType()
		if !typeinfo.IsIntLike(typ) {
			return nil
		}
		return typ
	}

	return nil
}

// isBitwiseBinaryOp reports whether op represents an explicitly bit-oriented
// operation whose constants should retain unsigned machine typing.
func isBitwiseBinaryOp(op Op) bool {
	switch op {
	case OpAnd, OpOr, OpXor, OpShl, OpShr:
		return true
	default:
		return false
	}
}

func containsBitwiseExpr(expr Expr) bool {
	switch e := expr.(type) {
	case *Binary:
		if isBitwiseBinaryOp(e.Op) {
			return true
		}
		return containsBitwiseExpr(e.LHS) ||
			containsBitwiseExpr(e.RHS)

	case *Cast:
		return containsBitwiseExpr(e.Value)

	case *Unary:
		return containsBitwiseExpr(e.X)

	default:
		return false
	}
}

func sameConstType(a, b typeinfo.Type) bool {
	if a == nil || b == nil {
		return a == b
	}

	// If your typeinfo package has a proper equality helper, use that instead.
	return a.String() == b.String()
}

// resolveConstTypesChildren is the generic fallback for expressions that
// aren't themselves interesting but can contain child expressions.
func resolveConstTypesChildren(expr Expr, bitwise bool) (Expr, bool) {
	rewriter := &semRewriter{
		expr: func(w *semRewriter, child Expr) (Expr, bool, bool) {
			if child == expr {
				return child, false, true
			}

			next, changed := resolveConstTypesExpr(child, nil, bitwise)
			return next, changed, false
		},
	}

	return rewriter.rewriteExpr(expr)
}
