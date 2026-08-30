package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCoalesceCopiesProcessorCoalescesContiguousCopies verifies word copies
// over one contiguous address range become one wider copy.
func TestCoalesceCopiesProcessorCoalescesContiguousCopies(t *testing.T) {
	dst := machine.AddressVal(machine.MemoryAccess{Base: machine.FrameBaseVal(), Disp: -12, Width: 2})
	src := machine.AddressVal(machine.MemoryAccess{Seg: machine.ConstVal(0x25), Base: machine.LoadVal(machine.MemoryAccess{Base: machine.FrameBaseVal(), Disp: 8, Width: 2}), Width: 2})

	block := machine.BlockEffects{Effects: []machine.Effect{
		copyEffect(0x1000, dst, src, 0),
		copyEffect(0x1001, dst, src, 2),
		copyEffect(0x1002, dst, src, 4),
		copyEffect(0x1003, dst, src, 6),
	}}

	gotBlock, changed := (&coalesceCopiesProcessor{}).ProcessMachineBlock(nil, machine.FuncEffects{}, block)
	if !changed {
		t.Fatal("ProcessMachineBlock changed = false, want true")
	}
	if got, want := len(gotBlock.Effects), 1; got != want {
		t.Fatalf("effects = %d, want %d", got, want)
	}
	got := gotBlock.Effects[0].(machine.CopyEffect)
	if got.Width != 8 {
		t.Fatalf("copy width = %d, want 8", got.Width)
	}
	if got.MetaInfo.InstOff != 0x1000 {
		t.Fatalf("copy inst off = %#x, want 0x1000", got.MetaInfo.InstOff)
	}
}

// TestCoalesceCopiesProcessorCoalescesTrailingByteCopy verifies a REP MOVSW
// span can coalesce with a trailing MOVSB for odd-sized copies.
func TestCoalesceCopiesProcessorCoalescesTrailingByteCopy(t *testing.T) {
	result := &machine.CallResult{
		Target:  &typeinfo.Function{Name: "LphuldefSBFromId"},
		Type:    &typeinfo.Pointer{Elem: &typeinfo.Struct{Name: "HULDEF", Size: 0x7b}},
		InstOff: 0x29ac,
	}
	dst := machine.AddressVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(0x25),
		Base:  machine.ConstVal(0x2436),
		Width: 0x7a,
	})
	src := machine.AddressVal(machine.MemoryAccess{
		Seg:   machine.FarPointerVal(result, machine.FarPointerSegment),
		Base:  machine.FarPointerVal(result, machine.FarPointerOffset),
		Width: 0x7a,
	})
	nextDst := machine.AddressVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(0x25),
		Base:  machine.ConstVal(0x24b0),
		Width: 1,
	})
	nextSrc := machine.AddressVal(machine.MemoryAccess{
		Seg:   machine.FarPointerVal(result, machine.FarPointerSegment),
		Base:  machine.BinaryVal(machine.ValueOpAdd, machine.FarPointerVal(result, machine.FarPointerOffset), machine.ConstVal(0x7a)),
		Width: 1,
	})

	block := machine.BlockEffects{Effects: []machine.Effect{
		machine.CopyEffect{MetaInfo: machine.Meta{InstOff: 0x29c4}, Dst: dst, Src: src, Width: 0x7a},
		machine.CopyEffect{MetaInfo: machine.Meta{InstOff: 0x29c6}, Dst: nextDst, Src: nextSrc, Width: 1},
	}}

	gotBlock, changed := (&coalesceCopiesProcessor{}).ProcessMachineBlock(nil, machine.FuncEffects{}, block)
	if !changed {
		t.Fatal("ProcessMachineBlock changed = false, want true")
	}
	if got, want := len(gotBlock.Effects), 1; got != want {
		t.Fatalf("effects = %d, want %d", got, want)
	}
	got := gotBlock.Effects[0].(machine.CopyEffect)
	if got.Width != 0x7b {
		t.Fatalf("copy width = %#x, want 0x7b", got.Width)
	}
}

// TestCoalesceCopiesProcessorKeepsAdjacentRunsSeparate verifies independent
// contiguous copy runs produce independent wider copies.
func TestCoalesceCopiesProcessorKeepsAdjacentRunsSeparate(t *testing.T) {
	prc := machine.LoadVal(machine.MemoryAccess{Base: machine.FrameBaseVal(), Disp: 8, Width: 2})
	rcIn := machine.AddressVal(machine.MemoryAccess{Base: machine.FrameBaseVal(), Disp: -0x2a, Width: 2})
	rc := machine.AddressVal(machine.MemoryAccess{Base: machine.FrameBaseVal(), Disp: -0x4e, Width: 2})

	block := machine.BlockEffects{Effects: []machine.Effect{
		copyEffect(0x6228, rcIn, prc, 0),
		copyEffect(0x6229, rcIn, prc, 2),
		copyEffect(0x622a, rcIn, prc, 4),
		copyEffect(0x622b, rcIn, prc, 6),
		copyEffect(0x6237, rc, rcIn, 0),
		copyEffect(0x6238, rc, rcIn, 2),
		copyEffect(0x6239, rc, rcIn, 4),
		copyEffect(0x623a, rc, rcIn, 6),
	}}

	gotBlock, changed := (&coalesceCopiesProcessor{}).ProcessMachineBlock(nil, machine.FuncEffects{}, block)
	if !changed {
		t.Fatal("ProcessMachineBlock changed = false, want true")
	}
	if got, want := len(gotBlock.Effects), 2; got != want {
		t.Fatalf("effects = %d, want %d", got, want)
	}
	wants := []uint32{0x6228, 0x6237}
	for i, wantOff := range wants {
		got := gotBlock.Effects[i].(machine.CopyEffect)
		if got.Width != 8 {
			t.Fatalf("copy[%d] width = %d, want 8", i, got.Width)
		}
		if got.MetaInfo.InstOff != wantOff {
			t.Fatalf("copy[%d] inst off = %#x, want %#x", i, got.MetaInfo.InstOff, wantOff)
		}
	}
}

// copyEffect builds one offset copy for coalescing tests.
func copyEffect(instOff uint32, dstBase, srcBase machine.Value, byteOff int) machine.CopyEffect {
	return machine.CopyEffect{
		MetaInfo: machine.Meta{InstOff: instOff},
		Dst:      copyAddressValue(dstBase, byteOff),
		Src:      copyAddressValue(srcBase, byteOff),
		Width:    2,
	}
}

// copyAddressValue advances a base copy address by byteOff bytes.
func copyAddressValue(base machine.Value, byteOff int) machine.Value {
	if byteOff == 0 {
		return machine.AddressVal(machine.MemoryAccess{Seg: machine.ConstVal(0x25), Base: base, Width: 2})
	}
	return machine.AddressVal(machine.MemoryAccess{
		Seg:   machine.ConstVal(0x25),
		Base:  machine.BinaryVal(machine.ValueOpAdd, base, machine.ConstVal(uint(byteOff))),
		Width: 2,
	})
}
