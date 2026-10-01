package savefile

import (
	"encoding/binary"
	"strings"
	"testing"
)

// TestCyberInfoDataDecodesBits checks the CYBERINFO fields against their header bit positions.
func TestCyberInfoDataDecodesBits(t *testing.T) {
	data := []byte{6, 0, 0xfd, 0x00, 0, 0}
	history, err := ParseCyberInfoData(data, 2)
	if err != nil {
		t.Fatal(err)
	}
	info := history.Planets[0]
	if info.ILstPktDir != 5 || !info.FBltColony || !info.FLaunchedPkt || info.IPktTarget != 3 || !info.FNeedScanPkt || info.Unused != 0 {
		t.Fatalf("incorrect CYBERINFO: %+v", info)
	}
	if _, err := ParseCyberInfoData(data[:5], 2); err == nil {
		t.Fatal("accepted truncated history")
	}
	data[0] = 4
	if _, err := ParseCyberInfoData(data, 2); err == nil {
		t.Fatal("accepted wrong history size word")
	}
}

// TestCompareRecordsReassemblesAIHistory checks changes across rtAiData chunk boundaries.
func TestCompareRecordsReassemblesAIHistory(t *testing.T) {
	game := Record{Type: RtGame, Data: make([]byte, 64)}
	binary.LittleEndian.PutUint16(game.Data[10:], 3)
	player := Record{Type: RtPlr, Data: make([]byte, 8)}
	binary.LittleEndian.PutUint16(player.Data[6:], 0x8200)
	left := []Record{game, player,
		{Type: RtAiData, Data: []byte{8, 0, 0, 0, 0}},
		{Type: RtAiData, Data: []byte{0, 0, 0}},
	}
	right := []Record{game, player,
		{Type: RtAiData, Data: []byte{8, 0, 0, 0, 0x60}},
		{Type: RtAiData, Data: []byte{0, 0, 0}},
	}
	diffs := CompareRecords(left, right)
	if len(diffs) != 1 || diffs[0].Label != "Cybertron history" || len(diffs[0].Fields) != 1 ||
		!strings.Contains(diffs[0].Fields[0].Path, "planet 1 iPktTarget") || diffs[0].Fields[0].After != "3" {
		t.Fatalf("unexpected AI history difference: %+v", diffs)
	}
}

// TestCompareRecordsInfersStandaloneHistory checks the guarded .hN layout fallback.
func TestCompareRecordsInfersStandaloneHistory(t *testing.T) {
	left := Record{Type: RtAiData, Data: []byte{6, 0, 0, 0, 0x60, 0}}
	right := Record{Type: RtAiData, Data: []byte{6, 0, 0, 0, 0, 0}}
	diffs := CompareRecords([]Record{left}, []Record{right})
	if len(diffs) != 1 || diffs[0].Label != "CYBERINFO-shaped AI history (inferred layout)" ||
		len(diffs[0].Fields) != 1 || diffs[0].Fields[0].Path != "planet 1 iPktTarget" {
		t.Fatalf("unexpected standalone history difference: %+v", diffs)
	}
	left.Data[5] = 1
	diffs = CompareRecords([]Record{left}, []Record{right})
	if len(diffs) != 1 || diffs[0].Label != "" || len(diffs[0].Fields) != 0 {
		t.Fatalf("guessed an unsupported AI layout: %+v", diffs)
	}
}
