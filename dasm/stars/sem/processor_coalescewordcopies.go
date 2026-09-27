package sem

import "github.com/sirgwain/stars-asm/dasm/typeinfo"

// coalesceWordCopiesProcessor joins a 32-bit copy that the compiler lowered to
// adjacent low-word and high-word assignments between the same two objects.
// Machine-level store collapse cannot prove these when the destination base is
// a value without a symbol path, such as a far pointer returned by a call.
type coalesceWordCopiesProcessor struct{}

// ProcessBlock replaces each adjacent low/high word copy pair with one whole
// assignment.
func (p *coalesceWordCopiesProcessor) ProcessBlock(_ *Result, _ Func, b Block) (Block, bool) {
	changed := false
	effects := make([]Effect, 0, len(b.Effects))
	for i := 0; i < len(b.Effects); i++ {
		if i+1 < len(b.Effects) {
			if copy, ok := wordCopyPair(b.Effects[i], b.Effects[i+1]); ok {
				effects = append(effects, copy)
				changed = true
				i++
				continue
			}
		}
		effects = append(effects, b.Effects[i])
	}
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// wordCopyPair returns the whole assignment for two adjacent word copies that
// move both words of one 32-bit value into the matching words of one object of
// the same type, in either order.
func wordCopyPair(first, second Effect) (*Assign, bool) {
	a, ok := first.(*Assign)
	if !ok {
		return nil, false
	}
	b, ok := second.(*Assign)
	if !ok {
		return nil, false
	}
	aDst, aDstOK := a.Dst.(*Part)
	bDst, bDstOK := b.Dst.(*Part)
	aSrc, aSrcOK := a.Src.(*Part)
	bSrc, bSrcOK := b.Src.(*Part)
	if !aDstOK || !bDstOK || !aSrcOK || !bSrcOK {
		return nil, false
	}
	for _, part := range []*Part{aDst, bDst, aSrc, bSrc} {
		if part.Width != 2 {
			return nil, false
		}
	}
	if aDst.ByteOff != aSrc.ByteOff || bDst.ByteOff != bSrc.ByteOff || aDst.ByteOff+bDst.ByteOff != 2 || aDst.ByteOff == bDst.ByteOff {
		return nil, false
	}
	dst, src := aDst.Base, aSrc.Base
	dstType, srcType := dst.ExprType(), src.ExprType()
	if dstType == nil || dstType.Bytes() != 4 || !typeinfo.Equals(dstType, srcType) {
		return nil, false
	}
	if !sameLValue(dst, bDst.Base) || !sameLValue(src, bSrc.Base) {
		return nil, false
	}
	return &Assign{MetaInfo: a.MetaInfo, Dst: dst, Src: src}, true
}
