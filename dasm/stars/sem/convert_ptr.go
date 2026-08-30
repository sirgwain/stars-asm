package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertNearPointerAddressTyped resolves DS-relative pointer arithmetic by expected type.
func (c *machineConverter) convertNearPointerAddressTyped(value machine.Value, expected typeinfo.Type) (Expr, bool) {
	expectedPtr, ok := expected.(*typeinfo.Pointer)
	if !ok {
		return nil, false
	}
	base, index, ok := c.splitNearPointerBaseAndIndex(value)
	if !ok {
		return nil, false
	}
	load, ok := base.(*machine.Load)
	if !ok {
		return nil, false
	}
	pointer := wholePointerFromOffsetPart(c.convertPointerBaseLoad(load))
	elem := nearPointerElementType(pointer)
	if elem == nil || !typeinfo.IsCallCompatible(expectedPtr.Elem, elem) {
		return nil, false
	}
	sourceIndex, ok := arrayScaledIndex(index, elem.Bytes())
	if !ok {
		return nil, false
	}
	target := &ArrayIndex{Base: pointer, Index: c.convertIndexExpr(sourceIndex), TypeInfo: elem}
	return &AddressOf{Target: target, TypeInfo: expected}, true
}

// convertCopyAddress resolves an address-valued copy operand at copy width.
func (c *machineConverter) convertCopyAddress(value machine.Value, width int) (LValue, bool) {
	addr, ok := value.(*machine.Address)
	if !ok {
		return nil, false
	}
	access := copyAddressMemoryAccess(addr.Access, width)
	if lvalue, ok := c.convertNearPointerMemoryLValue(access, width); ok {
		return lvalue, true
	}
	return c.convertMemoryLValue(access, width), true
}

// copyAddressMemoryAccess returns the memory spanned by a copy address.
func copyAddressMemoryAccess(access machine.MemoryAccess, width int) machine.MemoryAccess {
	access = normalizeCopyMemoryAccess(access)
	origin := access.Origin
	if base, ok := access.Base.(*machine.Address); ok && access.Index == nil {
		inner := normalizeCopyMemoryAccess(base.Access)
		inner.Disp += access.Disp
		inner.Width = width
		inner.Origin = origin
		return inner
	}
	access.Width = width
	return access
}

// convertNearPointerMemoryLValue resolves DS:offset memory through a known pointer variable.
func (c *machineConverter) convertNearPointerMemoryLValue(access machine.MemoryAccess, width int) (LValue, bool) {
	if !machine.ValueEquals(access.Seg, c.ctx.dsReg) {
		return nil, false
	}
	if access.Index != nil {
		return nil, false
	}
	if lvalue, ok := c.convertIndexedNearPointerMemoryLValue(access, width); ok {
		return lvalue, true
	}
	load, ok := access.Base.(*machine.Load)
	if !ok {
		return nil, false
	}
	pointer := c.convertPointerBaseLoad(load)
	pointer = wholePointerFromOffsetPart(pointer)
	switch pointer.(type) {
	case *RawMemory, *Memory:
		return nil, false
	}
	if lvalue, ok := c.consumeAddress(AddressExpr{Base: pointer, Offset: access.Disp, Deref: true}, width); ok {
		return lvalue, true
	}
	if lvalue, ok := c.ctx.resolvePointerFieldLoad(pointer, access.Disp, width); ok {
		return lvalue, true
	}
	return &Deref{
		Pointer:  pointer,
		ByteOff:  access.Disp,
		Width:    width,
		TypeInfo: derefType(pointer, width),
	}, true
}

// convertIndexedNearPointerMemoryLValue resolves DS:base+index through a near pointer.
func (c *machineConverter) convertIndexedNearPointerMemoryLValue(access machine.MemoryAccess, width int) (LValue, bool) {
	base, index, ok := c.splitNearPointerBaseAndIndex(access.Base)
	if !ok {
		return nil, false
	}
	load, ok := base.(*machine.Load)
	if !ok {
		return nil, false
	}
	pointer := c.convertPointerBaseLoad(load)
	pointer = wholePointerFromOffsetPart(pointer)
	if _, unresolved := pointer.(*RawMemory); unresolved {
		return nil, false
	}
	if _, unresolved := pointer.(*Memory); unresolved {
		return nil, false
	}
	elem := nearPointerElementType(pointer)
	if elem == nil {
		return nil, false
	}
	addr, ok := c.nearPointerAddressExpr(pointer, index, access.Disp)
	if !ok {
		return nil, false
	}
	if lvalue, ok := c.consumeAddress(addr, width); ok {
		return lvalue, true
	}
	return nil, false
}

// nearPointerAddressExpr lowers residual near-pointer byte arithmetic.
func (c *machineConverter) nearPointerAddressExpr(pointer Expr, index machine.Value, disp int) (AddressExpr, bool) {
	addr := machineAddressExpr{base: pointer, offset: disp}
	if indexAddr, ok := c.machineOffsetExpr(index); ok {
		addr.offset += indexAddr.offset
		addr.terms = append(addr.terms, indexAddr.terms...)
	} else {
		addr.terms = append(addr.terms, machineScaledTerm{value: index, scale: 1})
	}
	out := c.semanticAddressExpr(addr)
	out.Deref = true
	return out, true
}

// convertPointerBaseLoad resolves a loaded pointer before address projection.
func (c *machineConverter) convertPointerBaseLoad(load *machine.Load) LValue {
	if load.Access.Index != nil {
		return c.convertMemoryLValue(load.Access, load.Access.Width)
	}
	if _, ok := load.Access.Base.(*machine.FrameBase); ok {
		local, ok := c.ctx.res.ResolveLocal(c.ctx.fs, load.Access.Origin.InstOff, load.Access.Disp)
		if ok && local.FieldOff == 0 {
			return lvalueForLocalAccess(local, load.Access.Width)
		}
	} else if global, ok := c.ctx.resolveGlobal(load.Access); ok && global.FieldOff == 0 {
		return lvalueForGlobalAccess(global, load.Access.Width)
	}
	return c.convertMemoryLValue(load.Access, load.Access.Width)
}

// wholePointerFromOffsetPart recovers a pointer expression from its offset word.
func wholePointerFromOffsetPart(expr LValue) LValue {
	part, ok := expr.(*Part)
	if !ok || part.ByteOff != 0 || part.Width != 2 || !typeinfo.IsPointer(part.Base.ExprType()) {
		return expr
	}
	return part.Base
}

// splitNearPointerBaseAndIndex splits nested near-pointer byte arithmetic at a typed pointer load.
func (c *machineConverter) splitNearPointerBaseAndIndex(value machine.Value) (machine.Value, machine.Value, bool) {
	binary, ok := value.(*machine.Binary)
	if !ok || binary.Op != machine.ValueOpAdd {
		return nil, nil, false
	}
	if load, ok := binary.LHS.(*machine.Load); ok && c.machineLoadIsNearPointer(load) {
		return binary.LHS, binary.RHS, true
	}
	if load, ok := binary.RHS.(*machine.Load); ok && c.machineLoadIsNearPointer(load) {
		return binary.RHS, binary.LHS, true
	}
	if load, rest, ok := c.splitNearPointerBaseAndIndex(binary.LHS); ok {
		return load, joinIndexTerms(rest, binary.RHS), true
	}
	if load, rest, ok := c.splitNearPointerBaseAndIndex(binary.RHS); ok {
		return load, joinIndexTerms(binary.LHS, rest), true
	}
	return nil, nil, false
}

// machineLoadIsNearPointer reports whether load recovers to a typed pointer expression.
func (c *machineConverter) machineLoadIsNearPointer(load *machine.Load) bool {
	pointer := wholePointerFromOffsetPart(c.convertPointerBaseLoad(load))
	return nearPointerElementType(pointer) != nil
}

// nearPointerElementType returns the element type reached through a near pointer.
func nearPointerElementType(pointer Expr) typeinfo.Type {
	ptr, ok := pointer.ExprType().(*typeinfo.Pointer)
	if !ok {
		return nil
	}
	return ptr.Elem
}
