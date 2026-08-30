package typeinfo

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"strings"
)

type unionLoader struct {
}

// loadUnionRules loads union discriminator rules from json.
func (l *unionLoader) loadUnionRules(path string, sdb *SymbolDB) (*UnionRules, error) {

	f, err := os.Open(path)
	if err != nil {
		if os.IsNotExist(err) {
			return emptyUnionRules(), nil
		}
		return nil, fmt.Errorf("union rules %s: %w", path, err)
	}
	defer f.Close()

	data, err := io.ReadAll(f)
	if err != nil {
		return nil, fmt.Errorf("union rules read: %w", err)
	}

	var cfg unionConfigJSON
	dec := json.NewDecoder(bytes.NewReader(data))
	if err := dec.Decode(&cfg); err != nil {
		return nil, fmt.Errorf("union rules parse %s: %w", path, err)
	}

	rules := emptyUnionRules()
	rules.Variants = make([]*UnionVariantRule, 0, len(cfg.UnionVariants))
	rules.FunctionPathFacts = make([]*UnionFunctionPathFact, 0, len(cfg.FunctionPathFacts))
	rules.CallResultPathFacts = make([]*UnionCallResultPathFact, 0, len(cfg.CallResultPathFacts))

	for _, variantJSON := range cfg.UnionVariants {
		variant, err := l.parseUnionVariant(variantJSON, sdb)
		if err != nil {
			return nil, err
		}
		rules.Variants = append(rules.Variants, variant)
		rules.variantsByType[typeLookupName(variant.Type)] = variant
	}

	for _, factJSON := range cfg.FunctionPathFacts {
		fact, err := l.parseFunctionPathFact(factJSON, sdb, rules)
		if err != nil {
			return nil, err
		}
		rules.FunctionPathFacts = append(rules.FunctionPathFacts, fact)
		name := funcLookupName(fact.Func.Name)
		rules.functionPathFactsByFunc[name] = append(rules.functionPathFactsByFunc[name], fact)
	}

	for _, factJSON := range cfg.CallResultPathFacts {
		fact, err := l.parseCallResultPathFact(factJSON, sdb, rules)
		if err != nil {
			return nil, err
		}
		rules.CallResultPathFacts = append(rules.CallResultPathFacts, fact)
		name := funcLookupName(fact.Func.Name)
		rules.callResultPathFactsByFunc[name] = append(rules.callResultPathFactsByFunc[name], fact)
		paramName := funcLookupName(fact.FromParam)
		rules.callResultPathFactsByParam[paramName] = append(rules.callResultPathFactsByParam[paramName], fact)
	}

	return rules, nil
}

// emptyUnionRules creates an initialized empty union rule set.
func emptyUnionRules() *UnionRules {
	return &UnionRules{
		Variants:                   []*UnionVariantRule{},
		FunctionPathFacts:          []*UnionFunctionPathFact{},
		CallResultPathFacts:        []*UnionCallResultPathFact{},
		variantsByType:             make(map[string]*UnionVariantRule),
		functionPathFactsByFunc:    make(map[string][]*UnionFunctionPathFact),
		callResultPathFactsByFunc:  make(map[string][]*UnionCallResultPathFact),
		callResultPathFactsByParam: make(map[string][]*UnionCallResultPathFact),
	}
}

// parseUnionVariant resolves a union variant JSON record to typed rule data.
func (l *unionLoader) parseUnionVariant(cfg unionVariantJSON, sdb *SymbolDB) (*UnionVariantRule, error) {
	strct := sdb.GetStruct(cfg.Type)
	if strct == nil {
		return nil, fmt.Errorf("union variant type %s not found", cfg.Type)
	}
	enum := sdb.GetEnum(cfg.Enum)
	if enum == nil {
		return nil, fmt.Errorf("union variant enum %s not found for %s", cfg.Enum, cfg.Type)
	}
	if len(cfg.Discriminator) == 0 {
		return nil, fmt.Errorf("union variant %s has empty discriminator path", cfg.Type)
	}

	valueToMember := make(map[int]*StructField, len(cfg.ValueToMember))
	for valueName, memberName := range cfg.ValueToMember {
		value, ok := enumValueByName(enum, valueName)
		if !ok {
			return nil, fmt.Errorf("union variant %s enum value %s not found in %s", cfg.Type, valueName, cfg.Enum)
		}
		member := structFieldByName(strct, memberName)
		if member == nil {
			return nil, fmt.Errorf("union variant %s member %s not found", cfg.Type, memberName)
		}
		valueToMember[value.Value] = member
	}
	defaultMember, err := l.parseDefaultUnionMember(cfg, strct)
	if err != nil {
		return nil, err
	}

	return &UnionVariantRule{
		Type:          strct,
		Discriminator: append([]string(nil), cfg.Discriminator...),
		Enum:          enum,
		ValueToMember: valueToMember,
		DefaultMember: defaultMember,
	}, nil
}

// parseDefaultUnionMember resolves the optional fallback union member.
func (l *unionLoader) parseDefaultUnionMember(cfg unionVariantJSON, strct *Struct) (*StructField, error) {
	if cfg.DefaultMember != "" {
		member := structFieldByName(strct, cfg.DefaultMember)
		if member == nil {
			return nil, fmt.Errorf("union variant %s default member %s not found", cfg.Type, cfg.DefaultMember)
		}
		return member, nil
	}
	return nil, nil
}

// parseFunctionPathFact resolves a function path fact JSON record to typed rule data.
func (l *unionLoader) parseFunctionPathFact(cfg functionPathFactJSON, sdb *SymbolDB, rules *UnionRules) (*UnionFunctionPathFact, error) {
	fn := sdb.GetFunction(cfg.Func)
	if fn == nil {
		return nil, fmt.Errorf("union function fact function %s not found", cfg.Func)
	}
	strct := sdb.GetStruct(cfg.Type)
	if strct == nil {
		return nil, fmt.Errorf("union function fact type %s not found for %s", cfg.Type, cfg.Func)
	}
	enum := sdb.GetEnum(cfg.Enum)
	if enum == nil {
		return nil, fmt.Errorf("union function fact enum %s not found for %s", cfg.Enum, cfg.Func)
	}
	value, ok := enumValueByName(enum, cfg.Value)
	if !ok {
		return nil, fmt.Errorf("union function fact value %s not found in %s", cfg.Value, cfg.Enum)
	}
	variant, ok := rules.UnionVariantForType(strct)
	if !ok {
		return nil, fmt.Errorf("union function fact type %s has no variant rule", cfg.Type)
	}
	if _, ok := variant.MemberForValue(value.Value); !ok {
		return nil, fmt.Errorf("union function fact %s value %s has no member mapping", cfg.Func, cfg.Value)
	}

	return &UnionFunctionPathFact{
		Func:  fn,
		Root:  cfg.Root,
		Type:  strct,
		Path:  append([]string(nil), cfg.Path...),
		Enum:  enum,
		Value: value,
		Rule:  variant,
	}, nil
}

// parseCallResultPathFact resolves a call result path fact JSON record to typed rule data.
func (l *unionLoader) parseCallResultPathFact(cfg callResultPathFactJSON, sdb *SymbolDB, rules *UnionRules) (*UnionCallResultPathFact, error) {
	fn := sdb.GetFunction(cfg.Func)
	if fn == nil {
		return nil, fmt.Errorf("union call result fact function %s not found", cfg.Func)
	}
	strct := sdb.GetStruct(cfg.Type)
	if strct == nil {
		return nil, fmt.Errorf("union call result fact type %s not found for %s", cfg.Type, cfg.Func)
	}
	enum := sdb.GetEnum(cfg.Enum)
	if enum == nil {
		return nil, fmt.Errorf("union call result fact enum %s not found for %s", cfg.Enum, cfg.Func)
	}
	variant, ok := rules.UnionVariantForType(strct)
	if !ok {
		return nil, fmt.Errorf("union call result fact type %s has no variant rule", cfg.Type)
	}
	paramIndex := cfg.FromArg
	if cfg.FromParam != "" {
		index, ok := functionParamIndex(fn, cfg.FromParam)
		if !ok {
			return nil, fmt.Errorf("union call result fact %s param %s not found", cfg.Func, cfg.FromParam)
		}
		paramIndex = index
	}
	if paramIndex < 0 || paramIndex >= len(fn.Params) {
		return nil, fmt.Errorf("union call result fact %s arg index %d out of range", cfg.Func, paramIndex)
	}

	return &UnionCallResultPathFact{
		Func:       fn,
		Type:       strct,
		Path:       append([]string(nil), cfg.Path...),
		FromParam:  cfg.FromParam,
		FromArg:    cfg.FromArg,
		Enum:       enum,
		Rule:       variant,
		ParamIndex: paramIndex,
	}, nil
}

// enumValueByName returns an enum value by symbolic name.
func enumValueByName(enum *Enum, name string) (EnumValue, bool) {
	for _, value := range enum.Values {
		if value.Name == name {
			return value, true
		}
	}
	return EnumValue{}, false
}

// structFieldByName returns a field by name.
func structFieldByName(strct *Struct, name string) *StructField {
	for i := range strct.Fields {
		if strct.Fields[i].Name == name {
			return &strct.Fields[i]
		}
	}
	return nil
}

// namedStructType unwraps pointers and returns a named struct type.
func namedStructType(typ Type) (*Struct, bool) {
	unwrapped, _ := UnwrapPointer(typ)
	strct, ok := unwrapped.(*Struct)
	return strct, ok
}

// functionParamIndex returns the parameter index for a named function parameter.
func functionParamIndex(fn *Function, name string) (int, bool) {
	for i := range fn.Params {
		if fn.Params[i].Name == name {
			return i, true
		}
	}
	return 0, false
}

// typeLookupName returns the canonical lookup key for a struct type.
func typeLookupName(strct *Struct) string {
	return strings.ToLower(strct.String())
}

// funcLookupName returns a canonical lookup key for function and parameter names.
func funcLookupName(name string) string {
	return strings.ToLower(name)
}

type unionConfigJSON struct {
	UnionVariants       []unionVariantJSON       `json:"union_variants"`
	FunctionPathFacts   []functionPathFactJSON   `json:"function_path_facts"`
	CallResultPathFacts []callResultPathFactJSON `json:"call_result_path_facts"`
}

type unionVariantJSON struct {
	Type          string            `json:"type"`            // struct type name, e.g. "_part"
	Discriminator []string          `json:"discriminator"`   // field path, e.g. ["hs","grhst"]
	Enum          string            `json:"enum"`            // enum name for documentation
	DefaultMember string            `json:"default_member"`  // fallback member when no discriminator fact is known
	ValueToMember map[string]string `json:"value_to_member"` // enum-value-name → member-name
}

type functionPathFactJSON struct {
	Func  string   `json:"func"`
	Root  string   `json:"root"`
	Type  string   `json:"type"`
	Path  []string `json:"path"`
	Enum  string   `json:"enum"`
	Value string   `json:"value"`
}

type callResultPathFactJSON struct {
	Func      string   `json:"func"`
	Type      string   `json:"type"`
	Path      []string `json:"path"`
	FromParam string   `json:"from_param"`
	FromArg   int      `json:"from_arg"`
	Enum      string   `json:"enum"`
}
