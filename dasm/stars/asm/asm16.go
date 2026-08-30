package asm

import (
	"encoding/binary"
)

// Jcc mnemonics for opcodes 0x70..0x7F.
// We prefer the carry/borrow spellings used in your frequency list.
var jccNames = []string{
	"JO", "JNO", "JC", "JNC", "JZ", "JNZ", "JBE", "JA",
	"JS", "JNS", "JP", "JNP", "JL", "JGE", "JLE", "JG",
}

// Group 1 ALU mnemonics indexed by reg field.
var grp1Names = []string{"ADD", "OR", "ADC", "SBB", "AND", "SUB", "XOR", "CMP"}

// Group 2 shift/rotate mnemonics indexed by reg field.
var grp2Names = []string{"ROL", "ROR", "RCL", "RCR", "SHL", "SHR", "", "SAR"}

var reg16 = []Reg{
	RegAX, RegCX, RegDX, RegBX, RegSP, RegBP, RegSI, RegDI,
}

// u16 reinterprets a signed 16-bit value as its unsigned 16-bit bit pattern.
func u16(i int16) uint16 { return uint16(i) }

// simple asm decode cursor
type dec struct {
	b  []byte
	ip uint32
	i  int // cursor
}

func (d *dec) left(n int) bool { return d.i+n <= len(d.b) }
func (d *dec) u8() (uint8, bool) {
	if !d.left(1) {
		return 0, false
	}
	v := d.b[d.i]
	d.i++
	return v, true
}
func (d *dec) u16() (uint16, bool) {
	if !d.left(2) {
		return 0, false
	}
	v := binary.LittleEndian.Uint16(d.b[d.i:])
	d.i += 2
	return v, true
}
func (d *dec) peek() (uint8, bool) {
	if !d.left(1) {
		return 0, false
	}
	return d.b[d.i], true
}
func (d *dec) bytes() []byte { return d.b[:d.i] }

// decode decodes one Win16 / 8086-era instruction.
// It only covers the instruction subset we care about for Stars!.
//
// ip is the *absolute* segment offset of this instruction, used to compute
// absolute label targets.

func decode(b []byte, ip uint32) DecodedInst {
	d := &dec{b: b, ip: ip}
	noTarget := int32(-1)

	// 1) Consume any number of 8086 prefixes first.
	pfx := d.parsePrefixes()

	// 2) Read the real opcode byte (after prefixes).
	op, ok := d.u8()
	if !ok {
		return DecodedInst{Mnemonic: "DB", Len: 1, Target: noTarget}
	}

	// 3) Dispatch by opcode.
	if h := primary[op]; h != nil {
		in := h(d, pfx, op)
		applyPrefixesToInst(&in, pfx)
		return in
	}

	// 4) Fallback: compact range handlers.
	if in, ok := decodeRanges(d, pfx, op); ok {
		applyPrefixesToInst(&in, pfx)
		return in
	}

	return db1(d.b)
}

// ---- Opcode dispatch table + small handlers ----

type opHandler func(d *dec, pfx prefixes, op byte) DecodedInst

var primary [256]opHandler

func init() {
	buildDecodeTable()
}

func buildDecodeTable() {
	// Helpers for registering handlers.
	set := func(op byte, h opHandler) { primary[op] = h }

	// 0-operand singles.
	set(OP_NOP, h0("NOP", OpNOP))
	set(OP_WAIT, h0("WAIT", OpWAIT))
	set(0xA4, hStringMovs(false))
	set(0xA5, hStringMovs(true))
	set(0x9C, h0("PUSHF", OpUnknown))
	set(0x9D, h0("POPF", OpUnknown))
	set(0xC9, h0("LEAVE", OpUnknown))
	set(0xF8, h0("CLC", OpUnknown))
	set(0xF9, h0("STC", OpUnknown))
	set(0xFA, h0("CLI", OpUnknown))
	set(0xFB, h0("STI", OpUnknown))
	set(0xFC, h0("CLD", OpUnknown))
	set(0xFD, h0("STD", OpUnknown))
	set(0xAA, hStringStos(false))
	set(0xAB, hStringStos(true))
	set(0xAC, hStringLods(false))
	set(0xAD, hStringLods(true))
	set(0xAE, hStringScas(false))
	set(0xAF, hStringScas(true))

	// PUSH/POP segment registers.
	set(OP_PUSH_ES, hPushSeg(RegES))
	set(OP_POP_ES, hPopSeg(RegES))
	set(OP_PUSH_SS, hPushSeg(RegSS))
	set(OP_POP_SS, hPopSeg(RegSS))
	set(OP_PUSH_DS, hPushSeg(RegDS))
	set(OP_POP_DS, hPopSeg(RegDS))

	// Sign/extend helpers.
	set(OP_CBW, func(d *dec, _ prefixes, _ byte) DecodedInst {
		return mk0(d, "CBW", OpCBW, Operand{Kind: OKReg, Reg: RegAX}, Operand{Kind: OKReg, Reg: RegAL})
	})
	set(OP_CWD, func(d *dec, _ prefixes, _ byte) DecodedInst {
		return mk0(d, "CWD", OpCWD, Operand{Kind: OKReg, Reg: RegDX}, Operand{Kind: OKReg, Reg: RegAX})
	})

	// AX immediate ALU forms (05/0D/15/1D/25/2D/35/3D).
	for _, op := range []byte{OP_ADD_AX_IMM16, OP_OR_AX_IMM16, OP_ADC_AX_IMM16, OP_SBB_AX_IMM16, OP_AND_AX_IMM16, OP_SUB_AX_IMM16, OP_XOR_AX_IMM16, OP_CMP_AX_IMM16} {
		set(op, hAXImm16ALU(op))
	}

	// AL immediate ALU forms (04/0C/14/1C/24/2C/34/3C).
	for _, op := range []byte{0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C} {
		set(op, hALImm8ALU(op))
	}

	// MOV AX/AL, moffs16 and reverse.
	set(OP_MOV_AX_MOFFS16, hMovAXMoffs())
	set(OP_MOV_MOFFS16_AX, hMovMoffsAX())
	set(0xA0, hMovALMoffs())
	set(0xA2, hMovMoffsAL())

	// TEST AL/AX immediate.
	set(0xA8, hTestALImm8())
	set(0xA9, hTestAXImm16())

	// RET/RETF.
	set(OP_RET_NEAR, hRet(false, false))
	set(OP_RETF_FAR, hRet(true, false))
	set(OP_RET_NEAR_IMM16, hRet(false, true))
	set(OP_RETF_FAR_IMM16, hRet(true, true))

	// JMP rel8/rel16.
	set(OP_JMP_REL8, hJmpRel8())
	set(OP_JMP_REL16, hJmpRel16())

	// CALL rel16 (near).
	set(OP_CALL_REL16, hCallRel16())

	// CALLF/JMPF ptr16:16 immediate.
	set(OP_CALLF_PTR16_16, hCallFPtr1616())
	set(OP_JMPF_PTR16_16, hJmpFPtr1616())

	// PUSH immediate.
	set(0x68, hPushImm16())
	set(0x6A, hPushImm8sx())

	// INT imm8.
	set(0xCD, hIntImm8())

	// ENTER imm16, imm8.
	set(0xC8, hEnter())

	// LOOP/JCXZ rel8.
	for _, op := range []byte{0xE0, 0xE1, 0xE2, 0xE3} {
		set(op, hLoopRel8(op))
	}

	// IN/OUT.
	for _, op := range []byte{0xE4, 0xE5, 0xE6, 0xE7, 0xEC, 0xED, 0xEE, 0xEF} {
		set(op, hInOut(op))
	}

	// XCHG AX, r16 (0x91..0x97).
	for op := byte(0x91); op <= 0x97; op++ {
		set(op, hXchgAxR16(op))
	}

	// ModRM16: MOV/LEA/LDS/LES.
	set(OP_MOV_R16_RM16, hModRM16RegRm("MOV", OpMOV, true))
	set(OP_MOV_RM16_R16, hModRM16RegRm("MOV", OpMOV, false))
	set(OP_MOV_SREG_RM16, hMovSregRm16())
	set(OP_MOV_RM16_SREG, hMovRm16Sreg())
	set(OP_LEA_R16_M16, hLeaR16M16())
	set(OP_LES_R16_M16, hLesR16M1616())
	set(OP_LDS_R16_M16, hLdsR16M1616())

	// ModRM16 misc.
	set(0xC7, hMovRm16Imm16())
	set(0x8F, hPopRm16())
	set(0x85, hModRM16RegRm("TEST", OpUnknown, false))
	set(0x87, hModRM16RegRm("XCHG", OpUnknown, false))
	set(0x81, hGrp1Imm16())
	set(0x83, hGrp1Imm8sx())
	set(OP_GRP5_RM16, hGrp5())
	set(OP_GRP2_1_RM16, hGrp2_1())
	set(OP_GRP2_CL_RM16, hGrp2_cl())
	set(OP_GRP2_IMM8_RM16, hGrp2_imm8())
	set(OP_GRP3_RM16, hGrp3Word())
	set(0x69, hImulR16Rm16Imm16())
	set(0x6B, hImulR16Rm16Imm8())

	// ModRM16 ALU r/m16,r16 and r16,r/m16.
	aluRm16R16 := []byte{OP_ADD_RM16_R16, OP_OR_RM16_R16, OP_ADC_RM16_R16, OP_SBB_RM16_R16, OP_AND_RM16_R16, OP_SUB_RM16_R16, OP_XOR_RM16_R16, OP_CMP_RM16_R16}
	aluR16Rm16 := []byte{OP_ADD_R16_RM16, OP_OR_R16_RM16, OP_ADC_R16_RM16, OP_SBB_R16_RM16, OP_AND_R16_RM16, OP_SUB_R16_RM16, OP_XOR_R16_RM16, OP_CMP_R16_RM16}
	for i := 0; i < 8; i++ {
		set(aluRm16R16[i], hALURm16R16(i))
		set(aluR16Rm16[i], hALUR16Rm16(i))
	}

	// ModRM8: MOV.
	set(OP_MOV_R8_RM8, hModRM8RegRm("MOV", OpMOV, true))
	set(OP_MOV_RM8_R8, hModRM8RegRm("MOV", OpMOV, false))
	set(OP_MOV_RM8_IMM8, hMovRm8Imm8())
	set(0x84, hModRM8RegRm("TEST", OpUnknown, false))
	set(0x86, hModRM8RegRm("XCHG", OpUnknown, false))
	set(OP_GRP1_IMM8_RM8, hGrp1Imm8())
	set(0xD0, hGrp2b_1())
	set(0xD2, hGrp2b_cl())
	set(0xC0, hGrp2b_imm8())
	set(0xFE, hIncDecRm8())
	set(0xF6, hGrp3Byte())

	// ModRM8 ALU r/m8,r8 and r8,r/m8 (includes XOR 0x30 and 0x32).
	aluRm8R8 := []byte{0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38}
	aluR8Rm8 := []byte{0x02, 0x0A, 0x12, 0x1A, 0x22, 0x2A, 0x32, 0x3A}
	for i := 0; i < 8; i++ {
		set(aluRm8R8[i], hALURm8R8(i))
		set(aluR8Rm8[i], hALUR8Rm8(i))
	}

	// x87 escapes.
	set(OP_X87_D8, hX87D8())
	set(OP_X87_D9, hX87D9())
	set(OP_X87_DA, hX87DA())
	set(OP_X87_DB, hX87DB())
	set(OP_X87_DC, hX87DC())
	set(OP_X87_DD, hX87DD())
	set(OP_X87_DE, hX87DE())
}

func decodeRanges(d *dec, _ prefixes, op byte) (DecodedInst, bool) {
	// Ranges first.
	switch {
	case op >= OP_PUSH_R16_MIN && op <= OP_PUSH_R16_MAX:
		ri := int(op - OP_PUSH_R16_MIN)
		return mk0(d, "PUSH", OpPUSH,
			Operand{}, // implicit stack write
			Operand{Kind: OKReg, Reg: regIndexToReg(ri)},
		), true

	case op >= OP_POP_R16_MIN && op <= OP_POP_R16_MAX:
		ri := int(op - OP_POP_R16_MIN)
		return mk0(d, "POP", OpPOP,
			Operand{Kind: OKReg, Reg: regIndexToReg(ri)},
			Operand{}, // implicit stack read
		), true

	case op >= OP_MOV_R16_IMM16_MIN && op <= OP_MOV_R16_IMM16_MAX:
		imm, ok := d.u16()
		if !ok {
			return db1(d.b), true
		}
		ri := int(op - OP_MOV_R16_IMM16_MIN)
		return mk0(d, "MOV", OpMOV,
			Operand{Kind: OKReg, Reg: regIndexToReg(ri)},
			Operand{Kind: OKImm, Imm: imm},
		), true

	case op >= 0x40 && op <= 0x47: // INC r16
		ri := int(op - 0x40)
		return mk0(d, "INC", OpADD,
			Operand{Kind: OKReg, Reg: regIndexToReg(ri)},
			Operand{Kind: OKImm, Imm: 1},
		), true

	case op >= 0x48 && op <= 0x4F: // DEC r16
		ri := int(op - 0x48)
		return mk0(d, "DEC", OpSUB,
			Operand{Kind: OKReg, Reg: regIndexToReg(ri)},
			Operand{Kind: OKImm, Imm: 1},
		), true

	case op >= OP_MOV_R8_IMM8_MIN && op <= OP_MOV_R8_IMM8_MAX:
		u, ok := d.u8()
		if !ok {
			return db1(d.b), true
		}
		ri := int(op - OP_MOV_R8_IMM8_MIN)
		return mk0(d, "MOV", OpMOV,
			Operand{Kind: OKReg, Reg: reg8IndexToReg(ri)},
			Operand{Kind: OKImm, Imm: uint16(u)},
		), true

	case op >= OP_JCC_REL8_MIN && op <= OP_JCC_REL8_MAX:
		u, ok := d.u8()
		if !ok {
			return db1(d.b), true
		}
		idx := int(op - OP_JCC_REL8_MIN)
		if idx < 0 || idx >= len(jccNames) {
			return db1(d.b), true
		}
		disp := int16(int8(u))
		target := relTarget(d.ip, d.i, int32(disp))
		return mk(d, jccNames[idx], OpJcc, Operand{}, Operand{Kind: OKRel, rel: disp}, false, target), true
	}
	return DecodedInst{}, false
}

// ---- tiny handler constructors ----

func h0(mnem string, op Op) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		return mk0(d, mnem, op, Operand{}, Operand{})
	}
}

// ---- string op handlers (implicit operands) ----

// String instructions have implicit operands (SI/DI/CX/AL/AX and implied segments).
// We model these for annotation and for the register-tracking simulator.
func hStringMovs(word bool) opHandler {
	return func(d *dec, pfx prefixes, _ byte) DecodedInst {
		mnem := "MOVSB"
		if word {
			mnem = "MOVSW"
		}

		elemSize := 1
		if word {
			elemSize = 2
		}

		// Destination is always ES:[DI] (cannot be overridden).
		dst := Operand{Kind: OKMem, Mem: MemRef{Base: RegDI, SegOverride: RegES, MemSize: elemSize}}

		// Source defaults to DS:[SI], but segment override changes the source segment.
		srcSeg := RegDS
		if pfx.seg != RegNone {
			srcSeg = pfx.seg
		}
		src := Operand{Kind: OKMem, Mem: MemRef{Base: RegSI, SegOverride: srcSeg, MemSize: elemSize}}

		op := Op(OpMOVSB)
		if word {
			op = OpMOVSW
		}
		in := mk0(d, mnem, op, dst, src)

		return in
	}
}

func hStringStos(word bool) opHandler {
	return func(d *dec, pfx prefixes, _ byte) DecodedInst {
		mnem := "STOSB"
		srcReg := RegAL
		if word {
			mnem = "STOSW"
			srcReg = RegAX
		}

		elemSize := 1
		if word {
			elemSize = 2
		}

		dst := Operand{Kind: OKMem, Mem: MemRef{Base: RegDI, SegOverride: RegES, MemSize: elemSize}}
		src := Operand{Kind: OKReg, Reg: srcReg}

		in := mk0(d, mnem, OpUnknown, dst, src)

		return in
	}
}

func hStringLods(word bool) opHandler {
	return func(d *dec, pfx prefixes, _ byte) DecodedInst {
		mnem := "LODSB"
		dstReg := RegAL
		if word {
			mnem = "LODSW"
			dstReg = RegAX
		}

		dst := Operand{Kind: OKReg, Reg: dstReg}

		elemSize := 1
		if word {
			elemSize = 2
		}
		srcSeg := RegDS
		if pfx.seg != RegNone {
			srcSeg = pfx.seg
		}
		src := Operand{Kind: OKMem, Mem: MemRef{Base: RegSI, SegOverride: srcSeg, MemSize: elemSize}}

		in := mk0(d, mnem, OpUnknown, dst, src)

		return in
	}
}

func hStringScas(word bool) opHandler {
	return func(d *dec, pfx prefixes, _ byte) DecodedInst {
		mnem := "SCASB"
		acc := RegAL
		if word {
			mnem = "SCASW"
			acc = RegAX
		}

		elemSize := 1
		if word {
			elemSize = 2
		}

		dst := Operand{Kind: OKMem, Mem: MemRef{Base: RegDI, SegOverride: RegES, MemSize: elemSize}}
		src := Operand{Kind: OKReg, Reg: acc}

		in := mk0(d, mnem, OpUnknown, dst, src)

		return in
	}
}

func hPushSeg(seg Reg) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		return mk0(d, "PUSH", OpPUSH, Operand{}, Operand{Kind: OKSReg, Reg: seg})
	}
}

func hPopSeg(seg Reg) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		return mk0(d, "POP", OpPOP, Operand{Kind: OKSReg, Reg: seg}, Operand{})
	}
}

func moffs16(addr uint16) Operand {
	return Operand{Kind: OKMem, Mem: MemRef{Direct: true, Disp: int(addr), dispLen: 2}}
}

func hAXImm16ALU(opb byte) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		imm, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		mnems := []string{"ADD", "OR", "ADC", "SBB", "AND", "SUB", "XOR", "CMP"}
		ops := []Op{OpADD, OpOR, OpADC, OpSBB, OpAND, OpSUB, OpXOR, OpCMP}
		idx := int((opb - OP_ADD_AX_IMM16) / 8)
		return mk0(d, mnems[idx], ops[idx], Operand{Kind: OKReg, Reg: RegAX}, Operand{Kind: OKImm, Imm: imm})
	}
}

func hALImm8ALU(opb byte) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		mnems := []string{"ADD", "OR", "ADC", "SBB", "AND", "SUB", "XOR", "CMP"}
		ops := []Op{OpADD, OpOR, OpADC, OpSBB, OpAND, OpSUB, OpXOR, OpCMP}
		idx := int(opb / 8)
		return mk0(d, mnems[idx], ops[idx], Operand{Kind: OKReg, Reg: reg8IndexToReg(0)}, Operand{Kind: OKImm, Imm: uint16(u)})
	}
}

func hMovAXMoffs() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		addr, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "MOV", OpMOV, Operand{Kind: OKReg, Reg: RegAX}, moffs16(addr))
	}
}

func hMovMoffsAX() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		addr, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "MOV", OpMOV, moffs16(addr), Operand{Kind: OKReg, Reg: RegAX})
	}
}

func hMovALMoffs() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		addr, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "MOV", OpMOV, Operand{Kind: OKReg, Reg: reg8IndexToReg(0)}, moffs16(addr))
	}
}

func hMovMoffsAL() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		addr, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "MOV", OpMOV, moffs16(addr), Operand{Kind: OKReg, Reg: reg8IndexToReg(0)})
	}
}

func hTestALImm8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "TEST", OpUnknown, Operand{Kind: OKReg, Reg: reg8IndexToReg(0)}, Operand{Kind: OKImm, Imm: uint16(u)})
	}
}

func hTestAXImm16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		u, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "TEST", OpUnknown, Operand{Kind: OKReg, Reg: RegAX}, Operand{Kind: OKImm, Imm: u})
	}
}

func hRet(far bool, imm bool) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		if !imm {
			if far {
				return mk(d, "RETF", OpRETF, Operand{}, Operand{}, true, noTarget)
			}
			return mk(d, "RET", OpRET, Operand{}, Operand{}, true, noTarget)
		}
		amt, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		if far {
			return mk(d, "RETF", OpRETF, Operand{}, Operand{Kind: OKImm, Imm: amt}, true, noTarget)
		}
		return mk(d, "RET", OpRET, Operand{}, Operand{Kind: OKImm, Imm: amt}, true, noTarget)
	}
}

func hJmpRel8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		disp := int16(int8(u))
		target := relTarget(d.ip, d.i, int32(disp))
		return mk(d, "JMP", OpJMP, Operand{}, Operand{Kind: OKRel, rel: disp}, false, target)
	}
}

func hJmpRel16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		u, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		disp := int16(u)
		target := relTarget(d.ip, d.i, int32(disp))
		return mk(d, "JMP", OpJMP, Operand{}, Operand{Kind: OKRel, rel: disp}, false, target)
	}
}

func hCallRel16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		u, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		disp := int16(u)
		target := relTarget(d.ip, d.i, int32(disp))
		return mk(d, "CALL", OpUnknown, Operand{}, Operand{Kind: OKRel, rel: disp}, false, target)
	}
}

func hCallFPtr1616() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		off, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		seg, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		// For CALLF, we use Src=seg and Dst=off so operand rendering prints seg:off.
		return mk0(d, "CALLF", OpCALLF, Operand{Kind: OKImm, Imm: off}, Operand{Kind: OKImm, Imm: seg})
	}
}

func hJmpFPtr1616() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		off, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		seg, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "JMPF", OpUnknown, Operand{Kind: OKImm, Imm: off}, Operand{Kind: OKImm, Imm: seg})
	}
}

func hPushImm16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		imm, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "PUSH", OpPUSH, Operand{}, Operand{Kind: OKImm, Imm: imm})
	}
}

func hPushImm8sx() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		imm := u16(int16(int8(u)))
		return mk0(d, "PUSH", OpPUSH, Operand{}, Operand{Kind: OKImm, Imm: imm})
	}
}

func hIntImm8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "INT", OpUnknown, Operand{}, Operand{Kind: OKImm, Imm: uint16(u)})
	}
}

func hEnter() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		size, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		level, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "ENTER", OpUnknown, Operand{Kind: OKImm, Imm: uint16(level)}, Operand{Kind: OKImm, Imm: size})
	}
}

func hLoopRel8(opb byte) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		disp := int16(int8(u))
		target := relTarget(d.ip, d.i, int32(disp))
		names := map[byte]string{0xE0: "LOOPNZ", 0xE1: "LOOPZ", 0xE2: "LOOP", 0xE3: "JCXZ"}
		return mk(d, names[opb], OpUnknown, Operand{}, Operand{Kind: OKRel, rel: disp}, false, target)
	}
}

func hInOut(opb byte) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		readImm8 := func() (uint16, bool) {
			u, ok := d.u8()
			return uint16(u), ok
		}
		switch opb {
		case 0xE4: // IN AL, imm8
			n, ok := readImm8()
			if !ok {
				return db1(d.b)
			}
			return mk0(d, "IN", OpUnknown, Operand{Kind: OKReg, Reg: reg8IndexToReg(0)}, Operand{Kind: OKImm, Imm: n})
		case 0xE5: // IN AX, imm8
			n, ok := readImm8()
			if !ok {
				return db1(d.b)
			}
			return mk0(d, "IN", OpUnknown, Operand{Kind: OKReg, Reg: RegAX}, Operand{Kind: OKImm, Imm: n})
		case 0xE6: // OUT imm8, AL
			n, ok := readImm8()
			if !ok {
				return db1(d.b)
			}
			return mk0(d, "OUT", OpUnknown, Operand{Kind: OKImm, Imm: n}, Operand{Kind: OKReg, Reg: reg8IndexToReg(0)})
		case 0xE7: // OUT imm8, AX
			n, ok := readImm8()
			if !ok {
				return db1(d.b)
			}
			return mk0(d, "OUT", OpUnknown, Operand{Kind: OKImm, Imm: n}, Operand{Kind: OKReg, Reg: RegAX})
		case 0xEC: // IN AL, DX
			return mk0(d, "IN", OpUnknown, Operand{Kind: OKReg, Reg: reg8IndexToReg(0)}, Operand{Kind: OKReg, Reg: RegDX})
		case 0xED: // IN AX, DX
			return mk0(d, "IN", OpUnknown, Operand{Kind: OKReg, Reg: RegAX}, Operand{Kind: OKReg, Reg: RegDX})
		case 0xEE: // OUT DX, AL
			return mk0(d, "OUT", OpUnknown, Operand{Kind: OKReg, Reg: RegDX}, Operand{Kind: OKReg, Reg: reg8IndexToReg(0)})
		case 0xEF: // OUT DX, AX
			return mk0(d, "OUT", OpUnknown, Operand{Kind: OKReg, Reg: RegDX}, Operand{Kind: OKReg, Reg: RegAX})
		default:
			return db1(d.b)
		}
	}
}

func hXchgAxR16(opb byte) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		ri := int(opb - 0x90)
		return mk0(d, "XCHG", OpUnknown, Operand{Kind: OKReg, Reg: RegAX}, Operand{Kind: OKReg, Reg: regIndexToReg(ri)})
	}
}

func hModRM16RegRm(mnem string, op Op, dstIsReg bool) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok {
			return db1(d.b)
		}
		regOp := Operand{Kind: OKReg, Reg: regIndexToReg(regField)}
		if dstIsReg {
			return mk0(d, mnem, op, regOp, rmOp)
		}
		return mk0(d, mnem, op, rmOp, regOp)
	}
}

func hMovSregRm16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "MOV", OpMOV, Operand{Kind: OKSReg, Reg: sregIndexToReg(regField)}, rmOp)
	}
}

func hMovRm16Sreg() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "MOV", OpMOV, rmOp, Operand{Kind: OKSReg, Reg: sregIndexToReg(regField)})
	}
}

func hLeaR16M16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		return mk0(d, "LEA", OpLEA, Operand{Kind: OKReg, Reg: regIndexToReg(regField)}, rmOp)
	}
}

func hLesR16M1616() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		in := mk0(d, "LES", OpLES, Operand{Kind: OKReg, Reg: regIndexToReg(regField)}, rmOp)
		return in
	}
}

func hLdsR16M1616() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		in := mk0(d, "LDS", OpLDS, Operand{Kind: OKReg, Reg: regIndexToReg(regField)}, rmOp)
		return in
	}
}

func hMovRm16Imm16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || regField != 0 {
			return db1(d.b)
		}
		if rmOp.Kind == OKMem {
			rmOp.Mem.MemSize = 2
		}
		imm, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "MOV", OpMOV, rmOp, Operand{Kind: OKImm, Imm: imm})
	}
}

func hPopRm16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || regField != 0 {
			return db1(d.b)
		}
		return mk0(d, "POP", OpPOP, rmOp, Operand{})
	}
}

func hGrp1Imm16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || regField >= len(grp1Names) || grp1Names[regField] == "" {
			return db1(d.b)
		}
		imm, ok := d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, grp1Names[regField], grp1OpFor(regField), rmOp, Operand{Kind: OKImm, Imm: imm})
	}
}

func hGrp1Imm8sx() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || regField >= len(grp1Names) || grp1Names[regField] == "" {
			return db1(d.b)
		}
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		imm := u16(int16(int8(u)))
		return mk0(d, grp1Names[regField], grp1OpFor(regField), rmOp, Operand{Kind: OKImm, Imm: imm})
	}
}

func hALURm16R16(opIdx int) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok {
			return db1(d.b)
		}
		return mk0(d, grp1Names[opIdx], grp1OpFor(opIdx), rmOp, Operand{Kind: OKReg, Reg: regIndexToReg(regField)})
	}
}

func hALUR16Rm16(opIdx int) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok {
			return db1(d.b)
		}
		return mk0(d, grp1Names[opIdx], grp1OpFor(opIdx), Operand{Kind: OKReg, Reg: regIndexToReg(regField)}, rmOp)
	}
}

func hGrp5() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok {
			return db1(d.b)
		}
		switch regField {
		case 0:
			return mk0(d, "INC", OpUnknown, rmOp, Operand{})
		case 1:
			return mk0(d, "DEC", OpUnknown, rmOp, Operand{})
		case 2:
			return mk0(d, "CALL", OpUnknown, Operand{}, rmOp)
		case 3:
			return mk0(d, "CALLF", OpCALLF, Operand{}, rmOp)
		case 4:
			return mk0(d, "JMP", OpJMP, Operand{}, rmOp)
		case 5:
			return mk0(d, "JMPF", OpUnknown, Operand{}, rmOp)
		case 6:
			return mk0(d, "PUSH", OpPUSH, Operand{}, rmOp)
		default:
			return db1(d.b)
		}
	}
}

func hGrp2_1() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || regField >= len(grp2Names) || grp2Names[regField] == "" {
			return db1(d.b)
		}
		return mk0(d, grp2Names[regField], grp2OpFor(regField), rmOp, Operand{Kind: OKImm, Imm: 1})
	}
}

func hGrp2_cl() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || regField >= len(grp2Names) || grp2Names[regField] == "" {
			return db1(d.b)
		}
		return mk0(d, grp2Names[regField], grp2OpFor(regField), rmOp, Operand{Kind: OKReg, Reg: RegCX})
	}
}

func hGrp2_imm8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok || regField >= len(grp2Names) || grp2Names[regField] == "" {
			return db1(d.b)
		}
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, grp2Names[regField], grp2OpFor(regField), rmOp, Operand{Kind: OKImm, Imm: uint16(u)})
	}
}

func hGrp3Word() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok {
			return db1(d.b)
		}
		if regField == 0 {
			imm, ok := d.u16()
			if !ok {
				return db1(d.b)
			}
			return mk0(d, "TEST", OpUnknown, rmOp, Operand{Kind: OKImm, Imm: imm})
		}
		switch regField {
		case 2:
			return mk0(d, "NOT", OpNOT, rmOp, Operand{})
		case 3:
			return mk0(d, "NEG", OpNEG, rmOp, Operand{})
		case 4:
			in := mk0(d, "MUL", OpUnknown, Operand{}, rmOp)
			return in
		case 5:
			in := mk0(d, "IMUL", OpIMULrm, Operand{}, rmOp)
			return in
		case 6:
			in := mk0(d, "DIV", OpDIV, Operand{}, rmOp)
			return in
		case 7:
			in := mk0(d, "IDIV", OpIDIV, Operand{}, rmOp)
			return in
		default:
			return db1(d.b)
		}
	}
}

func hImulR16Rm16Imm16() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok {
			return db1(d.b)
		}
		_, ok = d.u16()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "IMUL", OpUnknown, Operand{Kind: OKReg, Reg: regIndexToReg(regField)}, rmOp)
	}
}

func hImulR16Rm16Imm8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM16(d)
		if !ok {
			return db1(d.b)
		}
		_, ok = d.u8()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "IMUL", OpUnknown, Operand{Kind: OKReg, Reg: regIndexToReg(regField)}, rmOp)
	}
}

func hModRM8RegRm(mnem string, op Op, dstIsReg bool) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok {
			return db1(d.b)
		}
		regOp := Operand{Kind: OKReg, Reg: reg8IndexToReg(regField)}
		if dstIsReg {
			return mk0(d, mnem, op, regOp, rmOp)
		}
		return mk0(d, mnem, op, rmOp, regOp)
	}
}

func hMovRm8Imm8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok || regField != 0 {
			return db1(d.b)
		}
		if rmOp.Kind == OKMem {
			rmOp.Mem.MemSize = 1
		}
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, "MOV", OpMOV, rmOp, Operand{Kind: OKImm, Imm: uint16(u)})
	}
}

func hGrp1Imm8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok || regField >= len(grp1Names) || grp1Names[regField] == "" {
			return db1(d.b)
		}
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, grp1Names[regField], grp1OpFor(regField), rmOp, Operand{Kind: OKImm, Imm: uint16(u)})
	}
}

func hGrp2b_1() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok || regField >= len(grp2Names) || grp2Names[regField] == "" {
			return db1(d.b)
		}
		return mk0(d, grp2Names[regField], grp2OpFor(regField), rmOp, Operand{Kind: OKImm, Imm: 1})
	}
}

func hGrp2b_cl() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok || regField >= len(grp2Names) || grp2Names[regField] == "" {
			return db1(d.b)
		}
		return mk0(d, grp2Names[regField], grp2OpFor(regField), rmOp, Operand{Kind: OKReg, Reg: RegCL})
	}
}

func hGrp2b_imm8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok || regField >= len(grp2Names) || grp2Names[regField] == "" {
			return db1(d.b)
		}
		u, ok := d.u8()
		if !ok {
			return db1(d.b)
		}
		return mk0(d, grp2Names[regField], grp2OpFor(regField), rmOp, Operand{Kind: OKImm, Imm: uint16(u)})
	}
}

func hIncDecRm8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok {
			return db1(d.b)
		}
		switch regField {
		case 0:
			return mk0(d, "INC", OpUnknown, rmOp, Operand{})
		case 1:
			return mk0(d, "DEC", OpUnknown, rmOp, Operand{})
		default:
			return db1(d.b)
		}
	}
}

func hGrp3Byte() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok {
			return db1(d.b)
		}
		if regField == 0 {
			u, ok := d.u8()
			if !ok {
				return db1(d.b)
			}
			return mk0(d, "TEST", OpUnknown, rmOp, Operand{Kind: OKImm, Imm: uint16(u)})
		}
		switch regField {
		case 2:
			return mk0(d, "NOT", OpNOT, rmOp, Operand{})
		case 3:
			return mk0(d, "NEG", OpNEG, rmOp, Operand{})
		case 4:
			in := mk0(d, "MUL", OpUnknown, Operand{}, rmOp)
			return in
		case 5:
			return mk0(d, "IMUL", OpUnknown, rmOp, Operand{})
		case 6:
			in := mk0(d, "DIV", OpDIV, Operand{}, rmOp)
			return in
		case 7:
			in := mk0(d, "IDIV", OpIDIV, Operand{}, rmOp)
			return in
		default:
			return db1(d.b)
		}
	}
}

func hALURm8R8(opIdx int) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok {
			return db1(d.b)
		}
		return mk0(d, grp1Names[opIdx], grp1OpFor(opIdx), rmOp, Operand{Kind: OKReg, Reg: reg8IndexToReg(regField)})
	}
}

func hALUR8Rm8(opIdx int) opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		regField, rmOp, ok := parseModRM8(d)
		if !ok {
			return db1(d.b)
		}
		return mk0(d, grp1Names[opIdx], grp1OpFor(opIdx), Operand{Kind: OKReg, Reg: reg8IndexToReg(regField)}, rmOp)
	}
}

// ---- x87 handlers (lifted from the original big switch) ----

func hX87D8() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		modrm, ok := d.peek()
		if !ok {
			return db1(d.b)
		}
		if modrm >= 0xC0 {
			_, _ = d.u8()
			return db1(d.b)
		}
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		rmOp.Mem.MemSize = 4
		d8Ops := []struct {
			name string
			op   Op
		}{
			{"FADD", OpFADD},
			{"FMUL", OpFMUL},
			{"FCOM", OpFCOM},
			{"FCOMP", OpFCOMP},
			{"FSUB", OpFSUB},
			{"FSUBR", OpFSUBR},
			{"FDIV", OpFDIV},
			{"FDIVR", OpFDIVR},
		}
		x87Op := d8Ops[regField]
		return mk0(d, x87Op.name, x87Op.op, Operand{}, rmOp)
	}
}

func hX87D9() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		modrm, ok := d.peek()
		if !ok {
			return db1(d.b)
		}
		if modrm >= 0xC0 {
			modrm, _ = d.u8()
			switch {
			case modrm >= 0xC0 && modrm <= 0xC7:
				i := int(modrm - 0xC0)
				return mk0(d, "FLD", OpFLD, Operand{}, opST(i))
			case modrm >= 0xC8 && modrm <= 0xCF:
				i := int(modrm - 0xC8)
				return mk0(d, "FXCH", OpFXCH, Operand{}, opST(i))
			case modrm == 0xE0:
				return mk0(d, "FCHS", OpUnknown, Operand{}, Operand{})
			case modrm == 0xE1:
				return mk0(d, "FABS", OpUnknown, Operand{}, Operand{})
			case modrm == 0xEE:
				return mk0(d, "FLDZ", OpFLDZ, Operand{}, Operand{})
			case modrm == 0xE8:
				return mk0(d, "FLD1", OpFLD1, Operand{}, Operand{})
			default:
				return db1(d.b)
			}
		}
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		rmOp.Mem.MemSize = 4
		switch regField {
		case 0:
			return mk0(d, "FLD", OpFLD, Operand{}, rmOp)
		case 2:
			return mk0(d, "FST", OpFST, rmOp, Operand{})
		case 3:
			return mk0(d, "FSTP", OpFSTP, rmOp, Operand{})
		default:
			return db1(d.b)
		}
	}
}

func hX87DD() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		modrm, ok := d.peek()
		if !ok {
			return db1(d.b)
		}
		if modrm >= 0xC0 {
			_, _ = d.u8()
			return db1(d.b)
		}
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		rmOp.Mem.MemSize = 8
		switch regField {
		case 0:
			return mk0(d, "FLD", OpFLD, Operand{}, rmOp)
		case 2:
			return mk0(d, "FST", OpFST, rmOp, Operand{})
		case 3:
			return mk0(d, "FSTP", OpFSTP, rmOp, Operand{})
		default:
			return db1(d.b)
		}
	}
}

func hX87DE() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		modrm, ok := d.peek()
		if !ok {
			return db1(d.b)
		}
		if modrm >= 0xC0 {
			modrm, _ = d.u8()
			switch modrm & 0xF8 {
			case 0xC0:
				i := int(modrm & 0x07)
				return mk0(d, "FADDP", OpFADDP, opST(i), opST(-1))
			case 0xC8:
				i := int(modrm & 0x07)
				return mk0(d, "FMULP", OpFMULP, opST(i), opST(-1))
			case 0xE0:
				i := int(modrm & 0x07)
				return mk0(d, "FSUBRP", OpFSUBRP, opST(i), opST(-1))
			case 0xE8:
				i := int(modrm & 0x07)
				return mk0(d, "FSUBP", OpFSUBP, opST(i), opST(-1))
			case 0xF0:
				i := int(modrm & 0x07)
				return mk0(d, "FDIVRP", OpFDIVRP, opST(i), opST(-1))
			case 0xF8:
				i := int(modrm & 0x07)
				return mk0(d, "FDIVP", OpFDIVP, opST(i), opST(-1))
			default:
				return db1(d.b)
			}
		}
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		rmOp.Mem.MemSize = 2 // m16int
		switch regField {
		case 0:
			return mk0(d, "FIADD", OpUnknown, Operand{}, rmOp)
		case 1:
			return mk0(d, "FIMUL", OpUnknown, Operand{}, rmOp)
		case 2:
			return mk0(d, "FICOM", OpUnknown, Operand{}, rmOp)
		case 3:
			return mk0(d, "FICOMP", OpUnknown, Operand{}, rmOp)
		case 4:
			return mk0(d, "FISUB", OpUnknown, Operand{}, rmOp)
		case 5:
			return mk0(d, "FISUBR", OpUnknown, Operand{}, rmOp)
		case 6:
			return mk0(d, "FIDIV", OpUnknown, Operand{}, rmOp)
		case 7:
			return mk0(d, "FIDIVR", OpUnknown, Operand{}, rmOp)
		default:
			return db1(d.b)
		}
	}
}

func hX87DA() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		modrm, ok := d.peek()
		if !ok {
			return db1(d.b)
		}
		if modrm >= 0xC0 {
			_, _ = d.u8()
			return db1(d.b)
		}
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		rmOp.Mem.MemSize = 4 // m32int
		daNames := []string{"FIADD", "FIMUL", "FICOM", "FICOMP", "FISUB", "FISUBR", "FIDIV", "FIDIVR"}
		return mk0(d, daNames[regField], OpUnknown, Operand{}, rmOp)
	}
}

func hX87DB() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		modrm, ok := d.peek()
		if !ok {
			return db1(d.b)
		}
		if modrm >= 0xC0 {
			_, _ = d.u8()
			return db1(d.b)
		}
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		switch regField {
		case 0:
			rmOp.Mem.MemSize = 4 // m32int (FILD)
			return mk0(d, "FILD", OpFILD, Operand{}, rmOp)
		case 2:
			rmOp.Mem.MemSize = 4 // m32int (FIST)
			return mk0(d, "FIST", OpUnknown, rmOp, Operand{})
		case 3:
			rmOp.Mem.MemSize = 4 // m32int (FISTP)
			return mk0(d, "FISTP", OpUnknown, rmOp, Operand{})
		case 5:
			rmOp.Mem.MemSize = 10
			return mk0(d, "FLD", OpFLD, Operand{}, rmOp)
		case 7:
			rmOp.Mem.MemSize = 10
			return mk0(d, "FSTP", OpFSTP, rmOp, Operand{})
		default:
			return db1(d.b)
		}
	}
}

func hX87DC() opHandler {
	return func(d *dec, _ prefixes, _ byte) DecodedInst {
		modrm, ok := d.peek()
		if !ok {
			return db1(d.b)
		}
		if modrm >= 0xC0 {
			_, _ = d.u8()
			return db1(d.b)
		}
		regField, rmOp, ok := parseModRM16(d)
		if !ok || rmOp.Kind != OKMem {
			return db1(d.b)
		}
		rmOp.Mem.MemSize = 8
		dcOps := []struct {
			name string
			op   Op
		}{
			{"FADD", OpFADD},
			{"FMUL", OpFMUL},
			{"FCOM", OpFCOM},
			{"FCOMP", OpFCOMP},
			{"FSUB", OpFSUB},
			{"FSUBR", OpFSUBR},
			{"FDIV", OpFDIV},
			{"FDIVR", OpFDIVR},
		}
		x87Op := dcOps[regField]
		return mk0(d, x87Op.name, x87Op.op, Operand{}, rmOp)
	}
}

// relTarget computes the absolute IP-relative target for a near branch/call.
//
// ip is the absolute offset of the *start* of the current instruction.
// instLen is the decoded instruction length in bytes (including any prefixes).
// disp is the signed displacement from the end of the instruction.
//
// The result is: ip + instLen + disp.
func relTarget(ip uint32, instLen int, disp int32) int32 {
	return int32(ip) + int32(instLen) + disp
}

func applySegOverrideToOp(op *Operand, seg Reg) {
	if seg == RegNone || op == nil {
		return
	}
	if op.Kind != OKMem {
		return
	}
	m := op.Mem
	// If you want “first prefix wins”, only set if not already set:
	if m.SegOverride == RegNone {
		m.SegOverride = seg
		op.Mem = m
	}
}

type prefixes struct {
	seg  Reg   // RegNone if none, else RegES/CS/SS/DS
	rep  uint8 // 0 none, 0xF2 repne, 0xF3 rep
	lock bool
}

func (d *dec) parsePrefixes() prefixes {
	p := prefixes{seg: RegNone} // <-- critical
	for {
		x, ok := d.peek()
		if !ok {
			return p
		}
		switch x {
		case 0x26:
			d.i++
			p.seg = RegES
		case 0x2E:
			d.i++
			p.seg = RegCS
		case 0x36:
			d.i++
			p.seg = RegSS
		case 0x3E:
			d.i++
			p.seg = RegDS
		case 0xF0:
			d.i++
			p.lock = true
		case 0xF2:
			d.i++
			p.rep = 0xF2
		case 0xF3:
			d.i++
			p.rep = 0xF3
		default:
			return p
		}
	}
}

func applyPrefixesToInst(in *DecodedInst, p prefixes) {
	// Segment override applies to memory operands.
	applySegOverrideToOp(&in.Dst, p.seg)
	applySegOverrideToOp(&in.Src, p.seg)

	// LOCK / REP* are render-time decorations (don’t bake into Mnemonic).
	in.hasLOCK = p.lock

	in.hasREP = (p.rep == 0xF3)
	in.hasREPNE = (p.rep == 0xF2)
}

// parseModRM16 parses a 16-bit ModR/M operand stream from d at the current cursor.
// It consumes the ModR/M byte and any displacement bytes.
// Returns the reg field (0..7) and the decoded r/m operand (OKReg or OKMem).
//
// Addressing (16-bit rm encodings):
// 000 [bx+si]  001 [bx+di]  010 [bp+si]  011 [bp+di]
// 100 [si]     101 [di]     110 [bp]*    111 [bx]
// * special case: mod=00 rm=110 => [disp16] direct (MemRef.Direct=true)
func parseModRM16(d *dec) (regField int, rmOp Operand, ok bool) {
	m, ok := d.u8()
	if !ok {
		return 0, Operand{}, false
	}

	mod := (m >> 6) & 0x3
	regField = int((m >> 3) & 0x7)
	rm := int(m & 0x7)

	// mod=3 => register-direct (r/m is reg16)
	if mod == 3 {
		return regField, Operand{Kind: OKReg, Reg: reg16[rm]}, true
	}

	type pair struct{ base, index Reg }
	var pairs = [8]pair{
		{RegBX, RegSI},
		{RegBX, RegDI},
		{RegBP, RegSI},
		{RegBP, RegDI},
		{RegSI, RegNone},
		{RegDI, RegNone},
		{RegBP, RegNone},
		{RegBX, RegNone},
	}

	mem := MemRef{
		Base:   pairs[rm].base,
		index:  pairs[rm].index,
		Disp:   0,
		Direct: false,
	}

	// Special case: mod=00 rm=110 => [disp16] direct address
	if mod == 0 && rm == 6 {
		disp16, ok := d.u16()
		if !ok {
			return 0, Operand{}, false
		}
		mem.Base, mem.index = RegNone, RegNone
		mem.Disp = int(disp16)
		mem.dispLen = 2
		mem.Direct = true
		return regField, Operand{Kind: OKMem, Mem: mem}, true
	}

	// Displacement by mod
	switch mod {
	case 0:
		// no displacement
		mem.dispLen = 0

	case 1:
		// disp8 (signed)
		disp8, ok := d.u8()
		if !ok {
			return 0, Operand{}, false
		}
		mem.Disp = int(int8(disp8))
		mem.dispLen = 1

	case 2:
		// disp16 (signed)
		disp16, ok := d.u16()
		if !ok {
			return 0, Operand{}, false
		}
		mem.Disp = int(int16(disp16))
		mem.dispLen = 2

	default:
		return 0, Operand{}, false
	}

	return regField, Operand{Kind: OKMem, Mem: mem}, true
}

// parseModRM8 parses ModRM for byte ops.
// Returns regField and rm operand (either OKReg with RegAL..RegBH, or OKMem via parseModRM16).
func parseModRM8(d *dec) (regField int, rmOp Operand, ok bool) {
	m, ok := d.peek()
	if !ok {
		return 0, Operand{}, false
	}
	mod := (m >> 6) & 0x3
	regField = int((m >> 3) & 0x7)
	rmIdx := int(m & 0x7)

	if mod == 3 {
		_, _ = d.u8() // consume modrm
		return regField, Operand{Kind: OKReg, Reg: reg8IndexToReg(rmIdx)}, true
	}

	// Memory form: delegate to 16-bit ModRM parser.
	rf, rm16, ok := parseModRM16(d)
	if !ok || rm16.Kind != OKMem {
		return 0, Operand{}, false
	}
	rm16.Mem.MemSize = 1
	return rf, rm16, true
}

// DecodedInst creation helpers
func db1(b []byte) DecodedInst {
	return DecodedInst{Bytes: b[:1], Mnemonic: "DB", Len: 1, Target: noTarget}
}

const noTarget = int32(-1)

func mk0(d *dec, mnem string, op Op, dst, src Operand) DecodedInst {
	return mk(d, mnem, op, dst, src, false, noTarget)
}

func mk(d *dec, mnem string, op Op, dst, src Operand, stop bool, target int32) DecodedInst {
	return DecodedInst{
		Bytes:    d.bytes(),
		Mnemonic: mnem,
		Op:       op,
		Dst:      dst,
		Src:      src,
		Len:      d.i,
		stop:     stop,
		Target:   target,
	}
}

func opST(i int) Operand {
	if i < 0 {
		i = 0
	}
	return Operand{Kind: OKX87Reg, X87Index: int8(i)}
}

// grp2OpFor returns the Op constant for a group-2 shift/rotate reg field.
func grp2OpFor(regField int) Op {
	switch regField {
	case 4: // SHL
		return OpSHL
	case 5: // SHR
		return OpSHR
	case 7: // SAR
		return OpSAR
	}
	return OpUnknown
}

// grp1OpFor returns the Op constant for a group-1 ALU reg field.
func grp1OpFor(regField int) Op {
	switch regField {
	case 0:
		return OpADD
	case 1:
		return OpOR
	case 2:
		return OpADC
	case 3:
		return OpSBB
	case 4:
		return OpAND
	case 5:
		return OpSUB
	case 6:
		return OpXOR
	case 7:
		return OpCMP
	default:
		return OpUnknown
	}
}

func regIndexToReg(i int) Reg {
	switch i {
	case 0:
		return RegAX
	case 1:
		return RegCX
	case 2:
		return RegDX
	case 3:
		return RegBX
	case 4:
		return RegSP
	case 5:
		return RegBP
	case 6:
		return RegSI
	case 7:
		return RegDI
	default:
		return RegAX
	}
}

func reg8IndexToReg(i int) Reg {
	switch i {
	case 0:
		return RegAL
	case 1:
		return RegCL
	case 2:
		return RegDL
	case 3:
		return RegBL
	case 4:
		return RegAH
	case 5:
		return RegCH
	case 6:
		return RegDH
	case 7:
		return RegBH
	default:
		return RegAL
	}
}

func sregIndexToReg(i int) Reg {
	switch i {
	case 0:
		return RegES
	case 1:
		return RegCS
	case 2:
		return RegSS
	case 3:
		return RegDS
	default:
		return RegDS
	}
}
