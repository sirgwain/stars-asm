package stars

import (
	"io"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpFuncIR runs the normal analysis pipeline through SEM, lowers the final
// semantic function into low-level IR, and renders each analysis stage by block.
func DumpFuncIR(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpIROptions) error {
	analysis, err := analyzeFunc(img, sdb, fs, opt.DumpOptions)
	if err != nil {
		return err
	}
	return renderFuncIRWithSections(w, analysis, opt)
}

func renderFuncIR(w io.Writer, analysis FuncAnalysis, opt DumpIROptions) error {
	return startemplates.RenderDumpIR(w, startemplates.NewDumpIRView(analysis.IR, opt))
}

// renderFuncIRWithSections renders the analysis stages together with low-level IR.
func renderFuncIRWithSections(w io.Writer, analysis FuncAnalysis, opt DumpIROptions) error {
	semView := startemplates.NewDumpSemView(
		analysis.Sem,
		&analysis.Effects,
		startemplates.DumpSemOptions{
			DumpOptions: opt.DumpOptions,
			ShowAsm:     opt.ShowAsm,
			ShowEffects: opt.ShowEffects,
		},
		nil,
		analysis.Annotations,
		analysis.Annotations,
	)
	view := startemplates.NewDumpIRViewWithSem(analysis.IR, semView, DumpIROptions{
		DumpOptions: opt.DumpOptions,
		ShowAsm:     opt.ShowAsm,
		ShowEffects: opt.ShowEffects,
		ShowSem:     opt.ShowSem,
		ShowIR:      opt.ShowIR,
	})
	return startemplates.RenderDumpIR(w, view)
}
