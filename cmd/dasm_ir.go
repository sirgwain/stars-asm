package cmd

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/spf13/cobra"
)

func newDasmIRCmd() *cobra.Command {
	return &cobra.Command{
		Use:   "ir",
		Short: "dump low-level IR as ugly C",
		Long:  `Run semantic analysis, lower SEM into C-like IR, and dump explicit-block ugly C`,
		RunE: func(cmd *cobra.Command, args []string) error {
			f := sdb.GetFunction(funcName)
			if f == nil {
				return fmt.Errorf("proc %q not found", funcName)
			}
			if err := stars.DumpFuncIR(cmd.OutOrStdout(), img, sdb, f, buildDasmDumpOptions(fromAddr, toAddr)); err != nil {
				return fmt.Errorf("dump ir: %v", err)
			}
			return nil
		},
	}
}
