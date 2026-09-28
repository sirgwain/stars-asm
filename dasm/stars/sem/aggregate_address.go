package sem

import "github.com/sirgwain/stars-asm/dasm/typeinfo"

// recoverExpectedValue uses a destination or parameter type to recover a
// complete aggregate load or the pointee of an address-valued expression.
func (c *machineConverter) recoverExpectedValue(expr Expr, expected typeinfo.Type) Expr {
	if projected, ok := c.leadingMemberAddress(expr, expected); ok {
		return projected
	}
	if raw, ok := packedStorageValue(expr, expected); ok {
		return raw
	}
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

// packedStorageValue reinterprets a bitfield-only struct used as an integer
// of the same width as its raw storage word, for example
// *(uint16_t *)&rgmdplr[iPlayer] passed as DoAiTurn's uint16_t wMdPlr or
// assigned from rgplr[iPlayer].wMdPlr.
func packedStorageValue(expr Expr, expected typeinfo.Type) (LValue, bool) {
	want, ok := expected.(*typeinfo.Primitive)
	if !ok || want.TypeKind != typeinfo.KInt {
		return nil, false
	}
	target, ok := expr.(LValue)
	if !ok {
		return nil, false
	}
	strct, ok := target.ExprType().(*typeinfo.Struct)
	if !ok || strct.SKind != typeinfo.StructKindStruct || len(strct.OverlapRegions) != 0 || strct.Bytes() != want.Bytes() {
		return nil, false
	}
	for _, field := range strct.Fields {
		if field.Bitfield == nil {
			return nil, false
		}
	}
	if _, ok := strct.ScalarBitPartition(0, strct.Bytes()*8); !ok {
		return nil, false
	}
	pointer := objectAddress(target, 0, &typeinfo.Pointer{Elem: want, Class: typeinfo.PtrNear})
	return &Deref{Pointer: pointer, Width: want.Bytes(), TypeInfo: want}, true
}

// leadingMemberAddress projects the address of a struct onto its unique
// member at offset zero whose type is the expected pointee, for example
// &lpshdef->hul or &rglpshdef[i][ish].hul where SHDEF begins with HUL hul.
// The address is either a struct pointer value or an AddressOf a struct.
func (c *machineConverter) leadingMemberAddress(expr Expr, expected typeinfo.Type) (Expr, bool) {
	// Only embedded aggregates are projected. A byte or scalar pointer to a
	// record is a raw view of it, not its first member.
	want, ok := expected.(*typeinfo.Pointer)
	if !ok {
		return nil, false
	}
	if _, ok := want.Elem.(*typeinfo.Struct); !ok {
		return nil, false
	}
	var base LValue
	if address, ok := expr.(*AddressOf); ok {
		base = address.Target
	} else if ptr, ok := expr.ExprType().(*typeinfo.Pointer); ok {
		base = &Deref{Pointer: expr, Width: ptr.Elem.Bytes(), TypeInfo: ptr.Elem}
	} else {
		return nil, false
	}
	if _, ok := base.ExprType().(*typeinfo.Struct); !ok || typeinfo.Equals(want.Elem, base.ExprType()) {
		return nil, false
	}
	target, ok := c.typedAddressTarget(base, 0, expected)
	if !ok {
		return nil, false
	}
	// Render members selected through the pointer as p->hul, not (*p).hul.
	return &AddressOf{Target: normalizeBitfieldAggregateBase(target).(LValue), TypeInfo: expected}, true
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
		// Whole elements of residual offset move into the index, as with
		// &szWork[(i - 1278) * 30 + 160] rather than byte arithmetic.
		if size := elem.Bytes(); size > 0 && offset%size == 0 {
			next := *value
			next.Index = offsetArrayIndex(value.Index, offset/size)
			value = &next
			offset = 0
		}
		address = &AddressOf{Target: value, TypeInfo: &typeinfo.Pointer{Elem: elem, Class: class}}
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
		address = &PointerOffset{Pointer: address, Offset: signedIndexConst(offset), TypeInfo: &typeinfo.Pointer{Elem: typeinfo.U8, Class: class}}
	}
	want, _ := expected.(*typeinfo.Pointer)
	return castAddress(address, want, class)
}
