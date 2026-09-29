package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

type forwardScratchAddressesProcessor struct {
	ctx *FuncContext
}

// ProcessMachineBlock substitutes address words the compiler spilled to
// scratch back into the memory addresses that reload them. MSC recomputes a
// far element address such as &lpplProdGlob->rgprod[lSel] into a fresh
// scratch pair for each access:
//
//	MOV [bp-0x38], bx ; MOV [bp-0x36], cx
//	MOV bx, [bp-0x38] ; MOV es, [bp-0x36] ; AND es:[bx], 0xfc00
//
// Forwarding the spilled words lets later passes see that each access names
// the same storage. Only address components are rewritten; scratch loads used
// as values stay for scratch recovery and bitfield RMW matching.
func (p *forwardScratchAddressesProcessor) ProcessMachineBlock(_ *Result, _ machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	effects := b.Effects
	var out []machine.Effect
	for i, effect := range effects {
		spills := &machineRewriter{
			value: func(_ *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
				load, ok := value.(*machine.Load)
				if !ok {
					return value, false, false
				}
				spilled, ok := p.spilledAddressWord(effects, i, load)
				if !ok {
					return value, false, false
				}
				return spilled, true, true
			},
		}
		addresses := &machineRewriter{
			memory: func(_ *machineRewriter, mem machine.MemoryAddress) (machine.MemoryAddress, bool, bool) {
				next, changed := spills.rewriteMachineMemoryChildren(mem)
				return next, changed, true
			},
		}
		next, changed := addresses.rewriteMachineEffect(effect)
		if !changed {
			continue
		}
		if out == nil {
			out = append([]machine.Effect(nil), effects...)
		}
		out[i] = next
	}
	if out == nil {
		return b, false
	}
	b.Effects = out
	return b, true
}

// spilledAddressWord returns the value last stored to the scratch word a load
// reads, when nothing between that store and the load can change what the
// value reads: no calls, copies, or stores outside scratch, and the value
// itself reads no scratch. The window ends at the load's own instruction, not
// at the consuming effect, because a register loaded from scratch keeps its
// value across later stores, e.g. the high-word store of a split dword RMW.
func (p *forwardScratchAddressesProcessor) spilledAddressWord(effects []machine.Effect, use int, load *machine.Load) (machine.Value, bool) {
	if load.Addr.Width != 2 || !p.ctx.syntheticScratchAddress(load.Addr) {
		return nil, false
	}
	before := use
	if loadAt := load.Addr.Origin.InstOff; loadAt != 0 {
		for before > 0 && effects[before-1].EffectMeta().InstOff > loadAt {
			before--
		}
	}
	store, index, ok := p.ctx.reachingScratchStore(effects, before, load.Addr)
	if !ok {
		return nil, false
	}
	for _, effect := range effects[index+1 : before] {
		switch e := effect.(type) {
		case machine.StoreEffect:
			if !p.ctx.syntheticScratchAddress(e.Addr) {
				return nil, false
			}
		case machine.CallEffect, machine.CopyEffect:
			return nil, false
		}
	}
	if p.ctx.readsScratch(store.Src) {
		return nil, false
	}
	return store.Src, true
}

// readsScratch reports whether a machine value loads compiler scratch storage.
func (ctx *FuncContext) readsScratch(value machine.Value) bool {
	found := false
	finder := &machineRewriter{
		value: func(_ *machineRewriter, candidate machine.Value) (machine.Value, bool, bool) {
			if load, ok := candidate.(*machine.Load); ok && ctx.syntheticScratchAddress(load.Addr) {
				found = true
				return candidate, false, true
			}
			return candidate, false, false
		},
	}
	finder.rewriteMachineValue(value)
	return found
}
