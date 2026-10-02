package stars

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestNativeWin16LayoutRepairs checks that reads through an index that may be
// -1 take the value the Win16 layout placed before the array, that guarded
// reads are left alone, and that Macinti's late-game splits use a 16-entry
// array.
func TestNativeWin16LayoutRepairs(t *testing.T) {
	fx := testfixture.Stars(t)
	writes := machine.NewWriteSummaries(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB))
	for name, wants := range map[string][]string{
		"DoMacintiAiTurn": {
			"branch (iLatestMiner != -1 ? rgshdef[iLatestMiner].cExist : (uint32_t)((uint32_t)(uint16_t)vtimer.mdForce | ((uint32_t)(uint16_t)vtimer.fAutoGenWhenIn << 16))) < 7500",
			"branch rgshdef[iLatestMiner].cExist < 5000",
			"branch (iLatestDestroyer != -1 ? lpfl->rgcsh[iLatestDestroyer] : lpfl->pt.y) < 20",
			"call memset(rgSplitShdef, 0, 16)",
			"rgSplitShdef[14] = 2",
			"rgSplitShdef[10] = 2",
		},
		"DoRobotoidAiTurn": {
			"branch (iLatestDestroyer != -1 ? lpfl->rgcsh[iLatestDestroyer] : lpfl->pt.y) < 20",
		},
	} {
		analysis, err := analyzeFunc(fx.Image, fx.SDB, fx.SDB.GetFunction(name), DumpOptions{}, writes)
		if err != nil {
			t.Fatal(err)
		}
		var lines []string
		for _, block := range analysis.Sem.Blocks {
			for _, effect := range block.Effects {
				lines = append(lines, sem.FormatEffect(effect))
			}
		}
		text := strings.Join(lines, "\n")
		for _, want := range wants {
			if !strings.Contains(text, want) {
				t.Errorf("%s: missing %q", name, want)
			}
		}
	}
}
