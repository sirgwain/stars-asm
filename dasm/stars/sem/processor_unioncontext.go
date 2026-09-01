package sem

import (
	"slices"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type unionContextProcessor struct {
	ctx *FuncContext
}

type unionFlowState struct {
	// ctx contains authoritative selections from function and call-result facts.
	// Discriminator-flow selections are materialized only after convergence.
	ctx           *symresolve.UnionContext
	aliases       map[string]map[string]unionAliasTarget
	enumAliases   map[string]map[string]dependentEnumAliasTarget
	possibleEnums map[string]map[int]typeinfo.EnumValue
	callResults   map[string]unionCallResultSelection
}

type unionAliasTarget struct {
	Key    string
	Root   symresolve.SymbolPath
	Rule   *typeinfo.UnionVariantRule
	Direct bool
}

type dependentEnumAliasTarget struct {
	Key    string
	Target symresolve.SymbolPath
	Rule   *typeinfo.DependentEnumRule
	Enum   *typeinfo.Enum
	Direct bool
}

type unionCallResultSelection struct {
	Rule  *typeinfo.UnionVariantRule
	Value typeinfo.EnumValue
}

// ProcessFunc computes block-entry union contexts for a function.
func (p *unionContextProcessor) ProcessFunc(result *Result, f *Func) bool {
	if p.ctx == nil || p.ctx.sdb == nil || p.ctx.fs == nil || f == nil {
		return false
	}
	if p.ctx.sdb.UnionRules == nil && len(p.ctx.sdb.DependentEnumRules) == 0 {
		return false
	}
	initial := p.initialState()
	entries := make(map[machine.BlockID]*unionFlowState, len(f.Blocks))
	if len(f.Blocks) > 0 {
		entries[f.Blocks[0].ID] = initial
	}

	blockByID := make(map[machine.BlockID]Block, len(f.Blocks))
	blockIndexByID := make(map[machine.BlockID]int, len(f.Blocks))
	queue := make([]machine.BlockID, 0, len(f.Blocks))
	inQueue := make(map[machine.BlockID]bool, len(f.Blocks))
	for i, block := range f.Blocks {
		blockByID[block.ID] = block
		blockIndexByID[block.ID] = i
		queue = append(queue, block.ID)
		inQueue[block.ID] = true
	}

	for len(queue) > 0 {
		id := queue[0]
		queue = queue[1:]
		inQueue[id] = false

		block := blockByID[id]
		if entries[id] == nil {
			continue
		}
		exits := p.processBlock(entries[id].clone(), block, f)
		succs := make([]machine.BlockID, 0, len(exits))
		for succ := range exits {
			succs = append(succs, succ)
		}
		slices.Sort(succs)

		for _, succ := range succs {
			state := exits[succ]
			next, changed := mergeUnionFlowState(entries[succ], state)
			if !changed {
				continue
			}
			entries[succ] = next
			if !inQueue[succ] {
				queue = append(queue, succ)
				inQueue[succ] = true
			}
		}
	}

	contexts := make(map[machine.BlockID]*symresolve.UnionContext, len(entries))
	for id, state := range entries {
		contexts[id] = materializeUnionContext(state)
	}
	p.ctx.SetUnionContexts(contexts)
	return true
}

// initialState creates the function-entry union facts and discriminator aliases.
func (p *unionContextProcessor) initialState() *unionFlowState {
	state := &unionFlowState{
		ctx:           symresolve.NewUnionContext(),
		aliases:       make(map[string]map[string]unionAliasTarget),
		enumAliases:   make(map[string]map[string]dependentEnumAliasTarget),
		possibleEnums: make(map[string]map[int]typeinfo.EnumValue),
		callResults:   make(map[string]unionCallResultSelection),
	}
	for _, root := range p.symbolRoots() {
		p.addDirectAliases(state, root)
		p.addDependentEnumAliases(state, root)
	}
	p.addExternalDiscriminatorAliases(state)
	if p.ctx.sdb.UnionRules != nil {
		for _, fact := range p.ctx.sdb.UnionRules.FunctionFactsFor(p.ctx.fs) {
			root, ok := p.functionRootByName(fact.Root)
			if !ok {
				continue
			}
			state.ctx.Add(root, fact.Rule, fact.Value)
		}
	}
	return state
}

// addExternalDiscriminatorAliases adds configured function-path discriminator aliases.
func (p *unionContextProcessor) addExternalDiscriminatorAliases(state *unionFlowState) {
	if p.ctx.sdb.UnionRules == nil {
		return
	}
	for _, alias := range p.ctx.sdb.UnionRules.ExternalDiscriminatorAliasesFor(p.ctx.fs) {
		source, ok := p.functionPathByComponents(alias.Source)
		if !ok {
			continue
		}
		root, ok := p.functionRootByName(alias.Root)
		if !ok {
			continue
		}
		target := unionAliasTarget{
			Key:    unionAliasTargetKey(root, alias.Rule),
			Root:   root,
			Rule:   alias.Rule,
			Direct: true,
		}
		key := symbolPathKey(source)
		addAliasTarget(state.aliases, key, target)
		state.possibleEnums[key] = possibleEnumValuesForUnionTargets(state.aliases[key])
	}
}

// processBlock applies assignments and terminator branch facts to one block.
func (p *unionContextProcessor) processBlock(state *unionFlowState, block Block, f *Func) map[machine.BlockID]*unionFlowState {
	exits := make(map[machine.BlockID]*unionFlowState)
	for _, effect := range block.Effects {
		switch e := effect.(type) {
		case *Assign:
			p.processAssign(state, e)
		case *CallEffect:
			p.processCall(state, e)
		case *Branch:
			trueState := state.clone()
			falseState := state.clone()
			p.applyBranchFact(e.Cond, trueState, falseState)
			exits[e.TrueBlock] = trueState
			exits[e.FalseBlock] = falseState
			return exits
		case *Jump:
			exits[e.To] = state.clone()
			return exits
		case *Return:
			return exits
		}
	}
	for _, succ := range fallthroughSuccessors(block.ID, f) {
		exits[succ] = state.clone()
	}
	return exits
}

// fallthroughSuccessors returns successors for a block with no explicit sem terminator.
func fallthroughSuccessors(id machine.BlockID, f *Func) []machine.BlockID {
	return f.CFG.Successors(id)
}

// processAssign propagates discriminator aliases through simple path copies.
func (p *unionContextProcessor) processAssign(state *unionFlowState, assign *Assign) {
	if selection, ok := p.callResultSelection(state, assign.Src); ok {
		if root, ok := pointerPartRoot(assign.Dst); ok {
			state.ctx.Add(root, selection.Rule, selection.Value)
		}
	}
	dst, ok := symbolPathForExpr(assign.Dst)
	if !ok {
		return
	}
	dstKey := symbolPathKey(dst)
	directTargets := directAliasTargets(state.aliases[dstKey])
	directEnumTargets := directDependentEnumAliasTargets(state.enumAliases[dstKey])
	enumValue, hasEnumValue := enumConstValue(assign.Src)
	delete(state.aliases, dstKey)
	delete(state.enumAliases, dstKey)
	delete(state.possibleEnums, dstKey)
	src, ok := symbolPathForExpr(assign.Src)
	if !ok {
		if len(directTargets) > 0 {
			state.aliases[dstKey] = directTargets
		}
		if len(directEnumTargets) > 0 {
			state.enumAliases[dstKey] = directEnumTargets
		}
		if len(directTargets) > 0 || len(directEnumTargets) > 0 {
			state.possibleEnums[dstKey] = possibleEnumValuesForAliases(directTargets, directEnumTargets)
		}
		if hasEnumValue && len(state.possibleEnums[dstKey]) > 0 {
			state.possibleEnums[dstKey] = map[int]typeinfo.EnumValue{enumValue.Value: enumValue}
		}
		return
	}
	targets := cloneAliasTargets(state.aliases[symbolPathKey(src)])
	for key, target := range targets {
		target.Direct = false
		targets[key] = target
	}
	for key, target := range directTargets {
		targets[key] = target
	}
	if len(targets) > 0 {
		state.aliases[dstKey] = targets
	}
	enumTargets := cloneDependentEnumAliasTargets(state.enumAliases[symbolPathKey(src)])
	for key, target := range enumTargets {
		target.Direct = false
		enumTargets[key] = target
	}
	for key, target := range directEnumTargets {
		enumTargets[key] = target
	}
	if len(enumTargets) > 0 {
		state.enumAliases[dstKey] = enumTargets
	}
	if len(targets) > 0 || len(enumTargets) > 0 {
		srcPossible := state.possibleEnums[symbolPathKey(src)]
		state.possibleEnums[dstKey] = clonePossibleEnumValues(srcPossible)
		if len(state.possibleEnums[dstKey]) == 0 {
			state.possibleEnums[dstKey] = possibleEnumValuesForAliases(targets, enumTargets)
		}
	}
}

// processCall records union selections implied by typed call result facts.
func (p *unionContextProcessor) processCall(state *unionFlowState, effect *CallEffect) {
	if p.ctx.sdb.UnionRules == nil || effect.Call == nil || effect.Call.Function == nil || effect.Result == nil {
		return
	}
	result, ok := effect.Result.(*CallResult)
	if !ok {
		return
	}
	for _, fact := range p.ctx.sdb.UnionRules.CallResultFactsFor(effect.Call.Function) {
		if fact.ParamIndex < 0 || fact.ParamIndex >= len(effect.Call.Args) {
			continue
		}
		value, ok := enumConstValue(effect.Call.Args[fact.ParamIndex])
		if !ok {
			continue
		}
		if _, ok := fact.Rule.MemberForValue(value.Value); !ok {
			continue
		}
		state.callResults[unionCallResultKey(result)] = unionCallResultSelection{Rule: fact.Rule, Value: value}
	}
}

// callResultSelection returns a selection carried by a call result expression.
func (p *unionContextProcessor) callResultSelection(state *unionFlowState, expr Expr) (unionCallResultSelection, bool) {
	result := callResultExpr(expr)
	if result == nil {
		return unionCallResultSelection{}, false
	}
	selection, ok := state.callResults[unionCallResultKey(result)]
	return selection, ok
}

// applyBranchFact narrows discriminator possibilities on each branch edge.
func (p *unionContextProcessor) applyBranchFact(cond Expr, trueState, falseState *unionFlowState) {
	compare, ok := cond.(*Compare)
	if !ok {
		return
	}
	path, value, ok := unionCompareFact(compare)
	if !ok {
		return
	}
	key := symbolPathKey(path)
	switch compare.Op {
	case CompareEQ:
		narrowPossibleEnumsEqual(trueState, key, value)
		narrowPossibleEnumsNotEqual(falseState, key, value)
	case CompareNE:
		narrowPossibleEnumsNotEqual(trueState, key, value)
		narrowPossibleEnumsEqual(falseState, key, value)
	}
}

// materializeUnionContext combines explicit context with converged flow facts.
//
// Explicit selections come from authoritative sources such as function facts and
// typed call-result facts. Flow-derived discriminator selections are added only
// when the converged possible-value set is a singleton, and never overwrite an
// explicit selection.
func materializeUnionContext(state *unionFlowState) *symresolve.UnionContext {
	if state == nil {
		return symresolve.NewUnionContext()
	}

	ctx := state.ctx.Clone()
	for path, values := range state.possibleEnums {
		value, ok := singletonPossibleEnumValue(values)
		if !ok {
			continue
		}
		addUnionSelectionsIfUnset(ctx, state.aliases[path], value)
		addDependentEnumSelectionsIfUnset(ctx, state.enumAliases[path], value)
	}
	return ctx
}

// addUnionSelectionsIfUnset adds flow-derived union selections without
// replacing selections established by an authoritative context source.
func addUnionSelectionsIfUnset(ctx *symresolve.UnionContext, targets map[string]unionAliasTarget, value typeinfo.EnumValue) {
	for _, target := range targets {
		if target.Rule == nil || target.Rule.Type == nil {
			continue
		}
		if _, ok := ctx.SelectionFor(target.Root, target.Rule.Type); ok {
			continue
		}
		ctx.Add(target.Root, target.Rule, value)
	}
}

// addDependentEnumSelectionsIfUnset adds flow-derived dependent enum
// selections without replacing authoritative path-specific enum selections.
func addDependentEnumSelectionsIfUnset(ctx *symresolve.UnionContext, targets map[string]dependentEnumAliasTarget, value typeinfo.EnumValue) {
	for _, target := range targets {
		if _, ok := ctx.EnumFor(target.Target); ok {
			continue
		}
		enumType, ok := target.Rule.TargetEnumForValue(value.Value)
		if !ok {
			continue
		}
		ctx.AddEnum(target.Target, enumType)
	}
}

// narrowPossibleEnumsEqual restricts a discriminator path to one enum value.
func narrowPossibleEnumsEqual(state *unionFlowState, path string, value typeinfo.EnumValue) {
	if len(state.possibleEnums[path]) == 0 {
		return
	}
	state.possibleEnums[path] = map[int]typeinfo.EnumValue{value.Value: value}
}

// narrowPossibleEnumsNotEqual removes one enum value from a discriminator path.
func narrowPossibleEnumsNotEqual(state *unionFlowState, path string, value typeinfo.EnumValue) {
	if len(state.possibleEnums[path]) == 0 {
		return
	}
	next := clonePossibleEnumValues(state.possibleEnums[path])
	delete(next, value.Value)
	state.possibleEnums[path] = next
}

// singletonPossibleEnumValue returns the only value in a possible-value set.
func singletonPossibleEnumValue(values map[int]typeinfo.EnumValue) (typeinfo.EnumValue, bool) {
	if len(values) != 1 {
		return typeinfo.EnumValue{}, false
	}
	for _, value := range values {
		return value, true
	}
	return typeinfo.EnumValue{}, false
}

// unionCompareFact extracts a path == enum-value comparison.
func unionCompareFact(compare *Compare) (symresolve.SymbolPath, typeinfo.EnumValue, bool) {
	if path, ok := symbolPathForExpr(compare.LHS); ok {
		if value, ok := enumConstValue(compare.RHS); ok {
			return path, value, true
		}
	}
	if path, ok := symbolPathForExpr(compare.RHS); ok {
		if value, ok := enumConstValue(compare.LHS); ok {
			return path, value, true
		}
	}
	return nil, typeinfo.EnumValue{}, false
}

// enumConstValue returns a typed enum constant's value.
func enumConstValue(expr Expr) (typeinfo.EnumValue, bool) {
	c, ok := expr.(*Const)
	if !ok {
		return typeinfo.EnumValue{}, false
	}
	enumType, ok := c.TypeInfo.(*typeinfo.Enum)
	if !ok {
		return typeinfo.EnumValue{}, false
	}
	for _, value := range enumType.Values {
		if value.Value == int(c.U64) {
			return value, true
		}
	}
	return typeinfo.EnumValue{}, false
}

// addDirectAliases adds aliases for every union discriminator rooted at root.
func (p *unionContextProcessor) addDirectAliases(state *unionFlowState, root symresolve.SymbolPath) {
	if p.ctx.sdb.UnionRules == nil {
		return
	}
	rule, ok := p.ctx.sdb.UnionRules.UnionVariantForType(root.Type())
	if !ok {
		return
	}
	path, ok := appendSymbolFieldPath(root, rule.Discriminator)
	if !ok {
		return
	}
	target := unionAliasTarget{
		Key:    unionAliasTargetKey(root, rule),
		Root:   root,
		Rule:   rule,
		Direct: true,
	}
	addAliasTarget(state.aliases, symbolPathKey(path), target)
	state.possibleEnums[symbolPathKey(path)] = possibleEnumValuesForUnionTargets(state.aliases[symbolPathKey(path)])
}

// addDependentEnumAliases adds aliases for every dependent enum discriminator rooted at root.
func (p *unionContextProcessor) addDependentEnumAliases(state *unionFlowState, root symresolve.SymbolPath) {
	for _, rule := range p.ctx.sdb.DependentEnumRules {
		if !rule.AppliesToType(root.Type()) {
			continue
		}
		discriminator, ok := appendSymbolFieldPath(root, rule.Discriminator)
		if !ok {
			continue
		}
		targetPath, ok := appendSymbolFieldPath(root, rule.Target)
		if !ok {
			continue
		}
		discriminatorEnum, ok := discriminator.Type().(*typeinfo.Enum)
		if !ok {
			continue
		}
		target := dependentEnumAliasTarget{
			Key:    dependentEnumAliasTargetKey(targetPath, rule),
			Target: targetPath,
			Rule:   rule,
			Enum:   discriminatorEnum,
			Direct: true,
		}
		key := symbolPathKey(discriminator)
		addDependentEnumAliasTarget(state.enumAliases, key, target)
		state.possibleEnums[key] = possibleEnumValuesForAliases(state.aliases[key], state.enumAliases[key])
	}
}

// symbolRoots returns all function and global symbols as symbolic roots.
func (p *unionContextProcessor) symbolRoots() []symresolve.SymbolPath {
	roots := make([]symresolve.SymbolPath, 0, len(p.ctx.fs.Params)+len(p.ctx.fs.Vars)+len(p.ctx.sdb.Globals))
	for i := range p.ctx.fs.Params {
		roots = append(roots, &symresolve.SymbolRoot{Symbol: &p.ctx.fs.Params[i]})
	}
	for i := range p.ctx.fs.Vars {
		roots = append(roots, &symresolve.SymbolRoot{Symbol: &p.ctx.fs.Vars[i]})
	}
	for _, global := range p.ctx.sdb.Globals {
		roots = append(roots, &symresolve.SymbolRoot{Symbol: global})
	}
	return roots
}

// functionRootByName returns a function param or local root by name.
func (p *unionContextProcessor) functionRootByName(name string) (symresolve.SymbolPath, bool) {
	for i := range p.ctx.fs.Params {
		if p.ctx.fs.Params[i].Name == name {
			return &symresolve.SymbolRoot{Symbol: &p.ctx.fs.Params[i]}, true
		}
	}
	for i := range p.ctx.fs.Vars {
		if p.ctx.fs.Vars[i].Name == name {
			return &symresolve.SymbolRoot{Symbol: &p.ctx.fs.Vars[i]}, true
		}
	}
	return nil, false
}

// functionPathByComponents builds a function-rooted path from path components.
func (p *unionContextProcessor) functionPathByComponents(components []string) (symresolve.SymbolPath, bool) {
	if len(components) == 0 {
		return nil, false
	}
	root, ok := p.functionRootByName(components[0])
	if !ok {
		return nil, false
	}
	if len(components) == 1 {
		return root, true
	}
	return appendSymbolFieldPath(root, components[1:])
}

// appendSymbolFieldPath builds a symbol path by selecting named fields.
func appendSymbolFieldPath(base symresolve.SymbolPath, names []string) (symresolve.SymbolPath, bool) {
	path := base
	for _, name := range names {
		strct, ok := symbolPathStruct(path)
		if !ok {
			return nil, false
		}
		field := fieldByName(strct, name)
		if field == nil {
			return nil, false
		}
		path = &symresolve.SymbolField{Base: path, Field: field}
	}
	return path, true
}

// symbolPathStruct returns the struct type for a symbolic path.
func symbolPathStruct(path symresolve.SymbolPath) (*typeinfo.Struct, bool) {
	typ, _ := typeinfo.UnwrapPointer(path.Type())
	strct, ok := typ.(*typeinfo.Struct)
	return strct, ok
}

// fieldByName returns a struct field by name.
func fieldByName(strct *typeinfo.Struct, name string) *typeinfo.StructField {
	for i := range strct.Fields {
		if strct.Fields[i].Name == name {
			return &strct.Fields[i]
		}
	}
	return nil
}

// clone returns a detached copy of a flow state.
func (s *unionFlowState) clone() *unionFlowState {
	if s == nil {
		return &unionFlowState{
			ctx:           symresolve.NewUnionContext(),
			aliases:       make(map[string]map[string]unionAliasTarget),
			enumAliases:   make(map[string]map[string]dependentEnumAliasTarget),
			possibleEnums: make(map[string]map[int]typeinfo.EnumValue),
			callResults:   make(map[string]unionCallResultSelection),
		}
	}
	return &unionFlowState{
		ctx:           s.ctx.Clone(),
		aliases:       cloneAliases(s.aliases),
		enumAliases:   cloneDependentEnumAliases(s.enumAliases),
		possibleEnums: clonePossibleEnums(s.possibleEnums),
		callResults:   cloneCallResults(s.callResults),
	}
}

// mergeUnionFlowState intersects facts from incoming control-flow paths.
func mergeUnionFlowState(existing, incoming *unionFlowState) (*unionFlowState, bool) {
	if existing == nil {
		return incoming.clone(), true
	}
	merged := &unionFlowState{
		ctx:           symresolve.IntersectUnionContexts([]*symresolve.UnionContext{existing.ctx, incoming.ctx}),
		aliases:       intersectAliases(existing.aliases, incoming.aliases),
		enumAliases:   intersectDependentEnumAliases(existing.enumAliases, incoming.enumAliases),
		possibleEnums: unionPossibleEnums(existing.possibleEnums, incoming.possibleEnums),
		callResults:   intersectCallResults(existing.callResults, incoming.callResults),
	}
	if existing.ctx.Equal(merged.ctx) && aliasMapsEqual(existing.aliases, merged.aliases) && dependentEnumAliasMapsEqual(existing.enumAliases, merged.enumAliases) && possibleEnumMapsEqual(existing.possibleEnums, merged.possibleEnums) && callResultMapsEqual(existing.callResults, merged.callResults) {
		return existing, false
	}
	return merged, true
}

// clonePossibleEnums returns a detached possible-value map.
func clonePossibleEnums(in map[string]map[int]typeinfo.EnumValue) map[string]map[int]typeinfo.EnumValue {
	out := make(map[string]map[int]typeinfo.EnumValue, len(in))
	for key, values := range in {
		out[key] = clonePossibleEnumValues(values)
	}
	return out
}

// clonePossibleEnumValues returns a detached possible-value set.
func clonePossibleEnumValues(in map[int]typeinfo.EnumValue) map[int]typeinfo.EnumValue {
	out := make(map[int]typeinfo.EnumValue, len(in))
	for value, enumValue := range in {
		out[value] = enumValue
	}
	return out
}

// unionPossibleEnums joins possible-value sets from incoming flow paths.
func unionPossibleEnums(a, b map[string]map[int]typeinfo.EnumValue) map[string]map[int]typeinfo.EnumValue {
	out := clonePossibleEnums(a)
	for path, values := range b {
		if out[path] == nil {
			out[path] = make(map[int]typeinfo.EnumValue)
		}
		for value, enumValue := range values {
			out[path][value] = enumValue
		}
	}
	return out
}

// possibleEnumMapsEqual reports whether two possible-value maps match.
func possibleEnumMapsEqual(a, b map[string]map[int]typeinfo.EnumValue) bool {
	if len(a) != len(b) {
		return false
	}
	for path, aValues := range a {
		bValues := b[path]
		if len(aValues) != len(bValues) {
			return false
		}
		for value := range aValues {
			if _, ok := bValues[value]; !ok {
				return false
			}
		}
	}
	return true
}

// cloneCallResults returns a detached call result selection map.
func cloneCallResults(in map[string]unionCallResultSelection) map[string]unionCallResultSelection {
	out := make(map[string]unionCallResultSelection, len(in))
	for key, selection := range in {
		out[key] = selection
	}
	return out
}

// intersectCallResults keeps call result selections shared by both paths.
func intersectCallResults(a, b map[string]unionCallResultSelection) map[string]unionCallResultSelection {
	out := make(map[string]unionCallResultSelection)
	for key, aSelection := range a {
		bSelection, ok := b[key]
		if !ok || aSelection.Rule != bSelection.Rule || aSelection.Value.Value != bSelection.Value.Value {
			continue
		}
		out[key] = aSelection
	}
	return out
}

// callResultMapsEqual reports whether two call result selection maps match.
func callResultMapsEqual(a, b map[string]unionCallResultSelection) bool {
	if len(a) != len(b) {
		return false
	}
	for key, aSelection := range a {
		bSelection, ok := b[key]
		if !ok || aSelection.Rule != bSelection.Rule || aSelection.Value.Value != bSelection.Value.Value {
			return false
		}
	}
	return true
}

// cloneAliases returns a detached alias map.
func cloneAliases(in map[string]map[string]unionAliasTarget) map[string]map[string]unionAliasTarget {
	out := make(map[string]map[string]unionAliasTarget, len(in))
	for key, targets := range in {
		out[key] = cloneAliasTargets(targets)
	}
	return out
}

// cloneAliasTargets returns a detached target map.
func cloneAliasTargets(in map[string]unionAliasTarget) map[string]unionAliasTarget {
	out := make(map[string]unionAliasTarget, len(in))
	for key, target := range in {
		out[key] = target
	}
	return out
}

// directAliasTargets returns only aliases intrinsic to their current path.
func directAliasTargets(in map[string]unionAliasTarget) map[string]unionAliasTarget {
	out := make(map[string]unionAliasTarget)
	for key, target := range in {
		if target.Direct {
			out[key] = target
		}
	}
	return out
}

// possibleEnumValuesForUnionTargets returns all mapped values for aliased unions.
func possibleEnumValuesForUnionTargets(targets map[string]unionAliasTarget) map[int]typeinfo.EnumValue {
	values := make(map[int]typeinfo.EnumValue)
	for _, target := range targets {
		if target.Rule == nil || target.Rule.Enum == nil {
			continue
		}
		for _, value := range target.Rule.Enum.Values {
			if _, ok := target.Rule.MemberForValue(value.Value); ok {
				values[value.Value] = value
			}
		}
	}
	return values
}

// possibleEnumValuesForAliases returns all discriminator values represented by aliases.
func possibleEnumValuesForAliases(
	unionTargets map[string]unionAliasTarget,
	enumTargets map[string]dependentEnumAliasTarget,
) map[int]typeinfo.EnumValue {
	values := possibleEnumValuesForUnionTargets(unionTargets)
	for _, target := range enumTargets {
		if target.Rule == nil || target.Enum == nil {
			continue
		}
		for _, value := range target.Enum.Values {
			if _, ok := target.Rule.EnumByValue[value.Value]; ok {
				values[value.Value] = value
			}
		}
	}
	return values
}

// cloneDependentEnumAliases returns a detached dependent enum alias map.
func cloneDependentEnumAliases(in map[string]map[string]dependentEnumAliasTarget) map[string]map[string]dependentEnumAliasTarget {
	out := make(map[string]map[string]dependentEnumAliasTarget, len(in))
	for key, targets := range in {
		out[key] = cloneDependentEnumAliasTargets(targets)
	}
	return out
}

// cloneDependentEnumAliasTargets returns a detached dependent enum target map.
func cloneDependentEnumAliasTargets(in map[string]dependentEnumAliasTarget) map[string]dependentEnumAliasTarget {
	out := make(map[string]dependentEnumAliasTarget, len(in))
	for key, target := range in {
		out[key] = target
	}
	return out
}

// directDependentEnumAliasTargets returns only aliases intrinsic to their current path.
func directDependentEnumAliasTargets(in map[string]dependentEnumAliasTarget) map[string]dependentEnumAliasTarget {
	out := make(map[string]dependentEnumAliasTarget)
	for key, target := range in {
		if target.Direct {
			out[key] = target
		}
	}
	return out
}

// intersectAliases keeps only aliases shared by both maps.
func intersectAliases(a, b map[string]map[string]unionAliasTarget) map[string]map[string]unionAliasTarget {
	out := make(map[string]map[string]unionAliasTarget)
	for path, aTargets := range a {
		bTargets := b[path]
		for key, aTarget := range aTargets {
			bTarget, ok := bTargets[key]
			if !ok || aTarget.Rule != bTarget.Rule || aTarget.Direct != bTarget.Direct {
				continue
			}
			addAliasTarget(out, path, aTarget)
		}
	}
	return out
}

// aliasMapsEqual reports whether two alias maps are identical.
func aliasMapsEqual(a, b map[string]map[string]unionAliasTarget) bool {
	if len(a) != len(b) {
		return false
	}
	for path, aTargets := range a {
		bTargets := b[path]
		if len(aTargets) != len(bTargets) {
			return false
		}
		for key, aTarget := range aTargets {
			bTarget, ok := bTargets[key]
			if !ok || aTarget.Rule != bTarget.Rule || aTarget.Direct != bTarget.Direct {
				return false
			}
		}
	}
	return true
}

// intersectDependentEnumAliases keeps only dependent enum aliases shared by both maps.
func intersectDependentEnumAliases(a, b map[string]map[string]dependentEnumAliasTarget) map[string]map[string]dependentEnumAliasTarget {
	out := make(map[string]map[string]dependentEnumAliasTarget)
	for path, aTargets := range a {
		bTargets := b[path]
		for key, aTarget := range aTargets {
			bTarget, ok := bTargets[key]
			if !ok || aTarget.Rule != bTarget.Rule || aTarget.Target.CDecl() != bTarget.Target.CDecl() || aTarget.Direct != bTarget.Direct {
				continue
			}
			addDependentEnumAliasTarget(out, path, aTarget)
		}
	}
	return out
}

// dependentEnumAliasMapsEqual reports whether two dependent enum alias maps match.
func dependentEnumAliasMapsEqual(a, b map[string]map[string]dependentEnumAliasTarget) bool {
	if len(a) != len(b) {
		return false
	}
	for path, aTargets := range a {
		bTargets := b[path]
		if len(aTargets) != len(bTargets) {
			return false
		}
		for key, aTarget := range aTargets {
			bTarget, ok := bTargets[key]
			if !ok || aTarget.Rule != bTarget.Rule || aTarget.Target.CDecl() != bTarget.Target.CDecl() || aTarget.Direct != bTarget.Direct {
				return false
			}
		}
	}
	return true
}

// addAliasTarget records one target for a discriminator path.
func addAliasTarget(aliases map[string]map[string]unionAliasTarget, path string, target unionAliasTarget) {
	if aliases[path] == nil {
		aliases[path] = make(map[string]unionAliasTarget)
	}
	aliases[path][target.Key] = target
}

// addDependentEnumAliasTarget records one target for a dependent enum discriminator path.
func addDependentEnumAliasTarget(aliases map[string]map[string]dependentEnumAliasTarget, path string, target dependentEnumAliasTarget) {
	if aliases[path] == nil {
		aliases[path] = make(map[string]dependentEnumAliasTarget)
	}
	aliases[path][target.Key] = target
}

// unionAliasTargetKey returns a stable key for a union root/rule pair.
func unionAliasTargetKey(root symresolve.SymbolPath, rule *typeinfo.UnionVariantRule) string {
	return strings.ToLower(root.CDecl()) + "|" + strings.ToLower(rule.Type.String())
}

// dependentEnumAliasTargetKey returns a stable key for a dependent enum target/rule pair.
func dependentEnumAliasTargetKey(target symresolve.SymbolPath, rule *typeinfo.DependentEnumRule) string {
	return strings.ToLower(target.CDecl()) + "|" + strings.ToLower(rule.Type.String())
}

// symbolPathKey returns a stable lookup key for a symbolic path.
func symbolPathKey(path symresolve.SymbolPath) string {
	return strings.ToLower(path.CDecl())
}

// callResultExpr returns the call result carried by an expression.
func callResultExpr(expr Expr) *CallResult {
	switch e := expr.(type) {
	case *CallResult:
		return e
	case *FarPointer:
		return callResultExpr(e.Parent)
	default:
		return nil
	}
}

// unionCallResultKey returns a stable identity for a call result.
func unionCallResultKey(result *CallResult) string {
	if result == nil || result.Function == nil {
		return ""
	}
	return strings.ToLower(result.Function.Name) + "|" + machine.BlockID(result.InstOff).String()
}

// pointerPartRoot returns the root being assigned a call-result pointer word.
func pointerPartRoot(value LValue) (symresolve.SymbolPath, bool) {
	part, ok := value.(*Part)
	if !ok || part.ByteOff != 0 {
		return nil, false
	}
	return symbolPathForExpr(part.Base)
}
