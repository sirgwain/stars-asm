package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

type coalesceCopiesProcessor struct{}

type copyAddress struct {
	access machine.MemoryAccess
}

// ProcessMachineBlock coalesces adjacent contiguous copy effects in one machine block.
func (p *coalesceCopiesProcessor) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	changed := false
	effects := make([]machine.Effect, 0, len(b.Effects))
	for i := 0; i < len(b.Effects); {
		copyEffect, ok := b.Effects[i].(machine.CopyEffect)
		if !ok || copyEffect.Width <= 0 {
			effects = append(effects, b.Effects[i])
			i++
			continue
		}

		dst, dstOK := normalizeCopyAddress(copyEffect.Dst)
		src, srcOK := normalizeCopyAddress(copyEffect.Src)
		if !dstOK || !srcOK {
			effects = append(effects, b.Effects[i])
			i++
			continue
		}

		width := copyEffect.Width
		j := i + 1
		for ; j < len(b.Effects); j++ {
			next, ok := b.Effects[j].(machine.CopyEffect)
			if !ok || next.Width <= 0 {
				break
			}
			nextDst, nextDstOK := normalizeCopyAddress(next.Dst)
			nextSrc, nextSrcOK := normalizeCopyAddress(next.Src)
			if !nextDstOK || !nextSrcOK {
				break
			}
			if !copyAddressAt(dst, width).equals(nextDst) || !copyAddressAt(src, width).equals(nextSrc) {
				break
			}
			width += next.Width
		}

		if j == i+1 {
			effects = append(effects, b.Effects[i])
			i++
			continue
		}

		copyEffect.Width = width
		effects = append(effects, copyEffect)
		changed = true
		i = j
	}
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// normalizeCopyAddress returns a comparable copy address with constant offset in Disp.
func normalizeCopyAddress(value machine.Value) (copyAddress, bool) {
	addr, ok := value.(*machine.Address)
	if !ok {
		return copyAddress{}, false
	}
	access := normalizeCopyMemoryAccess(addr.Access)
	access.Origin = machine.Origin{}
	return copyAddress{access: access}, true
}

// normalizeCopyMemoryAccess folds additive constant base tails into memory displacement.
func normalizeCopyMemoryAccess(access machine.MemoryAccess) machine.MemoryAccess {
	for {
		if constant, ok := access.Base.(*machine.Const); ok && access.Index == nil {
			access.Base = nil
			access.Disp += int(constant.Val)
			continue
		}
		binary, ok := access.Base.(*machine.Binary)
		if !ok || binary.Op != machine.ValueOpAdd {
			return access
		}
		constant, ok := binary.RHS.(*machine.Const)
		if !ok {
			return access
		}
		access.Base = binary.LHS
		access.Disp += int(constant.Val)
	}
}

// copyAddressAt returns addr advanced by byteOff bytes.
func copyAddressAt(addr copyAddress, byteOff int) copyAddress {
	addr.access.Disp += byteOff
	return addr
}

// equals reports whether two copy addresses describe the same machine address.
func (addr copyAddress) equals(other copyAddress) bool {
	return addr.access.Disp == other.access.Disp &&
		addr.access.Scale == other.access.Scale &&
		machine.ValueEquals(addr.access.Seg, other.access.Seg) &&
		machine.ValueEquals(addr.access.Base, other.access.Base) &&
		machine.ValueEquals(addr.access.Index, other.access.Index)
}
