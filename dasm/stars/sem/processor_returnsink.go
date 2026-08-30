package sem

import (
	"log/slog"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

type returnSinkProcessor struct{}

// ProcessFunc sinks return merge arms into the blocks that produced them.
//
// Merge-arm provenance is preserved: an arm may reach the shared return block
// through any number of semantically transparent jump/fallthrough blocks. The
// return is sunk to the arm block itself, not to the final trampoline.
func (p *returnSinkProcessor) ProcessFunc(result *Result, f *Func) bool {

	blockIndexByID := blockIndexByID(f.Blocks)
	changed := false

	for i := range f.Blocks {
		ret, merge, ok := trailingMergeReturn(f.Blocks[i])
		if !ok {
			continue
		}

		sinkID := f.Blocks[i].ID
		paths, ok := returnSinkPaths(f, sinkID, merge, blockIndexByID)
		if !ok {
			continue
		}

		for ai, arm := range merge.Arms {
			path := paths[ai]
			armIndex := blockIndexByID[arm.Block]

			// The arm's immediate successor may be a trampoline rather than
			// the return sink itself.
			nextID := sinkID
			if len(path) > 1 {
				nextID = path[1]
			}

			sinkReturnIntoBlock(
				&f.Blocks[armIndex],
				nextID,
				ret,
				arm.Value,
			)
		}

		// The shared return is now represented by returns at each merge arm.
		f.Blocks[i].Effects = nil
		changed = true
	}

	return changed
}

// blockIndexByID maps each semantic block ID to its index in blocks.
func blockIndexByID(blocks []Block) map[machine.BlockID]int {
	out := make(map[machine.BlockID]int, len(blocks))
	for i, block := range blocks {
		out[block.ID] = i
	}
	return out
}

// trailingMergeReturn returns the final return and merge when a block only
// returns a merge value.
func trailingMergeReturn(block Block) (*Return, *Merge, bool) {
	if len(block.Effects) != 1 {
		return nil, nil, false
	}

	ret, ok := block.Effects[0].(*Return)
	if !ok {
		return nil, nil, false
	}

	merge, ok := ret.Value.(*Merge)
	if !ok || len(merge.Arms) == 0 {
		return nil, nil, false
	}

	return ret, merge, true
}

// returnSinkPaths proves that every merge arm reaches sinkID through only
// transparent single-successor blocks.
//
// It also verifies the reverse relationship: walking backward from the sink
// through transparent blocks must terminate exactly at merge-arm blocks.
// This prevents accidentally swallowing an unrelated path that happens to
// enter one of the same trampolines.
//
// Each returned path starts at the merge arm's provenance block and ends at
// sinkID.
func returnSinkPaths(f *Func, sinkID machine.BlockID, merge *Merge, blockIndexByID map[machine.BlockID]int) ([][]machine.BlockID, bool) {
	armBlocks := make(map[machine.BlockID]bool, len(merge.Arms))
	paths := make([][]machine.BlockID, len(merge.Arms))

	for i, arm := range merge.Arms {
		if arm.Block == sinkID {
			return nil, false
		}
		if armBlocks[arm.Block] {
			return nil, false
		}

		armBlocks[arm.Block] = true

		path, ok := returnPathToSink(f, arm.Block, sinkID, blockIndexByID)
		if !ok {
			return nil, false
		}

		paths[i] = path
	}

	// Forward reachability alone is insufficient. For example:
	//
	//     arm ----\
	//              trampoline -> sink
	//     other --/
	//
	// If "other" isn't represented by a merge arm, sinking would discard a
	// real return path. Verify the sink's complete transparent predecessor
	// frontier instead.
	if !returnSinkFrontierMatchesArms(f, sinkID, armBlocks, blockIndexByID) {
		return nil, false
	}

	return paths, true
}

// returnPathToSink follows the unique CFG successor chain from an arm block to
// sinkID.
//
// The arm block itself may contain ordinary semantic work, but may not end in
// conflicting control flow. Every block after the arm and before the sink must
// be semantically transparent.
func returnPathToSink(f *Func, from machine.BlockID, sinkID machine.BlockID, blockIndexByID map[machine.BlockID]int) ([]machine.BlockID, bool) {
	path := []machine.BlockID{from}
	seen := map[machine.BlockID]bool{
		from: true,
	}

	current := from

	for current != sinkID {
		succs := f.CFG.Successors(current)
		if len(succs) != 1 {
			return nil, false
		}

		next := succs[0]

		if seen[next] {
			return nil, false
		}
		seen[next] = true

		blockIndex, ok := blockIndexByID[current]
		if !ok {
			slog.Error("Unknown block ID", "current", current)
			return nil, false
		}

		if current == from {
			if hasUnsinkableReturnOrigin(f.Blocks[blockIndex], next) {
				return nil, false
			}
		} else {
			if !isTransparentReturnPathBlock(f.Blocks[blockIndex], next) {
				return nil, false
			}
		}

		path = append(path, next)
		current = next
	}

	return path, true
}

// returnSinkFrontierMatchesArms walks backward from sinkID through transparent
// blocks.
//
// Every backward path must terminate at a merge-arm block. This means the merge
// arms describe the complete set of control-flow paths reaching this return,
// even when one or more intermediate trampoline blocks are shared.
func returnSinkFrontierMatchesArms(f *Func, sinkID machine.BlockID, armBlocks map[machine.BlockID]bool, blockIndexByID map[machine.BlockID]int,
) bool {
	seen := map[machine.BlockID]bool{
		sinkID: true,
	}

	stack := []machine.BlockID{sinkID}
	found := make(map[machine.BlockID]bool, len(armBlocks))

	for len(stack) != 0 {
		current := stack[len(stack)-1]
		stack = stack[:len(stack)-1]

		for _, pred := range f.CFG.Predecessors(current) {
			// Merge arms are the frontier. Do not walk past them; their
			// predecessors represent the source control flow that selected
			// the returned value.
			if armBlocks[pred] {
				found[pred] = true
				continue
			}

			if seen[pred] {
				continue
			}

			blockIndex, ok := blockIndexByID[pred]
			if !ok {
				return false
			}

			// A non-arm predecessor is acceptable only when it is itself a
			// transparent path block leading directly toward current.
			if !isTransparentReturnPathBlock(f.Blocks[blockIndex], current) {
				return false
			}

			seen[pred] = true
			stack = append(stack, pred)
		}
	}

	if len(found) != len(armBlocks) {
		return false
	}

	for arm := range armBlocks {
		if !found[arm] {
			return false
		}
	}

	return true
}

// hasUnsinkableReturnOrigin reports whether an arm block ends in control flow
// other than a jump to its unique successor.
//
// Ordinary semantic work before that terminator is allowed:
//
//	foo()
//	x = 1
//	goto trampoline
//
// can become:
//
//	foo()
//	x = 1
//	return value
func hasUnsinkableReturnOrigin(block Block, nextID machine.BlockID) bool {
	if len(block.Effects) == 0 {
		return false
	}

	switch effect := block.Effects[len(block.Effects)-1].(type) {
	case *Jump:
		return effect.To != nextID

	case *Branch, *Return:
		return true

	default:
		// No explicit terminator means the CFG edge is fallthrough.
		return false
	}
}

// isTransparentReturnPathBlock reports whether an intermediate block can be
// crossed without losing semantic behavior.
//
// An intermediate return-path block may contain:
//
//	<nothing>
//
// or:
//
//	goto next
//
// but no stores, calls, branches, or other semantic work.
func isTransparentReturnPathBlock(block Block, nextID machine.BlockID) bool {
	switch len(block.Effects) {
	case 0:
		return true

	case 1:
		jump, ok := block.Effects[0].(*Jump)
		return ok && jump.To == nextID

	default:
		return false
	}
}

// sinkReturnIntoBlock replaces a trailing jump to nextID with the arm return,
// or appends the return when the arm reaches nextID by fallthrough.
//
// Importantly, block is the original merge-arm provenance block, not an
// intermediate trampoline.
func sinkReturnIntoBlock(block *Block, nextID machine.BlockID, ret *Return, value Expr) {
	next := *ret
	next.MetaInfo.BlockID = block.ID
	next.Value = value

	if len(block.Effects) > 0 {
		if jump, ok := block.Effects[len(block.Effects)-1].(*Jump); ok &&
			jump.To == nextID {
			block.Effects[len(block.Effects)-1] = &next
			return
		}
	}

	block.Effects = append(block.Effects, &next)
}
