package templates

import (
	"bytes"
	"embed"
	"fmt"
	"io"
	"strings"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

const asmCommentColumn = 36

//go:embed assets/*
var templatesFS embed.FS

// InstructionAnnotations renders original assembly operands with source facts.
type InstructionAnnotations interface {
	InstructionOperands(inst asm.DecodedInst) string
	InstructionComment(inst asm.DecodedInst) string
}

// instructionAnnotations returns a nil interface for nil annotation results.
func instructionAnnotations(annotations InstructionAnnotations) InstructionAnnotations {
	if annotations == nil {
		return nil
	}
	return annotations
}

type DumpAsmView struct {
	Options     DumpAsmOptions
	Styles      DumpStyles
	CFG         *machine.CFG
	Blocks      []DumpAsmBlockView
	SourceFile  string
	SourceLines []typeinfo.SourceLine
	Annotations InstructionAnnotations
}

type DumpAsmBlockView struct {
	Label  string
	Off    uint32
	Instrs []asm.DecodedInst
}

type DumpAsmBlockInstsView struct {
	Options     DumpAsmOptions
	Styles      DumpStyles
	CFG         *machine.CFG
	Block       DumpAsmBlockView
	SourceFile  string
	SourceLines []typeinfo.SourceLine
	Annotations InstructionAnnotations
}

func NewDumpAsmView(
	opt DumpAsmOptions,
	styles DumpStyles,
	function *typeinfo.Function,
	cfg *machine.CFG,
	annotations InstructionAnnotations,
) DumpAsmView {
	view := DumpAsmView{
		Options:     opt,
		Styles:      styles,
		CFG:         cfg,
		SourceFile:  function.SourceFile.File,
		SourceLines: function.SourceFile.Lines,
		Annotations: instructionAnnotations(annotations),
	}

	for _, block := range cfg.BlocksInRange(machine.BlockRange{
		FromAddr: opt.FromAddr,
		ToAddr:   opt.ToAddr,
	}) {
		view.Blocks = append(view.Blocks, DumpAsmBlockView{
			Label:  block.Label,
			Off:    uint32(block.ID),
			Instrs: cfg.BlockInstrs(block.ID),
		})
	}

	return view
}

func RenderDumpAsm(w io.Writer, view DumpAsmView) error {
	t := template.New("dump_asm.templ").
		Funcs(template.FuncMap{
			"blockInstsView": newDumpAsmBlockInstsView,
			"renderBytes": func(styles DumpStyles, b []byte) string {
				return styles.Bytes.Render(hexBytes(b))
			},
			"asmCommentLine": asmCommentLine,
			"sourceComments": sourceComments,
		}).
		Funcs(asmTemplateFuncs())

	tmpl, err := t.ParseFS(templatesFS, "assets/dump_asm.templ", "assets/dump_asm_block_insts.templ")
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

func RenderDumpAsmBlockInsts(w io.Writer, view DumpAsmBlockInstsView) error {
	t := template.New("dump_asm_block_insts.templ").
		Funcs(template.FuncMap{
			"renderBytes": func(styles DumpStyles, b []byte) string {
				return styles.Bytes.Render(hexBytes(b))
			},
			"asmCommentLine": asmCommentLine,
			"sourceComments": sourceComments,
		}).
		Funcs(asmTemplateFuncs())

	tmpl, err := t.ParseFS(templatesFS, "assets/dump_asm_block_insts.templ")
	if err != nil {
		return err
	}

	return tmpl.ExecuteTemplate(w, "dump_asm_block_insts", view)
}

// asmTemplateFuncs returns helpers used by the shared asm instruction partial.
func asmTemplateFuncs() template.FuncMap {
	return template.FuncMap{
		"instructionMnemonic": func(cfg *machine.CFG, inst asm.DecodedInst) string {
			if cfg == nil {
				return inst.Mnemonic
			}
			return cfg.InstructionMnemonic(inst)
		},
		"instructionOperands": func(cfg *machine.CFG, annotations InstructionAnnotations, inst asm.DecodedInst) string {
			if cfg != nil && (cfg.Calls[inst.Off] != nil || cfg.Jumps[inst.Off] != nil) {
				return cfg.InstructionOperands(inst)
			}
			if annotations == nil {
				if cfg == nil {
					return inst.Operands()
				}
				return cfg.InstructionOperands(inst)
			}
			return annotations.InstructionOperands(inst)
		},
		"instructionComment": func(cfg *machine.CFG, annotations InstructionAnnotations, inst asm.DecodedInst) string {
			if cfg != nil && (cfg.Calls[inst.Off] != nil || cfg.Jumps[inst.Off] != nil) {
				return cfg.InstructionComment(inst)
			}
			if annotations == nil {
				if cfg == nil {
					return ""
				}
				return cfg.InstructionComment(inst)
			}
			return annotations.InstructionComment(inst)
		},
	}
}

func newDumpAsmBlockInstsView(root any, block any) DumpAsmBlockInstsView {
	asmBlock := DumpAsmBlockView{}
	switch b := block.(type) {
	case DumpAsmBlockView:
		asmBlock = b
	case DumpCFGBlockView:
		asmBlock = b.DumpAsmBlockView
	case DumpEffectsBlockView:
		asmBlock = b.DumpAsmBlockView
	case DumpSemBlockView:
		asmBlock = b.DumpAsmBlockView
	}

	switch view := root.(type) {
	case DumpAsmView:
		return DumpAsmBlockInstsView{
			Options:     view.Options,
			Styles:      view.Styles,
			CFG:         view.CFG,
			Block:       asmBlock,
			SourceFile:  view.SourceFile,
			SourceLines: view.SourceLines,
			Annotations: view.Annotations,
		}
	case DumpCFGView:
		return DumpAsmBlockInstsView{
			Options: DumpAsmOptions{
				DumpOptions: view.Options.DumpOptions,
			},
			Styles:      view.Styles,
			CFG:         view.CFG,
			Block:       asmBlock,
			SourceFile:  view.SourceFile,
			SourceLines: view.SourceLines,
			Annotations: view.Annotations,
		}
	case DumpEffectsView:
		return DumpAsmBlockInstsView{
			Options: DumpAsmOptions{
				DumpOptions: view.Options.DumpOptions,
				ShowOffsets: view.Options.ShowOffsets,
			},
			Styles:      view.Styles,
			CFG:         view.CFG,
			Block:       asmBlock,
			SourceFile:  view.SourceFile,
			SourceLines: view.SourceLines,
			Annotations: view.Annotations,
		}
	case DumpSemView:
		return DumpAsmBlockInstsView{
			Options: DumpAsmOptions{
				DumpOptions: view.Options.DumpOptions,
				ShowOffsets: view.Options.ShowOffsets,
			},
			Styles:      view.Styles,
			CFG:         view.CFG,
			Block:       asmBlock,
			SourceFile:  view.SourceFile,
			SourceLines: view.SourceLines,
			Annotations: view.Annotations,
		}
	default:
		return DumpAsmBlockInstsView{Block: asmBlock}
	}
}

func sourceComments(file string, lines []typeinfo.SourceLine, off uint32) []string {
	if file == "" || len(lines) == 0 {
		return nil
	}
	var comments []string
	for _, line := range lines {
		if line.Off == off {
			comments = append(comments, fmt.Sprintf("%s:%d", file, line.Line))
		}
	}
	return comments
}

// asmCommentLine pads prefix so source and instruction comments share a stable column.
func asmCommentLine(prefix, comment string) string {
	padding := 0
	if n := len(prefix); n < asmCommentColumn {
		padding = asmCommentColumn - n
	} else if n > asmCommentColumn {
		padding = 1
	}
	return prefix + strings.Repeat(" ", padding) + "; " + comment
}

func hexBytes(b []byte) string {
	var sb strings.Builder
	for i := range b {
		if i > 0 {
			sb.WriteByte(' ')
		}
		fmt.Fprintf(&sb, "%02x", b[i])
	}
	return sb.String()
}
