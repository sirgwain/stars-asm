package sem

import (
	"slices"

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

// messageFact is the message known on entry to a block. A fact that is not
// known means the message parameter may hold any value.
type messageFact struct {
	value int
	known bool
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
				if next == existing {
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

	messages := make(map[machine.BlockID]*typeinfo.MessageRule)
	for id, fact := range entries {
		if !fact.known {
			continue
		}
		if message := p.ctx.sdb.GetMessage(fact.value); message != nil {
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
			if p.isParamStorage(e.Addr, msg) {
				fact = messageFact{}
			}
		case machine.BranchEffect:
			trueFact, falseFact := fact, fact
			if value, op, ok := p.messageCompare(e.Predicate, msg); ok {
				switch op {
				case "==":
					trueFact, falseFact = messageFact{value: value, known: true}, messageFact{}
				case "!=":
					trueFact, falseFact = messageFact{}, messageFact{value: value, known: true}
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
	if !ok || !p.isParamStorage(load.Addr, msg) {
		return 0, "", false
	}
	value, ok := rhs.(*machine.Const)
	if !ok {
		return 0, "", false
	}
	return int(value.Val), op, true
}

// isParamStorage reports whether mem addresses the storage of param.
func (p *messageContextProcessor) isParamStorage(mem machine.MemoryAddress, param *typeinfo.FunctionVar) bool {
	path, ok := p.ctx.symbols.exactMemoryPath(mem)
	if !ok {
		return false
	}
	path, offset, _ := symbolOffsetRoot(path)
	root, ok := path.(*symresolve.SymbolRoot)
	if !ok || offset != 0 {
		return false
	}
	v, ok := root.Symbol.(*typeinfo.FunctionVar)
	return ok && v.Name == param.Name
}

// mergeMessageFacts joins facts from two incoming edges; the message stays
// known only when both edges agree.
func mergeMessageFacts(a, b messageFact) messageFact {
	if a == b {
		return a
	}
	return messageFact{}
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
