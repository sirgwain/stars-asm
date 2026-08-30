package machine

import (
	"log/slog"
	"sort"

	"github.com/sirgwain/stars-asm/dasm/log"
)

type extractor struct {
	*FuncContext
	log          *slog.Logger
	currentBlock *Block
	fromAddr     uint32
	toAddr       uint32
}

type ExtractOptions struct {
	FromAddr uint32
	ToAddr   uint32
}

// custom lazy logger
func (ctx *extractor) LogValue() slog.Value {
	if ctx.currentBlock == nil {
		return slog.GroupValue(
			slog.String("func", ctx.fs.Name),
		)
	}
	return slog.GroupValue(
		slog.String("func", ctx.fs.Name),
		slog.String("block", ctx.currentBlock.ID.String()),
	)
}

func newExtractor(ctx *FuncContext, fromAddr, toAddr uint32) *extractor {
	extractor := extractor{
		FuncContext: ctx,
		fromAddr:    fromAddr,
		toAddr:      toAddr,
	}
	handler := &extractorLogger{
		Handler: &log.LazyHandler{Handler: slog.Default().Handler()},
		include: extractor.shouldLogCurrentBlock,
	}
	extractor.log = slog.New(handler).With(slog.Any("e", &extractor))

	return &extractor
}

// shouldLogCurrentBlock reports whether the currently processed block is in the
// requested debug range.
func (ctx *extractor) shouldLogCurrentBlock() bool {
	if ctx.fromAddr == 0 && ctx.toAddr == 0 {
		return true
	}
	if ctx.currentBlock == nil {
		return true
	}
	if ctx.fromAddr != 0 && uint32(ctx.currentBlock.ID) < ctx.fromAddr {
		return false
	}
	if ctx.toAddr != 0 && uint32(ctx.currentBlock.ID) >= ctx.toAddr {
		return false
	}
	return true
}

// Extract builds CFG/block effectprs by running instruction transfer to a simple
// fixed point. Replace the tiny CFG shell below with the existing sem/cfg.go
// implementation when integrating this package.
func Extract(ctx *FuncContext, cfg *CFG, opt ExtractOptions) *FuncEffects {
	extractor := newExtractor(ctx, opt.FromAddr, opt.ToAddr)
	return extractor.extract(cfg)
}

func (ctx *extractor) extract(cfg *CFG) *FuncEffects {

	// Expand a single debug address into the containing block's address range.
	if ctx.fromAddr != 0 && ctx.toAddr == 0 {
		for _, blk := range cfg.Blocks {
			if uint32(blk.ID) <= ctx.fromAddr && blk.EndOff > ctx.fromAddr {
				ctx.toAddr = blk.EndOff
				break
			}
		}
	}

	if len(cfg.Blocks) == 0 {
		return &FuncEffects{CFG: cfg}
	}

	// Seed the entry block and allocate the per-block state/effect stores used
	// by the transfer fixed point.
	entryID := cfg.Blocks[0].ID
	entryStates := map[BlockID]*state{
		entryID: seedEntryState(ctx.FuncContext),
	}
	exitStates := make(map[BlockID]*state, len(cfg.Blocks))
	effectsByBlock := make(map[BlockID][]Effect, len(cfg.Blocks))
	blockOrder := cfg.sccBlockOrder
	blockRank := make(map[BlockID]int, len(blockOrder))
	for i, id := range blockOrder {
		blockRank[id] = i
	}
	changeCounts := make(map[BlockID]int, len(cfg.Blocks))
	inQueue := make(map[BlockID]bool, len(cfg.Blocks))
	queue := make([]BlockID, 0, len(cfg.Blocks))

	// Keep the worklist ordered by SCC rank so straight-line regions settle
	// before downstream blocks and loops are revisited predictably.
	enqueue := func(id BlockID) {
		if inQueue[id] || cfg.Block(id) == nil {
			return
		}
		inQueue[id] = true
		queue = append(queue, id)
		sort.SliceStable(queue, func(i, j int) bool {
			return blockRank[queue[i]] < blockRank[queue[j]]
		})
	}

	// Start at the function entry even if the SCC ordering fallback omitted it.
	for _, id := range blockOrder {
		if id == entryID {
			enqueue(id)
			break
		}
	}
	if len(queue) == 0 {
		enqueue(entryID)
	}

	iterations := 0
	limit := max(len(cfg.Blocks)*len(cfg.Blocks)*16, 64)
	for len(queue) > 0 {
		// Pop one scheduled block. Transfer only runs once predecessor propagation has
		// produced an entry state for that block.
		id := queue[0]
		queue = queue[1:]
		inQueue[id] = false

		blk := cfg.Block(id)
		stIn := entryStates[id]
		if blk == nil || stIn == nil {
			continue
		}
		iterations++
		if iterations > limit {
			ctx.log.Warn("extract worklist reached iteration limit", "limit", limit)
			break
		}

		// Transfer the block, then project away temporaries that should not
		// escape across the block boundary.
		stOut, effects := ctx.processBlock(stIn, cfg, blk)
		stOut = projectStateForBlockExit(blk, stOut)

		// For cyclic blocks, widen repeatedly changing state so loop analysis
		// converges instead of chasing every precise iteration forever.
		if exitStates[id] != nil && cfg.cyclicBlocks[id] && !sameState(exitStates[id], stOut) {
			changeCounts[id]++
			if changeCounts[id] > 4 {
				stOut = widenChangingLoopState(stOut, exitStates[id])
			}
			if changeCounts[id] > 8 && !sameState(exitStates[id], stOut) {
				effectsByBlock[id] = effects
				continue
			}
		}
		if sameState(exitStates[id], stOut) {
			effectsByBlock[id] = effects
			continue
		}
		exitStates[id] = stOut
		effectsByBlock[id] = effects

		// Join predecessor exits into each successor's entry state and requeue
		// any successor whose entry changed.
		for _, succID := range cfg.Successors(id) {
			succ := cfg.Block(succID)
			if succ == nil {
				continue
			}
			nextEntry := joinBlockEntry(cfg, succ, exitStates, entryStates[succID], cfg.loopPredsByBlock)
			if nextEntry == nil {
				continue
			}
			if sameState(entryStates[succID], nextEntry) {
				continue
			}
			entryStates[succID] = nextEntry
			enqueue(succID)
		}
	}

	// Preserve CFG block order in the final effects view.
	var blocksEffects []BlockEffects
	for _, blk := range cfg.Blocks {
		blocksEffects = append(blocksEffects, BlockEffects{
			Block:   blk.ID,
			Entry:   entryStates[blk.ID],
			Exit:    exitStates[blk.ID],
			Effects: effectsByBlock[blk.ID],
		})
	}

	return &FuncEffects{CFG: cfg, Blocks: blocksEffects}
}

// sccBlockOrder returns blocks ordered by the CFG's SCC condensation graph.
func sccBlockOrder(cfg *CFG) []BlockID {
	// Collapse the CFG into strongly connected components so each loop can be
	// scheduled as a unit.
	sccs := cfg.SCCs
	if len(sccs) == 0 {
		out := make([]BlockID, 0, len(cfg.Blocks))
		for _, block := range cfg.Blocks {
			out = append(out, block.ID)
		}
		return out
	}

	// Project inter-block edges onto the SCC condensation graph and track each
	// component's incoming edge count.
	sccSucc := make(map[int]map[int]bool, len(sccs))
	indegree := make([]int, len(sccs))
	for _, block := range cfg.Blocks {
		from := cfg.sccByBlock[block.ID]
		for _, succID := range cfg.Successors(block.ID) {
			to := cfg.sccByBlock[succID]
			if from == to {
				continue
			}
			if sccSucc[from] == nil {
				sccSucc[from] = make(map[int]bool)
			}
			if sccSucc[from][to] {
				continue
			}
			sccSucc[from][to] = true
			indegree[to]++
		}
	}

	// Start the topological walk with components that have no incoming edges.
	var ready []int
	for i, degree := range indegree {
		if degree == 0 {
			ready = append(ready, i)
		}
	}
	order := make([]BlockID, 0, len(cfg.Blocks))
	for len(ready) > 0 {
		// Pick components and successors in numeric order to keep output stable.
		sort.Ints(ready)
		sccID := ready[0]
		ready = ready[1:]
		order = append(order, sccs[sccID]...)
		var succs []int
		for succID := range sccSucc[sccID] {
			succs = append(succs, succID)
		}
		sort.Ints(succs)
		for _, succID := range succs {
			indegree[succID]--
			if indegree[succID] == 0 {
				ready = append(ready, succID)
			}
		}
	}
	// If the condensation walk missed anything, fall back to the CFG's original
	// block order rather than returning a partial schedule.
	if len(order) != len(cfg.Blocks) {
		order = order[:0]
		for _, block := range cfg.Blocks {
			order = append(order, block.ID)
		}
	}
	return order
}
