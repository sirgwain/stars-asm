package typeinfo

import (
	"fmt"
	"strings"
)

// DialogEnumName is the enum naming dialog template resource ids.
const DialogEnumName = "DialogId"

// ControlEnumName is the enum naming WM_COMMAND ids shared by dialogs and
// menus; a dialog's own controls are named by its dialog_controls enum.
const ControlEnumName = "ControlId"

// WindowClass is a window class whose control messages in the WM_USER range
// are named by their own enum. Listbox, combobox and edit messages reuse the
// same numbers, so a message value is only meaningful with its target's
// class.
type WindowClass struct {
	Name     string
	Messages *Enum
}

// WindowRule describes what an HWND parameter, local or global refers to:
// a dialog created from a template, or a control of a window class.
type WindowRule struct {
	Func   string
	Param  string
	Local  string
	Global string

	// Dialog is the dialog template resource id when HasDialog is set.
	Dialog    int
	HasDialog bool

	Class *WindowClass
}

// SentMessage is a control message a function sends to a window whose class
// the function does not show, such as the item window of a DRAWITEMSTRUCT
// that may be a listbox or a combo box. It names the message by its class's
// enum where the number alone is ambiguous.
type SentMessage struct {
	Func  string
	Enum  *Enum
	Value int
}

type sentMessagesJSON struct {
	Func     string   `json:"func"`
	Messages []string `json:"messages"`
}

type windowClassJSON struct {
	Class    string `json:"class"`
	Messages string `json:"messages"`
}

// BufferView is a byte buffer a function reads and writes as one record
// struct, as the original source did with casts like ((RTPLANET *)rgbCur):
// a global or local array, or a param or local pointing at one.
//
// A view either always reads Struct, or reads the struct Views gives the
// value of Discriminator, a path rooted at one of the function's params or
// locals or at a global, such as the record type rt of the record the buffer
// holds.
type BufferView struct {
	Func   string
	Param  string
	Local  string
	Global string
	Struct *Struct

	Discriminator     []string
	DiscriminatorEnum *Enum
	Views             map[int]*Struct
}

// Var reports whether v is the buffer the view covers.
func (bv *BufferView) Var(v Var) bool {
	switch v := v.(type) {
	case *GlobalVar:
		return bv.Global != "" && bv.Global == v.Name
	case *FunctionVar:
		return bv.Param == v.Name || bv.Local == v.Name
	}
	return false
}

// bufferViewJSON is one buffer_views record.
type bufferViewJSON struct {
	Func              string            `json:"func"`
	Param             string            `json:"param"`
	Local             string            `json:"local"`
	Global            string            `json:"global"`
	Struct            string            `json:"struct"`
	Discriminator     []string          `json:"discriminator"`
	DiscriminatorEnum string            `json:"discriminator_enum"`
	Views             map[string]string `json:"views"`
}

// dialogControlsJSON names the enum of one dialog template's own controls.
type dialogControlsJSON struct {
	Dialog string `json:"dialog"`
	Enum   string `json:"enum"`
}

type windowRuleJSON struct {
	Func   string `json:"func"`
	Param  string `json:"param"`
	Local  string `json:"local"`
	Global string `json:"global"`
	Dialog string `json:"dialog"`
	Class  string `json:"class"`
}

// loadWindows loads window classes and HWND rules from enums.json.
func (l *enumLoader) loadWindows(path string, sdb *SymbolDB) error {
	cfg, err := l.loadEnumConfig(path)
	if err != nil {
		return err
	}
	for _, c := range cfg.WindowClasses {
		messages := sdb.GetEnum(c.Messages)
		if messages == nil {
			return fmt.Errorf("window class %s: message enum %s not found", c.Class, c.Messages)
		}
		sdb.WindowClasses = append(sdb.WindowClasses, &WindowClass{Name: strings.ToLower(c.Class), Messages: messages})
	}
	dialogs := sdb.GetEnum(DialogEnumName)
	for _, w := range cfg.Windows {
		rule := &WindowRule{Func: w.Func, Param: w.Param, Local: w.Local, Global: w.Global}
		if (w.Global == "") == (w.Func == "") || w.Func != "" && (w.Param == "") == (w.Local == "") {
			return fmt.Errorf("window rule %+v must name one global, or one param or local of a func", w)
		}
		if (w.Dialog == "") == (w.Class == "") {
			return fmt.Errorf("window rule %+v must specify exactly one of dialog or class", w)
		}
		if w.Dialog != "" {
			if dialogs == nil {
				return fmt.Errorf("window rule %+v: enum %s not found", w, DialogEnumName)
			}
			value, ok := enumValueByName(dialogs, w.Dialog)
			if !ok {
				return fmt.Errorf("window rule %+v: dialog %s not found in %s", w, w.Dialog, DialogEnumName)
			}
			rule.Dialog, rule.HasDialog = value.Value, true
		} else if rule.Class = sdb.GetWindowClass(w.Class); rule.Class == nil {
			return fmt.Errorf("window rule %+v: window class %s not configured", w, w.Class)
		}
		sdb.WindowRules = append(sdb.WindowRules, rule)
	}
	sdb.DialogControls = make(map[int]*Enum, len(cfg.DialogControls))
	for _, dc := range cfg.DialogControls {
		if dialogs == nil {
			return fmt.Errorf("dialog controls %+v: enum %s not found", dc, DialogEnumName)
		}
		dialog, ok := enumValueByName(dialogs, dc.Dialog)
		if !ok {
			return fmt.Errorf("dialog controls %+v: dialog %s not found in %s", dc, dc.Dialog, DialogEnumName)
		}
		controls := sdb.GetEnum(dc.Enum)
		if controls == nil {
			return fmt.Errorf("dialog controls %+v: enum %s not found", dc, dc.Enum)
		}
		if _, dup := sdb.DialogControls[dialog.Value]; dup {
			return fmt.Errorf("dialog controls %+v: dialog %s already has a control enum", dc, dc.Dialog)
		}
		sdb.DialogControls[dialog.Value] = controls
	}
	for _, bv := range cfg.BufferViews {
		view, err := parseBufferView(bv, sdb)
		if err != nil {
			return err
		}
		sdb.BufferViews = append(sdb.BufferViews, view)
	}
	for _, sent := range cfg.SentMessages {
		if sent.Func == "" || len(sent.Messages) == 0 {
			return fmt.Errorf("sent messages rule %+v must name a func and its messages", sent)
		}
		for _, name := range sent.Messages {
			message, ok := sdb.controlMessageByName(name)
			if !ok {
				return fmt.Errorf("sent messages rule for %s: %s is not a window class message", sent.Func, name)
			}
			message.Func = sent.Func
			sdb.SentMessages = append(sdb.SentMessages, message)
		}
	}
	return nil
}

// controlMessageByName returns the window class message with the name.
func (sdb *SymbolDB) controlMessageByName(name string) (*SentMessage, bool) {
	for _, c := range sdb.WindowClasses {
		if value, ok := enumValueByName(c.Messages, name); ok {
			return &SentMessage{Enum: c.Messages, Value: value.Value}, true
		}
	}
	return nil, false
}

// SentMessageEnum returns the enum naming a control message value a function
// sends to a window of unknown class, or nil if no rule names it.
func (sdb *SymbolDB) SentMessageEnum(funcName string, value int) *Enum {
	for _, m := range sdb.SentMessages {
		if m.Func == funcName && m.Value == value {
			return m.Enum
		}
	}
	return nil
}

// GetWindowClass returns the configured window class with the name, ignoring
// case as Windows does.
func (sdb *SymbolDB) GetWindowClass(name string) *WindowClass {
	name = strings.ToLower(name)
	for _, c := range sdb.WindowClasses {
		if c.Name == name {
			return c
		}
	}
	return nil
}

// GlobalWindow returns the rule for an HWND global.
func (sdb *SymbolDB) GlobalWindow(name string) *WindowRule {
	for _, r := range sdb.WindowRules {
		if r.Global == name {
			return r
		}
	}
	return nil
}

// FunctionVarWindow returns the rule for an HWND parameter (param set) or
// local of a function.
func (sdb *SymbolDB) FunctionVarWindow(funcName, name string, param bool) *WindowRule {
	for _, r := range sdb.WindowRules {
		if r.Func != funcName {
			continue
		}
		if param && r.Param == name || !param && r.Local == name {
			return r
		}
	}
	return nil
}

// parseBufferView resolves a buffer_views record, checking that its function,
// buffer variable and struct exist.
func parseBufferView(bv bufferViewJSON, sdb *SymbolDB) (*BufferView, error) {
	names := 0
	for _, name := range []string{bv.Param, bv.Local, bv.Global} {
		if name != "" {
			names++
		}
	}
	if bv.Func == "" || names != 1 {
		return nil, fmt.Errorf("buffer view %+v must name a func and one param, local or global", bv)
	}
	fn := sdb.GetFunction(bv.Func)
	if fn == nil {
		return nil, fmt.Errorf("buffer view %+v: func %s not found", bv, bv.Func)
	}
	switch {
	case bv.Param != "" && !hasFunctionVar(fn.Params, bv.Param):
		return nil, fmt.Errorf("buffer view %+v: %s has no param %s", bv, bv.Func, bv.Param)
	case bv.Local != "" && !hasFunctionVar(fn.Vars, bv.Local):
		return nil, fmt.Errorf("buffer view %+v: %s has no local %s", bv, bv.Func, bv.Local)
	case bv.Global != "" && sdb.GetGlobal(bv.Global) == nil:
		return nil, fmt.Errorf("buffer view %+v: global %s not found", bv, bv.Global)
	}
	view := &BufferView{Func: bv.Func, Param: bv.Param, Local: bv.Local, Global: bv.Global}
	if (bv.Struct == "") == (len(bv.Views) == 0) {
		return nil, fmt.Errorf("buffer view %+v must name one struct or discriminated views", bv)
	}
	if bv.Struct != "" {
		if view.Struct = sdb.GetStruct(bv.Struct); view.Struct == nil {
			return nil, fmt.Errorf("buffer view %+v: struct %s not found", bv, bv.Struct)
		}
		return view, nil
	}
	if len(bv.Discriminator) == 0 {
		return nil, fmt.Errorf("buffer view %+v: views need a discriminator", bv)
	}
	if !hasFunctionVar(fn.Params, bv.Discriminator[0]) && !hasFunctionVar(fn.Vars, bv.Discriminator[0]) && sdb.GetGlobal(bv.Discriminator[0]) == nil {
		return nil, fmt.Errorf("buffer view %+v: discriminator %s is not a param, local or global", bv, bv.Discriminator[0])
	}
	if view.DiscriminatorEnum = sdb.GetEnum(bv.DiscriminatorEnum); view.DiscriminatorEnum == nil {
		return nil, fmt.Errorf("buffer view %+v: discriminator enum %s not found", bv, bv.DiscriminatorEnum)
	}
	view.Discriminator = append([]string(nil), bv.Discriminator...)
	view.Views = make(map[int]*Struct, len(bv.Views))
	for valueName, structName := range bv.Views {
		value, ok := enumValueByName(view.DiscriminatorEnum, valueName)
		if !ok {
			return nil, fmt.Errorf("buffer view %+v: %s is not a %s value", bv, valueName, bv.DiscriminatorEnum)
		}
		strct := sdb.GetStruct(structName)
		if strct == nil {
			return nil, fmt.Errorf("buffer view %+v: struct %s not found", bv, structName)
		}
		view.Views[value.Value] = strct
	}
	return view, nil
}

// hasFunctionVar reports whether vars has one named name.
func hasFunctionVar(vars []FunctionVar, name string) bool {
	for _, v := range vars {
		if v.Name == name {
			return true
		}
	}
	return false
}

// FunctionBufferView returns the struct funcName always reads the variable
// as, a global or one of its params or locals, or nil when no view covers it
// or the view depends on a discriminator.
func (sdb *SymbolDB) FunctionBufferView(funcName string, v Var) *Struct {
	for _, bv := range sdb.BufferViews {
		if bv.Func == funcName && bv.Struct != nil && bv.Var(v) {
			return bv.Struct
		}
	}
	return nil
}

// FunctionDiscriminatedBufferViews returns funcName's buffer views that
// depend on a discriminator.
func (sdb *SymbolDB) FunctionDiscriminatedBufferViews(funcName string) []*BufferView {
	var views []*BufferView
	for _, bv := range sdb.BufferViews {
		if bv.Func == funcName && bv.Views != nil {
			views = append(views, bv)
		}
	}
	return views
}

// FunctionDialog returns the dialog template of the dialog a function's HWND
// parameter is configured to hold, such as a dialog procedure's own window.
func (sdb *SymbolDB) FunctionDialog(funcName string) (int, bool) {
	for _, r := range sdb.WindowRules {
		if r.Func == funcName && r.Param != "" && r.HasDialog {
			return r.Dialog, true
		}
	}
	return 0, false
}

// messageEnums returns the enums that name messages: window messages first,
// then each window class's control messages.
func (sdb *SymbolDB) messageEnums() []*Enum {
	enums := []*Enum{}
	if wm := sdb.GetEnum(MessageEnumName); wm != nil {
		enums = append(enums, wm)
	}
	for _, c := range sdb.WindowClasses {
		enums = append(enums, c.Messages)
	}
	return enums
}

// MessageHandler is a helper a window procedure calls with the parameters of
// one message, such as CommandHandler for WM_COMMAND. Its reads of the
// message's parameters use the message's crackers like the window
// procedure's own.
type MessageHandler struct {
	Func    string
	Message *MessageRule
	WParam  string
	LParam  string
}

type messageHandlerJSON struct {
	Func    string `json:"func"`
	Message string `json:"message"`
	WParam  string `json:"wparam"`
	LParam  string `json:"lparam"`
}

// loadMessageHandlers resolves message handler records against the loaded
// message rules, and types the handlers' message parameters WPARAM and
// LPARAM so callers pass the window procedure's values through unchanged.
func (l *enumLoader) loadMessageHandlers(path string, sdb *SymbolDB, resolver *typeResolver) error {
	cfg, err := l.loadEnumConfig(path)
	if err != nil {
		return err
	}
	wm := sdb.GetEnum(MessageEnumName)
	for _, h := range cfg.MessageHandlers {
		value, ok := enumValueByName(wm, h.Message)
		if !ok {
			return fmt.Errorf("message handler %s: message %s not found", h.Func, h.Message)
		}
		message := sdb.GetMessage(wm, value.Value)
		if message == nil {
			return fmt.Errorf("message handler %s: message %s has no payload rule", h.Func, h.Message)
		}
		fn := sdb.GetFunction(h.Func)
		if fn == nil {
			return fmt.Errorf("message handler %s: function not found", h.Func)
		}
		for _, p := range []struct{ name, typ string }{{h.WParam, "WPARAM"}, {h.LParam, "LPARAM"}} {
			if p.name == "" {
				continue
			}
			i := functionParamIndexByName(fn, p.name)
			if i < 0 {
				return fmt.Errorf("message handler %s: parameter %s not found", h.Func, p.name)
			}
			fn.Params[i].Type = resolver.getNamedType(p.typ)
		}
		sdb.MessageHandlers = append(sdb.MessageHandlers, &MessageHandler{Func: h.Func, Message: message, WParam: h.WParam, LParam: h.LParam})
	}
	return nil
}

// GetMessageHandler returns the message handler declaration for a function.
func (sdb *SymbolDB) GetMessageHandler(funcName string) *MessageHandler {
	for _, h := range sdb.MessageHandlers {
		if h.Func == funcName {
			return h
		}
	}
	return nil
}
