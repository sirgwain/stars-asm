package symresolve

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// SymbolPath describes a resolved source-level path for a machine expression.
type SymbolPath interface {
	symbolPath()
	String() string
	Type() typeinfo.Type
}

// SymbolRoot is the global, parameter, or local root of a symbolic path.
type SymbolRoot struct {
	Symbol typeinfo.Var
}

// symbolPath marks SymbolRoot as a symbolic path node.
func (*SymbolRoot) symbolPath() {}

// String returns the C expression for the root symbol.
func (s *SymbolRoot) String() string {
	return varName(s.Symbol)
}

// Type returns the root symbol type.
func (s *SymbolRoot) Type() typeinfo.Type {
	switch v := s.Symbol.(type) {
	case *typeinfo.GlobalVar:
		return v.Type
	case *typeinfo.FunctionVar:
		return v.Type
	default:
		return nil
	}
}

// SymbolScratch is a synthetic root for unresolved BP-relative scratch storage.
//
// Scratch storage is identified by its function and BP displacement. TypeInfo
// describes the current access view; it is intentionally separate from
// StorageSize because the same storage may be viewed at different widths.
type SymbolScratch struct {
	Function    typeinfo.Addr
	BPOffset    int
	StorageSize int
	TypeInfo    typeinfo.Type
}

// symbolPath marks SymbolScratch as a symbolic path node.
func (*SymbolScratch) symbolPath() {}

// String returns a stable synthetic name for scratch storage.
func (s *SymbolScratch) String() string {
	offset := fmt.Sprintf("p%x", s.BPOffset)
	if s.BPOffset < 0 {
		offset = fmt.Sprintf("m%x", -s.BPOffset)
	}
	return "scratch_bp_" + offset
}

// Type returns the type of the current scratch access view.
func (s *SymbolScratch) Type() typeinfo.Type {
	return s.TypeInfo
}

type SymbolConst struct {
	Const *machine.Const
	Typ   typeinfo.Type
}

func (*SymbolConst) symbolPath() {}

func (s *SymbolConst) String() string {
	return fmt.Sprintf("0x%x", s.Const.Val)
}

func (s *SymbolConst) Type() typeinfo.Type {
	return s.Typ
}

type SymbolLiteral struct {
	Literal any
	Typ     typeinfo.Type
}

func (*SymbolLiteral) symbolPath() {}
func (s *SymbolLiteral) String() string {
	return fmt.Sprintf("%v", s.Literal)
}

func (s *SymbolLiteral) Type() typeinfo.Type {
	return s.Typ
}

type SymbolTerm struct {
	Base     SymbolPath
	Index    SymbolPath
	IndexVal machine.Value
	Scale    int
	Result   typeinfo.Type
}

func (*SymbolTerm) symbolPath() {}
func (s *SymbolTerm) String() string {
	index := ""
	if s.Index != nil {
		index = s.Index.String()
	} else {
		index = s.IndexVal.String()
	}
	if elem, ok := indexedElementType(s.Base.Type()); ok && elem.Bytes() == s.Scale {
		return fmt.Sprintf("%s[%s]", s.Base.String(), index)
	}
	return fmt.Sprintf("%s[%s*0x%x]", s.Base.String(), index, s.Scale)
}

// Type returns the root symbol type.
func (s *SymbolTerm) Type() typeinfo.Type {
	if s.Result != nil {
		return s.Result
	}
	return typeinfo.I16
}

// indexedElementType returns the element type addressed by indexing a path.
func indexedElementType(typ typeinfo.Type) (typeinfo.Type, bool) {
	switch t := typ.(type) {
	case *typeinfo.Pointer:
		return t.Elem, t.Elem != nil
	case *typeinfo.Array:
		return t.Elem, t.Elem != nil
	}
	return nil, false
}

// SymbolField is a field selected from an aggregate path.
type SymbolField struct {
	Base  SymbolPath
	Field *typeinfo.StructField
}

// symbolPath marks SymbolField as a symbolic path node.
func (*SymbolField) symbolPath() {}

// String returns the C expression for a selected aggregate field.
func (s *SymbolField) String() string {
	op := "."
	if _, ok := s.Base.Type().(*typeinfo.Pointer); ok {
		op = "->"
	}
	return s.Base.String() + op + s.Field.Name
}

// Type returns the selected field type.
func (s *SymbolField) Type() typeinfo.Type {
	return s.Field.Type
}

// SymbolBitfield is a logical bitfield selected from an aggregate path.
//
// Base names the immediate containing aggregate. Field carries both the
// source-level type and the physical storage range used to recover the field
// from machine masks and shifts.
type SymbolBitfield struct {
	Base  SymbolPath
	Field *typeinfo.StructField
}

// symbolPath marks SymbolBitfield as a symbolic path node.
func (*SymbolBitfield) symbolPath() {}

// String returns the C expression for a selected bitfield.
func (s *SymbolBitfield) String() string {
	op := "."
	if _, ok := s.Base.Type().(*typeinfo.Pointer); ok {
		op = "->"
	}
	return s.Base.String() + op + s.Field.Name
}

// Type returns the declared base type of the selected bitfield.
func (s *SymbolBitfield) Type() typeinfo.Type {
	return s.Field.Type
}

// SymbolDeref is a pointer dereference path node.
type SymbolDeref struct {
	Base SymbolPath
}

// symbolPath marks SymbolDeref as a symbolic path node.
func (*SymbolDeref) symbolPath() {}

// CDecl returns the C expression for a dereferenced pointer path.
func (s *SymbolDeref) String() string {
	return "(*" + s.Base.String() + ")"
}

// Type returns the dereferenced element type.
func (s *SymbolDeref) Type() typeinfo.Type {
	if derefType, ok := typeinfo.UnwrapPointer(s.Base.Type()); ok {
		return derefType
	}
	return s.Base.Type()
}

// SymbolOffset is an unresolved byte offset from another symbolic path.
type SymbolOffset struct {
	Base   SymbolPath
	Offset int
	Result typeinfo.Type
}

// symbolPath marks SymbolOffset as a symbolic path node.
func (*SymbolOffset) symbolPath() {}

// CDecl returns the C expression for an unresolved byte offset path.
func (s *SymbolOffset) String() string {
	if s.Offset == 0 {
		return s.Base.String()
	}
	if s.Offset < 0 {
		return fmt.Sprintf("%s-0x%x", s.Base.String(), -s.Offset)
	}
	return fmt.Sprintf("%s+0x%x", s.Base.String(), s.Offset)
}

// Type returns the offset result type.
func (s *SymbolOffset) Type() typeinfo.Type {
	return s.Result
}

func Equals(a, b SymbolPath) bool {
	if left, ok := a.(*SymbolBitfield); ok {
		right, rightOK := b.(*SymbolBitfield)
		return rightOK && left.Field == right.Field && Equals(left.Base, right.Base)
	}
	if _, ok := b.(*SymbolBitfield); ok {
		return false
	}
	if left, ok := a.(*SymbolScratch); ok {
		right, rightOK := b.(*SymbolScratch)
		return rightOK && left.Function == right.Function && left.BPOffset == right.BPOffset
	}
	if _, ok := b.(*SymbolScratch); ok {
		return false
	}
	// TODO: make a symresolve.Equals for symbols
	return a.String() == b.String()
}

// varName returns the C identifier for a typeinfo variable.
func varName(v typeinfo.Var) string {
	switch v := v.(type) {
	case *typeinfo.GlobalVar:
		return v.Name
	case *typeinfo.FunctionVar:
		return v.Name
	default:
		panic(fmt.Sprintf("unsupported symbol variable %T", v))
	}
}
