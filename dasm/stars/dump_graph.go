package stars

import (
	"fmt"
	"io"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpFuncGraph disassembles a function, builds its CFG graph, and prints SCCs.
func DumpFuncGraph(w io.Writer, img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpOptions) error {
	data, err := BuildFuncGraphViewData(img, sdb, fs, opt)
	if err != nil {
		return err
	}

	fmt.Fprintf(w, "SCCs: %d\n", len(data.SCCs))
	for i, scc := range data.SCCs {
		labels := make([]string, 0, len(scc))
		for _, id := range scc {
			labels = append(labels, graphNodeLabel(data, id))
		}
		fmt.Fprintf(w, "scc %02d: %s\n", i, strings.Join(labels, " -> "))
	}

	return nil
}

func graphNodeLabel(data *FuncGraphViewData, id string) string {
	for _, node := range data.Nodes {
		if node.ID == id {
			return node.Label
		}
	}
	return id
}
