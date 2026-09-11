package sem

import "github.com/sirgwain/stars-asm/dasm/typeinfo"

// recoverExpectedValue uses a destination or parameter type to recover a
// complete aggregate load or the pointee of an address-valued expression.
func recoverExpectedValue(expr Expr, expected typeinfo.Type) Expr {
	if address, ok := expr.(*AddressOf); ok && typeinfo.IsPointer(expected) && !typeinfo.Equals(address.TypeInfo, expected) {
		next := *address
		next.TypeInfo = expected
		return &next
	}
	aggregate, ok := expected.(*typeinfo.Struct)
	if !ok || aggregate.Bytes() <= 0 {
		return expr
	}
	var pointer Expr
	switch value := expr.(type) {
	case *Part:
		if value.Width != aggregate.Bytes() || typeinfo.IsPointer(value.Base.ExprType()) {
			return expr
		}
		pointer = objectAddress(value.Base, value.ByteOff, &typeinfo.Pointer{Elem: aggregate, Class: typeinfo.PtrNear})
	case *Deref:
		if value.Width != aggregate.Bytes() || value.ByteOff == 0 && typeinfo.Equals(value.TypeInfo, aggregate) {
			return expr
		}
		ptr := value.Pointer.ExprType().(*typeinfo.Pointer)
		offset := signedWordOffset(uint(value.ByteOff))
		if typeinfo.Equals(ptr.Elem, aggregate) {
			if projected, ok := projectPointerAddress(value.Pointer, offset, nil); ok {
				return &Deref{Pointer: projected, Width: value.Width, TypeInfo: aggregate}
			}
		}
		next := *value
		next.ByteOff = offset
		pointer = objectAddress(&next, 0, &typeinfo.Pointer{Elem: aggregate, Class: ptr.Class})
	case LValue:
		array, ok := value.ExprType().(*typeinfo.Array)
		if !ok || array.Bytes() != aggregate.Bytes() {
			return expr
		}
		pointer = objectAddress(value, 0, &typeinfo.Pointer{Elem: aggregate, Class: typeinfo.PtrNear})
	default:
		return expr
	}
	return &Deref{Pointer: pointer, Width: aggregate.Bytes(), TypeInfo: aggregate}
}

// typedAddressTarget selects a unique addressable subobject using the expected
// pointee type. Ambiguous union members and bitfields remain unselected.
func (c *machineConverter) typedAddressTarget(base LValue, offset int, expected typeinfo.Type) (LValue, bool) {
	ptr, pointerExpected := expected.(*typeinfo.Pointer)
	if offset == 0 && (!pointerExpected || ptr.Elem.Kind() == typeinfo.KVoid || typeinfo.Equals(ptr.Elem, base.ExprType())) {
		return base, true
	}
	switch typ := base.ExprType().(type) {
	case *typeinfo.Array:
		if typ.Elem.Bytes() <= 0 || offset < 0 || typ.Count > 0 && offset > typ.Bytes() {
			return nil, false
		}
		if offset == 0 && pointerExpected && typeinfo.Equals(ptr.Elem, typ.Elem) {
			return base, true
		}
		index := &ArrayIndex{Base: base, Index: &Const{TypeInfo: typeinfo.I16, U64: uint64(offset / typ.Elem.Bytes())}, TypeInfo: typ.Elem}
		return c.typedAddressTarget(index, offset%typ.Elem.Bytes(), expected)
	case *typeinfo.Struct:
		var target LValue
		for _, match := range c.unionFieldMatches(base, typ, typ.FieldsContainingOffset(offset)) {
			if match.Field.Bitfield != nil {
				continue
			}
			field := bitfieldFieldAccess(base, match.Field)
			candidate, ok := c.typedAddressTarget(field, match.Off, expected)
			if !ok {
				continue
			}
			if target != nil {
				return nil, false
			}
			target = candidate
		}
		return target, target != nil
	default:
		return nil, false
	}
}

// objectAddress forms a typed address without loading the object. Residual
// offsets use byte arithmetic before conversion to the requested pointer type.
func objectAddress(base LValue, offset int, expected typeinfo.Type) Expr {
	var address Expr
	class := typeinfo.PtrNear
	switch value := base.(type) {
	case *Deref:
		address = value.Pointer
		offset += value.ByteOff
		class = address.ExprType().(*typeinfo.Pointer).Class
	case *ArrayIndex:
		elem := indexElementType(value.Base.ExprType())
		if ptr, ok := value.Base.ExprType().(*typeinfo.Pointer); ok {
			class = ptr.Class
		}
		address = &Binary{TypeInfo: &typeinfo.Pointer{Elem: elem, Class: class}, Op: OpAdd, LHS: value.Base, RHS: value.Index}
	default:
		if array, ok := base.ExprType().(*typeinfo.Array); ok {
			address = base
			// Array expressions decay at this address-valued use.
			if offset != 0 {
				pointer := &typeinfo.Pointer{Elem: array.Elem, Class: class}
				address = &Cast{To: pointer.String(), TypeInfo: pointer, Value: base}
			}
		} else {
			address = &AddressOf{Target: base, TypeInfo: &typeinfo.Pointer{Elem: base.ExprType(), Class: class}}
		}
	}
	if offset != 0 {
		var displacement Expr = &Const{TypeInfo: typeinfo.I16, U64: uint64(absInt(offset))}
		if offset < 0 {
			displacement = &Unary{TypeInfo: typeinfo.I16, Op: OpNeg, X: displacement}
		}
		address = &PointerOffset{Pointer: address, Offset: displacement, TypeInfo: &typeinfo.Pointer{Elem: typeinfo.U8, Class: class}}
	}
	if ptr, ok := expected.(*typeinfo.Pointer); ok {
		actual := address.ExprType()
		if array, ok := actual.(*typeinfo.Array); ok {
			actual = &typeinfo.Pointer{Elem: array.Elem, Class: class}
		}
		if !typeinfo.Equals(ptr, actual) && ptr.Elem.Kind() != typeinfo.KVoid {
			return &Cast{To: ptr.String(), TypeInfo: ptr, Value: address}
		}
	}
	return address
}
