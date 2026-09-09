package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertCopyAddress resolves an address-valued copy operand at copy width.
func (c *machineConverter) convertCopyAddress(value machine.Value, width int) (LValue, bool) {
	addr, ok := value.(*machine.Address)
	if !ok {
		return nil, false
	}
	mem := copyAddressMemoryAccess(addr.Addr, width)
	if segment, ok := mem.Seg.(*machine.FarPointer); ok {
		if parent, ok := commonFarPointerParent(segment, mem.Base); ok {
			pointer := c.convertValue(parent)
			if mem.Index == nil && typeinfo.IsPointer(pointer.ExprType()) {
				offset := signedWordOffset(uint(mem.Disp))
				ptr := pointer.ExprType().(*typeinfo.Pointer)
				if ptr.Elem.Bytes() == width {
					if projected, ok := projectPointerAddress(pointer, offset, nil); ok {
						return &Deref{Pointer: projected, Width: width, TypeInfo: ptr.Elem}, true
					}
				}
				if target, ok := c.consumeAddress(AddressExpr{Base: pointer, Offset: offset, Deref: true}, width); ok {
					return target, true
				}
			}
		}
	}
	return c.convertMemoryLValue(mem, width), true
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
