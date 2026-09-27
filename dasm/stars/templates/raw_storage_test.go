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

// TestRawStorageExecute checks emitted C for LpReAlloc's heap header word,
// FReadFleet's byte-stream reads, LpAlloc's byte displacement from a heap
// block and a byte difference of word pointers, none of which may be scaled by
// the pointee size. Accesses with an unmapped layout offset, no access type or
// a conflicting address type must be reported instead of emitted.
func TestRawStorageExecute(t *testing.T) {
	cc, err := exec.LookPath("cc")
	if err != nil {
		t.Skip("C compiler unavailable")
	}
	fx := testfixture.Stars(t)
	ctx := sem.NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fx.SDB.GetFunction("LpReAlloc"))
	hbType := fx.SDB.GetStruct("HB")
	bytePtr := &typeinfo.Pointer{Elem: typeinfo.U8}
	wordPtr := &typeinfo.Pointer{Elem: typeinfo.I16}
	local := func(name string, typ typeinfo.Type) *sem.Local {
		return &sem.Local{FunctionVar: typeinfo.FunctionVar{Name: name, Type: typ}}
	}
	lp := local("lp", &typeinfo.Pointer{Elem: &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"}})
	pb := local("pb", bytePtr)
	lphb := local("lphb", &typeinfo.Pointer{Elem: hbType})
	ibTop := hbType.Fields[3]
	if ibTop.Name != "ibTop" {
		t.Fatalf("HB field 3 = %s, want ibTop", ibTop.Name)
	}
	lower := func(effects []sem.Effect) ir.Func {
		src := sem.Func{Blocks: []sem.Block{{ID: 1, Effects: effects}}}
		result := &sem.Result{Operands: make(map[machine.AnnotationKey]sem.Annotation), Memory: make(map[string][]sem.Annotation), Values: make(map[string]sem.Annotation)}
		for _, spec := range sem.ProcessorSpecs() {
			if spec.Name == "resolve-late-addresses" {
				spec.Func(ctx).ProcessFunc(result, &src)
			}
		}
		return ir.Lower(src, &typeinfo.Function{Name: "fixture", Ret: typeinfo.U16})
	}

	fn := lower([]sem.Effect{
		&sem.Assign{Dst: local("cb", typeinfo.U16), Src: &sem.Deref{Pointer: lp, ByteOff: -2, Width: 2, TypeInfo: typeinfo.U16}},
		&sem.Assign{Dst: local("l", typeinfo.U32), Src: &sem.Deref{Pointer: pb, ByteOff: 2, Width: 4, TypeInfo: typeinfo.U32}},
		&sem.Assign{Dst: &sem.Deref{Pointer: lp, ByteOff: -2, Width: 2, TypeInfo: typeinfo.U16}, Src: &sem.Const{TypeInfo: typeinfo.U16, U64: 0x1234}},
		&sem.Assign{Dst: local("top", bytePtr), Src: &sem.Binary{TypeInfo: typeinfo.U16, Op: sem.OpAdd, LHS: lphb, RHS: &sem.FieldAccess{Base: lphb, Field: &ibTop}}},
		// BattleVCR-style loss index: a byte difference of int16_t pointers halved.
		&sem.Assign{Dst: local("n", typeinfo.U16), Src: &sem.Binary{TypeInfo: typeinfo.U16, Op: sem.OpShr, RHS: &sem.Const{TypeInfo: typeinfo.U16, U64: 1},
			LHS: &sem.Binary{TypeInfo: typeinfo.U16, Op: sem.OpSub, LHS: local("pw", wordPtr), RHS: local("rg", wordPtr)}}},
	})
	if fn.Analyze().Untranslated != 0 {
		t.Fatalf("untranslated fixture: %+v", fn.Analyze())
	}
	// Use the raw storage helpers exactly as common.h declares them.
	common, err := templatesFS.ReadFile("assets/common.h.templ")
	if err != nil {
		t.Fatal(err)
	}
	var c strings.Builder
	c.WriteString("#include <stdint.h>\n#include <string.h>\n#include <assert.h>\n")
	for _, line := range strings.Split(string(common), "\n") {
		if strings.HasPrefix(line, "static inline") {
			c.WriteString(line + "\n")
		}
	}
	c.WriteString(`typedef struct { uint16_t cbFree, cbBlock, cbSlop, ibTop; } HB;
int main(void) {
uint8_t buf[16]; for (int i = 0; i < 16; i++) buf[i] = (uint8_t)(i * 17 + 3);
void *lp = buf + 6; uint8_t *pb = buf; HB hb = {0, 0, 0, 5}; HB *lphb = &hb;
uint16_t cb; uint32_t l, want; uint8_t *top; uint16_t header;
int16_t words[8]; int16_t *pw = words + 5, *rg = words; uint16_t n;
memcpy(&header, buf + 4, 2); memcpy(&want, buf + 2, 4);
`)
	for _, stmt := range fn.Blocks[0].Stmts {
		c.WriteString(formatIRStmt(stmt) + "\n")
	}
	c.WriteString(`assert(cb == header);
assert(buf[4] == 0x34 && buf[5] == 0x12);
assert(l == want);
assert(top == (uint8_t *)lphb + 5);
assert(n == 5);
return 0;
}
`)
	dir := t.TempDir()
	path := filepath.Join(dir, "raw.c")
	exe := filepath.Join(dir, "raw")
	if err := os.WriteFile(path, []byte(c.String()), 0600); err != nil {
		t.Fatal(err)
	}
	if out, err := exec.Command(cc, "-std=c99", "-Wall", "-Werror", "-Wno-unused-function", "-fsanitize=undefined", path, "-o", exe).CombinedOutput(); err != nil {
		t.Fatalf("compile: %v\n%s\n%s", err, out, c.String())
	}
	if out, err := exec.Command(exe).CombinedOutput(); err != nil {
		t.Fatalf("execute: %v\n%s\n%s", err, out, c.String())
	}

	// Accesses whose layout or type cannot be established are reported.
	charPtr := &typeinfo.Pointer{Elem: typeinfo.LpStr.Elem}
	for _, tc := range []struct {
		dst  typeinfo.Type
		src  sem.Expr
		want string
	}{
		{typeinfo.U16, &sem.Deref{Pointer: lphb, ByteOff: 1, Width: 2, TypeInfo: typeinfo.U16}, "original-layout-offset"},
		{typeinfo.U16, &sem.Deref{Pointer: pb, ByteOff: 2, Width: 2}, "unknown-access-type"},
		{&typeinfo.Pointer{Elem: charPtr}, &sem.AddressOf{Target: &sem.Deref{Pointer: local("psz", charPtr), Width: 1, TypeInfo: typeinfo.LpStr.Elem}, TypeInfo: &typeinfo.Pointer{Elem: charPtr}}, "address-type-conflict"},
	} {
		reported := lower([]sem.Effect{&sem.Assign{Dst: local("w", tc.dst), Src: tc.src}})
		comment, ok := reported.Blocks[0].Stmts[0].(*ir.Comment)
		if !ok || !strings.Contains(fmt.Sprint(comment.Failures), tc.want) {
			t.Fatalf("%s = %s, want %s", sem.FormatExpr(tc.src), formatIRStmt(reported.Blocks[0].Stmts[0]), tc.want)
		}
	}
}
