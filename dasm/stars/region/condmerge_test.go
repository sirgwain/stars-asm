package region

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// TestBuildShortCircuitConditions verifies that each of the four ways a
// condition-only block can share a target with its predecessor merges into
// one && or || condition, and that chains keep merging.
func TestBuildShortCircuitConditions(t *testing.T) {
	const a, b, c, then, other, done machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60
	body := func(id machine.BlockID, v string) ir.Block {
		return testBlock(id, &ir.Assign{Dst: &ir.Var{Name: v}, Src: &ir.IntConst{Value: 1}}, &ir.Goto{Label: done.String()})
	}

	tests := []struct {
		name   string
		blocks []ir.Block
		want   string
	}{
		{
			name: "or: shared true target",
			blocks: []ir.Block{
				testBlock(a, testIfGoto("x", then, b)),
				testBlock(b, testIfGoto("y", then, other)),
				body(then, "t"), body(other, "o"),
				testBlock(done, &ir.Return{}),
			},
			want: "if x||y { t=1; } else { o=1; } return;",
		},
		{
			name: "and: shared false target",
			blocks: []ir.Block{
				testBlock(a, testIfGoto("x", b, other)),
				testBlock(b, testIfGoto("y", then, other)),
				body(then, "t"), body(other, "o"),
				testBlock(done, &ir.Return{}),
			},
			want: "if x&&y { t=1; } else { o=1; } return;",
		},
		{
			name: "and not: A's true target is B's false target",
			blocks: []ir.Block{
				testBlock(a, testIfGoto("x", other, b)),
				testBlock(b, testIfGoto("y", then, other)),
				body(then, "t"), body(other, "o"),
				testBlock(done, &ir.Return{}),
			},
			want: "if !x&&y { t=1; } else { o=1; } return;",
		},
		{
			name: "or not: A's false target is B's true target",
			blocks: []ir.Block{
				testBlock(a, testIfGoto("x", b, other)),
				testBlock(b, testIfGoto("y", other, then)),
				body(then, "t"), body(other, "o"),
				testBlock(done, &ir.Return{}),
			},
			want: "if !x||y { o=1; } else { t=1; } return;",
		},
		{
			name: "chain: (x && y) || z",
			blocks: []ir.Block{
				testBlock(a, testIfGoto("x", b, c)),
				testBlock(b, testIfGoto("y", then, c)),
				testBlock(c, testIfGoto("z", then, other)),
				body(then, "t"), body(other, "o"),
				testBlock(done, &ir.Return{}),
			},
			want: "if x&&y||z { t=1; } else { o=1; } return;",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := Build(ir.Func{Name: "Cond", Blocks: tt.blocks})
			if err != nil {
				t.Fatal(err)
			}
			if s := sketchNodes(got.Body); s != tt.want {
				t.Errorf("Build body =\n  %s\nwant\n  %s", s, tt.want)
			}
		})
	}
}
