package stars

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"io"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/region"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpFuncRegion runs the analysis pipeline through IR, rebuilds the IR
// control flow as structured regions, and renders the result as C.
func DumpFuncRegion(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpOptions) error {
	analysis, err := analyzeFunc(img, sdb, fs, opt, machine.NewWriteSummaries(img, sdb, symresolve.NewResolver(img, sdb)))
	if err != nil {
		return err
	}
	return renderFuncRegion(w, analysis, opt)
}

// renderFuncRegion rebuilds an analyzed function's IR control flow as
// structured regions and renders it as C.
func renderFuncRegion(w io.Writer, analysis FuncAnalysis, opt DumpOptions) error {
	fn, err := region.Build(analysis.IR)
	if err != nil {
		return err
	}
	return startemplates.RenderDumpRegion(w, fn, opt)
}
