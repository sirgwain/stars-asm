package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestNativeCastsFloatPrecision verifies exact promotions disappear while x87
// arithmetic and intervening stores retain their required rounding boundaries.
func TestNativeCastsFloatPrecision(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "CalcPctSurvive")
	p := &nativeCastsProcessor{ctx: ctx}
	float := &typeinfo.Primitive{TypeKind: typeinfo.KFloat, Name: "float", Size: 4}
	size := &Local{FunctionVar: typeinfo.FunctionVar{Name: "iSize", Type: typeinfo.I16}}
	warp := &Local{FunctionVar: typeinfo.FunctionVar{Name: "iWarp", Type: typeinfo.I16}}
	count := &Local{FunctionVar: typeinfo.FunctionVar{Name: "cDefenses", Type: typeinfo.I16}}
	mineral := &Local{FunctionVar: typeinfo.FunctionVar{Name: "lMineral", Type: typeinfo.I32}}
	product := &Binary{
		Op: OpMul, TypeInfo: typeinfo.F80,
		LHS: castTo(size, typeinfo.F80),
		RHS: castTo(&FloatConst{TypeInfo: typeinfo.Double, F64: 0.3}, typeinfo.F80),
	}
	pow := &Call{
		Function: &typeinfo.Function{Name: "pow", Ret: typeinfo.Double, Params: []typeinfo.FunctionVar{
			{Name: "base", Type: typeinfo.Double}, {Name: "exponent", Type: typeinfo.Double},
		}},
		Args: []Expr{castTo(product, typeinfo.Double), castTo(castTo(count, typeinfo.F80), typeinfo.Double)},
	}
	for _, tc := range []struct {
		name string
		expr Expr
		want string
	}{
		{"constant", castTo(castTo(&FloatConst{TypeInfo: typeinfo.Double, F64: 1}, typeinfo.F80), float), "(float)1.0"},
		{"pow result and arguments", castTo(castTo(pow, typeinfo.F80), float), "(float)pow((double)((long double)iSize * 0.3), (double)cDefenses)"},
		{"integer truncation", castTo(castTo(castTo(&Binary{Op: OpMul, TypeInfo: typeinfo.I16, LHS: warp, RHS: warp}, typeinfo.I16), typeinfo.F80), typeinfo.Double), "(double)(int16_t)(iWarp * iWarp)"},
		{"scanner random range", castTo(product, typeinfo.I32), "(int32_t)((long double)iSize * 0.3)"},
		{"double store and reload", castTo(castTo(product, typeinfo.Double), typeinfo.F80), "(long double)(double)((long double)iSize * 0.3)"},
		{"double then float rounding", castTo(castTo(product, typeinfo.Double), float), "(float)(double)((long double)iSize * 0.3)"},
		{"integer rounded through float", castTo(castTo(mineral, float), typeinfo.Double), "(double)(float)lMineral"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			got, _ := p.rewriter().rewriteExpr(tc.expr)
			if text := FormatExpr(got); text != tc.want {
				t.Fatalf("expression = %q, want %q", text, tc.want)
			}
		})
	}
}
