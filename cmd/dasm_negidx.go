package cmd

import (
	"fmt"
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/spf13/cobra"
)

// newDasmNegIdxCmd reports array accesses that may index with -1 and the
// may_be_minus_one semantics derived for them.
func newDasmNegIdxCmd() *cobra.Command {
	return &cobra.Command{
		Use:   "negidx",
		Short: "Find array accesses that may index with -1",
		Long: `Run the -1 index dataflow over every function, deriving may_be_minus_one
return and out-parameter semantics to a fixpoint, and list the accesses that
may still index with -1. Derived semantics belong in overrides-semantics.json.`,
		RunE: func(cmd *cobra.Command, args []string) error {
			audit, err := stars.AuditNegativeIndexes(img, sdb)
			if err != nil {
				return err
			}
			out := cmd.OutOrStdout()
			fmt.Fprintf(out, "Derived return semantics (%d):\n", len(audit.Returns))
			for _, name := range audit.Returns {
				fmt.Fprintf(out, "  %s\n", name)
			}
			names := make([]string, 0, len(audit.Stores))
			for name := range audit.Stores {
				names = append(names, name)
			}
			slices.Sort(names)
			fmt.Fprintf(out, "Derived out-parameter semantics (%d):\n", len(names))
			for _, name := range names {
				fmt.Fprintf(out, "  %s %v\n", name, audit.Stores[name])
			}
			fmt.Fprintf(out, "Sites (%d):\n", len(audit.Sites))
			for _, site := range audit.Sites {
				status := "repaired by native-negative-index"
				if reason, ok := sem.NegativeIndexAudited[site.Function][site.Index]; ok {
					status = "checked: " + reason
				}
				fmt.Fprintf(out, "  %s %s [%s] %s\n      %s\n      %s\n", site.Function, site.Block, site.Index, site.Access, site.Effect, status)
			}
			return nil
		},
	}
}
