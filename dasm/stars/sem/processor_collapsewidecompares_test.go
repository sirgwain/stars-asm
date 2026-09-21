package sem

import (
	"reflect"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCollapseWideCompareOrdering verifies a signed-high/unsigned-low ladder
// becomes one wide predicate and that its scaffold is isolated in the CFG.
func TestCollapseWideCompareOrdering(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JL", 0x1100),
		jccForWideCompareTest(0x1002, "JG", 0x1006),
		jccForWideCompareTest(0x1004, "JB", 0x1100),
		retForWideCompareTest(0x1006),
		retForWideCompareTest(0x1100),
	})
	lhs := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9000}
	rhs := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9002}
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JL", machine.WordVal(lhs, machine.WordHigh), machine.WordVal(rhs, machine.WordHigh), 0x1100, 0x1002),
		compareBlock(0x1002, "JG", machine.WordVal(lhs, machine.WordHigh), machine.WordVal(rhs, machine.WordHigh), 0x1006, 0x1004),
		compareBlock(0x1004, "JB", machine.WordVal(lhs, machine.WordLow), machine.WordVal(rhs, machine.WordLow), 0x1100, 0x1006),
		{Block: 0x1006},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{}).ProcessMachineFunc(nil, &fn); !changed {
		t.Fatal("ProcessMachineFunc changed = false, want true")
	}
	branch := fn.Blocks[0].Effects[0].(machine.BranchEffect)
	if got, want := branch.Predicate.Op, "JL"; got != want {
		t.Fatalf("wide predicate op = %q, want %q", got, want)
	}
	if !machine.ValueEquals(branch.Predicate.LHS, lhs) || !machine.ValueEquals(branch.Predicate.RHS, rhs) {
		t.Fatalf("wide predicate = %s, want original wide operands", branch.Predicate)
	}
	if got, want := []machine.BlockID{branch.TrueBlock, branch.FalseBlock}, []machine.BlockID{0x1100, 0x1006}; !reflect.DeepEqual(got, want) {
		t.Fatalf("wide targets = %#v, want %#v", got, want)
	}
	if got, want := cfg.Successors(0x1000), []machine.BlockID{0x1006, 0x1100}; !reflect.DeepEqual(got, want) {
		t.Fatalf("root successors = %#v, want %#v", got, want)
	}
	for _, id := range []machine.BlockID{0x1002, 0x1004} {
		if len(cfg.Successors(id)) != 0 || len(cfg.Predecessors(id)) != 0 {
			t.Fatalf("scaffold %s remains connected: pred=%v succ=%v", id, cfg.Predecessors(id), cfg.Successors(id))
		}
	}
}

// TestCollapseWideCompareOrderingThroughJumpOutcome verifies a pure jump
// trampoline does not make one logical comparison result look like two targets.
func TestCollapseWideCompareOrderingThroughJumpOutcome(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JL", 0x1100),
		jccForWideCompareTest(0x1002, "JG", 0x100a),
		jccForWideCompareTest(0x1004, "JB", 0x1100),
		{Off: 0x1006, Len: 1, Op: asm.OpNOP, Mnemonic: "NOP"},
		{Off: 0x1007, Len: 3, Op: asm.OpJMP, Mnemonic: "JMP", Target: 0x100a},
		retForWideCompareTest(0x100a),
		retForWideCompareTest(0x1100),
	})
	lhs := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9000}
	rhs := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9002}
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JL", machine.WordVal(lhs, machine.WordHigh), machine.WordVal(rhs, machine.WordHigh), 0x1100, 0x1002),
		compareBlock(0x1002, "JG", machine.WordVal(lhs, machine.WordHigh), machine.WordVal(rhs, machine.WordHigh), 0x100a, 0x1004),
		compareBlock(0x1004, "JB", machine.WordVal(lhs, machine.WordLow), machine.WordVal(rhs, machine.WordLow), 0x1100, 0x1006),
		{Block: 0x1006, Effects: []machine.Effect{machine.JumpEffect{To: 0x100a}}},
		{Block: 0x100a},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{}).ProcessMachineFunc(nil, &fn); !changed {
		t.Fatal("ProcessMachineFunc changed = false, want jump outcome collapse")
	}
	branch := fn.Blocks[0].Effects[0].(machine.BranchEffect)
	if got, want := []machine.BlockID{branch.TrueBlock, branch.FalseBlock}, []machine.BlockID{0x1100, 0x100a}; !reflect.DeepEqual(got, want) {
		t.Fatalf("wide targets = %#v, want %#v", got, want)
	}
}

// TestCollapseWideCompareEquality verifies an inverted low/high inequality tree
// is proved by outcomes rather than by one fixed branch arrangement.
func TestCollapseWideCompareEquality(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JNE", 0x1100),
		jccForWideCompareTest(0x1002, "JNE", 0x1100),
		retForWideCompareTest(0x1004),
		retForWideCompareTest(0x1100),
	})
	value := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9000}
	zero := machine.ConstVal(0)
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JNE", machine.WordVal(value, machine.WordLow), zero, 0x1100, 0x1002),
		compareBlock(0x1002, "JNE", machine.WordVal(value, machine.WordHigh), zero, 0x1100, 0x1004),
		{Block: 0x1004},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{}).ProcessMachineFunc(nil, &fn); !changed {
		t.Fatal("ProcessMachineFunc changed = false, want true")
	}
	branch := fn.Blocks[0].Effects[0].(machine.BranchEffect)
	if got, want := branch.Predicate.Op, "JNE"; got != want {
		t.Fatalf("wide predicate op = %q, want %q", got, want)
	}
	if !machine.ValueEquals(branch.Predicate.LHS, value) {
		t.Fatalf("wide lhs = %s, want %s", branch.Predicate.LHS, value)
	}
	if constant, ok := branch.Predicate.RHS.(*machine.Const); !ok || constant.Val != 0 {
		t.Fatalf("wide rhs = %#v, want dword zero", branch.Predicate.RHS)
	}
}

// TestCollapseWideCompareRejectsFourByteAggregate verifies adjacent fields of
// a four-byte struct are not treated as the lanes of one scalar value.
func TestCollapseWideCompareRejectsFourByteAggregate(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JNE", 0x1100),
		jccForWideCompareTest(0x1002, "JNE", 0x1100),
		retForWideCompareTest(0x1004),
		retForWideCompareTest(0x1100),
	})
	point := &typeinfo.Struct{Name: "POINT", SKind: typeinfo.StructKindStruct, Size: 4}
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	fs := &typeinfo.Function{
		Name: "ComparePoints",
		Addr: typeinfo.Addr{Seg: 1, Off: 0x1000},
		Len:  0x200,
		Ret:  &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"},
		Vars: []typeinfo.FunctionVar{{Name: "pt", Type: point, BPOffset: -4}},
	}
	ctx := NewFuncContext(img, sdb, symresolve.NewResolver(img, sdb), fs)
	load := func(disp int) machine.Value {
		return machine.LoadVal(machine.MemoryAddress{
			Base:   machine.FrameBaseVal(),
			Disp:   disp,
			Width:  2,
			Origin: machine.Origin{InstOff: 0x1000, Role: machine.OperandSrc},
		})
	}
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JNE", load(-4), machine.ConstVal(0), 0x1100, 0x1002),
		compareBlock(0x1002, "JNE", load(-2), machine.ConstVal(0), 0x1100, 0x1004),
		{Block: 0x1004},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{ctx: ctx}).ProcessMachineFunc(nil, &fn); changed {
		t.Fatal("ProcessMachineFunc changed = true, want four-byte aggregate rejection")
	}
}

// TestCollapseWideCompareRejectsStructArrayElement verifies typed array
// projection does not turn two adjacent fields into one wide scalar.
func TestCollapseWideCompareRejectsStructArrayElement(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JNE", 0x1100),
		jccForWideCompareTest(0x1002, "JNE", 0x1100),
		retForWideCompareTest(0x1004),
		retForWideCompareTest(0x1100),
	})
	point := &typeinfo.Struct{Name: "POINT", SKind: typeinfo.StructKindStruct, Size: 4}
	point.Fields = []typeinfo.StructField{
		{Name: "x", Offset: 0, Size: 2, End: 2, Type: typeinfo.I16},
		{Name: "y", Offset: 2, Size: 2, End: 4, Type: typeinfo.I16},
	}
	point.FinalizeLayout()
	points := &typeinfo.Array{Elem: point, Count: 2}
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	fs := &typeinfo.Function{
		Name: "ComparePointArray",
		Addr: typeinfo.Addr{Seg: 1, Off: 0x1000},
		Len:  0x200,
		Ret:  &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"},
		Vars: []typeinfo.FunctionVar{{Name: "points", Type: points, BPOffset: -8}},
	}
	ctx := NewFuncContext(img, sdb, symresolve.NewResolver(img, sdb), fs)
	load := func(disp int) machine.Value {
		return machine.LoadVal(machine.MemoryAddress{
			Base:   machine.FrameBaseVal(),
			Disp:   disp,
			Width:  2,
			Origin: machine.Origin{InstOff: 0x1000, Role: machine.OperandSrc},
		})
	}
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JNE", load(-8), machine.ConstVal(0), 0x1100, 0x1002),
		compareBlock(0x1002, "JNE", load(-6), machine.ConstVal(0), 0x1100, 0x1004),
		{Block: 0x1004},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{ctx: ctx}).ProcessMachineFunc(nil, &fn); changed {
		t.Fatal("ProcessMachineFunc changed = true, want struct array element rejection")
	}
}

// TestCollapseWideCompareArrayElementReloads verifies a scalar array element
// remains eligible when the compiler reloads its high lane at a new origin.
func TestCollapseWideCompareArrayElementReloads(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JL", 0x1100),
		jccForWideCompareTest(0x1002, "JG", 0x1006),
		jccForWideCompareTest(0x1004, "JBE", 0x1100),
		retForWideCompareTest(0x1006),
		retForWideCompareTest(0x1100),
	})
	values := &typeinfo.Array{Elem: typeinfo.I32, Count: 4}
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	fs := &typeinfo.Function{
		Name: "CompareArrayElement",
		Addr: typeinfo.Addr{Seg: 1, Off: 0x1000},
		Len:  0x200,
		Ret:  &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"},
		Vars: []typeinfo.FunctionVar{{Name: "values", Type: values, BPOffset: -16}},
	}
	ctx := NewFuncContext(img, sdb, symresolve.NewResolver(img, sdb), fs)
	load := func(disp int, instOff uint32) machine.Value {
		return machine.LoadVal(machine.MemoryAddress{
			Base:   machine.FrameBaseVal(),
			Disp:   disp,
			Width:  2,
			Origin: machine.Origin{InstOff: instOff, Role: machine.OperandSrc},
		})
	}
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JL", load(-2, 0x1000), machine.ConstVal(0), 0x1100, 0x1002),
		compareBlock(0x1002, "JG", load(-2, 0x1002), machine.ConstVal(0), 0x1006, 0x1004),
		compareBlock(0x1004, "JBE", load(-4, 0x1004), machine.ConstVal(0), 0x1100, 0x1006),
		{Block: 0x1006},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{ctx: ctx}).ProcessMachineFunc(nil, &fn); !changed {
		t.Fatal("ProcessMachineFunc changed = false, want scalar array element collapse")
	}
	branch := fn.Blocks[0].Effects[0].(machine.BranchEffect)
	loadValue, ok := branch.Predicate.LHS.(*machine.Load)
	if !ok || loadValue.Addr.Width != 4 {
		t.Fatalf("wide lhs = %#v, want four-byte array element load", branch.Predicate.LHS)
	}
}

// TestCollapseWideCompareDynamicStructArrayField verifies a low/high equality
// ladder through a runtime struct-array index resolves to its scalar field.
func TestCollapseWideCompareDynamicStructArrayField(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "DoCyberAiTurn")
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JNE", 0x1100),
		jccForWideCompareTest(0x1002, "JNE", 0x1100),
		retForWideCompareTest(0x1004),
		retForWideCompareTest(0x1100),
	})

	rel := uint32(0x036c) - ctx.fs.Addr.Off
	j := frameLoad(ctx, rel, -0x5a, 2)
	indexed := machine.BinaryVal(machine.ValueOpMul, machine.ConstVal(0x93), j)
	base := machine.BinaryVal(machine.ValueOpAdd, machine.ConstVal(0x3f00), machine.WordVal(indexed, machine.WordLow))
	load := func(disp int, instOff uint32) machine.Value {
		return machine.LoadVal(machine.MemoryAddress{
			Seg:    machine.RegVal(asm.RegDS),
			Base:   base,
			Disp:   disp,
			Width:  2,
			Origin: machine.Origin{InstOff: instOff, Role: machine.OperandSrc},
		})
	}
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JNE", load(0x83, 0x1000), machine.ConstVal(0), 0x1100, 0x1002),
		compareBlock(0x1002, "JNE", load(0x85, 0x1002), machine.ConstVal(0), 0x1100, 0x1004),
		{Block: 0x1004},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{ctx: ctx}).ProcessMachineFunc(nil, &fn); !changed {
		t.Fatal("ProcessMachineFunc changed = false, want dynamic struct-array field collapse")
	}
	branch := fn.Blocks[0].Effects[0].(machine.BranchEffect)
	if got := FormatExpr((&machineConverter{ctx: ctx}).convertValue(branch.Predicate.LHS)); got != "rgshdef[j].cExist" {
		t.Fatalf("wide lhs = %s, want rgshdef[j].cExist", got)
	}
}

// TestPreserveCompareDomainCastsSignExtensionForUnsignedCompare verifies the
// signed result of CWD-style widening is explicitly cast for unsigned Jccs.
func TestPreserveCompareDomainCastsSignExtensionForUnsignedCompare(t *testing.T) {
	extended := machine.SignExtendVal(machine.ConstVal(1), 16, 32)
	lhs, _ := (&collapseWideComparesProcessor{}).preserveCompareDomain(extended, machine.ConstVal(0), compareUnsigned)
	cast, ok := lhs.(*machine.Cast)
	if !ok || cast.To != typeinfo.U32 {
		t.Fatalf("unsigned compare lhs = %#v, want uint32_t cast", lhs)
	}
}

// TestCollapseWideCompareRejectsSideEffectingScaffold verifies source-level
// work in an intermediate block prevents control-flow removal.
func TestCollapseWideCompareRejectsSideEffectingScaffold(t *testing.T) {
	cfg := cfgForWideCompareTest(t, []asm.DecodedInst{
		jccForWideCompareTest(0x1000, "JNE", 0x1100),
		jccForWideCompareTest(0x1002, "JNE", 0x1100),
		retForWideCompareTest(0x1004),
		retForWideCompareTest(0x1100),
	})
	value := &machine.CallResult{Type: typeinfo.I32, InstOff: 0x9000}
	fn := machine.FuncEffects{CFG: cfg, Blocks: []machine.BlockEffects{
		compareBlock(0x1000, "JNE", machine.WordVal(value, machine.WordLow), machine.ConstVal(0), 0x1100, 0x1002),
		{
			Block: 0x1002,
			Effects: []machine.Effect{
				machine.CallEffect{MetaInfo: machine.Meta{BlockID: 0x1002}},
				wideCompareBranch(0x1002, "JNE", machine.WordVal(value, machine.WordHigh), machine.ConstVal(0), 0x1100, 0x1004),
			},
		},
		{Block: 0x1004},
		{Block: 0x1100},
	}}

	if changed := (&collapseWideComparesProcessor{}).ProcessMachineFunc(nil, &fn); changed {
		t.Fatal("ProcessMachineFunc changed = true, want conservative rejection")
	}
}

// compareBlock builds a single-effect comparison block.
func compareBlock(id machine.BlockID, op string, lhs, rhs machine.Value, trueBlock, falseBlock machine.BlockID) machine.BlockEffects {
	return machine.BlockEffects{Block: id, Effects: []machine.Effect{wideCompareBranch(id, op, lhs, rhs, trueBlock, falseBlock)}}
}

// wideCompareBranch builds a machine comparison branch for processor tests.
func wideCompareBranch(id machine.BlockID, op string, lhs, rhs machine.Value, trueBlock, falseBlock machine.BlockID) machine.BranchEffect {
	return machine.BranchEffect{
		MetaInfo:   machine.Meta{BlockID: id},
		Predicate:  &machine.PredicateValue{Kind: machine.PredicateCompare, Op: op, LHS: lhs, RHS: rhs},
		TrueBlock:  trueBlock,
		FalseBlock: falseBlock,
	}
}

// cfgForWideCompareTest builds a real machine CFG for wide compare tests.
func cfgForWideCompareTest(t *testing.T, instrs []asm.DecodedInst) *machine.CFG {
	t.Helper()
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	fs := &typeinfo.Function{Ret: &typeinfo.Primitive{TypeKind: typeinfo.KVoid, Name: "void"}}
	ctx := machine.NewFuncContext(img, sdb, symresolve.NewResolver(img, sdb), fs)
	cfg, err := machine.BuildCFG(ctx, instrs, false, machine.CFGOptions{})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}
	return cfg
}

// jccForWideCompareTest builds a conditional jump instruction.
func jccForWideCompareTest(off uint32, mnemonic string, target uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 2, Op: asm.OpJcc, Mnemonic: mnemonic, Target: int32(target)}
}

// retForWideCompareTest builds a return instruction.
func retForWideCompareTest(off uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 1, Op: asm.OpRET, Mnemonic: "RET"}
}
