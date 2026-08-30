package cmd

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/spf13/cobra"
)

func newDasmEffectsCmd() *cobra.Command {
	var showEffects bool
	var showAsm bool
	var showCfg bool
	var diff bool
	var diffState bool

	cmd := &cobra.Command{
		Use:   "effects",
		Short: "dump effects",
		Long:  `Dump the effects of asm for a function`,
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

			if showEffects {
				if err := stars.DumpFuncEffects(cmd.OutOrStdout(), img, sdb, f, stars.DumpEffectsOptions{
					DumpOptions: opt,
					Config:      showCfg,
					ShowAsm:     showAsm,
					ShowOffsets: showAsm,
					DiffState:   diffState,
				}); err != nil {
					return fmt.Errorf("dump effects: %v", err)
				}
				fmt.Fprintln(cmd.OutOrStdout())
			}

			return nil
		},
	}

	cmd.Flags().BoolVar(&showEffects, "effects", true, "show effects")
	cmd.Flags().BoolVar(&showAsm, "asm", true, "show asm before effects")
	cmd.Flags().BoolVar(&showCfg, "cfg", false, "lower effects to sem FuncEvents and print ugly IR C")
	cmd.Flags().BoolVar(&diff, "diff", false, "emit shell diff between consecutive effect processor states")
	cmd.Flags().BoolVar(&diffState, "diff-state", false, "emit per-block machine state in/out diffs")

	return cmd
}
