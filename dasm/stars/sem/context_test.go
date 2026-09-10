package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestFunctionFactDependentEnumAtEntry verifies configured discriminator facts
// select dependent enums in the entry block, before any predecessor propagates them.
func TestFunctionFactDependentEnumAtEntry(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "FGetBestDefensePart")
	root, ok := ctx.symbolRootByName("part")
	if !ok {
		t.Fatal("part local not found")
	}
	path, ok := appendSymbolFieldPath(root, []string{"hs", "iItem"})
	if !ok {
		t.Fatal("part.hs.iItem path not found")
	}
	fn := Func{
		CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{retForReturnSinkTest(0x21f6)}),
		Blocks: []Block{{ID: 0x21f6, Effects: []Effect{
			&Assign{Dst: &SymbolRef{Path: path}, Src: testConst(9)},
			&Return{Value: testConst(1)},
		}}},
	}
	(&unionContextProcessor{ctx: ctx}).ProcessFunc(newResult(ctx.fs), &fn)
	ctx.SetCurrentBlock(0x21f6)
	resolved, _ := (&resolveEnumsProcessor{ctx: ctx}).ProcessBlock(newResult(ctx.fs), fn, fn.Blocks[0])
	if got := FormatEffect(resolved.Effects[0]); got != "part.hs.iItem = iplanetarySDI" {
		t.Fatalf("entry assignment = %s", got)
	}
}
