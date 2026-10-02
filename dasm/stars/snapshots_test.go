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
	"CBattles",
	"CBuildProdItem",
	"ChgCargo",
	"CMaxMines",
	"CreateChildWindows",
	"DeleteWpFar",
	"DpShieldOfShdef",
	"DrawMassWarpGauge",
	"DrawShipCargo",
	"DoBombing",
	"DropSalvage",
	"EnsureAis",
	"EnsureMacintiStarbaseDesigns",
	"ExecuteButton",
	"FCreateStuff",
	"FFleetMightHaveTeeth",
	"FCheckXferWP",
	"FBuildObject",
	"FCanKillTok",
	"FGetBestDefensePart",
	"FGetMouseMove",
	"FreeHb",
	"FIsAiAttack",
	"FLookupOrbitingXfer",
	"FLookupPart",
	"FOpenFile",
	"FReadFleet",
	"FTrackXfer",
	"GetFileStatus",
	"GetShdefScannerRange",
	"GetTechLevelCost",
	"HfontPrinterCreate",
	"InitMDIApp",
	"LCalcFuelGainFromRamScoops",
	"LpengineFromId",
	"LpflNew",
	"LphbAlloc",
	"LphuldefFromId",
	"LpscannerFromId",
	"LogChangeThing",
	"MessageWndProc",
	"MineClick",
	"NybbleFromCh",
	"PopRandom",
	"Popup",
	"PopupMenu",
	"PszFormatString",
	"PszNameProdItem",
	"ReadPlayerMessages",
	"ReportColumnPopup",
	"ReportDlg",
	"PushRandom",
	"SzVersion",
	"WrapTextOut",
	"WritePlayerMessages",
	"WriteRtShDef",
	"WtMaxShdefStat",
}

// TestDASM_RecoverySnapshots records the remaining compiler-pattern recoveries
// against complete real functions, including shared work and exact call results.
func TestDASM_RecoverySnapshots(t *testing.T) {
	fx := testfixture.Stars(t)
	for _, tc := range []struct {
		name         string
		want, absent []string
		once         string
	}{
		{name: "AnimateAttack", want: []string{"ptTorp = ptTop;", "ptTorp = ptBottom;"}, absent: []string{"t_merge_3ef2", "t_merge_3f10", "t_merge_3f2e", "t_merge_3f96", "t_merge_3fb4", "t_merge_3fd2"}},
		{name: "FWriteDataFile", want: []string{"pt = lpfl->pt;"}, absent: []string{"t_merge_"}},
		{name: "CMineFromLpfl", want: []string{"part.hs = *lphs;"}, absent: []string{"t_fields_"}},
		{name: "FReadShDef", want: []string{"part.hs = lphul->rghs[0];"}, absent: []string{"t_fields_"}},
		{name: "UpdatePlayerScores", want: []string{"vlprgScoreX[i].grbitVC |= 1;"}, absent: []string{"grbitVC = 0;", "wWord |= t_scratch_"}},
		{name: "PackageUpMsg", want: []string{"lpmt->msghdr.grWord |= grbit;"}, absent: []string{"t_fields_", "t_scratch_m16_2"}},
		{name: "FSendPlrMsg2XGen", want: []string{"pmsghdr->grWord |= grbit;"}, absent: []string{"t_fields_"}},
		{name: "DoBattles", want: []string{"cplr = CplrBattle("}, absent: []string{"t_call_3b1d"}, once: "CplrBattle("},
		{name: "SortReportCache", want: []string{"default:\n            return;"}, absent: []string{"goto L_5b5d"}, once: "qsort("},
		{name: "FTutorialEnabledShipBuilder", want: []string{"return t_call_7c62;"}, absent: []string{"t_merge_81d4"}, once: "FCheckShipBuilder(0, 2)"},
		{name: "PopupMineralScanChoices", want: []string{"rgid[c++] = (uint32_t)(uint16_t)lpth->idFull | 0x20000000;"}, absent: []string{"t_assign_", "t_50dd"}},
		{name: "ClickInShipOrders", want: []string{"rgid[c++] = (uint32_t)(uint16_t)lpth->idFull | 0x20000000;"}, absent: []string{"t_assign_", "t_84ec"}},
		{name: "ScannerWndProc", want: []string{"rgid[c++] = (uint32_t)(uint16_t)lpth->idFull | 0x20000000;"}, absent: []string{"t_assign_", "t_08b4"}},
		{name: "InitInstance", want: []string{"hAccel = LoadAccelerators(", "if (hAccel == 0)", "hAccelTitle = LoadAccelerators(", "if (hAccelTitle == 0)"}, absent: []string{"t_call_"}, once: "MAKEINTRESOURCE(IDA_MAIN)"},
		{name: "SetVisPFPlanets", want: []string{"rgStargateRange[i] = StargateRangeFromLppl(NULL, iPlr, i);", "if (rgStargateRange[i] > 0)"}, absent: []string{"t_call_"}, once: "StargateRangeFromLppl(NULL, iPlr, i)"},
		{name: "FGenerateTurn", want: []string{"rglpshdef[i][ish].hul.rghs[0].cItem = 1;"}, absent: []string{"t_fields_"}},
		{name: "WinMain", want: []string{"for (; *lpT == ' '; lpT++)", "if (*lpT == '-' || *lpT == '/')", "for (lpT++; *lpT != 0 && *lpT != ' '; lpT++) {", "case 'F':", "i = 10 * i + *lpT - '0';", "ini.fCmdLine = szBase[0] != 0;"}, absent: []string{"goto ", "(uint16_t)(*lpT", "? 0 : 1"}},
		{name: "FillBuildPartsLB", want: []string{"sz[1] = i + 'A';", "sz[2] = part.pcom->ibmp % 26 + 'A';"}, absent: []string{"LOBYTE"}},
		{name: "FakeListProc", want: []string{"CallWindowProc(lpfnRealListProc, hwnd, WM_LBUTTONDOWN, wParam, lParam);", "1 << (szWork[0] - 'A')"}},
	} {
		t.Run(tc.name, func(t *testing.T) {
			if err := dumpFunction(fx.SDB, tc.name, "region.c", func(w io.Writer, f *typeinfo.Function) {
				var output bytes.Buffer
				if err := DumpFuncRegion(&output, fx.Image, fx.SDB, f, DumpOptions{}); err != nil {
					t.Fatal(err)
				}
				text := output.String()
				for _, want := range tc.want {
					if !strings.Contains(text, want) {
						t.Fatalf("missing %q in recovered output:\n%s", want, text)
					}
				}
				for _, absent := range tc.absent {
					if strings.Contains(text, absent) {
						t.Fatalf("remaining %q in recovered output:\n%s", absent, text)
					}
				}
				if tc.once != "" && strings.Count(text, tc.once) != 1 {
					t.Fatalf("shared work %q changed count", tc.once)
				}
				if _, err := w.Write(output.Bytes()); err != nil {
					t.Fatal(err)
				}
			}); err != nil {
				t.Fatal(err)
			}
		})
	}
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
		{name: "DoCyberFreighter", from: 0x3916, want: "ord.fValidTask = TRUE"},
		{name: "GenerateWorld", from: 0x2c9d, want: "rgplr[i].cshdefSB = (rgplr[i].cshdefSB + 1)"},
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

// TestDASM_WideArithmeticSnapshot records the real ADD/ADC distance expression
// in FAddWayPoint and verifies that its scratch-backed word pair is rebuilt.
func TestDASM_WideArithmeticSnapshot(t *testing.T) {
	fx := testfixture.Stars(t)
	err := dumpFunction(fx.SDB, "FAddWayPoint", "L_75c6.sem", func(w io.Writer, f *typeinfo.Function) {
		var output bytes.Buffer
		if err := DumpFuncSem(&output, fx.Image, fx.SDB, f, DumpSemOptions{DumpOptions: DumpOptions{FromAddr: 0x75c6, ToAddr: 0x7644}}); err != nil {
			t.Fatal(err)
		}
		text := output.String()
		if strings.Contains(text, "words(") || !strings.Contains(text, "sext16to32(dx) * sext16to32(dx)") {
			t.Fatalf("unresolved wide arithmetic:\n%s", text)
		}
		if _, err := w.Write(output.Bytes()); err != nil {
			t.Fatal(err)
		}
	})
	if err != nil {
		t.Fatal(err)
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

func TestDASM_RegionSnapshots(t *testing.T) {
	t.Helper()
	fx := testfixture.Stars(t)

	for _, name := range procs {
		t.Run(name, func(t *testing.T) {

			if err := dumpFunction(fx.SDB, name, "region.c", func(w io.Writer, f *typeinfo.Function) {
				if err := DumpFuncRegion(w, fx.Image, fx.SDB, f, DumpOptions{}); err != nil {
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

// TestDASM_CargoTransferSignExtension preserves CBW/CWD when cargo quantities
// are read through the log byte pointer, viewed by record type as the
// transfer record whose signed rgcQuan width matches the record.
func TestDASM_CargoTransferSignExtension(t *testing.T) {
	fx := testfixture.Stars(t)
	err := dumpFunction(fx.SDB, "FRunLogRecord", "cargo.ir.c", func(w io.Writer, f *typeinfo.Function) {
		var output bytes.Buffer
		if err := DumpFuncIR(&output, fx.Image, fx.SDB, f, DumpIROptions{
			DumpOptions: DumpOptions{FromAddr: 0xae39, ToAddr: 0xae9d},
			ShowIR:      true,
		}); err != nil {
			t.Fatal(err)
		}
		text := output.String()
		for _, want := range []string{
			"rgcXfer[i] = (int16_t)((RTXFER *)lpb)->rgcQuan[iLook];",
			"rgcXfer[i] = ((RTXFERX *)lpb)->rgcQuan[iLook];",
		} {
			if !strings.Contains(text, want) {
				t.Fatalf("missing signed cargo load %q:\n%s", want, text)
			}
		}
		if _, err := w.Write(output.Bytes()); err != nil {
			t.Fatal(err)
		}
	})
	if err != nil {
		t.Fatal(err)
	}
}
