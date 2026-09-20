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
	"sort"
	"strings"
)

type config struct {
	cc           string
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
}

type fileSummary struct {
	Errors int `json:"errors"`
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
	fmt.Printf("MinGW syntax analysis: %d errors (%d unique) in %d/%d translation units\n",
		report.Summary.Errors,
		report.Summary.UniqueErrors,
		report.Summary.FailedTranslationUnits,
		report.Summary.TranslationUnits,
	)
	if cfg.failOnErrors && report.Summary.Errors != 0 {
		os.Exit(1)
	}
}

// generateReport invokes MinGW once per translation unit and aggregates its diagnostics.
func generateReport(cfg config) (report, error) {
	files, err := sourceFiles(cfg.sourceDir)
	if err != nil {
		return report{}, err
	}
	info, err := inspectCompiler(cfg.cc)
	if err != nil {
		return report{}, err
	}

	result := report{
		Compiler: info,
		Files:    make(map[string]fileSummary, len(files)),
	}
	unique := make(map[string]int)
	for _, file := range files {
		diagnostics, failed, err := compileFile(cfg.cc, cfg.sourceDir, file)
		if err != nil {
			return report{}, err
		}
		errorCount := len(diagnostics)
		result.Files[file] = fileSummary{Errors: errorCount}
		result.Summary.TranslationUnits++
		result.Summary.Errors += errorCount
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
		}
	}
	result.Summary.UniqueErrors = len(result.Diagnostics)
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

// compileFile returns the error diagnostics and compiler failure status for one C file.
func compileFile(cc, sourceDir, file string) ([]diagnostic, bool, error) {
	cmd := exec.Command(cc,
		"-std=gnu11",
		"-fsyntax-only",
		"-fdiagnostics-format=json",
		"-fdiagnostics-color=never",
		"-fmax-errors=0",
		"-w",
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
		if item.Kind != "error" && item.Kind != "fatal error" {
			continue
		}
		result = append(result, convertDiagnostic(item))
	}
	if failed && len(result) == 0 {
		return nil, false, fmt.Errorf("compiler failed for %s without an error diagnostic", file)
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
