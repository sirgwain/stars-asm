package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// Expr is a semantic expression node.
type Expr interface {
	expr()
	ExprType() typeinfo.Type
}

// LValue is an expression that can also denote addressable storage.
type LValue interface {
	Expr
	lvalue()
}

// Effect is a semantic effect node.
type Effect interface {
	effect()
	EffectMeta() machine.Meta
}

// Op identifies a semantic unary or binary operation.
type Op uint8

const (
	OpUnknown Op = iota
	OpAdd
	OpSub
	OpMul
	OpDiv
	OpMod
	OpAnd
	OpOr
	OpXor
	OpShl
	OpShr
	OpNeg
	OpNot
)

func (op Op) Invert() Op {
	switch op {
	case OpAdd:
		return OpSub
	case OpSub:
		return OpAdd
	case OpMul:
		return OpDiv
	case OpDiv:
		return OpMul
	case OpShl:
		return OpShr
	case OpShr:
		return OpShl
	case OpNeg:
		return OpNeg
	case OpNot:
		return OpNot
	default:
		return OpUnknown
	}
}

// CompareOp identifies a semantic comparison operation.
type CompareOp uint8

const (
	CompareUnknown CompareOp = iota
	CompareEQ
	CompareNE
	CompareLT
	CompareLE
	CompareGT
	CompareGE
)

// Func is a semantic function body produced from machine effects.
type Func struct {
	CFG    *machine.CFG
	Blocks []Block
}

// Block is a semantic block produced from one machine effect block.
type Block struct {
	ID      machine.BlockID
	Effects []Effect
}

// Local is a function local or parameter expression.
type Local struct {
	typeinfo.FunctionVar
}

// expr marks Local as an expression.
func (*Local) expr() {}

// lvalue marks Local as assignable storage.
func (*Local) lvalue() {}

// ExprType returns the local variable type.
func (v *Local) ExprType() typeinfo.Type { return v.FunctionVar.Type }

// Global is a global variable expression.
type Global struct {
	*typeinfo.GlobalVar
}

// expr marks Global as an expression.
func (*Global) expr() {}

// lvalue marks Global as assignable storage.
func (*Global) lvalue() {}

// ExprType returns the global variable type.
func (v *Global) ExprType() typeinfo.Type { return v.GlobalVar.Type }

// FunctionRef is a resolved source-level function pointer expression.
type FunctionRef struct {
	*typeinfo.Function
	TypeInfo typeinfo.Type
}

// expr marks FunctionRef as an expression.
func (*FunctionRef) expr() {}

// ExprType returns the function pointer type.
func (v *FunctionRef) ExprType() typeinfo.Type { return v.TypeInfo }

// CallResult is a semantic placeholder for a machine call result.
type CallResult struct {
	*typeinfo.Function
	TypeInfo typeinfo.Type
	InstOff  uint32
}

// expr marks CallResult as an expression.
func (*CallResult) expr() {}

// ExprType returns the call result type.
func (v *CallResult) ExprType() typeinfo.Type { return v.TypeInfo }

// Const is an integer constant expression.
type Const struct {
	TypeInfo typeinfo.Type
	U64      uint64
	Origin   *machine.Origin
	Fixup    *asm.Fixup
}

// expr marks Const as an expression.
func (*Const) expr() {}

// ExprType returns the constant type.
func (v *Const) ExprType() typeinfo.Type { return v.TypeInfo }

func (c *Const) Int64() (int64, bool) {
	if !typeinfo.IsIntLike(c.TypeInfo) {
		return 0, false
	}

	switch c.TypeInfo.Bytes() {
	case 1:
		return int64(int8(c.U64)), true
	case 2:
		return int64(int16(c.U64)), true
	case 4:
		return int64(int32(c.U64)), true
	default:
		return 0, false
	}
}

// Const is an integer constant expression.
type Register struct {
	Val    asm.Reg
	SegNum uint16
}

// expr marks Const as an expression.
func (*Register) expr() {}

// ExprType returns the constant type.
func (v *Register) ExprType() typeinfo.Type { return typeinfo.U16 }

// StringLiteral is a resolved static C string literal.
type StringLiteral struct {
	TypeInfo typeinfo.Type
	Text     string
}

// expr marks StringLiteral as an expression.
func (*StringLiteral) expr() {}

// ExprType returns the string literal pointer type.
func (v *StringLiteral) ExprType() typeinfo.Type { return v.TypeInfo }

// ResourceID is an integer resource identifier passed through a pointer-typed Win16 resource parameter.
type ResourceID struct {
	Value    Expr
	TypeInfo typeinfo.Type
}

// expr marks ResourceID as an expression.
func (*ResourceID) expr() {}

// ExprType returns the resource parameter type.
func (v *ResourceID) ExprType() typeinfo.Type { return v.TypeInfo }

// FloatConst is a floating-point constant expression.
type FloatConst struct {
	TypeInfo typeinfo.Type
	F64      float64
}

// expr marks FloatConst as an expression.
func (*FloatConst) expr() {}

// ExprType returns the floating-point constant type.
func (v *FloatConst) ExprType() typeinfo.Type { return v.TypeInfo }

// Unary is a semantic unary expression.
type Unary struct {
	TypeInfo typeinfo.Type
	Op       Op
	X        Expr
}

// expr marks Unary as an expression.
func (*Unary) expr() {}

// ExprType returns the unary expression type.
func (v *Unary) ExprType() typeinfo.Type { return v.TypeInfo }

// Binary is a semantic binary expression.
type Binary struct {
	TypeInfo typeinfo.Type
	Op       Op
	LHS      Expr
	RHS      Expr
}

// expr marks Binary as an expression.
func (*Binary) expr() {}

// ExprType returns the binary expression type.
func (v *Binary) ExprType() typeinfo.Type { return v.TypeInfo }

// Byte is a semantic byte projection or replacement within a parent word.
type Byte struct {
	Parent   Expr
	Part     machine.BytePart
	Value    Expr
	TypeInfo typeinfo.Type
}

// expr marks Byte as an expression.
func (*Byte) expr() {}

// ExprType returns the byte expression type.
func (v *Byte) ExprType() typeinfo.Type { return v.TypeInfo }

// Cast converts an expression to another source-level type.
type Cast struct {
	Value    Expr
	To       string
	TypeInfo typeinfo.Type
}

// expr marks Cast as an expression.
func (*Cast) expr() {}

// ExprType returns the cast result type.
func (v *Cast) ExprType() typeinfo.Type { return v.TypeInfo }

// ArrayIndex is an array or pointer element selected by an expression.
type ArrayIndex struct {
	Base     Expr
	Index    Expr
	TypeInfo typeinfo.Type
}

// expr marks ArrayIndex as an expression.
func (*ArrayIndex) expr() {}

// lvalue marks ArrayIndex as assignable storage.
func (*ArrayIndex) lvalue() {}

// ExprType returns the indexed element type.
func (v *ArrayIndex) ExprType() typeinfo.Type { return v.TypeInfo }

// FieldAccess is a field selected from a semantic aggregate expression.
type FieldAccess struct {
	Base  Expr
	Field *typeinfo.StructField
}

// expr marks FieldAccess as an expression.
func (*FieldAccess) expr() {}

// lvalue marks FieldAccess as assignable storage.
func (*FieldAccess) lvalue() {}

// ExprType returns the selected field type.
func (v *FieldAccess) ExprType() typeinfo.Type { return v.Field.Type }

// SymbolRef is a resolved source-level symbol path expression.
type SymbolRef struct {
	Path symresolve.SymbolPath
}

// expr marks SymbolRef as an expression.
func (*SymbolRef) expr() {}

// lvalue marks SymbolRef as assignable storage.
func (*SymbolRef) lvalue() {}

// ExprType returns the resolved symbol path type.
func (v *SymbolRef) ExprType() typeinfo.Type { return v.Path.Type() }

// Compare is a semantic comparison expression.
type Compare struct {
	Op  CompareOp
	LHS Expr
	RHS Expr
}

// expr marks Compare as an expression.
func (*Compare) expr() {}

// ExprType returns the comparison expression type.
func (v *Compare) ExprType() typeinfo.Type { return typeinfo.U16 }

// SignExtend is a signed widening expression from the machine layer.
type SignExtend struct {
	Parent   Expr
	FromBits int
	ToBits   int
	TypeInfo typeinfo.Type
}

// expr marks SignExtend as an expression.
func (*SignExtend) expr() {}

// ExprType returns the widened integer type.
func (v *SignExtend) ExprType() typeinfo.Type { return v.TypeInfo }

// Call is a semantic call expression.
type Call struct {
	*typeinfo.Function
	Target Expr
	Args   []Expr
}

// expr marks Call as an expression.
func (*Call) expr() {}

// ExprType returns the function return type.
func (v *Call) ExprType() typeinfo.Type {
	if v.Function == nil {
		return nil
	}
	return v.Function.Ret
}

// Word is a projected word of a wider semantic expression.
type Word struct {
	Parent Expr
	Part   machine.WordPart
}

// expr marks Word as an expression.
func (*Word) expr() {}

// ExprType returns the projected word type.
func (v *Word) ExprType() typeinfo.Type { return typeinfo.U16 }

// FarPointer is a whole far pointer or a projected word from one.
type FarPointer struct {
	Parent   Expr
	Part     machine.FarPointerPart
	Offset   Expr
	Segment  Expr
	TypeInfo typeinfo.Type
}

// expr marks FarPointer as an expression.
func (*FarPointer) expr() {}

// ExprType returns the far pointer expression type.
func (v *FarPointer) ExprType() typeinfo.Type {
	if v.TypeInfo != nil {
		return v.TypeInfo
	}
	return typeinfo.U16
}

// For a far pointer, return the segment as a number, if it's a const or a register
func (v *FarPointer) SegNum() (uint16, bool) {
	if segReg, ok := v.Segment.(*Register); ok {
		if segReg.SegNum == 0 {
			return 0, false
		}
		return segReg.SegNum, true
	}

	if segConst, ok := v.Segment.(*Const); ok {
		segNum := uint16(segConst.U64)
		if fx := segConst.Fixup; fx != nil &&
			fx.Source == asm.FixupSourceSegment &&
			fx.Target == asm.FixupTargetInternalRef {

			segNum = fx.TargetSegNum
		}
		return segNum, true
	}
	return 0, false
}

// PointerOffset is a pointer advanced by a byte offset expression.
type PointerOffset struct {
	Pointer  Expr
	Offset   Expr
	TypeInfo typeinfo.Type
}

// expr marks PointerOffset as an expression.
func (*PointerOffset) expr() {}

// ExprType returns the pointer offset result type.
func (v *PointerOffset) ExprType() typeinfo.Type { return v.TypeInfo }

// Deref is a memory lvalue reached through a resolved pointer expression.
type Deref struct {
	Pointer  Expr
	ByteOff  int
	Width    int
	TypeInfo typeinfo.Type
}

// expr marks Deref as an expression.
func (*Deref) expr() {}

// lvalue marks Deref as assignable storage.
func (*Deref) lvalue() {}

// ExprType returns the dereferenced memory type.
func (v *Deref) ExprType() typeinfo.Type { return v.TypeInfo }

// Words records a multi-word semantic expression before ABI-specific lowering.
type Words struct {
	Words []Expr
}

// expr marks Words as an expression.
func (*Words) expr() {}

// ExprType returns the aggregate word expression type.
func (v *Words) ExprType() typeinfo.Type { return intTypeForWidth(len(v.Words) * 2) }

// Part is a byte range inside a larger addressable expression.
type Part struct {
	Base     LValue
	ByteOff  int
	Width    int
	TypeInfo typeinfo.Type
}

// expr marks Part as an expression.
func (*Part) expr() {}

// lvalue marks Part as assignable storage.
func (*Part) lvalue() {}

// ExprType returns the part type.
func (v *Part) ExprType() typeinfo.Type { return v.TypeInfo }

// AddressOf is an address-of expression.
type AddressOf struct {
	Target   LValue
	TypeInfo typeinfo.Type
}

// expr marks AddressOf as an expression.
func (*AddressOf) expr() {}

// ExprType returns the pointer type produced by address-of.
func (v *AddressOf) ExprType() typeinfo.Type { return v.TypeInfo }

// Temp is a synthetic local used to preserve a machine value across mutation.
type Temp struct {
	Name     string
	ID       machine.ValueID
	Source   LValue
	TypeInfo typeinfo.Type
}

// expr marks Temp as an expression.
func (*Temp) expr() {}

// lvalue marks Temp as assignable storage.
func (*Temp) lvalue() {}

// ExprType returns the temp value type.
func (v *Temp) ExprType() typeinfo.Type { return v.TypeInfo }

// MergeArm records one control-flow arm for a merged expression.
type MergeArm struct {
	Block machine.BlockID
	Value Expr
}

// Merge is a value selected from multiple control-flow arms.
type Merge struct {
	TypeInfo typeinfo.Type
	Join     machine.BlockID
	Arms     []MergeArm
}

// expr marks Merge as an expression.
func (*Merge) expr() {}

// ExprType returns the merge result type.
func (v *Merge) ExprType() typeinfo.Type { return v.TypeInfo }

// RawValue keeps an unlowered machine value inside semantic IR.
type RawValue struct {
	Value    machine.Value
	TypeInfo typeinfo.Type
}

// expr marks RawValue as an expression.
func (*RawValue) expr() {}

// lvalue marks RawValue as transitional assignable storage.
func (*RawValue) lvalue() {}

// ExprType returns the known raw value type.
func (v *RawValue) ExprType() typeinfo.Type { return v.TypeInfo }

// RawMemory keeps an unlowered machine memory access inside semantic IR.
type RawMemory struct {
	Access   machine.MemoryAccess
	TypeInfo typeinfo.Type
}

// expr marks RawMemory as an expression.
func (*RawMemory) expr() {}

// lvalue marks RawMemory as assignable storage.
func (*RawMemory) lvalue() {}

// ExprType returns the known raw memory type.
func (v *RawMemory) ExprType() typeinfo.Type { return v.TypeInfo }

// Memory is an unresolved memory lvalue with lowered address expressions.
type Memory struct {
	Seg      Expr
	Base     Expr
	Disp     int
	Width    int
	Index    Expr
	Scale    int
	TypeInfo typeinfo.Type
}

// expr marks Memory as an expression.
func (*Memory) expr() {}

// lvalue marks Memory as assignable storage.
func (*Memory) lvalue() {}

// ExprType returns the memory access type.
func (v *Memory) ExprType() typeinfo.Type { return v.TypeInfo }

// RawEffect keeps an unlowered machine effect inside semantic IR.
type RawEffect struct {
	Effect   machine.Effect
	MetaInfo machine.Meta
}

// effect marks RawEffect as a semantic effect.
func (*RawEffect) effect() {}

// EffectMeta returns the machine origin for RawEffect.
func (e *RawEffect) EffectMeta() machine.Meta { return e.MetaInfo }

// Assign stores a source expression into an addressable destination.
type Assign struct {
	MetaInfo machine.Meta
	Dst      LValue
	Src      Expr
}

// effect marks Assign as a semantic effect.
func (*Assign) effect() {}

// EffectMeta returns the machine origin for Assign.
func (e *Assign) EffectMeta() machine.Meta { return e.MetaInfo }

// CallEffect is a semantic call used for void calls or calls kept as effects.
type CallEffect struct {
	MetaInfo machine.Meta
	Call     *Call
	Result   Expr
}

// effect marks CallEffect as a semantic effect.
func (*CallEffect) effect() {}

// EffectMeta returns the machine origin for CallEffect.
func (e *CallEffect) EffectMeta() machine.Meta { return e.MetaInfo }

// Branch is a semantic conditional terminator.
type Branch struct {
	MetaInfo   machine.Meta
	Cond       Expr
	TrueBlock  machine.BlockID
	FalseBlock machine.BlockID
}

// effect marks Branch as a semantic effect.
func (*Branch) effect() {}

// EffectMeta returns the machine origin for Branch.
func (e *Branch) EffectMeta() machine.Meta { return e.MetaInfo }

// Jump is a semantic unconditional terminator.
type Jump struct {
	MetaInfo machine.Meta
	To       machine.BlockID
}

// effect marks Jump as a semantic effect.
func (*Jump) effect() {}

// EffectMeta returns the machine origin for Jump.
func (e *Jump) EffectMeta() machine.Meta { return e.MetaInfo }

// Return is a semantic return effect.
type Return struct {
	MetaInfo machine.Meta
	Value    Expr
}

// effect marks Return as a semantic effect.
func (*Return) effect() {}

// EffectMeta returns the machine origin for Return.
func (e *Return) EffectMeta() machine.Meta { return e.MetaInfo }
