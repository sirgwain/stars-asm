package stars

import (
	"io"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpGlobalDetail prints symbol details for a global.
func DumpGlobalDetail(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, g *typeinfo.GlobalVar) error {
	staticInitializer := ""
	si, ok := LoadGlobalInitializer(img, sdb, g)
	if ok {
		staticInitializer = FormatInitializerC(sdb, si)
	}
	return startemplates.RenderGlobalDetail(w, startemplates.NewGlobalDetailView(img, g, staticInitializer))
}
