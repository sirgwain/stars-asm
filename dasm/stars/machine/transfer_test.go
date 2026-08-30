package machine

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
)

func TestHandleX87BinaryPopUsesStackOperands(t *testing.T) {
	st := newValueState()
	ctx := &extractor{}

	ctx.handleX87(st, x87Inst(asm.OpFLD1, asm.Operand{}, asm.Operand{}), Meta{})
	ctx.handleX87(st, x87Inst(asm.OpFLDZ, asm.Operand{}, asm.Operand{}), Meta{})
	ctx.handleX87(st, x87Inst(asm.OpFADDP, x87Operand(1), x87Operand(-1)), Meta{})

	if got, want := st.fpd, 1; got != want {
		t.Fatalf("fp depth = %d, want %d", got, want)
	}
	if got, want := st.peekFP(0).String(), "(1 + 0)"; got != want {
		t.Fatalf("ST(0) = %s, want %s", got, want)
	}
}

func TestHandleX87FCOMPPopsOnlyForFCOMP(t *testing.T) {
	st := newValueState()
	ctx := &extractor{}

	ctx.handleX87(st, x87Inst(asm.OpFLD1, asm.Operand{}, asm.Operand{}), Meta{})
	ctx.handleX87(st, x87Inst(asm.OpFLDZ, asm.Operand{}, asm.Operand{}), Meta{})
	ctx.handleX87(st, x87Inst(asm.OpFCOM, asm.Operand{}, x87Operand(1)), Meta{})
	if got, want := st.fpd, 2; got != want {
		t.Fatalf("fp depth after FCOM = %d, want %d", got, want)
	}

	ctx.handleX87(st, x87Inst(asm.OpFCOMP, asm.Operand{}, x87Operand(1)), Meta{})
	if got, want := st.fpd, 1; got != want {
		t.Fatalf("fp depth after FCOMP = %d, want %d", got, want)
	}
}

func TestMemoryAccessFromBPOperandUsesFrameBase(t *testing.T) {
	st := seedEntryState(ctxForGraphTest())
	st.writeReg(asm.RegBP, st.readReg(asm.RegSP))

	mem := st.memoryAccessFromOperand(0, OperandDst, memOperandForTest(asm.RegBP, 10, 2))

	if got, want := mem.String(), "[bp+0xa]"; got != want {
		t.Fatalf("BP memory = %s, want %s", got, want)
	}
}

func TestMemoryAccessFromAddressRegisterUsesAddressProvenance(t *testing.T) {
	st := seedEntryState(ctxForGraphTest())
	index := BinaryVal(ValueOpShl, LoadVal(MemoryAccess{Base: FrameBaseVal(), Disp: -0x90, Width: 2}), ConstVal(1))
	st.writeReg(asm.RegBX, BinaryVal(ValueOpAdd, AddressVal(MemoryAccess{Base: FrameBaseVal(), Disp: -0x8a, Width: 2}), index))

	mem := st.memoryAccessFromOperand(0xaeae, OperandSrc, memOperandForTest(asm.RegBX, 0, 2))

	if mem.Seg != nil {
		t.Fatalf("memory segment = %s, want nil", mem.Seg)
	}
	if _, ok := mem.Base.(*FrameBase); !ok {
		t.Fatalf("memory base = %T, want FrameBase", mem.Base)
	}
	if got, want := mem.Disp, -0x8a; got != want {
		t.Fatalf("memory disp = %#x, want %#x", got, want)
	}
	if got, want := mem.Index.String(), "(load([bp-0x90]) << 0x1)"; got != want {
		t.Fatalf("memory index = %s, want %s", got, want)
	}
}

func TestHandleMOVSWCopiesAndAdvancesIndexes(t *testing.T) {
	st := seedEntryState(ctxForGraphTest())
	ctx := &extractor{}
	st.writeReg(asm.RegSI, ConstVal(0x200))
	st.writeReg(asm.RegDI, ConstVal(0x300))

	effects := ctx.handleMOVS(st, movsInst(asm.OpMOVSW), Meta{InstOff: 0x1000}, 2, false)

	if got, want := len(effects), 1; got != want {
		t.Fatalf("effects = %d, want %d", got, want)
	}
	copyEffect := effects[0].(CopyEffect)
	if got, want := copyEffect.Width, 2; got != want {
		t.Fatalf("copy width = %d, want %d", got, want)
	}
	if got, want := copyEffect.Src.String(), "addr(0x0:[0x200])"; got != want {
		t.Fatalf("copy src = %s, want %s", got, want)
	}
	if got, want := copyEffect.Dst.String(), "addr(es:[0x300])"; got != want {
		t.Fatalf("copy dst = %s, want %s", got, want)
	}
	if got, want := st.readReg(asm.RegSI).String(), "0x202"; got != want {
		t.Fatalf("SI = %s, want %s", got, want)
	}
	if got, want := st.readReg(asm.RegDI).String(), "0x302"; got != want {
		t.Fatalf("DI = %s, want %s", got, want)
	}
}

func TestHandleREPMOVSWCopiesConstantCountAndClearsCX(t *testing.T) {
	st := seedEntryState(ctxForGraphTest())
	ctx := &extractor{}
	st.writeReg(asm.RegSI, ConstVal(0x200))
	st.writeReg(asm.RegDI, ConstVal(0x300))
	st.writeReg(asm.RegCX, ConstVal(0x12))

	effects := ctx.handleMOVS(st, movsInst(asm.OpMOVSW), Meta{InstOff: 0x1000}, 2, true)

	if got, want := len(effects), 1; got != want {
		t.Fatalf("effects = %d, want %d", got, want)
	}
	copyEffect := effects[0].(CopyEffect)
	if got, want := copyEffect.Width, 0x24; got != want {
		t.Fatalf("copy width = %#x, want %#x", got, want)
	}
	if got, want := st.readReg(asm.RegSI).String(), "0x224"; got != want {
		t.Fatalf("SI = %s, want %s", got, want)
	}
	if got, want := st.readReg(asm.RegDI).String(), "0x324"; got != want {
		t.Fatalf("DI = %s, want %s", got, want)
	}
	if got, want := st.readReg(asm.RegCX).String(), "0x0"; got != want {
		t.Fatalf("CX = %s, want %s", got, want)
	}
}

func TestHandleREPMOVSBUnknownCountEmitsUnknownAndClobbersIndexes(t *testing.T) {
	st := seedEntryState(ctxForGraphTest())
	ctx := &extractor{}

	effects := ctx.handleMOVS(st, movsInst(asm.OpMOVSB), Meta{InstOff: 0x1000}, 1, true)

	if got, want := len(effects), 1; got != want {
		t.Fatalf("effects = %d, want %d", got, want)
	}
	if _, ok := effects[0].(UnknownEffect); !ok {
		t.Fatalf("effect = %T, want UnknownEffect", effects[0])
	}
	if got, want := st.readReg(asm.RegSI).String(), "Unknown movs"; got != want {
		t.Fatalf("SI = %s, want %s", got, want)
	}
	if got, want := st.readReg(asm.RegDI).String(), "Unknown movs"; got != want {
		t.Fatalf("DI = %s, want %s", got, want)
	}
	if got, want := st.readReg(asm.RegCX).String(), "0x0"; got != want {
		t.Fatalf("CX = %s, want %s", got, want)
	}
}

func TestBinaryResultMasksDeadLowByteWriteParent(t *testing.T) {
	written := LoadVal(MemoryAccess{Base: ConstVal(0x1234), Width: 1})
	patched := ByteWriteVal(UnknownVal("loop"), ByteLow, written)

	got := binaryResult(ValueOpAnd, patched, ConstVal(0x00ff))

	if containsLoopUnknown(got, 0) {
		t.Fatalf("masked value still contains loop unknown: %s", got)
	}
	if got, want := got.String(), "load(byte [0x1234])"; got != want {
		t.Fatalf("masked value = %s, want %s", got, want)
	}
}

// x87Inst builds an x87 instruction for transfer tests.
func x87Inst(op asm.Op, dst asm.Operand, src asm.Operand) asm.DecodedInst {
	return asm.DecodedInst{Op: op, Dst: dst, Src: src}
}

// x87Operand builds an x87 register operand for transfer tests.
func x87Operand(index int8) asm.Operand {
	return asm.Operand{Kind: asm.OKX87Reg, X87Index: index}
}

// memOperandForTest builds a memory operand for transfer tests.
func memOperandForTest(base asm.Reg, disp int, width int) asm.Operand {
	return asm.Operand{
		Kind: asm.OKMem,
		Mem:  asm.MemRef{Base: base, Disp: disp, MemSize: width},
	}
}

// movsInst builds a MOVS instruction for transfer tests.
func movsInst(op asm.Op) asm.DecodedInst {
	width := 1
	mnemonic := "MOVSB"
	if op == asm.OpMOVSW {
		width = 2
		mnemonic = "MOVSW"
	}
	dst := asm.Operand{Kind: asm.OKMem, Mem: asm.MemRef{Base: asm.RegDI, SegOverride: asm.RegES, MemSize: width}}
	src := asm.Operand{Kind: asm.OKMem, Mem: asm.MemRef{Base: asm.RegSI, SegOverride: asm.RegDS, MemSize: width}}
	return asm.DecodedInst{Op: op, Mnemonic: mnemonic, Dst: dst, Src: src}
}
