package templates

import (
	"fmt"
	"io"
	"regexp"
	"slices"
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

// Win16DefinesView lists the Win16 constants and message crackers emitted
// to win16defines.h.
type Win16DefinesView struct {
	Defines  []DefineView
	Crackers []CrackerView
}

// CrackerView is one message cracker emitted as a function-like #define of
// its Win32 read.
type CrackerView struct {
	Name   string
	Params string
	Win32  string
}

// DefineView is one Win16 constant emitted as a guarded #define.
type DefineView struct {
	Name  string
	Value string
}

// NewWin16DefinesView collects the values of every Win16 #define constant
// family, the enums with no typedef, once each by name, and the crackers of
// the message rules sorted by name.
func NewWin16DefinesView(enums []*typeinfo.Enum, messages []*typeinfo.MessageRule) Win16DefinesView {
	view := Win16DefinesView{Crackers: messageCrackers(messages)}
	seen := make(map[string]bool)
	for _, e := range enums {
		if e.Typedef != nil {
			continue
		}
		for _, v := range e.Values {
			if seen[v.Name] {
				continue
			}
			seen[v.Name] = true
			value := fmt.Sprintf("0x%04X", v.Value)
			if v.Value < 0 {
				value = fmt.Sprintf("(%d)", v.Value)
			}
			view.Defines = append(view.Defines, DefineView{Name: v.Name, Value: value})
		}
	}
	return view
}

// messageCrackers returns the crackers of every message rule's payload
// parts, once each by name, sorted by name.
func messageCrackers(messages []*typeinfo.MessageRule) []CrackerView {
	byName := make(map[string]CrackerView)
	for _, message := range messages {
		for _, payload := range []*typeinfo.MessagePayloadRule{message.WParam, message.LParam} {
			if payload == nil {
				continue
			}
			for _, part := range []typeinfo.MessagePart{payload.Whole, payload.Loword, payload.Hiword} {
				if part.Get == nil {
					continue
				}
				params := make([]string, len(part.Get.Params))
				for i, param := range part.Get.Params {
					params[i] = param.Name
				}
				byName[part.Get.Name] = CrackerView{Name: part.Get.Name, Params: strings.Join(params, ", "), Win32: part.Win32}
			}
		}
	}
	crackers := make([]CrackerView, 0, len(byName))
	for _, cracker := range byName {
		crackers = append(crackers, cracker)
	}
	slices.SortFunc(crackers, func(a, b CrackerView) int { return strings.Compare(a.Name, b.Name) })
	return crackers
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
	if err := tmpl.Execute(&buf, view); err != nil {
		return err
	}

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
	if err := tmpl.Execute(&buf, view); err != nil {
		return err
	}

	formatted, err := formatCSource(buf.String())
	if err != nil {
		return err
	}

	fmt.Fprint(w, formatted)
	return nil
}

// reTypedefEnumDecl matches a `typedef enum [tag] { body } Name;` declaration.
var reTypedefEnumDecl = regexp.MustCompile(`(?s)\btypedef\s+enum\b\s*([A-Za-z_][A-Za-z0-9_]*)?\s*(\{.*?\})\s*([A-Za-z_][A-Za-z0-9_]*)\s*;`)

// RenderEnums rewrites the enums.h source so each enum name is a typedef of
// its original 16-bit integer type. The constants stay enumerators, but a C
// enum is int-sized, so declaring the name as the enum itself would widen
// struct fields past their Win16 layout.
func RenderEnums(w io.Writer, source string, enums []*typeinfo.Enum) error {
	typedefs := make(map[string]typeinfo.Type, len(enums))
	for _, e := range enums {
		if e.Typedef != nil {
			typedefs[e.Name] = e.Typedef
		}
	}
	var missing []string
	out := reTypedefEnumDecl.ReplaceAllStringFunc(source, func(decl string) string {
		m := reTypedefEnumDecl.FindStringSubmatch(decl)
		tag, body, name := m[1], m[2], m[3]
		typedef, ok := typedefs[name]
		if !ok {
			missing = append(missing, name)
			return decl
		}
		if tag == "" {
			tag = name
		}
		return fmt.Sprintf("enum %s %s;\ntypedef %s %s;", tag, body, typedef, name)
	})
	if len(missing) > 0 {
		return fmt.Errorf("enums.h declares enums missing from the symbol db: %s", strings.Join(missing, ", "))
	}
	_, err := io.WriteString(w, out)
	return err
}

// RenderCommon renders common.h, which includes the generated headers.
func RenderCommon(w io.Writer, view DumpCommonView) error {
	return renderHeader(w, "common.h.templ", view)
}

// RenderWin16Defines renders win16defines.h, the Win16 constants windows.h
// does not already define.
func RenderWin16Defines(w io.Writer, view Win16DefinesView) error {
	return renderHeader(w, "win16defines.h.templ", view)
}

// renderHeader executes a header template asset and writes the formatted C.
func renderHeader(w io.Writer, name string, view any) error {
	t := template.New(name).
		Funcs(sprig.TxtFuncMap()).
		Funcs(sprig.GenericFuncMap())

	tmpl, err := t.ParseFS(templatesFS, "assets/"+name)
	if err != nil {
		return err
	}

	var buf strings.Builder
	if err := tmpl.Execute(&buf, view); err != nil {
		return err
	}

	formatted, err := formatCSource(buf.String())
	if err != nil {
		return err
	}

	fmt.Fprint(w, formatted)
	return nil
}
