package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestElideScratchSlotsRewritesSymbolicScratch verifies scratch aliases remain
// eligible for elision after storage resolution creates SymbolRef nodes.
func TestElideScratchSlotsRewritesSymbolicScratch(t *testing.T) {
	scratch := &symresolve.SymbolScratch{
		Function:    typeinfo.Addr{Seg: 1, Off: 0x100},
		BPOffset:    -4,
		StorageSize: 2,
		TypeInfo:    typeinfo.U16,
	}
	dst := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dst", Type: typeinfo.U16}}
	block := Block{Effects: []Effect{
		&Assign{
			Dst: &SymbolRef{Path: scratch},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 7},
		},
		&Assign{
			Dst: dst,
			Src: &SymbolRef{Path: &symresolve.SymbolScratch{
				Function:    scratch.Function,
				BPOffset:    scratch.BPOffset,
				StorageSize: scratch.StorageSize,
				TypeInfo:    typeinfo.U16,
			}},
		},
	}}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(got.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(got.Effects))
	}
	if text := FormatEffect(got.Effects[0]); text != "dst = 0x7" {
		t.Fatalf("effect = %q, want %q", text, "dst = 0x7")
	}
}

// TestElideScratchSlotsReconstructsSymbolicWideValue verifies adjacent
// symbolic scratch words can satisfy a later explicit dword read.
func TestElideScratchSlotsReconstructsSymbolicWideValue(t *testing.T) {
	function := typeinfo.Addr{Seg: 1, Off: 0x100}
	src := &Local{FunctionVar: typeinfo.FunctionVar{Name: "src", Type: typeinfo.I16}}
	dst := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dst", Type: typeinfo.I32}}
	scratch := func(disp, width int, typ typeinfo.Type) *SymbolRef {
		return &SymbolRef{Path: &symresolve.SymbolScratch{
			Function:    function,
			BPOffset:    disp,
			StorageSize: width,
			TypeInfo:    typ,
		}}
	}
	block := Block{Effects: []Effect{
		&Assign{Dst: scratch(-8, 2, typeinfo.U16), Src: src},
		&Assign{
			Dst: scratch(-6, 2, typeinfo.U16),
			Src: &Word{Parent: src, Part: machine.WordSignHigh},
		},
		&Assign{Dst: dst, Src: scratch(-8, 4, typeinfo.I32)},
	}}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(got.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(got.Effects))
	}
	if text := FormatEffect(got.Effects[0]); text != "dst = sext16to32(src)" {
		t.Fatalf("effect = %q, want %q", text, "dst = sext16to32(src)")
	}
}

// TestElideScratchSlotsReconstructsSymbolicWideWords verifies an explicit
// dword read combines otherwise unrelated low and high scratch aliases.
func TestElideScratchSlotsReconstructsSymbolicWideWords(t *testing.T) {
	function := typeinfo.Addr{Seg: 1, Off: 0x100}
	dst := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dst", Type: typeinfo.U32}}
	scratch := func(disp, width int) *SymbolRef {
		return &SymbolRef{Path: &symresolve.SymbolScratch{
			Function:    function,
			BPOffset:    disp,
			StorageSize: width,
			TypeInfo:    scratchTypeForWidth(width),
		}}
	}
	block := Block{Effects: []Effect{
		&Assign{Dst: scratch(-8, 2), Src: &Const{TypeInfo: typeinfo.U16, U64: 1}},
		&Assign{Dst: scratch(-6, 2), Src: &Const{TypeInfo: typeinfo.U16, U64: 2}},
		&Assign{Dst: dst, Src: scratch(-8, 4)},
	}}

	got, changed := (&elideScratchSlotsProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	if len(got.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(got.Effects))
	}
	if text := FormatEffect(got.Effects[0]); text != "dst = 131073" {
		t.Fatalf("effect = %q, want %q", text, "dst = 131073")
	}
}
