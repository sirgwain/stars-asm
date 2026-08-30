package cmd

import (
	"fmt"
	"os"
	"slices"
	"strings"

	"github.com/lensesio/tableprinter"
	"github.com/sirgwain/stars-asm/dasm/nb09"
	"github.com/spf13/cobra"
)

var db *nb09.NB09DB

func newNB09Command() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "nb09",
		Short: "Interact with nb09 data",
		Long:  `Root command for interacting with nb09 data.`,
		PersistentPreRunE: func(cmd *cobra.Command, args []string) error {
			var err error
			db, err = nb09.Load(inputDir + "/stars.exe")
			if err != nil {
				return err
			}

			return nil
		},
	}

	cmd.AddCommand(newNB09DirectoriesCmd(),
		newNB09ModulesCmd(),
		newNB09LibrariesCmd(),
		newNB09SegmentsCmd(),
		newNB09AlignSymsCmd(),
		newNB09SrcModulesCmd(),
		newNB09GlobalPubsCmd(),
		newNB09GlobalSymsCmd(),
		newNB09StaticSymsCmd(),
		newNB09GlobalTypesCmd(),
		newNB09FileIndicesCmd(),
	)
	return cmd
}

func sortedUint16Keys[T any](m map[uint16]T) []uint16 {
	keys := make([]uint16, 0, len(m))
	for k := range m {
		keys = append(keys, k)
	}
	slices.Sort(keys)
	return keys
}

func describeSymRecord(db *nb09.NB09DB, r nb09.SymRecord) (name string, addr string, typind uint16, extra string) {
	switch v := r.Parsed.(type) {
	case *nb09.SObjName:
		if v != nil {
			return v.Name, "", 0, fmt.Sprintf("sig=0x%08x", v.Signature)
		}
	case *nb09.SProc16:
		if v != nil {
			return v.Name, fmt.Sprintf("%04x:%04x", v.Addr.Seg, v.Addr.Off), v.TypInd,
				fmt.Sprintf("len=0x%x flags=0x%02x parent=0x%08x next=0x%08x end=0x%08x typind=0x%04x", v.ProcLen, v.Flags, v.PParent, v.PNext, v.PEnd, v.TypInd)
		}
	case *nb09.SBpRel16:
		if v != nil {
			return v.Name, "", v.TypInd, fmt.Sprintf("bp%+d", v.Off)
		}
	case *nb09.SRegister:
		if v != nil {
			return v.Name, "", v.TypInd, fmt.Sprintf("reg=0x%04x", v.Reg)
		}
	case *nb09.SUdt:
		if v != nil {
			return v.Name, "", v.TypInd, fmt.Sprintf("typind=0x%04x", v.TypInd)
		}
	case *nb09.SPub16:
		if v != nil {
			return v.Name, fmt.Sprintf("%04x:%04x", v.Addr.Seg, v.Addr.Off), v.TypInd, fmt.Sprintf("typind=0x%04x", v.TypInd)
		}
	case *nb09.SPub32:
		if v != nil {
			return v.Name, fmt.Sprintf("%04x:%08x", v.Seg, v.Off), v.TypInd, fmt.Sprintf("typind=0x%04x", v.TypInd)
		}
	case *nb09.SData16:
		if v != nil {
			scope := "L"
			if v.IsGlobal {
				scope = "G"
			}
			return v.Name, fmt.Sprintf("%04x:%04x", v.Addr.Seg, v.Addr.Off), v.TypInd, scope
		}
	case *nb09.SProcRef:
		if v != nil {
			if target, ok := findAlignSymRecord(db, v.IMod, v.SymOff); ok {
				name, addr, typind, extra := describeSymRecord(db, target)
				return name, addr, typind, fmt.Sprintf("ref=%s symOff=0x%x cksum=0x%08x refTypind=0x%04x target=%s %s", db.ModuleName(v.IMod), v.SymOff, v.Checksum, v.TypInd, target.RecTyp, extra)
			}
			return "PROCREF", "", v.TypInd, fmt.Sprintf("unresolved ref=%s symOff=0x%x cksum=0x%08x typind=0x%04x", db.ModuleName(v.IMod), v.SymOff, v.Checksum, v.TypInd)
		}
	case *nb09.SDataRef:
		if v != nil {
			if target, ok := findAlignSymRecord(db, v.IMod, v.SymOff); ok {
				name, addr, typind, extra := describeSymRecord(db, target)
				return name, addr, typind, fmt.Sprintf("ref=%s symOff=0x%x cksum=0x%08x refTypind=0x%04x target=%s %s", db.ModuleName(v.IMod), v.SymOff, v.Checksum, v.TypInd, target.RecTyp, extra)
			}
			return "DATAREF", "", v.TypInd, fmt.Sprintf("unresolved ref=%s symOff=0x%x cksum=0x%08x typind=0x%04x", db.ModuleName(v.IMod), v.SymOff, v.Checksum, v.TypInd)
		}
	case *nb09.SBlock16:
		if v != nil {
			return v.Name, fmt.Sprintf("%04x:%04x", v.Addr.Seg, v.Addr.Off), 0, fmt.Sprintf("len=0x%x parent=0x%08x end=0x%08x", v.Len, v.PParent, v.PEnd)
		}
	case *nb09.SLabel16:
		if v != nil {
			return v.Name, fmt.Sprintf("%04x:%04x", v.Addr.Seg, v.Addr.Off), 0, fmt.Sprintf("flags=0x%02x", v.Flags)
		}
	case *nb09.SCompile:
		if v != nil {
			extra := fmt.Sprintf("flags=0x%04x", v.Flags)
			if v.Machine != 0 {
				extra += fmt.Sprintf(" machine=%04x", v.Machine)
			}
			return fmt.Sprintf("MS C %s", v.Version), "", 0, extra
		}
	case *nb09.SSSearch:
		if v != nil {
			return "SSEARCH",
				fmt.Sprintf("%04x:%04x", v.Addr.Seg, v.Addr.Off),
				0,
				fmt.Sprintf("symOff=0x%04x", v.SymOff)
		}

	default:
		// these are fine, we don't parse them
		if r.RecTyp != nb09.S_END && r.RecTyp != nb09.S_ENDARG && r.RecTyp != nb09.S_ALIGN {
			return "", "", 0, "unknown sym record"
		}
	}
	return "", "", 0, ""
}

func findAlignSymRecord(db *nb09.NB09DB, imod uint16, symOff uint32) (nb09.SymRecord, bool) {
	if db == nil {
		return nb09.SymRecord{}, false
	}
	stream, ok := db.AlignSyms[imod]
	if !ok {
		return nb09.SymRecord{}, false
	}
	for _, rec := range stream.Records {
		if rec.Offset == symOff {
			return rec, true
		}
	}
	return nb09.SymRecord{}, false
}

func newNB09DirectoriesCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "directories",
		Short: "list directories",
		Long:  `List directories in nb09.`,
		RunE: func(cmd *cobra.Command, args []string) error {

			type record struct {
				IMod uint   `header:"imod"`
				Name string `header:"Name"`
				Size uint   `header:"Size (bytes)"`
			}

			printer := tableprinter.New(os.Stdout)
			tableRecords := make([]record, 0, len(db.Dir.Entries))

			for _, r := range db.Dir.Entries {
				tableRecords = append(tableRecords, record{IMod: uint(r.IMod), Name: r.Subsection.String(), Size: uint(r.CB)})
			}

			printer.Print(tableRecords)
			return nil
		},
	}

	return cmd
}

func newNB09ModulesCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "modules",
		Short: "list modules",
		Long:  `List modules in nb09.`,
		RunE: func(cmd *cobra.Command, args []string) error {

			type record struct {
				IMod              uint   `header:"imod"`
				Name              string `header:"Name"`
				SegsContributedTo uint   `header:"Segments Contributed To"`
				Lib               uint   `header:"Library"`
			}

			printer := tableprinter.New(os.Stdout)
			tableRecords := make([]record, 0, len(db.Modules))

			for imod, r := range db.Modules {
				tableRecords = append(tableRecords, record{IMod: uint(imod), Name: r.Name, SegsContributedTo: uint(len(r.SegInfo)), Lib: uint(r.ILib)})
			}

			slices.SortFunc(tableRecords, func(a, b record) int { return int(a.IMod - b.IMod) })

			printer.Print(tableRecords)
			return nil
		},
	}

	return cmd
}

func newNB09SegmentsCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "segments",
		Short: "list segments",
		Long:  `List segments in nb09.`,
		RunE: func(cmd *cobra.Command, args []string) error {

			type record struct {
				Segment int    `header:"Seg"`
				Name    string `header:"Name"`
				Group   uint   `header:"Group"`
				Frame   uint   `header:"Frame"`
				Class   string `header:"Class"`
				Offset  string `header:"Offset"`
				Size    string `header:"Size(bytes)"`
				Flags   string `header:"Flags"`
			}

			printer := tableprinter.New(os.Stdout)
			tableRecords := make([]record, 0, len(db.SegMap.Segs))

			for i, r := range db.SegMap.Segs {
				tableRecords = append(tableRecords, record{
					Segment: i + 1,
					Group:   uint(r.Group),
					Frame:   uint(r.Frame),
					Name:    db.SegNames.At[uint32(r.ISegName)],
					Class:   db.SegNames.At[uint32(r.IClassName)],
					Size:    fmt.Sprintf("0x%0x", uint(r.CB)),
					Offset:  fmt.Sprintf("0x%0x", r.Off),
					Flags:   fmt.Sprintf("0x%0x", r.Flags)})
			}

			printer.Print(tableRecords)
			return nil
		},
	}

	return cmd
}

func newNB09LibrariesCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "libraries",
		Short: "list libraries",
		Long:  `List libraries in nb09.`,
		RunE: func(cmd *cobra.Command, args []string) error {

			type record struct {
				Name string `header:"Name"`
			}

			printer := tableprinter.New(os.Stdout)
			tableRecords := make([]record, 0, len(db.Libraries))

			for _, r := range db.Libraries {
				tableRecords = append(tableRecords, record{Name: r})
			}

			printer.Print(tableRecords)
			return nil
		},
	}

	return cmd
}

// -----------------------------
// AlignSyms
// -----------------------------

func newNB09AlignSymsCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "alignsyms",
		Short: "list per-module SST_ALIGN_SYM symbol records",
		Long:  "List per-module sstAlignSym symbol records (one row per record).",
		RunE: func(cmd *cobra.Command, args []string) error {

			type row struct {
				Module string `header:"Module"`
				Type   string `header:"RecTyp"`
				Name   string `header:"Name"`
				Addr   string `header:"Addr"`
				TypInd string `header:"TypInd"`
				Extra  string `header:"Extra"`
			}

			printer := tableprinter.New(os.Stdout)
			var rows []row

			for _, imod := range sortedUint16Keys(db.AlignSyms) {
				stream := db.AlignSyms[imod]
				modName := db.ModuleName(imod)

				for _, r := range stream.Records {
					name, addr, typind, extra := describeSymRecord(db, r)
					rows = append(rows, row{
						Module: modName,
						Type:   r.RecTyp.String(),
						Name:   name,
						Addr:   addr,
						TypInd: fmt.Sprintf("0x%04x", typind),
						Extra:  extra,
					})
				}
			}

			printer.Print(rows)
			return nil
		},
	}
	return cmd
}

// -----------------------------
// SrcModules
// -----------------------------

func newNB09SrcModulesCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "srcmodules",
		Short: "list SST_SRC_MODULE info",
		Long:  "List sstSrcModule info (files/segments/line counts).",
		RunE: func(cmd *cobra.Command, args []string) error {

			type row struct {
				IMod      uint16 `header:"IMod"`
				Module    string `header:"Module"`
				File      string `header:"File"`
				Seg       uint16 `header:"Seg"`
				Range     string `header:"Range"`
				LineCount int    `header:"Lines"`
			}

			printer := tableprinter.New(os.Stdout)
			var rows []row

			for _, imod := range sortedUint16Keys(db.SrcModules) {
				sm := db.SrcModules[imod]
				modName := db.ModuleName(imod)

				for _, f := range sm.Files {
					for _, s := range f.Segs {
						rows = append(rows, row{
							IMod:      imod,
							Module:    modName,
							File:      f.Name,
							Seg:       s.Seg,
							Range:     fmt.Sprintf("0x%04x...0x%04x", s.Start, s.End),
							LineCount: len(s.Lines),
						})
					}
				}
			}

			printer.Print(rows)
			return nil
		},
	}
	return cmd
}

// -----------------------------
// GlobalPubs / GlobalSyms / StaticSyms
// -----------------------------

func newNB09GlobalPubsCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "globalpubs",
		Short: "list globalpubs",
		Long:  "List records from the globalpubs section.",
		RunE: func(cmd *cobra.Command, args []string) error {

			type row struct {
				Type string `header:"RecTyp"`
				Name string `header:"Name"`
				Addr string `header:"Addr"`
			}

			printer := tableprinter.New(os.Stdout)
			rows := make([]row, 0, len(db.GlobalPub.Symbols.Records))

			for _, r := range db.GlobalPub.Symbols.Records {
				if r.RecTyp == nb09.S_ALIGN {
					continue
				}
				name, addr, _, _ := describeSymRecord(db, r)
				rows = append(rows, row{
					Type: r.RecTyp.String(),
					Name: name,
					Addr: addr,
				})
			}

			slices.SortFunc(rows, func(a, b row) int { return strings.Compare(a.Addr, b.Addr) })

			printer.Print(rows)
			return nil
		},
	}
	return cmd
}

func newNB09GlobalSymsCmd() *cobra.Command {
	return newNB09HashSymsCmd("globalsyms", "list SST_GLOBAL_SYM symbols", func(db *nb09.NB09DB) *nb09.HashSymSubsection {
		if db == nil {
			return nil
		}
		return &db.GlobalSym
	})
}

func newNB09StaticSymsCmd() *cobra.Command {
	return newNB09HashSymsCmd("staticsyms", "list SST_STATIC_SYM symbols", func(db *nb09.NB09DB) *nb09.HashSymSubsection {
		if db == nil {
			return nil
		}
		return &db.StaticSym
	})
}

func newNB09HashSymsCmd(use string, short string, pick func(*nb09.NB09DB) *nb09.HashSymSubsection) *cobra.Command {
	cmd := &cobra.Command{
		Use:   use,
		Short: short,
		Long:  "List symbols from the hashed global/static subsections (one row per record).",
		RunE: func(cmd *cobra.Command, args []string) error {
			hs := pick(db)
			if hs == nil {
				return fmt.Errorf("missing subsection")
			}

			type row struct {
				// RecOff string `header:"RecOff"`
				// Len    uint16 `header:"RecLen"`
				Type   string `header:"RecTyp"`
				Name   string `header:"Name"`
				Addr   string `header:"Addr"`
				TypInd uint16 `header:"TypInd"`
				Extra  string `header:"Extra"`
			}

			printer := tableprinter.New(os.Stdout)
			rows := make([]row, 0, len(hs.Symbols.Records))

			for _, r := range hs.Symbols.Records {
				name, addr, typind, extra := describeSymRecord(db, r)
				rows = append(rows, row{
					// RecOff: fmt.Sprintf("0x%x", r.Offset),
					// Len:    r.RecLen,
					Type:   r.RecTyp.String(),
					Name:   name,
					Addr:   addr,
					TypInd: typind,
					Extra:  extra,
				})
			}

			slices.SortFunc(rows, func(a, b row) int {
				return strings.Compare(a.Addr, b.Addr)
			})

			printer.Print(rows)
			return nil
		},
	}
	return cmd
}

// -----------------------------
// GlobalTypes
// -----------------------------

func newNB09GlobalTypesCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "globaltypes",
		Short: "list SST_GLOBAL_TYPES type records",
		Long:  "List global type records (one row per type record).",
		RunE: func(cmd *cobra.Command, args []string) error {

			type row struct {
				TypeInd string `header:"TypInd"`
				Leaf    string `header:"Leaf"`
				Detail  string `header:"Detail"`
			}

			printer := tableprinter.New(os.Stdout)
			rows := make([]row, 0, len(db.GlobalTypes.Records))

			base := db.GlobalTypes.BaseIndex
			for i, tr := range db.GlobalTypes.Records {
				typind := uint16(0)
				if base != 0 {
					typind = base + uint16(i)
				}

				rows = append(rows, row{
					TypeInd: func() string {
						if base == 0 {
							return ""
						}
						return fmt.Sprintf("0x%04x", typind)
					}(),
					Leaf:   tr.Leaf.String(),
					Detail: describeTypeRecord(tr),
				})
			}

			printer.Print(rows)
			return nil
		},
	}
	return cmd
}

func describeTypeRecord(tr nb09.TypeRecord) string {
	return describeParsedType(tr.Parsed)
}

func describeParsedType(parsed any) string {
	switch v := parsed.(type) {
	case *nb09.LFModifier:
		if v != nil {
			return fmt.Sprintf("utype=%s mod=0x%04x", formatTypeIndex(v.UType), v.Mod)
		}
	case *nb09.LFPointer:
		if v != nil {
			return fmt.Sprintf("utype=%s attr=0x%04x", formatTypeIndex(v.UType), v.Attr)
		}
	case *nb09.LFArray:
		if v != nil {
			detail := fmt.Sprintf("elem=%s index=%s length=%s", formatTypeIndex(v.ElemType), formatTypeIndex(v.IndexType), formatNumericLeaf(v.Length))
			if v.Name != "" {
				detail += fmt.Sprintf(" name=%q", v.Name)
			}
			return detail
		}
	case *nb09.LFStruct:
		if v != nil {
			detail := fmt.Sprintf("count=%d fieldList=%s property=0x%04x deriv=%s vshape=%s size=%s",
				v.Count,
				formatTypeIndex(v.FieldList),
				v.Property,
				formatTypeIndex(v.Deriv),
				formatTypeIndex(v.VShape),
				formatNumericLeaf(v.Size),
			)
			if v.Name != "" {
				detail += fmt.Sprintf(" name=%q", v.Name)
			}
			return detail
		}
	case *nb09.LFProcedure:
		if v != nil {
			return fmt.Sprintf("ret=%s call=0x%02x reserved=0x%02x params=%d argList=%s",
				formatTypeIndex(v.RetType),
				v.CallType,
				v.Reserved,
				v.ParamCount,
				formatTypeIndex(v.ArgList),
			)
		}
	case *nb09.LFArgList:
		if v != nil {
			return fmt.Sprintf("count=%d args=[%s]", v.Count, formatTypeIndexList(v.Args))
		}
	case *nb09.LFFieldList:
		if v != nil {
			return fmt.Sprintf("fields=[%s]", describeFieldList(v))
		}
	case *nb09.LFBitfield:
		if v != nil {
			return fmt.Sprintf("base=%s length=%d position=%d", formatTypeIndex(v.BaseType), v.Length, v.Position)
		}
	}
	return ""
}

func describeFieldList(fl *nb09.LFFieldList) string {
	parts := make([]string, 0, len(fl.Fields))
	for _, field := range fl.Fields {
		switch v := field.Parsed.(type) {
		case *nb09.LFMember:
			if v != nil {
				part := fmt.Sprintf("member type=%s attr=0x%04x offset=%s", formatTypeIndex(v.Type), v.Attr, formatNumericLeaf(v.Offset))
				if v.Name != "" {
					part += fmt.Sprintf(" name=%q", v.Name)
				}
				parts = append(parts, part)
				continue
			}
		}
		parts = append(parts, field.Leaf.String())
	}
	return strings.Join(parts, ", ")
}

func formatTypeIndex(typind uint16) string {
	return fmt.Sprintf("0x%04x", typind)
}

func formatTypeIndexList(args []uint16) string {
	parts := make([]string, 0, len(args))
	for _, arg := range args {
		parts = append(parts, formatTypeIndex(arg))
	}
	return strings.Join(parts, ", ")
}

func formatNumericLeaf(n nb09.NumericLeaf) string {
	if n.Str != "" {
		return fmt.Sprintf("%q", n.Str)
	}
	if n.IsLeaf {
		return fmt.Sprintf("leaf=0x%04x", n.Leaf)
	}
	return fmt.Sprintf("%d", n.Value)
}

// -----------------------------
// FileIndices
// -----------------------------

func newNB09FileIndicesCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "fileindices",
		Short: "list SST_FILE_INDEX file mappings",
		Long:  "List the module->file list mapping from sstFileIndex.",
		RunE: func(cmd *cobra.Command, args []string) error {

			type row struct {
				IMod      uint16 `header:"IMod"`
				Module    string `header:"Module"`
				FileCount int    `header:"Files"`
				Files     string `header:"FileList"`
			}

			printer := tableprinter.New(os.Stdout)
			var rows []row

			// Prefer ModToFiles if your parser populates it; otherwise fall back to Names/NameRef.
			if db.FileIndex.ModToFiles != nil {
				imods := make([]uint16, 0, len(db.FileIndex.ModToFiles))
				for imod := range db.FileIndex.ModToFiles {
					imods = append(imods, imod)
				}
				slices.Sort(imods)

				for _, imod := range imods {
					files := db.FileIndex.ModToFiles[imod]
					rows = append(rows, row{
						IMod:      imod,
						Module:    db.ModuleName(imod),
						FileCount: len(files),
						Files:     strings.Join(files, ", "),
					})
				}
			} else {
				// Minimal fallback: just dump the expanded Names list if present.
				rows = append(rows, row{
					IMod:      0,
					Module:    "(no ModToFiles mapping)",
					FileCount: len(db.FileIndex.Names),
					Files:     strings.Join(db.FileIndex.Names, ", "),
				})
			}

			printer.Print(rows)
			return nil
		},
	}
	return cmd
}

func init() {
	rootCmd.AddCommand(newNB09Command())
}
