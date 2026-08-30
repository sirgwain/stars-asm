package typeinfo

type typePair struct {
	a Type
	b Type
}

// IsCallCompatible reports whether a value of actual type may be passed to a
// parameter of formal type without an explicit cast.
func IsCallCompatible(formal, actual Type) bool {
	return isCallCompatible(formal, actual, make(map[typePair]bool))
}

// isCallCompatible checks call compatibility while guarding recursive types.
func isCallCompatible(formal, actual Type, seen map[typePair]bool) bool {
	if formal == nil || actual == nil {
		return false
	}
	if Equals(formal, actual) {
		return true
	}

	pair := typePair{a: formal, b: actual}
	if seen[pair] {
		return true
	}
	seen[pair] = true

	switch t := formal.(type) {
	case *Primitive:
		v, ok := actual.(*Primitive)
		return ok && primitivesCallCompatible(t, v)
	case *Pointer:
		v, ok := actual.(*Pointer)
		return ok && t.Class == v.Class && isCallCompatible(t.Elem, v.Elem, seen)
	case *Array:
		v, ok := actual.(*Array)
		return ok && t.Count == v.Count && isCallCompatible(t.Elem, v.Elem, seen)
	case *Struct:
		v, ok := actual.(*Struct)
		return ok && structsCallCompatible(t, v, seen)
	case *Function:
		v, ok := actual.(*Function)
		return ok && functionsCallCompatible(t, v, seen)
	default:
		return false
	}
}

// primitivesCallCompatible reports whether scalar values share the same call ABI shape.
func primitivesCallCompatible(formal, actual *Primitive) bool {
	switch formal.TypeKind {
	case KInt:
		return actual.TypeKind == KInt &&
			formal.Size == actual.Size &&
			formal.Signed == actual.Signed
	case KFloat:
		return actual.TypeKind == KFloat && formal.Size == actual.Size
	case KVoid:
		return actual.TypeKind == KVoid
	default:
		return false
	}
}

// structsCallCompatible reports whether actual has the fixed-size header layout formal expects.
func structsCallCompatible(formal, actual *Struct, seen map[typePair]bool) bool {
	if formal.SKind != StructKindStruct || actual.SKind != StructKindStruct {
		return false
	}
	if formal.Size <= 0 || actual.Size < formal.Size {
		return false
	}
	if len(formal.Fields) == 0 || len(actual.Fields) == 0 {
		return false
	}

	for _, field := range formal.Fields {
		if field.Size == 0 {
			continue
		}
		if field.End != field.Offset+field.Size || field.End > formal.Size {
			return false
		}
		if !hasCompatibleFieldAt(actual, field, seen) {
			return false
		}
	}
	return true
}

// hasCompatibleFieldAt reports whether a struct has a matching field shape at an offset.
func hasCompatibleFieldAt(strct *Struct, want StructField, seen map[typePair]bool) bool {
	for _, field := range strct.Fields {
		if field.Offset != want.Offset || field.Size != want.Size {
			continue
		}
		if field.End != field.Offset+field.Size {
			continue
		}
		if bitfieldsCallCompatible(want.Bitfield, field.Bitfield, seen) && isCallCompatible(want.Type, field.Type, seen) {
			return true
		}
	}
	return false
}

// bitfieldsCallCompatible reports whether two bitfield descriptors occupy the same bits.
func bitfieldsCallCompatible(formal, actual *Bitfield, seen map[typePair]bool) bool {
	if formal == nil || actual == nil {
		return formal == nil && actual == nil
	}
	return formal.StorageSize == actual.StorageSize &&
		formal.BitOffset == actual.BitOffset &&
		formal.BitWidth == actual.BitWidth &&
		isCallCompatible(formal.BaseType, actual.BaseType, seen)
}

// functionsCallCompatible reports whether two function signatures are call-compatible.
func functionsCallCompatible(formal, actual *Function, seen map[typePair]bool) bool {
	if formal.Conv != actual.Conv || formal.VarArgs != actual.VarArgs || len(formal.Params) != len(actual.Params) {
		return false
	}
	if !isCallCompatible(formal.Ret, actual.Ret, seen) {
		return false
	}
	for i := range formal.Params {
		if !isCallCompatible(formal.Params[i].Type, actual.Params[i].Type, seen) {
			return false
		}
	}
	return true
}
