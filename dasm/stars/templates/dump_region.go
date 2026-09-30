package templates

import (
	"fmt"
	"io"
	"slices"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/region"
)

// RenderDumpRegion renders a structured region function as formatted C.
func RenderDumpRegion(w io.Writer, fn region.Func, opt DumpOptions) error {
	var src strings.Builder
	src.WriteString(fn.Decl + " {\n")
	src.WriteString(renderIRLocals(uniqueIRLocals(fn.Locals)))
	writeRegionNodes(&src, fn.Body, 1)
	src.WriteString("}\n")

	formatted, err := formatCSource(src.String())
	if err != nil {
		return err
	}
	printHighlightedC(w, formatted, opt.ShowColor)
	return nil
}

// writeRegionNodes writes nodes as C statements indented to depth.
func writeRegionNodes(w *strings.Builder, nodes []region.Node, depth int) {
	indent := strings.Repeat("\t", depth)
	for i, n := range nodes {
		switch n := n.(type) {
		case *region.Label:
			// A label must be followed by a statement.
			if i == len(nodes)-1 {
				fmt.Fprintf(w, "%s:;\n", n.Name)
			} else {
				fmt.Fprintf(w, "%s:\n", n.Name)
			}
		case *region.Basic:
			for _, stmt := range n.Stmts {
				fmt.Fprintf(w, "%s%s\n", indent, formatIRStmt(stmt))
			}
		case *region.Goto, *region.Break, *region.Continue:
			fmt.Fprintf(w, "%s%s\n", indent, formatRegionJump(n))
		case *region.If:
			if len(n.Then) == 1 && len(n.Else) == 0 {
				if jump := formatRegionJump(n.Then[0]); jump != "" {
					fmt.Fprintf(w, "%sif %s %s\n", indent, formatIRCond(n.Cond), jump)
					continue
				}
			}
			fmt.Fprintf(w, "%sif %s {\n", indent, formatIRCond(n.Cond))
			writeRegionNodes(w, n.Then, depth+1)
			// An else arm that is a single if continues the chain as else-if.
			for len(n.Else) == 1 {
				next, ok := n.Else[0].(*region.If)
				if !ok {
					break
				}
				fmt.Fprintf(w, "%s} else if %s {\n", indent, formatIRCond(next.Cond))
				writeRegionNodes(w, next.Then, depth+1)
				n = next
			}
			if len(n.Else) > 0 {
				fmt.Fprintf(w, "%s} else {\n", indent)
				writeRegionNodes(w, n.Else, depth+1)
			}
			fmt.Fprintf(w, "%s}\n", indent)
		case *region.Loop:
			switch n.Kind {
			case region.LoopWhile:
				fmt.Fprintf(w, "%swhile %s {\n", indent, formatIRCond(n.Cond))
			case region.LoopDoWhile:
				fmt.Fprintf(w, "%sdo {\n", indent)
			case region.LoopFor:
				// formatIRCond wraps the condition in exactly one pair of
				// parentheses, which the for header does not need.
				cond := formatIRCond(n.Cond)
				fmt.Fprintf(w, "%sfor (%s; %s; %s) {\n", indent, formatForClause(n.Init), cond[1:len(cond)-1], formatForClause(n.Post))
			default:
				fmt.Fprintf(w, "%swhile (1) {\n", indent)
			}
			writeRegionNodes(w, n.Body, depth+1)
			if n.Kind == region.LoopDoWhile {
				fmt.Fprintf(w, "%s} while %s;\n", indent, formatIRCond(n.Cond))
			} else {
				fmt.Fprintf(w, "%s}\n", indent)
			}
		case *region.Switch:
			fmt.Fprintf(w, "%sswitch (%s) {\n", indent, formatIRExpr(n.Index, precLowest))
			// Empty trailing cases, typically a bare default, go to the end of
			// the switch, and a label may not end a block. They can be left out
			// only when no default remains to catch their values instead;
			// otherwise the last one ends with a break.
			cases := n.Cases
			trailing := len(cases)
			for trailing > 0 && len(cases[trailing-1].Body) == 0 {
				trailing--
			}
			if !slices.ContainsFunc(cases[:trailing], func(c region.Case) bool { return c.Default }) {
				cases = cases[:trailing]
			}
			for i, c := range cases {
				for _, v := range c.Values {
					fmt.Fprintf(w, "%scase %s:\n", indent, formatIRExpr(v, precLowest))
				}
				if c.Default {
					fmt.Fprintf(w, "%sdefault:\n", indent)
				}
				writeRegionNodes(w, c.Body, depth+1)
				if i == len(cases)-1 && len(c.Body) == 0 {
					fmt.Fprintf(w, "%s\tbreak;\n", indent)
				}
			}
			fmt.Fprintf(w, "%s}\n", indent)
		default:
			fmt.Fprintf(w, "%s/* unknown region node: %T */\n", indent, n)
		}
	}
}

// formatRegionJump renders a goto, break, or continue node as a statement,
// or returns "" for any other node.
func formatRegionJump(n region.Node) string {
	switch n := n.(type) {
	case *region.Goto:
		return "goto " + n.Label + ";"
	case *region.Break:
		return "break;"
	case *region.Continue:
		return "continue;"
	}
	return ""
}

// formatForClause renders a for loop's init or post assignment without its
// semicolon. A nil assignment renders empty.
func formatForClause(a *ir.Assign) string {
	if a == nil {
		return ""
	}
	return strings.TrimSuffix(formatIRStmt(a), ";")
}

// formatIRCond renders a condition wrapped in exactly one pair of
// parentheses.
func formatIRCond(cond ir.Expr) string {
	return "(" + formatIRExpr(cond, precLowest) + ")"
}
