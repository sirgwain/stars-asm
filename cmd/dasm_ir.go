package cmd

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/spf13/cobra"
)

func newDasmIRCmd() *cobra.Command {
	var showSem bool
	var showAsm bool
	var showEffects bool
	var showIR bool

	cmd := &cobra.Command{
		Use:   "ir",
		Short: "dump low-level IR as ugly C",
		Long:  `Run semantic analysis, lower SEM into C-like IR, and dump explicit-block ugly C`,
		RunE: func(cmd *cobra.Command, args []string) error {
			f := sdb.GetFunction(funcName)
			if f == nil {
				return fmt.Errorf("proc %q not found", funcName)
			}
			opt := buildDasmDumpOptions(fromAddr, toAddr)

			if err := stars.DumpFuncIR(cmd.OutOrStdout(), img, sdb, f, stars.DumpIROptions{
				DumpOptions: opt,
				ShowAsm:     showAsm,
				ShowEffects: showEffects,
				ShowSem:     showSem,
				ShowIR:      showIR,
			}); err != nil {
				return fmt.Errorf("dump ir: %v", err)
			}
			return nil
		},
	}

	cmd.Flags().BoolVar(&showIR, "ir", true, "show ir statements")
	cmd.Flags().BoolVar(&showSem, "sem", false, "show semantic effects")
	cmd.Flags().BoolVar(&showAsm, "asm", false, "show asm before semantic effects")
	cmd.Flags().BoolVar(&showEffects, "effects", false, "show machine effects before semantic effects")

	return cmd
}
