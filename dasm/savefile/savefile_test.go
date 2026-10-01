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
	left := []Record{
		{Type: RtAiData, Data: []byte{8, 0, 0, 0, 0}},
		{Type: RtAiData, Data: []byte{0, 0, 0}},
	}
	right := []Record{
		{Type: RtAiData, Data: []byte{8, 0, 0, 0, 0x60}},
		{Type: RtAiData, Data: []byte{0, 0, 0}},
	}
	diffs := CompareRecords(left, right, AIHistoryCyber)
	if len(diffs) != 1 || diffs[0].Label != "Cybertron history (CYBERINFO)" || len(diffs[0].Fields) != 1 || diffs[0].Benign ||
		!strings.Contains(diffs[0].Fields[0].Path, "planet 1 iPktTarget") || diffs[0].Fields[0].After != "3" {
		t.Fatalf("unexpected AI history difference: %+v", diffs)
	}
}

// aiHistFixture encodes an AIHIST with one starbase per entry of starbases.
func aiHistFixture(starbases ...AIStarbase) []byte {
	data := make([]byte, 4+20*len(starbases))
	binary.LittleEndian.PutUint16(data, uint16(len(data)))
	binary.LittleEndian.PutUint16(data[2:], uint16(len(starbases)))
	for i, sb := range starbases {
		entry := data[4+i*20:]
		binary.LittleEndian.PutUint16(entry, uint16(sb.IDPlanet))
		binary.LittleEndian.PutUint16(entry[2:], uint16(sb.CFreighter))
		for j, id := range sb.RGFlid {
			binary.LittleEndian.PutUint16(entry[4+j*2:], uint16(id))
		}
	}
	return data
}

// TestCompareRecordsSeparatesUnusedFreighterSlots checks that AIHIST slots past
// cFreighter on both sides are benign while active slots and counts are not.
func TestCompareRecordsSeparatesUnusedFreighterSlots(t *testing.T) {
	base := AIStarbase{IDPlanet: 82, CFreighter: 2, RGFlid: [8]int16{24, 25, 0x210, 0x218}}
	stale := base
	stale.RGFlid[2], stale.RGFlid[3] = 0x201, 0x203
	left := []Record{{Type: RtAiData, Data: aiHistFixture(base)}}
	diffs := CompareRecords(left, []Record{{Type: RtAiData, Data: aiHistFixture(stale)}}, AIHistoryStarbase)
	if len(diffs) != 1 || !diffs[0].Benign || len(diffs[0].Fields) != 0 || len(diffs[0].Unused) != 2 ||
		diffs[0].Unused[0] != (FieldChange{"starbase 0 (planet 82) rgflid[2]", "0x0210", "0x0201"}) {
		t.Fatalf("unused slots not separated: %+v", diffs)
	}
	grown := stale
	grown.CFreighter = 3
	diffs = CompareRecords(left, []Record{{Type: RtAiData, Data: aiHistFixture(grown)}}, AIHistoryStarbase)
	if len(diffs) != 1 || diffs[0].Benign || len(diffs[0].Fields) != 2 || len(diffs[0].Unused) != 1 ||
		diffs[0].Fields[1] != (FieldChange{"starbase 0 (planet 82) rgflid[2]", "528", "513"}) {
		t.Fatalf("newly active slot treated as unused: %+v", diffs)
	}
	diffs = CompareRecords(left, []Record{{Type: RtAiData, Data: aiHistFixture(base, base)}}, AIHistoryStarbase)
	if len(diffs) != 1 || diffs[0].Benign || len(diffs[0].Fields) != 2 ||
		diffs[0].Fields[1] != (FieldChange{"starbase 1", "absent", "planet 82 freighters [24 25]"}) {
		t.Fatalf("added starbase not reported: %+v", diffs)
	}
}
