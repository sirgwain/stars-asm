package cmd

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/sirgwain/stars-asm/dasm/starsenv"
	"github.com/spf13/cobra"
)

func buildDasmDumpOptions(fromAddr, toAddr uint32) stars.DumpOptions {
	return stars.DumpOptions{
		FromAddr:  fromAddr,
		ToAddr:    toAddr,
		ShowColor: showColor,
	}
}

var funcName string
var fromAddr uint32
var toAddr uint32

func addDasmFuncFlags(cmd *cobra.Command, fromHex, toHex *string) {
	cmd.Flags().StringVarP(&funcName, "name", "n", "", "the function to disassemble")
	cmd.Flags().StringVarP(&funcName, "func", "f", "", "the function to disassemble")
	cmd.Flags().StringVarP(&funcName, "proc", "p", "", "the function to disassemble")
	cmd.MarkFlagsMutuallyExclusive("func", "proc", "name")

	cmd.MarkFlagsOneRequired("func", "proc", "name")
	cmd.Flags().StringVar(fromHex, "from", "", "start address in hex (e.g. b0a2); with --asm/--sem, restricts output to this block")
	cmd.Flags().StringVar(toHex, "to", "", "end label/address in hex; labels include the final block, non-label addresses are exclusive")
}

func newDasmCmd() *cobra.Command {
	var fromHex string
	var toHex string
	cmd := &cobra.Command{
		Use:   "dasm",
		Short: "Disassemble",
		Long:  `Root command for disassembling functions`,
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

			// allow L_ style labels
			fromHex = strings.ReplaceAll(fromHex, "L_", "")
			toHex = strings.ReplaceAll(toHex, "L_", "")

			if fromHex != "" {
				v, err := strconv.ParseUint(fromHex, 16, 32)
				if err != nil {
					return fmt.Errorf("--from: invalid hex address %q: %v", fromHex, err)
				}
				fromAddr = uint32(v)
			}
			if toHex != "" {
				v, err := strconv.ParseUint(toHex, 16, 32)
				if err != nil {
					return fmt.Errorf("--to: invalid hex address %q: %v", toHex, err)
				}
				toAddr = uint32(v)
			}

			return nil
		},
	}

	asmCmd := newDasmAsmCmd()
	cfgCmd := newDasmCfgCmd()
	effectsCmd := newDasmEffectsCmd()
	semCmd := newDasmSemCmd()
	irCmd := newDasmIRCmd()
	graphCmd := newDasmGraphCmd()
	addDasmFuncFlags(asmCmd, &fromHex, &toHex)
	addDasmFuncFlags(cfgCmd, &fromHex, &toHex)
	addDasmFuncFlags(effectsCmd, &fromHex, &toHex)
	addDasmFuncFlags(semCmd, &fromHex, &toHex)
	addDasmFuncFlags(irCmd, &fromHex, &toHex)
	addDasmFuncFlags(graphCmd, &fromHex, &toHex)

	cmd.AddCommand(asmCmd)
	cmd.AddCommand(cfgCmd)
	cmd.AddCommand(effectsCmd)
	cmd.AddCommand(semCmd)
	cmd.AddCommand(irCmd)
	cmd.AddCommand(graphCmd)
	cmd.AddCommand(newDasmAllCmd())
	cmd.AddCommand(newDasmStructCmd())
	return cmd
}

func init() {
	rootCmd.AddCommand(newDasmCmd())
}
