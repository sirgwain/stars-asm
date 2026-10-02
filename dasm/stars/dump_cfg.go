package stars

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"io"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpFuncCFG disassembles a function, builds its CFG, and prints block state.
func DumpFuncCFG(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpCFGOptions) error {
	analysis, err := analyzeFunc(img, sdb, fs, opt.DumpOptions, machine.NewWriteSummaries(img, sdb, symresolve.NewResolver(img, sdb)))
	if err != nil {
		return err
	}

	return startemplates.RenderDumpCFG(w, startemplates.NewDumpCFGView(analysis.CFG, opt, fs, analysis.Annotations))
}
