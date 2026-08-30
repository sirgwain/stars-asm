package cmd

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/spf13/cobra"
)

func newDasmAllCmd() *cobra.Command {
	var outDir string
	var emitAll bool
	var emitC bool
	var emitIR bool
	var emitSem bool
	var emitEffects bool
	var emitAsm bool
	var emitStructs bool
	var verbose bool

	cmd := &cobra.Command{
		Use:   "all",
		Short: "Decompile all user C functions into generated source files",
		Long:  `Decompile all user C functions and emit grouped .c/.h files plus globals/stars headers.`,
		RunE: func(cmd *cobra.Command, args []string) error {

			if emitAll {
				emitC = true
				emitIR = true
				emitSem = true
				emitEffects = true
				emitAsm = true
				emitStructs = true
			}

			report, err := stars.DumpAll(img, sdb, stars.DumpAllOptions{
				OutDir:      outDir,
				EmitC:       emitC,
				EmitIR:      emitIR,
				EmitSem:     emitSem,
				EmitEffects: emitEffects,
				EmitASM:     emitAsm,
				EmitStructs: emitStructs,
				Verbose:     verbose,
			})

			fmt.Printf(`
Analysis: 
	Remaining Merges: %d
	Remaining FarPointers: %d
	Remaining HIWORDS: %d
`,
				report.Analysis.Merges,
				report.Analysis.FarPointers,
				report.Analysis.HiWords)
			if err != nil {
				return err
			}
			return nil
		},
	}

	cmd.Flags().StringVar(&outDir, "out", "./decompiled", "output directory")
	cmd.Flags().BoolVar(&emitAll, "all", false, "write all files (c, sem, effect, etc)")
	cmd.Flags().BoolVar(&emitC, "c", false, "write c files (<out>/*.h, *.c, globals.h, etc)")
	cmd.Flags().BoolVar(&emitIR, "ir", false, "write ir.c files (<out>/ir/*.ir.c)")
	cmd.Flags().BoolVar(&emitSem, "sem", false, "write per-function sem files (<out>/sem/<F>.sem)")
	cmd.Flags().BoolVar(&emitEffects, "effects", false, "write per-function effect files (<out>/effects/<F>.effect)")
	cmd.Flags().BoolVar(&emitAsm, "asm", false, "write per-function ASM files (<out>/asm/<F>.asm)")
	cmd.Flags().BoolVar(&emitStructs, "structs", false, "write structs.h file to <out>/structs.h")
	cmd.Flags().BoolVarP(&verbose, "verbose", "v", false, "print each function and its decompile time as it completes")

	// must have at least some output
	cmd.MarkFlagsOneRequired("asm", "structs", "effects", "sem", "ir", "c", "all")
	return cmd
}
