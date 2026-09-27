package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestResolveConstTypesDoubleConstants folds numeric casts without confusing
// unsigned bit patterns, signed values, or pointer conversions.
func TestResolveConstTypesDoubleConstants(t *testing.T) {
	for _, tc := range []struct {
		name   string
		source typeinfo.Type
		bits   uint64
		want   float64
	}{
		{"decimal", typeinfo.U32, 0x3e8, 1000},
		{"signed word", typeinfo.I16, 0xffff, -1},
		{"unsigned word", typeinfo.U16, 0xffff, 65535},
		{"signed dword", typeinfo.I32, 0x80000000, -2147483648},
		{"unsigned dword", typeinfo.U32, 0xffffffff, 4294967295},
		{"source width", typeinfo.U16, 0x10001, 1},
	} {
		t.Run(tc.name, func(t *testing.T) {
			constant := &Const{TypeInfo: tc.source, U64: tc.bits}
			cast := &Cast{Value: constant, To: "double", TypeInfo: typeinfo.Double}
			got, changed := resolveConstTypesExpr(cast, nil, false)
			literal, ok := got.(*FloatConst)
			if !changed || !ok || literal.F64 != tc.want || literal.TypeInfo != typeinfo.Double {
				t.Fatalf("conversion = %#v, want double %g", got, tc.want)
			}
			if cast.Value != constant || constant.U64 != tc.bits || constant.TypeInfo != tc.source {
				t.Fatal("conversion mutated the source constant")
			}
		})
	}
	pointer := &typeinfo.Pointer{Elem: typeinfo.I16, Class: typeinfo.PtrNear}
	cast := &Cast{Value: &Const{TypeInfo: typeinfo.U16, U64: 0x3e8}, To: pointer.String(), TypeInfo: pointer}
	if got, changed := resolveConstTypesExpr(cast, nil, false); changed || got != cast {
		t.Fatalf("pointer cast changed to %#v", got)
	}
}

// TestResolveConstTypesCallCount recovers signed arithmetic inside a byte
// count and removes low-word truncation only when the callee supplies it.
func TestResolveConstTypesCallCount(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "DeleteWpFar")
	fn := fx.SDB.GetFunction("fmemmove")
	if fn == nil {
		t.Fatal("fmemmove not found")
	}
	lpfl := &Local{FunctionVar: ctx.fs.Params[0]}
	iDel := &Local{FunctionVar: ctx.fs.Params[1]}
	converter := machineConverter{ctx: ctx}
	cord, ok := converter.consumeAddress(AddressExpr{Base: lpfl, Offset: 0x62, Deref: true}, 2)
	if !ok {
		t.Fatal("FLEET.cord did not resolve")
	}
	count := &Word{Part: machine.WordLow, Parent: &Binary{TypeInfo: typeinfo.U16, Op: OpMul,
		LHS: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd,
			LHS: &Binary{TypeInfo: typeinfo.U16, Op: OpSub, LHS: cord, RHS: iDel},
			RHS: &Const{TypeInfo: typeinfo.U16, U64: 0xffff}},
		RHS: &Const{TypeInfo: typeinfo.U16, U64: 18}}}
	for _, tc := range []struct {
		name string
		typ  typeinfo.Type
		argc int
		arg  Expr
		want string
	}{
		{"word parameter", typeinfo.U16, 3, count, "(((lpfl->cord - iDel) - 1) * 18)"},
		{"wide parameter", typeinfo.U32, 3, count, "loword((((lpfl->cord - iDel) - 1) * 18))"},
		{"variadic argument", typeinfo.U16, 2, count, "loword((((lpfl->cord - iDel) - 1) * 18))"},
		{"pointer offset word", typeinfo.U16, 3, &Word{Part: machine.WordLow, Parent: lpfl}, "loword(lpfl)"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			callee := *fn
			callee.Params = append([]typeinfo.FunctionVar(nil), fn.Params...)
			callee.Params[2].Type = tc.typ
			callee.Params = callee.Params[:tc.argc]
			call := &Call{Function: &callee, Args: []Expr{lpfl, lpfl, tc.arg}}
			block := Block{ID: 0x9ef2, Effects: []Effect{&CallEffect{Call: call}}}
			got, _ := (&resolveConstTypesProcessor{}).ProcessBlock(newResult(ctx.fs), Func{Blocks: []Block{block}}, block)
			arg := got.Effects[0].(*CallEffect).Call.Args[2]
			if FormatExpr(arg) != tc.want {
				t.Fatalf("count = %s, want %s", FormatExpr(arg), tc.want)
			}
			if call.Args[2] != tc.arg {
				t.Fatal("constant recovery mutated the input call")
			}
		})
	}
}

func TestResolveConstTypesAppliesComparePeerTypeInBranch(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	i := &Local{
		FunctionVar: typeinfo.FunctionVar{
			Name: "i",
			Type: int16Type,
		},
	}
	block := Block{
		ID: 0x720,
		Effects: []Effect{
			&Branch{
				Cond: &Compare{
					Op:  CompareLT,
					LHS: i,
					RHS: &Const{TypeInfo: typeinfo.U16, U64: 5},
				},
				TrueBlock:  machine.BlockID(0x6c4),
				FalseBlock: machine.BlockID(0x729),
			},
		},
	}

	got, changed := (&resolveConstTypesProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}

	gotBranch := FormatEffect(got.Effects[0])
	wantBranch := "branch i < 5 ? L_06c4 : L_0729"
	if gotBranch != wantBranch {
		t.Fatalf("branch = %q, want %q", gotBranch, wantBranch)
	}
}

func TestResolveConstTypesAppliesIntTypeToArrayIndexConst(t *testing.T) {
	int16Type := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
	rgwt := &Local{
		FunctionVar: typeinfo.FunctionVar{
			Name: "rgwt",
			Type: &typeinfo.Array{Elem: int16Type, Count: 4},
		},
	}
	block := Block{
		ID: 0x1000,
		Effects: []Effect{
			&Assign{
				Dst: &ArrayIndex{
					Base:     rgwt,
					Index:    &Const{TypeInfo: typeinfo.U16, U64: 0},
					TypeInfo: int16Type,
				},
				Src: &Const{TypeInfo: typeinfo.U16, U64: 7},
			},
			&Assign{
				Dst: &Local{
					FunctionVar: typeinfo.FunctionVar{
						Name: "i",
						Type: int16Type,
					},
				},
				Src: &ArrayIndex{
					Base:     rgwt,
					Index:    &Const{TypeInfo: typeinfo.U16, U64: 3},
					TypeInfo: int16Type,
				},
			},
		},
	}

	got, changed := (&resolveConstTypesProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}

	wants := []string{
		"rgwt[0] = 7",
		"i = rgwt[3]",
	}
	for i, want := range wants {
		if gotAssign := FormatEffect(got.Effects[i]); gotAssign != want {
			t.Fatalf("assign[%d] = %q, want %q", i, gotAssign, want)
		}
	}
}

func TestResolveConstTypesDoesNotApplyEnumTypeToBinaryConst(t *testing.T) {
	enumType := &typeinfo.Enum{
		Name: "MessageId",
		Values: []typeinfo.EnumValue{
			{Name: "idmColonistsDroppedDestroyedPlanetaryDefensesRestMa", Value: 1},
		},
	}
	idm := &Local{
		FunctionVar: typeinfo.FunctionVar{
			Name: "idm",
			Type: enumType,
		},
	}
	block := Block{
		ID: 0x1a3e,
		Effects: []Effect{
			&Assign{
				Dst: idm,
				Src: &Binary{
					TypeInfo: enumType,
					Op:       OpAdd,
					LHS:      idm,
					RHS:      &Const{TypeInfo: typeinfo.U16, U64: 1},
				},
			},
		},
	}

	got, changed := (&resolveConstTypesProcessor{}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}

	gotAssign := FormatEffect(got.Effects[0])
	wantAssign := "idm = (idm + 1)"
	if gotAssign != wantAssign {
		t.Fatalf("assign = %q, want %q", gotAssign, wantAssign)
	}
}
