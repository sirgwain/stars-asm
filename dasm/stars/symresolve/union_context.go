package symresolve

import (
	"sort"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// UnionSelection records the concrete union member selected for one symbol root.
type UnionSelection struct {
	Root        SymbolPath
	Rule        *typeinfo.UnionVariantRule
	Value       typeinfo.EnumValue
	Member      *typeinfo.StructField
	AllElements bool
}

// UnionContext carries path-sensitive union selections for symbolic resolution.
type UnionContext struct {
	selections        map[string]UnionSelection
	elementSelections map[string]UnionSelection
	enumSelections    map[string]*typeinfo.Enum
	selectionObserver func(UnionSelection)
}

// NewUnionContext creates an empty union resolution context.
func NewUnionContext() *UnionContext {
	return &UnionContext{
		selections:        make(map[string]UnionSelection),
		elementSelections: make(map[string]UnionSelection),
		enumSelections:    make(map[string]*typeinfo.Enum),
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
	for key, selection := range c.elementSelections {
		out.elementSelections[key] = selection
	}
	for key, enumType := range c.enumSelections {
		out.enumSelections[key] = enumType
	}
	return out
}

// MergeUnionContexts combines a derived context with authoritative configured
// selections. Configured selections replace conflicts while derived selections
// for other paths are preserved.
func MergeUnionContexts(derived, configured *UnionContext) *UnionContext {
	out := derived.Clone()
	if configured == nil {
		return out
	}
	for key, selection := range configured.selections {
		out.selections[key] = selection
	}
	for key, selection := range configured.elementSelections {
		out.elementSelections[key] = selection
	}
	for key, enumType := range configured.enumSelections {
		out.enumSelections[key] = enumType
	}
	return out
}

// AddAllElements records one union member selection for every indexed element of a root.
func (c *UnionContext) AddAllElements(root SymbolPath, rule *typeinfo.UnionVariantRule, value typeinfo.EnumValue) bool {
	if c == nil || root == nil || rule == nil {
		return false
	}
	member, ok := rule.MemberForValue(value.Value)
	if !ok {
		return false
	}
	key := unionSelectionKey(root, rule.Type)
	selection := UnionSelection{Root: root, Rule: rule, Value: value, Member: member, AllElements: true}
	if previous, ok := c.elementSelections[key]; ok && previous.Member == member && previous.Value.Value == value.Value {
		return false
	}
	c.elementSelections[key] = selection
	return true
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
	if !ok {
		if deref, isDeref := base.(*SymbolDeref); isDeref {
			selection, ok = c.selections[unionSelectionKey(deref.Base, strct)]
		}
	}
	if ok {
		if c.selectionObserver != nil {
			c.selectionObserver(selection)
		}
		return selection, true
	}
	root, ok := indexedSelectionRoot(base)
	if !ok {
		return UnionSelection{}, false
	}
	selection, ok = c.elementSelections[unionSelectionKey(root, strct)]
	if ok && c.selectionObserver != nil {
		c.selectionObserver(selection)
	}
	return selection, ok
}

// AllElementsSelectionFor returns a union selection applying to every indexed element of a root.
func (c *UnionContext) AllElementsSelectionFor(root SymbolPath, strct *typeinfo.Struct) (UnionSelection, bool) {
	if c == nil || root == nil || strct == nil {
		return UnionSelection{}, false
	}
	selection, ok := c.elementSelections[unionSelectionKey(root, strct)]
	if ok && c.selectionObserver != nil {
		c.selectionObserver(selection)
	}
	return selection, ok
}

// Selections returns the union member selections carried by the context.
func (c *UnionContext) Selections() []UnionSelection {
	if c == nil {
		return nil
	}
	selections := make([]UnionSelection, 0, len(c.selections)+len(c.elementSelections))
	for _, selection := range c.selections {
		selections = append(selections, selection)
	}
	for _, selection := range c.elementSelections {
		selections = append(selections, selection)
	}
	sort.Slice(selections, func(i, j int) bool {
		left := selections[i]
		right := selections[j]
		if left.Root.String() != right.Root.String() {
			return left.Root.String() < right.Root.String()
		}
		if left.Rule.Type.String() != right.Rule.Type.String() {
			return left.Rule.Type.String() < right.Rule.Type.String()
		}
		if left.AllElements != right.AllElements {
			return !left.AllElements
		}
		return left.Value.Value < right.Value.Value
	})
	return selections
}

// SwapSelectionObserver installs a selection-use observer and returns the previous observer.
func (c *UnionContext) SwapSelectionObserver(observer func(UnionSelection)) func(UnionSelection) {
	previous := c.selectionObserver
	c.selectionObserver = observer
	return previous
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
	if c == nil || len(c.selections) == 0 && len(c.elementSelections) == 0 && len(c.enumSelections) == 0 {
		return other == nil || len(other.selections) == 0 && len(other.elementSelections) == 0 && len(other.enumSelections) == 0
	}
	if other == nil || len(c.selections) != len(other.selections) || len(c.elementSelections) != len(other.elementSelections) || len(c.enumSelections) != len(other.enumSelections) {
		return false
	}
	for key, selection := range c.selections {
		otherSelection, ok := other.selections[key]
		if !ok || selection.Member != otherSelection.Member || selection.Value.Value != otherSelection.Value.Value {
			return false
		}
	}
	for key, selection := range c.elementSelections {
		otherSelection, ok := other.elementSelections[key]
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
	for key, selection := range out.elementSelections {
		for _, ctx := range contexts[1:] {
			other, ok := ctx.elementSelections[key]
			if !ok || other.Member != selection.Member || other.Value.Value != selection.Value.Value {
				delete(out.elementSelections, key)
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

// indexedSelectionRoot returns the collection root for an indexed symbol path.
func indexedSelectionRoot(path SymbolPath) (SymbolPath, bool) {
	term, ok := path.(*SymbolTerm)
	if !ok {
		return nil, false
	}
	root := term.Base
	if offset, ok := root.(*SymbolOffset); ok && offset.Offset == 0 {
		root = offset.Base
	}
	return root, root != nil
}

// unionSelectionKey returns a stable key for a root/type pair.
func unionSelectionKey(root SymbolPath, strct *typeinfo.Struct) string {
	return symbolPathSelectionKey(root) + "|" + strings.ToLower(strct.String())
}

// symbolPathSelectionKey returns a stable key for an exact symbolic path.
func symbolPathSelectionKey(path SymbolPath) string {
	return strings.ToLower(path.String())
}
