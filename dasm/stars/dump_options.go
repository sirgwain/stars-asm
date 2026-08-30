package stars

import startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"

type DumpOptions = startemplates.DumpOptions
type DumpStyles = startemplates.DumpStyles
type DumpAsmOptions = startemplates.DumpAsmOptions
type DumpCFGOptions = startemplates.DumpCFGOptions
type DumpEffectsOptions = startemplates.DumpEffectsOptions
type DumpSemOptions = startemplates.DumpSemOptions
type FuncDetailOptions = startemplates.FuncDetailOptions
type FuncDetailCommentStyle = startemplates.FuncDetailCommentStyle

const (
	FuncDetailCommentC   = startemplates.FuncDetailCommentC
	FuncDetailCommentAsm = startemplates.FuncDetailCommentAsm
)

type DumpAllOptions struct {
	OutDir      string
	EmitC       bool
	EmitIR      bool
	EmitSem     bool
	EmitEffects bool
	EmitASM     bool
	EmitStructs bool
	Verbose     bool
}
