package machine

import (
	"fmt"
	"slices"
	"sort"

	graphlib "github.com/dominikbraun/graph"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
)

// buildGraphFacts caches SCC-derived facts after CFG topology is finalized.
func (cfg *CFG) buildGraphFacts() {
	cfg.SCCs = computeStronglyConnectedComponents(cfg)
	cfg.sccByBlock = make(map[BlockID]int, len(cfg.Blocks))
	cfg.cyclicBlocks = make(map[BlockID]bool)
	cfg.loopPredsByBlock = make(map[BlockID]map[BlockID]bool)

	for i, scc := range cfg.SCCs {
		cyclic := len(scc) > 1
		for _, id := range scc {
			cfg.sccByBlock[id] = i
		}
		if len(scc) == 1 {
			id := scc[0]
			for _, succ := range cfg.Successors(id) {
				if succ == id {
					cyclic = true
					break
				}
			}
		}
		if !cyclic {
			continue
		}
		for _, id := range scc {
			cfg.cyclicBlocks[id] = true
		}
	}

	for _, block := range cfg.Blocks {
		blockSCC, ok := cfg.sccByBlock[block.ID]
		if !ok || !cfg.cyclicBlocks[block.ID] {
			continue
		}
		for _, pred := range cfg.Predecessors(block.ID) {
			if cfg.sccByBlock[pred] != blockSCC {
				continue
			}
			if cfg.loopPredsByBlock[block.ID] == nil {
				cfg.loopPredsByBlock[block.ID] = make(map[BlockID]bool)
			}
			cfg.loopPredsByBlock[block.ID][pred] = true
		}
	}

	cfg.sccBlockOrder = sccBlockOrder(cfg)
}

// computeStronglyConnectedComponents returns deterministic SCCs from graphlib.
func computeStronglyConnectedComponents(c *CFG) [][]BlockID {
	sccs, err := graphlib.StronglyConnectedComponents(c.Graph)
	if err != nil {
		return nil
	}
	for _, scc := range sccs {
		slices.Sort(scc)
	}
	sort.Slice(sccs, func(i, j int) bool {
		if len(sccs[i]) == 0 {
			return len(sccs[j]) != 0
		}
		if len(sccs[j]) == 0 {
			return false
		}
		return sccs[i][0] < sccs[j][0]
	})
	return sccs
}

// newBlockGraph creates the graphlib wrapper used for algorithms that are
// easier to delegate, such as strongly connected component detection. The
// in-memory CFG still keeps explicit succ/pred maps for fast local queries.
func newBlockGraph() graphlib.Graph[BlockID, *Block] {
	return graphlib.New(func(block *Block) BlockID {
		if block == nil {
			return 0
		}
		return block.ID
	}, graphlib.Directed())
}

// addBlockEdges derives the outgoing edges for every basic block, records them
// in deterministic successor/predecessor maps, and mirrors them into Graph when
// graphlib-backed algorithms are enabled.
func addBlockEdges(cfg *CFG) error {
	cfg.succ = make(map[BlockID][]BlockID, len(cfg.Blocks))
	cfg.pred = make(map[BlockID][]BlockID, len(cfg.Blocks))

	for _, block := range cfg.Blocks {
		succs := rawBlockSuccessors(cfg, block)
		if cfg.Options.CollapseJumps {
			succs = resolveBlockSuccessors(cfg, succs)
		}
		for _, succ := range succs {
			// Some branch targets can point outside the built CFG, especially
			// around malformed data or unresolved switch tables. Keep the edge
			// indexes limited to known blocks.
			// TODO: remove this if Stars!.exe doesn't have it
			if cfg.byID[succ] == nil {
				continue
			}

			cfg.succ[block.ID] = append(cfg.succ[block.ID], succ)
			cfg.pred[succ] = append(cfg.pred[succ], block.ID)

			if cfg.Graph != nil {
				if err := cfg.Graph.AddEdge(block.ID, succ); err != nil {
					return fmt.Errorf(
						"add cfg edge %s -> %s: %w",
						block,
						labelFor(uint32(succ)),
						err,
					)
				}
			}
		}
	}

	for id := range cfg.succ {
		slices.Sort(cfg.succ[id])
	}

	for id := range cfg.pred {
		slices.Sort(cfg.pred[id])
	}

	return nil
}

// InsertSyntheticBlockOnEdge replaces one CFG edge with a new synthetic block.
func (cfg *CFG) InsertSyntheticBlockOnEdge(from BlockID, to BlockID, id BlockID) error {
	if cfg.byID[from] == nil {
		return fmt.Errorf("split edge from unknown block %s", from)
	}
	if cfg.byID[to] == nil {
		return fmt.Errorf("split edge to unknown block %s", to)
	}
	if cfg.byID[id] != nil {
		return fmt.Errorf("split edge synthetic block already exists %s", id)
	}
	if !hasBlockID(cfg.succ[from], to) {
		return fmt.Errorf("split edge missing CFG edge %s -> %s", from, to)
	}

	block := &Block{ID: id, Label: id.String()}
	cfg.insertSyntheticBlockAfter(from, block)
	cfg.byID[id] = block

	cfg.succ[from] = replaceBlockID(cfg.succ[from], to, id)
	cfg.pred[to] = replaceBlockID(cfg.pred[to], from, id)
	cfg.succ[id] = []BlockID{to}
	cfg.pred[id] = []BlockID{from}

	slices.Sort(cfg.succ[from])
	slices.Sort(cfg.pred[to])

	if cfg.Graph != nil {
		if err := cfg.Graph.AddVertex(block); err != nil {
			return fmt.Errorf("add synthetic cfg vertex %s: %w", block, err)
		}
		if err := cfg.Graph.RemoveEdge(from, to); err != nil {
			return fmt.Errorf("remove cfg edge %s -> %s: %w", from, to, err)
		}
		if err := cfg.Graph.AddEdge(from, id); err != nil {
			return fmt.Errorf("add cfg edge %s -> %s: %w", from, id, err)
		}
		if err := cfg.Graph.AddEdge(id, to); err != nil {
			return fmt.Errorf("add cfg edge %s -> %s: %w", id, to, err)
		}
	}

	cfg.buildGraphFacts()
	return nil
}

// insertSyntheticBlockAfter inserts block after from in physical CFG order.
func (cfg *CFG) insertSyntheticBlockAfter(from BlockID, block *Block) {
	for i, existing := range cfg.Blocks {
		if existing.ID != from {
			continue
		}
		cfg.Blocks = append(cfg.Blocks, nil)
		copy(cfg.Blocks[i+2:], cfg.Blocks[i+1:])
		cfg.Blocks[i+1] = block
		return
	}
	cfg.Blocks = append(cfg.Blocks, block)
}

// hasBlockID reports whether ids contains id.
func hasBlockID(ids []BlockID, id BlockID) bool {
	for _, existing := range ids {
		if existing == id {
			return true
		}
	}
	return false
}

// replaceBlockID returns ids with old replaced by next.
func replaceBlockID(ids []BlockID, old BlockID, next BlockID) []BlockID {
	out := append([]BlockID(nil), ids...)
	for i, existing := range out {
		if existing == old {
			out[i] = next
		}
	}
	return out
}

// resolveBlockSuccessors resolves jump trampolines in successors to collapse the graph
func resolveBlockSuccessors(cfg *CFG, succs []BlockID) []BlockID {
	out := make([]BlockID, 0, len(succs))
	for _, succ := range succs {
		out = appendTarget(out, uint32(cfg.resolvedBlock(succ)))
	}
	return out
}

// rawBlockSuccessors interprets the block terminator and returns the literal
// block IDs reachable after executing this block. Conditional jumps include both
// the branch target and fallthrough, unconditional jumps include only the
// target, and ordinary instructions fall through to the next block when one
// exists.
func rawBlockSuccessors(cfg *CFG, block *Block) []BlockID {
	if block.EndIdx <= block.StartIdx || block.EndIdx > len(cfg.Instrs) {
		return nil
	}

	last := cfg.Instrs[block.EndIdx-1]
	if jump := cfg.Jumps[last.Off]; jump != nil {
		// Jump tables have multiple computed destinations and no implicit
		// fallthrough from this block.
		if len(jump.TableTargetOffs) > 0 {
			return blockIDsFromOffsets(jump.TableTargetOffs)
		}
		if last.Op == asm.OpJcc {
			return appendTarget(nil, jump.TargetOff, uint32(cfg.fallthroughTarget(block)))
		}
		if last.Op == asm.OpJMP {
			return appendTarget(nil, jump.TargetOff)
		}
	}

	switch last.Op {
	case asm.OpRET, asm.OpRETF:
		return nil
	default:
		ft := cfg.fallthroughTarget(block)
		if cfg.byID[ft] == nil {
			return nil
		}
		return []BlockID{ft}
	}
}

// blockIDsFromOffsets converts raw instruction offsets from decoded jumps into
// CFG block IDs, dropping duplicate targets along the way.
func blockIDsFromOffsets(offsets []uint32) []BlockID {
	out := make([]BlockID, 0, len(offsets))
	for _, off := range offsets {
		out = appendTarget(out, off)
	}
	return out
}

// appendTarget appends branch targets as block IDs while preserving first-seen
// order. This keeps conditional branch target/fallthrough order stable.
func appendTarget(out []BlockID, targets ...uint32) []BlockID {
	seen := make(map[BlockID]bool, len(out)+len(targets))
	for _, id := range out {
		seen[id] = true
	}
	for _, target := range targets {
		id := BlockID(target)
		if seen[id] {
			continue
		}
		seen[id] = true
		out = append(out, id)
	}
	return out
}
