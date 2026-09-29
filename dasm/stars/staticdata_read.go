package stars

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// ReadGlobalBytes reads all bytes for a GlobalVar from the NE image.
func ReadGlobalBytes(img *asm.ImageNE, g *typeinfo.GlobalVar) (buf []byte, ok bool) {
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
