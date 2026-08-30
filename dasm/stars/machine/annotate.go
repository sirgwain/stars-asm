package machine

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type OperandRole uint8

const (
	OperandDst OperandRole = iota
	OperandSrc
)

// String renders an operand role for diagnostics and value identities.
func (r OperandRole) String() string {
	switch r {
	case OperandDst:
		return "dst"
	case OperandSrc:
		return "src"
	default:
		return "operand"
	}
}

type Origin struct {
	InstOff uint32
	Role    OperandRole
}

type AnnotationKey struct {
	InstOff uint32
	Role    OperandRole
}

func labelFor(off uint32) string {
	return fmt.Sprintf("L_%04x", off)
}

// resolveInstMetadata resolves call and jump metadata for decoded instructions.
func resolveInstMetadata(ctx *FuncContext, insts []asm.DecodedInst) (map[uint32]*InstCall, map[uint32]*InstJump) {
	calls := make(map[uint32]*InstCall)
	jumps := make(map[uint32]*InstJump)

	for i, inst := range insts {
		switch inst.Op {
		case asm.OpCALLF:
			if callTarget, ok := ctx.res.ResolveFunction(inst.Fixup); ok {
				calls[inst.Off] = &InstCall{Target: callTarget}

			} else if inst.Src.Kind == asm.OKMem {
				// mem load like CALLF [bp+0x6]
				if inst.Src.Mem.Base == asm.RegBP {
					// check for a local var function pointer
					if local, ok := ctx.res.ResolveLocal(ctx.fs, inst.Off, inst.Src.Mem.Disp); ok {
						if callTarget, ok := typeinfo.GetFunctionPointerFunction(local.Local.Type); ok {
							calls[inst.Off] = &InstCall{Target: callTarget}
						}
					}
				}
			}
		case asm.OpJMP, asm.OpJcc:
			if inst.Target == 0 && inst.Src.Kind == asm.OKMem || inst.Dst.Kind == asm.OKMem {
				jumps[inst.Off] = &InstJump{TableTargetOffs: jumpTableTargets(insts, i+1)}
				continue
			}
			label := labelFor(uint32(inst.Target))
			if symbolicLabel := ctx.fs.GetLabel(uint32(inst.Target)); symbolicLabel != nil {
				label = symbolicLabel.Name
			}
			jumps[inst.Off] = &InstJump{TargetOff: uint32(inst.Target), Label: label}
		}
	}
	return calls, jumps
}

func jumpTableTargets(insts []asm.DecodedInst, startIdx int) []uint32 {
	if startIdx < 0 || startIdx >= len(insts) {
		return nil
	}
	seen := make(map[uint32]struct{})
	targets := make([]uint32, 0, 8)
	for i := startIdx; i < len(insts); i++ {
		inst := insts[i]
		if inst.Op != asm.OpDW {
			break
		}
		if inst.Target < 0 {
			continue
		}
		target := uint32(inst.Target)
		if _, ok := seen[target]; ok {
			continue
		}
		seen[target] = struct{}{}
		targets = append(targets, target)
	}
	return targets
}
