package typeinfo

import (
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"regexp"
	"slices"
	"strings"
)

// win16DefinesJSON groups Win16 SDK #define constants into named families.
// Member values come from the listed SDK headers; a member written as
// "NAME = expr" is an application constant the headers do not define.
type win16DefinesJSON struct {
	Headers  []string          `json:"headers"`
	Families []win16FamilyJSON `json:"families"`
}

// win16FamilyJSON is one named family of Win16 constants. Kind "flags"
// renders combined values as A|B, choosing members greedily in listed order.
type win16FamilyJSON struct {
	Name    string   `json:"name"`
	Kind    string   `json:"kind"`
	Members []string `json:"members"`
}

// reDefine matches an object-like #define; function-like macros have no
// space between the name and its parameter list and are skipped.
var reDefine = regexp.MustCompile(`^\s*#\s*define\s+([A-Za-z_]\w*)\s+(.+)$`)

// loadWin16Defines loads the Win16 constant families from win16defines.json,
// evaluating member values from the SDK headers in inputDir. Every family is
// a #define constant family whose storage defaults to an unsigned integer.
func (l *enumLoader) loadWin16Defines(inputDir string) ([]*Enum, error) {
	path := filepath.Join(inputDir, "win16defines.json")
	data, err := os.ReadFile(path)
	if err != nil {
		return nil, fmt.Errorf("win16 defines %s: %w", path, err)
	}
	var cfg win16DefinesJSON
	if err := json.Unmarshal(data, &cfg); err != nil {
		return nil, fmt.Errorf("win16 defines %s: %w", path, err)
	}

	defines := make(map[string]string)
	for _, header := range cfg.Headers {
		text, err := os.ReadFile(filepath.Join(inputDir, header))
		if err != nil {
			return nil, fmt.Errorf("win16 header %s: %w", header, err)
		}
		parseDefines(string(text), defines)
	}
	values := win16DefineValues{defines: defines, resolved: make(map[string]int)}

	enums := make([]*Enum, 0, len(cfg.Families))
	for _, family := range cfg.Families {
		e := &Enum{Name: family.Name, EnumKind: EnumExact}
		switch family.Kind {
		case "":
		case "flags":
			e.EnumKind = EnumFlags
		default:
			return nil, fmt.Errorf("win16 family %s: unknown kind %q", family.Name, family.Kind)
		}
		for _, member := range family.Members {
			name, expr, custom := strings.Cut(member, "=")
			name = strings.TrimSpace(name)
			var v int
			if custom {
				v, err = values.define(name, strings.TrimSpace(expr))
			} else {
				v, err = values.value(name)
			}
			if err != nil {
				return nil, fmt.Errorf("win16 family %s: %w", family.Name, err)
			}
			e.Values = append(e.Values, EnumValue{Name: name, Value: v})
		}
		e.Storage = UintForWidth(e.Bytes())
		enums = append(enums, e)
	}
	return enums, nil
}

// parseDefines adds each object-like #define in a header to defines. The
// first definition of a name wins.
func parseDefines(text string, defines map[string]string) {
	for line := range strings.SplitSeq(stripCComments(text), "\n") {
		m := reDefine.FindStringSubmatch(line)
		if m == nil {
			continue
		}
		if _, ok := defines[m[1]]; !ok {
			defines[m[1]] = strings.TrimSpace(m[2])
		}
	}
}

// win16DefineValues evaluates SDK #define values on demand, following names
// that refer to other defines.
type win16DefineValues struct {
	defines  map[string]string
	resolved map[string]int
	visiting []string
}

// value evaluates the named SDK define.
func (v *win16DefineValues) value(name string) (int, error) {
	if n, ok := v.resolved[name]; ok {
		return n, nil
	}
	expr, ok := v.defines[name]
	if !ok {
		return 0, fmt.Errorf("%s is not defined in the Win16 headers", name)
	}
	return v.define(name, expr)
}

// define evaluates expr as the value of name and records it, so application
// constants can refer to SDK defines and to each other.
func (v *win16DefineValues) define(name, expr string) (int, error) {
	if slices.Contains(v.visiting, name) {
		return 0, fmt.Errorf("%s is defined in terms of itself", name)
	}
	v.visiting = append(v.visiting, name)
	n, err := evalConstExpr(expr, v.value)
	v.visiting = v.visiting[:len(v.visiting)-1]
	if err != nil {
		return 0, fmt.Errorf("%s = %s: %w", name, expr, err)
	}
	v.resolved[name] = n
	return n, nil
}
