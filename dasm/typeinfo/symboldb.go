package typeinfo

import (
	"sort"
	"strings"
)

type SymbolDB struct {
	DGroupFrame uint
	Globals     []*GlobalVar
	Functions   []*Function
	Structs     []*Struct
	Enums       []*Enum
	Publics     []*Public
	Modules     []string
	Sources     []SourceRange

	EnumRules          []*EnumUseRule
	DependentEnumRules []*DependentEnumRule
	UnionRules         *UnionRules

	globalsByName   map[string]*GlobalVar
	globalsByModule map[string][]*GlobalVar
	globalsByAddr   map[Addr]*GlobalVar

	functionsByName   map[string]*Function
	functionsByModule map[string][]*Function
	functionsByAddr   map[Addr]*Function
	functionsBySeg    map[uint16][]*Function

	structsByName map[string]*Struct

	enumsByName map[string]*Enum

	publicsByAddr map[Addr]*Public
	publicsByName map[string]*Public

	sourcesByName map[string]SourceRange
}

// SourceRange describes one source/object module contribution in NE address space.
type SourceRange struct {
	Source string
	Addr   Addr
	End    uint32
	Len    uint32
	Lines  []SourceLine
}

// public symbols we don't otherwise have an address for, like sqrt, _aFlmul, etc
type Public struct {
	Name string
	Addr Addr
}

func (sdb *SymbolDB) GetGlobal(name string) *GlobalVar {
	return sdb.globalsByName[strings.ToLower(name)]
}

func (sdb *SymbolDB) GetGlobalsForModule(module string) []*GlobalVar {
	return sdb.globalsByModule[module]
}

func (sdb *SymbolDB) GetGlobalContaining(addr Addr) (*GlobalVar, int, bool) {
	i := sort.Search(len(sdb.Globals), func(i int) bool {
		g := sdb.Globals[i]
		return g != nil && CompareAddr(g.Addr, addr) > 0
	})
	if i == 0 {
		return nil, 0, false
	}

	g := sdb.Globals[i-1]
	size := g.Type.Bytes()
	start := g.Addr.Off
	end := start + uint32(size)
	if addr.Off >= start && addr.Off < end {
		return g, int(addr.Off - start), true
	}
	return nil, 0, false
}

// AddGlobal adds a global variable to the symboldb.
func (sdb *SymbolDB) AddGlobal(g *GlobalVar) {
	sdb.Globals = append(sdb.Globals, g)
	sdb.globalsByAddr[g.Addr] = g
	sdb.globalsByName[strings.ToLower(g.Name)] = g
	sdb.globalsByModule[g.Module] = append(sdb.globalsByModule[g.Module], g)
}

func (sdb *SymbolDB) GetFunction(name string) *Function {
	return sdb.functionsByName[strings.ToLower(name)]
}

func (sdb *SymbolDB) GetFunctionByAddr(addr Addr) *Function {
	return sdb.functionsByAddr[addr]
}

func (sdb *SymbolDB) GetFunctionsForModule(module string) []*Function {
	return sdb.functionsByModule[module]
}

func (sdb *SymbolDB) GetPublic(name string) *Public {
	return sdb.publicsByName[strings.ToLower(name)]
}

func (sdb *SymbolDB) GetSource(module string) SourceRange {
	return sdb.sourcesByName[module]
}

func (sdb *SymbolDB) GetSourceForAddr(addr Addr) (SourceRange, bool) {
	if sdb == nil {
		return SourceRange{}, false
	}
	var best SourceRange
	found := false
	for _, source := range sdb.Sources {
		if source.Addr.Seg != addr.Seg {
			continue
		}
		start := source.Addr.Off
		end := start + source.Len
		if addr.Off >= start && addr.Off < end {
			if !found || source.Len < best.Len {
				best = source
				found = true
			}
		}
	}
	if found {
		return best, true
	}
	return SourceRange{}, false
}

func (sdb *SymbolDB) GetStruct(name string) *Struct {
	return sdb.structsByName[strings.ToLower(name)]
}

func (sdb *SymbolDB) GetEnum(name string) *Enum {
	return sdb.enumsByName[strings.ToLower(name)]
}

// AddFunction adds a new function to the symboldb
func (sdb *SymbolDB) AddFunction(f *Function) {
	existing := sdb.GetFunction(f.Name)
	if existing != nil {
		// update the existing signature
		existing.Conv = f.Conv
		existing.Ret = f.Ret
		existing.Params = f.Params
		existing.VarArgs = f.VarArgs
		return
	}

	sdb.Functions = append(sdb.Functions, f)
	sdb.functionsByName[strings.ToLower(f.Name)] = f
	sdb.functionsByModule[f.Module] = append(sdb.functionsByModule[f.Module], f)
	sdb.functionsByAddr[f.Addr] = f
	sdb.functionsBySeg[f.Addr.Seg] = append(sdb.functionsBySeg[f.Addr.Seg], f)
}

// AddPublic adds a public symbol to the symboldb.
func (sdb *SymbolDB) AddPublic(p *Public) {
	sdb.Publics = append(sdb.Publics, p)
	sdb.publicsByAddr[p.Addr] = p
	sdb.publicsByName[strings.ToLower(p.Name)] = p
}
