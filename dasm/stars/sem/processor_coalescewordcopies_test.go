package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCoalesceWordCopies joins low/high word copies of one far pointer in
// either order, and leaves mismatched objects, types, and words split.
func TestCoalesceWordCopies(t *testing.T) {
	farPtr := &typeinfo.Pointer{Elem: typeinfo.I16, Class: typeinfo.PtrFar}
	dst := testLocal("lpDst", farPtr)
	src := testLocal("lpSrc", farPtr)
	other := testLocal("lpOther", farPtr)
	long := testLocal("l", typeinfo.I32)
	word := func(base LValue, off int) *Part {
		return &Part{Base: base, ByteOff: off, Width: 2, TypeInfo: typeinfo.U16}
	}
	copyWord := func(dst, src LValue, dstOff, srcOff int) *Assign {
		return &Assign{Dst: word(dst, dstOff), Src: word(src, srcOff)}
	}
	for _, tt := range []struct {
		name    string
		effects []Effect
		want    []string
	}{
		{"low then high", []Effect{copyWord(dst, src, 0, 0), copyWord(dst, src, 2, 2)}, []string{"lpDst = lpSrc"}},
		{"high then low", []Effect{copyWord(dst, src, 2, 2), copyWord(dst, src, 0, 0)}, []string{"lpDst = lpSrc"}},
		{"different sources", []Effect{copyWord(dst, src, 0, 0), copyWord(dst, other, 2, 2)}, nil},
		{"different types", []Effect{copyWord(dst, long, 0, 0), copyWord(dst, long, 2, 2)}, nil},
		{"swapped words", []Effect{copyWord(dst, src, 0, 2), copyWord(dst, src, 2, 0)}, nil},
	} {
		t.Run(tt.name, func(t *testing.T) {
			got, changed := (&coalesceWordCopiesProcessor{}).ProcessBlock(nil, Func{}, Block{ID: 1, Effects: tt.effects})
			if tt.want == nil {
				if changed || len(got.Effects) != len(tt.effects) {
					t.Fatalf("coalesced unrelated copies: %v", formatEffects(got.Effects))
				}
				return
			}
			if lines := formatEffects(got.Effects); !changed || len(lines) != len(tt.want) || lines[0] != tt.want[0] {
				t.Fatalf("effects = %v, want %v", lines, tt.want)
			}
		})
	}
}
