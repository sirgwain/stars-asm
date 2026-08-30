package templates

import (
	"bytes"
	"fmt"
	"io"
	"sort"
	"strings"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type DumpCFGView struct {
	Options     DumpCFGOptions
	Styles      DumpStyles
	CFG         *machine.CFG
	Blocks      []DumpCFGBlockView
	SourceFile  string
	SourceLines []typeinfo.SourceLine
	Annotations InstructionAnnotations
}

type DumpCFGBlockView struct {
	DumpAsmBlockView
	ID                    string
	Offsets               string
	KilledBeforeRead      string
	FlagsKilledBeforeRead bool
	RegLiveIn             string
	RegLiveOut            string
	BPLiveIn              string
	BPLiveOut             string
	FlagsLiveIn           bool
	FlagsLiveOut          bool
}

func NewDumpCFGView(cfg *machine.CFG, opt DumpCFGOptions, function *typeinfo.Function, annotations *sem.Result) DumpCFGView {
	view := DumpCFGView{Options: opt, CFG: cfg}
	if annotations != nil {
		view.Annotations = annotations
	}
	if function != nil {
		view.SourceFile = function.SourceFile.File
		view.SourceLines = function.SourceFile.Lines
	}

	for _, block := range cfg.BlocksInRange(machine.BlockRange{
		FromAddr: opt.FromAddr,
		ToAddr:   opt.ToAddr,
	}) {
		view.Blocks = append(view.Blocks, DumpCFGBlockView{
			DumpAsmBlockView: DumpAsmBlockView{
				Label:  block.Label,
				Off:    uint32(block.ID),
				Instrs: cfg.BlockInstrs(block.ID),
			},
			ID:                    block.ID.String(),
			Offsets:               fmt.Sprintf("0x%04x..0x%04x", block.ID, block.EndOff-1),
			KilledBeforeRead:      formatRegSet(block.KilledBeforeRead),
			FlagsKilledBeforeRead: block.FlagsKilledBeforeRead,
			RegLiveIn:             formatRegSet(block.RegLiveIn),
			RegLiveOut:            formatRegSet(block.RegLiveOut),
			BPLiveIn:              formatBPSet(block.BPLiveIn),
			BPLiveOut:             formatBPSet(block.BPLiveOut),
			FlagsLiveIn:           block.FlagsLiveIn,
			FlagsLiveOut:          block.FlagsLiveOut,
		})
	}

	return view
}

func RenderDumpCFG(w io.Writer, view DumpCFGView) error {
	t := template.New("dump_cfg.templ").
		Funcs(template.FuncMap{
			"blockInstsView": newDumpAsmBlockInstsView,
			"renderBytes": func(styles DumpStyles, b []byte) string {
				return styles.Bytes.Render(hexBytes(b))
			},
			"asmCommentLine": asmCommentLine,
			"sourceComments": sourceComments,
		}).
		Funcs(asmTemplateFuncs())

	tmpl, err := t.ParseFS(templatesFS, "assets/dump_cfg.templ", "assets/dump_asm_block_insts.templ")
	if err != nil {
		return err
	}

	var buf bytes.Buffer
	if err := tmpl.Execute(&buf, view); err != nil {
		return err
	}
	printHighlightedAsm(w, buf.String(), view.Options.ShowColor)
	return nil
}

func formatRegSet(regs map[asm.Reg]bool) string {
	if len(regs) == 0 {
		return "-"
	}
	out := make([]asm.Reg, 0, len(regs))
	for reg, live := range regs {
		if live {
			out = append(out, reg)
		}
	}
	if len(out) == 0 {
		return "-"
	}
	sort.Slice(out, func(i, j int) bool {
		return out[i] < out[j]
	})
	names := make([]string, 0, len(out))
	for _, reg := range out {
		names = append(names, reg.String())
	}
	return strings.Join(names, ", ")
}

func formatBPSet(offsets map[int]bool) string {
	if len(offsets) == 0 {
		return "-"
	}
	out := make([]int, 0, len(offsets))
	for off, live := range offsets {
		if live {
			out = append(out, off)
		}
	}
	if len(out) == 0 {
		return "-"
	}
	sort.Ints(out)
	names := make([]string, 0, len(out))
	for _, off := range out {
		names = append(names, formatBPOffset(off))
	}
	return strings.Join(names, ", ")
}

func formatBPOffset(off int) string {
	if off < 0 {
		return fmt.Sprintf("bp%#x", off)
	}
	return fmt.Sprintf("bp+%#x", off)
}
