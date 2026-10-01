package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertCopyEffect preserves the full byte range of a machine copy. Whole
// typed objects use assignment; array slices and untyped ranges use fmemmove.
// MOVS copies forward, which fmemmove matches unless the destination overlaps
// the source from above; the direction flag is not modeled.
func (c *machineConverter) convertCopyEffect(e machine.CopyEffect) Effect {
	dst := c.convertCopyAddress(e.Dst.(*machine.Address), e.Width)
	src := c.convertCopyAddress(e.Src.(*machine.Address), e.Width)
	if target, ok := c.recoverExpectedValue(dst, src.ExprType()).(LValue); ok {
		dst = target
	}
	source := c.recoverExpectedValue(src, dst.ExprType())
	c.recordCopyWrite(e.Dst, e.Width)
	if copyAssignable(dst.ExprType(), e.Width) && copyAssignable(source.ExprType(), e.Width) {
		return &Assign{MetaInfo: e.MetaInfo, Dst: dst, Src: source}
	}
	move := c.ctx.sdb.GetFunction("fmemmove")
	return &CallEffect{MetaInfo: e.MetaInfo, Call: &Call{
		Function: move,
		Args: []Expr{
			objectAddress(dst, 0, move.Params[0].Type),
			objectAddress(src, 0, move.Params[1].Type),
			&Const{TypeInfo: move.Params[2].Type, U64: uint64(e.Width)},
		},
	}}
}

// copyAssignable reports whether a copy operand of type t can be expressed as
// a C assignment spanning exactly width bytes.
func copyAssignable(t typeinfo.Type, width int) bool {
	if t.Bytes() != width || t.Kind() == typeinfo.KArray {
		return false
	}
	return t.Kind() != typeinfo.KInt || width == 1 || width == 2 || width == 4 || width == 8
}

// convertCopyAddress resolves an address-valued copy operand at copy width.
func (c *machineConverter) convertCopyAddress(addr *machine.Address, width int) LValue {
	mem := copyAddressMemoryAccess(addr.Addr, width)
	if segment, ok := mem.Seg.(*machine.FarPointer); ok {
		if parent, ok := commonFarPointerParent(segment, mem.Base); ok {
			pointer := c.convertValue(parent)
			if mem.Index == nil && typeinfo.IsPointer(pointer.ExprType()) {
				offset := signedWordOffset(uint(mem.Disp))
				ptr := pointer.ExprType().(*typeinfo.Pointer)
				if ptr.Elem.Bytes() == width {
					if projected, ok := projectPointerAddress(pointer, offset, nil); ok {
						target := &Deref{Pointer: projected, Width: width, TypeInfo: ptr.Elem}
						return preferWholeStructCopy(target, width)
					}
				}
				if target, ok := c.consumeAddress(AddressExpr{Base: pointer, Offset: offset, Deref: true}, width); ok {
					return preferWholeStructCopy(target, width)
				}
			}
		}
	}

	target := c.convertMemoryLValue(mem, width)
	return preferWholeStructCopy(target, width)
}

// preferWholeStructCopy promotes an array-field projection back to its
// enclosing struct when the copy spans that entire struct. Struct assignment
// is valid C even when the struct contains arrays; array assignment is not.
func preferWholeStructCopy(value LValue, width int) LValue {
	field, ok := value.(*FieldAccess)
	if !ok || field.Field == nil || field.Field.Offset != 0 {
		return value
	}

	array, ok := field.ExprType().(*typeinfo.Array)
	if !ok || array.Bytes() != width {
		return value
	}

	base, ok := field.Base.(LValue)
	if !ok {
		return value
	}

	strct, ok := base.ExprType().(*typeinfo.Struct)
	if !ok || strct.SKind != typeinfo.StructKindStruct || strct.Bytes() != width {
		return value
	}

	return base
}

// copyAddressMemoryAccess returns the memory spanned by a copy address.
func copyAddressMemoryAccess(mem machine.MemoryAddress, width int) machine.MemoryAddress {
	mem = normalizeCopyMemoryAccess(mem)
	origin := mem.Origin
	if base, ok := mem.Base.(*machine.Address); ok && mem.Index == nil {
		inner := normalizeCopyMemoryAccess(base.Addr)
		inner.Disp += mem.Disp
		inner.Width = width
		inner.Origin = origin
		return inner
	}
	mem.Width = width
	return mem
}
