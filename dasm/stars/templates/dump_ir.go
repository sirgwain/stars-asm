package templates

import (
	"bytes"
	"fmt"
	"io"
	"strconv"
	"strings"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
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
func renderIRLocals(view DumpIRView) string {
	if len(view.Locals) == 0 {
		return ""
	}

	var out strings.Builder
	for _, local := range view.Locals {
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
		return fmt.Sprintf("%s = %s;", formatIRExpr(s.Dst), formatIRExpr(s.Src))
	case *ir.ExprStmt:
		return formatIRExpr(s.Expr) + ";"
	case *ir.IfGoto:
		return fmt.Sprintf("if (%s) goto %s; else goto %s;", formatIRExpr(s.Cond), s.TrueLabel, s.FalseLabel)
	case *ir.TableJump:
		var text strings.Builder
		fmt.Fprintf(&text, "switch (%s) {", formatIRExpr(s.Index))
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
		return "return " + formatIRExpr(s.Value) + ";"
	case *ir.Comment:
		return "/* " + sanitizeIRComment(s.Text) + " */"
	default:
		return fmt.Sprintf("/* untranslated IR statement: %T */", stmt)
	}
}

// formatIRExpr renders a single low-level IR expression as C-like text.
func formatIRExpr(expr ir.Expr) string {
	switch e := expr.(type) {
	case *ir.Var:
		return e.Name
	case *ir.IntConst:
		if e.Text != "" {
			return e.Text
		}
		return fmt.Sprintf("0x%x", e.Value)
	case *ir.FloatConst:
		return strconv.FormatFloat(e.Value, 'g', -1, 64)
	case *ir.StringConst:
		return e.Value
	case *ir.Unary:
		if e.Functional {
			return e.Op + "(" + formatIRExpr(e.X) + ")"
		}
		return "(" + e.Op + formatIRExpr(e.X) + ")"
	case *ir.Binary:
		return "(" + formatIRExpr(e.LHS) + " " + e.Op + " " + formatIRExpr(e.RHS) + ")"
	case *ir.Cast:
		return "(" + e.Type + ")(" + formatIRExpr(e.Value) + ")"
	case *ir.Index:
		return formatIRExpr(e.Base) + "[" + formatIRExpr(e.Index) + "]"
	case *ir.Field:
		op := "."
		if e.Pointer {
			op = "->"
		}
		return formatIRExpr(e.Base) + op + e.Name
	case *ir.Call:
		args := make([]string, len(e.Args))
		for i, a := range e.Args {
			args[i] = formatIRExpr(a)
		}
		return formatIRExpr(e.Target) + "(" + strings.Join(args, ", ") + ")"
	case *ir.Macro:
		args := make([]string, len(e.Args))
		for i, a := range e.Args {
			args[i] = formatIRExpr(a)
		}
		return e.Name + "(" + strings.Join(args, ", ") + ")"
	case *ir.AddressOf:
		return "&(" + formatIRExpr(e.Target) + ")"
	case *ir.Deref:
		p := formatIRExpr(e.Pointer)
		if e.ByteOff == 0 {
			return "*(" + p + ")"
		}
		if e.ByteOff < 0 {
			return fmt.Sprintf("*(%s - 0x%x)", p, -e.ByteOff)
		}
		return fmt.Sprintf("*(%s + 0x%x)", p, e.ByteOff)
	case *ir.PointerOffset:
		return "((uint8_t *)(" + formatIRExpr(e.Pointer) + ") + " + formatIRExpr(e.Offset) + ")"
	default:
		return fmt.Sprintf("/*expr %T*/0", expr)
	}
}

// sanitizeIRComment keeps block comment delimiters from leaking into output.
func sanitizeIRComment(s string) string {
	return strings.ReplaceAll(s, "*/", "* /")
}
