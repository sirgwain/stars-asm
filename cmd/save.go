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
	return cmd
}

// newSaveDumpCmd returns the command that dumps the records of a game file.
func newSaveDumpCmd() *cobra.Command {
	var showHex bool
	var onlyMsgs bool
	cmd := &cobra.Command{
		Use:   "dump <file>...",
		Short: "Dump the decrypted records of game files",
		Long: `Decrypt each record of a Stars! game file and print it. The file header,
turn messages, and player messages are decoded; other records are listed by type
and size (use --hex to see their bytes).`,
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
				if err := d.dump(path, showHex, onlyMsgs); err != nil {
					return fmt.Errorf("%s: %w", path, err)
				}
			}
			return nil
		},
	}
	cmd.Flags().BoolVar(&showHex, "hex", false, "hex dump every decrypted record")
	cmd.Flags().BoolVar(&onlyMsgs, "msgs", false, "only print the header and message records")
	return cmd
}

// saveDumper holds the exe tables and enum names used to decode game files.
type saveDumper struct {
	tables   savefile.Tables
	rtNames  map[int]string
	dtNames  map[int]string
	msgNames map[int]string
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
	}{{"RecordType", &d.rtNames}, {"DtFileType", &d.dtNames}, {"MessageId", &d.msgNames}} {
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
func (d *saveDumper) dump(path string, showHex, onlyMsgs bool) error {
	b, err := os.ReadFile(path)
	if err != nil {
		return err
	}
	fmt.Printf("== %s (%d bytes)\n", path, len(b))
	recs, readErr := savefile.ReadRecords(b, d.tables)

	counts := map[int]int{}
	cMsg := 0
	for _, r := range recs {
		counts[r.Type]++
		isMsg := r.Type == savefile.RtMsg || r.Type == savefile.RtPlrMsg || r.Type == savefile.RtMsgFilt
		if onlyMsgs && !isMsg && r.Type != savefile.RtBOF {
			continue
		}
		fmt.Printf("%06x  %-22s cb=%d\n", r.Offset, enumName(d.rtNames, r.Type), len(r.Data))
		switch r.Type {
		case savefile.RtBOF:
			bof, err := savefile.ParseBOF(r.Data)
			if err != nil {
				return err
			}
			fmt.Printf("        magic=%q game=%08x ver=%d.%d.%d turn=%d (year %d) player=%d salt=%d dt=%s\n",
				bof.Magic, uint32(bof.LidGame), bof.VerMajor, bof.VerMinor, bof.VerInc, bof.Turn, 2400+int(bof.Turn),
				bof.IPlayer, bof.LSaltTime, enumName(d.dtNames, bof.Dt))
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
