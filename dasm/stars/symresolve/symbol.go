package symresolve

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// SymbolPath describes a resolved source-level path for a machine expression.
type SymbolPath interface {
	symbolPath()
	CDecl() string
	Type() typeinfo.Type
}

// SymbolRoot is the global, parameter, or local root of a symbolic path.
type SymbolRoot struct {
	Symbol typeinfo.Var
}

// symbolPath marks SymbolRoot as a symbolic path node.
func (*SymbolRoot) symbolPath() {}

// CDecl returns the C expression for the root symbol.
func (s *SymbolRoot) CDecl() string {
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

// SymbolField is a field selected from an aggregate path.
type SymbolField struct {
	Base  SymbolPath
	Field *typeinfo.StructField
}

// symbolPath marks SymbolField as a symbolic path node.
func (*SymbolField) symbolPath() {}

// CDecl returns the C expression for a selected aggregate field.
func (s *SymbolField) CDecl() string {
	op := "."
	if _, ok := s.Base.Type().(*typeinfo.Pointer); ok {
		op = "->"
	}
	return s.Base.CDecl() + op + s.Field.Name
}

// Type returns the selected field type.
func (s *SymbolField) Type() typeinfo.Type {
	return s.Field.Type
}

// SymbolDeref is a pointer dereference path node.
type SymbolDeref struct {
	Base SymbolPath
	Elem typeinfo.Type
}

// symbolPath marks SymbolDeref as a symbolic path node.
func (*SymbolDeref) symbolPath() {}

// CDecl returns the C expression for a dereferenced pointer path.
func (s *SymbolDeref) CDecl() string {
	return "(*" + s.Base.CDecl() + ")"
}

// Type returns the dereferenced element type.
func (s *SymbolDeref) Type() typeinfo.Type {
	return s.Elem
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
func (s *SymbolOffset) CDecl() string {
	if s.Offset < 0 {
		return fmt.Sprintf("%s-0x%x", s.Base.CDecl(), -s.Offset)
	}
	return fmt.Sprintf("%s+0x%x", s.Base.CDecl(), s.Offset)
}

// Type returns the offset result type.
func (s *SymbolOffset) Type() typeinfo.Type {
	return s.Result
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
