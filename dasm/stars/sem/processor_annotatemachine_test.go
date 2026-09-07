package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestAnnotateMachineStorageRespectsLocalScopes verifies reused stack slots are
// rendered with the local that is in scope at the access instruction.
func TestAnnotateMachineStorageRespectsLocalScopes(t *testing.T) {
	fx := testfixture.Stars(t)
	fn := fx.SDB.GetFunction("FBuildObject")
	if fn == nil {
		t.Fatal("FBuildObject not found")
	}
	ctx := NewFuncContext(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB), fn)
	processor := &annotateProcessor{ctx: ctx}
	result := newResult(fn)

	access := func(instOff uint32, role machine.OperandRole) machine.MemoryAddress {
		return machine.MemoryAddress{
			Seg:    machine.RegVal(asm.RegSS),
			Base:   machine.FrameBaseVal(),
			Disp:   -0x18,
			Width:  2,
			Origin: machine.Origin{InstOff: instOff, Role: role},
		}
	}

	dpOrig := access(0x1d3a, machine.OperandDst)
	lpthMac := access(0x2681, machine.OperandDst)
	if !processor.annotateMemory(result, dpOrig) {
		t.Fatal("dpOrig access was not annotated")
	}
	if !processor.annotateMemory(result, lpthMac) {
		t.Fatal("lpthMac access was not annotated")
	}

	tests := []struct {
		name string
		mem  machine.MemoryAddress
		want string
	}{
		{name: "dpOrig exact origin", mem: dpOrig, want: "[bp-dpOrig]"},
		{name: "lpthMac exact origin", mem: lpthMac, want: "[bp-lpthMac]"},
		{name: "dpOrig scope fallback", mem: access(0x1d3b, machine.OperandSrc), want: "[bp-dpOrig]"},
		{name: "lpthMac scope fallback", mem: access(0x2682, machine.OperandSrc), want: "[bp-lpthMac]"},
		{name: "outside both scopes", mem: access(0x2452, machine.OperandDst), want: "ss:[bp-0x18]"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := result.MemoryAddress(tt.mem); got != tt.want {
				t.Fatalf("MemoryAccess() = %q, want %q", got, tt.want)
			}
		})
	}
}
