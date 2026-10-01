package cmd

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"regexp"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/savefile"
	"github.com/spf13/cobra"
)

// newSaveCompareCmd compares decrypted files while excluding clock-derived identity fields.
func newSaveCompareCmd() *cobra.Command {
	var showBytes bool
	cmd := &cobra.Command{
		Use:          "compare <left> <right>",
		Short:        "Compare decrypted saves, ignoring game IDs and encryption salts",
		SilenceUsage: true,
		Args:         cobra.ExactArgs(2),
		RunE: func(cmd *cobra.Command, args []string) error {
			tables := savefile.DefaultTables()
			var sides [2][]savefile.Record
			var layouts [2]savefile.AIHistoryLayout
			for i, path := range args {
				b, err := os.ReadFile(path)
				if err != nil {
					return err
				}
				sides[i], err = comparisonRecords(b, strings.EqualFold(filepath.Ext(path), ".xy"), tables)
				if err != nil {
					return fmt.Errorf("%s: %w", path, err)
				}
				if layouts[i], err = aiHistoryLayout(path, sides[i], tables); err != nil {
					return fmt.Errorf("%s: %w", path, err)
				}
			}
			if layouts[0] != layouts[1] {
				return fmt.Errorf("AI history owners differ: %s uses %s layout, %s uses %s", args[0], layouts[0], args[1], layouts[1])
			}
			differences := savefile.CompareRecords(sides[0], sides[1], layouts[0])
			if len(differences) == 0 {
				fmt.Fprintln(cmd.OutOrStdout(), "MATCH (decrypted; game IDs and encryption salts excluded)")
				return nil
			}
			var out strings.Builder
			benign := true
			for _, diff := range differences {
				formatRecordDifference(&out, diff, showBytes)
				benign = benign && diff.Benign
			}
			if !benign {
				return fmt.Errorf("%s", strings.TrimSuffix(out.String(), "\n"))
			}
			fmt.Fprintf(cmd.OutOrStdout(), "MATCH with warnings: only unused storage differs (decrypted; game IDs and encryption salts excluded)\n%s", out.String())
			return nil
		},
	}
	cmd.Flags().BoolVar(&showBytes, "show-bytes", false, "show changed decrypted bytes and full payloads")
	return cmd
}

// formatRecordDifference writes semantic changes and optional decrypted bytes.
func formatRecordDifference(out *strings.Builder, diff savefile.RecordDifference, showBytes bool) {
	if len(diff.Left) == 0 || len(diff.Right) == 0 {
		fmt.Fprintf(out, "Record %d: record count differs (%d left, %d right remain)\n", diff.Index, len(diff.Left), len(diff.Right))
		return
	}
	a, b := diff.Left[0], diff.Right[0]
	name := savefile.RecordType(a.Type).String()
	if a.Type == -1 {
		name = "STARPACK coordinates"
	}
	if a.Type != b.Type {
		name += " → " + savefile.RecordType(b.Type).String()
	}
	if diff.Label != "" && a.Type != -1 {
		name += " — " + diff.Label
	}
	fmt.Fprintf(out, "Record %d: %s", diff.Index, name)
	var leftSize, rightSize int
	for _, record := range diff.Left {
		leftSize += len(record.Data)
	}
	for _, record := range diff.Right {
		rightSize += len(record.Data)
	}
	if diff.Benign {
		out.WriteString(" — only unused storage differs")
	} else if len(diff.Fields) == 0 {
		if leftSize == rightSize {
			fmt.Fprintf(out, " — payload changed (%d bytes)", leftSize)
		} else {
			fmt.Fprintf(out, " — payload changed (%d → %d bytes)", leftSize, rightSize)
		}
	}
	if len(diff.Left) != len(diff.Right) {
		fmt.Fprintf(out, " — %d → %d chunks", len(diff.Left), len(diff.Right))
	}
	out.WriteByte('\n')
	for _, field := range diff.Fields {
		if field.Before == "changed" && field.After == "changed" {
			fmt.Fprintf(out, "  %s changed\n", field.Path)
		} else {
			fmt.Fprintf(out, "  %s: %s → %s\n", field.Path, field.Before, field.After)
		}
	}
	for _, field := range diff.Unused {
		fmt.Fprintf(out, "  unused %s: %s → %s\n", field.Path, field.Before, field.After)
	}
	if !showBytes {
		return
	}
	for k := 0; k < len(diff.Left) && k < len(diff.Right); k++ {
		a, b := diff.Left[k], diff.Right[k]
		for i := 0; i < len(a.Data) && i < len(b.Data); i++ {
			if a.Data[i] != b.Data[i] {
				leftOff, rightOff := a.Offset+2+i, b.Offset+2+i
				if a.Type == -1 {
					leftOff, rightOff = a.Offset+i, b.Offset+i
				}
				fmt.Fprintf(out, "  byte +%#x (file %#x/%#x): %02x → %02x\n", i, leftOff, rightOff, a.Data[i], b.Data[i])
			}
		}
		fmt.Fprintf(out, "  left: %x\n  right: %x\n", a.Data, b.Data)
	}
}

// historyExt matches player history extensions (.h1-.h16).
var historyExt = regexp.MustCompile(`(?i)^\.h([1-9]|1[0-6])$`)

// aiHistoryLayout identifies a history file's rtAiData layout from the owning
// player's companion turn file (.hN → .mN), since histories omit PLAYER records.
// Other files and histories without a companion turn file are left undecoded.
func aiHistoryLayout(path string, recs []savefile.Record, tables savefile.Tables) (savefile.AIHistoryLayout, error) {
	ext := filepath.Ext(path)
	if !historyExt.MatchString(ext) {
		return savefile.AIHistoryUnknown, nil
	}
	m := "m"
	if ext[1] == 'H' {
		m = "M"
	}
	turnPath := strings.TrimSuffix(path, ext) + "." + m + ext[2:]
	b, err := os.ReadFile(turnPath)
	if errors.Is(err, fs.ErrNotExist) {
		return savefile.AIHistoryUnknown, nil
	}
	if err != nil {
		return savefile.AIHistoryUnknown, err
	}
	bof, err := savefile.ParseBOF(recs[0].Data)
	if err != nil {
		return savefile.AIHistoryUnknown, err
	}
	turn, err := savefile.ReadRecords(b, tables)
	if err != nil {
		return savefile.AIHistoryUnknown, fmt.Errorf("%s: %w", turnPath, err)
	}
	for _, r := range turn {
		if r.Type != savefile.RtPlr {
			continue
		}
		player, err := savefile.ParsePlayerHeader(r.Data)
		if err != nil {
			return savefile.AIHistoryUnknown, fmt.Errorf("%s: %w", turnPath, err)
		}
		if int16(player.IPlayer) == bof.IPlayer {
			return savefile.AIHistoryLayoutFor(player), nil
		}
	}
	return savefile.AIHistoryUnknown, fmt.Errorf("%s has no PLAYER record for player %d", turnPath, bof.IPlayer+1)
}

// comparisonRecords decrypts saves and preserves the raw star-coordinate section in XY files.
func comparisonRecords(b []byte, xy bool, tables savefile.Tables) ([]savefile.Record, error) {
	var tail []byte
	if xy {
		// XY starts with RTBOF (16 bytes), GAME (64 bytes), then raw STARPACKs.
		const end = 18 + 66
		if len(b) < end || binary.LittleEndian.Uint16(b) != 8<<10|16 || binary.LittleEndian.Uint16(b[18:]) != 7<<10|64 {
			return nil, fmt.Errorf("invalid XY BOF/GAME header")
		}
		tail = b[end:]
	}
	recs, err := savefile.ReadRecords(b, tables)
	if err != nil {
		return nil, err
	}
	if len(recs) == 0 || recs[0].Type != savefile.RtBOF {
		return nil, fmt.Errorf("missing BOF")
	}
	if recs[len(recs)-1].Type != savefile.RtEOF {
		return nil, fmt.Errorf("missing final EOF record")
	}
	if xy {
		if len(recs) < 3 || recs[1].Type != savefile.RtGame {
			return nil, fmt.Errorf("missing XY GAME record")
		}
		count := int(binary.LittleEndian.Uint16(recs[1].Data[10:]))
		if len(tail) != count*4+4 || binary.LittleEndian.Uint16(tail[count*4:]) != 2 {
			return nil, fmt.Errorf("invalid XY coordinate count or EOF")
		}
		// The reader decodes Stars on rtGame; retain the exact STARPACK bytes
		// and EOF here so comparison also catches changes in their encoding.
		recs = append(recs[:2], savefile.Record{Offset: 84, Type: -1, Data: append([]byte(nil), tail...), Stars: recs[1].Stars})
	}
	for _, r := range recs {
		switch r.Type {
		case savefile.RtBOF:
			if len(r.Data) != 16 || string(r.Data[:4]) != "J3J3" {
				return nil, fmt.Errorf("invalid Stars! BOF record at %#x", r.Offset)
			}
			// RTBOF.lidGame and lSaltTime: structs.h offsets 4 and 12.
			clear(r.Data[4:8])
			binary.LittleEndian.PutUint16(r.Data[12:], binary.LittleEndian.Uint16(r.Data[12:])&31)
		case 7:
			if len(r.Data) != 64 {
				return nil, fmt.Errorf("invalid GAME size %d", len(r.Data))
			}
			clear(r.Data[:4]) // GAME.lid, structs.h offset 0.
		}
	}
	return recs, nil
}
