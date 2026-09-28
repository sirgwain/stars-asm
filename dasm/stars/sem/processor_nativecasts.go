package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// nativeCastsProcessor adds the casts a native Win32 compile needs where a
// value passes into an argument, assignment or return of a type it only
// converted to implicitly under the original compiler. It runs after
// addresses, fields and constants have their final types.
type nativeCastsProcessor struct {
	ctx *FuncContext
}

// ProcessBlock adds native casts to one semantic block, and copies Windows
// struct fields passed by address through temps of the parameter's width.
func (p *nativeCastsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	effects, changed := p.rewriter().rewriteEffects(b.Effects)
	var out []Effect
	for _, effect := range effects {
		if call, ok := effect.(*CallEffect); ok {
			if copied, ok := copyNativeFieldArgs(call); ok {
				out = append(out, copied...)
				changed = true
				continue
			}
		}
		out = append(out, effect)
	}
	if !changed {
		return b, false
	}
	b.Effects = out
	return b, true
}

// copyNativeFieldArgs passes the address of an integer field of a Windows
// struct through a temp of the parameter's pointee type, copying the field
// in before the call and back out after it. Win16 declared fields such as
// RECT.top as 16-bit int, so the original passed &rc.top as an int *; the
// native headers widen them to LONG, which no cast makes a valid int16_t *
// the callee reads and writes through.
func copyNativeFieldArgs(call *CallEffect) ([]Effect, bool) {
	if call.Call == nil || call.Call.Function == nil {
		return nil, false
	}
	var before, after []Effect
	var args []Expr
	for i, arg := range call.Call.Args {
		if i >= len(call.Call.Params) {
			break
		}
		field, elem, ok := nativeFieldAddressArg(arg, call.Call.Params[i].Type)
		if !ok {
			continue
		}
		temp := &Temp{Name: fmt.Sprintf("t_%s_%04x", field.Field.Name, uint16(call.MetaInfo.InstOff)), TypeInfo: elem}
		before = append(before, &Assign{MetaInfo: call.MetaInfo, Dst: temp, Src: field})
		after = append(after, &Assign{MetaInfo: call.MetaInfo, Dst: field, Src: temp})
		if args == nil {
			args = append([]Expr(nil), call.Call.Args...)
		}
		args[i] = &AddressOf{Target: temp, TypeInfo: call.Call.Params[i].Type}
	}
	if args == nil {
		return nil, false
	}
	nextCall := *call.Call
	nextCall.Args = args
	next := *call
	next.Call = &nextCall
	return append(append(before, &next), after...), true
}

// nativeFieldAddressArg matches &s.field for an integer field of a Windows
// struct passed as a pointer to an integer, returning the field and the
// parameter's pointee type.
func nativeFieldAddressArg(arg Expr, param typeinfo.Type) (*FieldAccess, typeinfo.Type, bool) {
	addr, ok := arg.(*AddressOf)
	if !ok {
		return nil, nil, false
	}
	field, ok := addr.Target.(*FieldAccess)
	if !ok || !typeinfo.IsNative(field.Field.Type, typeinfo.NativeInt) {
		return nil, nil, false
	}
	base := field.Base.ExprType()
	if ptr, ok := base.(*typeinfo.Pointer); ok {
		base = ptr.Elem
	}
	strct, ok := base.(*typeinfo.Struct)
	if !ok || !strct.IsExternalWindowsStruct() {
		return nil, nil, false
	}
	ptr, ok := param.(*typeinfo.Pointer)
	if !ok || !typeinfo.IsNative(ptr.Elem, typeinfo.NativeInt) {
		return nil, nil, false
	}
	return field, ptr.Elem, true
}

// rewriter casts assignment sources to their destination's type, return
// values to the function's return type, call arguments to their parameter
// types, and compared pointers to a common type.
func (p *nativeCastsProcessor) rewriter() *semRewriter {
	return &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			switch expr.(type) {
			case *Compare, *Binary:
			default:
				return nil, false, false
			}
			next, changed := w.rewriteExprChildren(expr)
			switch e := next.(type) {
			case *Compare:
				if lhs, rhs, ok := nativeCompareOperands(e.Op, e.LHS, e.RHS); ok {
					cast := *e
					cast.LHS, cast.RHS = lhs, rhs
					return &cast, true, true
				}
			case *Binary:
				if lhs, rhs, ok := nativePointerDifference(e); ok {
					diff := *e
					diff.LHS, diff.RHS = lhs, rhs
					return &diff, true, true
				}
			}
			return next, changed, true
		},
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			next, changed := w.rewriteEffectChildren(effect)
			switch e := next.(type) {
			case *Assign:
				if src := nativeCast(e.Src, e.Dst.ExprType()); src != e.Src {
					cast := *e
					cast.Src = src
					return &cast, true, true
				}
			case *Return:
				if e.Value == nil || p.ctx.fs.Ret == nil {
					break
				}
				if value := nativeReturnCast(e.Value, p.ctx.fs.Ret); value != e.Value {
					cast := *e
					cast.Value = value
					return &cast, true, true
				}
			}
			return next, changed, true
		},
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			next, changed := w.rewriteCallChildren(call)
			if next.Function == nil {
				return next, changed, true
			}
			var args []Expr
			for i, arg := range next.Args {
				if i >= len(next.Params) {
					break
				}
				if cast := nativeArgCast(arg, &next.Params[i]); cast != arg {
					if args == nil {
						args = append([]Expr(nil), next.Args...)
					}
					args[i] = cast
				}
			}
			if args == nil {
				return next, changed, true
			}
			cast := *next
			cast.Args = args
			return &cast, true, true
		},
	}
}
