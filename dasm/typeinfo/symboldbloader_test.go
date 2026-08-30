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
		GlobalTypes: nb09.TypeStream{BaseIndex: 0x1000},
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
