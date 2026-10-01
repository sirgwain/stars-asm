// Package savefile reads Stars! game files (.xy, .hst, .mN, .xN, .hN) into
// decrypted records, following the original ReadRt/SetFileXorStream code.
package savefile

import (
	"bytes"
	"encoding/binary"
	"fmt"
)

// RecordType is the six-bit record type in the file header (enums.h).
type RecordType uint16

// DtFileType is the file kind in RTBOF (enums.h).
type DtFileType uint16

// Record types match RecordType in decompiled/enums.h.
const (
	RtEOF                  = 0
	RtLogCargoXfer8        = 1
	RtLogCargoXfer16       = 2
	RtLogFleetOrderDelete  = 3
	RtLogFleetOrderInsert  = 4
	RtLogFleetOrderUpdate  = 5
	RtPlr                  = 6
	RtGame                 = 7
	RtBOF                  = 8
	RtLogFleetFlagBit9     = 10
	RtLogFleetOrderAttrNib = 11
	RtMsg                  = 12
	RtPlanet               = 13
	RtPlanetB              = 14
	RtFleetA               = 16
	RtOrderA               = 19
	RtOrderB               = 20
	RtString               = 21
	RtSel                  = 22
	RtLogFleetCargoXfer    = 23
	RtLogFleetSplit        = 24
	RtLogCargoXfer32       = 25
	RtShDef                = 26
	RtLogShDef             = 27
	RtProdQ                = 28
	RtLogPlanetProdQ       = 29
	RtBtlPlan              = 30
	RtBtlData              = 31
	RtHistHdr              = 32
	RtMsgFilt              = 33
	RtLogResearch          = 34
	RtLogPlanetRouting     = 35
	RtChgPassword          = 36
	RtLogFleetMerge        = 37
	RtLogRelations         = 38
	RtContinue             = 39
	RtPlrMsg               = 40
	RtAiData               = 41
	RtLogFleetPlan         = 42
	RtThing                = 43
	RtLogThingByteParam    = 43
	RtLogFleetName         = 44
	RtScore                = 45
	RtLogPlayerZpq1        = 46
)

const (
	DtXY DtFileType = iota
	DtLog
	DtHost
	DtTurn
	DtHist
)

var recordTypeNames = map[RecordType]string{
	RtEOF: "rtEOF", RtLogCargoXfer8: "rtLogCargoXfer8", RtLogCargoXfer16: "rtLogCargoXfer16",
	RtLogFleetOrderDelete: "rtLogFleetOrderDelete", RtLogFleetOrderInsert: "rtLogFleetOrderInsert",
	RtLogFleetOrderUpdate: "rtLogFleetOrderUpdate", RtPlr: "rtPlr", RtGame: "rtGame",
	RtBOF: "rtBOF", RtLogFleetFlagBit9: "rtLogFleetFlagBit9", RtLogFleetOrderAttrNib: "rtLogFleetOrderAttrNib",
	RtMsg: "rtMsg", RtPlanet: "rtPlanet", RtPlanetB: "rtPlanetB", RtFleetA: "rtFleetA",
	RtOrderA: "rtOrderA", RtOrderB: "rtOrderB", RtString: "rtString", RtSel: "rtSel",
	RtLogFleetCargoXfer: "rtLogFleetCargoXfer", RtLogFleetSplit: "rtLogFleetSplit", RtLogCargoXfer32: "rtLogCargoXfer32",
	RtShDef: "rtShDef", RtLogShDef: "rtLogShDef", RtProdQ: "rtProdQ", RtLogPlanetProdQ: "rtLogPlanetProdQ",
	RtBtlPlan: "rtBtlPlan", RtBtlData: "rtBtlData", RtHistHdr: "rtHistHdr", RtMsgFilt: "rtMsgFilt",
	RtLogResearch: "rtLogResearch", RtLogPlanetRouting: "rtLogPlanetRouting", RtChgPassword: "rtChgPassword",
	RtLogFleetMerge: "rtLogFleetMerge", RtLogRelations: "rtLogRelations", RtContinue: "rtContinue",
	RtPlrMsg: "rtPlrMsg", RtAiData: "rtAiData", RtLogFleetPlan: "rtLogFleetPlan", RtThing: "rtThing",
	RtLogFleetName: "rtLogFleetName", RtScore: "rtScore", RtLogPlayerZpq1: "rtLogPlayerZpq1",
}

// String returns the enum name or its numeric value for an unknown record type.
func (rt RecordType) String() string {
	if name, ok := recordTypeNames[rt]; ok {
		return name
	}
	return fmt.Sprintf("rt%d", rt)
}

// DefaultTables returns the file decryption table from utilgen.c.
func DefaultTables() Tables {
	return Tables{Primes: append([]int16(nil), filePrimes[:]...)}
}

// filePrimes matches rgPrimes in decompiled/utilgen.c, including its original ordering.
var filePrimes = [...]int16{
	3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103,
	107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241,
	251, 257, 263, 279, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401,
	409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571,
	577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727,
}

// Tables holds the static data from stars.exe needed to decode a file.
type Tables struct {
	// Primes is rgPrimes, the seed table for the file XOR stream.
	Primes []int16
	// MsgArgCounts is rgcMsgArgs, the number of parameters for each MessageId.
	MsgArgCounts []byte
}

// Record is one decrypted file record.
type Record struct {
	Offset int
	Type   int
	Data   []byte
	// Stars contains the unencrypted STARPACK array following an XY rtGame.
	Stars []Star
}

// Star is one XY planet location and name ID, decoded from STARPACK.
type Star struct {
	X, Y int
	ID   int
}

// StarPack matches the four-byte STARPACK bitfield in structs.h.
type StarPack struct {
	DX uint16
	Y  uint16
	ID uint16
}

// ParseStarPack decodes one packed XY planet coordinate and name ID.
func ParseStarPack(data []byte) (StarPack, error) {
	if len(data) != 4 {
		return StarPack{}, fmt.Errorf("STARPACK is %d bytes, want 4", len(data))
	}
	w := binary.LittleEndian.Uint32(data)
	return StarPack{DX: uint16(w & 0x3ff), Y: uint16(w >> 10 & 0xfff), ID: uint16(w >> 22)}, nil
}

// Game matches the fixed 64-byte GAME layout in structs.h.
type Game struct {
	Lid         int32
	MdSize      int16
	MdDensity   int16
	CPlayer     int16
	CPlanMax    int16
	MdStartDist int16
	FDirty      int16
	WCrap       uint16
	Turn        uint16
	RGVC        [12]byte
	SZName      [32]byte
}

// ParseGame decodes the fixed GAME record.
func ParseGame(data []byte) (Game, error) {
	if len(data) != 64 {
		return Game{}, fmt.Errorf("GAME record is %d bytes, want 64", len(data))
	}
	game := Game{
		Lid:         int32(binary.LittleEndian.Uint32(data)),
		MdSize:      int16(binary.LittleEndian.Uint16(data[4:])),
		MdDensity:   int16(binary.LittleEndian.Uint16(data[6:])),
		CPlayer:     int16(binary.LittleEndian.Uint16(data[8:])),
		CPlanMax:    int16(binary.LittleEndian.Uint16(data[10:])),
		MdStartDist: int16(binary.LittleEndian.Uint16(data[12:])),
		FDirty:      int16(binary.LittleEndian.Uint16(data[14:])),
		WCrap:       binary.LittleEndian.Uint16(data[16:]),
		Turn:        binary.LittleEndian.Uint16(data[18:]),
	}
	copy(game.RGVC[:], data[20:32])
	copy(game.SZName[:], data[32:64])
	return game, nil
}

// PlayerHeader matches the first eight bytes of PLAYER in structs.h.
type PlayerHeader struct {
	IPlayer  int8
	CShDef   int8
	CPlanet  int16
	CFleet   uint16
	CShDefSB uint8
	WMdPlr   uint16
	AI       bool
	AILevel  uint8
	AIID     uint8
}

// ParsePlayerHeader decodes PLAYER identity and AI fields from its fixed prefix.
func ParsePlayerHeader(data []byte) (PlayerHeader, error) {
	if len(data) < 8 {
		return PlayerHeader{}, fmt.Errorf("PLAYER record is %d bytes, want at least 8", len(data))
	}
	wFleet := binary.LittleEndian.Uint16(data[4:])
	w := binary.LittleEndian.Uint16(data[6:])
	return PlayerHeader{IPlayer: int8(data[0]), CShDef: int8(data[1]), CPlanet: int16(binary.LittleEndian.Uint16(data[2:])),
		CFleet: wFleet & 0xfff, CShDefSB: uint8(wFleet >> 12), WMdPlr: w,
		AI: w&0x200 != 0, AILevel: uint8(w >> 10 & 7), AIID: uint8(w >> 13 & 7)}, nil
}

// PlanetHeader matches the four-byte RTPLANET fixed prefix in structs.h.
type PlanetHeader struct {
	ID          int16
	IPlayer     int16
	Det         uint8
	FHomeworld  bool
	FInclude    bool
	FStarbase   bool
	FIncEVO     bool
	FIncImp     bool
	FIsArtifact bool
	FIncSurfMin bool
	FRouting    bool
	FFirstYear  bool
}

// ParsePlanetHeader decodes the fixed RTPLANET identity and flags.
func ParsePlanetHeader(data []byte) (PlanetHeader, error) {
	if len(data) < 4 {
		return PlanetHeader{}, fmt.Errorf("RTPLANET record is %d bytes, want at least 4", len(data))
	}
	idOwner, flags := binary.LittleEndian.Uint16(data), binary.LittleEndian.Uint16(data[2:])
	return PlanetHeader{ID: int16(idOwner & 0x7ff), IPlayer: int16(idOwner) >> 11, Det: uint8(flags & 0x7f),
		FHomeworld: flags&0x80 != 0, FInclude: flags&0x100 != 0, FStarbase: flags&0x200 != 0,
		FIncEVO: flags&0x400 != 0, FIncImp: flags&0x800 != 0, FIsArtifact: flags&0x1000 != 0,
		FIncSurfMin: flags&0x2000 != 0, FRouting: flags&0x4000 != 0, FFirstYear: flags&0x8000 != 0}, nil
}

// CyberInfo matches the named bitfields of CYBERINFO in structs.h.
type CyberInfo struct {
	WInfo        uint16
	ILstPktDir   uint8
	FBltColony   bool
	FLaunchedPkt bool
	IPktTarget   uint8
	FNeedScanPkt bool
	Unused       uint8
}

// CyberInfoData is the on-disk size word followed by one CYBERINFO per planet.
type CyberInfoData struct {
	Size    uint16
	Planets []CyberInfo
}

// ParseCyberInfo decodes the two-byte CYBERINFO bitfield.
func ParseCyberInfo(data []byte) (CyberInfo, error) {
	if len(data) != 2 {
		return CyberInfo{}, fmt.Errorf("CYBERINFO is %d bytes, want 2", len(data))
	}
	w := binary.LittleEndian.Uint16(data)
	return CyberInfo{WInfo: w, ILstPktDir: uint8(w & 7), FBltColony: w&8 != 0, FLaunchedPkt: w&16 != 0, IPktTarget: uint8(w >> 5 & 3), FNeedScanPkt: w&128 != 0, Unused: uint8(w >> 8)}, nil
}

// ParseCyberInfoData decodes Cybertron history using GAME.cPlanMax.
func ParseCyberInfoData(data []byte, planetCount int) (CyberInfoData, error) {
	if planetCount < 0 || planetCount > 1000 {
		return CyberInfoData{}, fmt.Errorf("planet count %d is outside 0..1000", planetCount)
	}
	want := 2 + planetCount*2
	if len(data) != want {
		return CyberInfoData{}, fmt.Errorf("Cybertron history is %d bytes, want %d", len(data), want)
	}
	size := binary.LittleEndian.Uint16(data)
	if int(size) != want {
		return CyberInfoData{}, fmt.Errorf("Cybertron history size word is %d, want %d", size, want)
	}
	result := CyberInfoData{Size: size, Planets: make([]CyberInfo, planetCount)}
	for i := range result.Planets {
		info, err := ParseCyberInfo(data[2+i*2 : 4+i*2])
		if err != nil {
			return CyberInfoData{}, err
		}
		result.Planets[i] = info
	}
	return result, nil
}

// FieldChange is one decoded value change within a record or AI history.
type FieldChange struct {
	Path   string
	Before string
	After  string
}

// RecordDifference describes one changed record while retaining its bytes for diagnostics.
type RecordDifference struct {
	Index  int
	Left   []Record
	Right  []Record
	Fields []FieldChange
	Label  string
}

// CompareRecords compares every record and groups consecutive rtAiData chunks.
func CompareRecords(left, right []Record) []RecordDifference {
	var differences []RecordDifference
	for i, j := 0, 0; i < len(left) || j < len(right); {
		if i >= len(left) || j >= len(right) {
			differences = append(differences, RecordDifference{Index: i, Left: left[i:], Right: right[j:]})
			break
		}
		startI, startJ := i, j
		if left[i].Type == RtAiData && right[j].Type == RtAiData {
			for i < len(left) && left[i].Type == RtAiData {
				i++
			}
			for j < len(right) && right[j].Type == RtAiData {
				j++
			}
		} else {
			i++
			j++
		}
		a, b := left[startI:i], right[startJ:j]
		if sameRecordGroup(a, b) {
			continue
		}
		diff := RecordDifference{Index: startI, Left: a, Right: b}
		if a[0].Type == RtAiData && b[0].Type == RtAiData {
			diff.Fields, diff.Label = cyberInfoChanges(a, b, left, right)
		} else if a[0].Type == RtGame && b[0].Type == RtGame {
			diff.Fields = gameChanges(a[0].Data, b[0].Data)
		} else if a[0].Type == RtBOF && b[0].Type == RtBOF {
			diff.Fields = bofChanges(a[0].Data, b[0].Data)
		} else if a[0].Type == RtPlanet && b[0].Type == RtPlanet {
			diff.Fields = planetTailChanges(a[0], b[0])
		} else if a[0].Type == -1 && b[0].Type == -1 {
			diff.Fields = starChanges(a[0].Stars, b[0].Stars)
			diff.Label = "STARPACK coordinates"
		}
		differences = append(differences, diff)
	}
	return differences
}

// gameChanges compares the fixed GAME fields without exposing raw bytes.
func gameChanges(a, b []byte) []FieldChange {
	x, errX := ParseGame(a)
	y, errY := ParseGame(b)
	if errX != nil || errY != nil {
		return nil
	}
	var changes []FieldChange
	for _, field := range []struct {
		name        string
		left, right int64
	}{
		{"lid", int64(x.Lid), int64(y.Lid)}, {"mdSize", int64(x.MdSize), int64(y.MdSize)},
		{"mdDensity", int64(x.MdDensity), int64(y.MdDensity)}, {"cPlayer", int64(x.CPlayer), int64(y.CPlayer)},
		{"cPlanMax", int64(x.CPlanMax), int64(y.CPlanMax)}, {"mdStartDist", int64(x.MdStartDist), int64(y.MdStartDist)},
		{"fDirty", int64(x.FDirty), int64(y.FDirty)}, {"turn", int64(x.Turn), int64(y.Turn)},
	} {
		if field.left != field.right {
			changes = append(changes, FieldChange{field.name, fmt.Sprint(field.left), fmt.Sprint(field.right)})
		}
	}
	for _, flag := range []struct {
		name string
		mask uint16
	}{
		{"fExtraFuel", 1}, {"fSlowTech", 2}, {"fSinglePlr", 4}, {"fTutorial", 8},
		{"fAisBand", 16}, {"fBBSPlay", 32}, {"fVisScores", 64}, {"fNoRandom", 128}, {"fClumping", 256},
	} {
		p, q := x.WCrap&flag.mask != 0, y.WCrap&flag.mask != 0
		if p != q {
			changes = append(changes, FieldChange{flag.name, fmt.Sprint(p), fmt.Sprint(q)})
		}
	}
	if p, q := x.WCrap>>9&7, y.WCrap>>9&7; p != q {
		changes = append(changes, FieldChange{"wGen", fmt.Sprint(p), fmt.Sprint(q)})
	}
	if x.WCrap&0xf000 != y.WCrap&0xf000 {
		changes = append(changes, FieldChange{"reserved game flags", "changed", "changed"})
	}
	if x.RGVC != y.RGVC {
		changes = append(changes, FieldChange{"rgvc", "changed", "changed"})
	}
	if x.SZName != y.SZName {
		changes = append(changes, FieldChange{"szName", fmt.Sprintf("%q", bytes.TrimRight(x.SZName[:], "\x00")), fmt.Sprintf("%q", bytes.TrimRight(y.SZName[:], "\x00"))})
	}
	return changes
}

// bofChanges compares decoded BOF fields after caller-supplied normalization.
func bofChanges(a, b []byte) []FieldChange {
	x, errX := ParseBOF(a)
	y, errY := ParseBOF(b)
	if errX != nil || errY != nil {
		return nil
	}
	var changes []FieldChange
	if x.Magic != y.Magic {
		changes = append(changes, FieldChange{"magic", x.Magic, y.Magic})
	}
	for _, field := range []struct {
		name        string
		left, right int64
	}{
		{"lidGame", int64(x.LidGame), int64(y.LidGame)}, {"verMajor", int64(x.VerMajor), int64(y.VerMajor)},
		{"verMinor", int64(x.VerMinor), int64(y.VerMinor)}, {"verInc", int64(x.VerInc), int64(y.VerInc)},
		{"turn", int64(x.Turn), int64(y.Turn)}, {"iPlayer", int64(x.IPlayer), int64(y.IPlayer)},
		{"lSaltTime", int64(x.LSaltTime), int64(y.LSaltTime)}, {"dt", int64(x.Dt), int64(y.Dt)}, {"wGen", int64(x.WGen), int64(y.WGen)},
	} {
		if field.left != field.right {
			changes = append(changes, FieldChange{field.name, fmt.Sprint(field.left), fmt.Sprint(field.right)})
		}
	}
	for _, flag := range []struct {
		name        string
		left, right bool
	}{
		{"fDone", x.FDone, y.FDone}, {"fInUse", x.FInUse, y.FInUse}, {"fMulti", x.FMulti, y.FMulti},
		{"fGameOverMan", x.FGameOver, y.FGameOver}, {"fCrippled", x.FCrippled, y.FCrippled},
	} {
		if flag.left != flag.right {
			changes = append(changes, FieldChange{flag.name, fmt.Sprint(flag.left), fmt.Sprint(flag.right)})
		}
	}
	return changes
}

// starChanges reports changes in decoded XY STARPACK coordinates and IDs.
func starChanges(a, b []Star) []FieldChange {
	var changes []FieldChange
	for i := 0; i < len(a) && i < len(b); i++ {
		prefix := fmt.Sprintf("star %d ", i)
		if a[i].X != b[i].X {
			changes = append(changes, FieldChange{prefix + "x", fmt.Sprint(a[i].X), fmt.Sprint(b[i].X)})
		}
		if a[i].Y != b[i].Y {
			changes = append(changes, FieldChange{prefix + "y", fmt.Sprint(a[i].Y), fmt.Sprint(b[i].Y)})
		}
		if a[i].ID != b[i].ID {
			changes = append(changes, FieldChange{prefix + "id", fmt.Sprint(a[i].ID), fmt.Sprint(b[i].ID)})
		}
	}
	if len(a) != len(b) {
		changes = append(changes, FieldChange{"star count", fmt.Sprint(len(a)), fmt.Sprint(len(b))})
	}
	return changes
}

// sameRecordGroup reports whether record types and decrypted payloads agree.
func sameRecordGroup(a, b []Record) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i].Type != b[i].Type || !bytes.Equal(a[i].Data, b[i].Data) {
			return false
		}
	}
	return true
}

// cyberInfoChanges reports named CYBERINFO fields for confirmed or inferred layouts.
func cyberInfoChanges(a, b, left, right []Record) ([]FieldChange, string) {
	var count int
	for _, r := range left {
		if r.Type == RtGame {
			game, err := ParseGame(r.Data)
			if err == nil {
				count = int(game.CPlanMax)
			}
			break
		}
	}
	var aiID uint8
	confirmed := false
	for _, r := range left {
		if r.Type == RtPlr {
			player, err := ParsePlayerHeader(r.Data)
			if err == nil && player.AI {
				aiID, confirmed = player.AIID, true
			}
		}
	}
	if confirmed && aiID != 4 {
		return nil, ""
	}
	for _, r := range right {
		if r.Type == RtPlr {
			player, err := ParsePlayerHeader(r.Data)
			if err == nil && player.AI && player.AIID != 4 {
				return nil, ""
			}
		}
	}
	var ab, bb []byte
	for _, r := range a {
		ab = append(ab, r.Data...)
	}
	for _, r := range b {
		bb = append(bb, r.Data...)
	}
	if count == 0 && len(ab) >= 4 && len(ab)%2 == 0 {
		count = (len(ab) - 2) / 2
	}
	label := "Cybertron history"
	if !confirmed {
		// Standalone .hN files omit GAME and PLAYER. Interpret the history only
		// when its size word and every CYBERINFO reserved byte fit the layout.
		if len(ab) != len(bb) || len(ab) < 4 || len(ab)%2 != 0 ||
			int(binary.LittleEndian.Uint16(ab)) != len(ab) || int(binary.LittleEndian.Uint16(bb)) != len(bb) {
			return nil, ""
		}
		for i := 3; i < len(ab); i += 2 {
			if ab[i] != 0 || bb[i] != 0 {
				return nil, ""
			}
		}
		label = "CYBERINFO-shaped AI history (inferred layout)"
	}
	x, errX := ParseCyberInfoData(ab, count)
	y, errY := ParseCyberInfoData(bb, count)
	if errX != nil || errY != nil {
		return nil, ""
	}
	var changes []FieldChange
	for i := range x.Planets {
		p, q := x.Planets[i], y.Planets[i]
		prefix := fmt.Sprintf("planet %d ", i)
		if p.ILstPktDir != q.ILstPktDir {
			changes = append(changes, FieldChange{prefix + "iLstPktDir", fmt.Sprint(p.ILstPktDir), fmt.Sprint(q.ILstPktDir)})
		}
		if p.FBltColony != q.FBltColony {
			changes = append(changes, FieldChange{prefix + "fBltColony", fmt.Sprint(p.FBltColony), fmt.Sprint(q.FBltColony)})
		}
		if p.FLaunchedPkt != q.FLaunchedPkt {
			changes = append(changes, FieldChange{prefix + "fLaunchedPkt", fmt.Sprint(p.FLaunchedPkt), fmt.Sprint(q.FLaunchedPkt)})
		}
		if p.IPktTarget != q.IPktTarget {
			changes = append(changes, FieldChange{prefix + "iPktTarget", fmt.Sprint(p.IPktTarget), fmt.Sprint(q.IPktTarget)})
		}
		if p.FNeedScanPkt != q.FNeedScanPkt {
			changes = append(changes, FieldChange{prefix + "fNeedScanPkt", fmt.Sprint(p.FNeedScanPkt), fmt.Sprint(q.FNeedScanPkt)})
		}
		if p.Unused != q.Unused {
			changes = append(changes, FieldChange{prefix + "unused", fmt.Sprintf("0x%02x", p.Unused), fmt.Sprintf("0x%02x", q.Unused)})
		}
	}
	return changes, label
}

// planetTailChanges identifies the documented routing and starbase tail words.
func planetTailChanges(a, b Record) []FieldChange {
	if len(a.Data) != len(b.Data) || len(a.Data) < 6 {
		return nil
	}
	planet, err := ParsePlanetHeader(a.Data)
	other, otherErr := ParsePlanetHeader(b.Data)
	if err != nil || otherErr != nil || planet != other || planet.IPlayer == -1 || planet.Det <= 3 {
		return nil
	}
	end := len(a.Data)
	var changes []FieldChange
	id := planet.ID
	if planet.FRouting && bytes.Equal(a.Data[:end-2], b.Data[:end-2]) {
		p, q := binary.LittleEndian.Uint16(a.Data[end-2:]), binary.LittleEndian.Uint16(b.Data[end-2:])
		if p != q {
			changes = append(changes, FieldChange{fmt.Sprintf("planet %d idRoute", id), fmt.Sprint(p & 0x3ff), fmt.Sprint(q & 0x3ff)})
		}
	}
	if planet.FRouting {
		end -= 2
	}
	if planet.FStarbase && end >= 4 && bytes.Equal(a.Data[:end-2], b.Data[:end-2]) && bytes.Equal(a.Data[end:], b.Data[end:]) {
		p, q := binary.LittleEndian.Uint16(a.Data[end-2:]), binary.LittleEndian.Uint16(b.Data[end-2:])
		if p != q {
			changes = append(changes, FieldChange{fmt.Sprintf("planet %d starbase idFling", id), fmt.Sprint(p & 0x3ff), fmt.Sprint(q & 0x3ff)})
		}
	}
	return changes
}

// BOF is the decoded file header record (RTBOF).
type BOF struct {
	Magic     string
	LidGame   int32
	VerMajor  int
	VerMinor  int
	VerInc    int
	Turn      uint16
	IPlayer   int16
	LSaltTime int16
	Dt        DtFileType
	FDone     bool
	FInUse    bool
	FMulti    bool
	FGameOver bool
	FCrippled bool
	WGen      int
}

// ParseBOF decodes an rtBOF record payload.
func ParseBOF(data []byte) (BOF, error) {
	if len(data) < 16 {
		return BOF{}, fmt.Errorf("BOF record is %d bytes, want 16", len(data))
	}
	ver := binary.LittleEndian.Uint16(data[8:])
	w12 := binary.LittleEndian.Uint16(data[12:])
	w14 := binary.LittleEndian.Uint16(data[14:])
	return BOF{
		Magic:     string(data[0:4]),
		LidGame:   int32(binary.LittleEndian.Uint32(data[4:])),
		VerInc:    int(ver & 0x1f),
		VerMinor:  int((ver >> 5) & 0x7f),
		VerMajor:  int(ver >> 12),
		Turn:      binary.LittleEndian.Uint16(data[10:]),
		IPlayer:   int16(w12<<11) >> 11,
		LSaltTime: int16(w12) >> 5,
		Dt:        DtFileType(w14 & 0xff),
		FDone:     w14&0x100 != 0,
		FInUse:    w14&0x200 != 0,
		FMulti:    w14&0x400 != 0,
		FGameOver: w14&0x800 != 0,
		FCrippled: w14&0x1000 != 0,
		WGen:      int(w14 >> 13),
	}, nil
}

// xorStream is the file XOR generator (lFileSeed1/lFileSeed2).
type xorStream struct {
	s1, s2 int32
}

// newXorStream seeds the XOR stream from a file header, as SetFileXorStream.
func newXorStream(primes []int16, bof BOF) (*xorStream, error) {
	salt := bof.LSaltTime
	a := int(salt & 0x1f)
	b := int((salt >> 5) & 0x1f)
	if salt&0x400 != 0 {
		a += 32
	} else {
		b += 32
	}
	if a >= len(primes) || b >= len(primes) {
		return nil, fmt.Errorf("prime index %d/%d out of range of %d primes", a, b, len(primes))
	}
	x := &xorStream{s1: int32(primes[a]), s2: int32(primes[b])}
	crippled := int16(0)
	if bof.FCrippled {
		crippled = 1
	}
	n := ((int16(bof.LidGame)&3)+1)*((int16(bof.Turn)&3)+1)*((bof.IPlayer&3)+1) + crippled
	for ; n > 0; n-- {
		x.next()
	}
	return x, nil
}

// next advances the generator, as LGetNextFileXor.
func (x *xorStream) next() int32 {
	s1, s2 := x.s1, x.s2
	k := s1 / 53668
	s1 = (s1-k*53668)*40014 - k*12211
	if s1 < 0 {
		s1 += 2147483563
	}
	k = s2 / 52774
	s2 = (s2-k*52774)*40692 - k*3791
	if s2 < 0 {
		s2 += 2147483399
	}
	x.s1, x.s2 = s1, s2
	return s1 - s2
}

// xor decrypts buf in place, as XorFileBuf.
func (x *xorStream) xor(buf []byte) {
	i := 0
	for ; i+4 <= len(buf); i += 4 {
		v := binary.LittleEndian.Uint32(buf[i:]) ^ uint32(x.next())
		binary.LittleEndian.PutUint32(buf[i:], v)
	}
	if i < len(buf) {
		l := uint32(x.next())
		for ; i < len(buf); i++ {
			buf[i] ^= byte(l)
			l >>= 8
		}
	}
}

// ReadRecords splits a file into records and decrypts each one. Every rtBOF
// record reseeds the XOR stream; records before the first rtBOF are an error.
// In XY files, rtGame is followed by a raw STARPACK array, not record headers.
func ReadRecords(b []byte, t Tables) ([]Record, error) {
	var recs []Record
	var x *xorStream
	isXY := false
	for off := 0; off < len(b); {
		if off+2 > len(b) {
			return recs, fmt.Errorf("truncated record header at 0x%x", off)
		}
		hdr := binary.LittleEndian.Uint16(b[off:])
		rt, cb := int(hdr>>10), int(hdr&0x3ff)
		if off+2+cb > len(b) {
			return recs, fmt.Errorf("record rt=%d cb=%d at 0x%x runs past end of file", rt, cb, off)
		}
		data := append([]byte(nil), b[off+2:off+2+cb]...)
		switch {
		case rt == RtBOF:
			bof, err := ParseBOF(data)
			if err != nil {
				return recs, fmt.Errorf("at 0x%x: %w", off, err)
			}
			if x, err = newXorStream(t.Primes, bof); err != nil {
				return recs, fmt.Errorf("at 0x%x: %w", off, err)
			}
			isXY = bof.Dt == 0 // dtXY
		case rt == RtEOF:
		case x == nil:
			return recs, fmt.Errorf("record rt=%d at 0x%x precedes the file header", rt, off)
		default:
			x.xor(data)
		}
		recs = append(recs, Record{Offset: off, Type: rt, Data: data})
		off += 2 + cb
		if isXY && rt == RtGame {
			// GAME.cPlanMax and STARPACK layout are defined in decompiled/structs.h.
			if len(data) != 64 {
				return recs, fmt.Errorf("XY game record is %d bytes, want 64", len(data))
			}
			count := int(int16(binary.LittleEndian.Uint16(data[10:])))
			if count < 0 || count > 1000 {
				return recs, fmt.Errorf("XY planet count %d is outside 0..1000", count)
			}
			if count*4 > len(b)-off {
				return recs, fmt.Errorf("XY STARPACK array at 0x%x needs %d bytes, have %d", off, count*4, len(b)-off)
			}
			stars := make([]Star, count)
			xpos := 1000
			for i := range stars {
				packed, err := ParseStarPack(b[off : off+4])
				if err != nil {
					return recs, fmt.Errorf("XY STARPACK %d at 0x%x: %w", i, off, err)
				}
				xpos += int(packed.DX)
				stars[i] = Star{X: xpos, Y: int(packed.Y), ID: int(packed.ID)}
				off += 4
			}
			recs[len(recs)-1].Stars = stars
		}
	}
	return recs, nil
}

// Message is one turn message from an rtMsg record (MSGHDR plus parameters).
type Message struct {
	ID   int
	Goto int16
	// Wide marks parameters stored as 16 bits (grWord); others are one byte.
	Wide []bool
	Args []uint16
}

// ParseMessages decodes the packed messages in an rtMsg record, walking them
// as ReadPlayerMessages does.
func ParseMessages(data []byte, argCounts []byte) ([]Message, error) {
	var msgs []Message
	for off := 0; off < len(data); {
		if off+4 > len(data) {
			return msgs, fmt.Errorf("truncated message header at +0x%x", off)
		}
		w := binary.LittleEndian.Uint16(data[off:])
		m := Message{ID: int(w & 0x1ff), Goto: int16(binary.LittleEndian.Uint16(data[off+2:]))}
		if m.ID >= len(argCounts) {
			return msgs, fmt.Errorf("message id %d at +0x%x is past %d known messages", m.ID, off, len(argCounts))
		}
		off += 4
		gr := w >> 9
		for i := 0; i < int(argCounts[m.ID]); i++ {
			wide := gr&1 != 0
			gr >>= 1
			n := 1
			if wide {
				n = 2
			}
			if off+n > len(data) {
				return msgs, fmt.Errorf("message id %d argument %d runs past record end", m.ID, i)
			}
			v := uint16(data[off])
			if wide {
				v = binary.LittleEndian.Uint16(data[off:])
			}
			m.Wide = append(m.Wide, wide)
			m.Args = append(m.Args, v)
			off += n
		}
		msgs = append(msgs, m)
	}
	return msgs, nil
}

// PlayerMessage is a player-to-player message from an rtPlrMsg record (MSGPLR
// without its leading 4-byte list pointer).
type PlayerMessage struct {
	IPlrFrom int16
	IPlrTo   int16
	IInRe    int16
	CLen     int16
	Text     []byte
}

// ParsePlayerMessage decodes an rtPlrMsg record.
func ParsePlayerMessage(data []byte) (PlayerMessage, error) {
	if len(data) < 12 {
		return PlayerMessage{}, fmt.Errorf("player message record is %d bytes, want at least 12", len(data))
	}
	return PlayerMessage{
		IPlrFrom: int16(binary.LittleEndian.Uint16(data[4:])),
		IPlrTo:   int16(binary.LittleEndian.Uint16(data[6:])),
		IInRe:    int16(binary.LittleEndian.Uint16(data[8:])),
		CLen:     int16(binary.LittleEndian.Uint16(data[10:])),
		Text:     data[12:],
	}, nil
}
