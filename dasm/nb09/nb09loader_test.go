package nb09

import (
	"encoding/binary"
	"fmt"
	"os"
	"testing"
)

const starsExePath = "../input/stars.exe"

// debugString returns a human-friendly summary.
func (n nb09Info) debugString() string {
	return fmt.Sprintf(
		"NB09 base=0x%X dirRel=0x%X dir=0x%X hdr=0x%X ent=0x%X count=%d end=0x%X",
		n.baseOffset, n.dirRelativeOff, n.dirOffset,
		n.dirHeaderBytes, n.dirEntryBytes, n.dirEntryCount,
		n.blobEndOffset,
	)
}

func TestLocateAndSliceNB09_Stars(t *testing.T) {
	exe, err := os.ReadFile(starsExePath)
	if err != nil {
		t.Skipf("unable to read %s: %v", starsExePath, err)
	}

	blob, info, err := locateAndSliceNB09(exe)
	if err != nil {
		t.Fatalf("LocateAndSliceNB09 error: %v", err)
	}

	if len(blob) < 8 {
		t.Fatalf("blob too small: %d", len(blob))
	}
	if string(blob[:4]) != "NB09" {
		t.Fatalf("blob signature mismatch: %q", blob[:4])
	}

	// These constants are for this project’s Stars! build (stars26jrc3.exe)
	if info.baseOffset != 0x35E0C0 {
		t.Fatalf("BaseOffset got 0x%X want 0x35E0C0 (%s)", info.baseOffset, info.debugString())
	}
	if info.dirRelativeOff != 0x0A7134 {
		t.Fatalf("DirRelativeOff got 0x%X want 0x0A7134 (%s)", info.dirRelativeOff, info.debugString())
	}
	if info.dirOffset != 0x4051F4 {
		t.Fatalf("DirOffset got 0x%X want 0x4051F4 (%s)", info.dirOffset, info.debugString())
	}
	if info.dirHeaderBytes != 0x0010 {
		t.Fatalf("DirHeaderBytes got 0x%X want 0x0010", info.dirHeaderBytes)
	}
	if info.dirEntryBytes != 0x000C {
		t.Fatalf("DirEntryBytes got 0x%X want 0x000C", info.dirEntryBytes)
	}
	if info.dirEntryCount != 0x00EA {
		t.Fatalf("DirEntryCount got %d want 0x00EA", info.dirEntryCount)
	}

	// Basic internal consistency checks.
	if info.blobEndOffset <= info.baseOffset {
		t.Fatalf("BlobEndOffset invalid: base=0x%X end=0x%X", info.baseOffset, info.blobEndOffset)
	}
	if info.blobEndOffset > len(exe) {
		t.Fatalf("BlobEndOffset beyond file: end=0x%X len=0x%X", info.blobEndOffset, len(exe))
	}

	// Ensure header's lfoDir matches what we parsed.
	lfoDir := binary.LittleEndian.Uint32(exe[info.baseOffset+4 : info.baseOffset+8])
	if lfoDir != info.dirRelativeOff {
		t.Fatalf("lfoDir mismatch in file: got 0x%X want 0x%X", lfoDir, info.dirRelativeOff)
	}
}
