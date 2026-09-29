package templates

import (
	"bytes"
	"fmt"
	"io"
	"strings"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpIRView is the template view for low-level IR and optional analysis sections.
type DumpIRView struct {
	Options DumpIROptions
	Func    ir.Func
	Locals  []ir.Local
	Blocks  []DumpIRBlockView
	SemView DumpSemView
}

// DumpIRBlockView is the combined dump view for one IR block.
type DumpIRBlockView struct {
	Off      uint32
	Label    string
	SemBlock DumpSemBlockView
	Stmts    []ir.Stmt
}

// NewDumpIRView creates a low-level IR dump view.
func NewDumpIRView(fn ir.Func, opt DumpIROptions) DumpIRView {
	fn.Blocks = irBlocksInRange(fn.Blocks, machine.BlockRange{
		FromAddr: opt.FromAddr,
		ToAddr:   opt.ToAddr,
	})
	view := DumpIRView{
		Options: opt,
		Func:    fn,
		Locals:  uniqueIRLocals(fn.Locals),
	}
	view.setIRBlocks()
	return view
}

// NewDumpIRViewWithSem creates a combined ASM, machine-effect, semantic, and IR view.
func NewDumpIRViewWithSem(fn ir.Func, semView DumpSemView, opt DumpIROptions) DumpIRView {
	view := NewDumpIRView(fn, opt)
	view.Options = opt
	view.SemView = semView

	semBlocksByOff := make(map[uint32]DumpSemBlockView, len(semView.Blocks))
	for _, block := range semView.Blocks {
		semBlocksByOff[block.Off] = block
	}
	for i := range view.Blocks {
		if block, ok := semBlocksByOff[view.Blocks[i].Off]; ok {
			view.Blocks[i].SemBlock = block
		}
	}
	return view
}

// RenderDumpIR renders explicit-block IR intended for analysis, not prettiness.
func RenderDumpIR(w io.Writer, view DumpIRView) error {
	t := template.New("dump_ir.templ").
		Funcs(template.FuncMap{
			"renderLocals":   renderIRLocals,
			"formatIRStmt":   formatIRStmt,
			"renderAsmBlock": renderDumpSemAsmBlock,
			"highlightLines": HighlightLines,
			"highlightText":  HighlightTextLines,
			"showIRSections": showIRSections,
		})

	tmpl, err := t.ParseFS(templatesFS, "assets/dump_ir.templ")
	if err != nil {
		return err
	}

	var buf bytes.Buffer
	if err := tmpl.Execute(&buf, view); err != nil {
		return err
	}
	if showIRSections(view) {
		_, err := io.WriteString(w, buf.String())
		return err
	}
	formatted, err := formatCSource(buf.String())
	if err != nil {
		return err
	}
	printHighlightedC(w, formatted, view.Options.ShowColor)
	return nil
}

// setIRBlocks populates the IR-only block portion of a dump view.
func (view *DumpIRView) setIRBlocks() {
	view.Blocks = make([]DumpIRBlockView, len(view.Func.Blocks))
	for i, block := range view.Func.Blocks {
		view.Blocks[i] = DumpIRBlockView{
			Off:   block.StartOff,
			Label: block.Label,
			Stmts: block.Stmts,
		}
	}
}

// showIRSections reports whether the combined dump needs section headers.
func showIRSections(view DumpIRView) bool {
	return view.Options.ShowAsm || view.Options.ShowEffects || view.Options.ShowSem
}

// renderIRLocals renders tab-indented local declarations.
func renderIRLocals(locals []ir.Local) string {
	if len(locals) == 0 {
		return ""
	}

	var out strings.Builder
	for _, local := range locals {
		fmt.Fprintf(&out, "\t%s;\n", typeinfo.TypeDecl(local.Type, local.Name))
	}
	out.WriteString("\n")
	return out.String()
}

// uniqueIRLocals returns the first named local for each local name.
func uniqueIRLocals(locals []ir.Local) []ir.Local {
	out := make([]ir.Local, 0, len(locals))
	seen := map[string]bool{}
	for _, l := range locals {
		if l.Name != "" && !seen[l.Name] {
			seen[l.Name] = true
			out = append(out, l)
		}
	}
	return out
}

// irBlocksInRange returns IR blocks that intersect the requested address range.
func irBlocksInRange(blocks []ir.Block, r machine.BlockRange) []ir.Block {
	r = normalizeIRBlockRange(blocks, r)
	out := make([]ir.Block, 0, len(blocks))
	for _, block := range blocks {
		if irBlockInRange(block, r) {
			out = append(out, block)
		}
	}
	return out
}

// irBlockInRange reports whether a block intersects the requested address range.
func irBlockInRange(block ir.Block, r machine.BlockRange) bool {
	if r.FromAddr == 0 && r.ToAddr == 0 {
		return true
	}
	if block.EndOff == 0 {
		return true
	}
	if r.FromAddr != 0 && block.EndOff <= r.FromAddr {
		return false
	}
	if r.ToAddr != 0 && block.StartOff >= r.ToAddr {
		return false
	}
	return true
}

// normalizeIRBlockRange expands partial bounds to whole IR blocks.
func normalizeIRBlockRange(blocks []ir.Block, r machine.BlockRange) machine.BlockRange {
	explicitTo := r.ToAddr != 0
	if r.FromAddr != 0 && r.ToAddr == 0 {
		for _, block := range blocks {
			if block.StartOff <= r.FromAddr && block.EndOff > r.FromAddr {
				r.ToAddr = block.EndOff
				break
			}
		}
	}
	if explicitTo {
		for _, block := range blocks {
			if block.StartOff <= r.ToAddr && block.EndOff > r.ToAddr {
				r.ToAddr = block.EndOff
				break
			}
		}
	}
	return r
}

// formatIRStmt renders a single low-level IR statement as C-like text.
func formatIRStmt(stmt ir.Stmt) string {
	switch s := stmt.(type) {
	case *ir.Assign:
		if raw, ok := s.Dst.(*ir.Deref); ok && raw.Type != nil && raw.Type.Bytes() > 1 {
			return fmt.Sprintf("RawStore%d(%s, %s);", raw.Type.Bytes()*8, formatRawAddress(raw, precLowest), formatIRExpr(s.Src, precLowest))
		}
		return fmt.Sprintf("%s = %s;", formatIRExpr(s.Dst, precLowest), formatIRExpr(s.Src, precLowest))
	case *ir.ExprStmt:
		return formatIRExpr(s.Expr, precLowest) + ";"
	case *ir.IfGoto:
		return fmt.Sprintf("if (%s) goto %s; else goto %s;", formatIRExpr(s.Cond, precLowest), s.TrueLabel, s.FalseLabel)
	case *ir.TableJump:
		var text strings.Builder
		fmt.Fprintf(&text, "switch (%s) {", formatIRExpr(s.Index, precLowest))
		for i, label := range s.Labels {
			fmt.Fprintf(&text, " case %#x: goto %s;", i*2, label)
		}
		text.WriteString(" }")
		return text.String()
	case *ir.Goto:
		return "goto " + s.Label + ";"
	case *ir.Return:
		if s.Value == nil {
			return "return;"
		}
		return "return " + formatIRExpr(s.Value, precLowest) + ";"
	case *ir.Comment:
		return "/* " + sanitizeIRComment(s.Text) + " */"
	default:
		return fmt.Sprintf("/* untranslated IR statement: %T */", stmt)
	}
}

// C operator precedence levels for formatIRExpr; a higher level binds
// tighter. Calls, subscripts, member access, and primary expressions share
// precPostfix; prefix operators and casts share precUnary.
const (
	precLowest = iota
	precCond
	precOr
	precAnd
	precBitOr
	precBitXor
	precBitAnd
	precEquality
	precRelational
	precShift
	precAdditive
	precMultiplicative
	precUnary
	precPostfix
)

// binaryPrec maps each IR binary operator to its C precedence level.
var binaryPrec = map[string]int{
	"||": precOr,
	"&&": precAnd,
	"|":  precBitOr,
	"^":  precBitXor,
	"&":  precBitAnd,
	"==": precEquality, "!=": precEquality,
	"<": precRelational, "<=": precRelational, ">": precRelational, ">=": precRelational,
	"<<": precShift, ">>": precShift,
	"+": precAdditive, "-": precAdditive,
	"*": precMultiplicative, "/": precMultiplicative, "%": precMultiplicative,
}

// formatIRExpr renders expr as C, wrapped in parentheses when it binds more
// loosely than prec, the precedence its position requires.
func formatIRExpr(expr ir.Expr, prec int) string {
	text, own := formatIRExprText(expr)
	if own < prec {
		return "(" + text + ")"
	}
	return text
}

// formatIRExprText renders expr as C without outer parentheses and returns
// the precedence of its outermost operator.
func formatIRExprText(expr ir.Expr) (string, int) {
	switch e := expr.(type) {
	case *ir.Var:
		return e.Name, precPostfix
	case *ir.IntConst:
		text := e.Text
		if text == "" {
			text = fmt.Sprintf("0x%x", e.Value)
		}
		return text, signedLiteralPrec(text)
	case *ir.FloatConst:
		text := sem.FormatFloat(e.Value)
		return text, signedLiteralPrec(text)
	case *ir.StringConst:
		return e.Value, precPostfix
	case *ir.SizeOf:
		return "sizeof(" + e.Type + ")", precPostfix
	case *ir.Unary:
		if e.Functional {
			return e.Op + "(" + formatIRExpr(e.X, precLowest) + ")", precPostfix
		}
		return e.Op + formatUnaryOperand(e.Op, e.X), precUnary
	case *ir.Binary:
		return formatIRBinary(e)
	case *ir.Cond:
		// ?: is right-associative: a nested conditional in the else arm needs no
		// parentheses, one in the condition does.
		return formatIRExpr(e.Cond, precOr) + " ? " + formatIRExpr(e.Then, precLowest) + " : " + formatIRExpr(e.Else, precCond), precCond
	case *ir.Cast:
		return "(" + e.Type + ")" + formatUnaryOperand("", e.Value), precUnary
	case *ir.Index:
		return formatIRExpr(e.Base, precPostfix) + "[" + formatIRExpr(e.Index, precLowest) + "]", precPostfix
	case *ir.Field:
		op := "."
		if e.Pointer {
			op = "->"
		}
		return formatIRExpr(e.Base, precPostfix) + op + e.Name, precPostfix
	case *ir.Call:
		return formatIRExpr(e.Target, precPostfix) + "(" + formatIRArgs(e.Args) + ")", precPostfix
	case *ir.Macro:
		return e.Name + "(" + formatIRArgs(e.Args) + ")", precPostfix
	case *ir.AddressOf:
		return "&" + formatUnaryOperand("&", e.Target), precUnary
	case *ir.Deref:
		if e.Type == nil {
			return "*" + formatUnaryOperand("*", e.Pointer), precUnary
		}
		width, decl := e.Type.Bytes(), typeinfo.TypeDecl(e.Type, "")
		if width == 1 {
			return fmt.Sprintf("*(%s *)%s", decl, formatRawAddress(e, precUnary)), precUnary
		}
		load := fmt.Sprintf("RawLoad%d(%s)", width*8, formatRawAddress(e, precLowest))
		if typeinfo.Equals(e.Type, typeinfo.UintForWidth(width)) {
			return load, precPostfix
		}
		return "(" + decl + ")" + load, precUnary
	case *ir.PointerOffset:
		op, off := "+", formatIRExpr(e.Offset, precAdditive+1)
		if c, ok := e.Offset.(*ir.IntConst); ok && strings.HasPrefix(off, "-") && c.Text != "" {
			op, off = "-", off[1:]
		}
		offset := "(uint8_t *)" + formatUnaryOperand("", e.Pointer) + " " + op + " " + off
		if e.Type == nil {
			return offset, precAdditive
		}
		return "(" + typeinfo.TypeDecl(e.Type, "") + ")(" + offset + ")", precUnary
	default:
		return fmt.Sprintf("/*expr %T*/0", expr), precPostfix
	}
}

// signedLiteralPrec returns the precedence of a numeric literal's text: a
// leading sign makes it a unary expression.
func signedLiteralPrec(text string) int {
	if strings.HasPrefix(text, "-") || strings.HasPrefix(text, "+") {
		return precUnary
	}
	return precPostfix
}

// formatUnaryOperand renders the operand of the prefix operator op, or of a
// cast when op is empty. An operand starting with the same sign as op is
// parenthesized so the two cannot fuse into -- or ++ or &&.
func formatUnaryOperand(op string, x ir.Expr) string {
	text := formatIRExpr(x, precUnary)
	if op != "" && strings.ContainsAny(op, "-+&") && strings.HasPrefix(text, op) {
		return "(" + text + ")"
	}
	return text
}

// formatIRBinary renders a binary expression with its operands parenthesized
// by precedence; the right operand of a left-associative operator also
// needs them at equal precedence. Operands are also parenthesized where
// gcc's -Wparentheses would ask for clarity.
func formatIRBinary(e *ir.Binary) (string, int) {
	prec, ok := binaryPrec[e.Op]
	if !ok {
		return "(" + formatIRExpr(e.LHS, precPostfix) + " " + e.Op + " " + formatIRExpr(e.RHS, precPostfix) + ")", precPostfix
	}
	lhs, rhs := prec, prec+1
	if clarityParens(e.Op, e.LHS) {
		lhs = precPostfix
	}
	if clarityParens(e.Op, e.RHS) {
		rhs = precPostfix
	}
	return formatIRExpr(e.LHS, lhs) + " " + e.Op + " " + formatIRExpr(e.RHS, rhs), prec
}

// clarityParens reports whether operand of the binary operator op keeps its
// parentheses although precedence makes them unnecessary: && inside ||,
// mixed bitwise operators, a comparison inside a bitwise operator or another
// comparison, and + or - inside a shift.
func clarityParens(op string, operand ir.Expr) bool {
	b, ok := operand.(*ir.Binary)
	if !ok {
		return false
	}
	inner, outer := binaryPrec[b.Op], binaryPrec[op]
	bitwise := func(p int) bool { return p == precBitOr || p == precBitXor || p == precBitAnd }
	compare := func(p int) bool { return p == precEquality || p == precRelational }
	switch {
	case outer == precOr && inner == precAnd:
		return true
	case bitwise(outer) && (bitwise(inner) || compare(inner)) && b.Op != op:
		return true
	case compare(outer) && compare(inner):
		return true
	case outer == precShift && inner == precAdditive:
		return true
	}
	return false
}

// formatIRArgs renders call or macro arguments separated by commas.
func formatIRArgs(args []ir.Expr) string {
	parts := make([]string, len(args))
	for i, a := range args {
		parts[i] = formatIRExpr(a, precLowest)
	}
	return strings.Join(parts, ", ")
}

// formatRawAddress renders the byte address of a raw storage access,
// parenthesized when it binds more loosely than prec.
func formatRawAddress(e *ir.Deref, prec int) string {
	if e.ByteOff == 0 {
		return formatIRExpr(e.Pointer, prec)
	}
	op, off := "+", e.ByteOff
	if off < 0 {
		op, off = "-", -off
	}
	text := fmt.Sprintf("(uint8_t *)%s %s 0x%x", formatUnaryOperand("", e.Pointer), op, off)
	if prec > precAdditive {
		return "(" + text + ")"
	}
	return text
}

// sanitizeIRComment keeps block comment delimiters from leaking into output.
func sanitizeIRComment(s string) string {
	return strings.ReplaceAll(s, "*/", "* /")
}
