package stars

import (
	"bytes"
	"encoding/json"
	"fmt"
	"log/slog"
	"os"
	"path/filepath"
	"slices"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type DumpAllResult struct {
	OutDir    string
	Analysis  sem.AnalyzeResult            `json:"analysis,omitempty"`
	Functions map[string]sem.AnalyzeResult `json:"functions,omitempty"`
}

func DumpAll(img *asm.ImageNE, sdb *typeinfo.SymbolDB, opt DumpAllOptions) (DumpAllResult, error) {
	result := DumpAllResult{OutDir: opt.OutDir}

	if strings.TrimSpace(opt.OutDir) == "" {
		return DumpAllResult{}, fmt.Errorf("no output dir defined")
	}

	// make the base output dir
	if err := os.MkdirAll(opt.OutDir, 0o755); err != nil {
		return result, fmt.Errorf("mkdir %s: %w", opt.OutDir, err)
	}

	funcs := make([]*typeinfo.Function, 0, len(sdb.Functions))
	for _, f := range sdb.Functions {
		if f.IsOverride() {
			continue
		}
		funcs = append(funcs, f)
	}
	slices.SortFunc(funcs, func(a, b *typeinfo.Function) int {
		return strings.Compare(strings.ToLower(a.Name), strings.ToLower(b.Name))
	})

	result.Functions = make(map[string]sem.AnalyzeResult, len(funcs))
	funcAnalyses := make(map[string]FuncAnalysis, len(funcs))
	funcIRBodies := make(map[string]string, len(funcs))

	if opt.EmitStructs {
		structsPath := filepath.Join(opt.OutDir, "structs.h")
		structs, err := orderedDumpStructs(sdb)
		if err != nil {
			return DumpAllResult{}, err
		}

		f, err := os.Create(structsPath)
		if err != nil {
			return DumpAllResult{}, err
		}

		slog.Debug("Dumping structs.h", "path", structsPath, "count", len(structs))
		if err := templates.RenderDumpStructs(f, templates.NewDumpStructsView(structs, templates.DumpOptions{})); err != nil {
			f.Close()
			return DumpAllResult{}, err
		}
		if err := f.Close(); err != nil {
			return result, fmt.Errorf("close %s: %w", structsPath, err)
		}
	}

	// output globals, common, headers
	if opt.EmitC {
		// common.h
		commonPath := filepath.Join(opt.OutDir, "common.h")

		f, err := os.Create(commonPath)
		if err != nil {
			return DumpAllResult{}, err
		}

		slog.Debug("Dumping common.h", "path", commonPath)
		if err := templates.RenderCommon(f, templates.DumpCommonView{Modules: sdb.Modules}); err != nil {
			f.Close()
			return DumpAllResult{}, err
		}
		if err := f.Close(); err != nil {
			return result, fmt.Errorf("close %s: %w", commonPath, err)
		}

		// copy enums.h
		if err := copyFile("./dasm/input/enums.h", filepath.Join(opt.OutDir, "enums.h")); err != nil {
			return DumpAllResult{}, err
		}

		for _, module := range sdb.Modules {
			if module == typeinfo.OverrideModule {
				// don't output our override functions, those are just for better typing
				continue
			}
			// .h file
			path := filepath.Join(opt.OutDir, module+".h")
			globals := sdb.GetGlobalsForModule(module)
			functions := sdb.GetFunctionsForModule(module)
			slog.Debug("Dumping .h", "path", path, "globals", len(globals), "functions", len(functions))

			f, err := os.Create(path)
			if err != nil {
				return DumpAllResult{}, err
			}

			if module == typeinfo.GlobalsModule {
				// sort globals
				slices.SortFunc(globals, func(a, b *typeinfo.GlobalVar) int { return strings.Compare(a.CDecl(), b.CDecl()) })
			}

			if err := templates.RenderDumpHeader(f, templates.NewDumpSourceView(module, globals, functions)); err != nil {
				f.Close()
				return DumpAllResult{}, err
			}
			if err := f.Close(); err != nil {
				return result, fmt.Errorf("close %s: %w", path, err)
			}

			for _, g := range globals {
				si, ok := LoadGlobalInitializer(img, sdb, g)
				if ok {
					g.StaticInitializer = FormatInitializerC(sdb, si)
				}
			}

			if module == typeinfo.GlobalsModule {
				path = filepath.Join(opt.OutDir, module+".c")

				f, err = os.Create(path)
				if err != nil {
					return DumpAllResult{}, err
				}

				slog.Debug("Dumping globals .c", "path", path, "globals", len(globals))
				if err := templates.RenderDumpSource(f, templates.NewDumpSourceView(module, globals, nil)); err != nil {
					f.Close()
					return DumpAllResult{}, err
				}
				if err := f.Close(); err != nil {
					return result, fmt.Errorf("close %s: %w", path, err)
				}
			}
		}
	}

	asmOutDir := filepath.Join(opt.OutDir, "asm")
	if opt.EmitASM {
		if err := os.MkdirAll(asmOutDir, 0o755); err != nil {
			return result, fmt.Errorf("mkdir %s: %w", asmOutDir, err)
		}
	}

	effectsOutDir := filepath.Join(opt.OutDir, "effects")
	if opt.EmitEffects {
		if err := os.MkdirAll(effectsOutDir, 0o755); err != nil {
			return result, fmt.Errorf("mkdir %s: %w", effectsOutDir, err)
		}
	}

	semOutDir := filepath.Join(opt.OutDir, "sem")
	if opt.EmitSem {
		if err := os.MkdirAll(semOutDir, 0o755); err != nil {
			return result, fmt.Errorf("mkdir %s: %w", semOutDir, err)
		}
	}

	irOutDir := filepath.Join(opt.OutDir, "ir")
	if opt.EmitIR {
		if err := os.MkdirAll(irOutDir, 0o755); err != nil {
			return result, fmt.Errorf("mkdir %s: %w", irOutDir, err)
		}
	}

	emitFuncDumps := opt.EmitASM || opt.EmitEffects || opt.EmitSem || opt.EmitC || opt.EmitIR
	if emitFuncDumps {
		slog.Debug("Dumping all functions", "count", len(funcs), "outDir", opt.OutDir)
	}

	for _, function := range funcs {
		if !emitFuncDumps {
			continue
		}

		analysis, err := analyzeFunc(img, sdb, function, templates.DumpOptions{})
		if err != nil {
			return result, fmt.Errorf("analyze %s: %w", function.Name, err)
		}

		funcAnalyses[function.Name] = analysis
		result.Functions[function.Name] = analysis.SemAnalysis
		result.Analysis.Merges += analysis.SemAnalysis.Merges
		result.Analysis.Temps += analysis.SemAnalysis.Temps
		result.Analysis.FarPointers += analysis.SemAnalysis.FarPointers
		result.Analysis.HiWords += analysis.SemAnalysis.HiWords
		result.Analysis.LoWords += analysis.SemAnalysis.LoWords
		result.Analysis.BranchHiWords += analysis.SemAnalysis.BranchHiWords
		result.Analysis.BranchLoWords += analysis.SemAnalysis.BranchLoWords
		result.Analysis.DSRefs += analysis.SemAnalysis.DSRefs

		if opt.EmitASM {
			path := filepath.Join(asmOutDir, function.Name+".asm")
			f, err := os.Create(path)
			if err != nil {
				return DumpAllResult{}, err
			}

			slog.Debug("Dumping", "fs", function.Name, "path", path)
			if err := DumpFuncDetail(f, img, function, FuncDetailOptions{CommentStyle: FuncDetailCommentAsm}); err != nil {
				f.Close()
				return DumpAllResult{}, err
			}
			if err := renderFuncAsm(f, function, analysis, templates.DumpAsmOptions{}); err != nil {
				f.Close()
				return result, fmt.Errorf("dump asm %s: %w", function.Name, err)
			}
			if err := f.Close(); err != nil {
				return result, fmt.Errorf("close %s: %w", path, err)
			}
		}

		if opt.EmitEffects {
			path := filepath.Join(effectsOutDir, function.Name+".effect")
			f, err := os.Create(path)
			if err != nil {
				return DumpAllResult{}, err
			}

			slog.Debug("Dumping", "fs", function.Name, "path", path)
			if err := DumpFuncDetail(f, img, function, FuncDetailOptions{}); err != nil {
				f.Close()
				return DumpAllResult{}, err
			}
			if err := renderFuncEffects(f, function, analysis, templates.DumpEffectsOptions{ShowAsm: true}); err != nil {
				f.Close()
				return result, fmt.Errorf("dump effect %s: %w", function.Name, err)
			}
			if err := f.Close(); err != nil {
				return result, fmt.Errorf("close %s: %w", path, err)
			}
		}

		if opt.EmitSem {
			path := filepath.Join(semOutDir, function.Name+".sem")
			f, err := os.Create(path)
			if err != nil {
				return DumpAllResult{}, err
			}

			slog.Debug("Dumping", "fs", function.Name, "path", path)
			if err := DumpFuncDetail(f, img, function, FuncDetailOptions{}); err != nil {
				f.Close()
				return DumpAllResult{}, err
			}
			if err := renderFuncSem(f, function, analysis, templates.DumpSemOptions{}); err != nil {
				f.Close()
				return result, fmt.Errorf("dump sem %s: %w", function.Name, err)
			}
			if err := f.Close(); err != nil {
				return result, fmt.Errorf("close %s: %w", path, err)
			}
		}

		if opt.EmitIR || opt.EmitC {
			// record the IR body for later
			var buf bytes.Buffer
			if err := renderFuncIR(&buf, analysis, templates.DumpOptions{}); err != nil {
				return result, fmt.Errorf("dump ir %s: %w", function.Name, err)
			}
			funcIRBodies[function.Name] = buf.String()

			// don't dump individual IR files
			if !opt.EmitIR {
				slog.Debug("Rendering IR", "fs", function.Name)
				continue
			}
			path := filepath.Join(irOutDir, function.Name+".ir.c")
			f, err := os.Create(path)
			if err != nil {
				return DumpAllResult{}, err
			}

			slog.Debug("Dumping", "fs", function.Name, "path", path)
			if _, err := f.Write(buf.Bytes()); err != nil {
				f.Close()
				return result, fmt.Errorf("write %s: %w", path, err)
			}
			if err := f.Close(); err != nil {
				return result, fmt.Errorf("close %s: %w", path, err)
			}
		}
	}

	path := filepath.Join(opt.OutDir, "analysis.json")
	f, err := os.Create(path)
	if err != nil {
		return DumpAllResult{}, err
	}

	slog.Debug("Dumping analysis", "path", path)
	json := json.NewEncoder(f)
	json.SetIndent("", "  ")
	if err := json.Encode(result); err != nil {
		f.Close()
		return DumpAllResult{}, err
	}
	if err := f.Close(); err != nil {
		return result, fmt.Errorf("close %s: %w", path, err)
	}

	if opt.EmitC {
		for _, module := range sdb.Modules {
			if module == typeinfo.OverrideModule || module == typeinfo.GlobalsModule {
				continue
			}

			path := filepath.Join(opt.OutDir, module+".c")
			globals := sdb.GetGlobalsForModule(module)
			functions := sdb.GetFunctionsForModule(module)
			bodies, err := moduleIRBodies(functions, funcIRBodies)
			if err != nil {
				return DumpAllResult{}, err
			}

			f, err := os.Create(path)
			if err != nil {
				return DumpAllResult{}, err
			}

			slog.Debug("Dumping .c", "path", path, "globals", len(globals), "functions", len(functions))
			if err := templates.RenderDumpSource(f, templates.NewDumpSourceViewWithBodies(module, globals, functions, bodies)); err != nil {
				f.Close()
				return DumpAllResult{}, err
			}
			if err := f.Close(); err != nil {
				return result, fmt.Errorf("close %s: %w", path, err)
			}
		}
	}

	return result, nil
}

// moduleIRBodies returns rendered IR functions for a module source file.
func moduleIRBodies(functions []*typeinfo.Function, rendered map[string]string) (map[string]string, error) {
	bodies := make(map[string]string, len(functions))
	for _, function := range functions {
		body, ok := rendered[function.Name]
		if !ok {
			return nil, fmt.Errorf("missing rendered ir for %s", function.Name)
		}
		bodies[function.Name] = body
	}
	return bodies, nil
}

func copyFile(src, dst string) error {
	data, err := os.ReadFile(src)
	if err != nil {
		return err
	}
	return os.WriteFile(dst, data, 0o644)
}
