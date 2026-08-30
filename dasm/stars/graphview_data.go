package stars

import (
	"bytes"
	"fmt"
	"strings"

	"github.com/alecthomas/chroma/v2"
	chromahtml "github.com/alecthomas/chroma/v2/formatters/html"
	"github.com/alecthomas/chroma/v2/lexers"
	"github.com/alecthomas/chroma/v2/styles"
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type FuncGraphViewData struct {
	Function string              `json:"function"`
	Nodes    []FuncGraphViewNode `json:"nodes"`
	Edges    []FuncGraphViewEdge `json:"edges"`
	SCCs     [][]string          `json:"sccs"`
}

// FuncGraphViewNode is the browser-facing representation of one CFG basic block.
// It carries both plain assembly lines for sizing/searching and highlighted
// HTML for display in the graph overlay and side pane.
type FuncGraphViewNode struct {
	ID           string   `json:"id"`
	Label        string   `json:"label"`
	Title        string   `json:"title"`
	Instructions []string `json:"instructions"`
	AsmHTML      string   `json:"asmHtml"`
	SCC          int      `json:"scc"`
}

// FuncGraphViewEdge connects two rendered CFG block nodes by their string IDs.
type FuncGraphViewEdge struct {
	ID     string `json:"id"`
	Source string `json:"source"`
	Target string `json:"target"`
	Kind   string `json:"kind,omitempty"`
}

// BuildFuncGraphViewData disassembles a function, annotates the decoded
// instructions, builds a control-flow graph, and converts it into the JSON-ready
// data consumed by the graph viewer.
func BuildFuncGraphViewData(img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpOptions) (*FuncGraphViewData, error) {
	analysis, err := analyzeFunc(img, sdb, fs, opt)
	if err != nil {
		return nil, err
	}

	return buildGraphViewDataFromCFG(analysis.CFG, fs, opt, analysis.Annotations)
}

// buildGraphViewDataFromCFG converts machine CFG blocks and successors into the
// viewer's node/edge payload, including strongly connected component groups for
// loop highlighting.
func buildGraphViewDataFromCFG(cfg *machine.CFG, fs *typeinfo.Function, opt DumpOptions, annotations *sem.Result) (*FuncGraphViewData, error) {
	// Read SCCs before filtering so each included block keeps its original group
	// identity while SCC boxes omit hidden blocks from the rendered view.
	sccs := cfg.SCCs
	sccByBlock := map[machine.BlockID]int{}
	viewSCCs := make([][]string, 0, len(sccs))
	blockRange := machine.BlockRange{
		FromAddr: opt.FromAddr,
		ToAddr:   opt.ToAddr,
	}
	included := cfg.IncludedBlockIDs(blockRange)
	for i, scc := range sccs {
		viewSCC := make([]string, 0, len(scc))
		for _, id := range scc {
			sccByBlock[id] = i
			if !included[id] {
				continue
			}
			viewSCC = append(viewSCC, id.String())
		}
		if len(viewSCC) > 0 {
			viewSCCs = append(viewSCCs, viewSCC)
		}
	}

	// Render each visible block once, storing plain lines for graph sizing and
	// highlighted HTML for the custom DOM nodes drawn over Cytoscape.
	out := &FuncGraphViewData{
		Function: fs.Name,
		SCCs:     viewSCCs,
	}
	for _, block := range cfg.Blocks {
		if !included[block.ID] {
			continue
		}
		nodeID := block.String()
		asmText, err := renderGraphBlockInsts(cfg, block, fs, annotations)
		if err != nil {
			return nil, err
		}
		asmHTML, err := highlightGraphAsm(asmText)
		if err != nil {
			return nil, err
		}
		out.Nodes = append(out.Nodes, FuncGraphViewNode{
			ID:           nodeID,
			Label:        block.Label,
			Title:        block.Label,
			Instructions: graphAsmLines(asmText),
			AsmHTML:      asmHTML,
			SCC:          sccByBlock[block.ID],
		})
		for _, succ := range cfg.Successors(block.ID) {
			if !included[succ] {
				continue
			}
			// Edges are filtered with the same visibility set so partial graph
			// views never point at blocks that were intentionally omitted.
			out.Edges = append(out.Edges, FuncGraphViewEdge{
				ID:     fmt.Sprintf("%s_%s", nodeID, succ.String()),
				Source: nodeID,
				Target: succ.String(),
				Kind:   graphEdgeKind(cfg, block, succ),
			})
		}
	}

	return out, nil
}

// graphEdgeKind classifies branch edges for the browser renderer.
func graphEdgeKind(cfg *machine.CFG, block *machine.Block, succ machine.BlockID) string {
	instrs := cfg.BlockInstrs(block.ID)
	if len(instrs) == 0 {
		return ""
	}

	last := instrs[len(instrs)-1]
	jmp := cfg.Jumps[last.Off]
	if jmp == nil || last.Op != asm.OpJcc || len(jmp.TableTargetOffs) > 0 {
		return ""
	}

	if succ == machine.BlockID(jmp.TargetOff) {
		return "true"
	}
	if next, ok := nextGraphBlockID(cfg, block.ID); ok && succ == next {
		return "fallback"
	}
	return ""
}

// nextGraphBlockID returns the next block in the rendered CFG order.
func nextGraphBlockID(cfg *machine.CFG, id machine.BlockID) (machine.BlockID, bool) {
	for i, block := range cfg.Blocks {
		if block.ID != id {
			continue
		}
		if i+1 < len(cfg.Blocks) {
			return cfg.Blocks[i+1].ID, true
		}
		return 0, false
	}
	return 0, false
}

// renderGraphBlockInsts renders one basic block through the existing assembly
// dump template, keeping graph output visually consistent with text dumps.
func renderGraphBlockInsts(cfg *machine.CFG, block *machine.Block, fs *typeinfo.Function, annotations *sem.Result) (string, error) {
	var sourceFile string
	var sourceLines []typeinfo.SourceLine
	if fs != nil {
		sourceFile = fs.SourceFile.File
		sourceLines = fs.SourceFile.Lines
	}

	var instAnnotations startemplates.InstructionAnnotations
	if annotations != nil {
		instAnnotations = annotations
	}

	view := startemplates.DumpAsmBlockInstsView{
		Options: startemplates.DumpAsmOptions{
			RawOperands: false,
			ShowBytes:   false,
			ShowOffsets: false,
		},
		CFG: cfg,
		Block: startemplates.DumpAsmBlockView{
			Label:  block.Label,
			Off:    uint32(block.ID),
			Instrs: cfg.BlockInstrs(block.ID),
		},
		SourceFile:  sourceFile,
		SourceLines: sourceLines,
		Annotations: instAnnotations,
	}

	var out bytes.Buffer
	if err := startemplates.RenderDumpAsmBlockInsts(&out, view); err != nil {
		return "", err
	}
	return strings.TrimRight(out.String(), "\n"), nil
}

// graphAsmLines splits rendered assembly into stable display lines and treats
// empty blocks as having no instructions.
func graphAsmLines(src string) []string {
	if strings.TrimSpace(src) == "" {
		return nil
	}
	return strings.Split(src, "\n")
}

// highlightGraphAsm syntax-highlights rendered assembly for insertion into the
// graph viewer's custom HTML nodes.
func highlightGraphAsm(src string) (string, error) {
	lexer := lexers.Get("nasm")
	if lexer == nil {
		lexer = lexers.Fallback
	}
	lexer = chroma.Coalesce(lexer)
	it, err := lexer.Tokenise(nil, src)
	if err != nil {
		return "", err
	}

	style := styles.Get("dracula")
	if style == nil {
		style = styles.Fallback
	}
	formatter := chromahtml.New(
		chromahtml.WithClasses(false),
		chromahtml.PreventSurroundingPre(true),
	)

	var out bytes.Buffer
	if err := formatter.Format(&out, style, it); err != nil {
		return "", err
	}
	return strings.TrimRight(out.String(), "\n"), nil
}
