package asm

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"os"
	"sort"
	"strings"
)

// Minimal NE loader: enough to map (segment#, segment offset) -> file offset.
//
// For Stars!' debug (unpacked) binaries, the NE segment table is authoritative.
//
// This file also parses per-segment NE relocation (fixup) tables and builds a
// best-effort mapping from the 16-bit segment words embedded in far pointers
// (which are typically *selectors*) to NE segment numbers.

// neSegEntry is one 8-byte NE segment table entry.
type neSegEntry struct {
	SectorOffset uint16 // offset in "sectors" (sector size = 1<<AlignShift)
	Length       uint16 // bytes present in the file (0 means 64KiB)
	Flags        uint16
	MinAlloc     uint16 // bytes to allocate in memory (often > Length for BSS)
}

// SegmentImage is the file-backed initialized image of one NE segment.
type SegmentImage struct {
	Data    []byte
	MemSize uint32
	Flags   uint16
}

type ImageNE struct {
	alignShift     uint16
	segmentHeaders []neSegEntry
	Segs           []SegmentImage // 1-based logical segment index => Segs[seg-1]

	// selectorToSeg maps a runtime selector to an NE segment number.
	selectorToSeg map[uint16]int

	// segToSelector is the inverse of SelectorToSeg, mapping a 1-based NE
	// segment number to its runtime selector.
	segToSelector map[uint16]uint16

	fixups []segFixups // 1-based segment => Fixups[seg-1]

	// moduleNames lists the imported module names from the NE module reference
	// table (0-indexed; fixup ModuleIdx is 1-based).
	moduleNames []string

	// importExports maps module name (uppercase) → ordinal → export entry.
	// Populated by LoadExports.
	importExports map[string]map[uint16]exportEntry
}

// ---- NE relocation (fixup) records ----

// Segment flag: relocation data present.
// (NE docs: "has relocation" bit is 0x0100.)
const segFlagsHasRelocs = 0x0100

// FixupSource describes what part of the instruction/data is relocated.
// Values per NE spec.
type FixupSource uint8

const (
	FixupSourceLowByte FixupSource = 0x00
	FixupSourceSegment FixupSource = 0x02
)

// FixupTarget describes the kind of relocation target.
type FixupTarget uint8

const (
	FixupTargetInternalRef   FixupTarget = 0x00
	FixupTargetImportOrdinal FixupTarget = 0x01
)

type Fixup struct {
	SrcOff uint16 // offset within this segment where fixup applies
	Source FixupSource
	Target FixupTarget

	Additive bool

	// Internal reference target info:
	TargetSegNum uint16 // NE segment number (1-based)
	TargetOff    uint16 // optional offset addend (used for FAR_ADDR sometimes)

	// Resolved internal reference NE address.
	ResolvedSeg uint16 // NE segment for symbol lookup/rendering
	ResolvedOff uint16 // NE segment offset for symbol lookup/rendering

	// Import ordinal target info (FixupTargetImportOrdinal):
	ModuleIdx  uint16 // module reference table index (1-based)
	Ordinal    uint16 // import ordinal number
	ModuleName string // resolved NE module reference name
	FuncName   string // resolved export name or ordinal fallback
	Purge      int    // imported function stack purge bytes (-1 = unknown)
}

func (f Fixup) String() string {
	switch f.Target {
	case FixupTargetInternalRef:
		return fmt.Sprintf("%04x:%04x", f.ResolvedSeg, f.ResolvedOff)
	case FixupTargetImportOrdinal:
		moduleName := f.ModuleName
		funcName := f.FuncName
		if moduleName == "" || funcName == "" {
			return "uknown"
		}
		return moduleName + ":" + funcName
	default:
		return "unknown"
	}
}

type segFixups struct {
	ByOff map[uint16]Fixup
}

// OpenNE opens a Win16 NE executable and reads its segment images + fixups.
func OpenNE(exePath string) (*ImageNE, error) {
	f, err := os.Open(exePath)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	// DOS MZ header: e_lfanew at 0x3c.
	var mz [64]byte
	if _, err := f.ReadAt(mz[:], 0); err != nil {
		return nil, err
	}
	if string(mz[0:2]) != "MZ" {
		return nil, fmt.Errorf("not MZ: %q", mz[0:2])
	}
	neOff := int64(binary.LittleEndian.Uint32(mz[0x3c:0x40]))
	if neOff <= 0 {
		return nil, fmt.Errorf("invalid e_lfanew: 0x%x", uint64(neOff))
	}

	// NE header fixed area.
	var neh [0x40]byte
	if _, err := f.ReadAt(neh[:], neOff); err != nil {
		return nil, err
	}
	if string(neh[0:2]) != "NE" {
		return nil, fmt.Errorf("not NE at 0x%x: %q", uint64(neOff), neh[0:2])
	}

	segCount := int(binary.LittleEndian.Uint16(neh[0x1c:0x1e]))
	segTabOff := int64(binary.LittleEndian.Uint16(neh[0x22:0x24]))
	alignShift := binary.LittleEndian.Uint16(neh[0x32:0x34])

	if segCount <= 0 {
		return nil, fmt.Errorf("NE segment count is %d", segCount)
	}

	// NE spec: an alignment shift of 0 means 9 (512-byte sectors).
	// Stars.exe uses shift=6 (64-byte sectors).
	if alignShift == 0 {
		alignShift = 9
	}

	img := &ImageNE{
		alignShift:     alignShift,
		segmentHeaders: make([]neSegEntry, segCount),
		Segs:           make([]SegmentImage, segCount),
		fixups:         make([]segFixups, segCount),
		selectorToSeg:  make(map[uint16]int),
		segToSelector:  make(map[uint16]uint16),
	}

	segTabAbs := neOff + segTabOff
	var ent [8]byte
	for i := range segCount {
		if _, err := f.ReadAt(ent[:], segTabAbs+int64(i*8)); err != nil {
			return nil, err
		}
		img.segmentHeaders[i] = neSegEntry{
			SectorOffset: binary.LittleEndian.Uint16(ent[0:2]),
			Length:       binary.LittleEndian.Uint16(ent[2:4]),
			Flags:        binary.LittleEndian.Uint16(ent[4:6]),
			MinAlloc:     binary.LittleEndian.Uint16(ent[6:8]),
		}

	}

	// Parse module reference table → imported module names.
	moduleCount := int(binary.LittleEndian.Uint16(neh[0x1e:0x20]))
	modRefTabOff := int64(binary.LittleEndian.Uint16(neh[0x28:0x2a]))
	impNamesTabOff := int64(binary.LittleEndian.Uint16(neh[0x2a:0x2c]))

	if moduleCount > 0 {
		img.moduleNames = make([]string, moduleCount)
		var mref [2]byte
		for i := 0; i < moduleCount; i++ {
			absOff := neOff + modRefTabOff + int64(i*2)
			if _, err := f.ReadAt(mref[:], absOff); err != nil {
				return nil, fmt.Errorf("read module ref %d: %w", i, err)
			}
			nameOff := int64(binary.LittleEndian.Uint16(mref[:]))
			// Name is in the imported names table: 1-byte length + N bytes.
			nameAbs := neOff + impNamesTabOff + nameOff
			var lenBuf [1]byte
			if _, err := f.ReadAt(lenBuf[:], nameAbs); err != nil {
				return nil, fmt.Errorf("read module name len %d: %w", i, err)
			}
			nameLen := int(lenBuf[0])
			if nameLen > 0 {
				nameBuf := make([]byte, nameLen)
				if _, err := f.ReadAt(nameBuf, nameAbs+1); err != nil {
					return nil, fmt.Errorf("read module name %d: %w", i, err)
				}
				img.moduleNames[i] = strings.ToUpper(string(nameBuf))
			}
		}
	}

	if err := img.loadSegmentImages(f); err != nil {
		return nil, err
	}

	if err := img.loadFixups(f); err != nil {
		return nil, err
	}
	img.buildSegToSelector()

	return img, nil
}

// readFrameBytes reads n bytes from the NE image at frame:off.
func (img *ImageNE) ReadFrameBytes(frame int, off uint32, n int) ([]byte, bool) {
	avail, ok := img.segmentAvail(frame, off)
	if !ok || avail == 0 {
		return nil, false
	}
	if int(avail) < n {
		n = int(avail)
	}
	buf := make([]byte, n)
	readN, ok := img.ReadSegmentAt(frame, off, buf)
	if readN <= 0 {
		return nil, false
	}
	if !ok {
		return nil, false
	}
	return buf[:readN], true
}

// SegmentForSelector maps a runtime selector value to a 1-based NE segment.
func (img *ImageNE) SegmentForSelector(selector uint16) (uint16, bool) {
	seg, ok := img.selectorToSeg[selector]
	return uint16(seg), ok
}

func (img *ImageNE) buildSegToSelector() {
	img.segToSelector = make(map[uint16]uint16, len(img.Segs))
	img.seedDefaultRuntimeSelectors()
}

func (img *ImageNE) seedDefaultRuntimeSelectors() {
	const selectorBase = uint16(0x0440)
	for seg := 1; seg <= len(img.Segs); seg++ {
		segWord := uint16(seg)
		selector := selectorBase + segWord*8
		if _, exists := img.selectorToSeg[selector]; !exists {
			img.selectorToSeg[selector] = seg
		}
		if _, exists := img.segToSelector[segWord]; exists {
			continue
		}
		img.segToSelector[segWord] = selector
	}
}

// SegFileOffset returns the absolute file offset for (segNum, segOff) and
// the remaining bytes available in the *file-backed* part of that segment.
//
// If segOff is within the in-memory (MinAlloc) part but beyond the file-backed
// Length (typical for BSS), this returns an error.
func (img *ImageNE) SegFileOffset(segNum int, segOff uint32) (fileOff int64, avail uint32, err error) {
	base, err := img.segBaseFileOffset(segNum)
	if err != nil {
		return 0, 0, err
	}
	fileLen, err := img.segFileLen(segNum)
	if err != nil {
		return 0, 0, err
	}
	if segOff >= fileLen {
		return 0, 0, fmt.Errorf("segment %d offset 0x%x beyond file-backed length 0x%x", segNum, segOff, fileLen)
	}
	return base + int64(segOff), fileLen - segOff, nil
}

// segmentAvail returns the loaded memory bytes available at segNum:segOff.
func (img *ImageNE) segmentAvail(segNum int, segOff uint32) (uint32, bool) {
	if segNum <= 0 || segNum > len(img.Segs) {
		return 0, false
	}
	seg := img.Segs[segNum-1]
	memSize := seg.MemSize
	if memSize == 0 {
		memSize = uint32(len(seg.Data))
	}
	if segOff >= memSize {
		return 0, false
	}
	return memSize - segOff, true
}

// segmentSlice returns an exact loaded-memory byte slice at segNum:segOff.
// Bytes past the file-backed data and within MemSize are the NE segment's
// zero-filled allocation tail.
func (img *ImageNE) segmentSlice(segNum int, segOff uint32, n int) ([]byte, bool) {
	if n < 0 {
		return nil, false
	}
	if segNum <= 0 || segNum > len(img.Segs) {
		return nil, false
	}
	seg := img.Segs[segNum-1]
	end := uint64(segOff) + uint64(n)
	memSize := uint64(seg.MemSize)
	if memSize == 0 {
		memSize = uint64(len(seg.Data))
	}
	if end > memSize {
		return nil, false
	}
	if end <= uint64(len(seg.Data)) {
		return seg.Data[int(segOff):int(end)], true
	}
	buf := make([]byte, n)
	if segOff < uint32(len(seg.Data)) {
		copy(buf, seg.Data[int(segOff):])
	}
	return buf, true
}

// segmentFileSlice returns an exact file-backed byte slice at segNum:segOff.
func (img *ImageNE) segmentFileSlice(segNum int, segOff uint32, n int) ([]byte, bool) {
	if n < 0 {
		return nil, false
	}
	if segNum <= 0 || segNum > len(img.Segs) {
		return nil, false
	}
	seg := img.Segs[segNum-1]
	end := uint64(segOff) + uint64(n)
	if end > uint64(len(seg.Data)) {
		return nil, false
	}
	return seg.Data[int(segOff):int(end)], true
}

// ReadSegmentAt copies initialized bytes from segNum:segOff into dst.
func (img *ImageNE) ReadSegmentAt(segNum int, segOff uint32, dst []byte) (int, bool) {
	if len(dst) == 0 {
		return 0, true
	}
	avail, ok := img.segmentAvail(segNum, segOff)
	if !ok {
		return 0, false
	}
	n := len(dst)
	if uint32(n) > avail {
		n = int(avail)
	}
	b, ok := img.segmentSlice(segNum, segOff, n)
	if !ok {
		return 0, false
	}
	return copy(dst, b), n > 0
}

func (img *ImageNE) ReadCStringAt(seg uint16, off uint32) (string, bool) {
	if seg == 0 {
		return "", false
	}
	buf, ok := img.ReadFrameBytes(int(seg), off, 256)
	if !ok {
		return "", false
	}
	return img.ReadCString(buf)
}

// ReadCString reads a null-terminated C string from frame:off in the NE image.
// maxRead limits how many bytes are searched for the terminator.
func (img *ImageNE) ReadCString(buf []byte) (string, bool) {
	end := bytes.IndexByte(buf, 0)
	if end < 0 {
		return "", false
	}
	return quoteCString(buf[:end])
}

// quoteCString converts a raw byte slice to a C string literal (with quotes).
// Returns ("", false) if the bytes contain non-printable characters.
func quoteCString(raw []byte) (string, bool) {
	var b strings.Builder
	b.Grow(len(raw) + 2)
	b.WriteByte('"')
	for _, c := range raw {
		switch c {
		case '\\':
			b.WriteString("\\\\")
		case '"':
			b.WriteString("\\\"")
		case '\n':
			b.WriteString("\\n")
		case '\r':
			b.WriteString("\\r")
		case '\t':
			b.WriteString("\\t")
		default:
			if c < 0x20 || c >= 0x7f {
				return "", false
			}
			b.WriteByte(c)
		}
	}
	b.WriteByte('"')
	return b.String(), true
}

// FixupAt returns the relocation record (if any) for the given segment-relative offset.
func (img *ImageNE) FixupAt(segNum int, segOff uint16) (Fixup, bool) {
	if segNum <= 0 || segNum > len(img.fixups) {
		return Fixup{}, false
	}
	fx := img.fixups[segNum-1]
	if fx.ByOff == nil {
		return Fixup{}, false
	}
	f, ok := fx.ByOff[segOff]
	return f, ok
}

// Fixups returns all relocation records for the given segment, sorted by source offset.
func (img *ImageNE) Fixups(segNum int) []Fixup {
	if segNum <= 0 || segNum > len(img.fixups) {
		return nil
	}
	fx := img.fixups[segNum-1]
	if fx.ByOff == nil {
		return nil
	}
	out := make([]Fixup, 0, len(fx.ByOff))
	for _, f := range fx.ByOff {
		out = append(out, f)
	}
	sort.Slice(out, func(i, j int) bool {
		return out[i].SrcOff < out[j].SrcOff
	})
	return out
}

// ---- internals ----

// loadFixups parses per-segment relocation data (if present) and stores it in img.Fixups.
func (img *ImageNE) loadFixups(f *os.File) error {
	var hdr [2]byte
	var rec [8]byte

	for segNum := 1; segNum <= len(img.Segs); segNum++ {
		se := img.segmentHeaders[segNum-1]
		if se.Flags&segFlagsHasRelocs == 0 {
			continue
		}

		base, err := img.segBaseFileOffset(segNum)
		if err != nil {
			return err
		}
		fileLen, err := img.segFileLen(segNum)
		if err != nil {
			return err
		}

		// Relocation table begins immediately after file-backed segment bytes.
		relOff := base + int64(fileLen)
		if _, err := f.ReadAt(hdr[:], relOff); err != nil {
			return fmt.Errorf("read seg %d fixup count: %w", segNum, err)
		}
		count := int(binary.LittleEndian.Uint16(hdr[:]))
		if count <= 0 {
			continue
		}

		fx := segFixups{ByOff: make(map[uint16]Fixup, count)}
		cur := relOff + 2

		for i := range count {
			off := cur + int64(i*8)
			if _, err := f.ReadAt(rec[:], off); err != nil {
				return fmt.Errorf("read seg %d fixup rec %d: %w", segNum, i, err)
			}

			src := FixupSource(rec[0])
			flags := rec[1]
			tgt := FixupTarget(flags & 0x03)
			additive := (flags & 0x04) != 0
			srcOff := binary.LittleEndian.Uint16(rec[2:4])

			f := Fixup{SrcOff: srcOff, Source: src, Target: tgt, Additive: additive}
			switch tgt {
			case FixupTargetInternalRef:
				f.TargetSegNum = binary.LittleEndian.Uint16(rec[4:6])
				f.TargetOff = binary.LittleEndian.Uint16(rec[6:8])
			case FixupTargetImportOrdinal:
				f.ModuleIdx = binary.LittleEndian.Uint16(rec[4:6])
				f.Ordinal = binary.LittleEndian.Uint16(rec[6:8])
				f.ModuleName = img.moduleName(f.ModuleIdx)
			}

			if additive {
				// Additive fixups: single site, no chain.
				fx.ByOff[srcOff] = f
			} else {
				// Non-additive fixups: SrcOff is the head of a chain.
				// At each site, the word at the fixup location is the offset
				// of the next site in the chain (terminated by 0xFFFF).
				chainOff := srcOff
				for range 4096 { // safety limit
					site := f
					site.SrcOff = chainOff
					fx.ByOff[chainOff] = site

					// Read the chain pointer at this site.
					// For FAR_ADDR the chain word is at the offset field (chainOff),
					// for SEGMENT it's at chainOff itself.
					w, ok := img.segmentFileSlice(segNum, uint32(chainOff), 2)
					if !ok {
						break
					}
					next := binary.LittleEndian.Uint16(w)
					if next == 0xFFFF {
						break
					}
					chainOff = next
				}
			}
		}

		img.fixups[segNum-1] = fx
	}

	return nil
}

func (img *ImageNE) loadSegmentImages(f *os.File) error {
	for segNum := 1; segNum <= len(img.segmentHeaders); segNum++ {
		base, err := img.segBaseFileOffset(segNum)
		if err != nil {
			return err
		}
		fileLen, err := img.segFileLen(segNum)
		if err != nil {
			return err
		}
		memLen, err := img.segMemLen(segNum)
		if err != nil {
			return err
		}
		se := img.segmentHeaders[segNum-1]
		data := make([]byte, fileLen)
		if fileLen > 0 {
			if _, err := f.ReadAt(data, base); err != nil {
				return fmt.Errorf("read seg %d data: %w", segNum, err)
			}
		}
		img.Segs[segNum-1] = SegmentImage{
			Data:    data,
			MemSize: memLen,
			Flags:   se.Flags,
		}
	}
	return nil
}

// segBaseFileOffset returns the absolute file offset of the start of segNum (1-based).
func (img *ImageNE) segBaseFileOffset(segNum int) (int64, error) {
	se, err := img.segEntry(segNum)
	if err != nil {
		return 0, err
	}
	sectorSize := int64(1) << img.alignShift
	return int64(se.SectorOffset) * sectorSize, nil
}

// segEntry fetches a 1-based segment entry with range checking.
func (img *ImageNE) segEntry(segNum int) (neSegEntry, error) {
	if segNum <= 0 || segNum > len(img.segmentHeaders) {
		return neSegEntry{}, fmt.Errorf("segment %d out of range [1..%d]", segNum, len(img.segmentHeaders))
	}
	return img.segmentHeaders[segNum-1], nil
}

// segFileLen returns the number of bytes for segNum that are present in the file.
// (NE Length==0 means 64KiB.)
func (img *ImageNE) segFileLen(segNum int) (uint32, error) {
	se, err := img.segEntry(segNum)
	if err != nil {
		return 0, err
	}
	return uint32(se.Length), nil
}

// segMemLen returns the number of bytes Windows will allocate for the segment in memory.
func (img *ImageNE) segMemLen(segNum int) (uint32, error) {
	se, err := img.segEntry(segNum)
	if err != nil {
		return 0, err
	}
	return uint32(se.MinAlloc), nil
}
