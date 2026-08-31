package machine

import (
	"fmt"
	"reflect"
	"sort"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
)

// PhiArm records the value contributed by one predecessor block to a joined
// machine/effect value.
type PhiArm struct {
	Block *Block
	Value Value
}

// PhiValue preserves the predecessor alternatives for a value that crosses a
// CFG join.
type PhiValue struct {
	Join BlockID
	Arms []PhiArm
}

func (m *PhiValue) value() {}

// String renders phi arms explicitly for dumps and snapshot output.
func (m *PhiValue) String() string {
	if m == nil {
		return ""
	}
	parts := make([]string, 0, len(m.Arms))
	for _, arm := range m.Arms {
		label := "<?>"
		if arm.Block != nil {
			label = arm.Block.ID.String()
		}
		parts = append(parts, fmt.Sprintf("%s:%s", label, arm.Value))
	}
	return fmt.Sprintf("merge(Join: %s, (%s))", m.Join, strings.Join(parts, ", "))
}

// phiArmBlockID returns the sortable block ID for a phi arm, using -1 for
// synthetic or missing block provenance.
func phiArmBlockID(arm PhiArm) int {
	if arm.Block == nil {
		return -1
	}
	return int(arm.Block.ID)
}

// phiValueWithPrevious builds a canonical phi value and widens changing
// loop-carried arms to keep cyclic CFG joins finite.
func phiValueWithPrevious(join BlockID, arms []PhiArm, previous Value, loopPreds map[BlockID]bool) Value {
	flattenNested := len(loopPreds) == 0
	arms = canonicalPhiArms(join, arms, flattenNested)
	arms = widenChangingLoopArms(arms, previous, loopPreds)
	if len(loopPreds) == 0 {
		if v, ok := collapseLoopUnknownArms(arms); ok {
			return v
		}
	}
	if len(arms) == 0 {
		return nil
	}
	first := arms[0].Value
	allSame := true
	for _, arm := range arms[1:] {
		if !convergenceValueEquals(first, arm.Value) {
			allSame = false
			break
		}
	}
	if allSame {
		return first
	}
	return &PhiValue{Join: join, Arms: append([]PhiArm(nil), arms...)}
}

// canonicalPhiArms keeps one arm per predecessor block, sorts by predecessor
// identity, optionally flattens nested phis, and coalesces duplicate unknown
// arms.
func canonicalPhiArms(join BlockID, arms []PhiArm, flattenNested bool) []PhiArm {
	if len(arms) == 0 {
		return nil
	}
	flat := make([]PhiArm, 0, len(arms))
	var appendArm func(PhiArm)
	appendArm = func(arm PhiArm) {
		if phi, ok := arm.Value.(*PhiValue); flattenNested && ok && phi != nil && phi.Join == join {
			for _, nested := range phi.Arms {
				appendArm(nested)
			}
			return
		}
		flat = append(flat, arm)
	}
	for _, arm := range arms {
		appendArm(arm)
	}
	if len(flat) == 0 {
		return nil
	}
	byBlock := make(map[int]PhiArm, len(flat))
	order := make([]int, 0, len(flat))
	for _, arm := range flat {
		id := phiArmBlockID(arm)
		if _, ok := byBlock[id]; !ok {
			order = append(order, id)
		}
		byBlock[id] = arm
	}
	sort.Ints(order)
	out := make([]PhiArm, 0, len(order))
	for _, id := range order {
		out = append(out, byBlock[id])
	}
	return coalesceDuplicateUnknownArms(out)
}

// coalesceDuplicateUnknownArms keeps a single arm for repeated unknown values,
// preventing equivalent phi/loop placeholders from bloating debug output.
func coalesceDuplicateUnknownArms(arms []PhiArm) []PhiArm {
	if len(arms) == 0 {
		return nil
	}
	out := make([]PhiArm, 0, len(arms))
	seenUnknown := make(map[string]bool)
	for _, arm := range arms {
		if unknown, ok := arm.Value.(*Unknown); ok {
			if seenUnknown[unknown.Desc] {
				continue
			}
			seenUnknown[unknown.Desc] = true
		}
		out = append(out, arm)
	}
	return out
}

// samePhiShape reports whether two phi values have the same predecessor
// block layout.
func samePhiShape(a, b *PhiValue) bool {
	if a == nil || b == nil || a.Join != b.Join || len(a.Arms) != len(b.Arms) {
		return false
	}
	for i := range a.Arms {
		if a.Arms[i].Block == nil || b.Arms[i].Block == nil {
			if a.Arms[i].Block != b.Arms[i].Block {
				return false
			}
			continue
		}
		if a.Arms[i].Block.ID != b.Arms[i].Block.ID {
			return false
		}
	}
	return true
}

// collapseLoopUnknownArms returns the shared concrete value when the only
// disagreement is a loop-widening sentinel.
func collapseLoopUnknownArms(arms []PhiArm) (Value, bool) {
	var first Value
	haveConcrete := false
	haveLoopUnknown := false
	for _, arm := range arms {
		if isLoopUnknown(arm.Value) {
			haveLoopUnknown = true
			continue
		}
		if !haveConcrete {
			first = arm.Value
			haveConcrete = true
			continue
		}
		if !convergenceValueEquals(first, arm.Value) {
			return nil, false
		}
	}
	if haveConcrete && haveLoopUnknown {
		return first, true
	}
	return nil, false
}

// phiStackWordsByArm combines matching word-wise phis into a single phi
// with one reconstructed multi-word value per predecessor.
func phiStackWordsByArm(words []Value) (Value, bool) {
	if len(words) < 2 {
		return nil, false
	}
	first, ok := words[0].(*PhiValue)
	if !ok || first == nil {
		return nil, false
	}
	phis := make([]*PhiValue, len(words))
	phis[0] = first
	for i := range words[1:] {
		phi, ok := words[i+1].(*PhiValue)
		if !ok || !samePhiShape(first, phi) {
			return nil, false
		}
		phis[i+1] = phi
	}
	arms := make([]PhiArm, 0, len(first.Arms))
	for armIdx, firstArm := range first.Arms {
		armWords := make([]Value, len(words))
		for wordIdx, phi := range phis {
			armWords[wordIdx] = phi.Arms[armIdx].Value
		}
		arms = append(arms, PhiArm{
			Block: firstArm.Block,
			Value: stackWordsValue(armWords),
		})
	}
	return phiValueWithPrevious(first.Join, arms, nil, nil), true
}

// widenChangingLoopArms replaces a loop-carried arm with an unknown once a
// previous value for the same predecessor keeps changing across iterations.
func widenChangingLoopArms(arms []PhiArm, previous Value, loopPreds map[BlockID]bool) []PhiArm {
	prev, ok := previous.(*PhiValue)
	if !ok || len(loopPreds) == 0 {
		return arms
	}
	prevByBlock := make(map[BlockID]Value, len(prev.Arms))
	for _, arm := range prev.Arms {
		if arm.Block != nil {
			prevByBlock[arm.Block.ID] = arm.Value
		}
	}
	out := append([]PhiArm(nil), arms...)
	for i, arm := range out {
		if arm.Block == nil || !loopPreds[arm.Block.ID] {
			continue
		}
		old, ok := prevByBlock[arm.Block.ID]
		if !ok || convergenceValueEquals(old, arm.Value) {
			continue
		}
		if isLoopUnknown(old) {
			out[i].Value = old
			continue
		}
		out[i].Value = UnknownVal("loop")
	}
	return canonicalPhiArms(prev.Join, out, false)
}

// isLoopUnknown reports whether v is the widening sentinel for a cyclic phi.
func isLoopUnknown(v Value) bool {
	u, ok := v.(*Unknown)
	return ok && u.Desc == "loop"
}

// valueKey returns a deterministic structural key for a machine value.
func valueKey(v Value) string {
	return valueKeyDepth(v, 0)
}

// valueKeyDepth returns a deterministic key, summarizing very deep expressions.
func valueKeyDepth(v Value, depth int) string {
	if v == nil {
		return "<nil>"
	}
	rv := reflect.ValueOf(v)
	if rv.Kind() == reflect.Pointer && rv.IsNil() {
		return "<nil>"
	}
	if depth > 4 {
		return fmt.Sprintf("%T:<deep>", v)
	}
	switch x := v.(type) {
	case *Unknown:
		return "unknown:" + keyString(x.Desc)
	case *Const:
		if x.Origin == nil {
			return fmt.Sprintf("const:%x", x.Val)
		}
		return fmt.Sprintf("const:%x@%x/%d", x.Val, x.Origin.InstOff, x.Origin.Role)
	case *Reg:
		return "scalar:" + keyString(x.Val.String())
	case *FrameBase:
		return "framebase"
	case *FloatConst:
		return fmt.Sprintf("float:%g", x.Val)
	case *CallResult:
		target := ""
		if x.Target != nil {
			target = x.Target.Name
		}
		return fmt.Sprintf("callresult:%x:%s:%s", x.InstOff, target, x.String())
	case *PredicateValue:
		return fmt.Sprintf("pred:%d:%s:%s:%s", x.Kind, x.Op, valueKeyDepth(x.LHS, depth+1), valueKeyDepth(x.RHS, depth+1))
	case *WordValue:
		return fmt.Sprintf("word:%d:%s", x.Part, valueKeyDepth(x.Parent, depth+1))
	case *FarPointer:
		return fmt.Sprintf(
			"farptr:%d:%s:%s:%s",
			x.Part,
			valueKeyDepth(x.Parent, depth+1),
			valueKeyDepth(x.Offset, depth+1),
			valueKeyDepth(x.Segment, depth+1),
		)
	case *SignExtendValue:
		return fmt.Sprintf("sext:%d:%d:%s", x.FromBits, x.ToBits, valueKeyDepth(x.Parent, depth+1))
	case *StackWords:
		parts := make([]string, 0, len(x.Words))
		for _, word := range x.Words {
			parts = append(parts, valueKeyDepth(word, depth+1))
		}
		return "words:" + strings.Join(parts, ",")
	case *ByteValue:
		value := ""
		if x.Value != nil {
			value = valueKeyDepth(*x.Value, depth+1)
		}
		return fmt.Sprintf("byte:%d:%s:%s", x.Part, valueKeyDepth(x.Parent, depth+1), value)
	case *Cast:
		return "cast:" + x.To.String() + ":" + valueKeyDepth(x.Value, depth+1)
	case *Binary:
		return fmt.Sprintf("binary:%d:%s:%s", x.Op, valueKeyDepth(x.LHS, depth+1), valueKeyDepth(x.RHS, depth+1))
	case *Load:
		return fmt.Sprintf("load:%s:%s", x.ID, memoryAccessKeyDepth(x.Access, depth+1))
	case *Address:
		return "addr:" + memoryAccessKeyDepth(x.Access, depth+1)
	case *PhiValue:
		parts := make([]string, 0, len(x.Arms))
		for _, arm := range x.Arms {
			parts = append(parts, fmt.Sprintf("%d=%s", phiArmBlockID(arm), valueKeyDepth(arm.Value, depth+1)))
		}
		return fmt.Sprintf("phi:%d:%s", x.Join, strings.Join(parts, "|"))
	default:
		return fmt.Sprintf("%T:%s", v, v.String())
	}
}

// memoryAccessKeyDepth returns a deterministic key for a memory access.
func memoryAccessKeyDepth(a MemoryAccess, depth int) string {
	origin := fmt.Sprintf("@%x/%d", a.Origin.InstOff, a.Origin.Role)
	return fmt.Sprintf(
		"seg=%s;base=%s;idx=%s;scale=%d;disp=%d;width=%d%s",
		valueKeyDepth(a.Seg, depth+1),
		valueKeyDepth(a.Base, depth+1),
		valueKeyDepth(a.Index, depth+1),
		a.Scale,
		a.Disp,
		a.Width,
		origin,
	)
}

// joinBlockEntry builds the abstract state visible at the start of block from
// the currently known predecessor exit states.
func joinBlockEntry(cfg *CFG, block *Block, exits map[BlockID]*state, previous *state, loops map[BlockID]map[BlockID]bool) *state {
	if block == nil {
		return nil
	}
	preds := cfg.Predecessors(block.ID)
	if len(preds) == 0 {
		return previous
	}
	states := make([]predecessorState, 0, len(preds))
	for _, predID := range preds {
		predExit := exits[predID]
		if predExit == nil {
			continue
		}
		states = append(states, predecessorState{block: cfg.Block(predID), state: predExit})
	}
	if len(states) == 0 {
		return previous
	}
	return joinStatesForBlock(block, states, previous, loops[block.ID])
}

type predecessorState struct {
	block *Block
	state *state
}

// joinStatesForBlock merges predecessor states for one block entry.
func joinStatesForBlock(block *Block, preds []predecessorState, previous *state, loopPreds map[BlockID]bool) *state {
	if len(preds) == 0 {
		return nil
	}
	if len(preds) == 1 {
		return projectStateForBlock(block, preds[0].state)
	}

	out := newValueState()
	out.warnStackWordUnderflow = preds[0].state.warnStackWordUnderflow
	out.regs = joinRegisterState(block, preds, previous, loopPreds)
	out.flags = joinFlags(block, preds)
	out.stack = joinStackState(block.ID, preds, previous, loopPreds)
	out.fp, out.fpd = joinFPState(block.ID, preds, previous, loopPreds)
	out.outgoingStackBytes = joinIntState(preds, func(st *state) int { return st.outgoingStackBytes })
	out.fpCallSlots = joinFPCallSlots(preds)
	return out
}

// projectStateForBlock keeps only state observable at the start of block.
func projectStateForBlock(block *Block, in *state) *state {
	if in == nil {
		return nil
	}
	out := in.clone()
	out.regs = projectRegisterState(block, in.regs)
	if block == nil || !block.FlagsLiveIn {
		out.flags = nil
	}
	return out
}

// projectStateForBlockExit keeps only state observable by block successors.
func projectStateForBlockExit(block *Block, in *state) *state {
	if in == nil {
		return nil
	}
	var live map[asm.Reg]bool
	if block != nil {
		live = block.RegLiveOut
	}
	out := in.clone()
	out.regs = projectRegisterSet(in.regs, live)
	if block == nil || !block.FlagsLiveOut {
		out.flags = nil
	}
	return out
}

// projectRegisterState keeps only registers live into block.
func projectRegisterState(block *Block, regs map[asm.Reg]Value) map[asm.Reg]Value {
	var live map[asm.Reg]bool
	if block != nil {
		live = block.RegLiveIn
	}
	return projectRegisterSet(regs, live)
}

// projectRegisterSet keeps segment registers and registers present in live.
func projectRegisterSet(regs map[asm.Reg]Value, live map[asm.Reg]bool) map[asm.Reg]Value {
	out := make(map[asm.Reg]Value, len(regs))
	for _, reg := range []asm.Reg{asm.RegBP, asm.RegSP, asm.RegDS, asm.RegSS, asm.RegCS, asm.RegES} {
		if v, ok := regs[reg]; ok {
			out[reg] = v
		}
	}
	if len(live) == 0 {
		return out
	}
	for reg, isLive := range live {
		if !isLive {
			continue
		}
		if v, ok := regs[reg]; ok {
			out[reg] = v
		}
	}
	return out
}

// joinRegisterState merges register values that are live into the destination block.
func joinRegisterState(block *Block, preds []predecessorState, previous *state, loopPreds map[BlockID]bool) map[asm.Reg]Value {
	regs := block.RegLiveIn
	out := make(map[asm.Reg]Value, len(regs))
	for reg := range regs {
		prev := previousValue(previous, func(st *state) Value {
			return st.regs[reg]
		})
		out[reg] = joinOptionalValue(block.ID, preds, prev, loopPreds, func(st *state) Value {
			return st.regs[reg]
		})
	}
	return out
}

// joinOptionalValue merges one optional Value field across predecessor states.
func joinOptionalValue(join BlockID, preds []predecessorState, previous Value, loopPreds map[BlockID]bool, read func(*state) Value) Value {
	arms := make([]PhiArm, 0, len(preds))
	for _, pred := range preds {
		v := read(pred.state)
		if v == nil {
			continue
		}
		arms = append(arms, PhiArm{Block: pred.block, Value: v})
	}
	return phiValueWithPrevious(join, arms, previous, loopPreds)
}

// previousValue returns the prior state value for a field when available.
func previousValue(previous *state, read func(*state) Value) Value {
	if previous == nil {
		return nil
	}
	return read(previous)
}

// joinFlags merges condition flags only when the destination can read them.
func joinFlags(block *Block, preds []predecessorState) *PredicateValue {
	if block == nil || !block.FlagsLiveIn {
		return nil
	}
	var first *PredicateValue
	for _, pred := range preds {
		if pred.state.flags == nil {
			return PredicateVal(PredicateFromFlags, "flags").(*PredicateValue)
		}
		if first == nil {
			cp := *pred.state.flags
			first = &cp
			continue
		}
		if !convergenceValueEquals(first, pred.state.flags) {
			return PredicateVal(PredicateFromFlags, "flags").(*PredicateValue)
		}
	}
	return first
}

// joinStackState merges stack words when all predecessors agree on depth.
func joinStackState(join BlockID, preds []predecessorState, previous *state, loopPreds map[BlockID]bool) []stackWord {
	depth := preds[0].state.stackDepth()
	for _, pred := range preds[1:] {
		if pred.state.stackDepth() != depth {
			return nil
		}
	}
	out := make([]stackWord, depth)
	for i := 0; i < depth; i++ {
		prev := previousValue(previous, func(st *state) Value {
			if i >= len(st.stack) {
				return nil
			}
			return st.stack[i].value
		})
		out[i] = stackWord{
			value: joinOptionalValue(join, preds, prev, loopPreds, func(st *state) Value {
				return st.stack[i].value
			}),
			meta: preds[0].state.stack[i].meta,
		}
	}
	return out
}

// joinFPState merges the tracked x87 stack when all predecessors agree on depth.
func joinFPState(join BlockID, preds []predecessorState, previous *state, loopPreds map[BlockID]bool) ([8]Value, int) {
	depth := preds[0].state.fpd
	for _, pred := range preds[1:] {
		if pred.state.fpd != depth {
			return [8]Value{}, 0
		}
	}
	var out [8]Value
	for i := 0; i < depth; i++ {
		idx := i
		prev := previousValue(previous, func(st *state) Value {
			return st.fp[idx]
		})
		out[i] = joinOptionalValue(join, preds, prev, loopPreds, func(st *state) Value {
			return st.fp[idx]
		})
	}
	return out, depth
}

// joinIntState keeps an integer field only when all predecessors agree.
func joinIntState(preds []predecessorState, read func(*state) int) int {
	first := read(preds[0].state)
	for _, pred := range preds[1:] {
		if read(pred.state) != first {
			return 0
		}
	}
	return first
}

// joinFPCallSlots keeps staged FP call slots only when all predecessors agree.
func joinFPCallSlots(preds []predecessorState) []fpCallSlot {
	first := preds[0].state.fpCallSlots
	for _, pred := range preds[1:] {
		if len(pred.state.fpCallSlots) != len(first) {
			return nil
		}
		for i, slot := range pred.state.fpCallSlots {
			if slot.depth != first[i].depth || slot.disp != first[i].disp || !convergenceValueEquals(slot.value, first[i].value) {
				return nil
			}
		}
	}
	return append([]fpCallSlot(nil), first...)
}

// widenChangingLoopState forces still-changing loop entry state to a finite top value.
func widenChangingLoopState(next, previous *state) *state {
	out := next.clone()
	for reg, v := range out.regs {
		prev := previous.regs[reg]
		if convergenceValueEquals(v, prev) || containsLoopUnknown(v, 0) {
			continue
		}
		out.regs[reg] = UnknownVal("loop")
	}
	for i := range out.stack {
		if i >= len(previous.stack) {
			out.stack[i].value = UnknownVal("loop")
			continue
		}
		v := out.stack[i].value
		prev := previous.stack[i].value
		if convergenceValueEquals(v, prev) || containsLoopUnknown(v, 0) {
			continue
		}
		out.stack[i].value = UnknownVal("loop")
	}
	for i := 0; i < out.fpd; i++ {
		prev := Value(nil)
		if i < previous.fpd {
			prev = previous.fp[i]
		}
		if convergenceValueEquals(out.fp[i], prev) || containsLoopUnknown(out.fp[i], 0) {
			continue
		}
		out.fp[i] = UnknownVal("loop")
	}
	return out
}

// containsLoopUnknown reports whether v already contains the loop widening sentinel.
func containsLoopUnknown(v Value, depth int) bool {
	if v == nil || depth > 16 {
		return false
	}
	if isLoopUnknown(v) {
		return true
	}
	switch x := v.(type) {
	case *PredicateValue:
		return containsLoopUnknown(x.LHS, depth+1) || containsLoopUnknown(x.RHS, depth+1)
	case *WordValue:
		return containsLoopUnknown(x.Parent, depth+1)
	case *FarPointer:
		return containsLoopUnknown(x.Parent, depth+1) ||
			containsLoopUnknown(x.Offset, depth+1) ||
			containsLoopUnknown(x.Segment, depth+1)
	case *SignExtendValue:
		return containsLoopUnknown(x.Parent, depth+1)
	case *StackWords:
		for _, word := range x.Words {
			if containsLoopUnknown(word, depth+1) {
				return true
			}
		}
	case *ByteValue:
		if containsLoopUnknown(x.Parent, depth+1) {
			return true
		}
		return x.Value != nil && containsLoopUnknown(*x.Value, depth+1)
	case *Cast:
		return containsLoopUnknown(x.Value, depth+1)
	case *Binary:
		return containsLoopUnknown(x.LHS, depth+1) || containsLoopUnknown(x.RHS, depth+1)
	case *Load:
		return memoryAccessContainsLoopUnknown(x.Access, depth+1)
	case *Address:
		return memoryAccessContainsLoopUnknown(x.Access, depth+1)
	case *PhiValue:
		for _, arm := range x.Arms {
			if containsLoopUnknown(arm.Value, depth+1) {
				return true
			}
		}
	}
	return false
}

// memoryAccessContainsLoopUnknown reports whether a memory access contains loop top.
func memoryAccessContainsLoopUnknown(a MemoryAccess, depth int) bool {
	return containsLoopUnknown(a.Seg, depth+1) ||
		containsLoopUnknown(a.Base, depth+1) ||
		containsLoopUnknown(a.Index, depth+1)
}

// keyString caps free-form text in convergence keys.
func keyString(s string) string {
	if len(s) <= 64 {
		return s
	}
	return s[:64] + "<truncated>"
}

// stackDepth returns the number of abstract machine words tracked on the stack.
func (st *state) stackDepth() int {
	if st == nil {
		return 0
	}
	return len(st.stack)
}

// sameState reports whether two abstract states have the same observable data.
func sameState(a, b *state) bool {
	return stateKey(a) == stateKey(b)
}

// stateKey returns a deterministic key for state fixpoint comparisons.
func stateKey(st *state) string {
	if st == nil {
		return "<nil>"
	}
	var sb strings.Builder
	regs := make([]asm.Reg, 0, len(st.regs))
	for reg := range st.regs {
		regs = append(regs, reg)
	}
	sort.Slice(regs, func(i, j int) bool { return regs[i] < regs[j] })
	sb.WriteString("regs:")
	for _, reg := range regs {
		fmt.Fprintf(&sb, "%d=%s;", reg, valueKey(st.regs[reg]))
	}
	sb.WriteString("flags:")
	sb.WriteString(valueKey(st.flags))
	sb.WriteString(";stack:")
	for _, word := range st.stack {
		sb.WriteString(valueKey(word.value))
		sb.WriteByte(';')
	}
	sb.WriteString("fp:")
	for i := 0; i < st.fpd; i++ {
		sb.WriteString(valueKey(st.fp[i]))
		sb.WriteByte(';')
	}
	fmt.Fprintf(&sb, "fpd:%d;out:%d;", st.fpd, st.outgoingStackBytes)
	return sb.String()
}
