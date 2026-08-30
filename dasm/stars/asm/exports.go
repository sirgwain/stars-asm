package asm

import (
	"encoding/xml"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

// exportEntry represents one exported function from a DLL.
type exportEntry struct {
	name  string
	purge int // stack bytes to clean up (-1 = unknown)
}

// xmlLibrary mirrors the Ghidra XML export format.
type xmlLibrary struct {
	XMLName xml.Name    `xml:"LIBRARY"`
	Name    string      `xml:"NAME,attr"`
	Exports []xmlExport `xml:"EXPORT"`
}

type xmlExport struct {
	Ordinal string `xml:"ORDINAL,attr"`
	Name    string `xml:"NAME,attr"`
	Purge   string `xml:"PURGE,attr"`
}

// LoadExports parses one or more Ghidra-format XML export files and populates
// img.ImportExports. Module names are stored in uppercase to match NE convention.
func (img *ImageNE) LoadExports(paths ...string) error {
	if img.importExports == nil {
		img.importExports = make(map[string]map[uint16]exportEntry)
	}

	for _, path := range paths {
		if err := img.loadExportFile(path); err != nil {
			return fmt.Errorf("load exports %s: %w", path, err)
		}
	}

	// resolve all the fixups that point to ordinals so decompile functions
	// can use them
	img.resolveImportFixups()
	return nil
}

func (img *ImageNE) loadExportFile(path string) error {
	data, err := os.ReadFile(path)
	if err != nil {
		return err
	}

	var lib xmlLibrary
	if err := xml.Unmarshal(data, &lib); err != nil {
		return err
	}

	modName := strings.TrimSpace(lib.Name)
	if ext := filepath.Ext(modName); ext != "" {
		modName = strings.TrimSuffix(modName, ext)
	}
	modName = strings.ToUpper(modName)
	if modName == "" {
		return fmt.Errorf("no library name in %s", path)
	}

	exports := img.importExports[modName]
	if exports == nil {
		exports = make(map[uint16]exportEntry)
		img.importExports[modName] = exports
	}

	for _, x := range lib.Exports {
		ord, err := strconv.ParseUint(x.Ordinal, 10, 16)
		if err != nil {
			continue
		}
		purge, _ := strconv.Atoi(x.Purge)
		exports[uint16(ord)] = exportEntry{
			name:  x.Name,
			purge: purge,
		}
	}

	return nil
}

func (img *ImageNE) resolveImportFixups() {
	for segIdx := range img.fixups {
		fx := img.fixups[segIdx]
		for off, f := range fx.ByOff {
			if f.Target != FixupTargetImportOrdinal {
				continue
			}
			f.ModuleName = img.moduleName(f.ModuleIdx)
			f.FuncName = fmt.Sprintf("ordinal_%d", f.Ordinal)
			f.Purge = -1
			if exports := img.importExports[f.ModuleName]; exports != nil {
				if entry, found := exports[f.Ordinal]; found {
					f.FuncName = entry.name
					f.Purge = entry.purge
				}
			}
			fx.ByOff[off] = f
		}
	}
}

func (img *ImageNE) moduleName(moduleIdx uint16) string {
	if int(moduleIdx) < 1 || int(moduleIdx) > len(img.moduleNames) {
		return ""
	}
	return img.moduleNames[moduleIdx-1]
}
