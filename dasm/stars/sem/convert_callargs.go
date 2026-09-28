package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertCallArgs converts call arguments using the callee parameter types when known.
func (c *machineConverter) convertCallArgs(fn *typeinfo.Function, values []machine.Value) []Expr {
	out := make([]Expr, len(values))
	messageCall, isMessageCall := c.ctx.messageCallInfo(fn, values)
	for i, value := range values {
		if isMessageCall && i == messageCall.index {
			// Name the message by its target's window class; control
			// messages share numbers across classes.
			out[i] = &Const{TypeInfo: messageCall.enum, U64: uint64(messageCall.value)}
			continue
		}
		var expected typeinfo.Type
		var param *typeinfo.FunctionVar
		if fn != nil && i < len(fn.Params) {
			param = &fn.Params[i]
			expected = param.Type
			if messageType := c.ctx.messageCallArgumentType(fn, values, i); messageType != nil {
				// The message payload type recovers the value, but the
				// callee's parameter is still the generic WPARAM or LPARAM.
				out[i] = c.convertValueTyped(value, messageType)
				if !typeinfo.Equals(messageType, param.Type) {
					out[i] = &Cast{Value: out[i], To: param.Type.String(), TypeInfo: param.Type}
				}
				continue
			}
			if expr, ok := c.convertResourceIDArg(value, param); ok {
				out[i] = expr
				continue
			}
			if _, ok := expected.(*typeinfo.Enum); ok && param.Semantic == typeinfo.ParamSemanticResourceNameOrID {
				// An enum names the numeric IDs; any other value is the
				// resource's string name.
				expected = typeinfo.LpStr
			}
			if expr, ok := c.messagePayloadCast(value, expected); ok {
				out[i] = expr
				continue
			}
		} else if fn != nil && machineVarArgFarPointer(value) {
			// Win16 varargs only pass segment:offset pairs for far pointers,
			// which wsprintf-style callees consume as %s strings.
			expected = typeinfo.LpStr
		}
		out[i] = c.convertValueTyped(value, expected)
	}
	return out
}

// convertValueTyped converts one machine value with an optional expected call type.
func (c *machineConverter) convertValueTyped(value machine.Value, expected typeinfo.Type) Expr {
	if expected != nil {
		if phi, ok := value.(*machine.PhiValue); ok {
			return c.convertPhiTyped(phi, expected)
		}
		if expr, ok := c.convertAddressArgTyped(value, expected); ok {
			return expr
		}
		if load, ok := value.(*machine.Load); ok {
			if storage, ok := c.resolveAddressLValue(load.Addr, load.Addr.Width, expected); ok {
				return storage
			}
		}
		if expr, ok := c.convertSymbolValueTyped(value, expected); ok {
			return expr
		}
		if expr, ok := c.convertComputedAddressArgTyped(value, expected); ok {
			return expr
		}
	}
	return coerceConvertedValue(c.convertValue(value), expected)
}

// coerceConvertedValue removes machine register-preservation details that are
// outside the width required by the source-level destination.
func coerceConvertedValue(expr Expr, expected typeinfo.Type) Expr {
	if expected == nil || expected.Bytes() != 1 {
		return expr
	}
	replacement, ok := expr.(*Byte)
	if !ok || replacement.Value == nil {
		return expr
	}
	return byteProjection(replacement.Value, machine.ByteLow)
}

// convertSymbolValueTyped resolves a machine value through the function-scoped
// symbol resolver before falling back to structural conversion.
func (c *machineConverter) convertSymbolValueTyped(value machine.Value, expected typeinfo.Type) (Expr, bool) {
	path, ok := c.ctx.symbols.symbolFromValueTyped(value, expected)
	if !ok {
		return nil, false
	}
	ptr, pointerExpected := expected.(*typeinfo.Pointer)
	_, words := value.(*machine.StackWords)
	_, binary := value.(*machine.Binary)
	addressValue := words && typeinfo.IsFarPointer(expected) || binary && typeinfo.IsPointer(expected)
	if pointerExpected && addressValue && !typeinfo.IsPointer(path.Type()) {
		// A resolved address such as rgplr[i]+0x80 names a field inside an
		// aggregate, but its symbolic path still has PLAYER as its result
		// type. Project it as an address with width zero so the field array is
		// preserved for pointer compatibility and array decay; using the
		// expected pointer width here would produce Part(field, 0, 2/4).
		if offset, ok := path.(*symresolve.SymbolOffset); ok && offset.Offset != 0 {
			if target, ok := c.convertSymbolAddressPath(path); ok {
				if lvalue, ok := target.(LValue); ok {
					return convertAddressArgTargetTyped(lvalue, expected, ptr)
				}
			}
		}

		// The resolved address may be a containing aggregate whose subobject
		// at this same address is the expected pointee type. For example,
		// SHDEF begins with HUL at offset zero.
		if expr, ok := c.convertSymbolPath(path, path.Type()); ok && !addressWordIsLoad(value) {
			if _, raw := expr.(*SymbolRef); !raw {
				if target, ok := expr.(LValue); ok {
					return c.typedAddressArg(target, target, 0, expected, ptr), true
				}
			}
		}
		if typeinfo.IsCallCompatible(ptr.Elem, path.Type()) {
			return convertAddressArgTargetTyped(&SymbolRef{Path: path}, expected, ptr)
		}
	}
	if pointerExpected && binary && !typeinfo.IsPointer(path.Type()) {
		// Keep computed pointer values on the address-projection path. A
		// symbol path for the addressed byte describes the destination
		// object, but consuming a further offset from that byte would
		// incorrectly turn pointer arithmetic into a Part of the byte.
		if target, ok := c.convertSymbolPath(path, path.Type()); ok {
			if lvalue, ok := target.(LValue); ok {
				if decayed, ok := decayArrayLValue(lvalue, expected); ok {
					return decayed, true
				}
			}
		}
		return nil, false
	}
	if offset, ok := path.(*symresolve.SymbolOffset); ok && addressValue && typeinfo.IsPointer(offset.Base.Type()) {
		base, ok := c.convertSymbolPath(offset.Base, offset.Base.Type())
		if !ok {
			return nil, false
		}
		if projected, ok := projectPointerAddress(base, offset.Offset, nil); ok {
			return projected, true
		}
		magnitude := offset.Offset
		negative := magnitude < 0
		if negative {
			magnitude = -magnitude
		}
		var displacement Expr = &Const{TypeInfo: typeinfo.I16, U64: uint64(magnitude)}
		if negative {
			displacement = &Unary{TypeInfo: typeinfo.I16, Op: OpNeg, X: displacement}
		}
		return &PointerOffset{
			Pointer:  base,
			Offset:   displacement,
			TypeInfo: expected,
		}, true
	}
	if offset, ok := path.(*symresolve.SymbolOffset); ok && offset.Offset > 0 {
		if base, ok := c.convertSymbolPath(offset.Base, offset.Base.Type()); ok {
			if lvalue, ok := base.(LValue); ok {
				if projected, ok := c.consumeAddressExpr(AddressExpr{Base: lvalue, Offset: offset.Offset}, expected.Bytes()); ok {
					return projected, true
				}
			}
		}
	}
	return c.convertSymbolPath(path, expected)
}

// addressWordIsLoad reports whether the offset word of an address-valued
// machine value is a pointer loaded from storage rather than a formed address.
// The symbol path of such a value names the storage, not the addressed object.
func addressWordIsLoad(value machine.Value) bool {
	if words, ok := value.(*machine.StackWords); ok {
		value = words.Words[len(words.Words)-1]
	}
	_, ok := unwrapAddressWord(value).(*machine.Load)
	return ok
}

// convertSymbolAddressPath converts a symbolic address path to its addressed
// lvalue without creating a narrow value slice from the terminal object.
func (c *machineConverter) convertSymbolAddressPath(path symresolve.SymbolPath) (Expr, bool) {
	// This handles both near-pointer arithmetic and DS:offset far-pointer
	// pairs after symbol resolution, for example rgplr[i].szName and
	// rgplr[i].szNames. The result is the lvalue being addressed, not a
	// narrow load of that lvalue.
	addr := resolvedAddressFromPath(path)
	if addr.base == nil {
		return nil, false
	}
	base, ok := c.convertSymbolPath(addr.base, addr.base.Type())
	if !ok {
		return nil, false
	}
	return c.consumeAddressProjection(AddressExpr{
		Base:   base,
		Offset: addr.offset,
		Deref:  addr.deref,
	}, 0)
}

// convertSymbolPath converts a resolved symbol path into the corresponding
// semantic expression node.
func (c *machineConverter) convertSymbolPath(path symresolve.SymbolPath, expected typeinfo.Type) (Expr, bool) {
	switch p := path.(type) {
	case *symresolve.SymbolRoot:
		switch symbol := p.Symbol.(type) {
		case *typeinfo.FunctionVar:
			return &Local{FunctionVar: *symbol}, true
		case *typeinfo.GlobalVar:
			return &Global{GlobalVar: symbol}, true
		default:
			return nil, false
		}
	case *symresolve.SymbolField:
		base, ok := c.convertSymbolPath(p.Base, p.Base.Type())
		if !ok {
			return nil, false
		}
		return &FieldAccess{Base: base, Field: p.Field}, true
	case *symresolve.SymbolBitfield:
		base, ok := c.convertSymbolPath(p.Base, p.Base.Type())
		if !ok {
			return nil, false
		}
		return &FieldAccess{Base: base, Field: p.Field}, true
	case *symresolve.SymbolTerm:
		base, ok := c.convertSymbolPath(p.Base, p.Base.Type())
		if !ok {
			return nil, false
		}
		var index Expr
		if p.Index != nil {
			index, ok = c.convertSymbolPath(p.Index, p.Index.Type())
		} else if p.IndexVal != nil {
			index = c.convertValue(p.IndexVal)
			ok = true
		}
		if !ok || index == nil {
			return nil, false
		}
		elem := indexElementType(base.ExprType())
		if elem == nil || elem.Bytes() <= 0 || p.Scale%elem.Bytes() != 0 {
			return &SymbolRef{Path: path}, true
		}
		// A stride that is a multiple of the element size indexes by a
		// scaled count, as with szWork[(i - 1278) * 30] into a char buffer.
		index = scaledArrayIndexTerm(index, p.Scale/elem.Bytes())
		return &ArrayIndex{Base: base, Index: index, TypeInfo: elem}, true
	case *symresolve.SymbolDeref:
		base, ok := c.convertSymbolPath(p.Base, p.Base.Type())
		if !ok || !typeinfo.IsPointer(base.ExprType()) {
			return nil, false
		}
		width := p.Type().Bytes()
		return &Deref{Pointer: base, Width: width, TypeInfo: p.Type()}, true
	case *symresolve.SymbolOffset:
		base, ok := c.convertSymbolPath(p.Base, p.Base.Type())
		if !ok {
			return nil, false
		}
		if p.Offset == 0 {
			return base, true
		}
		if lvalue, ok := base.(LValue); ok && p.Offset >= 0 {
			width := 0
			if expected != nil {
				width = expected.Bytes()
			}
			if width <= 0 && p.Result != nil {
				width = p.Result.Bytes()
			}
			if projected, ok := c.consumeAddressExpr(AddressExpr{Base: lvalue, Offset: p.Offset}, width); ok {
				return projected, true
			}
		}
		return &SymbolRef{Path: path}, true
	case *symresolve.SymbolConst:
		return &Const{TypeInfo: p.Type(), U64: uint64(p.Const.Val), Origin: p.Const.Origin, Fixup: p.Const.Fixup}, true
	case *symresolve.SymbolLiteral:
		switch literal := p.Literal.(type) {
		case *typeinfo.Function:
			return &FunctionRef{Function: literal, TypeInfo: expected}, true
		case string:
			return &StringLiteral{Text: literal, TypeInfo: p.Type()}, true
		default:
			return &StringLiteral{Text: p.String(), TypeInfo: p.Type()}, true
		}
	default:
		return &SymbolRef{Path: path}, true
	}
}

// convertAddressArgTyped preserves machine address operands as source address-of expressions.
func (c *machineConverter) convertAddressArgTyped(value machine.Value, expected typeinfo.Type) (Expr, bool) {
	ptrType, ok := expected.(*typeinfo.Pointer)
	if !ok {
		return nil, false
	}

	// A non-zero constant passed where a near pointer is expected is
	// potentially a DS-relative address. Resolve the addressed aggregate,
	// not a value of pointer-width at that address.
	if cn, ok := value.(*machine.Const); ok &&
		cn.Val != 0 &&
		ptrType.Class == typeinfo.PtrNear {

		ds := c.ctx.segFromRegister(asm.RegDS)

		if resolved, ok := c.ctx.symbols.addressFromValue(value, ds); ok &&
			resolved.hasBase() {

			if semantic, ok := c.semanticResolvedAddress(resolved); ok {
				if projected, ok := c.consumeAddressProjection(semantic, 0); ok {
					if target, ok := projected.(LValue); ok {
						return c.typedAddressArg(target, target, 0, expected, ptrType), true
					}
				}
			}
		}
	}

	addr, addrOK := value.(*machine.Address)
	if !addrOK {
		words, wordsOK := value.(*machine.StackWords)
		if !wordsOK || len(words.Words) != 2 {
			return nil, false
		}
		if _, ok := words.Words[0].(*machine.Reg); !ok {
			return nil, false
		}
		addr, addrOK = words.Words[1].(*machine.Address)
		if !addrOK {
			return nil, false
		}
	}
	if resolved, ok := c.ctx.symbols.addressFromMemory(addr.Addr, nil); ok {
		if semantic, ok := c.semanticResolvedAddress(resolved); ok {
			if projected, ok := c.consumeAddressProjection(semantic, 0); ok {
				if _, isLValue := projected.(LValue); !isLValue && typeinfo.IsCallCompatible(expected, projected.ExprType()) {
					return projected, true
				}
			}
		}
	}
	width := ptrType.Elem.Bytes()
	if width == 0 {
		width = addr.Addr.Width
	}
	target := c.convertAddressArgTarget(addr.Addr, width, ptrType.Elem)
	base, offset, ok := c.addressArgObject(addr.Addr)
	if !ok {
		base, offset = target, 0
	}
	return c.typedAddressArg(target, base, offset, expected, ptrType), true
}

// typedAddressArg converts an address to the expected pointer type. The
// unique subobject of base at offset whose type is the expected pointee is
// preferred, for example &btn.rc where BTN begins with RECT rc, or the szName
// array beginning a ZIPPROD passed as char *. Otherwise target, the
// resolver's lvalue at the address, is used, with an explicit cast when its
// type is not compatible; the address itself is never lowered to a value.
func (c *machineConverter) typedAddressArg(target LValue, base LValue, offset int, expected typeinfo.Type, ptrType *typeinfo.Pointer) Expr {
	if projected, ok := c.typedAddressTarget(base, offset, expected); ok {
		if expr, ok := convertAddressArgTargetTyped(projected, expected, ptrType); ok {
			return expr
		}
	}
	if expr, ok := convertAddressArgTargetTyped(target, expected, ptrType); ok {
		return expr
	}
	// Integer pointees that differ only in signedness share a representation,
	// and C converts between their pointers without a cast.
	if array, ok := target.ExprType().(*typeinfo.Array); ok && typeinfo.IsSignednessVariant(ptrType.Elem, array.Elem) {
		return target
	}
	if typeinfo.IsSignednessVariant(ptrType.Elem, target.ExprType()) {
		return &AddressOf{Target: target, TypeInfo: expected}
	}
	if part, ok := target.(*Part); ok {
		return objectAddress(part.Base, part.ByteOff, expected)
	}
	return objectAddress(target, 0, expected)
}

// addressArgObject resolves the outermost object addressed by a machine
// address and the byte offset of that address within the object, without
// committing to the resolver's choice of field at the offset.
func (c *machineConverter) addressArgObject(mem machine.MemoryAddress) (LValue, int, bool) {
	sym, ok := c.ctx.symbols.symbolFromAddressAddress(mem)
	if !ok {
		return nil, 0, false
	}
	offset := 0
	for {
		switch path := sym.(type) {
		case *symresolve.SymbolOffset:
			offset += path.Offset
			sym = path.Base
			continue
		case *symresolve.SymbolField:
			if path.Field.Bitfield == nil {
				offset += path.Field.Offset
				sym = path.Base
				continue
			}
		}
		break
	}
	if offset < 0 {
		return nil, 0, false
	}
	expr, ok := c.convertSymbolPath(sym, sym.Type())
	if !ok {
		return nil, 0, false
	}
	base, ok := expr.(LValue)
	return base, offset, ok
}

// convertComputedAddressArgTyped converts address arithmetic that could not
// be resolved as a typed symbol into an explicit pointer to its target.
func (c *machineConverter) convertComputedAddressArgTyped(value machine.Value, expected typeinfo.Type) (Expr, bool) {
	ptrType, ok := expected.(*typeinfo.Pointer)
	if !ok {
		return nil, false
	}
	if _, ok := value.(*machine.Binary); !ok {
		return nil, false
	}
	// Near pointer arithmetic is DS-relative, so a constant base is the
	// address of a global.
	segNum := uint16(0)
	if ptrType.Class == typeinfo.PtrNear {
		segNum = c.ctx.segFromRegister(asm.RegDS)
	}
	resolved, ok := c.resolveAddressValue(value, segNum)
	if !ok {
		return nil, false
	}
	var target LValue
	switch expr := resolved.(type) {
	case LValue:
		target = expr
	case *AddressOf:
		target = expr.Target
	default:
		return nil, false
	}
	return convertAddressArgTargetTyped(target, expected, ptrType)
}

// convertAddressArgTargetTyped applies pointer compatibility and array-decay
// rules to one resolved address argument target.
func convertAddressArgTargetTyped(target LValue, expected typeinfo.Type, ptrType *typeinfo.Pointer) (Expr, bool) {
	// Whole character arrays decay to pointers, but an interior character
	// remains an lvalue whose address must be explicit.
	if ptrType.IsCStringPointer() {
		if _, ok := target.ExprType().(*typeinfo.Array); ok {
			return target, true
		}
		return &AddressOf{Target: target, TypeInfo: expected}, true
	}

	pointee := target.ExprType()
	var address Expr = &AddressOf{Target: target, TypeInfo: expected}
	if decayed, ok := decayArrayLValue(target, expected); ok {
		pointee = target.ExprType().(*typeinfo.Array).Elem
		address = decayed
	} else if ptrType.Elem.Kind() != typeinfo.KVoid && !typeinfo.IsCallCompatible(ptrType.Elem, pointee) {
		return nil, false
	}

	// Call compatibility accepts a struct whose leading fields match the
	// pointee's layout, such as RECT for POINT. C still requires the
	// conversion to be explicit.
	if _, ok := pointee.(*typeinfo.Struct); ok && !typeinfo.Equals(ptrType.Elem, pointee) && ptrType.Elem.Kind() != typeinfo.KVoid {
		return &Cast{To: expected.String(), TypeInfo: expected, Value: address}, true
	}
	return address, true
}

// convertAddressArgTarget resolves the lvalue named by an address-valued
// argument without treating the address as a memory load.
func (c *machineConverter) convertAddressArgTarget(mem machine.MemoryAddress, width int, targetType typeinfo.Type) LValue {
	if sym, ok := c.ctx.symbols.symbolFromAddressAddress(mem); ok {
		if offset, ok := sym.(*symresolve.SymbolOffset); ok && offset.Offset >= 0 {
			if base, ok := c.convertSymbolPath(offset.Base, offset.Base.Type()); ok {
				if lvalue, ok := base.(LValue); ok {
					if projected, ok := c.consumeAddressTarget(lvalue, offset.Offset, width, targetType); ok {
						return projected
					}
				}
			}
		}
		if expr, ok := c.convertSymbolPath(sym, sym.Type()); ok {
			if lvalue, ok := expr.(LValue); ok {
				return lvalue
			}
		}
		if resolved, ok := c.resolveAddressValue(&machine.Address{Addr: mem}, 0); ok {
			if addressOf, ok := resolved.(*AddressOf); ok {
				return addressOf.Target
			}
			if lvalue, ok := resolved.(LValue); ok {
				return lvalue
			}
		}
		return &SymbolRef{Path: sym}
	}
	return c.convertMemoryLValue(mem, width)
}

// consumeAddressTarget resolves an address projection until its expected pointee type is reached.
func (c *machineConverter) consumeAddressTarget(base LValue, offset, width int, targetType typeinfo.Type) (LValue, bool) {
	var current Expr = base
	for {
		if offset == 0 {
			if targetType == nil || targetType.Bytes() == 0 || typeinfo.IsCallCompatible(current.ExprType(), targetType) {
				lvalue, ok := current.(LValue)
				return lvalue, ok
			}
		}
		next, nextOffset, _, changed := c.consumeObjectAddressStep(current, offset, nil, width)
		if !changed {
			return nil, false
		}
		current = next
		offset = nextOffset
	}
}
