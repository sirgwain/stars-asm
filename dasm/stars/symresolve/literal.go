package symresolve

import (
	"encoding/binary"
	"math"
)

func (r *Resolver) ResolveFloatLiteral(seg int, off uint32, bytes int) (float64, bool) {

	buf, ok := r.img.ReadFrameBytes(seg, off, bytes)
	if !ok {
		return 0, false
	}

	switch bytes {
	case 4:
		return float64(math.Float32frombits(binary.LittleEndian.Uint32(buf[:4]))), true
	case 8:
		return math.Float64frombits(binary.LittleEndian.Uint64(buf[:8])), true
	}

	return 0, false
}
