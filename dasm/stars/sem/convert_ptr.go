package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

// convertCopyAddress resolves an address-valued copy operand at copy width.
func (c *machineConverter) convertCopyAddress(value machine.Value, width int) (LValue, bool) {
	addr, ok := value.(*machine.Address)
	if !ok {
		return nil, false
	}
	return c.convertMemoryLValue(copyAddressMemoryAccess(addr.Addr, width), width), true
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
