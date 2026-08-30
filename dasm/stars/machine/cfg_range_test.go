package machine

import (
	"reflect"
	"testing"
)

func TestCFGBlocksInRangeIncludesWholeIntersectingBlocks(t *testing.T) {
	cfg := cfgWithRangeBlocks()

	blocks := cfg.BlocksInRange(BlockRange{
		FromAddr: 0x1003,
		ToAddr:   0x1006,
	})

	if got, want := blockIDs(blocks), []BlockID{0x1002, 0x1006}; !reflect.DeepEqual(got, want) {
		t.Fatalf("BlocksInRange IDs = %#v, want %#v", got, want)
	}
}

func TestCFGBlocksInRangeFromOnlyStopsAtContainingBlockEnd(t *testing.T) {
	cfg := cfgWithRangeBlocks()

	blocks := cfg.BlocksInRange(BlockRange{FromAddr: 0x1003})

	if got, want := blockIDs(blocks), []BlockID{0x1002}; !reflect.DeepEqual(got, want) {
		t.Fatalf("BlocksInRange IDs = %#v, want %#v", got, want)
	}
}

func TestCFGIncludedBlockIDs(t *testing.T) {
	cfg := cfgWithRangeBlocks()

	got := cfg.IncludedBlockIDs(BlockRange{
		FromAddr: 0x1003,
		ToAddr:   0x1006,
	})

	want := map[BlockID]bool{0x1002: true, 0x1006: true}
	if !reflect.DeepEqual(got, want) {
		t.Fatalf("IncludedBlockIDs = %#v, want %#v", got, want)
	}
}

func cfgWithRangeBlocks() *CFG {
	return &CFG{
		Blocks: []*Block{
			{ID: 0x1000, EndOff: 0x1002},
			{ID: 0x1002, EndOff: 0x1006},
			{ID: 0x1006, EndOff: 0x1008},
		},
	}
}

func blockIDs(blocks []*Block) []BlockID {
	out := make([]BlockID, 0, len(blocks))
	for _, block := range blocks {
		out = append(out, block.ID)
	}
	return out
}
