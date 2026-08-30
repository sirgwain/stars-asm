package machine

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestMemoryAccessEqualsComparesAddressShape(t *testing.T) {
	a := MemoryAccess{
		Seg:    ScalarVal("ds"),
		Base:   BinaryVal(ValueOpAdd, FrameBaseVal(), ConstVal(4)),
		Disp:   8,
		Width:  2,
		Index:  ScalarVal("si"),
		Scale:  2,
		Origin: Origin{InstOff: 0x1234, Role: OperandSrc},
	}
	b := MemoryAccess{
		Seg:    ScalarVal("ds"),
		Base:   BinaryVal(ValueOpAdd, FrameBaseVal(), ConstVal(4)),
		Disp:   8,
		Width:  2,
		Index:  ScalarVal("si"),
		Scale:  2,
		Origin: Origin{InstOff: 0x1234, Role: OperandSrc},
	}
	if !a.Equals(b) {
		t.Fatalf("matching memory accesses should compare equal: %s vs %s", a, b)
	}

	b.Width = 1
	if a.Equals(b) {
		t.Fatalf("memory accesses with different widths should not compare equal: %s vs %s", a, b)
	}
}

func TestValueEqualsComparesNestedValueShape(t *testing.T) {
	a := CastVal(
		ByteWriteVal(
			WordVal(LoadVal(MemoryAccess{Base: FrameBaseVal(), Disp: -2, Width: 2}), WordLow),
			ByteHigh,
			ConstVal(0x12).WithOrigin(&Origin{InstOff: 0x2000, Role: OperandDst}),
		),
		typeinfo.U16,
	)
	b := CastVal(
		ByteWriteVal(
			WordVal(LoadVal(MemoryAccess{Base: FrameBaseVal(), Disp: -2, Width: 2}), WordLow),
			ByteHigh,
			ConstVal(0x12).WithOrigin(&Origin{InstOff: 0x2000, Role: OperandDst}),
		),
		typeinfo.U16,
	)
	if !ValueEquals(a, b) {
		t.Fatalf("matching nested values should compare equal: %s vs %s", a, b)
	}

	c := CastVal(
		ByteWriteVal(
			WordVal(LoadVal(MemoryAccess{Base: FrameBaseVal(), Disp: -2, Width: 2}), WordLow),
			ByteLow,
			ConstVal(0x12).WithOrigin(&Origin{InstOff: 0x2000, Role: OperandDst}),
		),
		typeinfo.U16,
	)
	if ValueEquals(a, c) {
		t.Fatalf("different nested byte writes should not compare equal: %s vs %s", a, c)
	}
}

func TestFarPointerValueIdentifiesOffsetAndSegment(t *testing.T) {
	parent := LoadVal(MemoryAccess{Base: FrameBaseVal(), Disp: 6, Width: 4})
	whole := FarPointerWordsVal(ConstVal(0x1234), ConstVal(0x5678))
	offset := FarPointerVal(parent, FarPointerOffset)
	segment := FarPointerVal(parent, FarPointerSegment)

	if got, want := whole.String(), "farptr(0x5678, 0x1234)"; got != want {
		t.Fatalf("whole pointer string = %q, want %q", got, want)
	}
	if got, want := offset.String(), "faroff(load(dword [bp+0x6]))"; got != want {
		t.Fatalf("offset string = %q, want %q", got, want)
	}
	if got, want := segment.String(), "farseg(load(dword [bp+0x6]))"; got != want {
		t.Fatalf("segment string = %q, want %q", got, want)
	}
	if ValueEquals(offset, segment) {
		t.Fatalf("offset and segment projections should not compare equal: %s vs %s", offset, segment)
	}
	if !ValueEquals(offset, FarPointerVal(parent, FarPointerOffset)) {
		t.Fatalf("matching far pointer projections should compare equal: %s", offset)
	}
	if !ValueEquals(whole, FarPointerWordsVal(ConstVal(0x1234), ConstVal(0x5678))) {
		t.Fatalf("matching far pointers should compare equal: %s", whole)
	}
}
