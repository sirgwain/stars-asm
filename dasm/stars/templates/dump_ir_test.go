package templates

import (
	"bytes"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

func TestNewDumpIRViewFiltersFromTo(t *testing.T) {
	view := NewDumpIRView(irFuncForRangeTest(), DumpIROptions{DumpOptions: DumpOptions{
		FromAddr: 0x1003,
		ToAddr:   0x1006,
	},
		ShowIR: true,
	})

	assertIRBlockLabels(t, view.Func.Blocks, []string{"L_1000", "L_1004"})
}

func TestNewDumpIRViewFromOnlyStopsAtContainingBlockEnd(t *testing.T) {
	view := NewDumpIRView(irFuncForRangeTest(), DumpIROptions{DumpOptions: DumpOptions{
		FromAddr: 0x1003,
	},
		ShowIR: true,
	})

	assertIRBlockLabels(t, view.Func.Blocks, []string{"L_1000"})
}

// TestRenderDumpIRSectionHeaders verifies that section headers are only added
// when the view includes analysis stages alongside IR.
func TestRenderDumpIRSectionHeaders(t *testing.T) {
	fn := ir.Func{
		Decl: "void Test(void)",
		Blocks: []ir.Block{{
			Label:    "L_1000",
			StartOff: 0x1000,
			EndOff:   0x1002,
			Stmts:    []ir.Stmt{&ir.Return{}},
		}},
	}

	var irOnly bytes.Buffer
	if err := RenderDumpIR(&irOnly, NewDumpIRView(fn, DumpIROptions{
		ShowIR: true,
	})); err != nil {
		t.Fatal(err)
	}
	if strings.Contains(irOnly.String(), "ir:") {
		t.Fatalf("IR-only output unexpectedly has an ir header: %s", irOnly.String())
	}

	semView := DumpSemView{
		Blocks: []DumpSemBlockView{{
			DumpAsmBlockView: DumpAsmBlockView{Label: "L_1000", Off: 0x1000},
			Effects:          []string{"return"},
		}},
	}
	combined := NewDumpIRViewWithSem(fn, semView, DumpIROptions{ShowIR: true, ShowSem: true})
	var combinedOutput bytes.Buffer
	if err := RenderDumpIR(&combinedOutput, combined); err != nil {
		t.Fatal(err)
	}
	for _, header := range []string{"sem:", "ir:"} {
		if !strings.Contains(combinedOutput.String(), header) {
			t.Fatalf("combined output missing %q: %s", header, combinedOutput.String())
		}
	}
}

// irFuncForRangeTest creates an IR function with realistic block offsets.
func irFuncForRangeTest() ir.Func {
	return ir.Func{
		Decl: "void Test(void)",
		Blocks: []ir.Block{
			{ID: machine.BlockID(0x1000), Label: "L_1000", StartOff: 0x1000, EndOff: 0x1004},
			{ID: machine.BlockID(0x1004), Label: "L_1004", StartOff: 0x1004, EndOff: 0x1008},
			{ID: machine.BlockID(0x1008), Label: "L_1008", StartOff: 0x1008, EndOff: 0x100c},
		},
	}
}

func assertIRBlockLabels(t *testing.T, blocks []ir.Block, want []string) {
	t.Helper()
	got := make([]string, 0, len(blocks))
	for _, block := range blocks {
		got = append(got, block.Label)
	}
	if len(got) != len(want) {
		t.Fatalf("IR block labels = %#v, want %#v", got, want)
	}
	for i := range want {
		if got[i] != want[i] {
			t.Fatalf("IR block labels = %#v, want %#v", got, want)
		}
	}
}
