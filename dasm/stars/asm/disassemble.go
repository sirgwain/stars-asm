package asm

import (
	"cmp"
	"fmt"
	"slices"
)

type DecodedFunc struct {
	Instrs []DecodedInst
	Labels map[uint16]string
}

// DecodeFunc decodes the raw bytes of the function described by ctx.FS into a
// slice of instructions.  It does not perform any analysis passes (jump-table
// fixup, call/label resolution, or annotation); those are handled by the
// pipeline.
//
// On success ctx.Bytes, ctx.BaseOff, ctx.DecodeLimit, and ctx.Avail are
// populated.
func DecodeFunc(ctx *FuncContext) (*DecodedFunc, error) {
	img := ctx.img
	fs := ctx.fs

	neSeg := int(fs.Addr.Seg)
	baseOff := fs.Addr.Off
	avail, ok := img.segmentAvail(neSeg, baseOff)
	if !ok {
		return nil, fmt.Errorf("no initialized bytes for function %s at seg %d off 0x%x", fs.Name, neSeg, baseOff)
	}

	// Clamp decode window to NB09 procedure length.
	limit := avail
	if fs.Len != 0 && uint32(limit) > uint32(fs.Len) {
		limit = uint32(fs.Len)
	}
	if limit <= 0 {
		return nil, fmt.Errorf("no bytes available for function %s (avail=%d len=%d)", fs.Name, avail, fs.Len)
	}

	// Allocate a buffer large enough for the full function plus a small pad
	// so the last instruction can be fully decoded.  When the function extends
	// into the segment's BSS/uninitialized region (fs.Len > avail), the
	// trailing bytes stay zero-filled, matching the in-memory image.
	const decodePad = 16
	bufLen := uint32(limit) + decodePad
	readLen := min(bufLen, avail)
	buf := make([]byte, bufLen)
	if readLen > 0 {
		if _, ok := img.ReadSegmentAt(neSeg, baseOff, buf[:readLen]); !ok {
			return nil, fmt.Errorf("read function %s at seg %d off 0x%x", fs.Name, neSeg, baseOff)
		}
	}

	// Populate context fields for callers that need the decoded byte window.
	ctx.bytes = buf
	ctx.baseOff = baseOff
	ctx.decodeLimit = limit
	ctx.avail = avail

	instrs, labels := decodeReachableFunc(ctx, buf, baseOff, limit, neSeg)

	return &DecodedFunc{Instrs: instrs, Labels: labels}, nil
}

func resolveImmediateOperandFixups(img *ImageNE, segNum int, inst *DecodedInst) {
	switch {
	case inst.Op == OpCALLF:
		resolveFarCallFixup(img, segNum, inst)
	case inst.Src.Kind == OKImm && inst.Len >= 2:
		applyImmediateFixup(img, segNum, uint16(inst.Off+uint32(inst.Len-2)), &inst.Src)
	}
}

func resolveFarCallFixup(img *ImageNE, segNum int, inst *DecodedInst) {
	if inst.Dst.Kind != OKImm || inst.Src.Kind != OKImm {
		return
	}

	if fx, ok := img.FixupAt(segNum, uint16(inst.Off+1)); ok {
		fx = resolveCallFixup(fx, inst.Dst.Imm)
		inst.Fixup = &fx
		return
	}

	if fx, ok := img.FixupAt(segNum, uint16(inst.Off+3)); ok {
		fx = resolveCallFixup(fx, inst.Dst.Imm)
		inst.Fixup = &fx
	}
}

func resolveCallFixup(fx Fixup, callOff uint16) Fixup {
	if fx.Target == FixupTargetInternalRef {
		fx.ResolvedSeg = fx.TargetSegNum
		switch fx.Source {
		case FixupSourceSegment:
			fx.ResolvedOff = callOff
		default:
			fx.ResolvedOff = fx.TargetOff
		}
	}
	return fx
}

func applyImmediateFixup(img *ImageNE, segNum int, wordSite uint16, op *Operand) {
	if op.Kind != OKImm {
		return
	}
	fx, ok := img.FixupAt(segNum, wordSite)
	if !ok {
		return
	}
	fxCopy := fx
	op.Fixup = &fxCopy
}

type switchTable struct {
	entries []DecodedInst
	targets []uint32
}

// decodeReachableFunc decodes a function by following reachable control flow
// instead of sweeping linearly through every byte. This matters for compiler
// switch tables: an indirect JMP may be followed by inline data that only
// becomes code after the table target is selected.
func decodeReachableFunc(ctx *FuncContext, buf []byte, baseOff, limit uint32, neSeg int) ([]DecodedInst, map[uint16]string) {
	instrs := make(map[uint32]DecodedInst)
	data := make(map[uint32]DecodedInst)
	dataBytes := make(map[uint32]bool)
	labels := make(map[uint16]string)
	queue := []uint32{0}

	addLabel := func(target int32) {
		if target > 0 {
			labels[uint16(target)] = fmt.Sprintf("L_%04x", target)
		}
	}
	queueTarget := func(target int32) {
		addLabel(target)
		if target < int32(baseOff) {
			return
		}
		rel := uint32(target) - baseOff
		if rel < limit {
			queue = append(queue, rel)
		}
	}

	for len(queue) > 0 {
		ip := queue[len(queue)-1]
		queue = queue[:len(queue)-1]

		for ip < limit {
			if _, ok := instrs[ip]; ok {
				break
			}
			if dataBytes[ip] {
				break
			}

			instOff := baseOff + ip
			inst := decode(buf[ip:], instOff)
			inst.Off = instOff
			resolveImmediateOperandFixups(ctx.img, neSeg, &inst)
			instrs[ip] = inst
			addLabel(inst.Target)

			if inst.Len <= 0 {
				break
			}

			// A recognized switch dispatch consumes the inline table as data
			// and queues each table entry as a code target. There is no
			// fallthrough from the indirect JMP into the table bytes.
			if table, ok := detectSwitchTable(instrs, dataBytes, inst, buf, baseOff, limit); ok {
				for _, entry := range table.entries {
					rel := entry.Off - baseOff
					data[rel] = entry
					for n := uint32(0); n < uint32(entry.Len); n++ {
						dataBytes[rel+n] = true
					}
					addLabel(entry.Target)
				}
				for _, target := range table.targets {
					queue = append(queue, target-baseOff)
				}
				break
			}

			next := ip + uint32(inst.Len)
			if inst.stop {
				break
			}

			// Conditional branches continue decoding the fallthrough path and
			// queue the taken path. Direct unconditional jumps only queue their
			// target; indirect jumps stop the current path unless recognized
			// above as a switch table.
			switch {
			case inst.Op == OpJcc:
				queueTarget(inst.Target)
				ip = next
			case inst.Op == OpJMP:
				if inst.Target >= 0 {
					queueTarget(inst.Target)
				}
			default:
				ip = next
			}
		}
	}

	out := make([]DecodedInst, 0, len(instrs)+len(data))
	for _, inst := range instrs {
		out = append(out, inst)
	}
	for _, inst := range data {
		out = append(out, inst)
	}
	slices.SortFunc(out, func(a, b DecodedInst) int {
		return cmp.Compare(a.Off, b.Off)
	})

	return out, labels
}

// detectSwitchTable recognizes the narrow Win16 compiler pattern used in this
// binary:
//
//	CMP ax, max
//	JBE dispatch
//	...
//	dispatch:
//	SHL ax, 1
//	MOV bx, ax
//	JMP cs:[bx+table]
//
// It returns synthetic DW instructions for the table and the code targets read
// from those words. The checks are intentionally strict to avoid treating an
// arbitrary indirect jump as inline data.
func detectSwitchTable(instrs map[uint32]DecodedInst, dataBytes map[uint32]bool, jmp DecodedInst, buf []byte, baseOff, limit uint32) (switchTable, bool) {
	if jmp.Op != OpJMP || jmp.Src.Kind != OKMem {
		return switchTable{}, false
	}
	mem := jmp.Src.Mem
	if mem.SegOverride != RegCS || mem.Base != RegBX || mem.index != RegNone {
		return switchTable{}, false
	}
	if mem.Disp < int(baseOff) {
		return switchTable{}, false
	}
	tableStart := uint32(mem.Disp)
	if tableStart < baseOff || tableStart >= baseOff+limit {
		return switchTable{}, false
	}

	// Look at the instruction stream already decoded before this JMP. Recursive
	// traversal may enter the dispatch block from a queued branch target, so the
	// range check is not necessarily adjacent to SHL/MOV/JMP in this path.
	prev := previousDecoded(instrs, jmp.Off-baseOff, 10)
	if len(prev) < 2 {
		return switchTable{}, false
	}
	shl := prev[len(prev)-2]
	mov := prev[len(prev)-1]
	if shl.Op != OpSHL || !isReg(shl.Dst, RegAX) || !isImm(shl.Src, 1) {
		return switchTable{}, false
	}
	if mov.Op != OpMOV || !isReg(mov.Dst, RegBX) || !isReg(mov.Src, RegAX) {
		return switchTable{}, false
	}

	maxCase, ok := previousAXBound(prev[:len(prev)-2], shl.Off)
	if !ok {
		return switchTable{}, false
	}

	numEntries := int(maxCase) + 1
	tableEnd := tableStart + uint32(numEntries)*2
	if tableEnd > baseOff+limit || tableEnd-baseOff > uint32(len(buf)) {
		return switchTable{}, false
	}

	table := switchTable{
		entries: make([]DecodedInst, 0, numEntries),
		targets: make([]uint32, 0, numEntries),
	}
	for e := range numEntries {
		off := tableStart + uint32(e)*2
		rel := off - baseOff
		if dataBytes[rel] || dataBytes[rel+1] {
			return switchTable{}, false
		}
		val := uint16(buf[rel]) | uint16(buf[rel+1])<<8
		target := uint32(val)
		if target < baseOff || target >= baseOff+limit {
			return switchTable{}, false
		}
		table.entries = append(table.entries, DecodedInst{
			Off:      off,
			Bytes:    buf[rel : rel+2],
			Mnemonic: "DW",
			Op:       OpDW,
			Src:      Operand{Kind: OKImm, Imm: val},
			Len:      2,
			Target:   int32(val),
		})
		table.targets = append(table.targets, target)
	}

	return table, true
}

// previousDecoded returns the most recent decoded instructions before beforeRel,
// sorted in address order. It deliberately uses the map of instructions already
// reached by control flow rather than re-decoding bytes.
func previousDecoded(instrs map[uint32]DecodedInst, beforeRel uint32, max int) []DecodedInst {
	prev := make([]DecodedInst, 0, max)
	for rel, inst := range instrs {
		if rel < beforeRel {
			prev = append(prev, inst)
		}
	}
	slices.SortFunc(prev, func(a, b DecodedInst) int {
		return cmp.Compare(a.Off, b.Off)
	})
	if len(prev) > max {
		prev = prev[len(prev)-max:]
	}
	return prev
}

// previousAXBound finds the CMP/JBE pair that proves AX is in range for a jump
// table dispatch. Only unsigned JBE is accepted because that is the pattern
// emitted in the known switch tables.
func previousAXBound(prev []DecodedInst, dispatchOff uint32) (uint16, bool) {
	for i := len(prev) - 1; i >= 0; i-- {
		inst := prev[i]
		if inst.Op == OpCMP && isReg(inst.Dst, RegAX) && inst.Src.Kind == OKImm {
			for j := i + 1; j < len(prev); j++ {
				if prev[j].Mnemonic == "JBE" && prev[j].Target == int32(dispatchOff) {
					return inst.Src.Imm, true
				}
			}
		}
	}
	return 0, false
}

func isReg(op Operand, r Reg) bool {
	return op.Kind == OKReg && op.Reg == r
}

func isImm(op Operand, v uint16) bool {
	return op.Kind == OKImm && op.Imm == v
}
