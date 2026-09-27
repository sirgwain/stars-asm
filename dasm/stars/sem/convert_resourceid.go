package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertResourceIDArg converts a machine zero-segment far pointer resource
// parameter. A merged argument whose every arm is a resource ID becomes a
// merge of resource IDs, so the temp carrying it holds MAKEINTRESOURCE values.
func (c *machineConverter) convertResourceIDArg(value machine.Value, param *typeinfo.FunctionVar) (Expr, bool) {
	if param == nil || param.Semantic != typeinfo.ParamSemanticResourceNameOrID {
		return nil, false
	}
	phi, ok := value.(*machine.PhiValue)
	if !ok {
		id, ok := resourceIDOffset(value)
		if !ok {
			return nil, false
		}
		return c.resourceID(id, param.Type), true
	}

	arms := make([]MergeArm, 0, len(phi.Arms))
	for _, arm := range phi.Arms {
		if arm.Block == nil {
			continue
		}
		id, ok := resourceIDOffset(arm.Value)
		if !ok {
			return nil, false
		}
		// Convert each arm in its own block's context, like other merge arms.
		value := c.convertValueInBlock(arm.Block.ID, id, typeinfo.U16)
		arms = append(arms, MergeArm{Block: arm.Block.ID, Value: &ResourceID{Value: value, TypeInfo: param.Type}})
	}
	return &Merge{TypeInfo: param.Type, Join: phi.Join, Arms: arms}, true
}

// resourceIDOffset returns the integer ID carried by a resource name-or-ID
// value: a bare constant or a far pointer with a zero segment.
func resourceIDOffset(value machine.Value) (machine.Value, bool) {
	if c, ok := value.(*machine.Const); ok {
		return c, true
	}
	words, ok := value.(*machine.StackWords)
	if !ok || len(words.Words) != 2 {
		return nil, false
	}
	seg, ok := words.Words[0].(*machine.Const)
	if !ok || seg.Val != 0 {
		return nil, false
	}
	return words.Words[1], true
}

// resourceID wraps an integer resource ID as a value of the parameter type.
func (c *machineConverter) resourceID(id machine.Value, typ typeinfo.Type) Expr {
	if k, ok := id.(*machine.Const); ok {
		return &ResourceID{
			Value:    &Const{TypeInfo: typeinfo.U16, U64: uint64(k.Val), Origin: k.Origin, Fixup: k.Fixup},
			TypeInfo: typ,
		}
	}
	return &ResourceID{Value: c.convertValue(id), TypeInfo: typ}
}
