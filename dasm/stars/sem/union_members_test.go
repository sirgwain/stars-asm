package sem

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestBlockMemberSelectionsResolveIndexedWords verifies direct choices apply
// during initial lowering and remain block-local after discriminator flow.
func TestBlockMemberSelectionsResolveIndexedWords(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	ctx := mustFuncContext(t, fx, res, "FIsAiAttack")
	global := fx.SDB.GetGlobal("rglpshdef")
	player := &ArrayIndex{
		Base: &Global{GlobalVar: global}, Index: &Global{GlobalVar: fx.SDB.GetGlobal("idPlayer")},
		TypeInfo: global.Type.(*typeinfo.Array).Elem,
	}
	var index Expr
	for _, local := range ctx.fs.Vars {
		if local.Name == "i" {
			index = &Local{FunctionVar: local}
		}
	}
	if index == nil {
		t.Fatal("FIsAiAttack.i not found")
	}
	shdef := fx.SDB.GetStruct("SHDEF")
	element := &ArrayIndex{Base: player, Index: index, TypeInfo: shdef}
	path, ok := symbolPathForExpr(element)
	if !ok {
		t.Fatal("indexed SHDEF path did not resolve")
	}
	converter := &machineConverter{ctx: ctx}
	fn := Func{
		CFG: cfgForReturnSinkTest(t, []asm.DecodedInst{
			jmpForReturnSinkTest(0x4ad0, 0x4b05), retForReturnSinkTest(0x4b05),
		}),
		Blocks: []Block{
			{ID: 0x4ad0, Effects: []Effect{&Jump{To: 0x4b05}}},
			{ID: 0x4b05, Effects: []Effect{&Return{Value: testConst(1)}}},
		},
	}
	for _, phase := range []string{"initial", "after flow"} {
		t.Run(phase, func(t *testing.T) {
			if phase == "after flow" {
				(&unionContextProcessor{ctx: ctx}).ProcessFunc(newResult(ctx.fs), &fn)
			}
			for _, block := range []machine.BlockID{0x4ad0, 0x4af5, 0x4afa} {
				ctx.SetCurrentBlock(block)
				for _, access := range []struct {
					offset int
					want   string
				}{{0x89, "HIWORD(rglpshdef[idPlayer][i].lPower)"}, {0x87, "LOWORD(rglpshdef[idPlayer][i].lPower)"}} {
					value, ok := converter.consumeAddress(AddressExpr{Base: element, Offset: access.offset}, 2)
					if !ok || FormatExpr(value) != access.want {
						t.Fatalf("%s +%x = %v, %v; want %s", block, access.offset, value, ok, access.want)
					}
					field, remainder, ok := res.ResolveContainingFieldPathInContext(path, access.offset, ctx.unionContext())
					if !ok || field.String() != "rglpshdef[idPlayer][i].lPower" || remainder != access.offset-0x87 {
						t.Fatalf("symbolic field = %v +%x, %v", field, remainder, ok)
					}
				}
			}
			ctx.SetCurrentBlock(0x4b05)
			if _, _, ok := res.ResolveContainingFieldPathInContext(path, 0x89, ctx.unionContext()); ok {
				t.Fatal("member choice propagated into an unconfigured successor")
			}
		})
	}
	ctx.SetCurrentBlock(0x4ad0)
	if _, _, ok := res.ResolveContainingFieldPathInContext(path, 0x7b, ctx.unionContext()); ok {
		t.Fatal("power selection resolved the independent flags union")
	}
	// Arithmetic indexes cannot always become symbolic paths. Collection facts
	// must also work through semantic array projections in that case.
	element.Index = &Binary{TypeInfo: typeinfo.I16, Op: OpAdd, LHS: index, RHS: testConst(1)}
	value, ok := converter.consumeAddress(AddressExpr{Base: element, Offset: 0x89}, 2)
	if !ok {
		t.Fatal("arithmetic-index projection did not resolve")
	}
	if got := FormatExpr(value); got != "HIWORD(rglpshdef[idPlayer][(i + 0x1)].lPower)" {
		t.Fatalf("arithmetic-index projection = %s", got)
	}
	facts := fn.Analyze(ctx).BlockMemberFacts
	if len(facts) != 6 {
		t.Fatalf("exported member facts = %d, want 6", len(facts))
	}
	for _, fact := range facts {
		if fact.Func != ctx.fs.Name || fact.Root != "rglpshdef" || fact.Type != "SHDEF" || fact.Member != "lPower" || !fact.AllElements {
			t.Fatalf("exported member fact = %+v", fact)
		}
	}
}

// TestBlockMemberCallResults verifies both call representations resolve the
// selected member, while other callees and blocks retain ambiguous layouts.
func TestBlockMemberCallResults(t *testing.T) {
	fx := testfixture.Stars(t)
	ctx := mustFuncContext(t, fx, symresolve.NewResolver(fx.Image, fx.SDB), "FCanKillTok")
	callee := fx.SDB.GetFunction("LpshdefFromTok")
	shdef := fx.SDB.GetStruct("SHDEF")
	converter := &machineConverter{ctx: ctx}
	ctx.SetCurrentBlock(0x391e)
	for _, base := range []Expr{
		&CallResult{Function: callee, TypeInfo: callee.Ret, InstOff: 0x392d},
		&Call{Function: callee, Args: []Expr{&Local{FunctionVar: ctx.fs.Params[0]}}},
	} {
		value, ok := converter.consumeAddress(AddressExpr{Base: base, Offset: 0x87, Deref: true}, 4)
		if !ok {
			t.Fatal("call result member did not resolve")
		}
		field, ok := value.(*FieldAccess)
		if !ok || field.Field.Name != "lPower" {
			t.Fatalf("call result access = %s, want lPower field", FormatExpr(value))
		}
	}
	if _, ok := ctx.unionContext().CallResultMemberFor(fx.SDB.GetFunction("LComputePower"), shdef, 0x87); ok {
		t.Fatal("call result fact selected an unrelated callee")
	}
	ctx.SetCurrentBlock(0x3979)
	if _, ok := ctx.unionContext().CallResultMemberFor(callee, shdef, 0x87); ok {
		t.Fatal("call result fact escaped its configured block")
	}
	fn := Func{
		CFG:    cfgForReturnSinkTest(t, []asm.DecodedInst{retForReturnSinkTest(0x3979)}),
		Blocks: []Block{{ID: 0x3979, Effects: []Effect{&Return{Value: testConst(0)}}}},
	}
	facts := fn.Analyze(ctx).BlockMemberFacts
	if len(facts) != 1 || facts[0].CallResult != callee.Name || facts[0].Root != "" || facts[0].Member != "lPower" {
		t.Fatalf("call result fact export = %+v", facts)
	}
}
