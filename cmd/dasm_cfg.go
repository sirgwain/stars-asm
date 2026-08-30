package cmd

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/spf13/cobra"
)

func newDasmCfgCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "cfg",
		Short: "dump cfg",
		Long:  `Dump CFG block state for a function`,
		RunE: func(cmd *cobra.Command, args []string) error {

			f := sdb.GetFunction(funcName)
			if f == nil {
				return fmt.Errorf("proc %q not found", funcName)
			}

			if err := stars.DumpFuncDetail(cmd.OutOrStdout(), img, f, stars.FuncDetailOptions{
				CommentStyle: stars.FuncDetailCommentAsm,
				ShowColor:    showColor,
			}); err != nil {
				return err
			}

			if err := stars.DumpFuncCFG(cmd.OutOrStdout(), img, sdb, f, stars.DumpCFGOptions{
				DumpOptions: buildDasmDumpOptions(fromAddr, toAddr),
			}); err != nil {
				return fmt.Errorf("dump cfg: %v", err)
			}

			return nil
		},
	}

	return cmd
}
