package resources

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"
	"unicode/utf16"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestWriteRoundTripsThroughWindres compiles the generated resource script
// with windres and checks every resource it produces against stars.exe.
func TestWriteRoundTripsThroughWindres(t *testing.T) {
	windres, err := exec.LookPath("x86_64-w64-mingw32-windres")
	if err != nil {
		t.Skip("x86_64-w64-mingw32-windres not installed")
	}
	fx := testfixture.Stars(t)
	dir := t.TempDir()
	if err := Write(dir, fx.Image, fx.SDB); err != nil {
		t.Fatalf("Write() error = %v", err)
	}
	resPath := filepath.Join(dir, "stars.res")
	cmd := exec.Command(windres, "stars.rc", "-O", "res", "-o", resPath)
	cmd.Dir = dir
	if out, err := cmd.CombinedOutput(); err != nil {
		t.Fatalf("windres: %v\n%s", err, out)
	}
	compiled := readRes(t, resPath)

	for _, r := range fx.Image.Resources() {
		if !r.Type.Ordinal || r.Type.ID == asm.ResourceIcon || r.Type.ID == asm.ResourceCursor {
			continue // images are checked through their groups
		}
		t.Run(fmt.Sprintf("%s/%s", r.Type, r.Name), func(t *testing.T) {
			got, ok := compiled[resKey{r.Type, upperName(r.Name)}]
			if !ok {
				t.Fatal("resource missing from compiled script")
			}
			switch r.Type.ID {
			case asm.ResourceDialog:
				want, _ := fx.Image.Dialog(r.Name.ID)
				compareDialog(t, want, got)
			case asm.ResourceMenu:
				// Skip the menu template header's version and offset words.
				compareMenuItems(t, fx.Image.Menus()[0].Items, parseWin32MenuItems(&wreader{buf: got, pos: 4}))
			case asm.ResourceAccelerator:
				compareAccelerators(t, r.Data, got)
			case asm.ResourceGroupIcon, asm.ResourceGroupCursor:
				compareGroup(t, fx.Image, compiled, r, got)
			case asm.ResourceBitmap:
				if !bytes.HasPrefix(r.Data, got) || len(bytes.Trim(r.Data[len(got):], "\x00")) != 0 {
					t.Fatalf("bitmap data differs")
				}
			default:
				if !bytes.Equal(r.Data, got) {
					t.Fatalf("data differs")
				}
			}
		})
	}
}

// resKey identifies a compiled resource by type and name.
type resKey struct {
	typ, name asm.ResourceKey
}

// upperName returns a resource name as the resource compiler stores it:
// string names in upper case.
func upperName(k asm.ResourceKey) asm.ResourceKey {
	if !k.Ordinal {
		k.Name = strings.ToUpper(k.Name)
	}
	return k
}

// readRes parses a Win32 .res file into its resources' data.
func readRes(t *testing.T, path string) map[resKey][]byte {
	t.Helper()
	data := mustRead(t, path)
	out := make(map[resKey][]byte)
	for off := 0; off+8 <= len(data); {
		dataSize := int(binary.LittleEndian.Uint32(data[off:]))
		headerSize := int(binary.LittleEndian.Uint32(data[off+4:]))
		r := &wreader{buf: data, pos: off + 8}
		typ, name := r.nameOrOrdinal(), r.nameOrOrdinal()
		body := data[off+headerSize : off+headerSize+dataSize]
		if dataSize > 0 {
			out[resKey{typ, name}] = body
		}
		off = align4(off + headerSize + dataSize)
	}
	return out
}

// compareDialog checks a compiled Win32 dialog template against a Win16 one.
func compareDialog(t *testing.T, want *asm.Dialog, data []byte) {
	t.Helper()
	r := &wreader{buf: data}
	style := r.u32()
	r.u32() // extended style
	count := int(r.u16())
	x, y, cx, cy := int16(r.u16()), int16(r.u16()), int16(r.u16()), int16(r.u16())
	menu, class, caption := r.nameOrOrdinal(), r.string(), r.string()
	if style != want.Style || count != len(want.Controls) || x != want.X || y != want.Y || cx != want.CX || cy != want.CY ||
		menu != want.Menu || class != want.Class || caption != want.Caption {
		t.Fatalf("dialog header = %08x %d (%d,%d,%d,%d) %v %q %q, want %08x %d (%d,%d,%d,%d) %v %q %q",
			style, count, x, y, cx, cy, menu, class, caption,
			want.Style, len(want.Controls), want.X, want.Y, want.CX, want.CY, want.Menu, want.Class, want.Caption)
	}
	if style&dialogStyleSetFont != 0 {
		size, face := r.u16(), r.string()
		if size != want.FontSize || face != want.FontName {
			t.Fatalf("dialog font = %d %q, want %d %q", size, face, want.FontSize, want.FontName)
		}
	}
	for _, c := range want.Controls {
		r.pos = align4(r.pos)
		style := r.u32()
		r.u32() // extended style
		x, y, cx, cy := int16(r.u16()), int16(r.u16()), int16(r.u16()), int16(r.u16())
		id := r.u16()
		class, text := r.nameOrOrdinal(), r.nameOrOrdinal()
		r.pos += int(r.u16())
		if class.Ordinal {
			class = asm.ResourceKey{Name: controlClassOrdinals[class.ID]}
		}
		if style != c.Style || x != c.X || y != c.Y || cx != c.CX || cy != c.CY || id != c.ID ||
			!strings.EqualFold(class.Name, c.Class) || text != c.Text {
			t.Fatalf("control = %08x (%d,%d,%d,%d) %d %s %v, want %08x (%d,%d,%d,%d) %d %s %v",
				style, x, y, cx, cy, id, class.Name, text, c.Style, c.X, c.Y, c.CX, c.CY, c.ID, c.Class, c.Text)
		}
	}
}

// dialogStyleSetFont is the DS_SETFONT dialog style.
const dialogStyleSetFont = 0x40

// controlClassOrdinals names the predefined control class ordinals of a
// Win32 dialog template.
var controlClassOrdinals = map[uint16]string{
	0x80: "button", 0x81: "edit", 0x82: "static", 0x83: "listbox", 0x84: "scrollbar", 0x85: "combobox",
}

// parseWin32MenuItems parses one level of a Win32 menu template.
func parseWin32MenuItems(r *wreader) []asm.MenuItem {
	var items []asm.MenuItem
	for r.pos < len(r.buf) {
		flags := r.u16()
		item := asm.MenuItem{Flags: flags &^ 0x80}
		if flags&asm.MenuPopup == 0 {
			item.ID = r.u16()
		}
		item.Text = r.string()
		if flags&asm.MenuPopup != 0 {
			item.Items = parseWin32MenuItems(r)
		}
		items = append(items, item)
		if flags&0x80 != 0 {
			break
		}
	}
	return items
}

// compareMenuItems checks compiled menu items against the Win16 ones.
func compareMenuItems(t *testing.T, want, got []asm.MenuItem) {
	t.Helper()
	if len(got) != len(want) {
		t.Fatalf("menu has %d items, want %d", len(got), len(want))
	}
	for i := range want {
		if got[i].Flags != want[i].Flags || got[i].ID != want[i].ID || got[i].Text != want[i].Text {
			t.Fatalf("menu item = %04x %d %q, want %04x %d %q", got[i].Flags, got[i].ID, got[i].Text, want[i].Flags, want[i].ID, want[i].Text)
		}
		compareMenuItems(t, want[i].Items, got[i].Items)
	}
}

// compareAccelerators checks compiled 8-byte Win32 accelerator entries
// against the 5-byte Win16 ones.
func compareAccelerators(t *testing.T, win16, win32 []byte) {
	t.Helper()
	for i := 0; ; i++ {
		flags16, key16, id16 := win16[5*i], binary.LittleEndian.Uint16(win16[5*i+1:]), binary.LittleEndian.Uint16(win16[5*i+3:])
		flags32, key32, id32 := binary.LittleEndian.Uint16(win32[8*i:]), binary.LittleEndian.Uint16(win32[8*i+2:]), binary.LittleEndian.Uint16(win32[8*i+4:])
		if uint16(flags16) != flags32 || key16 != key32 || id16 != id32 {
			t.Fatalf("accelerator %d = %02x %04x %d, want %02x %04x %d", i, flags32, key32, id32, flags16, key16, id16)
		}
		if flags16&0x80 != 0 {
			return
		}
	}
}

// compareGroup checks a compiled icon or cursor group's directory and
// images against the Win16 group. The compiler numbers the images itself.
func compareGroup(t *testing.T, img *asm.ImageNE, compiled map[resKey][]byte, group asm.Resource, got []byte) {
	t.Helper()
	imageType := asm.ResourceIcon
	if group.Type.ID == asm.ResourceGroupCursor {
		imageType = asm.ResourceCursor
	}
	want, err := groupEntries(&group)
	if err != nil {
		t.Fatal(err)
	}
	gotEntries, err := groupEntries(&asm.Resource{Name: group.Name, Data: got})
	if err != nil {
		t.Fatal(err)
	}
	if len(gotEntries) != len(want) {
		t.Fatalf("group has %d entries, want %d", len(gotEntries), len(want))
	}
	for i := range want {
		if gotEntries[i].dir != want[i].dir || gotEntries[i].bytes != want[i].bytes {
			t.Fatalf("group entry %d = %v %d, want %v %d", i, gotEntries[i].dir, gotEntries[i].bytes, want[i].dir, want[i].bytes)
		}
		wantImage, _ := img.Resource(imageType, asm.ResourceKey{Ordinal: true, ID: want[i].id})
		gotImage := compiled[resKey{asm.ResourceKey{Ordinal: true, ID: imageType}, asm.ResourceKey{Ordinal: true, ID: gotEntries[i].id}}]
		if !bytes.Equal(gotImage, wantImage.Data[:want[i].bytes]) {
			t.Fatalf("group entry %d image differs", i)
		}
	}
}

// wreader reads a Win32 resource: little-endian words and UTF-16 strings,
// which the Stars! resources keep within Latin-1.
type wreader struct {
	buf []byte
	pos int
}

// u16 reads a word.
func (r *wreader) u16() uint16 {
	r.pos += 2
	return binary.LittleEndian.Uint16(r.buf[r.pos-2:])
}

// u32 reads a double word.
func (r *wreader) u32() uint32 {
	r.pos += 4
	return binary.LittleEndian.Uint32(r.buf[r.pos-4:])
}

// string reads a NUL-terminated UTF-16 string as Latin-1 bytes.
func (r *wreader) string() string {
	var units []uint16
	for u := r.u16(); u != 0; u = r.u16() {
		units = append(units, u)
	}
	var b strings.Builder
	for _, c := range utf16.Decode(units) {
		b.WriteByte(latin1(c))
	}
	return b.String()
}

// nameOrOrdinal reads a Win32 name: 0xFFFF followed by an ordinal, or a
// string.
func (r *wreader) nameOrOrdinal() asm.ResourceKey {
	if binary.LittleEndian.Uint16(r.buf[r.pos:]) == 0xffff {
		r.pos += 2
		return asm.ResourceKey{Ordinal: true, ID: r.u16()}
	}
	return asm.ResourceKey{Name: r.string()}
}

// latin1 maps a character back to the Windows-1252 byte the Win16 resource
// held; the Stars! resources only use Latin-1 characters.
func latin1(c rune) byte {
	if c > 0xff {
		return '?'
	}
	return byte(c)
}

// align4 rounds an offset up to a double-word boundary.
func align4(off int) int {
	return (off + 3) &^ 3
}

// mustRead reads a file, failing the test on error.
func mustRead(t *testing.T, path string) []byte {
	t.Helper()
	data, err := os.ReadFile(path)
	if err != nil {
		t.Fatal(err)
	}
	return data
}
