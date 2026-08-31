package sem

import (
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type HelperKind uint8

const (
	HelperNone HelperKind = iota
	HelperWideMulU
	HelperWideDivU
	HelperWideDivS
	HelperWideRemU
	HelperWideRemS
	HelperWideShlS
	HelperWideShrS
	HelperWideShrU
	HelperFloatToLong
)

type HelperInfo struct {
	Kind     HelperKind
	Name     string
	PureExpr bool
	FlagOnly bool
}

type compilerHelpersProcessors struct {
	replacements []compilerHelperReplacement
}

type compilerHelperReplacement struct {
	off   uint32
	value machine.Value
}

// ProcessMachineBlock lowers pure compiler helper calls and records their
// result expressions for uses in following machine blocks.
func (p *compilerHelpersProcessors) ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool) {
	if len(b.Effects) == 0 {
		return b, false
	}

	changed := false
	rewriter := p.rewriter()
	effects := make([]machine.Effect, 0, len(b.Effects))
	for _, effect := range b.Effects {
		next, effectChanged := rewriter.rewriteMachineEffect(effect)
		changed = changed || effectChanged

		call, ok := next.(machine.CallEffect)
		if !ok || call.Target == nil {
			effects = append(effects, next)
			continue
		}
		helper := p.classifyCompilerHelper(call.Target.Name)
		value, ok := p.lowerCompilerHelperValue(helper, call.Args)
		if !ok {
			effects = append(effects, next)
			continue
		}
		if result, ok := call.Result.(*machine.CallResult); ok {
			p.replacements = append(p.replacements, compilerHelperReplacement{off: result.InstOff, value: value})
		} else {
			effects = append(effects, next)
			continue
		}
		changed = true
	}

	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// classifyCompilerHelper identifies known compiler runtime helpers and records
// whether they can be represented as pure effect values.
func (p *compilerHelpersProcessors) classifyCompilerHelper(name string) HelperInfo {
	switch strings.ToLower(name) {
	case "__afulmul":
		return HelperInfo{Kind: HelperWideMulU, Name: name, PureExpr: true}
	case "__afuldiv":
		return HelperInfo{Kind: HelperWideDivU, Name: name, PureExpr: true}
	case "__afldiv":
		return HelperInfo{Kind: HelperWideDivS, Name: name, PureExpr: true}
	case "__afulrem":
		return HelperInfo{Kind: HelperWideRemU, Name: name, PureExpr: true}
	case "__aflrem":
		return HelperInfo{Kind: HelperWideRemS, Name: name, PureExpr: true}
	case "__aflshl":
		return HelperInfo{Kind: HelperWideShlS, Name: name, PureExpr: true}
	case "__aflshr":
		return HelperInfo{Kind: HelperWideShrS, Name: name, PureExpr: true}
	case "__afulshr":
		return HelperInfo{Kind: HelperWideShrU, Name: name, PureExpr: true}
	case "__ftol":
		return HelperInfo{Kind: HelperFloatToLong, Name: name, PureExpr: true}
	case "__affcompp":
		return HelperInfo{Name: name, FlagOnly: true}
	default:
		return HelperInfo{Name: name}
	}
}

// lowerCompilerHelperValue converts a classified pure compiler helper call into
// the equivalent structured effect value.
func (p *compilerHelpersProcessors) lowerCompilerHelperValue(helper HelperInfo, args []machine.Value) (machine.Value, bool) {
	if !helper.PureExpr || len(args) == 0 {
		return nil, false
	}
	switch helper.Kind {
	case HelperFloatToLong:
		return machine.CastVal(args[0], typeinfo.I32), true
	case HelperWideMulU:
		return p.lowerBinaryCompilerHelperValue(machine.ValueOpMul, typeinfo.U32, args)
	case HelperWideDivU, HelperWideDivS:
		return p.lowerBinaryCompilerHelperValue(machine.ValueOpDiv, helperResultType(helper), args)
	case HelperWideRemU, HelperWideRemS:
		return p.lowerBinaryCompilerHelperValue(machine.ValueOpMod, helperResultType(helper), args)
	case HelperWideShlS:
		return p.lowerBinaryCompilerHelperValue(machine.ValueOpShl, typeinfo.I32, args)
	case HelperWideShrS:
		return p.lowerBinaryCompilerHelperValue(machine.ValueOpSar, typeinfo.I32, args)
	case HelperWideShrU:
		return p.lowerBinaryCompilerHelperValue(machine.ValueOpShr, typeinfo.U32, args)
	default:
		return nil, false
	}
}

// helperResultType returns the source result type for a wide helper.
func helperResultType(helper HelperInfo) typeinfo.Type {
	switch helper.Kind {
	case HelperWideDivU, HelperWideRemU:
		return typeinfo.U32
	default:
		return typeinfo.I32
	}
}

// lowerBinaryCompilerHelperValue lowers a two-argument helper to a binary value
// when both operands are usable.
func (p *compilerHelpersProcessors) lowerBinaryCompilerHelperValue(op machine.ValueOp, to typeinfo.Type, args []machine.Value) (machine.Value, bool) {
	if len(args) < 2 {
		return nil, false
	}
	return machine.CastVal(machine.BinaryResult(op, args[0], args[1]), to), true
}

// rewriter returns the machine tree rewrite for compiler helper replacements.
func (p *compilerHelpersProcessors) rewriter() *machineRewriter {
	return &machineRewriter{
		value: func(w *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
			result, ok := value.(*machine.CallResult)
			if !ok {
				return value, false, false
			}
			for i := len(p.replacements) - 1; i >= 0; i-- {
				if p.replacements[i].off == result.InstOff {
					return p.replacements[i].value, true, true
				}
			}
			return value, false, true
		},
	}
}
