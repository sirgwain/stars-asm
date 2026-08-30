package typeinfo

import "slices"

// Equals reports whether two type descriptions represent the same type.
func Equals(a, b Type) bool {
	if a == nil || b == nil {
		return a == nil && b == nil
	}

	switch ta := a.(type) {
	case *Primitive:
		tb, ok := b.(*Primitive)
		return ok && primitivesEqual(ta, tb)
	case *Pointer:
		tb, ok := b.(*Pointer)
		return ok && pointersEqual(ta, tb)
	case *Array:
		tb, ok := b.(*Array)
		return ok && arraysEqual(ta, tb)
	case *Struct:
		tb, ok := b.(*Struct)
		return ok && structsEqual(ta, tb)
	case *Enum:
		tb, ok := b.(*Enum)
		return ok && enumsEqual(ta, tb)
	case *Function:
		tb, ok := b.(*Function)
		return ok && functionsEqual(ta, tb)
	default:
		return false
	}
}

// primitivesEqual reports whether two primitive types have the same shape and name.
func primitivesEqual(a, b *Primitive) bool {
	return a.TypeKind == b.TypeKind &&
		a.Name == b.Name &&
		a.Size == b.Size &&
		a.Signed == b.Signed
}

// pointersEqual reports whether two pointer types have the same pointer class and element type.
func pointersEqual(a, b *Pointer) bool {
	return a.Class == b.Class && Equals(a.Elem, b.Elem)
}

// arraysEqual reports whether two array types have the same length and element type.
func arraysEqual(a, b *Array) bool {
	return a.Count == b.Count && Equals(a.Elem, b.Elem)
}

// structsEqual reports whether two aggregate types have the same nominal identity.
func structsEqual(a, b *Struct) bool {
	return a.SKind == b.SKind &&
		a.Name == b.Name &&
		a.Typedef == b.Typedef &&
		a.Size == b.Size
}

// enumsEqual reports whether two enum types have the same name, mode, and values.
func enumsEqual(a, b *Enum) bool {
	return a.Name == b.Name &&
		a.EnumKind == b.EnumKind &&
		a.Bytes() == b.Bytes() &&
		slices.Equal(a.Values, b.Values)
}

// functionsEqual reports whether two function signatures are equivalent.
func functionsEqual(a, b *Function) bool {
	if a.Conv != b.Conv || a.VarArgs != b.VarArgs || !Equals(a.Ret, b.Ret) || len(a.Params) != len(b.Params) {
		return false
	}
	return slices.EqualFunc(a.Params, b.Params, func(a, b FunctionVar) bool { return Equals(a.Type, b.Type) })
}
