package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestScratchRecoveryInlineMergeAssignment removes single-use staging without
// changing merge edge assignments, sharing multiple uses, or losing narrowing.
func TestScratchRecoveryInlineMergeAssignment(t *testing.T) {
	for _, tc := range []struct {
		name   string
		reused bool
		typ    typeinfo.Type
		want   string
	}{
		{name: "single use", typ: typeinfo.U16, want: "dst = t_merge_1006_0001"},
		{name: "narrowing", typ: typeinfo.U32, want: "dst = (uint16_t)t_merge_1006_0001"},
		{name: "multiple uses", typ: typeinfo.U16, reused: true, want: "t_scratch_m4 = t_merge_1006_0001"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			slot := scratchTestSlot(-4, typeinfo.U16)
			merge := &Merge{Join: 0x1006, TypeInfo: tc.typ, Arms: []MergeArm{
				{Block: 0x1002, Value: &Const{TypeInfo: tc.typ, U64: 1}},
				{Block: 0x1004, Value: &Const{TypeInfo: tc.typ, U64: 0}},
			}}
			ret := &Return{}
			if tc.reused {
				ret.Value = slot
			}
			fn := Func{
				CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
					jccForReturnSinkTest(0x1000, 0x1004),
					jmpForReturnSinkTest(0x1002, 0x1006),
					jmpForReturnSinkTest(0x1004, 0x1006),
					retForReturnSinkTest(0x1006),
				}),
				Blocks: []Block{
					{ID: 0x1000, Effects: []Effect{&Branch{Cond: testLocal("condition", typeinfo.U16), TrueBlock: 0x1004, FalseBlock: 0x1002}}},
					{ID: 0x1002, Effects: []Effect{&Jump{To: 0x1006}}},
					{ID: 0x1004, Effects: []Effect{&Jump{To: 0x1006}}},
					{ID: 0x1006, Effects: []Effect{&Assign{Dst: slot, Src: merge}, &Assign{Dst: testLocal("dst", tc.typ), Src: slot}, ret}},
				},
			}
			(&scratchRecoveryProcessor{}).ProcessFunc(nil, &fn)
			(&lowerMergesProcessor{}).ProcessFunc(nil, &fn)
			for i, block := range fn.Blocks[1:3] {
				if len(block.Effects) != 2 {
					t.Fatal("merge edge assignment was duplicated")
				}
				assignment := block.Effects[0].(*Assign)
				if !sameExpr(assignment.Src, merge.Arms[i].Value) {
					t.Fatal("merge incoming value changed")
				}
			}
			if got := FormatEffect(fn.Blocks[3].Effects[0]); got != tc.want {
				t.Fatalf("join assignment = %q, want %q", got, tc.want)
			}
			count := 2
			if tc.reused {
				count = 3
			}
			if len(fn.Blocks[3].Effects) != count {
				t.Fatalf("join effects = %v", formatEffects(fn.Blocks[3].Effects))
			}
			assertMergeAnalysisZero(t, &fn)
		})
	}
}

// TestScratchRecoveryInlineCallAssignment verifies a single-use call can feed a
// fixed destination without duplicating the call or reordering other evaluation.
func TestScratchRecoveryInlineCallAssignment(t *testing.T) {
	plan := testfixture.Stars(t).SDB.GetStruct("BTLPLAN")
	var dumpCargo *typeinfo.StructField
	for i := range plan.Fields {
		if plan.Fields[i].Name == "fDumpCargo" {
			dumpCargo = &plan.Fields[i]
			break
		}
	}
	if dumpCargo == nil {
		t.Fatal("BTLPLAN is missing fDumpCargo")
	}
	field := &SymbolRef{Path: &symresolve.SymbolBitfield{
		Base:  &symresolve.SymbolRoot{Symbol: &typeinfo.GlobalVar{Name: "btlplan", Type: plan}},
		Field: dumpCargo,
	}}
	slot := scratchTestSlot(-4, typeinfo.U16)
	alias := scratchTestSlot(-8, typeinfo.U16)
	read := &typeinfo.Function{Name: "Read", Ret: typeinfo.I32}
	call := &Call{Function: read}
	definition := &Assign{Dst: slot, Src: &Word{Parent: call, Part: machine.WordLow}}
	use := &Assign{Dst: field, Src: slot}
	other := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "other", Type: typeinfo.U16}}
	pointer := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "pointer", Type: &typeinfo.Pointer{Elem: typeinfo.U16, Class: typeinfo.PtrNear}}}
	for _, tc := range []struct {
		name    string
		effects []Effect
		extra   []Block
		want    string
	}{
		{name: "bitfield assignment", effects: []Effect{definition, use}, want: "btlplan.fDumpCargo = loword(Read())"},
		{name: "narrowing store", effects: []Effect{&Assign{Dst: slot, Src: call}, &Assign{Dst: testLocal("wide", typeinfo.I32), Src: slot}}, want: "wide = (uint16_t)Read()"},
		{name: "signedness conversion", effects: []Effect{&Assign{Dst: slot, Src: &Cast{Value: call, To: typeinfo.I16.String(), TypeInfo: typeinfo.I16}}, &Assign{Dst: testLocal("unsigned", typeinfo.U16), Src: slot}}, want: "unsigned = (uint16_t)(int16_t)Read()"},
		{name: "call effect result", effects: []Effect{&CallEffect{Call: call, Result: scratchTestSlot(-4, typeinfo.I32)}, &Assign{Dst: testLocal("wide", typeinfo.I32), Src: scratchTestSlot(-4, typeinfo.I32)}}, want: "wide = Read()"},
		{name: "multiple uses", effects: []Effect{definition, use, &Return{Value: slot}}},
		{name: "unused result", effects: []Effect{definition, &Return{}}},
		{name: "uses through alias", effects: []Effect{definition, &Assign{Dst: alias, Src: slot}, &Assign{Dst: field, Src: &Binary{Op: OpAdd, TypeInfo: typeinfo.U16, LHS: alias, RHS: alias}}}},
		{name: "sibling memory read", effects: []Effect{definition, &Assign{Dst: field, Src: &Binary{Op: OpAdd, TypeInfo: typeinfo.U16, LHS: slot, RHS: other}}}},
		{name: "pointer destination", effects: []Effect{definition, &Assign{Dst: &Deref{Pointer: pointer, Width: 2, TypeInfo: typeinfo.U16}, Src: slot}}},
		{name: "intervening call", effects: []Effect{definition, &CallEffect{Call: &Call{Function: &typeinfo.Function{Name: "Mutate", Ret: typeinfo.U16}}}, use}},
		{name: "use in another block", effects: []Effect{definition, &Jump{To: 2}}, extra: []Block{{ID: 2, Effects: []Effect{use}}}},
		{name: "second use in another block", effects: []Effect{definition, use, &Jump{To: 2}}, extra: []Block{{ID: 2, Effects: []Effect{&Return{Value: slot}}}}},
	} {
		t.Run(tc.name, func(t *testing.T) {
			fn := Func{Blocks: append([]Block{{ID: 1, Effects: tc.effects}}, tc.extra...)}
			(&scratchRecoveryProcessor{}).ProcessFunc(nil, &fn)
			calls := 0
			for _, block := range fn.Blocks {
				for _, effect := range block.Effects {
					walkEffect(effect, func(expr Expr) {
						if c, ok := expr.(*Call); ok && c.Function == read {
							calls++
						}
					})
				}
			}
			if calls != 1 {
				t.Fatalf("Read is evaluated %d times, want once", calls)
			}
			first := fn.Blocks[0].Effects[0].(*Assign)
			if tc.want != "" {
				if got := FormatEffect(first); got != tc.want {
					t.Fatalf("assignment = %q, want %q", got, tc.want)
				}
				if len(fn.Blocks[0].Effects) != 1 {
					t.Fatal("call capture was not removed")
				}
			} else if _, captured := first.Dst.(*Temp); !captured {
				t.Fatalf("call capture was lost: %s", FormatEffect(first))
			}
		})
	}
}
