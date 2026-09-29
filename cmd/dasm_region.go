package cmd

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/spf13/cobra"
)

// newDasmRegionCmd creates the command that dumps a function's IR with its
// control flow rebuilt as structured regions.
func newDasmRegionCmd() *cobra.Command {
	return &cobra.Command{
		Use:   "region",
		Short: "dump IR with structured control flow",
		Long:  `Lower a function to IR, rebuild its control flow as structured regions, and dump it as C`,
		RunE: func(cmd *cobra.Command, args []string) error {
			f := sdb.GetFunction(funcName)
			if f == nil {
				return fmt.Errorf("proc %q not found", funcName)
			}
			if err := stars.DumpFuncRegion(cmd.OutOrStdout(), img, sdb, f, stars.DumpOptions{ShowColor: showColor}); err != nil {
				return fmt.Errorf("dump region: %v", err)
			}
			return nil
		},
	}
}
