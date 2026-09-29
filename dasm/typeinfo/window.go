package typeinfo

import (
	"fmt"
	"strings"
)

// DialogEnumName is the enum naming dialog template resource ids.
const DialogEnumName = "DialogId"

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
