package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertCallArgs converts call arguments using the callee parameter types when known.
func (c *machineConverter) convertCallArgs(fn *typeinfo.Function, values []machine.Value) []Expr {
	out := make([]Expr, len(values))
	for i, value := range values {
		var expected typeinfo.Type
		var param *typeinfo.FunctionVar
		if fn != nil && i < len(fn.Params) {
			param = &fn.Params[i]
			expected = param.Type
			if messageType := messageCallArgumentType(c.ctx.sdb, fn, values, i); messageType != nil {
				expected = messageType
			}
			if expr, ok := c.convertResourceIDArg(value, param); ok {
				out[i] = expr
				continue
			}
		}
		out[i] = c.convertValueTyped(value, expected)
	}
	return out
}

// convertValueTyped converts one machine value with an optional expected call type.
func (c *machineConverter) convertValueTyped(value machine.Value, expected typeinfo.Type) Expr {
	if expected != nil {
		if phi, ok := value.(*machine.PhiValue); ok && typeinfo.IsPointer(expected) {
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
	return c.convertValue(value)
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
	addressValue := words && typeinfo.IsFarPointer(expected) || binary && typeinfo.IsNearPointer(expected)
	if pointerExpected && addressValue && !typeinfo.IsPointer(path.Type()) && typeinfo.IsCallCompatible(ptr.Elem, path.Type()) {
		target := LValue(&SymbolRef{Path: path})
		if expr, ok := c.convertSymbolPath(path, path.Type()); ok {
			if lvalue, ok := expr.(LValue); ok {
				target = lvalue
			}
		}
		return convertAddressArgTargetTyped(target, expected, ptr)
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
		if elem == nil || elem.Bytes() != p.Scale {
			return &SymbolRef{Path: path}, true
		}
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
	return convertAddressArgTargetTyped(target, expected, ptrType)
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
	resolved, ok := c.resolveAddressValue(value)
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

	if ptrType.Elem.Kind() != typeinfo.KVoid && !typeinfo.IsCallCompatible(ptrType.Elem, target.ExprType()) {
		if decayed, ok := decayArrayLValue(target, expected); ok {
			return decayed, true
		}
		return nil, false
	}
	if decayed, ok := decayArrayLValue(target, expected); ok {
		return decayed, true
	}

	return &AddressOf{Target: target, TypeInfo: expected}, true
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
		if resolved, ok := c.resolveAddressValue(&machine.Address{Addr: mem}); ok {
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
