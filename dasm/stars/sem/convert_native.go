package sem

import (
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// nativeArgCast adds the cast a native Win32 compile needs for an argument
// whose Win16 value is valid but whose native types no longer convert
// implicitly: a handle passed as an integer (AppendMenu's popup HMENU), a
// control ID passed as a menu_or_id handle, or any conversion
// nativeAssignCast adds.
func nativeArgCast(expr Expr, param *typeinfo.FunctionVar) Expr {
	have, want := expr.ExprType(), param.Type
	switch {
	case typeinfo.IsNativePointer(have) && isPlainInteger(want):
		return &Cast{Value: expr, To: typeinfo.TypeDecl(want, ""), TypeInfo: want}
	case typeinfo.IsNativePointer(want) && param.Semantic == typeinfo.ParamSemanticMenuOrID && isPlainInteger(have):
		return &Cast{Value: expr, To: typeinfo.TypeDecl(want, ""), TypeInfo: want}
	}
	return nativeAssignCast(expr, want)
}

// nativeAssignCast adds the cast a native Win32 compile needs to pass, store
// or return a value as want: a nonzero constant as a handle (see
// nativeConstCast), or a pointer as a pointer to an incompatible type. The
// original compiler only warned on the pointer conversions, which the code
// uses to walk byte buffers as records (a uint8_t * as a CYBERINFO *), view
// records as bytes, and pass list and bitmap header types as their generic
// or containing forms (a PLPROD * as the PL * LpplReAlloc takes, a
// BITMAPINFOHEADER * as a BITMAPINFO *); C requires a cast for each.
func nativeAssignCast(expr Expr, want typeinfo.Type) Expr {
	if needsPointerCast(cExprType(expr), want) {
		return &Cast{Value: expr, To: typeinfo.TypeDecl(want, ""), TypeInfo: want}
	}
	return nativeConstCast(expr, want)
}

// nativeConstCast casts a nonzero integer constant stored as a handle, such
// as HWND_TOPMOST's 0xFFFF or a COLOR_ index as a class brush. Win16
// handles are 16-bit, so the constant is sign-extended: 0xFFFF becomes
// (HWND)-1 as the native headers define HWND_TOPMOST.
func nativeConstCast(expr Expr, want typeinfo.Type) Expr {
	c, ok := expr.(*Const)
	if !ok || c.U64 == 0 || !typeinfo.IsNativePointer(want) {
		return expr
	}
	value := &Const{TypeInfo: typeinfo.I16, U64: c.U64 & 0xffff}
	return &Cast{Value: value, To: typeinfo.TypeDecl(want, ""), TypeInfo: want}
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

// isPlainInteger reports whether typ is an integer that is also an integer
// in the native headers.
func isPlainInteger(typ typeinfo.Type) bool {
	p, ok := typ.(*typeinfo.Primitive)
	return ok && p.TypeKind == typeinfo.KInt && !p.NativePointer
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
