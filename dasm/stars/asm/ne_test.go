package asm

import (
	"bytes"
	"testing"
)

func TestReadFrameBytesZeroFillsAllocatedTail(t *testing.T) {
	img := &ImageNE{
		Segs: []SegmentImage{
			{
				Data:    []byte{0x11, 0x22, 0x33},
				MemSize: 8,
			},
		},
	}

	got, ok := img.ReadFrameBytes(1, 5, 2)
	if !ok {
		t.Fatal("ReadFrameBytes() failed for allocated zero-filled tail")
	}
	if want := []byte{0x00, 0x00}; !bytes.Equal(got, want) {
		t.Fatalf("ReadFrameBytes() = % x, want % x", got, want)
	}
}

func TestReadFrameBytesSpansFileDataAndZeroFill(t *testing.T) {
	img := &ImageNE{
		Segs: []SegmentImage{
			{
				Data:    []byte{0x11, 0x22, 0x33},
				MemSize: 6,
			},
		},
	}

	got, ok := img.ReadFrameBytes(1, 2, 3)
	if !ok {
		t.Fatal("ReadFrameBytes() failed for range spanning file data and zero fill")
	}
	if want := []byte{0x33, 0x00, 0x00}; !bytes.Equal(got, want) {
		t.Fatalf("ReadFrameBytes() = % x, want % x", got, want)
	}
}

func TestSegmentFileSliceRejectsAllocatedTail(t *testing.T) {
	img := &ImageNE{
		Segs: []SegmentImage{
			{
				Data:    []byte{0x11, 0x22, 0x33},
				MemSize: 8,
			},
		},
	}

	if _, ok := img.segmentFileSlice(1, 3, 1); ok {
		t.Fatal("segmentFileSlice() succeeded beyond file-backed data")
	}
}
