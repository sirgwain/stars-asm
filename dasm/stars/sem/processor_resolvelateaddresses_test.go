package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
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

// TestResolveLateReconstructedOrderAddress verifies saved far-pointer byte
// offsets resolve to PLORD.rgord for both memory accesses and pointer arguments.
func TestResolveLateReconstructedOrderAddress(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "DeleteWpFar")
	lpfl := &Local{FunctionVar: ctx.fs.Params[0]}
	iDel := &Local{FunctionVar: ctx.fs.Params[1]}
	converter := machineConverter{ctx: ctx}
	lpplord, ok := converter.consumeAddress(AddressExpr{Base: lpfl, Offset: 0x64, Deref: true}, 4)
	if !ok {
		t.Fatal("FLEET.lpplord did not resolve")
	}
	high := &Word{Parent: lpplord, Part: machine.WordHigh}
	low := &Binary{TypeInfo: typeinfo.U16, Op: OpAdd,
		LHS: &Word{Parent: lpplord, Part: machine.WordLow}, RHS: &Const{TypeInfo: typeinfo.U16, U64: 4}}
	pointer, ok := collapseWideExprPair(high, low, nil)
	if !ok {
		t.Fatal("saved far-pointer lanes did not reconstruct")
	}
	if _, ok := pointer.(*PointerOffset); !ok {
		t.Fatalf("reconstructed pointer = %T, want byte offset", pointer)
	}
	stride := &Word{Part: machine.WordLow, Parent: &Binary{TypeInfo: typeinfo.U32, Op: OpMul,
		LHS: &Const{TypeInfo: typeinfo.U16, U64: 18}, RHS: iDel}}
	processor := &resolveLateAddressesProcessor{ctx: ctx}
	if got, changed := processor.rewriter().rewriteExpr(low); changed {
		t.Fatalf("offset lane became a whole pointer: %s", FormatExpr(got))
	}
	for _, tc := range []struct {
		name string
		expr Expr
		want string
	}{
		{"array address", pointer, "lpfl->lpplord->rgord"},
		{"element address", &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: pointer, RHS: stride}, "&lpfl->lpplord->rgord[iDel]"},
		{"x load", &Memory{Seg: &Word{Parent: pointer, Part: machine.WordHigh},
			Base:  &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Word{Parent: pointer, Part: machine.WordLow}, RHS: stride},
			Width: 2, TypeInfo: typeinfo.I16}, "lpfl->lpplord->rgord[iDel].pt.x"},
		{"y load", &Memory{Seg: &Word{Parent: pointer, Part: machine.WordHigh},
			Base: &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: &Word{Parent: pointer, Part: machine.WordLow}, RHS: stride},
			Disp: 2, Width: 2, TypeInfo: typeinfo.I16}, "lpfl->lpplord->rgord[iDel].pt.y"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			got, changed := processor.rewriter().rewriteExpr(tc.expr)
			if !changed || FormatExpr(got) != tc.want {
				t.Fatalf("resolved address = %s, changed %v, want %s", FormatExpr(got), changed, tc.want)
			}
			if address, ok := got.(*AddressOf); ok && !typeinfo.IsFarPointer(address.ExprType()) {
				t.Fatal("element address lost its far-pointer type")
			}
			if next, changed := processor.rewriter().rewriteExpr(got); changed {
				t.Fatalf("resolved address changed on a second pass: %s", FormatExpr(next))
			}
		})
	}
}

// TestConsumeAddressConsumesPointerPermission verifies normalization never
// follows a pointer-valued field after the original dereference is consumed.
func TestConsumeAddressConsumesPointerPermission(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "DeleteWpFar")
	lpfl := &Local{FunctionVar: ctx.fs.Params[0]}
	iDel := &Local{FunctionVar: ctx.fs.Params[1]}
	converter := machineConverter{ctx: ctx}
	lpplord, ok := converter.consumeAddress(AddressExpr{Base: lpfl, Offset: 0x64, Deref: true}, 4)
	if !ok {
		t.Fatal("FLEET.lpplord did not resolve")
	}
	fieldAddress := &PointerOffset{Pointer: lpfl, Offset: &Const{TypeInfo: typeinfo.U16, U64: 0x64}, TypeInfo: lpfl.ExprType()}
	for _, tc := range []struct {
		name string
		addr AddressExpr
		want string
	}{
		{"pointer storage", AddressExpr{Base: lpplord, Offset: 2}, "HIWORD(lpfl->lpplord)"},
		{"pointer field after crossing", AddressExpr{Base: fieldAddress, Offset: 2, Deref: true}, "HIWORD(lpfl->lpplord)"},
		{"address of pointer field", AddressExpr{Base: &AddressOf{Target: lpplord,
			TypeInfo: &typeinfo.Pointer{Elem: lpplord.ExprType(), Class: typeinfo.PtrFar}}, Offset: 2, Deref: true}, "HIWORD(lpfl->lpplord)"},
		{"loaded pointer field", AddressExpr{Base: lpplord, Offset: 4, Terms: []ScaledTerm{{Expr: iDel, Scale: 18}}, Deref: true}, "lpfl->lpplord->rgord[iDel].pt.x"},
		{"element steps", AddressExpr{Base: &AddressOf{Target: &ArrayIndex{Base: lpfl, Index: iDel, TypeInfo: lpfl.ExprType().(*typeinfo.Pointer).Elem}, TypeInfo: lpfl.ExprType()}, Offset: 0x66, Deref: true}, "HIWORD(lpfl[iDel].lpplord)"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			got, ok := converter.consumeAddress(tc.addr, 2)
			if !ok {
				t.Fatal("storage did not resolve")
			}
			if FormatExpr(got) != tc.want {
				t.Fatalf("resolved storage = %s, want %s", FormatExpr(got), tc.want)
			}
		})
	}
	if got, ok := converter.consumeAddress(AddressExpr{Base: fieldAddress, Offset: 2}, 2); ok {
		t.Fatalf("followed pointer arithmetic without a dereference: %s", FormatExpr(got))
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

// TestByteAddressTyping verifies byte displacements from DrawXferDlg and
// BtlDataGet: a union member selected by the parameter type, a heap header
// offset kept in bytes and cast to its destination, and a mistyped address
// that must not be stepped by its claimed element size.
func TestByteAddressTyping(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	xferCtx := mustFuncContext(t, fx, res, "DrawXferDlg")
	pxfer := &Global{GlobalVar: fx.SDB.GetGlobal("pxfer")}
	i := localNamed(t, xferCtx.fs, "i")
	element := &AddressOf{Target: &ArrayIndex{Base: pxfer, Index: i, TypeInfo: fx.SDB.GetStruct("XFER")}, TypeInfo: pxfer.ExprType()}
	call := &Call{Function: fx.SDB.GetFunction("DrawFleetCargoXferSide"), Args: []Expr{
		&Local{FunctionVar: typeinfo.FunctionVar{Name: "hdc", Type: typeinfo.U16}},
		&Local{FunctionVar: typeinfo.FunctionVar{Name: "prc", Type: &typeinfo.Pointer{Elem: fx.SDB.GetStruct("RECT")}}},
		&Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: element, RHS: &Const{TypeInfo: typeinfo.U16, U64: 4}},
		i,
	}}
	p := &resolveLateAddressesProcessor{ctx: xferCtx}
	effects, _ := p.byteAddressRewriter().rewriteEffects([]Effect{&CallEffect{Call: call}})
	if got := FormatExpr(effects[0].(*CallEffect).Call.Args[2]); got != "&pxfer[i].fl" {
		t.Fatalf("xfer fleet argument = %q, want &pxfer[i].fl", got)
	}

	btlCtx := mustFuncContext(t, fx, res, "BtlDataGet")
	lphb := localNamed(t, btlCtx.fs, "lphb")
	lpbd := localNamed(t, btlCtx.fs, "lpbd")
	header := &Assign{Dst: lpbd, Src: &PointerOffset{Pointer: lphb, Offset: &Const{TypeInfo: typeinfo.I16, U64: 0x12}, TypeInfo: lphb.ExprType()}}
	p = &resolveLateAddressesProcessor{ctx: btlCtx}
	effects, _ = p.byteAddressRewriter().rewriteEffects([]Effect{header})
	offset, ok := effects[0].(*Assign).Src.(*PointerOffset)
	if !ok || !typeinfo.Equals(offset.TypeInfo, lpbd.ExprType()) {
		t.Fatalf("heap header = %s, want a BTLDATA * byte offset", FormatExpr(effects[0].(*Assign).Src))
	}

	// rgshdef[raw byte index] claims int16_t elements; eight bytes must stay bytes.
	rgshdef := &Global{GlobalVar: fx.SDB.GetGlobal("rgshdef")}
	mistyped := &AddressOf{Target: &ArrayIndex{Base: rgshdef, Index: i, TypeInfo: typeinfo.I16}, TypeInfo: &typeinfo.Pointer{Elem: typeinfo.I16}}
	raw := &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: mistyped, RHS: &Const{TypeInfo: typeinfo.U16, U64: 8}}
	got, ok := p.typeByteAddress(raw, nil, nil)
	if _, bytes := got.(*PointerOffset); !ok || !bytes {
		t.Fatalf("mistyped address = %v, want a byte PointerOffset", got)
	}
}
