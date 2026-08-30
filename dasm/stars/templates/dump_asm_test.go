package templates

import (
	"bytes"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestNewDumpAsmViewFiltersFromTo(t *testing.T) {
	instrs := rawInsts(
		asm.DecodedInst{Off: 0x1000, Op: asm.OpJcc, Target: 0x1006},
		asm.DecodedInst{Off: 0x1002},
		asm.DecodedInst{Off: 0x1004},
		asm.DecodedInst{Off: 0x1006},
	)

	view := NewDumpAsmView(DumpAsmOptions{
		DumpOptions: DumpOptions{
			FromAddr: 0x1002,
			ToAddr:   0x1006,
		},
	}, DumpStyles{}, &typeinfo.Function{}, cfgForTest(t, instrs, &typeinfo.Function{}), nil)

	assertBlockInstOffsets(t, view.Blocks, [][]uint32{{0x1002, 0x1004}})
}

func TestNewDumpAsmViewIncludesExplicitToLabelBlock(t *testing.T) {
	instrs := rawInsts(
		asm.DecodedInst{Off: 0x72f4, Len: 3, Op: asm.OpJcc, Target: 0x72fd},
		asm.DecodedInst{Off: 0x72f7, Len: 3},
		asm.DecodedInst{Off: 0x72fa, Len: 3},
		asm.DecodedInst{Off: 0x72fd, Len: 2},
		asm.DecodedInst{Off: 0x72ff, Len: 2},
		asm.DecodedInst{Off: 0x7301, Len: 2},
	)

	view := NewDumpAsmView(DumpAsmOptions{
		DumpOptions: DumpOptions{
			FromAddr: 0x72f4,
			ToAddr:   0x72fd,
		},
	}, DumpStyles{}, &typeinfo.Function{}, cfgForTest(t, instrs, &typeinfo.Function{}), nil)

	assertBlockInstOffsets(t, view.Blocks, [][]uint32{{0x72f4}, {0x72f7, 0x72fa}, {0x72fd, 0x72ff, 0x7301}})
}

func TestNewDumpAsmViewStopsAtNextLabelWhenToOmitted(t *testing.T) {
	instrs := rawInsts(
		asm.DecodedInst{Off: 0x1000, Op: asm.OpJcc, Target: 0x1006},
		asm.DecodedInst{Off: 0x1002},
		asm.DecodedInst{Off: 0x1004},
		asm.DecodedInst{Off: 0x1006},
	)

	view := NewDumpAsmView(DumpAsmOptions{
		DumpOptions: DumpOptions{
			FromAddr: 0x1002,
		},
	}, DumpStyles{}, &typeinfo.Function{}, cfgForTest(t, instrs, &typeinfo.Function{}), nil)

	assertBlockInstOffsets(t, view.Blocks, [][]uint32{{0x1002, 0x1004}})
}

func TestNewDumpAsmViewIncludesAllWithoutRange(t *testing.T) {
	instrs := rawInsts(
		asm.DecodedInst{Off: 0x1000},
		asm.DecodedInst{Off: 0x1002},
	)

	view := NewDumpAsmView(DumpAsmOptions{}, DumpStyles{}, &typeinfo.Function{}, cfgForTest(t, instrs, &typeinfo.Function{}), nil)

	assertBlockInstOffsets(t, view.Blocks, [][]uint32{{0x1000, 0x1002}})
}

func TestNewDumpAsmViewCollapsesJumpTrampolines(t *testing.T) {
	instrs := []asm.DecodedInst{
		{Off: 0x1000, Len: 2, Op: asm.OpJcc, Mnemonic: "JNZ", Target: 0x1010},
		{Off: 0x1002, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: 0x1012},
		{Off: 0x1010, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: 0x1014},
		{Off: 0x1012, Len: 1, Op: asm.OpNOP, Mnemonic: "NOP"},
		{Off: 0x1014, Len: 1, Op: asm.OpNOP, Mnemonic: "NOP"},
	}
	cfg := cfgForTestWithOptions(t, instrs, &typeinfo.Function{}, machine.CFGOptions{CollapseJumps: true})

	view := NewDumpAsmView(DumpAsmOptions{}, DumpStyles{}, &typeinfo.Function{}, cfg, nil)

	assertBlockInstOffsets(t, view.Blocks, [][]uint32{{0x1000}, {0x1012}, {0x1014}})
	if got, want := cfg.InstructionOperands(view.Blocks[0].Instrs[0]), "L_1014"; got != want {
		t.Fatalf("collapsed jump operands = %q, want %q", got, want)
	}
}

func TestNewDumpAsmViewKeepsJumpTrampolinesWhenCollapseDisabled(t *testing.T) {
	instrs := []asm.DecodedInst{
		{Off: 0x1000, Len: 2, Op: asm.OpJcc, Mnemonic: "JNZ", Target: 0x1010},
		{Off: 0x1002, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: 0x1012},
		{Off: 0x1010, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: 0x1014},
		{Off: 0x1012, Len: 1, Op: asm.OpNOP, Mnemonic: "NOP"},
		{Off: 0x1014, Len: 1, Op: asm.OpNOP, Mnemonic: "NOP"},
	}
	cfg := cfgForTestWithOptions(t, instrs, &typeinfo.Function{}, machine.CFGOptions{})

	view := NewDumpAsmView(DumpAsmOptions{DumpOptions: DumpOptions{PreserveJumps: true}}, DumpStyles{}, &typeinfo.Function{}, cfg, nil)

	assertBlockInstOffsets(t, view.Blocks, [][]uint32{{0x1000}, {0x1002}, {0x1010}, {0x1012}, {0x1014}})
	if got, want := cfg.InstructionOperands(view.Blocks[0].Instrs[0]), "L_1010"; got != want {
		t.Fatalf("uncollapsed jump operands = %q, want %q", got, want)
	}
}

func TestNewDumpAsmViewKeepsJumpTrampolinesWithRawOperands(t *testing.T) {
	instrs := []asm.DecodedInst{
		{Off: 0x1000, Len: 2, Op: asm.OpJcc, Mnemonic: "JNZ", Target: 0x1010},
		{Off: 0x1002, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: 0x1012},
		{Off: 0x1010, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: 0x1014},
		{Off: 0x1012, Len: 1, Op: asm.OpNOP, Mnemonic: "NOP"},
		{Off: 0x1014, Len: 1, Op: asm.OpNOP, Mnemonic: "NOP"},
	}
	cfg := cfgForTestWithOptions(t, instrs, &typeinfo.Function{}, machine.CFGOptions{})

	view := NewDumpAsmView(DumpAsmOptions{RawOperands: true}, DumpStyles{}, &typeinfo.Function{}, cfg, nil)

	assertBlockInstOffsets(t, view.Blocks, [][]uint32{{0x1000}, {0x1002}, {0x1010}, {0x1012}, {0x1014}})
}

func TestRenderDumpAsmAlignsSourceCommentsWithInstructionComments(t *testing.T) {
	instrs := []asm.DecodedInst{
		{
			Off:      0x1000,
			Mnemonic: "MOV",
			Op:       asm.OpMOV,
			Dst:      asm.Operand{Kind: asm.OKReg, Reg: asm.RegAX},
			Src: asm.Operand{Kind: asm.OKMem, Mem: asm.MemRef{
				Base:    asm.RegBP,
				Disp:    0x6,
				MemSize: 2,
			}},
		},
	}
	view := DumpAsmView{
		Options: DumpAsmOptions{ShowOffsets: true},
		Annotations: &sem.Result{Operands: map[machine.AnnotationKey]sem.Annotation{
			{InstOff: 0x1000, Role: machine.OperandSrc}: {
				Kind: sem.AnnotationLocal,
				Text: "iplr",
				Var: &symresolve.LocalAccess{
					Local: typeinfo.FunctionVar{Name: "iplr", BPOffset: 0x6, Type: typeinfo.U16},
				},
			},
		}},
		Blocks:      []DumpAsmBlockView{{Label: "L_1000", Off: 0x1000, Instrs: instrs}},
		SourceFile:  "planet.c",
		SourceLines: []typeinfo.SourceLine{{Off: 0x1000, Line: 2931}},
	}

	var buf bytes.Buffer
	if err := RenderDumpAsm(&buf, view); err != nil {
		t.Fatalf("RenderDumpAsm: %v", err)
	}

	lines := strings.Split(strings.TrimRight(buf.String(), "\n"), "\n")
	if len(lines) != 2 {
		t.Fatalf("got %d rendered lines, want 2:\n%s", len(lines), buf.String())
	}
	sourceSemi := strings.IndexByte(lines[0], ';')
	instSemi := strings.IndexByte(lines[1], ';')
	if sourceSemi < 0 || instSemi < 0 {
		t.Fatalf("expected both lines to contain comments:\n%s", buf.String())
	}
	if sourceSemi != instSemi {
		t.Fatalf("source comment semicolon column = %d, instruction comment semicolon column = %d:\n%s", sourceSemi, instSemi, buf.String())
	}
	if got, want := lines[0][sourceSemi:], "; planet.c:2931"; got != want {
		t.Fatalf("source comment = %q, want %q", got, want)
	}
	if got, want := lines[1][instSemi:], "; ax, [bp+0x6]"; got != want {
		t.Fatalf("instruction comment = %q, want %q", got, want)
	}
}

func TestRenderDumpAsmHidesOffsetsByDefault(t *testing.T) {
	instrs := rawInsts(asm.DecodedInst{
		Off:      0x1000,
		Mnemonic: "PUSH",
		Dst:      asm.Operand{Kind: asm.OKReg, Reg: asm.RegBP},
	})
	view := DumpAsmView{
		Blocks: []DumpAsmBlockView{{Label: "L_1000", Off: 0x1000, Instrs: instrs}},
	}

	var buf bytes.Buffer
	if err := RenderDumpAsm(&buf, view); err != nil {
		t.Fatalf("RenderDumpAsm: %v", err)
	}

	if strings.Contains(buf.String(), "1000  PUSH") {
		t.Fatalf("rendered offset with ShowOffsets false:\n%s", buf.String())
	}
	if !strings.Contains(buf.String(), "PUSH      bp") {
		t.Fatalf("missing instruction without offset:\n%s", buf.String())
	}
}

func rawInsts(insts ...asm.DecodedInst) []asm.DecodedInst {
	return insts
}

func cfgForTest(t *testing.T, instrs []asm.DecodedInst, function *typeinfo.Function) *machine.CFG {
	t.Helper()
	return cfgForTestWithOptions(t, instrs, function, machine.CFGOptions{})
}

func cfgForTestWithOptions(t *testing.T, instrs []asm.DecodedInst, function *typeinfo.Function, opt machine.CFGOptions) *machine.CFG {
	t.Helper()
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	ctx := machine.NewFuncContext(nil, sdb, symresolve.NewResolver(img, sdb), function)
	cfg, err := machine.BuildCFG(ctx, instrs, false, opt)
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}
	return cfg
}

func assertBlockInstOffsets(t *testing.T, blocks []DumpAsmBlockView, want [][]uint32) {
	t.Helper()
	if len(blocks) != len(want) {
		t.Fatalf("got %d blocks, want %d", len(blocks), len(want))
	}
	for i, block := range blocks {
		if len(block.Instrs) != len(want[i]) {
			t.Fatalf("block[%d] got %d instrs, want %d", i, len(block.Instrs), len(want[i]))
		}
		for j, inst := range block.Instrs {
			if inst.Off != want[i][j] {
				t.Fatalf("blocks[%d].Instrs[%d].Off = 0x%04x, want 0x%04x", i, j, inst.Off, want[i][j])
			}
		}
	}
}
