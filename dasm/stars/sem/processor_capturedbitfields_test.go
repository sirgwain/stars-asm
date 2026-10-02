package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCapturedBitfields verifies declared field recovery and its boundaries
// using the SCOREX and MSGHDR layouts from the original debug symbols.
func TestCapturedBitfields(t *testing.T) {
	fx := testfixture.Stars(t)
	for _, name := range []string{"SCOREX", "MSGHDR"} {
		t.Run(name, func(t *testing.T) {
			typ := fx.SDB.GetStruct(name)
			var member *typeinfo.StructField
			for i := range typ.Fields {
				if typ.Fields[i].Name == "grbitVC" || typ.Fields[i].Name == "grWord" {
					member = &typ.Fields[i]
				}
			}
			base := testLocal("record", typ)
			field := &FieldAccess{Base: base, Field: member}
			word := &Part{Base: base, Width: 2, TypeInfo: typeinfo.U16}
			temp := &Temp{Name: "t_snapshot", TypeInfo: typeinfo.U16}
			start, width := member.BitRange()
			var added Expr = &Const{TypeInfo: typeinfo.U16, U64: 1 << start}
			if name == "MSGHDR" {
				added = &Binary{TypeInfo: typeinfo.U16, Op: OpShl, LHS: testLocal("grbit", typeinfo.U16), RHS: &Const{TypeInfo: typeinfo.U16, U64: uint64(start)}}
			}
			mask := &Const{TypeInfo: typeinfo.U16, U64: ((uint64(1) << width) - 1) << start}
			capture := &Assign{Dst: temp, Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAnd, LHS: &Binary{TypeInfo: typeinfo.U16, Op: OpOr, LHS: word, RHS: added}, RHS: mask}}
			clear := &Assign{Dst: field, Src: &Const{TypeInfo: typeinfo.U16}}
			insert := &Assign{Dst: word, Src: &Binary{TypeInfo: typeinfo.U16, Op: OpOr, LHS: word, RHS: temp}}
			other := *clear
			other.Dst = &FieldAccess{Base: testLocal("different", typ), Field: member}
			spill := *capture
			badMask := *capture.Src.(*Binary)
			badMask.RHS = &Const{TypeInfo: typeinfo.U16, U64: mask.U64 | 1}
			spill.Src = &badMask
			observe := &typeinfo.Function{Name: "Observe", Ret: typeinfo.I16}
			call := &CallEffect{Call: &Call{Function: observe, Target: &FunctionRef{Function: observe}}}
			for _, tc := range []struct {
				name    string
				effects []Effect
				want    bool
			}{
				{"complete", []Effect{capture, clear, insert}, true},
				{"neighbor bits", []Effect{&spill, clear, insert}, false},
				{"other destination", []Effect{capture, &other, insert}, false},
				{"intervening call", []Effect{capture, clear, call, insert}, false},
				{"extra snapshot use", []Effect{capture, clear, insert, &Return{Value: temp}}, false},
			} {
				t.Run(tc.name, func(t *testing.T) {
					f := &Func{Blocks: []Block{{ID: 1, Effects: append([]Effect{}, tc.effects...)}}}
					if got := (&capturedBitfieldsProcessor{}).ProcessFunc(nil, f); got != tc.want {
						t.Fatalf("recovered = %v, want %v", got, tc.want)
					}
					if tc.want {
						if len(f.Blocks[0].Effects) != 1 {
							t.Fatal("snapshot or clear retained")
						}
						a := f.Blocks[0].Effects[0].(*Assign)
						or, ok := a.Src.(*Binary)
						if !ok || or.Op != OpOr || !sameExpr(a.Dst, field) || !sameExpr(or.LHS, field) {
							t.Fatal("recovered update changed its destination")
						}
						if name == "MSGHDR" && !sameExpr(or.RHS, added.(*Binary).LHS) {
							t.Fatalf("recovered flag = %s", FormatEffect(a))
						}
					}
				})
			}
		})
	}
}
