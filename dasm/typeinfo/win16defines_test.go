package typeinfo

import (
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func TestLoadWin16DefinesTakesValuesFromHeaders(t *testing.T) {
	dir := t.TempDir()
	header := `
#define WM_USER             0x0400
#define WS_POPUP            0x80000000L
#define GWL_WNDPROC         (-4)
#define IDC_ARROW           MAKEINTRESOURCE(32512)
#define CB_ADDSTRING        (WM_USER+3)
#define CW_USEDEFAULT       ((int)0x8000)
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
`
	config := `{
  "headers": ["WINDOWS.H"],
  "families": [
    { "name": "WMType", "members": ["CB_ADDSTRING", "WM_APP_PING = WM_USER + 0x64"] },
    { "name": "WindowStyle", "kind": "flags", "members": ["WS_POPUP"] },
    { "name": "Misc", "members": ["GWL_WNDPROC", "IDC_ARROW", "CW_USEDEFAULT"] }
  ]
}`
	writeTestFile(t, filepath.Join(dir, "WINDOWS.H"), header)
	writeTestFile(t, filepath.Join(dir, "win16defines.json"), config)

	enums, err := (&enumLoader{}).loadWin16Defines(dir)
	if err != nil {
		t.Fatalf("loadWin16Defines() error = %v", err)
	}
	want := map[string]int{
		"CB_ADDSTRING":  0x0403,
		"WM_APP_PING":   0x0464,
		"WS_POPUP":      0x80000000,
		"GWL_WNDPROC":   -4,
		"IDC_ARROW":     32512,
		"CW_USEDEFAULT": 0x8000,
	}
	for _, e := range enums {
		if e.Storage == nil {
			t.Errorf("%s has no storage type; Win16 families are #define constants", e.Name)
		}
		for _, v := range e.Values {
			if got := v.Value; got != want[v.Name] {
				t.Errorf("%s.%s = %#x, want %#x", e.Name, v.Name, got, want[v.Name])
			}
		}
	}
	if enums[1].EnumKind != EnumFlags {
		t.Errorf("WindowStyle kind = %v, want flags", enums[1].EnumKind)
	}
}

func TestLoadWin16DefinesRejectsNamesMissingFromHeaders(t *testing.T) {
	dir := t.TempDir()
	writeTestFile(t, filepath.Join(dir, "WINDOWS.H"), "#define MB_OK 0x0000\n")
	writeTestFile(t, filepath.Join(dir, "win16defines.json"),
		`{"headers": ["WINDOWS.H"], "families": [{"name": "MessageBoxType", "members": ["MB_OK", "MB_HELP"]}]}`)

	_, err := (&enumLoader{}).loadWin16Defines(dir)
	if err == nil || !strings.Contains(err.Error(), "MB_HELP is not defined") {
		t.Fatalf("loadWin16Defines() error = %v, want MB_HELP not defined", err)
	}
}

// writeTestFile writes contents to path or fails the test.
func writeTestFile(t *testing.T, path, contents string) {
	t.Helper()
	if err := os.WriteFile(path, []byte(contents), 0o644); err != nil {
		t.Fatalf("write %s: %v", path, err)
	}
}
