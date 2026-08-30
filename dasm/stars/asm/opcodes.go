package asm

// Win16 / 8086 opcode byte constants used by the Stars! disassembler.
//
// NOTE: These are raw *opcode bytes* from the instruction stream.
// They are intentionally separate from the higher-level Op classification
// used by the state machine.

const (
	OP_PUSH_R16_MIN byte = 0x50
	OP_PUSH_R16_MAX byte = 0x57
	OP_POP_R16_MIN  byte = 0x58
	OP_POP_R16_MAX  byte = 0x5F

	OP_MOV_R16_IMM16_MIN byte = 0xB8
	OP_MOV_R16_IMM16_MAX byte = 0xBF
	OP_MOV_R8_IMM8_MIN   byte = 0xB0
	OP_MOV_R8_IMM8_MAX   byte = 0xB7

	OP_JCC_REL8_MIN byte = 0x70
	OP_JCC_REL8_MAX byte = 0x7F

	OP_JMP_REL8  byte = 0xEB
	OP_JMP_REL16 byte = 0xE9

	OP_CALL_REL16     byte = 0xE8
	OP_CALLF_PTR16_16 byte = 0x9A
	OP_JMPF_PTR16_16  byte = 0xEA

	OP_RET_NEAR       byte = 0xC3
	OP_RET_NEAR_IMM16 byte = 0xC2
	OP_RETF_FAR       byte = 0xCB
	OP_RETF_FAR_IMM16 byte = 0xCA

	OP_NOP  byte = 0x90
	OP_WAIT byte = 0x9B

	// Segment register push/pop
	OP_PUSH_ES byte = 0x06
	OP_POP_ES  byte = 0x07
	OP_PUSH_SS byte = 0x16
	OP_POP_SS  byte = 0x17
	OP_PUSH_DS byte = 0x1E
	OP_POP_DS  byte = 0x1F

	// Misc
	OP_CBW byte = 0x98
	OP_CWD byte = 0x99

	// ModRM-based core data ops
	OP_MOV_RM16_R16  byte = 0x89
	OP_MOV_R16_RM16  byte = 0x8B
	OP_MOV_RM8_R8    byte = 0x88
	OP_MOV_R8_RM8    byte = 0x8A
	OP_MOV_RM16_SREG byte = 0x8C
	OP_MOV_SREG_RM16 byte = 0x8E
	OP_LEA_R16_M16   byte = 0x8D
	OP_LES_R16_M16   byte = 0xC4
	OP_LDS_R16_M16   byte = 0xC5

	OP_ADD_AX_IMM16 byte = 0x05
	OP_ADD_RM16_R16 byte = 0x01
	OP_ADD_R16_RM16 byte = 0x03

	OP_SUB_AX_IMM16 byte = 0x2D

	OP_CMP_AX_IMM16 byte = 0x3D

	OP_OR_AX_IMM16  byte = 0x0D
	OP_ADC_AX_IMM16 byte = 0x15
	OP_SBB_AX_IMM16 byte = 0x1D
	OP_AND_AX_IMM16 byte = 0x25
	OP_XOR_AX_IMM16 byte = 0x35

	// ModRM-based ALU ops (r/m16,r16 and r16,r/m16)
	OP_OR_RM16_R16  byte = 0x09
	OP_OR_R16_RM16  byte = 0x0B
	OP_ADC_RM16_R16 byte = 0x11
	OP_ADC_R16_RM16 byte = 0x13
	OP_SBB_RM16_R16 byte = 0x19
	OP_SBB_R16_RM16 byte = 0x1B
	OP_AND_RM16_R16 byte = 0x21
	OP_AND_R16_RM16 byte = 0x23
	OP_SUB_RM16_R16 byte = 0x29
	OP_SUB_R16_RM16 byte = 0x2B
	OP_XOR_RM16_R16 byte = 0x31
	OP_XOR_R16_RM16 byte = 0x33
	OP_CMP_RM16_R16 byte = 0x39
	OP_CMP_R16_RM16 byte = 0x3B
	OP_CMP_RM8_R8   byte = 0x38

	// Group 1 immediates
	OP_GRP1_IMM8_RM8    byte = 0x80
	OP_GRP1_IMM16_RM16  byte = 0x81
	OP_GRP1_IMM8SE_RM16 byte = 0x83

	// Group 2 shifts/rotates
	OP_GRP2_1_RM16    byte = 0xD1
	OP_GRP2_CL_RM16   byte = 0xD3
	OP_GRP2_IMM8_RM16 byte = 0xC1

	// Group 3 (test/not/neg/mul/imul/div/idiv)
	OP_GRP3_RM16 byte = 0xF7
	// Group 5 (inc/dec/call/jmp/push)
	OP_GRP5_RM16 byte = 0xFF

	// Absolute memory offset moves
	OP_MOV_AX_MOFFS16 byte = 0xA1 // MOV AX, moffs16
	OP_MOV_MOFFS16_AX byte = 0xA3 // MOV moffs16, AX

	// MOV r/m16, imm16 (C7 /0 iw)
	OP_MOV_RM16_IMM16 byte = 0xC7
	// MOV r/m8, imm8 (C6 /0 ib)
	OP_MOV_RM8_IMM8 byte = 0xC6

	// String moves
	OP_MOVSB byte = 0xA4
	OP_MOVSW byte = 0xA5

	// PUSH immediates
	OP_PUSH_IMM16 byte = 0x68
	OP_PUSH_IMM8  byte = 0x6A

	OP_X87_D8 byte = 0xD8
	OP_X87_D9 byte = 0xD9
	OP_X87_DA byte = 0xDA
	OP_X87_DB byte = 0xDB
	OP_X87_DC byte = 0xDC
	OP_X87_DD byte = 0xDD
	OP_X87_DE byte = 0xDE
)
