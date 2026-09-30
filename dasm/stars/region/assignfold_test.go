package region

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestBuildFoldsAssignments verifies that if-else chains that only give one
// integer local a merged value fold into a conditional expression, 1-or-0
// arms into the test itself, and a folded merge temp into its single use,
// while separate stores and arms that update the local stay statements.
func TestBuildFoldsAssignments(t *testing.T) {
	const a, b, c, d, done machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50
	less := &ir.Binary{Op: "<", LHS: &ir.Var{Name: "x"}, RHS: &ir.Var{Name: "n"}}
	// set is a store sem made on one edge in place of a merge temp.
	set := func(v string, value uint64) *ir.Assign {
		return &ir.Assign{Dst: &ir.Var{Name: v}, Src: &ir.IntConst{Value: value}, Merge: true}
	}
	jump := &ir.Goto{Label: done.String()}
	i16 := []ir.Local{{Name: "v", Type: typeinfo.I16}, {Name: "t_merge_0050_0001", Type: typeinfo.I16}}

	tests := []struct {
		name   string
		locals []ir.Local
		blocks []ir.Block
		want   string
	}{
		{
			name:   "1 or 0 folds to the test",
			locals: i16,
			blocks: []ir.Block{
				testBlock(a, &ir.IfGoto{Cond: less, TrueLabel: c.String(), FalseLabel: b.String()}),
				testBlock(b, set("v", 0), jump),
				testBlock(c, set("v", 1)),
				testBlock(done, &ir.Return{}),
			},
			want: "v=x<n; return;",
		},
		{
			name:   "else-if chain folds to nested conditionals",
			locals: i16,
			blocks: []ir.Block{
				testBlock(a, testIfGoto("p", b, c)),
				testBlock(b, set("v", 5), jump),
				testBlock(c, testIfGoto("q", d, 0x60)),
				testBlock(d, set("v", 6), jump),
				testBlock(0x60, set("v", 7)),
				testBlock(done, &ir.Return{}),
			},
			want: "v=(p ? 5 : (q ? 6 : 7)); return;",
		},
		{
			// Stores the compiler made on each path are the source's own
			// if-else, not a conditional expression.
			name:   "separate stores not folded",
			locals: i16,
			blocks: []ir.Block{
				testBlock(a, testIfGoto("p", b, c)),
				testBlock(b, &ir.Assign{Dst: &ir.Var{Name: "v"}, Src: &ir.IntConst{Value: 5}}, jump),
				testBlock(c, &ir.Assign{Dst: &ir.Var{Name: "v"}, Src: &ir.IntConst{Value: 7}}),
				testBlock(done, &ir.Return{}),
			},
			want: "if p { v=5; } else { v=7; } return;",
		},
		{
			name:   "double local not folded",
			locals: []ir.Local{{Name: "v", Type: typeinfo.Double}},
			blocks: []ir.Block{
				testBlock(a, testIfGoto("p", b, c)),
				testBlock(b, set("v", 5), jump),
				testBlock(c, set("v", 7)),
				testBlock(done, &ir.Return{}),
			},
			want: "if p { v=5; } else { v=7; } return;",
		},
		{
			name:   "updates of the local not folded",
			locals: i16,
			blocks: []ir.Block{
				testBlock(a, testIfGoto("p", b, c)),
				testBlock(b, &ir.Assign{Dst: &ir.Var{Name: "v"}, Src: &ir.Binary{Op: "|", LHS: &ir.Var{Name: "v"}, RHS: &ir.IntConst{Value: 8}}, Merge: true}, jump),
				testBlock(c, &ir.Assign{Dst: &ir.Var{Name: "v"}, Src: &ir.Binary{Op: "|", LHS: &ir.Var{Name: "v"}, RHS: &ir.IntConst{Value: 4}}, Merge: true}),
				testBlock(done, &ir.Return{}),
			},
			want: "if p { v=v|8; } else { v=v|4; } return;",
		},
		{
			name:   "merge temp forwarded into its call",
			locals: i16,
			blocks: []ir.Block{
				testBlock(a, &ir.IfGoto{Cond: less, TrueLabel: c.String(), FalseLabel: b.String()}),
				testBlock(b, set("t_merge_0050_0001", 0), jump),
				testBlock(c, set("t_merge_0050_0001", 1)),
				testBlock(done, &ir.ExprStmt{Expr: &ir.Call{Target: &ir.Var{Name: "Enable"}, Args: []ir.Expr{&ir.Var{Name: "t_merge_0050_0001"}}}}, &ir.Return{}),
			},
			want: "Enable(x<n); return;",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := Build(ir.Func{Name: "Fold", Locals: tt.locals, Blocks: tt.blocks})
			if err != nil {
				t.Fatal(err)
			}
			if s := sketchNodes(got.Body); s != tt.want {
				t.Errorf("Build body =\n  %s\nwant\n  %s", s, tt.want)
			}
		})
	}
}
