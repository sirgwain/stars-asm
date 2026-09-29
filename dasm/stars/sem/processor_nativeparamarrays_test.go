package sem

import (
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestNativeParamArraysCopiesWalkedParams verifies PackageUpMsg's walk over
// p1..p7 reads a local copy of the parameters, and that a pointer set from a
// parameter address but never advanced is left alone.
func TestNativeParamArraysCopiesWalkedParams(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "PackageUpMsg")
	pi := localNamed(t, ctx.fs, "pi")
	var p1 *Local
	for _, v := range ctx.fs.Params {
		if v.Name == "p1" {
			p1 = &Local{FunctionVar: v}
		}
	}
	if p1 == nil {
		t.Fatal("parameter p1 not found")
	}
	start := &Assign{Dst: pi, Src: &AddressOf{Target: p1, TypeInfo: pi.Type}}
	advance := &Assign{Dst: pi, Src: &AddressOf{Target: &ArrayIndex{Base: pi, Index: &Const{U64: 1}, TypeInfo: p1.Type}, TypeInfo: pi.Type}}

	f := &Func{Blocks: []Block{{ID: 0x80bd, Effects: []Effect{start}}, {ID: 0x8151, Effects: []Effect{advance}}}}
	if !(&nativeParamArraysProcessor{ctx: ctx}).ProcessFunc(nil, f) {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	var got []string
	for _, effect := range f.Blocks[0].Effects {
		got = append(got, FormatEffect(effect))
	}
	want := "rgArgs[0] = p1; rgArgs[1] = p2; rgArgs[2] = p3; rgArgs[3] = p4; rgArgs[4] = p5; rgArgs[5] = p6; rgArgs[6] = p7; pi = &rgArgs[0]"
	if strings.Join(got, "; ") != want {
		t.Fatalf("effects = %q, want %q", strings.Join(got, "; "), want)
	}
	if len(f.RecoveredLocals) != 1 || f.RecoveredLocals[0].Type.String() != "int16_t[7]" {
		t.Fatalf("recovered locals = %v, want one int16_t[7]", f.RecoveredLocals)
	}

	unwalked := &Func{Blocks: []Block{{ID: 0x80bd, Effects: []Effect{start}}}}
	if (&nativeParamArraysProcessor{ctx: ctx}).ProcessFunc(nil, unwalked) {
		t.Fatal("ProcessFunc rewrote a parameter address that is never advanced")
	}
}
