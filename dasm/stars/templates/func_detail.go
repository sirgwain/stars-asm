package templates

import (
	"bytes"
	"fmt"
	"io"
	"reflect"
	"strings"
	"text/template"

	"github.com/Masterminds/sprig/v3"
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type FuncDetailView struct {
	*typeinfo.Function
	Segment      uint16
	CommentStyle FuncDetailCommentStyle
	ShowColor    bool
}

type FuncDetailCommentStyle int

const (
	FuncDetailCommentC FuncDetailCommentStyle = iota
	FuncDetailCommentAsm
)

type FuncDetailOptions struct {
	CommentStyle FuncDetailCommentStyle
	ShowColor    bool
}

func NewFuncDetailView(img *asm.ImageNE, f *typeinfo.Function, opt FuncDetailOptions) FuncDetailView {
	return FuncDetailView{
		Function:     f,
		Segment:      f.Addr.Seg,
		CommentStyle: opt.CommentStyle,
		ShowColor:    opt.ShowColor,
	}
}

func RenderFuncDetail(w io.Writer, view FuncDetailView) error {
	t := template.New("func_detail.templ").
		Funcs(sprig.GenericFuncMap()).
		Funcs(sprig.TxtFuncMap()).
		Funcs(template.FuncMap{
			"hex16":        func(v uint16) string { return fmt.Sprintf("0x%04X", v) },
			"hex32":        func(v uint32) string { return fmt.Sprintf("0x%04X", v) },
			"addr":         func(a typeinfo.Addr) string { return fmt.Sprintf("%04x:%04x", a.Seg, a.Off) },
			"callValue":    callValue,
			"field":        fieldValue,
			"typeString":   typeString,
			"valueString":  valueString,
			"varLine":      varLine,
			"paramLine":    paramLine,
			"indent":       indent,
			"scopeText":    scopeText,
			"scopeTree":    scopeTree,
			"commentOpen":  commentOpen,
			"commentLine":  commentLine,
			"commentText":  commentText,
			"commentClose": commentClose,
		})

	tmpl, err := t.ParseFS(templatesFS, "assets/func_detail.templ")
	if err != nil {
		return err
	}

	var buf bytes.Buffer
	if err := tmpl.Execute(&buf, view); err != nil {
		return err
	}
	if view.CommentStyle == FuncDetailCommentAsm {
		printHighlightedAsm(w, buf.String(), view.ShowColor)
	} else {
		printHighlightedC(w, buf.String(), view.ShowColor)
	}
	return nil
}

func commentOpen(view FuncDetailView) string {
	if view.CommentStyle == FuncDetailCommentAsm {
		return ""
	}
	return "/*\n"
}

func commentLine(view FuncDetailView, line string) string {
	if view.CommentStyle == FuncDetailCommentAsm {
		if line == "" {
			return ";"
		}
		return "; " + line
	}
	return line
}

func commentText(view FuncDetailView, text string) string {
	text = strings.TrimRight(text, "\n")
	if text == "" {
		return ""
	}
	lines := strings.Split(text, "\n")
	for i, line := range lines {
		lines[i] = commentLine(view, line)
	}
	return strings.Join(lines, "\n")
}

func commentClose(view FuncDetailView) string {
	if view.CommentStyle == FuncDetailCommentAsm {
		return ""
	}
	return "*/"
}

func typeString(typ typeinfo.Type) string {
	if typ == nil {
		return ""
	}
	return typ.String()
}

func valueString(v fmt.Stringer) string {
	return v.String()
}

func varLine(v typeinfo.FunctionVar) string {
	const typeWidth = 16
	typ := v.TypeString()
	if typ == "" {
		return v.Name
	}
	return fmt.Sprintf("%-*.*s %-14s %s", typeWidth, typeWidth, typ, v.Name, v.BPString())
}

func paramLine(p typeinfo.FunctionVar) string {
	const typeWidth = 16
	typ := p.TypeString()
	if typ == "" {
		return p.Name
	}
	return fmt.Sprintf("%-*.*s %-14s %s", typeWidth, typeWidth, typ, p.Name, p.BPString())
}

func indent(depth int) string {
	if depth <= 0 {
		return ""
	}
	return strings.Repeat("  ", depth)
}

func scopeText(f *typeinfo.Function, s typeinfo.Scope) string {
	var b strings.Builder
	writeScope(&b, f, s)
	return b.String()
}

func scopeTree(f *typeinfo.Function) string {
	var b strings.Builder
	for _, scope := range f.RootScopes() {
		writeScope(&b, f, scope)
	}
	return strings.TrimRight(b.String(), "\n")
}

func writeScope(b *strings.Builder, f *typeinfo.Function, s typeinfo.Scope) {
	pad := indent(s.Depth)
	fmt.Fprintf(b, "%s  %s\n", pad, s.Header())
	for _, v := range f.ScopeLocals(s.ID) {
		fmt.Fprintf(b, "%s    %s\n", pad, varLine(v))
	}
	for _, child := range f.ChildScopes(s.ID) {
		writeScope(b, f, child)
	}
}

func fieldValue(v any, name string) any {
	value := indirectValue(reflect.ValueOf(v))
	if !value.IsValid() {
		return nil
	}
	field := value.FieldByName(name)
	if !field.IsValid() || !field.CanInterface() {
		return nil
	}
	return field.Interface()
}

func callValue(v any, name string, args ...any) any {
	value := reflect.ValueOf(v)
	if !value.IsValid() {
		return nil
	}
	method := value.MethodByName(name)
	if !method.IsValid() {
		method = indirectValue(value).MethodByName(name)
	}
	if !method.IsValid() || method.Type().NumIn() != len(args) {
		return nil
	}

	in := make([]reflect.Value, 0, len(args))
	for i, arg := range args {
		argValue := reflect.ValueOf(arg)
		want := method.Type().In(i)
		if !argValue.IsValid() {
			in = append(in, reflect.Zero(want))
			continue
		}
		if argValue.Type().AssignableTo(want) {
			in = append(in, argValue)
			continue
		}
		if argValue.Type().ConvertibleTo(want) {
			in = append(in, argValue.Convert(want))
			continue
		}
		return nil
	}

	out := method.Call(in)
	if len(out) == 0 {
		return nil
	}
	return out[0].Interface()
}

func indirectValue(v reflect.Value) reflect.Value {
	for v.IsValid() && (v.Kind() == reflect.Pointer || v.Kind() == reflect.Interface) {
		if v.IsNil() {
			return reflect.Value{}
		}
		v = v.Elem()
	}
	return v
}
