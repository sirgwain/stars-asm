package typeinfo

import (
	"cmp"
	"fmt"
	"slices"
	"strings"
)

const OverrideModule = "override"
const GlobalsModule = "globals"
const ParamSemanticResourceNameOrID = "resource_name_or_id"
const ParamSemanticCallbackProc = "callback_proc"

// Function describes a function from nb09 data
type Function struct {
	Name       string
	Addr       Addr
	Len        uint16
	Module     string
	Conv       CallConv
	Ret        Type
	Params     []FunctionVar
	Vars       []FunctionVar
	Scopes     []Scope
	Labels     []Label
	SourceFile SourceFile
	VarArgs    bool

	labelsByOff map[uint32]Label

	// loaded outside symboldb
	Body string
}

func (f *Function) Kind() Kind { return KFunc }
func (f *Function) Bytes() int { return 0 }
func (f *Function) String() string {
	params := make([]string, 0, len(f.Params))
	for _, param := range f.Params {
		if param.Name == "" {
			params = append(params, typeString(param.Type))
			continue
		}
		params = append(params, param.String())
	}
	if f.VarArgs {
		params = append(params, "...")
	}
	ret := typeString(f.Ret)
	if ret == "" {
		ret = "void"
	}
	return fmt.Sprintf("%s(%s)", ret, strings.Join(params, ", "))
}

func (f *Function) AddLabel(label Label) {
	f.Labels = append(f.Labels, label)
	if f.labelsByOff == nil {
		f.labelsByOff = make(map[uint32]Label)
	}
	f.labelsByOff[label.Addr.Off] = label
}

func (f *Function) GetLabel(off uint32) *Label {
	if label, ok := f.labelsByOff[off]; ok {
		return &label
	}
	return nil
}

func (f *Function) IsOverride() bool { return f.Module == OverrideModule }

// CallConv is a best-effort calling convention classifier.
// (Primarily used for Win16 ABI decisions like stack cleanup.)
type CallConv uint8

const (
	CCUnknown CallConv = iota
	CCCdecl
	CCPascal
	CCStdcall
	CCDxaxCx // Win16 compiler helper: val in DX:AX, shift/extra in CX
)

func (c CallConv) String() string {
	switch c {
	case CCUnknown:
		return "unknown"
	case CCCdecl:
		return "cdecl"
	case CCPascal:
		return "pascal"
	case CCStdcall:
		return "stdcall"
	case CCDxaxCx:
		return "dxax_cx"
	default:
		return fmt.Sprintf("CallConv(%d)", uint8(c))
	}
}

func CallConvFromString(s string) CallConv {
	switch s {
	case "cdecl":
		return CCCdecl
	case "pascal":
		return CCPascal
	case "stdcall":
		return CCStdcall
	case "dxax_cx":
		return CCDxaxCx
	default:
		return CCUnknown
	}
}

// FunctionVar is a function local variable or param
type FunctionVar struct {
	Name     string
	Type     Type
	BPOffset int
	Register Register
	ScopeID  int
	Semantic string
}

func (f *FunctionVar) variable()     {}
func (f *FunctionVar) VarType() Type { return f.Type }

func (f *FunctionVar) String() string {
	return TypeDecl(f.Type, f.Name)
}

func (f *FunctionVar) CDecl() string {
	return TypeDecl(f.Type, f.Name)
}

func (f *FunctionVar) TypeString() string {
	return typeString(f.Type)
}

func (f *FunctionVar) BPString() string {
	return varLocationString(f.BPOffset, f.Register)
}

// Words returns the Win16 stack width (in 16-bit words) for this parameter.
func (f *FunctionVar) Words() int {
	if f.Type.Bytes() > 0 {
		return (f.Type.Bytes() + 1) / 2
	}
	return 1
}

func bpString(off int) string {
	if off == 0 {
		return ""
	}
	if off > 0 {
		return fmt.Sprintf("[BP+%#x]", off)
	}
	return fmt.Sprintf("[BP%#x]", off)
}

// varLocationString renders known stack and register storage for a variable.
func varLocationString(bpOff int, reg Register) string {
	regText := ""
	if reg != RegNone {
		regText = reg.String()
	}
	bpText := bpString(bpOff)
	switch {
	case regText != "" && bpText != "":
		return fmt.Sprintf("%s %s", regText, bpText)
	case regText != "":
		return regText
	default:
		return bpText
	}
}

type Scope struct {
	ID       int
	ParentID int
	Depth    int
	Name     string
	Addr     Addr
	Len      uint16
}

func (s Scope) Header() string {
	text := fmt.Sprintf("block %04X:%04X", s.Addr.Seg, uint16(s.Addr.Off))
	if s.Len > 0 {
		text += fmt.Sprintf("  len=0x%X", s.Len)
	}
	return text
}

type Label struct {
	Name string
	Addr Addr
}

//
// Source Mapping
//

// SourceFile describes where the function came from in source code.
type SourceFile struct {
	File  string
	First int
	Last  int
	Lines []SourceLine
}

// SourceLine maps a code offset to a source line number.
// i.e. 7250 line 2934 (for planet.c, which is our function Module)
type SourceLine struct {
	Off  uint32
	Line uint16
}

func (f *Function) CDecl() string {
	name := f.Name
	if name == "" {
		name = "/*anon*/"
	}
	params := make([]string, 0, len(f.Params))
	for _, param := range f.Params {
		params = append(params, param.String())
	}
	ret := typeString(f.Ret)
	if ret == "" {
		ret = "void"
	}
	return fmt.Sprintf("%s %s(%s)", ret, name, strings.Join(params, ", "))
}

func (f *Function) RootLocals() []FunctionVar {
	return f.ScopeLocals(0)
}

func (f *Function) RootScopes() []Scope {
	return f.ChildScopes(0)
}

func (f *Function) ScopeLocals(scopeID int) []FunctionVar {
	var vars []FunctionVar
	for _, v := range f.Vars {
		if v.ScopeID == scopeID {
			vars = append(vars, v)
		}
	}
	slices.SortFunc(vars, func(a, b FunctionVar) int {
		return cmp.Compare(a.BPOffset, b.BPOffset)
	})
	return vars
}

// InScope determines if a variable is in scope at a given offset
func (f *Function) InScope(v FunctionVar, off uint32) bool {
	if v.ScopeID == 0 {
		return inOffsetRange(off, f.Addr.Off, f.Len)
	}
	if v.ScopeID < 0 || v.ScopeID >= len(f.Scopes) {
		return false
	}
	scope := f.Scopes[v.ScopeID]
	return inOffsetRange(off, scope.Addr.Off, scope.Len)
}

func inOffsetRange(off, start uint32, length uint16) bool {
	return off >= start && off < start+uint32(length)
}

func (f *Function) ChildScopes(parentID int) []Scope {
	var scopes []Scope
	for _, s := range f.Scopes {
		if s.ParentID == parentID {
			scopes = append(scopes, s)
		}
	}
	slices.SortFunc(scopes, func(a, b Scope) int {
		return cmp.Or(
			cmp.Compare(a.Addr.Seg, b.Addr.Seg),
			cmp.Compare(a.Addr.Off, b.Addr.Off),
		)
	})
	return scopes
}

func (f *Function) BlockCount() int {
	if len(f.Scopes) == 0 {
		return 0
	}
	return len(f.Scopes) - 1
}

// ParamWords returns the number of words for the stack params
func (f *Function) ParamWords() int {
	words := 0
	for _, param := range f.Params {
		words += param.Words()
	}
	return words
}

// ReturnWords returns the number of words in the return signature of the function
func (f *Function) ReturnWords() int {
	return (f.Ret.Bytes() + 1) / 2
}
