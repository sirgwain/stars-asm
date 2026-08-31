package sem

import (
	"fmt"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// FormatEffect renders a semantic effect for dump output.
func FormatEffect(effect Effect) string {
	switch e := effect.(type) {
	case *Assign:
		return fmt.Sprintf("%s = %s", FormatExpr(e.Dst), FormatExpr(e.Src))
	case *CallEffect:
		line := fmt.Sprintf("call %s(%s)", formatCallName(e.Call), formatExprs(e.Call.Args))
		if e.Result != nil {
			line += fmt.Sprintf(" -> %s", FormatExpr(e.Result))
		}
		return line
	case *Branch:
		return fmt.Sprintf("branch %s ? %s : %s", FormatExpr(e.Cond), e.TrueBlock, e.FalseBlock)
	case *Jump:
		return fmt.Sprintf("goto %s", e.To)
	case *Return:
		if e.Value == nil {
			return "return"
		}
		return fmt.Sprintf("return %s", FormatExpr(e.Value))
	case *RawEffect:
		return fmt.Sprint(e.Effect)
	default:
		return fmt.Sprintf("%T", effect)
	}
}

// FormatExpr renders a semantic expression for dump output.
func FormatExpr(expr Expr) string {
	switch e := expr.(type) {
	case nil:
		return "<nil>"
	case *Local:
		return e.Name
	case *Global:
		return e.Name
	case *FunctionRef:
		return e.Name
	case *Temp:
		return e.Name
	case *CallResult:
		return fmt.Sprintf("callresult(%s)", e.ExprType())
	case *Const:
		if text, ok := formatEnumConst(e); ok {
			return text
		}
		return fmt.Sprintf("0x%x", e.U64)
	case *Register:
		return e.Val.String()
	case *StringLiteral:
		return e.Text
	case *ResourceID:
		return fmt.Sprintf("MAKEINTRESOURCE(%s)", FormatExpr(e.Value))
	case *FloatConst:
		return fmt.Sprintf("%g", e.F64)
	case *Unary:
		return fmt.Sprintf("%s(%s)", formatOp(e.Op), FormatExpr(e.X))
	case *Binary:
		return fmt.Sprintf("(%s %s %s)", FormatExpr(e.LHS), formatOp(e.Op), FormatExpr(e.RHS))
	case *Byte:
		return formatByte(e)
	case *Cast:
		return fmt.Sprintf("(%s)%s", e.To, FormatExpr(e.Value))
	case *ArrayIndex:
		return fmt.Sprintf("%s[%s]", FormatExpr(e.Base), FormatExpr(e.Index))
	case *FieldAccess:
		return formatFieldAccess(e)
	case *SymbolRef:
		return e.Path.CDecl()
	case *Compare:
		return fmt.Sprintf("%s %s %s", FormatExpr(e.LHS), formatCompareOp(e.Op), FormatExpr(e.RHS))
	case *SignExtend:
		return fmt.Sprintf("sext%dto%d(%s)", e.FromBits, e.ToBits, FormatExpr(e.Parent))
	case *Call:
		return fmt.Sprintf("%s(%s)", formatCallName(e), formatExprs(e.Args))
	case *Word:
		return fmt.Sprintf("%s(%s)", e.Part, FormatExpr(e.Parent))
	case *FarPointer:
		if e.Part == machine.FarPointerWhole {
			return fmt.Sprintf("%s(%s, %s)", e.Part, FormatExpr(e.Segment), FormatExpr(e.Offset))
		}
		return fmt.Sprintf("%s(%s)", e.Part, FormatExpr(e.Parent))
	case *PointerOffset:
		return fmt.Sprintf("ptroff(%s, %s)", FormatExpr(e.Pointer), FormatExpr(e.Offset))
	case *Deref:
		return formatDeref(e)
	case *Words:
		return "words(" + formatExprs(e.Words) + ")"
	case *Part:
		return formatPart(e)
	case *AddressOf:
		return formatAddressOf(e)
	case *Merge:
		return formatMerge(e)
	case *RawValue:
		return fmt.Sprint(e.Value)
	case *RawMemory:
		return e.Access.String()
	case *Memory:
		return formatMemory(e)
	default:
		return fmt.Sprintf("%T", expr)
	}
}

// formatAddressOf renders address-of with C array-to-pointer decay cleanup.
func formatAddressOf(e *AddressOf) string {
	if base, ok := zeroIndexArrayBase(e.Target); ok {
		return FormatExpr(base)
	}
	return "&" + FormatExpr(e.Target)
}

// zeroIndexArrayBase returns the array base for address-of array[0].
func zeroIndexArrayBase(target LValue) (Expr, bool) {
	index, ok := target.(*ArrayIndex)
	if !ok || !constExprEquals(index.Index, 0) {
		return nil, false
	}
	if _, ok := index.Base.ExprType().(*typeinfo.Array); !ok {
		return nil, false
	}
	return index.Base, true
}

// formatFieldAccess renders a semantic field projection.
func formatFieldAccess(e *FieldAccess) string {
	op := "."
	if _, ok := typeinfo.UnwrapPointer(e.Base.ExprType()); ok {
		op = "->"
	}
	return FormatExpr(e.Base) + op + e.Field.Name
}

// formatByte renders byte projection and byte replacement expressions.
func formatByte(e *Byte) string {
	if e.Value != nil {
		return fmt.Sprintf("set%s(%s, %s)", e.Part, FormatExpr(e.Parent), FormatExpr(e.Value))
	}
	return fmt.Sprintf("%s(%s)", e.Part, FormatExpr(e.Parent))
}

// formatEnumConst renders an enum-typed integer constant symbolically.
func formatEnumConst(e *Const) (string, bool) {
	enumType, ok := e.TypeInfo.(*typeinfo.Enum)
	if !ok {
		return "", false
	}
	return formatEnumValue(enumType, int(e.U64))
}

// formatEnumValue renders a concrete enum value by name.
func formatEnumValue(enumType *typeinfo.Enum, val int) (string, bool) {
	for _, ev := range enumType.Values {
		if ev.Value == val {
			return ev.Name, true
		}
	}
	if enumType.EnumKind != typeinfo.EnumFlags || val == 0 {
		return "", false
	}
	remaining := val
	parts := make([]string, 0)
	for _, ev := range enumType.Values {
		if ev.Value == 0 || ev.Value&remaining != ev.Value {
			continue
		}
		parts = append(parts, ev.Name)
		remaining &^= ev.Value
	}
	if remaining != 0 || len(parts) == 0 {
		return "", false
	}
	return strings.Join(parts, "|"), true
}

// formatDeref renders source-level memory reached through a pointer expression.
func formatDeref(e *Deref) string {
	pointer := FormatExpr(e.Pointer)
	if e.ByteOff == 0 {
		return "*" + pointer
	}
	if e.ByteOff < 0 {
		return fmt.Sprintf("*(%s-0x%x)", pointer, -e.ByteOff)
	}
	return fmt.Sprintf("*(%s+0x%x)", pointer, e.ByteOff)
}

// formatMemory renders an unresolved memory lvalue with semantic address parts.
func formatMemory(e *Memory) string {
	var sb strings.Builder
	if e.Width > 0 && e.Width != 2 {
		fmt.Fprintf(&sb, "%s ", memoryWidthString(e.Width))
	}
	if e.Seg != nil {
		sb.WriteString(FormatExpr(e.Seg))
		sb.WriteByte(':')
	}
	sb.WriteByte('[')
	needSep := false
	if e.Base != nil {
		sb.WriteString(FormatExpr(e.Base))
		needSep = true
	}
	if e.Index != nil {
		if needSep {
			sb.WriteByte('+')
		}
		sb.WriteString(FormatExpr(e.Index))
		if e.Scale != 0 && e.Scale != 1 {
			fmt.Fprintf(&sb, "*%#x", e.Scale)
		}
		needSep = true
	}
	if e.Disp != 0 || !needSep {
		if needSep {
			fmt.Fprintf(&sb, "%+#x", e.Disp)
		} else {
			fmt.Fprintf(&sb, "0x%04x", uint16(e.Disp))
		}
	}
	sb.WriteByte(']')
	return sb.String()
}

// memoryWidthString returns a C-like width prefix for memory expressions.
func memoryWidthString(width int) string {
	switch width {
	case 1:
		return "byte"
	case 2:
		return "word"
	case 4:
		return "dword"
	case 8:
		return "qword"
	default:
		return fmt.Sprintf("%d-byte", width)
	}
}

// formatPart renders a byte part in a C-like form when it matches common words.
func formatPart(e *Part) string {
	base := FormatExpr(e.Base)
	if e.Width == 2 {
		switch e.ByteOff {
		case 0:
			return fmt.Sprintf("LOWORD(%s)", base)
		case 2:
			return fmt.Sprintf("HIWORD(%s)", base)
		}
	}
	return fmt.Sprintf("part[%d:%d](%s)", e.ByteOff, e.Width, base)
}

// formatExprs renders a comma-separated expression list.
func formatExprs(exprs []Expr) string {
	parts := make([]string, len(exprs))
	for i, expr := range exprs {
		parts[i] = FormatExpr(expr)
	}
	return strings.Join(parts, ", ")
}

// formatCallName renders a semantic call target name.
func formatCallName(call *Call) string {
	if call.Target != nil {
		return FormatExpr(call.Target)
	}
	if call.Function == nil {
		return "<nil>"
	}
	return call.Function.Name
}

// formatMerge renders a semantic merge expression.
func formatMerge(e *Merge) string {
	parts := make([]string, 0, len(e.Arms))
	for _, arm := range e.Arms {
		parts = append(parts, fmt.Sprintf("%s:%s", arm.Block, FormatExpr(arm.Value)))
	}
	return fmt.Sprintf("merge(Join: %s, (%s))", e.Join, strings.Join(parts, ", "))
}

// formatOp renders a semantic operation.
func formatOp(op Op) string {
	switch op {
	case OpAdd:
		return "+"
	case OpSub:
		return "-"
	case OpMul:
		return "*"
	case OpDiv:
		return "/"
	case OpMod:
		return "%"
	case OpAnd:
		return "&"
	case OpOr:
		return "|"
	case OpXor:
		return "^"
	case OpShl:
		return "<<"
	case OpShr:
		return ">>"
	case OpNeg:
		return "neg"
	case OpNot:
		return "~"
	default:
		return "op?"
	}
}

// formatCompareOp renders a semantic comparison operation.
func formatCompareOp(op CompareOp) string {
	switch op {
	case CompareEQ:
		return "=="
	case CompareNE:
		return "!="
	case CompareLT:
		return "<"
	case CompareLE:
		return "<="
	case CompareGT:
		return ">"
	case CompareGE:
		return ">="
	default:
		return machine.PredicateVal(machine.PredicateUnknown, "cmp?").String()
	}
}
