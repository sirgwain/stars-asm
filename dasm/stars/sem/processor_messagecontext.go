package sem

import (
	"slices"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// messageContextProcessor records which window message a window procedure is
// handling on entry to each block, so wParam and lParam can be viewed through
// that message's payload types.
type messageContextProcessor struct {
	ctx *FuncContext
}

// messageFact is the set of messages a block may be handling on entry. A
// fact that is not known means the message parameter may hold any value; a
// known fact holds more than one message where cases fall through into a
// shared block.
type messageFact struct {
	values []int
	known  bool
}

// ProcessMachineFunc computes block-entry message facts from comparisons of
// the message parameter against constants.
func (p *messageContextProcessor) ProcessMachineFunc(result *Result, f *machine.FuncEffects) bool {
	params, ok := p.ctx.windowProcParams()
	if !ok || len(f.Blocks) == 0 {
		return false
	}

	entries := make(map[machine.BlockID]messageFact, len(f.Blocks))
	blockIndexByID := make(map[machine.BlockID]int, len(f.Blocks))
	queue := make([]machine.BlockID, 0, len(f.Blocks))
	inQueue := make(map[machine.BlockID]bool, len(f.Blocks))
	for i, block := range f.Blocks {
		blockIndexByID[block.Block] = i
	}
	entries[f.Blocks[0].Block] = messageFact{}
	queue = append(queue, f.Blocks[0].Block)
	inQueue[f.Blocks[0].Block] = true

	for len(queue) > 0 {
		id := queue[0]
		queue = queue[1:]
		inQueue[id] = false

		index, ok := blockIndexByID[id]
		if !ok {
			continue
		}
		exits := p.processBlock(entries[id], f.Blocks[index], f.CFG, params.msg)
		succs := make([]machine.BlockID, 0, len(exits))
		for succ := range exits {
			succs = append(succs, succ)
		}
		slices.Sort(succs)
		for _, succ := range succs {
			incoming := exits[succ]
			existing, seen := entries[succ]
			next := incoming
			if seen {
				next = mergeMessageFacts(existing, incoming)
				if messageFactsEqual(next, existing) {
					continue
				}
			}
			entries[succ] = next
			if !inQueue[succ] {
				queue = append(queue, succ)
				inQueue[succ] = true
			}
		}
	}

	messageEnum := params.msg.Type.(*typeinfo.Enum)
	messages := make(map[machine.BlockID]*typeinfo.MessageRule)
	for id, fact := range entries {
		if message := p.factMessageRule(messageEnum, fact); message != nil {
			messages[id] = message
		}
	}
	p.ctx.messageByBlock = messages
	return false
}

// processBlock propagates one block's entry fact to its successors, narrowing
// branch edges that compare the message parameter with a constant.
func (p *messageContextProcessor) processBlock(fact messageFact, block machine.BlockEffects, cfg *machine.CFG, msg *typeinfo.FunctionVar) map[machine.BlockID]messageFact {
	exits := make(map[machine.BlockID]messageFact)
	for _, effect := range block.Effects {
		switch e := effect.(type) {
		case machine.StoreEffect:
			if _, ok := p.ctx.paramStorageOffset(e.Addr, msg); ok {
				fact = messageFact{}
			}
		case machine.BranchEffect:
			trueFact, falseFact := fact, fact
			if value, op, ok := p.messageCompare(e.Predicate, msg); ok {
				switch op {
				case "==":
					trueFact, falseFact = messageFact{values: []int{value}, known: true}, fact.without(value)
				case "!=":
					trueFact, falseFact = fact.without(value), messageFact{values: []int{value}, known: true}
				}
			}
			exits[e.TrueBlock] = trueFact
			exits[e.FalseBlock] = falseFact
			return exits
		case machine.JumpEffect:
			exits[e.To] = fact
			return exits
		case machine.TableJumpEffect:
			for _, target := range e.Targets {
				exits[target] = fact
			}
			return exits
		case machine.ReturnEffect:
			return exits
		}
	}
	for _, succ := range cfg.Successors(block.Block) {
		exits[succ] = fact
	}
	return exits
}

// messageCompare matches an equality predicate between the message parameter
// and a constant message value.
func (p *messageContextProcessor) messageCompare(pred *machine.PredicateValue, msg *typeinfo.FunctionVar) (int, string, bool) {
	if pred.Kind != machine.PredicateCompare {
		return 0, "", false
	}
	op := machine.JccCompareOp(pred.Op)
	if op != "==" && op != "!=" {
		return 0, "", false
	}
	lhs, rhs := pred.LHS, pred.RHS
	if _, ok := lhs.(*machine.Const); ok {
		lhs, rhs = rhs, lhs
	}
	load, ok := lhs.(*machine.Load)
	if !ok {
		return 0, "", false
	}
	if offset, ok := p.ctx.paramStorageOffset(load.Addr, msg); !ok || offset != 0 {
		return 0, "", false
	}
	value, ok := rhs.(*machine.Const)
	if !ok {
		return 0, "", false
	}
	return int(value.Val), op, true
}

// paramStorageOffset returns the byte offset within param's storage that
// mem addresses, if mem addresses that storage at all.
func (ctx *FuncContext) paramStorageOffset(mem machine.MemoryAddress, param *typeinfo.FunctionVar) (int, bool) {
	v, offset, ok := ctx.storageVar(mem)
	if !ok {
		return 0, false
	}
	fv, ok := v.(*typeinfo.FunctionVar)
	if !ok || fv.Name != param.Name {
		return 0, false
	}
	return offset, true
}

// without returns the fact narrowed by the message parameter being known not
// to equal value. An unknown fact stays unknown.
func (f messageFact) without(value int) messageFact {
	if !f.known {
		return f
	}
	values := make([]int, 0, len(f.values))
	for _, v := range f.values {
		if v != value {
			values = append(values, v)
		}
	}
	return messageFact{values: values, known: true}
}

// mergeMessageFacts joins facts from two incoming edges; the joined block may
// handle any message of either edge, and is unknown if either edge is.
func mergeMessageFacts(a, b messageFact) messageFact {
	if !a.known || !b.known {
		return messageFact{}
	}
	values := append(slices.Clone(a.values), b.values...)
	slices.Sort(values)
	return messageFact{values: slices.Compact(values), known: true}
}

// messageFactsEqual reports whether two facts describe the same messages.
func messageFactsEqual(a, b messageFact) bool {
	return a.known == b.known && slices.Equal(a.values, b.values)
}

// factMessageRule returns the payload rule for the messages a fact allows.
// Messages without a payload rule carry nothing to view. When several
// messages have rules, each payload type is kept only where every rule that
// specifies it agrees, so the merged rule names the messages but has no
// single value.
func (p *messageContextProcessor) factMessageRule(messageEnum *typeinfo.Enum, fact messageFact) *typeinfo.MessageRule {
	if !fact.known {
		return nil
	}
	var rules []*typeinfo.MessageRule
	for _, value := range fact.values {
		if message := p.ctx.sdb.GetMessage(messageEnum, value); message != nil {
			rules = append(rules, message)
		}
	}
	switch len(rules) {
	case 0:
		return nil
	case 1:
		return rules[0]
	}
	names := make([]string, len(rules))
	wparams := make([]*typeinfo.MessagePayloadRule, len(rules))
	lparams := make([]*typeinfo.MessagePayloadRule, len(rules))
	results := make([]typeinfo.Type, len(rules))
	for i, rule := range rules {
		names[i] = rule.Name
		wparams[i] = rule.WParam
		lparams[i] = rule.LParam
		results[i] = rule.Result
	}
	return &typeinfo.MessageRule{
		Name:   strings.Join(names, "|"),
		WParam: agreedMessagePayload(wparams),
		LParam: agreedMessagePayload(lparams),
		Result: agreedMessageType(results),
	}
}

// agreedMessagePayload combines one parameter's payload rules from several
// messages, keeping each part's type only where the rules agree.
func agreedMessagePayload(payloads []*typeinfo.MessagePayloadRule) *typeinfo.MessagePayloadRule {
	var wholes, lowords, hiwords []typeinfo.Type
	for _, payload := range payloads {
		if payload != nil {
			wholes = append(wholes, payload.Whole)
			lowords = append(lowords, payload.Loword)
			hiwords = append(hiwords, payload.Hiword)
		}
	}
	if len(wholes) == 0 {
		return nil
	}
	return &typeinfo.MessagePayloadRule{
		Whole:  agreedMessageType(wholes),
		Loword: agreedMessageType(lowords),
		Hiword: agreedMessageType(hiwords),
	}
}

// agreedMessageType returns the type every specified entry agrees on, or nil
// when none is specified or two entries conflict.
func agreedMessageType(types []typeinfo.Type) typeinfo.Type {
	var agreed typeinfo.Type
	for _, typ := range types {
		switch {
		case typ == nil:
		case agreed == nil:
			agreed = typ
		case !typeinfo.Equals(agreed, typ):
			return nil
		}
	}
	return agreed
}

// windowProcMessageParams names the message and payload parameters of a
// window procedure.
type windowProcMessageParams struct {
	msg    *typeinfo.FunctionVar
	wParam *typeinfo.FunctionVar
	lParam *typeinfo.FunctionVar
}

// windowProcParams finds the message parameter, typed with the window message
// enum, and the wParam and lParam parameters that follow it.
func (ctx *FuncContext) windowProcParams() (windowProcMessageParams, bool) {
	for i := range ctx.fs.Params {
		enum, ok := ctx.fs.Params[i].Type.(*typeinfo.Enum)
		if !ok || enum.Name != typeinfo.MessageEnumName || i+2 >= len(ctx.fs.Params) {
			continue
		}
		return windowProcMessageParams{
			msg:    &ctx.fs.Params[i],
			wParam: &ctx.fs.Params[i+1],
			lParam: &ctx.fs.Params[i+2],
		}, true
	}
	return windowProcMessageParams{}, false
}

// messagePointerView returns a window procedure's wParam or lParam viewed as
// the pointer type the current block's message carries in it. Other paths,
// and parameters without a pointer payload for this message, are returned
// unchanged.
func (sr *symbolResolver) messagePointerView(path symresolve.SymbolPath) symresolve.SymbolPath {
	message := sr.currentMessage
	if message == nil {
		return path
	}
	root, ok := path.(*symresolve.SymbolRoot)
	if !ok {
		return path
	}
	param, ok := root.Symbol.(*typeinfo.FunctionVar)
	if !ok {
		return path
	}
	params, ok := sr.windowProcParams()
	if !ok {
		return path
	}
	var payload *typeinfo.MessagePayloadRule
	switch param.Name {
	case params.wParam.Name:
		payload = message.WParam
	case params.lParam.Name:
		payload = message.LParam
	}
	if payload == nil || !typeinfo.IsPointer(payload.Whole) || payload.Whole.Bytes() != param.Type.Bytes() {
		return path
	}
	return &symresolve.SymbolCast{Base: root, To: payload.Whole}
}

// messagePayloadCast views a window procedure's wParam or lParam, loaded
// whole or by word, as the type the current block's message carries there,
// such as the HDC in WM_ERASEBKGND's wParam or the control HWND in the low
// word of WM_COMMAND's lParam. The cast is only produced when the consumer
// expects exactly that type.
func (c *machineConverter) messagePayloadCast(value machine.Value, expected typeinfo.Type) (Expr, bool) {
	message := c.ctx.currentMessage
	if message == nil {
		return nil, false
	}
	load, shift, width, ok := paramWordRead(value)
	if !ok {
		return nil, false
	}
	params, ok := c.ctx.windowProcParams()
	if !ok {
		return nil, false
	}
	param, payload := params.wParam, message.WParam
	offset, ok := c.ctx.paramStorageOffset(load.Addr, param)
	if !ok {
		param, payload = params.lParam, message.LParam
		if offset, ok = c.ctx.paramStorageOffset(load.Addr, param); !ok {
			return nil, false
		}
	}
	if payload == nil {
		return nil, false
	}
	offset += shift
	if width == 0 {
		width = load.Addr.Width
	}
	var typ typeinfo.Type
	var part Expr
	switch {
	case offset == 0 && width == param.Type.Bytes():
		typ, part = payload.Whole, &Local{FunctionVar: *param}
	case offset == 0 && width == 2:
		typ, part = payload.Loword, &Word{Parent: &Local{FunctionVar: *param}, Part: machine.WordLow}
	case offset == 2 && width == 2:
		typ, part = payload.Hiword, &Word{Parent: &Local{FunctionVar: *param}, Part: machine.WordHigh}
	}
	if typ == nil || !typeinfo.Equals(typ, expected) || typeinfo.Equals(typ, param.Type) {
		return nil, false
	}
	return &Cast{Value: part, To: expected.String(), TypeInfo: expected}, true
}

// paramWordRead matches a read of a parameter's storage, whole or one word
// of it: a load, optionally shifted down 16 bits (the __aFulshr form of
// HIWORD), projected to its low word and masked to 0xFFFF. It returns the
// load, the byte offset the shift adds, and the word width read, or 0 when
// the read is the whole load.
func paramWordRead(value machine.Value) (*machine.Load, int, int, bool) {
	switch v := value.(type) {
	case *machine.Load:
		return v, 0, 0, true
	case *machine.Cast:
		// The shift helper's result is typed uint32_t; the cast keeps the
		// parameter's width.
		load, shift, width, ok := paramWordRead(v.Value)
		if !ok || v.To.Bytes() != load.Addr.Width {
			return nil, 0, 0, false
		}
		return load, shift, width, true
	case *machine.WordValue:
		if v.Part != machine.WordLow {
			return nil, 0, 0, false
		}
		load, shift, _, ok := paramWordRead(v.Parent)
		return load, shift, 2, ok
	case *machine.Binary:
		mask, ok := v.RHS.(*machine.Const)
		if !ok {
			return nil, 0, 0, false
		}
		switch {
		case v.Op == machine.ValueOpAnd && mask.Val == 0xffff:
			load, shift, _, ok := paramWordRead(v.LHS)
			return load, shift, 2, ok
		case v.Op == machine.ValueOpShr && mask.Val == 16:
			load, shift, _, ok := paramWordRead(v.LHS)
			if !ok || load.Addr.Width != 4 {
				return nil, 0, 0, false
			}
			return load, shift + 2, 2, true
		}
	}
	return nil, 0, 0, false
}

// messageResultCasts casts a window procedure's return value to the
// procedure's return type where it has the result type of the message
// handled by the block producing it, such as the HBRUSH answering
// WM_CTLCOLOR. Each arm of a shared-epilogue merge is cast using its own
// block's message.
func (ctx *FuncContext) messageResultCasts(value Expr, ret typeinfo.Type) Expr {
	merge, ok := value.(*Merge)
	if !ok {
		return messageResultCast(ctx.currentMessage, value, ret)
	}
	for i := range merge.Arms {
		merge.Arms[i].Value = messageResultCast(ctx.messageByBlock[merge.Arms[i].Block], merge.Arms[i].Value, ret)
	}
	return merge
}

// messageResultCast casts value to ret when it has message's result type.
func messageResultCast(message *typeinfo.MessageRule, value Expr, ret typeinfo.Type) Expr {
	if message == nil || message.Result == nil ||
		!typeinfo.Equals(value.ExprType(), message.Result) || typeinfo.Equals(message.Result, ret) {
		return value
	}
	return &Cast{Value: value, To: ret.String(), TypeInfo: ret}
}
