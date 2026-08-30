package templates

import (
	"fmt"
	"io"
	"strings"
	"text/template"

	"github.com/Masterminds/sprig/v3"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type DumpSourceView struct {
	Module         string
	Globals        []*typeinfo.GlobalVar
	Functions      []*typeinfo.Function
	FunctionBodies map[string]string
}

type DumpCommonView struct {
	Modules []string
}

func NewDumpSourceView(module string, globals []*typeinfo.GlobalVar, functions []*typeinfo.Function) DumpSourceView {
	view := DumpSourceView{
		Module:    module,
		Globals:   globals,
		Functions: functions,
	}
	return view
}

// NewDumpSourceViewWithBodies creates a source dump view with rendered function bodies.
func NewDumpSourceViewWithBodies(module string, globals []*typeinfo.GlobalVar, functions []*typeinfo.Function, bodies map[string]string) DumpSourceView {
	view := NewDumpSourceView(module, globals, functions)
	view.FunctionBodies = bodies
	return view
}

func RenderDumpHeader(w io.Writer, view DumpSourceView) error {
	t := template.New("source.h.templ").
		Funcs(sprig.TxtFuncMap()).
		Funcs(sprig.GenericFuncMap())

	tmpl, err := t.ParseFS(templatesFS, "assets/source.h.templ")
	if err != nil {
		return err
	}

	var buf strings.Builder
	tmpl.Execute(&buf, view)

	formatted, err := formatCSource(buf.String())
	if err != nil {
		return err
	}

	fmt.Fprint(w, formatted)
	return nil
}

func RenderDumpSource(w io.Writer, view DumpSourceView) error {
	t := template.New("source.c.templ").
		Funcs(sprig.TxtFuncMap()).
		Funcs(sprig.GenericFuncMap())

	tmpl, err := t.ParseFS(templatesFS, "assets/source.c.templ")
	if err != nil {
		return err
	}

	var buf strings.Builder
	tmpl.Execute(&buf, view)

	formatted, err := formatCSource(buf.String())
	if err != nil {
		return err
	}

	fmt.Fprint(w, formatted)
	return nil
}

func RenderCommon(w io.Writer, view DumpCommonView) error {
	t := template.New("common.h.templ").
		Funcs(sprig.TxtFuncMap()).
		Funcs(sprig.GenericFuncMap())

	tmpl, err := t.ParseFS(templatesFS, "assets/common.h.templ")
	if err != nil {
		return err
	}

	var buf strings.Builder
	tmpl.Execute(&buf, view)

	formatted, err := formatCSource(buf.String())
	if err != nil {
		return err
	}

	fmt.Fprint(w, formatted)
	return nil

}
