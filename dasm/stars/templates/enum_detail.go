package templates

import (
	"io"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type EnumDetailView struct {
	Name   string
	Values []typeinfo.EnumValue
}

func NewEnumDetailView(e *typeinfo.Enum) EnumDetailView {
	if e == nil {
		return EnumDetailView{}
	}
	return EnumDetailView{
		Name:   e.Name,
		Values: e.Values,
	}
}

func RenderEnumDetail(w io.Writer, view EnumDetailView) error {
	tmpl, err := template.ParseFS(templatesFS, "assets/enum_detail.templ")
	if err != nil {
		return err
	}
	return tmpl.Execute(w, view)
}
