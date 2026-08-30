package cmd

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/spf13/cobra"
)

func newDasmStructCmd() *cobra.Command {
	var structName string
	var all bool

	cmd := &cobra.Command{
		Use:   "struct",
		Short: "dump struct",
		Long:  `Dump a C-style declaration for a struct`,
		RunE: func(cmd *cobra.Command, args []string) error {
			opt := buildDasmDumpOptions(0, 0)

			// dump all
			if all {
				if err := stars.DumpAllStructs(cmd.OutOrStdout(), sdb, opt); err != nil {
					return err
				}
				return nil
			}

			s := sdb.GetStruct(structName)
			if s == nil {
				return fmt.Errorf("struct %q not found", structName)
			}

			if err := stars.DumpStruct(cmd.OutOrStdout(), s, opt); err != nil {
				return err
			}
			return nil
		},
	}

	cmd.Flags().StringVar(&structName, "struct", "", "the struct to dump")
	cmd.Flags().BoolVar(&all, "all", false, "dump all structs")
	cmd.MarkFlagsMutuallyExclusive("struct", "all")
	cmd.MarkFlagsOneRequired("struct", "all")
	return cmd
}
