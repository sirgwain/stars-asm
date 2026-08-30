package templates

import (
	"bytes"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestRenderDumpStructs(t *testing.T) {
	s := &typeinfo.Struct{
		Name:    "_sample",
		Typedef: "Sample",
		Size:    2,
		Fields: []typeinfo.StructField{
			{
				Name:   "value",
				Type:   &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "uint16_t", Size: 2},
				Offset: 0,
				Size:   2,
				End:    2,
			},
		},
	}

	var buf bytes.Buffer
	if err := RenderDumpStructs(&buf, NewDumpStructsView([]*typeinfo.Struct{s}, DumpOptions{})); err != nil {
		t.Fatalf("RenderDumpStructs() error = %v", err)
	}

	got := buf.String()
	for _, want := range []string{
		"#ifndef STARS_DECOMPILED_STRUCTS_H",
		"#include <stdint.h>",
		"typedef struct _sample {",
		"    uint16_t value; /* +0x0000 (2) */",
		"} Sample; /* size=0x2 */",
		"#endif",
	} {
		if !strings.Contains(got, want) {
			t.Fatalf("RenderDumpStructs() missing %q in:\n%s", want, got)
		}
	}
}
