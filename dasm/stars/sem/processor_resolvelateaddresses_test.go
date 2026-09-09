package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestResolveLateDataAddressProjectsGlobalArrayField verifies a scratch-exposed
// DS address is projected through PLAYER.rgmdRelation from structs.h.
func TestResolveLateDataAddressProjectsGlobalArrayField(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "DrawScanner")
	idPlayer := fx.SDB.GetGlobal("idPlayer")
	if idPlayer == nil {
		t.Fatal("idPlayer global not found")
	}
	i := localNamed(t, ctx.fs, "i")
	base := &Binary{
		TypeInfo: typeinfo.U16,
		Op:       OpAdd,
		LHS: &Binary{
			TypeInfo: typeinfo.U16,
			Op:       OpAdd,
			LHS:      &Const{TypeInfo: typeinfo.U16, U64: 0x59a2},
			RHS: &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpMul,
				LHS:      &Global{GlobalVar: idPlayer},
				RHS:      &Const{TypeInfo: typeinfo.U16, U64: 0xc0},
			},
		},
		RHS: &Const{TypeInfo: typeinfo.U16, U64: 0x70},
	}
	base = &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: base, RHS: i}
	memory := &Memory{
		Seg:      &Register{Val: asm.RegDS, SegNum: ctx.segFromRegister(asm.RegDS)},
		Base:     base,
		Width:    1,
		TypeInfo: typeinfo.U8,
	}

	got, ok := (&resolveLateAddressesProcessor{ctx: ctx}).resolveMemory(memory)
	if !ok {
		t.Fatal("late DS address did not resolve")
	}
	if formatted := FormatExpr(got); formatted != "rgplr[idPlayer].rgmdRelation[i]" {
		t.Fatalf("resolved DS address = %q, want rgplr[idPlayer].rgmdRelation[i]", formatted)
	}
}

// TestResolveLateFarAddressPairsPointerWords verifies separate segment and
// offset lanes recover one far pointer before indexed byte projection.
func TestResolveLateFarAddressPairsPointerWords(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "DoRobotoidAiTurn")
	global := fx.SDB.GetGlobal("vlpbAiPlanet")
	if global == nil {
		t.Fatal("vlpbAiPlanet global not found")
	}
	pointer := &Global{GlobalVar: global}
	i := localNamed(t, ctx.fs, "i")
	index := &Binary{
		TypeInfo: typeinfo.U16,
		Op:       OpAdd,
		LHS: &Binary{
			TypeInfo: typeinfo.U16,
			Op:       OpMul,
			LHS:      i,
			RHS:      &Const{TypeInfo: typeinfo.U16, U64: 16},
		},
		RHS: &Const{TypeInfo: typeinfo.U16, U64: 10},
	}
	memory := &Memory{
		Seg:      &Part{Base: pointer, ByteOff: 2, Width: 2, TypeInfo: typeinfo.U16},
		Base:     &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Part{Base: pointer, Width: 2, TypeInfo: typeinfo.U16}, RHS: index},
		Width:    1,
		TypeInfo: typeinfo.U8,
	}

	got, ok := (&resolveLateAddressesProcessor{ctx: ctx}).resolveMemory(memory)
	if !ok {
		t.Fatal("late far address did not resolve")
	}
	if formatted := FormatExpr(got); formatted != "vlpbAiPlanet[((i * 16) + 0xa)]" {
		t.Fatalf("resolved far address = %q, want vlpbAiPlanet[((i * 16) + 0xa)]", formatted)
	}
}

// TestResolveLateAddressOfPointerPartUsesByteArithmetic verifies an address
// beyond a typed pointee is retained as byte pointer arithmetic.
func TestResolveLateAddressOfPointerPartUsesByteArithmetic(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "LpReAlloc")
	lphb := localNamed(t, ctx.fs, "lphb")
	address := &AddressOf{
		Target:   &Part{Base: lphb, ByteOff: 0x12, Width: 0},
		TypeInfo: ctx.fs.Params[0].Type,
	}

	got, ok := (&resolveLateAddressesProcessor{ctx: ctx}).resolveAddressOfPart(address)
	if !ok {
		t.Fatal("pointer part address did not resolve")
	}
	if formatted := FormatExpr(got); formatted != "ptroff(lphb, 18)" {
		t.Fatalf("resolved pointer part = %q, want ptroff(lphb, 18)", formatted)
	}
}

// localNamed returns a declared local from a fully initialized fixture function.
func localNamed(t *testing.T, fn *typeinfo.Function, name string) *Local {
	t.Helper()
	for i := range fn.Vars {
		if fn.Vars[i].Name == name {
			return &Local{FunctionVar: fn.Vars[i]}
		}
	}
	t.Fatalf("local %s not found", name)
	return nil
}
