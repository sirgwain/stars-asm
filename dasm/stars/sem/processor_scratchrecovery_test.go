package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestScratchRecoveryRewritesSymbolicScratch verifies scratch aliases remain
// eligible for elision after storage resolution creates SymbolRef nodes.
func TestScratchRecoveryRewritesSymbolicScratch(t *testing.T) {
	scratch := &symresolve.SymbolScratch{
		Function:    typeinfo.Addr{Seg: 1, Off: 0x100},
		BPOffset:    -4,
		StorageSize: 2,
		TypeInfo:    typeinfo.U16,
	}
	dst := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dst", Type: typeinfo.U16}}
	block := Block{Effects: []Effect{
		&Assign{
			Dst: &SymbolRef{Path: scratch},
			Src: &Const{TypeInfo: typeinfo.U16, U64: 7},
		},
		&Assign{
			Dst: dst,
			Src: &SymbolRef{Path: &symresolve.SymbolScratch{
				Function:    scratch.Function,
				BPOffset:    scratch.BPOffset,
				StorageSize: scratch.StorageSize,
				TypeInfo:    typeinfo.U16,
			}},
		},
	}}

	fn := Func{Blocks: []Block{block}}
	changed := (&scratchRecoveryProcessor{}).ProcessFunc(nil, &fn)
	got := fn.Blocks[0]
	if !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if len(got.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(got.Effects))
	}
	if text := FormatEffect(got.Effects[0]); text != "dst = 0x7" {
		t.Fatalf("effect = %q, want %q", text, "dst = 0x7")
	}
}

// TestScratchRecoveryCallEvaluation distinguishes calls consuming a saved load
// from sibling calls which may run before that load is evaluated.
func TestScratchRecoveryCallEvaluation(t *testing.T) {
	slot := scratchTestSlot(-4, typeinfo.U16)
	global := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "value", Type: typeinfo.U16}}
	callee := &typeinfo.Function{Name: "Use", Ret: typeinfo.U16, Params: []typeinfo.FunctionVar{{Name: "value", Type: typeinfo.U16}}}
	mutate := &Call{Function: &typeinfo.Function{Name: "Mutate", Ret: typeinfo.U16}}
	consumer := &Call{Function: callee, Args: []Expr{slot}}
	for _, tc := range []struct {
		name    string
		use     Effect
		capture bool
	}{
		{"call effect argument", &CallEffect{Call: consumer}, false},
		{"assigned call argument", &Assign{Dst: &Local{FunctionVar: typeinfo.FunctionVar{Name: "result", Type: typeinfo.U16}}, Src: consumer}, false},
		{"return call argument", &Return{Value: consumer}, false},
		{"nested enclosing calls", &Return{Value: &Call{Function: callee, Args: []Expr{consumer}}}, false},
		{"sibling argument call", &CallEffect{Call: &Call{Function: callee, Args: []Expr{slot, mutate}}}, true},
		{"sibling operand call", &Return{Value: &Binary{Op: OpAdd, TypeInfo: typeinfo.U16, LHS: slot, RHS: mutate}}, true},
		{"two consuming calls", &Return{Value: &Binary{Op: OpAdd, TypeInfo: typeinfo.U16, LHS: consumer, RHS: consumer}}, true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{&Assign{Dst: slot, Src: global}, tc.use}}}}
			(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
			want := 1
			if tc.capture {
				want = 2
			}
			if len(f.Blocks[0].Effects) != want {
				t.Fatalf("effects = %v, want %d", f.Blocks[0].Effects, want)
			}
		})
	}
}

// TestScratchRecoveryIndependentWrites allows unescaped staging stores between
// a saved load and its use, but retains the load when the written object escaped.
func TestScratchRecoveryIndependentWrites(t *testing.T) {
	for _, escaped := range []bool{false, true} {
		saved := scratchTestSlot(-8, typeinfo.U16)
		other := scratchTestSlot(-4, typeinfo.U16)
		pointerType := &typeinfo.Pointer{Elem: typeinfo.U16, Class: typeinfo.PtrNear}
		pointer := &Local{FunctionVar: typeinfo.FunctionVar{Name: "pointer", Type: pointerType}}
		var effects []Effect
		if escaped {
			effects = append(effects, &Assign{Dst: pointer, Src: &AddressOf{Target: other, TypeInfo: pointerType}})
		}
		effects = append(effects, &Assign{Dst: saved, Src: &Deref{Pointer: pointer, Width: 2, TypeInfo: typeinfo.U16}}, &Assign{Dst: other, Src: &Const{TypeInfo: typeinfo.U16, U64: 9}}, &Return{Value: saved})
		f := Func{Blocks: []Block{{ID: 1, Effects: effects}}}
		(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
		if escaped {
			if _, ok := f.Blocks[0].Effects[1].(*Assign).Dst.(*Temp); !ok {
				t.Fatal("load through an escaped scratch address was moved past its write")
			}
		} else if len(f.Blocks[0].Effects) != 1 {
			t.Fatal("unrelated scratch write blocked substitution")
		}
	}
}

// TestScratchRecoveryUndefinedDependency keeps an undefined source as physical
// storage when a defined staging slot is inlined into a later read.
func TestScratchRecoveryUndefinedDependency(t *testing.T) {
	unknown := scratchTestSlot(-4, typeinfo.U16)
	staging := scratchTestSlot(-8, typeinfo.U16)
	f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{&Assign{Dst: staging, Src: unknown}, &Return{Value: staging}}}}}
	(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
	if len(f.Blocks[0].Effects) != 1 || f.Blocks[0].Effects[0].(*Return).Value != unknown {
		t.Fatal("undefined source was replaced with an uninitialized semantic temp")
	}
}

// TestScratchRecoveryCallDestination inventories a scratch call result as a
// definition and preserves the call when the result is materialized.
func TestScratchRecoveryCallDestination(t *testing.T) {
	slot := scratchTestSlot(-4, typeinfo.U16)
	f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{&CallEffect{Call: &Call{Function: &typeinfo.Function{Name: "Read", Ret: typeinfo.U16}}, Result: slot}, &Return{Value: slot}}}}}
	(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
	call := f.Blocks[0].Effects[0].(*CallEffect)
	result, ok := call.Result.(*Temp)
	if !ok || f.Blocks[0].Effects[1].(*Return).Value != result {
		t.Fatal("call result was not materialized consistently")
	}
}

// TestScratchRecoveryPromotedDependency preserves a saved loop value when its
// dependency's physical storage is rewritten before the saved value is consumed.
func TestScratchRecoveryPromotedDependency(t *testing.T) {
	value, saved := scratchTestSlot(-4, typeinfo.U16), scratchTestSlot(-8, typeinfo.U16)
	output := &Local{FunctionVar: typeinfo.FunctionVar{Name: "output", Type: typeinfo.U16}}
	f := Func{Blocks: []Block{
		{ID: 1, Effects: []Effect{&Assign{Dst: value, Src: &Const{TypeInfo: typeinfo.U16, U64: 1}}, &Jump{To: 2}}},
		{ID: 2, Effects: []Effect{&Assign{Dst: saved, Src: value}, &Assign{Dst: value, Src: &Const{TypeInfo: typeinfo.U16, U64: 9}}, &Assign{Dst: output, Src: saved}, &Branch{Cond: output, TrueBlock: 2, FalseBlock: 3}}},
		{ID: 3, Effects: []Effect{&Return{Value: value}}},
	}}
	(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
	effects := f.Blocks[1].Effects
	if len(effects) != 4 {
		t.Fatal("saved loop value was inlined across dependency mutation")
	}
	first := effects[0].(*Assign).Dst
	if _, ok := first.(*Temp); !ok || !sameExpr(effects[2].(*Assign).Src, first) {
		t.Fatal("saved loop value did not retain its own temporary")
	}
}

// TestScratchRecoveryReusedTypes keeps a far pointer and a later scalar which
// reuse the same bytes in separate, correctly typed semantic lifetimes.
func TestScratchRecoveryReusedTypes(t *testing.T) {
	ptrType := &typeinfo.Pointer{Elem: typeinfo.I16, Class: typeinfo.PtrFar}
	pointer := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "pointer", Type: ptrType}}
	integer := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "integer", Type: typeinfo.I32}}
	slot := scratchTestSlot(-8, typeinfo.U32)
	call := &CallEffect{Call: &Call{Function: &typeinfo.Function{Name: "Mutate", Ret: typeinfo.U16}}}
	f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{
		&Assign{Dst: scratchTestSlot(-8, typeinfo.U16), Src: &Word{Parent: pointer, Part: machine.WordLow}},
		&Assign{Dst: scratchTestSlot(-6, typeinfo.U16), Src: &Word{Parent: pointer, Part: machine.WordHigh}},
		call, &Assign{Dst: &Local{FunctionVar: typeinfo.FunctionVar{Name: "p", Type: ptrType}}, Src: slot},
		&Assign{Dst: slot, Src: integer}, call, &Return{Value: slot},
	}}}}
	(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
	first := f.Blocks[0].Effects[0].(*Assign).Dst.(*Temp)
	second := f.Blocks[0].Effects[3].(*Assign).Dst.(*Temp)
	if first.Name == second.Name || first.TypeInfo != ptrType || second.TypeInfo != typeinfo.I32 {
		t.Fatalf("reused types: %v, %v", first, second)
	}
}

// TestScratchRecoveryAggregateCopy recovers the complete RTBOF storage type and
// leaves its field projection to the ordinary late address pass.
func TestScratchRecoveryAggregateCopy(t *testing.T) {
	fx := testfixture.Stars(t)
	header := fx.SDB.GetStruct("RTBOF")
	slot := scratchTestSlot(-32, scratchTypeForWidth(header.Bytes()))
	field := &SymbolRef{Path: &symresolve.SymbolField{Base: &symresolve.SymbolScratch{Function: typeinfo.Addr{Seg: 1, Off: 0x100}, BPOffset: -32, StorageSize: 16, TypeInfo: header}, Field: header.FieldsContainingOffset(10)[0].Field}}
	f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{
		&Assign{Dst: slot, Src: &Global{GlobalVar: &typeinfo.GlobalVar{Name: "header", Type: header}}},
		&Return{Value: field},
	}}}}
	(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
	if len(f.RecoveredLocals) != 1 || f.RecoveredLocals[0].Type != header {
		t.Fatal("aggregate copy did not recover its complete type")
	}
	if got := FormatEffect(f.Blocks[0].Effects[1]); got != "return t_scratch_m20.turn" {
		t.Fatalf("field = %s", got)
	}
}

// TestScratchRecoveryRawStorage handles both raw BP memory and address-only
// frame operands, retaining storage even when the address precedes any write.
func TestScratchRecoveryRawStorage(t *testing.T) {
	for _, addressOnly := range []bool{false, true} {
		memory := &Memory{Base: &RawValue{Value: &machine.FrameBase{}, TypeInfo: typeinfo.U16}, Disp: -4, Width: 2, TypeInfo: typeinfo.U16}
		f := Func{Blocks: []Block{{ID: 1}}}
		if addressOnly {
			memory.Width = 0
			f.Blocks[0].Effects = []Effect{&Return{Value: &AddressOf{Target: memory, TypeInfo: &typeinfo.Pointer{Elem: typeinfo.U16, Class: typeinfo.PtrNear}}}}
		} else {
			raw := &RawMemory{Address: machine.MemoryAddress{Base: &machine.FrameBase{}, Disp: -4, Width: 2}, TypeInfo: typeinfo.U16}
			f.Blocks[0].Effects = []Effect{&Assign{Dst: memory, Src: &Const{TypeInfo: typeinfo.U16, U64: 7}}, &Return{Value: raw}}
		}
		(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
		if addressOnly {
			if len(f.RecoveredLocals) != 1 {
				t.Fatal("address-only scratch storage was not recovered")
			}
		} else if got := FormatEffect(f.Blocks[0].Effects[0]); got != "return 0x7" {
			t.Fatalf("raw read = %s", got)
		}
	}
}

// TestScratchRecoveryReconstructsSymbolicWideValue verifies adjacent
// symbolic scratch words can satisfy a later explicit dword read.
func TestScratchRecoveryReconstructsSymbolicWideValue(t *testing.T) {
	function := typeinfo.Addr{Seg: 1, Off: 0x100}
	src := &Local{FunctionVar: typeinfo.FunctionVar{Name: "src", Type: typeinfo.I16}}
	dst := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dst", Type: typeinfo.I32}}
	scratch := func(disp, width int, typ typeinfo.Type) *SymbolRef {
		return &SymbolRef{Path: &symresolve.SymbolScratch{
			Function:    function,
			BPOffset:    disp,
			StorageSize: width,
			TypeInfo:    typ,
		}}
	}
	block := Block{Effects: []Effect{
		&Assign{Dst: scratch(-8, 2, typeinfo.U16), Src: src},
		&Assign{
			Dst: scratch(-6, 2, typeinfo.U16),
			Src: &Word{Parent: src, Part: machine.WordSignHigh},
		},
		&Assign{Dst: dst, Src: scratch(-8, 4, typeinfo.I32)},
	}}

	fn := Func{Blocks: []Block{block}}
	changed := (&scratchRecoveryProcessor{}).ProcessFunc(nil, &fn)
	got := fn.Blocks[0]
	if !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if len(got.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(got.Effects))
	}
	if text := FormatEffect(got.Effects[0]); text != "dst = sext16to32(src)" {
		t.Fatalf("effect = %q, want %q", text, "dst = sext16to32(src)")
	}
}

// TestScratchRecoveryReconstructsSymbolicWideWords verifies an explicit
// dword read combines otherwise unrelated low and high scratch aliases.
func TestScratchRecoveryReconstructsSymbolicWideWords(t *testing.T) {
	function := typeinfo.Addr{Seg: 1, Off: 0x100}
	dst := &Local{FunctionVar: typeinfo.FunctionVar{Name: "dst", Type: typeinfo.U32}}
	scratch := func(disp, width int) *SymbolRef {
		return &SymbolRef{Path: &symresolve.SymbolScratch{
			Function:    function,
			BPOffset:    disp,
			StorageSize: width,
			TypeInfo:    scratchTypeForWidth(width),
		}}
	}
	block := Block{Effects: []Effect{
		&Assign{Dst: scratch(-8, 2), Src: &Const{TypeInfo: typeinfo.U16, U64: 1}},
		&Assign{Dst: scratch(-6, 2), Src: &Const{TypeInfo: typeinfo.U16, U64: 2}},
		&Assign{Dst: dst, Src: scratch(-8, 4)},
	}}

	fn := Func{Blocks: []Block{block}}
	changed := (&scratchRecoveryProcessor{}).ProcessFunc(nil, &fn)
	got := fn.Blocks[0]
	if !changed {
		t.Fatal("ProcessFunc changed = false, want true")
	}
	if len(got.Effects) != 1 {
		t.Fatalf("effects = %d, want 1", len(got.Effects))
	}
	if text := FormatEffect(got.Effects[0]); text != "dst = 0x20001" {
		t.Fatalf("effect = %q, want %q", text, "dst = 0x20001")
	}
}

// TestScratchRecoveryMergedWideValue reconstructs split arithmetic on equivalent
// edge-selected values without duplicating their merge assignments.
func TestScratchRecoveryMergedWideValue(t *testing.T) {
	for _, tc := range []struct {
		name       string
		changeHigh func(*Merge)
		impure     bool
		merges     int
	}{
		{name: "equivalent", merges: 1},
		{name: "different value", merges: 2, changeHigh: func(m *Merge) {
			m.Arms[1].Value = testLocal("other", typeinfo.I32)
		}},
		{name: "different incoming edges", merges: 2, changeHigh: func(m *Merge) {
			m.Arms[0].Block, m.Arms[1].Block = m.Arms[1].Block, m.Arms[0].Block
		}},
		{name: "different type", merges: 2, changeHigh: func(m *Merge) {
			m.TypeInfo = typeinfo.U32
		}},
		{name: "calls in arms", impure: true, merges: 2},
	} {
		t.Run(tc.name, func(t *testing.T) {
			// Build independent expression trees, as semantic conversion does
			// for the low and high words of a compiler helper result.
			value := func(high bool) Expr {
				merge := &Merge{TypeInfo: typeinfo.I32, Join: 0x1006, Arms: []MergeArm{
					{Block: 0x1002, Value: testLocal("left", typeinfo.I32)},
					{Block: 0x1004, Value: testLocal("right", typeinfo.I32)},
				}}
				if tc.impure {
					merge.Arms[0].Value = &Call{Function: &typeinfo.Function{Name: "Read", Ret: typeinfo.I32}}
				}
				if high && tc.changeHigh != nil {
					tc.changeHigh(merge)
				}
				return &Cast{To: typeinfo.I32.String(), TypeInfo: typeinfo.I32, Value: &Binary{
					Op: OpMul, TypeInfo: typeinfo.I32, LHS: merge, RHS: testConst(2),
				}}
			}
			fn := Func{
				CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
					jccForReturnSinkTest(0x1000, 0x1004),
					jmpForReturnSinkTest(0x1002, 0x1006),
					jmpForReturnSinkTest(0x1004, 0x1006),
					jmpForReturnSinkTest(0x1006, 0x1008),
					retForReturnSinkTest(0x1008),
				}),
				Blocks: []Block{
					{ID: 0x1000, Effects: []Effect{&Branch{Cond: testLocal("condition", typeinfo.U16), TrueBlock: 0x1004, FalseBlock: 0x1002}}},
					{ID: 0x1002, Effects: []Effect{&Jump{To: 0x1006}}},
					{ID: 0x1004, Effects: []Effect{&Jump{To: 0x1006}}},
					{ID: 0x1006, Effects: []Effect{
						&Assign{Dst: scratchTestSlot(-4, typeinfo.U16), Src: &Word{Part: machine.WordLow, Parent: value(false)}},
						&Assign{Dst: scratchTestSlot(-2, typeinfo.U16), Src: &Word{Part: machine.WordHigh, Parent: value(true)}},
						&Jump{To: 0x1008},
					}},
					{ID: 0x1008, Effects: []Effect{&Return{Value: scratchTestSlot(-4, typeinfo.I32)}}},
				},
			}
			(&scratchRecoveryProcessor{}).ProcessFunc(nil, &fn)
			(&lowerMergesProcessor{}).ProcessFunc(nil, &fn)
			for _, block := range fn.Blocks[1:3] {
				if got := len(block.Effects) - 1; got != tc.merges {
					t.Fatalf("edge %s has %d merge assignments, want %d: %v", block.ID, got, tc.merges, formatEffects(block.Effects))
				}
			}
			stores := 1
			if tc.impure {
				stores = 2
			}
			if got := len(fn.Blocks[3].Effects) - 1; got != stores {
				t.Fatalf("join has %d stores, want %d", got, stores)
			}
			if tc.merges == 1 {
				if got, want := FormatEffect(fn.Blocks[3].Effects[0]), "t_scratch_m4 = (int32_t)(t_merge_1006_0001 * 0x2)"; got != want {
					t.Fatalf("wide assignment = %q, want %q", got, want)
				}
			}
			assertMergeAnalysisZero(t, &fn)
		})
	}
}

// scratchTestSlot constructs a typed physical scratch view for recovery fixtures.
func scratchTestSlot(offset int, typ typeinfo.Type) *SymbolRef {
	return &SymbolRef{Path: &symresolve.SymbolScratch{Function: typeinfo.Addr{Seg: 1, Off: 0x100}, BPOffset: offset, StorageSize: typ.Bytes(), TypeInfo: typ}}
}

// TestScratchRecoveryCFG covers equivalent and conflicting joins, missing
// incoming definitions, loops, and propagation through a diamond.
func TestScratchRecoveryCFG(t *testing.T) {
	for _, tc := range []struct {
		name             string
		right            uint64
		missing, loop    bool
		equivalentScalar bool
		want             scratchDisposition
	}{
		{name: "equivalent", right: 7, want: scratchInline},
		{name: "equivalent scalar", equivalentScalar: true, want: scratchInline},
		{name: "different", right: 9, want: scratchTemp},
		{name: "undefined predecessor", missing: true, want: scratchUnresolved},
		{name: "loop", right: 9, loop: true, want: scratchTemp},
	} {
		t.Run(tc.name, func(t *testing.T) {
			slot := scratchTestSlot(-4, typeinfo.U16)
			left := []Effect{&Assign{Dst: slot, Src: &Const{TypeInfo: typeinfo.U16, U64: 7}}, &Jump{To: 4}}
			right := []Effect{&Assign{Dst: slot, Src: &Const{TypeInfo: typeinfo.U16, U64: tc.right}}, &Jump{To: 4}}
			if tc.equivalentScalar {
				left[0].(*Assign).Src = &Local{FunctionVar: typeinfo.FunctionVar{Name: "source", Type: typeinfo.U16}}
				right[0].(*Assign).Src = &Local{FunctionVar: typeinfo.FunctionVar{Name: "source", Type: typeinfo.U16}}
			}
			if tc.missing {
				right = right[1:]
			}
			tail := []Effect{&Return{Value: slot}}
			if tc.loop {
				tail = []Effect{&Assign{Dst: &Local{FunctionVar: typeinfo.FunctionVar{Name: "out", Type: typeinfo.U16}}, Src: slot}, &Branch{Cond: &Const{TypeInfo: typeinfo.U16, U64: 1}, TrueBlock: 3, FalseBlock: 5}}
			}
			f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{&Branch{Cond: &Local{FunctionVar: typeinfo.FunctionVar{Name: "condition", Type: typeinfo.U16}}, TrueBlock: 2, FalseBlock: 3}}}, {ID: 2, Effects: left}, {ID: 3, Effects: right}, {ID: 4, Effects: tail}, {ID: 5, Effects: []Effect{&Return{}}}}}
			a := analyzeScratchStorage(&f)
			a.solve(&f)
			buildScratchRecoveryPlan(&f, a)
			if a.objects[0].disposition != tc.want {
				t.Fatalf("disposition = %v, want %v", a.objects[0].disposition, tc.want)
			}
			(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
			if tc.want == scratchInline {
				want := "return 0x7"
				if tc.equivalentScalar {
					want = "return source"
				}
				if got := FormatEffect(f.Blocks[3].Effects[0]); got != want {
					t.Fatalf("join = %s", got)
				}
			}
			if tc.want == scratchTemp {
				if _, ok := f.Blocks[1].Effects[0].(*Assign).Dst.(*Temp); !ok {
					t.Fatal("join definition was not promoted")
				}
			}
		})
	}
}

// TestScratchRecoveryOrdering preserves snapshots of mutable memory, registers,
// and locals, including reads used in a destination's address or call target.
func TestScratchRecoveryOrdering(t *testing.T) {
	slot := scratchTestSlot(-4, typeinfo.U16)
	local := &Local{FunctionVar: typeinfo.FunctionVar{Name: "value", Type: typeinfo.U16}}
	global := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "global", Type: typeinfo.U16}}
	call := &CallEffect{Call: &Call{Function: &typeinfo.Function{Name: "Mutate", Ret: typeinfo.U16}}}
	for _, tc := range []struct {
		name    string
		source  Expr
		barrier Effect
		use     Effect
	}{
		{"memory across call", global, call, &Return{Value: slot}},
		{"register across call", &Register{Val: asm.RegAX}, call, &Return{Value: slot}},
		{"local overwritten", local, &Assign{Dst: local, Src: &Const{TypeInfo: typeinfo.U16, U64: 9}}, &Return{Value: slot}},
		{"symbolic local overwritten", &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: &local.FunctionVar}}, &Assign{Dst: local, Src: &Const{TypeInfo: typeinfo.U16, U64: 9}}, &Return{Value: slot}},
		{"local call destination", local, &CallEffect{Call: call.Call, Result: local}, &Return{Value: slot}},
		{"destination address", global, call, &Assign{Dst: &Deref{Pointer: &Cast{Value: slot, TypeInfo: &typeinfo.Pointer{Elem: typeinfo.U16, Class: typeinfo.PtrNear}, To: "uint16_t *"}, Width: 2, TypeInfo: typeinfo.U16}, Src: local}},
		{"indirect call", global, call, &CallEffect{Call: &Call{Target: slot}}},
	} {
		t.Run(tc.name, func(t *testing.T) {
			f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{&Assign{Dst: slot, Src: tc.source}, tc.barrier, tc.use}}}}
			(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
			if len(f.Blocks[0].Effects) != 3 {
				t.Fatal("saved value was removed")
			}
			if _, ok := f.Blocks[0].Effects[0].(*Assign).Dst.(*Temp); !ok {
				t.Fatal("expected a captured temporary")
			}
			found := false
			walkEffect(f.Blocks[0].Effects[2], func(e Expr) {
				if _, ok := e.(*Temp); ok {
					found = true
				}
			})
			if !found {
				t.Fatal("use did not refer to captured value")
			}
		})
	}
}

// TestScratchRecoveryPartialWrites proves overlapping stores cannot reuse a
// stale wide definition and that unmodified word projections can inline.
func TestScratchRecoveryPartialWrites(t *testing.T) {
	wide := scratchTestSlot(-8, typeinfo.U32)
	high := scratchTestSlot(-6, typeinfo.U16)
	f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{
		&Assign{Dst: wide, Src: &Const{TypeInfo: typeinfo.U32, U64: 0x11223344}},
		&Assign{Dst: high, Src: &Const{TypeInfo: typeinfo.U16, U64: 0x5566}},
		&Return{Value: wide},
	}}}}
	(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
	if len(f.Blocks[0].Effects) != 3 {
		t.Fatal("overlapping write was incorrectly elided")
	}
	part, ok := f.Blocks[0].Effects[1].(*Assign).Dst.(*Part)
	if !ok || part.ByteOff != 2 || part.Width != 2 {
		t.Fatalf("high lane = %#v", f.Blocks[0].Effects[1])
	}
	f = Func{Blocks: []Block{{ID: 1, Effects: []Effect{&Assign{Dst: wide, Src: &Const{TypeInfo: typeinfo.U32, U64: 0x11223344}}, &Return{Value: high}}}}}
	(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
	if got := f.Blocks[0].Effects[0].(*Return).Value.(*Const).U64; got != 0x1122 {
		t.Fatalf("high word = %x", got)
	}
}

// TestScratchRecoveryAddressableLocal retains storage identity and uses call
// signatures to infer the complete pointee, including previously unseen bytes.
func TestScratchRecoveryAddressableLocal(t *testing.T) {
	// RTBOF is a 16-byte header; turn is a uint16_t at +0x0a in structs.h.
	fx := testfixture.Stars(t)
	rect := fx.SDB.GetStruct("RTBOF")
	ptr := &typeinfo.Pointer{Elem: rect, Class: typeinfo.PtrNear}
	slot := scratchTestSlot(-20, typeinfo.U16)
	f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{
		&Assign{Dst: slot, Src: &Const{TypeInfo: typeinfo.U16, U64: 1}},
		&CallEffect{Call: &Call{Function: &typeinfo.Function{Name: "ReadHeader", Ret: typeinfo.U16, Params: []typeinfo.FunctionVar{{Name: "header", Type: ptr}}}, Args: []Expr{&AddressOf{Target: slot, TypeInfo: &typeinfo.Pointer{Elem: typeinfo.U16, Class: typeinfo.PtrNear}}}}},
		&Return{Value: scratchTestSlot(-10, typeinfo.U16)},
	}}}}
	(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
	if len(f.RecoveredLocals) != 1 || f.RecoveredLocals[0].Type != rect {
		t.Fatalf("recovered locals = %#v", f.RecoveredLocals)
	}
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "FOpenFile")
	projected, _ := (&resolveLateAddressesProcessor{ctx: ctx}).ProcessBlock(newResult(ctx.fs), f, f.Blocks[0])
	if got := FormatEffect(projected.Effects[2]); got != "return t_scratch_m14.turn" {
		t.Fatalf("projected header field = %s", got)
	}
	addr := f.Blocks[0].Effects[1].(*CallEffect).Call.Args[0].(*AddressOf)
	if _, ok := addr.Target.(*Temp); ok {
		t.Fatal("address taken temporary")
	}
	if part, ok := f.Blocks[0].Effects[2].(*Return).Value.(*Part); !ok || part.ByteOff != 10 {
		t.Fatal("lost field offset")
	}
}

// TestScratchRecoveryAdjacentSlotsStaySeparate requires evidence before merging
// neighbors, even when a generic word combiner could fold their constants.
func TestScratchRecoveryAdjacentSlotsStaySeparate(t *testing.T) {
	f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{
		&Assign{Dst: scratchTestSlot(-8, typeinfo.U16), Src: &Const{TypeInfo: typeinfo.U16, U64: 1}},
		&Assign{Dst: scratchTestSlot(-6, typeinfo.U16), Src: &Const{TypeInfo: typeinfo.U16, U64: 2}},
		&Return{Value: scratchTestSlot(-8, typeinfo.U16)},
	}}}}
	a := analyzeScratchStorage(&f)
	if recoverScratchWideDefinitions(&f, a) {
		t.Fatal("unrelated adjacent stores merged")
	}
}

// TestScratchRecoveryF80 preserves the extended-real type on spills that must
// survive an ordering barrier, and removes simple extended staging.
func TestScratchRecoveryF80(t *testing.T) {
	for _, barrier := range []bool{false, true} {
		source := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "real", Type: typeinfo.Double}}
		slot := scratchTestSlot(-16, typeinfo.F80)
		effects := []Effect{&Assign{Dst: slot, Src: source}}
		if barrier {
			effects = append(effects, &CallEffect{Call: &Call{Function: &typeinfo.Function{Name: "UpdateReal", Ret: typeinfo.U16}}})
		}
		effects = append(effects, &Return{Value: &Cast{Value: slot, To: "double", TypeInfo: typeinfo.Double}})
		f := Func{Blocks: []Block{{ID: 1, Effects: effects}}}
		(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
		if barrier {
			tmp, ok := f.Blocks[0].Effects[0].(*Assign).Dst.(*Temp)
			if !ok || tmp.TypeInfo != typeinfo.F80 {
				t.Fatal("extended spill lost float type")
			}
		} else if len(f.Blocks[0].Effects) != 1 {
			t.Fatal("simple extended staging survived")
		}
	}
}

// TestScratchRecoveryFloatRounding retains the narrowing store conversion when
// x87 staging is inlined and the stored float is subsequently widened again.
func TestScratchRecoveryFloatRounding(t *testing.T) {
	slot := scratchTestSlot(-8, typeinfo.U32)
	f := Func{Blocks: []Block{{ID: 1, Effects: []Effect{
		&Assign{MetaInfo: machine.Meta{InstOp: asm.OpFSTP}, Dst: slot, Src: &FloatConst{TypeInfo: typeinfo.Double, F64: 1.00000001}},
		&Return{Value: &Cast{Value: slot, To: "double", TypeInfo: typeinfo.Double}},
	}}}}
	(&scratchRecoveryProcessor{}).ProcessFunc(nil, &f)
	if len(f.Blocks[0].Effects) != 1 {
		t.Fatal("simple floating-point staging was retained")
	}
	outer := f.Blocks[0].Effects[0].(*Return).Value.(*Cast)
	inner, ok := outer.Value.(*Cast)
	if !ok || inner.ExprType().Kind() != typeinfo.KFloat || inner.ExprType().Bytes() != 4 {
		t.Fatal("float store rounding was lost")
	}
}
