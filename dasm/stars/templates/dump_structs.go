package templates

import (
	"bytes"
	"fmt"
	"io"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type DumpStructsView struct {
	Options             DumpOptions
	ForwardDeclarations []StructDetailView
	Structs             []StructDetailView
}

func NewDumpStructsView(structs []*typeinfo.Struct, opt DumpOptions) DumpStructsView {
	view := DumpStructsView{
		Options:             opt,
		ForwardDeclarations: make([]StructDetailView, 0, len(structs)),
		Structs:             make([]StructDetailView, 0, len(structs)),
	}
	for _, s := range structs {
		detail := NewStructDetailView(s, opt)
		view.ForwardDeclarations = append(view.ForwardDeclarations, detail)
		detail.DefinitionOnly = true
		view.Structs = append(view.Structs, detail)
	}
	return view
}

func RenderDumpStructs(w io.Writer, view DumpStructsView) error {
	t := template.New("structs.h.templ").
		Funcs(template.FuncMap{
			"hex": func(v int) string { return fmt.Sprintf("%#x", v) },
		})

	tmpl, err := t.ParseFS(templatesFS, "assets/structs.h.templ", "assets/struct_detail.templ")
	if err != nil {
		return err
	}
	var buf bytes.Buffer
	if err := tmpl.Execute(&buf, view); err != nil {
		return err
	}

	formatted, err := formatCSource(buf.String())
	if err != nil {
		return err
	}

	printHighlightedC(w, formatted, view.Options.ShowColor)
	return nil
}
