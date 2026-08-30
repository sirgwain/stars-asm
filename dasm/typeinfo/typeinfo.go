package typeinfo

import (
	"fmt"
	"strings"
)

var U8 = &Primitive{TypeKind: KInt, Name: "uint8_t", Size: 1, Signed: false}
var U16 = &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2, Signed: false}
var U32 = &Primitive{TypeKind: KInt, Name: "uint32_t", Size: 4, Signed: false}
var I32 = &Primitive{TypeKind: KInt, Name: "int32_t", Size: 4, Signed: true}
var Double = &Primitive{TypeKind: KFloat, Name: "double", Size: 8}

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
}

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
	case "char", "const char", "signed char", "unsigned char", "int8_t", "LPSTR", "LPCSTR":
		return true
	default:
		return false
	}
}

// Pointer describes a Win16 pointer type.
type Pointer struct {
	Elem  Type
	Class PtrClass
}

func (p *Pointer) Kind() Kind { return KPointer }
func (p *Pointer) String() string {
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

// IsCStringPointer reports whether a type represents a C byte-string pointer.
// This accepts direct pointer-to-char forms and common Win16 aliases like
// LPCSTR/LPSTR when alias expansion is unavailable.
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
	Elem  Type
	Count int
}

func (a *Array) Kind() Kind { return KArray }
func (a *Array) String() string {
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
	if p.Elem == nil {
		return "*" + name
	}
	if isFunctionType(p.Elem) {
		return TypeDecl(p.Elem, "**"+name)
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
