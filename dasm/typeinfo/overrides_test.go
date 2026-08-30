package typeinfo

import (
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/nb09"
)

func TestApplyOverridesAppliesStructFieldRules(t *testing.T) {
	inputDir := t.TempDir()
	json := `{
  "types": [
    { "name": "HWND", "canonical": "uint16_t" },
    {
      "name": "WNDPROC",
      "funcptr": {
        "ptr_class": "far",
        "callconv": "pascal",
        "ret": "int32_t",
        "params": [
          { "name": "hwnd", "ctype": "HWND" },
          { "name": "payload", "ctype": "const void far *" }
        ]
      }
    }
  ],
  "struct_overrides": [
    { "struct": "_btlplan", "field": "szName", "ctype": "char[32]" },
    { "struct": "_rpt", "prefix": "hwnd", "ctype": "HWND" }
  ]
}`
	if err := os.WriteFile(filepath.Join(inputDir, "overrides-types.json"), []byte(json), 0o644); err != nil {
		t.Fatal(err)
	}

	int16Type := &Primitive{TypeKind: KInt, Name: "int16_t", Size: 2, Signed: true}
	uint8ArrayType := &Array{Count: 32, Elem: &Primitive{TypeKind: KInt, Name: "uint8_t", Size: 1, Signed: false}}
	btlplan := &Struct{
		Name:  "_btlplan",
		SKind: StructKindStruct,
		Fields: []StructField{
			{Name: "szName", Type: uint8ArrayType, Offset: 0, Size: 32, End: 32},
			{Name: "next", Type: int16Type, Offset: 32, Size: 2, End: 34},
		},
	}
	rpt := &Struct{
		Name:  "_rpt",
		SKind: StructKindStruct,
		Fields: []StructField{
			{Name: "hwndReport", Type: int16Type, Offset: 0, Size: 2, End: 2},
		},
	}
	btlplan.FinalizeLayout()
	rpt.FinalizeLayout()

	loader := symboldbLoader{
		sdb:          &SymbolDB{Structs: []*Struct{btlplan, rpt}},
		typeResolver: newTypeResolver(nb09.TypeStream{}),
	}
	if err := loader.applyOverrides(inputDir); err != nil {
		t.Fatal(err)
	}

	if got, want := TypeDecl(btlplan.Fields[0].Type, btlplan.Fields[0].Name), "char szName[32]"; got != want {
		t.Fatalf("szName decl = %q, want %q", got, want)
	}
	if got, want := btlplan.Fields[0].Size, 32; got != want {
		t.Fatalf("szName size = %d, want %d", got, want)
	}
	if got, want := btlplan.Fields[0].End, 32; got != want {
		t.Fatalf("szName end = %d, want %d", got, want)
	}
	if got, want := btlplan.Chunks[0].End, 32; got != want {
		t.Fatalf("szName chunk end = %d, want %d", got, want)
	}
	if got, want := TypeDecl(rpt.Fields[0].Type, rpt.Fields[0].Name), "HWND hwndReport"; got != want {
		t.Fatalf("hwndReport decl = %q, want %q", got, want)
	}

	wndproc, ok := loader.typeResolver.getNamedType("WNDPROC").(*Pointer)
	if !ok {
		t.Fatalf("WNDPROC = %T, want *Pointer", loader.typeResolver.getNamedType("WNDPROC"))
	}
	if wndproc.Class != PtrFar {
		t.Fatalf("WNDPROC ptr class = %v, want %v", wndproc.Class, PtrFar)
	}
	fn, ok := wndproc.Elem.(*Function)
	if !ok {
		t.Fatalf("WNDPROC elem = %T, want Function", wndproc.Elem)
	}
	if fn.Conv != CCPascal {
		t.Fatalf("WNDPROC callconv = %v, want %v", fn.Conv, CCPascal)
	}
	if got, want := fn.Ret.String(), "int32_t"; got != want {
		t.Fatalf("WNDPROC ret = %q, want %q", got, want)
	}
	if got, want := len(fn.Params), 2; got != want {
		t.Fatalf("WNDPROC params len = %d, want %d", got, want)
	}
	if got, want := fn.Params[0].String(), "HWND hwnd"; got != want {
		t.Fatalf("WNDPROC param 0 = %q, want %q", got, want)
	}
	if got, want := fn.Params[1].String(), "void *payload"; got != want {
		t.Fatalf("WNDPROC param 1 = %q, want %q", got, want)
	}
}

func TestApplyOverridesAddsGlobalFromPublic(t *testing.T) {
	inputDir := t.TempDir()
	if err := os.WriteFile(filepath.Join(inputDir, "overrides-types.json"), []byte("{}"), 0o644); err != nil {
		t.Fatal(err)
	}
	json := `{
  "globals": [
    { "name": "__fac", "ctype": "double" }
  ]
}`
	if err := os.WriteFile(filepath.Join(inputDir, "override-globals.json"), []byte(json), 0o644); err != nil {
		t.Fatal(err)
	}

	sdb := &SymbolDB{
		globalsByAddr:     make(map[Addr]*GlobalVar),
		globalsByName:     make(map[string]*GlobalVar),
		globalsByModule:   make(map[string][]*GlobalVar),
		functionsByName:   make(map[string]*Function),
		functionsByModule: make(map[string][]*Function),
		functionsByAddr:   make(map[Addr]*Function),
		functionsBySeg:    make(map[uint16][]*Function),
		publicsByAddr:     make(map[Addr]*Public),
		publicsByName:     make(map[string]*Public),
	}
	publicAddr := Addr{Seg: 0x28, Off: 0x16f6}
	sdb.AddPublic(&Public{Name: "__fac", Addr: publicAddr})

	loader := symboldbLoader{
		sdb:          sdb,
		typeResolver: newTypeResolver(nb09.TypeStream{}),
	}
	if err := loader.applyOverrides(inputDir); err != nil {
		t.Fatal(err)
	}

	got := sdb.GetGlobal("__fac")
	if got == nil {
		t.Fatal("GetGlobal(__fac) = nil")
	}
	if got.Addr != publicAddr {
		t.Fatalf("Addr = %s, want %s", got.Addr, publicAddr)
	}
	if got.Module != GlobalsModule {
		t.Fatalf("Module = %q, want %q", got.Module, GlobalsModule)
	}
	if !strings.EqualFold(got.Type.String(), "double") {
		t.Fatalf("Type = %q, want double", got.Type.String())
	}
}

func TestApplyOverridesAppliesFunctionParamSemantics(t *testing.T) {
	inputDir := t.TempDir()
	if err := os.WriteFile(filepath.Join(inputDir, "overrides-types.json"), []byte("{}"), 0o644); err != nil {
		t.Fatal(err)
	}
	functionsJSON := `{
  "functions": [
    {
      "name": "LoadBitmap",
      "ret": "uint16_t",
      "params": [
        { "name": "arg1", "ctype": "uint16_t" },
        { "name": "arg2", "ctype": "const char far *" }
      ]
    }
  ]
}`
	if err := os.WriteFile(filepath.Join(inputDir, "override-user.json"), []byte(functionsJSON), 0o644); err != nil {
		t.Fatal(err)
	}
	semanticsJSON := `{
  "function_semantics": [
    {
      "name": "LoadBitmap",
      "params": [
        { "name": "arg2", "semantic": "resource_name_or_id" }
      ]
    }
  ]
}`
	if err := os.WriteFile(filepath.Join(inputDir, "overrides-semantics.json"), []byte(semanticsJSON), 0o644); err != nil {
		t.Fatal(err)
	}

	sdb := &SymbolDB{
		globalsByAddr:     make(map[Addr]*GlobalVar),
		globalsByName:     make(map[string]*GlobalVar),
		globalsByModule:   make(map[string][]*GlobalVar),
		functionsByName:   make(map[string]*Function),
		functionsByModule: make(map[string][]*Function),
		functionsByAddr:   make(map[Addr]*Function),
		functionsBySeg:    make(map[uint16][]*Function),
		publicsByAddr:     make(map[Addr]*Public),
		publicsByName:     make(map[string]*Public),
	}
	loader := symboldbLoader{
		sdb:          sdb,
		typeResolver: newTypeResolver(nb09.TypeStream{}),
	}
	if err := loader.applyOverrides(inputDir); err != nil {
		t.Fatal(err)
	}

	fn := sdb.GetFunction("LoadBitmap")
	if fn == nil {
		t.Fatal("LoadBitmap not found")
	}
	if got, want := fn.Params[1].Semantic, ParamSemanticResourceNameOrID; got != want {
		t.Fatalf("arg2 semantic = %q, want %q", got, want)
	}
}
