package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// Stars' points are POINT16, which keeps the Win16 layout its records store;
// the Win32 API takes POINT, whose fields are LONG.
const (
	win16PointTypedef  = "POINT16"
	nativePointTypedef = "POINT"
)

// outputPointFuncs are the Win32 functions whose POINT * parameter only
// receives a point, so the caller's point is not read before the call.
var outputPointFuncs = map[string]bool{
	"GetCaretPos":  true,
	"GetCursorPos": true,
}

// nativePointsProcessor converts points where they cross between Stars and
// the Win32 API, which C does not do implicitly between the two structs:
//
//   - a POINT16 passed as a POINT argument becomes PointFrom16(pt).
//   - the address of a POINT16 passed as a POINT * argument, such as
//     ScreenToClient(hwnd, &pt), goes through a POINT temp that is filled
//     before the call and copied back after it. A function that only writes
//     the point, such as GetCursorPos, leaves the temp unfilled.
//   - a RECT corner passed as a Stars POINT16 *, as in
//     LogicalToScan((POINT *)&rc.right), goes through a POINT16 temp
//     holding copies of the corner's fields.
//   - an assignment between a POINT16 and a POINT converts the source.
type nativePointsProcessor struct{}

// ProcessBlock converts points crossing the Win32 boundary in one block.
func (p *nativePointsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	rewriter := &semRewriter{
		call: func(w *semRewriter, call *Call, _ machine.Meta) (*Call, bool, bool) {
			next, changed := w.rewriteCallChildren(call)
			var args []Expr
			for i, arg := range next.Args {
				if i >= len(next.Params) {
					break
				}
				converted, ok := convertPoint(arg, next.Params[i].Type)
				if !ok {
					continue
				}
				if args == nil {
					args = append([]Expr(nil), next.Args...)
				}
				args[i] = converted
			}
			if args == nil {
				return next, changed, true
			}
			converted := *next
			converted.Args = args
			return &converted, true, true
		},
	}
	effects, changed := rewriter.rewriteEffects(b.Effects)
	var out []Effect
	for _, effect := range effects {
		switch e := effect.(type) {
		case *Assign:
			if src, ok := convertPoint(e.Src, e.Dst.ExprType()); ok {
				next := *e
				next.Src = src
				out = append(out, &next)
				changed = true
				continue
			}
		case *CallEffect:
			if copied, ok := copyPointArgs(e); ok {
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

// copyPointArgs passes the address of each POINT16 given to a POINT *
// parameter through a POINT temp, converting the point into the temp before
// the call and back out of it after, since the callee may read and write it.
// A callee in outputPointFuncs only writes it, so the point is not converted
// in: it may not be initialized yet.
func copyPointArgs(call *CallEffect) ([]Effect, bool) {
	if call.Call == nil || call.Call.Function == nil {
		return nil, false
	}
	var before, after []Effect
	var args []Expr
	for i, arg := range call.Call.Args {
		if i >= len(call.Call.Params) {
			break
		}
		param, ok := call.Call.Params[i].Type.(*typeinfo.Pointer)
		if !ok {
			continue
		}
		// Call argument conversion already cast &pt to the parameter type.
		if cast, ok := arg.(*Cast); ok {
			arg = cast.Value
		}
		addr, ok := arg.(*AddressOf)
		if !ok {
			continue
		}
		name := fmt.Sprintf("t_pt_%04x", uint16(call.MetaInfo.InstOff))
		if i > 0 {
			name = fmt.Sprintf("t_pt_%04x_%d", uint16(call.MetaInfo.InstOff), i)
		}
		temp := &Temp{Name: name, TypeInfo: param.Elem}
		switch {
		case isPointStruct(param.Elem, nativePointTypedef) && isPointStruct(addr.Target.ExprType(), win16PointTypedef):
			if !outputPointFuncs[call.Call.Function.Name] {
				before = append(before, &Assign{MetaInfo: call.MetaInfo, Dst: temp, Src: pointConversion("PointFrom16", addr.Target, param.Elem)})
			}
			after = append(after, &Assign{MetaInfo: call.MetaInfo, Dst: addr.Target, Src: pointConversion("PointTo16", temp, addr.Target.ExprType())})
		case isPointStruct(param.Elem, win16PointTypedef):
			// Win16 code treats a RECT as its top-left and bottom-right
			// points; the native RECT holds LONGs, so copy the corner.
			corner, ok := rectCornerFields(addr.Target)
			if !ok {
				continue
			}
			point := param.Elem.(*typeinfo.Struct)
			for j, field := range corner {
				tempField := &FieldAccess{Base: temp, Field: &point.Fields[j]}
				before = append(before, &Assign{MetaInfo: call.MetaInfo, Dst: tempField, Src: field})
				after = append(after, &Assign{MetaInfo: call.MetaInfo, Dst: field, Src: tempField})
			}
		default:
			continue
		}
		if args == nil {
			args = append([]Expr(nil), call.Call.Args...)
		}
		args[i] = &AddressOf{Target: temp, TypeInfo: param}
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

// rectCornerFields returns the x and y fields of the RECT corner target
// addresses: left and top for the RECT or its left field, right and bottom
// for its right field.
func rectCornerFields(target LValue) ([2]*FieldAccess, bool) {
	var base Expr = target
	first := "left"
	switch t := target.(type) {
	case *SymbolRef:
		if field, ok := t.Path.(*symresolve.SymbolField); ok && (field.Field.Name == "left" || field.Field.Name == "right") {
			base = &SymbolRef{Path: field.Base}
			first = field.Field.Name
		}
	case *FieldAccess:
		if t.Field.Name == "left" || t.Field.Name == "right" {
			base = t.Base
			first = t.Field.Name
		}
	}
	rect, ok := base.ExprType().(*typeinfo.Struct)
	if !ok || rect.Typedef != "RECT" {
		return [2]*FieldAccess{}, false
	}
	second := "top"
	if first == "right" {
		second = "bottom"
	}
	var fields [2]*FieldAccess
	for i, name := range []string{first, second} {
		for j := range rect.Fields {
			if rect.Fields[j].Name == name {
				fields[i] = &FieldAccess{Base: base, Field: &rect.Fields[j]}
			}
		}
		if fields[i] == nil {
			return [2]*FieldAccess{}, false
		}
	}
	return fields, true
}

// convertPoint converts a point value of one point struct to want, the
// other one.
func convertPoint(expr Expr, want typeinfo.Type) (Expr, bool) {
	have := cExprType(expr)
	switch {
	case isPointStruct(have, win16PointTypedef) && isPointStruct(want, nativePointTypedef):
		return pointConversion("PointFrom16", expr, want), true
	case isPointStruct(have, nativePointTypedef) && isPointStruct(want, win16PointTypedef):
		return pointConversion("PointTo16", expr, want), true
	}
	return nil, false
}

// pointConversion calls the win16defines.h helper that converts pt to ret.
func pointConversion(name string, pt Expr, ret typeinfo.Type) *Call {
	fn := &typeinfo.Function{
		Name:   name,
		Ret:    ret,
		Params: []typeinfo.FunctionVar{{Name: "pt", Type: pt.ExprType()}},
	}
	return &Call{Function: fn, Args: []Expr{pt}}
}

// isPointStruct reports whether typ is the point struct with typedef name.
func isPointStruct(typ typeinfo.Type, name string) bool {
	s, ok := typ.(*typeinfo.Struct)
	return ok && s.Typedef == name
}
