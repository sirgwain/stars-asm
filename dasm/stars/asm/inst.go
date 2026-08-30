package asm

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// Op is a small opcode classification used by the register-tracking state machine.
// It is intentionally coarse; we only model what we currently need for annotation.
type Op uint8

const (
	OpUnknown Op = iota
	OpPUSH
	OpPOP
	OpMOV
	OpADD
	OpADC
	OpSUB
	OpSBB
	OpNEG
	OpNOT
	OpCMP
	OpIMULrm
	OpIDIV
	OpDIV
	OpJMP
	OpJcc
	OpCALLF
	OpRET
	OpRETF
	OpLES   // LES r16, m32 — load far pointer: r16=offset, ES=segment
	OpLDS   // LDS r16, m32 — load far pointer: r16=offset, DS=segment (unused in my binary)
	OpSHL   // SHL r/m, imm — shift left (tracked for array-index scale detection)
	OpSHR   // SHR r/m, imm — logical shift right
	OpSAR   // SAR r/m, imm — arithmetic shift right
	OpLEA   // LEA r16, m   — load effective address of memory operand into register
	OpAND   // AND r/m, r/m — bitwise AND (tracked for 32-bit AND pair recovery)
	OpOR    // OR r/m, r/m  — bitwise OR
	OpXOR   // XOR r/m, r/m — bitwise XOR
	OpCBW   // CBW   — sign-extend AL into AX
	OpCWD   // CWD   — sign-extend AX into DX:AX
	OpNOP   // NOP   — no operation; no semantic effect
	OpMOVSW // MOVSW — word string move (ES:DI ← DS:SI, advance SI/DI)
	OpMOVSB // MOVSB — byte string move (ES:DI ← DS:SI, advance SI/DI)
	OpWAIT  // WAIT/FWAIT — synchronize with x87
	OpFILD  // FILD — load integer into x87 stack
	OpFLD   // FLD — load floating-point value onto x87 stack
	OpFLD1  // FLD1 — push 1.0 onto x87 stack
	OpFLDZ  // FLDZ — push 0.0 onto x87 stack
	OpFST   // FST — store x87 top without pop
	OpFSTP  // FSTP — store x87 top and pop
	OpFXCH  // FXCH — exchange x87 top with ST(i)
	OpFADD  // FADD — add to x87 top without pop
	OpFMUL  // FMUL — multiply x87 top without pop
	OpFCOM  // FCOM — compare x87 top with floating-point memory
	OpFCOMP // FCOMP — compare x87 top with floating-point memory and pop
	OpFSUB  // FSUB — subtract from x87 top without pop
	OpFSUBR // FSUBR — reverse subtract x87 top without pop
	OpFDIV  // FDIV — divide x87 top without pop
	OpFDIVR // FDIVR — reverse divide x87 top without pop
	OpFADDP // FADDP — add and pop
	OpFMULP // FMULP — multiply and pop
	OpFSUBP // FSUBP — subtract and pop
	OpFSUBRP
	OpFDIVP
	OpFDIVRP
	OpDW // DW — synthetic word data, used for decoded inline jump tables
)

func (o Op) IsX87() bool {
	switch o {
	case OpWAIT,
		OpFILD,
		OpFLD, OpFLD1, OpFLDZ,
		OpFST, OpFSTP,
		OpFXCH,
		OpFADD, OpFMUL,
		OpFCOM, OpFCOMP,
		OpFSUB, OpFSUBR,
		OpFDIV, OpFDIVR,
		OpFADDP, OpFMULP,
		OpFSUBP, OpFSUBRP,
		OpFDIVP, OpFDIVRP:
		return true
	default:
		return false
	}
}

// OperandKind describes a decoded operand.
type OperandKind uint8

const (
	OKNone OperandKind = iota
	OKReg
	OKSReg
	OKImm
	OKMem
	OKRel
	OKX87Reg // ST / ST(i)
)

// Reg is the asm-facing alias for the shared Win16 register type.
type Reg = typeinfo.Register

const RegNone = typeinfo.RegNone

const (
	RegAX = typeinfo.RegAX
	RegBX = typeinfo.RegBX
	RegCX = typeinfo.RegCX
	RegDX = typeinfo.RegDX
	RegSP = typeinfo.RegSP
	RegBP = typeinfo.RegBP
	RegSI = typeinfo.RegSI
	RegDI = typeinfo.RegDI
	RegES = typeinfo.RegES
	RegCS = typeinfo.RegCS
	RegDS = typeinfo.RegDS
	RegSS = typeinfo.RegSS
	RegAL = typeinfo.RegAL
	RegBL = typeinfo.RegBL
	RegCL = typeinfo.RegCL
	RegDL = typeinfo.RegDL
	RegAH = typeinfo.RegAH
	RegBH = typeinfo.RegBH
	RegCH = typeinfo.RegCH
	RegDH = typeinfo.RegDH
)

// DecodedInst is the structured instruction record produced by decode().
// Mnemonic/Operands are for printing; Op/Dst/Src are for state updates.
type DecodedInst struct {
	Bytes    []byte
	Off      uint32
	Len      int
	Mnemonic string
	Op       Op
	Dst      Operand
	Src      Operand

	// absolute offset of jump/branch target, or -1 if none
	Target int32
	// Fixup is the resolved relocation for immediate CALLF operands.
	Fixup *Fixup

	// Render-time prefix flags (do NOT bake these into Mnemonic in decode).
	hasREP   bool
	hasREPNE bool
	hasLOCK  bool

	// is this instruction a RET/RETF
	stop bool
}

func (i DecodedInst) HasREP() bool {
	return i.hasREP
}

func (i DecodedInst) HasREPNE() bool {
	return i.hasREPNE
}

func (i DecodedInst) HasRepeatPrefix() bool {
	return i.hasREP || i.hasREPNE
}

func (i DecodedInst) HasFloatMemOperand() bool {
	if !i.Dst.isFloatMemOperandFor(i.Op) && !i.Src.isFloatMemOperandFor(i.Op) {
		return false
	}
	return true
}

// Operand is a tiny operand representation used by decoding and later passes.
type Operand struct {
	Kind     OperandKind
	Reg      Reg
	Imm      uint16
	Mem      MemRef
	X87Index int8   // X87Index is valid when Kind == OKX87Reg; 0 means ST/ST(0).
	Fixup    *Fixup // immediate fixups for things like MakeProcInstance function pointers

	// for JCC, this is the relative offset
	rel int16
}

func (o Operand) isFloatMemOperandFor(op Op) bool {
	if o.Kind != OKMem {
		return false
	}
	switch op {
	case OpFLD, OpFST, OpFSTP,
		OpFADD, OpFMUL, OpFCOM, OpFCOMP,
		OpFSUB, OpFSUBR, OpFDIV, OpFDIVR:
		switch o.Mem.MemSize {
		case 4, 8, 10:
			return true
		default:
			return false
		}
	default:
		return false
	}
}

func (o Operand) String() string {
	switch o.Kind {
	case OKNone:
		return ""
	case OKReg, OKSReg:
		return o.Reg.String()
	case OKImm:
		return fmt.Sprintf("0x%04x", o.Imm)
	case OKMem:
		return o.Mem.String()
	case OKRel:
		// RAW: do not use i.Target/L_#### labels.
		return fmt.Sprintf("%+d", o.rel)
	case OKX87Reg:
		return o.X87String()
	default:
		return "?"
	}
}

func (o Operand) X87String() string {
	if o.Kind != OKX87Reg {
		return "?"
	}
	if o.X87Index == 0 {
		return "st"
	}
	return fmt.Sprintf("st(%d)", o.X87Index)
}

func (o Operand) Width() int {
	if o.Kind == OKMem && o.Mem.MemSize > 0 {
		return o.Mem.MemSize
	}
	if (o.Kind == OKReg || o.Kind == OKSReg) && o.Reg.IsByte() {
		return 1
	}
	return 2
}

// [1234h]       ; zero regs: Direct=true, Base=None, Index=None, dispLen=2
// [bx]          ; one reg:  Base=BX, Index=None
// [bp+6]        ; one reg plus displacement: Base=BP, Index=None, Disp=6
// [bx+si]       ; two regs: Base=BX, Index=SI
// [bp+di+8]     ; two regs plus displacement: Base=BP, Index=DI, Disp=8
// cs:[bx+0x229] ; one reg: Base=BX, SegOverride=CS, Disp=0x229, dispLen=2
type MemRef struct {
	Base        Reg
	SegOverride Reg
	Disp        int
	Direct      bool

	MemSize int // 0 = unknown; 1/2/4/8/10 = byte count of the referenced memory value.
	dispLen int // 0/1/2 for length accounting
	index   Reg // set by move [bx+si] style index operations, not used in Stars
}

func (m MemRef) String() string {
	seg := ""
	if m.SegOverride != RegNone && m.SegOverride.IsSeg() {
		seg = m.SegOverride.String() + ":"
	}

	// Direct: [disp16]
	if m.Direct {
		// In 16-bit, direct is an unsigned 16-bit address.
		return fmt.Sprintf("%s[0x%04x]", seg, uint16(m.Disp))
	}

	inner := m.Base.String()

	// Add displacement, if any.
	if m.Disp != 0 {
		if m.Base == RegBP {
			inner += fmt.Sprintf("%+#x", m.Disp)
		} else {
			inner += fmt.Sprintf("%+#x", m.Disp)
		}
	}

	return seg + "[" + inner + "]"
}

// String returns the assembly form of the decoded instruction.
func (i DecodedInst) String() string {
	mn := i.Mnemonic
	if mn == "" {
		mn = "???"
	}
	ops := i.Operands()
	if ops == "" {
		return mn
	}
	return fmt.Sprintf("%-9s %-20s", mn, ops)
}

// StringRaw returns the raw assembly form of the decoded instruction.
// It intentionally avoids semantic display fields like CallExpr/CallName
// and prints only the mnemonic + raw operands.
func (i DecodedInst) StringRaw() string {
	mn := i.Mnemonic
	if mn == "" {
		mn = "???"
	}
	ops := i.rawOperands()
	if ops == "" {
		return mn
	}
	return fmt.Sprintf("%-9s %-20s", mn, ops)
}

// Operands renders Operands with fixups, jump targets, etc
func (i *DecodedInst) Operands() string {
	var ops string
	if i.Dst.Kind != OKNone {
		ops = i.Dst.String()
	}

	if i.Op == OpDW && i.Target >= 0 {
		return fmt.Sprintf("L_%04x", i.Target)
	}

	if (i.Op == OpJMP || i.Op == OpJcc) && i.Target >= 0 {
		return fmt.Sprintf("L_%04x", i.Target)
	}

	if i.Op == OpCALLF && i.Fixup != nil {
		if s, ok := i.fixupOperands(); ok {
			return s
		}
	}

	// add src operand if present
	if i.Src.Kind != OKNone {
		if ops != "" {
			ops += ", "
		}
		ops += i.Src.String()
	}
	return ops
}

func (i *DecodedInst) fixupOperands() (string, bool) {
	s := i.Fixup.String()
	return s, s != ""
}

// rawOperands renders operands with no extras, just as they were loaded
func (i *DecodedInst) rawOperands() string {
	var ops string
	if i.Dst.Kind != OKNone {
		ops = i.Dst.String()
	}

	if i.Op == OpCALLF && i.Dst.Kind == OKImm && i.Src.Kind == OKImm {
		return fmt.Sprintf("%04x:%04x", i.Src.Imm, i.Dst.Imm)
	}

	// add src operand if present
	if i.Src.Kind != OKNone {
		if ops != "" {
			ops += ", "
		}
		ops += i.Src.String()
	}
	return ops
}
