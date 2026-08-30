package machine

import (
	"fmt"
	"log/slog"
	"maps"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
)

// state is the effect-layer abstract machine state.
type state struct {
	regs map[asm.Reg]Value
	seq  map[asm.Reg]int
	tick int

	flags *PredicateValue

	stack []stackWord
	fp    [8]Value
	fpd   int

	outgoingStackBytes int
	fpCallSlots        []fpCallSlot

	// warnStackWordUnderflow controls diagnostics for CALLF/stack argument pops
	// that ask for more words than this abstract state currently carries. The
	// worklist solver may execute blocks from partial predecessor state while
	// converging, so it leaves this off to avoid false positives from transient
	// underflows. After the fixpoint is stable, extraction replays each block from
	// its final entry state with this enabled; warnings emitted then describe a
	// real final machine-state gap instead of solver churn.
	warnStackWordUnderflow bool
	log                    *slog.Logger
}

type stackWord struct {
	value Value
	meta  Meta
}

type fpCallSlot struct {
	depth int
	disp  int
	value Value
}

func newValueState() *state {
	return &state{
		regs:        make(map[asm.Reg]Value),
		seq:         make(map[asm.Reg]int),
		fpCallSlots: make([]fpCallSlot, 0),
		log:         slog.New(slog.DiscardHandler),
	}
}

// initialize the state for function entry
func seedEntryState(ctx *FuncContext) *state {
	st := newValueState()
	for _, r := range []asm.Reg{asm.RegAX, asm.RegBX, asm.RegCX, asm.RegDX, asm.RegSI, asm.RegDI, asm.RegBP, asm.RegSP} {
		st.regs[r] = ScalarVal(r.String())
	}
	for _, r := range []asm.Reg{asm.RegDS, asm.RegES, asm.RegSS, asm.RegCS} {
		st.regs[r] = ScalarVal(r.String())
	}

	// write the actual values we care about for DS/CS registers
	st.writeReg(asm.RegDS, ctx.dsReg)
	st.writeReg(asm.RegSS, ctx.ssReg)
	st.writeReg(asm.RegCS, ctx.csReg)
	st.writeReg(asm.RegSP, BinaryVal(ValueOpAdd, st.regs[asm.RegSP], ConstVal(0)))
	return st
}

func (st *state) clone() *state {
	out := newValueState()
	maps.Copy(out.regs, st.regs)
	maps.Copy(out.seq, st.seq)
	out.tick = st.tick
	if st.flags != nil {
		cp := *st.flags
		out.flags = &cp
	}
	out.stack = append(out.stack, st.stack...)
	out.fp = st.fp
	out.fpd = st.fpd
	out.outgoingStackBytes = st.outgoingStackBytes
	out.fpCallSlots = append(out.fpCallSlots, st.fpCallSlots...)
	out.warnStackWordUnderflow = st.warnStackWordUnderflow
	return out
}

func (st *state) readReg(r asm.Reg) Value {
	if r.IsHiByte() {
		return ByteVal(st.regs[r.FullReg()], ByteHigh)
	}
	if r.IsLoByte() {
		return ByteVal(st.regs[r.FullReg()], ByteLow)
	}
	if v, ok := st.regs[r]; ok {
		return v
	}
	panic(fmt.Errorf("read invalid register %s", r.String()))
}

func (st *state) writeReg(r asm.Reg, v Value) {
	if r == asm.RegNone {
		panic("write invalid register")
	}
	if r.IsByte() {
		full := r.FullReg()
		part := ByteLow
		if r.IsHiByte() {
			part = ByteHigh
		}
		st.regs[full] = ByteWriteVal(st.regs[full], part, v)
		return
	}
	st.regs[r] = v
	st.tick++
	st.seq[r] = st.tick
}

func (st *state) push(w stackWord) {
	st.stack = append(st.stack, w)
}

func (st *state) pop() (stackWord, bool) {
	if len(st.stack) == 0 {
		return stackWord{}, false
	}
	w := st.stack[len(st.stack)-1]
	st.stack = st.stack[:len(st.stack)-1]

	return w, true
}

func (st *state) popStackWords(n int) []Value {
	if n <= 0 {
		return nil
	}
	if n > len(st.stack) {
		if st.warnStackWordUnderflow {
			st.log.Warn("popStackWords has more words than stack state", "n", n, "stack", len(st.stack))
		}
		words := make([]Value, n)
		missing := n - len(st.stack)
		for i := 0; i < missing; i++ {
			words[i] = UnknownVal("stack")
		}
		for i, w := range st.stack {
			words[missing+i] = w.value
		}
		st.stack = nil
		return words
	}
	words := make([]Value, n)
	for i := range n {
		words[i] = st.stack[len(st.stack)-n+i].value
	}
	st.stack = st.stack[:len(st.stack)-n]

	return words
}

func (st *state) resetOutgoingFPArgs() {
	st.outgoingStackBytes = 0
	st.fpCallSlots = st.fpCallSlots[:0]
}

// noteSPAdjust tracks stack space reserved for outgoing floating-point args.
// Some calls pass x87 values by reserving stack space, storing ST(0) through a
// register alias of SP, then calling:
//
//	SUB  sp, 0x0008
//	FILD [l]
//	MOV  bx, sp
//	FSTP [bx]
//	CALL sqrt
//	ADD  sp, 0x0008
//
// The SUB opens an outgoing-arg window, FSTP records the floating value inside
// that window, and the ADD closes it after CALLF has drained the staged arg.
func (st *state) noteSPAdjust(op ValueOp, bytes int) {
	if bytes <= 0 {
		if st.outgoingStackBytes > 0 {
			st.resetOutgoingFPArgs()
		}
		return
	}
	switch op {
	case ValueOpSub:
		st.outgoingStackBytes += bytes
	case ValueOpAdd:
		if st.outgoingStackBytes == 0 {
			return
		}
		st.outgoingStackBytes -= bytes
		if st.outgoingStackBytes <= 0 {
			st.resetOutgoingFPArgs()
		}
	default:
		if st.outgoingStackBytes > 0 {
			st.resetOutgoingFPArgs()
		}
	}
}

// drainFPStackSlotArgs returns floating-point args staged into outgoing stack slots.
func (st *state) drainFPStackSlotArgs() []Value {
	if len(st.fpCallSlots) == 0 {
		st.outgoingStackBytes = 0
		return nil
	}
	type slot struct {
		off   int
		value Value
	}
	slots := make([]slot, 0, len(st.fpCallSlots))
	finalDepth := st.outgoingStackBytes
	for _, fp := range st.fpCallSlots {
		off := finalDepth - fp.depth + fp.disp
		slots = append(slots, slot{off: off, value: fp.value})
	}
	for i := 1; i < len(slots); i++ {
		for j := i; j > 0 && slots[j].off < slots[j-1].off; j-- {
			slots[j], slots[j-1] = slots[j-1], slots[j]
		}
	}
	out := make([]Value, len(slots))
	for i, sl := range slots {
		out[i] = sl.value
	}
	st.resetOutgoingFPArgs()
	return out
}

// pushFP pushes a value onto the x87 register stack.
func (st *state) pushFP(v Value) bool {
	if st.fpd >= len(st.fp) {
		return false
	}
	for i := st.fpd; i > 0; i-- {
		st.fp[i] = st.fp[i-1]
	}
	st.fp[0] = v
	st.fpd++
	return true
}

// popFP pops the top value from the x87 register stack.
func (st *state) popFP() Value {
	if st.fpd == 0 {
		return nil
	}
	v := st.fp[0]
	for i := 0; i < st.fpd-1; i++ {
		st.fp[i] = st.fp[i+1]
	}
	st.fpd--
	st.fp[st.fpd] = nil
	return v
}

// peekFP returns the x87 stack value at i, where 0 is ST(0).
func (st *state) peekFP(i int) Value {
	if i < 0 || i >= st.fpd {
		return nil
	}
	return st.fp[i]
}

// setFP writes the x87 stack value at i, where 0 is ST(0).
func (st *state) setFP(i int, v Value) {
	if i < 0 || i >= st.fpd {
		return
	}
	st.fp[i] = v
}

// swapFP exchanges ST(0) with ST(i).
func (st *state) swapFP(i int) {
	if i < 0 || i >= st.fpd {
		return
	}
	st.fp[0], st.fp[i] = st.fp[i], st.fp[0]
}

// killFP clears the tracked x87 register stack.
func (st *state) killFP() {
	for i := range st.fp {
		st.fp[i] = nil
	}
	st.fpd = 0
}

// readOperand creates a Value for a given operand (dst or src) and instruction
func (st *state) readOperand(off uint32, role OperandRole, op asm.Operand) Value {
	switch op.Kind {
	case asm.OKReg, asm.OKSReg:
		// OKReg/OKSreg
		//     MOV ax, bx
		//     ; source operand bx is a general register
		//     ; readOperand(bx) => current value of BX
		return st.readReg(op.Reg)

	case asm.OKImm:
		return ConstVal(uint(op.Imm)).
			WithFixup(op.Fixup). // for function pointers
			WithOrigin(&Origin{InstOff: off, Role: role})
	case asm.OKMem:
		return LoadVal(st.memoryAccessFromOperand(off, role, op))
	default:
		return UnknownVal("operand")
	}

}

// writeOperand writes a Value to an operand, returning a memory access, if resolved.
func (st *state) writeOperand(off uint32, role OperandRole, op asm.Operand, v Value) (MemoryAccess, bool) {
	switch op.Kind {
	case asm.OKReg, asm.OKSReg:
		st.writeReg(op.Reg, v)
	case asm.OKMem:
		// we are writing to memory
		// MOV [1234], ax 	; write ax into address 1234, 2 bytes
		// MOV [1234], 0 	; write 0 into address 1234, 2 bytes
		return st.memoryAccessFromOperand(off, role, op), true
	}
	return MemoryAccess{}, false
}

// memoryAccessFromOperand creates a MemoryAccess for a memory operand
// using the current state to populate reg values
//
// MOV [1234], ax 	   ; write ax into address 1234, 2 bytes
// MOV [1234], 0 	   ; write 0 into address 1234, 2 bytes
// MOV ax, [1234] 	   ; load from address 1234 into ax
// MOV ax, [bp-6] 	   ; load from ss:bp local var into ax
// MOV ax, es:[bx+0x2] ; load from es:bx with displacement into ax
// MOV al, es:[bx+0x2] ; load 1 byte from es:bx with displacement into al
func (st *state) memoryAccessFromOperand(off uint32, role OperandRole, op asm.Operand) MemoryAccess {
	origin := Origin{InstOff: off, Role: role}
	base := operandBase(op)
	if base != asm.RegNone && base != asm.RegBP && op.Mem.SegOverride == asm.RegNone {
		// A non-BP memory operand normally defaults to DS, but the base register
		// may hold address provenance from LEA:
		//   LEA bx, [bp-rgpflNew]
		//   ADD bx, ax
		//   MOV bx, [bx]
		// In that case [bx] dereferences the computed stack/local address, not
		// an unrelated DS offset. Preserve the address's original storage.
		if mem, ok := memoryAccessFromAddressValue(st.readReg(base), op.Mem.Disp, op.Width(), origin); ok {
			return mem
		}
	}
	mem := MemoryAccess{
		Seg:    st.readReg(operandSeg(op)),
		Disp:   op.Mem.Disp,
		Width:  op.Width(),
		Origin: origin,
	}
	if base != asm.RegNone {
		if base == asm.RegBP {
			mem.Base = FrameBaseVal()
		} else {
			mem.Base = st.readReg(base)
		}
	}
	return mem
}

// memoryAccessFromAddressValue dereferences a value known to carry address provenance.
func memoryAccessFromAddressValue(value Value, disp int, width int, origin Origin) (MemoryAccess, bool) {
	switch v := value.(type) {
	case *Address:
		access := v.Access
		access.Disp += disp
		access.Width = width
		access.Origin = origin
		return access, true
	case *Binary:
		if v.Op != ValueOpAdd {
			return MemoryAccess{}, false
		}
		if access, ok := memoryAccessFromAddressIndex(v.LHS, v.RHS, disp, width, origin); ok {
			return access, true
		}
		return memoryAccessFromAddressIndex(v.RHS, v.LHS, disp, width, origin)
	default:
		return MemoryAccess{}, false
	}
}

// memoryAccessFromAddressIndex dereferences an address value plus a dynamic byte offset.
func memoryAccessFromAddressIndex(addressValue Value, index Value, disp int, width int, origin Origin) (MemoryAccess, bool) {
	addr, ok := addressValue.(*Address)
	if !ok {
		return MemoryAccess{}, false
	}
	access := addr.Access
	access.Disp += disp
	access.Width = width
	access.Origin = origin
	if access.Index != nil {
		access.Index = BinaryVal(ValueOpAdd, access.Index, index)
	} else {
		access.Index = index
	}
	return access, true
}

// memoryAccessFromOperandLike creates a MemoryAccess for memory-shaped operands.
func (st *state) memoryAccessFromOperandLike(off uint32, role OperandRole, op asm.Operand) MemoryAccess {
	return st.memoryAccessFromOperandLikeWidth(off, role, op, op.Width())
}

// memoryAccessFromOperandLikeWidth creates a MemoryAccess with an explicit width.
func (st *state) memoryAccessFromOperandLikeWidth(off uint32, role OperandRole, op asm.Operand, width int) MemoryAccess {
	mem := st.memoryAccessFromOperand(off, role, op)
	mem.Width = width
	return mem
}

// operandSeg returns the effective segment register for a memory operand.
func operandSeg(op asm.Operand) asm.Reg {
	if op.Mem.SegOverride != asm.RegNone {
		return op.Mem.SegOverride
	}
	if op.Mem.Base == asm.RegBP {
		return asm.RegSS
	}
	return asm.RegDS
}

// operandBase returns the base register for a memory operand.
func operandBase(op asm.Operand) asm.Reg {
	return op.Mem.Base
}
