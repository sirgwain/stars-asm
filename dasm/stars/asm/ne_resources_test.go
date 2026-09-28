package asm

import (
	"path/filepath"
	"testing"
)

// TestOpenNEParsesDialogTemplates verifies the Stars! dialog templates are
// read from the NE resource table with their control classes.
func TestOpenNEParsesDialogTemplates(t *testing.T) {
	img, err := OpenNE(filepath.Join("..", "..", "input", "stars.exe"))
	if err != nil {
		t.Fatalf("OpenNE() error = %v", err)
	}
	if got := len(img.Dialogs()); got != 36 {
		t.Fatalf("len(Dialogs()) = %d, want 36", got)
	}
	d, ok := img.Dialog(2008)
	if !ok {
		t.Fatal("Dialog(2008) not found")
	}
	if d.Caption != "Player Relations" || d.FontSize != 8 || d.FontName != "MS Sans Serif" || len(d.Controls) != 7 {
		t.Fatalf("Dialog(2008) = %q font %d %q with %d controls", d.Caption, d.FontSize, d.FontName, len(d.Controls))
	}
	for _, want := range []struct {
		id    uint16
		class string
		text  string
	}{
		{2, "button", "Close"},
		{0xffff, "static", "&Player:"},
		{2003, "listbox", ""},
		{2004, "button", "&Neutral"},
	} {
		c, ok := d.Control(want.id)
		if !ok {
			t.Fatalf("Dialog(2008).Control(%d) not found", want.id)
		}
		if c.Class != want.class || c.Text.String() != want.text {
			t.Fatalf("Dialog(2008).Control(%d) = %s %q, want %s %q", want.id, c.Class, c.Text.String(), want.class, want.text)
		}
	}
}
