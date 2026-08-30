package asm

import "github.com/sirgwain/stars-asm/dasm/typeinfo"

// FuncContext holds shared state for decoding a single function.
// DecodeFunc populates the byte-window fields after reading from the NE image.
type FuncContext struct {
	img *ImageNE
	fs  *typeinfo.Function

	// Filled by DecodeFunc; retained for callers that need the decoded byte window.
	bytes       []byte
	baseOff     uint32
	decodeLimit uint32
	avail       uint32
}

func NewFuncContext(img *ImageNE, fs *typeinfo.Function) *FuncContext {
	return &FuncContext{
		img: img,
		fs:  fs,
	}
}
