package machine

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// getRegisterCallArgs gets args for register-based compiler helpers
// such as __aFlshl/__aFlshr. These are carried in DX:AX and CX.
func getRegisterCallArgs(st *state, f *typeinfo.Function) []Value {
	args := make([]Value, 0, len(f.Params))
	if len(f.Params) > 0 {
		args = append(args, typedWordsValue([]Value{st.readReg(asm.RegAX), st.readReg(asm.RegDX)}, f.Params[0].Type))
	}
	if len(f.Params) > 1 {
		args = append(args, st.readReg(asm.RegCX))
	}
	return args
}

// getCallFArgs pops stack words for a CALLF and groups them by the known
// function signature. This intentionally records machine values only; it does
// not resolve arguments into source variables.
func getCallFArgs(st *state, f *typeinfo.Function, words int) []Value {
	fpSlotArgs := st.drainFPStackSlotArgs()
	fpSlotWords := 0
	fpSlotCount := 0
	for _, param := range f.Params {
		if fpSlotCount >= len(fpSlotArgs) {
			break
		}
		if isFloatType(param.Type) {
			fpSlotWords += param.Words()
			fpSlotCount++
		}
	}

	stackWords := words - fpSlotWords
	var rawWords []Value
	if stackWords > 0 {
		rawWords = st.popStackWords(stackWords)
	}

	// Pascal pops off the stack in left to right order
	args := make([]Value, 0, len(f.Params))
	if f.Conv == typeinfo.CCPascal {
		idx := 0
		fpIdx := 0
		for _, param := range f.Params {
			if isFloatType(param.Type) && fpIdx < len(fpSlotArgs) {
				args = append(args, fpSlotArgs[fpIdx])
				fpIdx++
				continue
			}
			n := param.Words()
			if idx+n > len(rawWords) {
				break
			}
			args = append(args, typedStackWordsValue(rawWords[idx:idx+n], param.Type))
			idx += n
		}
		for idx < len(rawWords) {
			args = append(args, rawWords[idx])
			idx++
		}
		return args
	}

	// cdecl/stdcall pop off the stack in right to left order
	idx := len(rawWords)
	fpIdx := 0
	for _, param := range f.Params {
		if isFloatType(param.Type) && fpIdx < len(fpSlotArgs) {
			args = append(args, fpSlotArgs[fpIdx])
			fpIdx++
			continue
		}
		n := param.Words()
		if idx-n < 0 {
			break
		}
		args = append(args, typedStackWordsValue(rawWords[idx-n:idx], param.Type))
		idx -= n
	}
	for i := idx - 1; i >= 0; i-- {
		args = append(args, rawWords[i])
	}
	return args
}

// convert 1 to 2 word args into a Value
func stackWordsValue(words []Value) Value {
	if len(words) == 0 {
		return UnknownVal("stack")
	}
	if len(words) == 1 {
		return words[0]
	}
	if v, ok := phiStackWordsByArm(words); ok {
		return v
	}
	cp := append([]Value(nil), words...)
	return &StackWords{Words: cp}
}

// typedWordsValue converts stack or register words using the known source type.
func typedWordsValue(words []Value, typ typeinfo.Type) Value {
	value := stackWordsValue(words)
	if len(words) != 2 || !typeinfo.IsFarPointer(typ) {
		return value
	}
	return FarPointerWordsVal(words[0], words[1])
}

// typedStackWordsValue converts stack words using the known source type.
func typedStackWordsValue(words []Value, typ typeinfo.Type) Value {
	value := stackWordsValue(words)
	if len(words) != 2 || !typeinfo.IsFarPointer(typ) {
		return value
	}
	// far pointers are pushed seg:off
	return FarPointerWordsVal(words[1], words[0])
}

// numCleanupWordsAfterCall gets the number of 16-bit words to pop off the
// modeled stack after a call.
func numCleanupWordsAfterCall(f *typeinfo.Function, next *asm.DecodedInst) int {
	if f.Conv == typeinfo.CCDxaxCx {
		return 0
	}
	if next != nil && next.Op == asm.OpADD {
		if next.Dst.Kind == asm.OKReg && next.Src.Kind == asm.OKImm && next.Dst.Reg == asm.RegSP {
			return int(next.Src.Imm+1) / 2
		}
	}
	return f.ParamWords()
}
