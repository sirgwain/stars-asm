package templates

import (
	"bytes"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestNewDumpEffectsViewIncludesExplicitToBlock(t *testing.T) {
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{
			Blocks: []*machine.Block{
				{ID: 0x72f4, StartIdx: 62, EndIdx: 65, EndOff: 0x72fd},
				{ID: 0x72fd, StartIdx: 65, EndIdx: 70, EndOff: 0x7303},
			},
		},
	}

	view := NewDumpEffectsView(effects, DumpEffectsOptions{
		DumpOptions: DumpOptions{
			FromAddr: 0x72f4,
			ToAddr:   0x72fd,
		},
	}, nil, nil)

	assertBlockIDs(t, view.Blocks, []string{"L_72f4", "L_72fd"})
}

func TestNewDumpEffectsViewFromOnlyStopsAtContainingBlockEnd(t *testing.T) {
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{
			Blocks: []*machine.Block{
				{ID: 0x72f4, StartIdx: 62, EndIdx: 65, EndOff: 0x72fd},
				{ID: 0x72fd, StartIdx: 65, EndIdx: 70, EndOff: 0x7303},
			},
		},
	}

	view := NewDumpEffectsView(effects, DumpEffectsOptions{
		DumpOptions: DumpOptions{
			FromAddr: 0x72f4,
		},
	}, nil, nil)

	assertBlockIDs(t, view.Blocks, []string{"L_72f4"})
}

func TestRenderDumpEffectsIncludesTypedEffects(t *testing.T) {
	effects := &machine.FuncEffects{
		CFG: &machine.CFG{
			Blocks: []*machine.Block{
				{ID: 0x72f4, StartIdx: 62, EndIdx: 65, EndOff: 0x72fd, Label: "L_72f4"},
			},
		},
		Blocks: []machine.BlockEffects{
			{
				Block: 0x72f4,
				Effects: []machine.Effect{
					machine.StoreEffect{
						MetaInfo: machine.Meta{InstOff: 0x72f6},
						Addr: machine.MemoryAccess{
							Base:  machine.RegVal(asm.RegBP),
							Disp:  -2,
							Width: 2,
						},
						Src:   machine.ConstVal(0x1234),
						Width: 2,
					},
					machine.CallEffect{
						MetaInfo: machine.Meta{InstOff: 0x72fa},
						Target:   &typeinfo.Function{Name: "DoThing"},
						Args: []machine.Value{
							machine.RegVal(asm.RegNone),
							machine.ConstVal(0x2),
						},
						Result: machine.RegVal(asm.RegAX),
					},
				},
			},
		},
	}

	var buf bytes.Buffer
	if err := RenderDumpEffects(&buf, NewDumpEffectsView(effects, DumpEffectsOptions{}, nil, nil)); err != nil {
		t.Fatalf("RenderDumpEffects: %v", err)
	}

	got := buf.String()
	for _, want := range []string{
		"  effects:",
		"    72f6  store[2] [bp-0x2] = 0x1234",
		"    72fa  call DoThing(reg(0), 0x2) -> ax",
	} {
		if !strings.Contains(got, want) {
			t.Fatalf("rendered dump missing %q:\n%s", want, got)
		}
	}
}

func TestFormatEffectsHidesUnambiguousLoadID(t *testing.T) {
	cMax := machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   -6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x72b7, Role: machine.OperandDst},
	}
	effects := []machine.Effect{
		machine.BranchEffect{
			MetaInfo:   machine.Meta{InstOff: 0x72bb},
			Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JNC", LHS: machine.LoadVal(cMax), RHS: machine.ConstVal(0xa)},
			TrueBlock:  0x72ca,
			FalseBlock: 0x72c0,
		},
	}

	got := formatEffects(effects, nil)
	want := "72bb  branch load([bp-0x6]) >= 0xa ? L_72ca : L_72c0"
	if len(got) != 1 || got[0] != want {
		t.Fatalf("effects = %#v, want %#v", got, []string{want})
	}
}

func TestFormatEffectsShowsStaleLoadID(t *testing.T) {
	nAtMov := machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x5c4f, Role: machine.OperandSrc},
	}
	nAtSub := machine.MemoryAccess{
		Base:   machine.FrameBaseVal(),
		Disp:   6,
		Width:  2,
		Origin: machine.Origin{InstOff: 0x5c52, Role: machine.OperandDst},
	}
	effects := []machine.Effect{
		machine.StoreEffect{
			MetaInfo: machine.Meta{InstOff: 0x5c52},
			Addr:     nAtSub,
			Src:      machine.BinaryVal(machine.ValueOpSub, machine.LoadVal(nAtSub), machine.ConstVal(1)),
			Width:    2,
		},
		machine.BranchEffect{
			MetaInfo:   machine.Meta{InstOff: 0x5c59},
			Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: "JNZ", LHS: machine.LoadVal(nAtMov), RHS: machine.ConstVal(0)},
			TrueBlock:  0x5c81,
			FalseBlock: 0x5c5e,
		},
	}

	got := formatEffects(effects, nil)
	want := []string{
		"5c52  store[2] [bp+0x6] = (load@5c52/dst([bp+0x6]) - 0x1)",
		"5c59  branch load@5c4f/src([bp+0x6]) != 0x0 ? L_5c81 : L_5c5e",
	}
	if len(got) != len(want) {
		t.Fatalf("effects = %#v, want %#v", got, want)
	}
	for i := range want {
		if got[i] != want[i] {
			t.Fatalf("effect %d = %q, want %q", i, got[i], want[i])
		}
	}
}

func TestRenderDumpEffectsCanInlineAsmBeforeEffects(t *testing.T) {
	instrs := []asm.DecodedInst{
		{
			Off:      0x7248,
			Len:      1,
			Mnemonic: "PUSH",
			Dst:      asm.Operand{Kind: asm.OKReg, Reg: asm.RegBP},
		},
		{
			Off:      0x7249,
			Len:      2,
			Mnemonic: "JMP",
			Op:       asm.OpJMP,
			Target:   0x72af,
		},
		{
			Off:      0x72af,
			Len:      1,
			Mnemonic: "RET",
		},
	}
	fn := &typeinfo.Function{
		SourceFile: typeinfo.SourceFile{
			File:  "planet.c",
			Lines: []typeinfo.SourceLine{{Off: 0x7248, Line: 2931}},
		},
	}
	effects := &machine.FuncEffects{
		CFG: cfgForTest(t, instrs, fn),
		Blocks: []machine.BlockEffects{{
			Block: 0x7248,
			Effects: []machine.Effect{
				machine.JumpEffect{MetaInfo: machine.Meta{InstOff: 0x7249}, To: 0x72af},
			},
		}},
	}

	var buf bytes.Buffer
	if err := RenderDumpEffects(&buf, NewDumpEffectsView(effects, DumpEffectsOptions{ShowAsm: true, ShowOffsets: true}, fn, nil)); err != nil {
		t.Fatalf("RenderDumpEffects: %v", err)
	}

	got := buf.String()
	for _, want := range []string{
		"  instrs:",
		"    L_7248:                             ; planet.c:2931",
		"    7248  PUSH      bp",
		"  effects:",
		"    7249  goto L_72af",
	} {
		if !strings.Contains(got, want) {
			t.Fatalf("rendered dump missing %q:\n%s", want, got)
		}
	}
	if strings.Index(got, "  instrs:") > strings.Index(got, "  effects:") {
		t.Fatalf("instrs rendered after effects:\n%s", got)
	}
}

func assertBlockIDs(t *testing.T, blocks []DumpEffectsBlockView, want []string) {
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
