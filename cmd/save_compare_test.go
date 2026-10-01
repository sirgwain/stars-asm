package cmd

import (
	"bytes"
	"encoding/binary"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/savefile"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// comparisonXY builds an encrypted small-universe fixture with a complete GAME and star table.
func comparisonXY(t *testing.T, tables savefile.Tables, lid uint32, salt uint16) []byte {
	t.Helper()
	b := make([]byte, 84)
	binary.LittleEndian.PutUint16(b, 8<<10|16)
	copy(b[2:], "J3J3")
	binary.LittleEndian.PutUint32(b[6:], lid)
	binary.LittleEndian.PutUint16(b[10:], 2<<12|83<<5)
	binary.LittleEndian.PutUint16(b[14:], salt<<5|31)
	binary.LittleEndian.PutUint16(b[18:], 7<<10|64)
	game := b[20:]
	binary.LittleEndian.PutUint32(game, lid)
	binary.LittleEndian.PutUint16(game[4:], 1)
	binary.LittleEndian.PutUint16(game[6:], 1)
	binary.LittleEndian.PutUint16(game[8:], 7)
	binary.LittleEndian.PutUint16(game[10:], 128)
	binary.LittleEndian.PutUint16(game[12:], 1)
	copy(game[32:], "Regression small")
	// XOR is symmetric: applying the reader's stream to plaintext encodes it.
	// Use a host header during encoding so the reader does not interpret the
	// still-unencrypted GAME as XY metadata. Dt does not affect the XOR seed.
	binary.LittleEndian.PutUint16(b[16:], 2)
	records, err := savefile.ReadRecords(b, tables)
	if err != nil {
		t.Fatal(err)
	}
	copy(b[20:], records[1].Data)
	binary.LittleEndian.PutUint16(b[16:], 0)
	for i := 0; i < 128; i++ {
		var star [4]byte
		binary.LittleEndian.PutUint32(star[:], uint32(5|(1100+i)<<10|i<<22))
		b = append(b, star[:]...)
	}
	return append(b, 2, 0, 7, 0)
}

// TestDescribePlanetDifference identifies a changed starbase destination in a full planet record.
func TestDescribePlanetDifference(t *testing.T) {
	left := savefile.Record{Offset: 0x55b, Type: 13, Data: []byte{
		0x52, 0x08, 0x87, 0x2b, 0x15, 0xf1, 0xef, 0xf1, 0x65, 0x77, 0x6e, 0x32, 0x32, 0x32, 0x2b, 0x20,
		0x66, 0x38, 0x01, 0xf4, 0x16, 0x01, 0xb6, 0x07, 0x0a, 0xa0, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x58, 0x10,
	}}
	right := savefile.Record{Offset: left.Offset, Type: left.Type, Data: bytes.Clone(left.Data)}
	right.Data[33] = 0x66
	diffs := savefile.CompareRecords([]savefile.Record{left}, []savefile.Record{right})
	var out strings.Builder
	formatRecordDifference(&out, diffs[0], false)
	got := out.String()
	if !strings.Contains(got, "planet 82 starbase idFling: 88 → 102") || strings.Contains(got, "byte +") {
		t.Fatalf("missing planet field or byte difference:\n%s", got)
	}
	out.Reset()
	formatRecordDifference(&out, diffs[0], true)
	if !strings.Contains(out.String(), "byte +0x21 (file 0x57e/0x57e): 58 → 66") {
		t.Fatalf("missing optional byte difference:\n%s", out.String())
	}
}

// TestDescribeAiDataDifference identifies Cybertron packet target changes by planet.
func TestDescribeAiDataDifference(t *testing.T) {
	game := savefile.Record{Type: savefile.RtGame, Data: make([]byte, 64)}
	binary.LittleEndian.PutUint16(game.Data[10:], 128)
	player := savefile.Record{Type: savefile.RtPlr, Data: make([]byte, 8)}
	binary.LittleEndian.PutUint16(player.Data[6:], 0x200|4<<13)
	left := savefile.Record{Type: savefile.RtAiData, Data: make([]byte, 258)}
	binary.LittleEndian.PutUint16(left.Data, 258)
	for _, planet := range []int{84, 88, 94, 102} {
		binary.LittleEndian.PutUint16(left.Data[2+planet*2:], 0x60)
	}
	right := savefile.Record{Type: savefile.RtAiData, Data: bytes.Clone(left.Data)}
	binary.LittleEndian.PutUint16(right.Data[2+84*2:], 0)
	diffs := savefile.CompareRecords([]savefile.Record{game, player, left}, []savefile.Record{game, player, right})
	var out strings.Builder
	formatRecordDifference(&out, diffs[0], false)
	got := out.String()
	if !strings.Contains(got, "planet 84 iPktTarget: 3 → 0") || strings.Contains(got, "left:") {
		t.Fatalf("missing Cybertron planet field difference:\n%s", got)
	}
}

// TestComparisonRecords excludes only identity and salt while retaining universe changes.
func TestComparisonRecords(t *testing.T) {
	d, err := newSaveDumper(testfixture.Stars(t))
	if err != nil {
		t.Fatal(err)
	}
	a := comparisonXY(t, d.tables, 100, 37)
	b := comparisonXY(t, d.tables, 999, 725)
	left, err := comparisonRecords(a, true, d.tables)
	if err != nil {
		t.Fatal(err)
	}
	right, err := comparisonRecords(b, true, d.tables)
	if err != nil {
		t.Fatal(err)
	}
	if len(left) != len(right) {
		t.Fatal("different record counts")
	}
	for i := range left {
		if left[i].Type != right[i].Type || !bytes.Equal(left[i].Data, right[i].Data) {
			t.Fatalf("record %d differs after normalization", i)
		}
	}
	b[84] ^= 1
	changed, err := comparisonRecords(b, true, d.tables)
	if err != nil {
		t.Fatal(err)
	}
	if bytes.Equal(left[2].Data, changed[2].Data) {
		t.Fatal("coordinate change was hidden")
	}
	differences := savefile.CompareRecords(left, changed)
	if len(differences) != 1 || differences[0].Label != "STARPACK coordinates" || len(differences[0].Fields) == 0 || differences[0].Fields[0].Path != "star 0 x" {
		t.Fatalf("missing decoded XY coordinate difference: %+v", differences)
	}
	if _, err := comparisonRecords(a[:len(a)-1], true, d.tables); err == nil {
		t.Fatal("accepted truncated XY")
	}
}
