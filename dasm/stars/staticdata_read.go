package stars

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// readGlobalBytes reads all bytes for a GlobalVAr from the NE image.
// Returns (buf, frame, off, ok).
func readGlobalBytes(img *asm.ImageNE, g *typeinfo.GlobalVar) (buf []byte, ok bool) {
	sz := g.Type.Bytes()
	if sz <= 0 {
		return nil, false
	}
	fr := int(g.Addr.Seg)
	segOff := g.Addr.Off
	b, read := img.ReadFrameBytes(fr, segOff, sz)
	if !read {
		return nil, false
	}
	return b, true
}
