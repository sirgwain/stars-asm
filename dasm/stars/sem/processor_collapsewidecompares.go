package sem

import (
	"fmt"
	"slices"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type compareRelation uint8

const (
	compareEQ compareRelation = iota
	compareNE
	compareLT
	compareLE
	compareGT
	compareGE
)

type compareDomain uint8

const (
	compareEquality compareDomain = iota
	compareSigned
	compareUnsigned
)

type wideCompareNode struct {
	index  int
	block  machine.BlockID
	branch machine.BranchEffect
}

type wideCompareMatch struct {
	nodes      []wideCompareNode
	relation   compareRelation
	domain     compareDomain
	lhs        machine.Value
	rhs        machine.Value
	trueBlock  machine.BlockID
	falseBlock machine.BlockID
}

type collapseWideComparesProcessor struct {
	ctx *FuncContext
}

// ProcessMachineFunc collapses proven two-word compare decision trees and
// isolates the compiler-generated comparison blocks they consume.
func (p *collapseWideComparesProcessor) ProcessMachineFunc(_ *Result, f *machine.FuncEffects) bool {
	changed := false
	for i := range f.Blocks {
		if p.ctx != nil {
			p.ctx.SetCurrentBlock(f.Blocks[i].Block)
		}
		match, ok := p.matchAt(f, i)
		if !ok {
			continue
		}

		root := match.nodes[0]
		lhs, rhs := p.preserveCompareDomain(match.lhs, match.rhs, match.domain)
		branch := root.branch
		branch.Predicate = &machine.PredicateValue{
			Kind: machine.PredicateCompare,
			Op:   canonicalCompareMnemonic(match.relation, match.domain),
			LHS:  lhs,
			RHS:  rhs,
		}
		branch.TrueBlock = match.trueBlock
		branch.FalseBlock = match.falseBlock
		f.Blocks[root.index].Effects[len(f.Blocks[root.index].Effects)-1] = branch

		rewrites := map[machine.BlockID][]machine.BlockID{
			root.block: {match.trueBlock, match.falseBlock},
		}
		for _, node := range match.nodes[1:] {
			f.Blocks[node.index].Effects = nil
			rewrites[node.block] = nil
		}
		if err := f.CFG.RewriteOutgoingEdges(rewrites); err != nil {
			panic(fmt.Errorf("collapse wide compare at %s: %w", root.block, err))
		}
		changed = true
	}
	if p.ctx != nil {
		p.ctx.ClearCurrentBlock()
	}
	return changed
}

// matchAt recognizes either a two-node equality ladder or a three-node
// lexicographic ordering ladder rooted at block index.
func (p *collapseWideComparesProcessor) matchAt(f *machine.FuncEffects, index int) (wideCompareMatch, bool) {
	root, ok := compareNode(f, index)
	if !ok {
		return wideCompareMatch{}, false
	}
	if match, ok := p.matchEquality(f, root); ok {
		return match, true
	}
	return p.matchOrdering(f, root)
}

// matchEquality recognizes a two-node low/high == or != decision tree.
func (p *collapseWideComparesProcessor) matchEquality(f *machine.FuncEffects, root wideCompareNode) (wideCompareMatch, bool) {
	for _, childID := range branchTargets(root.branch) {
		child, ok := disposableCompareNode(f, childID, root.block)
		if !ok {
			continue
		}
		nodes := []wideCompareNode{root, child}
		if !allCompareDomain(nodes, compareEquality) || len(finalTargets(nodes)) != 2 {
			continue
		}

		for lowIndex := range nodes {
			highIndex := 1 - lowIndex
			lhs, rhs, ok := p.pairCompareOperands(nodes[lowIndex], nodes[highIndex])
			if !ok {
				continue
			}
			equalTarget := evaluateCompareTree(root.block, nodes, lowIndex, [2]int{0, 0})
			unequalTargets := []machine.BlockID{
				evaluateCompareTree(root.block, nodes, lowIndex, [2]int{-1, 0}),
				evaluateCompareTree(root.block, nodes, lowIndex, [2]int{0, -1}),
				evaluateCompareTree(root.block, nodes, lowIndex, [2]int{-1, -1}),
			}
			if equalTarget == 0 || !allSameTarget(unequalTargets) || equalTarget == unequalTargets[0] {
				continue
			}
			relation := compareEQ
			trueBlock, falseBlock := equalTarget, unequalTargets[0]
			if rootRelation, _, _ := classifyCompare(root.branch.Predicate.Op); rootRelation == compareNE {
				relation = compareNE
				trueBlock, falseBlock = falseBlock, trueBlock
			}
			return wideCompareMatch{nodes: nodes, relation: relation, domain: compareEquality, lhs: lhs, rhs: rhs, trueBlock: trueBlock, falseBlock: falseBlock}, true
		}
	}
	return wideCompareMatch{}, false
}

// matchOrdering recognizes a three-node high/high/low lexicographic decision tree.
func (p *collapseWideComparesProcessor) matchOrdering(f *machine.FuncEffects, root wideCompareNode) (wideCompareMatch, bool) {
	for _, secondID := range branchTargets(root.branch) {
		second, ok := disposableCompareNode(f, secondID, root.block)
		if !ok {
			continue
		}
		for _, thirdID := range branchTargets(second.branch) {
			third, ok := disposableCompareNode(f, thirdID, second.block)
			if !ok {
				continue
			}
			nodes := []wideCompareNode{root, second, third}
			canonicalTargets := len(finalTargets(nodes)) != 2
			if canonicalTargets && len(canonicalFinalTargets(f, nodes)) != 2 {
				continue
			}
			if match, ok := p.matchOrderingNodes(f, nodes, canonicalTargets); ok {
				return match, true
			}
		}
	}
	return wideCompareMatch{}, false
}

// matchOrderingNodes proves lane identity, signedness, and the final wide relation.
func (p *collapseWideComparesProcessor) matchOrderingNodes(f *machine.FuncEffects, nodes []wideCompareNode, canonicalTargets bool) (wideCompareMatch, bool) {
	for lowIndex := range nodes {
		high := make([]int, 0, 2)
		for i := range nodes {
			if i != lowIndex {
				high = append(high, i)
			}
		}
		if !p.sameCompareOperands(nodes[high[0]], nodes[high[1]]) {
			continue
		}
		lhs, rhs, ok := p.pairCompareOperands(nodes[lowIndex], nodes[high[0]])
		if !ok {
			continue
		}
		_, lowDomain, lowOK := classifyCompare(nodes[lowIndex].branch.Predicate.Op)
		_, highDomain, highOK := classifyCompare(nodes[high[0]].branch.Predicate.Op)
		_, otherHighDomain, otherHighOK := classifyCompare(nodes[high[1]].branch.Predicate.Op)
		if !lowOK || !highOK || !otherHighOK || lowDomain != compareUnsigned || highDomain == compareEquality || highDomain != otherHighDomain {
			continue
		}

		outcomes := []struct{ high, low int }{{-1, 0}, {0, -1}, {0, 0}, {0, 1}, {1, 0}}
		results := make([]machine.BlockID, len(outcomes))
		for i, outcome := range outcomes {
			results[i] = evaluateOrderingTree(nodes[0].block, nodes, lowIndex, outcome.high, outcome.low)
			if canonicalTargets {
				results[i] = comparisonOutcomeTarget(f, results[i])
			}
		}
		for _, relation := range []compareRelation{compareLT, compareLE, compareGT, compareGE} {
			trueBlock, falseBlock, ok := relationTargets(relation, outcomes, results)
			if !ok {
				continue
			}
			return wideCompareMatch{nodes: nodes, relation: relation, domain: highDomain, lhs: lhs, rhs: rhs, trueBlock: trueBlock, falseBlock: falseBlock}, true
		}
	}
	return wideCompareMatch{}, false
}

// pairCompareOperands reconstructs both logical operands from low/high lanes.
func (p *collapseWideComparesProcessor) pairCompareOperands(low, high wideCompareNode) (machine.Value, machine.Value, bool) {
	collapser := wideMachineCollapser{ctx: p.ctx}
	lhs, lhsOK := collapser.pair(low.branch.Predicate.LHS, high.branch.Predicate.LHS)
	rhs, rhsOK := collapser.pair(low.branch.Predicate.RHS, high.branch.Predicate.RHS)
	if !lhsOK || !rhsOK || !p.validWideCompareOperand(lhs) || !p.validWideCompareOperand(rhs) {
		return nil, nil, false
	}
	return lhs, rhs, true
}

// validWideCompareOperand rejects adjacent aggregate members that happen to
// look like low/high lanes but do not resolve as one four-byte scalar.
func (p *collapseWideComparesProcessor) validWideCompareOperand(value machine.Value) bool {
	switch v := value.(type) {
	case *machine.Load:
		if v.Addr.Width != 4 || p.ctx == nil {
			return true
		}
		path, ok := p.ctx.symbols.symbolFromValue(v)
		var typ typeinfo.Type
		if ok {
			typ = path.Type()
		} else if addr, resolved := p.ctx.symbols.addressFromMemory(v.Addr, nil); resolved {
			lane, resolved := p.ctx.symbols.storageLaneFromMemory(v.Addr, addr)
			if resolved {
				typ, _ = p.wideCompareScalarTypeAtOffset(lane.object, lane.offset)
			}
		}
		if typ == nil {
			return false
		}
		if typ.Bytes() != 4 || (typ.Kind() != typeinfo.KInt && typ.Kind() != typeinfo.KPointer) {
			offset, ok := path.(*symresolve.SymbolOffset)
			if !ok {
				return false
			}
			var resolved bool
			typ, resolved = p.wideCompareScalarTypeAtOffset(offset.Base, offset.Offset)
			if !resolved {
				return false
			}
		}
		if typ.Bytes() != 4 {
			return false
		}
		switch typ.Kind() {
		case typeinfo.KInt, typeinfo.KPointer:
			return true
		default:
			return false
		}
	case *machine.Binary:
		return p.validWideCompareOperand(v.LHS) && p.validWideCompareOperand(v.RHS)
	case *machine.Cast:
		return p.validWideCompareOperand(v.Value)
	case *machine.SignExtendValue:
		return p.validWideCompareOperand(v.Parent)
	case *machine.PhiValue:
		for _, arm := range v.Arms {
			if !p.validWideCompareOperand(arm.Value) {
				return false
			}
		}
	}
	return true
}

// wideCompareScalarTypeAtOffset resolves a fixed storage offset through
// nested structs and arrays and returns only the exact leaf type it selects.
func (p *collapseWideComparesProcessor) wideCompareScalarTypeAtOffset(base symresolve.SymbolPath, offset int) (typeinfo.Type, bool) {
	if offset < 0 {
		return nil, false
	}
	path := base
	for {
		typ := path.Type()
		if offset == 0 && typ.Bytes() == 4 && (typ.Kind() == typeinfo.KInt || typ.Kind() == typeinfo.KPointer) {
			return typ, true
		}
		switch aggregate := typ.(type) {
		case *typeinfo.Pointer:
			if aggregate.Elem == nil {
				return nil, false
			}
			path = &symresolve.SymbolDeref{Base: path}
		case *typeinfo.Array:
			if aggregate.Elem == nil || aggregate.Elem.Bytes() <= 0 ||
				offset+4 > aggregate.Bytes() {
				return nil, false
			}
			elemSize := aggregate.Elem.Bytes()
			index := offset / elemSize
			offset %= elemSize
			path = &symresolve.SymbolTerm{
				Base:     path,
				IndexVal: machine.ConstVal(uint(index)),
				Scale:    elemSize,
				Result:   aggregate.Elem,
			}
		case *typeinfo.Struct:
			field, remainder, ok := p.ctx.res.ResolveContainingFieldPathInContext(path, offset, p.ctx.unionContext())
			if !ok {
				return nil, false
			}
			path = field
			offset = remainder
		default:
			if _, ok := path.(*symresolve.SymbolDeref); ok && typ.Bytes() > 0 && offset%typ.Bytes() == 0 {
				return typ, true
			}
			return typ, offset == 0
		}
	}
}

// preserveCompareDomain adds a machine-domain cast only when a known recovered
// integer type would otherwise give the collapsed C comparison different signedness.
func (p *collapseWideComparesProcessor) preserveCompareDomain(lhs, rhs machine.Value, domain compareDomain) (machine.Value, machine.Value) {
	if domain == compareEquality {
		return lhs, rhs
	}
	wantSigned := domain == compareSigned
	target := typeinfo.U32
	if wantSigned {
		target = typeinfo.I32
	}
	if signed, ok := p.valueSignedness(lhs); ok && signed != wantSigned {
		lhs = machine.CastVal(lhs, target)
	}
	if signed, ok := p.valueSignedness(rhs); ok && signed != wantSigned {
		rhs = machine.CastVal(rhs, target)
	}
	return lhs, rhs
}

// valueSignedness reports signedness for a known four-byte integer value.
func (p *collapseWideComparesProcessor) valueSignedness(value machine.Value) (bool, bool) {
	typ := machineValueType(value)
	if typ == nil && p.ctx != nil {
		if path, ok := p.ctx.symbols.symbolFromValue(value); ok {
			typ = path.Type()
		}
	}
	primitive, ok := typ.(*typeinfo.Primitive)
	return primitive != nil && primitive.Signed, ok && primitive.TypeKind == typeinfo.KInt && primitive.Bytes() == 4
}

// compareNode returns the terminal compare branch in one effects block.
func compareNode(f *machine.FuncEffects, index int) (wideCompareNode, bool) {
	if index < 0 || index >= len(f.Blocks) || len(f.Blocks[index].Effects) == 0 {
		return wideCompareNode{}, false
	}
	effect, ok := f.Blocks[index].Effects[len(f.Blocks[index].Effects)-1].(machine.BranchEffect)
	if !ok || effect.Predicate == nil || effect.Predicate.Kind != machine.PredicateCompare {
		return wideCompareNode{}, false
	}
	return wideCompareNode{index: index, block: f.Blocks[index].Block, branch: effect}, true
}

// disposableCompareNode returns a side-effect-free compare block with one expected predecessor.
func disposableCompareNode(f *machine.FuncEffects, id, predecessor machine.BlockID) (wideCompareNode, bool) {
	index := slices.IndexFunc(f.Blocks, func(block machine.BlockEffects) bool { return block.Block == id })
	node, ok := compareNode(f, index)
	if !ok || len(f.Blocks[index].Effects) != 1 || !slices.Equal(f.CFG.Predecessors(id), []machine.BlockID{predecessor}) {
		return wideCompareNode{}, false
	}
	return node, true
}

// classifyCompare normalizes one x86 conditional jump without losing its signedness domain.
func classifyCompare(op string) (compareRelation, compareDomain, bool) {
	switch strings.ToUpper(op) {
	case "JE", "JZ":
		return compareEQ, compareEquality, true
	case "JNE", "JNZ":
		return compareNE, compareEquality, true
	case "JL", "JNGE":
		return compareLT, compareSigned, true
	case "JLE", "JNG":
		return compareLE, compareSigned, true
	case "JG", "JNLE":
		return compareGT, compareSigned, true
	case "JGE", "JNL":
		return compareGE, compareSigned, true
	case "JB", "JC", "JNAE":
		return compareLT, compareUnsigned, true
	case "JBE", "JNA":
		return compareLE, compareUnsigned, true
	case "JA", "JNBE":
		return compareGT, compareUnsigned, true
	case "JAE", "JNC", "JNB":
		return compareGE, compareUnsigned, true
	default:
		return 0, 0, false
	}
}

// canonicalCompareMnemonic returns one stable Jcc spelling for a wide predicate.
func canonicalCompareMnemonic(relation compareRelation, domain compareDomain) string {
	if relation == compareEQ {
		return "JE"
	}
	if relation == compareNE {
		return "JNE"
	}
	if domain == compareSigned {
		return map[compareRelation]string{compareLT: "JL", compareLE: "JLE", compareGT: "JG", compareGE: "JGE"}[relation]
	}
	return map[compareRelation]string{compareLT: "JB", compareLE: "JBE", compareGT: "JA", compareGE: "JAE"}[relation]
}

// compareHolds evaluates a normalized relation for a three-way CMP outcome.
func compareHolds(op string, outcome int) bool {
	relation, _, ok := classifyCompare(op)
	if !ok {
		return false
	}
	switch relation {
	case compareEQ:
		return outcome == 0
	case compareNE:
		return outcome != 0
	case compareLT:
		return outcome < 0
	case compareLE:
		return outcome <= 0
	case compareGT:
		return outcome > 0
	case compareGE:
		return outcome >= 0
	default:
		return false
	}
}

// evaluateCompareTree follows an equality tree for supplied low/high outcomes.
func evaluateCompareTree(root machine.BlockID, nodes []wideCompareNode, lowIndex int, outcomes [2]int) machine.BlockID {
	current := root
	for range len(nodes) + 1 {
		index := slices.IndexFunc(nodes, func(node wideCompareNode) bool { return node.block == current })
		if index < 0 {
			return current
		}
		lane := 1
		if index == lowIndex {
			lane = 0
		}
		branch := nodes[index].branch
		current = branch.FalseBlock
		if compareHolds(branch.Predicate.Op, outcomes[lane]) {
			current = branch.TrueBlock
		}
	}
	return 0
}

// evaluateOrderingTree follows an ordering tree for supplied high/low outcomes.
func evaluateOrderingTree(root machine.BlockID, nodes []wideCompareNode, lowIndex, highOutcome, lowOutcome int) machine.BlockID {
	current := root
	for range len(nodes) + 1 {
		index := slices.IndexFunc(nodes, func(node wideCompareNode) bool { return node.block == current })
		if index < 0 {
			return current
		}
		outcome := highOutcome
		if index == lowIndex {
			outcome = lowOutcome
		}
		branch := nodes[index].branch
		current = branch.FalseBlock
		if compareHolds(branch.Predicate.Op, outcome) {
			current = branch.TrueBlock
		}
	}
	return 0
}

// relationTargets verifies that results exactly partition according to relation.
func relationTargets(relation compareRelation, outcomes []struct{ high, low int }, results []machine.BlockID) (machine.BlockID, machine.BlockID, bool) {
	var trueBlock, falseBlock machine.BlockID
	for i, outcome := range outcomes {
		wideOutcome := outcome.high
		if wideOutcome == 0 {
			wideOutcome = outcome.low
		}
		wantTrue := compareHolds(canonicalCompareMnemonic(relation, compareSigned), wideOutcome)
		target := &falseBlock
		if wantTrue {
			target = &trueBlock
		}
		if *target == 0 {
			*target = results[i]
		} else if *target != results[i] {
			return 0, 0, false
		}
	}
	return trueBlock, falseBlock, trueBlock != 0 && falseBlock != 0 && trueBlock != falseBlock
}

// allCompareDomain reports whether every node is a valid compare in domain.
func allCompareDomain(nodes []wideCompareNode, domain compareDomain) bool {
	for _, node := range nodes {
		_, got, ok := classifyCompare(node.branch.Predicate.Op)
		if !ok || got != domain {
			return false
		}
	}
	return true
}

// sameCompareOperands reports whether two predicates compare equivalent lanes.
func (p *collapseWideComparesProcessor) sameCompareOperands(a, b wideCompareNode) bool {
	return p.sameCompareValue(a.branch.Predicate.LHS, b.branch.Predicate.LHS) &&
		p.sameCompareValue(a.branch.Predicate.RHS, b.branch.Predicate.RHS)
}

// sameCompareValue ignores instruction provenance after proving value equivalence.
func (p *collapseWideComparesProcessor) sameCompareValue(a, b machine.Value) bool {
	if machine.ValueEquals(a, b) {
		return true
	}
	if aConst, ok := a.(*machine.Const); ok {
		bConst, ok := b.(*machine.Const)
		return ok && aConst.Val == bConst.Val
	}
	return p.ctx != nil && p.ctx.symbols.sameResolvedMachineValue(a, b)
}

// branchTargets returns both destinations of a compare branch.
func branchTargets(branch machine.BranchEffect) []machine.BlockID {
	return []machine.BlockID{branch.TrueBlock, branch.FalseBlock}
}

// finalTargets returns destinations outside the candidate comparison nodes.
func finalTargets(nodes []wideCompareNode) []machine.BlockID {
	internal := make(map[machine.BlockID]bool, len(nodes))
	for _, node := range nodes {
		internal[node.block] = true
	}
	var out []machine.BlockID
	for _, node := range nodes {
		for _, target := range branchTargets(node.branch) {
			if !internal[target] && !slices.Contains(out, target) {
				out = append(out, target)
			}
		}
	}
	return out
}

// canonicalFinalTargets returns comparison destinations after following pure
// jump blocks which may otherwise make one outcome appear as several targets.
func canonicalFinalTargets(f *machine.FuncEffects, nodes []wideCompareNode) []machine.BlockID {
	var out []machine.BlockID
	for _, target := range finalTargets(nodes) {
		target = comparisonOutcomeTarget(f, target)
		if !slices.Contains(out, target) {
			out = append(out, target)
		}
	}
	return out
}

// comparisonOutcomeTarget follows side-effect-free jump blocks so equivalent
// comparison outcomes share one canonical destination.
func comparisonOutcomeTarget(f *machine.FuncEffects, target machine.BlockID) machine.BlockID {
	visited := map[machine.BlockID]bool{}
	for target != 0 && !visited[target] {
		visited[target] = true
		index := slices.IndexFunc(f.Blocks, func(block machine.BlockEffects) bool { return block.Block == target })
		if index < 0 || len(f.Blocks[index].Effects) != 1 {
			break
		}
		jump, ok := f.Blocks[index].Effects[0].(machine.JumpEffect)
		if !ok {
			break
		}
		target = jump.To
	}
	return target
}

// allSameTarget reports whether targets is non-empty and contains one ID.
func allSameTarget(targets []machine.BlockID) bool {
	if len(targets) == 0 || targets[0] == 0 {
		return false
	}
	for _, target := range targets[1:] {
		if target != targets[0] {
			return false
		}
	}
	return true
}
