package templates

import (
	"bytes"
	"fmt"
	"os/exec"
	"path/filepath"
	"runtime"
)

// formatCSource runs clang-format over rendered C source.
func formatCSource(src string) (string, error) {
	cmd := exec.Command("clang-format",
		"--assume-filename="+filepath.Join(repoRoot(), "dump.c"),
		"--style=file",
	)
	cmd.Stdin = bytes.NewBufferString(src)

	var out bytes.Buffer
	var stderr bytes.Buffer
	cmd.Stdout = &out
	cmd.Stderr = &stderr
	if err := cmd.Run(); err != nil {
		if stderr.Len() > 0 {
			return "", fmt.Errorf("clang-format: %w: %s", err, stderr.String())
		}
		return "", fmt.Errorf("clang-format: %w", err)
	}
	return out.String(), nil
}

// repoRoot returns the repository root directory.
func repoRoot() string {
	_, file, _, _ := runtime.Caller(0)
	dir := filepath.Dir(file)
	for {
		if filepath.Base(dir) == "dasm" {
			return filepath.Dir(dir)
		}
		parent := filepath.Dir(dir)
		if parent == dir {
			panic("could not find repo root")
		}
		dir = parent
	}
}
