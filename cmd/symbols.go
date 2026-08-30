package cmd

import (
	"fmt"
	"io"
	"os"
	"slices"
	"strings"

	"github.com/lensesio/tableprinter"
	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/sirgwain/stars-asm/dasm/starsenv"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
	"github.com/spf13/cobra"
)

func newSymbolsCmd() *cobra.Command {
	cmd := &cobra.Command{
		Use:   "symbols",
		Short: "Symbol database stats and queries",
		Long:  `Load the NB09 symbol database and print stats or query symbols.`,
		PersistentPreRunE: func(cmd *cobra.Command, args []string) error {
			if err := logPreRun(cmd, args); err != nil {
				return err
			}
			env, err := starsenv.LoadStars(starsenv.Options{InputDir: inputDir})
			if err != nil {
				return err
			}
			img = env.Image
			sdb = env.SDB
			starsEnv = env

			return nil
		},
		RunE: func(cmd *cobra.Command, args []string) error {

			fmt.Printf("Symbol Database Stats\n")
			fmt.Printf("  Functions:  %d\n", len(sdb.Functions))
			fmt.Printf("  Globals:    %d\n", len(sdb.Globals))
			fmt.Printf("  Structs:    %d\n", len(sdb.Structs))
			fmt.Printf("  Publics:    %d\n", len(sdb.Publics))

			return cmd.Usage()
		},
	}

	cmd.AddCommand(newSymbolsPublicsCmd())
	cmd.AddCommand(newSymbolsGlobalsCmd())
	cmd.AddCommand(newSymbolsFuncsCmd())
	cmd.AddCommand(newSymbolsStructsCmd())
	cmd.AddCommand(newSymbolsEnumsCmd())
	return cmd
}

func newSymbolsGlobalsCmd() *cobra.Command {
	var global string

	cmd := &cobra.Command{
		Use:   "globals",
		Short: "List or inspect globals",
		Long:  `List all globals in the symbol database, or show details for a specific global.`,
		RunE: func(cmd *cobra.Command, args []string) error {

			if global != "" {
				g := sdb.GetGlobal(global)
				if g == nil {
					return fmt.Errorf("global %s not found", global)
				}

				if err := stars.DumpGlobalDetail(os.Stdout, img, sdb, g); err != nil {
					return err
				}

				return nil
			}
			type row struct {
				Addr   string `header:"Address"`
				Name   string `header:"Name"`
				Type   string `header:"Type"`
				Module string `header:"Module"`
			}

			globals := sdb.Globals
			rows := make([]row, 0, len(globals))
			for _, g := range globals {
				rows = append(rows, row{
					Name:   g.Name,
					Addr:   g.Addr.String(),
					Module: g.Module,
					Type:   g.Type.String(),
				})
			}

			printer := tableprinter.New(os.Stdout)
			printer.Print(rows)
			fmt.Printf("\n%d globals\n", len(globals))
			return nil
		},
	}

	cmd.Flags().StringVarP(&global, "name", "n", "", "show details for a specific global")
	return cmd
}

func newSymbolsFuncsCmd() *cobra.Command {
	var function string
	var includeOverrides bool

	cmd := &cobra.Command{
		Use:   "functions",
		Short: "List or inspect functions",
		Long:  `List all functions in the symbol database, or show details for a specific function.`,
		RunE: func(cmd *cobra.Command, args []string) error {

			if function != "" {
				f := sdb.GetFunction(function)
				if f == nil {
					return fmt.Errorf("function %s not found", function)
				}

				if err := stars.DumpFuncDetail(os.Stdout, img, f, stars.FuncDetailOptions{}); err != nil {
					return err
				}

				return nil
			}
			type row struct {
				Addr   string `header:"Address"`
				Name   string `header:"Name"`
				Type   string `header:"Type"`
				Len    uint   `header:"Len"`
				Blocks int    `header:"Blocks"`
				Labels int    `header:"Labels"`
				Module string `header:"Module"`
			}

			funcs := sdb.Functions
			rows := make([]row, 0, len(funcs))
			for _, f := range funcs {
				if f.IsOverride() && !includeOverrides {
					continue
				}
				rows = append(rows, row{
					Addr:   f.Addr.String(),
					Name:   f.Name,
					Len:    uint(f.Len),
					Type:   f.String(),
					Blocks: len(f.Scopes),
					Labels: len(f.Labels),
					Module: f.Module,
				})
			}

			printer := tableprinter.New(os.Stdout)
			printer.Print(rows)
			fmt.Printf("\n%d procedures\n", len(funcs))
			return nil
		},
	}

	cmd.Flags().StringVarP(&function, "name", "n", "", "show details for a specific function")
	cmd.Flags().BoolVar(&includeOverrides, "include-overrides", false, "include override functions")
	return cmd
}

func newSymbolsStructsCmd() *cobra.Command {
	var structName string
	var includeWindows bool

	cmd := &cobra.Command{
		Use:   "structs",
		Short: "List or inspect structs",
		Long:  `List all structs in the symbol database, or show details for a specific struct.`,
		RunE: func(cmd *cobra.Command, args []string) error {

			if structName != "" {
				s := sdb.GetStruct(structName)
				if s == nil {
					return fmt.Errorf("struct %s not found", structName)
				}

				return dumpStructDetail(os.Stdout, s)
			}
			type row struct {
				Name    string `header:"Name"`
				Size    string `header:"Size"`
				Windows string `header:"Windows"`
			}

			structs := sdb.Structs
			rows := make([]row, 0, len(structs))
			for _, s := range structs {
				if !includeWindows && s.IsExternalWindowsStruct() {
					continue
				}
				windows := ""
				if s.IsExternalWindowsStruct() {
					windows = "✅"
				}
				rows = append(rows, row{
					Name:    s.String(),
					Size:    fmt.Sprintf("0x%04x", s.Size),
					Windows: windows,
				})
			}

			printer := tableprinter.New(os.Stdout)
			printer.Print(rows)
			fmt.Printf("\n%d structs\n", len(structs))
			return nil
		},
	}

	cmd.Flags().StringVarP(&structName, "name", "n", "", "show details for a specific struct")
	cmd.Flags().BoolVar(&includeWindows, "include-windows", false, "include windows structs in output")
	return cmd
}

func dumpStructDetail(w io.Writer, s *typeinfo.Struct) error {
	if s == nil {
		return nil
	}
	if len(s.Chunks) == 0 && len(s.Fields) > 0 {
		s.FinalizeLayout()
	}

	fmt.Fprintf(w, "%s\n", s.String())
	fmt.Fprintf(w, "  Size:   %d bytes\n", s.Size)
	fmt.Fprintf(w, "  Fields: %d\n\n", len(s.Fields))

	fmt.Fprintf(w, "  %-14s %6s   %-12s %s\n", fmt.Sprintf("OFFSET (%d)", len(s.Fields)), "SIZE", "TYPE", "NAME")
	fmt.Fprintln(w, " -------------- ------ ------------ -----------------")
	for _, field := range s.Fields {
		fmt.Fprintf(w, "  %-14s %6d   %-12s %s\n",
			fmt.Sprintf("0x%04X", field.Offset),
			field.Size,
			field.Type.String(),
			field.String(),
		)
	}
	return nil
}

func newSymbolsPublicsCmd() *cobra.Command {

	cmd := &cobra.Command{
		Use:   "publics",
		Short: "List or inspect publics",
		Long:  `List all publics in the symbol database`,
		RunE: func(cmd *cobra.Command, args []string) error {

			type row struct {
				Addr string `header:"Address"`
				Name string `header:"Name"`
			}

			publics := sdb.Publics
			rows := make([]row, 0, len(publics))
			for _, g := range publics {
				rows = append(rows, row{
					Name: g.Name,
					Addr: g.Addr.String(),
				})
			}

			printer := tableprinter.New(os.Stdout)
			printer.Print(rows)
			fmt.Printf("\n%d publics\n", len(publics))
			return nil
		},
	}

	return cmd
}

func newSymbolsEnumsCmd() *cobra.Command {
	var enum string

	cmd := &cobra.Command{
		Use:   "enums",
		Short: "List or inspect enums",
		Long:  `List all enums in the symbol database, or show details for a specific enum.`,
		RunE: func(cmd *cobra.Command, args []string) error {

			if enum != "" {
				e := sdb.GetEnum(enum)
				if e == nil {
					return fmt.Errorf("enum %s not found", enum)
				}

				if err := stars.DumpEnumDetail(os.Stdout, sdb, e); err != nil {
					return err
				}

				return nil
			}
			type row struct {
				Name   string `header:"Name"`
				Values int    `header:"Values"`
			}

			enums := sdb.Enums
			rows := make([]row, 0, len(enums))
			for _, g := range enums {
				rows = append(rows, row{
					Name:   g.Name,
					Values: len(g.Values),
				})
			}
			slices.SortFunc(rows, func(a, b row) int { return strings.Compare(a.Name, b.Name) })

			printer := tableprinter.New(os.Stdout)
			printer.Print(rows)
			fmt.Printf("\n%d enums\n", len(enums))
			return nil
		},
	}

	cmd.Flags().StringVarP(&enum, "name", "n", "", "show details for a specific enum")
	return cmd
}

func init() {
	rootCmd.AddCommand(newSymbolsCmd())
}
