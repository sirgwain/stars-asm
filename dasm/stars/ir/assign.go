package ir

import (
	"fmt"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// lowerAssign validates storage before lowering reads, and preserves the order
// destination address, source value, containing scalar read, and final write.
func (l *lowerer) lowerAssign(e *sem.Assign) []Stmt {
	base, offset, width, partial, failure := assignmentRange(e.Dst)
	if failure == "" && !partial && (base.ExprType().Kind() == typeinfo.KStruct || base.ExprType().Kind() == typeinfo.KUnion) && !typeinfo.Equals(base.ExprType(), e.Src.ExprType()) {
		failure = "incompatible-aggregate"
	}
	if failure == "" && !writableStorage(base) {
		failure = "invalid-destination"
	}
	if failure != "" {
		comment := untranslatedEffect(e).(*Comment)
		comment.Failures = append(comment.Failures, LowerFailure{Kind: failure, Path: "assign.dst"})
		comment.Text += " (" + failure + ")"
		return []Stmt{comment}
	}
	if stmts, ok := l.splitArrayConstStore(base, e.Src); ok {
		return stmts
	}
	dst, dstOK := l.lowerExpr(base)
	src, srcOK := l.lowerExpr(e.Src)
	if !dstOK || !srcOK {
		return []Stmt{untranslatedEffect(e)}
	}
	if !partial {
		temp, isTemp := e.Dst.(*sem.Temp)
		return []Stmt{&Assign{Dst: dst, Src: src, Merge: e.Merge, Fits: isTemp && sem.ForwardableValue(e.Src, temp.TypeInfo)}}
	}
	typ := base.ExprType()
	if offset == 0 && width == typ.Bytes() {
		return []Stmt{&Assign{Dst: dst, Src: &Cast{Type: typeinfo.TypeDecl(typ, ""), Value: src}}}
	}
	var stmts []Stmt
	switch base.(type) {
	case *sem.Local, *sem.Global, *sem.Temp:
	default:
		address := l.assignmentTemp(&typeinfo.Pointer{Elem: typ})
		stmts = append(stmts, &Assign{Dst: address, Src: addressOf(dst)})
		if raw, ok := dst.(*Deref); ok && raw.Type != nil {
			dst = &Deref{Pointer: address, Type: raw.Type}
		} else {
			dst = &Deref{Pointer: address}
		}
	}
	// A source call may modify the containing scalar, so finish it before the
	// read used to preserve the surrounding bits.
	if expressionCalls(src) {
		value := l.assignmentTemp(e.Src.ExprType())
		stmts = append(stmts, &Assign{Dst: value, Src: src})
		src = value
	}
	stmts = append(stmts, &Assign{Dst: dst, Src: scalarReplacement(dst, src, typ, offset, width)})
	return stmts
}

// assignmentRange combines nested byte and word projections into one scalar
// range, rejecting projections that cross their immediate parent's boundary.
func assignmentRange(expr sem.Expr) (base sem.Expr, offset, width int, partial bool, failure string) {
	var parent sem.Expr
	var off, size int
	switch e := expr.(type) {
	case *sem.Part:
		if slice, ok := scalarArraySlice(e); ok {
			return slice, 0, slice.Width, false, ""
		}
		parent, off, size = e.Base, e.ByteOff, e.Width
	default:
		return expr, 0, expr.ExprType().Bytes(), false, ""
	}
	base, offset, width, _, failure = assignmentRange(parent)
	if failure != "" {
		return base, 0, 0, true, failure
	}
	typ := base.ExprType()
	if typ.Kind() == typeinfo.KPointer {
		return base, 0, 0, true, "pointer-fragment"
	}
	if typ.Kind() == typeinfo.KStruct || typ.Kind() == typeinfo.KUnion || typ.Kind() == typeinfo.KArray {
		return base, 0, 0, true, "aggregate-slice"
	}
	if typ.Kind() != typeinfo.KInt || (typ.Bytes() != 1 && typ.Bytes() != 2 && typ.Bytes() != 4 && typ.Bytes() != 8) {
		return base, 0, 0, true, "unknown-scalar-width"
	}
	if off < 0 || size <= 0 || off > width || size > width-off {
		return base, 0, 0, true, "part"
	}
	switch e := base.(type) {
	case *sem.FieldAccess:
		if e.Field.IsBitfield() {
			return base, 0, 0, true, "bitfield-slice"
		}
	case *sem.SymbolRef:
		if _, ok := e.Path.(*symresolve.SymbolBitfield); ok {
			return base, 0, 0, true, "bitfield-slice"
		}
	}
	return base, offset + off, size, true, ""
}

// scalarArraySlice reinterprets a byte range of an integer array, such as the
// dword at the start of a uint8_t record buffer, as a dereference of the array.
// Deref lowering then emits element indexing when the range is one aligned
// element, and a raw typed access otherwise.
func scalarArraySlice(e *sem.Part) (*sem.Deref, bool) {
	array, ok := e.Base.ExprType().(*typeinfo.Array)
	if !ok || array.Elem.Kind() != typeinfo.KInt {
		return nil, false
	}
	if e.Width != 1 && e.Width != 2 && e.Width != 4 || e.ByteOff < 0 || e.ByteOff+e.Width > array.Bytes() {
		return nil, false
	}
	return &sem.Deref{Pointer: e.Base, ByteOff: e.ByteOff, Width: e.Width, TypeInfo: typeinfo.UintForWidth(e.Width)}, true
}

// splitArrayConstStore splits a constant stored across several aligned
// elements of an integer array into one store per element, recovering the
// source's adjacent element assignments that were merged into one wide store.
func (l *lowerer) splitArrayConstStore(dst sem.Expr, src sem.Expr) ([]Stmt, bool) {
	deref, ok := dst.(*sem.Deref)
	if !ok {
		return nil, false
	}
	value, ok := src.(*sem.Const)
	if !ok {
		return nil, false
	}
	array, ok := deref.Pointer.ExprType().(*typeinfo.Array)
	if !ok {
		return nil, false
	}
	size := array.Elem.Bytes()
	if size <= 0 || deref.Width <= size || deref.ByteOff%size != 0 || deref.Width%size != 0 {
		return nil, false
	}
	base, ok := l.lowerExpr(deref.Pointer)
	if !ok {
		return nil, false
	}
	mask := ^uint64(0) >> (64 - size*8)
	var stmts []Stmt
	for i := 0; i < deref.Width/size; i++ {
		index := deref.ByteOff/size + i
		elem, _ := l.lowerExpr(&sem.Const{TypeInfo: array.Elem, U64: (value.U64 >> (i * size * 8)) & mask})
		stmts = append(stmts, &Assign{
			Dst: &Index{Base: base, Index: &IntConst{Value: uint64(index), Text: fmt.Sprint(index)}},
			Src: elem,
		})
	}
	return stmts, true
}

// writableStorage accepts only semantic objects that can be C lvalues.
func writableStorage(expr sem.Expr) bool {
	switch e := expr.(type) {
	case *sem.Local, *sem.Global, *sem.Temp, *sem.Deref:
		return expr.ExprType().Kind() != typeinfo.KArray
	case *sem.ArrayIndex:
		return e.ExprType().Kind() != typeinfo.KArray
	case *sem.FieldAccess:
		_, pointer := typeinfo.UnwrapPointer(e.Base.ExprType())
		return e.ExprType().Kind() != typeinfo.KArray && (pointer || writableStorage(e.Base))
	case *sem.SymbolRef:
		return e.ExprType().Kind() != typeinfo.KArray && writableSymbol(e.Path)
	}
	return false
}

// writableSymbol rejects unresolved offsets and literal symbolic paths.
func writableSymbol(path symresolve.SymbolPath) bool {
	switch p := path.(type) {
	case *symresolve.SymbolRoot, *symresolve.SymbolScratch, *symresolve.SymbolDeref, *symresolve.SymbolTerm:
		return true
	case *symresolve.SymbolField:
		return writableSymbol(p.Base)
	case *symresolve.SymbolBitfield:
		return writableSymbol(p.Base)
	}
	return false
}

// assignmentTemp declares a temporary whose name cannot shadow input symbols.
func (l *lowerer) assignmentTemp(typ typeinfo.Type) *Var {
	for {
		l.nextTemp++
		name := fmt.Sprintf("t_assign_%d", l.nextTemp)
		if strings.Contains(l.names, name) || l.tempSeen[name] {
			continue
		}
		l.tempSeen[name] = true
		l.temps = append(l.temps, Local{Name: name, Type: typ})
		return &Var{Name: name}
	}
}

// addressOf returns the address of a lowered lvalue. A dereference folds to
// its pointer, displaced by raw bytes for a raw access.
func addressOf(target Expr) Expr {
	deref, ok := target.(*Deref)
	if !ok {
		return &AddressOf{Target: target}
	}
	if deref.Type == nil {
		return deref.Pointer
	}
	return rawAddress(deref.Pointer, deref.ByteOff, &typeinfo.Pointer{Elem: deref.Type})
}

// rawAddress displaces a pointer by raw bytes and converts the result to typ,
// leaving a byte pointer when typ is nil.
func rawAddress(pointer Expr, byteOff int, typ typeinfo.Type) Expr {
	if byteOff == 0 {
		if typ == nil {
			return pointer
		}
		return &Cast{Type: typeinfo.TypeDecl(typ, ""), Value: pointer}
	}
	return &PointerOffset{Pointer: pointer, Offset: &IntConst{Value: uint64(byteOff), Text: fmt.Sprint(byteOff)}, Type: typ}
}

// scalarReplacement replaces a byte range using unsigned containing-width
// arithmetic and converts the result back to the destination's declared type.
func scalarReplacement(parent, value Expr, typ typeinfo.Type, offset, width int) Expr {
	bits := typ.Bytes() * 8
	unsigned := typeinfo.TypeDecl(typeinfo.UintForWidth(typ.Bytes()), "")
	mask := ^uint64(0) >> (64 - width*8)
	keep := (^uint64(0) >> (64 - bits)) & ^(mask << (offset * 8))
	insert := Expr(&Binary{Op: "&", LHS: &Cast{Type: unsigned, Value: value}, RHS: &IntConst{Value: mask}})
	if offset != 0 {
		insert = &Binary{Op: "<<", LHS: insert, RHS: &IntConst{Value: uint64(offset * 8)}}
	}
	result := Expr(&Binary{Op: "|", LHS: &Binary{Op: "&", LHS: &Cast{Type: unsigned, Value: parent}, RHS: &IntConst{Value: keep}}, RHS: insert})
	if p, ok := typ.(*typeinfo.Primitive); ok && p.Signed {
		result = &Cast{Type: typeinfo.TypeDecl(typ, ""), Value: result}
	}
	return result
}

// expressionCalls reports whether evaluating an IR expression can call code.
func expressionCalls(expr Expr) bool {
	switch e := expr.(type) {
	case *Call:
		return true
	case *Binary:
		return expressionCalls(e.LHS) || expressionCalls(e.RHS)
	case *Unary:
		return expressionCalls(e.X)
	case *Cond:
		return expressionCalls(e.Cond) || expressionCalls(e.Then) || expressionCalls(e.Else)
	case *Cast:
		return expressionCalls(e.Value)
	case *Index:
		return expressionCalls(e.Base) || expressionCalls(e.Index)
	case *Field:
		return expressionCalls(e.Base)
	case *Deref:
		return expressionCalls(e.Pointer)
	case *AddressOf:
		return expressionCalls(e.Target)
	case *PointerOffset:
		return expressionCalls(e.Pointer) || expressionCalls(e.Offset)
	case *Macro:
		for _, arg := range e.Args {
			if expressionCalls(arg) {
				return true
			}
		}
	}
	return false
}
