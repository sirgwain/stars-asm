package machine

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestExtractPhisRegisterAtDiamondJoin(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		jccInstForExtractTest(0x1000, "JNZ", 0x1006),
		movRegImmInstForExtractTest(0x1002, asm.RegAX, 1),
		jmpInstForExtractTest(0x1004, 0x1008),
		movRegImmInstForExtractTest(0x1006, asm.RegAX, 2),
		retInstForExtractTest(0x1008),
	})

	effects := Extract(ctx, cfg, ExtractOptions{})
	ret := returnEffectForExtractTest(t, effects, 0x1008)
	merge, ok := ret.Value.(*PhiValue)
	if !ok {
		t.Fatalf("return value = %T %[1]s, want PhiValue", ret.Value)
	}
	if got, want := len(merge.Arms), 2; got != want {
		t.Fatalf("phi arms = %d, want %d", got, want)
	}
	if got, want := merge.Arms[0].Block.ID, BlockID(0x1002); got != want {
		t.Fatalf("first phi arm block = %s, want %s", got, want)
	}
	if got, want := merge.Arms[0].Value.String(), "0x1"; got != want {
		t.Fatalf("first phi arm value = %s, want %s", got, want)
	}
	if got, want := merge.Arms[1].Block.ID, BlockID(0x1006); got != want {
		t.Fatalf("second phi arm block = %s, want %s", got, want)
	}
	if got, want := merge.Arms[1].Value.String(), "0x2"; got != want {
		t.Fatalf("second phi arm value = %s, want %s", got, want)
	}
	if got, want := merge.String(), "merge(Join: L_1008, (L_1002:0x1, L_1006:0x2))"; got != want {
		t.Fatalf("phi string = %q, want %q", got, want)
	}
}

func TestExtractLoopHeaderReachesWidenedFixpoint(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		movRegImmInstForExtractTest(0x1000, asm.RegAX, 0),
		jmpInstForExtractTest(0x1002, 0x1006),
		addRegImmInstForExtractTest(0x1004, asm.RegAX, 1),
		cmpRegImmInstForExtractTest(0x1006, asm.RegAX, 3),
		jccInstForExtractTest(0x1008, "JL", 0x1004),
		retInstForExtractTest(0x100a),
	})

	effects := Extract(ctx, cfg, ExtractOptions{})
	header := blockEffectsForExtractTest(t, effects, 0x1006)
	ax := header.Entry.regs[asm.RegAX]
	merge, ok := ax.(*PhiValue)
	if !ok {
		t.Fatalf("loop header AX = %T %[1]s, want PhiValue", ax)
	}

	gotLoopUnknown := false
	for _, arm := range merge.Arms {
		if isLoopUnknown(arm.Value) {
			gotLoopUnknown = true
		}
	}
	if !gotLoopUnknown {
		t.Fatalf("loop header AX = %s, want widened loop arm", ax)
	}
	if got, want := merge.Join, BlockID(0x1006); got != want {
		t.Fatalf("loop header AX phi join = %s, want %s", got, want)
	}
	loopPreds := cfg.loopPredsByBlock
	if !loopPreds[0x1006][0x1004] {
		t.Fatalf("loop predecessor map for L_1006 missing L_1004")
	}
	if loopPreds[0x1006][0x1002] {
		t.Fatalf("loop predecessor map for L_1006 included entry predecessor L_1002")
	}
}

func TestExtractPreservesDiamondPhiInsideLoop(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		movRegImmInstForExtractTest(0x1000, asm.RegCX, 0),
		jmpInstForExtractTest(0x1002, 0x1006),
		addRegImmInstForExtractTest(0x1004, asm.RegCX, 1),
		cmpRegImmInstForExtractTest(0x1006, asm.RegCX, 3),
		jccInstForExtractTest(0x1008, "JGE", 0x101c),
		cmpRegImmInstForExtractTest(0x100a, asm.RegCX, 1),
		jccInstForExtractTest(0x100c, "JNZ", 0x1014),
		movRegImmInstForExtractTest(0x100e, asm.RegAX, 1),
		jmpInstForExtractTest(0x1010, 0x1016),
		movRegImmInstForExtractTest(0x1014, asm.RegAX, 0),
		movMemRegInstForExtractTest(0x1016, 0x2000, asm.RegAX),
		jmpInstForExtractTest(0x1018, 0x1004),
		retInstForExtractTest(0x101c),
	})

	effects := Extract(ctx, cfg, ExtractOptions{})
	store := storeEffectForExtractTest(t, effects, 0x1016)
	merge, ok := store.Src.(*PhiValue)
	if !ok {
		t.Fatalf("stored value = %T %[1]s, want PhiValue", store.Src)
	}
	if containsLoopUnknown(merge, 0) {
		t.Fatalf("stored phi contains loop unknown: %s", merge)
	}
	if got, want := merge.String(), "merge(Join: L_1016, (L_100e:0x1, L_1014:0x0))"; got != want {
		t.Fatalf("stored phi = %q, want %q", got, want)
	}
}

func TestExtractPreservesPendingStackArgAcrossSinglePredBlocksInLoop(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	helper := &typeinfo.Function{
		Name: "TwoArgPascal",
		Conv: typeinfo.CCPascal,
		Ret:  typeinfo.U16,
		Params: []typeinfo.FunctionVar{
			{Type: typeinfo.U16},
			{Type: typeinfo.U16},
		},
	}
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		movRegImmInstForExtractTest(0x1000, asm.RegCX, 0),
		jmpInstForExtractTest(0x1002, 0x1006),
		addRegImmInstForExtractTest(0x1004, asm.RegCX, 1),
		cmpRegImmInstForExtractTest(0x1006, asm.RegCX, 3),
		jccInstForExtractTest(0x1008, "JGE", 0x1020),
		pushMemInstForExtractTest(0x100a, 0x2000),
		cmpRegImmInstForExtractTest(0x100c, asm.RegCX, 1),
		jccInstForExtractTest(0x100e, "JNZ", 0x1016),
		movRegImmInstForExtractTest(0x1010, asm.RegAX, 1),
		jmpInstForExtractTest(0x1012, 0x1018),
		movRegImmInstForExtractTest(0x1016, asm.RegAX, 2),
		pushRegInstForExtractTest(0x1018, asm.RegAX),
		{Off: 0x101a, Len: 2, Op: asm.OpCALLF, Mnemonic: "CALLF"},
		jmpInstForExtractTest(0x101c, 0x1004),
		retInstForExtractTest(0x1020),
	})
	cfg.Calls[0x101a] = &InstCall{Target: helper}

	effects := Extract(ctx, cfg, ExtractOptions{})
	call := callEffectForExtractTest(t, effects, 0x1018)
	if got, want := len(call.Args), 2; got != want {
		t.Fatalf("call args = %d, want %d", got, want)
	}
	if containsLoopUnknown(call.Args[0], 0) {
		t.Fatalf("pending arg contains loop unknown: %s", call.Args[0])
	}
	if got, want := call.Args[0].String(), "load(0x0:[0x2000])"; got != want {
		t.Fatalf("pending arg = %s, want %s", got, want)
	}
	if got, want := call.Args[1].String(), "merge(Join: L_1018, (L_1010:0x1, L_1016:0x2))"; got != want {
		t.Fatalf("selected arg = %s, want %s", got, want)
	}
}

func TestExtractKeepsRegisterCallArgsLiveAcrossBlockBoundary(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	helper := &typeinfo.Function{
		Name: "RegHelper",
		Conv: typeinfo.CCDxaxCx,
		Ret:  typeinfo.U16,
		Params: []typeinfo.FunctionVar{
			{Type: &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "uint32_t", Size: 4}},
			{Type: typeinfo.U16},
		},
	}
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		movRegImmInstForExtractTest(0x1000, asm.RegAX, 1),
		movRegImmInstForExtractTest(0x1002, asm.RegDX, 2),
		movRegImmInstForExtractTest(0x1004, asm.RegCX, 3),
		jmpInstForExtractTest(0x1006, 0x1008),
		{Off: 0x1008, Len: 5, Op: asm.OpCALLF, Mnemonic: "CALLF"},
		retInstForExtractTest(0x100d),
	})
	cfg.Calls[0x1008] = &InstCall{Target: helper}

	effects := Extract(ctx, cfg, ExtractOptions{})
	call := callEffectForExtractTest(t, effects, 0x1008)
	if got, want := len(call.Args), 2; got != want {
		t.Fatalf("call args = %d, want %d", got, want)
	}
	if got, want := call.Args[0].String(), "words(0x1, 0x2)"; got != want {
		t.Fatalf("first call arg = %s, want %s", got, want)
	}
	if got, want := call.Args[1].String(), "0x3"; got != want {
		t.Fatalf("second call arg = %s, want %s", got, want)
	}
}

func TestExtractUsesFarPointerForTypedRegisterCallArg(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	farPtr := &typeinfo.Pointer{Class: typeinfo.PtrFar, Elem: typeinfo.U16}
	helper := &typeinfo.Function{
		Name: "RegHelper",
		Conv: typeinfo.CCDxaxCx,
		Ret:  typeinfo.U16,
		Params: []typeinfo.FunctionVar{
			{Type: farPtr},
		},
	}
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		movRegImmInstForExtractTest(0x1000, asm.RegAX, 0x1234),
		movRegImmInstForExtractTest(0x1002, asm.RegDX, 0x5678),
		{Off: 0x1004, Len: 5, Op: asm.OpCALLF, Mnemonic: "CALLF"},
		retInstForExtractTest(0x1009),
	})
	cfg.Calls[0x1004] = &InstCall{Target: helper}

	effects := Extract(ctx, cfg, ExtractOptions{})
	call := callEffectForExtractTest(t, effects, 0x1000)
	if got, want := len(call.Args), 1; got != want {
		t.Fatalf("call args = %d, want %d", got, want)
	}
	if _, ok := call.Args[0].(*FarPointer); !ok {
		t.Fatalf("call arg = %T %[1]s, want FarPointer", call.Args[0])
	}
	if got, want := call.Args[0].String(), "farptr(0x5678, 0x1234)"; got != want {
		t.Fatalf("call arg = %s, want %s", got, want)
	}
}

// TestExtractUsesOffsetSegmentOrderForTypedStackFarPointer verifies stack far
// pointers are normalized from push order into source pointer word order.
func TestExtractUsesOffsetSegmentOrderForTypedStackFarPointer(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	farPtr := &typeinfo.Pointer{Class: typeinfo.PtrFar, Elem: typeinfo.U16}
	helper := &typeinfo.Function{
		Name: "StackHelper",
		Conv: typeinfo.CCCdecl,
		Ret:  typeinfo.U16,
		Params: []typeinfo.FunctionVar{
			{Type: farPtr},
		},
	}
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		movRegImmInstForExtractTest(0x1000, asm.RegDX, 0x25),
		pushRegInstForExtractTest(0x1002, asm.RegDX),
		movRegImmInstForExtractTest(0x1004, asm.RegAX, 0x57a4),
		pushRegInstForExtractTest(0x1006, asm.RegAX),
		{Off: 0x1008, Len: 5, Op: asm.OpCALLF, Mnemonic: "CALLF"},
		retInstForExtractTest(0x100d),
	})
	cfg.Calls[0x1008] = &InstCall{Target: helper}

	effects := Extract(ctx, cfg, ExtractOptions{})
	call := callEffectForExtractTest(t, effects, 0x1000)
	if got, want := len(call.Args), 1; got != want {
		t.Fatalf("call args = %d, want %d", got, want)
	}
	if _, ok := call.Args[0].(*FarPointer); !ok {
		t.Fatalf("call arg = %T %[1]s, want FarPointer", call.Args[0])
	}
	if got, want := call.Args[0].String(), "farptr(0x25, 0x57a4)"; got != want {
		t.Fatalf("call arg = %s, want %s", got, want)
	}
}

// TestExtractKeepsSSForStackLocalFarPointer verifies stack-local far pointers
// preserve SS as their segment word.
func TestExtractKeepsSSForStackLocalFarPointer(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	farPtr := &typeinfo.Pointer{Class: typeinfo.PtrFar, Elem: typeinfo.U16}
	helper := &typeinfo.Function{
		Name: "StackLocalHelper",
		Conv: typeinfo.CCCdecl,
		Ret:  typeinfo.U16,
		Params: []typeinfo.FunctionVar{
			{Type: farPtr},
		},
	}
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		{
			Off:      0x1000,
			Len:      3,
			Op:       asm.OpLEA,
			Mnemonic: "LEA",
			Dst:      asm.Operand{Kind: asm.OKReg, Reg: asm.RegAX},
			Src:      asm.Operand{Kind: asm.OKMem, Mem: asm.MemRef{Base: asm.RegBP, Disp: -0xa, MemSize: 2}},
		},
		{
			Off:      0x1003,
			Len:      2,
			Op:       asm.OpMOV,
			Mnemonic: "MOV",
			Dst:      asm.Operand{Kind: asm.OKReg, Reg: asm.RegDX},
			Src:      asm.Operand{Kind: asm.OKSReg, Reg: asm.RegSS},
		},
		pushRegInstForExtractTest(0x1005, asm.RegDX),
		pushRegInstForExtractTest(0x1006, asm.RegAX),
		{Off: 0x1007, Len: 5, Op: asm.OpCALLF, Mnemonic: "CALLF"},
		retInstForExtractTest(0x100c),
	})
	cfg.Calls[0x1007] = &InstCall{Target: helper}

	effects := Extract(ctx, cfg, ExtractOptions{})
	call := callEffectForExtractTest(t, effects, 0x1000)
	if got, want := len(call.Args), 1; got != want {
		t.Fatalf("call args = %d, want %d", got, want)
	}
	if got, want := call.Args[0].String(), "farptr(ss, addr([bp-0xa]))"; got != want {
		t.Fatalf("call arg = %s, want %s", got, want)
	}
}

func TestExtractWritesFarPointerReturnRegistersForTypedCallResult(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	farPtr := &typeinfo.Pointer{Class: typeinfo.PtrFar, Elem: typeinfo.U16}
	helper := &typeinfo.Function{Name: "FarRet", Ret: farPtr}
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		{Off: 0x1000, Len: 5, Op: asm.OpCALLF, Mnemonic: "CALLF"},
		jmpInstForExtractTest(0x1005, 0x1007),
		retInstForExtractTest(0x1007),
	})
	cfg.Calls[0x1000] = &InstCall{Target: helper}

	effects := Extract(ctx, cfg, ExtractOptions{})
	block := blockEffectsForExtractTest(t, effects, 0x1000)
	if _, ok := block.Exit.regs[asm.RegAX].(*FarPointer); !ok {
		t.Fatalf("AX after call = %T %[1]s, want FarPointer", block.Exit.regs[asm.RegAX])
	}
	if got, want := block.Exit.regs[asm.RegAX].String(), "faroff(callresult(uint16_t *))"; got != want {
		t.Fatalf("AX after call = %s, want %s", got, want)
	}
	if _, ok := block.Exit.regs[asm.RegDX].(*FarPointer); !ok {
		t.Fatalf("DX after call = %T %[1]s, want FarPointer", block.Exit.regs[asm.RegDX])
	}
	if got, want := block.Exit.regs[asm.RegDX].String(), "farseg(callresult(uint16_t *))"; got != want {
		t.Fatalf("DX after call = %s, want %s", got, want)
	}
}

func TestExtractUsesFarPointerForTypedReturnEffect(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	ctx.fs.Ret = &typeinfo.Pointer{Class: typeinfo.PtrFar, Elem: typeinfo.U16}
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		movRegImmInstForExtractTest(0x1000, asm.RegAX, 0x1234),
		movRegImmInstForExtractTest(0x1002, asm.RegDX, 0x5678),
		retInstForExtractTest(0x1004),
	})

	effects := Extract(ctx, cfg, ExtractOptions{})
	ret := returnEffectForExtractTest(t, effects, 0x1000)
	if _, ok := ret.Value.(*FarPointer); !ok {
		t.Fatalf("return value = %T %[1]s, want FarPointer", ret.Value)
	}
	if got, want := ret.Value.String(), "farptr(0x5678, 0x1234)"; got != want {
		t.Fatalf("return value = %s, want %s", got, want)
	}
}

func TestExtractSelfSubRegisterDoesNotRequireIncomingValue(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		movRegImmInstForExtractTest(0x1000, asm.RegDX, 7),
		jmpInstForExtractTest(0x1002, 0x1004),
		subRegRegInstForExtractTest(0x1004, asm.RegDX),
		movRegRegInstForExtractTest(0x1006, asm.RegAX, asm.RegDX),
		retInstForExtractTest(0x1008),
	})

	effects := Extract(ctx, cfg, ExtractOptions{})
	block := blockEffectsForExtractTest(t, effects, 0x1004)
	if _, ok := block.Entry.regs[asm.RegDX]; ok {
		t.Fatalf("DX should not be live-in to self-sub block")
	}
	ret := returnEffectForExtractTest(t, effects, 0x1004)
	if got, want := ret.Value.String(), "0x0"; got != want {
		t.Fatalf("return after self-sub copy = %s, want %s", got, want)
	}
}

func TestExtractPreservesLoadIdentityAcrossMemoryWrite(t *testing.T) {
	ctx := ctxForIntReturnExtractTest()
	cfg := cfgForExtractTest(t, ctx, []asm.DecodedInst{
		movRegMemInstForExtractTest(0x1000, asm.RegAX, 6),
		subMemImmInstForExtractTest(0x1003, 6, 1),
		cmpRegImmInstForExtractTest(0x1007, asm.RegAX, 0),
		jccInstForExtractTest(0x100a, "JNZ", 0x100d),
		retInstForExtractTest(0x100c),
		retInstForExtractTest(0x100d),
	})

	effects := Extract(ctx, cfg, ExtractOptions{})
	store := storeEffectForExtractTest(t, effects, 0x1000)
	storeSrc, ok := store.Src.(*Binary)
	if !ok {
		t.Fatalf("store source = %T %[1]s, want Binary", store.Src)
	}
	storeLoad, ok := storeSrc.LHS.(*Load)
	if !ok {
		t.Fatalf("store lhs = %T %[1]s, want Load", storeSrc.LHS)
	}
	branch := branchEffectForExtractTest(t, effects, 0x1000)
	branchLoad, ok := branch.Predicate.LHS.(*Load)
	if !ok {
		t.Fatalf("branch lhs = %T %[1]s, want Load", branch.Predicate.LHS)
	}
	if got, want := branchLoad.ID, (ValueID{InstOff: 0x1000, Role: OperandSrc}); got != want {
		t.Fatalf("branch load id = %s, want %s", got, want)
	}
	if got, want := storeLoad.ID, (ValueID{InstOff: 0x1003, Role: OperandDst}); got != want {
		t.Fatalf("store load id = %s, want %s", got, want)
	}
	if ValueEquals(branchLoad, storeLoad) {
		t.Fatalf("branch load and store load should be distinct values: %s vs %s", branchLoad, storeLoad)
	}
}

func TestCallResultValueKeyUsesCallSiteIdentity(t *testing.T) {
	fn := &typeinfo.Function{Name: "Foo", Ret: typeinfo.U16}
	a := &CallResult{Target: fn, Type: fn.Ret, InstOff: 0x1234}
	b := &CallResult{Target: fn, Type: fn.Ret, InstOff: 0x1234}

	if !ValueEquals(a, b) {
		t.Fatalf("same call-site results should compare equal: %s vs %s", valueKey(a), valueKey(b))
	}
}

// ctxForIntReturnExtractTest returns a function context whose RET reads AX.
func ctxForIntReturnExtractTest() *FuncContext {
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	fn := &typeinfo.Function{
		Ret: &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true},
	}
	return NewFuncContext(nil, sdb, symresolve.NewResolver(img, sdb), fn)
}

// cfgForExtractTest builds a CFG for extractor tests.
func cfgForExtractTest(t *testing.T, ctx *FuncContext, instrs []asm.DecodedInst) *CFG {
	t.Helper()
	cfg, err := BuildCFG(ctx, instrs, true, CFGOptions{})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}
	return cfg
}

// blockEffectsForExtractTest returns the effects record for one block.
func blockEffectsForExtractTest(t *testing.T, effects *FuncEffects, id BlockID) BlockEffects {
	t.Helper()
	for _, block := range effects.Blocks {
		if block.Block == id {
			return block
		}
	}
	t.Fatalf("missing block effects for %s", id)
	return BlockEffects{}
}

// returnEffectForExtractTest returns the first return effect in a block.
func returnEffectForExtractTest(t *testing.T, effects *FuncEffects, id BlockID) ReturnEffect {
	t.Helper()
	block := blockEffectsForExtractTest(t, effects, id)
	for _, effect := range block.Effects {
		if ret, ok := effect.(ReturnEffect); ok {
			return ret
		}
	}
	t.Fatalf("missing return effect for %s", id)
	return ReturnEffect{}
}

// storeEffectForExtractTest returns the first store effect in a block.
func storeEffectForExtractTest(t *testing.T, effects *FuncEffects, id BlockID) StoreEffect {
	t.Helper()
	block := blockEffectsForExtractTest(t, effects, id)
	for _, effect := range block.Effects {
		if store, ok := effect.(StoreEffect); ok {
			return store
		}
	}
	t.Fatalf("missing store effect for %s", id)
	return StoreEffect{}
}

// branchEffectForExtractTest returns the first branch effect in a block.
func branchEffectForExtractTest(t *testing.T, effects *FuncEffects, id BlockID) BranchEffect {
	t.Helper()
	block := blockEffectsForExtractTest(t, effects, id)
	for _, effect := range block.Effects {
		if branch, ok := effect.(BranchEffect); ok {
			return branch
		}
	}
	t.Fatalf("missing branch effect for %s", id)
	return BranchEffect{}
}

// callEffectForExtractTest returns the first call effect in a block.
func callEffectForExtractTest(t *testing.T, effects *FuncEffects, id BlockID) CallEffect {
	t.Helper()
	block := blockEffectsForExtractTest(t, effects, id)
	for _, effect := range block.Effects {
		if call, ok := effect.(CallEffect); ok {
			return call
		}
	}
	t.Fatalf("missing call effect for %s", id)
	return CallEffect{}
}

// movRegMemInstForExtractTest builds a MOV reg, [bp+disp] instruction.
func movRegMemInstForExtractTest(off uint32, reg asm.Reg, disp int) asm.DecodedInst {
	return asm.DecodedInst{
		Off:      off,
		Len:      3,
		Op:       asm.OpMOV,
		Mnemonic: "MOV",
		Dst:      asm.Operand{Kind: asm.OKReg, Reg: reg},
		Src:      asm.Operand{Kind: asm.OKMem, Mem: asm.MemRef{Base: asm.RegBP, Disp: disp, MemSize: 2}},
	}
}

// movRegImmInstForExtractTest builds a MOV reg, imm instruction.
func movRegImmInstForExtractTest(off uint32, reg asm.Reg, imm uint16) asm.DecodedInst {
	return asm.DecodedInst{
		Off:      off,
		Len:      2,
		Op:       asm.OpMOV,
		Mnemonic: "MOV",
		Dst:      asm.Operand{Kind: asm.OKReg, Reg: reg},
		Src:      asm.Operand{Kind: asm.OKImm, Imm: imm},
	}
}

// subMemImmInstForExtractTest builds a SUB [bp+disp], imm instruction.
func subMemImmInstForExtractTest(off uint32, disp int, imm uint16) asm.DecodedInst {
	return asm.DecodedInst{
		Off:      off,
		Len:      4,
		Op:       asm.OpSUB,
		Mnemonic: "SUB",
		Dst:      asm.Operand{Kind: asm.OKMem, Mem: asm.MemRef{Base: asm.RegBP, Disp: disp, MemSize: 2}},
		Src:      asm.Operand{Kind: asm.OKImm, Imm: imm},
	}
}

// movRegRegInstForExtractTest builds a MOV dst, src instruction.
func movRegRegInstForExtractTest(off uint32, dst, src asm.Reg) asm.DecodedInst {
	return asm.DecodedInst{
		Off:      off,
		Len:      2,
		Op:       asm.OpMOV,
		Mnemonic: "MOV",
		Dst:      asm.Operand{Kind: asm.OKReg, Reg: dst},
		Src:      asm.Operand{Kind: asm.OKReg, Reg: src},
	}
}

// movMemRegInstForExtractTest builds a MOV [disp], reg instruction.
func movMemRegInstForExtractTest(off uint32, disp int, src asm.Reg) asm.DecodedInst {
	return asm.DecodedInst{
		Off:      off,
		Len:      2,
		Op:       asm.OpMOV,
		Mnemonic: "MOV",
		Dst:      asm.Operand{Kind: asm.OKMem, Mem: asm.MemRef{Disp: disp, MemSize: 2}},
		Src:      asm.Operand{Kind: asm.OKReg, Reg: src},
	}
}

// pushMemInstForExtractTest builds a PUSH [disp] instruction.
func pushMemInstForExtractTest(off uint32, disp int) asm.DecodedInst {
	return asm.DecodedInst{
		Off:      off,
		Len:      2,
		Op:       asm.OpPUSH,
		Mnemonic: "PUSH",
		Src:      asm.Operand{Kind: asm.OKMem, Mem: asm.MemRef{Disp: disp, MemSize: 2}},
	}
}

// pushRegInstForExtractTest builds a PUSH reg instruction.
func pushRegInstForExtractTest(off uint32, reg asm.Reg) asm.DecodedInst {
	return asm.DecodedInst{
		Off:      off,
		Len:      2,
		Op:       asm.OpPUSH,
		Mnemonic: "PUSH",
		Src:      asm.Operand{Kind: asm.OKReg, Reg: reg},
	}
}

// addRegImmInstForExtractTest builds an ADD reg, imm instruction.
func addRegImmInstForExtractTest(off uint32, reg asm.Reg, imm uint16) asm.DecodedInst {
	return asm.DecodedInst{
		Off:      off,
		Len:      2,
		Op:       asm.OpADD,
		Mnemonic: "ADD",
		Dst:      asm.Operand{Kind: asm.OKReg, Reg: reg},
		Src:      asm.Operand{Kind: asm.OKImm, Imm: imm},
	}
}

// subRegRegInstForExtractTest builds a SUB reg, reg instruction.
func subRegRegInstForExtractTest(off uint32, reg asm.Reg) asm.DecodedInst {
	op := asm.Operand{Kind: asm.OKReg, Reg: reg}
	return asm.DecodedInst{Off: off, Len: 2, Op: asm.OpSUB, Mnemonic: "SUB", Dst: op, Src: op}
}

// cmpRegImmInstForExtractTest builds a CMP reg, imm instruction.
func cmpRegImmInstForExtractTest(off uint32, reg asm.Reg, imm uint16) asm.DecodedInst {
	return asm.DecodedInst{
		Off:      off,
		Len:      2,
		Op:       asm.OpCMP,
		Mnemonic: "CMP",
		Dst:      asm.Operand{Kind: asm.OKReg, Reg: reg},
		Src:      asm.Operand{Kind: asm.OKImm, Imm: imm},
	}
}

// jccInstForExtractTest builds a conditional jump instruction.
func jccInstForExtractTest(off uint32, mnemonic string, target uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 2, Op: asm.OpJcc, Mnemonic: mnemonic, Target: int32(target)}
}

// jmpInstForExtractTest builds an unconditional jump instruction.
func jmpInstForExtractTest(off uint32, target uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: int32(target)}
}

// retInstForExtractTest builds a RETF instruction.
func retInstForExtractTest(off uint32) asm.DecodedInst {
	return asm.DecodedInst{Off: off, Len: 1, Op: asm.OpRETF, Mnemonic: "RETF"}
}
