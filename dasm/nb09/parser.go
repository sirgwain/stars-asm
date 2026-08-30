// Package nb09 parses NB09 debug database blobs into structured directory,
// subsection, type, symbol, line, and procedure metadata for disassembly tools.
package nb09

import (
	"encoding/binary"
	"fmt"
	"log/slog"
)

// le is a shorthand for binary.LittleEndian.
var le = binary.LittleEndian

// parseNB09 validates the NB09 header, reads the directory table, slices out
// each subsection, and decodes module, library, segment, symbol, type, and
// source-line metadata into an NB09DB.
func parseNB09(blob []byte) (NB09DB, error) {
	var out NB09DB

	if len(blob) < 8 {
		return out, fmt.Errorf("nb09: blob too small (%d)", len(blob))
	}

	// ---- NB09 header ----
	copy(out.Header.Sig[:], blob[0:4])
	if string(out.Header.Sig[:]) != "NB09" {
		return out, fmt.Errorf("nb09: bad signature %q (want %q)", string(out.Header.Sig[:]), "NB09")
	}
	out.Header.LfoDir = le.Uint32(blob[4:8])

	lfoDir := int(out.Header.LfoDir)
	if lfoDir < 0 || lfoDir+16 > len(blob) {
		return out, fmt.Errorf("nb09: lfoDir out of range: 0x%x (len=0x%x)", out.Header.LfoDir, len(blob))
	}

	// ---- Directory header ----
	dh := DirHeader{
		CBDirHeader: le.Uint16(blob[lfoDir+0 : lfoDir+2]),
		CBDirEntry:  le.Uint16(blob[lfoDir+2 : lfoDir+4]),
		CDir:        le.Uint32(blob[lfoDir+4 : lfoDir+8]),
		LfoNextDir:  le.Uint32(blob[lfoDir+8 : lfoDir+12]),
		Flags:       le.Uint32(blob[lfoDir+12 : lfoDir+16]),
	}
	out.Dir.Header = dh
	slog.Debug("nb09: directory header",
		"cbDirHeader", fmt.Sprintf("0x%X", dh.CBDirHeader),
		"cbDirEntry", fmt.Sprintf("0x%X", dh.CBDirEntry),
		"cDir", dh.CDir,
		"lfoNextDir", fmt.Sprintf("0x%X", dh.LfoNextDir),
		"flags", fmt.Sprintf("0x%X", dh.Flags),
	)

	if dh.CBDirHeader != 0x10 {
		return out, fmt.Errorf("nb09: unexpected CBDirHeader=0x%x (want 0x10)", dh.CBDirHeader)
	}
	if dh.CBDirEntry < 0x0C {
		return out, fmt.Errorf("nb09: unexpected CBDirEntry=0x%x (<0x0C)", dh.CBDirEntry)
	}

	// ---- Directory entries ----
	entryOff := lfoDir + int(dh.CBDirHeader)
	wantBytes := int(dh.CDir) * int(dh.CBDirEntry)
	if entryOff < 0 || entryOff+wantBytes > len(blob) {
		return out, fmt.Errorf("nb09: directory entries out of range: off=0x%x bytes=0x%x len=0x%x",
			entryOff, wantBytes, len(blob))
	}

	out.Dir.Entries = make([]DirEntry, 0, dh.CDir)
	out.subsections = make([]RawSubsection, 0, dh.CDir)

	for i := 0; i < int(dh.CDir); i++ {
		off := entryOff + i*int(dh.CBDirEntry)

		sub := le.Uint16(blob[off+0 : off+2])
		imod := le.Uint16(blob[off+2 : off+4])
		lfo := le.Uint32(blob[off+4 : off+8])
		cb := le.Uint32(blob[off+8 : off+12])

		de := DirEntry{
			Subsection: SubsectionKind(sub),
			IMod:       imod,
			Lfo:        lfo,
			CB:         cb,
		}
		out.Dir.Entries = append(out.Dir.Entries, de)

		if cb == 0 {
			out.subsections = append(out.subsections, RawSubsection{
				Kind: SubsectionKind(sub),
				IMod: imod,
				Lfo:  lfo,
				Data: nil,
			})
			continue
		}

		l := int(lfo)
		n := int(cb)
		if l < 0 || n < 0 || l+n > len(blob) {
			return out, fmt.Errorf("nb09: subsection out of range: sst=0x%x imod=%d lfo=0x%x cb=0x%x (len=0x%x)",
				sub, imod, lfo, cb, len(blob))
		}

		out.subsections = append(out.subsections, RawSubsection{
			Kind: SubsectionKind(sub),
			IMod: imod,
			Lfo:  lfo,
			Data: blob[l : l+n],
		})
	}

	// ---- Decode subsections ----
	out.Modules = make(map[uint16]SstModule)
	out.AlignSyms = make(map[uint16]SymStream)
	out.SrcModules = make(map[uint16]SstSrcModule)

	for _, subsection := range out.subsections {
		if subsection.Data == nil {
			continue
		}
		// logger.Debug("nb09: parsing subsection")
		switch subsection.Kind {
		case SST_MODULE:
			mod, err := parseModule(subsection.Data)
			if err != nil {
				return out, fmt.Errorf("nb09: parseModule imod=%d: %w", subsection.IMod, err)
			}
			out.Modules[subsection.IMod] = mod

		case SST_LIBRARIES:
			out.Libraries = parseLibraries(subsection.Data)

		case SST_SEG_NAME:
			out.SegNames = parseSegName(subsection.Data)

		case SST_SEG_MAP:
			sm, err := parseSegMap(subsection.Data)
			if err != nil {
				return out, fmt.Errorf("nb09: parseSegMap: %w", err)
			}
			out.SegMap = sm

		case SST_FILE_INDEX:
			fi, err := parseFileIndex(subsection.Data)
			if err != nil {
				return out, fmt.Errorf("nb09: parseFileIndex: %w", err)
			}
			out.FileIndex = fi

		case SST_ALIGN_SYM:
			syms, err := parseSymStream(subsection.Data)
			if err != nil {
				return out, fmt.Errorf("nb09: parseSymStream (alignSym imod=%d): %w", subsection.IMod, err)
			}
			out.AlignSyms[subsection.IMod] = syms

		case SST_GLOBAL_PUB:
			hss, err := parseHashSymSubsection(subsection.Data)
			if err != nil {
				return out, fmt.Errorf("nb09: parseHashSymSubsection (globalPub): %w", err)
			}
			out.GlobalPub = hss

		case SST_GLOBAL_SYM:
			hss, err := parseHashSymSubsection(subsection.Data)
			if err != nil {
				return out, fmt.Errorf("nb09: parseHashSymSubsection (globalSym): %w", err)
			}
			out.GlobalSym = hss

		case SST_STATIC_SYM:
			hss, err := parseHashSymSubsection(subsection.Data)
			if err != nil {
				return out, fmt.Errorf("nb09: parseHashSymSubsection (staticSym): %w", err)
			}
			out.StaticSym = hss

		case SST_GLOBAL_TYPES:
			ts, err := parseTypeStream(subsection.Data)
			if err != nil {
				return out, fmt.Errorf("nb09: parseTypeStream: %w", err)
			}
			out.GlobalTypes = ts

		case SST_SRC_MODULE:
			src, err := parseSrcModule(subsection.Data)
			if err != nil {
				return out, fmt.Errorf("nb09: parseSrcModule imod=%d: %w", subsection.IMod, err)
			}
			out.SrcModules[subsection.IMod] = src

		default:
			slog.Warn("nb09: uknown", "kind", subsection.Kind.String())
		}

	}

	return out, nil
}

// ---------- helpers ----------

// readPascalString reads a one-byte-length-prefixed string and returns the
// string plus the number of bytes consumed from the input buffer.
func readPascalString(data []byte) (string, int) {
	if len(data) < 1 {
		return "", 0
	}
	n := int(data[0])
	if 1+n > len(data) {
		return string(data[1:]), len(data)
	}
	return string(data[1 : 1+n]), 1 + n
}

// ---------- sstModule ----------

// parseModule decodes an sstModule subsection, extracting overlay/library
// identifiers, segment contribution records, style, and the module name.
func parseModule(data []byte) (SstModule, error) {
	if len(data) < 8 {
		return SstModule{}, fmt.Errorf("sstModule too short (%d)", len(data))
	}
	m := SstModule{
		Ovl:   le.Uint16(data[0:2]),
		ILib:  le.Uint16(data[2:4]),
		CSeg:  le.Uint16(data[4:6]),
		Style: le.Uint16(data[6:8]),
	}
	off := 8
	for i := 0; i < int(m.CSeg); i++ {
		if off+12 > len(data) {
			return m, fmt.Errorf("sstModule seginfo truncated at seg %d", i)
		}
		si := ModuleSegInfo{
			Seg: le.Uint16(data[off : off+2]),
			Pad: le.Uint16(data[off+2 : off+4]),
			Off: le.Uint32(data[off+4 : off+8]),
			CB:  le.Uint32(data[off+8 : off+12]),
		}
		m.SegInfo = append(m.SegInfo, si)
		off += 12
	}
	if off < len(data) {
		m.Name, _ = readPascalString(data[off:])
	}
	return m, nil
}

// ---------- sstLibraries ----------

// parseLibraries decodes an sstLibraries subsection into the list of Pascal
// strings naming libraries referenced by the debug database.
func parseLibraries(data []byte) []string {
	var libs []string
	off := 0
	for off < len(data) {
		s, n := readPascalString(data[off:])
		if n == 0 {
			break
		}
		if s != "" {
			libs = append(libs, s)
		}
		off += n
	}
	return libs
}

// ---------- sstSegName ----------

// parseSegName decodes an sstSegName subsection into a map from name-table
// offsets to NUL-terminated segment and class name strings.
func parseSegName(data []byte) SstSegName {
	sn := SstSegName{At: make(map[uint32]string)}
	off := 0
	for off < len(data) {
		start := off
		// find NUL
		end := off
		for end < len(data) && data[end] != 0 {
			end++
		}
		s := string(data[start:end])
		sn.At[uint32(start)] = s
		off = end + 1 // skip NUL
	}
	return sn
}

// ---------- sstSegMap ----------

// parseSegMap decodes an sstSegMap subsection, extracting logical segment
// descriptors such as flags, overlay, group, frame, name indexes, offsets, and
// byte lengths.
func parseSegMap(data []byte) (SstSegMap, error) {
	if len(data) < 4 {
		return SstSegMap{}, fmt.Errorf("sstSegMap too short (%d)", len(data))
	}
	sm := SstSegMap{
		CSeg:    le.Uint16(data[0:2]),
		CSegLog: le.Uint16(data[2:4]),
	}
	off := 4
	for i := 0; i < int(sm.CSeg); i++ {
		if off+20 > len(data) {
			return sm, fmt.Errorf("sstSegMap desc truncated at seg %d", i)
		}
		d := SegMapDesc{
			Flags:      le.Uint16(data[off : off+2]),
			Ovl:        le.Uint16(data[off+2 : off+4]),
			Group:      le.Uint16(data[off+4 : off+6]),
			Frame:      le.Uint16(data[off+6 : off+8]),
			ISegName:   le.Uint16(data[off+8 : off+10]),
			IClassName: le.Uint16(data[off+10 : off+12]),
			Off:        le.Uint32(data[off+12 : off+16]),
			CB:         le.Uint32(data[off+16 : off+20]),
		}
		sm.Segs = append(sm.Segs, d)
		off += 20
	}
	return sm, nil
}

// ---------- sstFileIndex ----------

// parseFileIndex decodes an sstFileIndex subsection, extracting module-to-file
// reference counts, file-name offsets, resolved file names, and the per-module
// source file mapping.
func parseFileIndex(data []byte) (SstFileIndex, error) {
	if len(data) < 4 {
		return SstFileIndex{}, fmt.Errorf("sstFileIndex too short (%d)", len(data))
	}

	fi := SstFileIndex{
		CMod: le.Uint16(data[0:2]),
		CRef: le.Uint16(data[2:4]),
	}
	off := 4

	// ModStart array: CMod × u16
	fi.ModStart = make([]uint16, fi.CMod)
	for i := range fi.ModStart {
		if off+2 > len(data) {
			return fi, fmt.Errorf("sstFileIndex modStart truncated (i=%d off=0x%x len=%d)", i, off, len(data))
		}
		fi.ModStart[i] = le.Uint16(data[off : off+2])
		off += 2
	}

	// RefCnt array: CMod × u16
	fi.RefCnt = make([]uint16, fi.CMod)
	for i := range fi.RefCnt {
		if off+2 > len(data) {
			return fi, fmt.Errorf("sstFileIndex refCnt truncated (i=%d off=0x%x len=%d)", i, off, len(data))
		}
		fi.RefCnt[i] = le.Uint16(data[off : off+2])
		off += 2
	}

	// NameRef array: CRef × u32
	fi.NameRef = make([]uint32, fi.CRef)
	for i := range fi.NameRef {
		if off+4 > len(data) {
			return fi, fmt.Errorf("sstFileIndex nameRef truncated (i=%d off=0x%x len=%d)", i, off, len(data))
		}
		fi.NameRef[i] = le.Uint32(data[off : off+4])
		off += 4
	}

	// Names blob starts here; NameRef offsets are RELATIVE to this point.
	namesBase := off
	namesBlob := data[namesBase:]

	// Names: resolve NameRef offsets to 1-byte-length strings within namesBlob
	fi.Names = make([]string, fi.CRef)
	for i, ref := range fi.NameRef {
		r := int(ref)
		if r < 0 || r >= len(namesBlob) {
			// leave empty; or record an error if you want strictness
			continue
		}
		s, _ := readPascalString(namesBlob[r:]) // ref points at length byte
		fi.Names[i] = s
	}

	// ModToFiles mapping
	fi.ModToFiles = make(map[uint16][]string, fi.CMod)
	for i := 0; i < int(fi.CMod); i++ {
		imod := uint16(i + 1) // 1-based
		start := int(fi.ModStart[i])
		count := int(fi.RefCnt[i])

		// Clamp to CRef to avoid panics on malformed data
		if start < 0 {
			start = 0
		}
		end := start + count
		if end > int(fi.CRef) {
			end = int(fi.CRef)
		}
		if start > end {
			start = end
		}

		files := make([]string, 0, end-start)
		for j := start; j < end; j++ {
			files = append(files, fi.Names[j])
		}
		fi.ModToFiles[imod] = files
	}

	return fi, nil
}

// ---------- Symbol stream ----------

// parseSymStream decodes a stream of CodeView symbol records, preserving each
// record's stream offset, length, type, raw payload, and parsed payload when the
// record type is recognized.
func parseSymStream(data []byte) (SymStream, error) {
	var ss SymStream
	off := 0
	for off+4 <= len(data) {
		recOff := uint32(off)
		recLen := le.Uint16(data[off : off+2])
		if recLen < 2 {
			// recLen includes rectyp (2 bytes); skip zero/padding
			off += 2
			continue
		}
		if off+2+int(recLen) > len(data) {
			break
		}
		recTyp := SymRecType(le.Uint16(data[off+2 : off+4]))
		payload := data[off+4 : off+2+int(recLen)]

		rec := SymRecord{
			Offset: recOff,
			RecLen: recLen,
			RecTyp: recTyp,
			Raw:    payload,
			Parsed: parseSymPayload(recTyp, payload),
		}
		ss.Records = append(ss.Records, rec)
		off += 2 + int(recLen)

	}
	return ss, nil
}

// parseSymPayload dispatches a symbol record payload to the parser for its
// concrete CodeView symbol kind, such as registers, locals, publics,
// procedures, thunks, blocks, labels, references, compile data, and searches.
func parseSymPayload(rt SymRecType, d []byte) any {
	switch rt {
	case S_REGISTER:
		return parseRegister(d)
	case S_BPREL16:
		return parseBpRel16(d)
	case S_LDATA16, S_GDATA16:
		return parseData16(rt, d)
	case S_PUB16:
		return parsePub16(d)
	case S_LPROC16, S_GPROC16:
		return parseProc16(d)
	case S_BLOCK16:
		return parseBlock16(d)
	case S_LABEL16:
		return parseLabel16(d)
	case S_UDT:
		return parseUdt(d)
	case S_OBJNAME:
		return parseObjName(d)
	case S_PUB32:
		return parsePub32(d)
	case S_PROCREF:
		return parseProcRef(d)
	case S_DATAREF:
		return parseDataRef(d)
	case S_COMPILE:
		return parseCompile(d)
	case S_SSEARCH:
		return parseSearch(d)
	case S_END, S_ENDARG, S_ALIGN:
		return nil
	default:
		slog.Warn("unknown record type found", "rt", rt, "bytes", len(d))
		return nil
	}
}

// parseData16 decodes an S_LDATA16 or S_GDATA16 record, extracting whether the
// data symbol is global, its 16-bit segment address, type index, and name.
func parseData16(rt SymRecType, d []byte) *SData16 {
	if len(d) < 6 {
		return nil
	}
	s := &SData16{
		IsGlobal: rt == S_GDATA16,
		Addr:     Addr16{Off: le.Uint16(d[0:2]), Seg: le.Uint16(d[2:4])},
		TypInd:   le.Uint16(d[4:6]),
	}
	if len(d) > 6 {
		s.Name, _ = readPascalString(d[6:])
	}
	return s
}

// parsePub16 decodes an S_PUB16 record, extracting the public symbol's 16-bit
// segment address, type index, and name.
func parsePub16(d []byte) *SPub16 {
	if len(d) < 6 {
		return nil
	}
	s := &SPub16{
		Addr:   Addr16{Off: le.Uint16(d[0:2]), Seg: le.Uint16(d[2:4])},
		TypInd: le.Uint16(d[4:6]),
	}
	if len(d) > 6 {
		s.Name, _ = readPascalString(d[6:])
	}
	return s
}

// parseProc16 decodes an S_LPROC16 or S_GPROC16 record, extracting parent/end/
// next symbol links, procedure length, debug range, 16-bit address, type index,
// flags, and procedure name.
func parseProc16(d []byte) *SProc16 {
	if len(d) < 23 {
		return nil
	}
	s := &SProc16{
		PParent:  le.Uint32(d[0:4]),
		PEnd:     le.Uint32(d[4:8]),
		PNext:    le.Uint32(d[8:12]),
		ProcLen:  le.Uint16(d[12:14]),
		DbgStart: le.Uint16(d[14:16]),
		DbgEnd:   le.Uint16(d[16:18]),
		Addr:     Addr16{Off: le.Uint16(d[18:20]), Seg: le.Uint16(d[20:22])},
		TypInd:   le.Uint16(d[22:24]),
		Flags:    d[24],
	}
	if len(d) > 25 {
		s.Name, _ = readPascalString(d[25:])
	}
	return s
}

// parseBlock16 decodes an S_BLOCK16 record, extracting the parent/end links,
// block length, 16-bit address, and optional block name.
func parseBlock16(d []byte) *SBlock16 {
	if len(d) < 14 {
		return nil
	}
	s := &SBlock16{
		PParent: le.Uint32(d[0:4]),
		PEnd:    le.Uint32(d[4:8]),
		Len:     le.Uint16(d[8:10]),
		Addr:    Addr16{Off: le.Uint16(d[10:12]), Seg: le.Uint16(d[12:14])},
	}
	if len(d) > 14 {
		s.Name, _ = readPascalString(d[14:])
	}
	return s
}

// parseLabel16 decodes an S_LABEL16 record, extracting the 16-bit label
// address, flags, and label name.
func parseLabel16(d []byte) *SLabel16 {
	if len(d) < 5 {
		return nil
	}
	s := &SLabel16{
		Addr:  Addr16{Off: le.Uint16(d[0:2]), Seg: le.Uint16(d[2:4])},
		Flags: d[4],
	}
	if len(d) > 5 {
		s.Name, _ = readPascalString(d[5:])
	}
	return s
}

// parseSearch decodes an S_SSEARCH record, extracting the segment address used
// to find a symbol scope and, when present, the symbol offset.
func parseSearch(d []byte) *SSSearch {
	// minimal: addr16 (4 bytes)
	if len(d) < 4 {
		return nil
	}
	s := &SSSearch{
		Addr: Addr16{
			Off: le.Uint16(d[0:2]),
			Seg: le.Uint16(d[2:4]),
		},
	}
	if len(d) >= 6 {
		s.SymOff = le.Uint16(d[4:6])
	}
	return s
}

// parseCompile decodes an S_COMPILE record, extracting compiler flags, an
// optional target-machine value, and the remaining compiler/version string.
func parseCompile(d []byte) *SCompile {
	// We want to be robust across slight layout variants.
	// Commonly:
	//   u16 flags
	//   [u16 machine] (optional / toolchain-dependent)
	//   string (NUL-terminated or length-prefixed/pascal)
	if len(d) < 2 {
		return nil
	}

	s := &SCompile{
		Flags: le.Uint16(d[0:2]),
	}

	rest := d[2:]

	// If we have at least 2 more bytes, treat them as Machine.
	// If this turns out wrong for your specific toolchain, it's still harmless
	// (you'll see weird machine values), and you can gate it behind heuristics.
	if len(rest) >= 2 {
		s.Machine = le.Uint16(rest[0:2])
		rest = rest[2:]
	}

	// Extract a version/compiler string from whatever remains.
	// Try, in order:
	//   1) Pascal string (your existing helper) if it looks plausible
	//   2) NUL-terminated C-string
	//   3) Raw bytes as printable subset
	if len(rest) == 0 {
		return s
	}

	// 1) try pascal (length-prefixed) first — only if length byte is plausible
	if len(rest) >= 1 {
		n := int(rest[0])
		if n > 0 && n <= len(rest)-1 {
			if v, n := readPascalString(rest); n >= 0 {
				s.Version = v
				return s
			}
		}
	}

	// 3) fallback: just take it as-is
	s.Version = string(rest)
	return s
}

// parseUdt decodes an S_UDT record, extracting the user-defined type index and
// its type name.
func parseUdt(d []byte) *SUdt {
	if len(d) < 2 {
		return nil
	}
	s := &SUdt{TypInd: le.Uint16(d[0:2])}
	if len(d) > 2 {
		s.Name, _ = readPascalString(d[2:])
	}
	return s
}

// parseObjName decodes an S_OBJNAME record, extracting the object signature and
// object-file name.
func parseObjName(d []byte) *SObjName {
	if len(d) < 4 {
		return nil
	}
	s := &SObjName{Signature: le.Uint32(d[0:4])}
	if len(d) > 4 {
		s.Name, _ = readPascalString(d[4:])
	}
	return s
}

// parsePub32 decodes an S_PUB32 record, extracting the public symbol's 32-bit
// offset, segment, type index, and name.
func parsePub32(d []byte) *SPub32 {
	if len(d) < 8 {
		return nil
	}
	s := &SPub32{
		Off:    le.Uint32(d[0:4]),
		Seg:    le.Uint16(d[4:6]),
		TypInd: le.Uint16(d[6:8]),
	}
	if len(d) > 8 {
		s.Name, _ = readPascalString(d[8:])
	}
	return s
}

// parseProcRef decodes an S_PROCREF record, extracting checksum, symbol-stream
// offset, module index, and procedure type index for a referenced procedure.
func parseProcRef(d []byte) *SProcRef {
	if len(d) < 12 {
		return nil
	}
	return &SProcRef{
		Checksum: le.Uint32(d[0:4]),
		SymOff:   le.Uint32(d[4:8]),
		IMod:     le.Uint16(d[8:10]),
		TypInd:   le.Uint16(d[10:12]),
	}
}

// parseDataRef decodes an S_DATAREF record, extracting checksum, symbol-stream
// offset, module index, and data type index for a referenced data symbol.
func parseDataRef(d []byte) *SDataRef {
	if len(d) < 12 {
		return nil
	}
	return &SDataRef{
		Checksum: le.Uint32(d[0:4]),
		SymOff:   le.Uint32(d[4:8]),
		IMod:     le.Uint16(d[8:10]),
		TypInd:   le.Uint16(d[10:12]),
	}
}

// parseBpRel16 decodes an S_BPREL16 record, extracting the signed frame-pointer
// relative offset, type index, and local variable or parameter name.
func parseBpRel16(d []byte) *SBpRel16 {
	if len(d) < 4 {
		return nil
	}
	s := &SBpRel16{
		Off:    int16(le.Uint16(d[0:2])),
		TypInd: le.Uint16(d[2:4]),
	}
	if len(d) > 4 {
		s.Name, _ = readPascalString(d[4:])
	}
	return s
}

// parseRegister decodes an S_REGISTER record, extracting the type index,
// register number, and register-resident symbol name.
func parseRegister(d []byte) *SRegister {
	if len(d) < 4 {
		return nil
	}
	s := &SRegister{
		TypInd: le.Uint16(d[0:2]),
		Reg:    le.Uint16(d[2:4]),
	}
	if len(d) > 4 {
		s.Name, _ = readPascalString(d[4:])
	}
	return s
}

// ---------- Hash sym subsection (GlobalPub/GlobalSym/StaticSym) ----------

// parseHashSymSubsection decodes global/public/static symbol subsections,
// extracting hash metadata, the embedded symbol stream, and raw symbol and
// address hash tables.
func parseHashSymSubsection(data []byte) (HashSymSubsection, error) {
	if len(data) < 12 {
		return HashSymSubsection{}, fmt.Errorf("hashSymSubsection too short (%d)", len(data))
	}
	hss := HashSymSubsection{
		SymHash:    le.Uint16(data[0:2]),
		AddrHash:   le.Uint16(data[2:4]),
		CBSymbol:   le.Uint32(data[4:8]),
		CBSymHash:  le.Uint32(data[8:12]),
		CBAddrHash: le.Uint32(data[12:16]),
	}
	off := 16
	symEnd := off + int(hss.CBSymbol)
	if symEnd > len(data) {
		symEnd = len(data)
	}
	syms, err := parseSymStream(data[off:symEnd])
	if err != nil {
		return hss, err
	}
	hss.Symbols = syms

	hashStart := symEnd
	hashEnd := hashStart + int(hss.CBSymHash)
	if hashEnd > len(data) {
		hashEnd = len(data)
	}
	if hashStart < len(data) {
		hss.SymHashBytes = data[hashStart:hashEnd]
	}

	addrStart := hashEnd
	addrEnd := addrStart + int(hss.CBAddrHash)
	if addrEnd > len(data) {
		addrEnd = len(data)
	}
	if addrStart < len(data) {
		hss.AddrHashBytes = data[addrStart:addrEnd]
	}

	return hss, nil
}

// ---------- Type stream (sstGlobalTypes) ----------

// parseTypeStream decodes an sstGlobalTypes stream, reading the type-count
// header and offset table before parsing each type record's leaf, raw payload,
// and recognized structured payload.
func parseTypeStream(data []byte) (TypeStream, error) {
	// sstGlobalTypes header: u32 flags, u32 cTypes, then cTypes × u32 offsets, then type records
	if len(data) < 8 {
		return TypeStream{}, fmt.Errorf("typeStream too short (%d)", len(data))
	}
	// flags := le.Uint32(data[0:4]) // typically 1
	cTypes := le.Uint32(data[4:8])

	offTableEnd := 8 + int(cTypes)*4
	if offTableEnd > len(data) {
		return TypeStream{}, fmt.Errorf("typeStream offset table truncated")
	}

	ts := TypeStream{BaseIndex: 0x1000}
	recordBase := offTableEnd
	recData := data[recordBase:]

	off := 0
	for off+4 <= len(recData) {
		recOff := uint32(off)
		recLen := le.Uint16(recData[off : off+2])
		if recLen < 2 {
			off += 2
			continue
		}
		if off+2+int(recLen) > len(recData) {
			break
		}
		leaf := TypeLeaf(le.Uint16(recData[off+2 : off+4]))
		payload := recData[off+4 : off+2+int(recLen)]

		rec := TypeRecord{
			Offset: recOff,
			RecLen: recLen,
			Leaf:   leaf,
			Raw:    payload,
			Parsed: parseTypePayload(leaf, payload),
		}
		ts.Records = append(ts.Records, rec)
		off += 2 + int(recLen)
	}

	return ts, nil
}

// parseTypePayload dispatches a type record payload to the parser for its
// concrete CodeView leaf, such as pointers, modifiers, arrays, structures,
// procedures, argument lists, field lists, and bitfields.
func parseTypePayload(leaf TypeLeaf, d []byte) any {
	switch leaf {
	case LF_POINTER:
		return parsePointer(d)
	case LF_MODIFIER:
		return parseModifier(d)
	case LF_ARRAY:
		return parseArray(d)
	case LF_STRUCTURE, LF_CLASS:
		return parseStruct(d)
	case LF_PROCEDURE:
		return parseProcedure(d)
	case LF_ARGLIST:
		return parseArgList(d)
	case LF_FIELDLIST:
		return parseFieldList(d)
	case LF_BITFIELD:
		return parseBitfield(d)
	default:
		return nil
	}
}

// parsePointer decodes an LF_POINTER record, extracting the pointer attributes
// and referenced underlying type index.
func parsePointer(d []byte) *LFPointer {
	if len(d) < 4 {
		return nil
	}
	// Old CV (NB09) format: attr(u16), utype(u16).
	return &LFPointer{
		Attr:  uint32(le.Uint16(d[0:2])),
		UType: le.Uint16(d[2:4]),
	}
}

// parseModifier decodes an LF_MODIFIER record, extracting the modified type
// index and modifier flags such as const or volatile.
func parseModifier(d []byte) *LFModifier {
	if len(d) < 4 {
		return nil
	}
	return &LFModifier{
		UType: le.Uint16(d[0:2]),
		Mod:   le.Uint16(d[2:4]),
	}
}

// parseArray decodes an LF_ARRAY record, extracting element and index type
// indexes, the numeric length leaf, and the optional array name.
func parseArray(d []byte) *LFArray {
	if len(d) < 4 {
		return nil
	}
	a := &LFArray{
		ElemType:  le.Uint16(d[0:2]),
		IndexType: le.Uint16(d[2:4]),
	}
	off := 4
	if off < len(d) {
		nl, n := parseNumericLeaf(d[off:])
		a.Length = nl
		off += n
	}
	if off < len(d) {
		a.Name, _ = readPascalString(d[off:])
	}
	return a
}

// parseStruct decodes an LF_STRUCTURE or LF_CLASS record, extracting field
// count, field-list type index, properties, derivation and vshape indexes,
// numeric size, and type name.
func parseStruct(d []byte) *LFStruct {
	if len(d) < 10 {
		return nil
	}
	s := &LFStruct{
		Count:     le.Uint16(d[0:2]),
		FieldList: le.Uint16(d[2:4]),
		Property:  le.Uint16(d[4:6]),
		Deriv:     le.Uint16(d[6:8]),
		VShape:    le.Uint16(d[8:10]),
	}
	off := 10
	if off < len(d) {
		nl, n := parseNumericLeaf(d[off:])
		s.Size = nl
		off += n
	}
	if off < len(d) {
		s.Name, _ = readPascalString(d[off:])
	}
	return s
}

// parseProcedure decodes an LF_PROCEDURE record, extracting return type,
// calling convention, parameter count, and argument-list type index.
func parseProcedure(d []byte) *LFProcedure {
	if len(d) < 8 {
		return nil
	}
	return &LFProcedure{
		RetType:    le.Uint16(d[0:2]),
		CallType:   d[2],
		Reserved:   d[3],
		ParamCount: le.Uint16(d[4:6]),
		ArgList:    le.Uint16(d[6:8]),
	}
}

// parseArgList decodes an LF_ARGLIST record, extracting the declared argument
// count and the ordered list of argument type indexes.
func parseArgList(d []byte) *LFArgList {
	if len(d) < 2 {
		return nil
	}
	count := le.Uint16(d[0:2])
	a := &LFArgList{Count: count}
	off := 2
	for i := 0; i < int(count); i++ {
		if off+2 > len(d) {
			break
		}
		a.Args = append(a.Args, le.Uint16(d[off:off+2]))
		off += 2
	}
	return a
}

// parseBitfield decodes an LF_BITFIELD record, extracting bit length, bit
// position, and the base type index.
func parseBitfield(d []byte) *LFBitfield {
	if len(d) < 4 {
		return nil
	}
	return &LFBitfield{
		Length:   d[0],
		Position: d[1],
		BaseType: le.Uint16(d[2:4]),
	}
}

// ---------- Field list parsing ----------

// parseFieldList decodes an LF_FIELDLIST payload, skipping CodeView padding and
// parsing supported member entries while preserving each field leaf kind.
func parseFieldList(d []byte) *LFFieldList {
	fl := &LFFieldList{}
	off := 0
	for off < len(d) {
		// Align to even offset (padding bytes >= 0xF0 are skip bytes)
		for off < len(d) && d[off] >= 0xF0 {
			off++
		}
		if off+2 > len(d) {
			break
		}
		leaf := FieldLeaf(le.Uint16(d[off : off+2]))
		off += 2

		entry := FieldEntry{Leaf: leaf}
		var consumed int
		switch leaf {
		case LF_MEMBER:
			entry.Parsed, consumed = parseFieldMember(d[off:])
		default:
			// Unknown leaf — skip to end
			consumed = len(d) - off
		}
		off += consumed
		fl.Fields = append(fl.Fields, entry)
	}
	return fl
}

// parseFieldMember decodes an LF_MEMBER field-list entry, extracting member
// type, attributes, numeric offset within the aggregate, name, and byte count
// consumed from the field-list payload.
func parseFieldMember(d []byte) (*LFMember, int) {
	if len(d) < 4 {
		return nil, len(d)
	}
	m := &LFMember{
		Type: le.Uint16(d[0:2]),
		Attr: le.Uint16(d[2:4]),
	}
	off := 4
	if off < len(d) {
		nl, n := parseNumericLeaf(d[off:])
		m.Offset = nl
		off += n
	}
	if off < len(d) {
		name, n := readPascalString(d[off:])
		m.Name = name
		off += n
	}
	return m, off
}

// parseNumericLeaf decodes a CodeView numeric leaf, returning inline small
// integer values directly and preserving extended numeric leaf tags for callers
// that need to interpret larger numeric encodings.
func parseNumericLeaf(data []byte) (NumericLeaf, int) {
	if len(data) < 2 {
		return NumericLeaf{}, 0
	}
	v := le.Uint16(data[0:2])
	if v < 0x8000 {
		return NumericLeaf{Value: int64(v)}, 2
	}
	return NumericLeaf{IsLeaf: true, Leaf: v}, 2
}

// ---------- sstSrcModule ----------

// parseSrcModule decodes an sstSrcModule subsection, extracting source file
// tables, per-file segment mappings, and line-number pairs that map code
// offsets back to source line numbers.
func parseSrcModule(data []byte) (SstSrcModule, error) {
	sm := SstSrcModule{Raw: data}
	if len(data) < 4 {
		return sm, nil
	}
	sm.CFile = le.Uint16(data[0:2])
	sm.CSeg = le.Uint16(data[2:4])

	// baseSrcFile offsets: cFile × u32
	off := 4
	fileOffsets := make([]uint32, sm.CFile)
	for i := range fileOffsets {
		if off+4 > len(data) {
			return sm, nil
		}
		fileOffsets[i] = le.Uint32(data[off : off+4])
		off += 4
	}

	// seg start/end pairs: cSeg × (u32, u32)
	// skip these in the module header — they are per-seg ranges
	off += int(sm.CSeg) * 8

	// seg numbers: cSeg × u16
	off += int(sm.CSeg) * 2

	// Now parse each file table entry
	for i := 0; i < int(sm.CFile); i++ {
		foff := int(fileOffsets[i])
		if foff+4 > len(data) {
			continue
		}
		f := SrcModuleFile{}

		cSeg := le.Uint16(data[foff : foff+2])
		// pad at foff+2
		foff += 4

		// baseSrcLn offsets: cSeg × u32
		lnOffsets := make([]uint32, cSeg)
		for j := range lnOffsets {
			if foff+4 > len(data) {
				break
			}
			lnOffsets[j] = le.Uint32(data[foff : foff+4])
			foff += 4
		}

		// seg start/end: cSeg × (u32, u32)
		segRanges := make([][2]uint32, cSeg)
		for j := range segRanges {
			if foff+8 > len(data) {
				break
			}
			segRanges[j][0] = le.Uint32(data[foff : foff+4])
			segRanges[j][1] = le.Uint32(data[foff+4 : foff+8])
			foff += 8
		}

		// file name: pascal string
		if foff < len(data) {
			f.Name, _ = readPascalString(data[foff:])
		}

		// Parse line number tables
		for j := 0; j < int(cSeg); j++ {
			lnOff := int(lnOffsets[j])
			if lnOff+4 > len(data) {
				continue
			}
			seg := SrcModuleFileSeg{
				Seg:   le.Uint16(data[lnOff : lnOff+2]),
				Start: segRanges[j][0],
				End:   segRanges[j][1],
			}
			cPair := le.Uint16(data[lnOff+2 : lnOff+4])
			lnOff += 4

			// offsets: cPair × u32
			offsets := make([]uint32, 0, cPair)
			for k := 0; k < int(cPair); k++ {
				if lnOff+4 > len(data) {
					break
				}
				offsets = append(offsets, le.Uint32(data[lnOff:lnOff+4]))
				lnOff += 4
			}

			// line numbers: cPair × u16
			for k := 0; k < int(cPair) && k < len(offsets); k++ {
				if lnOff+2 > len(data) {
					break
				}
				lineNum := le.Uint16(data[lnOff : lnOff+2])
				seg.Lines = append(seg.Lines, SrcModuleLine{
					Off:     offsets[k],
					LineNum: lineNum,
				})
				lnOff += 2
			}

			f.Segs = append(f.Segs, seg)
		}

		sm.Files = append(sm.Files, f)
	}

	return sm, nil
}
