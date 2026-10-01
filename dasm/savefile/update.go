package savefile

import (
	"encoding/binary"
	"fmt"
	"os"
)

// UpdatePlayerAI assigns Maid AI to a player in an encrypted host or turn file.
// playerIndex is zero-based; -1 selects the player named by a turn file's BOF.
func UpdatePlayerAI(path string, playerIndex int) error {
	b, err := os.ReadFile(path)
	if err != nil {
		return err
	}
	records, err := ReadRecords(b, DefaultTables())
	if err != nil {
		return err
	}
	if len(records) == 0 || records[0].Type != RtBOF {
		return fmt.Errorf("missing turn file header")
	}
	bof, err := ParseBOF(records[0].Data)
	if err != nil {
		return err
	}
	if bof.Dt != DtTurn && bof.Dt != DtHost {
		return fmt.Errorf("expected a host or player turn file, got type %d", bof.Dt)
	}
	if bof.Dt == DtTurn {
		if bof.IPlayer < 0 || bof.IPlayer > 15 {
			return fmt.Errorf("invalid turn file player %d", bof.IPlayer)
		}
		if playerIndex == -1 {
			playerIndex = int(bof.IPlayer)
		} else if playerIndex != int(bof.IPlayer) {
			return fmt.Errorf("turn file player %d does not match requested player %d", bof.IPlayer, playerIndex)
		}
	} else if playerIndex == -1 {
		return fmt.Errorf("host file requires a player index")
	}
	if playerIndex < 0 || playerIndex > 15 {
		return fmt.Errorf("player index %d is outside 0..15", playerIndex)
	}
	var playerRecord *Record
	for i := range records {
		if records[i].Type != RtPlr {
			continue
		}
		player, err := ParsePlayerHeader(records[i].Data)
		if err != nil {
			return err
		}
		if int(player.IPlayer) != playerIndex {
			if bof.Dt == DtTurn {
				return fmt.Errorf("player record %d does not match turn file player %d", player.IPlayer, bof.IPlayer)
			}
			continue
		}
		if playerRecord != nil {
			return fmt.Errorf("multiple player records in turn file")
		}
		playerRecord = &records[i]
	}
	if playerRecord == nil {
		return fmt.Errorf("missing player record %d", playerIndex)
	}
	if len(playerRecord.Data) < 16 {
		return fmt.Errorf("player record %d is %d bytes, want at least 16", playerIndex, len(playerRecord.Data))
	}
	// PLAYER.wMdPlr: fAi is bit 9 and idAi is bits 13..15 (structs.h).
	old := binary.LittleEndian.Uint16(playerRecord.Data[6:])
	updated := old&^uint16(0xe200) | 0xe200
	if updated == old {
		return nil
	}
	// XOR encryption is symmetric, so the plaintext difference is also the
	// ciphertext difference. This leaves every other encrypted byte untouched.
	off := playerRecord.Offset + 2 + 6
	delta := old ^ updated
	b[off] ^= byte(delta)
	b[off+1] ^= byte(delta >> 8)
	if old&0x200 == 0 {
		// FMarkFile in save.c complements PLAYER.lSalt when changing a human
		// player into an AI. Its four bytes start at offset 12 in PLAYER.
		for i := 0; i < 4; i++ {
			b[playerRecord.Offset+2+12+i] ^= 0xff
		}
	}
	return os.WriteFile(path, b, 0o666)
}
