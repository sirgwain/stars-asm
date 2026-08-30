package stars

import (
	"io"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpFuncIR runs the normal analysis pipeline through SEM, lowers the final
// semantic function into low-level IR, and renders explicit-block ugly C.
func DumpFuncIR(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpOptions) error {
	analysis, err := analyzeFunc(img, sdb, fs, opt)
	if err != nil {
		return err
	}
	return renderFuncIR(w, analysis, opt)
}

func renderFuncIR(w io.Writer, analysis FuncAnalysis, opt DumpOptions) error {
	return startemplates.RenderDumpIR(w, startemplates.NewDumpIRView(analysis.IR, opt))
}
