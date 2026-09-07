package cmd

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/spf13/cobra"
)

func newDasmSemCmd() *cobra.Command {
	var showSem bool
	var showAsm bool
	var showEffects bool
	var analyze bool
	var diff bool
	var diffPasses []string

	cmd := &cobra.Command{
		Use:   "sem",
		Short: "dump semantic effects",
		Long:  `Dump lowered semantic effects for a function`,
		RunE: func(cmd *cobra.Command, args []string) error {
			f := sdb.GetFunction(funcName)
			if f == nil {
				return fmt.Errorf("proc %q not found", funcName)
			}

			opt := buildDasmDumpOptions(fromAddr, toAddr)

			headerOpt := stars.FuncDetailOptions{}
			if showAsm {
				headerOpt.CommentStyle = stars.FuncDetailCommentAsm
			}
			headerOpt.ShowColor = showColor
			if err := stars.DumpFuncDetail(cmd.OutOrStdout(), img, f, headerOpt); err != nil {
				return err
			}
			fmt.Fprintln(cmd.OutOrStdout())

			if showSem {
				diffDir := ""
				if diff {
					diffDir = "dist"
				}
				if err := stars.DumpFuncSem(cmd.OutOrStdout(), img, sdb, f, stars.DumpSemOptions{
					DumpOptions: opt,
					ShowAsm:     showAsm,
					ShowEffects: showEffects,
					ShowOffsets: showAsm,
					Analyze:     analyze,
					DiffDir:     diffDir,
					DiffPasses:  diffPasses,
				}); err != nil {
					return fmt.Errorf("dump sem: %v", err)
				}
				fmt.Fprintln(cmd.OutOrStdout())
			}

			return nil
		},
	}

	cmd.Flags().BoolVar(&showSem, "sem", true, "show semantic effects")
	cmd.Flags().BoolVar(&showAsm, "asm", false, "show asm before semantic effects")
	cmd.Flags().BoolVar(&showEffects, "effects", false, "show machine effects before semantic effects")
	cmd.Flags().BoolVar(&analyze, "analyze", false, "output semantic analysis as JSON")
	cmd.Flags().BoolVar(&diff, "diff", false, "write per-pass semantic dumps to dist and show changed pass diffs")
	cmd.Flags().StringSliceVar(&diffPasses, "diff-passes", nil, "only show diffs for the named semantic passes")

	return cmd
}
