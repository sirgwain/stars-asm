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

// TestAddFunctionPreservesParamLocations verifies a signature override keeps
// the stack locations learned from CodeView.
func TestAddFunctionPreservesParamLocations(t *testing.T) {
	sdb := &SymbolDB{
		functionsByName:   make(map[string]*Function),
		functionsByModule: make(map[string][]*Function),
		functionsByAddr:   make(map[Addr]*Function),
		functionsBySeg:    make(map[uint16][]*Function),
	}
	existing := &Function{
		Name: "HfontPrinterCreate",
		Conv: CCStdcall,
		Params: []FunctionVar{
			{Name: "hdc", Type: U16, BPOffset: 6},
			{Name: "iSize", Type: I16, BPOffset: 8},
			{Name: "pdyFont", Type: &Pointer{Elem: I16, Class: PtrNear}, BPOffset: 10},
		},
	}
	sdb.AddFunction(existing)

	sdb.AddFunction(&Function{
		Name: "HfontPrinterCreate",
		Conv: CCStdcall,
		Params: []FunctionVar{
			{Name: "hdc", Type: U16},
			{Name: "iSize", Type: I16},
			{Name: "pdyFont", Type: &Pointer{Elem: I16, Class: PtrNear}},
		},
	})

	for i, want := range []int{6, 8, 10} {
		if got := existing.Params[i].BPOffset; got != want {
			t.Fatalf("param %d BP offset = %#x, want %#x", i, got, want)
		}
	}
}

// TestNamedArrayDeclarations preserves native typedef spelling and source storage size.
func TestNamedArrayDeclarations(t *testing.T) {
	o := newOverrideDB(&SymbolDB{}, newTypeResolver(nb09.TypeStream{}))
	env, err := o.resolveNamedType("jmp_buf", "int16_t[9]")
	if err != nil {
		t.Fatal(err)
	}
	if env.Bytes() != 18 {
		t.Fatalf("source size = %d, want 18", env.Bytes())
	}
	tests := []struct {
		typ  Type
		want string
	}{
		{env, "jmp_buf value"},
		{&Pointer{Elem: env}, "jmp_buf *value"},
		{&Array{Elem: env, Count: 2}, "jmp_buf value[2]"},
		{&Pointer{Elem: &Array{Elem: I16, Count: 9}}, "int16_t (*value)[9]"},
		{&Array{Elem: &Pointer{Elem: I16}, Count: 9}, "int16_t *value[9]"},
		{&Pointer{Elem: &Pointer{Elem: &Array{Elem: I16, Count: 9}}}, "int16_t (**value)[9]"},
	}
	for _, tt := range tests {
		if got := TypeDecl(tt.typ, "value"); got != tt.want {
			t.Errorf("declaration = %q, want %q", got, tt.want)
		}
	}
	matrix, err := o.resolveNamedType("", "int16_t[4][2]")
	if err != nil {
		t.Fatal(err)
	}
	if got := TypeDecl(matrix, "matrix"); got != "int16_t matrix[4][2]" || matrix.Bytes() != 16 {
		t.Fatalf("matrix = %s (%d bytes)", got, matrix.Bytes())
	}
}

// TestFunctionOverrideRenamesDefinedBody retains body metadata and original parameter locations.
func TestFunctionOverrideRenamesDefinedBody(t *testing.T) {
	sdb := &SymbolDB{
		functionsByName:   make(map[string]*Function),
		functionsByModule: make(map[string][]*Function),
		functionsByAddr:   make(map[Addr]*Function),
		functionsBySeg:    make(map[uint16][]*Function),
		publicsByName:     make(map[string]*Public),
	}
	addr := Addr{Seg: 4, Off: 0x123}
	original := &Function{Name: "CopyFile", Addr: addr, Module: "utilgen", Len: 100, Ret: I16,
		Params: []FunctionVar{{Name: "value", Type: I16, BPOffset: 6}},
		Vars:   []FunctionVar{{Name: "local", Type: I16, BPOffset: -2}}}
	sdb.AddFunction(original)
	path := filepath.Join(t.TempDir(), "overrides.json")
	if err := os.WriteFile(path, []byte(`{"functions":[{"name":"CopyFile","rename":"StarsCopyFile","native_decl":"int StarsCopyFile(int value)","ret":"int16_t","params":[{"name":"value","ctype":"int16_t"}],"callconv":"pascal"}]}`), 0600); err != nil {
		t.Fatal(err)
	}
	o := newOverrideDB(sdb, newTypeResolver(nb09.TypeStream{}))
	if err := o.loadFunctions(path); err != nil {
		t.Fatal(err)
	}
	if len(sdb.Functions) != 1 || sdb.GetFunction("StarsCopyFile") != original || sdb.GetFunction("CopyFile") != original || sdb.GetFunctionByAddr(addr) != original || sdb.GetFunctionsForModule("utilgen")[0] != original {
		t.Fatal("rename lost original function identity")
	}
	if original.Len != 100 || len(original.Vars) != 1 || original.Params[0].BPOffset != 6 || original.Ret.Bytes() != 2 {
		t.Fatal("override changed source body or ABI storage")
	}
	if original.CDecl() != "int StarsCopyFile(int value)" {
		t.Fatal(original.CDecl())
	}
}
