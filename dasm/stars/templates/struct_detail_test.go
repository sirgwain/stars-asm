package templates

import (
	"bytes"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestRenderStructDetail(t *testing.T) {
	uint16Type := primitive("uint16_t", 2)
	s := &typeinfo.Struct{
		Name:    "_sample",
		Typedef: "SAMPLE",
		SKind:   typeinfo.StructKindStruct,
		Size:    4,
		Fields: []typeinfo.StructField{
			{Name: "id", Type: primitive("int16_t", 2), Offset: 0},
			{
				Name:   "lo",
				Type:   uint16Type,
				Offset: 2,
				Bitfield: &typeinfo.Bitfield{
					BaseType:    uint16Type,
					StorageSize: 2,
					BitOffset:   0,
					BitWidth:    8,
				},
			},
			{
				Name:   "hi",
				Type:   uint16Type,
				Offset: 2,
				Bitfield: &typeinfo.Bitfield{
					BaseType:    uint16Type,
					StorageSize: 2,
					BitOffset:   8,
					BitWidth:    8,
				},
			},
			{Name: "word", Type: uint16Type, Offset: 2},
		},
	}
	s.FinalizeLayout()

	var buf bytes.Buffer
	if err := RenderStructDetail(&buf, NewStructDetailView(s, DumpOptions{})); err != nil {
		t.Fatal(err)
	}

	want := `typedef struct _sample {
    int16_t id; /* +0x0000 (2) */
    union {
        uint16_t lo : 8, /* +0x0002 (2) @bit0 */
            hi : 8;      /* @bit8 */
        uint16_t word;   /* +0x0002 (2) */
    };
} SAMPLE; /* size=0x4 */
`
	if got := buf.String(); got != want {
		t.Fatalf("RenderStructDetail() =\n%s\nwant:\n%s", got, want)
	}
}

func primitive(name string, size int) *typeinfo.Primitive {
	return &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: name, Size: size}
}
