package ir

import "testing"

// TestAnalyzeCountsNonControlFlowStatements verifies control-flow statements
// and block labels are excluded from the statement count.
func TestAnalyzeCountsNonControlFlowStatements(t *testing.T) {
	fn := Func{
		Blocks: []Block{{
			Label: "L_1000",
			Stmts: []Stmt{
				&Assign{Dst: &Var{Name: "value"}, Src: &IntConst{Value: 1}},
				&IfGoto{Cond: &Var{Name: "condition"}, TrueLabel: "L_1010", FalseLabel: "L_1020"},
				&TableJump{Index: &Var{Name: "index"}, Labels: []string{"L_1010", "L_1020"}},
				&Goto{Label: "L_1030"},
				&ExprStmt{Expr: &Call{Target: &Var{Name: "record"}}},
				&Return{},
				&Comment{Text: "unsupported operation"},
			},
		}},
	}

	if got, want := fn.Analyze().NumStatements, 4; got != want {
		t.Fatalf("NumStatements = %d, want %d", got, want)
	}
}
