package stars

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"os/exec"
	"path/filepath"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpFuncSem disassembles, extracts machine effects, and lowers them to semantic effects.
func DumpFuncSem(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpSemOptions) error {
	if opt.DiffDir != "" {
		return dumpFuncSemDiff(w, img, sdb, fs, opt)
	}

	analysis, err := analyzeFunc(img, sdb, fs, opt.DumpOptions)
	if err != nil {
		return err
	}

	return renderFuncSem(w, fs, analysis, opt)
}

// renderFuncSem prints semantic effects from an existing function analysis.
func renderFuncSem(w io.Writer, fs *typeinfo.Function, analysis FuncAnalysis, opt DumpSemOptions) error {
	return startemplates.RenderDumpSem(w, startemplates.NewDumpSemView(analysis.Sem, &analysis.Effects, opt, fs, analysis.Annotations, analysis.Annotations))
}

// dumpFuncSemDiff renders each semantic pass to files and prints changed pass diffs.
func dumpFuncSemDiff(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpSemOptions) error {
	if err := os.MkdirAll(opt.DiffDir, 0o755); err != nil {
		return err
	}

	var previous string
	passOpt := opt
	passOpt.DiffDir = ""
	analysis, err := analyzeFuncWithSemPassSnapshots(img, sdb, fs, opt.DumpOptions, func(snapshot sem.PassSnapshot, effects *machine.FuncEffects) error {
		name := fmt.Sprintf("%02d-%s.sem", snapshot.Index, cleanSemPassName(snapshot.Name))
		path := filepath.Join(opt.DiffDir, name)
		rendered, err := renderSemSnapshotFile(path, fs, snapshot.Func, effects, snapshot.Result, passOpt)
		if err != nil {
			return err
		}
		if previous == "" {
			if _, err := w.Write(rendered); err != nil {
				return err
			}
		} else if err := writeSemPassDiff(w, previous, path); err != nil {
			return err
		}
		previous = path
		return nil
	})
	if err != nil {
		return err
	}

	finalPath := filepath.Join(opt.DiffDir, "final.sem")
	if _, err := renderSemSnapshotFile(finalPath, fs, analysis.Sem, &analysis.Effects, analysis.Annotations, passOpt); err != nil {
		return err
	}
	if previous != "" {
		if err := writeSemPassDiff(w, previous, finalPath); err != nil {
			return err
		}
	}

	return renderFuncSem(w, fs, analysis, passOpt)
}

// renderSemSnapshotFile writes one semantic function snapshot to disk and returns the rendered bytes.
func renderSemSnapshotFile(path string, fs *typeinfo.Function, fn sem.Func, effects *machine.FuncEffects, annotations *sem.Result, opt DumpSemOptions) ([]byte, error) {
	var buf bytes.Buffer
	if err := startemplates.RenderDumpSem(&buf, startemplates.NewDumpSemView(fn, effects, opt, fs, annotations, annotations)); err != nil {
		return nil, err
	}
	rendered := buf.Bytes()
	if err := os.WriteFile(path, rendered, 0o644); err != nil {
		return nil, err
	}
	return rendered, nil
}

// writeSemPassDiff runs diff for two pass snapshot files and writes any output.
func writeSemPassDiff(w io.Writer, before string, after string) error {
	cmd := exec.Command("diff", "-u", before, after)
	out, err := cmd.CombinedOutput()
	if len(out) > 0 {
		if _, writeErr := w.Write(out); writeErr != nil {
			return writeErr
		}
	}
	if err == nil {
		return nil
	}
	if exitErr, ok := err.(*exec.ExitError); ok && exitErr.ExitCode() == 1 {
		return nil
	}
	return err
}

// cleanSemPassName makes a pass name safe and readable as a filename component.
func cleanSemPassName(name string) string {
	var b strings.Builder
	for _, r := range name {
		if r >= 'a' && r <= 'z' || r >= 'A' && r <= 'Z' || r >= '0' && r <= '9' || r == '-' || r == '_' {
			b.WriteRune(r)
		} else {
			b.WriteByte('-')
		}
	}
	return strings.Trim(b.String(), "-")
}
