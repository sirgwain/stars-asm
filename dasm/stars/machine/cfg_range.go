package machine

// BlockRange selects whole CFG blocks that intersect an address interval.
// FromAddr and ToAddr are instruction offsets; zero means that side of the
// range is open.
type BlockRange struct {
	FromAddr uint32
	ToAddr   uint32
}

// BlocksInRange returns CFG blocks that intersect r, preserving CFG block order.
// If ToAddr is omitted, the range is extended to the end of the block containing
// FromAddr so callers never render a partial basic block. An explicit ToAddr
// inside a block is similarly extended to that block's end.
func (c *CFG) BlocksInRange(r BlockRange) []*Block {
	if c == nil {
		return nil
	}

	r = c.normalizeBlockRange(r)
	out := make([]*Block, 0, len(c.Blocks))
	for _, block := range c.Blocks {
		if blockInRange(block, r) {
			out = append(out, block)
		}
	}
	return out
}

// IncludedBlockIDs returns the IDs of BlocksInRange as a lookup set.
func (c *CFG) IncludedBlockIDs(r BlockRange) map[BlockID]bool {
	out := make(map[BlockID]bool)
	for _, block := range c.BlocksInRange(r) {
		out[block.ID] = true
	}
	return out
}

// blockInRange reports whether a block intersects r. A zero bound means that
// side of the range is open.
func blockInRange(block *Block, r BlockRange) bool {
	if block == nil {
		return false
	}
	if r.FromAddr == 0 && r.ToAddr == 0 {
		return true
	}
	if r.FromAddr != 0 && block.EndOff <= r.FromAddr {
		return false
	}
	if r.ToAddr != 0 && uint32(block.ID) >= r.ToAddr {
		return false
	}
	return true
}

func (c *CFG) normalizeBlockRange(r BlockRange) BlockRange {
	explicitTo := r.ToAddr != 0
	if r.FromAddr != 0 && r.ToAddr == 0 {
		for _, block := range c.Blocks {
			if uint32(block.ID) <= r.FromAddr && block.EndOff > r.FromAddr {
				r.ToAddr = block.EndOff
				break
			}
		}
	}
	if explicitTo {
		for _, block := range c.Blocks {
			if uint32(block.ID) <= r.ToAddr && block.EndOff > r.ToAddr {
				r.ToAddr = block.EndOff
				break
			}
		}
	}
	return r
}
