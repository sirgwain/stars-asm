package ir

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type lowerer struct {
	fn       *typeinfo.Function
	cfg      *machine.CFG
	temps    []Local
	tempSeen map[string]bool
}

// Lower converts post-processed semantic IR into low-level C-like IR.
// Unsupported semantic constructs are preserved as comment statements rather
// than guessed at, so the dump remains conservative.
func Lower(src sem.Func, fn *typeinfo.Function) Func {
	l := &lowerer{fn: fn, cfg: src.CFG, tempSeen: make(map[string]bool)}
	out := Func{Name: fn.Name, Decl: fn.CDecl()}

	for _, v := range fn.Vars {
		out.Locals = append(out.Locals, Local{Name: v.Name, Type: v.Type})
	}

	for _, b := range src.Blocks {
		block := Block{ID: b.ID, Label: b.ID.String(), StartOff: uint32(b.ID)}
		if src.CFG != nil {
			if cfgBlock := src.CFG.Block(b.ID); cfgBlock != nil {
				block.Label = cfgBlock.Label
				block.EndOff = cfgBlock.EndOff
			}
		}
		for _, effect := range b.Effects {
			block.Stmts = append(block.Stmts, l.lowerEffect(effect))
		}
		out.Blocks = append(out.Blocks, block)
	}
	out.Locals = append(out.Locals, l.temps...)
	return out
}

func (l *lowerer) lowerEffect(effect sem.Effect) Stmt {
	switch e := effect.(type) {
	case *sem.Assign:
		dst, ok1 := l.lowerExpr(e.Dst)
		src, ok2 := l.lowerExpr(e.Src)
		if ok1 && ok2 {
			return &Assign{Dst: dst, Src: src}
		}
	case *sem.CallEffect:
		call, ok := l.lowerExpr(e.Call)
		if ok {
			if e.Result == nil {
				return &ExprStmt{Expr: call}
			}
			if _, unused := e.Result.(*sem.CallResult); unused {
				return &ExprStmt{Expr: call}
			}
			result, rok := l.lowerExpr(e.Result)
			if rok {
				return &Assign{Dst: result, Src: call}
			}
		}
	case *sem.Branch:
		cond, ok := l.lowerExpr(e.Cond)
		if ok {
			return &IfGoto{Cond: cond, TrueLabel: l.blockLabel(e.TrueBlock), FalseLabel: l.blockLabel(e.FalseBlock)}
		}
	case *sem.Jump:
		return &Goto{Label: l.blockLabel(e.To)}
	case *sem.Return:
		if e.Value == nil {
			return &Return{}
		}
		value, ok := l.lowerExpr(e.Value)
		if ok {
			return &Return{Value: value}
		}
	case *sem.RawEffect:
		return &Comment{Text: "untranslated: " + sem.FormatEffect(effect)}
	}
	return &Comment{Text: "untranslated: " + sem.FormatEffect(effect)}
}

// blockLabel returns the rendered label for a CFG block ID.
func (l *lowerer) blockLabel(id machine.BlockID) string {
	if l.cfg != nil {
		if block := l.cfg.Block(id); block != nil {
			return block.Label
		}
	}
	return id.String()
}

func (l *lowerer) lowerExpr(expr sem.Expr) (Expr, bool) {
	switch e := expr.(type) {
	case nil:
		return nil, false
	case *sem.Local:
		return &Var{Name: e.Name}, true
	case *sem.Global:
		return &Var{Name: e.Name}, true
	case *sem.FunctionRef:
		return &Var{Name: e.Name}, true
	case *sem.Temp:
		l.addTemp(e)
		return &Var{Name: e.Name}, true
	case *sem.Const:
		text := ""
		if formatted := sem.FormatExpr(e); formatted != fmt.Sprintf("0x%x", e.U64) {
			text = formatted
		}
		return &IntConst{Value: e.U64, Text: text}, true
	case *sem.StringLiteral:
		return &StringConst{Value: e.Text}, true
	case *sem.ResourceID:
		v, ok := l.lowerExpr(e.Value)
		return &Macro{Name: "MAKEINTRESOURCE", Args: []Expr{v}}, ok
	case *sem.FloatConst:
		return &FloatConst{Value: e.F64}, true
	case *sem.Unary:
		x, ok := l.lowerExpr(e.X)
		if !ok {
			return nil, false
		}
		op, functional, ok := lowerUnaryOp(e.Op)
		if !ok {
			return nil, false
		}
		return &Unary{Op: op, X: x, Functional: functional}, true
	case *sem.Binary:
		lhs, ok1 := l.lowerExpr(e.LHS)
		rhs, ok2 := l.lowerExpr(e.RHS)
		op, ok3 := lowerBinaryOp(e.Op)
		if !ok1 || !ok2 || !ok3 {
			return nil, false
		}
		return &Binary{Op: op, LHS: lhs, RHS: rhs}, true
	case *sem.Byte:
		parent, ok := l.lowerExpr(e.Parent)
		if !ok {
			return nil, false
		}
		name := "LOBYTE"
		if e.Part == machine.ByteHigh {
			name = "HIBYTE"
		}
		if e.Value == nil {
			return &Macro{Name: name, Args: []Expr{parent}}, true
		}
		return nil, false
	case *sem.Cast:
		v, ok := l.lowerExpr(e.Value)
		if !ok {
			return nil, false
		}
		return &Cast{Type: e.To, Value: v}, true
	case *sem.ArrayIndex:
		base, ok1 := l.lowerExpr(e.Base)
		idx, ok2 := l.lowerExpr(e.Index)
		if !ok1 || !ok2 {
			return nil, false
		}
		return &Index{Base: base, Index: idx}, true
	case *sem.FieldAccess:
		base, ok := l.lowerExpr(e.Base)
		if !ok {
			return nil, false
		}
		_, ptr := typeinfo.UnwrapPointer(e.Base.ExprType())
		return &Field{Base: base, Name: e.Field.Name, Pointer: ptr}, true
	case *sem.SymbolRef:
		return &Var{Name: e.Path.CDecl()}, true
	case *sem.Compare:
		lhs, ok1 := l.lowerExpr(e.LHS)
		rhs, ok2 := l.lowerExpr(e.RHS)
		op, ok3 := lowerCompareOp(e.Op)
		if !ok1 || !ok2 || !ok3 {
			return nil, false
		}
		return &Binary{Op: op, LHS: lhs, RHS: rhs}, true
	case *sem.SignExtend:
		v, ok := l.lowerExpr(e.Parent)
		if !ok {
			return nil, false
		}
		return &Cast{Type: typeinfo.TypeDecl(e.ExprType(), ""), Value: v}, true
	case *sem.Call:
		var target Expr
		var ok bool
		if e.Target != nil {
			target, ok = l.lowerExpr(e.Target)
		} else if e.Function != nil {
			target, ok = &Var{Name: e.Function.Name}, true
		}
		if !ok {
			return nil, false
		}
		args := make([]Expr, 0, len(e.Args))
		for _, a := range e.Args {
			arg, aok := l.lowerExpr(a)
			if !aok {
				return nil, false
			}
			args = append(args, arg)
		}
		return &Call{Target: target, Args: args}, true
	case *sem.Word:
		parent, ok := l.lowerExpr(e.Parent)
		if !ok {
			return nil, false
		}
		name := "LOWORD"
		switch e.Part {
		case machine.WordHigh:
			name = "HIWORD"
		case machine.WordSignHigh:
			name = "SIGNHIWORD"
		}
		return &Macro{Name: name, Args: []Expr{parent}}, true
	case *sem.FarPointer:
		if e.Part == machine.FarPointerWhole {
			seg, ok1 := l.lowerExpr(e.Segment)
			off, ok2 := l.lowerExpr(e.Offset)
			if !ok1 || !ok2 {
				return nil, false
			}
			return &Macro{Name: "farptr", Args: []Expr{seg, off}}, true
		}
		parent, ok := l.lowerExpr(e.Parent)
		if !ok {
			return nil, false
		}
		name := "FAROFF"
		if e.Part == machine.FarPointerSegment {
			name = "FARSEG"
		}
		return &Macro{Name: name, Args: []Expr{parent}}, true
	case *sem.PointerOffset:
		ptr, ok1 := l.lowerExpr(e.Pointer)
		off, ok2 := l.lowerExpr(e.Offset)
		if !ok1 || !ok2 {
			return nil, false
		}
		return &PointerOffset{Pointer: ptr, Offset: off}, true
	case *sem.Deref:
		ptr, ok := l.lowerExpr(e.Pointer)
		if !ok {
			return nil, false
		}
		return &Deref{Pointer: ptr, ByteOff: e.ByteOff}, true
	case *sem.AddressOf:
		if base, ok := addressOfZeroIndexArrayBase(e); ok {
			return l.lowerExpr(base)
		}
		target, ok := l.lowerExpr(e.Target)
		if !ok {
			return nil, false
		}
		return &AddressOf{Target: target}, true
	case *sem.Part:
		base, ok := l.lowerExpr(e.Base)
		if !ok {
			return nil, false
		}
		if e.Width == 2 && e.ByteOff == 0 {
			return &Macro{Name: "LOWORD", Args: []Expr{base}}, true
		}
		if e.Width == 2 && e.ByteOff == 2 {
			return &Macro{Name: "HIWORD", Args: []Expr{base}}, true
		}
		return nil, false
	case *sem.CallResult, *sem.Words, *sem.Merge, *sem.RawValue, *sem.RawMemory, *sem.Memory:
		return nil, false
	default:
		return nil, false
	}
}

// addressOfZeroIndexArrayBase returns the array base for &array[0].
func addressOfZeroIndexArrayBase(expr *sem.AddressOf) (sem.Expr, bool) {
	index, ok := expr.Target.(*sem.ArrayIndex)
	if !ok || !semConstExprEquals(index.Index, 0) {
		return nil, false
	}
	if _, ok := index.Base.ExprType().(*typeinfo.Array); !ok {
		return nil, false
	}
	return index.Base, true
}

// semConstExprEquals reports whether expr is a semantic constant with value.
func semConstExprEquals(expr sem.Expr, value uint64) bool {
	c, ok := expr.(*sem.Const)
	return ok && c.U64 == value
}

func (l *lowerer) addTemp(t *sem.Temp) {
	if l.tempSeen[t.Name] {
		return
	}
	l.tempSeen[t.Name] = true
	l.temps = append(l.temps, Local{Name: t.Name, Type: t.ExprType()})
}

func lowerUnaryOp(op sem.Op) (string, bool, bool) {
	switch op {
	case sem.OpNeg:
		return "-", false, true
	case sem.OpNot:
		return "~", false, true
	default:
		return "", false, false
	}
}
func lowerBinaryOp(op sem.Op) (string, bool) {
	switch op {
	case sem.OpAdd:
		return "+", true
	case sem.OpSub:
		return "-", true
	case sem.OpMul:
		return "*", true
	case sem.OpDiv:
		return "/", true
	case sem.OpMod:
		return "%", true
	case sem.OpAnd:
		return "&", true
	case sem.OpOr:
		return "|", true
	case sem.OpXor:
		return "^", true
	case sem.OpShl:
		return "<<", true
	case sem.OpShr:
		return ">>", true
	default:
		return "", false
	}
}
func lowerCompareOp(op sem.CompareOp) (string, bool) {
	switch op {
	case sem.CompareEQ:
		return "==", true
	case sem.CompareNE:
		return "!=", true
	case sem.CompareLT:
		return "<", true
	case sem.CompareLE:
		return "<=", true
	case sem.CompareGT:
		return ">", true
	case sem.CompareGE:
		return ">=", true
	default:
		return "", false
	}
}
