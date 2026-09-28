package sem

import (
	"maps"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// windowContextProcessor records the window class of control handles a
// function obtains from a dialog, so the control messages sent to them can be
// named and typed by that class.
type windowContextProcessor struct {
	ctx *FuncContext
}

// ProcessMachineFunc records the class of each GetDlgItem result that names
// a control of a known dialog, then flows those classes through the stores
// to locals so each message send sees the control its local holds there. A
// local such as hwndItem may hold different controls on different paths.
func (p *windowContextProcessor) ProcessMachineFunc(result *Result, f *machine.FuncEffects) bool {
	calls := make(map[uint32]*typeinfo.WindowClass)
	for _, block := range f.Blocks {
		for _, effect := range block.Effects {
			call, ok := effect.(machine.CallEffect)
			if !ok || call.Target == nil || call.Target.Name != "GetDlgItem" || len(call.Args) != 2 {
				continue
			}
			res, ok := call.Result.(*machine.CallResult)
			if !ok {
				continue
			}
			if class := p.ctx.dialogControlClass(call.Args[0], call.Args[1]); class != nil {
				calls[res.InstOff] = class
			}
		}
	}
	p.ctx.callWindowClasses = calls
	p.ctx.loadWindowClasses = make(map[machine.ValueID]*typeinfo.WindowClass)
	if len(f.Blocks) == 0 {
		return false
	}

	blockIndexByID := make(map[machine.BlockID]int, len(f.Blocks))
	for i, block := range f.Blocks {
		blockIndexByID[block.Block] = i
	}
	entries := map[machine.BlockID]localWindowClasses{f.Blocks[0].Block: {}}
	queue := []machine.BlockID{f.Blocks[0].Block}
	inQueue := map[machine.BlockID]bool{f.Blocks[0].Block: true}
	for len(queue) > 0 {
		id := queue[0]
		queue = queue[1:]
		inQueue[id] = false
		index, ok := blockIndexByID[id]
		if !ok {
			continue
		}
		exit := p.processBlock(entries[id], f.Blocks[index])
		for _, succ := range f.CFG.Successors(id) {
			next := exit
			if existing, seen := entries[succ]; seen {
				next = mergeLocalWindowClasses(existing, exit)
				if maps.Equal(next, existing) {
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
	return false
}

// localWindowClasses maps a local to the window class of the control it
// holds; a nil class means the local holds something else or conflicting
// controls. A local without an entry has not been assigned on any path.
type localWindowClasses map[string]*typeinfo.WindowClass

// processBlock applies one block's stores to locals and records the class of
// each local sent a message, returning the state at the block's exit.
func (p *windowContextProcessor) processBlock(entry localWindowClasses, block machine.BlockEffects) localWindowClasses {
	state := maps.Clone(entry)
	for _, effect := range block.Effects {
		switch e := effect.(type) {
		case machine.StoreEffect:
			if local, ok := p.ctx.localVar(e.Addr); ok {
				var class *typeinfo.WindowClass
				if res, ok := e.Src.(*machine.CallResult); ok {
					class = p.ctx.callWindowClasses[res.InstOff]
				}
				state[local.Name] = class
			}
		case machine.CallEffect:
			if len(e.Args) == 0 {
				continue
			}
			load, ok := e.Args[0].(*machine.Load)
			if !ok {
				continue
			}
			if local, ok := p.ctx.localVar(load.Addr); ok {
				if class, ok := state[local.Name]; ok {
					p.ctx.loadWindowClasses[load.ID] = class
				}
			}
		}
	}
	return state
}

// mergeLocalWindowClasses joins the states of two incoming edges. A local
// assigned on only one path keeps that path's class; paths that disagree
// leave the class unknown.
func mergeLocalWindowClasses(a, b localWindowClasses) localWindowClasses {
	out := maps.Clone(a)
	for name, class := range b {
		if existing, ok := out[name]; ok && existing != class {
			out[name] = nil
			continue
		}
		out[name] = class
	}
	return out
}

// localVar returns the current function's local (not parameter) whose whole
// storage mem addresses.
func (ctx *FuncContext) localVar(mem machine.MemoryAddress) (*typeinfo.FunctionVar, bool) {
	v, offset, ok := ctx.storageVar(mem)
	if !ok || offset != 0 || ctx.isParam(v) {
		return nil, false
	}
	local, ok := v.(*typeinfo.FunctionVar)
	return local, ok
}

// storageVar returns the variable whose storage mem addresses and the byte
// offset within it.
func (ctx *FuncContext) storageVar(mem machine.MemoryAddress) (typeinfo.Var, int, bool) {
	path, ok := ctx.symbols.exactMemoryPath(mem)
	if !ok {
		return nil, 0, false
	}
	path, offset, _ := symbolOffsetRoot(path)
	root, ok := path.(*symresolve.SymbolRoot)
	if !ok {
		return nil, 0, false
	}
	return root.Symbol, offset, true
}

// isParam reports whether v is one of the current function's parameters.
func (ctx *FuncContext) isParam(v typeinfo.Var) bool {
	fv, ok := v.(*typeinfo.FunctionVar)
	if !ok {
		return false
	}
	for i := range ctx.fs.Params {
		if ctx.fs.Params[i].Name == fv.Name {
			return true
		}
	}
	return false
}

// windowRule returns the configured rule for the HWND variable value loads.
func (ctx *FuncContext) windowRule(value machine.Value) *typeinfo.WindowRule {
	load, ok := value.(*machine.Load)
	if !ok {
		return nil
	}
	v, offset, ok := ctx.storageVar(load.Addr)
	if !ok {
		return nil
	}
	switch v := v.(type) {
	case *typeinfo.GlobalVar:
		// A rule on an HWND array global covers every element.
		if _, array := v.Type.(*typeinfo.Array); offset != 0 && !array {
			return nil
		}
		return ctx.sdb.GlobalWindow(v.Name)
	case *typeinfo.FunctionVar:
		if offset != 0 {
			return nil
		}
		return ctx.sdb.FunctionVarWindow(ctx.fs.Name, v.Name, ctx.isParam(v))
	}
	return nil
}

// windowDialog returns the dialog template of the dialog window value holds.
func (ctx *FuncContext) windowDialog(value machine.Value) (*asm.Dialog, bool) {
	rule := ctx.windowRule(value)
	if rule == nil || !rule.HasDialog {
		return nil, false
	}
	return ctx.img.Dialog(uint16(rule.Dialog))
}

// dialogControlClass returns the window class of the control with the
// constant id in the dialog window hDlg holds.
func (ctx *FuncContext) dialogControlClass(hDlg, id machine.Value) *typeinfo.WindowClass {
	dialog, ok := ctx.windowDialog(hDlg)
	if !ok {
		return nil
	}
	constID, ok := id.(*machine.Const)
	if !ok {
		return nil
	}
	control, ok := dialog.Control(uint16(constID.Val))
	if !ok {
		return nil
	}
	return ctx.sdb.GetWindowClass(control.Class)
}

// windowClass returns the window class of the control window value holds:
// a GetDlgItem result, a variable with a configured class, or a local
// holding a GetDlgItem result where it is sent a message.
func (ctx *FuncContext) windowClass(value machine.Value) *typeinfo.WindowClass {
	if res, ok := value.(*machine.CallResult); ok {
		return ctx.callWindowClasses[res.InstOff]
	}
	if rule := ctx.windowRule(value); rule != nil {
		return rule.Class
	}
	if load, ok := value.(*machine.Load); ok {
		return ctx.loadWindowClasses[load.ID]
	}
	return nil
}
