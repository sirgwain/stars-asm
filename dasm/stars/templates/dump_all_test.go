package templates

import (
	"bytes"
	"strings"
	"testing"
)

func TestRenderCommonDeclaresCompatibilityTypesBeforeGeneratedHeaders(t *testing.T) {
	var buf bytes.Buffer
	if err := RenderCommon(&buf, DumpCommonView{Modules: []string{"sample"}}); err != nil {
		t.Fatalf("RenderCommon() error = %v", err)
	}

	got := buf.String()
	aliases := strings.Index(got, "typedef uint16_t WMType;")
	structs := strings.Index(got, "#include \"structs.h\"")
	module := strings.Index(got, "#include \"sample.h\"")
	if aliases < 0 || structs < 0 || module < 0 {
		t.Fatalf("RenderCommon() output missing expected declarations:\n%s", got)
	}
	if aliases > structs || structs > module {
		t.Fatalf("RenderCommon() declaration order is aliases=%d, structs=%d, module=%d", aliases, structs, module)
	}
}
