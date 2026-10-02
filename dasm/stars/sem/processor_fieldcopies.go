package sem

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type fieldCopiesProcessor struct{}

// ProcessFunc recovers complete scalar-field copies, including pairs of
// coordinates selected on merge edges. Layout and use checks exclude partial
// objects, padding, unrelated temps, and intervening observable work.
func (*fieldCopiesProcessor) ProcessFunc(_ *Result, f *Func) bool {
	changed := false
	for sinkAggregateMerge(f) {
		changed = true
	}
	for bi := range f.Blocks {
		for i := 0; i < len(f.Blocks[bi].Effects); i++ {
			if copy, end, ok := completeFieldCopy(f, f.Blocks[bi].Effects, i); ok {
				effects := f.Blocks[bi].Effects
				f.Blocks[bi].Effects = append(slices.Clone(effects[:i]), append([]Effect{copy}, effects[end:]...)...)
				changed = true
			}
		}
	}
	return changed
}

// fieldObject turns a field base into its whole object, preserving a pointer
// dereference and normalizing a captured address back to its direct object.
func fieldObject(base Expr) (LValue, bool) {
	if addr, ok := base.(*AddressOf); ok {
		return addr.Target, true
	}
	if ptr, ok := base.ExprType().(*typeinfo.Pointer); ok {
		return &Deref{Pointer: base, TypeInfo: ptr.Elem, Width: ptr.Elem.Bytes()}, true
	}
	object, ok := base.(LValue)
	return object, ok
}

// stableCopyObject accepts nested direct objects and pointers with stable
// addresses, excluding memory-dependent indexes and repeated calls.
func stableCopyObject(object Expr) bool {
	switch x := object.(type) {
	case *FieldAccess:
		if isCPointer(x.ExprType()) {
			return false
		}
		return stableCopyObject(x.Base)
	case *Deref:
		return x.ByteOff == 0 && stableAggregateAddress(x.Pointer)
	case *ArrayIndex:
		return stableCopyObject(x.Base) && stableAggregateAddress(x.Index)
	default:
		return stableAggregateAddress(object)
	}
}

// completeFieldCopy proves a bounded run of stores covers exactly one struct.
// Only pure, lossless temporary captures used entirely by this run may occur
// between its field stores, and one side must be a direct local object.
func completeFieldCopy(f *Func, effects []Effect, start int) (*Assign, int, bool) {
	var dst, src LValue
	var fields []typeinfo.FieldBitRange
	var seen []string
	values := map[string]Expr{}
	var temps []*Temp
	rewriter := &semRewriter{expr: func(_ *semRewriter, expr Expr) (Expr, bool, bool) {
		if t, ok := expr.(*Temp); ok {
			if value, found := values[t.Name]; found {
				return value, true, true
			}
		}
		return nil, false, false
	}}
	for i := start; i < len(effects) && i < start+8; i++ {
		a, ok := effects[i].(*Assign)
		if !ok {
			break
		}
		if temp, ok := a.Dst.(*Temp); ok {
			value, _ := rewriter.rewriteExpr(a.Src)
			lossless := false
			switch value.(type) {
			case *AddressOf:
				lossless = typeinfo.Equals(value.ExprType(), temp.TypeInfo)
			case *FieldAccess:
				lo, hi, _, ok := armIntRange(value)
				min, max, fits := cIntRange(temp.TypeInfo)
				lossless = ok && fits && lo >= min && hi <= max
			}
			if countTempDefs(f, temp) != 1 || !lossless {
				break
			}
			values[temp.Name] = value
			temps = append(temps, temp)
			continue
		}
		d, _ := rewriter.rewriteExpr(a.Dst)
		s, _ := rewriter.rewriteExpr(a.Src)
		df, dok := d.(*FieldAccess)
		sf, sok := s.(*FieldAccess)
		if !dok || !sok || df.Field.Name != sf.Field.Name {
			break
		}
		dStart, dWidth := df.Field.BitRange()
		sStart, sWidth := sf.Field.BitRange()
		if dStart != sStart || dWidth != sWidth || !typeinfo.Equals(df.ExprType(), sf.ExprType()) {
			break
		}
		do, dok := fieldObject(df.Base)
		so, sok := fieldObject(sf.Base)
		if !dok || !sok || !typeinfo.Equals(do.ExprType(), so.ExprType()) || !stableCopyObject(do) || !stableCopyObject(so) {
			break
		}
		if dst == nil {
			typ, ok := do.ExprType().(*typeinfo.Struct)
			if !ok || len(typ.OverlapRegions) != 0 {
				break
			}
			fields, ok = typ.ScalarBitPartition(0, typ.Bytes()*8)
			if !ok || len(fields) < 2 {
				break
			}
			_, dl := storageRoot(do).(*Local)
			_, sl := storageRoot(so).(*Local)
			if !dl && !sl {
				break
			}
			dst, src = do, so
		} else if !sameExpr(dst, do) || !sameExpr(src, so) {
			break
		}
		if slices.Contains(seen, df.Field.Name) || !slices.ContainsFunc(fields, func(field typeinfo.FieldBitRange) bool {
			return field.Field.Name == df.Field.Name && field.Start == dStart && field.Width == dWidth && typeinfo.Equals(field.Field.Type, df.Field.Type)
		}) {
			break
		}
		seen = append(seen, df.Field.Name)
		if len(seen) != len(fields) {
			continue
		}
		for _, temp := range temps {
			total := 0
			for _, b := range f.Blocks {
				total += countTempRefs(b.Effects, temp)
			}
			if total != countTempRefs(effects[start:i+1], temp) {
				return nil, 0, false
			}
		}
		return &Assign{MetaInfo: effects[start].(*Assign).MetaInfo, Dst: dst, Src: src}, i + 1, true
	}
	return nil, 0, false
}

// sinkAggregateMerge recognizes a complete two-field aggregate selected on
// the same edges. All reads must be the adjacent destination stores, and each
// edge must capture both fields together from the same compatible object.
func sinkAggregateMerge(f *Func) bool {
	for bi, block := range f.Blocks {
		if len(block.Effects) < 2 {
			continue
		}
		first, ok := block.Effects[0].(*Assign)
		if !ok {
			continue
		}
		second, ok := block.Effects[1].(*Assign)
		if !ok {
			continue
		}
		d1, ok1 := first.Dst.(*FieldAccess)
		d2, ok2 := second.Dst.(*FieldAccess)
		t1, tok1 := first.Src.(*Temp)
		t2, tok2 := second.Src.(*Temp)
		if !ok1 || !ok2 || !tok1 || !tok2 || sameExpr(t1, t2) || !sameExpr(d1.Base, d2.Base) {
			continue
		}
		dst, ok := fieldObject(d1.Base)
		if !ok {
			continue
		}
		if _, local := dst.(*Local); !local {
			continue
		}
		typ, ok := dst.ExprType().(*typeinfo.Struct)
		if !ok || len(typ.OverlapRegions) != 0 {
			continue
		}
		fields, ok := typ.ScalarBitPartition(0, typ.Bytes()*8)
		if !ok || len(fields) != 2 || fields[0].Field.Name != d1.Field.Name || fields[1].Field.Name != d2.Field.Name {
			continue
		}
		defs, ok := edgeDefinitions(f, block.ID, t2)
		if !ok {
			continue
		}
		var sources []LValue
		for _, def := range defs {
			if def.index < 1 {
				break
			}
			a1, ok1 := f.Blocks[def.block].Effects[def.index-1].(*Assign)
			a2, ok2 := f.Blocks[def.block].Effects[def.index].(*Assign)
			if !ok1 || !ok2 || !sameExpr(a1.Dst, t1) || !sameExpr(a2.Dst, t2) {
				break
			}
			s1, ok1 := a1.Src.(*FieldAccess)
			s2, ok2 := a2.Src.(*FieldAccess)
			if !ok1 || !ok2 || !sameStructField(s1.Field, d1.Field) || !sameStructField(s2.Field, d2.Field) || !sameExpr(s1.Base, s2.Base) || !typeinfo.Equals(t1.TypeInfo, d1.ExprType()) || !typeinfo.Equals(t2.TypeInfo, d2.ExprType()) || !typeinfo.Equals(t1.TypeInfo, s1.ExprType()) || !typeinfo.Equals(t2.TypeInfo, s2.ExprType()) {
				break
			}
			src, ok := fieldObject(s1.Base)
			if !ok || !typeinfo.Equals(src.ExprType(), typ) || !stableCopyObject(src) {
				break
			}
			sources = append(sources, src)
		}
		if len(sources) != len(defs) {
			continue
		}
		refs1, refs2 := 0, 0
		for _, b := range f.Blocks {
			refs1 += countTempRefs(b.Effects, t1)
			refs2 += countTempRefs(b.Effects, t2)
		}
		if refs1 != len(defs)+1 || refs2 != len(defs)+1 {
			continue
		}
		for i, def := range defs {
			effects := f.Blocks[def.block].Effects
			a := *effects[def.index-1].(*Assign)
			a.Dst, a.Src, a.Merge = dst, sources[i], true
			f.Blocks[def.block].Effects = append(append(slices.Clone(effects[:def.index-1]), &a), effects[def.index+1:]...)
		}
		f.Blocks[bi].Effects = slices.Clone(block.Effects[2:])
		return true
	}
	return false
}
