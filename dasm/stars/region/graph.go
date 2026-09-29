package region

import (
	"errors"
	"fmt"
	"math"

	graphlib "github.com/dominikbraun/graph"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// ExitID is the virtual exit vertex that every returning block reaches.
const ExitID machine.BlockID = math.MaxUint32

// Graph is the control-flow graph over IR blocks. Edges come from each
// block's final statement, so the graph matches exactly what the IR prints.
type Graph struct {
	Graph graphlib.Graph[machine.BlockID, *ir.Block]
	Entry machine.BlockID
	// Order holds block IDs in IR layout order, followed by ExitID.
	Order []machine.BlockID
	// Opaque is set when a block ends in untranslated control flow, so its
	// successors are unknown and the graph cannot be structured.
	Opaque bool

	blocks  map[machine.BlockID]*ir.Block
	byLabel map[string]machine.BlockID
	layout  map[machine.BlockID]int
	succ    map[machine.BlockID][]machine.BlockID
	pred    map[machine.BlockID][]machine.BlockID
}

// NewGraph builds the control-flow graph for fn's IR blocks plus a virtual
// exit vertex. It returns an error when a jump names an unknown label.
func NewGraph(fn *ir.Func) (*Graph, error) {
	if len(fn.Blocks) == 0 {
		return nil, fmt.Errorf("region graph %s: function has no blocks", fn.Name)
	}

	g := &Graph{
		Graph: graphlib.New(func(b *ir.Block) machine.BlockID { return b.ID }, graphlib.Directed()),
		Entry: fn.Blocks[0].ID,

		blocks:  make(map[machine.BlockID]*ir.Block, len(fn.Blocks)+1),
		byLabel: make(map[string]machine.BlockID, len(fn.Blocks)),
		layout:  make(map[machine.BlockID]int, len(fn.Blocks)+1),
		succ:    make(map[machine.BlockID][]machine.BlockID, len(fn.Blocks)),
		pred:    make(map[machine.BlockID][]machine.BlockID, len(fn.Blocks)+1),
	}

	exit := &ir.Block{ID: ExitID, Label: "exit"}
	for i := range fn.Blocks {
		if err := g.addVertex(&fn.Blocks[i]); err != nil {
			return nil, err
		}
		g.byLabel[fn.Blocks[i].Label] = fn.Blocks[i].ID
	}
	if err := g.addVertex(exit); err != nil {
		return nil, err
	}

	for i := range fn.Blocks {
		block := &fn.Blocks[i]
		next := ExitID
		if i+1 < len(fn.Blocks) {
			next = fn.Blocks[i+1].ID
		}
		targets, err := g.terminatorTargets(block, next)
		if err != nil {
			return nil, fmt.Errorf("region graph %s: %w", fn.Name, err)
		}
		for _, to := range targets {
			if err := g.addEdge(block.ID, to); err != nil {
				return nil, err
			}
		}
	}

	return g, nil
}

// addVertex adds block to the graph and appends it to the layout order.
func (g *Graph) addVertex(block *ir.Block) error {
	if err := g.Graph.AddVertex(block); err != nil {
		return fmt.Errorf("add region vertex %s: %w", block.Label, err)
	}
	g.blocks[block.ID] = block
	g.layout[block.ID] = len(g.Order)
	g.Order = append(g.Order, block.ID)
	return nil
}

// addEdge adds from→to once, keeping successor and predecessor lists in
// the order the edges are discovered.
func (g *Graph) addEdge(from, to machine.BlockID) error {
	err := g.Graph.AddEdge(from, to)
	if errors.Is(err, graphlib.ErrEdgeAlreadyExists) {
		return nil
	}
	if err != nil {
		return fmt.Errorf("add region edge %s -> %s: %w", g.blocks[from].Label, g.blocks[to].Label, err)
	}
	g.succ[from] = append(g.succ[from], to)
	g.pred[to] = append(g.pred[to], from)
	return nil
}

// terminatorTargets returns the successors of block given the block that
// follows it in layout order. Untranslated control flow marks the graph
// opaque and yields no edges.
func (g *Graph) terminatorTargets(block *ir.Block, next machine.BlockID) ([]machine.BlockID, error) {
	if len(block.Stmts) == 0 {
		return []machine.BlockID{next}, nil
	}

	switch s := block.Stmts[len(block.Stmts)-1].(type) {
	case *ir.IfGoto:
		return g.labelTargets(s.TrueLabel, s.FalseLabel)
	case *ir.Goto:
		return g.labelTargets(s.Label)
	case *ir.TableJump:
		return g.labelTargets(s.Labels...)
	case *ir.SwitchGoto:
		labels := []string{s.Default}
		for _, c := range s.Cases {
			labels = append(labels, c.Label)
		}
		return g.labelTargets(labels...)
	case *ir.Return:
		return []machine.BlockID{ExitID}, nil
	case *ir.Comment:
		switch s.EffectKind {
		case "branch", "jump", "tablejump":
			g.Opaque = true
			return nil, nil
		}
	}
	return []machine.BlockID{next}, nil
}

// labelTargets resolves jump labels to block IDs.
func (g *Graph) labelTargets(labels ...string) ([]machine.BlockID, error) {
	out := make([]machine.BlockID, len(labels))
	for i, label := range labels {
		id, ok := g.byLabel[label]
		if !ok {
			return nil, fmt.Errorf("jump to unknown label %s", label)
		}
		out[i] = id
	}
	return out, nil
}

// Successors returns the distinct successors of id in edge discovery order.
func (g *Graph) Successors(id machine.BlockID) []machine.BlockID {
	return g.succ[id]
}

// Predecessors returns the distinct predecessors of id in edge discovery order.
func (g *Graph) Predecessors(id machine.BlockID) []machine.BlockID {
	return g.pred[id]
}
