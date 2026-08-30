package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// indirectCallArgs groups raw indirect-call stack words by the inferred signature.
func indirectCallArgs(fn *typeinfo.Function, rawWords []machine.Value) []machine.Value {
	if fn.Conv == typeinfo.CCPascal {
		return leftToRightIndirectCallArgs(fn, rawWords)
	}
	return rightToLeftIndirectCallArgs(fn, rawWords)
}

// leftToRightIndirectCallArgs groups Pascal stack words in parameter order.
func leftToRightIndirectCallArgs(fn *typeinfo.Function, rawWords []machine.Value) []machine.Value {
	args := make([]machine.Value, 0, len(fn.Params))
	idx := 0
	for _, param := range fn.Params {
		n := param.Words()
		if idx+n > len(rawWords) {
			break
		}
		args = append(args, indirectStackWordsValue(rawWords[idx:idx+n]))
		idx += n
	}
	args = append(args, rawWords[idx:]...)
	return args
}

// rightToLeftIndirectCallArgs groups cdecl/stdcall stack words in parameter order.
func rightToLeftIndirectCallArgs(fn *typeinfo.Function, rawWords []machine.Value) []machine.Value {
	args := make([]machine.Value, 0, len(fn.Params))
	idx := len(rawWords)
	for _, param := range fn.Params {
		n := param.Words()
		if idx-n < 0 {
			break
		}
		args = append(args, indirectStackWordsValue(rawWords[idx-n:idx]))
		idx -= n
	}
	for i := idx - 1; i >= 0; i-- {
		args = append(args, rawWords[i])
	}
	return args
}

// indirectStackWordsValue preserves single-word args and wraps wider raw args.
func indirectStackWordsValue(words []machine.Value) machine.Value {
	if len(words) == 0 {
		return machine.UnknownVal("stack")
	}
	if len(words) == 1 {
		return words[0]
	}
	return &machine.StackWords{Words: append([]machine.Value(nil), words...)}
}

// convertCallArgs converts call arguments using the callee parameter types when known.
func (c *machineConverter) convertCallArgs(fn *typeinfo.Function, values []machine.Value) []Expr {
	out := make([]Expr, len(values))
	for i, value := range values {
		var expected typeinfo.Type
		var param *typeinfo.FunctionVar
		if fn != nil && i < len(fn.Params) {
			param = &fn.Params[i]
			expected = param.Type
			if expr, ok := c.convertResourceIDArg(value, param); ok {
				out[i] = expr
				continue
			}
		}
		out[i] = c.convertValueTyped(value, expected)
	}
	return out
}

// convertResourceIDArg converts a machine zero-segment far pointer resource parameter.
func (c *machineConverter) convertResourceIDArg(value machine.Value, param *typeinfo.FunctionVar) (Expr, bool) {
	if param == nil || param.Semantic != typeinfo.ParamSemanticResourceNameOrID {
		return nil, false
	}
	ptr, ok := value.(*machine.FarPointer)
	if !ok || ptr.Part != machine.FarPointerWhole {
		return nil, false
	}
	seg, ok := ptr.Segment.(*machine.Const)
	if !ok || seg.Val != 0 {
		return nil, false
	}
	return &ResourceID{Value: c.convertValue(ptr.Offset), TypeInfo: param.Type}, true
}

// convertValueTyped converts one machine value with an optional expected call type.
func (c *machineConverter) convertValueTyped(value machine.Value, expected typeinfo.Type) Expr {
	if expected != nil {
		if expr, ok := c.convertAddressArgTyped(value, expected); ok {
			return expr
		}
		if expr, ok := c.convertNearPointerAddressTyped(value, expected); ok {
			return expr
		}
		if expr, ok := c.convertFarPointerTyped(value, expected); ok {
			return expr
		}
	}
	return c.convertValue(value)
}

// convertAddressArgTyped preserves machine address operands as source address-of expressions.
func (c *machineConverter) convertAddressArgTyped(value machine.Value, expected typeinfo.Type) (Expr, bool) {
	ptrType, ok := expected.(*typeinfo.Pointer)
	addr, addrOK := value.(*machine.Address)
	if !ok || !addrOK {
		return nil, false
	}
	width := ptrType.Elem.Bytes()
	if width == 0 {
		width = addr.Access.Width
	}
	target := c.convertMemoryLValue(addr.Access, width)
	if !typeinfo.IsCallCompatible(ptrType.Elem, target.ExprType()) {
		return nil, false
	}
	return &AddressOf{Target: target, TypeInfo: expected}, true
}
