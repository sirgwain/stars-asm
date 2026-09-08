package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestSymbolResolverScratchAccess verifies unresolved SS:BP storage resolves
// to a synthetic scratch symbol while real locals retain precedence.
func TestSymbolResolverScratchAccess(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "AddMinesToBlockedQueues")
	sr := newSymbolResolver(ctx)
	alertCtx := mustFuncContext(t, fx, res, "AlertSz")
	alertSR := newSymbolResolver(alertCtx)

	base := machine.FrameBaseVal()
	ss := machine.RegVal(asm.RegSS)
	access := func(disp, width int) machine.MemoryAddress {
		return machine.MemoryAddress{
			Seg:   ss,
			Base:  base,
			Disp:  disp,
			Width: width,
			Origin: machine.Origin{
				InstOff: ctx.fs.Addr.Off,
			},
		}
	}

	path, ok := sr.memoryPath(access(-0x136, 2))
	if !ok {
		t.Fatal("scratch access did not resolve")
	}
	if got := path.String(); got != "scratch_bp_m136" {
		t.Fatalf("scratch path = %q, want scratch_bp_m136", got)
	}
	if path.Type() != typeinfo.U16 {
		t.Fatalf("scratch type = %v, want uint16_t", path.Type())
	}

	typedAddress, ok := sr.addressFromMemory(access(-0x136, 4), typeinfo.I32)
	if !ok {
		t.Fatal("typed scratch access did not resolve")
	}
	typed, ok := typedAddress.path()
	if !ok {
		t.Fatal("typed scratch address did not produce a path")
	}
	if typed.Type() != typeinfo.I32 {
		t.Fatalf("typed scratch type = %v, want int32_t", typed.Type())
	}

	segmentless, ok := sr.memoryPath(machine.MemoryAddress{
		Base:  base,
		Disp:  -0x136,
		Width: 2,
	})
	if !ok || segmentless.String() != "scratch_bp_m136" {
		t.Fatalf("segmentless scratch = %v, %v", segmentless, ok)
	}

	local, ok := sr.memoryPath(access(-0xa, 2))
	if !ok {
		t.Fatal("known local access did not resolve")
	}
	if got := local.String(); got != "cMaxBuild" {
		t.Fatalf("known local = %q, want cMaxBuild", got)
	}

	typedParam := machine.MemoryAddress{
		Seg:   ss,
		Base:  base,
		Disp:  0x8,
		Width: 2,
		Origin: machine.Origin{
			InstOff: 0x220f,
		},
	}
	paramAddress, ok := alertSR.addressFromMemory(typedParam, fx.SDB.GetFunction("AlertSz").Params[1].Type)
	if !ok {
		t.Fatal("typed parameter access did not resolve")
	}
	param, ok := paramAddress.path()
	if !ok {
		t.Fatal("typed parameter address did not produce a path")
	}
	if got := param.String(); got != "mbType" {
		t.Fatalf("typed parameter = %q, want mbType", got)
	}

	nonFrame := access(-0x136, 2)
	nonFrame.Seg = machine.RegVal(asm.RegDS)
	if _, ok := sr.symbolFromScratchMemoryAddress(nonFrame, nil); ok {
		t.Fatal("DS access was classified as scratch")
	}

	indexed := access(-0x136, 2)
	indexed.Index = machine.ConstVal(1)
	if _, ok := sr.symbolFromScratchMemoryAddress(indexed, nil); ok {
		t.Fatal("indexed frame access was classified as scratch")
	}
}
