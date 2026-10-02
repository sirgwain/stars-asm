package stars

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"io"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpFuncEffects disassembles and then extracts effects from the disassembly.
func DumpFuncEffects(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpEffectsOptions) error {
	analysis, err := analyzeFunc(img, sdb, fs, opt.DumpOptions, machine.NewWriteSummaries(img, sdb, symresolve.NewResolver(img, sdb)))
	if err != nil {
		return err
	}

	return renderFuncEffects(w, fs, analysis, opt)
}

// renderFuncEffects prints machine effects from an existing function analysis.
func renderFuncEffects(w io.Writer, fs *typeinfo.Function, analysis FuncAnalysis, opt DumpEffectsOptions) error {
	return startemplates.RenderDumpEffects(w, startemplates.NewDumpEffectsView(&analysis.Effects, opt, fs, analysis.Annotations))
}
