package typeinfo

import "fmt"

// Register is a logical Win16 x86 register used by symbol metadata and asm.
type Register uint8

const RegNone Register = 0

const (
	RegAX Register = iota + 1
	RegBX
	RegCX
	RegDX
	RegSP
	RegBP
	RegSI
	RegDI
	RegES
	RegCS
	RegDS
	RegSS
	RegAL
	RegBL
	RegCL
	RegDL
	RegAH
	RegBH
	RegCH
	RegDH
)

// String returns the assembler spelling for the register.
func (r Register) String() string {
	switch r {
	case RegAX:
		return "ax"
	case RegCX:
		return "cx"
	case RegDX:
		return "dx"
	case RegBX:
		return "bx"
	case RegSP:
		return "sp"
	case RegBP:
		return "bp"
	case RegSI:
		return "si"
	case RegDI:
		return "di"
	case RegES:
		return "es"
	case RegCS:
		return "cs"
	case RegSS:
		return "ss"
	case RegDS:
		return "ds"
	case RegAL:
		return "al"
	case RegCL:
		return "cl"
	case RegDL:
		return "dl"
	case RegBL:
		return "bl"
	case RegAH:
		return "ah"
	case RegCH:
		return "ch"
	case RegDH:
		return "dh"
	case RegBH:
		return "bh"
	default:
		return fmt.Sprintf("reg(%d)", r)
	}
}

// IsSeg reports whether the register is a segment register.
func (r Register) IsSeg() bool {
	switch r {
	case RegES, RegCS, RegSS, RegDS:
		return true
	default:
		return false
	}
}

// IsByte reports whether the register is an 8-bit register.
func (r Register) IsByte() bool {
	switch r {
	case RegAL, RegCL, RegDL, RegBL, RegAH, RegCH, RegDH, RegBH:
		return true
	default:
		return false
	}
}

// IsLoByte reports whether the register is the low byte of a word register.
func (r Register) IsLoByte() bool {
	switch r {
	case RegAL, RegCL, RegDL, RegBL:
		return true
	default:
		return false
	}
}

// IsHiByte reports whether the register is the high byte of a word register.
func (r Register) IsHiByte() bool {
	switch r {
	case RegAH, RegCH, RegDH, RegBH:
		return true
	default:
		return false
	}
}

// FullReg returns the full word register for a byte register.
func (r Register) FullReg() Register {
	switch r {
	case RegAL, RegAH:
		return RegAX
	case RegCL, RegCH:
		return RegCX
	case RegDL, RegDH:
		return RegDX
	case RegBL, RegBH:
		return RegBX
	default:
		return r
	}
}
