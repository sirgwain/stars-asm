package region

import (
	"fmt"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// TestBuildSwitchReturningDefault verifies cases break to one shared
// continuation and an unmatched value returns without running that work.
func TestBuildSwitchReturningDefault(t *testing.T) {
	const top, next, last, a, b, c, continuation, done machine.BlockID = 0x10, 0x20, 0x25, 0x30, 0x40, 0x48, 0x50, 0x60
	x := &ir.Var{Name: "x"}
	test := func(k uint64, tl, fl machine.BlockID) *ir.IfGoto {
		return &ir.IfGoto{Cond: &ir.Binary{Op: "==", LHS: x, RHS: &ir.IntConst{Value: k}}, TrueLabel: tl.String(), FalseLabel: fl.String()}
	}
	fn := ir.Func{Name: "ReturningDefault", Blocks: []ir.Block{
		testBlock(top, test(1, a, next)),
		testBlock(next, test(2, b, last)),
		testBlock(last, test(3, c, done)),
		testBlock(a, testIncrement("a"), &ir.Goto{Label: continuation.String()}),
		testBlock(b, testIncrement("b"), &ir.Goto{Label: continuation.String()}),
		testBlock(c, testIncrement("c"), &ir.Goto{Label: continuation.String()}),
		testBlock(continuation, testIncrement("shared")),
		testBlock(done, &ir.Return{}),
	}}
	got, err := Build(fn)
	if err != nil {
		t.Fatal(err)
	}
	want := "switch x { case 1: a=a+1; break; case 2: b=b+1; break; case 3: c=c+1; break; default: return; } shared=shared+1; return;"
	if actual := sketchNodes(got.Body); actual != want {
		t.Fatalf("got %s; want %s", actual, want)
	}
}

// TestBuildIfElseThenLoop verifies that Build turns an if/else diamond and a
// while loop into structured nodes with no gotos or labels left.
func TestBuildIfElseThenLoop(t *testing.T) {
	const a, b, c, d, e, f machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60
	fn := ir.Func{Name: "Diamond", Blocks: []ir.Block{
		testBlock(a, testIfGoto("x", c, b)),
		testBlock(b, &ir.Assign{Dst: &ir.Var{Name: "y"}, Src: &ir.IntConst{Value: 1}}, &ir.Goto{Label: d.String()}),
		testBlock(c, &ir.Assign{Dst: &ir.Var{Name: "y"}, Src: &ir.IntConst{Value: 2}}),
		testBlock(d, testIfGoto("more", e, f)),
		testBlock(e, testIncrement("i"), &ir.Goto{Label: d.String()}),
		testBlock(f, &ir.Return{}),
	}}

	got, err := Build(fn)
	if err != nil {
		t.Fatal(err)
	}
	want := "if !x { y=1; } else { y=2; } while more { i=i+1; } return;"
	if s := sketchNodes(got.Body); s != want {
		t.Errorf("Build body =\n  %s\nwant\n  %s", s, want)
	}
}

// sketchNodes renders nodes as compact one-line text for test comparisons.
func sketchNodes(nodes []Node) string {
	var parts []string
	for _, n := range nodes {
		switch n := n.(type) {
		case *Label:
			parts = append(parts, n.Name+":")
		case *Basic:
			for _, s := range n.Stmts {
				parts = append(parts, sketchStmt(s))
			}
		case *Goto:
			parts = append(parts, "goto "+n.Label+";")
		case *Break:
			parts = append(parts, "break;")
		case *Continue:
			parts = append(parts, "continue;")
		case *If:
			s := fmt.Sprintf("if %s { %s }", sketchExpr(n.Cond), sketchNodes(n.Then))
			if len(n.Else) > 0 {
				s += fmt.Sprintf(" else { %s }", sketchNodes(n.Else))
			}
			parts = append(parts, s)
		case *Loop:
			switch n.Kind {
			case LoopWhile:
				parts = append(parts, fmt.Sprintf("while %s { %s }", sketchExpr(n.Cond), sketchNodes(n.Body)))
			case LoopDoWhile:
				parts = append(parts, fmt.Sprintf("do { %s } while %s;", sketchNodes(n.Body), sketchExpr(n.Cond)))
			case LoopFor:
				init := ""
				if n.Init != nil {
					init = sketchStmt(n.Init)
				}
				parts = append(parts, fmt.Sprintf("for (%s %s; %s) { %s }", init, sketchExpr(n.Cond), sketchStmt(n.Post), sketchNodes(n.Body)))
			default:
				parts = append(parts, fmt.Sprintf("while 1 { %s }", sketchNodes(n.Body)))
			}
		case *Switch:
			var cases []string
			for _, c := range n.Cases {
				for _, v := range c.Values {
					cases = append(cases, "case "+sketchExpr(v)+":")
				}
				if c.Default {
					cases = append(cases, "default:")
				}
				cases = append(cases, sketchNodes(c.Body))
			}
			parts = append(parts, fmt.Sprintf("switch %s { %s }", sketchExpr(n.Index), strings.Join(cases, " ")))
		default:
			parts = append(parts, fmt.Sprintf("%T", n))
		}
	}
	return strings.Join(parts, " ")
}

// sketchStmt renders the IR statements used by region test fixtures.
func sketchStmt(s ir.Stmt) string {
	switch s := s.(type) {
	case *ir.Assign:
		return sketchExpr(s.Dst) + "=" + sketchExpr(s.Src) + ";"
	case *ir.Return:
		return "return;"
	case *ir.ExprStmt:
		return sketchExpr(s.Expr) + ";"
	}
	return fmt.Sprintf("%T;", s)
}

// sketchExpr renders the IR expressions used by region test fixtures.
func sketchExpr(e ir.Expr) string {
	switch e := e.(type) {
	case *ir.Var:
		return e.Name
	case *ir.IntConst:
		return fmt.Sprint(e.Value)
	case *ir.Unary:
		return e.Op + sketchExpr(e.X)
	case *ir.Binary:
		return sketchExpr(e.LHS) + e.Op + sketchExpr(e.RHS)
	case *ir.Cond:
		return "(" + sketchExpr(e.Cond) + " ? " + sketchExpr(e.Then) + " : " + sketchExpr(e.Else) + ")"
	case *ir.Call:
		args := make([]string, len(e.Args))
		for i, a := range e.Args {
			args[i] = sketchExpr(a)
		}
		return sketchExpr(e.Target) + "(" + strings.Join(args, ", ") + ")"
	}
	return fmt.Sprintf("%T", e)
}

// TestBuildLoopForms verifies that while (1) loops become for loops, with
// jumps to the latch turned into continue, and do-while loops.
func TestBuildLoopForms(t *testing.T) {
	const entry, latch, head, body, c, d, join, done machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80
	const head2, body2, latch2 machine.BlockID = 0x90, 0xa0, 0xb0
	lessThan := &ir.Binary{Op: "<", LHS: &ir.Var{Name: "i"}, RHS: &ir.Var{Name: "n"}}

	tests := []struct {
		name   string
		blocks []ir.Block
		want   string
	}{
		{
			// The jump from c to the latch skips the join, so it becomes continue.
			name: "for with continue",
			blocks: []ir.Block{
				testBlock(entry, &ir.Assign{Dst: &ir.Var{Name: "i"}, Src: &ir.IntConst{Value: 0}}, &ir.Goto{Label: head.String()}),
				testBlock(latch, testIncrement("i")),
				testBlock(head, &ir.IfGoto{Cond: lessThan, TrueLabel: body.String(), FalseLabel: done.String()}),
				testBlock(body, testIfGoto("a", c, d)),
				testBlock(c, testIfGoto("x", latch, join)),
				testBlock(d, testIncrement("d"), &ir.Goto{Label: join.String()}),
				testBlock(join, testIncrement("j"), &ir.Goto{Label: latch.String()}),
				testBlock(done, &ir.Return{}),
			},
			want: "for (i=0; i<n; i=i+1;) { if !a { d=d+1; } else { if x { continue; } } j=j+1; } return;",
		},
		{
			// The jump to the latch from inside the nested loop keeps its
			// goto and the label, but the one at the outer loop's level still
			// becomes continue.
			name: "for with continue and a nested loop goto",
			blocks: []ir.Block{
				testBlock(entry, &ir.Assign{Dst: &ir.Var{Name: "i"}, Src: &ir.IntConst{Value: 0}}, &ir.Goto{Label: head.String()}),
				testBlock(latch, testIncrement("i")),
				testBlock(head, &ir.IfGoto{Cond: lessThan, TrueLabel: body.String(), FalseLabel: done.String()}),
				testBlock(body, testIfGoto("a", c, head2)),
				testBlock(c, testIfGoto("x", latch, join)),
				testBlock(head2, testIfGoto("y", latch, body2)),
				testBlock(body2, testIfGoto("z", join, latch2)),
				testBlock(latch2, testIncrement("k"), &ir.Goto{Label: head2.String()}),
				testBlock(join, testIncrement("j"), &ir.Goto{Label: latch.String()}),
				testBlock(done, &ir.Return{}),
			},
			want: "for (i=0; i<n; i=i+1;) { if a { if x { continue; } } else { while 1 { if y { goto L_0020; } if z { break; } k=k+1; } } j=j+1; L_0020: } return;",
		},
		{
			// The code after the loop only the found test reaches moves into
			// the loop, leaving the bound as the loop condition.
			name: "search loop returning what it found",
			blocks: []ir.Block{
				testBlock(entry, &ir.Assign{Dst: &ir.Var{Name: "i"}, Src: &ir.IntConst{Value: 0}}),
				testBlock(head, &ir.IfGoto{Cond: lessThan, TrueLabel: body.String(), FalseLabel: done.String()}),
				testBlock(body, testIfGoto("found", c, latch)),
				testBlock(latch, testIncrement("i"), &ir.Goto{Label: head.String()}),
				testBlock(c, &ir.Return{Value: &ir.Var{Name: "i"}}),
				testBlock(done, &ir.Return{Value: &ir.IntConst{Value: 0}}),
			},
			want: "for (i=0; i<n; i=i+1;) { if found { return; } } return;",
		},
		{
			name: "search loop falling into its exit",
			blocks: []ir.Block{
				testBlock(entry, &ir.Assign{Dst: &ir.Var{Name: "i"}, Src: &ir.IntConst{Value: 0}}),
				testBlock(head, &ir.IfGoto{Cond: lessThan, TrueLabel: body.String(), FalseLabel: done.String()}),
				testBlock(body, testIfGoto("found", c, latch)),
				testBlock(latch, testIncrement("i"), &ir.Goto{Label: head.String()}),
				testBlock(c, testIncrement("x")),
				testBlock(done, &ir.Return{}),
			},
			want: "for (i=0; i<n; i=i+1;) { if found { x=x+1; break; } } return;",
		},
		{
			// The found code after the loop's own return is reached only by
			// a goto from inside the loop, so it moves to that goto.
			name: "found code moved to its only goto",
			blocks: []ir.Block{
				testBlock(head, testIfGoto("more", body, done)),
				testBlock(body, testIfGoto("found", c, head)),
				testBlock(done, &ir.Return{}),
				testBlock(c, testIncrement("x"), &ir.Return{}),
			},
			want: "while more { if found { x=x+1; return; } } return;",
		},
		{
			// Loops with no break of their own, one per arm, sharing the
			// return after them: each leading test becomes the loop's break,
			// and the arms join into an if-else before the shared return.
			name: "search loops in both arms",
			blocks: []ir.Block{
				testBlock(entry, testIfGoto("sb", head, head2)),
				testBlock(head, &ir.IfGoto{Cond: lessThan, TrueLabel: body.String(), FalseLabel: done.String()}),
				testBlock(body, testIfGoto("f1", c, latch)),
				testBlock(latch, testIncrement("i"), &ir.Goto{Label: head.String()}),
				testBlock(head2, &ir.IfGoto{Cond: lessThan, TrueLabel: body2.String(), FalseLabel: done.String()}),
				testBlock(body2, testIfGoto("f2", d, latch2)),
				testBlock(latch2, testIncrement("i"), &ir.Goto{Label: head2.String()}),
				testBlock(c, &ir.Return{}),
				testBlock(d, &ir.Return{}),
				testBlock(done, &ir.Return{}),
			},
			want: "if sb { for ( i<n; i=i+1;) { if f1 { return; } } } else { for ( i<n; i=i+1;) { if f2 { return; } } } return;",
		},
		{
			name: "do while",
			blocks: []ir.Block{
				testBlock(entry, &ir.Assign{Dst: &ir.Var{Name: "i"}, Src: &ir.IntConst{Value: 0}}),
				testBlock(head, testIncrement("i"), testIfGoto("more", head, done)),
				testBlock(done, &ir.Return{}),
			},
			want: "i=0; do { i=i+1; } while more; return;",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := Build(ir.Func{Name: "Loops", Blocks: tt.blocks})
			if err != nil {
				t.Fatal(err)
			}
			if s := sketchNodes(got.Body); s != tt.want {
				t.Errorf("Build body =\n  %s\nwant\n  %s", s, tt.want)
			}
		})
	}
}

// TestBuildSwitch verifies that a jump table on (x - 1) * 2 becomes a switch on
// x with grouped case values, dominated destinations as case bodies, case
// fall-through, and break to the code after the switch.
func TestBuildSwitch(t *testing.T) {
	const top, a, b, c, done machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50
	index := &ir.Binary{Op: "*", LHS: &ir.Binary{Op: "-", LHS: &ir.Var{Name: "x"}, RHS: &ir.IntConst{Value: 1}}, RHS: &ir.IntConst{Value: 2}}
	fn := ir.Func{Name: "Switch", Blocks: []ir.Block{
		testBlock(top, &ir.TableJump{Index: index, Labels: []string{b.String(), a.String(), b.String(), c.String()}}),
		testBlock(a, testIncrement("a")),
		testBlock(b, testIncrement("b"), &ir.Goto{Label: done.String()}),
		testBlock(c, testIncrement("c"), &ir.Goto{Label: done.String()}),
		testBlock(done, &ir.Return{}),
	}}

	got, err := Build(fn)
	if err != nil {
		t.Fatal(err)
	}
	want := "switch x { case 2: a=a+1; case 1: case 3: b=b+1; break; case 4: c=c+1; } return;"
	if s := sketchNodes(got.Body); s != want {
		t.Errorf("Build body =\n  %s\nwant\n  %s", s, want)
	}
}

// TestBuildSwitchFromEqualityChain verifies that a chain of == and != tests on
// one expression becomes a switch with grouped values and a default.
func TestBuildSwitchFromEqualityChain(t *testing.T) {
	const t1, t2, t3, a, b, other, done machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70
	test := func(op string, k uint64, tl, fl machine.BlockID) *ir.IfGoto {
		cond := &ir.Binary{Op: op, LHS: &ir.Var{Name: "x"}, RHS: &ir.IntConst{Value: k}}
		return &ir.IfGoto{Cond: cond, TrueLabel: tl.String(), FalseLabel: fl.String()}
	}
	fn := ir.Func{Name: "Chain", Blocks: []ir.Block{
		testBlock(t1, test("==", 1, a, t2)),
		testBlock(t2, test("!=", 2, t3, b)),
		testBlock(t3, test("==", 3, a, other)),
		testBlock(a, testIncrement("a"), &ir.Goto{Label: done.String()}),
		testBlock(b, testIncrement("b"), &ir.Goto{Label: done.String()}),
		testBlock(other, testIncrement("o")),
		testBlock(done, &ir.Return{}),
	}}

	got, err := Build(fn)
	if err != nil {
		t.Fatal(err)
	}
	want := "switch x { case 1: case 3: a=a+1; break; case 2: b=b+1; break; default: o=o+1; } return;"
	if s := sketchNodes(got.Body); s != want {
		t.Errorf("Build body =\n  %s\nwant\n  %s", s, want)
	}
}

// TestBuildThreadsTrampolines verifies that jumps through a goto-only block
// and an empty block go straight to their final destination, which lets the
// two tests merge into one condition sharing the then target.
func TestBuildThreadsTrampolines(t *testing.T) {
	const top, b, hop, empty, then, other, done machine.BlockID = 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70
	fn := ir.Func{Name: "Thread", Blocks: []ir.Block{
		testBlock(top, testIfGoto("x", then, b)),
		testBlock(b, testIfGoto("y", hop, other)),
		testBlock(hop, &ir.Goto{Label: empty.String()}),
		testBlock(empty),
		testBlock(then, testIncrement("t"), &ir.Goto{Label: done.String()}),
		testBlock(other, testIncrement("o"), &ir.Goto{Label: done.String()}),
		testBlock(done, &ir.Return{}),
	}}

	got, err := Build(fn)
	if err != nil {
		t.Fatal(err)
	}
	want := "if x||y { t=t+1; } else { o=o+1; } return;"
	if s := sketchNodes(got.Body); s != want {
		t.Errorf("Build body =\n  %s\nwant\n  %s", s, want)
	}
}
