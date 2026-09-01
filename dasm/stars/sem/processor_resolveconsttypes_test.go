package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

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
