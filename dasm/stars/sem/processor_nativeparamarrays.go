package sem

import (
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// nativeParamArraysProcessor gives a native copy of the Win16 stack to code
// that walks its parameters through a pointer. The original pushed 16-bit
// arguments next to each other, so PackageUpMsg reads p1..p7 with
//
//	pi = &p1; ... pi = &pi[1]
//
// Native arguments are not laid out that way: each x64 stack argument has its
// own 8-byte slot, so &p1 + 1 reads the upper bytes of p1. Where a parameter's
// address is stored to a pointer that is then indexed or advanced, the
// parameters from that one on are copied into a local array there, and the
// pointer walks the array instead.
type nativeParamArraysProcessor struct {
	ctx *FuncContext
}

// ProcessFunc rewrites parameter walks throughout one function.
func (p *nativeParamArraysProcessor) ProcessFunc(result *Result, f *Func) bool {
	walked := walkedPointerLocals(f)
	if len(walked) == 0 {
		return false
	}
	changed := false
	arrays := map[string]*Local{}
	for bi := range f.Blocks {
		var effects []Effect
		for _, effect := range f.Blocks[bi].Effects {
			assign, ok := effect.(*Assign)
			if !ok {
				effects = append(effects, effect)
				continue
			}
			dst, dstOK := assign.Dst.(*Local)
			addr, addrOK := assign.Src.(*AddressOf)
			if !dstOK || !addrOK || !walked[dst.Name] {
				effects = append(effects, effect)
				continue
			}
			param, ok := addr.Target.(*Local)
			if !ok {
				effects = append(effects, effect)
				continue
			}
			params, ok := p.walkedParams(param.Name, dst.ExprType())
			if !ok {
				effects = append(effects, effect)
				continue
			}
			array, ok := arrays[param.Name]
			if !ok {
				name := "rgArgs"
				if len(arrays) > 0 {
					name += "_" + param.Name
				}
				array = &Local{FunctionVar: typeinfo.FunctionVar{
					Name: name,
					Type: &typeinfo.Array{Elem: param.Type, Count: len(params)},
				}}
				arrays[param.Name] = array
				f.RecoveredLocals = append(f.RecoveredLocals, &array.FunctionVar)
			}
			for i, v := range params {
				effects = append(effects, &Assign{
					MetaInfo: assign.MetaInfo,
					Dst:      paramArrayElem(array, i),
					Src:      &Local{FunctionVar: v},
				})
			}
			next := *assign
			next.Src = &AddressOf{Target: paramArrayElem(array, 0), TypeInfo: addr.TypeInfo}
			effects = append(effects, &next)
			changed = true
		}
		f.Blocks[bi].Effects = effects
	}
	return changed
}

// walkedParams returns the stack parameters a pointer of type ptr reads when
// it starts at the parameter named name and walks forward: that parameter and
// the ones after it, while they are the pointee's size. It returns false for
// a register parameter or one without a following stack parameter to walk to.
func (p *nativeParamArraysProcessor) walkedParams(name string, ptr typeinfo.Type) ([]typeinfo.FunctionVar, bool) {
	pointer, ok := ptr.(*typeinfo.Pointer)
	if !ok {
		return nil, false
	}
	var params []typeinfo.FunctionVar
	for _, v := range p.ctx.fs.Params {
		if len(params) == 0 && v.Name != name {
			continue
		}
		if v.BPOffset <= 0 || v.Type.Bytes() != pointer.Elem.Bytes() {
			break
		}
		params = append(params, v)
	}
	return params, len(params) > 1
}

// paramArrayElem returns element i of a parameter copy array.
func paramArrayElem(array *Local, i int) *ArrayIndex {
	elem := array.Type.(*typeinfo.Array).Elem
	return &ArrayIndex{Base: array, Index: &Const{TypeInfo: typeinfo.I16, U64: uint64(i)}, TypeInfo: elem}
}

// walkedPointerLocals returns the names of the locals that are indexed or
// advanced as pointers anywhere in f, such as pi in pi = &pi[1].
func walkedPointerLocals(f *Func) map[string]bool {
	walked := map[string]bool{}
	for _, b := range f.Blocks {
		for _, effect := range b.Effects {
			walkEffect(effect, func(expr Expr) {
				var base Expr
				switch e := expr.(type) {
				case *ArrayIndex:
					if c, ok := e.Index.(*Const); ok && c.U64 == 0 {
						return
					}
					base = e.Base
				case *PointerOffset:
					base = e.Pointer
				default:
					return
				}
				if local, ok := base.(*Local); ok && typeinfo.IsPointer(local.Type) {
					walked[local.Name] = true
				}
			})
		}
	}
	return walked
}
