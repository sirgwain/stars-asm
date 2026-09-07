package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// TestMachineRewriterPreservesCarryBinary verifies child rewrites cannot fold
// away the explicit ADC/SBB node required for later wide reconstruction.
func TestMachineRewriterPreservesCarryBinary(t *testing.T) {
	producer := machine.Meta{InstOff: 0x1234, InstOp: asm.OpADC, InstLen: 2}
	value := &machine.Binary{
		Op:       machine.ValueOpAdd,
		LHS:      machine.UnknownVal("rewrite me"),
		RHS:      machine.ConstVal(0),
		Producer: producer,
	}
	rewriter := &machineRewriter{
		value: func(_ *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
			if _, ok := value.(*machine.Unknown); ok {
				return machine.ConstVal(0), true, true
			}
			return value, false, false
		},
	}

	got, changed := rewriter.rewriteMachineValue(value)
	if !changed {
		t.Fatal("rewriteMachineValue() changed = false, want true")
	}
	binary, ok := got.(*machine.Binary)
	if !ok {
		t.Fatalf("rewriteMachineValue() = %T, want *machine.Binary", got)
	}
	if binary.Producer != producer {
		t.Fatalf("producer = %+v, want %+v", binary.Producer, producer)
	}
}
