package machine

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// InstCall records the resolved call target for a decoded CALLF instruction.
type InstCall struct {
	Target *typeinfo.Function
}

// InstJump records the resolved target metadata for a decoded jump instruction.
type InstJump struct {
	TargetOff       uint32
	Label           string
	TableTargetOffs []uint32
}

// instMnemonic returns the display mnemonic for a decoded instruction.
func instMnemonic(inst asm.DecodedInst) string {
	if inst.HasRepeatPrefix() {
		return inst.Mnemonic + ".REP"
	}
	return inst.Mnemonic
}
