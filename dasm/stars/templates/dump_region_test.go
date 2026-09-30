package templates

import (
	"strconv"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/region"
)

// TestWriteRegionSwitchTrailingCases verifies that empty trailing cases are
// left out only when no default would catch their values instead; with a
// default earlier in the switch they are kept and end with a break.
func TestWriteRegionSwitchTrailingCases(t *testing.T) {
	step := &region.Basic{Stmts: []ir.Stmt{&ir.Assign{Dst: &ir.Var{Name: "i"}, Src: &ir.IntConst{Value: 1, Text: "1"}}}}
	value := func(v uint64) ir.Expr { return &ir.IntConst{Value: v, Text: strconv.FormatUint(v, 10)} }

	for _, tc := range []struct {
		name  string
		cases []region.Case
		want  []string
		skip  []string
	}{
		{
			name: "default before the empty cases",
			cases: []region.Case{
				{Default: true, Body: []region.Node{step}},
				{Values: []ir.Expr{value(0), value(10)}},
			},
			want: []string{"default:", "case 0:", "case 10:", "break;"},
		},
		{
			name: "no default left",
			cases: []region.Case{
				{Values: []ir.Expr{value(1)}, Body: []region.Node{step}},
				{Values: []ir.Expr{value(2)}, Default: true},
			},
			want: []string{"case 1:"},
			skip: []string{"case 2:", "default:"},
		},
	} {
		t.Run(tc.name, func(t *testing.T) {
			var w strings.Builder
			writeRegionNodes(&w, []region.Node{&region.Switch{Index: &ir.Var{Name: "x"}, Cases: tc.cases}}, 1)
			got := w.String()
			for _, s := range tc.want {
				if !strings.Contains(got, s) {
					t.Errorf("switch output missing %q:\n%s", s, got)
				}
			}
			for _, s := range tc.skip {
				if strings.Contains(got, s) {
					t.Errorf("switch output has %q:\n%s", s, got)
				}
			}
		})
	}
}
