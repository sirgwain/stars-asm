package sem

import (
	"fmt"
	"math/bits"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// aggregateStorageRange identifies physical storage within a declared struct.
// Dereferences are mapped through their pointee's original layout, never by C
// pointer arithmetic using the original byte displacement.
func aggregateStorageRange(expr Expr) (Expr, *typeinfo.Struct, int, int, bool) {
	switch e := expr.(type) {
	case *Part:
		base, typ, off, width, ok := aggregateStorageRange(e.Base)
		if !ok || e.ByteOff < 0 || e.Width <= 0 || e.ByteOff*8+e.Width*8 > width {
			return nil, nil, 0, 0, false
		}
		return base, typ, off + e.ByteOff*8, e.Width * 8, true
	case *Word:
		base, typ, off, width, ok := aggregateStorageRange(e.Parent)
		lane := 0
		if e.Part == machine.WordHigh {
			lane = 16
		} else if e.Part != machine.WordLow {
			return nil, nil, 0, 0, false
		}
		if !ok || lane+16 > width {
			return nil, nil, 0, 0, false
		}
		return base, typ, off + lane, 16, true
	case *Deref:
		typ, ok := e.Pointer.ExprType().(*typeinfo.Pointer)
		if !ok {
			break
		}
		strct, ok := typ.Elem.(*typeinfo.Struct)
		if !ok || e.ByteOff < 0 || e.Width <= 0 || e.ByteOff+e.Width > strct.Bytes() {
			break
		}
		return e.Pointer, strct, e.ByteOff * 8, e.Width * 8, true
	}
	if _, ok := expr.(LValue); ok {
		if typ, ok := expr.ExprType().(*typeinfo.Struct); ok {
			return expr, typ, 0, typ.Bytes() * 8, true
		}
	}
	return nil, nil, 0, 0, false
}

// aggregateConst creates a constant for unsigned storage calculations.
func aggregateConst(value uint64) *Const { return &Const{TypeInfo: typeinfo.U32, U64: value} }

// aggregateBinary folds constants and identities in unsigned storage arithmetic.
func aggregateBinary(op Op, lhs, rhs Expr) Expr {
	a, aok := lhs.(*Const)
	b, bok := rhs.(*Const)
	if aok && bok {
		switch op {
		case OpAnd:
			return aggregateConst(a.U64 & b.U64)
		case OpOr:
			return aggregateConst(a.U64 | b.U64)
		case OpXor:
			return aggregateConst(a.U64 ^ b.U64)
		case OpShl:
			return aggregateConst((a.U64 << b.U64) & 0xffffffff)
		case OpShr:
			return aggregateConst(a.U64 >> b.U64)
		}
	}
	if (op == OpOr || op == OpXor) && aok && a.U64 == 0 {
		return rhs
	}
	if op == OpOr {
		if merged, ok := mergeAggregateSlices(lhs, rhs); ok {
			return merged
		}
	}
	if bok {
		if b.U64 == 0 {
			if op == OpAnd {
				return aggregateConst(0)
			}
			if op == OpOr || op == OpXor || op == OpShl || op == OpShr {
				return lhs
			}
		}
		if op == OpAnd && b.U64 == 0xffffffff {
			return lhs
		}
	}
	return &Binary{TypeInfo: typeinfo.U32, Op: op, LHS: lhs, RHS: rhs}
}

// aggregateSlice is an unsigned bit range of a value placed at a shift.
type aggregateSlice struct {
	base                Expr
	off, width, shifted int
}

// unsignedValueWidth returns the value width of an unsigned bitfield or
// unsigned integer storage read, whose bits above that width are zero.
// Arithmetic is excluded because C evaluates it in the promoted type.
func unsignedValueWidth(expr Expr) (int, bool) {
	if _, ok := expr.(LValue); !ok {
		return 0, false
	}
	if field, ok := expr.(*FieldAccess); ok && field.Field.Bitfield != nil {
		if field.Field.Bitfield.Signed() {
			return 0, false
		}
		return field.Field.Bitfield.BitWidth, true
	}
	if p, ok := expr.ExprType().(*typeinfo.Primitive); ok && !p.Signed && p.Kind() == typeinfo.KInt && p.Bytes() <= 4 {
		return p.Bytes() * 8, true
	}
	return 0, false
}

// aggregateSliceOf recognizes the slice shapes produced by aggregateSliceExpr.
func aggregateSliceOf(expr Expr) (aggregateSlice, bool) {
	shifted := 0
	if b, ok := expr.(*Binary); ok && b.Op == OpShl {
		c, ok := b.RHS.(*Const)
		if !ok {
			return aggregateSlice{}, false
		}
		shifted, expr = int(c.U64), b.LHS
	}
	if width, ok := unsignedValueWidth(expr); ok {
		return aggregateSlice{expr, 0, width, shifted}, true
	}
	b, ok := expr.(*Binary)
	if !ok || b.Op != OpAnd {
		return aggregateSlice{}, false
	}
	mask, ok := b.RHS.(*Const)
	if !ok {
		return aggregateSlice{}, false
	}
	width, ok := lowBitMaskWidth(uint(mask.U64))
	if !ok || width > 32 {
		return aggregateSlice{}, false
	}
	off, inner := 0, b.LHS
	if s, ok := inner.(*Binary); ok && s.Op == OpShr {
		c, ok := s.RHS.(*Const)
		if !ok {
			return aggregateSlice{}, false
		}
		off, inner = int(c.U64), s.LHS
	}
	if cast, ok := inner.(*Cast); ok && typeinfo.Equals(cast.TypeInfo, typeinfo.U32) {
		return aggregateSlice{cast.Value, off, width, shifted}, true
	}
	// Masks kept from a destination apply directly to its unsigned read.
	if _, ok := unsignedValueWidth(inner); ok {
		return aggregateSlice{inner, off, width, shifted}, true
	}
	return aggregateSlice{}, false
}

// aggregateSliceExpr emits a slice, reading an unsigned value directly when
// the slice covers all of its bits.
func aggregateSliceExpr(s aggregateSlice) Expr {
	var value Expr
	if width, ok := unsignedValueWidth(s.base); ok && s.off == 0 && s.width >= width {
		value = s.base
	} else {
		value = &Cast{To: "uint32_t", TypeInfo: typeinfo.U32, Value: s.base}
		value = aggregateBinary(OpShr, value, aggregateConst(uint64(s.off)))
		value = aggregateBinary(OpAnd, value, aggregateConst(uint64(1)<<s.width-1))
	}
	return aggregateBinary(OpShl, value, aggregateConst(uint64(s.shifted)))
}

// mergeAggregateSlices joins two adjacent slices of one pure value that keep
// their relative bit positions, so split word reads rebuild the original read.
func mergeAggregateSlices(lhs, rhs Expr) (Expr, bool) {
	l, ok := aggregateSliceOf(lhs)
	if !ok {
		return nil, false
	}
	r, ok := aggregateSliceOf(rhs)
	if !ok || !sameExpr(l.base, r.base) || !scratchExpressionPure(l.base) {
		return nil, false
	}
	if r.off < l.off {
		l, r = r, l
	}
	if l.off+l.width != r.off || l.shifted+l.width != r.shifted || l.width+r.width > 32 {
		return nil, false
	}
	return aggregateSliceExpr(aggregateSlice{l.base, l.off, l.width + r.width, l.shifted}), true
}

// aggregateBits extracts unsigned representation bits, distributing masks over
// packed fields so preserved fields simplify back to the original field read.
func aggregateBits(expr Expr, off, width int) (Expr, bool) {
	if off < 0 || width <= 0 || off+width > 32 {
		return nil, false
	}
	mask := uint64(1)<<width - 1
	switch e := expr.(type) {
	case *Const:
		return aggregateConst((e.U64 >> off) & mask), true
	case *Words:
		if len(e.Words) != 2 {
			return nil, false
		}
		if off >= 16 {
			return aggregateBits(e.Words[0], off-16, width)
		}
		if off+width <= 16 {
			return aggregateBits(e.Words[1], off, width)
		}
		lo, ok1 := aggregateBits(e.Words[1], off, 16-off)
		hi, ok2 := aggregateBits(e.Words[0], 0, width-(16-off))
		if !ok1 || !ok2 {
			return nil, false
		}
		return aggregateBinary(OpOr, lo, aggregateBinary(OpShl, hi, aggregateConst(uint64(16-off)))), true
	case *Binary:
		if e.Op == OpAnd || e.Op == OpOr || e.Op == OpXor {
			// A constant zero mask must not require recovering the discarded operand.
			a, aok := e.LHS.(*Const)
			b, bok := e.RHS.(*Const)
			if e.Op == OpAnd && (aok && (a.U64>>off)&mask == 0 || bok && (b.U64>>off)&mask == 0) {
				return aggregateConst(0), true
			}
			lhs, ok1 := aggregateBits(e.LHS, off, width)
			rhs, ok2 := aggregateBits(e.RHS, off, width)
			if !ok1 || !ok2 {
				return nil, false
			}
			if c, ok := lhs.(*Const); ok && e.Op == OpAnd && c.U64 == mask {
				return rhs, true
			}
			if c, ok := rhs.(*Const); ok && e.Op == OpAnd && c.U64 == mask {
				return lhs, true
			}
			return aggregateBinary(e.Op, lhs, rhs), true
		}
		c, ok := e.RHS.(*Const)
		size := e.ExprType().Bytes() * 8
		if !ok || c.U64 >= 32 && e.Op != OpMul || off+width > size {
			break
		}
		shift := int(c.U64)
		if e.Op == OpMul {
			if c.U64 == 0 || c.U64&(c.U64-1) != 0 {
				break
			}
			shift = bits.TrailingZeros64(c.U64)
		}
		switch e.Op {
		case OpShl, OpMul:
			if off+width <= shift {
				return aggregateConst(0), true
			}
			if off >= shift {
				return aggregateBits(e.LHS, off-shift, width)
			}
			low, ok := aggregateBits(e.LHS, 0, off+width-shift)
			if !ok {
				return nil, false
			}
			return aggregateBinary(OpShl, low, aggregateConst(uint64(shift-off))), true
		case OpShr:
			// Shifted-in bits are only zero in C when the operand is unsigned.
			if e.LHS.ExprType().Bytes()*8 != size {
				break
			}
			if off+shift+width > size {
				if p, ok := e.LHS.ExprType().(*typeinfo.Primitive); !ok || p.Signed {
					break
				}
				if off+shift >= size {
					return aggregateConst(0), true
				}
			}
			return aggregateBits(e.LHS, off+shift, min(width, size-off-shift))
		}
	case *Byte:
		if e.Value == nil && off+width <= 8 {
			switch e.Part {
			case machine.ByteLow:
				return aggregateBits(e.Parent, off, width)
			case machine.ByteHigh:
				return aggregateBits(e.Parent, off+8, width)
			}
		}
	case *Cast:
		// Truncation and extension both preserve the bits below the narrower width.
		if e.TypeInfo.Kind() == typeinfo.KInt && e.Value.ExprType().Kind() == typeinfo.KInt &&
			off+width <= min(e.TypeInfo.Bytes(), e.Value.ExprType().Bytes())*8 {
			return aggregateBits(e.Value, off, width)
		}
	case *Word:
		if e.Part == machine.WordLow && off+width <= 16 {
			return aggregateBits(e.Parent, off, width)
		}
		if e.Part == machine.WordHigh && off+width <= 16 {
			return aggregateBits(e.Parent, off+16, width)
		}
	}
	if base, typ, start, size, ok := aggregateStorageRange(expr); ok {
		if off+width > size {
			return nil, false
		}
		fields, ok := typ.ScalarBitPartition(start+off, width)
		if !ok {
			return nil, false
		}
		var value Expr = aggregateConst(0)
		for _, f := range fields {
			lo, hi := max(start+off, f.Start), min(start+off+width, f.Start+f.Width)
			field := &FieldAccess{Base: base, Field: f.Field}
			slice, ok := aggregateBits(field, lo-f.Start, hi-lo)
			if !ok {
				return nil, false
			}
			value = aggregateBinary(OpOr, value, aggregateBinary(OpShl, slice, aggregateConst(uint64(lo-start-off))))
		}
		return value, true
	}
	if valueWidth, ok := unsignedValueWidth(expr); ok && off >= valueWidth {
		return aggregateConst(0), true
	}
	if expr.ExprType().Kind() != typeinfo.KInt {
		return nil, false
	}
	// Resolve aggregate operands inside scalar arithmetic before extracting bits.
	valid := true
	walkExpr(expr, func(e Expr) {
		switch e.(type) {
		case *Memory, *RawMemory, *RawValue, *Register, *Merge:
			valid = false
		}
	})
	if !valid {
		return nil, false
	}
	w := semRewriter{expr: func(w *semRewriter, child Expr) (Expr, bool, bool) {
		if _, _, _, size, ok := aggregateStorageRange(child); ok {
			value, ok := aggregateBits(child, 0, size)
			if !ok {
				valid = false
				return child, false, true
			}
			return value, true, true
		}
		if _, storage := child.(LValue); storage {
			return child, false, true
		}
		return child, false, false
	}}
	value, _ := w.rewriteExpr(expr)
	if !valid {
		return nil, false
	}
	return aggregateSliceExpr(aggregateSlice{value, off, width, 0}), true
}

// packAggregateSource stores a whole small struct into same-width integer
// storage as its packed field representation rather than as a struct value.
func packAggregateSource(a *Assign) (*Assign, bool) {
	dst, src := a.Dst.ExprType(), a.Src.ExprType()
	if dst.Kind() != typeinfo.KInt || src.Kind() != typeinfo.KStruct || dst.Bytes() != src.Bytes() || src.Bytes() > 4 {
		return nil, false
	}
	packed, ok := aggregateBits(a.Src, 0, src.Bytes()*8)
	if !ok {
		return nil, false
	}
	next := *a
	next.Src = packed
	return &next, true
}

// aggregateTemp reserves a temporary distinct from all input identifiers.
func (p *resolveLateBitfieldsProcessor) aggregateTemp(typ typeinfo.Type) *Temp {
	for {
		p.nextTemp++
		name := fmt.Sprintf("t_fields_%d", p.nextTemp)
		if p.names[name] {
			continue
		}
		p.names[name] = true
		return &Temp{Name: name, TypeInfo: typ}
	}

}

// expandAggregateWrite replaces a proven storage write by field assignments.
// All source values are captured before any field changes, and a compound
// destination address is evaluated once before those source reads.
func (p *resolveLateBitfieldsProcessor) expandAggregateWrite(a *Assign) ([]Effect, bool) {
	base, typ, start, width, ok := aggregateStorageRange(a.Dst)
	if !ok || width > 32 {
		return nil, false
	}
	// Whole-object copies of the same struct stay ordinary struct assignments.
	if _, whole := a.Dst.ExprType().(*typeinfo.Struct); whole && typeinfo.Equals(a.Dst.ExprType(), a.Src.ExprType()) {
		return nil, false
	}
	fields, ok := typ.ScalarBitPartition(start, width)
	if !ok {
		return nil, false
	}
	// Field writes cannot change the variables and indexes of a stable address,
	// so only other bases are evaluated once into a pointer temporary.
	var prefix []Effect
	stable := base
	if !stableAggregateAddress(base) {
		pointer, ptr := base.ExprType().(*typeinfo.Pointer)
		address := base
		if !ptr {
			pointer = &typeinfo.Pointer{Elem: typ}
			address = &AddressOf{Target: base.(LValue), TypeInfo: pointer}
		}
		temp := p.aggregateTemp(pointer)
		prefix = append(prefix, &Assign{MetaInfo: a.MetaInfo, Dst: temp, Src: address})
		stable = temp
	}
	source := a.Src
	if !scratchExpressionPure(source) {
		if sourceBase, sourceType, off, size, ok := aggregateStorageRange(source); ok {
			ptr, indirect := sourceBase.ExprType().(*typeinfo.Pointer)
			address := sourceBase
			if !indirect {
				ptr = &typeinfo.Pointer{Elem: sourceType}
				address = &AddressOf{Target: sourceBase.(LValue), TypeInfo: ptr}
			}
			temp := p.aggregateTemp(ptr)
			prefix = append(prefix, &Assign{MetaInfo: a.MetaInfo, Dst: temp, Src: address})
			source = &Deref{Pointer: temp, ByteOff: off / 8, Width: size / 8, TypeInfo: typeinfo.UintForWidth(size / 8)}
		} else {
			if source.ExprType().Kind() != typeinfo.KInt {
				return nil, false
			}
			containsAggregate := false
			walkExpr(source, func(e Expr) {
				if _, _, _, _, ok := aggregateStorageRange(e); ok {
					containsAggregate = true
				}
			})
			if containsAggregate {
				return nil, false
			}
			temp := p.aggregateTemp(source.ExprType())
			prefix = append(prefix, &Assign{MetaInfo: a.MetaInfo, Dst: temp, Src: source})
			source = temp
		}
	}
	var writes []*Assign
	for _, f := range fields {
		lo, hi := max(start, f.Start), min(start+width, f.Start+f.Width)
		src, ok := aggregateBits(source, lo-start, hi-lo)
		if !ok {
			return nil, false
		}
		dst := &FieldAccess{Base: base, Field: f.Field}
		if lo != f.Start || hi != f.Start+f.Width {
			mask := (uint64(1)<<(hi-lo) - 1) << (lo - f.Start)
			old, ok := aggregateBits(dst, 0, f.Width)
			if !ok {
				return nil, false
			}
			src = aggregateBinary(OpOr, aggregateBinary(OpAnd, old, aggregateConst((uint64(1)<<f.Width-1)&^mask)), aggregateBinary(OpShl, src, aggregateConst(uint64(lo-f.Start))))
		}
		if sameExpr(src, dst) {
			continue
		}
		writes = append(writes, &Assign{MetaInfo: a.MetaInfo, Dst: dst, Src: src})
	}
	if len(writes) == 0 {
		if scratchExpressionPure(a.Dst) && scratchExpressionPure(a.Src) {
			return nil, true
		}
		return nil, false
	}
	for _, write := range writes {
		write.Dst = &FieldAccess{Base: stable, Field: write.Dst.(*FieldAccess).Field}
		if stable != base && scratchExpressionPure(a.Src) {
			w := semRewriter{expr: func(w *semRewriter, e Expr) (Expr, bool, bool) {
				if field, ok := e.(*FieldAccess); ok && sameExpr(field.Base, base) {
					next := *field
					next.Base = stable
					return &next, true, true
				}
				return e, false, false
			}}
			write.Src, _ = w.rewriteExpr(write.Src)
		}
		if len(writes) > 1 {
			if _, constant := write.Src.(*Const); !constant {
				temp := p.aggregateTemp(typeinfo.U32)
				prefix = append(prefix, &Assign{MetaInfo: a.MetaInfo, Dst: temp, Src: write.Src})
				write.Src = temp
			}
		}
	}
	for _, write := range writes {
		prefix = append(prefix, write)
	}
	return prefix, true
}

// coalesceAggregateCopy proves that two adjacent word transfers cover one
// compatible padding-free object. Pure stable addresses exclude repeated calls
// and indexes loaded from the object being modified.
func coalesceAggregateCopy(low, high *Assign) (*Assign, bool) {
	dst, typ, off, width, ok := aggregateStorageRange(low.Dst)
	if !ok || off != 0 || width != 16 || typ.Bytes() != 4 || len(typ.OverlapRegions) != 0 {
		return nil, false
	}
	d2, t2, o2, w2, ok := aggregateStorageRange(high.Dst)
	if !ok || o2 != 16 || w2 != 16 || !typeinfo.Equals(typ, t2) || !sameExpr(dst, d2) {
		return nil, false
	}
	src, st, so, sw, ok := aggregateStorageRange(low.Src)
	if !ok || so != 0 || sw != 16 || !typeinfo.Equals(typ, st) {
		return nil, false
	}
	s2, st2, so2, sw2, ok := aggregateStorageRange(high.Src)
	if !ok || so2 != 16 || sw2 != 16 || !typeinfo.Equals(st, st2) || !sameExpr(src, s2) {
		return nil, false
	}
	if _, ok := typ.ScalarBitPartition(0, 32); !ok {
		return nil, false
	}
	if !stableAggregateAddress(dst) || !stableAggregateAddress(src) {
		return nil, false
	}
	// At least one direct object rules out partially overlapping pointer ranges.
	_, dl := dst.(*Local)
	_, dt := dst.(*Temp)
	_, sl := src.(*Local)
	_, stemp := src.(*Temp)
	if !dl && !dt && !sl && !stemp {
		return nil, false
	}
	destination, dok := dst.(LValue)
	source := src
	if pointer, ok := dst.ExprType().(*typeinfo.Pointer); ok {
		destination = &Deref{Pointer: dst, Width: typ.Bytes(), TypeInfo: pointer.Elem}
		dok = true
	}
	if pointer, ok := src.ExprType().(*typeinfo.Pointer); ok {
		source = &Deref{Pointer: src, Width: typ.Bytes(), TypeInfo: pointer.Elem}
	}
	if !dok {
		return nil, false
	}
	return &Assign{MetaInfo: low.MetaInfo, Dst: destination, Src: source}, true
}

// stableAggregateAddress accepts addressing based on variables and arithmetic,
// excluding calls and memory-dependent indexes that a partial write could alter.
func stableAggregateAddress(expr Expr) bool {
	switch e := expr.(type) {
	case *Local, *Global, *Temp, *Const:
		return true
	case *ArrayIndex:
		return stableAggregateAddress(e.Base) && stableAggregateAddress(e.Index)
	case *Binary:
		return stableAggregateAddress(e.LHS) && stableAggregateAddress(e.RHS)
	case *Cast:
		return stableAggregateAddress(e.Value)
	}
	return false
}
