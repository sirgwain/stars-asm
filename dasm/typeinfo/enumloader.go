package typeinfo

import (
	"bufio"
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"regexp"
	"strconv"
	"strings"
)

type enumLoader struct {
}

// loadEnumsFromHeader parses a C header file for typedef enum definitions
// and loads them into the SymbolDB.
func (l *enumLoader) loadEnumsFromHeader(path string) ([]*Enum, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, fmt.Errorf("enum header %s: %w", path, err)
	}
	defer f.Close()

	data, err := io.ReadAll(f)
	if err != nil {
		return nil, fmt.Errorf("enum header read: %w", err)
	}
	eds, err := parseHeaderEnums(string(data))
	if err != nil {
		return nil, err
	}
	return eds, nil
}

// loadEnumConfig reads enum symbolic configuration from JSON.
func (l *enumLoader) loadEnumConfig(path string) (symbolicConfigJSON, error) {
	f, err := os.Open(path)
	if err != nil {
		return symbolicConfigJSON{}, fmt.Errorf("enum header %s: %w", path, err)
	}
	defer f.Close()

	data, err := io.ReadAll(f)
	if err != nil {
		return symbolicConfigJSON{}, fmt.Errorf("enum header read: %w", err)
	}

	var cfg symbolicConfigJSON
	dec := json.NewDecoder(bytes.NewReader(data))
	if err := dec.Decode(&cfg); err != nil {
		return symbolicConfigJSON{}, fmt.Errorf("symbolic values parse %s: %w", path, err)
	}
	return cfg, nil
}

// loadEnumRules loads enum use rules from JSON.
func (l *enumLoader) loadEnumRules(path string) ([]*EnumUseRule, error) {
	cfg, err := l.loadEnumConfig(path)
	if err != nil {
		return nil, err
	}
	uses := make([]*EnumUseRule, 0, len(cfg.Uses))
	for _, u := range cfg.Uses {
		r := parseUseRuleJSON(u)
		if r == nil {
			return nil, fmt.Errorf("failed to load enum use kind: %s, enum: %s, func: %s, struct: %s", u.Kind, u.Enum, u.Func, u.Struct)
		}
		uses = append(uses, r)
	}
	return uses, nil
}

// loadDependentEnumRules loads dependent enum path rules from JSON.
func (l *enumLoader) loadDependentEnumRules(path string, sdb *SymbolDB) ([]*DependentEnumRule, error) {
	cfg, err := l.loadEnumConfig(path)
	if err != nil {
		return nil, err
	}
	dependent := make([]*DependentEnumRule, 0, len(cfg.DependentEnums))
	for _, ruleJSON := range cfg.DependentEnums {
		rule, err := parseDependentEnumRuleJSON(ruleJSON, sdb)
		if err != nil {
			return nil, err
		}
		dependent = append(dependent, rule)
	}
	return dependent, nil
}

var reTypedefEnum = regexp.MustCompile(`(?s)\btypedef\s+enum\b\s*(?:[A-Za-z_][A-Za-z0-9_]*\s*)?\{(.*?)\}\s*([A-Za-z_][A-Za-z0-9_]*)\s*;`)

func parseHeaderEnums(text string) ([]*Enum, error) {
	text = stripCComments(text)
	ms := reTypedefEnum.FindAllStringSubmatch(text, -1)
	if len(ms) == 0 {
		return nil, nil
	}
	result := make([]*Enum, 0, len(ms))
	for _, m := range ms {
		body := m[1]
		name := m[2]
		vals, err := parseEnumBody(body)
		if err != nil {
			return nil, fmt.Errorf("enum %s: %w", name, err)
		}
		result = append(result, &Enum{
			Name:     name,
			EnumKind: inferEnumKind(vals),
			Values:   vals,
		})
	}
	return result, nil
}

func parseEnumBody(body string) ([]EnumValue, error) {
	parts := strings.Split(body, ",")
	vals := []EnumValue{}
	resolved := make(map[string]int)
	var prev int
	havePrev := false
	for _, p := range parts {
		p = strings.TrimSpace(p)
		if p == "" {
			continue
		}
		name, expr, hasEq := strings.Cut(p, "=")
		name = strings.TrimSpace(name)
		if name == "" {
			continue
		}
		var v int
		if hasEq {
			expr = strings.TrimSpace(expr)
			n, err := evalEnumExpr(expr, resolved)
			if err != nil {
				return nil, fmt.Errorf("bad value for %s: %q: %w", name, expr, err)
			}
			v = n
		} else {
			if !havePrev {
				v = 0
			} else {
				v = prev + 1
			}
		}
		prev, havePrev = v, true
		resolved[name] = v
		vals = append(vals, EnumValue{Name: name, Value: v})
	}
	return vals, nil
}

func evalEnumExpr(expr string, resolved map[string]int) (int, error) {
	expr = trimOuterParens(strings.TrimSpace(expr))
	if expr == "" {
		return 0, fmt.Errorf("empty expression")
	}

	parts := splitTopLevel(expr, '|')
	if len(parts) > 1 {
		var out int
		for _, part := range parts {
			v, err := evalEnumExpr(part, resolved)
			if err != nil {
				return 0, err
			}
			out |= v
		}
		return out, nil
	}

	if v, ok := resolved[expr]; ok {
		return v, nil
	}
	if n, err := strconv.ParseInt(expr, 0, 64); err == nil {
		return int(n), nil
	}
	return 0, fmt.Errorf("unknown token %q", expr)
}

func trimOuterParens(expr string) string {
	expr = strings.TrimSpace(expr)
	for len(expr) >= 2 && expr[0] == '(' && expr[len(expr)-1] == ')' {
		depth := 0
		wrapsWholeExpr := true
		for i, ch := range expr {
			switch ch {
			case '(':
				depth++
			case ')':
				depth--
				if depth < 0 {
					return expr
				}
				if depth == 0 && i != len(expr)-1 {
					wrapsWholeExpr = false
				}
			}
		}
		if depth != 0 || !wrapsWholeExpr {
			return expr
		}
		expr = strings.TrimSpace(expr[1 : len(expr)-1])
	}
	return expr
}

func splitTopLevel(expr string, sep rune) []string {
	var parts []string
	start := 0
	depth := 0
	for i, ch := range expr {
		switch ch {
		case '(':
			depth++
		case ')':
			if depth > 0 {
				depth--
			}
		default:
			if ch == sep && depth == 0 {
				parts = append(parts, strings.TrimSpace(expr[start:i]))
				start = i + 1
			}
		}
	}
	if len(parts) == 0 {
		return []string{strings.TrimSpace(expr)}
	}
	parts = append(parts, strings.TrimSpace(expr[start:]))
	return parts
}

func inferEnumKind(values []EnumValue) EnumKind {
	if len(values) == 0 {
		return EnumExact
	}
	var nonZero, singleBit int
	for v := range values {
		if v == 0 {
			continue
		}
		nonZero++
		if v&(v-1) == 0 {
			singleBit++
		}
	}
	if nonZero > 0 && singleBit*100/nonZero >= 80 {
		return EnumFlags
	}
	max := 0
	for v := range values {
		if v > max {
			max = v
		}
	}
	return EnumExact
}

func stripCComments(s string) string {
	block := regexp.MustCompile(`(?s)/\*.*?\*/`)
	s = block.ReplaceAllString(s, "")
	var out strings.Builder
	sc := bufio.NewScanner(strings.NewReader(s))
	for sc.Scan() {
		line := sc.Text()
		if idx := strings.Index(line, "//"); idx >= 0 {
			line = line[:idx]
		}
		out.WriteString(line)
		out.WriteByte('\n')
	}
	return out.String()
}

type symbolicConfigJSON struct {
	Enums struct {
		Import        string   `json:"import"`
		ImportHeaders []string `json:"import_headers"`
	} `json:"enums"`
	Uses           []useRuleJSON           `json:"uses"`
	DependentEnums []dependentEnumRuleJSON `json:"dependent_enums"`
}

type useRuleJSON struct {
	Kind string `json:"kind"`

	Name string `json:"name"`

	Func  string `json:"func"`
	Param string `json:"param"`

	Struct string `json:"struct"`
	Field  string `json:"field"`

	Enum string              `json:"enum"`
	When []argConstraintJSON `json:"when"`
}

type argConstraintJSON struct {
	Param string `json:"param"`
	Value int    `json:"value"`
}

type dependentEnumRuleJSON struct {
	Type          string            `json:"type"`
	Target        []string          `json:"target"`
	Discriminator []string          `json:"discriminator"`
	EnumByValue   map[string]string `json:"enum_by_value"`
}

func parseUseRuleJSON(u useRuleJSON) *EnumUseRule {
	r := &EnumUseRule{
		Name:       u.Name,
		FuncName:   u.Func,
		ParamName:  u.Param,
		StructName: u.Struct,
		FieldName:  u.Field,
		EnumName:   u.Enum,
	}
	switch strings.ToLower(u.Kind) {
	case "param":
		r.Kind = UseParam
	case "field":
		r.Kind = UseField
	case "global":
		r.Kind = UseGlobal
	case "local":
		r.Kind = UseLocal
	case "call_result":
		r.Kind = UseCallResult
	default:
		return nil
	}
	if len(u.When) != 0 {
		r.WhenArgs = make([]ArgConstraint, 0, len(u.When))
		for _, when := range u.When {
			r.WhenArgs = append(r.WhenArgs, ArgConstraint{
				ParamName: when.Param,
				Value:     when.Value,
			})
		}
	}
	return r
}

// parseDependentEnumRuleJSON resolves a dependent enum JSON record to typed rule data.
func parseDependentEnumRuleJSON(cfg dependentEnumRuleJSON, sdb *SymbolDB) (*DependentEnumRule, error) {
	strct := sdb.GetStruct(cfg.Type)
	if strct == nil {
		return nil, fmt.Errorf("dependent enum type %s not found", cfg.Type)
	}
	if len(cfg.Target) == 0 {
		return nil, fmt.Errorf("dependent enum %s has empty target path", cfg.Type)
	}
	if len(cfg.Discriminator) == 0 {
		return nil, fmt.Errorf("dependent enum %s has empty discriminator path", cfg.Type)
	}
	if _, ok := resolveStructFieldPathForEnum(strct, cfg.Target); !ok {
		return nil, fmt.Errorf("dependent enum %s target %s not found", cfg.Type, strings.Join(cfg.Target, "."))
	}
	discriminator, ok := resolveStructFieldPathForEnum(strct, cfg.Discriminator)
	if !ok {
		return nil, fmt.Errorf("dependent enum %s discriminator %s not found", cfg.Type, strings.Join(cfg.Discriminator, "."))
	}
	discriminatorEnum, ok := discriminator.Type.(*Enum)
	if !ok {
		return nil, fmt.Errorf("dependent enum %s discriminator %s is %s, not enum", cfg.Type, strings.Join(cfg.Discriminator, "."), discriminator.Type)
	}

	enumByValue := make(map[int]*Enum, len(cfg.EnumByValue))
	for valueName, enumName := range cfg.EnumByValue {
		value, ok := enumValueByNameForDependent(discriminatorEnum, valueName)
		if !ok {
			return nil, fmt.Errorf("dependent enum %s discriminator value %s not found in %s", cfg.Type, valueName, discriminatorEnum.Name)
		}
		enumType := resolveDependentTargetEnum(sdb, enumName)
		if enumType == nil {
			return nil, fmt.Errorf("dependent enum %s target enum %s not found", cfg.Type, enumName)
		}
		enumByValue[value.Value] = enumType
	}

	return &DependentEnumRule{
		Type:          strct,
		Target:        append([]string(nil), cfg.Target...),
		Discriminator: append([]string(nil), cfg.Discriminator...),
		EnumByValue:   enumByValue,
	}, nil
}

// resolveDependentTargetEnum returns the enum named or value-prefixed by name.
func resolveDependentTargetEnum(sdb *SymbolDB, name string) *Enum {
	if enumType := sdb.GetEnum(name); enumType != nil {
		return enumType
	}
	for _, enumType := range sdb.Enums {
		if enumUsesValuePrefix(enumType, name) {
			return enumType
		}
	}
	return nil
}

// enumUsesValuePrefix reports whether every named enum value uses prefix.
func enumUsesValuePrefix(enumType *Enum, prefix string) bool {
	if len(enumType.Values) == 0 {
		return false
	}
	for _, value := range enumType.Values {
		if !strings.HasPrefix(value.Name, prefix) {
			return false
		}
	}
	return true
}

// resolveStructFieldPathForEnum resolves a named field path rooted at a struct.
func resolveStructFieldPathForEnum(strct *Struct, path []string) (*StructField, bool) {
	var field *StructField
	current := strct
	for _, name := range path {
		field = structFieldByNameForDependent(current, name)
		if field == nil {
			return nil, false
		}
		next, ok := namedStructTypeForDependent(field.Type)
		if !ok {
			current = nil
			continue
		}
		current = next
	}
	return field, true
}

// structFieldByNameForDependent returns a field by name.
func structFieldByNameForDependent(strct *Struct, name string) *StructField {
	if strct == nil {
		return nil
	}
	for i := range strct.Fields {
		if strct.Fields[i].Name == name {
			return &strct.Fields[i]
		}
	}
	return nil
}

// namedStructTypeForDependent unwraps pointers and returns a struct type.
func namedStructTypeForDependent(typ Type) (*Struct, bool) {
	unwrapped, _ := UnwrapPointer(typ)
	strct, ok := unwrapped.(*Struct)
	return strct, ok
}

// enumValueByNameForDependent returns an enum value by symbolic name.
func enumValueByNameForDependent(enum *Enum, name string) (EnumValue, bool) {
	for _, value := range enum.Values {
		if value.Name == name {
			return value, true
		}
	}
	return EnumValue{}, false
}
