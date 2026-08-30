package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertFarPointerTyped resolves whole far pointers only when compatible with the expected type.
func (c *machineConverter) convertFarPointerTyped(value machine.Value, expected typeinfo.Type) (Expr, bool) {
	ptr, ok := value.(*machine.FarPointer)
	if !ok || ptr.Part != machine.FarPointerWhole {
		return nil, false
	}
	if expr, ok := c.resolveDirectFarPointerTyped(ptr, expected); ok {
		return expr, true
	}
	if expr, ok := c.resolveStackFarPointerTyped(ptr, expected); ok {
		return expr, true
	}
	if expr, ok := c.resolveFarPointerAddressTyped(ptr, expected); ok {
		return expr, true
	}
	off, _ := ptr.Offset.(*machine.Load)
	seg, _ := ptr.Segment.(*machine.Load)
	if off != nil && seg != nil {
		field, ok := c.resolveIndexedFarPointerField(off.Access, seg.Access, expected)
		if !ok {
			field, ok = c.resolveIndexedFarPointerField(seg.Access, off.Access, expected)
		}
		if ok {
			return field, true
		}
	}
	return collapseTypedFarPointerCallArg(c.ctx, c.convertValue(value), &typeinfo.FunctionVar{Type: expected})
}

// resolveStackFarPointerTyped resolves SS:offset far pointers to local addresses.
func (c *machineConverter) resolveStackFarPointerTyped(ptr *machine.FarPointer, expected typeinfo.Type) (Expr, bool) {
	expectedPtr, ok := expected.(*typeinfo.Pointer)
	seg, segOK := ptr.Segment.(*machine.Scalar)
	if !ok || !typeinfo.IsFarPointer(expected) || !segOK || seg.Name != "ss" {
		return nil, false
	}
	addr, ok := ptr.Offset.(*machine.Address)
	if !ok {
		return nil, false
	}
	if _, ok := addr.Access.Base.(*machine.FrameBase); !ok || addr.Access.Index != nil {
		return nil, false
	}
	width := expectedPtr.Elem.Bytes()
	if width == 0 {
		width = addr.Access.Width
	}
	target := c.convertMemoryLValue(addr.Access, width)
	switch target.(type) {
	case *RawMemory, *Memory:
		return nil, false
	}
	if !typeinfo.IsCallCompatible(expectedPtr.Elem, target.ExprType()) {
		return nil, false
	}
	return &AddressOf{Target: target, TypeInfo: expected}, true
}

// resolveFarPointerAddressTyped resolves whole far pointer values to typed address expressions.
func (c *machineConverter) resolveFarPointerAddressTyped(ptr *machine.FarPointer, expected typeinfo.Type) (Expr, bool) {
	rootLoad, indexBytes, ok := splitFarPointerAddressBase(ptr.Offset, ptr.Segment)
	if !ok {
		return nil, false
	}
	rootAccess := rootLoad.Access
	rootAccess.Width = 4
	root := c.convertMemoryLValue(rootAccess, rootAccess.Width)
	switch root.(type) {
	case *RawMemory, *Memory:
		return nil, false
	}
	addr := AddressExpr{Base: root, Deref: true}
	if indexBytes != nil {
		indexAddr, ok := c.machineAddressExpr(indexBytes)
		if !ok || indexAddr.base != nil {
			return nil, false
		}
		addr.Offset += indexAddr.offset
		for _, term := range indexAddr.terms {
			addr.Terms = append(addr.Terms, ScaledTerm{Expr: c.convertIndexExpr(term.value), Scale: term.scale})
		}
	}
	target, ok := c.consumeAddress(addr, 0)
	if !ok {
		return nil, false
	}
	if typeinfo.IsCallCompatible(expected, target.ExprType()) {
		return target, true
	}
	return &AddressOf{Target: target, TypeInfo: expected}, true
}

// resolveDirectFarPointerTyped resolves concrete far-pointer call arguments.
func (c *machineConverter) resolveDirectFarPointerTyped(ptr *machine.FarPointer, expected typeinfo.Type) (Expr, bool) {
	if !typeinfo.IsFarPointer(expected) {
		return nil, false
	}
	off, offOK := ptr.Offset.(*machine.Const)
	seg, segOK := ptr.Segment.(*machine.Const)
	if !offOK || !segOK {
		return nil, false
	}
	if off.Val == 0 && seg.Val == 0 {
		// nil pointer
		return &Const{U64: 0, TypeInfo: typeinfo.U32, Origin: off.Origin}, true
	}

	segNum := uint16(seg.Val)
	if fx := seg.Fixup; fx != nil &&
		fx.Source == asm.FixupSourceSegment &&
		fx.Target == asm.FixupTargetInternalRef {

		segNum = fx.TargetSegNum
	}

	if typeinfo.IsFunctionPointer(expected) {
		if f := c.ctx.sdb.GetFunctionByAddr(typeinfo.Addr{Seg: segNum, Off: uint32(off.Val)}); f != nil {
			return &FunctionRef{Function: f, TypeInfo: expected}, true
		}
	}

	if global, ok := c.ctx.res.ResolveGlobal(uint16(seg.Val), uint32(off.Val), 0); ok {
		if global.FieldOff == 0 {
			return &Global{GlobalVar: global.Global}, true
		}
		ptrType, ok := expected.(*typeinfo.Pointer)
		if ok {
			width := ptrType.Elem.Bytes()
			target, ok := c.ctx.resolveFieldStorage(global.Global, global.FieldOff, width)
			if ok && typeinfo.IsCallCompatible(ptrType.Elem, target.ExprType()) {
				return &AddressOf{Target: target, TypeInfo: expected}, true
			}
		}
	}
	ptrType, ok := expected.(*typeinfo.Pointer)
	if !ok || !ptrType.IsCStringPointer() {
		return nil, false
	}
	if text, ok := c.ctx.res.ResolveLiteral(uint16(seg.Val), uint32(off.Val)); ok {
		return &StringLiteral{TypeInfo: expected, Text: text}, true
	}
	return nil, false
}

// resolveIndexedFarPointerField resolves far-pointer words loaded from a pointer array field.
func (c *machineConverter) resolveIndexedFarPointerField(off, seg machine.MemoryAccess, expected typeinfo.Type) (Expr, bool) {
	if expected.Bytes() != 4 || off.Width != 2 || seg.Width != 2 {
		return nil, false
	}
	if !farPointerWordStorage(off, seg) {
		return nil, false
	}
	array, ok := c.resolveIndexedFarPointerArray(off)
	if !ok {
		return nil, false
	}
	field, ok := c.consumeAddressExpr(AddressExpr{Base: array, Offset: off.Disp}, expected.Bytes())
	if !ok || !typeinfo.IsCallCompatible(expected, field.ExprType()) {
		return nil, false
	}
	return field, true
}

// resolveIndexedFarPointerArray resolves the pointer array element behind an access.
func (c *machineConverter) resolveIndexedFarPointerArray(access machine.MemoryAccess) (Expr, bool) {
	rootSeg, ok := access.Seg.(*machine.Load)
	if !ok {
		return nil, false
	}
	rootLoad, indexBytes, ok := splitIndexedFarPointerBase(access.Base, rootSeg.Access)
	if !ok || indexBytes == nil {
		return nil, false
	}
	rootAccess := rootLoad.Access
	rootAccess.Width = 4
	root, ok := c.ctx.resolveMemoryPath(rootAccess)
	if !ok {
		return nil, false
	}
	elem := indexElementType(root.Type())
	if elem == nil {
		return nil, false
	}
	index, ok := arrayScaledIndex(indexBytes, elem.Bytes())
	if !ok {
		return nil, false
	}
	indexExpr := c.convertIndexExpr(index)
	return &ArrayIndex{Base: &SymbolRef{Path: root}, Index: indexExpr, TypeInfo: elem}, true
}

// splitIndexedFarPointerBase finds the loaded far-pointer offset root and byte index.
func splitIndexedFarPointerBase(value machine.Value, seg machine.MemoryAccess) (*machine.Load, machine.Value, bool) {
	if load, ok := value.(*machine.Load); ok && farPointerWordStorage(load.Access, seg) {
		return load, nil, true
	}
	binary, ok := value.(*machine.Binary)
	if !ok || binary.Op != machine.ValueOpAdd {
		return nil, nil, false
	}
	if load, rest, ok := splitIndexedFarPointerBase(binary.LHS, seg); ok {
		return load, joinIndexTerms(rest, binary.RHS), true
	}
	if load, rest, ok := splitIndexedFarPointerBase(binary.RHS, seg); ok {
		return load, joinIndexTerms(binary.LHS, rest), true
	}
	return nil, nil, false
}

// joinIndexTerms rebuilds the non-root byte-index expression.
func joinIndexTerms(a, b machine.Value) machine.Value {
	if a == nil {
		return b
	}
	if b == nil {
		return a
	}
	return machine.BinaryVal(machine.ValueOpAdd, a, b)
}

// convertFarPointerMemoryLValue resolves segment:offset memory through a known pointer variable.
func (c *machineConverter) convertFarPointerMemoryLValue(access machine.MemoryAccess, width int) (LValue, bool) {
	ptrLoad, ok := farPointerMemoryLoad(access)
	if !ok {
		return nil, false
	}
	pointer := c.convertMemoryLValue(ptrLoad.Access, ptrLoad.Access.Width)
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
