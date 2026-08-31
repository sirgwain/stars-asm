package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestNormalizeCallArgsPreProcessorRebuildsDataSegmentVarArgFarPointers verifies vararg
// offset/DS word pairs are reconstructed as source far pointers.
func TestNormalizeCallArgsPreProcessorRebuildsDataSegmentVarArgFarPointers(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	wsprintf := &typeinfo.Function{
		Name:    "_wsprintf",
		Ret:     typeinfo.U16,
		VarArgs: true,
		Params: []typeinfo.FunctionVar{
			{Name: "lpszout", Type: farCharPtrType},
			{Name: "lpszfmt", Type: farCharPtrType},
		},
	}
	sz := machine.LoadVal(machine.MemoryAccess{Base: machine.FrameBaseVal(), Disp: 6, Width: 2})
	title := machine.ConstVal(0x1385)
	block := machine.BlockEffects{
		Effects: []machine.Effect{
			machine.CallEffect{
				Target: wsprintf,
				Args: []machine.Value{
					machine.FarPointerWordsVal(machine.ConstVal(0x2000), machine.RegVal(asm.RegSS)),
					machine.FarPointerWordsVal(machine.ConstVal(0x137b), machine.RegVal(asm.RegDS)),
					sz,
					machine.RegVal(asm.RegDS),
					machine.ConstVal(0x2),
					title,
					machine.RegVal(asm.RegDS),
				},
			},
		},
	}

	gotBlock, changed := (&normalizeCallArgsProcessor{ctx: &FuncContext{dsReg: machine.RegVal(asm.RegDS)}}).ProcessMachineBlock(nil, machine.FuncEffects{}, block)
	if !changed {
		t.Fatal("ProcessMachineBlock changed = false, want true")
	}
	if len(gotBlock.Effects) != 1 {
		t.Fatalf("effects = %#v, want one call", gotBlock.Effects)
	}
	got := gotBlock.Effects[0].(machine.CallEffect)
	want := []string{
		"farptr(ss, 0x2000)",
		"farptr(ds, 0x137b)",
		"farptr(ds, load([bp+0x6]))",
		"0x2",
		"farptr(ds, 0x1385)",
	}
	if len(got.Args) != len(want) {
		t.Fatalf("args = %#v, want %d args", got.Args, len(want))
	}
	for i, wantArg := range want {
		if gotArg := got.Args[i].String(); gotArg != wantArg {
			t.Fatalf("arg %d = %q, want %q", i, gotArg, wantArg)
		}
	}
}

// TestNormalizeCallArgsProcessorRebuildsSemanticVarArgFarPointers verifies
// lowered vararg word pairs still resolve to source-level far pointers.
func TestNormalizeCallArgsProcessorRebuildsSemanticVarArgFarPointers(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	wsprintf := &typeinfo.Function{
		Name:    "_wsprintf",
		Ret:     typeinfo.U16,
		VarArgs: true,
		Params: []typeinfo.FunctionVar{
			{Name: "lpszout", Type: farCharPtrType},
			{Name: "lpszfmt", Type: farCharPtrType},
		},
	}
	sdb := &typeinfo.SymbolDB{Globals: []*typeinfo.GlobalVar{
		{Name: "szOne", Addr: typeinfo.Addr{Seg: 0x25, Off: 0xc85}, Type: &typeinfo.Array{Elem: charType, Count: 8}},
		{Name: "szTwo", Addr: typeinfo.Addr{Seg: 0x25, Off: 0xc86}, Type: &typeinfo.Array{Elem: charType, Count: 8}},
	}}
	ctx := &FuncContext{dsReg: machine.RegVal(asm.RegDS), res: symresolve.NewResolver(&asm.ImageNE{}, sdb)}
	block := Block{Effects: []Effect{
		&CallEffect{
			Call: &Call{Function: wsprintf, Args: []Expr{
				&Global{GlobalVar: &typeinfo.GlobalVar{Name: "szWork", Type: farCharPtrType}},
				&Global{GlobalVar: &typeinfo.GlobalVar{Name: "pszFmt", Type: farCharPtrType}},
				&Merge{
					TypeInfo: typeinfo.U16,
					Join:     0x20,
					Arms: []MergeArm{
						{Block: 0x10, Value: &Const{TypeInfo: typeinfo.U16, U64: 0xc85}},
						{Block: 0x11, Value: &Const{TypeInfo: typeinfo.U16, U64: 0xc86}},
					},
				},
				&Register{Val: asm.RegDS, SegNum: 0x25},
			}},
		},
	}}

	gotBlock, changed := (&normalizeCallArgsProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "call _wsprintf(szWork, pszFmt, merge(Join: L_0020, (L_0010:&szOne, L_0011:&szTwo)))"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}

// TestNormalizeCallArgsProcessorLeavesTrailingScalarVarArgs verifies scalar
// words after an explicit DS pointer are not guessed to be another far pointer.
func TestNormalizeCallArgsProcessorLeavesTrailingScalarVarArgs(t *testing.T) {
	charType := &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "char", Size: 1, Signed: true}
	farCharPtrType := &typeinfo.Pointer{Elem: charType, Class: typeinfo.PtrFar}
	wsprintf := &typeinfo.Function{
		Name:    "_wsprintf",
		Ret:     typeinfo.U16,
		VarArgs: true,
		Params: []typeinfo.FunctionVar{
			{Name: "lpszout", Type: farCharPtrType},
			{Name: "lpszfmt", Type: farCharPtrType},
		},
	}
	playerName := &CallResult{
		Function: &typeinfo.Function{Name: "PszPlayerName", Ret: farCharPtrType},
		TypeInfo: farCharPtrType,
		InstOff:  0x6665,
	}
	ctx := &FuncContext{dsReg: machine.RegVal(asm.RegDS), res: symresolve.NewResolver(&asm.ImageNE{}, &typeinfo.SymbolDB{})}
	block := Block{Effects: []Effect{
		&CallEffect{
			Call: &Call{Function: wsprintf, Args: []Expr{
				&Global{GlobalVar: &typeinfo.GlobalVar{Name: "lpb2k", Type: farCharPtrType}},
				&Local{FunctionVar: typeinfo.FunctionVar{Name: "szT", Type: &typeinfo.Array{Elem: charType, Count: 32}, BPOffset: -0x6a}},
				playerName,
				&Register{Val: asm.RegDS, SegNum: 0x25},
				&Const{TypeInfo: typeinfo.U16, U64: 0xd},
				&Const{TypeInfo: typeinfo.U16, U64: 0xa},
			}},
		},
	}}

	gotBlock, changed := (&normalizeCallArgsProcessor{ctx: ctx}).ProcessBlock(nil, Func{}, block)
	if !changed {
		t.Fatal("ProcessBlock changed = false, want true")
	}
	got := FormatEffect(gotBlock.Effects[0])
	want := "call _wsprintf(lpb2k, szT, callresult(char *), 0xd, 0xa)"
	if got != want {
		t.Fatalf("effect = %q, want %q", got, want)
	}
}
