package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// ScratchRange identifies a half-open byte interval relative to BP.
type ScratchRange struct{ Offset, Size int }

// End returns the first byte outside the range.
func (r ScratchRange) End() int { return r.Offset + r.Size }

// Overlaps reports whether two storage views share bytes.
func (r ScratchRange) Overlaps(other ScratchRange) bool {
	return r.Offset < other.End() && other.Offset < r.End()
}

// Contains reports whether r covers every byte of other.
func (r ScratchRange) Contains(other ScratchRange) bool {
	return r.Offset <= other.Offset && r.End() >= other.End()
}

// scratchPathRoot returns the storage root of an already-classified scratch path.
func scratchPathRoot(path symresolve.SymbolPath) *symresolve.SymbolScratch {
	for {
		switch p := path.(type) {
		case *symresolve.SymbolScratch:
			return p
		case *symresolve.SymbolOffset:
			path = p.Base
		case *symresolve.SymbolField:
			path = p.Base
		case *symresolve.SymbolBitfield:
			path = p.Base
		default:
			panic("expected a scratch-rooted storage path")
		}
	}
}

// scratchSlotRangeExpr returns the BP displacement and width named by a raw
// memory expression or a symbolic scratch path.
func scratchSlotRangeExpr(expr Expr) (int, int, bool) {
	switch e := expr.(type) {
	case *RawMemory:
		mem := e.Address
		if _, ok := mem.Base.(*machine.FrameBase); !ok || mem.Index != nil {
			return 0, 0, false
		}
		if mem.Seg != nil {
			seg, ok := mem.Seg.(*machine.Reg)
			if !ok || seg.Val != asm.RegSS {
				return 0, 0, false
			}
		}
		return mem.Disp, mem.Width, mem.Width > 0
	case *Memory:
		if e.Index != nil || e.Scale != 0 && e.Scale != 1 {
			return 0, 0, false
		}
		if !scratchStackSegment(e.Seg) || !scratchFrameBase(e.Base) {
			return 0, 0, false
		}
		return e.Disp, e.Width, e.Width > 0
	case *SymbolRef:
		return scratchSymbolPathRange(e.Path)
	case *Part:
		disp, _, ok := scratchSlotRangeExpr(e.Base)
		if !ok || e.Width <= 0 {
			return 0, 0, false
		}
		return disp + e.ByteOff, e.Width, true
	default:
		return 0, 0, false
	}
}

// scratchSymbolPathRange returns the BP displacement and access width for a
// path rooted at synthetic scratch storage.
func scratchSymbolPathRange(path symresolve.SymbolPath) (int, int, bool) {
	switch p := path.(type) {
	case *symresolve.SymbolScratch:
		return p.BPOffset, p.StorageSize, p.StorageSize > 0
	case *symresolve.SymbolOffset:
		disp, _, ok := scratchSymbolPathRange(p.Base)
		if !ok || p.Type() == nil || p.Type().Bytes() <= 0 {
			return 0, 0, false
		}
		return disp + p.Offset, p.Type().Bytes(), true
	case *symresolve.SymbolField:
		if typeinfo.IsPointer(p.Base.Type()) {
			return 0, 0, false
		}
		disp, _, ok := scratchSymbolPathRange(p.Base)
		if !ok || p.Field == nil || p.Field.Type == nil {
			return 0, 0, false
		}
		return disp + p.Field.Offset, p.Field.Type.Bytes(), true
	case *symresolve.SymbolBitfield:
		if typeinfo.IsPointer(p.Base.Type()) {
			return 0, 0, false
		}
		disp, _, ok := scratchSymbolPathRange(p.Base)
		if !ok || p.Field == nil || p.Field.Bitfield == nil {
			return 0, 0, false
		}
		return disp + p.Field.Offset, p.Field.Bitfield.StorageSize, true
	default:
		return 0, 0, false
	}
}

// scratchStackSegment reports whether expr names the stack segment or omits it.
func scratchStackSegment(expr Expr) bool {
	if expr == nil {
		return true
	}
	seg, ok := expr.(*Register)

	return ok && seg.Val == asm.RegSS
}

// scratchFrameBase reports whether expr names the frame base pointer.
func scratchFrameBase(expr Expr) bool {
	raw, ok := expr.(*RawValue)
	if !ok {
		return false
	}
	_, ok = raw.Value.(*machine.FrameBase)
	return ok
}
