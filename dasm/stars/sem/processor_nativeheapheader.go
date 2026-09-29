package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// heapHeaderStruct is the Stars heap block header (HB). Each heap block
// starts with an HB followed by its items, so the original code encoded the
// 16-byte Win16 header size as constants. The native HB holds full-width
// pointer and handle fields and is larger, so those constants must become
// sizeof(HB) for the native compile.
const heapHeaderStruct = "_hb"

// nativeHeapHeaderProcessor replaces the Win16 heap header size with
// sizeof(HB) where the code measures a heap block from its start:
//
//   - a byte offset from a header pointer past the header, such as the first
//     item's payload at ptroff(lphb, 18), becomes sizeof(HB) + 2.
//   - the ibTop offset stored or compared as 16 becomes sizeof(HB).
//   - cbFree and cbSlop computed as a block size minus 16 subtract
//     sizeof(HB).
//   - a block size stored to cbBlock that was grown by 16 to hold the header
//     adds sizeof(HB).
//
// Every match is anchored on a header-typed operand, so an unrelated 16 is
// left alone.
type nativeHeapHeaderProcessor struct{}

// ProcessFunc rewrites heap header size constants throughout one function.
func (p *nativeHeapHeaderProcessor) ProcessFunc(result *Result, f *Func) bool {
	blockSizes := heapBlockSizeValues(f)
	rewriter := &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			assign, ok := effect.(*Assign)
			if !ok {
				return nil, false, false
			}
			src, ok := heapHeaderAssignSrc(assign, blockSizes)
			if !ok {
				return nil, false, false
			}
			next := *assign
			next.Src = src
			rewritten, _ := w.rewriteEffectChildren(&next)
			return rewritten, true, true
		},
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			switch e := expr.(type) {
			case *PointerOffset:
				header, ok := structPointee(e.Pointer.ExprType())
				if !ok || header.Name != heapHeaderStruct {
					return nil, false, false
				}
				offset, ok := heapHeaderOffset(header, e.Offset)
				if !ok {
					return nil, false, false
				}
				next := *e
				next.Offset = offset
				return &next, true, true
			case *Compare:
				if header, ok := heapHeaderField(e.LHS, "ibTop"); ok {
					if rhs, ok := heapHeaderOffset(header, e.RHS); ok {
						return &Compare{Op: e.Op, LHS: e.LHS, RHS: rhs}, true, true
					}
				}
				if header, ok := heapHeaderField(e.RHS, "ibTop"); ok {
					if lhs, ok := heapHeaderOffset(header, e.LHS); ok {
						return &Compare{Op: e.Op, LHS: lhs, RHS: e.RHS}, true, true
					}
				}
			}
			return nil, false, false
		},
	}
	changed := false
	for i := range f.Blocks {
		effects, blockChanged := rewriter.rewriteEffects(f.Blocks[i].Effects)
		if blockChanged {
			f.Blocks[i].Effects = effects
			changed = true
		}
	}
	return changed
}

// heapBlockSizeValues returns each header struct keyed by the values the
// function stores to a header's cbBlock, the full block size including the
// header.
func heapBlockSizeValues(f *Func) map[Expr]*typeinfo.Struct {
	sizes := map[Expr]*typeinfo.Struct{}
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			assign, ok := effect.(*Assign)
			if !ok {
				continue
			}
			if header, ok := heapHeaderField(assign.Dst, "cbBlock"); ok {
				sizes[assign.Src] = header
			}
		}
	}
	return sizes
}

// heapHeaderAssignSrc returns the rewritten source of an assignment that
// stores a header-relative size: ibTop set to the header size, cbFree or
// cbSlop set to a block size less the header, or a block size later stored
// to cbBlock grown by the header.
func heapHeaderAssignSrc(assign *Assign, blockSizes map[Expr]*typeinfo.Struct) (Expr, bool) {
	if header, ok := heapHeaderField(assign.Dst, "ibTop"); ok {
		return heapHeaderOffset(header, assign.Src)
	}
	for _, name := range []string{"cbFree", "cbSlop"} {
		if header, ok := heapHeaderField(assign.Dst, name); ok {
			return heapHeaderAdjust(header, assign.Src, OpSub)
		}
	}
	for value, header := range blockSizes {
		if sameExpr(value, assign.Dst) {
			if bin, ok := assign.Src.(*Binary); ok && sameExpr(bin.LHS, assign.Dst) {
				return heapHeaderAdjust(header, assign.Src, OpAdd)
			}
		}
	}
	return nil, false
}

// heapHeaderAdjust rewrites x + header size (op OpAdd) or x - header size
// (op OpSub, which the compiler emits as x + -size) to use sizeof(header).
func heapHeaderAdjust(header *typeinfo.Struct, expr Expr, op Op) (Expr, bool) {
	bin, ok := expr.(*Binary)
	if !ok {
		return nil, false
	}
	c, ok := bin.RHS.(*Const)
	if !ok {
		return nil, false
	}
	value := int64(int16(c.U64))
	size := int64(header.Size)
	switch {
	case op == OpAdd && bin.Op == OpAdd && value == size,
		op == OpSub && bin.Op == OpAdd && value == -size,
		op == OpSub && bin.Op == OpSub && value == size:
		return &Binary{TypeInfo: bin.TypeInfo, Op: op, LHS: bin.LHS, RHS: &SizeOf{Type: header}, Producer: bin.Producer}, true
	}
	return nil, false
}

// Header constants are compared as signed words: a block is at most 64K, and
// the compiler emits a subtracted size as an added negative word.

// heapHeaderOffset rewrites a constant byte offset from the start of a heap
// block that lies at or past the header as sizeof(header) plus the offset
// into the items.
func heapHeaderOffset(header *typeinfo.Struct, expr Expr) (Expr, bool) {
	c, ok := expr.(*Const)
	if !ok {
		return nil, false
	}
	value := int64(int16(c.U64))
	if value < int64(header.Size) {
		return nil, false
	}
	size := &SizeOf{Type: header}
	if value == int64(header.Size) {
		return size, true
	}
	past := &Const{TypeInfo: typeinfo.I16, U64: uint64(value) - uint64(header.Size)}
	return &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: size, RHS: past}, true
}

// heapHeaderField returns the header struct when expr is the named field of
// a heap header.
func heapHeaderField(expr Expr, name string) (*typeinfo.Struct, bool) {
	header, ok := fieldOwner(expr, name)
	return header, ok && header.Name == heapHeaderStruct
}

// fieldOwner returns the struct that owns expr when expr is its named field,
// selected from the struct or through a pointer to it, as a field access or a
// resolved symbol path.
func fieldOwner(expr Expr, name string) (*typeinfo.Struct, bool) {
	var typ typeinfo.Type
	switch e := expr.(type) {
	case *FieldAccess:
		if e.Field.Name != name {
			return nil, false
		}
		typ = e.Base.ExprType()
	case *SymbolRef:
		field, ok := e.Path.(*symresolve.SymbolField)
		if !ok || field.Field.Name != name {
			return nil, false
		}
		typ = field.Base.Type()
	default:
		return nil, false
	}
	if s, ok := structPointee(typ); ok {
		return s, true
	}
	s, ok := typ.(*typeinfo.Struct)
	return s, ok
}
