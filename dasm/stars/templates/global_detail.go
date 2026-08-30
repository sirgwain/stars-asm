package templates

import (
	"fmt"
	"io"
	"text/template"

	"github.com/Masterminds/sprig/v3"
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type GlobalDetailView struct {
	*typeinfo.GlobalVar
	StaticInitializer string
}

func NewGlobalDetailView(img *asm.ImageNE, g *typeinfo.GlobalVar, staticInitializer string) GlobalDetailView {
	return GlobalDetailView{
		GlobalVar:         g,
		StaticInitializer: staticInitializer,
	}
}

func RenderGlobalDetail(w io.Writer, view GlobalDetailView) error {
	t := template.New("global_detail.templ").
		Funcs(sprig.TxtFuncMap()).
		Funcs(sprig.GenericFuncMap()).
		Funcs(template.FuncMap{
			"hex16":      func(v uint16) string { return fmt.Sprintf("0x%04X", v) },
			"addr":       func(a typeinfo.Addr) string { return fmt.Sprintf("%04x:%04x", a.Seg, a.Off) },
			"typeString": typeString,
		})

	tmpl, err := t.ParseFS(templatesFS, "assets/global_detail.templ")
	if err != nil {
		return err
	}

	return tmpl.Execute(w, view)
}
