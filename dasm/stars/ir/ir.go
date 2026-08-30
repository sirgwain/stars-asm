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

type Assign struct{ Dst, Src Expr }

func (*Assign) stmt() {}

type ExprStmt struct{ Expr Expr }

func (*ExprStmt) stmt() {}

type IfGoto struct {
	Cond                  Expr
	TrueLabel, FalseLabel string
}

func (*IfGoto) stmt() {}

type Goto struct{ Label string }

func (*Goto) stmt() {}

type Return struct{ Value Expr }

func (*Return) stmt() {}

type Comment struct{ Text string }

func (*Comment) stmt() {}

type Var struct{ Name string }

func (*Var) expr() {}

type IntConst struct {
	Value uint64
	Text  string
}

func (*IntConst) expr() {}

type FloatConst struct{ Value float64 }

func (*FloatConst) expr() {}

type StringConst struct{ Value string }

func (*StringConst) expr() {}

type Unary struct {
	Op         string
	X          Expr
	Functional bool
}

func (*Unary) expr() {}

type Binary struct {
	Op       string
	LHS, RHS Expr
}

func (*Binary) expr() {}

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

type Deref struct {
	Pointer Expr
	ByteOff int
}

func (*Deref) expr() {}

type PointerOffset struct{ Pointer, Offset Expr }

func (*PointerOffset) expr() {}
