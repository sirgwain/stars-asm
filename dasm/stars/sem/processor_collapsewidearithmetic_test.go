package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCollapseCarryAwareWideArithmetic verifies that an ADD/ADC expression is
// rebuilt while an identical lane-wise ADD pair without carry is preserved.
func TestCollapseCarryAwareWideArithmetic(t *testing.T) {
	dx := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dx", Type: typeinfo.I16}}
	dy := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dy", Type: typeinfo.I16}}
	square := func(value Expr) Expr {
		extended := &SignExtend{Parent: value, FromBits: 16, ToBits: 32, TypeInfo: typeinfo.I32}
		return &Binary{TypeInfo: typeinfo.I32, Op: OpMul, LHS: extended, RHS: extended}
	}
	dxSquared := square(dx)
	dySquared := square(dy)
	lowProducer := machine.Meta{InstOff: 0x761a, InstOp: asm.OpADD, InstLen: 2}
	highProducer := machine.Meta{InstOff: 0x761c, InstOp: asm.OpADC, InstLen: 2}
	words := &Words{Words: []Expr{
		&Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Word{Parent: dxSquared, Part: machine.WordHigh}, RHS: &Word{Parent: dySquared, Part: machine.WordHigh}, Producer: highProducer},
		&Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Word{Parent: dxSquared, Part: machine.WordLow}, RHS: &Word{Parent: dySquared, Part: machine.WordLow}, Producer: lowProducer},
	}}

	got, ok := collapseCarryAwareWideArithmetic(words)
	if !ok {
		t.Fatal("collapseCarryAwareWideArithmetic returned false, want ADD/ADC collapse")
	}
	if got := FormatExpr(got); got != "((sext16to32(dx) * sext16to32(dx)) + (sext16to32(dy) * sext16to32(dy)))" {
		t.Fatalf("wide arithmetic = %s", got)
	}

	for i, expr := range words.Words {
		binary := *expr.(*Binary)
		binary.Op = OpSub
		if i == 0 {
			binary.Producer.InstOp = asm.OpSBB
		} else {
			binary.Producer.InstOp = asm.OpSUB
		}
		words.Words[i] = &binary
	}
	got, ok = collapseCarryAwareWideArithmetic(words)
	if !ok || FormatExpr(got) != "((sext16to32(dx) * sext16to32(dx)) - (sext16to32(dy) * sext16to32(dy)))" {
		t.Fatalf("wide subtraction = %v, ok = %v", got, ok)
	}

	withoutCarry := *words.Words[0].(*Binary)
	withoutCarry.Producer.InstOp = asm.OpSUB
	words.Words[0] = &withoutCarry
	if got, ok := collapseCarryAwareWideArithmetic(words); ok {
		t.Fatalf("lane-wise ADD pair collapsed without ADC: %s", FormatExpr(got))
	}

	pointType := &typeinfo.Struct{Name: "POINT", SKind: typeinfo.StructKindStruct, Size: 4}
	leftPoint := &Local{FunctionVar: typeinfo.FunctionVar{Name: "left", Type: pointType}}
	rightPoint := &Local{FunctionVar: typeinfo.FunctionVar{Name: "right", Type: pointType}}
	words.Words = []Expr{
		&Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Part{Base: leftPoint, ByteOff: 2, Width: 2, TypeInfo: typeinfo.I16}, RHS: &Part{Base: rightPoint, ByteOff: 2, Width: 2, TypeInfo: typeinfo.I16}, Producer: highProducer},
		&Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Part{Base: leftPoint, Width: 2, TypeInfo: typeinfo.I16}, RHS: &Part{Base: rightPoint, Width: 2, TypeInfo: typeinfo.I16}, Producer: lowProducer},
	}
	if got, ok := collapseCarryAwareWideArithmetic(words); ok {
		t.Fatalf("aggregate lanes collapsed as scalar arithmetic: %s", FormatExpr(got))
	}
}
