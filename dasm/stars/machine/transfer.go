package machine

import (
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// processBlock processes all instructions in a block, modifying incoming
// state and returning outgoing state
func (ctx *extractor) processBlock(stIn *state, cfg *CFG, blk *Block) (*state, []Effect) {
	ctx.currentBlock = blk
	defer func() {
		ctx.currentBlock = nil
	}()

	// ctx.log.Debug("processing block", "block", blk.ID)
	stOut := stIn.clone()

	insts := cfg.BlockInstrs(blk.ID)
	var effects []Effect
	for i := range insts {
		effects = append(effects, ctx.processInst(stOut, cfg, blk, insts, i)...)
	}

	return stOut, effects
}

// processInst advances ValueState for one instruction and returns raw effects.
// It intentionally does not emit sem.Events or choose final C lvalues.
func (ctx *extractor) processInst(st *state, cfg *CFG, blk *Block, instrs []asm.DecodedInst, instIdx int) []Effect {
	inst := instrs[instIdx]
	meta := Meta{BlockID: blk.ID, InstOff: inst.Off}

	if inst.Mnemonic == "DW" {
		return nil
	}

	if call := cfg.Calls[inst.Off]; call != nil {
		return ctx.handleCALLF(st, call, nextInst(instrs, instIdx), meta)
	}

	if jump := cfg.Jumps[inst.Off]; jump != nil {
		if len(jump.TableTargetOffs) > 0 {
			// TODO: handle table target jumps
			return []Effect{UnknownEffect{MetaInfo: meta, Inst: inst, Why: "unhandled table jump"}}
		}

		if inst.Op == asm.OpJMP {
			return []Effect{JumpEffect{MetaInfo: meta, To: cfg.jumpTarget(jump)}}
		}
		return ctx.handleJcc(st, cfg, blk, inst, jump, meta)
	}

	switch inst.Op {
	case asm.OpMOV:
		return ctx.handleMOV(st, inst, meta)
	case asm.OpLEA:
		ctx.handleLEA(st, inst)
		return nil
	case asm.OpLES:
		ctx.handleLES(st, inst)
		return nil
	case asm.OpMOVSW:
		return ctx.handleMOVS(st, inst, meta, 2, inst.HasRepeatPrefix())
	case asm.OpMOVSB:
		return ctx.handleMOVS(st, inst, meta, 1, inst.HasRepeatPrefix())
	case asm.OpADD, asm.OpADC:
		return ctx.handleBinary(st, inst, meta, ValueOpAdd)
	case asm.OpSUB, asm.OpSBB:
		return ctx.handleBinary(st, inst, meta, ValueOpSub)
	case asm.OpAND:
		return ctx.handleBinary(st, inst, meta, ValueOpAnd)
	case asm.OpOR:
		return ctx.handleBinary(st, inst, meta, ValueOpOr)
	case asm.OpXOR:
		return ctx.handleBinary(st, inst, meta, ValueOpXor)
	case asm.OpSHL:
		return ctx.handleBinary(st, inst, meta, ValueOpShl)
	case asm.OpSHR:
		return ctx.handleBinary(st, inst, meta, ValueOpShr)
	case asm.OpSAR:
		return ctx.handleBinary(st, inst, meta, ValueOpSar)
	case asm.OpIMULrm:
		ctx.handleIMULrm(st, inst)
		return nil
	case asm.OpIDIV:
		ctx.handleDIV(st, inst, true)
		return nil
	case asm.OpDIV:
		ctx.handleDIV(st, inst, false)
		return nil
	case asm.OpNEG:
		return ctx.handleUnary(st, inst, meta, ValueOpNeg)
	case asm.OpNOT:
		return ctx.handleUnary(st, inst, meta, ValueOpNot)
	case asm.OpCMP:
		ctx.handleCmp(st, inst)
		return nil
	case asm.OpPUSH:
		return ctx.handlePUSH(st, inst, meta)
	case asm.OpPOP:
		return ctx.handlePOP(st, inst, meta)
	case asm.OpRET, asm.OpRETF:
		return ctx.handleRET(st, meta)
	case asm.OpCBW:
		ctx.handleCBW(st)
		return nil
	case asm.OpCWD:
		ctx.handleCWD(st)
		return nil
	case asm.OpCALLF:
		return ctx.handleCALLFUnknown(st, inst, nextInst(instrs, instIdx), meta)
	case asm.OpNOP:
		return nil
	default:
		if inst.Op.IsX87() {
			return ctx.handleX87(st, inst, meta)
		}
	}

	return []Effect{UnknownEffect{MetaInfo: meta, Inst: inst, Why: "unhandled instruction"}}
}

func nextInst(instrs []asm.DecodedInst, instIdx int) *asm.DecodedInst {
	if instIdx+1 >= len(instrs) {
		return nil
	}
	return &instrs[instIdx+1]
}

func (ctx *extractor) handleCALLF(st *state, call *InstCall, next *asm.DecodedInst, meta Meta) []Effect {
	target := call.Target
	if handleCompilerFlagHelper(st, target) {
		return nil
	}

	words := numCleanupWordsAfterCall(target, next)

	var args []Value
	if target.Conv == typeinfo.CCDxaxCx {
		args = getRegisterCallArgs(st, target)
	} else {
		args = getCallFArgs(st, target, words)
	}

	st.flags = nil

	result := &CallResult{Target: target, Type: target.Ret, InstOff: meta.InstOff}
	if isFloatType(target.Ret) {
		st.killFP()
		st.pushFP(result)
		st.writeReg(asm.RegAX, WordVal(result, WordLow))
		st.writeReg(asm.RegDX, UnknownVal("floatret"))
		return []Effect{CallEffect{MetaInfo: meta, Target: target, Args: args, Result: result}}
	}
	switch target.ReturnWords() {
	case 1:
		st.writeReg(asm.RegAX, result)
	case 2:
		if typeinfo.IsFarPointer(target.Ret) {
			st.writeReg(asm.RegAX, FarPointerVal(result, FarPointerOffset))
			st.writeReg(asm.RegDX, FarPointerVal(result, FarPointerSegment))
		} else {
			st.writeReg(asm.RegAX, WordVal(result, WordLow))
			st.writeReg(asm.RegDX, WordVal(result, WordHigh))
		}
	}

	return []Effect{CallEffect{MetaInfo: meta, Target: target, Args: args, Result: result}}
}

// handleCompilerFlagHelper applies compiler helper calls that only update
// condition flags.
func handleCompilerFlagHelper(st *state, target *typeinfo.Function) bool {
	if target == nil || !strings.EqualFold(target.Name, "__aFfcompp") {
		return false
	}
	lhs := st.peekFP(0)
	if lhs == nil {
		lhs = UnknownVal("fp")
	}
	rhs := st.peekFP(1)
	if rhs == nil {
		rhs = UnknownVal("fp")
	}
	st.flags = &PredicateValue{Kind: PredicateCompare, LHS: lhs, RHS: rhs, Op: "cmp"}
	st.popFP()
	st.popFP()
	return true
}

// handle a func pointer like
// CALLF     [bx+0x6]
func (ctx *extractor) handleCALLFUnknown(st *state, inst asm.DecodedInst, next *asm.DecodedInst, meta Meta) []Effect {
	poppedBytes := uint16(0)
	if next != nil && next.Op == asm.OpPOP && next.Dst.Kind == asm.OKReg && next.Dst.Reg == asm.RegSP {
		poppedBytes = next.Src.Imm
	} else if next != nil && next.Op == asm.OpADD && next.Dst.Kind == asm.OKReg && next.Dst.Reg == asm.RegSP && next.Src.Kind == asm.OKImm {
		poppedBytes = next.Src.Imm
	}
	rawWords := st.popStackWords(int((poppedBytes + 1) / 2))

	var access MemoryAccess
	if inst.Src.Kind == asm.OKMem {
		access = st.memoryAccessFromOperand(inst.Off, OperandDst, inst.Src)
	}

	return []Effect{CallEffect{MetaInfo: meta, MemoryAccess: access, Args: rawWords}}
}

func (ctx *extractor) handleJcc(st *state, cfg *CFG, blk *Block, inst asm.DecodedInst, jump *InstJump, meta Meta) []Effect {
	trueBlock, falseBlock := cfg.branchTargets(blk, jump)

	pred := PredicateVal(PredicateFromFlags, inst.Mnemonic).(*PredicateValue)
	if st.flags != nil {
		flagsPred := *st.flags
		pred = &flagsPred
		pred.Op = inst.Mnemonic
	}
	return []Effect{BranchEffect{MetaInfo: meta, Predicate: pred, TrueBlock: trueBlock, FalseBlock: falseBlock}}
}

func (ctx *extractor) handleMOV(st *state, inst asm.DecodedInst, meta Meta) []Effect {
	src := st.readOperand(inst.Off, OperandSrc, inst.Src)

	// update state with this write. If it's a memory write like
	// MOV [mem], imm
	// that's a store effect
	mem, isMem := st.writeOperand(inst.Off, OperandDst, inst.Dst, src)
	if !isMem {
		return nil
	}

	return []Effect{StoreEffect{MetaInfo: meta, Addr: mem, Src: src, Width: mem.Width}}
}

func (ctx *extractor) handleLEA(st *state, inst asm.DecodedInst) {
	dst := inst.Dst
	mem := st.memoryAccessFromOperandLike(inst.Off, OperandSrc, inst.Src)

	// LEA computes only the offset word. Keep Addr provenance so a later
	// segment:offset pair can recover &local.
	st.writeReg(dst.Reg, AddressVal(mem))
}

// handleLES applies LES by loading a 4-byte far pointer value into ES:<dst>.
// LES bx, [lppl] loads the offset word stored at lppl into BX and the segment
// word stored at lppl+2 into ES.
func (ctx *extractor) handleLES(st *state, inst asm.DecodedInst) {
	dst := inst.Dst
	src := LoadVal(st.memoryAccessFromOperandLikeWidth(inst.Off, OperandSrc, inst.Src, 4))

	st.writeReg(asm.RegES, FarPointerVal(src, FarPointerSegment))
	st.writeReg(dst.Reg, FarPointerVal(src, FarPointerOffset))
}

// handleMOVS applies MOVSB/MOVSW as a concrete memory copy and advances SI/DI.
func (ctx *extractor) handleMOVS(st *state, inst asm.DecodedInst, meta Meta, elemWidth int, repeat bool) []Effect {
	count := 1
	if repeat {
		cx, ok := st.readReg(asm.RegCX).(*Const)
		if !ok {
			advanceUnknownMOVSState(st, inst, repeat)
			return []Effect{UnknownEffect{MetaInfo: meta, Inst: inst, Why: "REP MOVS count unresolved"}}
		}
		count = int(cx.Val)
	}

	width := elemWidth * count
	if width == 0 {
		advanceMOVSState(st, inst, width, repeat)
		return nil
	}

	dst := AddressVal(st.memoryAccessFromOperandLike(inst.Off, OperandDst, inst.Dst))
	src := AddressVal(st.memoryAccessFromOperandLike(inst.Off, OperandSrc, inst.Src))
	advanceMOVSState(st, inst, width, repeat)
	return []Effect{CopyEffect{MetaInfo: meta, Dst: dst, Src: src, Width: width}}
}

// advanceMOVSState advances implicit MOVS registers after a known-size copy.
func advanceMOVSState(st *state, inst asm.DecodedInst, bytes int, repeat bool) {
	advanceMOVSReg(st, movsBase(inst.Src, asm.RegSI), bytes)
	advanceMOVSReg(st, movsBase(inst.Dst, asm.RegDI), bytes)
	if repeat {
		st.writeReg(asm.RegCX, ConstVal(0))
	}
}

// advanceUnknownMOVSState conservatively clobbers MOVS index state.
func advanceUnknownMOVSState(st *state, inst asm.DecodedInst, repeat bool) {
	st.writeReg(movsBase(inst.Src, asm.RegSI), UnknownVal("movs"))
	st.writeReg(movsBase(inst.Dst, asm.RegDI), UnknownVal("movs"))
	if repeat {
		st.writeReg(asm.RegCX, ConstVal(0))
	}
}

// advanceMOVSReg adds bytes to a MOVS index register.
func advanceMOVSReg(st *state, reg asm.Reg, bytes int) {
	if reg == asm.RegNone || bytes == 0 {
		return
	}
	v := st.readReg(reg)
	if c, ok := v.(*Const); ok {
		st.writeReg(reg, ConstVal(c.Val+uint(bytes)))
		return
	}
	st.writeReg(reg, BinaryResult(ValueOpAdd, v, ConstVal(uint(bytes))))
}

// movsBase returns the base register used by an implicit MOVS memory operand.
func movsBase(op asm.Operand, def asm.Reg) asm.Reg {
	if op.Kind == asm.OKMem && op.Mem.Base != asm.RegNone {
		return op.Mem.Base
	}
	return def
}

func (ctx *extractor) handleBinary(st *state, inst asm.DecodedInst, meta Meta, op ValueOp) []Effect {
	if op == ValueOpSub && sameFullRegisterOperands(inst.Dst, inst.Src) {
		mem, isMem := st.writeOperand(inst.Off, OperandDst, inst.Dst, ConstVal(0))
		if isMem {
			return []Effect{StoreEffect{MetaInfo: meta, Addr: mem, Src: ConstVal(0), Width: mem.Width}}
		}
		return nil
	}

	lhs := st.readOperand(inst.Off, OperandDst, inst.Dst)
	rhs := st.readOperand(inst.Off, OperandSrc, inst.Src)
	out := BinaryResult(op, lhs, rhs)

	mem, isMem := st.writeOperand(inst.Off, OperandDst, inst.Dst, out)
	if inst.Dst.Kind == asm.OKReg && inst.Dst.Reg == asm.RegSP {
		if inst.Src.Kind == asm.OKImm {
			st.noteSPAdjust(op, int(inst.Src.Imm))
		} else {
			st.noteSPAdjust(op, 0)
		}
	}
	if isMem {
		return []Effect{StoreEffect{MetaInfo: meta, Addr: mem, Src: out, Width: mem.Width}}
	}
	return nil
}

func (ctx *extractor) handleUnary(st *state, inst asm.DecodedInst, meta Meta, op ValueOp) []Effect {
	lhs := st.readOperand(inst.Off, OperandDst, inst.Dst)
	out := BinaryVal(op, lhs, ConstVal(0))

	mem, isMem := st.writeOperand(inst.Off, OperandDst, inst.Dst, out)
	if isMem {
		return []Effect{StoreEffect{MetaInfo: meta, Addr: mem, Src: out, Width: mem.Width}}
	}
	return nil
}

// handleCBW sign-extends AL into AX.
func (ctx *extractor) handleCBW(st *state) {
	st.writeReg(asm.RegAX, signExtendByteWord(st.readReg(asm.RegAL)))
}

// handleCWD sign-extends AX into DX.
func (ctx *extractor) handleCWD(st *state) {
	st.writeReg(asm.RegDX, signExtendHighWord(st.readReg(asm.RegAX)))
}

// signExtendByteWord returns the 16-bit signed extension of an 8-bit value.
func signExtendByteWord(v Value) Value {
	if c, ok := v.(*Const); ok {
		b := c.Val & 0xff
		if b&0x80 != 0 {
			return ConstVal(b | 0xff00)
		}
		return ConstVal(b)
	}
	return SignExtendVal(v, 8, 16)
}

// signExtendHighWord returns the high word produced by signed 16-to-32 extension.
func signExtendHighWord(v Value) Value {
	if c, ok := v.(*Const); ok {
		if c.Val&0x8000 != 0 {
			return ConstVal(0xffff)
		}
		return ConstVal(0)
	}
	return WordVal(v, WordSignHigh)
}

// handleIMULrm applies one-operand signed word multiply into DX:AX.
func (ctx *extractor) handleIMULrm(st *state, inst asm.DecodedInst) {
	lhs := st.readReg(asm.RegAX)
	rhs := st.readOperand(inst.Off, OperandSrc, inst.Src)
	low, high := imulWordResult(lhs, rhs)

	st.writeReg(asm.RegAX, low)
	st.writeReg(asm.RegDX, high)
}

// handleDIV applies one-operand word DIV/IDIV into AX quotient and DX remainder.
func (ctx *extractor) handleDIV(st *state, inst asm.DecodedInst, signed bool) {
	dividend := stackWordsValue([]Value{st.readReg(asm.RegAX), st.readReg(asm.RegDX)})
	divisor := st.readOperand(inst.Off, OperandSrc, inst.Src)
	quotient, remainder := divWordResult(dividend, divisor, signed)

	st.writeReg(asm.RegAX, quotient)
	st.writeReg(asm.RegDX, remainder)
}

// handleX87 applies tracked x87 floating-point stack instructions.
func (ctx *extractor) handleX87(st *state, inst asm.DecodedInst, meta Meta) []Effect {
	switch inst.Op {
	case asm.OpWAIT:
		return nil
	case asm.OpFILD:
		st.pushFP(CastVal(st.readOperand(inst.Off, OperandSrc, inst.Src), typeinfo.Double))
	case asm.OpFLD1:
		st.pushFP(FloatConstVal(1.0))
	case asm.OpFLDZ:
		st.pushFP(FloatConstVal(0.0))
	case asm.OpFLD:
		st.pushFP(ctx.fpOperandValue(st, inst.Off, OperandSrc, inst.Src))
	case asm.OpFXCH:
		st.swapFP(x87Index(inst.Src, 1))
	case asm.OpFADD:
		ctx.handleFPBinary(st, inst, ValueOpAdd, false)
	case asm.OpFMUL:
		ctx.handleFPBinary(st, inst, ValueOpMul, false)
	case asm.OpFSUB:
		ctx.handleFPBinary(st, inst, ValueOpSub, false)
	case asm.OpFSUBR:
		ctx.handleFPBinary(st, inst, ValueOpSub, true)
	case asm.OpFDIV:
		ctx.handleFPBinary(st, inst, ValueOpDiv, false)
	case asm.OpFDIVR:
		ctx.handleFPBinary(st, inst, ValueOpDiv, true)
	case asm.OpFCOM:
		return nil
	case asm.OpFCOMP:
		st.popFP()
	case asm.OpFST:
		return ctx.handleFST(st, inst, meta, false)
	case asm.OpFSTP:
		return ctx.handleFST(st, inst, meta, true)
	case asm.OpFADDP:
		ctx.handleFPBinaryPop(st, inst, ValueOpAdd, false)
	case asm.OpFMULP:
		ctx.handleFPBinaryPop(st, inst, ValueOpMul, false)
	case asm.OpFSUBP:
		ctx.handleFPBinaryPop(st, inst, ValueOpSub, false)
	case asm.OpFSUBRP:
		ctx.handleFPBinaryPop(st, inst, ValueOpSub, true)
	case asm.OpFDIVP:
		ctx.handleFPBinaryPop(st, inst, ValueOpDiv, false)
	case asm.OpFDIVRP:
		ctx.handleFPBinaryPop(st, inst, ValueOpDiv, true)
	default:
		st.killFP()
	}
	return nil
}

// fpOperandValue returns the value read by an x87 operand.
func (ctx *extractor) fpOperandValue(st *state, off uint32, role OperandRole, op asm.Operand) Value {
	switch op.Kind {
	case asm.OKX87Reg:
		if v := st.peekFP(int(op.X87Index)); v != nil {
			return v
		}
		return UnknownVal("fp")
	case asm.OKMem:
		if value, ok := ctx.floatLiteralValue(op); ok {
			return value
		}
		if v, ok := hiddenBufferFPReturnValue(st, op); ok {
			return v
		}
		return st.readOperand(off, role, op)
	default:
		return st.readOperand(off, role, op)
	}
}

// floatLiteralValue resolves direct x87 memory operands that point at constants.
func (ctx *extractor) floatLiteralValue(op asm.Operand) (Value, bool) {
	if !op.Mem.Direct {
		return nil, false
	}
	seg, ok := ctx.dsReg.(*Reg)
	if !ok {
		return nil, false
	}
	if op.Mem.SegOverride == asm.RegCS {
		seg, ok = ctx.csReg.(*Reg)
		if !ok {
			return nil, false
		}
	}
	if value, ok := ctx.res.ResolveFloatLiteral(int(ctx.segFromRegister(seg.Val)), uint32(op.Mem.Disp), op.Mem.MemSize); ok {
		return FloatConstVal(value), true
	}
	return nil, false
}

// x87Index returns an x87 register index or def when op is not an x87 register.
func x87Index(op asm.Operand, def int) int {
	if op.Kind == asm.OKX87Reg {
		return int(op.X87Index)
	}
	return def
}

// handleFST stores ST(0), optionally popping it for FSTP.
func (ctx *extractor) handleFST(st *state, inst asm.DecodedInst, meta Meta, pop bool) []Effect {
	top := st.peekFP(0)
	if top == nil {
		top = UnknownVal("fp")
	}

	if trackAsStackSlot(st, inst.Dst) {
		if inst.Dst.Kind == asm.OKMem {
			st.fpCallSlots = append(st.fpCallSlots, fpCallSlot{
				depth: st.outgoingStackBytes,
				disp:  inst.Dst.Mem.Disp,
				value: top,
			})
		}
		if pop {
			st.popFP()
		}
		return nil
	}

	mem, isMem := st.writeOperand(inst.Off, OperandDst, inst.Dst, top)
	if pop {
		st.popFP()
	}
	if !isMem {
		return nil
	}
	return []Effect{StoreEffect{MetaInfo: meta, Addr: mem, Src: top, Width: inst.Dst.Width()}}
}

// hiddenBufferFPReturnValue recognizes FLD [reg] after a floating-point call return.
func hiddenBufferFPReturnValue(st *state, op asm.Operand) (Value, bool) {
	if op.Kind != asm.OKMem || op.Mem.Disp != 0 || op.Mem.Base == asm.RegNone {
		return nil, false
	}
	parent, ok := lowWordParent(st.readReg(op.Mem.Base))
	if !ok {
		return nil, false
	}
	call, ok := parent.(*CallResult)
	if !ok || !isFloatType(call.Type) {
		return nil, false
	}
	return parent, true
}

// lowWordParent returns the parent value of a low-word projection.
func lowWordParent(v Value) (Value, bool) {
	word, ok := v.(*WordValue)
	if !ok || word.Part != WordLow {
		return nil, false
	}
	return word.Parent, true
}

// trackAsStackSlot reports whether an x87 store targets reserved outgoing stack space.
func trackAsStackSlot(st *state, op asm.Operand) bool {
	if op.Kind != asm.OKMem || op.Mem.Direct {
		return false
	}
	if op.Mem.Base == asm.RegSP {
		return true
	}
	if op.Mem.Base == asm.RegBX {
		return st.readReg(asm.RegBX) == st.readReg(asm.RegSP)
	}
	return false
}

// handleFPBinaryPop applies an x87 binary operation to ST(i) and ST(0), then pops.
func (ctx *extractor) handleFPBinaryPop(st *state, inst asm.DecodedInst, op ValueOp, reversed bool) {
	dstIdx := x87Index(inst.Dst, 1)
	dst := st.peekFP(dstIdx)
	src := st.peekFP(0)
	st.setFP(dstIdx, fpBinaryResult(op, dst, src, reversed))
	st.popFP()
}

// handleFPBinary applies an x87 binary operation to ST(0) and the instruction operand.
func (ctx *extractor) handleFPBinary(st *state, inst asm.DecodedInst, op ValueOp, reversed bool) {
	dst := st.peekFP(0)
	src := ctx.fpOperandValue(st, inst.Off, OperandSrc, inst.Src)
	st.setFP(0, fpBinaryResult(op, dst, src, reversed))
}

// fpBinaryResult returns the symbolic x87 binary operation result.
func fpBinaryResult(op ValueOp, dst, src Value, reversed bool) Value {
	if dst == nil || src == nil {
		return UnknownVal("fp")
	}
	if reversed {
		return BinaryResult(op, src, dst)
	}
	return BinaryResult(op, dst, src)
}

// BinaryResult converts a binary operation into a simplified value.
func BinaryResult(op ValueOp, lhs, rhs Value) Value {
	lc, lok := lhs.(*Const)
	rc, rok := rhs.(*Const)
	if lok && rok && (op != ValueOpDiv && op != ValueOpMod || rc.Val != 0) {
		switch op {
		case ValueOpAdd:
			return ConstVal(lc.Val + rc.Val)
		case ValueOpSub:
			return ConstVal(lc.Val - rc.Val)
		case ValueOpMul:
			return ConstVal(lc.Val * rc.Val)
		case ValueOpAnd:
			return ConstVal(lc.Val & rc.Val)
		case ValueOpOr:
			return ConstVal(lc.Val | rc.Val)
		case ValueOpXor:
			return ConstVal(lc.Val ^ rc.Val)
		case ValueOpShl:
			return ConstVal(lc.Val << rc.Val)
		case ValueOpShr, ValueOpSar:
			return ConstVal(lc.Val >> rc.Val)
		case ValueOpDiv:
			return ConstVal(lc.Val / rc.Val)
		case ValueOpMod:
			return ConstVal(lc.Val % rc.Val)
		}
	}
	if op == ValueOpAdd {
		if rok {
			if out, ok := combineAddConst(lhs, rc.Val); ok {
				return out
			}
		}
		if lok {
			if out, ok := combineAddConst(rhs, lc.Val); ok {
				return out
			}
		}
	}
	if op == ValueOpAnd {
		if rok && rc.Val == 0 {
			return ConstVal(0)
		}
		if lok && lc.Val == 0 {
			return ConstVal(0)
		}
		if rok {
			if out, ok := simplifyByteWriteMask(lhs, rc.Val); ok {
				return out
			}
		}
		if lok {
			if out, ok := simplifyByteWriteMask(rhs, lc.Val); ok {
				return out
			}
		}
	}
	return BinaryVal(op, lhs, rhs)
}

// combineAddConst folds a constant into the tail of an existing offset expression.
func combineAddConst(v Value, add uint) (Value, bool) {
	bv, ok := v.(*Binary)
	if !ok {
		return nil, false
	}
	tail, ok := bv.RHS.(*Const)
	if !ok {
		return nil, false
	}
	switch bv.Op {
	case ValueOpAdd:
		return BinaryResult(ValueOpAdd, bv.LHS, ConstVal(tail.Val+add)), true
	default:
		return nil, false
	}
}

// simplifyByteWriteMask removes byte-write parents that are discarded by an
// AND mask, such as setlobyte(cx, v) & 0xff.
func simplifyByteWriteMask(v Value, mask uint) (Value, bool) {
	bv, ok := v.(*ByteValue)
	if !ok || bv.Value == nil {
		return nil, false
	}
	switch bv.Part {
	case ByteLow:
		switch mask & 0xffff {
		case 0x00ff:
			return maskedByteValue(*bv.Value), true
		case 0xff00:
			return BinaryResult(ValueOpAnd, bv.Parent, ConstVal(0xff00)), true
		}
	case ByteHigh:
		switch mask & 0xffff {
		case 0x00ff:
			return BinaryResult(ValueOpAnd, bv.Parent, ConstVal(0x00ff)), true
		case 0xff00:
			return BinaryResult(ValueOpShl, maskedByteValue(*bv.Value), ConstVal(8)), true
		}
	}
	return nil, false
}

// maskedByteValue returns v when it is already known to be byte-wide, otherwise
// it records an explicit low-byte mask.
func maskedByteValue(v Value) Value {
	if isByteWideValue(v) {
		return v
	}
	return BinaryResult(ValueOpAnd, v, ConstVal(0x00ff))
}

// isByteWideValue reports whether v cannot contain bits outside the low byte.
func isByteWideValue(v Value) bool {
	switch x := v.(type) {
	case *Const:
		return x.Val&^uint(0xff) == 0
	case *Load:
		return x.Access.Width == 1
	case *ByteValue:
		return x.Value == nil
	}
	return false
}

// imulWordResult returns the low and high words produced by signed 16-bit IMUL.
func imulWordResult(lhs, rhs Value) (Value, Value) {
	if lc, lok := lhs.(*Const); lok {
		if rc, rok := rhs.(*Const); rok {
			product := int32(int16(lc.Val&0xffff)) * int32(int16(rc.Val&0xffff))
			u := uint32(product)
			return ConstVal(uint(u & 0xffff)), ConstVal(uint((u >> 16) & 0xffff))
		}
	}

	product := BinaryVal(ValueOpMul, lhs, rhs)
	return WordVal(product, WordLow), WordVal(product, WordHigh)
}

// divWordResult returns word quotient and remainder values for DIV/IDIV.
func divWordResult(dividend, divisor Value, signed bool) (Value, Value) {
	dividendConst, dividendOK := constWideWordValue(dividend)
	divisorConst, divisorOK := divisor.(*Const)
	if dividendOK && divisorOK && divisorConst.Val&0xffff != 0 {
		if signed {
			lhs := int32(dividendConst)
			rhs := int32(int16(divisorConst.Val & 0xffff))
			return ConstVal(uint(uint16(lhs / rhs))), ConstVal(uint(uint16(lhs % rhs)))
		}
		lhs := uint32(dividendConst)
		rhs := uint32(divisorConst.Val & 0xffff)
		return ConstVal(uint(uint16(lhs / rhs))), ConstVal(uint(uint16(lhs % rhs)))
	}

	return BinaryVal(ValueOpDiv, dividend, divisor), BinaryVal(ValueOpMod, dividend, divisor)
}

// constWideWordValue returns a DX:AX-style constant from a two-word value.
func constWideWordValue(v Value) (uint32, bool) {
	words, ok := v.(*StackWords)
	if !ok || len(words.Words) != 2 {
		return 0, false
	}
	lo, loOK := words.Words[0].(*Const)
	hi, hiOK := words.Words[1].(*Const)
	if !loOK || !hiOK {
		return 0, false
	}
	return uint32(lo.Val&0xffff) | uint32(hi.Val&0xffff)<<16, true
}

// isFloatType reports whether typ is a floating-point type.
func isFloatType(typ typeinfo.Type) bool {
	return typ != nil && typ.Kind() == typeinfo.KFloat
}

func (ctx *extractor) handleCmp(st *state, inst asm.DecodedInst) {
	lhs := st.readOperand(inst.Off, OperandDst, inst.Dst)
	rhs := st.readOperand(inst.Off, OperandSrc, inst.Src)

	st.flags = &PredicateValue{Kind: PredicateCompare, LHS: lhs, RHS: rhs, Op: "cmp"}
}

func (ctx *extractor) handlePUSH(st *state, inst asm.DecodedInst, meta Meta) []Effect {
	v := st.readOperand(inst.Off, OperandSrc, inst.Src)
	st.push(stackWord{value: v, meta: meta})
	return nil
}

func (ctx *extractor) handlePOP(st *state, inst asm.DecodedInst, meta Meta) []Effect {
	w, ok := st.pop()
	if !ok {
		w = stackWord{value: UnknownVal("stack"), meta: meta}
	}
	mem, isMem := st.writeOperand(inst.Off, OperandDst, inst.Dst, w.value)
	if isMem {
		return []Effect{StoreEffect{MetaInfo: meta, Addr: mem, Src: w.value, Width: mem.Width}}
	}
	return nil
}

func (ctx *extractor) handleRET(st *state, meta Meta) []Effect {
	if ctx.fs.Ret.Kind() == typeinfo.KVoid {
		return []Effect{ReturnEffect{MetaInfo: meta}}
	}

	// get a value for the return registers
	var retVal Value
	if isFloatType(ctx.fs.Ret) {
		retVal = st.peekFP(0)
		if retVal == nil {
			retVal = UnknownVal("fp")
		}
	} else {
		switch ctx.fs.ReturnWords() {
		case 1:
			retVal = st.readReg(asm.RegAX)
		case 2:
			retVal = typedWordsValue([]Value{st.readReg(asm.RegAX), st.readReg(asm.RegDX)}, ctx.fs.Ret)
		default:
			retVal = UnknownVal("return")
		}
	}

	return []Effect{ReturnEffect{MetaInfo: meta, Value: retVal}}
}
