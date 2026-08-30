package stars

import (
	"io"

	"github.com/charmbracelet/lipgloss"
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpFuncAsm disassembles a 16-bit x86 function and prints the disassembly.
func DumpFuncAsm(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpAsmOptions) error {
	analysis, err := analyzeFunc(img, sdb, fs, opt.DumpOptions)
	if err != nil {
		return err
	}

	return renderFuncAsm(w, fs, analysis, opt)
}

// renderFuncAsm prints the disassembly from an existing function analysis.
func renderFuncAsm(w io.Writer, fs *typeinfo.Function, analysis FuncAnalysis, opt DumpAsmOptions) error {
	styles := startemplates.DumpStyles{
		Bytes: lipgloss.NewStyle().Width(20),
	}

	return startemplates.RenderDumpAsm(w, startemplates.NewDumpAsmView(
		opt,
		styles,
		fs,
		analysis.CFG,
		analysis.Annotations,
	))
}
