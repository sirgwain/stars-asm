package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// nativeRecordsProcessor keeps file records in their Win16 layout for
// structs whose only native difference is the pointers they start with,
// such as MSGPLR, which links messages through lpmsgplrNext. Stars reads and
// writes such a struct's bytes as a file record, and the pointer that opens
// it is 4 bytes in the record but 8 natively, shifting every later field.
//
// The bytes from 4 before the first field after the pointers, its Win16
// offset, hold the record natively: the pointer's upper half, then fields
// laid out as in Win16. So:
//
//   - a record argument, such as WriteRt's rg, and a record struct copied
//     to or from bytes, as in fmemcpy(lpmp, rgbCur, hdrCur.cb), point at
//     (uint8_t *)&p->iPlrFrom - 4 instead of p.
//   - an allocation of a record struct, sized as its record, grows by
//     sizeof(T) less the Win16 size.
//
// The pointer bytes a record holds were never meaningful: Stars clears the
// link after reading a record.
type nativeRecordsProcessor struct{}

// ProcessFunc rewrites record accesses throughout one function.
func (p *nativeRecordsProcessor) ProcessFunc(result *Result, f *Func) bool {
	tempTargets := callResultTempTargets(f)
	rewriter := &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			var call *Call
			var dst Expr
			switch e := effect.(type) {
			case *Assign:
				call, _ = e.Src.(*Call)
				dst = e.Dst
			case *CallEffect:
				call, dst = e.Call, e.Result
			}
			if call == nil || dst == nil {
				return nil, false, false
			}
			target, ok := allocationTarget(dst, tempTargets)
			if !ok {
				return nil, false, false
			}
			sized, ok := recordAllocation(call, target)
			if !ok {
				return nil, false, false
			}
			var next Effect
			switch e := effect.(type) {
			case *Assign:
				assign := *e
				assign.Src = sized
				next = &assign
			case *CallEffect:
				callEffect := *e
				callEffect.Call = sized
				next = &callEffect
			}
			rewritten, _ := w.rewriteEffectChildren(next)
			return rewritten, true, true
		},
		call: func(w *semRewriter, call *Call, _ machine.Meta) (*Call, bool, bool) {
			next, changed := w.rewriteCallChildren(call)
			if rewritten, ok := recordArgs(next); ok {
				return rewritten, true, true
			}
			return next, changed, true
		},
	}
	changed := false
	for i := range f.Blocks {
		effects, blockChanged := rewriter.rewriteEffects(f.Blocks[i].Effects)
		if blockChanged {
			f.Blocks[i].Effects = effects
			changed = true
		}
	}
	return changed
}

// recordAllocation grows an allocation of a record struct, target, by the
// native struct's extra size: its alloc_size becomes
// size + (sizeof(T) - Win16 size).
func recordAllocation(call *Call, target typeinfo.Type) (*Call, bool) {
	s, ok := target.(*typeinfo.Struct)
	if !ok || call.Function == nil {
		return nil, false
	}
	if _, ok := recordFirstField(s); !ok {
		return nil, false
	}
	for i, param := range call.Params {
		if param.Semantic != typeinfo.ParamSemanticAllocSize || i >= len(call.Args) {
			continue
		}
		extra := &Binary{
			TypeInfo: typeinfo.U16,
			Op:       OpSub,
			LHS:      &SizeOf{Type: s},
			RHS:      &Const{TypeInfo: typeinfo.I16, U64: uint64(s.Size)},
		}
		next := *call
		next.Args = append([]Expr(nil), call.Args...)
		next.Args[i] = &Binary{TypeInfo: call.Args[i].ExprType(), Op: OpAdd, LHS: call.Args[i], RHS: extra}
		return &next, true
	}
	return nil, false
}

// recordArgs points record arguments of a record struct at its record: a
// record parameter's argument, and a record struct copied to or from a byte
// buffer by a call with a byte_count.
func recordArgs(call *Call) (*Call, bool) {
	if call.Function == nil {
		return nil, false
	}
	copies := false
	for _, param := range call.Params {
		if param.Semantic == typeinfo.ParamSemanticByteCount {
			copies = true
		}
	}
	byteBuffer := -1
	if copies {
		for i, arg := range call.Args {
			if i < len(call.Params) && isByteBuffer(cExprType(arg)) {
				byteBuffer = i
			}
		}
	}
	var args []Expr
	for i, arg := range call.Args {
		if i >= len(call.Params) {
			break
		}
		if call.Params[i].Semantic != typeinfo.ParamSemanticRecord && (byteBuffer < 0 || i == byteBuffer) {
			continue
		}
		record, ok := recordPointer(arg)
		if !ok {
			continue
		}
		if args == nil {
			args = append([]Expr(nil), call.Args...)
		}
		args[i] = record
	}
	if args == nil {
		return nil, false
	}
	next := *call
	next.Args = args
	return &next, true
}

// recordPointer returns the address of the record a pointer to a record
// struct holds natively: its first field after the leading pointers, less
// that field's Win16 offset.
func recordPointer(ptr Expr) (Expr, bool) {
	// Call argument conversion may already have cast the pointer to void *.
	if cast, ok := ptr.(*Cast); ok {
		ptr = cast.Value
	}
	s, ok := structPointee(cExprType(ptr))
	if !ok {
		return nil, false
	}
	field, ok := recordFirstField(s)
	if !ok {
		return nil, false
	}
	bytePtr := &typeinfo.Pointer{Elem: typeinfo.U8}
	return &PointerOffset{
		Pointer:  &AddressOf{Target: &FieldAccess{Base: ptr, Field: field}, TypeInfo: &typeinfo.Pointer{Elem: field.Type}},
		Offset:   &Const{TypeInfo: typeinfo.I16, U64: uint64(uint16(-field.Offset))},
		TypeInfo: bytePtr,
	}, true
}

// recordFirstField returns the first field after the pointers a record
// struct starts with. A record struct starts with one or more pointers and
// all its other fields have their Win16 layout natively.
func recordFirstField(s *typeinfo.Struct) (*typeinfo.StructField, bool) {
	if s.SKind != typeinfo.StructKindStruct || s.IsExternalWindowsStruct() {
		return nil, false
	}
	first := -1
	for i := range s.Fields {
		field := &s.Fields[i]
		if first < 0 {
			if isNativePointerType(field.Type) {
				continue
			}
			if i == 0 {
				return nil, false
			}
			first = i
		}
		if !nativeLayoutStable(field.Type) {
			return nil, false
		}
	}
	if first < 0 {
		return nil, false
	}
	return &s.Fields[first], true
}

// nativeLayoutStable reports whether typ has its Win16 size and layout
// natively: no pointers, handles or Windows structs, which widen.
func nativeLayoutStable(typ typeinfo.Type) bool {
	switch t := typ.(type) {
	case *typeinfo.Primitive:
		return t.Native == typeinfo.NativeInt
	case *typeinfo.Enum:
		return true
	case *typeinfo.Array:
		return nativeLayoutStable(t.Elem)
	case *typeinfo.Struct:
		if t.IsExternalWindowsStruct() {
			return false
		}
		for _, field := range t.Fields {
			if !nativeLayoutStable(field.Type) {
				return false
			}
		}
		return true
	}
	return false
}

// isNativePointerType reports whether typ is a pointer or a handle, which
// is a pointer natively.
func isNativePointerType(typ typeinfo.Type) bool {
	if _, ok := typ.(*typeinfo.Pointer); ok {
		return true
	}
	return typeinfo.IsNative(typ, typeinfo.NativePointer)
}

// isByteBuffer reports whether typ is a pointer to, or an array of, bytes.
func isByteBuffer(typ typeinfo.Type) bool {
	switch t := typ.(type) {
	case *typeinfo.Pointer:
		return t.Elem.Kind() != typeinfo.KVoid && t.Elem.Bytes() == 1
	case *typeinfo.Array:
		return t.Elem.Bytes() == 1
	}
	return false
}
