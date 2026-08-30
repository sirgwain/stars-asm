package typeinfo

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/nb09"
)

func TestStructKind(t *testing.T) {
	s := Struct{Name: "POINT", SKind: StructKindStruct, Size: 4}
	u := Struct{Name: "WORD_OR_BYTES", SKind: StructKindUnion, Size: 2}

	if s.Kind() != KStruct {
		t.Fatalf("struct Kind() = %v, want %v", s.Kind(), KStruct)
	}
	if s.Bytes() != 4 {
		t.Fatalf("struct Bytes() = %d, want 4", s.Bytes())
	}
	if u.Kind() != KUnion {
		t.Fatalf("union Kind() = %v, want %v", u.Kind(), KUnion)
	}
	if u.Bytes() != 2 {
		t.Fatalf("union Bytes() = %d, want 2", u.Bytes())
	}
}

func TestStructFinalizeLayoutBuildsChunksAndOverlaps(t *testing.T) {
	uint16Type := &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}
	uint32Type := &Primitive{TypeKind: KInt, Name: "uint32_t", Size: 4}
	s := Struct{
		Name:  "OVERLAY",
		SKind: StructKindUnion,
		Fields: []StructField{
			{Name: "wide", Type: uint32Type, Offset: 0},
			{Name: "lo", Type: uint16Type, Offset: 0},
			{Name: "hi", Type: uint16Type, Offset: 2},
		},
	}

	s.FinalizeLayout()

	if got, want := len(s.Chunks), 3; got != want {
		t.Fatalf("chunks len = %d, want %d", got, want)
	}
	if got, want := s.Fields[0].End, 4; got != want {
		t.Fatalf("wide end = %d, want %d", got, want)
	}
	if got, want := len(s.OverlapRegions), 1; got != want {
		t.Fatalf("overlap regions len = %d, want %d", got, want)
	}
	region := s.OverlapRegions[0]
	if region.Start != 0 || region.End != 4 {
		t.Fatalf("overlap region = [%d,%d), want [0,4)", region.Start, region.End)
	}
	if got, want := len(region.Paths), 2; got != want {
		t.Fatalf("overlap paths len = %d, want %d", got, want)
	}
	fields := s.FieldsAtOffset(0)
	if len(fields) != 2 || fields[0].Name != "wide" || fields[1].Name != "lo" {
		t.Fatalf("FieldsAtOffset(0) = %#v, want wide and lo", fields)
	}
}

func TestStructTypedefString(t *testing.T) {
	s := Struct{Name: "_planet", Typedef: "PLANET", SKind: StructKindStruct}

	if got, want := s.String(), "PLANET"; got != want {
		t.Fatalf("String() = %q, want %q", got, want)
	}
}

func TestStructStringUsesNameWithoutTagPrefix(t *testing.T) {
	s := Struct{Name: "tagRECT", SKind: StructKindStruct}

	if got, want := s.String(), "tagRECT"; got != want {
		t.Fatalf("String() = %q, want %q", got, want)
	}
}

func TestStructFieldStringIncludesBitWidth(t *testing.T) {
	field := StructField{
		Name:     "fHomeworld",
		Bitfield: &Bitfield{BitWidth: 1},
	}

	if got, want := field.String(), "fHomeworld:1"; got != want {
		t.Fatalf("String() = %q, want %q", got, want)
	}
}

func TestSetStructTypedefPrefersNonTagName(t *testing.T) {
	s := Struct{Name: "_planet", SKind: StructKindStruct}

	setStructTypedef(&s, "PLANET")
	setStructTypedef(&s, "_planet")

	if got, want := s.Typedef, "PLANET"; got != want {
		t.Fatalf("Typedef = %q, want %q", got, want)
	}
}

func TestTypeResolverKeepsRecursiveStructPointersResolved(t *testing.T) {
	ts := nb09.TypeStream{
		BaseIndex: 0x1000,
		Records: []nb09.TypeRecord{
			{
				Leaf: nb09.LF_POINTER,
				Parsed: &nb09.LFPointer{
					UType: 0x1002,
					Attr:  0x0001,
				},
			},
			{
				Leaf: nb09.LF_FIELDLIST,
				Parsed: &nb09.LFFieldList{
					Fields: []nb09.FieldEntry{
						{
							Leaf: nb09.LF_MEMBER,
							Parsed: &nb09.LFMember{
								Type:   0x1000,
								Offset: nb09.NumericLeaf{Value: 0},
								Name:   "next",
							},
						},
					},
				},
			},
			{
				Leaf: nb09.LF_STRUCTURE,
				Parsed: &nb09.LFStruct{
					FieldList: 0x1001,
					Size:      nb09.NumericLeaf{Value: 4},
					Name:      "_fleet",
				},
			},
		},
	}

	resolver := newTypeResolver(ts)

	ptr, ok := resolver.getType(0x1000).(*Pointer)
	if !ok {
		t.Fatalf("type 0x1000 = %T, want *Pointer", resolver.getType(0x1000))
	}
	strct, ok := ptr.Elem.(*Struct)
	if !ok {
		t.Fatalf("pointer elem = %T, want *Struct", ptr.Elem)
	}
	if strct.Name != "_fleet" {
		t.Fatalf("struct name = %q, want _fleet", strct.Name)
	}
	fieldPtr, ok := strct.Fields[0].Type.(*Pointer)
	if !ok {
		t.Fatalf("recursive field type = %T, want *Pointer", strct.Fields[0].Type)
	}
	if fieldPtr.Elem != strct {
		t.Fatalf("recursive pointer elem = %#v, want same struct", fieldPtr.Elem)
	}
}

func TestTypeResolverLoadsBitfieldMembers(t *testing.T) {
	ts := nb09.TypeStream{
		BaseIndex: 0x1000,
		Records: []nb09.TypeRecord{
			{
				Leaf: nb09.LF_BITFIELD,
				Parsed: &nb09.LFBitfield{
					BaseType: 0x0073,
					Length:   3,
					Position: 0,
				},
			},
			{
				Leaf: nb09.LF_BITFIELD,
				Parsed: &nb09.LFBitfield{
					BaseType: 0x0073,
					Length:   5,
					Position: 3,
				},
			},
			{
				Leaf: nb09.LF_FIELDLIST,
				Parsed: &nb09.LFFieldList{
					Fields: []nb09.FieldEntry{
						{
							Leaf: nb09.LF_MEMBER,
							Parsed: &nb09.LFMember{
								Type:   0x1000,
								Offset: nb09.NumericLeaf{Value: 0},
								Name:   "mode",
							},
						},
						{
							Leaf: nb09.LF_MEMBER,
							Parsed: &nb09.LFMember{
								Type:   0x1001,
								Offset: nb09.NumericLeaf{Value: 0},
								Name:   "flags",
							},
						},
					},
				},
			},
			{
				Leaf: nb09.LF_STRUCTURE,
				Parsed: &nb09.LFStruct{
					FieldList: 0x1002,
					Size:      nb09.NumericLeaf{Value: 2},
					Name:      "BITS",
				},
			},
		},
	}

	resolver := newTypeResolver(ts)
	strct, ok := resolver.getType(0x1003).(*Struct)
	if !ok {
		t.Fatalf("type 0x1003 = %T, want *Struct", resolver.getType(0x1003))
	}
	if len(strct.Fields) != 2 {
		t.Fatalf("fields len = %d, want 2", len(strct.Fields))
	}
	if !strct.Fields[0].IsBitfield() {
		t.Fatalf("first field is not a bitfield")
	}
	if got, want := strct.Fields[1].Bitfield.BitOffset, 3; got != want {
		t.Fatalf("second bit offset = %d, want %d", got, want)
	}
	if got, want := len(strct.Chunks), 1; got != want {
		t.Fatalf("chunks len = %d, want %d", got, want)
	}
	if strct.Chunks[0].Kind != StructFieldChunkBitfield {
		t.Fatalf("chunk kind = %v, want bitfield", strct.Chunks[0].Kind)
	}
	if got, want := len(strct.Chunks[0].Fields), 2; got != want {
		t.Fatalf("bitfield chunk fields len = %d, want %d", got, want)
	}
}
