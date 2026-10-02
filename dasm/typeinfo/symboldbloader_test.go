package typeinfo

import (
	"os"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/nb09"
)

func TestLoadAddsMissingAlignSymData16Global(t *testing.T) {
	inputDir := t.TempDir()
	writeLoaderInputFiles(t, inputDir)

	db := &nb09.NB09DB{
		Modules: map[uint16]nb09.SstModule{
			7: {
				Name: "utilgen.obj",
				SegInfo: []nb09.ModuleSegInfo{
					{Seg: 1, Off: 0, CB: 0x10},
				},
			},
		},
		AlignSyms: map[uint16]nb09.SymStream{
			7: {
				Records: []nb09.SymRecord{
					{
						RecTyp: nb09.S_LDATA16,
						Parsed: &nb09.SData16{
							Addr:   nb09.Addr16{Seg: 1, Off: 4},
							TypInd: 0x0011,
							Name:   "lFileSeed2",
						},
					},
				},
			},
		},
		SrcModules: map[uint16]nb09.SstSrcModule{
			7: {
				Files: []nb09.SrcModuleFile{
					{Name: `c:\src\utilgen.c`},
				},
			},
		},
		SegMap: nb09.SstSegMap{
			Segs: []nb09.SegMapDesc{
				{Frame: 0x45},
			},
		},
		GlobalTypes: nb09.TypeStream{
			BaseIndex: 0x1000,
			Records: []nb09.TypeRecord{
				{
					Leaf: nb09.LF_FIELDLIST,
					Parsed: &nb09.LFFieldList{
						Fields: []nb09.FieldEntry{
							{Leaf: nb09.LF_MEMBER, Parsed: &nb09.LFMember{Type: 0x0011, Offset: nb09.NumericLeaf{Value: 0}, Name: "x"}},
							{Leaf: nb09.LF_MEMBER, Parsed: &nb09.LFMember{Type: 0x0011, Offset: nb09.NumericLeaf{Value: 2}, Name: "y"}},
						},
					},
				},
				{
					Leaf: nb09.LF_STRUCTURE,
					Parsed: &nb09.LFStruct{
						FieldList: 0x1000,
						Size:      nb09.NumericLeaf{Value: 4},
						Name:      "tagPOINT",
					},
				},
			},
		},
	}

	sdb, err := Load(inputDir, db)
	if err != nil {
		t.Fatalf("Load() error = %v", err)
	}

	got := sdb.GetGlobal("lFileSeed2")
	if got == nil {
		t.Fatal("GetGlobal(lFileSeed2) = nil")
	}
	if got.Addr != (Addr{Seg: 0x45, Off: 4}) {
		t.Fatalf("Addr = %s, want 0045:0004", got.Addr)
	}
	if got.Module != "utilgen" {
		t.Fatalf("Module = %q, want utilgen", got.Module)
	}
	if len(sdb.GetGlobalsForModule("utilgen")) != 1 {
		t.Fatalf("len(GetGlobalsForModule(utilgen)) = %d, want 1", len(sdb.GetGlobalsForModule("utilgen")))
	}
}

func writeLoaderInputFiles(t *testing.T, dir string) {
	t.Helper()
	files := map[string]string{
		"enums.h":              "",
		"win16defines.json":    `{"families": []}`,
		"enums.json":           "{}",
		"overrides-types.json": "{}",
	}
	for name, contents := range files {
		if err := os.WriteFile(dir+"/"+name, []byte(contents), 0o644); err != nil {
			t.Fatalf("write %s: %v", name, err)
		}
	}
}

func TestLoadSourceRangesUsesSrcModuleOffsets(t *testing.T) {
	db := &nb09.NB09DB{
		SrcModules: map[uint16]nb09.SstSrcModule{
			7: {
				Files: []nb09.SrcModuleFile{
					{
						Name: `c:\src\planet.c`,
						Segs: []nb09.SrcModuleFileSeg{
							{Seg: 2, Start: 0x10, End: 0x1f},
						},
					},
				},
			},
		},
	}
	cvSegMap := map[uint16]nb09.SegMapDesc{
		2: {Frame: 10, Off: 0x1000},
	}

	got := loadSourceRanges(db, cvSegMap)
	if len(got) != 1 {
		t.Fatalf("len(loadSourceRanges) = %d, want 1", len(got))
	}
	if got[0].Source != "planet" {
		t.Fatalf("Source = %q, want planet", got[0].Source)
	}
	if got[0].Addr != (Addr{Seg: 10, Off: 0x1010}) {
		t.Fatalf("Addr = %s, want 000a:1010", got[0].Addr)
	}
	if got[0].Len != 0x10 {
		t.Fatalf("Len = %#x, want 0x10", got[0].Len)
	}
}

func TestLoadModuleSourceRangesCoversModuleContribution(t *testing.T) {
	db := &nb09.NB09DB{
		Modules: map[uint16]nb09.SstModule{
			7: {
				SegInfo: []nb09.ModuleSegInfo{
					{Seg: 2, Off: 0, CB: 0x20},
				},
			},
		},
	}
	cvSegMap := map[uint16]nb09.SegMapDesc{
		2: {Frame: 10, Off: 0x1000},
	}
	sourceByIMod := map[uint16]string{7: "planet"}

	got := loadModuleSourceRanges(db, cvSegMap, sourceByIMod)
	if len(got) != 1 {
		t.Fatalf("len(loadModuleSourceRanges) = %d, want 1", len(got))
	}
	if got[0].Source != "planet" {
		t.Fatalf("Source = %q, want planet", got[0].Source)
	}
	if got[0].Addr != (Addr{Seg: 10, Off: 0x1000}) {
		t.Fatalf("Addr = %s, want 000a:1000", got[0].Addr)
	}
	if got[0].Len != 0x20 {
		t.Fatalf("Len = %#x, want 0x20", got[0].Len)
	}
}

func TestGetSourceForAddrPrefersMostSpecificRange(t *testing.T) {
	sdb := &SymbolDB{
		Sources: []SourceRange{
			{Source: "module", Addr: Addr{Seg: 10, Off: 0x1000}, Len: 0x100},
			{Source: "source", Addr: Addr{Seg: 10, Off: 0x1010}, Len: 0x10},
		},
	}

	got, ok := sdb.GetSourceForAddr(Addr{Seg: 10, Off: 0x1014})
	if !ok {
		t.Fatal("GetSourceForAddr did not find source")
	}
	if got.Source != "source" {
		t.Fatalf("Source = %q, want source", got.Source)
	}

	got, ok = sdb.GetSourceForAddr(Addr{Seg: 10, Off: 0x1004})
	if !ok {
		t.Fatal("GetSourceForAddr did not find fallback source")
	}
	if got.Source != "module" {
		t.Fatalf("fallback Source = %q, want module", got.Source)
	}
}

func TestApplyEnumPrefixRuleFollowsHungarianNames(t *testing.T) {
	int16Type := &Primitive{TypeKind: KInt, Name: "int16_t", Size: 2, Signed: true}
	uint16Type := &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}
	boolEnum := &Enum{
		Name:    "Bool",
		Truth:   true,
		Values:  []EnumValue{{Name: "FALSE", Value: 0}, {Name: "TRUE", Value: 1}},
		Storage: uint16Type,
		Decl:    int16Type,
	}
	fn := &Function{
		Name: "FCheckFleet",
		Ret:  int16Type,
		Vars: []FunctionVar{
			{Name: "fDirty", Type: uint16Type},
			{Name: "f", Type: int16Type},
			{Name: "fleet", Type: int16Type},
			{Name: "fkb", Type: int16Type},
			{Name: "fRet", Type: int16Type},
			{Name: "pfMulti", Type: &Pointer{Elem: int16Type}},
		},
	}
	order := &Struct{
		Name:  "_order",
		SKind: StructKindStruct,
		Fields: []StructField{
			{Name: "fValid", Type: uint16Type, Offset: 0, Size: 2, Bitfield: &Bitfield{BaseType: uint16Type, StorageSize: 2, BitWidth: 1}},
			{Name: "fScore", Type: uint16Type, Offset: 0, Size: 2, Bitfield: &Bitfield{BaseType: uint16Type, StorageSize: 2, BitOffset: 1, BitWidth: 2}},
		},
	}
	order.FinalizeLayout()
	loader := symboldbLoader{sdb: &SymbolDB{
		Functions:   []*Function{fn},
		Structs:     []*Struct{order},
		enumsByName: map[string]*Enum{"bool": boolEnum},
	}}

	rules := []*EnumUseRule{
		{Kind: UseLocal, Prefix: "f", EnumName: "Bool", Exclude: []string{"FCheckFleet.fRet"}},
		{Kind: UseCallResult, Prefix: "F", EnumName: "Bool"},
		{Kind: UseField, Prefix: "f", EnumName: "Bool"},
	}
	for _, rule := range rules {
		if err := loader.applyEnumPrefixRule(rule); err != nil {
			t.Fatalf("applyEnumPrefixRule(%s) error = %v", rule.Prefix, err)
		}
	}

	for _, v := range fn.Vars {
		_, annotated := v.Type.(*Enum)
		want := v.Name == "fDirty" || v.Name == "f"
		if annotated != want {
			t.Errorf("local %s annotated = %v, want %v", v.Name, annotated, want)
		}
	}
	if got, want := fn.Vars[0].Type.String(), "int16_t"; got != want {
		t.Errorf("fDirty declared as %s, want the family decl %s", got, want)
	}
	if _, ok := fn.Ret.(*Enum); !ok {
		t.Errorf("FCheckFleet return = %s, want Bool", fn.Ret)
	}
	if _, ok := order.Fields[0].Type.(*Enum); !ok {
		t.Errorf("one-bit fValid = %s, want Bool", order.Fields[0].Type)
	}
	if _, ok := order.Fields[1].Type.(*Enum); ok {
		t.Errorf("two-bit fScore annotated as Bool")
	}

	stale := &EnumUseRule{Kind: UseLocal, Prefix: "f", EnumName: "Bool", Exclude: []string{"FCheckFleet.fGone"}}
	if err := loader.applyEnumPrefixRule(stale); err == nil {
		t.Error("stale exclusion FCheckFleet.fGone was accepted")
	}
}
