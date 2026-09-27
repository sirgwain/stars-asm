package templates

import (
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestAggregateWritesExecute checks emitted C against the real HS, PLANET and
// RTXFER metadata, including aliasing, once-only calls, and preservation of
// every bit outside the requested field updates.
func TestAggregateWritesExecute(t *testing.T) {
	cc, err := exec.LookPath("cc")
	if err != nil {
		t.Skip("C compiler unavailable")
	}
	fx := testfixture.Stars(t)
	ctx := sem.NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("GenerateWorld"))
	hsType := fx.SDB.GetStruct("HS")
	planetType := fx.SDB.GetStruct("PLANET")
	var c strings.Builder
	c.WriteString(`#include <stdint.h>
#include <assert.h>
typedef struct { uint16_t grhst; uint16_t iItem:8, cItem:8; } HS;
typedef struct { uint8_t before[24]; uint32_t cDefenses:12, iScanner:5, unused5:5, fArtifact:1, fNoResearch:1, unused2:8; } PLANET;
typedef struct { uint16_t id1, id2; uint8_t grobj1:4, grobj2:4; uint8_t grbitItems; char rgcQuan[1]; } RTXFER;
static HS slots[2]; static int calls;
static int next(void) { assert(calls++ == 0); return 1; }
static uint16_t source(void) { assert(calls++ == 1); return 0x0103; }
int main(void) {
`)
	constant := func(v uint64) sem.Expr { return &sem.Const{TypeInfo: typeinfo.U32, U64: v} }
	binary := func(op sem.Op, l, r sem.Expr) sem.Expr {
		return &sem.Binary{TypeInfo: typeinfo.U32, Op: op, LHS: l, RHS: r}
	}
	emit := func(effects []sem.Effect) {
		t.Helper()
		src := sem.Func{Blocks: []sem.Block{{ID: 1, Effects: effects}}}
		result := &sem.Result{Operands: make(map[machine.AnnotationKey]sem.Annotation), Memory: make(map[string][]sem.Annotation), Values: make(map[string]sem.Annotation)}
		for _, spec := range sem.ProcessorSpecs() {
			if spec.Name == "resolve-late-bitfields" {
				block, _ := spec.Sem(ctx).ProcessBlock(result, src, src.Blocks[0])
				src.Blocks[0] = block
			}
		}
		fn := ir.Lower(src, &typeinfo.Function{Name: "fixture", Ret: typeinfo.U16})
		if fn.Analyze().Untranslated != 0 {
			t.Fatalf("untranslated fixture: %+v", fn.Analyze())
		}
		for _, local := range fn.Locals {
			fmt.Fprintf(&c, "%s;\n", typeinfo.TypeDecl(local.Type, local.Name))
		}
		for _, stmt := range fn.Blocks[0].Stmts {
			c.WriteString(formatIRStmt(stmt) + "\n")
		}
	}
	// Destination and source calls must execute once in that order.
	c.WriteString("{ slots[1].grhst=0x1234;\n")
	slot := &sem.ArrayIndex{Base: &sem.Global{GlobalVar: &typeinfo.GlobalVar{Name: "slots", Type: &typeinfo.Array{Elem: hsType, Count: 2}}}, Index: &sem.Call{Function: &typeinfo.Function{Name: "next", Ret: typeinfo.I16}}, TypeInfo: hsType}
	emit([]sem.Effect{&sem.Assign{Dst: &sem.Part{Base: slot, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, Src: &sem.Call{Function: &typeinfo.Function{Name: "source", Ret: typeinfo.U16}}}})
	c.WriteString("assert(calls==2);assert(slots[1].grhst==0x1234);assert(slots[1].iItem==3);assert(slots[1].cItem==1);}\n")
	// Swap two packed fields using reads from the destination itself.
	c.WriteString("{ HS hs={0x5678,0x12,0x34};\n")
	hs := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "hs", Type: hsType}}
	high := &sem.Word{Parent: hs, Part: machine.WordHigh}
	swapped := binary(sem.OpOr, binary(sem.OpShr, high, constant(8)), binary(sem.OpShl, binary(sem.OpAnd, high, constant(255)), constant(8)))
	emit([]sem.Effect{&sem.Assign{Dst: &sem.Part{Base: hs, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16}, Src: swapped}})
	c.WriteString("assert(hs.grhst==0x5678);assert(hs.iItem==0x34);assert(hs.cItem==0x12);}\n")
	// This is GenerateWorld's combined scanner/defense write at byte 0x18.
	c.WriteString("for(uint32_t seed=0;seed<1000;seed++){uint32_t old=seed*2654435761u; PLANET pl={0};pl.cDefenses=old;pl.iScanner=old>>12;pl.unused5=old>>17;pl.fArtifact=old>>22;pl.fNoResearch=old>>23;pl.unused2=old>>24;\n")
	pl := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "pl", Type: planetType}}
	low := &sem.Part{Base: pl, ByteOff: 0x18, Width: 2, TypeInfo: typeinfo.U16}
	hi := &sem.Part{Base: pl, ByteOff: 0x1a, Width: 2, TypeInfo: typeinfo.U16}
	words := &sem.Words{Words: []sem.Expr{binary(sem.OpOr, binary(sem.OpAnd, hi, constant(0xfffe)), constant(1)), binary(sem.OpOr, binary(sem.OpAnd, low, constant(0x0fff)), constant(0xf000))}}
	emit([]sem.Effect{&sem.Assign{Dst: &sem.Part{Base: pl, ByteOff: 0x18, Width: 4, TypeInfo: typeinfo.U32}, Src: words}})
	c.WriteString("uint32_t got=pl.cDefenses|(pl.iScanner<<12)|(pl.unused5<<17)|(pl.fArtifact<<22)|(pl.fNoResearch<<23)|((uint32_t)pl.unused2<<24);assert(got==((old&0xfffe0fff)|0x1f000));}\n")
	// GenerateWorld's fArtifact write through a planet pointer: a 32-bit shift
	// labelled uint16_t, split into HIWORD/LOWORD lanes of a sign cast.
	c.WriteString("for(uint32_t seed=0;seed<1000;seed++){uint32_t old=seed*2654435761u; uint16_t flag=seed>>3; PLANET target={0}, *lppl=&target;lppl->cDefenses=old;lppl->iScanner=old>>12;lppl->unused5=old>>17;lppl->fArtifact=old>>22;lppl->fNoResearch=old>>23;lppl->unused2=old>>24;\n")
	lppl := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "lppl", Type: &typeinfo.Pointer{Elem: planetType}}}
	flag := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "flag", Type: typeinfo.U16}}
	shifted := &sem.Cast{To: "int32_t", TypeInfo: typeinfo.I32, Value: &sem.Binary{TypeInfo: typeinfo.U16, Op: sem.OpShl,
		LHS: &sem.Cast{To: "uint32_t", TypeInfo: typeinfo.U32, Value: binary(sem.OpAnd, flag, constant(1))}, RHS: constant(0x16)}}
	flagWords := &sem.Words{Words: []sem.Expr{
		binary(sem.OpOr, binary(sem.OpAnd, &sem.Deref{Pointer: lppl, ByteOff: 0x1a, Width: 2, TypeInfo: typeinfo.U16}, constant(0xffbf)), &sem.Word{Parent: shifted, Part: machine.WordHigh}),
		binary(sem.OpOr, binary(sem.OpAnd, &sem.Deref{Pointer: lppl, ByteOff: 0x18, Width: 2, TypeInfo: typeinfo.U16}, constant(0xffff)), &sem.Word{Parent: shifted, Part: machine.WordLow}),
	}}
	emit([]sem.Effect{&sem.Assign{Dst: &sem.Deref{Pointer: lppl, ByteOff: 0x18, Width: 4, TypeInfo: typeinfo.U32}, Src: flagWords}})
	c.WriteString("uint32_t got=lppl->cDefenses|(lppl->iScanner<<12)|(lppl->unused5<<17)|(lppl->fArtifact<<22)|(lppl->fNoResearch<<23)|((uint32_t)lppl->unused2<<24);assert(got==((old&~(1u<<22))|((flag&1u)<<22)));}\n")
	// LogMakeValidXfer's nibble writes into one byte through LOBYTE and a multiply.
	c.WriteString("for(uint16_t seed=0;seed<512;seed++){uint16_t g1=seed*37, g2=seed*91; RTXFER rt={0x1111,0x2222,seed&0xf,seed>>4,0x33,{0x44}}, *prt=&rt;\n")
	prt := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "prt", Type: &typeinfo.Pointer{Elem: fx.SDB.GetStruct("RTXFER")}}}
	nibbles := &sem.Deref{Pointer: prt, ByteOff: 4, Width: 1, TypeInfo: typeinfo.U8}
	lowByte := func(value sem.Expr) sem.Expr {
		return &sem.Byte{Parent: value, Part: machine.ByteLow, TypeInfo: typeinfo.U8}
	}
	local := func(name string) sem.Expr {
		return &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: name, Type: typeinfo.U16}}
	}
	emit([]sem.Effect{
		&sem.Assign{Dst: nibbles, Src: lowByte(binary(sem.OpOr, binary(sem.OpAnd, nibbles, constant(0xf0)), binary(sem.OpAnd, lowByte(local("g1")), constant(0xf))))},
		&sem.Assign{Dst: nibbles, Src: lowByte(binary(sem.OpOr, binary(sem.OpAnd, nibbles, constant(0x0f)), &sem.Binary{TypeInfo: typeinfo.U16, Op: sem.OpMul, LHS: binary(sem.OpAnd, lowByte(local("g2")), constant(0xf)), RHS: constant(0x10)}))},
	})
	c.WriteString("assert(rt.grobj1==(g1&0xf));assert(rt.grobj2==(g2&0xf));assert(rt.id1==0x1111&&rt.id2==0x2222&&rt.grbitItems==0x33&&rt.rgcQuan[0]==0x44);}\nreturn 0;}\n")
	dir := t.TempDir()
	path := filepath.Join(dir, "aggregate.c")
	exe := filepath.Join(dir, "aggregate")
	if err := os.WriteFile(path, []byte(c.String()), 0600); err != nil {
		t.Fatal(err)
	}
	if out, err := exec.Command(cc, "-std=c99", "-Wall", "-Werror", "-fsanitize=undefined", path, "-o", exe).CombinedOutput(); err != nil {
		t.Fatalf("compile: %v\n%s\n%s", err, out, c.String())
	}
	if out, err := exec.Command(exe).CombinedOutput(); err != nil {
		t.Fatalf("execute: %v\n%s\n%s", err, out, c.String())
	}
}
