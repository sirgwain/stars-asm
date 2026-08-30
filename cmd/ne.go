package cmd

import (
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/lensesio/tableprinter"
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/spf13/cobra"
)

func newNECmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "ne",
		Short: "NE executable inspection",
		Long:  `Load a Win16 NE executable and inspect its structure.`,
	}

	cmd.AddCommand(newNESegmentsCmd())
	cmd.AddCommand(newNEFixupsCmd())
	cmd.AddCommand(newNEStringCmd())
	return cmd
}

func newNESegmentsCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "segments",
		Short: "List NE segment table",
		Long:  `List all segments in the NE executable with their file offsets and sizes.`,
		RunE: func(cmd *cobra.Command, args []string) error {
			img, err := asm.OpenNE(filepath.Join(inputDir, "stars.exe"))
			if err != nil {
				fmt.Fprintf(os.Stderr, "failed to open NE: %v\n", err)
				os.Exit(2)
			}

			return printNESegments(img)
		},
	}

	return cmd
}

func newNEStringCmd() *cobra.Command {
	var segHex string
	var offHex string

	cmd := &cobra.Command{
		Use:   "string",
		Short: "Read a string from the NE image",
		Long:  `Read a string from the NE image.`,
		RunE: func(cmd *cobra.Command, args []string) error {
			img, err := asm.OpenNE(filepath.Join(inputDir, "stars.exe"))
			if err != nil {
				fmt.Fprintf(os.Stderr, "failed to open NE: %v\n", err)
				os.Exit(2)
			}

			var seg uint16
			var off uint32

			v, err := strconv.ParseUint(strings.ReplaceAll(segHex, "0x", ""), 16, 32)
			if err != nil {
				return fmt.Errorf("--seg: invalid hex address %q: %v", segHex, err)
			}
			seg = uint16(v)

			v, err = strconv.ParseUint(strings.ReplaceAll(offHex, "0x", ""), 16, 32)
			if err != nil {
				return fmt.Errorf("--off: invalid hex address %q: %v", offHex, err)
			}
			off = uint32(v)

			str, ok := img.ReadCStringAt(seg, off)
			if !ok {
				fmt.Printf("no string found")
				return nil
			}
			fmt.Printf("str: %s", str)

			return nil
		},
	}

	cmd.Flags().StringVar(&segHex, "seg", "", "segment in hex (e.g. 0x25)")
	cmd.Flags().StringVar(&offHex, "off", "", "offset in hex (e.g. 0x0202)")
	cmd.MarkFlagRequired("seg")
	cmd.MarkFlagRequired("off")

	return cmd
}

func newNEFixupsCmd() *cobra.Command {
	var segText string
	var fromText string
	var toText string

	cmd := &cobra.Command{
		Use:   "fixups",
		Short: "List NE relocation fixups for a segment or range",
		Long:  `List NE relocation fixups for a segment, including source offsets and relocation targets.`,
		RunE: func(cmd *cobra.Command, args []string) error {
			if segText == "" {
				return fmt.Errorf("--seg is required")
			}
			seg, err := parseHex16Flag("seg", segText)
			if err != nil {
				return err
			}
			from := uint16(0)
			if fromText != "" {
				from, err = parseHex16Flag("from", fromText)
				if err != nil {
					return err
				}
			}
			to := uint16(0xffff)
			if toText != "" {
				to, err = parseHex16Flag("to", toText)
				if err != nil {
					return err
				}
			}
			if from > to {
				return fmt.Errorf("--from 0x%04x is after --to 0x%04x", from, to)
			}

			img, err := asm.OpenNE(filepath.Join(inputDir, "stars.exe"))
			if err != nil {
				fmt.Fprintf(os.Stderr, "failed to open NE: %v\n", err)
				os.Exit(2)
			}

			return printNEFixups(img, int(seg), from, to)
		},
	}

	cmd.Flags().StringVar(&segText, "seg", "", "NE/CV segment number in hex, e.g. 0020")
	cmd.Flags().StringVar(&fromText, "from", "", "first source offset in hex, e.g. 06b4")
	cmd.Flags().StringVar(&toText, "to", "", "last source offset in hex, e.g. 0a2f")
	return cmd
}

func printNEFixups(img *asm.ImageNE, segNum int, from, to uint16) error {
	type row struct {
		Source string `header:"Source"`
		Kind   string `header:"Kind"`
		Target string `header:"Target"`
		Flags  string `header:"Flags"`
	}

	fixups := img.Fixups(segNum)
	rows := make([]row, 0, len(fixups))
	for _, f := range fixups {
		if f.SrcOff < from || f.SrcOff > to {
			continue
		}
		rows = append(rows, row{
			Source: fmt.Sprintf("%04x:%04x", segNum, f.SrcOff),
			Kind:   fixupSourceString(f.Source),
			Target: fixupTargetString(f),
			Flags:  fixupFlagsString(f),
		})
	}

	printer := tableprinter.New(os.Stdout)
	printer.Print(rows)
	fmt.Printf("\n%d fixups in %04x:%04x..%04x\n", len(rows), segNum, from, to)
	return nil
}

func printNESegments(img *asm.ImageNE) error {
	type row struct {
		Num    int    `header:"Seg"`
		Offset string `header:"FileOffset"`
		Size   string `header:"Size"`
	}

	rows := make([]row, 0, len(img.Segs))
	for i := range img.Segs {
		segNum := i + 1
		fileOff, avail, err := img.SegFileOffset(segNum, 0)
		if err != nil {
			continue
		}
		rows = append(rows, row{
			Num:    segNum,
			Offset: fmt.Sprintf("0x%04x", fileOff),
			Size:   fmt.Sprintf("0x%04x", avail),
		})
	}

	printer := tableprinter.New(os.Stdout)
	printer.Print(rows)
	fmt.Printf("\n%d segments\n", len(rows))
	return nil
}

func parseHex16Flag(name, text string) (uint16, error) {
	s := strings.TrimSpace(text)
	s = strings.TrimPrefix(strings.ToLower(s), "0x")
	v, err := strconv.ParseUint(s, 16, 16)
	if err != nil {
		return 0, fmt.Errorf("--%s must be a 16-bit hex value: %q", name, text)
	}
	return uint16(v), nil
}

func fixupSourceString(src asm.FixupSource) string {
	switch src {
	case asm.FixupSourceLowByte:
		return "low-byte"
	case asm.FixupSourceSegment:
		return "segment"
	default:
		return fmt.Sprintf("source-0x%02x", uint8(src))
	}
}

func fixupTargetString(f asm.Fixup) string {
	switch f.Target {
	case asm.FixupTargetInternalRef:
		return fmt.Sprintf("internal %04x:%04x", f.TargetSegNum, f.TargetOff)
	case asm.FixupTargetImportOrdinal:
		return fmt.Sprintf("import module=%d ordinal=%d", f.ModuleIdx, f.Ordinal)
	default:
		return fmt.Sprintf("target-0x%02x", uint8(f.Target))
	}
}

func fixupFlagsString(f asm.Fixup) string {
	if f.Additive {
		return "additive"
	}
	return "chain"
}

func init() {
	rootCmd.AddCommand(newNECmd())
}
