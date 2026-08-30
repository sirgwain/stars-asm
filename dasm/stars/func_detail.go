package stars

import (
	"io"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpFuncDetail prints symbol details for a function.
func DumpFuncDetail(w io.Writer, img *asm.ImageNE, f *typeinfo.Function, opt FuncDetailOptions) error {
	return startemplates.RenderFuncDetail(w, startemplates.NewFuncDetailView(img, f, opt))
}
