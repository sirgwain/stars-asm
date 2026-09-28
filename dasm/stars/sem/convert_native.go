package sem

import (
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// nativeArgCast adds the casts nativeCast adds, and casts a control ID
// passed as a menu_or_id handle, as CreateWindow's hMenu takes for a child
// window, through uintptr_t to the handle's pointer width.
func nativeArgCast(expr Expr, param *typeinfo.FunctionVar) Expr {
	if param.Semantic == typeinfo.ParamSemanticMenuOrID && typeinfo.IsNative(cExprType(expr), typeinfo.NativeInt) {
		return castTo(castTo(expr, typeinfo.UintPtr), param.Type)
	}
	return nativeCast(expr, param.Type)
}

// nativeCast adds the cast a native Win32 compile needs to pass, store or
// return expr as want, where the original compiler converted implicitly:
//
//   - a pointer to an incompatible pointer type. The code walks byte
//     buffers as records (a uint8_t * as a CYBERINFO *), views records as
//     bytes, and passes list and bitmap header types as their generic or
//     containing forms (a PLPROD * as the PL * LpplReAlloc takes, a
//     BITMAPINFOHEADER * as a BITMAPINFO *).
//   - a pointer, array or handle to a pointer-sized integer, such as a
//     string in LPARAM or a popup HMENU as AppendMenu's UINT_PTR item,
//     including a handle already cast to a narrower integer.
//   - a nonzero constant to a handle (see nativeConstCast).
func nativeCast(expr Expr, want typeinfo.Type) Expr {
	// A handle rebuilt as MAKELONG(h, 0) arrives cast to a 32-bit integer,
	// which would truncate a native pointer; cast the handle itself.
	if cast, ok := expr.(*Cast); ok && typeinfo.IsNative(want, typeinfo.NativeIntPtr) && holdsAddress(cExprType(cast.Value)) {
		return castTo(cast.Value, want)
	}
	have := cExprType(expr)
	switch {
	case needsPointerCast(have, want),
		typeinfo.IsNative(want, typeinfo.NativeIntPtr) && holdsAddress(have):
		return castTo(expr, want)
	}
	return nativeConstCast(expr, want)
}

// nativeCompareOperands gives two compared pointers a common type: the
// address of an array compared with a pointer to its elements decays to the
// array, as the source wrote psz == szWork, and a pointer to another type is
// cast to the left operand's type, such as a byte pointer compared with a
// heap block pointer. Only equality may mix void * with another pointer, so
// a relational comparison casts the void * side to the other's type.
func nativeCompareOperands(op CompareOp, lhs, rhs Expr) (Expr, Expr, bool) {
	changed := false
	if decayed, ok := decayedArrayAddress(lhs, cExprType(rhs)); ok {
		lhs, changed = decayed, true
	}
	if decayed, ok := decayedArrayAddress(rhs, cExprType(lhs)); ok {
		rhs, changed = decayed, true
	}
	lhsType, rhsType := cExprType(lhs), cExprType(rhs)
	relational := op != CompareEQ && op != CompareNE
	switch {
	case needsPointerCast(rhsType, lhsType):
		rhs, changed = castTo(rhs, lhsType), true
	case relational && isVoidPointer(lhsType) && typeinfo.IsPointer(rhsType) && !isVoidPointer(rhsType):
		lhs, changed = castTo(lhs, rhsType), true
	case relational && isVoidPointer(rhsType) && typeinfo.IsPointer(lhsType) && !isVoidPointer(lhsType):
		rhs, changed = castTo(rhs, lhsType), true
	}
	return lhs, rhs, changed
}

// isVoidPointer reports whether typ is void *.
func isVoidPointer(typ typeinfo.Type) bool {
	ptr, ok := typ.(*typeinfo.Pointer)
	return ok && isVoid(ptr.Elem)
}

// nativePointerDifference rewrites a Win16 pointer difference, which the
// compiler computes on offset words, as a native one: LOWORD(lpb) - pb and
// LOWORD(lpb) - LOWORD(lpbBase) become lpb - pb and lpb - lpbBase, with the
// right pointer cast to the left's type when their pointees differ.
func nativePointerDifference(sub *Binary) (Expr, Expr, bool) {
	if sub.Op != OpSub {
		return nil, nil, false
	}
	lhs, lhsWord := farPointerOffset(sub.LHS)
	rhs, rhsWord := farPointerOffset(sub.RHS)
	lhsType, rhsType := cExprType(lhs), cExprType(rhs)
	if !lhsWord && !rhsWord || !typeinfo.IsPointer(lhsType) || !typeinfo.IsPointer(rhsType) {
		return nil, nil, false
	}
	if needsPointerCast(rhsType, lhsType) {
		rhs = castTo(rhs, lhsType)
	}
	return lhs, rhs, true
}

// farPointerOffset returns the pointer whose offset word expr reads, or expr
// itself when it is not such a read.
func farPointerOffset(expr Expr) (Expr, bool) {
	part, ok := expr.(*Part)
	if !ok || part.ByteOff != 0 || part.Width != 2 || !typeinfo.IsFarPointer(part.Base.ExprType()) {
		return expr, false
	}
	return part.Base, true
}

// decayedArrayAddress returns the address of an array typed as a pointer to
// its first element when the other side of a comparison points to that
// element type, which the C output writes as the bare array name.
func decayedArrayAddress(expr Expr, other typeinfo.Type) (Expr, bool) {
	addr, ok := expr.(*AddressOf)
	if !ok {
		return nil, false
	}
	array, ok := addr.Target.ExprType().(*typeinfo.Array)
	if !ok || addressOfPrintsArray(addr, array) {
		return nil, false
	}
	otherPtr, ok := other.(*typeinfo.Pointer)
	if !ok || needsPointerCast(&typeinfo.Pointer{Elem: array.Elem}, otherPtr) {
		return nil, false
	}
	return &AddressOf{Target: addr.Target, TypeInfo: &typeinfo.Pointer{Elem: array.Elem, Class: otherPtr.Class}}, true
}

// nativeReturnCast adds the casts nativeCast adds to a returned value, and
// converts an integer call result returned as a handle through uintptr_t.
// That return is where the original fell off the end of a handle-returning
// function, leaving the last call's result in AX, as ClickInShipOrders
// does after ReleaseDC.
func nativeReturnCast(expr Expr, ret typeinfo.Type) Expr {
	if _, call := expr.(*Call); call && typeinfo.IsNative(ret, typeinfo.NativePointer) && typeinfo.IsNative(cExprType(expr), typeinfo.NativeInt) {
		return castTo(castTo(expr, typeinfo.UintPtr), ret)
	}
	return nativeCast(expr, ret)
}

// nativeConstCast casts a nonzero integer constant stored as a handle, such
// as HWND_TOPMOST's 0xFFFF or a COLOR_ index as a class brush. Win16
// handles are 16-bit, so the constant is sign-extended: 0xFFFF becomes
// (HWND)-1 as the native headers define HWND_TOPMOST.
func nativeConstCast(expr Expr, want typeinfo.Type) Expr {
	c, ok := expr.(*Const)
	if !ok || c.U64 == 0 || !typeinfo.IsNative(want, typeinfo.NativePointer) {
		return expr
	}
	return castTo(&Const{TypeInfo: typeinfo.I16, U64: c.U64 & 0xffff}, want)
}

// holdsAddress reports whether a value of type typ holds an address
// natively: a pointer, an array decaying to one, or a handle.
func holdsAddress(typ typeinfo.Type) bool {
	return typeinfo.IsPointer(typ) || typeinfo.IsArray(typ) || typeinfo.IsNative(typ, typeinfo.NativePointer)
}

// castTo casts expr to typ, written as a C type name.
func castTo(expr Expr, typ typeinfo.Type) *Cast {
	return &Cast{Value: expr, To: typeinfo.TypeDecl(typ, ""), TypeInfo: typ}
}

// cExprType returns the type C gives expr. Address-of expressions and
// function references may carry the pointer type their consumer expected,
// but C types &x as a pointer to x's type and a function name as a pointer
// to that function.
func cExprType(expr Expr) typeinfo.Type {
	switch e := expr.(type) {
	case *AddressOf:
		if array, ok := e.Target.ExprType().(*typeinfo.Array); ok && addressOfPrintsArray(e, array) {
			return &typeinfo.Pointer{Elem: array.Elem}
		}
		return &typeinfo.Pointer{Elem: e.Target.ExprType()}
	case *FunctionRef:
		return &typeinfo.Pointer{Elem: e.Function}
	}
	return expr.ExprType()
}

// addressOfPrintsArray reports whether the C output writes the address of
// an array as the bare array name, which decays to a pointer to its first
// element: when the address's pointer type is compatible with the element
// type, as IR lowering decides.
func addressOfPrintsArray(addr *AddressOf, array *typeinfo.Array) bool {
	ptr, ok := addr.TypeInfo.(*typeinfo.Pointer)
	if !ok {
		return false
	}
	return ptr.IsCStringPointer() && array.IsCStringArray() || typeinfo.IsCallCompatible(ptr.Elem, array.Elem)
}

// needsPointerCast reports whether a pointer, or an array decaying to one,
// needs a cast to convert to the pointer type want: its pointee differs and
// neither side is void * or an unprototyped function. Integer pointees of
// one size that differ only in signedness convert with a warning, so they
// need no cast.
func needsPointerCast(have, want typeinfo.Type) bool {
	wantPtr, ok := want.(*typeinfo.Pointer)
	if !ok {
		return false
	}
	var haveElem typeinfo.Type
	switch h := have.(type) {
	case *typeinfo.Pointer:
		haveElem = h.Elem
	case *typeinfo.Array:
		haveElem = h.Elem
	default:
		return false
	}
	if isVoid(haveElem) || isVoid(wantPtr.Elem) || typeinfo.Equals(haveElem, wantPtr.Elem) {
		return false
	}
	// A function type without parameters, such as FARPROC, is declared ()
	// with its parameters unspecified; C converts it to and from any
	// function pointer.
	haveFn, haveIsFn := haveElem.(*typeinfo.Function)
	wantFn, wantIsFn := wantPtr.Elem.(*typeinfo.Function)
	if haveIsFn && wantIsFn && (len(haveFn.Params) == 0 || len(wantFn.Params) == 0) {
		return false
	}
	haveInt, ok := haveElem.(*typeinfo.Primitive)
	if !ok {
		return true
	}
	wantInt, ok := wantPtr.Elem.(*typeinfo.Primitive)
	return !ok || haveInt.TypeKind != typeinfo.KInt || wantInt.TypeKind != typeinfo.KInt || haveInt.Size != wantInt.Size
}

// isVoid reports whether typ is void.
func isVoid(typ typeinfo.Type) bool {
	p, ok := typ.(*typeinfo.Primitive)
	return ok && p.TypeKind == typeinfo.KVoid
}
