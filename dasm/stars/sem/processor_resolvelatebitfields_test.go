package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestResolveLateBitfields verifies scratch-substituted wide storage is
// projected back to its declared PLANET bitfields.
func TestResolveLateBitfields(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "DoBombing")
	ctx.SetCurrentBlock(0xb14e)

	var lpplVar *typeinfo.FunctionVar
	for i := range ctx.fs.Vars {
		if ctx.fs.Vars[i].Name == "lppl" {
			lpplVar = &ctx.fs.Vars[i]
			break
		}
	}
	if lpplVar == nil {
		t.Fatal("DoBombing lppl local not found")
	}
	storage := &Deref{
		Pointer:  &Local{FunctionVar: *lpplVar},
		ByteOff:  0x14,
		Width:    4,
		TypeInfo: typeinfo.U32,
	}
	processor := resolveLateBitfieldsProcessor{ctx: ctx}

	for _, tc := range []struct {
		name  string
		shift uint64
		field string
	}{
		{name: "mines", shift: 8, field: "lppl->cMines"},
		{name: "factories", shift: 20, field: "lppl->cFactories"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			shifted := &Binary{
				TypeInfo: typeinfo.U32,
				Op:       OpShr,
				LHS:      storage,
				RHS:      &Const{TypeInfo: typeinfo.U16, U64: tc.shift},
			}
			expr := &Cast{
				Value: &Binary{
					TypeInfo: typeinfo.U16,
					Op:       OpAnd,
					LHS: &Word{
						Parent: &Cast{Value: shifted, To: typeinfo.U32.String(), TypeInfo: typeinfo.U32},
						Part:   machine.WordLow,
					},
					RHS: &Const{TypeInfo: typeinfo.U16, U64: 0xfff},
				},
				To:       typeinfo.U32.String(),
				TypeInfo: typeinfo.U32,
			}

			got, ok := processor.resolve(expr)
			if !ok {
				t.Fatal("resolve() did not recognize declared bitfield")
			}
			if formatted := FormatExpr(got); formatted != tc.field {
				t.Fatalf("resolve() = %q, want %q", formatted, tc.field)
			}
		})
	}
}

// TestResolveLateBitfieldMaskBeforeShift verifies scratch substitution can
// expose the compiler's shifted-mask extraction order.
func TestResolveLateBitfieldMaskBeforeShift(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "DoBombing")
	ctx.SetCurrentBlock(0xb14e)

	var lpplVar *typeinfo.FunctionVar
	for i := range ctx.fs.Vars {
		if ctx.fs.Vars[i].Name == "lppl" {
			lpplVar = &ctx.fs.Vars[i]
			break
		}
	}
	if lpplVar == nil {
		t.Fatal("DoBombing lppl local not found")
	}
	storage := &Deref{
		Pointer:  &Local{FunctionVar: *lpplVar},
		ByteOff:  0x14,
		Width:    4,
		TypeInfo: typeinfo.U32,
	}
	expr := &Binary{
		TypeInfo: typeinfo.U32,
		Op:       OpShr,
		LHS: &Binary{
			TypeInfo: typeinfo.U32,
			Op:       OpAnd,
			LHS:      storage,
			RHS:      &Const{TypeInfo: typeinfo.U32, U64: 0xfff00},
		},
		RHS: &Const{TypeInfo: typeinfo.U16, U64: 8},
	}

	got, ok := (&resolveLateBitfieldsProcessor{ctx: ctx}).resolve(expr)
	if !ok {
		t.Fatal("resolve() did not recognize mask-before-shift bitfield")
	}
	if formatted := FormatExpr(got); formatted != "lppl->cMines" {
		t.Fatalf("resolve() = %q, want lppl->cMines", formatted)
	}
}
