package sem

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// recordContextProcessor records, for each block, which struct a
// discriminated buffer view reads: a record buffer such as rgbCur holds
// whichever record was read last, and a comparison or switch on its record
// type, such as hdrCur.rt == rtPlanet, tells which one a block sees.
type recordContextProcessor struct {
	ctx *FuncContext
}

// recordDiscriminator locates a buffer view's discriminator in storage: a
// variable, the byte offset of the discriminating field within it, and the
// field's bitfield when it has one.
type recordDiscriminator struct {
	name     string
	global   bool
	offset   int
	bitfield *typeinfo.Bitfield
}

// ProcessMachineFunc computes the block-entry values of each discriminated
// buffer view's discriminator and the struct the view reads in each block.
func (p *recordContextProcessor) ProcessMachineFunc(result *Result, f *machine.FuncEffects) bool {
	views := p.ctx.sdb.FunctionDiscriminatedBufferViews(p.ctx.fs.Name)
	if len(views) == 0 || len(f.Blocks) == 0 {
		return false
	}
	byBlock := make(map[machine.BlockID]map[*typeinfo.BufferView]*typeinfo.Struct)
	for _, view := range views {
		disc, ok := p.discriminator(view)
		if !ok {
			p.ctx.log.Error("buffer view discriminator not found", "func", view.Func, "path", view.Discriminator)
			continue
		}
		facts := p.discriminatorFacts(f, disc)
		// A block that writes the buffer out as a record type fills it as
		// that record, whatever the discriminator held on entry.
		for id, fact := range p.writtenRecordFacts(f, view) {
			facts[id] = fact
		}
		for id, fact := range facts {
			strct := viewStruct(view, fact)
			if strct == nil {
				continue
			}
			if byBlock[id] == nil {
				byBlock[id] = make(map[*typeinfo.BufferView]*typeinfo.Struct)
			}
			byBlock[id][view] = strct
		}
	}
	p.ctx.bufferViewsByBlock = byBlock
	return false
}

// discriminator resolves a view's discriminator path to its storage.
func (p *recordContextProcessor) discriminator(view *typeinfo.BufferView) (recordDiscriminator, bool) {
	disc := recordDiscriminator{name: view.Discriminator[0]}
	var typ typeinfo.Type
	if v, ok := p.ctx.functionVar(disc.name); ok {
		typ = v.Type
	} else if global := p.ctx.sdb.GetGlobal(disc.name); global != nil {
		disc.global, typ = true, global.Type
	} else {
		return recordDiscriminator{}, false
	}
	for _, name := range view.Discriminator[1:] {
		strct, ok := typ.(*typeinfo.Struct)
		if !ok {
			return recordDiscriminator{}, false
		}
		field := fieldByName(strct, name)
		if field == nil {
			return recordDiscriminator{}, false
		}
		disc.offset += field.Offset
		disc.bitfield = field.Bitfield
		typ = field.Type
	}
	return disc, true
}

// writtenRecordFacts returns the record type each block writes its viewed
// buffer out as: a call passing a constant of the view's discriminator enum
// together with the buffer, as in WriteMemRt(rtLogPlanetRouting, 6, rgbCur).
// A block writing the buffer as more than one record type has no fact.
func (p *recordContextProcessor) writtenRecordFacts(f *machine.FuncEffects, view *typeinfo.BufferView) map[machine.BlockID]messageFact {
	facts := make(map[machine.BlockID]messageFact)
	for _, block := range f.Blocks {
		var values []int
		for _, effect := range block.Effects {
			call, ok := effect.(machine.CallEffect)
			if !ok || call.Target == nil {
				continue
			}
			if value, ok := p.writtenRecordType(call, view); ok && !slices.Contains(values, value) {
				values = append(values, value)
			}
		}
		if len(values) == 1 {
			facts[block.Block] = messageFact{values: values, known: true}
		}
	}
	return facts
}

// writtenRecordType returns the record type a call writes the viewed buffer
// out as: the constant it passes for a parameter of the view's discriminator
// enum, when it also passes the buffer for a pointer parameter.
func (p *recordContextProcessor) writtenRecordType(call machine.CallEffect, view *typeinfo.BufferView) (int, bool) {
	value, haveType, haveBuffer := 0, false, false
	for i, param := range call.Target.Params {
		if i >= len(call.Args) {
			break
		}
		switch {
		case param.Type == view.DiscriminatorEnum:
			c, ok := call.Args[i].(*machine.Const)
			if !ok {
				return 0, false
			}
			value, haveType = int(c.Val), true
		case typeinfo.IsPointer(param.Type):
			haveBuffer = haveBuffer || p.passesBuffer(call.Args[i], view)
		}
	}
	return value, haveType && haveBuffer
}

// passesBuffer reports whether a call argument is the viewed buffer: the
// address of an array buffer, or the value of a pointer to it.
func (p *recordContextProcessor) passesBuffer(arg machine.Value, view *typeinfo.BufferView) bool {
	switch a := arg.(type) {
	case *machine.Const:
		// a DGROUP global's near address
		root, offset, ok := p.ctx.symbols.globalAddressBase(p.ctx.segFromRegister(asm.RegDS), uint32(uint16(a.Val)))
		return ok && offset == 0 && view.Var(root.(*symresolve.SymbolRoot).Symbol)
	case *machine.Address:
		name, global, offset, ok := p.ctx.accessVar(a.Addr)
		return ok && offset == 0 && isViewVar(view, name, global)
	case *machine.Load:
		name, global, offset, ok := p.ctx.accessVar(a.Addr)
		return ok && offset == 0 && isViewVar(view, name, global)
	}
	return false
}

// isViewVar reports whether the named variable is the one view covers.
func isViewVar(view *typeinfo.BufferView, name string, global bool) bool {
	if global {
		return view.Global == name
	}
	return view.Param == name || view.Local == name
}

// discriminatorFacts propagates the values the discriminator may hold
// through the function: comparisons and switches on it narrow them, and a
// store to it, or for a global any call, forgets them.
func (p *recordContextProcessor) discriminatorFacts(f *machine.FuncEffects, disc recordDiscriminator) map[machine.BlockID]messageFact {
	entries := map[machine.BlockID]messageFact{f.Blocks[0].Block: {}}
	blockIndexByID := make(map[machine.BlockID]int, len(f.Blocks))
	for i, block := range f.Blocks {
		blockIndexByID[block.Block] = i
	}
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
		exits := p.processBlock(entries[id], f.Blocks[index], f.CFG, disc)
		succs := make([]machine.BlockID, 0, len(exits))
		for succ := range exits {
			succs = append(succs, succ)
		}
		slices.Sort(succs)
		for _, succ := range succs {
			next := exits[succ]
			if existing, seen := entries[succ]; seen {
				next = mergeMessageFacts(existing, next)
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
	return entries
}

// processBlock propagates one block's discriminator fact to its successors.
func (p *recordContextProcessor) processBlock(fact messageFact, block machine.BlockEffects, cfg *machine.CFG, disc recordDiscriminator) map[machine.BlockID]messageFact {
	exits := make(map[machine.BlockID]messageFact)
	for _, effect := range block.Effects {
		switch e := effect.(type) {
		case machine.StoreEffect:
			if name, global, _, ok := p.ctx.accessVar(e.Addr); ok && name == disc.name && global == disc.global {
				fact = messageFact{}
			}
		case machine.CallEffect:
			if disc.global {
				fact = messageFact{}
			}
		case machine.BranchEffect:
			trueFact, falseFact := fact, fact
			if value, op, ok := p.compare(e.Predicate, disc); ok {
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
			base, ok := p.tableBase(e.Index, disc)
			for i, target := range e.Targets {
				next := fact
				if ok {
					next = messageFact{values: []int{base + i}, known: true}
				}
				if existing, seen := exits[target]; seen {
					next = mergeMessageFacts(existing, next)
				}
				exits[target] = next
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

// compare matches an equality between the discriminator and a constant.
func (p *recordContextProcessor) compare(pred *machine.PredicateValue, disc recordDiscriminator) (int, string, bool) {
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
	value, ok := rhs.(*machine.Const)
	if !ok || !p.readsDiscriminator(lhs, disc) {
		return 0, "", false
	}
	return int(value.Val), op, true
}

// tableBase returns the discriminator value of a jump table's first entry
// when the table switches on the discriminator: its index is the value,
// possibly less a constant, scaled to the word table.
func (p *recordContextProcessor) tableBase(index machine.Value, disc recordDiscriminator) (int, bool) {
	scaled, ok := index.(*machine.Binary)
	if !ok {
		return 0, false
	}
	if factor, ok := machineShiftAmount(scaled.RHS); !ok || !(scaled.Op == machine.ValueOpShl && factor == 1 || scaled.Op == machine.ValueOpMul && factor == 2) {
		return 0, false
	}
	value, base := scaled.LHS, 0
	if shifted, ok := value.(*machine.Binary); ok && (shifted.Op == machine.ValueOpSub || shifted.Op == machine.ValueOpAdd) {
		if k, ok := shifted.RHS.(*machine.Const); ok {
			value, base = shifted.LHS, int(k.Val)
			if shifted.Op == machine.ValueOpAdd {
				base = -base
			}
		}
	}
	if !p.readsDiscriminator(value, disc) {
		return 0, false
	}
	return base, true
}

// readsDiscriminator reports whether value is a read of the discriminator:
// a load of its storage, or the bitfield extract of it.
func (p *recordContextProcessor) readsDiscriminator(value machine.Value, disc recordDiscriminator) bool {
	if disc.bitfield != nil {
		read, ok := recognizeBitfieldRead(p.ctx, value)
		if !ok || read.Access.BitOff != disc.bitfield.BitOffset || read.Access.BitWidth != disc.bitfield.BitWidth {
			return false
		}
		name, global, offset, ok := p.ctx.accessVar(read.Load.Addr)
		return ok && name == disc.name && global == disc.global && offset == disc.offset
	}
	load, ok := value.(*machine.Load)
	if !ok {
		return false
	}
	name, global, offset, ok := p.ctx.accessVar(load.Addr)
	return ok && name == disc.name && global == disc.global && offset == disc.offset
}

// accessVar returns the variable a direct memory access reads or writes,
// whether it is a global, and the byte offset within it.
func (ctx *FuncContext) accessVar(mem machine.MemoryAddress) (string, bool, int, bool) {
	access, ok := ctx.symbols.varAccessFromMemory(mem)
	if !ok {
		return "", false, 0, false
	}
	switch a := access.(type) {
	case *symresolve.LocalAccess:
		return a.Local.Name, false, a.FieldOff, true
	case *symresolve.GlobalAccess:
		return a.Global.Name, true, a.FieldOff, true
	}
	return "", false, 0, false
}

// functionVar returns the current function's param or local named name.
func (ctx *FuncContext) functionVar(name string) (*typeinfo.FunctionVar, bool) {
	for i := range ctx.fs.Params {
		if ctx.fs.Params[i].Name == name {
			return &ctx.fs.Params[i], true
		}
	}
	for i := range ctx.fs.Vars {
		if ctx.fs.Vars[i].Name == name {
			return &ctx.fs.Vars[i], true
		}
	}
	return nil, false
}

// viewStruct returns the struct a view reads where its discriminator holds
// one of fact's values. When the values select different structs, as the
// cargo transfers whose records differ only in their quantity width, it is
// the first of them limited to the leading fields they all share, so reads of
// the common header resolve and reads past it stay unresolved.
func viewStruct(view *typeinfo.BufferView, fact messageFact) *typeinfo.Struct {
	if !fact.known || len(fact.values) == 0 {
		return nil
	}
	values := slices.Sorted(slices.Values(fact.values))
	strct := view.Views[values[0]]
	if strct == nil {
		return nil
	}
	shared := len(strct.Fields)
	for _, value := range values[1:] {
		next := view.Views[value]
		if next == nil {
			return nil
		}
		if next != strct {
			shared = min(shared, sharedFieldCount(strct, next))
		}
	}
	if shared == len(strct.Fields) {
		return strct
	}
	if shared == 0 {
		return nil
	}
	header := typeinfo.Struct{Name: strct.Name, Typedef: strct.Typedef, SKind: strct.SKind, Size: strct.Size, Fields: slices.Clone(strct.Fields[:shared])}
	header.FinalizeLayout()
	return &header
}

// sharedFieldCount returns how many leading fields a and b have in common:
// the same name, offset, type and bit range.
func sharedFieldCount(a, b *typeinfo.Struct) int {
	n := 0
	for n < len(a.Fields) && n < len(b.Fields) && sameStructField(&a.Fields[n], &b.Fields[n]) {
		n++
	}
	return n
}
