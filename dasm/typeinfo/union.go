package typeinfo

// UnionRules stores typed union discriminator and path fact rules.
type UnionRules struct {
	Variants                     []*UnionVariantRule
	FunctionPathFacts            []*UnionFunctionPathFact
	BlockPathFacts               []*UnionBlockPathFact
	CallResultPathFacts          []*UnionCallResultPathFact
	ExternalDiscriminatorAliases []*UnionExternalDiscriminatorAlias
	ConditionalSelectionFacts    []*UnionConditionalSelectionFact

	variantsByType                     map[string]*UnionVariantRule
	functionPathFactsByFunc            map[string][]*UnionFunctionPathFact
	blockPathFactsByFunc               map[string]map[uint32][]*UnionBlockPathFact
	callResultPathFactsByFunc          map[string][]*UnionCallResultPathFact
	callResultPathFactsByParam         map[string][]*UnionCallResultPathFact
	externalDiscriminatorAliasesByFunc map[string][]*UnionExternalDiscriminatorAlias
	conditionalSelectionFactsByFunc    map[string][]*UnionConditionalSelectionFact
}

// UnionVariantRule maps a discriminator enum value to a concrete union member.
type UnionVariantRule struct {
	Type          *Struct
	Discriminator []string
	Enum          *Enum
	ValueToMember map[int]*StructField
	DefaultMember *StructField
}

// UnionFunctionPathFact records a discriminator value known for a function symbol.
type UnionFunctionPathFact struct {
	Func        *Function
	Root        string
	RootPath    []string
	AllElements bool
	Type        *Struct
	Path        []string
	Enum        *Enum
	Value       EnumValue
	Rule        *UnionVariantRule
}

// UnionBlockPathFact records a union selection known for one function block.
type UnionBlockPathFact struct {
	Func        *Function
	BlockOff    uint32
	Root        string
	RootPath    []string
	AllElements bool
	Type        *Struct
	Path        []string
	Enum        *Enum
	Value       EnumValue
	Rule        *UnionVariantRule
}

// BlockFactsFor returns known union facts for one function block.
func (r *UnionRules) BlockFactsFor(fn *Function, blockOff uint32) []*UnionBlockPathFact {
	if r == nil || fn == nil {
		return nil
	}
	return r.blockPathFactsByFunc[funcLookupName(fn.Name)][blockOff]
}

// UnionCallResultPathFact records a discriminator value copied from a call argument to the call result.
type UnionCallResultPathFact struct {
	Func       *Function
	Type       *Struct
	Path       []string
	FromParam  string
	FromArg    int
	Enum       *Enum
	Rule       *UnionVariantRule
	ParamIndex int
}

// UnionExternalDiscriminatorAlias records a function path that selects a union root.
type UnionExternalDiscriminatorAlias struct {
	Func   *Function
	Source []string
	Root   string
	Type   *Struct
	Enum   *Enum
	Rule   *UnionVariantRule
}

// UnionConditionalSelectionFact selects a union variant when an external enum path has a specific value.
type UnionConditionalSelectionFact struct {
	Func        *Function
	Source      []string
	SourceEnum  *Enum
	SourceValue EnumValue
	Root        string
	AllElements bool
	Type        *Struct
	Value       EnumValue
	Rule        *UnionVariantRule
}

// UnionVariantForType returns the variant rule for the named union-bearing type.
func (r *UnionRules) UnionVariantForType(typ Type) (*UnionVariantRule, bool) {
	if r == nil {
		return nil, false
	}
	strct, ok := namedStructType(typ)
	if !ok {
		return nil, false
	}
	rule := r.variantsByType[typeLookupName(strct)]
	return rule, rule != nil
}

// FunctionFactsFor returns known union facts for a function.
func (r *UnionRules) FunctionFactsFor(fn *Function) []*UnionFunctionPathFact {
	if r == nil || fn == nil {
		return nil
	}
	return r.functionPathFactsByFunc[funcLookupName(fn.Name)]
}

// CallResultFactsFor returns union facts for a call result-producing function.
func (r *UnionRules) CallResultFactsFor(fn *Function) []*UnionCallResultPathFact {
	if r == nil || fn == nil {
		return nil
	}
	return r.callResultPathFactsByFunc[funcLookupName(fn.Name)]
}

// ExternalDiscriminatorAliasesFor returns discriminator aliases for a function.
func (r *UnionRules) ExternalDiscriminatorAliasesFor(fn *Function) []*UnionExternalDiscriminatorAlias {
	if r == nil || fn == nil {
		return nil
	}
	return r.externalDiscriminatorAliasesByFunc[funcLookupName(fn.Name)]
}

// ConditionalSelectionFactsFor returns conditional union selections for a function.
func (r *UnionRules) ConditionalSelectionFactsFor(fn *Function) []*UnionConditionalSelectionFact {
	if r == nil || fn == nil {
		return nil
	}
	return r.conditionalSelectionFactsByFunc[funcLookupName(fn.Name)]
}

// MemberForValue returns the union member selected by an enum value.
func (r *UnionVariantRule) MemberForValue(value int) (*StructField, bool) {
	if r == nil {
		return nil, false
	}
	member := r.ValueToMember[value]
	return member, member != nil
}
