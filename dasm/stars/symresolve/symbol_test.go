package symresolve

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestSymbolScratchIdentity verifies scratch roots compare by storage identity,
// independently of their access-view types.
func TestSymbolScratchIdentity(t *testing.T) {
	addr := typeinfo.Addr{Seg: 1, Off: 2}
	low := &SymbolScratch{
		Function:    addr,
		BPOffset:    -0x136,
		StorageSize: 4,
		TypeInfo:    typeinfo.U16,
	}
	high := &SymbolScratch{
		Function:    addr,
		BPOffset:    -0x136,
		StorageSize: 4,
		TypeInfo:    typeinfo.U32,
	}
	other := &SymbolScratch{
		Function:    addr,
		BPOffset:    -0x134,
		StorageSize: 2,
		TypeInfo:    typeinfo.U16,
	}

	if low.String() != "scratch_bp_m136" {
		t.Fatalf("scratch name = %q, want scratch_bp_m136", low.String())
	}
	if !Equals(low, high) {
		t.Fatal("scratch roots with the same storage identity do not compare equal")
	}
	if Equals(low, other) {
		t.Fatal("scratch roots with different BP offsets compare equal")
	}
}

// TestSymbolBitfield verifies logical bitfield paths retain their declared
// type, render through value and pointer bases, and compare structurally.
func TestSymbolBitfield(t *testing.T) {
	field := &typeinfo.StructField{
		Name:   "flag",
		Type:   typeinfo.U16,
		Offset: 2,
		Bitfield: &typeinfo.Bitfield{
			BaseType:    typeinfo.U16,
			StorageSize: 2,
			BitOffset:   3,
			BitWidth:    1,
		},
	}
	aggregate := &typeinfo.Struct{Name: "Aggregate", Size: 4, Fields: []typeinfo.StructField{*field}}
	aggregate.FinalizeLayout()
	valueVar := &typeinfo.FunctionVar{Name: "value", Type: aggregate}
	pointerVar := &typeinfo.FunctionVar{Name: "pointer", Type: &typeinfo.Pointer{Elem: aggregate, Class: typeinfo.PtrFar}}

	valuePath := &SymbolBitfield{Base: &SymbolRoot{Symbol: valueVar}, Field: &aggregate.Fields[0]}
	equalPath := &SymbolBitfield{Base: &SymbolRoot{Symbol: valueVar}, Field: &aggregate.Fields[0]}
	pointerPath := &SymbolBitfield{Base: &SymbolRoot{Symbol: pointerVar}, Field: &aggregate.Fields[0]}
	otherField := aggregate.Fields[0]
	otherPath := &SymbolBitfield{Base: &SymbolRoot{Symbol: valueVar}, Field: &otherField}

	if got := valuePath.String(); got != "value.flag" {
		t.Fatalf("value bitfield = %q, want value.flag", got)
	}
	if got := pointerPath.String(); got != "pointer->flag" {
		t.Fatalf("pointer bitfield = %q, want pointer->flag", got)
	}
	if valuePath.Type() != typeinfo.U16 {
		t.Fatalf("bitfield type = %v, want uint16_t", valuePath.Type())
	}
	if !Equals(valuePath, equalPath) {
		t.Fatal("equivalent bitfield paths do not compare equal")
	}
	if Equals(valuePath, otherPath) {
		t.Fatal("bitfield paths with different field identities compare equal")
	}
}

// TestResolveFieldLoadExcludesBitfields verifies a raw storage load does not
// become a logical bitfield without a matching mask and shift.
func TestResolveFieldLoadExcludesBitfields(t *testing.T) {
	aggregate := &typeinfo.Struct{
		Name: "Aggregate",
		Size: 2,
		Fields: []typeinfo.StructField{
			{
				Name:   "flag",
				Type:   typeinfo.U16,
				Offset: 0,
				Bitfield: &typeinfo.Bitfield{
					BaseType:    typeinfo.U16,
					StorageSize: 2,
					BitOffset:   0,
					BitWidth:    1,
				},
			},
		},
	}
	aggregate.FinalizeLayout()
	variable := &typeinfo.FunctionVar{Name: "value", Type: aggregate}
	fx := testfixture.Stars(t)
	resolver := NewResolver(fx.Image, fx.SDB)

	if path, ok := resolver.ResolveFieldLoad(variable, 0, 2); ok {
		t.Fatalf("raw load resolved to %T %q, want unresolved storage", path, path.String())
	}
	if path, _, ok := resolver.ResolveField(variable, 0); ok {
		t.Fatalf("raw field path resolved to %T %q, want unresolved storage", path, path.String())
	}
	root := &SymbolRoot{Symbol: variable}
	if path, _, ok := resolver.ResolveContainingFieldPathInContext(root, 0, nil); ok {
		t.Fatalf("containing field resolved to %T %q, want unresolved storage", path, path.String())
	}
	path, ok := resolver.ResolveBitfieldLoad(variable, 0, 2, 0, 1)
	if !ok {
		t.Fatal("masked bitfield load did not resolve")
	}
	if _, ok := path.(*SymbolBitfield); !ok {
		t.Fatalf("masked load type = %T, want *SymbolBitfield", path)
	}
}

// TestResolveIndexedBitfieldPath verifies bitfield resolution preserves an
// indexed aggregate as the logical field base.
func TestResolveIndexedBitfieldPath(t *testing.T) {
	item := &typeinfo.Struct{
		Name: "Item",
		Size: 2,
		Fields: []typeinfo.StructField{
			{
				Name:   "flag",
				Type:   typeinfo.U16,
				Offset: 0,
				Bitfield: &typeinfo.Bitfield{
					BaseType:    typeinfo.U16,
					StorageSize: 2,
					BitOffset:   4,
					BitWidth:    1,
				},
			},
		},
	}
	item.FinalizeLayout()
	items := &typeinfo.FunctionVar{
		Name: "items",
		Type: &typeinfo.Array{Elem: item, Count: 4},
	}
	indexed := &SymbolTerm{
		Base:   &SymbolRoot{Symbol: items},
		Index:  &SymbolLiteral{Literal: 2, Typ: typeinfo.I16},
		Scale:  item.Bytes(),
		Result: item,
	}
	fx := testfixture.Stars(t)
	resolver := NewResolver(fx.Image, fx.SDB)

	path, ok := resolver.ResolveBitfieldPathLoad(indexed, 0, 2, 4, 1)
	if !ok {
		t.Fatal("indexed bitfield did not resolve")
	}
	if got := path.String(); got != "items[2].flag" {
		t.Fatalf("indexed bitfield = %q, want items[2].flag", got)
	}
}

// TestResolveBitfieldPathUsesUnionContext verifies ambiguous physical bit
// ranges remain unresolved until the active union member is selected.
func TestResolveBitfieldPathUsesUnionContext(t *testing.T) {
	choice := &typeinfo.Struct{
		Name:  "Choice",
		SKind: typeinfo.StructKindUnion,
		Size:  2,
		Fields: []typeinfo.StructField{
			{
				Name:   "first",
				Type:   typeinfo.U16,
				Offset: 0,
				Bitfield: &typeinfo.Bitfield{
					BaseType:    typeinfo.U16,
					StorageSize: 2,
					BitOffset:   0,
					BitWidth:    1,
				},
			},
			{
				Name:   "second",
				Type:   typeinfo.U16,
				Offset: 0,
				Bitfield: &typeinfo.Bitfield{
					BaseType:    typeinfo.U16,
					StorageSize: 2,
					BitOffset:   0,
					BitWidth:    1,
				},
			},
		},
	}
	choice.FinalizeLayout()
	variable := &typeinfo.FunctionVar{Name: "choice", Type: choice}
	root := &SymbolRoot{Symbol: variable}
	fx := testfixture.Stars(t)
	resolver := NewResolver(fx.Image, fx.SDB)

	if path, ok := resolver.ResolveBitfieldPathLoad(root, 0, 2, 0, 1); ok {
		t.Fatalf("ambiguous union bitfield resolved to %q", path.String())
	}
	rule := &typeinfo.UnionVariantRule{
		Type: choice,
		Enum: &typeinfo.Enum{Name: "ChoiceKind", Values: []typeinfo.EnumValue{
			{Name: "choiceFirst", Value: 1},
		}},
		ValueToMember: map[int]*typeinfo.StructField{1: &choice.Fields[0]},
	}
	ctx := NewUnionContext()
	if !ctx.Add(root, rule, typeinfo.EnumValue{Name: "choiceFirst", Value: 1}) {
		t.Fatal("union selection was not recorded")
	}
	path, ok := resolver.ResolveBitfieldPathLoadInContext(root, 0, 2, 0, 1, ctx)
	if !ok {
		t.Fatal("selected union bitfield did not resolve")
	}
	if got := path.String(); got != "choice.first" {
		t.Fatalf("selected union bitfield = %q, want choice.first", got)
	}
}
