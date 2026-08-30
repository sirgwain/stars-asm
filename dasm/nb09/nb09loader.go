package nb09

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
	"log/slog"
	"os"
)

// nb09Info describes the location and basic directory metadata of an embedded CodeView NB09 blob.
// Offsets are file offsets.
type nb09Info struct {
	baseOffset     int    // file offset where "NB09" begins
	dirRelativeOff uint32 // lfoDir value from NB09 header
	dirOffset      int    // BaseOffset + int(DirRelativeOff)

	dirHeaderBytes uint16 // expected 0x0010 for Stars! (directory header size)
	dirEntryBytes  uint16 // expected 0x000C for Stars! (directory entry size)
	dirEntryCount  uint32 // expected 0x00EA for Stars! (number of directory entries)

	blobEndOffset int // file offset just past the end of the NB09 blob (computed from directory entries)
}

var (
	ErrNB09Malformed = errors.New("NB09 data malformed")
)

func Load(path string) (*NB09DB, error) {
	bytes, _, err := loadNB09FromExePath(path)
	if err != nil {
		return nil, fmt.Errorf("failed to load nb09 data from exe %s %w", path, err)
	}

	db, err := parseNB09(bytes)
	if err != nil {
		return nil, fmt.Errorf("failed to parse nb09 data from exe %s %w", path, err)
	}

	return &db, nil
}

// loadNB09FromExePath locates the CodeView NB09 debug blob inside an .exe file and returns
// a byte slice containing the NB09 blob starting at "NB09" and extending far enough to
// include the entire directory (header + entries) and all referenced subsection data.
func loadNB09FromExePath(path string) ([]byte, nb09Info, error) {
	slog.Debug("nb09: loading exe", "path", path)
	exe, err := os.ReadFile(path)
	if err != nil {
		return nil, nb09Info{}, err
	}

	blob, info, err := locateAndSliceNB09(exe)
	if err != nil {
		return nil, nb09Info{}, err
	}

	// Copy out to a standalone buffer so the caller can drop the original exe slice.
	out := make([]byte, len(blob))
	copy(out, blob)
	return out, info, nil
}

// locateAndSliceNB09 finds the NB09 blob within an in-memory EXE image.
// It returns a subslice of exe (no copy) spanning the NB09 blob.
func locateAndSliceNB09(exe []byte) ([]byte, nb09Info, error) {
	// Find a candidate NB09 base that yields a plausible directory.
	for base := 0; ; {
		i := bytes.Index(exe[base:], []byte("NB09"))
		if i < 0 {
			return nil, nb09Info{}, fmt.Errorf("NB09 signature not found")
		}
		base += i

		// Need at least signature + lfoDir
		if base+8 > len(exe) {
			return nil, nb09Info{}, fmt.Errorf("NB09 data malformed")
		}

		lfoDir := binary.LittleEndian.Uint32(exe[base+4 : base+8])
		dirOff := base + int(lfoDir)

		// Quick bounds checks
		if lfoDir == 0 || dirOff < 0 || dirOff+8 > len(exe) {
			// Not a valid header; continue scanning for another NB09 occurrence.
			base++
			continue
		}

		// Directory header: cbDirHeader (u16), cbDirEntry (u16), cDir (u32), ...
		cbDirHeader := binary.LittleEndian.Uint16(exe[dirOff : dirOff+2])
		cbDirEntry := binary.LittleEndian.Uint16(exe[dirOff+2 : dirOff+4])
		cDir := binary.LittleEndian.Uint32(exe[dirOff+4 : dirOff+8])

		// Stars! uses cbDirHeader=0x10 and cbDirEntry=0x0c.
		if cbDirHeader != 0x0010 || cbDirEntry != 0x000C {
			base++
			continue
		}
		if cDir == 0 || cDir > 100000 {
			base++
			continue
		}

		dirHeaderEnd := dirOff + int(cbDirHeader)
		if dirHeaderEnd < 0 || dirHeaderEnd > len(exe) {
			return nil, nb09Info{}, ErrNB09Malformed
		}

		// Directory entries immediately follow the header.
		entriesBytes := int(cDir) * int(cbDirEntry)
		entriesStart := dirHeaderEnd
		entriesEnd := entriesStart + entriesBytes
		if entriesEnd < entriesStart || entriesEnd > len(exe) {
			return nil, nb09Info{}, ErrNB09Malformed
		}

		// Compute the last byte referenced by any entry's subsection (lfo+cb).
		maxEndRel := uint32(0)
		for j := 0; j < int(cDir); j++ {
			off := entriesStart + j*int(cbDirEntry)
			// subsection := binary.LittleEndian.Uint32(exe[off : off+4]) // unused
			lfo := binary.LittleEndian.Uint32(exe[off+4 : off+8])
			cb := binary.LittleEndian.Uint32(exe[off+8 : off+12])

			if cb > 0 && lfo > ^uint32(0)-cb {
				return nil, nb09Info{}, ErrNB09Malformed
			}
			endRel := lfo + cb
			if endRel > maxEndRel {
				maxEndRel = endRel
			}
		}

		// The directory itself may sit *after* all subsection blobs, so ensure we include it.
		dirEndRel := lfoDir + uint32(cbDirHeader) + uint32(entriesBytes)
		if dirEndRel > maxEndRel {
			maxEndRel = dirEndRel
		}

		blobEnd := base + int(maxEndRel)
		if blobEnd < base || blobEnd > len(exe) {
			return nil, nb09Info{}, ErrNB09Malformed
		}

		info := nb09Info{
			baseOffset:     base,
			dirRelativeOff: lfoDir,
			dirOffset:      dirOff,
			dirHeaderBytes: cbDirHeader,
			dirEntryBytes:  cbDirEntry,
			dirEntryCount:  cDir,
			blobEndOffset:  blobEnd,
		}
		return exe[base:blobEnd], info, nil
	}
}
