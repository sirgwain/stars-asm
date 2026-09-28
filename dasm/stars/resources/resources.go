// Package resources regenerates the Win16 resources of stars.exe as a
// resource script: dialog, menu and accelerator templates written as
// resource statements, and icons, cursors, bitmaps and data written as the
// files the script includes. IDs are named through the resource ID enums,
// which are also written as resource.h for the resource compiler.
package resources

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// Enums naming the IDs of each kind of resource.
const (
	dialogEnum      = "DialogId"
	controlEnum     = "ControlId"
	commandEnum     = "WParamMessageId"
	cursorEnum      = "CursorId"
	bitmapEnum      = "BitmapId"
	acceleratorEnum = "AcceleratorId"
	dataEnum        = "TutorialResourceId"
	virtualKeyEnum  = "VirtualKey"
)

// resourceFileDirs are the subdirectories holding the files the script
// includes.
var resourceFileDirs = []string{"icons", "cursors", "bitmaps", "data"}

// resourceHeaderEnums are the enums written to resource.h.
var resourceHeaderEnums = []string{dialogEnum, controlEnum, commandEnum, cursorEnum, bitmapEnum, acceleratorEnum, dataEnum}

// Menu item options and their resource script keywords.
var menuOptions = []styleFlag{
	{"GRAYED", 0x0001},
	{"INACTIVE", 0x0002},
	{"CHECKED", 0x0008},
	{"MENUBARBREAK", 0x0020},
	{"MENUBREAK", 0x0040},
	{"HELP", 0x4000},
}

// Accelerator flags and their resource script keywords.
const accelVirtKey = 0x01

var acceleratorOptions = []styleFlag{
	{"NOINVERT", 0x02},
	{"SHIFT", 0x04},
	{"CONTROL", 0x08},
	{"ALT", 0x10},
}

// writer renders the resources of one image.
type writer struct {
	img *asm.ImageNE
	sdb *typeinfo.SymbolDB
	dir string
}

// Write generates dir/stars.rc and dir/resource.h for the resources in img,
// with the icon, cursor, bitmap and data files the script includes in
// subdirectories of dir.
func Write(dir string, img *asm.ImageNE, sdb *typeinfo.SymbolDB) error {
	for _, name := range append(resourceHeaderEnums, virtualKeyEnum) {
		if sdb.GetEnum(name) == nil {
			return fmt.Errorf("resource enum %s not found", name)
		}
	}
	w := &writer{img: img, sdb: sdb, dir: dir}
	// The file directories hold only generated files; clear them so a
	// renamed resource leaves no stale file behind.
	for _, subdir := range resourceFileDirs {
		if err := os.RemoveAll(filepath.Join(dir, subdir)); err != nil {
			return err
		}
	}
	if err := os.MkdirAll(dir, 0o755); err != nil {
		return err
	}
	if err := os.WriteFile(filepath.Join(dir, "resource.h"), []byte(w.resourceHeader()), 0o644); err != nil {
		return err
	}
	script, err := w.script()
	if err != nil {
		return err
	}
	return os.WriteFile(filepath.Join(dir, "stars.rc"), []byte(script), 0o644)
}

// resourceHeader renders the resource ID enums as the #defines the resource
// compiler reads; C code uses the typed enums in enums.h instead. Each name
// is redefined so a windows.h macro of the same name, such as IDOK, takes
// the enum's value.
func (w *writer) resourceHeader() string {
	var b strings.Builder
	b.WriteString("/* Resource IDs for the resource compiler, generated from enums.h. */\n")
	b.WriteString("#ifndef STARS_RESOURCE_H\n#define STARS_RESOURCE_H\n")
	for _, name := range resourceHeaderEnums {
		fmt.Fprintf(&b, "\n/* %s */\n", name)
		for _, v := range w.sdb.GetEnum(name).Values {
			fmt.Fprintf(&b, "#undef %s\n#define %s %d\n", v.Name, v.Name, v.Value)
		}
	}
	b.WriteString("\n#endif\n")
	return b.String()
}

// script renders stars.rc, writing the files it includes as it goes.
func (w *writer) script() (string, error) {
	var b strings.Builder
	b.WriteString("/* Resources of stars.exe, generated from its NE resource table. */\n")
	b.WriteString("#pragma code_page(1252)\n\n#include <windows.h>\n#include \"resource.h\"\n")

	sections := []struct {
		title string
		typ   uint16
		emit  func(*strings.Builder, *asm.Resource) error
	}{
		{"Icons", asm.ResourceGroupIcon, w.icon},
		{"Cursors", asm.ResourceGroupCursor, w.cursor},
		{"Bitmaps", asm.ResourceBitmap, w.bitmap},
	}
	for _, s := range sections {
		fmt.Fprintf(&b, "\n/* %s */\n", s.title)
		for i := range w.img.Resources() {
			res := &w.img.Resources()[i]
			if res.Type.Ordinal && res.Type.ID == s.typ {
				if err := s.emit(&b, res); err != nil {
					return "", err
				}
			}
		}
	}
	b.WriteString("\n/* Data */\n")
	for i := range w.img.Resources() {
		res := &w.img.Resources()[i]
		if !res.Type.Ordinal || !knownResourceType(res.Type.ID) {
			if err := w.data(&b, res); err != nil {
				return "", err
			}
		}
	}
	for _, m := range w.img.Menus() {
		if err := w.menu(&b, m); err != nil {
			return "", err
		}
	}
	for _, t := range w.img.AcceleratorTables() {
		if err := w.accelerators(&b, t); err != nil {
			return "", err
		}
	}
	for _, d := range w.img.Dialogs() {
		w.dialog(&b, d)
	}
	return b.String(), nil
}

// knownResourceType reports whether a resource type ordinal is one the
// script writes in its own section, or as part of an icon or cursor group.
func knownResourceType(id uint16) bool {
	switch id {
	case asm.ResourceCursor, asm.ResourceIcon, asm.ResourceGroupCursor, asm.ResourceGroupIcon,
		asm.ResourceBitmap, asm.ResourceMenu, asm.ResourceDialog, asm.ResourceAccelerator:
		return true
	}
	return false
}

// icon writes a GROUP_ICON resource as an .ico file and its ICON statement.
func (w *writer) icon(b *strings.Builder, res *asm.Resource) error {
	data, err := iconFile(w.img, res)
	if err != nil {
		return err
	}
	return w.fileStatement(b, w.name("", res.Name), "ICON", "icons", ".ico", data)
}

// cursor writes a GROUP_CURSOR resource as a .cur file and its CURSOR
// statement.
func (w *writer) cursor(b *strings.Builder, res *asm.Resource) error {
	data, err := cursorFile(w.img, res)
	if err != nil {
		return err
	}
	return w.fileStatement(b, w.name(cursorEnum, res.Name), "CURSOR", "cursors", ".cur", data)
}

// bitmap writes a BITMAP resource as a .bmp file and its BITMAP statement.
func (w *writer) bitmap(b *strings.Builder, res *asm.Resource) error {
	data, err := bitmapFile(res)
	if err != nil {
		return err
	}
	return w.fileStatement(b, w.name(bitmapEnum, res.Name), "BITMAP", "bitmaps", ".bmp", data)
}

// data writes a resource of an application-defined type as a raw file and
// its user-defined resource statement.
func (w *writer) data(b *strings.Builder, res *asm.Resource) error {
	return w.fileStatement(b, w.name(dataEnum, res.Name), w.name(dataEnum, res.Type), "data", ".bin", res.Data)
}

// fileStatement writes data to subdir/name+ext and the statement including
// it as a resource of kind.
func (w *writer) fileStatement(b *strings.Builder, name, kind, subdir, ext string, data []byte) error {
	rel := subdir + "/" + strings.ToLower(name) + ext
	if err := os.MkdirAll(filepath.Join(w.dir, subdir), 0o755); err != nil {
		return err
	}
	if err := os.WriteFile(filepath.Join(w.dir, filepath.FromSlash(rel)), data, 0o644); err != nil {
		return err
	}
	fmt.Fprintf(b, "%s %s \"%s\"\n", name, kind, rel)
	return nil
}

// menu writes a MENU statement.
func (w *writer) menu(b *strings.Builder, m *asm.Menu) error {
	fmt.Fprintf(b, "\n%s MENU\n", w.name("", m.Name))
	return w.menuItems(b, m.Items, 0)
}

// menuItems writes one menu level between BEGIN and END.
func (w *writer) menuItems(b *strings.Builder, items []asm.MenuItem, depth int) error {
	indent := strings.Repeat("    ", depth)
	fmt.Fprintf(b, "%sBEGIN\n", indent)
	for _, item := range items {
		if item.Flags&asm.MenuPopup == 0 && item.Flags == 0 && item.ID == 0 && item.Text == "" {
			fmt.Fprintf(b, "%s    MENUITEM SEPARATOR\n", indent)
			continue
		}
		options, err := flagNames(uint32(item.Flags&^asm.MenuPopup), menuOptions)
		if err != nil {
			return fmt.Errorf("menu item %q: %w", item.Text, err)
		}
		if item.Flags&asm.MenuPopup != 0 {
			fmt.Fprintf(b, "%s    POPUP %s%s\n", indent, rcString(item.Text), options)
			if err := w.menuItems(b, item.Items, depth+1); err != nil {
				return err
			}
			continue
		}
		fmt.Fprintf(b, "%s    MENUITEM %s, %s%s\n", indent, rcString(item.Text), w.enumName(commandEnum, int(item.ID)), options)
	}
	fmt.Fprintf(b, "%sEND\n", indent)
	return nil
}

// accelerators writes an ACCELERATORS statement.
func (w *writer) accelerators(b *strings.Builder, t *asm.AcceleratorTable) error {
	fmt.Fprintf(b, "\n%s ACCELERATORS\nBEGIN\n", w.name(acceleratorEnum, t.Name))
	for _, e := range t.Entries {
		key, kind := w.acceleratorKey(e)
		options, err := flagNames(uint32(e.Flags&^accelVirtKey), acceleratorOptions)
		if err != nil {
			return fmt.Errorf("accelerators %s: %w", t.Name, err)
		}
		fmt.Fprintf(b, "    %s, %s, %s%s\n", key, w.enumName(commandEnum, int(e.ID)), kind, options)
	}
	b.WriteString("END\n")
	return nil
}

// acceleratorKey renders an accelerator's key and its VIRTKEY or ASCII
// keyword: letters and digits as quoted characters, other virtual keys by
// their VK_ name.
func (w *writer) acceleratorKey(e asm.Accelerator) (string, string) {
	printable := e.Key >= 0x20 && e.Key < 0x7f && e.Key != '"'
	if e.Flags&accelVirtKey == 0 {
		if printable {
			return fmt.Sprintf("%q", string(rune(e.Key))), "ASCII"
		}
		return fmt.Sprintf("%d", e.Key), "ASCII"
	}
	if e.Key >= 'A' && e.Key <= 'Z' || e.Key >= '0' && e.Key <= '9' {
		return fmt.Sprintf("\"%c\"", rune(e.Key)), "VIRTKEY"
	}
	return w.enumName(virtualKeyEnum, int(e.Key)), "VIRTKEY"
}

// dialog writes a DIALOG statement. Controls are written as generic
// CONTROL statements carrying their exact class and style; resource
// compilers add WS_CHILD | WS_VISIBLE to those, so a hidden control removes
// WS_VISIBLE again.
func (w *writer) dialog(b *strings.Builder, d *asm.Dialog) {
	fmt.Fprintf(b, "\n%s DIALOG %d, %d, %d, %d\n", w.name(dialogEnum, d.Name), d.X, d.Y, d.CX, d.CY)
	fmt.Fprintf(b, "STYLE %s\n", dialogStyleNames(d.Style))
	if d.Caption != "" {
		fmt.Fprintf(b, "CAPTION %s\n", rcString(d.Caption))
	}
	if d.Menu.Ordinal || d.Menu.Name != "" {
		fmt.Fprintf(b, "MENU %s\n", w.name("", d.Menu))
	}
	if d.Class != "" {
		fmt.Fprintf(b, "CLASS %s\n", rcString(d.Class))
	}
	if d.FontName != "" {
		fmt.Fprintf(b, "FONT %d, %s\n", d.FontSize, rcString(d.FontName))
	}
	b.WriteString("BEGIN\n")
	for _, c := range d.Controls {
		class, ok := rcClassNames[c.Class]
		if !ok {
			class = c.Class
		}
		style := controlStyleNames(c.Class, c.Style)
		if c.Style&0x10000000 == 0 {
			style += " | NOT WS_VISIBLE"
		}
		if c.Style&0x40000000 == 0 {
			style += " | NOT WS_CHILD"
		}
		fmt.Fprintf(b, "    CONTROL %s, %s, %s, %s, %d, %d, %d, %d\n",
			w.controlText(c.Text), w.controlID(c.ID), rcString(class), style, c.X, c.Y, c.CX, c.CY)
	}
	b.WriteString("END\n")
}

// controlText renders a control's text, or the ordinal of the resource it
// shows.
func (w *writer) controlText(text asm.ResourceKey) string {
	if text.Ordinal {
		return fmt.Sprintf("%d", text.ID)
	}
	return rcString(text.Name)
}

// controlID renders a control id by its ControlId name, with -1 for the
// 0xFFFF id of controls the dialog never addresses.
func (w *writer) controlID(id uint16) string {
	if id == 0xffff {
		return "-1"
	}
	return w.enumName(controlEnum, int(id))
}

// name renders a resource name: its string, or an ordinal by its name in
// enum when it has one.
func (w *writer) name(enum string, key asm.ResourceKey) string {
	if !key.Ordinal {
		return key.Name
	}
	if enum == "" {
		return fmt.Sprintf("%d", key.ID)
	}
	return w.enumName(enum, int(key.ID))
}

// enumName returns the first member of enum with value, or the decimal
// value when no member has it.
func (w *writer) enumName(enum string, value int) string {
	for _, v := range w.sdb.GetEnum(enum).Values {
		if v.Value == value {
			return v.Name
		}
	}
	return fmt.Sprintf("%d", value)
}

// flagNames renders set flags as ", NAME" suffixes, failing on bits no
// name covers.
func flagNames(value uint32, flags []styleFlag) (string, error) {
	var b strings.Builder
	for _, f := range flags {
		if value&f.value == f.value {
			fmt.Fprintf(&b, ", %s", f.name)
			value &^= f.value
		}
	}
	if value != 0 {
		return "", fmt.Errorf("unknown flags 0x%X", value)
	}
	return b.String(), nil
}

// rcString quotes s for a resource script: doubled quotes, escaped
// backslashes and tabs, and octal escapes for other control and non-ASCII
// bytes, which the script's code page maps to characters.
func rcString(s string) string {
	var b strings.Builder
	b.WriteByte('"')
	for i := 0; i < len(s); i++ {
		switch c := s[i]; {
		case c == '"':
			b.WriteString(`""`)
		case c == '\\':
			b.WriteString(`\\`)
		case c == '\t':
			b.WriteString(`\t`)
		case c < 0x20 || c >= 0x7f:
			fmt.Fprintf(&b, `\%03o`, c)
		default:
			b.WriteByte(c)
		}
	}
	b.WriteByte('"')
	return b.String()
}
