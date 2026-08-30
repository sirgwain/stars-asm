package sem

import "github.com/sirgwain/stars-asm/dasm/stars/machine"

type removeEmptyBlocksProcessor struct{}

// ProcessFunc removes empty semantic blocks that are not explicitly referenced
// by semantic control flow.
//
// The underlying machine CFG is intentionally left unchanged. This pass only
// cleans the semantic block list.
func (p *removeEmptyBlocksProcessor) ProcessFunc(result *Result, f *Func) bool {
	if f == nil || len(f.Blocks) == 0 {
		return false
	}

	targets := semanticBlockTargets(f)

	// Preserve the function entry even if it happens to contain no semantic
	// effects. Several analyses treat Blocks[0] as the semantic entry.
	targets[f.Blocks[0].ID] = true

	next := make([]Block, 0, len(f.Blocks))
	changed := false

	for _, block := range f.Blocks {
		if len(block.Effects) == 0 && !targets[block.ID] {
			changed = true
			continue
		}

		next = append(next, block)
	}

	if !changed {
		return false
	}

	f.Blocks = next
	return true
}

// semanticBlockTargets returns every block explicitly referenced by semantic
// control flow.
//
// These blocks must remain materialized even when they currently contain no
// semantic effects, because their IDs still represent meaningful labels.
func semanticBlockTargets(f *Func) map[machine.BlockID]bool {
	targets := make(map[machine.BlockID]bool)

	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			switch effect := effect.(type) {
			case *Jump:
				targets[effect.To] = true

			case *Branch:
				targets[effect.TrueBlock] = true
				targets[effect.FalseBlock] = true
			}
		}
	}

	return targets
}
