package stars

import (
	"io"

	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpEnumDetail prints a C-style declaration for an enum.
func DumpEnumDetail(w io.Writer, _ *typeinfo.SymbolDB, e *typeinfo.Enum) error {
	return startemplates.RenderEnumDetail(w, startemplates.NewEnumDetailView(e))
}
