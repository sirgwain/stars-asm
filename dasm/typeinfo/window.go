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
