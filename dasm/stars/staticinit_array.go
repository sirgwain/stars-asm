package stars

import (
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

	// Char-like byte array: try as string first.
	if typ.IsCStringArray() {
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
