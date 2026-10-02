package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// writesAnything reports that every call may store to any memory.
func writesAnything(*typeinfo.Function) machine.Writes {
	return machine.Writes{Any: true}
}

// TestForwardTempsProcessor verifies which temps are forwarded into their
// use and which keep the conversion or evaluation order they provide.
func TestForwardTempsProcessor(t *testing.T) {
	flag := testLocal("f", typeinfo.I16)
	count := testLocal("c", typeinfo.I16)
	sum := testLocal("sum", typeinfo.I16)
	id := testLocal("id", typeinfo.I16)
	lppl := testLocal("lppl", &typeinfo.Pointer{Elem: typeinfo.I16, Class: typeinfo.PtrFar})
	scratch := &Temp{Name: "t_scratch_m10", TypeInfo: typeinfo.U16}
	merge := &Temp{Name: "t_merge_1000_0001", TypeInfo: typeinfo.I16}
	result := &Temp{Name: "t_call_1000", TypeInfo: typeinfo.I16}
	global := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "gCount", Type: typeinfo.I16}}
	program := &typeinfo.Function{Name: "CountThings", Ret: typeinfo.I16}
	other := &typeinfo.Function{Name: "Touch", Ret: typeinfo.I16}
	caller := &typeinfo.Function{Name: "Caller", Ret: typeinfo.I16}
	signed := func(v int16) *Const { return &Const{TypeInfo: typeinfo.I16, U64: uint64(uint16(v))} }
	isZero := &Compare{Op: CompareEQ, LHS: flag, RHS: signed(0)}
	callOf := func(fn *typeinfo.Function, args ...Expr) *Call {
		return &Call{Function: fn, Target: &FunctionRef{Function: fn}, Args: args}
	}

	for _, tc := range []struct {
		name    string
		effects []Effect
		want    []string
	}{
		{
			name: "ternary forwarded into its use",
			effects: []Effect{
				&Assign{Dst: merge, Src: &Cond{TypeInfo: typeinfo.I16, Cond: isZero, Then: signed(0), Else: signed(24)}},
				&Assign{Dst: sum, Src: &Binary{TypeInfo: typeinfo.I16, Op: OpAdd, LHS: merge, RHS: count}},
			},
			want: []string{"sum = ((f == 0 ? 0 : 24) + c)"},
		},
		{
			name: "narrowing conversion kept",
			effects: []Effect{
				&Assign{Dst: scratch, Src: count},
				&Branch{Cond: &Compare{Op: CompareLT, LHS: scratch, RHS: sum}},
			},
			want: []string{"t_scratch_m10 = c", "branch t_scratch_m10 < sum ? L_0000 : L_0000"},
		},
		{
			name: "call forwarded ahead of isolated reads",
			effects: []Effect{
				&CallEffect{Call: callOf(program, id), Result: result},
				&Assign{Dst: sum, Src: &Binary{TypeInfo: typeinfo.I16, Op: OpSub, LHS: result, RHS: count}},
			},
			want: []string{"sum = (CountThings(id) - c)"},
		},
		{
			name: "call kept ahead of a global read",
			effects: []Effect{
				&CallEffect{Call: callOf(program, id), Result: result},
				&Assign{Dst: sum, Src: &Binary{TypeInfo: typeinfo.I16, Op: OpSub, LHS: global, RHS: result}},
			},
			want: []string{"call CountThings(id) -> t_call_1000", "sum = (gCount - t_call_1000)"},
		},
		{
			name: "call converted on assignment forwarded",
			effects: []Effect{
				&CallEffect{Call: callOf(program, id), Result: scratch},
				&Assign{Dst: sum, Src: scratch},
			},
			want: []string{"sum = CountThings(id)"},
		},
		{
			name: "masked value that fits forwarded",
			effects: []Effect{
				&Assign{Dst: scratch, Src: &Binary{TypeInfo: typeinfo.U16, Op: OpAnd, LHS: count, RHS: &Const{TypeInfo: typeinfo.U16, U64: 0xf}}},
				&Branch{Cond: &Compare{Op: CompareLT, LHS: scratch, RHS: sum}},
			},
			want: []string{"branch (c & 0xf) < sum ? L_0000 : L_0000"},
		},
		{
			name: "value forwarded past an unrelated store",
			effects: []Effect{
				&Assign{Dst: merge, Src: count},
				&Assign{Dst: id, Src: signed(5)},
				&Branch{Cond: &Compare{Op: CompareLT, LHS: merge, RHS: sum}},
			},
			want: []string{"id = 5", "branch c < sum ? L_0000 : L_0000"},
		},
		{
			name: "value kept ahead of a store to what it reads",
			effects: []Effect{
				&Assign{Dst: merge, Src: count},
				&Assign{Dst: count, Src: signed(5)},
				&Branch{Cond: &Compare{Op: CompareLT, LHS: merge, RHS: sum}},
			},
			want: []string{"t_merge_1000_0001 = c", "c = 5", "branch t_merge_1000_0001 < sum ? L_0000 : L_0000"},
		},
		{
			name: "call kept ahead of another call",
			effects: []Effect{
				&CallEffect{Call: callOf(program, id), Result: result},
				&CallEffect{Call: callOf(other, result, callOf(program, count))},
			},
			want: []string{"call CountThings(id) -> t_call_1000", "call Touch(t_call_1000, CountThings(c))"},
		},
		{
			name: "call kept out of a conditional arm",
			effects: []Effect{
				&CallEffect{Call: callOf(program, id), Result: result},
				&Assign{Dst: sum, Src: &Cond{TypeInfo: typeinfo.I16, Cond: isZero, Then: result, Else: count}},
			},
			want: []string{"call CountThings(id) -> t_call_1000", "sum = (f == 0 ? t_call_1000 : c)"},
		},
		{
			name: "copied call result assigned to its copy",
			effects: []Effect{
				&CallEffect{Call: callOf(program, id), Result: result},
				&Assign{Dst: count, Src: result},
				&Branch{Cond: &Compare{Op: CompareNE, LHS: result, RHS: signed(-1)}},
			},
			want: []string{"call CountThings(id) -> c", "branch c != -1 ? L_0000 : L_0000"},
		},
		{
			name: "copy kept when its target changes before the last read",
			effects: []Effect{
				&CallEffect{Call: callOf(program, id), Result: result},
				&Assign{Dst: count, Src: result},
				&Assign{Dst: count, Src: signed(0)},
				&Branch{Cond: &Compare{Op: CompareNE, LHS: result, RHS: signed(-1)}},
			},
			want: []string{"call CountThings(id) -> t_call_1000", "c = t_call_1000", "c = 0", "branch t_call_1000 != -1 ? L_0000 : L_0000"},
		},
		{
			name: "unread temps dropped, keeping calls",
			effects: []Effect{
				&Assign{Dst: merge, Src: count},
				&CallEffect{Call: callOf(program, id), Result: result},
			},
			want: []string{"call CountThings(id)"},
		},
		{
			name: "pointer ternary forwarded",
			effects: []Effect{
				&Assign{Dst: &Temp{Name: "t_merge_2000_0001", TypeInfo: lppl.Type}, Src: &Cond{TypeInfo: lppl.Type, Cond: isZero, Then: &Const{TypeInfo: lppl.Type}, Else: lppl}},
				&CallEffect{Call: callOf(other, &Temp{Name: "t_merge_2000_0001", TypeInfo: lppl.Type})},
			},
			want: []string{"call Touch((f == 0 ? NULL : lppl))"},
		},
	} {
		t.Run(tc.name, func(t *testing.T) {
			fn := &Func{
				CFG:    cfgForReturnSinkTest(t, []asm.DecodedInst{retForReturnSinkTest(0x1000)}),
				Blocks: []Block{{ID: 0x1000, Effects: tc.effects}},
			}
			(&forwardTempsProcessor{fs: caller, writes: writesAnything}).ProcessFunc(nil, fn)
			if got := formatEffects(fn.Blocks[0].Effects); !equalStrings(got, tc.want) {
				t.Fatalf("effects = %#v, want %#v", got, tc.want)
			}
		})
	}
}

// TestForwardTempsUsesCallWrites verifies that a call moves ahead of a
// global read only when what it may store to leaves that global alone.
func TestForwardTempsUsesCallWrites(t *testing.T) {
	sum := testLocal("sum", typeinfo.I16)
	id := testLocal("id", typeinfo.I16)
	result := &Temp{Name: "t_call_1000", TypeInfo: typeinfo.I16}
	global := &Global{GlobalVar: &typeinfo.GlobalVar{Name: "gCount", Type: typeinfo.I16}}
	other := &typeinfo.GlobalVar{Name: "gSeed", Type: typeinfo.I32}
	program := &typeinfo.Function{Name: "CountThings", Ret: typeinfo.I16}
	caller := &typeinfo.Function{Name: "Caller", Ret: typeinfo.I16}

	for _, tc := range []struct {
		name   string
		stores machine.Writes
		want   []string
	}{
		{
			name:   "call storing elsewhere forwarded",
			stores: machine.Writes{Globals: []*typeinfo.GlobalVar{other}},
			want:   []string{"sum = (gCount - CountThings(id))"},
		},
		{
			name:   "call storing to the global kept",
			stores: machine.Writes{Globals: []*typeinfo.GlobalVar{global.GlobalVar}},
			want:   []string{"call CountThings(id) -> t_call_1000", "sum = (gCount - t_call_1000)"},
		},
	} {
		t.Run(tc.name, func(t *testing.T) {
			fn := &Func{
				CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{retForReturnSinkTest(0x1000)}),
				Blocks: []Block{{ID: 0x1000, Effects: []Effect{
					&CallEffect{Call: &Call{Function: program, Target: &FunctionRef{Function: program}, Args: []Expr{id}}, Result: result},
					&Assign{Dst: sum, Src: &Binary{TypeInfo: typeinfo.I16, Op: OpSub, LHS: global, RHS: result}},
				}}},
			}
			writes := func(*typeinfo.Function) machine.Writes { return tc.stores }
			(&forwardTempsProcessor{fs: caller, writes: writes}).ProcessFunc(nil, fn)
			if got := formatEffects(fn.Blocks[0].Effects); !equalStrings(got, tc.want) {
				t.Fatalf("effects = %#v, want %#v", got, tc.want)
			}
		})
	}
}

// TestForwardTempsSinksMergeCopies verifies that a merge temp whose only read
// copies it into a local is replaced by stores to that local on each edge,
// and that a copy converting to another type is kept.
func TestForwardTempsSinksMergeCopies(t *testing.T) {
	merge := &Temp{Name: "t_merge_1006_0001", TypeInfo: typeinfo.I16}
	caller := &typeinfo.Function{Name: "Caller", Ret: typeinfo.I16}
	signed := func(v int16) *Const { return &Const{TypeInfo: typeinfo.I16, U64: uint64(uint16(v))} }

	for _, tc := range []struct {
		name      string
		dst       *Local
		thenValue Expr
		want      [3][]string
	}{
		{
			name:      "stores on each edge",
			dst:       testLocal("v", typeinfo.I16),
			thenValue: signed(1),
			want:      [3][]string{{"v = 1", "goto L_1006"}, {"v = 0", "goto L_1006"}, {"return v"}},
		},
		{
			name:      "store of the local itself dropped",
			dst:       testLocal("v", typeinfo.I16),
			thenValue: testLocal("v", typeinfo.I16),
			want:      [3][]string{{"goto L_1006"}, {"v = 0", "goto L_1006"}, {"return v"}},
		},
		{
			name:      "conversion kept",
			dst:       testLocal("v", typeinfo.U16),
			thenValue: signed(1),
			want:      [3][]string{{"t_merge_1006_0001 = 1", "goto L_1006"}, {"t_merge_1006_0001 = 0", "goto L_1006"}, {"v = t_merge_1006_0001", "return v"}},
		},
	} {
		t.Run(tc.name, func(t *testing.T) {
			fn := &Func{
				CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
					jccForReturnSinkTest(0x1000, 0x1004),
					jmpForReturnSinkTest(0x1002, 0x1006),
					jmpForReturnSinkTest(0x1004, 0x1006),
					retForReturnSinkTest(0x1006),
				}),
				Blocks: []Block{
					{ID: 0x1000, Effects: []Effect{&Branch{TrueBlock: 0x1004, FalseBlock: 0x1002}}},
					{ID: 0x1002, Effects: []Effect{&Assign{Dst: merge, Src: tc.thenValue}, &Jump{To: 0x1006}}},
					{ID: 0x1004, Effects: []Effect{&Assign{Dst: merge, Src: signed(0)}, &Jump{To: 0x1006}}},
					{ID: 0x1006, Effects: []Effect{&Assign{Dst: tc.dst, Src: merge}, &Return{Value: tc.dst}}},
				},
			}
			(&forwardTempsProcessor{fs: caller, writes: writesAnything}).ProcessFunc(nil, fn)
			for i, want := range tc.want {
				if got := formatEffects(fn.Blocks[i+1].Effects); !equalStrings(got, want) {
					t.Fatalf("block %s effects = %#v, want %#v", fn.Blocks[i+1].ID, got, want)
				}
			}
		})
	}
}
