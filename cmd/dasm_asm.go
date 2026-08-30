package cmd

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/spf13/cobra"
)

func newDasmAsmCmd() *cobra.Command {
	var showBytes bool
	var showOffsets bool
	var rawOperands bool

	cmd := &cobra.Command{
		Use:   "asm",
		Short: "dump asm",
		Long:  `Dump the assembly for a function`,
		RunE: func(cmd *cobra.Command, args []string) error {

			f := sdb.GetFunction(funcName)
			if f == nil {
				return fmt.Errorf("proc %q not found", funcName)
			}

			opt := stars.DumpAsmOptions{
				DumpOptions: buildDasmDumpOptions(fromAddr, toAddr),
				ShowBytes:   showBytes,
				ShowOffsets: showOffsets,
				RawOperands: rawOperands,
			}
			opt.DumpOptions.PreserveJumps = rawOperands

			if err := stars.DumpFuncDetail(cmd.OutOrStdout(), img, f, stars.FuncDetailOptions{
				CommentStyle: stars.FuncDetailCommentAsm,
				ShowColor:    showColor,
			}); err != nil {
				return err
			}

			if err := stars.DumpFuncAsm(cmd.OutOrStdout(), img, sdb, f, opt); err != nil {
				return fmt.Errorf("dump asm: %v", err)
			}

			return nil
		},
	}

	cmd.Flags().BoolVar(&showBytes, "show-bytes", false, "show raw instruction bytes in ASM output")
	cmd.Flags().BoolVar(&showOffsets, "show-offsets", false, "show instruction offsets in ASM output")
	cmd.Flags().BoolVar(&rawOperands, "raw", false, "show raw operands")

	return cmd
}
