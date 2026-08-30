package templates

import (
	"fmt"
	"io"
	"strings"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpSemView is the template view for semantic effects.
type DumpSemView struct {
	Options     DumpSemOptions
	Styles      DumpStyles
	CFG         *machine.CFG
	Blocks      []DumpSemBlockView
	SourceFile  string
	SourceLines []typeinfo.SourceLine
	Annotations *sem.Result
}

// DumpSemBlockView is the template view for one semantic block.
type DumpSemBlockView struct {
	DumpAsmBlockView
	ID      string
	Insts   string
	Offsets string
	Removed bool
	Preds   []string
	Succs   []string
	Machine []string
	Effects []string
}

// NewDumpSemView creates a semantic effects dump view.
func NewDumpSemView(
	fn sem.Func,
	machineEffects *machine.FuncEffects,
	opt DumpSemOptions,
	function *typeinfo.Function,
	annotations *sem.Result,
	effectAnnotations *sem.Result,
) DumpSemView {
	view := DumpSemView{
		Options:     opt,
		CFG:         fn.CFG,
		Annotations: annotations,
	}
	if function != nil {
		view.SourceFile = function.SourceFile.File
		view.SourceLines = function.SourceFile.Lines
	}

	effectsByBlock := make(map[machine.BlockID][]sem.Effect, len(fn.Blocks))
	for _, blockEffects := range fn.Blocks {
		effectsByBlock[blockEffects.ID] = blockEffects.Effects
	}
	machineEffectsByBlock := make(map[machine.BlockID][]machine.Effect)
	if machineEffects != nil {
		machineEffectsByBlock = make(map[machine.BlockID][]machine.Effect, len(machineEffects.Blocks))
		for _, blockEffects := range machineEffects.Blocks {
			machineEffectsByBlock[blockEffects.Block] = blockEffects.Effects
		}
	}
	semBlocksByID := make(map[machine.BlockID]*sem.Block)
	for _, block := range fn.Blocks {
		semBlocksByID[block.ID] = &block
	}

	for _, block := range fn.CFG.BlocksInRange(machine.BlockRange{
		FromAddr: opt.FromAddr,
		ToAddr:   opt.ToAddr,
	}) {
		view.Blocks = append(view.Blocks, DumpSemBlockView{
			DumpAsmBlockView: DumpAsmBlockView{
				Label:  block.Label,
				Off:    uint32(block.ID),
				Instrs: fn.CFG.BlockInstrs(block.ID),
			},						
			ID:      block.ID.String(),
			Insts:   fmt.Sprintf("%d..%d", block.StartIdx, block.EndIdx-1),
			Offsets: fmt.Sprintf("0x%04x..0x%04x", uint32(block.ID), block.EndOff-1),
			Preds:   formatBlockIDs(fn.CFG.Predecessors(block.ID)),
			Succs:   formatBlockIDs(fn.CFG.Successors(block.ID)),
			Machine: formatEffects(machineEffectsByBlock[block.ID], effectAnnotations),
			Effects: formatSemEffects(effectsByBlock[block.ID]),
			Removed: semBlocksByID[block.ID] == nil,
		})
	}

	return view
}

// RenderDumpSem renders a semantic effects dump.
func RenderDumpSem(w io.Writer, view DumpSemView) error {
	t := template.New("dump_sem.templ").
		Funcs(template.FuncMap{
			"join":           strings.Join,
			"blockInstsView": newDumpAsmBlockInstsView,
			"renderAsmBlock": renderDumpSemAsmBlock,
			"highlightLines": HighlightLines,
			"highlightText":  HighlightTextLines,
			"renderBytes":    func(styles DumpStyles, b []byte) string { return styles.Bytes.Render(hexBytes(b)) },
			"asmCommentLine": asmCommentLine,
			"sourceComments": sourceComments,
		}).
		Funcs(asmTemplateFuncs())

	tmpl, err := t.ParseFS(templatesFS, "assets/dump_sem.templ", "assets/dump_asm_block_insts.templ")
	if err != nil {
		return err
	}

	return tmpl.Execute(w, view)
}

// renderDumpSemAsmBlock renders asm instructions for a semantic block.
func renderDumpSemAsmBlock(root DumpSemView, block DumpSemBlockView) (string, error) {
	var buf strings.Builder
	if comments := sourceComments(root.SourceFile, root.SourceLines, block.Off); len(comments) > 0 {
		label := block.Label + ":"
		for _, comment := range comments {
			fmt.Fprintln(&buf, asmCommentLine(label, comment))
			label = ""
		}
	} else {
		fmt.Fprintf(&buf, "%s:\n", block.Label)
	}

	if err := RenderDumpAsmBlockInsts(&buf, newDumpAsmBlockInstsView(root, block)); err != nil {
		return "", err
	}
	return strings.TrimRight(buf.String(), "\n"), nil
}

// formatSemEffects renders semantic effects.
func formatSemEffects(effects []sem.Effect) []string {
	if len(effects) == 0 {
		return nil
	}
	lines := make([]string, 0, len(effects))
	for _, effect := range effects {
		lines = append(lines, sem.FormatEffect(effect))
	}
	return lines
}
