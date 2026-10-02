package region

import (
	"slices"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// TestBuildFlipsGuards verifies when a short arm that always jumps, or
// that falls into the return, is put first, as a guard or as the first
// link of an else-if chain, and when the compiler's layout keeps the
// source's order instead.
func TestBuildFlipsGuards(t *testing.T) {
	const a, b, c, ha, hb, tail, big, small machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80
	set := func(v string) *ir.Assign {
		return &ir.Assign{Dst: &ir.Var{Name: v}, Src: &ir.IntConst{Value: 1}}
	}

	tests := []struct {
		name   string
		blocks []ir.Block
		want   string
	}{
		{
			// The long path falls through from the test, so the source wrote
			// it first; a small body is not worth reordering.
			name: "laid-out if keeps the source order",
			blocks: []ir.Block{
				testBlock(a, testIfGoto("c", small, big)),
				testBlock(big, set("x"), set("y"), set("z"), &ir.Return{}),
				testBlock(small, &ir.Return{}),
			},
			want: "if !c { x=1; y=1; z=1; return; } return;",
		},
		{
			// A laid-out Then that is a large body is un-nested anyway.
			name: "large laid-out body un-nested",
			blocks: []ir.Block{
				testBlock(a, testIfGoto("c", small, big)),
				testBlock(big, append(slices.Repeat([]ir.Stmt{set("x")}, 30), &ir.Return{})...),
				testBlock(small, &ir.Return{}),
			},
			want: "if c { return; } " + strings.Repeat("x=1; ", 30) + "return;",
		},
		{
			// A short Else falling into the return takes its own copy of it,
			// so the large Then is no longer nested.
			name: "short else guards the return tail",
			blocks: []ir.Block{
				testBlock(a, testIfGoto("c", big, small)),
				testBlock(big, append(slices.Repeat([]ir.Stmt{set("x")}, 30), &ir.Goto{Label: tail.String()})...),
				testBlock(small, set("y"), &ir.Goto{Label: tail.String()}),
				testBlock(tail, &ir.Return{}),
			},
			want: "if !c { y=1; return; } " + strings.Repeat("x=1; ", 30) + "return;",
		},
		{
			// Each "not this case" test falls through to the next one.
			name: "not-this-case chain",
			blocks: []ir.Block{
				testBlock(a, testIfGoto("isA", ha, b)),
				testBlock(b, testIfGoto("isB", hb, c)),
				testBlock(c, set("z"), &ir.Goto{Label: tail.String()}),
				testBlock(ha, set("x"), &ir.Goto{Label: tail.String()}),
				testBlock(hb, set("y"), &ir.Goto{Label: tail.String()}),
				testBlock(tail, &ir.Return{}),
			},
			want: "if isA { x=1; } else { if !isB { z=1; } else { y=1; } } return;",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := Build(ir.Func{Name: "Guards", Blocks: tt.blocks})
			if err != nil {
				t.Fatal(err)
			}
			if s := sketchNodes(got.Body); s != tt.want {
				t.Errorf("Build body =\n  %s\nwant\n  %s", s, tt.want)
			}
		})
	}
}
