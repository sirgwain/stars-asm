package resources

import (
	"fmt"
	"strings"
)

// styleFlag names a style bit (or multi-bit value such as WS_CAPTION) that
// is set when every bit of value is present.
type styleFlag struct {
	name  string
	value uint32
}

// styleType names one value of an enumerated style field such as a
// button's type; mask selects the field.
type styleType struct {
	name  string
	value uint32
}

// classStyles describes the control-specific low word of a window class's
// style: an optional enumerated type field and independent flags.
type classStyles struct {
	typeMask uint32
	types    []styleType
	flags    []styleFlag
}

// Window style bits shared by dialogs and controls, widest first so
// WS_CAPTION absorbs WS_BORDER|WS_DLGFRAME.
var windowStyles = []styleFlag{
	{"WS_POPUP", 0x80000000},
	{"WS_CHILD", 0x40000000},
	{"WS_MINIMIZE", 0x20000000},
	{"WS_VISIBLE", 0x10000000},
	{"WS_DISABLED", 0x08000000},
	{"WS_CLIPSIBLINGS", 0x04000000},
	{"WS_CLIPCHILDREN", 0x02000000},
	{"WS_MAXIMIZE", 0x01000000},
	{"WS_CAPTION", 0x00C00000},
	{"WS_BORDER", 0x00800000},
	{"WS_DLGFRAME", 0x00400000},
	{"WS_VSCROLL", 0x00200000},
	{"WS_HSCROLL", 0x00100000},
	{"WS_SYSMENU", 0x00080000},
	{"WS_THICKFRAME", 0x00040000},
}

// controlWindowStyles are the WS_GROUP and WS_TABSTOP meanings of the bits
// a top-level window uses for its minimize and maximize boxes.
var controlWindowStyles = []styleFlag{
	{"WS_GROUP", 0x00020000},
	{"WS_TABSTOP", 0x00010000},
}

// dialogWindowStyles are the minimize/maximize box meanings of those bits.
var dialogWindowStyles = []styleFlag{
	{"WS_MINIMIZEBOX", 0x00020000},
	{"WS_MAXIMIZEBOX", 0x00010000},
}

// dialogStyles are the DS_ bits of a dialog's own style.
var dialogStyles = classStyles{flags: []styleFlag{
	{"DS_ABSALIGN", 0x0001},
	{"DS_SYSMODAL", 0x0002},
	{"DS_LOCALEDIT", 0x0020},
	{"DS_SETFONT", 0x0040},
	{"DS_MODALFRAME", 0x0080},
	{"DS_NOIDLEMSG", 0x0100},
}}

// predefinedClassStyles holds the low-word styles of the predefined control
// classes, keyed by the lowercase class name.
var predefinedClassStyles = map[string]classStyles{
	"button": {
		typeMask: 0x000F,
		types: []styleType{
			{"BS_PUSHBUTTON", 0x0}, {"BS_DEFPUSHBUTTON", 0x1}, {"BS_CHECKBOX", 0x2}, {"BS_AUTOCHECKBOX", 0x3},
			{"BS_RADIOBUTTON", 0x4}, {"BS_3STATE", 0x5}, {"BS_AUTO3STATE", 0x6}, {"BS_GROUPBOX", 0x7},
			{"BS_USERBUTTON", 0x8}, {"BS_AUTORADIOBUTTON", 0x9}, {"BS_OWNERDRAW", 0xB},
		},
		flags: []styleFlag{{"BS_LEFTTEXT", 0x0020}},
	},
	"edit": {
		typeMask: 0x0003,
		types:    []styleType{{"ES_LEFT", 0x0}, {"ES_CENTER", 0x1}, {"ES_RIGHT", 0x2}},
		flags: []styleFlag{
			{"ES_MULTILINE", 0x0004}, {"ES_UPPERCASE", 0x0008}, {"ES_LOWERCASE", 0x0010}, {"ES_PASSWORD", 0x0020},
			{"ES_AUTOVSCROLL", 0x0040}, {"ES_AUTOHSCROLL", 0x0080}, {"ES_NOHIDESEL", 0x0100},
			{"ES_OEMCONVERT", 0x0400}, {"ES_READONLY", 0x0800}, {"ES_WANTRETURN", 0x1000},
		},
	},
	"static": {
		typeMask: 0x001F,
		types: []styleType{
			{"SS_LEFT", 0x0}, {"SS_CENTER", 0x1}, {"SS_RIGHT", 0x2}, {"SS_ICON", 0x3},
			{"SS_BLACKRECT", 0x4}, {"SS_GRAYRECT", 0x5}, {"SS_WHITERECT", 0x6}, {"SS_BLACKFRAME", 0x7},
			{"SS_GRAYFRAME", 0x8}, {"SS_WHITEFRAME", 0x9}, {"SS_SIMPLE", 0xB}, {"SS_LEFTNOWORDWRAP", 0xC},
		},
		flags: []styleFlag{{"SS_NOPREFIX", 0x0080}},
	},
	"listbox": {flags: []styleFlag{
		{"LBS_NOTIFY", 0x0001}, {"LBS_SORT", 0x0002}, {"LBS_NOREDRAW", 0x0004}, {"LBS_MULTIPLESEL", 0x0008},
		{"LBS_OWNERDRAWFIXED", 0x0010}, {"LBS_OWNERDRAWVARIABLE", 0x0020}, {"LBS_HASSTRINGS", 0x0040},
		{"LBS_USETABSTOPS", 0x0080}, {"LBS_NOINTEGRALHEIGHT", 0x0100}, {"LBS_MULTICOLUMN", 0x0200},
		{"LBS_WANTKEYBOARDINPUT", 0x0400}, {"LBS_EXTENDEDSEL", 0x0800}, {"LBS_DISABLENOSCROLL", 0x1000},
	}},
	"combobox": {
		typeMask: 0x0003,
		types:    []styleType{{"CBS_SIMPLE", 0x1}, {"CBS_DROPDOWN", 0x2}, {"CBS_DROPDOWNLIST", 0x3}},
		flags: []styleFlag{
			{"CBS_OWNERDRAWFIXED", 0x0010}, {"CBS_OWNERDRAWVARIABLE", 0x0020}, {"CBS_AUTOHSCROLL", 0x0040},
			{"CBS_OEMCONVERT", 0x0080}, {"CBS_SORT", 0x0100}, {"CBS_HASSTRINGS", 0x0200},
			{"CBS_NOINTEGRALHEIGHT", 0x0400}, {"CBS_DISABLENOSCROLL", 0x0800},
		},
	},
	"scrollbar": {flags: []styleFlag{{"SBS_VERT", 0x0001}, {"SBS_SIZEBOX", 0x0008}}},
}

// rcClassNames spells the predefined control classes as resource scripts
// do; windres stores these names as the classes' ordinals.
var rcClassNames = map[string]string{
	"button":    "Button",
	"edit":      "Edit",
	"static":    "Static",
	"listbox":   "ListBox",
	"scrollbar": "ScrollBar",
	"combobox":  "ComboBox",
}

// controlStyleNames renders a control's style as symbolic names, with any
// bits no name covers as a trailing hex constant.
func controlStyleNames(class string, style uint32) string {
	high := append(append([]styleFlag{}, windowStyles...), controlWindowStyles...)
	return renderStyle(style, high, predefinedClassStyles[class])
}

// dialogStyleNames renders a dialog's own style as symbolic names.
func dialogStyleNames(style uint32) string {
	high := append(append([]styleFlag{}, windowStyles...), dialogWindowStyles...)
	return renderStyle(style, high, dialogStyles)
}

// renderStyle names the window style bits in the high word and the class
// styles in the low word.
func renderStyle(style uint32, high []styleFlag, low classStyles) string {
	var names []string
	remaining := style
	for _, f := range high {
		if remaining&f.value == f.value {
			names = append(names, f.name)
			remaining &^= f.value
		}
	}
	if low.typeMask != 0 {
		value := remaining & low.typeMask
		for _, t := range low.types {
			if t.value == value {
				names = append(names, t.name)
				remaining &^= low.typeMask
				break
			}
		}
	}
	for _, f := range low.flags {
		if remaining&f.value == f.value {
			names = append(names, f.name)
			remaining &^= f.value
		}
	}
	if remaining != 0 || len(names) == 0 {
		names = append(names, fmt.Sprintf("0x%X", remaining))
	}
	return strings.Join(names, " | ")
}
