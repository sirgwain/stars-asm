package cmd

import (
	"bytes"
	"fmt"
	"os"
	"os/exec"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/sirgwain/stars-asm/dasm/stars/graphview"
	"github.com/spf13/cobra"
)

func newDasmGraphCmd() *cobra.Command {
	var view bool
	cmd := &cobra.Command{
		Use:   "graph",
		Short: "dump cfg graph SCCs",
		Long:  `Dump CFG graph strongly connected components for a function`,
		RunE: func(cmd *cobra.Command, args []string) error {
			var buf bytes.Buffer

			f := sdb.GetFunction(funcName)
			if f == nil {
				return fmt.Errorf("proc %q not found", funcName)
			}

			opt := buildDasmDumpOptions(fromAddr, toAddr)

			// run a wails view for prettier graph navigating
			if view {
				if !graphview.ProductionBuild() && os.Getenv("STARS_ASM_WAILS_PRODUCTION") != "1" {
					args := append([]string{"run", "-tags", "production", "."}, os.Args[1:]...)
					runCmd := exec.Command("go", args...)
					runCmd.Env = append(os.Environ(), "STARS_ASM_WAILS_PRODUCTION=1")
					runCmd.Stdin = os.Stdin
					runCmd.Stdout = os.Stdout
					runCmd.Stderr = os.Stderr
					return runCmd.Run()
				}
				data, err := stars.BuildFuncGraphViewData(img, sdb, f, opt)
				if err != nil {
					return fmt.Errorf("build graph: %v", err)
				}
				return graphview.Run(data)
			}

			if err := stars.DumpFuncDetail(&buf, img, f, stars.FuncDetailOptions{}); err != nil {
				return err
			}

			if err := stars.DumpFuncGraph(&buf, img, sdb, f, opt); err != nil {
				return fmt.Errorf("dump graph: %v", err)
			}

			fmt.Fprint(cmd.OutOrStdout(), buf.String())

			return nil
		},
	}

	cmd.Flags().BoolVar(&view, "view", false, "launch a Wails Cytoscape graph viewer")

	return cmd
}
