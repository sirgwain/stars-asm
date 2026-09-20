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
