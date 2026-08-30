package sem

import (
	"fmt"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// AnnotationKind identifies the kind of source fact resolved for an operand.
type AnnotationKind uint8

const (
	AnnotationUnknown AnnotationKind = iota
	AnnotationGlobal
	AnnotationLocal
	AnnotationFloatLiteral
	AnnotationStringLiteral
)

// Annotation records a source-level interpretation of an original operand.
type Annotation struct {
	Kind         AnnotationKind
	Path         symresolve.SymbolPath
	Var          symresolve.VarAccess
	FloatLiteral float64
	Text         string
	FarPointer   bool
}

// Result contains sem annotations keyed by original instruction operands and machine values.
type Result struct {
	Operands map[machine.AnnotationKey]Annotation
	Memory   map[string]Annotation
	Values   map[string]Annotation
}

// InstructionOperands renders an instruction's operands with known annotations applied.
func (r *Result) InstructionOperands(inst asm.DecodedInst) string {
	dst := r.operandString(inst, machine.OperandDst, inst.Dst)
	src := r.operandString(inst, machine.OperandSrc, inst.Src)
	if dst != "" {
		if src != "" {
			return dst + ", " + src
		}
		return dst
	}
	return src
}

// InstructionComment returns the comment for an instruction after annotation display.
func (r *Result) InstructionComment(inst asm.DecodedInst) string {
	if r.InstructionOperands(inst) != inst.Operands() {
		return compactAsm(inst.Operands())
	}
	return ""
}

// annotateOperand records ann for key and reports whether it changed.
func (r *Result) annotateOperand(key machine.AnnotationKey, ann Annotation) bool {
	if prev, ok := r.Operands[key]; ok && prev.sameAnnotation(ann) {
		return false
	}
	r.Operands[key] = ann
	return true
}

// annotateMemory records a resolved memory annotation and its origin operand.
func (r *Result) annotateMemory(mem machine.MemoryAccess, ann Annotation) bool {
	r.annotateOperand(machine.AnnotationKey{InstOff: mem.Origin.InstOff, Role: mem.Origin.Role}, ann)
	key := memoryAnnotationKey(mem)
	if prev, ok := r.Memory[key]; ok && prev.sameAnnotation(ann) {
		return false
	}
	r.Memory[key] = ann
	return true
}



// localAnnotation creates an assembly annotation for a resolved local or parameter.
func localAnnotation(l symresolve.LocalAccess) Annotation {
	local := l.Local
	path := symresolve.SymbolPath(&symresolve.SymbolRoot{Symbol: &local})
	if l.FieldOff != 0 {
		path = &symresolve.SymbolOffset{Base: path, Offset: l.FieldOff, Result: local.Type}
	}
	return Annotation{Kind: AnnotationLocal, Path: path, Var: &l, Text: l.String()}
}

// globalAnnotation creates an assembly annotation for a resolved global.
func globalAnnotation(g symresolve.GlobalAccess) Annotation {
	path := symresolve.SymbolPath(&symresolve.SymbolRoot{Symbol: g.Global})
	if g.FieldOff != 0 {
		path = &symresolve.SymbolOffset{Base: path, Offset: g.FieldOff, Result: g.Global.Type}
	}
	return Annotation{Kind: AnnotationGlobal, Path: path, Var: &g, Text: g.String()}
}

// sameAnnotation reports whether two annotations have the same render-relevant fields.
func (a Annotation) sameAnnotation(b Annotation) bool {
	return a.Kind == b.Kind && a.Text == b.Text
}

// operandString renders one operand with an annotation when one is available.
func (r *Result) operandString(inst asm.DecodedInst, role machine.OperandRole, operand asm.Operand) string {
	key := machine.AnnotationKey{InstOff: inst.Off, Role: role}
	ann, ok := r.Operands[key]
	if !ok {
		return operand.String()
	}
	if operand.Kind != asm.OKMem {
		return annotatedOperand(operand, ann)
	}
	return annotatedMemoryOperand(operand, ann)
}

// annotatedOperand renders a non-memory operand using its source annotation.
func annotatedOperand(operand asm.Operand, ann Annotation) string {
	switch operand.Kind {
	case asm.OKImm:
		text := strings.TrimSpace(ann.Text)
		if text != "" {
			return text
		}
	}
	return operand.String()
}

// annotatedMemoryOperand renders a memory operand using its source annotation.
func annotatedMemoryOperand(mem asm.Operand, ann Annotation) string {
	switch ann.Kind {
	case AnnotationLocal:
		if ann.FarPointer {
			return segmentAnnotatedOperand(mem, ann.Text)
		}
		name := localOperandText(ann)
		return "[" + name + "]"
	case AnnotationGlobal:
		if ann.FarPointer {
			return segmentAnnotatedOperand(mem, ann.Text)
		}
		return "[" + ann.Text + "]"
	case AnnotationFloatLiteral:
		return ann.Text
	default:
		text := strings.TrimSpace(ann.Text)
		if text != "" {
			return "[" + text + "]"
		}
		return mem.String()
	}
}

// segmentAnnotatedOperand renders an indirect annotated operand with its segment.
func segmentAnnotatedOperand(mem asm.Operand, text string) string {
	if mem.Mem.SegOverride != asm.RegNone {
		return strings.ToLower(mem.Mem.SegOverride.String()) + ":[" + text + "]"
	}
	return "[" + text + "]"
}

// localOperandText renders a BP-relative local or parameter with the correct sign.
func localOperandText(ann Annotation) string {
	local, ok := ann.Var.(*symresolve.LocalAccess)
	if !ok {
		return ann.Text
	}

	sign := "+"
	if local.Local.BPOffset < 0 {
		sign = "-"
	}

	text := "bp" + sign + local.Local.Name
	if local.FieldOff != 0 {
		text += fmt.Sprintf("+0x%x", local.FieldOff)
	}
	return text
}

// compactAsm removes alignment padding from an instruction string used as a comment.
func compactAsm(s string) string {
	return strings.Join(strings.Fields(s), " ")
}

// Value renders a machine value with known semantic annotations applied.
func (r *Result) Value(value machine.Value) string {
	if value != nil {
		if ann, ok := r.valueAnnotation(value); ok {
			return ann.Text
		}
	}
	switch v := value.(type) {
	case nil:
		return "<nil>"
	case *machine.PredicateValue:
		if v.Kind == machine.PredicateCompare {
			return fmt.Sprintf("%s %s %s", r.Value(v.LHS), machine.JccCompareOp(v.Op), r.Value(v.RHS))
		}
		return v.Op
	case *machine.CallResult:
		return v.String()
	case *machine.WordValue:
		return fmt.Sprintf("%s(%s)", v.Part, r.Value(v.Parent))
	case *machine.FarPointer:
		if v.Part == machine.FarPointerWhole {
			return fmt.Sprintf("%s(%s, %s)", v.Part, r.Value(v.Segment), r.Value(v.Offset))
		}
		return fmt.Sprintf("%s(%s)", v.Part, r.Value(v.Parent))
	case *machine.SignExtendValue:
		return fmt.Sprintf("sext%dto%d(%s)", v.FromBits, v.ToBits, r.Value(v.Parent))
	case *machine.StackWords:
		parts := make([]string, 0, len(v.Words))
		for _, word := range v.Words {
			parts = append(parts, r.Value(word))
		}
		return "words(" + strings.Join(parts, ", ") + ")"
	case *machine.ByteValue:
		if v.Value != nil {
			return fmt.Sprintf("set%s(%s, %s)", v.Part, r.Value(v.Parent), r.Value(*v.Value))
		}
		return fmt.Sprintf("%s(%s)", v.Part, r.Value(v.Parent))
	case *machine.Const:
		if ann, ok := r.constAnnotation(v); ok {
			return ann.Text
		}
		return v.String()
	case *machine.Cast:
		return fmt.Sprintf("(%s)%s", v.To, r.Value(v.Value))
	case *machine.Binary:
		return fmt.Sprintf("(%s %s %s)", r.Value(v.LHS), v.Op, r.Value(v.RHS))
	case *machine.Load:
		if ann, ok := r.memoryAnnotation(v.Access); ok && ann.Kind == AnnotationFloatLiteral {
			return ann.Text
		}
		return fmt.Sprintf("load(%s)", r.MemoryAccess(v.Access))
	case *machine.Address:
		return fmt.Sprintf("addr(%s)", r.MemoryAccess(v.Access))
	default:
		return fmt.Sprint(value)
	}
}

// MemoryAccess renders a machine memory access with known semantic annotations applied.
func (r *Result) MemoryAccess(access machine.MemoryAccess) string {
	var sb strings.Builder
	if access.Width != 2 {
		sb.WriteString(memoryWidthString(access.Width))
		sb.WriteByte(' ')
	}
	if ann, ok := r.memoryAnnotation(access); ok {
		if ann.Kind == AnnotationFloatLiteral {
			sb.WriteString(ann.Text)
			return sb.String()
		}
		sb.WriteString(annotatedAccessText(ann))
		return sb.String()
	}
	if access.Seg != nil {
		sb.WriteString(r.Value(access.Seg))
		sb.WriteByte(':')
	}

	sb.WriteByte('[')
	needSep := false
	if access.Base != nil {
		sb.WriteString(r.Value(access.Base))
		needSep = true
	}
	if access.Index != nil {
		if needSep {
			sb.WriteByte('+')
		}
		sb.WriteString(r.Value(access.Index))
		if access.Scale != 0 && access.Scale != 1 {
			fmt.Fprintf(&sb, "*%#x", access.Scale)
		}
		needSep = true
	}
	if access.Disp != 0 || !needSep {
		if needSep {
			fmt.Fprintf(&sb, "%+#x", access.Disp)
		} else {
			fmt.Fprintf(&sb, "0x%04x", uint16(access.Disp))
		}
	}
	sb.WriteByte(']')

	return sb.String()
}

// memoryAnnotation returns the source annotation for a memory access.
func (r *Result) memoryAnnotation(access machine.MemoryAccess) (Annotation, bool) {
	key := machine.AnnotationKey{InstOff: access.Origin.InstOff, Role: access.Origin.Role}
	if ann, ok := r.Operands[key]; ok {
		return ann, true
	}
	ann, ok := r.Memory[memoryAnnotationKey(access)]
	return ann, ok
}

// valueAnnotation returns the source annotation for a value.
func (r *Result) valueAnnotation(value machine.Value) (Annotation, bool) {
	ann, ok := r.Values[valueAnnotationKey(value)]
	return ann, ok
}

// constAnnotation returns the source annotation for a linked constant origin.
func (r *Result) constAnnotation(value *machine.Const) (Annotation, bool) {
	if value.Origin == nil {
		return Annotation{}, false
	}
	key := machine.AnnotationKey{InstOff: value.Origin.InstOff, Role: value.Origin.Role}
	ann, ok := r.Operands[key]
	return ann, ok
}

// annotatedAccessText renders an annotated memory access without width.
func annotatedAccessText(ann Annotation) string {
	switch ann.Kind {
	case AnnotationLocal:
		if ann.FarPointer {
			return "[" + ann.Text + "]"
		}
		return "[" + localOperandText(ann) + "]"
	case AnnotationGlobal:
		return "[" + ann.Text + "]"
	default:
		text := strings.TrimSpace(ann.Text)
		if text != "" {
			return "[" + text + "]"
		}
		return "[?]"
	}
}

// constValue returns the uint value of a constant machine value.
func constValue(v machine.Value) uint {
	c, ok := v.(*machine.Const)
	if !ok {
		return 0
	}
	return c.Val
}

// memoryAnnotationKey returns the stable key for a machine memory annotation.
func memoryAnnotationKey(mem machine.MemoryAccess) string {
	return mem.String()
}

// valueAnnotationKey returns the stable key for a machine value annotation.
func valueAnnotationKey(value machine.Value) string {
	return fmt.Sprint(value)
}

// intTypeForWidth returns a generic integer type for width in bytes.
func intTypeForWidth(width int) typeinfo.Type {
	if width == 1 {
		return typeinfo.U8
	}
	if width == 2 {
		return typeinfo.U16
	}
	return &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: fmt.Sprintf("uint%d_t", width*8), Size: width}
}
