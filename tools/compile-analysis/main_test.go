package main

import (
	"os"
	"path/filepath"
	"runtime"
	"testing"
)

// TestGenerateReport verifies that ordinary compiler errors produce a usable report.
func TestGenerateReport(t *testing.T) {
	if runtime.GOOS == "windows" {
		t.Skip("test compiler is a shell script")
	}
	dir := t.TempDir()
	sourceDir := filepath.Join(dir, "decompiled")
	if err := os.Mkdir(sourceDir, 0o755); err != nil {
		t.Fatal(err)
	}
	for _, name := range []string{"a.c", "b.c"} {
		if err := os.WriteFile(filepath.Join(sourceDir, name), []byte("bad C"), 0o644); err != nil {
			t.Fatal(err)
		}
	}
	compiler := filepath.Join(dir, "fake-cc")
	script := `#!/bin/sh
case "$1" in
  -dumpfullversion) echo 13.3.0; exit 0 ;;
  -dumpmachine) echo x86_64-w64-mingw32; exit 0 ;;
esac
printf '%s\n' '[{"kind":"error","message":"unknown type name HB","locations":[{"caret":{"file":"decompiled/structs.h","line":258,"column":5}}],"children":[]}]' >&2
exit 1
`
	if err := os.WriteFile(compiler, []byte(script), 0o755); err != nil {
		t.Fatal(err)
	}

	got, err := generateReport(config{cc: compiler, sourceDir: sourceDir})
	if err != nil {
		t.Fatal(err)
	}
	if got.Summary.TranslationUnits != 2 || got.Summary.FailedTranslationUnits != 2 || got.Summary.Errors != 2 || got.Summary.UniqueErrors != 1 {
		t.Fatalf("unexpected summary: %+v", got.Summary)
	}
	if len(got.Diagnostics) != 1 || got.Diagnostics[0].Occurrences != 2 || len(got.Diagnostics[0].TranslationUnits) != 2 {
		t.Fatalf("unexpected diagnostics: %+v", got.Diagnostics)
	}
}

// TestGenerateReportResourceScripts verifies resource compiler errors are
// reported against the script, relative to the source directory.
func TestGenerateReportResourceScripts(t *testing.T) {
	if runtime.GOOS == "windows" {
		t.Skip("test compilers are shell scripts")
	}
	dir := t.TempDir()
	sourceDir := filepath.Join(dir, "decompiled")
	if err := os.MkdirAll(filepath.Join(sourceDir, "res"), 0o755); err != nil {
		t.Fatal(err)
	}
	for name, body := range map[string]string{"a.c": "int a;", "res/stars.rc": "BAD RC"} {
		if err := os.WriteFile(filepath.Join(sourceDir, name), []byte(body), 0o644); err != nil {
			t.Fatal(err)
		}
	}
	compiler := filepath.Join(dir, "fake-cc")
	ccScript := `#!/bin/sh
case "$1" in
  -dumpfullversion) echo 13.3.0; exit 0 ;;
  -dumpmachine) echo x86_64-w64-mingw32; exit 0 ;;
esac
printf '[]' >&2
exit 0
`
	windres := filepath.Join(dir, "fake-windres")
	windresScript := `#!/bin/sh
echo "fake-windres: stars.rc:7: syntax error" >&2
exit 1
`
	for path, script := range map[string]string{compiler: ccScript, windres: windresScript} {
		if err := os.WriteFile(path, []byte(script), 0o755); err != nil {
			t.Fatal(err)
		}
	}

	got, err := generateReport(config{cc: compiler, windres: windres, sourceDir: sourceDir})
	if err != nil {
		t.Fatal(err)
	}
	if got.Summary.TranslationUnits != 2 || got.Summary.FailedTranslationUnits != 1 || got.Summary.Errors != 1 {
		t.Fatalf("unexpected summary: %+v", got.Summary)
	}
	d := got.Diagnostics[0]
	wantFile := filepath.ToSlash(filepath.Join(sourceDir, "res", "stars.rc"))
	if d.Message != "syntax error" || d.Locations[0].Caret.File != wantFile || d.Locations[0].Caret.Line != 7 {
		t.Fatalf("unexpected diagnostic: %+v %+v", d, d.Locations[0].Caret)
	}
}
