package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// collapseWideExprPair reconstructs a wide expression from matching word trees.
func collapseWideExprPair(high, low Expr, expected typeinfo.Type) (Expr, bool) {
	if value, ok := collapseWideConstWords(high, low); ok {
		return value, true
	}
	if value, ok := collapseWideWordPair(high, low); ok {
		return value, true
	}
	if value, ok := collapseWideDerefPair(high, low, expected); ok {
		return value, true
	}
	if highConst, ok := high.(*Const); ok && highConst.U64 == 0 && semanticWordLane(low) {
		return &Cast{Value: low, To: typeinfo.U32.String(), TypeInfo: typeinfo.U32}, true
	}
	if value, ok := collapseWidePointerOffset(high, low, expected); ok {
		return value, true
	}
	hiBinary, hiOK := high.(*Binary)
	loBinary, loOK := low.(*Binary)
	if hiOK && loOK && hiBinary.Op == loBinary.Op {
		switch hiBinary.Op {
		case OpAdd, OpSub:
		default:
			return nil, false
		}
		lhs, lhsOK := collapseWideExprPair(hiBinary.LHS, loBinary.LHS, nil)
		rhs, rhsOK := collapseWideExprPair(hiBinary.RHS, loBinary.RHS, nil)
		if lhsOK && rhsOK {
			return &Binary{
				TypeInfo: collapseWideExprType(expected, lhs, rhs),
				Op:       hiBinary.Op,
				LHS:      lhs,
				RHS:      rhs,
			}, true
		}
	}
	return nil, false
}

// collapseWidePointerOffset reconstructs a wide pointer whose low word carries offset arithmetic.
func collapseWidePointerOffset(high, low Expr, expected typeinfo.Type) (Expr, bool) {
	binary, ok := low.(*Binary)
	if !ok || (binary.Op != OpAdd && binary.Op != OpSub) {
		return nil, false
	}
	if base, ok := collapseWideExprPair(high, binary.LHS, expected); ok {
		if !typeinfo.IsPointer(base.ExprType()) {
			return nil, false
		}
		offset := binary.RHS
		if binary.Op == OpSub {
			offset = &Unary{TypeInfo: offset.ExprType(), Op: OpNeg, X: offset}
		}
		return &PointerOffset{TypeInfo: base.ExprType(), Pointer: base, Offset: offset}, true
	}
	if binary.Op == OpAdd {
		if base, ok := collapseWideExprPair(high, binary.RHS, expected); ok {
			if !typeinfo.IsPointer(base.ExprType()) {
				return nil, false
			}
			return &PointerOffset{TypeInfo: base.ExprType(), Pointer: base, Offset: binary.LHS}, true
		}
	}
	return nil, false
}

// collapseWideDerefPair reconstructs adjacent low/high dereferences of the same storage.
func collapseWideDerefPair(high, low Expr, expected typeinfo.Type) (Expr, bool) {
	hi, hiOK := high.(*Deref)
	lo, loOK := low.(*Deref)
	if !hiOK || !loOK || hi.Width != 2 || lo.Width != 2 ||
		hi.ByteOff != lo.ByteOff+2 || !sameExpr(hi.Pointer, lo.Pointer) {
		return nil, false
	}
	next := *lo
	next.Width = 4
	next.TypeInfo = collapseWideExprType(expected, low, high)
	return &next, true
}

// semanticWordLane reports whether an expression carries at most one word of value.
func semanticWordLane(expr Expr) bool {
	if exprWidth(expr) == 2 {
		return true
	}
	field, ok := expr.(*FieldAccess)
	return ok && field.Field.Bitfield != nil && field.Field.Bitfield.BitWidth <= 16
}

// collapseWideExprType chooses the type for a rebuilt wide expression.
func collapseWideExprType(expected typeinfo.Type, exprs ...Expr) typeinfo.Type {
	if expected != nil && expected.Bytes() == 4 {
		return expected
	}
	for _, expr := range exprs {
		if expr != nil && expr.ExprType() != nil && expr.ExprType().Bytes() == 4 {
			return expr.ExprType()
		}
	}
	return intTypeForWidth(4)
}

// collapseWideWords matches two word args that reconstruct one wide expression.
func collapseWideWords(words *Words) (Expr, bool) {
	if len(words.Words) != 2 {
		return nil, false
	}
	if value, ok := collapseWideConstWords(words.Words[0], words.Words[1]); ok {
		return value, true
	}
	if parent, ok := collapseWideWordPair(words.Words[0], words.Words[1]); ok {
		return parent, true
	}
	return collapseWideExprPair(words.Words[0], words.Words[1], words.ExprType())
}

// collapseWideConstWords matches a high and low const word to one 32-bit const.
func collapseWideConstWords(high, low Expr) (Expr, bool) {
	hiConst, hiOK := high.(*Const)
	loConst, loOK := low.(*Const)
	if !hiOK || !loOK || exprWidth(high) != 2 || exprWidth(low) != 2 {
		return nil, false
	}
	return &Const{TypeInfo: intTypeForWidth(4), U64: ((hiConst.U64 & 0xffff) << 16) | (loConst.U64 & 0xffff)}, true
}

// collapseWideWordPair matches one high word and one low word from the same value.
func collapseWideWordPair(high, low Expr) (Expr, bool) {
	hiParent, hiOK := highWordParent(high)
	loParent, loOK := lowWordParent(low)
	if hiOK && loOK && sameExpr(hiParent, loParent) && exprWidth(hiParent) == 4 {
		return hiParent, true
	}

	hiParent, hiOK = wordPartParent(high, machine.WordSignHigh)
	loParent, loOK = lowWordParent(low)
	if hiOK && loOK && sameExpr(hiParent, loParent) && exprWidth(loParent) == 2 {
		return &SignExtend{Parent: loParent, FromBits: 16, ToBits: 32, TypeInfo: intTypeForWidth(4)}, true
	}
	return nil, false
}

// highWordParent returns the value represented by a high word expression.
func highWordParent(expr Expr) (Expr, bool) {
	if parent, ok := wordPartParent(expr, machine.WordHigh); ok {
		return parent, true
	}
	if parent, ok := farPointerPartParent(expr, machine.FarPointerSegment); ok {
		return parent, true
	}
	if deref, ok := expr.(*Deref); ok && deref.ByteOff == 2 && deref.Width == 2 && exprWidth(deref.Pointer) == 4 {
		return deref.Pointer, true
	}
	part, ok := expr.(*Part)
	if !ok || part.ByteOff != 2 || part.Width != 2 {
		return nil, false
	}
	return part.Base, true
}

// wordPartParent returns the parent of a matching word projection.
func wordPartParent(expr Expr, part machine.WordPart) (Expr, bool) {
	word, ok := expr.(*Word)
	if !ok || word.Part != part {
		return nil, false
	}
	return word.Parent, true
}

// lowWordParent returns the value represented by a low word expression.
func lowWordParent(expr Expr) (Expr, bool) {
	if parent, ok := wordPartParent(expr, machine.WordLow); ok {
		return parent, true
	}
	if parent, ok := farPointerPartParent(expr, machine.FarPointerOffset); ok {
		return parent, true
	}
	if part, ok := expr.(*Part); ok && part.ByteOff == 0 && part.Width == 2 {
		return part.Base, true
	}
	if exprWidth(expr) == 2 {
		return expr, true
	}
	return nil, false
}

// farPointerPartParent returns the parent of a matching far-pointer word projection.
func farPointerPartParent(expr Expr, part machine.FarPointerPart) (Expr, bool) {
	ptr, ok := expr.(*FarPointer)
	if !ok || ptr.Part != part || ptr.Parent == nil {
		return nil, false
	}
	return ptr.Parent, true
}

// exprWidth returns the expression width in bytes when it is known.
func exprWidth(expr Expr) int {
	if expr == nil || expr.ExprType() == nil {
		return 0
	}
	return expr.ExprType().Bytes()
}
