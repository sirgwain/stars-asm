package templates

import (
	"bytes"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestRenderEnumDetail(t *testing.T) {
	e := &typeinfo.Enum{
		Name: "HtMineType",
		Values: []typeinfo.EnumValue{
			{Name: "htMineNone", Value: 0},
			{Name: "htMineMineralConc1", Value: 1},
			{Name: "htMineMinefieldType", Value: 14},
		},
	}

	var buf bytes.Buffer
	if err := RenderEnumDetail(&buf, NewEnumDetailView(e)); err != nil {
		t.Fatal(err)
	}

	want := `typedef enum HtMineType {
    htMineNone = 0,
    htMineMineralConc1 = 1,
    htMineMinefieldType = 14,
} HtMineType;
`
	if got := buf.String(); got != want {
		t.Fatalf("RenderEnumDetail() =\n%s\nwant:\n%s", got, want)
	}
}
