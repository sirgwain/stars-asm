package typeinfo

import (
	"fmt"
	"strings"
)

var U8 = &Primitive{TypeKind: KInt, Name: "uint8_t", Size: 1, Signed: false}
var U16 = &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2, Signed: false}
var U32 = &Primitive{TypeKind: KInt, Name: "uint32_t", Size: 4, Signed: false}
var I16 = &Primitive{TypeKind: KInt, Name: "int16_t", Size: 2, Signed: true}

// UintPtr is C's uintptr_t, the integer a Win16 16-bit ID passes through on
// its way to a native pointer type.
var UintPtr = &Primitive{TypeKind: KInt, Name: "uintptr_t", Size: 2, Native: NativeIntPtr}
var I32 = &Primitive{TypeKind: KInt, Name: "int32_t", Size: 4, Signed: true}

// F80 represents x87 register precision and the 10-byte extended-real spill format.
var F80 = &Primitive{TypeKind: KFloat, Name: "long double", Size: 10}
var Double = &Primitive{TypeKind: KFloat, Name: "double", Size: 8}
var LpStr = &Pointer{Elem: &Primitive{TypeKind: KInt, Name: "char", Size: 1, Signed: true}, Class: PtrFar}

// UintForWidth returns the unsigned integer type of width bytes.
func UintForWidth(width int) Type {
	switch width {
	case 1:
		return U8
	case 2:
		return U16
	case 4:
		return U32
	}
	return &Primitive{TypeKind: KInt, Name: fmt.Sprintf("uint%d_t", width*8), Size: width}
}

// IntForWidth returns the signed integer type of width bytes.
func IntForWidth(width int) Type {
	switch width {
	case 2:
		return I16
	case 4:
		return I32
	}
	return &Primitive{TypeKind: KInt, Name: fmt.Sprintf("int%d_t", width*8), Size: width, Signed: true}
}

type Type interface {
	Kind() Kind
	Bytes() int
	String() string
}

// Primitive describes a scalar, built-in type.
type Primitive struct {
	TypeKind Kind
	Name     string
	Size     int
	Signed   bool

	// Native is how the native Win32 headers declare this integer type.
	Native NativeKind
}

// NativeKind is how the native Win32 headers declare a Win16 integer type.
type NativeKind uint8

const (
	NativeInt     NativeKind = iota // an integer, as in Win16
	NativePointer                   // a pointer, such as a window handle
	NativeIntPtr                    // an integer wide enough for a pointer, such as LPARAM
)

func (p *Primitive) Kind() Kind { return p.TypeKind }
func (p *Primitive) Bytes() int { return p.Size }
func (p *Primitive) String() string {
	if p.Name == "" {
		return p.Kind().String()
	}
	return p.Name
}

func (p *Primitive) WithName(name string) *Primitive {
	cp := *p
	cp.Name = name
	return &cp
}

func (p *Primitive) IsCharLikeByteType() bool {
	switch p.Name {
	case "char", "const char", "signed char", "unsigned char", "int8_t":
		return true
	default:
		return false
	}
}

// Pointer describes a Win16 pointer type.
type Pointer struct {
	// Name preserves a C pointer typedef, such as LPCSTR, while Elem and
	// Class describe Win16 storage.
	Name  string
	Elem  Type
	Class PtrClass
}

func (p *Pointer) Kind() Kind { return KPointer }

// String returns the pointer typedef name, or the pointee followed by *.
func (p *Pointer) String() string {
	if p.Name != "" {
		return p.Name
	}
	if p.Elem == nil {
		return "*"
	}
	return typeString(p.Elem) + " *"
}

func (p *Pointer) Bytes() int {
	switch p.Class {
	case PtrFar, PtrHuge:
		return 4
	default:
		return 2
	}
}

// IsCStringPointer reports whether a type represents a C byte-string pointer,
// including named aliases such as LPCSTR whose pointee is char.
func (p *Pointer) IsCStringPointer() bool {
	switch v := p.Elem.(type) {
	case *Primitive:
		return v.IsCharLikeByteType()
	case *Array:
		return v.IsCStringArray()
	}

	return false
}

// Array describes a fixed-length array type.
type Array struct {
	// Name preserves a C array typedef while Elem and Count describe Win16 storage.
	Name  string
	Elem  Type
	Count int
}

func (a *Array) Kind() Kind { return KArray }
func (a *Array) String() string {
	if a.Name != "" {
		return a.Name
	}
	return fmt.Sprintf("%s[%d]", typeString(a.Elem), a.Count)
}

func (a *Array) Bytes() int {
	if a.Elem == nil || a.Count <= 0 {
		return 0
	}
	elemBytes := a.Elem.Bytes()
	if elemBytes <= 0 {
		return 0
	}
	return elemBytes * a.Count
}

// IsCStringArray returns true if the elements of this array are chars or a cstring type char
func (p *Array) IsCStringArray() bool {
	switch v := p.Elem.(type) {
	case *Primitive:
		return v.IsCharLikeByteType()
	}

	return false
}

// Kind is the high-level shape of a type.
type Kind int

const (
	KInvalid Kind = iota
	KVoid
	KBool
	KInt     // sized integer (signed/unsigned)
	KFloat   // float/double/long double
	KStruct  // named struct
	KUnion   // named union
	KPointer // near/far/huge pointer
	KArray   // fixed-length array
	KFunc    // function type / signature
	KTypedef // alias name around an underlying type
)

func (k Kind) String() string {
	switch k {
	case KInvalid:
		return "invalid"
	case KVoid:
		return "void"
	case KBool:
		return "bool"
	case KInt:
		return "int"
	case KFloat:
		return "float"
	case KStruct:
		return "struct"
	case KUnion:
		return "union"
	case KPointer:
		return "pointer"
	case KArray:
		return "array"
	case KFunc:
		return "func"
	case KTypedef:
		return "typedef"
	default:
		return fmt.Sprintf("Kind(%d)", uint8(k))
	}
}

// PtrClass matters in Win16.
type PtrClass int

const (
	PtrDefault PtrClass = iota // if you don't know; printing can omit qualifier
	PtrNear
	PtrFar  // 32-bit seg:off
	PtrHuge // rare; but keep slot
)

func (p PtrClass) String() string {
	switch p {
	case PtrDefault:
		return ""
	case PtrNear:
		return "near"
	case PtrFar:
		return "far"
	case PtrHuge:
		return "huge"
	default:
		return fmt.Sprintf("PtrClass(%d)", uint8(p))
	}
}

//
// type string helpers
//

func typeString(typ Type) string {
	if typ == nil {
		return ""
	}
	return typ.String()
}

// TypeDecl outputs a c style declaration for a type with a given variable name
func TypeDecl(typ Type, name string) string {
	if typ == nil {
		return name
	}
	if name == "" {
		// A function pointer needs the abstract declarator ret (*)(params)
		// to be a C type name, as in a cast.
		if p, ok := typ.(*Pointer); ok && p.Name == "" {
			if _, ok := p.Elem.(*Function); ok {
				return pointerDecl(*p, "")
			}
		}
		return typeString(typ)
	}
	switch t := typ.(type) {
	case *Pointer:
		return pointerDecl(*t, name)
	case *Array:
		return arrayDecl(*t, name)
	case *Function:
		return functionDecl(*t, name)
	default:
		base := typeString(typ)
		if base == "" {
			return name
		}
		return base + " " + name
	}
}

func pointerDecl(p Pointer, name string) string {
	if p.Name != "" {
		return p.Name + " " + name
	}
	if p.Elem == nil {
		return "*" + name
	}
	if a, ok := p.Elem.(*Array); ok && a.Name == "" {
		return TypeDecl(p.Elem, "(*"+name+")")
	}
	return TypeDecl(p.Elem, "*"+name)
}

func isFunctionType(typ Type) bool {
	switch typ.(type) {
	case *Function:
		return true
	default:
		return false
	}
}

func arrayDecl(a Array, name string) string {
	if a.Name != "" {
		return a.Name + " " + name
	}
	if a.Elem == nil {
		return name
	}
	return TypeDecl(a.Elem, fmt.Sprintf("%s[%d]", name, a.Count))
}

func functionDecl(f Function, name string) string {
	params := make([]string, 0, len(f.Params))
	for _, param := range f.Params {
		if param.Name == "" {
			params = append(params, typeString(param.Type))
			continue
		}
		params = append(params, param.String())
	}
	ret := typeString(f.Ret)
	if ret == "" {
		ret = "void"
	}
	if strings.HasPrefix(name, "*") {
		name = "(" + name + ")"
	}
	return fmt.Sprintf("%s %s(%s)", ret, name, strings.Join(params, ", "))
}
