package machine

import (
	"fmt"
	"maps"

	graphlib "github.com/dominikbraun/graph"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
)

// BlockID is the starting instruction offset of a basic block in the CFG.
type BlockID uint32

func (b BlockID) String() string { return labelFor(uint32(b)) }

// Block is a maximal sequence of instructions with a single entry and exit.
type Block struct {
	ID                    BlockID
	StartIdx              int    // inclusive index into the instruction slice
	EndIdx                int    // exclusive index into the instruction slice
	EndOff                uint32 // offset just past the last instruction
	Label                 string
	KilledBeforeRead      map[asm.Reg]bool
	FlagsKilledBeforeRead bool
	RegLiveIn             map[asm.Reg]bool
	RegLiveOut            map[asm.Reg]bool
	BPLiveIn              map[int]bool
	BPLiveOut             map[int]bool
	FlagsLiveIn           bool
	FlagsLiveOut          bool
}

func (b *Block) String() string { return b.ID.String() }

// CFGOptions controls optional CFG normalization performed during construction.
type CFGOptions struct {
	// CollapseJumps rewrites direct branches through single-JMP trampoline
	// blocks and removes those trampoline blocks from the built graph.
	CollapseJumps bool
}

// CFG is the control-flow graph for a function.
type CFG struct {
	Options CFGOptions
	Blocks  []*Block
	Instrs  []asm.DecodedInst
	Calls   map[uint32]*InstCall
	Jumps   map[uint32]*InstJump
	Graph   graphlib.Graph[BlockID, *Block]
	// SCCs holds the final graph's strongly connected components in
	// deterministic block order.
	SCCs [][]BlockID

	byID map[BlockID]*Block
	// resolvedTargets maps original block IDs to the block reached after
	// following direct-JMP trampolines. It is populated only for collapsed CFGs.
	resolvedTargets map[BlockID]BlockID

	// built during graph building
	succ map[BlockID][]BlockID
	pred map[BlockID][]BlockID

	// derived from the final graph topology
	sccByBlock       map[BlockID]int
	cyclicBlocks     map[BlockID]bool
	loopPredsByBlock map[BlockID]map[BlockID]bool
	sccBlockOrder    []BlockID
}

func (cfg *CFG) Clone() *CFG {
	out := &CFG{
		Options: cfg.Options,

		Instrs: append([]asm.DecodedInst(nil), cfg.Instrs...),

		Calls: make(map[uint32]*InstCall, len(cfg.Calls)),
		Jumps: make(map[uint32]*InstJump, len(cfg.Jumps)),

		Blocks: make([]*Block, 0, len(cfg.Blocks)),
		byID:   make(map[BlockID]*Block, len(cfg.byID)),

		resolvedTargets: maps.Clone(cfg.resolvedTargets),

		succ:  make(map[BlockID][]BlockID, len(cfg.succ)),
		pred:  make(map[BlockID][]BlockID, len(cfg.pred)),
		Graph: newBlockGraph(),
	}

	for off, call := range cfg.Calls {
		cp := *call
		out.Calls[off] = &cp
	}

	for off, jump := range cfg.Jumps {
		cp := *jump
		cp.TableTargetOffs = append([]uint32(nil), jump.TableTargetOffs...)
		out.Jumps[off] = &cp
	}

	for _, block := range cfg.Blocks {
		cp := cloneBlock(block)

		out.Blocks = append(out.Blocks, cp)
		out.byID[cp.ID] = cp

		if err := out.Graph.AddVertex(cp); err != nil {
			panic(fmt.Errorf("clone CFG vertex %s: %w", cp, err))
		}
	}

	for id, succs := range cfg.succ {
		out.succ[id] = append([]BlockID(nil), succs...)
	}

	for id, preds := range cfg.pred {
		out.pred[id] = append([]BlockID(nil), preds...)
	}

	for from, succs := range out.succ {
		for _, to := range succs {
			if err := out.Graph.AddEdge(from, to); err != nil {
				panic(fmt.Errorf("clone CFG edge %s -> %s: %w", from, to, err))
			}
		}
	}

	out.buildGraphFacts()

	return out
}

func cloneBlock(block *Block) *Block {
	out := *block

	out.KilledBeforeRead = maps.Clone(block.KilledBeforeRead)
	out.RegLiveIn = maps.Clone(block.RegLiveIn)
	out.RegLiveOut = maps.Clone(block.RegLiveOut)
	out.BPLiveIn = maps.Clone(block.BPLiveIn)
	out.BPLiveOut = maps.Clone(block.BPLiveOut)

	return &out
}

// block returns the block with the given ID.
func (c *CFG) Block(id BlockID) *Block {
	return c.byID[id]
}

// BlockInstrs returns a slice of the instructions for a block
func (c *CFG) BlockInstrs(id BlockID) []asm.DecodedInst {
	block := c.byID[id]
	if block == nil {
		return nil
	}
	return c.Instrs[block.StartIdx:block.EndIdx]
}

// InstructionMnemonic returns the display mnemonic for an instruction.
func (c *CFG) InstructionMnemonic(inst asm.DecodedInst) string {
	return instMnemonic(inst)
}

// InstructionOperands returns call/jump-aware operands for an instruction.
func (c *CFG) InstructionOperands(inst asm.DecodedInst) string {
	if call := c.Calls[inst.Off]; call != nil {
		return call.Target.Name
	}
	if jump := c.Jumps[inst.Off]; jump != nil {
		if len(jump.TableTargetOffs) > 0 {
			return inst.Operands()
		}
		return jump.Label
	}
	return inst.Operands()
}

// InstructionComment returns call/jump-aware comments for an instruction.
func (c *CFG) InstructionComment(inst asm.DecodedInst) string {
	if call := c.Calls[inst.Off]; call != nil {
		return call.Target.CDecl()
	}
	return ""
}

// jumpTarget returns the target block recorded on a direct jump instruction.
func (c *CFG) jumpTarget(jump *InstJump) BlockID {
	return BlockID(jump.TargetOff)
}

// branchTargets returns the true and false block targets for a conditional
// jump in this CFG's constructed topology.
func (c *CFG) branchTargets(block *Block, jump *InstJump) (BlockID, BlockID) {
	return BlockID(jump.TargetOff), c.fallthroughTarget(block)
}

// fallthroughTarget returns the block reached by falling through from block in
// this CFG's constructed block layout.
func (c *CFG) fallthroughTarget(block *Block) BlockID {
	id := BlockID(block.EndOff)
	if !c.Options.CollapseJumps || c.byID[id] != nil {
		return id
	}
	for i, candidate := range c.Blocks {
		if candidate.ID != block.ID {
			continue
		}
		if i+1 < len(c.Blocks) {
			return c.Blocks[i+1].ID
		}
		break
	}
	return id
}

// resolvedBlock returns id after applying the jump-collapse mapping that was
// computed during CFG construction.
func (c *CFG) resolvedBlock(id BlockID) BlockID {
	if c.Options.CollapseJumps && c.resolvedTargets != nil {
		if resolved, ok := c.resolvedTargets[id]; ok {
			return resolved
		}
	}
	return id
}

// BuildCFG builds an instruction block graph from a function's instructions
// retReadsRegs controls whether RET/RETF are treated as reading AX:DX for
// return-value liveness.
func BuildCFG(ctx *FuncContext, insts []asm.DecodedInst, retReadsRegs bool, opt CFGOptions) (*CFG, error) {
	if len(insts) == 0 {
		return &CFG{
			Options: opt,
			Calls:   map[uint32]*InstCall{},
			Jumps:   map[uint32]*InstJump{},
			Graph:   newBlockGraph(),
			byID:    map[BlockID]*Block{},
		}, nil
	}

	calls, jumps := resolveInstMetadata(ctx, insts)

	// Pass 1: find all leaders.
	leaders := make(map[uint32]bool)
	leaders[insts[0].Off] = true // function entry is always a leader

	for i, inst := range insts {
		if jump := jumps[inst.Off]; jump != nil {
			if len(jump.TableTargetOffs) > 0 {
				for _, target := range jump.TableTargetOffs {
					leaders[target] = true
				}
			} else {
				leaders[jump.TargetOff] = true
			}
			if inst.Op == asm.OpJcc {
				// fallthrough is also a target
				if i+1 < len(insts) {
					leaders[insts[i+1].Off] = true
				}
			}
		}
	}

	// Pass 2: split into blocks.
	cfg := &CFG{
		Options: opt,
		Calls:   calls,
		Jumps:   jumps,
		Graph:   newBlockGraph(),
		byID:    make(map[BlockID]*Block),
		Instrs:  insts,
	}

	var cur *Block
	flush := func() {
		if cur != nil {
			cfg.Blocks = append(cfg.Blocks, cur)
		}
		cur = nil
	}

	for i, inst := range insts {
		if leaders[inst.Off] || cur == nil {
			flush()
			id := BlockID(inst.Off)
			label := labelFor(inst.Off)
			if lbl := ctx.fs.GetLabel(inst.Off); lbl != nil {
				label = lbl.Name
			}
			cur = &Block{
				ID:       id,
				StartIdx: i,
				Label:    label,
			}
			cfg.byID[id] = cur
			if err := cfg.Graph.AddVertex(cur); err != nil {
				return nil, fmt.Errorf("add cfg vertex %s: %w", cur, err)
			}
		}

		cur.EndIdx = i + 1
		cur.EndOff = inst.Off + uint32(inst.Len)
	}
	flush()

	if opt.CollapseJumps {
		if err := cfg.collapseJumpTrampolines(); err != nil {
			return nil, err
		}
	}

	if err := addBlockEdges(cfg); err != nil {
		return nil, err
	}
	cfg.buildGraphFacts()
	annotateBlockRegLiveness(cfg, retReadsRegs)

	return cfg, nil
}

// Successors returns the sorted block IDs directly reachable from id.
func (c *CFG) Successors(id BlockID) []BlockID {
	return c.succ[id]
}

// Predecessors returns the sorted block IDs that directly reach id.
func (c *CFG) Predecessors(id BlockID) []BlockID {
	return c.pred[id]
}

// --- Jump Trampoline Resolution

// resolveJumpTrampoline follows direct single-instruction JMP blocks to the
// next real control-flow target. It intentionally leaves the CFG's block list
// intact so dumps can still show the original assembly when needed.
func (c *CFG) resolveJumpTrampoline(id BlockID) BlockID {
	seen := map[BlockID]bool{}
	for {
		if seen[id] {
			return id
		}
		seen[id] = true

		block := c.Block(id)
		if block == nil {
			return id
		}

		insts := c.BlockInstrs(id)
		if len(insts) != 1 {
			return id
		}

		inst := insts[0]
		jmp := c.Jumps[inst.Off]
		if jmp == nil || inst.Op != asm.OpJMP || len(jmp.TableTargetOffs) > 0 {
			return id
		}

		id = BlockID(jmp.TargetOff)
	}
}

// isJumpTrampoline reports whether id is a single direct JMP block that can be
// omitted from simplified graph views after incoming edges have been resolved.
func (c *CFG) isJumpTrampoline(id BlockID) bool {
	insts := c.BlockInstrs(id)
	if len(insts) != 1 {
		return false
	}
	inst := insts[0]
	jmp := c.Jumps[inst.Off]
	return jmp != nil && inst.Op == asm.OpJMP && len(jmp.TableTargetOffs) == 0
}

// collapseJumpTrampolines normalizes direct-JMP trampoline blocks before graph
// edges and liveness are derived.
func (cfg *CFG) collapseJumpTrampolines() error {
	// records each original block's final target after following direct-JMP trampolines.
	cfg.resolvedTargets = make(map[BlockID]BlockID, len(cfg.Blocks))
	for _, block := range cfg.Blocks {
		cfg.resolvedTargets[block.ID] = cfg.resolveJumpTrampoline(block.ID)
	}

	removable := cfg.collapsibleJumpBlocks()
	cfg.preserveUnsafeFallthroughTrampolines(removable)
	cfg.collapseJumpTargets(removable)
	if err := cfg.removeCollapsedJumpBlocks(removable); err != nil {
		return err
	}
	cfg.removeRedundantNextBlockJumps()
	return nil
}

// collapsibleJumpBlocks finds direct-JMP trampoline blocks that can be removed
// after incoming explicit branches have been retargeted.
func (cfg *CFG) collapsibleJumpBlocks() map[BlockID]bool {
	removable := make(map[BlockID]bool, len(cfg.Blocks))
	for i, block := range cfg.Blocks {
		if i > 0 && cfg.isJumpTrampoline(block.ID) && cfg.resolvedBlock(block.ID) != block.ID {
			removable[block.ID] = true
		}
	}
	return removable
}

// preserveUnsafeFallthroughTrampolines keeps any trampoline whose removal would
// turn an implicit fallthrough into a different successor that cannot be
// represented by retargeting or inverting the preceding branch.
func (cfg *CFG) preserveUnsafeFallthroughTrampolines(removable map[BlockID]bool) {
	changed := true
	for changed {
		changed = false
		for _, block := range cfg.Blocks {
			if block.EndIdx <= block.StartIdx || block.EndIdx > len(cfg.Instrs) {
				continue
			}
			fallthroughBlock := BlockID(block.EndOff)
			if !removable[fallthroughBlock] {
				continue
			}

			next := cfg.nextKeptBlock(block.ID, removable)
			fallthroughTarget := cfg.resolvedBlock(fallthroughBlock)
			if next == fallthroughTarget {
				continue
			}

			last := cfg.Instrs[block.EndIdx-1]
			jcc := cfg.Jumps[last.Off]
			if jcc == nil || last.Op != asm.OpJcc || len(jcc.TableTargetOffs) > 0 {
				delete(removable, fallthroughBlock)
				changed = true
				continue
			}

			trueTarget := cfg.resolvedBlock(BlockID(jcc.TargetOff))
			if next == trueTarget {
				if _, ok := inverseJcc(last.Mnemonic); ok {
					continue
				}
			}

			delete(removable, fallthroughBlock)
			changed = true
		}
	}
}

// collapseJumpTargets rewrites direct jump instructions to point at resolved
// trampoline destinations while leaving the original instruction stream intact.
func (cfg *CFG) collapseJumpTargets(removable map[BlockID]bool) {
	nextInstrs := append([]asm.DecodedInst(nil), cfg.Instrs...)
	nextJumps := make(map[uint32]*InstJump, len(cfg.Jumps))
	for off, jump := range cfg.Jumps {
		copyJump := *jump
		nextJumps[off] = &copyJump
	}
	for _, block := range cfg.Blocks {
		if block.EndIdx <= block.StartIdx || block.EndIdx > len(cfg.Instrs) {
			continue
		}
		idx := block.EndIdx - 1
		inst := cfg.Instrs[idx]
		jmp := cfg.Jumps[inst.Off]
		if jmp == nil || len(jmp.TableTargetOffs) > 0 {
			continue
		}

		resolved, mnemonic := cfg.collapsedJumpTarget(block, inst, jmp, removable)
		if resolved == BlockID(jmp.TargetOff) {
			continue
		}

		next := inst
		next.Mnemonic = mnemonic
		next.Target = int32(resolved)
		nextJump := *jmp
		nextJump.TargetOff = uint32(resolved)
		nextJump.Label = resolved.String()
		if target := cfg.Block(resolved); target != nil {
			nextJump.Label = target.Label
		}
		nextInstrs[idx] = next
		nextJumps[next.Off] = &nextJump
	}
	cfg.Instrs = nextInstrs
	cfg.Jumps = nextJumps
}

// collapsedJumpTarget chooses the textual target for a jump in the collapsed
// block layout, inverting conditional branches when their removed fallthrough
// trampoline makes the old true target become the physical next block.
func (cfg *CFG) collapsedJumpTarget(block *Block, inst asm.DecodedInst, jmp *InstJump, removable map[BlockID]bool) (BlockID, string) {
	if inst.Op != asm.OpJcc {
		return cfg.resolvedBlock(BlockID(jmp.TargetOff)), inst.Mnemonic
	}

	trueTarget := cfg.resolvedBlock(BlockID(jmp.TargetOff))
	falseTarget := cfg.resolvedBlock(BlockID(block.EndOff))
	if trueTarget == falseTarget {
		return trueTarget, inst.Mnemonic
	}

	next := cfg.nextKeptBlock(block.ID, removable)
	if next != trueTarget {
		return trueTarget, inst.Mnemonic
	}

	if inverse, ok := inverseJcc(inst.Mnemonic); ok {
		return falseTarget, inverse
	}
	return trueTarget, inst.Mnemonic
}

// removeCollapsedJumpBlocks removes private trampoline blocks from the CFG's
// block list and graph after all incoming jumps have been retargeted.
func (cfg *CFG) removeCollapsedJumpBlocks(removable map[BlockID]bool) error {
	blocks := make([]*Block, 0, len(cfg.Blocks))
	byID := make(map[BlockID]*Block, len(cfg.byID))
	graph := newBlockGraph()

	for _, block := range cfg.Blocks {
		if removable[block.ID] {
			continue
		}

		blocks = append(blocks, block)
		byID[block.ID] = block
		if err := graph.AddVertex(block); err != nil {
			return fmt.Errorf("add collapsed cfg vertex %s: %w", block, err)
		}
	}

	cfg.Blocks = blocks
	cfg.byID = byID
	cfg.Graph = graph
	return nil
}

// removeRedundantNextBlockJumps drops terminal unconditional jumps whose target
// is already the next block in the collapsed physical layout.
func (cfg *CFG) removeRedundantNextBlockJumps() {
	for i, block := range cfg.Blocks {
		if block.EndIdx-block.StartIdx <= 1 || i+1 >= len(cfg.Blocks) {
			continue
		}

		inst := cfg.Instrs[block.EndIdx-1]
		jmp := cfg.Jumps[inst.Off]
		if jmp == nil || inst.Op != asm.OpJMP || len(jmp.TableTargetOffs) > 0 {
			continue
		}

		if BlockID(jmp.TargetOff) == cfg.Blocks[i+1].ID {
			block.EndIdx--
		}
	}
}

// nextKeptBlock returns the next block in physical order after id, skipping
// trampoline blocks selected for removal.
func (cfg *CFG) nextKeptBlock(id BlockID, removable map[BlockID]bool) BlockID {
	for i, block := range cfg.Blocks {
		if block.ID != id {
			continue
		}
		for j := i + 1; j < len(cfg.Blocks); j++ {
			next := cfg.Blocks[j].ID
			if !removable[next] {
				return next
			}
		}
		return 0
	}
	return 0
}

// inverseJcc returns the opposite conditional jump mnemonic.
func inverseJcc(mnemonic string) (string, bool) {
	switch mnemonic {
	case "JO":
		return "JNO", true
	case "JNO":
		return "JO", true
	case "JC", "JB", "JNAE":
		return "JNC", true
	case "JNC", "JAE", "JNB":
		return "JC", true
	case "JZ", "JE":
		return "JNZ", true
	case "JNZ", "JNE":
		return "JZ", true
	case "JBE", "JNA":
		return "JA", true
	case "JA", "JNBE":
		return "JBE", true
	case "JS":
		return "JNS", true
	case "JNS":
		return "JS", true
	case "JP", "JPE":
		return "JNP", true
	case "JNP", "JPO":
		return "JP", true
	case "JL", "JNGE":
		return "JGE", true
	case "JGE", "JNL":
		return "JL", true
	case "JLE", "JNG":
		return "JG", true
	case "JG", "JNLE":
		return "JLE", true
	default:
		return "", false
	}
}
