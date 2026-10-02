package sem

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type capturedBitfieldsProcessor struct{}

// ProcessFunc recovers a masked snapshot followed by clearing and inserting
// exactly one declared field. No intervening work or other snapshot use is allowed.
func (*capturedBitfieldsProcessor) ProcessFunc(_ *Result, f *Func) bool {
	changed := false
	for bi := range f.Blocks {
		effects := f.Blocks[bi].Effects
		for i := 0; i+2 < len(effects); i++ {
			capture, ok := effects[i].(*Assign)
			if !ok {
				continue
			}
			temp, ok := capture.Dst.(*Temp)
			if !ok || countTempDefs(f, temp) != 1 {
				continue
			}
			clear, ok := effects[i+1].(*Assign)
			if !ok {
				continue
			}
			field, ok := clear.Dst.(*FieldAccess)
			zero, zok := clear.Src.(*Const)
			if !ok || field.Field.Bitfield == nil || field.Field.Bitfield.Signed() || field.Field.Bitfield.StorageSize != temp.TypeInfo.Bytes() || !stableCopyObject(field.Base) || !zok || zero.U64 != 0 {
				continue
			}
			insert, ok := effects[i+2].(*Assign)
			if !ok || !capturedStorage(insert.Dst, capture.Src, field, temp) {
				continue
			}
			or, ok := insert.Src.(*Binary)
			if !ok || or.Op != OpOr || !sameCapturedWord(or.LHS, insert.Dst) || !sameExpr(or.RHS, temp) {
				continue
			}
			masked, ok := capture.Src.(*Binary)
			if !ok || masked.Op != OpAnd {
				continue
			}
			mask, ok := masked.RHS.(*Const)
			start, width := field.Field.BitRange()
			if !ok || start+width > temp.TypeInfo.Bytes()*8 || mask.U64 != ((uint64(1)<<width)-1)<<start {
				continue
			}
			value, ok := masked.LHS.(*Binary)
			if !ok || value.Op != OpOr {
				continue
			}
			var added Expr
			if sameCapturedWord(value.LHS, insert.Dst) {
				added = value.RHS
			} else if sameCapturedWord(value.RHS, insert.Dst) {
				added = value.LHS
			} else {
				continue
			}
			if !scratchExpressionPure(added) {
				continue
			}
			refs := 0
			for _, block := range f.Blocks {
				refs += countTempRefs(block.Effects, temp)
			}
			if refs != 2 {
				continue
			}
			relative, ok := aggregateBits(added, start, width)
			if !ok {
				continue
			}
			// The destination bitfield performs this final truncation itself.
			if bits, ok := relative.(*Binary); ok && bits.Op == OpAnd {
				if mask, ok := bits.RHS.(*Const); ok && mask.U64 == (uint64(1)<<width)-1 {
					relative = bits.LHS
				}
			}
			if cast, ok := relative.(*Cast); ok && cast.ExprType().Kind() == typeinfo.KInt && cast.Value.ExprType().Kind() == typeinfo.KInt && min(cast.ExprType().Bytes(), cast.Value.ExprType().Bytes())*8 >= width {
				relative = cast.Value
			}
			next := *clear
			next.Src = &Binary{TypeInfo: field.ExprType(), Op: OpOr, LHS: field, RHS: relative}
			effects = append(slices.Clone(effects[:i]), append([]Effect{&next}, effects[i+3:]...)...)
			f.Blocks[bi].Effects = effects
			changed = true
		}
	}
	return changed
}

// capturedStorage identifies the exact word containing the cleared field,
// including declared raw-word aliases. The mask check separately proves that
// the captured bits cannot affect its neighboring fields.
func capturedStorage(dst, value Expr, field *FieldAccess, temp *Temp) bool {
	masked, ok := value.(*Binary)
	if !ok || masked.Op != OpAnd {
		return false
	}
	or, ok := masked.LHS.(*Binary)
	if !ok || or.Op != OpOr {
		return false
	}
	for _, candidate := range []Expr{or.LHS, or.RHS} {
		if !sameCapturedWord(dst, candidate) {
			continue
		}
		switch word := candidate.(type) {
		case *Word:
			if word.Part == machine.WordLow && sameExpr(word.Parent, field.Base) && temp.TypeInfo.Bytes() == 2 {
				return true
			}
		case *Part:
			if word.ByteOff == 0 && word.Width == temp.TypeInfo.Bytes() && sameExpr(word.Base, field.Base) {
				return true
			}
		case *Deref:
			if word.ByteOff == 0 && word.Width == temp.TypeInfo.Bytes() && sameExpr(word.Pointer, field.Base) {
				return true
			}
		case *FieldAccess:
			if sameExpr(word.Base, field.Base) && word.Field.Bitfield == nil && word.Field.Offset == 0 && word.Field.Type.Bytes() == temp.TypeInfo.Bytes() {
				return true
			}
		}
	}
	return false
}

// sameCapturedWord compares read and write projections of the same exact
// storage word; semantic reads use Word while writes may use Part.
func sameCapturedWord(a, b Expr) bool {
	if sameExpr(a, b) {
		return true
	}
	aBase, aType, aStart, aWidth, aOK := aggregateStorageRange(a)
	bBase, bType, bStart, bWidth, bOK := aggregateStorageRange(b)
	return aOK && bOK && aStart == bStart && aWidth == bWidth && typeinfo.Equals(aType, bType) && sameExpr(aBase, bBase)
}
