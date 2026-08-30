package machine

import "github.com/sirgwain/stars-asm/dasm/stars/asm"

// annotateBlockRegLiveness records local killed-before-read facts plus
// whole-CFG register/flag liveness. The extractor uses this to avoid merging
// machine-state history that no downstream instruction can consume.
// retReadsRegs controls whether RET/RETF consume AX:DX as a source return
// value.
func annotateBlockRegLiveness(cfg *CFG, retReadsRegs bool) {
	for _, blk := range cfg.Blocks {
		blk.KilledBeforeRead = blockRegKillsBeforeRead(cfg.Instrs, blk, retReadsRegs)
		blk.FlagsKilledBeforeRead = blockFlagsKilledBeforeRead(cfg.Instrs, blk)
	}
	annotateBlockRegLiveSets(cfg, retReadsRegs)
}

// blockRegKillsBeforeRead scans a block in instruction order and returns full
// registers whose first local access is a full-register write. This is a local
// machine liveness fact used to suppress dead register joins at CFG merges.
// retReadsRegs controls whether RET/RETF count as reads of AX:DX.
func blockRegKillsBeforeRead(instrs []asm.DecodedInst, blk *Block, retReadsRegs bool) map[asm.Reg]bool {
	out := make(map[asm.Reg]bool)
	if blk == nil {
		return out
	}
	seen := make(map[asm.Reg]bool)
	for i := blk.StartIdx; i < blk.EndIdx && i < len(instrs); i++ {
		inst := instrs[i]
		for _, r := range instReadRegs(inst, retReadsRegs) {
			r = livenessReadReg(r)
			if r == asm.RegSP {
				continue
			}
			seen[r] = true
		}
		for _, r := range instFullWriteRegs(inst) {
			if r == asm.RegSP || seen[r] {
				continue
			}
			out[r] = true
			seen[r] = true
		}
	}
	return out
}

// annotateBlockRegLiveSets computes whole-CFG live-in/live-out sets for
// registers, BP-relative offsets, and flags using a backwards fixpoint pass.
// retReadsRegs controls whether RET/RETF make AX:DX live-in.
func annotateBlockRegLiveSets(cfg *CFG, retReadsRegs bool) {
	for _, blk := range cfg.Blocks {
		blk.RegLiveIn = make(map[asm.Reg]bool)
		blk.RegLiveOut = make(map[asm.Reg]bool)
		blk.BPLiveIn = make(map[int]bool)
		blk.BPLiveOut = make(map[int]bool)
	}
	changed := true
	for changed {
		changed = false
		for i := len(cfg.Blocks) - 1; i >= 0; i-- {
			blk := cfg.Blocks[i]
			nextOut := make(map[asm.Reg]bool)
			nextBPOut := make(map[int]bool)
			nextFlagsOut := false
			for _, succID := range cfg.Successors(blk.ID) {
				succ := cfg.Block(succID)
				if succ == nil {
					continue
				}
				unionLiveSet(nextOut, succ.RegLiveIn)
				unionLiveSet(nextBPOut, succ.BPLiveIn)
				nextFlagsOut = nextFlagsOut || succ.FlagsLiveIn
			}

			nextIn, nextBPIn, nextFlagsIn := blockLiveInFromOut(cfg.Instrs, blk, nextOut, nextBPOut, nextFlagsOut, retReadsRegs)
			if !sameLiveSet(blk.RegLiveOut, nextOut) {
				blk.RegLiveOut = nextOut
				changed = true
			}
			if !sameLiveSet(blk.RegLiveIn, nextIn) {
				blk.RegLiveIn = nextIn
				changed = true
			}
			if !sameLiveSet(blk.BPLiveOut, nextBPOut) {
				blk.BPLiveOut = nextBPOut
				changed = true
			}
			if !sameLiveSet(blk.BPLiveIn, nextBPIn) {
				blk.BPLiveIn = nextBPIn
				changed = true
			}
			if blk.FlagsLiveOut != nextFlagsOut {
				blk.FlagsLiveOut = nextFlagsOut
				changed = true
			}
			if blk.FlagsLiveIn != nextFlagsIn {
				blk.FlagsLiveIn = nextFlagsIn
				changed = true
			}
		}
	}
}

// blockLiveInFromOut walks one block backwards and derives the live-in state
// needed to satisfy the supplied live-out state.
// retReadsRegs controls whether RET/RETF consume AX:DX while walking the block.
func blockLiveInFromOut(instrs []asm.DecodedInst, blk *Block, liveOut map[asm.Reg]bool, bpOut map[int]bool, flagsOut bool, retReadsRegs bool) (map[asm.Reg]bool, map[int]bool, bool) {
	live := cloneLiveSet(liveOut)
	bpLive := cloneLiveSet(bpOut)
	flagsLive := flagsOut
	if blk == nil {
		return live, bpLive, flagsLive
	}
	for i := blk.EndIdx - 1; i >= blk.StartIdx && i >= 0 && i < len(instrs); i-- {
		inst := instrs[i]
		if instWritesFlags(inst) {
			flagsLive = false
		}
		if instReadsFlags(inst) {
			flagsLive = true
		}
		for _, r := range instFullWriteRegs(inst) {
			delete(live, livenessReadReg(r))
		}
		for _, off := range instBPWriteOffsets(inst) {
			delete(bpLive, off)
		}
		for _, r := range instReadRegs(inst, retReadsRegs) {
			r = livenessReadReg(r)
			if r == asm.RegNone {
				continue
			}
			live[r] = true
		}
		for _, off := range instBPReadOffsets(inst) {
			bpLive[off] = true
		}
	}
	return live, bpLive, flagsLive
}

// cloneLiveSet copies a liveness set, preserving only true entries.
func cloneLiveSet[K comparable](in map[K]bool) map[K]bool {
	out := make(map[K]bool, len(in))
	for k, live := range in {
		if live {
			out[k] = true
		}
	}
	return out
}

// unionLiveSet adds all live keys from src into dst.
func unionLiveSet[K comparable](dst, src map[K]bool) {
	for k, live := range src {
		if live {
			dst[k] = true
		}
	}
}

// sameLiveSet reports whether two liveness sets contain the same live keys.
func sameLiveSet[K comparable](a, b map[K]bool) bool {
	for k, live := range a {
		if live != b[k] {
			return false
		}
	}
	for k, live := range b {
		if live != a[k] {
			return false
		}
	}
	return true
}

// sameFullRegisterOperands reports whether a and b name the same non-byte
// register operand.
func sameFullRegisterOperands(a, b asm.Operand) bool {
	return (a.Kind == asm.OKReg || a.Kind == asm.OKSReg) &&
		(b.Kind == asm.OKReg || b.Kind == asm.OKSReg) &&
		a.Reg == b.Reg &&
		!a.Reg.IsByte()
}

// instBPReadOffsets returns BP-relative frame offsets whose current values are
// read by inst.
func instBPReadOffsets(inst asm.DecodedInst) []int {
	var out []int
	addRead := func(op asm.Operand) {
		if off, ok := operandBPOffset(op); ok {
			out = append(out, off)
		}
	}

	switch inst.Op {
	case asm.OpMOV:
		addRead(inst.Src)
	case asm.OpLES, asm.OpLDS, asm.OpCMP:
		addRead(inst.Dst)
		addRead(inst.Src)
	case asm.OpADD, asm.OpADC, asm.OpSBB,
		asm.OpAND, asm.OpOR, asm.OpXOR,
		asm.OpSHL, asm.OpSHR, asm.OpSAR:
		addRead(inst.Dst)
		addRead(inst.Src)
	case asm.OpSUB:
		if sameFullRegisterOperands(inst.Dst, inst.Src) {
			break
		}
		addRead(inst.Dst)
		addRead(inst.Src)
	case asm.OpNEG, asm.OpNOT:
		addRead(inst.Dst)
	case asm.OpPUSH:
		addRead(inst.Src)
	case asm.OpIMULrm, asm.OpIDIV, asm.OpDIV, asm.OpCALLF:
		addRead(inst.Src)
	default:
		if inst.Op.IsX87() {
			addRead(inst.Src)
		}
	}
	return out
}

// instBPWriteOffsets returns BP-relative frame offsets fully written by inst.
func instBPWriteOffsets(inst asm.DecodedInst) []int {
	var out []int
	addWrite := func(op asm.Operand) {
		if off, ok := operandBPOffset(op); ok {
			out = append(out, off)
		}
	}

	switch inst.Op {
	case asm.OpMOV, asm.OpADD, asm.OpADC, asm.OpSUB, asm.OpSBB,
		asm.OpAND, asm.OpOR, asm.OpXOR,
		asm.OpSHL, asm.OpSHR, asm.OpSAR,
		asm.OpNEG, asm.OpNOT, asm.OpPOP:
		addWrite(inst.Dst)
	default:
		if inst.Op.IsX87() {
			addWrite(inst.Dst)
		}
	}
	return out
}

// operandBPOffset returns the BP-relative displacement for a non-direct memory
// operand.
func operandBPOffset(op asm.Operand) (int, bool) {
	if op.Kind != asm.OKMem || op.Mem.Direct || op.Mem.Base != asm.RegBP {
		return 0, false
	}
	return op.Mem.Disp, true
}

// livenessReadReg normalizes byte-register reads to their containing word
// register so an incoming AX value is considered read by AL/AH consumers.
func livenessReadReg(r asm.Reg) asm.Reg {
	if r.IsByte() {
		return r.FullReg()
	}
	return r
}

// instReadRegs returns registers read directly by the instruction before its
// writes. It intentionally models only instruction-local machine reads.
// retReadsRegs controls whether RET/RETF read AX:DX for source return values.
func instReadRegs(inst asm.DecodedInst, retReadsRegs bool) []asm.Reg {
	var regs []asm.Reg
	addOperandReadRegs := func(op asm.Operand) {
		regs = append(regs, operandReadRegs(op)...)
	}

	switch inst.Op {
	case asm.OpMOV:
		addOperandReadRegs(inst.Src)
		if inst.Dst.Kind == asm.OKMem {
			addOperandReadRegs(inst.Dst)
		}
	case asm.OpLEA:
		if inst.Src.Kind == asm.OKMem {
			regs = append(regs, memAddressRegs(inst.Src.Mem)...)
		}
	case asm.OpLES, asm.OpLDS:
		addOperandReadRegs(inst.Src)
	case asm.OpADD, asm.OpADC, asm.OpSBB,
		asm.OpAND, asm.OpOR, asm.OpXOR,
		asm.OpSHL, asm.OpSHR, asm.OpSAR:
		addOperandReadRegs(inst.Dst)
		addOperandReadRegs(inst.Src)
	case asm.OpSUB:
		if sameFullRegisterOperands(inst.Dst, inst.Src) {
			break
		}
		addOperandReadRegs(inst.Dst)
		addOperandReadRegs(inst.Src)
	case asm.OpNEG, asm.OpNOT:
		addOperandReadRegs(inst.Dst)
	case asm.OpCMP:
		addOperandReadRegs(inst.Dst)
		addOperandReadRegs(inst.Src)
	case asm.OpPUSH:
		addOperandReadRegs(inst.Src)
		regs = append(regs, asm.RegSP)
	case asm.OpPOP:
		regs = append(regs, asm.RegSP)
		if inst.Dst.Kind == asm.OKMem {
			addOperandReadRegs(inst.Dst)
		}
	case asm.OpCBW:
		regs = append(regs, asm.RegAL)
	case asm.OpCWD:
		regs = append(regs, asm.RegAX)
	case asm.OpIMULrm:
		regs = append(regs, asm.RegAX)
		addOperandReadRegs(inst.Src)
	case asm.OpIDIV, asm.OpDIV:
		regs = append(regs, asm.RegAX, asm.RegDX)
		addOperandReadRegs(inst.Src)
	case asm.OpCALLF:
		regs = append(regs, asm.RegAX, asm.RegDX, asm.RegCX)
		if inst.Src.Kind == asm.OKMem {
			addOperandReadRegs(inst.Src)
		}
	case asm.OpRETF:
		if retReadsRegs {
			regs = append(regs, asm.RegAX, asm.RegDX)
		}
	case asm.OpMOVSW, asm.OpMOVSB:
		regs = append(regs, asm.RegDS, asm.RegSI, asm.RegES, asm.RegDI)
		if inst.HasRepeatPrefix() {
			regs = append(regs, asm.RegCX)
		}
	default:
		if inst.Op.IsX87() {
			addOperandReadRegs(inst.Src)
			if inst.Dst.Kind == asm.OKMem {
				addOperandReadRegs(inst.Dst)
			}
		}
	}
	regs = append(regs, instPartialWriteFullRegs(inst)...)
	return regs
}

// instPartialWriteFullRegs returns full registers read implicitly by byte
// writes that preserve the sibling byte.
func instPartialWriteFullRegs(inst asm.DecodedInst) []asm.Reg {
	var regs []asm.Reg
	addPartialWrite := func(op asm.Operand) {
		if op.Kind == asm.OKReg && op.Reg.IsByte() {
			regs = append(regs, op.Reg.FullReg())
		}
	}

	switch inst.Op {
	case asm.OpMOV, asm.OpADD, asm.OpADC, asm.OpSUB, asm.OpSBB,
		asm.OpAND, asm.OpOR, asm.OpXOR,
		asm.OpSHL, asm.OpSHR, asm.OpSAR,
		asm.OpNEG, asm.OpNOT, asm.OpPOP:
		addPartialWrite(inst.Dst)
	}
	return regs
}

// instFullWriteRegs returns registers whose full incoming value is overwritten
// by the instruction. Byte writes are excluded because they do not kill the
// containing word register.
func instFullWriteRegs(inst asm.DecodedInst) []asm.Reg {
	var regs []asm.Reg
	addFullRegWrite := func(op asm.Operand) {
		if (op.Kind == asm.OKReg || op.Kind == asm.OKSReg) && !op.Reg.IsByte() {
			regs = append(regs, op.Reg)
		}
	}

	switch inst.Op {
	case asm.OpMOV, asm.OpLEA, asm.OpPOP:
		addFullRegWrite(inst.Dst)
	case asm.OpLES:
		addFullRegWrite(inst.Dst)
		regs = append(regs, asm.RegES)
	case asm.OpLDS:
		addFullRegWrite(inst.Dst)
		regs = append(regs, asm.RegDS)
	case asm.OpADD, asm.OpADC, asm.OpSUB, asm.OpSBB,
		asm.OpAND, asm.OpOR, asm.OpXOR,
		asm.OpSHL, asm.OpSHR, asm.OpSAR,
		asm.OpNEG, asm.OpNOT:
		addFullRegWrite(inst.Dst)
	case asm.OpCBW:
		regs = append(regs, asm.RegAX)
	case asm.OpCWD:
		regs = append(regs, asm.RegDX)
	case asm.OpIMULrm, asm.OpIDIV, asm.OpDIV:
		regs = append(regs, asm.RegAX, asm.RegDX)
	case asm.OpPUSH:
		regs = append(regs, asm.RegSP)
	case asm.OpMOVSW, asm.OpMOVSB:
		regs = append(regs, asm.RegSI, asm.RegDI)
		if inst.HasRepeatPrefix() {
			regs = append(regs, asm.RegCX)
		}
	case asm.OpCALLF:
		regs = append(regs, asm.RegAX, asm.RegDX)
	}
	return regs
}

// operandReadRegs returns registers read by an operand, including memory
// effective-address registers for memory operands.
func operandReadRegs(op asm.Operand) []asm.Reg {
	switch op.Kind {
	case asm.OKReg, asm.OKSReg:
		return []asm.Reg{op.Reg}
	case asm.OKMem:
		return memAddressRegs(op.Mem)
	default:
		return nil
	}
}

// memAddressRegs returns registers used to form a 16-bit memory effective
// address, including an explicit segment override when present.
func memAddressRegs(mem asm.MemRef) []asm.Reg {
	var regs []asm.Reg
	if mem.Base != asm.RegNone {
		regs = append(regs, mem.Base)
	}
	if mem.SegOverride != asm.RegNone {
		regs = append(regs, mem.SegOverride)
	} else if mem.Base == asm.RegBP {
		regs = append(regs, asm.RegSS)
	} else {
		regs = append(regs, asm.RegDS)
	}
	return regs
}

// blockFlagsKilledBeforeRead reports whether a block writes flags before any
// local instruction reads them.
func blockFlagsKilledBeforeRead(instrs []asm.DecodedInst, blk *Block) bool {
	if blk == nil {
		return false
	}
	for i := blk.StartIdx; i < blk.EndIdx && i < len(instrs); i++ {
		inst := instrs[i]
		if instReadsFlags(inst) {
			return false
		}
		if instWritesFlags(inst) {
			return true
		}
	}
	return false
}

// instReadsFlags reports whether inst consumes the current condition flags.
func instReadsFlags(inst asm.DecodedInst) bool {
	return inst.Op == asm.OpJcc
}

// instWritesFlags reports whether inst replaces the current condition flags.
func instWritesFlags(inst asm.DecodedInst) bool {
	switch inst.Op {
	case asm.OpCMP, asm.OpCALLF:
		return true
	default:
		return false
	}
}
