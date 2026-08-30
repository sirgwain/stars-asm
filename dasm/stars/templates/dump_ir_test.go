package templates

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

func TestNewDumpIRViewFiltersFromTo(t *testing.T) {
	view := NewDumpIRView(irFuncForRangeTest(), DumpOptions{
		FromAddr: 0x1003,
		ToAddr:   0x1006,
	})

	assertIRBlockLabels(t, view.Func.Blocks, []string{"L_1000", "L_1004"})
}

func TestNewDumpIRViewFromOnlyStopsAtContainingBlockEnd(t *testing.T) {
	view := NewDumpIRView(irFuncForRangeTest(), DumpOptions{
		FromAddr: 0x1003,
	})

	assertIRBlockLabels(t, view.Func.Blocks, []string{"L_1000"})
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
