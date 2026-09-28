package asm

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"os"
	"strings"
)

// NE resource type ordinals used by Stars!.
const (
	ResourceCursor       uint16 = 1
	ResourceBitmap       uint16 = 2
	ResourceIcon         uint16 = 3
	ResourceMenu         uint16 = 4
	ResourceDialog       uint16 = 5
	ResourceString       uint16 = 6
	ResourceAccelerator  uint16 = 9
	ResourceRCData       uint16 = 10
	ResourceGroupCursor  uint16 = 12
	ResourceGroupIcon    uint16 = 14
	resourceOrdinalFlag  uint16 = 0x8000
	dialogStyleSetFont   uint32 = 0x40
	dialogItemClassFlag  byte   = 0x80
	dialogItemOrdinalTag byte   = 0xff

	// MenuPopup marks a menu item that opens a submenu; menuEnd marks the
	// last item at a menu level.
	MenuPopup uint16 = 0x0010
	menuEnd   uint16 = 0x0080

	// accelEnd marks the last entry of a Win16 accelerator table.
	accelEnd byte = 0x80
)

// ResourceKey identifies an NE resource type or resource, or names a dialog
// menu or control text, by ordinal or by string.
type ResourceKey struct {
	Ordinal bool
	ID      uint16
	Name    string
}

// String returns the decimal ordinal or the name.
func (k ResourceKey) String() string {
	if k.Ordinal {
		return fmt.Sprintf("%d", k.ID)
	}
	return k.Name
}

// Resource is one resource from the NE resource table with its raw data.
type Resource struct {
	Type ResourceKey
	Name ResourceKey
	Data []byte
}

// Dialog is a parsed Win16 dialog template.
type Dialog struct {
	Name     ResourceKey
	Style    uint32
	X, Y     int16
	CX, CY   int16
	Menu     ResourceKey
	Class    string
	Caption  string
	FontSize uint16
	FontName string
	Controls []DialogControl
}

// DialogControl is one item of a Win16 dialog template. Class is the
// lowercase predefined class name (button, edit, static, listbox, scrollbar,
// combobox) or the registered class name of a custom control.
type DialogControl struct {
	X, Y   int16
	CX, CY int16
	ID     uint16
	Style  uint32
	Class  string
	Text   ResourceKey
	Extra  []byte
}

// Menu is a parsed Win16 menu template.
type Menu struct {
	Name  ResourceKey
	Items []MenuItem
}

// MenuItem is one item of a menu template. A popup item (Flags has
// MenuPopup) carries its submenu in Items instead of a command ID.
type MenuItem struct {
	Flags uint16
	ID    uint16
	Text  string
	Items []MenuItem
}

// AcceleratorTable is a parsed Win16 accelerator table.
type AcceleratorTable struct {
	Name    ResourceKey
	Entries []Accelerator
}

// Accelerator is one accelerator table entry: FVIRTKEY/FNOINVERT/FSHIFT/
// FCONTROL/FALT flags, the key, and the command ID it sends.
type Accelerator struct {
	Flags byte
	Key   uint16
	ID    uint16
}

// predefinedDialogClasses maps Win16 dialog item class ordinals to names.
var predefinedDialogClasses = map[byte]string{
	0x80: "button",
	0x81: "edit",
	0x82: "static",
	0x83: "listbox",
	0x84: "scrollbar",
	0x85: "combobox",
}

// Resources returns every resource in the NE resource table, in table order.
func (img *ImageNE) Resources() []Resource {
	return img.resources
}

// Resource returns the resource with the ordinal type and the name.
func (img *ImageNE) Resource(typ uint16, name ResourceKey) (*Resource, bool) {
	for i := range img.resources {
		r := &img.resources[i]
		if r.Type.Ordinal && r.Type.ID == typ && r.Name == name {
			return r, true
		}
	}
	return nil, false
}

// Menus returns every parsed menu template, in resource table order.
func (img *ImageNE) Menus() []*Menu {
	return img.menus
}

// AcceleratorTables returns every parsed accelerator table, in resource
// table order.
func (img *ImageNE) AcceleratorTables() []*AcceleratorTable {
	return img.accelerators
}

// Dialog returns the parsed dialog template with the numeric resource id.
func (img *ImageNE) Dialog(id uint16) (*Dialog, bool) {
	d, ok := img.dialogs[id]
	return d, ok
}

// Dialogs returns every parsed dialog template, in resource table order.
func (img *ImageNE) Dialogs() []*Dialog {
	out := make([]*Dialog, 0, len(img.dialogs))
	for _, r := range img.resources {
		if r.Type.Ordinal && r.Type.ID == ResourceDialog && r.Name.Ordinal {
			out = append(out, img.dialogs[r.Name.ID])
		}
	}
	return out
}

// Control returns the dialog item with the control id.
func (d *Dialog) Control(id uint16) (*DialogControl, bool) {
	for i := range d.Controls {
		if d.Controls[i].ID == id {
			return &d.Controls[i], true
		}
	}
	return nil, false
}

// loadResources reads the NE resource table at resTabOff (relative to the NE
// header) and parses the dialog templates it contains.
func (img *ImageNE) loadResources(f *os.File, neOff, resTabOff, resTabEnd int64) error {
	if resTabEnd <= resTabOff {
		return nil
	}
	table := make([]byte, resTabEnd-resTabOff)
	if _, err := f.ReadAt(table, neOff+resTabOff); err != nil {
		return fmt.Errorf("read resource table: %w", err)
	}
	r := &byteReader{buf: table}
	shift := r.u16()
	name := func(v uint16) ResourceKey {
		if v&resourceOrdinalFlag != 0 {
			return ResourceKey{Ordinal: true, ID: v &^ resourceOrdinalFlag}
		}
		if int(v) >= len(table) {
			return ResourceKey{}
		}
		n := int(table[v])
		if int(v)+1+n > len(table) {
			return ResourceKey{}
		}
		return ResourceKey{Name: string(table[int(v)+1 : int(v)+1+n])}
	}
	for !r.err {
		typeID := r.u16()
		if typeID == 0 {
			break
		}
		count := int(r.u16())
		r.skip(4)
		typ := name(typeID)
		for range count {
			off, length := r.u16(), r.u16()
			r.skip(2)
			id := r.u16()
			r.skip(4)
			if r.err {
				break
			}
			data := make([]byte, int64(length)<<shift)
			if _, err := f.ReadAt(data, int64(off)<<shift); err != nil {
				return fmt.Errorf("read resource %s/%s: %w", typ, name(id), err)
			}
			img.resources = append(img.resources, Resource{Type: typ, Name: name(id), Data: data})
		}
	}
	if r.err {
		return fmt.Errorf("truncated NE resource table")
	}

	img.dialogs = make(map[uint16]*Dialog)
	for _, res := range img.resources {
		if !res.Type.Ordinal {
			continue
		}
		switch res.Type.ID {
		case ResourceDialog:
			if !res.Name.Ordinal {
				return fmt.Errorf("dialog %s: named dialogs are not supported", res.Name)
			}
			d, err := parseDialogTemplate(res.Data)
			if err != nil {
				return fmt.Errorf("dialog %s: %w", res.Name, err)
			}
			d.Name = res.Name
			img.dialogs[res.Name.ID] = d
		case ResourceMenu:
			m, err := parseMenuTemplate(res.Data)
			if err != nil {
				return fmt.Errorf("menu %s: %w", res.Name, err)
			}
			m.Name = res.Name
			img.menus = append(img.menus, m)
		case ResourceAccelerator:
			t, err := parseAcceleratorTable(res.Data)
			if err != nil {
				return fmt.Errorf("accelerators %s: %w", res.Name, err)
			}
			t.Name = res.Name
			img.accelerators = append(img.accelerators, t)
		}
	}
	return nil
}

// parseMenuTemplate parses a Win16 MENUITEMTEMPLATEHEADER and its items.
func parseMenuTemplate(data []byte) (*Menu, error) {
	r := &byteReader{buf: data}
	if version, offset := r.u16(), r.u16(); version != 0 || offset != 0 {
		return nil, fmt.Errorf("unsupported menu template version %d offset %d", version, offset)
	}
	m := &Menu{Items: parseMenuItems(r)}
	if r.err {
		return nil, fmt.Errorf("truncated menu template")
	}
	return m, nil
}

// parseMenuItems parses one menu level, through the item marked as its end.
func parseMenuItems(r *byteReader) []MenuItem {
	var items []MenuItem
	for !r.err {
		flags := r.u16()
		item := MenuItem{Flags: flags &^ menuEnd}
		if flags&MenuPopup == 0 {
			item.ID = r.u16()
		}
		item.Text = r.cstring()
		if flags&MenuPopup != 0 {
			item.Items = parseMenuItems(r)
		}
		items = append(items, item)
		if flags&menuEnd != 0 {
			break
		}
	}
	return items
}

// parseAcceleratorTable parses the 5-byte entries of a Win16 accelerator
// table, through the entry marked as the last.
func parseAcceleratorTable(data []byte) (*AcceleratorTable, error) {
	r := &byteReader{buf: data}
	t := &AcceleratorTable{}
	for !r.err {
		flags := r.u8()
		t.Entries = append(t.Entries, Accelerator{Flags: flags &^ accelEnd, Key: r.u16(), ID: r.u16()})
		if flags&accelEnd != 0 {
			break
		}
	}
	if r.err {
		return nil, fmt.Errorf("truncated accelerator table")
	}
	return t, nil
}

// parseDialogTemplate parses a Win16 DLGTEMPLATE and its items.
func parseDialogTemplate(data []byte) (*Dialog, error) {
	r := &byteReader{buf: data}
	d := &Dialog{Style: r.u32()}
	count := int(r.u8())
	d.X, d.Y, d.CX, d.CY = int16(r.u16()), int16(r.u16()), int16(r.u16()), int16(r.u16())
	d.Menu = r.nameOrOrdinal()
	d.Class = r.cstring()
	d.Caption = r.cstring()
	if d.Style&dialogStyleSetFont != 0 {
		d.FontSize = r.u16()
		d.FontName = r.cstring()
	}
	for range count {
		c := DialogControl{}
		c.X, c.Y, c.CX, c.CY = int16(r.u16()), int16(r.u16()), int16(r.u16()), int16(r.u16())
		c.ID = r.u16()
		c.Style = r.u32()
		if class := r.peek(); class&dialogItemClassFlag != 0 {
			r.skip(1)
			c.Class = predefinedDialogClasses[class]
			if c.Class == "" {
				return nil, fmt.Errorf("unknown predefined control class 0x%02x", class)
			}
		} else {
			c.Class = strings.ToLower(r.cstring())
		}
		c.Text = r.nameOrOrdinal()
		c.Extra = r.bytes(int(r.u8()))
		d.Controls = append(d.Controls, c)
	}
	if r.err {
		return nil, fmt.Errorf("truncated dialog template")
	}
	return d, nil
}

// byteReader reads little-endian values from a buffer, recording truncation
// instead of failing each read.
type byteReader struct {
	buf []byte
	pos int
	err bool
}

// need reports whether n more bytes are available, recording truncation.
func (r *byteReader) need(n int) bool {
	if r.err || r.pos+n > len(r.buf) {
		r.err = true
		return false
	}
	return true
}

// u8 reads one byte.
func (r *byteReader) u8() byte {
	if !r.need(1) {
		return 0
	}
	r.pos++
	return r.buf[r.pos-1]
}

// peek returns the next byte without consuming it.
func (r *byteReader) peek() byte {
	if !r.need(1) {
		return 0
	}
	return r.buf[r.pos]
}

// u16 reads a little-endian word.
func (r *byteReader) u16() uint16 {
	if !r.need(2) {
		return 0
	}
	r.pos += 2
	return binary.LittleEndian.Uint16(r.buf[r.pos-2:])
}

// u32 reads a little-endian double word.
func (r *byteReader) u32() uint32 {
	if !r.need(4) {
		return 0
	}
	r.pos += 4
	return binary.LittleEndian.Uint32(r.buf[r.pos-4:])
}

// skip advances n bytes.
func (r *byteReader) skip(n int) {
	if r.need(n) {
		r.pos += n
	}
}

// bytes reads n raw bytes.
func (r *byteReader) bytes(n int) []byte {
	if !r.need(n) {
		return nil
	}
	r.pos += n
	return bytes.Clone(r.buf[r.pos-n : r.pos])
}

// cstring reads a NUL-terminated string.
func (r *byteReader) cstring() string {
	if r.err {
		return ""
	}
	end := bytes.IndexByte(r.buf[r.pos:], 0)
	if end < 0 {
		r.err = true
		return ""
	}
	s := string(r.buf[r.pos : r.pos+end])
	r.pos += end + 1
	return s
}

// nameOrOrdinal reads a Win16 template name: 0xFF followed by an ordinal
// word, or a NUL-terminated string.
func (r *byteReader) nameOrOrdinal() ResourceKey {
	if r.peek() == dialogItemOrdinalTag {
		r.skip(1)
		return ResourceKey{Ordinal: true, ID: r.u16()}
	}
	return ResourceKey{Name: r.cstring()}
}
