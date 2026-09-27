package templates

import (
	"bytes"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestRenderCommonIncludesWin16DefinesBeforeGeneratedHeaders(t *testing.T) {
	var buf bytes.Buffer
	if err := RenderCommon(&buf, DumpCommonView{Modules: []string{"sample"}}); err != nil {
		t.Fatalf("RenderCommon() error = %v", err)
	}

	got := buf.String()
	defines := strings.Index(got, "#include \"win16defines.h\"")
	structs := strings.Index(got, "#include \"structs.h\"")
	module := strings.Index(got, "#include \"sample.h\"")
	if defines < 0 || structs < 0 || module < 0 {
		t.Fatalf("RenderCommon() output missing expected declarations:\n%s", got)
	}
	if defines > structs || structs > module {
		t.Fatalf("RenderCommon() declaration order is defines=%d, structs=%d, module=%d", defines, structs, module)
	}
}

func TestRenderWin16DefinesGuardsOnlyWin16Constants(t *testing.T) {
	messages := &typeinfo.Enum{
		Name:    typeinfo.MessageEnumName,
		Values:  []typeinfo.EnumValue{{Name: "WM_CREATE", Value: 0x0001}, {Name: "WM_STARS_HOST", Value: 0x0465}},
		Storage: typeinfo.U16,
	}
	gameEnum := &typeinfo.Enum{
		Name:    "RecordType",
		Values:  []typeinfo.EnumValue{{Name: "rtPlanet", Value: 13}},
		Typedef: typeinfo.U16,
	}
	var buf bytes.Buffer
	if err := RenderWin16Defines(&buf, NewWin16DefinesView([]*typeinfo.Enum{messages, gameEnum})); err != nil {
		t.Fatalf("RenderWin16Defines() error = %v", err)
	}

	got := buf.String()
	if !strings.Contains(got, "#ifndef WM_STARS_HOST\n#define WM_STARS_HOST 0x0465\n#endif") {
		t.Fatalf("RenderWin16Defines() missing guarded define:\n%s", got)
	}
	if strings.Contains(got, "rtPlanet") {
		t.Fatalf("RenderWin16Defines() emitted a game enum value as a define:\n%s", got)
	}
}

func TestRenderEnumsTypedefsEnumNamesToTheirStorage(t *testing.T) {
	source := "typedef enum MdXfer { mdXferNone = -1, mdXferCargo = 0 } MdXfer;\n" +
		"typedef enum { grStatFuel = 1 } GrStat;\n"
	enums := []*typeinfo.Enum{
		{Name: "MdXfer", Typedef: typeinfo.I16},
		{Name: "GrStat", Typedef: typeinfo.U16},
	}
	var buf bytes.Buffer
	if err := RenderEnums(&buf, source, enums); err != nil {
		t.Fatalf("RenderEnums() error = %v", err)
	}

	want := "enum MdXfer { mdXferNone = -1, mdXferCargo = 0 };\ntypedef int16_t MdXfer;\n" +
		"enum GrStat { grStatFuel = 1 };\ntypedef uint16_t GrStat;\n"
	if got := buf.String(); got != want {
		t.Fatalf("RenderEnums() =\n%s\nwant\n%s", got, want)
	}
}
