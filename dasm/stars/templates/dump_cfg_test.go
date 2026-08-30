package templates

import (
	"bytes"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

func TestNewDumpCFGViewIncludesExplicitToBlock(t *testing.T) {
	cfg := &machine.CFG{
		Blocks: []*machine.Block{
			{ID: 0x72f4, StartIdx: 62, EndIdx: 65, EndOff: 0x72fd},
			{ID: 0x72fd, StartIdx: 65, EndIdx: 70, EndOff: 0x7303},
		},
	}

	view := NewDumpCFGView(cfg, DumpCFGOptions{
		DumpOptions: DumpOptions{
			FromAddr: 0x72f4,
			ToAddr:   0x72fd,
		},
	}, nil, nil)

	assertCFGBlockIDs(t, view.Blocks, []string{"L_72f4", "L_72fd"})
}

func TestNewDumpCFGViewFromOnlyStopsAtContainingBlockEnd(t *testing.T) {
	cfg := &machine.CFG{
		Blocks: []*machine.Block{
			{ID: 0x72f4, StartIdx: 62, EndIdx: 65, EndOff: 0x72fd},
			{ID: 0x72fd, StartIdx: 65, EndIdx: 70, EndOff: 0x7303},
		},
	}

	view := NewDumpCFGView(cfg, DumpCFGOptions{
		DumpOptions: DumpOptions{
			FromAddr: 0x72f4,
		},
	}, nil, nil)

	assertCFGBlockIDs(t, view.Blocks, []string{"L_72f4"})
}

func TestNewDumpCFGViewFormatsStateDeterministically(t *testing.T) {
	cfg := &machine.CFG{
		Blocks: []*machine.Block{
			{
				ID:                    0x1000,
				StartIdx:              0,
				EndIdx:                1,
				EndOff:                0x1002,
				KilledBeforeRead:      map[asm.Reg]bool{asm.RegDX: true, asm.RegAX: true, asm.RegBX: false},
				FlagsKilledBeforeRead: true,
				RegLiveIn:             map[asm.Reg]bool{asm.RegDI: true, asm.RegCX: true},
				RegLiveOut:            map[asm.Reg]bool{},
				BPLiveIn:              map[int]bool{6: true, -4: true, 8: false},
				BPLiveOut:             map[int]bool{-2: true},
				FlagsLiveIn:           true,
			},
		},
	}

	view := NewDumpCFGView(cfg, DumpCFGOptions{}, nil, nil)
	if got, want := view.Blocks[0].KilledBeforeRead, "ax, dx"; got != want {
		t.Fatalf("KilledBeforeRead = %q, want %q", got, want)
	}
	if got, want := view.Blocks[0].RegLiveIn, "cx, di"; got != want {
		t.Fatalf("RegLiveIn = %q, want %q", got, want)
	}
	if got, want := view.Blocks[0].RegLiveOut, "-"; got != want {
		t.Fatalf("RegLiveOut = %q, want %q", got, want)
	}
	if got, want := view.Blocks[0].BPLiveIn, "bp-0x4, bp+0x6"; got != want {
		t.Fatalf("BPLiveIn = %q, want %q", got, want)
	}
}

func TestRenderDumpCFG(t *testing.T) {
	view := DumpCFGView{
		Blocks: []DumpCFGBlockView{{
			DumpAsmBlockView: DumpAsmBlockView{
				Label: "L_1000",
				Off:   0x1000,
				Instrs: rawInsts(asm.DecodedInst{
					Off:      0x1000,
					Mnemonic: "MOV",
					Dst:      asm.Operand{Kind: asm.OKReg, Reg: asm.RegAX},
					Src:      asm.Operand{Kind: asm.OKReg, Reg: asm.RegBX},
				}),
			},
			Offsets:               "0x1000..0x1001",
			KilledBeforeRead:      "ax",
			FlagsKilledBeforeRead: true,
			RegLiveIn:             "cx",
			RegLiveOut:            "-",
			BPLiveIn:              "bp-0x4",
			BPLiveOut:             "-",
			FlagsLiveIn:           false,
			FlagsLiveOut:          true,
		}},
	}

	var buf bytes.Buffer
	if err := RenderDumpCFG(&buf, view); err != nil {
		t.Fatalf("RenderDumpCFG: %v", err)
	}

	got := buf.String()
	for _, want := range []string{
		"block L_1000:",
		"MOV       ax, bx",
		"killed-before-read:",
		"regs: ax",
		"bp: bp-0x4",
		"flags: true",
	} {
		if !strings.Contains(got, want) {
			t.Fatalf("rendered CFG missing %q:\n%s", want, got)
		}
	}
}

func assertCFGBlockIDs(t *testing.T, blocks []DumpCFGBlockView, want []string) {
	t.Helper()
	if len(blocks) != len(want) {
		t.Fatalf("got %d blocks, want %d", len(blocks), len(want))
	}
	for i, block := range blocks {
		if block.ID != want[i] {
			t.Fatalf("blocks[%d].ID = %q, want %q", i, block.ID, want[i])
		}
	}
}
