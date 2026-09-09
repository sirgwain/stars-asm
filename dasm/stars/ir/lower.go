package ir

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type lowerer struct {
	fn       *typeinfo.Function
	cfg      *machine.CFG
	temps    []Local
	tempSeen map[string]bool
	scratch  map[string]int
}

// Lower converts post-processed semantic IR into low-level C-like IR.
// Unsupported semantic constructs are preserved as comment statements rather
// than guessed at, so the dump remains conservative.
func Lower(src sem.Func, fn *typeinfo.Function) Func {
	l := &lowerer{
		fn:       fn,
		cfg:      src.CFG,
		tempSeen: make(map[string]bool),
		scratch:  make(map[string]int),
	}
	out := Func{Name: fn.Name, Decl: fn.CDecl()}

	for _, v := range fn.Vars {
		out.Locals = append(out.Locals, Local{Name: v.Name, Type: v.Type})
	}

	for _, v := range src.RecoveredLocals {
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
	case *sem.TableJump:
		index, ok := l.lowerExpr(e.Index)
		if ok {
			labels := make([]string, len(e.Targets))
			for i, target := range e.Targets {
				labels[i] = l.blockLabel(target)
			}
			return &TableJump{Index: index, Labels: labels}
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
		return untranslatedEffect(effect)
	}
	return untranslatedEffect(effect)
}

// untranslatedEffect preserves an unsupported semantic effect together with
// structured diagnostics for every expression that blocked lowering.
func untranslatedEffect(effect sem.Effect) Stmt {
	return &Comment{
		Text:       "untranslated: " + sem.FormatEffect(effect),
		EffectKind: semanticEffectKind(effect),
		Failures:   unsupportedEffectFailures(effect),
	}
}

// semanticEffectKind returns the stable diagnostic name for an effect type.
func semanticEffectKind(effect sem.Effect) string {
	switch effect.(type) {
	case *sem.Assign:
		return "assign"
	case *sem.CallEffect:
		return "call"
	case *sem.Branch:
		return "branch"
	case *sem.TableJump:
		return "tablejump"
	case *sem.Jump:
		return "jump"
	case *sem.Return:
		return "return"
	case *sem.RawEffect:
		return "raw-effect"
	default:
		return fmt.Sprintf("%T", effect)
	}
}

// unsupportedEffectFailures returns the semantic nodes and paths that cannot
// be represented by the current low-level IR.
func unsupportedEffectFailures(effect sem.Effect) []LowerFailure {
	var failures []LowerFailure
	switch e := effect.(type) {
	case *sem.Assign:
		collectUnsupportedExpr(e.Dst, "assign.dst", &failures)
		collectUnsupportedExpr(e.Src, "assign.src", &failures)
	case *sem.CallEffect:
		if e.Call != nil {
			collectUnsupportedExpr(e.Call.Target, "call.target", &failures)
			for i, arg := range e.Call.Args {
				collectUnsupportedExpr(arg, fmt.Sprintf("call.arg[%d]", i), &failures)
			}
		}
		if _, definition := e.Result.(*sem.CallResult); !definition {
			collectUnsupportedExpr(e.Result, "call.result", &failures)
		}
	case *sem.Branch:
		collectUnsupportedExpr(e.Cond, "branch.cond", &failures)
	case *sem.TableJump:
		collectUnsupportedExpr(e.Index, "tablejump.index", &failures)
	case *sem.Return:
		collectUnsupportedExpr(e.Value, "return.value", &failures)
	case *sem.RawEffect:
		failures = append(failures, LowerFailure{Kind: "raw-effect", Path: "effect"})
	default:
		failures = append(failures, LowerFailure{Kind: fmt.Sprintf("%T", effect), Path: "effect"})
	}
	return failures
}

// collectUnsupportedExpr records unsupported nodes using the same support
// boundary as lowerExpr.
func collectUnsupportedExpr(expr sem.Expr, path string, failures *[]LowerFailure) {
	if expr == nil {
		return
	}
	switch e := expr.(type) {
	case *sem.Local, *sem.Global, *sem.FunctionRef, *sem.Temp, *sem.Const,
		*sem.StringLiteral, *sem.FloatConst, *sem.SymbolRef:
		return
	case *sem.ResourceID:
		collectUnsupportedExpr(e.Value, path+".value", failures)
	case *sem.Unary:
		if _, _, ok := lowerUnaryOp(e.Op); !ok {
			*failures = append(*failures, LowerFailure{Kind: "unary-op", Path: path})
			return
		}
		collectUnsupportedExpr(e.X, path+".operand", failures)
	case *sem.Binary:
		if _, ok := lowerBinaryOp(e.Op); !ok {
			*failures = append(*failures, LowerFailure{Kind: "binary-op", Path: path})
			return
		}
		collectUnsupportedExpr(e.LHS, path+".lhs", failures)
		collectUnsupportedExpr(e.RHS, path+".rhs", failures)
	case *sem.Byte:
		collectUnsupportedExpr(e.Parent, path+".parent", failures)
		collectUnsupportedExpr(e.Value, path+".value", failures)
	case *sem.Cast:
		collectUnsupportedExpr(e.Value, path+".value", failures)
	case *sem.ArrayIndex:
		collectUnsupportedExpr(e.Base, path+".base", failures)
		collectUnsupportedExpr(e.Index, path+".index", failures)
	case *sem.FieldAccess:
		collectUnsupportedExpr(e.Base, path+".base", failures)
	case *sem.Compare:
		if _, ok := lowerCompareOp(e.Op); !ok {
			*failures = append(*failures, LowerFailure{Kind: "compare-op", Path: path})
			return
		}
		collectUnsupportedExpr(e.LHS, path+".lhs", failures)
		collectUnsupportedExpr(e.RHS, path+".rhs", failures)
	case *sem.SignExtend:
		collectUnsupportedExpr(e.Parent, path+".parent", failures)
	case *sem.Call:
		collectUnsupportedExpr(e.Target, path+".target", failures)
		for i, arg := range e.Args {
			collectUnsupportedExpr(arg, fmt.Sprintf("%s.arg[%d]", path, i), failures)
		}
	case *sem.Word, *sem.FarPointer:
		var parent sem.Expr
		switch value := e.(type) {
		case *sem.Word:
			parent = value.Parent
		case *sem.FarPointer:
			parent = value.Parent
		}
		collectUnsupportedExpr(parent, path+".parent", failures)
	case *sem.PointerOffset:
		collectUnsupportedExpr(e.Pointer, path+".pointer", failures)
		collectUnsupportedExpr(e.Offset, path+".offset", failures)
	case *sem.Deref:
		collectUnsupportedExpr(e.Pointer, path+".pointer", failures)
	case *sem.AddressOf:
		collectUnsupportedExpr(e.Target, path+".target", failures)
	case *sem.Part:
		if e.Width != 2 || e.ByteOff != 0 && e.ByteOff != 2 {
			*failures = append(*failures, LowerFailure{Kind: "part", Path: path})
			return
		}
		collectUnsupportedExpr(e.Base, path+".base", failures)
	case *sem.CallResult:
		*failures = append(*failures, LowerFailure{Kind: "call-result", Path: path})
	case *sem.Words:
		*failures = append(*failures, LowerFailure{Kind: "words", Path: path})
	case *sem.Merge:
		*failures = append(*failures, LowerFailure{Kind: "merge", Path: path})
	case *sem.RawValue:
		*failures = append(*failures, LowerFailure{Kind: "raw-value", Path: path})
	case *sem.RawMemory:
		*failures = append(*failures, LowerFailure{Kind: "raw-memory", Path: path})
	case *sem.Memory:
		*failures = append(*failures, LowerFailure{Kind: "memory", Path: path})
	case *sem.Register:
		*failures = append(*failures, LowerFailure{Kind: "register", Path: path})
	default:
		*failures = append(*failures, LowerFailure{Kind: fmt.Sprintf("%T", expr), Path: path})
	}
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
		value, ok := l.lowerExpr(e.Value)
		if !ok {
			return nil, false
		}
		return lowerByteReplacement(parent, value, e.Part), true
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
		if scratch := scratchSymbolRoot(e.Path); scratch != nil {
			l.addScratch(scratch)
		}
		return &Var{Name: e.Path.String()}, true
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
		if base, ok := addressOfArrayBase(e); ok {
			return l.lowerExpr(base)
		}
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
		if e.Width == 1 && e.ByteOff >= 0 && e.ByteOff < e.Base.ExprType().Bytes() && e.Base.ExprType().Bytes() <= 4 {
			if e.Base.ExprType().Bytes() == 4 {
				word := "LOWORD"
				if e.ByteOff >= 2 {
					word = "HIWORD"
				}
				base = &Macro{Name: word, Args: []Expr{base}}
			}
			name := "LOBYTE"
			if e.ByteOff%2 != 0 {
				name = "HIBYTE"
			}
			return &Macro{Name: name, Args: []Expr{base}}, true
		}
		if e.Width == 2 && e.ByteOff == 0 {
			return &Macro{Name: "LOWORD", Args: []Expr{base}}, true
		}
		if e.Width == 2 && e.ByteOff == 2 {
			return &Macro{Name: "HIWORD", Args: []Expr{base}}, true
		}
		return nil, false
	case *sem.Words:
		if len(e.Words) != 2 {
			return nil, false
		}
		hi, hiOK := l.lowerExpr(e.Words[0])
		lo, loOK := l.lowerExpr(e.Words[1])
		if !hiOK || !loOK {
			return nil, false
		}
		return &Binary{Op: "|",
			LHS: &Binary{Op: "<<", LHS: &Cast{Type: "uint32_t", Value: &Cast{Type: "uint16_t", Value: hi}}, RHS: &IntConst{Value: 16}},
			RHS: &Cast{Type: "uint16_t", Value: lo},
		}, true
	case *sem.CallResult, *sem.Merge, *sem.RawValue, *sem.RawMemory, *sem.Memory:
		return nil, false
	default:
		return nil, false
	}
}

// lowerByteReplacement renders a partial-register byte update as ordinary
// mask-and-shift arithmetic on the containing word.
func lowerByteReplacement(parent, value Expr, part machine.BytePart) Expr {
	keepMask := uint64(0xff00)
	insert := Expr(&Binary{Op: "&", LHS: value, RHS: &IntConst{Value: 0xff}})
	if part == machine.ByteHigh {
		keepMask = 0x00ff
		insert = &Binary{Op: "<<", LHS: insert, RHS: &IntConst{Value: 8}}
	}
	return &Binary{
		Op:  "|",
		LHS: &Binary{Op: "&", LHS: parent, RHS: &IntConst{Value: keepMask}},
		RHS: insert,
	}
}

// addressOfArrayBase returns the array base when address-of is typed as a
// compatible pointer and C would decay the array to that pointer.
func addressOfArrayBase(expr *sem.AddressOf) (sem.Expr, bool) {
	array, ok := expr.Target.ExprType().(*typeinfo.Array)
	if !ok {
		return nil, false
	}
	ptr, ok := expr.TypeInfo.(*typeinfo.Pointer)
	if !ok {
		return nil, false
	}
	if ptr.IsCStringPointer() && array.IsCStringArray() {
		return expr.Target, true
	}
	if typeinfo.IsCallCompatible(ptr.Elem, array.Elem) {
		return expr.Target, true
	}
	return nil, false
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

// addScratch records a surviving synthetic scratch symbol as an IR local.
func (l *lowerer) addScratch(s *symresolve.SymbolScratch) {
	name := s.String()
	if index, ok := l.scratch[name]; ok {
		if l.temps[index].Type.Bytes() < s.Type().Bytes() {
			l.temps[index].Type = s.Type()
		}
		return
	}
	l.scratch[name] = len(l.temps)
	l.temps = append(l.temps, Local{Name: name, Type: s.Type()})
}

// scratchSymbolRoot returns the synthetic scratch root of a symbol path.
func scratchSymbolRoot(path symresolve.SymbolPath) *symresolve.SymbolScratch {
	for {
		switch p := path.(type) {
		case *symresolve.SymbolScratch:
			return p
		case *symresolve.SymbolOffset:
			path = p.Base
		case *symresolve.SymbolField:
			path = p.Base
		case *symresolve.SymbolBitfield:
			path = p.Base
		default:
			return nil
		}
	}
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
	case sem.OpShr, sem.OpSar:
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
