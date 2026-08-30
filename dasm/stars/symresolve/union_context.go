package symresolve

import (
	"strings"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// UnionSelection records the concrete union member selected for one symbol root.
type UnionSelection struct {
	Root   SymbolPath
	Rule   *typeinfo.UnionVariantRule
	Value  typeinfo.EnumValue
	Member *typeinfo.StructField
}

// UnionContext carries path-sensitive union selections for symbolic resolution.
type UnionContext struct {
	selections     map[string]UnionSelection
	enumSelections map[string]*typeinfo.Enum
}

// NewUnionContext creates an empty union resolution context.
func NewUnionContext() *UnionContext {
	return &UnionContext{
		selections:     make(map[string]UnionSelection),
		enumSelections: make(map[string]*typeinfo.Enum),
	}
}

// Clone returns a detached copy of the context.
func (c *UnionContext) Clone() *UnionContext {
	if c == nil {
		return NewUnionContext()
	}
	out := NewUnionContext()
	for key, selection := range c.selections {
		out.selections[key] = selection
	}
	for key, enumType := range c.enumSelections {
		out.enumSelections[key] = enumType
	}
	return out
}

// Add records one union member selection.
func (c *UnionContext) Add(root SymbolPath, rule *typeinfo.UnionVariantRule, value typeinfo.EnumValue) bool {
	if c == nil || root == nil || rule == nil {
		return false
	}
	member, ok := rule.MemberForValue(value.Value)
	if !ok {
		return false
	}
	key := unionSelectionKey(root, rule.Type)
	selection := UnionSelection{Root: root, Rule: rule, Value: value, Member: member}
	if previous, ok := c.selections[key]; ok && previous.Member == member && previous.Value.Value == value.Value {
		return false
	}
	c.selections[key] = selection
	return true
}

// SelectionFor returns a union member selection for the base path and struct.
func (c *UnionContext) SelectionFor(base SymbolPath, strct *typeinfo.Struct) (UnionSelection, bool) {
	if c == nil || base == nil || strct == nil {
		return UnionSelection{}, false
	}
	selection, ok := c.selections[unionSelectionKey(base, strct)]
	return selection, ok
}

// AddEnum records an enum type selected for an exact symbolic path.
func (c *UnionContext) AddEnum(path SymbolPath, enumType *typeinfo.Enum) bool {
	key := symbolPathSelectionKey(path)
	if c.enumSelections[key] == enumType {
		return false
	}
	c.enumSelections[key] = enumType
	return true
}

// EnumFor returns a path-sensitive enum type selected for an exact symbolic path.
func (c *UnionContext) EnumFor(path SymbolPath) (*typeinfo.Enum, bool) {
	enumType := c.enumSelections[symbolPathSelectionKey(path)]
	return enumType, enumType != nil
}

// Equal reports whether two contexts contain identical selections.
func (c *UnionContext) Equal(other *UnionContext) bool {
	if c == nil || len(c.selections) == 0 && len(c.enumSelections) == 0 {
		return other == nil || len(other.selections) == 0 && len(other.enumSelections) == 0
	}
	if other == nil || len(c.selections) != len(other.selections) || len(c.enumSelections) != len(other.enumSelections) {
		return false
	}
	for key, selection := range c.selections {
		otherSelection, ok := other.selections[key]
		if !ok || selection.Member != otherSelection.Member || selection.Value.Value != otherSelection.Value.Value {
			return false
		}
	}
	for key, enumType := range c.enumSelections {
		if other.enumSelections[key] != enumType {
			return false
		}
	}
	return true
}

// IntersectUnionContexts keeps only selections shared by every context.
func IntersectUnionContexts(contexts []*UnionContext) *UnionContext {
	if len(contexts) == 0 {
		return NewUnionContext()
	}
	out := contexts[0].Clone()
	for key, selection := range out.selections {
		for _, ctx := range contexts[1:] {
			other, ok := ctx.selections[key]
			if !ok || other.Member != selection.Member || other.Value.Value != selection.Value.Value {
				delete(out.selections, key)
				break
			}
		}
	}
	for key, enumType := range out.enumSelections {
		for _, ctx := range contexts[1:] {
			if ctx.enumSelections[key] != enumType {
				delete(out.enumSelections, key)
				break
			}
		}
	}
	return out
}

// unionSelectionKey returns a stable key for a root/type pair.
func unionSelectionKey(root SymbolPath, strct *typeinfo.Struct) string {
	return symbolPathSelectionKey(root) + "|" + strings.ToLower(strct.String())
}

// symbolPathSelectionKey returns a stable key for an exact symbolic path.
func symbolPathSelectionKey(path SymbolPath) string {
	return strings.ToLower(path.CDecl())
}
