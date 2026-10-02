package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type byteStoresProcessor struct{}

// ProcessBlock simplifies values stored into byte storage, which C's
// assignment truncates to their low byte: operations that cannot change that
// byte are dropped, as the low-byte projection in sz[1] = LOBYTE(i + 65) and
// the high-byte merge in rgbRaw[j] = (i & 0xff00) | (rgbRaw2[i] & 0xff).
func (p *byteStoresProcessor) ProcessBlock(_ *Result, _ Func, b Block) (Block, bool) {
	changed := false
	effects := make([]Effect, len(b.Effects))
	for i, effect := range b.Effects {
		effects[i] = effect
		assign, ok := effect.(*Assign)
		if !ok {
			continue
		}
		if dst := assign.Dst.ExprType(); dst == nil || dst.Kind() != typeinfo.KInt || dst.Bytes() != 1 {
			continue
		}
		value := lowByteSource(assign.Src)
		if value == assign.Src {
			continue
		}
		next := *assign
		next.Src = value
		effects[i] = &next
		changed = true
	}
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// lowByteSource strips from an integer value the operations that leave its
// low byte unchanged, which is all a byte store keeps: a low-byte or
// low-word projection, an integer cast, a mask keeping the low byte, an OR
// with a value whose low byte is zero, and a byte replacement, whose low
// byte is the replacing value's or the parent's.
//
// The low byte of a sum, difference, product, bitwise combination,
// negation, complement or left shift depends only on its operands' low
// bytes, so their operands are simplified the same way: b = b + LOBYTE(bT)
// is b += bT.
func lowByteSource(expr Expr) Expr {
	for {
		next, ok := lowByteStep(expr)
		if !ok {
			break
		}
		expr = next
	}
	switch e := expr.(type) {
	case *Binary:
		switch e.Op {
		case OpAdd, OpSub, OpMul, OpAnd, OpOr, OpXor:
			lhs, rhs := lowByteSource(e.LHS), lowByteSource(e.RHS)
			if lhs != e.LHS || rhs != e.RHS {
				next := *e
				next.LHS, next.RHS = lhs, rhs
				return &next
			}
		case OpShl:
			if lhs := lowByteSource(e.LHS); lhs != e.LHS {
				next := *e
				next.LHS = lhs
				return &next
			}
		}
	case *Unary:
		if e.Op == OpNeg || e.Op == OpNot {
			if x := lowByteSource(e.X); x != e.X {
				next := *e
				next.X = x
				return &next
			}
		}
	}
	return expr
}

// lowByteStep strips one operation that leaves expr's low byte unchanged.
func lowByteStep(expr Expr) (Expr, bool) {
	var value Expr
	switch e := expr.(type) {
	case *Byte:
		switch {
		case e.Value == nil && e.Part == machine.ByteLow:
			value = e.Parent
		case e.Value != nil && e.Part == machine.ByteLow:
			// the parent with its low byte replaced by Value's
			value = e.Value
		case e.Value != nil && e.Part == machine.ByteHigh:
			// the parent with its high byte replaced, keeping its low byte
			value = e.Parent
		default:
			return nil, false
		}
	case *Part:
		if e.ByteOff != 0 || e.Width > 2 {
			return nil, false
		}
		value = e.Base
	case *Word:
		if e.Part != machine.WordLow {
			return nil, false
		}
		value = e.Parent
	case *Cast:
		value = e.Value
	case *Binary:
		switch e.Op {
		case OpAnd:
			if mask, ok := e.RHS.(*Const); ok && mask.U64&0xff == 0xff {
				value = e.LHS
			}
		case OpOr:
			switch {
			case lowByteZero(e.LHS):
				value = e.RHS
			case lowByteZero(e.RHS):
				value = e.LHS
			}
		}
	}
	if value == nil {
		return nil, false
	}
	if typ := value.ExprType(); typ == nil || typ.Kind() != typeinfo.KInt || typ.Bytes() > 4 {
		return nil, false
	}
	return value, true
}

// lowByteZero reports whether expr's low byte is always zero: a mask
// clearing it, or a constant without one.
func lowByteZero(expr Expr) bool {
	switch e := expr.(type) {
	case *Const:
		return e.U64&0xff == 0
	case *Binary:
		if e.Op != OpAnd {
			return false
		}
		if mask, ok := e.RHS.(*Const); ok {
			return mask.U64&0xff == 0
		}
	case *Cast:
		return lowByteZero(e.Value)
	}
	return false
}
