package cmd

import (
	_ "embed"
	"fmt"
	"log/slog"
	"os"
	"strconv"
	"time"

	"github.com/phsym/console-slog"
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/starsenv"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
	"github.com/spf13/cobra"
)

var inputDir string
var debugEnabled = false
var showColor = false
var starsEnv *starsenv.Env

// loaded by preruns
var sdb *typeinfo.SymbolDB
var img *asm.ImageNE

// prerun method for enabling slog logging
func logPreRun(cmd *cobra.Command, args []string) error {
	// pick level based on your flag/env/config
	level := slog.LevelInfo
	if debugEnabled { // e.g. from a flag
		level = slog.LevelDebug
	}

	// base console handler (pretty printing)
	consoleHandler := console.NewHandler(os.Stderr, &console.HandlerOptions{
		Level:      level,
		NoColor:    !showColor,
		TimeFormat: " ",
	})

	var handler slog.Handler = consoleHandler

	// build the logger and set global
	logger := slog.New(handler)
	slog.SetDefault(logger)

	// emit startup logs
	slog.Debug("Debug logging enabled")

	return nil
}

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "stars-asm",
	Short: "A Stars! Disassembler and Annotator",
	Long: `stars-asm will read nb09 debug symbols from the stars.exe and disassemble 
functions, annotating them with useful information
`,
	PersistentPreRunE: logPreRun,
	Run: func(cmd *cobra.Command, args []string) {
		// Show usage
		cmd.Help()
		os.Exit(1)
	},
}

// Execute adds all child commands to the root command and sets flags appropriately.
// This is called by main.main(). It only needs to happen once to the rootCmd.
func Execute() {
	start := time.Now()
	defer func() {
		fmt.Println()
		slog.Debug(fmt.Sprintf("Duration: %v", time.Since(start)))
	}()

	err := rootCmd.Execute()
	if err != nil {
		os.Exit(1)
	}
}

func init() {

	if value, ok := os.LookupEnv("STARS_ASM_SHOW_COLOR"); ok {
		if parsed, err := strconv.ParseBool(value); err == nil {
			showColor = parsed
		}
	}

	// all commands have debug mode
	rootCmd.PersistentFlags().BoolVar(&debugEnabled, "debug", false, "enable debug logging")
	rootCmd.PersistentFlags().BoolVar(&showColor, "show-color", showColor, "enable colored syntax output")
	rootCmd.PersistentFlags().StringVarP(&inputDir, "input-path", "", "./dasm/input", "path to the input folder")
}
