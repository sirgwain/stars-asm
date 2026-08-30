package typeinfo

import (
	"cmp"
	"fmt"
	"maps"
	"path/filepath"
	"slices"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/nb09"
)

type symboldbLoader struct {
	sdb *SymbolDB
	// cvSegMap maps raw CodeView segment numbers to their NB09 segment map entry.
	cvSegMap     map[uint16]nb09.SegMapDesc
	typeResolver *typeResolver
}

// Load loads the symboldb from the nb09 database
func Load(inputDir string, db *nb09.NB09DB) (*SymbolDB, error) {
	typeResolver := newTypeResolver(db.GlobalTypes)

	loader := symboldbLoader{
		sdb: &SymbolDB{
			globalsByAddr:     make(map[Addr]*GlobalVar),
			globalsByName:     make(map[string]*GlobalVar),
			globalsByModule:   make(map[string][]*GlobalVar),
			functionsByName:   make(map[string]*Function),
			functionsByModule: make(map[string][]*Function),
			functionsByAddr:   make(map[Addr]*Function),
			functionsBySeg:    make(map[uint16][]*Function),
			structsByName:     make(map[string]*Struct),
			enumsByName:       make(map[string]*Enum),
			publicsByAddr:     make(map[Addr]*Public),
			publicsByName:     make(map[string]*Public),
			sourcesByName:     make(map[string]SourceRange),
		},
		cvSegMap:     loadCVSegMap(db),
		typeResolver: typeResolver,
	}
	sdb := loader.sdb

	// get a sorted list of imods
	imods := make([]uint16, 0, len(db.AlignSyms))
	for imod := range db.AlignSyms {
		imods = append(imods, imod)
	}
	slices.Sort(imods)

	sourceByIMod := loadSourceByIMod(db)
	sourceModules := make(map[string]struct{}, len(db.SrcModules)+1)
	for imod, module := range db.SrcModules {
		foundSource := false
		for _, file := range module.Files {
			if source := sourceBaseName(file.Name); source != "" {
				sourceModules[source] = struct{}{}
				foundSource = true
			}
		}
		if !foundSource {
			if source := sourceByIMod[imod]; source != "" {
				sourceModules[source] = struct{}{}
			}
		}
	}

	// Include globals in generated module lists.
	for _, seg := range db.SegMap.Segs {
		if db.SegNames.At[uint32(seg.ISegName)] == "DGROUP" {
			sdb.DGroupFrame = uint(seg.Frame)
			sourceModules[GlobalsModule] = struct{}{}
			break
		}
	}

	for _, source := range loadSourceRanges(db, loader.cvSegMap) {
		sdb.Sources = append(sdb.Sources, source)
		sdb.sourcesByName[source.Source] = source
	}
	// these ranges are for outside our source lines, don't update the sourcesByName
	sdb.Sources = append(sdb.Sources, loadModuleSourceRanges(db, loader.cvSegMap, sourceByIMod)...)

	slices.SortFunc(sdb.Sources, func(a, b SourceRange) int {
		return CompareAddr(a.Addr, b.Addr)
	})
	sdb.Modules = slices.Collect(maps.Keys(sourceModules))
	slices.Sort(sdb.Modules)

	// load non-proc records for alignsyms
	for _, rec := range db.GlobalSym.Symbols.Records {
		switch p := rec.Parsed.(type) {
		case *nb09.SData16:
			addr, err := loader.neAddrForCV(Addr{Seg: p.Addr.Seg, Off: uint32(p.Addr.Off)})
			if err != nil {
				return nil, err
			}
			module := GlobalsModule
			source, ok := sdb.GetSourceForAddr(addr)
			if ok {
				module = source.Source
			}
			global := GlobalVar{
				Name:   p.Name,
				Addr:   addr,
				Type:   typeResolver.resolve(p.TypInd),
				Module: module,
			}

			if _, ok := sdb.globalsByAddr[global.Addr]; !ok {
				sdb.AddGlobal(&global)
			}
		case *nb09.SUdt:
			// load typedefs from alignsyms
			typ := typeResolver.resolve(p.TypInd)
			switch v := typ.(type) {
			case *Struct:
				setStructTypedef(v, p.Name)
			case *Pointer:
				// ignore for now, this is like LPPLANET
			}
		}
	}

	// after typedefs are set, populate the struct maps
	for _, t := range typeResolver.types {
		switch v := t.(type) {
		case *Struct:
			sdb.Structs = append(sdb.Structs, v)
			sdb.structsByName[strings.ToLower(v.Name)] = v
			typeResolver.registerNamedType(v.Name, v)
			if v.Typedef != "" {
				sdb.structsByName[strings.ToLower(v.Typedef)] = v
				typeResolver.registerNamedType(v.Typedef, v)
			}
		}
	}

	// load alignsyms data
	for _, imod := range imods {
		stream := db.AlignSyms[imod]

		// update any globals with module information
		for _, rec := range stream.Records {

			switch p := rec.Parsed.(type) {
			case *nb09.SData16:
				addr, err := loader.neAddrForCV(Addr{Seg: p.Addr.Seg, Off: uint32(p.Addr.Off)})
				if err != nil {
					return nil, err
				}
				// not every global is in alignsyms, but there is often better types/module data there
				if global, ok := sdb.globalsByAddr[addr]; ok {
					// sometimes the alignsyms has more complete type information, like array lengths
					typ := typeResolver.resolve(p.TypInd)
					if t, ok := typ.(*Array); ok && t.Count > 0 {
						global.Type = typ
					}
				} else {
					module := sourceByIMod[imod]
					if module == "" {
						module = GlobalsModule
						if source, ok := sdb.GetSourceForAddr(addr); ok {
							module = source.Source
						}
					}
					sdb.AddGlobal(&GlobalVar{
						Name:   p.Name,
						Addr:   addr,
						Type:   typeResolver.resolve(p.TypInd),
						Module: module,
					})
				}
			}
		}

		// extensive proc information lives in alignsyms
		procs := extractProcedures(stream)
		for _, proc := range procs {
			p, ok := proc.proc.Parsed.(*nb09.SProc16)
			if !ok {
				return nil, fmt.Errorf("parsed proc imod: %d, offset: %04x not *nb09.SProc16", imod, proc.proc.Offset)
			}

			addr, err := loader.neAddrForCV(Addr{Seg: p.Addr.Seg, Off: uint32(p.Addr.Off)})
			if err != nil {
				return nil, err
			}

			source := sourceByIMod[imod]

			f, err := loader.newFunction(addr, p, proc.children, source)
			if err != nil {
				return nil, err
			}
			sdb.AddFunction(f)
		}
	}

	for _, r := range db.GlobalPub.Symbols.Records {
		if r.RecTyp != nb09.S_PUB16 {
			continue
		}
		switch p := r.Parsed.(type) {
		case *nb09.SPub16:
			addr, err := loader.neAddrForCV(Addr{Seg: p.Addr.Seg, Off: uint32(p.Addr.Off)})
			if err != nil {
				continue
			}

			// already have this as a function
			if _, ok := sdb.functionsByAddr[addr]; ok {
				continue
			}
			// already have this as a global
			if _, ok := sdb.globalsByAddr[addr]; ok {
				continue
			}

			sdb.AddPublic(&Public{Addr: addr, Name: p.Name})
		}

	}

	// apply source mappings after sorting
	loader.applySourceMappings()

	// load enums.h/json
	if err := loader.loadEnums(inputDir); err != nil {
		return nil, err
	}

	// load overrides.json and apply them to the symboldb
	if err := loader.applyOverrides(inputDir); err != nil {
		return nil, err
	}

	// apply enum overrides from enums.json
	if err := loader.applyEnumOverrides(); err != nil {
		return nil, err
	}

	// load dependent enum path rules after enum field overrides are available.
	if err := loader.loadDependentEnums(inputDir); err != nil {
		return nil, err
	}

	// load union discriminator rules after enums and overrides are available.
	if err := loader.loadUnions(inputDir); err != nil {
		return nil, err
	}

	slices.SortFunc(sdb.Functions, func(a, b *Function) int {
		return CompareAddr(a.Addr, b.Addr)
	})

	slices.SortFunc(sdb.Globals, func(a, b *GlobalVar) int {
		return CompareAddr(a.Addr, b.Addr)
	})

	slices.SortFunc(sdb.Structs, func(a, b *Struct) int {
		return strings.Compare(a.String(), b.String())
	})

	return sdb, nil
}

// loadUnions loads typed union discriminator rules.
func (l *symboldbLoader) loadUnions(inputDir string) error {
	unionLoader := unionLoader{}
	rules, err := unionLoader.loadUnionRules(filepath.Join(inputDir, "unions.json"), l.sdb)
	if err != nil {
		return err
	}
	l.sdb.UnionRules = rules
	return nil
}

// convert a codeview address to an NE image address
// CodeView NB09 "FRAME" segment numbers align with the NE segment table entries
// (1-based), so you can directly map CV addr {seg,off} -> file offset.
func (l *symboldbLoader) neAddrForCV(addr Addr) (Addr, error) {
	segDesc, ok := l.cvSegMap[addr.Seg]
	if !ok {
		return Addr{}, fmt.Errorf("missing segmap entry for CV address %s", addr)
	}
	if segDesc.Frame == 0 {
		return Addr{}, fmt.Errorf("segmap entry for CV address %s has zero NE frame", addr)
	}
	return Addr{Seg: segDesc.Frame, Off: segDesc.Off + addr.Off}, nil
}

func setStructTypedef(s *Struct, name string) {
	if s == nil || name == "" || name == s.Name {
		return
	}
	if s.Typedef == "" || s.Typedef == s.Name || strings.HasPrefix(s.Typedef, "_") {
		s.Typedef = name
	}
}

func loadSourceByIMod(db *nb09.NB09DB) map[uint16]string {
	sourceByIMod := make(map[uint16]string, len(db.Modules))
	for imod, source := range db.SrcModules {
		for _, file := range source.Files {
			if file.Name == "" {
				continue
			}
			sourceByIMod[imod] = sourceBaseName(file.Name)
			break
		}
	}
	for imod, files := range db.FileIndex.ModToFiles {
		if sourceByIMod[imod] != "" || len(files) == 0 {
			continue
		}
		sourceByIMod[imod] = sourceBaseName(files[0])
	}
	for imod, module := range db.Modules {
		if sourceByIMod[imod] != "" {
			continue
		}
		sourceByIMod[imod] = sourceBaseName(module.Name)
	}
	return sourceByIMod
}

// loadSourceRanges converts sstSrcModule file ranges into NE source ranges.
// These ranges describe source-line coverage and may omit module-owned bytes
// that do not correspond to a source line.
func loadSourceRanges(db *nb09.NB09DB, cvSegMap map[uint16]nb09.SegMapDesc) []SourceRange {
	var sources []SourceRange
	for _, module := range db.SrcModules {
		for _, file := range module.Files {
			source := sourceBaseName(file.Name)
			if source == "" {
				continue
			}
			for _, si := range file.Segs {
				segDesc, ok := cvSegMap[si.Seg]
				if !ok || segDesc.Frame == 0 {
					continue
				}

				// track lines of source for this source range
				lines := make([]SourceLine, 0, len(si.Lines))
				for _, l := range si.Lines {
					lines = append(lines, SourceLine{
						Off:  l.Off,
						Line: l.LineNum,
					})
				}

				sourceRange := SourceRange{
					Source: source,
					Addr:   Addr{Seg: segDesc.Frame, Off: segDesc.Off + si.Start},
					End:    si.End,
					Len:    si.End - si.Start + 1,
					Lines:  lines,
				}
				sources = append(sources, sourceRange)

			}
		}
	}
	return sources
}

// loadModuleSourceRanges converts sstModule segment contributions into NE
// source ranges. These broader ranges are used as fallback ownership for data
// or padding bytes outside sstSrcModule line coverage.
func loadModuleSourceRanges(db *nb09.NB09DB, cvSegMap map[uint16]nb09.SegMapDesc, sourceByIMod map[uint16]string) []SourceRange {
	var sources []SourceRange
	for imod, module := range db.Modules {
		source := sourceByIMod[imod]
		if source == "" {
			continue
		}
		for _, si := range module.SegInfo {
			if si.CB == 0 {
				continue
			}
			segDesc, ok := cvSegMap[si.Seg]
			if !ok || segDesc.Frame == 0 {
				continue
			}
			sources = append(sources, SourceRange{
				Source: source,
				Addr:   Addr{Seg: segDesc.Frame, Off: segDesc.Off + si.Off},
				Len:    si.CB,
			})
		}
	}
	return sources
}

func sourceBaseName(name string) string {
	name = strings.ReplaceAll(name, "\\", "/")
	base := filepath.Base(name)
	ext := filepath.Ext(base)
	if ext != "" {
		base = strings.TrimSuffix(base, ext)
	}
	return base
}

// applySourceMappings applies sourceRanges to each function we've loaded from nb09
func (l *symboldbLoader) applySourceMappings() {
	sourcesByModule := make(map[string][]SourceRange)
	for _, source := range l.sdb.Sources {
		if len(source.Lines) == 0 {
			continue
		}
		sourcesByModule[source.Source] = append(sourcesByModule[source.Source], source)
	}

	for module, funcs := range l.sdb.functionsByModule {
		slices.SortFunc(funcs, func(a, b *Function) int {
			return CompareAddr(a.Addr, b.Addr)
		})

		sources := sourcesByModule[module]
		if len(sources) == 0 {
			continue
		}

		si := 0
		for _, f := range funcs {
			fnStart := f.Addr.Off
			fnEnd := fnStart + uint32(f.Len)

			sf := SourceFile{
				File: module + ".c",
			}

			for si < len(sources) && sources[si].Addr.Seg < f.Addr.Seg {
				si++
			}
			for si < len(sources) && sources[si].Addr.Seg == f.Addr.Seg && sources[si].Addr.Off+sources[si].Len <= fnStart {
				si++
			}

			for j := si; j < len(sources); j++ {
				source := sources[j]
				if source.Addr.Seg != f.Addr.Seg || source.Addr.Off >= fnEnd {
					break
				}

				for _, line := range source.Lines {
					// Ideally SourceLine.Off should already be normalized to NE offset.
					if line.Off < fnStart || line.Off >= fnEnd {
						continue
					}
					sf.Lines = append(sf.Lines, line)
					lineNo := int(line.Line)
					if sf.First == 0 || lineNo < sf.First {
						sf.First = lineNo
					}
					if lineNo > sf.Last {
						sf.Last = lineNo
					}
				}
			}

			f.SourceFile = sf
		}
	}
}

func loadCVSegMap(db *nb09.NB09DB) map[uint16]nb09.SegMapDesc {
	cvSegMap := make(map[uint16]nb09.SegMapDesc, len(db.SegMap.Segs))
	for i, desc := range db.SegMap.Segs {
		cvSegMap[uint16(i+1)] = desc
	}
	return cvSegMap
}

// ---------- Procedure: proc + nested locals ----------

// nb09Proc groups a nb09Proc symbol record with all the symbol records
// that appear inside its scope (between PROC and matching END).
type nb09Proc struct {
	// The proc record itself (SProc16 or SProc32).
	proc nb09.SymRecord
	// All symbol records inside the proc scope, including nested blocks.
	// This includes S_BPREL16, S_REGISTER, S_BLOCK16, S_LABEL16, etc.
	children []nb09.SymRecord
}

// extractProcedures walks a symbol stream and groups each procedure
// (S_LPROC16, S_GPROC16, S_LPROC32, S_GPROC32) with all symbol records
// nested inside its scope (up to the matching S_END).
func extractProcedures(ss nb09.SymStream) []nb09Proc {
	var procs []nb09Proc
	for i := 0; i < len(ss.Records); i++ {
		rec := ss.Records[i]
		switch rec.RecTyp {
		case nb09.S_LPROC16, nb09.S_GPROC16, nb09.S_LPROC32, nb09.S_GPROC32:
			proc := nb09Proc{proc: rec}
			// Collect children until matching S_END.
			// Track nesting depth for nested blocks/withs.
			depth := 1
			for i++; i < len(ss.Records) && depth > 0; i++ {
				child := ss.Records[i]
				switch child.RecTyp {
				case nb09.S_LPROC16, nb09.S_GPROC16, nb09.S_LPROC32, nb09.S_GPROC32, nb09.S_BLOCK16, nb09.S_WITH16:
					depth++
					proc.children = append(proc.children, child)
				case nb09.S_END:
					depth--
					if depth > 0 {
						proc.children = append(proc.children, child)
					}
				default:
					proc.children = append(proc.children, child)
				}
			}
			i-- // back up so the outer loop's i++ is correct
			procs = append(procs, proc)
		}
	}
	return procs
}

// newFunction creates a new function from a SProc16 record and its children
func (l *symboldbLoader) newFunction(addr Addr, p *nb09.SProc16, children []nb09.SymRecord, module string) (*Function, error) {
	f := Function{
		Name:   p.Name,
		Addr:   addr,
		Len:    p.ProcLen,
		Module: module,
		Scopes: []Scope{{ID: 0, ParentID: -1}},
	}
	if typ := l.typeResolver.getType(p.TypInd); typ != nil {
		if sig, ok := typ.(*Function); ok {
			if sig.Name == "" {
				sig.Name = p.Name
			}
			f.Conv = sig.Conv
			f.Ret = sig.Ret
			f.Params = slices.Clone(sig.Params)
		}
	}
	if err := l.loadFunctionScope(&f, children); err != nil {
		return nil, err
	}

	return &f, nil
}

type bpSymbol struct {
	off  int16
	name string
	typ  Type
}

type paramSymbol struct {
	off   int16
	reg   Register
	name  string
	typ   Type
	order int
}

func (l *symboldbLoader) loadFunctionScope(f *Function, children []nb09.SymRecord) error {
	var params []bpSymbol
	var orderedParams []paramSymbol
	curScopeID := 0
	scopeStack := []int{0}
	nextScopeID := 1

	for childIndex, child := range children {
		switch v := child.Parsed.(type) {
		case *nb09.SBlock16:
			addr, err := l.neAddrForCV(Addr{Seg: v.Addr.Seg, Off: uint32(v.Addr.Off)})
			if err != nil {
				return err
			}
			parentID := curScopeID
			scope := Scope{
				ID:       nextScopeID,
				ParentID: parentID,
				Depth:    f.Scopes[parentID].Depth + 1,
				Name:     v.Name,
				Addr:     addr,
				Len:      v.Len,
			}
			f.Scopes = append(f.Scopes, scope)
			curScopeID = nextScopeID
			scopeStack = append(scopeStack, curScopeID)
			nextScopeID++
		case nil:
			if child.RecTyp == nb09.S_END && len(scopeStack) > 1 {
				scopeStack = scopeStack[:len(scopeStack)-1]
				curScopeID = scopeStack[len(scopeStack)-1]
			}
		case *nb09.SBpRel16:
			sym := bpSymbol{
				off:  v.Off,
				name: v.Name,
				typ:  l.typeResolver.getType(v.TypInd),
			}
			// for local vars/params that are function pointer
			// update the function name to be the var name
			if bpfPtr, ok := sym.typ.(*Pointer); ok {
				if bpf, ok := bpfPtr.Elem.(*Function); ok {
					bpf.Name = sym.name
				}
			}
			if v.Off > 0 {
				params = append(params, sym)
				orderedParams = append(orderedParams, paramSymbol{
					off:   v.Off,
					name:  v.Name,
					typ:   sym.typ,
					order: childIndex,
				})
			} else {
				f.Vars = append(f.Vars, FunctionVar{Name: v.Name, Type: sym.typ, BPOffset: int(v.Off), ScopeID: curScopeID})
			}
		case *nb09.SRegister:
			reg, err := registerFromCodeView(v.Reg)
			if err != nil {
				return err
			}
			sym := paramSymbol{
				reg:   reg,
				name:  v.Name,
				typ:   l.typeResolver.getType(v.TypInd),
				order: childIndex,
			}
			if curScopeID == 0 {
				orderedParams = append(orderedParams, sym)
			} else {
				f.Vars = append(f.Vars, FunctionVar{Name: v.Name, Type: sym.typ, Register: reg, ScopeID: curScopeID})
			}
		case *nb09.SLabel16:
			addr, err := l.neAddrForCV(Addr{Seg: v.Addr.Seg, Off: uint32(v.Addr.Off)})
			if err != nil {
				return err
			}
			f.AddLabel(Label{
				Name: v.Name,
				Addr: addr,
			})
		}
	}

	if containsRegisterParam(orderedParams) && len(f.Params) > 0 {
		l.applyOrderedParamSymbols(f, orderedParams)
		return nil
	}

	slices.SortFunc(params, func(a, b bpSymbol) int {
		if f.Conv == CCPascal {
			return cmp.Compare(b.off, a.off)
		}
		return cmp.Compare(a.off, b.off)
	})
	for i, param := range params {
		if i < len(f.Params) {
			f.Params[i].Name = param.name
			f.Params[i].BPOffset = int(param.off)
			if f.Params[i].Type == nil {
				f.Params[i].Type = param.typ
			}
			continue
		}
		f.Params = append(f.Params, FunctionVar{Name: param.name, Type: param.typ, BPOffset: int(param.off)})
	}
	return nil
}

// containsRegisterParam reports whether a procedure has root register symbols among its parameter symbols.
func containsRegisterParam(params []paramSymbol) bool {
	for _, param := range params {
		if param.reg != RegNone {
			return true
		}
	}
	return false
}

// applyOrderedParamSymbols merges CodeView parameter symbols into the function signature.
func (l *symboldbLoader) applyOrderedParamSymbols(f *Function, params []paramSymbol) {
	slices.SortFunc(params, func(a, b paramSymbol) int {
		return cmp.Compare(a.order, b.order)
	})
	stackOffsets := inferredParamStackOffsets(f)
	for i, param := range params {
		if i >= len(f.Params) {
			f.Params = append(f.Params, FunctionVar{Name: param.name, Type: param.typ, BPOffset: int(param.off), Register: param.reg})
			continue
		}
		if param.name != "" {
			f.Params[i].Name = param.name
		}
		if param.typ != nil {
			f.Params[i].Type = param.typ
		}
		if param.off != 0 {
			f.Params[i].BPOffset = int(param.off)
		} else if i < len(stackOffsets) {
			f.Params[i].BPOffset = stackOffsets[i]
		}
		f.Params[i].Register = param.reg
	}
}

// inferredParamStackOffsets returns ABI stack offsets for signature parameters.
func inferredParamStackOffsets(f *Function) []int {
	offsets := make([]int, len(f.Params))
	off := 6
	if f.Conv == CCPascal {
		for i := len(f.Params) - 1; i >= 0; i-- {
			offsets[i] = off
			off += f.Params[i].Words() * 2
		}
		return offsets
	}
	for i := range f.Params {
		offsets[i] = off
		off += f.Params[i].Words() * 2
	}
	return offsets
}

// registerFromCodeView maps a CodeView register identifier to the shared register model.
func registerFromCodeView(reg uint16) (Register, error) {
	switch reg {
	case 1:
		return RegAL, nil
	case 2:
		return RegCL, nil
	case 3:
		return RegDL, nil
	case 4:
		return RegBL, nil
	case 5:
		return RegAH, nil
	case 6:
		return RegCH, nil
	case 7:
		return RegDH, nil
	case 8:
		return RegBH, nil
	case 9:
		return RegAX, nil
	case 10:
		return RegCX, nil
	case 11:
		return RegDX, nil
	case 12:
		return RegBX, nil
	case 13:
		return RegSP, nil
	case 14:
		return RegBP, nil
	case 15:
		return RegSI, nil
	case 16:
		return RegDI, nil
	default:
		return RegNone, fmt.Errorf("unsupported CodeView register id %d", reg)
	}
}

func (l *symboldbLoader) loadEnums(inputDir string) error {

	// load enums.h
	enumLoader := enumLoader{}
	enums, err := enumLoader.loadEnumsFromHeader(filepath.Join(inputDir, "enums.h"))
	if err != nil {
		return err
	}

	rules, err := enumLoader.loadEnumRules(filepath.Join(inputDir, "enums.json"))
	if err != nil {
		return err
	}

	l.sdb.Enums = enums
	l.sdb.EnumRules = rules
	// make sure the typeresolver and sdb knows about these enums
	for _, e := range enums {
		l.typeResolver.registerNamedType(e.Name, e)
		l.sdb.enumsByName[strings.ToLower(e.Name)] = e
	}

	return nil
}

// loadDependentEnums loads dependent enum rules after enum overrides are applied.
func (l *symboldbLoader) loadDependentEnums(inputDir string) error {
	enumLoader := enumLoader{}
	rules, err := enumLoader.loadDependentEnumRules(filepath.Join(inputDir, "enums.json"), l.sdb)
	if err != nil {
		return err
	}
	l.sdb.DependentEnumRules = rules
	return nil
}

func (l *symboldbLoader) applyOverrides(inputDir string) error {

	// load new type and function definitions and apply them to the symboldb
	o := newOverrideDB(l.sdb, l.typeResolver)
	if err := o.load(inputDir); err != nil {
		return err
	}

	for _, rule := range o.typeOverrideRules {
		// update global types
		for _, g := range l.sdb.Globals {
			// apply name
			if rule.name != "" && g.Name == rule.name {
				g.Type = rule.typ
				continue
			}
			// apply prefix
			if rule.prefix != "" && strings.HasPrefix(g.Name, rule.prefix) {
				g.Type = rule.typ
				continue
			}
		}

		// update functions
		for _, f := range l.sdb.Functions {
			for i := range f.Params {
				p := &f.Params[i]
				// apply name
				if rule.name != "" && p.Name == rule.name {
					p.Type = rule.typ
					continue
				}
				// apply prefix
				if rule.prefix != "" && strings.HasPrefix(p.Name, rule.prefix) {
					p.Type = rule.typ
					continue
				}
			}
			for i := range f.Vars {
				v := &f.Vars[i]

				// apply name
				if rule.name != "" && v.Name == rule.name {
					v.Type = rule.typ
					continue
				}
				// apply prefix
				if rule.prefix != "" && strings.HasPrefix(v.Name, rule.prefix) {
					v.Type = rule.typ
					continue
				}
			}
		}
	}

	for _, rule := range o.structFieldOverrideRules {
		for _, s := range l.sdb.Structs {
			if !strings.EqualFold(s.Name, rule.structName) && !strings.EqualFold(s.Typedef, rule.structName) {
				continue
			}
			changed := false
			for i := range s.Fields {
				field := &s.Fields[i]

				// match fieldName or prefix
				if rule.fieldName != "" && !strings.EqualFold(field.Name, rule.fieldName) {
					continue
				}
				if rule.prefix != "" && !strings.HasPrefix(field.Name, rule.prefix) {
					continue
				}

				// don't allow bitfields to be overridden
				if field.Bitfield != nil {
					return fmt.Errorf("can't override struct field %s.%s with %s, it's a bitfield", s.Name, field.Name, rule.typ)
				}
				// don't allow size changes to structs
				if field.Type.Bytes() != rule.typ.Bytes() {
					return fmt.Errorf("can't override struct field %s.%s with %s, size doesn't match", s.Name, field.Name, rule.typ)
				}

				// found a new type!
				field.Type = rule.typ
				changed = true
			}
			if changed {
				s.FinalizeLayout()
			}
		}
	}
	return nil
}

func (l *symboldbLoader) applyEnumOverrides() error {

	for _, rule := range l.sdb.EnumRules {
		typ := l.sdb.GetEnum(rule.EnumName)
		if typ == nil {
			return fmt.Errorf("unable to load enum %s", rule.EnumName)
		}

		if rule.Kind == UseGlobal {
			g := l.sdb.GetGlobal(rule.Name)
			if g == nil {
				return fmt.Errorf("unable to find global %s", rule.Name)
			}
			g.Type = enumWithStorageSize(typ, g.Type)
			continue
		}

		if rule.Kind == UseLocal || rule.Kind == UseParam || rule.Kind == UseCallResult {
			f := l.sdb.GetFunction(rule.FuncName)
			if f == nil {
				return fmt.Errorf("unable to find function %s", rule.FuncName)
			}

			if rule.Kind == UseCallResult && len(rule.WhenArgs) == 0 {
				f.Ret = enumWithStorageSize(typ, f.Ret)
				continue
			}

			if rule.Kind == UseParam {
				for i := range f.Params {
					p := &f.Params[i]
					if p.Name != rule.ParamName {
						continue
					}
					p.Type = enumWithStorageSize(typ, p.Type)
					break
				}
				continue
			}

			if rule.Kind == UseLocal {
				for i := range f.Vars {
					v := &f.Vars[i]
					if v.Name != rule.Name {
						continue
					}
					v.Type = enumWithStorageSize(typ, v.Type)
					break
				}
				continue
			}
		}

		// struct enum overrides
		if rule.Kind == UseField {
			s := l.sdb.GetStruct(rule.StructName)
			if s == nil {
				return fmt.Errorf("unable to find struct %s", rule.StructName)
			}
			changed := false
			for i := range s.Fields {
				f := &s.Fields[i]
				if f.Name != rule.FieldName {
					continue
				}
				f.Type = enumWithStorageSize(typ, f.Type)
				changed = true
				break
			}
			if changed {
				s.FinalizeLayout()
			}
		}
	}

	return nil
}

// enumWithStorageSize returns an enum type whose byte width matches the type it
// annotates.
func enumWithStorageSize(enum *Enum, original Type) *Enum {
	if enum == nil || original == nil || original.Bytes() == enum.Bytes() {
		return enum
	}
	next := *enum
	next.Size = original.Bytes()
	return &next
}
