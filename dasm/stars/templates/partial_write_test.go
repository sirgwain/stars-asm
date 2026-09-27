package templates

import (
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestScalarPartialWritesExecute compiles and executes emitted C to verify
// preserved bits, signed values, nested slices, and once-only call evaluation.
func TestScalarPartialWritesExecute(t *testing.T) {
	cc, err := exec.LookPath("cc")
	if err != nil {
		t.Skip("C compiler unavailable")
	}
	types := []typeinfo.Type{typeinfo.U16, typeinfo.I16, typeinfo.U32, typeinfo.I32}
	var body strings.Builder
	body.WriteString("#include <stdint.h>\n#include <assert.h>\nstatic uint32_t values[2]; static int calls;\nstatic int next(void) { assert(calls++ == 0); return 1; }\nstatic uint32_t source(void) { assert(calls++ == 1); values[1] = 0x12345678; return 0x1ab; }\nint main(void) {\n")
	for _, typ := range types {
		for width := 1; width <= typ.Bytes(); width *= 2 {
			for offset := 0; offset+width <= typ.Bytes(); offset++ {
				local := &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: "value", Type: typ}}
				dst := &sem.Part{Base: local, ByteOff: offset, Width: width, TypeInfo: typ}
				fn := ir.Lower(sem.Func{Blocks: []sem.Block{{ID: 1, Effects: []sem.Effect{&sem.Assign{Dst: dst, Src: &sem.Const{TypeInfo: typeinfo.U32, U64: 0xfedcba98}}}}}}, &typeinfo.Function{Name: "fixture", Ret: typeinfo.U16, Vars: []typeinfo.FunctionVar{local.FunctionVar}})
				mask := (uint64(1) << (width * 8)) - 1
				all := (uint64(1) << (typ.Bytes() * 8)) - 1
				expected := ((uint64(0xa5a55a5a) & ^(mask << (offset * 8))) | ((0xfedcba98 & mask) << (offset * 8))) & all
				fmt.Fprintf(&body, "{ %s = (%s)0xa5a55a5a;\n", typeinfo.TypeDecl(typ, "value"), typeinfo.TypeDecl(typ, ""))
				for _, stmt := range fn.Blocks[0].Stmts {
					body.WriteString(formatIRStmt(stmt) + "\n")
				}
				fmt.Fprintf(&body, "assert((uint%d_t)value == 0x%x); }\n", typ.Bytes()*8, expected)
			}
		}
	}
	arrayType := &typeinfo.Array{Elem: typeinfo.U32, Count: 2}
	array := &sem.Global{GlobalVar: &typeinfo.GlobalVar{Name: "values", Type: arrayType}}
	index := &sem.ArrayIndex{Base: array, Index: &sem.Call{Function: &typeinfo.Function{Name: "next", Ret: typeinfo.I16}}, TypeInfo: typeinfo.U32}
	dst := &sem.Part{Base: &sem.Part{Base: index, ByteOff: 0, Width: 2, TypeInfo: typeinfo.U16}, ByteOff: 1, Width: 1, TypeInfo: typeinfo.U8}
	fn := ir.Lower(sem.Func{Blocks: []sem.Block{{ID: 1, Effects: []sem.Effect{&sem.Assign{Dst: dst, Src: &sem.Call{Function: &typeinfo.Function{Name: "source", Ret: typeinfo.U32}}}}}}}, &typeinfo.Function{Name: "fixture", Ret: typeinfo.U16})
	for _, local := range fn.Locals {
		body.WriteString(typeinfo.TypeDecl(local.Type, local.Name) + ";\n")
	}
	for _, stmt := range fn.Blocks[0].Stmts {
		body.WriteString(formatIRStmt(stmt) + "\n")
	}
	body.WriteString("assert(calls == 2); assert(values[1] == 0x1234ab78); return 0; }\n")
	dir := t.TempDir()
	source := filepath.Join(dir, "partial.c")
	if err := os.WriteFile(source, []byte(body.String()), 0600); err != nil {
		t.Fatal(err)
	}
	exe := filepath.Join(dir, "partial")
	if out, err := exec.Command(cc, "-std=c99", "-Wall", "-Werror", "-fsanitize=undefined", source, "-o", exe).CombinedOutput(); err != nil {
		t.Fatalf("compile: %v\n%s\n%s", err, out, body.String())
	}
	if out, err := exec.Command(exe).CombinedOutput(); err != nil {
		t.Fatalf("execute: %v\n%s", err, out)
	}
}
