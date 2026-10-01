package templates

import (
	"bytes"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"
)

// TestWin16QsortExecutes checks original tie ordering and native element widths
// by compiling the sorting shim from the rendered compatibility header.
func TestWin16QsortExecutes(t *testing.T) {
	cc, err := exec.LookPath("cc")
	if err != nil {
		t.Skip("C compiler unavailable")
	}
	var header bytes.Buffer
	if err := RenderWin16Defines(&header, NewWin16DefinesView(nil, nil)); err != nil {
		t.Fatal(err)
	}
	text := header.String()
	start := strings.Index(text, "// qsortSwap16")
	end := strings.Index(text, "static inline char *strdate")
	if start < 0 || end <= start {
		t.Fatal("rendered header is missing the sorting shim")
	}
	fixture, err := os.ReadFile("testdata/qsort16.c")
	if err != nil {
		t.Fatal(err)
	}
	source := "#include <stdint.h>\n#include <stddef.h>\n#include <limits.h>\n" + text[start:end] + string(fixture)
	dir := t.TempDir()
	path, exe := filepath.Join(dir, "qsort16.c"), filepath.Join(dir, "qsort16")
	if err := os.WriteFile(path, []byte(source), 0600); err != nil {
		t.Fatal(err)
	}
	if out, err := exec.Command(cc, "-std=c99", "-Wall", "-Werror", "-fsanitize=undefined,address", path, "-o", exe).CombinedOutput(); err != nil {
		t.Fatalf("compile: %v\n%s", err, out)
	}
	if out, err := exec.Command(exe).CombinedOutput(); err != nil {
		t.Fatalf("execute: %v\n%s", err, out)
	}
}
