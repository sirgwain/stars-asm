package stars

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

var procs = []string{
	"About",
	"AddMinesToBlockedQueues",
	"AlertSz",
	"BattlePlansDlg",
	"CalcPctSurvive",
	"CalcPlayerScore",
	"CBuildProdItem",
	"ChgCargo",
	"CMaxMines",
	"DeleteWpFar",
	"DoBombing",
	"DropSalvage",
	"FCreateStuff",
	"FCheckXferWP",
	"FBuildObject",
	"FCanKillTok",
	"FGetBestDefensePart",
	"FGetMouseMove",
	"FIsAiAttack",
	"FLookupOrbitingXfer",
	"FLookupPart",
	"FOpenFile",
	"FReadFleet",
	"FTrackXfer",
	"GetFileStatus",
	"GetTechLevelCost",
	"HfontPrinterCreate",
	"InitMDIApp",
	"LCalcFuelGainFromRamScoops",
	"LpengineFromId",
	"LphuldefFromId",
	"LpscannerFromId",
	"LogChangeThing",
	"MineClick",
	"PopRandom",
	"PushRandom",
	"SzVersion",
}

// TestDASM_BitfieldUpdateSnapshots verifies the reported compiler patterns and
// records their resolved block output alongside the full-function snapshots.
func TestDASM_BitfieldUpdateSnapshots(t *testing.T) {
	fx := testfixture.Stars(t)
	for _, tc := range []struct {
		name string
		from uint32
		want string
	}{
		{name: "DoCyberFreighter", from: 0x3916, want: "ord.fValidTask = 0x1"},
		{name: "GenerateWorld", from: 0x2c9d, want: "rgplr[i].cshdefSB = (rgplr[i].cshdefSB + 0x1)"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			err := dumpFunction(fx.SDB, tc.name, fmt.Sprintf("L_%04x.sem", tc.from), func(w io.Writer, f *typeinfo.Function) {
				var output bytes.Buffer
				if err := DumpFuncSem(&output, fx.Image, fx.SDB, f, DumpSemOptions{DumpOptions: DumpOptions{FromAddr: tc.from}}); err != nil {
					t.Fatal(err)
				}
				text := output.String()
				if !strings.Contains(text, tc.want) || strings.Contains(text, "part[") || strings.Contains(text, "t_scratch_") || strings.Contains(text, "t_396c") {
					t.Fatalf("unresolved bitfield update:\n%s", text)
				}
				if _, err := w.Write(output.Bytes()); err != nil {
					t.Fatal(err)
				}
			})
			if err != nil {
				t.Fatal(err)
			}
		})
	}
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
				if err := DumpFuncIR(w, fx.Image, fx.SDB, f, templates.DumpIROptions{ShowIR: true}); err != nil {
					t.Fatal(err)
				}
			}); err != nil {
				t.Fatal(err)
			}
		})
	}
}
