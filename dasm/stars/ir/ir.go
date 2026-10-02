package ir

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// Func is deliberately low-level C-like IR. It preserves basic blocks and
// explicit gotos; structural cleanup belongs in later shape passes.
type Func struct {
	Name   string
	Decl   string
	Locals []Local
	Blocks []Block
}

type Local struct {
	Name string
	Type typeinfo.Type
}

type Block struct {
	ID       machine.BlockID
	Label    string
	StartOff uint32
	EndOff   uint32
	Stmts    []Stmt
}

type Stmt interface{ stmt() }
type Expr interface{ expr() }

// Assign stores Src in Dst. Merge marks a store that replaces a merge
// temp's value on one incoming edge, so the stores on each path came from one
// conditional value in the source.
type Assign struct {
	Dst, Src Expr
	Merge    bool
	// Fits marks a store to a temp of a value that already has the temp's C
	// type and fits it, so reading Src where the temp is read gives the
	// same value and C type.
	Fits bool
}

func (*Assign) stmt() {}

type ExprStmt struct{ Expr Expr }

func (*ExprStmt) stmt() {}

type IfGoto struct {
	Cond                  Expr
	TrueLabel, FalseLabel string
}

func (*IfGoto) stmt() {}

// TableJump preserves a computed byte offset and each word-table destination.
// Enums name recovered case values, tried in order, and Char reports a switch
// on a char.
type TableJump struct {
	Index  Expr
	Labels []string
	Enums  []*typeinfo.Enum
	Char   bool
}

// stmt marks TableJump as an IR statement.
func (*TableJump) stmt() {}

// SwitchGoto jumps to the label of the case whose Value equals Index, or to
// Default when none does. Lowering never produces it; region structuring
// builds it from chains of equality tests.
type SwitchGoto struct {
	Index   Expr
	Cases   []SwitchCase
	Default string
}

// stmt marks SwitchGoto as an IR statement.
func (*SwitchGoto) stmt() {}

// SwitchCase is one SwitchGoto destination.
type SwitchCase struct {
	Value Expr
	Label string
}

type Goto struct{ Label string }

func (*Goto) stmt() {}

type Return struct{ Value Expr }

func (*Return) stmt() {}

type Comment struct {
	Text       string
	EffectKind string
	Failures   []LowerFailure
}

func (*Comment) stmt() {}

// LowerFailure identifies an unsupported semantic expression and its path
// within the containing effect.
type LowerFailure struct {
	Kind string
	Path string
}

type Var struct{ Name string }

func (*Var) expr() {}

type IntConst struct {
	Value uint64
	Text  string
}

func (*IntConst) expr() {}

type FloatConst struct{ Value float64 }

func (*FloatConst) expr() {}

// SizeOf is the C sizeof of the named type.
type SizeOf struct{ Type string }

func (*SizeOf) expr() {}

type StringConst struct{ Value string }

func (*StringConst) expr() {}

// Unary applies Op to X, written before X, or after it when Postfix is set,
// as in x++.
type Unary struct {
	Op      string
	X       Expr
	Postfix bool
}

func (*Unary) expr() {}

type Binary struct {
	Op       string
	LHS, RHS Expr
}

func (*Binary) expr() {}

// Cond is a C conditional expression: Cond ? Then : Else.
type Cond struct{ Cond, Then, Else Expr }

// expr marks Cond as an expression.
func (*Cond) expr() {}

type Cast struct {
	Type  string
	Value Expr
}

func (*Cast) expr() {}

type Index struct{ Base, Index Expr }

func (*Index) expr() {}

type Field struct {
	Base    Expr
	Name    string
	Pointer bool
}

func (*Field) expr() {}

type Call struct {
	Target Expr
	Args   []Expr
}

func (*Call) expr() {}

type Macro struct {
	Name string
	Args []Expr
}

func (*Macro) expr() {}

type AddressOf struct{ Target Expr }

func (*AddressOf) expr() {}

// Deref is an access through Pointer. Type is nil for the pointer's own
// pointee; otherwise it is the integer type accessed at ByteOff raw bytes from
// Pointer, so the displacement is never scaled by the pointee size and the
// access does not assume alignment.
type Deref struct {
	Pointer Expr
	ByteOff int
	Type    typeinfo.Type
}

func (*Deref) expr() {}

// PointerOffset advances Pointer by Offset bytes. Type is the resulting
// pointer type, or nil for a byte pointer.
type PointerOffset struct {
	Pointer, Offset Expr
	Type            typeinfo.Type
}

func (*PointerOffset) expr() {}
