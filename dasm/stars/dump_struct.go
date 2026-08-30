package stars

import (
	"fmt"
	"io"
	"sort"
	"strings"

	startemplates "github.com/sirgwain/stars-asm/dasm/stars/templates"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpStruct prints a C-style declaration for a struct.
func DumpStruct(w io.Writer, s *typeinfo.Struct, opt DumpOptions) error {
	return startemplates.RenderStructDetail(w, startemplates.NewStructDetailView(s, opt))
}

// DumpAllStructs prints structs.h contents for every known struct, with by-value
// dependencies emitted before the structs that contain them.
func DumpAllStructs(w io.Writer, sdb *typeinfo.SymbolDB, opt DumpOptions) error {
	structs, err := orderedDumpStructs(sdb)
	if err != nil {
		return err
	}
	return startemplates.RenderDumpStructs(w, startemplates.NewDumpStructsView(structs, opt))
}

func orderedDumpStructs(sdb *typeinfo.SymbolDB) ([]*typeinfo.Struct, error) {
	if sdb == nil {
		return nil, fmt.Errorf("nil symbol database")
	}

	structs, err := topoSortStructs(sdb.Structs)
	if err != nil {
		return nil, err
	}
	filtered := make([]*typeinfo.Struct, 0, len(structs))
	for _, s := range structs {
		if s.IsExternalWindowsStruct() {
			continue
		}
		filtered = append(filtered, s)
	}
	return filtered, nil
}

type structDumpNode struct {
	ord  int
	deps map[*typeinfo.Struct]struct{}
}

func topoSortStructs(structs []*typeinfo.Struct) ([]*typeinfo.Struct, error) {
	nodes := make(map[*typeinfo.Struct]*structDumpNode)
	byName := make(map[string]*typeinfo.Struct)
	order := make([]*typeinfo.Struct, 0, len(structs))
	for _, s := range structs {
		if s == nil {
			continue
		}
		if _, ok := nodes[s]; ok {
			continue
		}
		nodes[s] = &structDumpNode{
			ord:  len(order),
			deps: make(map[*typeinfo.Struct]struct{}),
		}
		order = append(order, s)
		indexStructName(byName, s.Name, s)
		indexStructName(byName, s.Typedef, s)
	}

	for _, s := range order {
		node := nodes[s]
		for _, field := range s.Fields {
			dep, ok := hardStructDep(field.Type, byName)
			if !ok || dep == s || nodes[dep] == nil {
				continue
			}
			node.deps[dep] = struct{}{}
		}
	}

	indegree := make(map[*typeinfo.Struct]int, len(nodes))
	rev := make(map[*typeinfo.Struct][]*typeinfo.Struct)
	for _, s := range order {
		indegree[s] = 0
	}
	for _, s := range order {
		for dep := range nodes[s].deps {
			indegree[s]++
			rev[dep] = append(rev[dep], s)
		}
	}

	ready := make([]*typeinfo.Struct, 0, len(order))
	for _, s := range order {
		if indegree[s] == 0 {
			ready = append(ready, s)
		}
	}
	sort.SliceStable(ready, func(i, j int) bool {
		return nodes[ready[i]].ord < nodes[ready[j]].ord
	})

	sorted := make([]*typeinfo.Struct, 0, len(order))
	for len(ready) > 0 {
		s := ready[0]
		ready = ready[1:]
		sorted = append(sorted, s)
		for _, dst := range rev[s] {
			indegree[dst]--
			if indegree[dst] == 0 {
				ready = append(ready, dst)
				sort.SliceStable(ready, func(i, j int) bool {
					return nodes[ready[i]].ord < nodes[ready[j]].ord
				})
			}
		}
	}

	if len(sorted) != len(order) {
		var stuck []string
		for _, s := range order {
			if indegree[s] > 0 {
				stuck = append(stuck, s.String())
			}
		}
		sort.Strings(stuck)
		return nil, fmt.Errorf("cannot emit structs in dependency order: hard by-value cycle involving %s", strings.Join(stuck, ", "))
	}
	return sorted, nil
}

func indexStructName(byName map[string]*typeinfo.Struct, name string, s *typeinfo.Struct) {
	name = strings.ToLower(strings.TrimSpace(name))
	if name == "" {
		return
	}
	byName[name] = s
}

func hardStructDep(typ typeinfo.Type, byName map[string]*typeinfo.Struct) (*typeinfo.Struct, bool) {
	switch t := typ.(type) {
	case *typeinfo.Struct:
		return t, true
	case *typeinfo.Array:
		return hardStructDep(t.Elem, byName)
	default:
		return nil, false
	}
}


