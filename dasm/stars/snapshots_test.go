package stars

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

var procs = []string{
	"About",
	"AddMinesToBlockedQueues",
	"AlertSz",
	"CalcPctSurvive",
	"CalcPlayerScore",
	"CBuildProdItem",
	"ChgCargo",
	"CMaxMines",
	"DeleteWpFar",
	"DropSalvage",
	"FCreateStuff",
	"FBuildObject",
	"FGetBestDefensePart",
	"FGetMouseMove",
	"FLookupOrbitingXfer",
	"FLookupPart",
	"InitMDIApp",
	"LpengineFromId",
	"LphuldefFromId",
	"LpscannerFromId",
	"PopRandom",
	"PushRandom",
	"SzVersion",
}

func dumpFunction(sdb *typeinfo.SymbolDB, name, ext string, dumper func(w io.Writer, f *typeinfo.Function)) error {
	f := sdb.GetFunction(name)
	if f == nil {
		return fmt.Errorf("function %s not found", name)
	}

	var buf bytes.Buffer

	dumper(&buf, f)

	outPath := filepath.Join("testdata", "snapshots", name+"."+ext)

	if err := os.MkdirAll(filepath.Dir(outPath), 0o755); err != nil {
		return fmt.Errorf("mkdir: %v", err)
	}

	out := bytes.ReplaceAll(buf.Bytes(), []byte("\r\n"), []byte("\n"))

	if err := os.WriteFile(outPath, out, 0o644); err != nil {
		return fmt.Errorf("write snapshot: %v", err)
	}

	return nil
}

func TestDASM_ASMSnapshots(t *testing.T) {
	t.Helper()
	fx := testfixture.Stars(t)

	for _, name := range procs {
		t.Run(name, func(t *testing.T) {

			if err := dumpFunction(fx.SDB, name, "asm", func(w io.Writer, f *typeinfo.Function) {
				if err := DumpFuncDetail(w, fx.Image, f, FuncDetailOptions{
					CommentStyle: FuncDetailCommentAsm,
				}); err != nil {
					t.Fatal(err)
				}

				if err := DumpFuncAsm(w, fx.Image, fx.SDB, f, templates.DumpAsmOptions{}); err != nil {
					t.Fatal(err)
				}
			}); err != nil {
				t.Fatal(err)
			}
		})
	}
}

func TestDASM_EffectSnapshots(t *testing.T) {
	t.Helper()
	fx := testfixture.Stars(t)

	for _, name := range procs {
		t.Run(name, func(t *testing.T) {

			if err := dumpFunction(fx.SDB, name, "effect", func(w io.Writer, f *typeinfo.Function) {
				if err := DumpFuncDetail(w, fx.Image, f, FuncDetailOptions{
					CommentStyle: templates.FuncDetailCommentC,
				}); err != nil {
					t.Fatal(err)
				}

				if err := DumpFuncEffects(w, fx.Image, fx.SDB, f, templates.DumpEffectsOptions{}); err != nil {
					t.Fatal(err)
				}
			}); err != nil {
				t.Fatal(err)
			}
		})
	}
}

func TestDASM_SemSnapshots(t *testing.T) {
	t.Helper()
	fx := testfixture.Stars(t)

	for _, name := range procs {
		t.Run(name, func(t *testing.T) {

			if err := dumpFunction(fx.SDB, name, "sem", func(w io.Writer, f *typeinfo.Function) {
				if err := DumpFuncDetail(w, fx.Image, f, FuncDetailOptions{
					CommentStyle: templates.FuncDetailCommentC,
				}); err != nil {
					t.Fatal(err)
				}

				if err := DumpFuncSem(w, fx.Image, fx.SDB, f, templates.DumpSemOptions{}); err != nil {
					t.Fatal(err)
				}
			}); err != nil {
				t.Fatal(err)
			}
		})
	}
}

func TestDASM_IRSnapshots(t *testing.T) {
	t.Helper()
	fx := testfixture.Stars(t)

	for _, name := range procs {
		t.Run(name, func(t *testing.T) {

			if err := dumpFunction(fx.SDB, name, "ir.c", func(w io.Writer, f *typeinfo.Function) {
				if err := DumpFuncIR(w, fx.Image, fx.SDB, f, templates.DumpOptions{}); err != nil {
					t.Fatal(err)
				}
			}); err != nil {
				t.Fatal(err)
			}
		})
	}
}
