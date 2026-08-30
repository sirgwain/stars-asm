package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type resolveEnumsProcessor struct {
	ctx       *FuncContext
	callTypes map[callResultKey]typeinfo.Type
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
			if enumType, isEnum := paramType.(*typeinfo.Enum); isEnum {
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

// resolveExpectedEnum applies an expected enum type to a compatible expression.
func (p *resolveEnumsProcessor) resolveExpectedEnum(expr Expr, enumType *typeinfo.Enum) (Expr, bool) {
	switch e := expr.(type) {
	case *Const:
		if e.TypeInfo == enumType {
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
	default:
		return expr, false
	}
}

// expectedEnumType returns the static or path-sensitive enum type for an expression.
func (p *resolveEnumsProcessor) expectedEnumType(expr Expr) (*typeinfo.Enum, bool) {
	if enumType, ok := exprEnumType(expr); ok {
		return enumType, true
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
		return enumType, true
	}
	if enumType, ok := call.Function.Ret.(*typeinfo.Enum); ok {
		return enumType, true
	}
	return nil, false
}

// callMatchesRule reports whether a call satisfies an enum use rule.
func (p *resolveEnumsProcessor) callMatchesRule(call *Call, rule *typeinfo.EnumUseRule) bool {
	for _, when := range rule.WhenArgs {
		index, ok := callParamIndex(call.Function, when.ParamName)
		if !ok || index >= len(call.Args) {
			return false
		}
		value, ok := exprConstInt(call.Args[index])
		if !ok || value != when.Value {
			return false
		}
	}
	return true
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
