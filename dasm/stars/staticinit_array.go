package stars

import (
	"bytes"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// decodeArrayInitializer decodes an array type from buf.
func decodeArrayInitializer(
	img *asm.ImageNE,
	sdb *typeinfo.SymbolDB,
	loc initLocation,
	typ *typeinfo.Array,
	buf []byte,
) (*Initializer, bool) {
	count := typ.Count
	if count <= 0 {
		return nil, false
	}

	// Char-like byte array: try as string first, unless bytes after the
	// terminator hold data a string literal would drop, as in a table of
	// small values whose first entry is 0.
	if typ.IsCStringArray() && stringHoldsAllBytes(buf) {
		if str, ok := img.ReadCString(buf); ok {
			return &Initializer{Kind: InitString, Type: typ, String: str}, true
		}
	}

	// General array: decode element by element.
	elems := make([]*Initializer, count)
	for i := range elems {
		start := i * typ.Elem.Bytes()
		end := start + typ.Elem.Bytes()
		if end > len(buf) {
			break
		}
		child, ok := decodeInitializer(img, sdb, initLocation{Seg: loc.Seg, Off: loc.Off + uint32(start)}, typ.Elem, buf[start:end])
		if !ok {
			return nil, false
		}
		elems[i] = child
	}
	return &Initializer{Kind: InitArray, Type: typ, Elems: elems}, true
}

// stringHoldsAllBytes reports whether buf is a NUL-terminated string followed
// only by zero padding, so a string literal initializer reproduces it exactly.
func stringHoldsAllBytes(buf []byte) bool {
	end := bytes.IndexByte(buf, 0)
	if end < 0 {
		return false
	}
	for _, c := range buf[end:] {
		if c != 0 {
			return false
		}
	}
	return true
}
