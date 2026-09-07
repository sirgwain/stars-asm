package typeinfo

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"slices"
	"strconv"
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
	rules.BlockPathFacts = make([]*UnionBlockPathFact, 0)
	rules.CallResultPathFacts = make([]*UnionCallResultPathFact, 0, len(cfg.CallResultPathFacts))
	rules.ExternalDiscriminatorAliases = make([]*UnionExternalDiscriminatorAlias, 0, len(cfg.ExternalDiscriminatorAliases))
	rules.ConditionalSelectionFacts = make([]*UnionConditionalSelectionFact, 0, len(cfg.ConditionalSelectionFacts))

	for _, variantJSON := range cfg.UnionVariants {
		variant, err := l.parseUnionVariant(variantJSON, sdb)
		if err != nil {
			return nil, err
		}
		rules.Variants = append(rules.Variants, variant)
		rules.variantsByType[typeLookupName(variant.Type)] = variant
	}

	for _, factJSON := range cfg.FunctionPathFacts {
		if err := l.appendFunctionPathFact(factJSON, sdb, rules); err != nil {
			return nil, err
		}
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

	for _, aliasJSON := range cfg.ExternalDiscriminatorAliases {
		alias, err := l.parseExternalDiscriminatorAlias(aliasJSON, sdb, rules)
		if err != nil {
			return nil, err
		}
		rules.ExternalDiscriminatorAliases = append(rules.ExternalDiscriminatorAliases, alias)
		name := funcLookupName(alias.Func.Name)
		rules.externalDiscriminatorAliasesByFunc[name] = append(rules.externalDiscriminatorAliasesByFunc[name], alias)
	}

	for _, factJSON := range cfg.ConditionalSelectionFacts {
		fact, err := l.parseConditionalSelectionFact(factJSON, sdb, rules)
		if err != nil {
			return nil, err
		}
		rules.ConditionalSelectionFacts = append(rules.ConditionalSelectionFacts, fact)
		name := funcLookupName(fact.Func.Name)
		rules.conditionalSelectionFactsByFunc[name] = append(rules.conditionalSelectionFactsByFunc[name], fact)
	}

	return rules, nil
}

// appendUnionFunctionPathFacts loads and appends function path facts from one union extension file.
func (l *unionLoader) appendUnionFunctionPathFacts(path string, sdb *SymbolDB, rules *UnionRules) error {
	f, err := os.Open(path)
	if err != nil {
		return fmt.Errorf("union rules %s: %w", path, err)
	}
	defer f.Close()

	data, err := io.ReadAll(f)
	if err != nil {
		return fmt.Errorf("union rules read %s: %w", path, err)
	}
	var cfg unionConfigJSON
	dec := json.NewDecoder(bytes.NewReader(data))
	if err := dec.Decode(&cfg); err != nil {
		return fmt.Errorf("union rules parse %s: %w", path, err)
	}
	if len(cfg.UnionVariants) != 0 || len(cfg.CallResultPathFacts) != 0 || len(cfg.ExternalDiscriminatorAliases) != 0 || len(cfg.ConditionalSelectionFacts) != 0 {
		return fmt.Errorf("union extension %s may only contain function_path_facts", path)
	}
	for _, factJSON := range cfg.FunctionPathFacts {
		if err := l.appendFunctionPathFact(factJSON, sdb, rules); err != nil {
			return err
		}
	}
	return nil
}

// appendFunctionPathFact resolves and indexes one function-level or block-level path fact declaration.
func (l *unionLoader) appendFunctionPathFact(factJSON FunctionPathFactJSON, sdb *SymbolDB, rules *UnionRules) error {
	if factJSON.Value == "" && len(factJSON.BlockPathFacts) == 0 {
		return fmt.Errorf("union function fact %s has neither a function value nor block facts", factJSON.Func)
	}
	if factJSON.Value != "" {
		fact, err := l.parseFunctionPathFact(factJSON, sdb, rules)
		if err != nil {
			return err
		}
		rules.FunctionPathFacts = append(rules.FunctionPathFacts, fact)
		name := funcLookupName(fact.Func.Name)
		rules.functionPathFactsByFunc[name] = append(rules.functionPathFactsByFunc[name], fact)
	}
	for _, blockJSON := range factJSON.BlockPathFacts {
		fact, err := l.parseBlockPathFact(factJSON, blockJSON, sdb, rules)
		if err != nil {
			return err
		}
		rules.BlockPathFacts = append(rules.BlockPathFacts, fact)
		name := funcLookupName(fact.Func.Name)
		if rules.blockPathFactsByFunc[name] == nil {
			rules.blockPathFactsByFunc[name] = make(map[uint32][]*UnionBlockPathFact)
		}
		for _, existing := range rules.blockPathFactsByFunc[name][fact.BlockOff] {
			if existing.Root == fact.Root && existing.Type == fact.Type && existing.AllElements == fact.AllElements {
				return fmt.Errorf("duplicate union block fact for %s %s root %s", fact.Func.Name, blockJSON.Block, fact.Root)
			}
		}
		rules.blockPathFactsByFunc[name][fact.BlockOff] = append(rules.blockPathFactsByFunc[name][fact.BlockOff], fact)
	}
	return nil
}

// emptyUnionRules creates an initialized empty union rule set.
func emptyUnionRules() *UnionRules {
	return &UnionRules{
		Variants:                           []*UnionVariantRule{},
		FunctionPathFacts:                  []*UnionFunctionPathFact{},
		BlockPathFacts:                     []*UnionBlockPathFact{},
		CallResultPathFacts:                []*UnionCallResultPathFact{},
		ExternalDiscriminatorAliases:       []*UnionExternalDiscriminatorAlias{},
		ConditionalSelectionFacts:          []*UnionConditionalSelectionFact{},
		variantsByType:                     make(map[string]*UnionVariantRule),
		functionPathFactsByFunc:            make(map[string][]*UnionFunctionPathFact),
		blockPathFactsByFunc:               make(map[string]map[uint32][]*UnionBlockPathFact),
		callResultPathFactsByFunc:          make(map[string][]*UnionCallResultPathFact),
		callResultPathFactsByParam:         make(map[string][]*UnionCallResultPathFact),
		externalDiscriminatorAliasesByFunc: make(map[string][]*UnionExternalDiscriminatorAlias),
		conditionalSelectionFactsByFunc:    make(map[string][]*UnionConditionalSelectionFact),
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
func (l *unionLoader) parseFunctionPathFact(cfg FunctionPathFactJSON, sdb *SymbolDB, rules *UnionRules) (*UnionFunctionPathFact, error) {
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
	if variant.Enum != enum {
		return nil, fmt.Errorf("union function fact %s enum %s does not match %s variant enum %s", cfg.Func, cfg.Enum, cfg.Type, variant.Enum.Name)
	}
	if !slices.Equal(cfg.Path, variant.Discriminator) {
		return nil, fmt.Errorf("union function fact %s path %s does not match %s discriminator %s", cfg.Func, strings.Join(cfg.Path, "."), cfg.Type, strings.Join(variant.Discriminator, "."))
	}
	if _, ok := variant.MemberForValue(value.Value); !ok {
		return nil, fmt.Errorf("union function fact %s value %s has no member mapping", cfg.Func, cfg.Value)
	}
	rootType, ok := functionOrGlobalRootType(fn, sdb, cfg.Root)
	if !ok {
		return nil, fmt.Errorf("union function fact %s root %s not found", cfg.Func, cfg.Root)
	}
	rootStruct, ok := conditionalSelectionRootType(rootType, cfg.AllElements)
	if !ok || rootStruct != strct {
		return nil, fmt.Errorf("union function fact %s root %s is %s, not %s", cfg.Func, cfg.Root, rootType, cfg.Type)
	}

	return &UnionFunctionPathFact{
		Func:        fn,
		Root:        cfg.Root,
		AllElements: cfg.AllElements,
		Type:        strct,
		Path:        append([]string(nil), cfg.Path...),
		Enum:        enum,
		Value:       value,
		Rule:        variant,
	}, nil
}

// parseBlockPathFact resolves a nested function block union fact.
func (l *unionLoader) parseBlockPathFact(parent FunctionPathFactJSON, cfg BlockPathFactJSON, sdb *SymbolDB, rules *UnionRules) (*UnionBlockPathFact, error) {
	blockOff, err := parseUnionBlockOffset(cfg.Block)
	if err != nil {
		return nil, fmt.Errorf("union block fact %s block %q: %w", parent.Func, cfg.Block, err)
	}
	factJSON := parent
	factJSON.Value = cfg.Value
	factJSON.BlockPathFacts = nil
	fact, err := l.parseFunctionPathFact(factJSON, sdb, rules)
	if err != nil {
		return nil, err
	}
	return &UnionBlockPathFact{
		Func:        fact.Func,
		BlockOff:    blockOff,
		Root:        fact.Root,
		AllElements: fact.AllElements,
		Type:        fact.Type,
		Path:        append([]string(nil), fact.Path...),
		Enum:        fact.Enum,
		Value:       fact.Value,
		Rule:        fact.Rule,
	}, nil
}

// parseUnionBlockOffset parses a source block label such as L_5def.
func parseUnionBlockOffset(label string) (uint32, error) {
	value := strings.TrimSpace(label)
	lower := strings.ToLower(value)
	switch {
	case strings.HasPrefix(lower, "l_"):
		value = value[2:]
	case strings.HasPrefix(lower, "0x"):
		value = value[2:]
	}
	if value == "" {
		return 0, fmt.Errorf("empty block offset")
	}
	parsed, err := strconv.ParseUint(value, 16, 32)
	if err != nil {
		return 0, fmt.Errorf("invalid hexadecimal block offset: %w", err)
	}
	return uint32(parsed), nil
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

// parseExternalDiscriminatorAlias resolves a function discriminator alias rule.
func (l *unionLoader) parseExternalDiscriminatorAlias(cfg externalDiscriminatorAliasJSON, sdb *SymbolDB, rules *UnionRules) (*UnionExternalDiscriminatorAlias, error) {
	fn := sdb.GetFunction(cfg.Func)
	if fn == nil {
		return nil, fmt.Errorf("union external discriminator alias function %s not found", cfg.Func)
	}
	if len(cfg.Source) == 0 {
		return nil, fmt.Errorf("union external discriminator alias %s has empty source path", cfg.Func)
	}
	if cfg.Root == "" {
		return nil, fmt.Errorf("union external discriminator alias %s has empty root", cfg.Func)
	}
	strct := sdb.GetStruct(cfg.Type)
	if strct == nil {
		return nil, fmt.Errorf("union external discriminator alias type %s not found for %s", cfg.Type, cfg.Func)
	}
	enum := sdb.GetEnum(cfg.Enum)
	if enum == nil {
		return nil, fmt.Errorf("union external discriminator alias enum %s not found for %s", cfg.Enum, cfg.Func)
	}
	variant, ok := rules.UnionVariantForType(strct)
	if !ok {
		return nil, fmt.Errorf("union external discriminator alias type %s has no variant rule", cfg.Type)
	}
	sourceType, ok := functionPathType(fn, cfg.Source)
	if !ok {
		return nil, fmt.Errorf("union external discriminator alias %s source path %s not found", cfg.Func, strings.Join(cfg.Source, "."))
	}
	if sourceType != enum {
		return nil, fmt.Errorf("union external discriminator alias %s source path %s is %s, not %s", cfg.Func, strings.Join(cfg.Source, "."), sourceType, cfg.Enum)
	}
	rootType, ok := functionRootType(fn, cfg.Root)
	if !ok {
		return nil, fmt.Errorf("union external discriminator alias %s target root %s not found", cfg.Func, cfg.Root)
	}
	rootStruct, ok := namedStructType(rootType)
	if !ok || rootStruct != strct {
		return nil, fmt.Errorf("union external discriminator alias %s target root %s is %s, not %s", cfg.Func, cfg.Root, rootType, cfg.Type)
	}
	return &UnionExternalDiscriminatorAlias{
		Func:   fn,
		Source: append([]string(nil), cfg.Source...),
		Root:   cfg.Root,
		Type:   strct,
		Enum:   enum,
		Rule:   variant,
	}, nil
}

// parseConditionalSelectionFact resolves a branch-conditioned union selection rule.
func (l *unionLoader) parseConditionalSelectionFact(cfg conditionalSelectionFactJSON, sdb *SymbolDB, rules *UnionRules) (*UnionConditionalSelectionFact, error) {
	fn := sdb.GetFunction(cfg.Func)
	if fn == nil {
		return nil, fmt.Errorf("union conditional selection function %s not found", cfg.Func)
	}
	if len(cfg.Source) == 0 {
		return nil, fmt.Errorf("union conditional selection %s has empty source path", cfg.Func)
	}
	if cfg.Root == "" {
		return nil, fmt.Errorf("union conditional selection %s has empty target root", cfg.Func)
	}
	sourceEnum := sdb.GetEnum(cfg.SourceEnum)
	if sourceEnum == nil {
		return nil, fmt.Errorf("union conditional selection source enum %s not found for %s", cfg.SourceEnum, cfg.Func)
	}
	sourceType, ok := functionOrGlobalPathType(fn, sdb, cfg.Source)
	if !ok {
		return nil, fmt.Errorf("union conditional selection %s source path %s not found", cfg.Func, strings.Join(cfg.Source, "."))
	}
	if sourceType != sourceEnum {
		return nil, fmt.Errorf("union conditional selection %s source path %s is %s, not %s", cfg.Func, strings.Join(cfg.Source, "."), sourceType, cfg.SourceEnum)
	}
	sourceValue, ok := enumValueByName(sourceEnum, cfg.SourceValue)
	if !ok {
		return nil, fmt.Errorf("union conditional selection %s source value %s not found in %s", cfg.Func, cfg.SourceValue, cfg.SourceEnum)
	}
	strct := sdb.GetStruct(cfg.Type)
	if strct == nil {
		return nil, fmt.Errorf("union conditional selection type %s not found for %s", cfg.Type, cfg.Func)
	}
	variant, ok := rules.UnionVariantForType(strct)
	if !ok {
		return nil, fmt.Errorf("union conditional selection type %s has no variant rule", cfg.Type)
	}
	value, ok := enumValueByName(variant.Enum, cfg.Value)
	if !ok {
		return nil, fmt.Errorf("union conditional selection %s target value %s not found in %s", cfg.Func, cfg.Value, variant.Enum.Name)
	}
	if _, ok := variant.MemberForValue(value.Value); !ok {
		return nil, fmt.Errorf("union conditional selection %s target value %s has no member mapping", cfg.Func, cfg.Value)
	}
	rootType, ok := functionOrGlobalRootType(fn, sdb, cfg.Root)
	if !ok {
		return nil, fmt.Errorf("union conditional selection %s target root %s not found", cfg.Func, cfg.Root)
	}
	rootStruct, ok := conditionalSelectionRootType(rootType, cfg.AllElements)
	if !ok || rootStruct != strct {
		return nil, fmt.Errorf("union conditional selection %s target root %s is %s, not %s", cfg.Func, cfg.Root, rootType, cfg.Type)
	}
	return &UnionConditionalSelectionFact{
		Func:        fn,
		Source:      append([]string(nil), cfg.Source...),
		SourceEnum:  sourceEnum,
		SourceValue: sourceValue,
		Root:        cfg.Root,
		AllElements: cfg.AllElements,
		Type:        strct,
		Value:       value,
		Rule:        variant,
	}, nil
}

// functionOrGlobalRootType resolves a function symbol or global root type.
func functionOrGlobalRootType(fn *Function, sdb *SymbolDB, name string) (Type, bool) {
	if typ, ok := functionRootType(fn, name); ok {
		return typ, true
	}
	global := sdb.GetGlobal(name)
	if global == nil {
		return nil, false
	}
	return global.Type, true
}

// functionOrGlobalPathType resolves a field path rooted in a function symbol or global.
func functionOrGlobalPathType(fn *Function, sdb *SymbolDB, components []string) (Type, bool) {
	if len(components) == 0 {
		return nil, false
	}
	typ, ok := functionOrGlobalRootType(fn, sdb, components[0])
	if !ok {
		return nil, false
	}
	for _, name := range components[1:] {
		strct, ok := namedStructType(typ)
		if !ok {
			return nil, false
		}
		field := structFieldByName(strct, name)
		if field == nil {
			return nil, false
		}
		typ = field.Type
	}
	return typ, true
}

// conditionalSelectionRootType returns the selected root or collection element struct.
func conditionalSelectionRootType(typ Type, allElements bool) (*Struct, bool) {
	if !allElements {
		return namedStructType(typ)
	}
	switch t := typ.(type) {
	case *Pointer:
		strct, ok := t.Elem.(*Struct)
		return strct, ok
	case *Array:
		strct, ok := t.Elem.(*Struct)
		return strct, ok
	default:
		return nil, false
	}
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

// functionRootType returns the type of a parameter or local in fn.
func functionRootType(fn *Function, name string) (Type, bool) {
	for i := range fn.Params {
		if fn.Params[i].Name == name {
			return fn.Params[i].Type, true
		}
	}
	for i := range fn.Vars {
		if fn.Vars[i].Name == name {
			return fn.Vars[i].Type, true
		}
	}
	return nil, false
}

// functionPathType resolves the final type of a parameter or local path.
func functionPathType(fn *Function, components []string) (Type, bool) {
	if len(components) == 0 {
		return nil, false
	}
	typ, ok := functionRootType(fn, components[0])
	if !ok {
		return nil, false
	}
	for _, name := range components[1:] {
		strct, ok := namedStructType(typ)
		if !ok {
			return nil, false
		}
		field := structFieldByName(strct, name)
		if field == nil {
			return nil, false
		}
		typ = field.Type
	}
	return typ, true
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
	UnionVariants                []unionVariantJSON               `json:"union_variants"`
	FunctionPathFacts            []FunctionPathFactJSON           `json:"function_path_facts"`
	CallResultPathFacts          []callResultPathFactJSON         `json:"call_result_path_facts"`
	ExternalDiscriminatorAliases []externalDiscriminatorAliasJSON `json:"external_discriminator_aliases"`
	ConditionalSelectionFacts    []conditionalSelectionFactJSON   `json:"conditional_selection_facts"`
}

type unionVariantJSON struct {
	Type          string            `json:"type"`            // struct type name, e.g. "_part"
	Discriminator []string          `json:"discriminator"`   // field path, e.g. ["hs","grhst"]
	Enum          string            `json:"enum"`            // enum name for documentation
	DefaultMember string            `json:"default_member"`  // fallback member when union context has no path selection
	ValueToMember map[string]string `json:"value_to_member"` // enum-value-name → member-name
}

// FunctionPathFactJSON is the serialized form of function- and block-scoped union facts.
type FunctionPathFactJSON struct {
	Func           string              `json:"func"`
	Root           string              `json:"root"`
	AllElements    bool                `json:"all_elements"`
	Type           string              `json:"type"`
	Path           []string            `json:"path"`
	Enum           string              `json:"enum"`
	Value          string              `json:"value,omitempty"`
	BlockPathFacts []BlockPathFactJSON `json:"block_path_facts"`
}

// BlockPathFactJSON is the serialized discriminator value for one function block.
type BlockPathFactJSON struct {
	Block string `json:"block"`
	Value string `json:"value"`
}

type callResultPathFactJSON struct {
	Func      string   `json:"func"`
	Type      string   `json:"type"`
	Path      []string `json:"path"`
	FromParam string   `json:"from_param"`
	FromArg   int      `json:"from_arg"`
	Enum      string   `json:"enum"`
}

type externalDiscriminatorAliasJSON struct {
	Func   string   `json:"func"`
	Source []string `json:"source"`
	Root   string   `json:"root"`
	Type   string   `json:"type"`
	Enum   string   `json:"enum"`
}

type conditionalSelectionFactJSON struct {
	Func        string   `json:"func"`
	Source      []string `json:"source"`
	SourceEnum  string   `json:"source_enum"`
	SourceValue string   `json:"source_value"`
	Root        string   `json:"root"`
	AllElements bool     `json:"all_elements"`
	Type        string   `json:"type"`
	Value       string   `json:"value"`
}
