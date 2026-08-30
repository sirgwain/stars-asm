package typeinfo

// UnionRules stores typed union discriminator and path fact rules.
type UnionRules struct {
	Variants            []*UnionVariantRule
	FunctionPathFacts   []*UnionFunctionPathFact
	CallResultPathFacts []*UnionCallResultPathFact

	variantsByType             map[string]*UnionVariantRule
	functionPathFactsByFunc    map[string][]*UnionFunctionPathFact
	callResultPathFactsByFunc  map[string][]*UnionCallResultPathFact
	callResultPathFactsByParam map[string][]*UnionCallResultPathFact
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
	Func  *Function
	Root  string
	Type  *Struct
	Path  []string
	Enum  *Enum
	Value EnumValue
	Rule  *UnionVariantRule
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

// MemberForValue returns the union member selected by an enum value.
func (r *UnionVariantRule) MemberForValue(value int) (*StructField, bool) {
	if r == nil {
		return nil, false
	}
	member := r.ValueToMember[value]
	return member, member != nil
}
