package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestPostIncrementsProcessor verifies which saved-then-stepped locals and
// globals fold into their use as x++ or x--.
func TestPostIncrementsProcessor(t *testing.T) {
	charPtr := &typeinfo.Pointer{Elem: &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}, Class: typeinfo.PtrFar}
	n := testLocal("n", typeinfo.I16)
	gCount := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "gCount", Type: typeinfo.I16}}
	src := testLocal("pszT", charPtr)
	dst := testLocal("psz", charPtr)
	saveN := &Temp{Name: "t_5cb7", TypeInfo: typeinfo.I16}
	saveSrc := &Temp{Name: "t_2249", TypeInfo: charPtr}
	saveDst := &Temp{Name: "t_2252", TypeInfo: charPtr}
	one := &Const{TypeInfo: typeinfo.I16, U64: 1}
	zero := &Const{TypeInfo: typeinfo.I16}
	decN := &Assign{Dst: n, Src: &Binary{TypeInfo: typeinfo.I16, Op: OpSub, LHS: n, RHS: one}}
	step := func(p *Local) *Assign {
		return &Assign{Dst: p, Src: &AddressOf{Target: &ArrayIndex{Base: p, Index: one, TypeInfo: charPtr.Elem}, TypeInfo: charPtr}}
	}
	deref := func(p Expr) *Deref { return &Deref{Pointer: p} }

	for _, tc := range []struct {
		name    string
		effects []Effect
		want    []string
	}{
		{
			name: "decrement folded into its test",
			effects: []Effect{
				&Assign{Dst: saveN, Src: n},
				decN,
				&Branch{Cond: &Compare{Op: CompareEQ, LHS: saveN, RHS: zero}},
			},
			want: []string{"branch n-- == 0 ? L_0000 : L_0000"},
		},
		{
			name: "interleaved pointer steps folded into one copy",
			effects: []Effect{
				&Assign{Dst: saveSrc, Src: src},
				step(src),
				&Assign{Dst: saveDst, Src: dst},
				step(dst),
				&Assign{Dst: deref(saveDst), Src: deref(saveSrc)},
			},
			want: []string{"*psz++ = *pszT++"},
		},
		{
			name: "global increment folded into the next effect",
			effects: []Effect{
				&Assign{Dst: saveN, Src: gCount},
				&Assign{Dst: gCount, Src: &Binary{TypeInfo: typeinfo.I16, Op: OpAdd, LHS: gCount, RHS: one}},
				&Assign{Dst: n, Src: saveN},
			},
			want: []string{"n = gCount++"},
		},
		{
			name: "global kept when a call in the use runs before the step",
			effects: []Effect{
				&Assign{Dst: saveN, Src: gCount},
				&Assign{Dst: gCount, Src: &Binary{TypeInfo: typeinfo.I16, Op: OpAdd, LHS: gCount, RHS: one}},
				&Assign{Dst: n, Src: &Binary{TypeInfo: typeinfo.I16, Op: OpAdd, LHS: saveN, RHS: &Call{Function: &typeinfo.Function{Name: "Touch"}, Target: &FunctionRef{Function: &typeinfo.Function{Name: "Touch"}}}}},
			},
			want: []string{"t_5cb7 = gCount", "gCount = (gCount + 1)", "n = (t_5cb7 + Touch())"},
		},
		{
			name: "kept when the use also reads the local",
			effects: []Effect{
				&Assign{Dst: saveN, Src: n},
				decN,
				&Branch{Cond: &Compare{Op: CompareEQ, LHS: saveN, RHS: n}},
			},
			want: []string{"t_5cb7 = n", "n = (n - 1)", "branch t_5cb7 == n ? L_0000 : L_0000"},
		},
		{
			name: "kept when the use evaluates it conditionally",
			effects: []Effect{
				&Assign{Dst: saveN, Src: n},
				decN,
				&Assign{Dst: testLocal("r", typeinfo.I16), Src: &Cond{TypeInfo: typeinfo.I16, Cond: &Compare{Op: CompareEQ, LHS: testLocal("f", typeinfo.I16), RHS: zero}, Then: saveN, Else: zero}},
			},
			want: []string{"t_5cb7 = n", "n = (n - 1)", "r = (f == 0 ? t_5cb7 : 0)"},
		},
		{
			name: "kept when the local's address is taken",
			effects: []Effect{
				&Assign{Dst: saveN, Src: n},
				decN,
				&CallEffect{Call: &Call{Function: &typeinfo.Function{Name: "Touch"}, Target: &FunctionRef{Function: &typeinfo.Function{Name: "Touch"}}, Args: []Expr{&AddressOf{Target: n, TypeInfo: &typeinfo.Pointer{Elem: typeinfo.I16}}, saveN}}},
			},
			want: []string{"t_5cb7 = n", "n = (n - 1)", "call Touch(&n, t_5cb7)"},
		},
	} {
		t.Run(tc.name, func(t *testing.T) {
			fn := &Func{
				CFG:    cfgForReturnSinkTest(t, []asm.DecodedInst{retForReturnSinkTest(0x1000)}),
				Blocks: []Block{{ID: 0x1000, Effects: tc.effects}},
			}
			(&postIncrementsProcessor{}).ProcessFunc(nil, fn)
			if got := formatEffects(fn.Blocks[0].Effects); !equalStrings(got, tc.want) {
				t.Fatalf("effects = %#v, want %#v", got, tc.want)
			}
		})
	}
}
