package sem

import (
	"fmt"
	"strconv"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type resolveEnumsProcessor struct {
	ctx       *FuncContext
	callTypes map[callResultKey]typeinfo.Type
	// wParam is the window procedure's wParam, and wParamEnum the enum or
	// wParamChar the character code the message handled by the current block
	// gives it.
	wParam     *typeinfo.FunctionVar
	wParamEnum *typeinfo.Enum
	wParamChar bool
}

type callResultKey struct {
	instOff uint32
	name    string
}

// ProcessBlock resolves enum-typed constants and call results in one semantic block.
func (p *resolveEnumsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	if p.callTypes == nil {
		p.callTypes = make(map[callResultKey]typeinfo.Type)
	}
	p.wParam, p.wParamEnum, p.wParamChar = p.blockWParam(b.ID)
	effects, changed := p.rewriter().rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// resolveCallWithRewriter applies enum argument and call-result rules using an
// existing tree rewriter.
func (p *resolveEnumsProcessor) resolveCallWithRewriter(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool) {
	if call == nil || call.Function == nil {
		return call, false
	}

	// check for enum args
	argsChanged := false
	args := make([]Expr, len(call.Args))
	for i, arg := range call.Args {
		next, ok := w.rewriteExpr(arg)
		if i < len(call.Function.Params) {
			paramType := call.Function.Params[i].Type
			if enumType, matched := p.callParamEnum(call, i); matched {
				constNext, constChanged := retypeCallParamConst(next, enumType)
				next = constNext
				ok = ok || constChanged
			} else if enumType, isEnum := paramType.(*typeinfo.Enum); isEnum {
				enumNext, enumChanged := p.resolveExpectedEnum(next, enumType)
				next = enumNext
				ok = ok || enumChanged
			}
		}
		args[i] = next
		argsChanged = argsChanged || ok
	}

	next := *call
	if argsChanged {
		next.Args = args
	}
	changed := argsChanged
	if enumType, ok := p.callResultEnum(&next); ok {
		p.callTypes[callResultKey{instOff: meta.InstOff, name: call.Function.Name}] = enumType
		if next.Function.Ret != enumType {
			fn := *next.Function
			fn.Ret = enumType
			next.Function = &fn
			changed = true
		}
	}
	if !changed {
		return call, false
	}
	return &next, true
}

// rewriter returns the semantic tree rewrite for enum resolution.
func (p *resolveEnumsProcessor) rewriter() *semRewriter {
	return &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			switch e := effect.(type) {
			case *Assign:
				dst, dstChanged := w.rewriteLValue(e.Dst)
				src, srcChanged := w.rewriteExpr(e.Src)
				if enumType, ok := p.expectedEnumType(dst); ok {
					if nextSrc, changed := p.resolveExpectedEnum(src, enumType); changed {
						src = nextSrc
						srcChanged = true
					}
				}
				if !dstChanged && !srcChanged {
					return effect, false, true
				}
				next := *e
				next.Dst = dst
				next.Src = src
				return &next, true, true
			case *TableJump:
				// A switch on a value whose enum depends on context, such
				// as a part's item under a known slot type, names its cases
				// by that enum.
				index, changed := w.rewriteExpr(e.Index)
				next := *e
				next.Index = index
				if p.wParamChar && !e.CharCases && p.isWParam(next.SwitchedExpr()) {
					next.CharCases = true
					changed = true
				}
				if e.CaseEnum == nil {
					if enumType, _ := next.SwitchedValue(); enumType == nil {
						if switched := next.SwitchedExpr(); switched != nil {
							if enumType, ok := p.expectedEnumType(switched); ok {
								next.CaseEnum = enumType
								changed = true
							}
						}
					}
				}
				if !changed {
					return effect, false, true
				}
				return &next, true, true
			default:
				return effect, false, false
			}
		},
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			switch e := expr.(type) {
			case *CallResult:
				if enumType, ok := p.callResultType(e); ok && e.TypeInfo != enumType {
					next := *e
					next.TypeInfo = enumType
					return &next, true, true
				}
				return expr, false, true
			case *Compare:
				lhs, lhsChanged := w.rewriteExpr(e.LHS)
				rhs, rhsChanged := w.rewriteExpr(e.RHS)
				if enumType, ok := p.expectedEnumType(lhs); ok {
					if nextRHS, changed := p.resolveExpectedEnum(rhs, enumType); changed {
						rhs = nextRHS
						rhsChanged = true
					}
				}
				if enumType, ok := p.expectedEnumType(rhs); ok {
					if nextLHS, changed := p.resolveExpectedEnum(lhs, enumType); changed {
						lhs = nextLHS
						lhsChanged = true
					}
				}
				if p.wParamChar {
					if p.isWParam(lhs) {
						if next := markCharLiteral(rhs); next != rhs {
							rhs, rhsChanged = next, true
						}
					}
					if p.isWParam(rhs) {
						if next := markCharLiteral(lhs); next != lhs {
							lhs, lhsChanged = next, true
						}
					}
				}
				if !lhsChanged && !rhsChanged {
					return expr, false, true
				}
				next := *e
				next.LHS = lhs
				next.RHS = rhs
				return &next, true, true
			case *Binary:
				if e.Op != OpAnd && e.Op != OpOr && e.Op != OpXor {
					return expr, false, false
				}
				lhs, lhsChanged := w.rewriteExpr(e.LHS)
				rhs, rhsChanged := w.rewriteExpr(e.RHS)
				// a mask of a flag enum is a set of its flags; a mask of any
				// other enum selects bits that are not its values
				if enumType, ok := p.expectedEnumType(lhs); ok && enumType.EnumKind == typeinfo.EnumFlags {
					if nextRHS, changed := p.resolveExpectedEnum(rhs, enumType); changed {
						rhs = nextRHS
						rhsChanged = true
					}
				}
				if enumType, ok := p.expectedEnumType(rhs); ok && enumType.EnumKind == typeinfo.EnumFlags {
					if nextLHS, changed := p.resolveExpectedEnum(lhs, enumType); changed {
						lhs = nextLHS
						lhsChanged = true
					}
				}
				if !lhsChanged && !rhsChanged {
					return expr, false, true
				}
				next := *e
				next.LHS = lhs
				next.RHS = rhs
				return &next, true, true
			default:
				return expr, false, false
			}
		},
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			next, changed := p.resolveCallWithRewriter(w, call, meta)
			return next, changed, true
		},
	}
}

// blockWParam returns the window procedure's wParam with the enum the
// message handled on entry to block gives its whole value, such as VirtualKey
// for WM_KEYDOWN, or whether it is a character code, as for WM_CHAR. It
// returns nil and false when the block's message is unknown or gives wParam
// neither.
func (p *resolveEnumsProcessor) blockWParam(block machine.BlockID) (*typeinfo.FunctionVar, *typeinfo.Enum, bool) {
	message := p.ctx.messageByBlock[block]
	if message == nil || message.WParam == nil {
		return nil, nil, false
	}
	enumType, isEnum := message.WParam.Whole.Type.(*typeinfo.Enum)
	if !isEnum && !message.WParam.Whole.Char {
		return nil, nil, false
	}
	params, ok := p.ctx.windowProcParams()
	if !ok || params.wParam == nil {
		return nil, nil, false
	}
	return params.wParam, enumType, message.WParam.Whole.Char
}

// isWParam reports whether expr reads the window procedure's whole wParam.
func (p *resolveEnumsProcessor) isWParam(expr Expr) bool {
	local, ok := expr.(*Local)
	return ok && p.wParam != nil && local.Name == p.wParam.Name
}

// resolveExpectedEnum applies an expected enum type to a compatible expression.
func (p *resolveEnumsProcessor) resolveExpectedEnum(expr Expr, enumType *typeinfo.Enum) (Expr, bool) {
	switch e := expr.(type) {
	case *Const:
		if e.TypeInfo == enumType {
			return expr, false
		}
		if keepsConstantFamily(e.TypeInfo, enumType) {
			return expr, false
		}
		next := *e
		next.TypeInfo = enumType
		return &next, true
	case *Words:
		collapsed, ok := collapseWideWords(e)
		if !ok {
			return expr, false
		}
		return p.resolveExpectedEnum(collapsed, enumType)
	case *ResourceID:
		value, changed := p.resolveExpectedEnum(e.Value, enumType)
		if !changed {
			return expr, false
		}
		next := *e
		next.Value = value
		return &next, true
	default:
		return expr, false
	}
}

// expectedEnumType returns the static or path-sensitive enum type for an expression.
func (p *resolveEnumsProcessor) expectedEnumType(expr Expr) (*typeinfo.Enum, bool) {
	if enumType, ok := exprEnumType(expr); ok {
		return enumType, true
	}
	if p.wParamEnum != nil && p.isWParam(expr) {
		return p.wParamEnum, true
	}
	path, ok := symbolPathForExpr(expr)
	if !ok {
		return nil, false
	}
	unionContext := p.ctx.unionContext()
	if unionContext == nil {
		return nil, false
	}
	enumType, ok := unionContext.EnumFor(path)
	return enumType, ok
}

// callResultEnum returns the enum type selected for a call result.
func (p *resolveEnumsProcessor) callResultEnum(call *Call) (*typeinfo.Enum, bool) {
	for _, rule := range p.ctx.sdb.EnumRules {
		if rule.Kind != typeinfo.UseCallResult || rule.FuncName != call.Function.Name {
			continue
		}
		if !p.callMatchesRule(call, rule) {
			continue
		}
		enumType := p.ctx.sdb.GetEnum(rule.EnumName)
		if enumType == nil {
			continue
		}
		// A rule without argument constraints already retyped the function's
		// return; otherwise fit the enum to the declared return storage.
		if ret, ok := call.Function.Ret.(*typeinfo.Enum); ok && ret.Name == enumType.Name {
			return ret, true
		}
		return typeinfo.EnumWithStorageSize(enumType, call.Function.Ret), true
	}
	if enumType, ok := call.Function.Ret.(*typeinfo.Enum); ok {
		return enumType, true
	}
	return nil, false
}

// callParamEnum returns the enum a constrained param rule selects for the
// call's argument at index, such as the window style family of the class a
// CreateWindow call names.
func (p *resolveEnumsProcessor) callParamEnum(call *Call, index int) (*typeinfo.Enum, bool) {
	param := call.Function.Params[index]
	for _, rule := range p.ctx.sdb.EnumRules {
		if rule.Kind != typeinfo.UseParam || len(rule.WhenArgs) == 0 || rule.FuncName != call.Function.Name || rule.ParamName != param.Name {
			continue
		}
		if !p.callMatchesRule(call, rule) {
			continue
		}
		enumType := p.ctx.sdb.GetEnum(rule.EnumName)
		if enumType == nil {
			panic(fmt.Sprintf("resolve-enums: %s %s rule names unknown enum %s", rule.FuncName, rule.ParamName, rule.EnumName))
		}
		return typeinfo.EnumWithStorageSize(enumType, param.Type), true
	}
	return nil, false
}

// retypeCallParamConst gives a constant argument, or the constants OR-ed into
// it, the enum a constrained param rule selects. The rule is more specific
// than the parameter's own enum, so it replaces the family the constant was
// given from the parameter.
func retypeCallParamConst(expr Expr, enumType *typeinfo.Enum) (Expr, bool) {
	switch e := expr.(type) {
	case *Const:
		if e.TypeInfo == enumType {
			return expr, false
		}
		next := *e
		next.TypeInfo = enumType
		return &next, true
	case *Binary:
		if e.Op != OpOr {
			return expr, false
		}
		lhs, lhsChanged := retypeCallParamConst(e.LHS, enumType)
		rhs, rhsChanged := retypeCallParamConst(e.RHS, enumType)
		if !lhsChanged && !rhsChanged {
			return expr, false
		}
		next := *e
		next.LHS = lhs
		next.RHS = rhs
		return &next, true
	default:
		return expr, false
	}
}

// callMatchesRule reports whether a call satisfies an enum use rule.
func (p *resolveEnumsProcessor) callMatchesRule(call *Call, rule *typeinfo.EnumUseRule) bool {
	for _, when := range rule.WhenArgs {
		index, ok := callParamIndex(call.Function, when.ParamName)
		if !ok || index >= len(call.Args) {
			return false
		}
		if !argMatchesConstraint(call.Args[index], when) {
			return false
		}
	}
	return true
}

// argMatchesConstraint reports whether a call argument is the global, C
// string literal or integer constant a constraint requires.
func argMatchesConstraint(arg Expr, when typeinfo.ArgConstraint) bool {
	switch {
	case when.Global != "":
		global, ok := globalOf(arg)
		return ok && global.Name == when.Global
	case when.String != "":
		literal, ok := arg.(*StringLiteral)
		return ok && literal.Text == strconv.Quote(when.String)
	default:
		value, ok := exprConstInt(arg)
		return ok && value == when.Value
	}
}

// callResultType returns the previously resolved type for a call result expression.
func (p *resolveEnumsProcessor) callResultType(result *CallResult) (typeinfo.Type, bool) {
	if result == nil || result.Function == nil {
		return nil, false
	}
	key := callResultKey{instOff: result.InstOff, name: result.Function.Name}
	if typ, ok := p.callTypes[key]; ok {
		return typ, true
	}
	if enumType, ok := result.TypeInfo.(*typeinfo.Enum); ok {
		return enumType, true
	}
	return nil, false
}

// callParamIndex returns the position of a named function parameter.
func callParamIndex(fn *typeinfo.Function, name string) (int, bool) {
	for i, param := range fn.Params {
		if param.Name == name {
			return i, true
		}
	}
	return 0, false
}

// exprConstInt returns the integer value of a semantic constant.
func exprConstInt(expr Expr) (int, bool) {
	c, ok := expr.(*Const)
	if !ok {
		return 0, false
	}
	return int(c.U64), true
}

// exprEnumType returns the enum type of an expression when it has one.
func exprEnumType(expr Expr) (*typeinfo.Enum, bool) {
	if expr == nil || expr.ExprType() == nil {
		return nil, false
	}
	enumType, ok := expr.ExprType().(*typeinfo.Enum)
	return enumType, ok
}
