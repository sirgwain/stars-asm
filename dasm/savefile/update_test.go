package savefile

import (
	"bytes"
	"encoding/binary"
	"os"
	"path/filepath"
	"testing"
)

// TestUpdatePlayerAI verifies turn and host files remain decryptable after AI assignment.
func TestUpdatePlayerAI(t *testing.T) {
	for _, tc := range []struct {
		name        string
		dt          DtFileType
		bofPlayer   uint16
		playerIndex int
	}{
		{"turn", DtTurn, 0, -1},
		{"host", DtHost, 0x1f, 0},
	} {
		t.Run(tc.name, func(t *testing.T) {
			b := make([]byte, 18+2+130+2)
			binary.LittleEndian.PutUint16(b, 8<<10|16)
			copy(b[2:], "J3J3")
			binary.LittleEndian.PutUint32(b[6:], 12345)
			binary.LittleEndian.PutUint16(b[10:], 2<<12|83<<5)
			binary.LittleEndian.PutUint16(b[12:], 31<<5|tc.bofPlayer)
			binary.LittleEndian.PutUint16(b[16:], uint16(tc.dt))
			binary.LittleEndian.PutUint16(b[18:], 6<<10|130)
			player := b[20:150]
			player[0] = 0
			player[1] = 6
			binary.LittleEndian.PutUint16(player[6:], 0x010f)
			player[112] = 0
			copy(player[113:], "Humans")
			player[120] = 0
			copy(player[121:], "Humanoid")
			records, err := ReadRecords(b, DefaultTables())
			if err != nil {
				t.Fatal(err)
			}
			copy(b[20:150], records[1].Data) // XOR the plaintext into file ciphertext.
			path := filepath.Join(t.TempDir(), "game.m1")
			if err := os.WriteFile(path, b, 0o600); err != nil {
				t.Fatal(err)
			}
			if err := UpdatePlayerAI(path, tc.playerIndex); err != nil {
				t.Fatal(err)
			}
			updated, err := os.ReadFile(path)
			if err != nil {
				t.Fatal(err)
			}
			got, err := ReadRecords(updated, DefaultTables())
			if err != nil {
				t.Fatal(err)
			}
			header, err := ParsePlayerHeader(got[1].Data)
			if err != nil {
				t.Fatal(err)
			}
			if !header.AI || header.AIID != 7 || header.AILevel != 0 || header.WMdPlr != 0xe30f {
				t.Fatalf("unexpected player AI: %+v", header)
			}
			if binary.LittleEndian.Uint32(got[1].Data[12:]) != ^uint32(0) {
				t.Fatal("PLAYER.lSalt was not complemented")
			}
			want := bytes.Clone(b)
			for _, off := range []int{26, 27, 32, 33, 34, 35} {
				want[off] = updated[off]
			}
			if !bytes.Equal(updated, want) {
				t.Fatal("changed bytes outside PLAYER.wMdPlr and PLAYER.lSalt")
			}
			if err := UpdatePlayerAI(path, tc.playerIndex); err != nil {
				t.Fatal(err)
			}
			again, err := os.ReadFile(path)
			if err != nil || !bytes.Equal(updated, again) {
				t.Fatalf("second update changed the file: %v", err)
			}
		})
	}
}
