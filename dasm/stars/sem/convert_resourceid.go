package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertResourceIDArg converts a machine zero-segment far pointer resource parameter.
func (c *machineConverter) convertResourceIDArg(value machine.Value, param *typeinfo.FunctionVar) (Expr, bool) {
	if param == nil || param.Semantic != typeinfo.ParamSemanticResourceNameOrID {
		return nil, false
	}
	if c, ok := value.(*machine.Const); ok {
		return &ResourceID{
			Value:    &Const{TypeInfo: typeinfo.U16, U64: uint64(c.Val), Origin: c.Origin, Fixup: c.Fixup},
			TypeInfo: param.Type,
		}, true
	}
	words, ok := value.(*machine.StackWords)
	if !ok || len(words.Words) != 2 {
		return nil, false
	}
	seg, ok := words.Words[0].(*machine.Const)
	if !ok || seg.Val != 0 {
		return nil, false
	}
	return &ResourceID{Value: c.convertValue(words.Words[1]), TypeInfo: param.Type}, true
}
