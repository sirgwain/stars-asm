package typeinfo

// GetFunctionPointerFunction checks if a type is a function pointer, and if so returns
// the underlying function
func GetFunctionPointerFunction(typ Type) (*Function, bool) {
	if ptr, ok := typ.(*Pointer); ok && isFunctionType(ptr.Elem) {
		if f, ok := ptr.Elem.(*Function); ok {
			return f, true
		}
	}
	return nil, false
}

// UnwrapPointer unwraps a pointer and returns the underlying element, and true if it was unwrapped
func UnwrapPointer(typ Type) (Type, bool) {
	if ptr, ok := typ.(*Pointer); ok {
		return ptr.Elem, true
	}
	return typ, false
}

// IsPointer reports whether a type is represented as a far machine pointer.
func IsPointer(typ Type) bool {
	_, ok := typ.(*Pointer)
	return ok
}

// IsFarPointer reports whether a type is represented as a far machine pointer.
func IsFarPointer(typ Type) bool {
	ptr, ok := typ.(*Pointer)
	if !ok {
		return false
	}
	return ptr.Class == PtrFar || ptr.Class == PtrHuge
}

func IsFunctionPointer(typ Type) bool {
	ptr, ok := typ.(*Pointer)
	if !ok {
		return false
	}
	_, ok = ptr.Elem.(*Function)
	return ok
}

func IsArray(typ Type) bool {
	_, ok := typ.(*Array)
	return ok
}

func IsArrayPointer(typ Type) bool {
	ptr, ok := typ.(*Pointer)
	if !ok {
		return false
	}
	_, ok = ptr.Elem.(*Array)
	return ok
}

func IsIntLike(typ Type) bool {
	if p, ok := typ.(*Primitive); ok {
		return p.Signed && p.TypeKind == KInt
	}
	return false
}
