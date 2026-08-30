package nb09

import (
	"testing"
)

// loadParsed is a test helper that loads and fully parses stars.exe.
func loadParsed(t *testing.T) *NB09DB {
	t.Helper()
	db, err := Load(starsExePath)
	if err != nil {
		t.Fatalf("ParseNB09 error: %v", err)
	}
	return db
}

func TestParseNB09_Stars(t *testing.T) {
	blob, info, err := loadNB09FromExePath(starsExePath)
	if err != nil {
		t.Skipf("unable to load NB09 from %s: %v", starsExePath, err)
	}

	nb, err := parseNB09(blob)
	if err != nil {
		t.Fatalf("ParseNB09 error: %v", err)
	}

	// ---- Header checks ----

	if string(nb.Header.Sig[:]) != "NB09" {
		t.Fatalf("header signature mismatch: %q", nb.Header.Sig)
	}

	if nb.Header.LfoDir != info.dirRelativeOff {
		t.Fatalf("LfoDir mismatch: got 0x%X want 0x%X",
			nb.Header.LfoDir, info.dirRelativeOff)
	}

	// ---- Directory header checks ----

	if nb.Dir.Header.CBDirHeader != 0x0010 {
		t.Fatalf("CBDirHeader got 0x%X want 0x0010", nb.Dir.Header.CBDirHeader)
	}

	if nb.Dir.Header.CBDirEntry != 0x000C {
		t.Fatalf("CBDirEntry got 0x%X want 0x000C", nb.Dir.Header.CBDirEntry)
	}

	if nb.Dir.Header.CDir != 0x00EA {
		t.Fatalf("CDir got %d want 0x00EA", nb.Dir.Header.CDir)
	}

	if nb.Dir.Header.CDir != info.dirEntryCount {
		t.Fatalf("DirEntryCount mismatch: parse=%d locate=%d",
			nb.Dir.Header.CDir, info.dirEntryCount)
	}

	// ---- Entries / subsections ----

	if len(nb.Dir.Entries) != int(nb.Dir.Header.CDir) {
		t.Fatalf("Dir.Entries len=%d want %d",
			len(nb.Dir.Entries), nb.Dir.Header.CDir)
	}

	if len(nb.subsections) != len(nb.Dir.Entries) {
		t.Fatalf("Subsections len=%d want %d",
			len(nb.subsections), len(nb.Dir.Entries))
	}

	// Validate each subsection slice is internally consistent.
	for i, de := range nb.Dir.Entries {
		sub := nb.subsections[i]

		if sub.Kind != de.Subsection {
			t.Fatalf("subsection kind mismatch at %d: 0x%X vs 0x%X",
				i, sub.Kind, de.Subsection)
		}

		if sub.IMod != de.IMod {
			t.Fatalf("subsection imod mismatch at %d: %d vs %d",
				i, sub.IMod, de.IMod)
		}

		if de.CB == 0 {
			if sub.Data != nil {
				t.Fatalf("subsection %d expected nil data for CB=0", i)
			}
			continue
		}

		if len(sub.Data) != int(de.CB) {
			t.Fatalf("subsection %d size mismatch: got %d want %d",
				i, len(sub.Data), de.CB)
		}
	}

	// ---- Sanity: ensure at least one known subsection exists ----

	foundModule := false
	foundGlobalTypes := false

	for _, s := range nb.subsections {
		switch s.Kind {
		case SST_MODULE:
			foundModule = true
		case SST_GLOBAL_TYPES:
			foundGlobalTypes = true
		}
	}

	if !foundModule {
		t.Fatalf("expected at least one SST_MODULE subsection")
	}
	if !foundGlobalTypes {
		t.Fatalf("expected at least one SST_GLOBAL_TYPES subsection")
	}
}

func TestParseModules(t *testing.T) {
	nb := loadParsed(t)

	if len(nb.Modules) == 0 {
		t.Fatal("expected at least one module")
	}

	// Check that all modules have names
	for imod, mod := range nb.Modules {
		if mod.Name == "" {
			t.Errorf("module imod=%d has empty name", imod)
		}
	}

	t.Logf("parsed %d modules", len(nb.Modules))

	// Spot-check: there should be a module with a .obj or .OBJ suffix
	foundObj := false
	for _, mod := range nb.Modules {
		if len(mod.Name) > 4 {
			foundObj = true
			break
		}
	}
	if !foundObj {
		t.Error("expected at least one module with a non-trivial name")
	}
}

func TestParseSegMap(t *testing.T) {
	nb := loadParsed(t)

	if nb.SegMap.CSeg == 0 {
		t.Fatal("expected non-zero segment count in segmap")
	}
	if len(nb.SegMap.Segs) != int(nb.SegMap.CSeg) {
		t.Fatalf("segmap segs count mismatch: got %d want %d",
			len(nb.SegMap.Segs), nb.SegMap.CSeg)
	}
	t.Logf("segmap: %d segments, %d logical", nb.SegMap.CSeg, nb.SegMap.CSegLog)
}

func TestParseSegNames(t *testing.T) {
	nb := loadParsed(t)

	if len(nb.SegNames.At) == 0 {
		t.Fatal("expected non-empty segment name table")
	}
	t.Logf("segnames: %d entries", len(nb.SegNames.At))
}

func TestParseLibraries(t *testing.T) {
	nb := loadParsed(t)

	if len(nb.Libraries) == 0 {
		t.Fatal("expected at least one library")
	}
	for i, lib := range nb.Libraries {
		if lib == "" {
			t.Errorf("library %d is empty", i)
		}
	}
	t.Logf("libraries: %d entries", len(nb.Libraries))
}

func TestParseGlobalPub(t *testing.T) {
	nb := loadParsed(t)

	if len(nb.GlobalPub.Symbols.Records) == 0 {
		t.Fatal("expected global pub symbols")
	}

	// Count procs and look for a known symbol
	var procCount, dataCount int
	foundKnown := false
	for _, rec := range nb.GlobalPub.Symbols.Records {
		switch rec.RecTyp {
		case S_PUB16:
			if p, ok := rec.Parsed.(*SPub16); ok && p != nil {
				if p.Name == "LpengineFromId" {
					foundKnown = true
				}
			}
		case S_GPROC16:
			procCount++
		case S_GDATA16:
			dataCount++
		}
	}
	t.Logf("globalPub: %d records (%d procs, %d data)", len(nb.GlobalPub.Symbols.Records), procCount, dataCount)
	if foundKnown {
		t.Log("found known symbol 'LpengineFromId' in globalPub")
	}
}

func TestParseGlobalSym(t *testing.T) {
	nb := loadParsed(t)

	if len(nb.GlobalSym.Symbols.Records) == 0 {
		t.Fatal("expected global sym records")
	}
	t.Logf("globalSym: %d records", len(nb.GlobalSym.Symbols.Records))
}

func TestParseAlignSym(t *testing.T) {
	nb := loadParsed(t)

	if len(nb.AlignSyms) == 0 {
		t.Fatal("expected at least one alignSym stream")
	}

	totalRecs := 0
	for _, ss := range nb.AlignSyms {
		totalRecs += len(ss.Records)
	}
	t.Logf("alignSym: %d modules, %d total records", len(nb.AlignSyms), totalRecs)
}

func TestParseGlobalTypes(t *testing.T) {
	nb := loadParsed(t)

	if len(nb.GlobalTypes.Records) == 0 {
		t.Fatal("expected type records")
	}
	if nb.GlobalTypes.BaseIndex != 0x1000 {
		t.Fatalf("base index got 0x%X want 0x1000", nb.GlobalTypes.BaseIndex)
	}

	// Count by leaf kind
	counts := make(map[TypeLeaf]int)
	for _, rec := range nb.GlobalTypes.Records {
		counts[rec.Leaf]++
	}

	t.Logf("globalTypes: %d records", len(nb.GlobalTypes.Records))
	for leaf, count := range counts {
		t.Logf("  %s: %d", leaf, count)
	}

	// Verify at least some common types are parsed
	parsedCount := 0
	for _, rec := range nb.GlobalTypes.Records {
		if rec.Parsed != nil {
			parsedCount++
		}
	}
	if parsedCount == 0 {
		t.Fatal("no type records were parsed")
	}
	t.Logf("  parsed: %d / %d", parsedCount, len(nb.GlobalTypes.Records))
}

func TestParseFileIndex(t *testing.T) {
	nb := loadParsed(t)

	if nb.FileIndex.CMod == 0 {
		t.Fatal("expected non-zero module count in file index")
	}
	if nb.FileIndex.CRef == 0 {
		t.Fatal("expected non-zero ref count in file index")
	}
	if len(nb.FileIndex.ModToFiles) == 0 {
		t.Fatal("expected non-empty ModToFiles map")
	}

	// Check that at least one module has files
	foundFiles := false
	for _, files := range nb.FileIndex.ModToFiles {
		if len(files) > 0 {
			foundFiles = true
			break
		}
	}
	if !foundFiles {
		t.Fatal("expected at least one module to have source files")
	}

	t.Logf("fileIndex: %d modules, %d refs", nb.FileIndex.CMod, nb.FileIndex.CRef)
}

func TestParseSrcModule(t *testing.T) {
	nb := loadParsed(t)

	if len(nb.SrcModules) == 0 {
		t.Fatal("expected at least one srcModule entry")
	}

	totalFiles := 0
	foundRange := false
	for _, sm := range nb.SrcModules {
		totalFiles += len(sm.Files)
		for _, file := range sm.Files {
			for range file.Segs {
				// found at least one source range
				foundRange = true
				break
			}
		}
	}
	if !foundRange {
		t.Fatal("expected at least one srcModule segment range")
	}
	t.Logf("srcModule: %d modules, %d total files", len(nb.SrcModules), totalFiles)
}

func TestIntegration_FindKnownFunction(t *testing.T) {
	nb := loadParsed(t)

	// Search all symbol streams for a known function name
	target := "LpengineFromId"

	// Check globalSym (S_PROCREF -> resolve to alignSym)
	for _, rec := range nb.GlobalSym.Symbols.Records {
		if ref, ok := rec.Parsed.(*SProcRef); ok && ref != nil {
			// Resolve into alignSym
			alignStream, ok := nb.AlignSyms[ref.IMod]
			if !ok {
				continue
			}
			for _, arec := range alignStream.Records {
				if arec.Offset != ref.SymOff {
					continue
				}
				switch p := arec.Parsed.(type) {
				case *SProc16:
					if p != nil && p.Name == target {
						t.Logf("found %s: seg=%d off=0x%04X typind=0x%04X (via S_PROCREF imod=%d)",
							target, p.Addr.Seg, p.Addr.Off, p.TypInd, ref.IMod)
						return
					}
				}
			}
		}
	}

	// Also search directly in alignSym streams
	for imod, ss := range nb.AlignSyms {
		for _, rec := range ss.Records {
			switch p := rec.Parsed.(type) {
			case *SProc16:
				if p != nil && p.Name == target {
					t.Logf("found %s in alignSym imod=%d: seg=%d off=0x%04X typind=0x%04X",
						target, imod, p.Addr.Seg, p.Addr.Off, p.TypInd)
					return
				}
			}
		}
	}

	// Check globalPub
	for _, rec := range nb.GlobalPub.Symbols.Records {
		switch p := rec.Parsed.(type) {
		case *SPub16:
			if p != nil && p.Name == target {
				t.Logf("found %s in globalPub: seg=%d off=0x%04X typind=0x%04X",
					target, p.Addr.Seg, p.Addr.Off, p.TypInd)
				return
			}
		}
	}

	t.Fatalf("could not find %s in any symbol stream", target)
}

func TestParseStats(t *testing.T) {
	nb := loadParsed(t)

	// Count procs, globals, labels across all sym streams
	var procCount, globalCount, labelCount int

	countSyms := func(ss SymStream) {
		for _, rec := range ss.Records {
			switch rec.RecTyp {
			case S_LPROC16, S_GPROC16, S_LPROC32, S_GPROC32:
				procCount++
			case S_LDATA16, S_GDATA16, S_LDATA32, S_GDATA32:
				globalCount++
			case S_LABEL16:
				labelCount++
			}
		}
	}

	for _, ss := range nb.AlignSyms {
		countSyms(ss)
	}

	t.Logf("stats: %d modules, %d procs, %d data, %d labels, %d types",
		len(nb.Modules), procCount, globalCount, labelCount, len(nb.GlobalTypes.Records))
}
