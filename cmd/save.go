package cmd

import (
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"os"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/savefile"
	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/sirgwain/stars-asm/dasm/starsenv"
	"github.com/spf13/cobra"
)

// newSaveCmd returns the command group for inspecting Stars! game files.
func newSaveCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "save",
		Short: "Stars! game file inspection",
		Long:  `Decrypt and dump Stars! game files (.xy, .hst, .m1-.m16, .x1-.x16, .h1-.h16).`,
	}
	cmd.AddCommand(newSaveDumpCmd())
	cmd.AddCommand(newSaveCompareCmd())
	cmd.AddCommand(newSaveUpdateCmd())
	return cmd
}

// newSaveUpdateCmd returns the command that assigns Maid AI in a turn or host file.
func newSaveUpdateCmd() *cobra.Command {
	var ai string
	var player int
	cmd := &cobra.Command{
		Use:   "update <file>",
		Short: "Update a player turn or host file",
		Args:  cobra.ExactArgs(1),
		RunE: func(cmd *cobra.Command, args []string) error {
			if ai != "maid" {
				return fmt.Errorf("--ai must be maid")
			}
			if cmd.Flags().Changed("player") {
				if player < 1 || player > 16 {
					return fmt.Errorf("--player must be between 1 and 16")
				}
				return savefile.UpdatePlayerAI(args[0], player-1)
			}
			return savefile.UpdatePlayerAI(args[0], -1)
		},
	}
	cmd.Flags().StringVar(&ai, "ai", "", "AI to assign (maid)")
	cmd.Flags().IntVar(&player, "player", 0, "player number (1-16; required for host files)")
	_ = cmd.MarkFlagRequired("ai")
	return cmd
}

// newSaveDumpCmd returns the command that dumps the records of a game file.
func newSaveDumpCmd() *cobra.Command {
	var showHex bool
	var onlyMsgs bool
	var onlyOrders bool
	cmd := &cobra.Command{
		Use:   "dump <file>...",
		Short: "Dump the decrypted records of game files",
		Long: `Decrypt each record of a Stars! game file and print it. The file header,
turn messages, player messages, waypoints, and cargo transfer orders are decoded;
other records are listed by type and size (use --hex to see their bytes).`,
		Args: cobra.MinimumNArgs(1),
		RunE: func(cmd *cobra.Command, args []string) error {
			env, err := starsenv.LoadStars(starsenv.Options{InputDir: inputDir})
			if err != nil {
				return err
			}
			d, err := newSaveDumper(env)
			if err != nil {
				return err
			}
			for _, path := range args {
				if err := d.dump(path, showHex, onlyMsgs, onlyOrders); err != nil {
					return fmt.Errorf("%s: %w", path, err)
				}
			}
			return nil
		},
	}
	cmd.Flags().BoolVar(&showHex, "hex", false, "hex dump every decrypted record")
	cmd.Flags().BoolVar(&onlyMsgs, "msgs", false, "only print the header and message records")
	cmd.Flags().BoolVar(&onlyOrders, "orders", false, "only print the header, fleet context, waypoints, and cargo transfer orders")
	cmd.MarkFlagsMutuallyExclusive("msgs", "orders")
	return cmd
}

// saveDumper holds the exe tables and enum names used to decode game files.
type saveDumper struct {
	tables      savefile.Tables
	rtNames     map[int]string
	dtNames     map[int]string
	msgNames    map[int]string
	taskNames   map[int]string
	objectNames map[int]string
	actionNames map[int]string
}

// newSaveDumper reads rgPrimes and rgcMsgArgs from stars.exe and the record,
// file, and message enum names from the symbol database.
func newSaveDumper(env *starsenv.Env) (*saveDumper, error) {
	primes, err := readSaveGlobal(env, "rgPrimes")
	if err != nil {
		return nil, err
	}
	argCounts, err := readSaveGlobal(env, "rgcMsgArgs")
	if err != nil {
		return nil, err
	}
	d := &saveDumper{tables: savefile.Tables{MsgArgCounts: argCounts}}
	for i := 0; i+1 < len(primes); i += 2 {
		d.tables.Primes = append(d.tables.Primes, int16(binary.LittleEndian.Uint16(primes[i:])))
	}
	for _, e := range []struct {
		name string
		dst  *map[int]string
	}{{"RecordType", &d.rtNames}, {"DtFileType", &d.dtNames}, {"MessageId", &d.msgNames}, {"TaskType", &d.taskNames}, {"GrobjClass", &d.objectNames}, {"XferActionType", &d.actionNames}} {
		enum := env.SDB.GetEnum(e.name)
		if enum == nil {
			return nil, fmt.Errorf("enum %s not found in symbol database", e.name)
		}
		*e.dst = map[int]string{}
		for _, v := range enum.Values {
			if _, dup := (*e.dst)[v.Value]; !dup {
				(*e.dst)[v.Value] = v.Name
			}
		}
	}
	return d, nil
}

// readSaveGlobal reads the static bytes of a named global from stars.exe.
func readSaveGlobal(env *starsenv.Env, name string) ([]byte, error) {
	g := env.SDB.GetGlobal(name)
	if g == nil {
		return nil, fmt.Errorf("global %s not found in symbol database", name)
	}
	b, ok := stars.ReadGlobalBytes(env.Image, g)
	if !ok {
		return nil, fmt.Errorf("could not read %s from stars.exe", name)
	}
	return b, nil
}

// enumName returns the enum value name, or the number when it has none.
func enumName(names map[int]string, v int) string {
	if n, ok := names[v]; ok {
		return n
	}
	return fmt.Sprintf("%d", v)
}

// dump prints the records of one game file.
func (d *saveDumper) dump(path string, showHex, onlyMsgs, onlyOrders bool) error {
	b, err := os.ReadFile(path)
	if err != nil {
		return err
	}
	fmt.Printf("== %s (%d bytes)\n", path, len(b))
	recs, readErr := savefile.ReadRecords(b, d.tables)

	counts := map[int]int{}
	cMsg := 0
	fleetID, waypoint := -1, 0
	for _, r := range recs {
		counts[r.Type]++
		isMsg := r.Type == savefile.RtMsg || r.Type == savefile.RtPlrMsg || r.Type == savefile.RtMsgFilt
		isOrder := false
		switch r.Type {
		case savefile.RtLogCargoXfer8, savefile.RtLogCargoXfer16, savefile.RtLogCargoXfer32,
			savefile.RtLogFleetOrderDelete, savefile.RtLogFleetOrderInsert, savefile.RtLogFleetOrderUpdate,
			savefile.RtLogFleetFlagBit9, savefile.RtLogFleetOrderAttrNib, savefile.RtOrderA, savefile.RtOrderB:
			isOrder = true
		}
		if onlyOrders && !isOrder && r.Type != savefile.RtBOF && r.Type != savefile.RtFleetA {
			continue
		}
		if onlyMsgs && !isMsg && r.Type != savefile.RtBOF {
			continue
		}
		fmt.Printf("%06x  %-22s cb=%d\n", r.Offset, enumName(d.rtNames, r.Type), len(r.Data))
		switch r.Type {
		case savefile.RtGame:
			for i, star := range r.Stars {
				fmt.Printf("        STARPACK[%d] position=(%d,%d) id=%d\n", i, star.X, star.Y, star.ID)
			}
		case savefile.RtFleetA: // FReadFleet copies the leading FLEET fields verbatim.
			if len(r.Data) < 12 {
				return fmt.Errorf("fleet record at 0x%x is %d bytes, want at least 12", r.Offset, len(r.Data))
			}
			fleetID, waypoint = int(binary.LittleEndian.Uint16(r.Data)), 0
			fmt.Printf("        fleet=%d (raw ID; displayed fleet number=%d)\n", fleetID, (fleetID&0x1ff)+1)
		default:
			if !isOrder {
				break
			}
			if r.Type == savefile.RtOrderA || r.Type == savefile.RtOrderB {
				if fleetID < 0 {
					return fmt.Errorf("waypoint record at 0x%x has no preceding fleet", r.Offset)
				}
				fmt.Printf("        fleet=%d waypoint=%d\n", fleetID, waypoint)
				waypoint++
			}
			order, err := d.formatOrderRecord(r.Type, r.Data)
			if err != nil {
				return fmt.Errorf("order record at 0x%x: %w", r.Offset, err)
			}
			fmt.Print(order)
		case savefile.RtBOF:
			fleetID, waypoint = -1, 0
			bof, err := savefile.ParseBOF(r.Data)
			if err != nil {
				return err
			}
			fmt.Printf("        magic=%q game=%08x ver=%d.%d.%d turn=%d (year %d) player=%d salt=%d dt=%s\n",
				bof.Magic, uint32(bof.LidGame), bof.VerMajor, bof.VerMinor, bof.VerInc, bof.Turn, 2400+int(bof.Turn),
				bof.IPlayer, bof.LSaltTime, enumName(d.dtNames, int(bof.Dt)))
			fmt.Printf("        done=%t inUse=%t multi=%t gameOver=%t crippled=%t wGen=%d\n",
				bof.FDone, bof.FInUse, bof.FMulti, bof.FGameOver, bof.FCrippled, bof.WGen)
		case savefile.RtMsg:
			msgs, err := savefile.ParseMessages(r.Data, d.tables.MsgArgCounts)
			for _, m := range msgs {
				cMsg++
				args := make([]string, len(m.Args))
				for i, a := range m.Args {
					args[i] = fmt.Sprintf("%d", a)
					if m.Wide[i] {
						args[i] += "w"
					}
				}
				fmt.Printf("        [%3d] %s goto=%d args=[%s]\n", m.ID, enumName(d.msgNames, m.ID), m.Goto, strings.Join(args, " "))
			}
			if err != nil {
				fmt.Printf("        !! %v\n", err)
			}
		case savefile.RtPlrMsg:
			pm, err := savefile.ParsePlayerMessage(r.Data)
			if err != nil {
				fmt.Printf("        !! %v\n", err)
				break
			}
			fmt.Printf("        from=%d to=%d inRe=%d cLen=%d text=%q\n", pm.IPlrFrom, pm.IPlrTo, pm.IInRe, pm.CLen, pm.Text)
		}
		if showHex && len(r.Data) > 0 {
			for _, line := range strings.Split(strings.TrimRight(hex.Dump(r.Data), "\n"), "\n") {
				fmt.Printf("        %s\n", line)
			}
		}
	}

	fmt.Printf("-- %d records, %d turn messages\n", len(recs), cMsg)
	for rt := 0; rt < 64; rt++ {
		if counts[rt] > 0 {
			fmt.Printf("   %-22s %d\n", enumName(d.rtNames, rt), counts[rt])
		}
	}
	if readErr != nil {
		fmt.Printf("!! %v\n", readErr)
	}
	fmt.Println()
	return nil
}

func init() {
	rootCmd.AddCommand(newSaveCmd())
}

// formatOrderRecord decodes ORDER, RTWAYPT, RTSHIPINT, and RTXFER records
// using their packed layouts in decompiled/structs.h.
func (d *saveDumper) formatOrderRecord(rt int, data []byte) (string, error) {
	var out strings.Builder
	cargoNames := [...]string{"Ironium", "Boranium", "Germanium", "Colonists", "Fuel"}
	switch rt {
	case savefile.RtLogCargoXfer8, savefile.RtLogCargoXfer16, savefile.RtLogCargoXfer32:
		if len(data) < 6 {
			return "", fmt.Errorf("cargo transfer is %d bytes, want at least 6", len(data))
		}
		width := map[int]int{savefile.RtLogCargoXfer8: 1, savefile.RtLogCargoXfer16: 2, savefile.RtLogCargoXfer32: 4}[rt]
		if data[5]&^byte(0x1f) != 0 {
			return "", fmt.Errorf("unknown cargo mask 0x%02x", data[5])
		}
		fmt.Fprintf(&out, "        object1=%s id=%d object2=%s id=%d quantityBits=%d mask=0x%02x\n",
			enumName(d.objectNames, int(data[4]&15)), binary.LittleEndian.Uint16(data),
			enumName(d.objectNames, int(data[4]>>4)), binary.LittleEndian.Uint16(data[2:]), width*8, data[5])
		off := 6
		for i, name := range cargoNames {
			if data[5]&(1<<i) == 0 {
				continue
			}
			if off+width > len(data) {
				return "", fmt.Errorf("truncated %s quantity at +0x%x", name, off)
			}
			var quantity int32
			switch width {
			case 1:
				quantity = int32(int8(data[off]))
			case 2:
				quantity = int32(int16(binary.LittleEndian.Uint16(data[off:])))
			case 4:
				quantity = int32(binary.LittleEndian.Uint32(data[off:]))
			}
			unit := "kT"
			if i == 4 {
				unit = "mg"
			}
			fmt.Fprintf(&out, "        %s: object1 delta=%+d%s object2 delta=%+d%s raw=% x\n", name, quantity, unit, -int64(quantity), unit, data[off:off+width])
			off += width
		}
		if off != len(data) {
			return "", fmt.Errorf("cargo transfer has %d trailing bytes", len(data)-off)
		}
	case savefile.RtLogFleetOrderDelete, savefile.RtLogFleetFlagBit9, savefile.RtLogFleetOrderAttrNib:
		want := 4
		if rt == savefile.RtLogFleetOrderAttrNib {
			want = 6
		}
		if len(data) != want {
			return "", fmt.Errorf("record is %d bytes, want %d", len(data), want)
		}
		id, value := binary.LittleEndian.Uint16(data), binary.LittleEndian.Uint16(data[2:])
		switch rt {
		case savefile.RtLogFleetOrderDelete:
			fmt.Fprintf(&out, "        fleet=%d delete waypoint=%d count=%d\n", id, value&0x7fff, 1+(value>>15))
		case savefile.RtLogFleetFlagBit9:
			fmt.Fprintf(&out, "        fleet=%d repeatOrders=%d\n", id, value)
		case savefile.RtLogFleetOrderAttrNib:
			fmt.Fprintf(&out, "        fleet=%d waypoint=%d task=%s\n", id, value, enumName(d.taskNames, int(binary.LittleEndian.Uint16(data[4:]))))
		}
	case savefile.RtLogFleetOrderInsert, savefile.RtLogFleetOrderUpdate, savefile.RtOrderA, savefile.RtOrderB:
		if rt == savefile.RtLogFleetOrderInsert || rt == savefile.RtLogFleetOrderUpdate {
			if len(data) > 22 {
				return "", fmt.Errorf("waypoint edit is %d bytes, want at most 22", len(data))
			}
			// LogChangeFleet omits trailing zero bytes from the entire RTWAYPT.
			var packed [22]byte
			copy(packed[:], data)
			fmt.Fprintf(&out, "        fleet=%d waypoint=%d\n", binary.LittleEndian.Uint16(packed[:]), int16(binary.LittleEndian.Uint16(packed[2:])))
			data = packed[4:]
		} else {
			want := 18
			if rt == savefile.RtOrderB {
				want = 8
			}
			if len(data) != want {
				return "", fmt.Errorf("waypoint is %d bytes, want %d", len(data), want)
			}
		}
		flags := binary.LittleEndian.Uint16(data[6:])
		task := int(flags & 15)
		fmt.Fprintf(&out, "        position=(%d,%d) target=%s id=%d warp=%d task=%s validTask=%t noAutoTrack=%t\n",
			int16(binary.LittleEndian.Uint16(data)), int16(binary.LittleEndian.Uint16(data[2:])),
			enumName(d.objectNames, int(flags>>8&15)), int16(binary.LittleEndian.Uint16(data[4:])), flags>>4&15,
			enumName(d.taskNames, task), flags&0x1000 != 0, flags&0x2000 != 0)
		if len(data) == 18 {
			switch task {
			case 1:
				for i, name := range cargoNames {
					item := binary.LittleEndian.Uint16(data[8+2*i:])
					fmt.Fprintf(&out, "        %s: action=%s quantity=%d\n", name, enumName(d.actionNames, int(item>>12)), item&0xfff)
				}
			case 6:
				fmt.Fprintf(&out, "        mineYears=%d previousYears=%d\n", binary.LittleEndian.Uint16(data[8:]), binary.LittleEndian.Uint16(data[10:]))
			case 7:
				fmt.Fprintf(&out, "        patrolWarp=%d distance=%d\n", binary.LittleEndian.Uint16(data[8:]), binary.LittleEndian.Uint16(data[10:]))
			case 9:
				fmt.Fprintf(&out, "        recipientPlayer=%d\n", binary.LittleEndian.Uint16(data[8:]))
			}
		}
	default:
		return "", fmt.Errorf("unsupported order record type %d", rt)
	}
	return out.String(), nil
}
