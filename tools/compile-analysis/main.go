// Command compile-analysis records MinGW C diagnostics without failing for
// ordinary errors in the generated source.
package main

import (
	"bytes"
	"encoding/json"
	"errors"
	"flag"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"sort"
	"strconv"
	"strings"
)

type config struct {
	cc           string
	windres      string
	out          string
	sourceDir    string
	failOnErrors bool
}

type compilerInfo struct {
	Command string `json:"command"`
	Version string `json:"version"`
	Target  string `json:"target"`
}

type summary struct {
	TranslationUnits       int `json:"translationUnits"`
	FailedTranslationUnits int `json:"failedTranslationUnits"`
	Errors                 int `json:"errors"`
	UniqueErrors           int `json:"uniqueErrors"`
	Warnings               int `json:"warnings"`
	UniqueWarnings         int `json:"uniqueWarnings"`
}

type fileSummary struct {
	Errors   int `json:"errors"`
	Warnings int `json:"warnings"`
}

type position struct {
	File          string `json:"file"`
	Line          int    `json:"line"`
	Column        int    `json:"column"`
	ByteColumn    int    `json:"byteColumn,omitempty"`
	DisplayColumn int    `json:"displayColumn,omitempty"`
}

type location struct {
	Caret  *position `json:"caret,omitempty"`
	Start  *position `json:"start,omitempty"`
	Finish *position `json:"finish,omitempty"`
}

type gccPosition struct {
	File          string `json:"file"`
	Line          int    `json:"line"`
	Column        int    `json:"column"`
	ByteColumn    int    `json:"byte-column"`
	DisplayColumn int    `json:"display-column"`
}

type gccLocation struct {
	Caret  *gccPosition `json:"caret"`
	Start  *gccPosition `json:"start"`
	Finish *gccPosition `json:"finish"`
}

type gccDiagnostic struct {
	Kind      string          `json:"kind"`
	Message   string          `json:"message"`
	Option    string          `json:"option"`
	Locations []gccLocation   `json:"locations"`
	Children  []gccDiagnostic `json:"children"`
}

type diagnostic struct {
	TranslationUnits []string     `json:"translationUnits,omitempty"`
	Kind             string       `json:"kind"`
	Message          string       `json:"message"`
	Option           string       `json:"option,omitempty"`
	Locations        []location   `json:"locations,omitempty"`
	Children         []diagnostic `json:"children,omitempty"`
	Occurrences      int          `json:"occurrences,omitempty"`
}

type report struct {
	Compiler    compilerInfo           `json:"compiler"`
	Summary     summary                `json:"summary"`
	Files       map[string]fileSummary `json:"files"`
	Diagnostics []diagnostic           `json:"diagnostics"`
}

// main parses command-line flags and writes the compile report.
func main() {
	var cfg config
	flag.StringVar(&cfg.cc, "cc", "x86_64-w64-mingw32-gcc", "MinGW C compiler")
	flag.StringVar(&cfg.windres, "windres", "", "MinGW resource compiler; when set, resource scripts under SOURCE_DIR/res are compiled too")
	flag.StringVar(&cfg.out, "out", "decompiled/compile-analysis.json", "JSON report path")
	flag.BoolVar(&cfg.failOnErrors, "fail-on-errors", false, "return failure when C errors are found")
	flag.Parse()
	if flag.NArg() != 1 {
		fmt.Fprintln(os.Stderr, "usage: compile-analysis [flags] SOURCE_DIR")
		os.Exit(2)
	}
	cfg.sourceDir = flag.Arg(0)

	report, err := generateReport(cfg)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
	if err := writeReport(cfg.out, report); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
	fmt.Printf("MinGW analysis: %d errors (%d unique) in %d/%d translation units, %d warnings (%d unique)\n",
		report.Summary.Errors,
		report.Summary.UniqueErrors,
		report.Summary.FailedTranslationUnits,
		report.Summary.TranslationUnits,
		report.Summary.Warnings,
		report.Summary.UniqueWarnings,
	)
	if cfg.failOnErrors && report.Summary.Errors != 0 {
		os.Exit(1)
	}
}

// generateReport invokes MinGW once per translation unit, C files and, with
// a resource compiler, resource scripts, and aggregates their diagnostics.
func generateReport(cfg config) (report, error) {
	files, err := sourceFiles(cfg.sourceDir)
	if err != nil {
		return report{}, err
	}
	var scripts []string
	if cfg.windres != "" {
		if scripts, err = resourceScripts(cfg.sourceDir); err != nil {
			return report{}, err
		}
	}
	info, err := inspectCompiler(cfg.cc)
	if err != nil {
		return report{}, err
	}

	result := report{
		Compiler: info,
		Files:    make(map[string]fileSummary, len(files)+len(scripts)),
	}
	unique := make(map[string]int)
	for _, file := range append(files, scripts...) {
		var diagnostics []diagnostic
		var failed bool
		if strings.HasSuffix(file, ".rc") {
			diagnostics, failed, err = compileResourceScript(cfg.windres, file)
		} else {
			diagnostics, failed, err = compileFile(cfg.cc, cfg.sourceDir, file)
		}
		if err != nil {
			return report{}, err
		}
		var counts fileSummary
		for _, item := range diagnostics {
			if item.Kind == "warning" {
				counts.Warnings++
			} else {
				counts.Errors++
			}
		}
		result.Files[file] = counts
		result.Summary.TranslationUnits++
		result.Summary.Errors += counts.Errors
		result.Summary.Warnings += counts.Warnings
		if failed {
			result.Summary.FailedTranslationUnits++
		}
		for _, item := range diagnostics {
			key := diagnosticKey(item)
			if index, ok := unique[key]; ok {
				result.Diagnostics[index].Occurrences++
				result.Diagnostics[index].TranslationUnits = append(result.Diagnostics[index].TranslationUnits, file)
				continue
			}
			item.TranslationUnits = []string{file}
			item.Occurrences = 1
			unique[key] = len(result.Diagnostics)
			result.Diagnostics = append(result.Diagnostics, item)
			if item.Kind == "warning" {
				result.Summary.UniqueWarnings++
			} else {
				result.Summary.UniqueErrors++
			}
		}
	}
	return result, nil
}

// sourceFiles returns stable, repository-relative C translation-unit paths.
func sourceFiles(sourceDir string) ([]string, error) {
	files, err := filepath.Glob(filepath.Join(sourceDir, "*.c"))
	if err != nil {
		return nil, fmt.Errorf("find C sources: %w", err)
	}
	if len(files) == 0 {
		return nil, fmt.Errorf("no C sources found in %s", sourceDir)
	}
	for i := range files {
		files[i] = filepath.ToSlash(filepath.Clean(files[i]))
	}
	sort.Strings(files)
	return files, nil
}

// resourceScripts returns the repository-relative resource scripts in
// SOURCE_DIR/res.
func resourceScripts(sourceDir string) ([]string, error) {
	scripts, err := filepath.Glob(filepath.Join(sourceDir, "res", "*.rc"))
	if err != nil {
		return nil, fmt.Errorf("find resource scripts: %w", err)
	}
	if len(scripts) == 0 {
		return nil, fmt.Errorf("no resource scripts found in %s", filepath.Join(sourceDir, "res"))
	}
	for i := range scripts {
		scripts[i] = filepath.ToSlash(filepath.Clean(scripts[i]))
	}
	sort.Strings(scripts)
	return scripts, nil
}

// inspectCompiler records the compiler version and target used by the report.
func inspectCompiler(cc string) (compilerInfo, error) {
	version, err := commandOutput(cc, "-dumpfullversion")
	if err != nil {
		version, err = commandOutput(cc, "-dumpversion")
	}
	if err != nil {
		return compilerInfo{}, fmt.Errorf("inspect compiler version: %w", err)
	}
	target, err := commandOutput(cc, "-dumpmachine")
	if err != nil {
		return compilerInfo{}, fmt.Errorf("inspect compiler target: %w", err)
	}
	return compilerInfo{Command: cc, Version: version, Target: target}, nil
}

// commandOutput runs a compiler metadata command and trims its output.
func commandOutput(name string, args ...string) (string, error) {
	output, err := exec.Command(name, args...).CombinedOutput()
	if err != nil {
		return "", fmt.Errorf("run %s %s: %w: %s", name, strings.Join(args, " "), err, strings.TrimSpace(string(output)))
	}
	return strings.TrimSpace(string(output)), nil
}

// compileFile returns the error and warning diagnostics and compiler failure
// status for one C file. GCC's default warnings stay on, since some
// extraction bugs, such as comparing a pointer with a constant address, only
// warn; -Wpointer-sign is off because integer pointees differing only in sign
// intentionally keep plain decay. -fsigned-char matches the original MSC build
// and the native build, which the decompiler relies on to drop char casts.
func compileFile(cc, sourceDir, file string) ([]diagnostic, bool, error) {
	cmd := exec.Command(cc,
		"-std=gnu11",
		"-fsigned-char",
		"-fsyntax-only",
		"-fdiagnostics-format=json",
		"-fdiagnostics-color=never",
		"-fmax-errors=0",
		"-fdiagnostics-show-option",
		"-Wno-pointer-sign",
		"-I"+filepath.ToSlash(filepath.Clean(sourceDir)),
		file,
	)
	var stderr bytes.Buffer
	cmd.Stderr = &stderr
	var stdout bytes.Buffer
	cmd.Stdout = &stdout
	err := cmd.Run()
	failed := err != nil
	if err != nil {
		var exitErr *exec.ExitError
		if !errors.As(err, &exitErr) {
			return nil, false, fmt.Errorf("start compiler for %s: %w", file, err)
		}
	}
	if stdout.Len() != 0 {
		return nil, false, fmt.Errorf("compiler wrote unexpected stdout for %s: %s", file, strings.TrimSpace(stdout.String()))
	}

	var raw []gccDiagnostic
	if err := json.Unmarshal(stderr.Bytes(), &raw); err != nil {
		return nil, false, fmt.Errorf("decode compiler diagnostics for %s: %w: %s", file, err, strings.TrimSpace(stderr.String()))
	}
	result := make([]diagnostic, 0, len(raw))
	for _, item := range raw {
		if item.Kind != "error" && item.Kind != "fatal error" && item.Kind != "warning" {
			continue
		}
		result = append(result, convertDiagnostic(item))
	}
	if failed && !hasError(result) {
		return nil, false, fmt.Errorf("compiler failed for %s without an error diagnostic", file)
	}
	return result, failed, nil
}

// hasError reports whether diagnostics hold an error or fatal error.
func hasError(diagnostics []diagnostic) bool {
	for _, item := range diagnostics {
		if item.Kind != "warning" {
			return true
		}
	}
	return false
}

// reWindresDiagnostic matches a windres or preprocessor diagnostic:
// an optional tool prefix, file:line[:column], an optional severity and the
// message.
var reWindresDiagnostic = regexp.MustCompile(`^(?:\S*windres: )?([^:\s]+):(\d+):(?:(\d+):)?\s*(?:(fatal error|error|warning): )?(.*)$`)

// compileResourceScript returns the error diagnostics and failure status for
// one resource script. The script is compiled in its own directory, since it
// names the files it includes relative to itself.
func compileResourceScript(windres, file string) ([]diagnostic, bool, error) {
	out, err := os.CreateTemp("", "compile-analysis-*.res")
	if err != nil {
		return nil, false, err
	}
	out.Close()
	defer os.Remove(out.Name())

	cmd := exec.Command(windres, filepath.Base(file), "-O", "res", "-o", out.Name())
	cmd.Dir = filepath.Dir(file)
	output, err := cmd.CombinedOutput()
	failed := err != nil
	if err != nil {
		var exitErr *exec.ExitError
		if !errors.As(err, &exitErr) {
			return nil, false, fmt.Errorf("start resource compiler for %s: %w", file, err)
		}
	}
	var result []diagnostic
	for _, line := range strings.Split(strings.TrimSpace(string(output)), "\n") {
		if line == "" {
			continue
		}
		m := reWindresDiagnostic.FindStringSubmatch(line)
		if m == nil {
			// Messages such as a missing icon file carry no location.
			if failed {
				result = append(result, diagnostic{Kind: "error", Message: line})
			}
			continue
		}
		kind := "error"
		if m[4] != "" {
			kind = m[4]
		}
		lineNo, _ := strconv.Atoi(m[2])
		column, _ := strconv.Atoi(m[3])
		path := m[1]
		if !filepath.IsAbs(path) {
			path = filepath.Join(filepath.Dir(file), path)
		}
		where := &position{File: filepath.ToSlash(path), Line: lineNo, Column: column}
		result = append(result, diagnostic{Kind: kind, Message: m[5], Locations: []location{{Caret: where}}})
	}
	if failed && !hasError(result) {
		return nil, false, fmt.Errorf("resource compiler failed for %s without an error diagnostic", file)
	}
	return result, failed, nil
}

// convertDiagnostic converts GCC's JSON field names into the stable report schema.
func convertDiagnostic(item gccDiagnostic) diagnostic {
	result := diagnostic{Kind: item.Kind, Message: item.Message, Option: item.Option}
	for _, where := range item.Locations {
		result.Locations = append(result.Locations, location{
			Caret:  convertPosition(where.Caret),
			Start:  convertPosition(where.Start),
			Finish: convertPosition(where.Finish),
		})
	}
	for _, child := range item.Children {
		result.Children = append(result.Children, convertDiagnostic(child))
	}
	return result
}

// convertPosition converts one optional GCC source position.
func convertPosition(value *gccPosition) *position {
	if value == nil {
		return nil
	}
	return &position{
		File:          filepath.ToSlash(value.File),
		Line:          value.Line,
		Column:        value.Column,
		ByteColumn:    value.ByteColumn,
		DisplayColumn: value.DisplayColumn,
	}
}

// diagnosticKey identifies repeated header diagnostics across translation units.
func diagnosticKey(item diagnostic) string {
	var file string
	var line, column int
	if len(item.Locations) != 0 && item.Locations[0].Caret != nil {
		file = item.Locations[0].Caret.File
		line = item.Locations[0].Caret.Line
		column = item.Locations[0].Caret.Column
	}
	return fmt.Sprintf("%s\x00%s\x00%s\x00%d\x00%d", item.Kind, item.Message, file, line, column)
}

// writeReport writes deterministic, indented JSON to the requested path.
func writeReport(path string, value report) error {
	if err := os.MkdirAll(filepath.Dir(path), 0o755); err != nil {
		return fmt.Errorf("create report directory: %w", err)
	}
	data, err := json.MarshalIndent(value, "", "  ")
	if err != nil {
		return fmt.Errorf("encode report: %w", err)
	}
	data = append(data, '\n')
	if err := os.WriteFile(path, data, 0o644); err != nil {
		return fmt.Errorf("write report: %w", err)
	}
	return nil
}
