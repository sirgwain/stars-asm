package machine

import (
	"fmt"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// DumpValue returns Go source for a machine.Value graph that can be pasted
// into tests.
func DumpValue(value Value) string {
	var d valueDumper
	return d.value(value, 0)
}

// DumpMemoryAddress returns Go source for a MemoryAddress that can be pasted
// into tests.
func DumpMemoryAddress(addr MemoryAddress) string {
	var d valueDumper
	return d.memoryAddress(addr, 0)
}

type valueDumper struct{}

// value returns a Go expression for one Value node.
func (d valueDumper) value(value Value, depth int) string {
	switch v := value.(type) {
	case nil:
		return "nil"
	case *Unknown:
		return fmt.Sprintf("machine.UnknownVal(%q)", v.Desc)
	case *Const:
		out := fmt.Sprintf("machine.ConstVal(0x%x)", v.Val)
		if v.Origin != nil {
			out += ".WithOrigin(" + d.originPtr(v.Origin, depth) + ")"
		}
		if v.Fixup != nil {
			out += " /* FIXME: add Fixup */"
		}
		return out
	case *Reg:
		return fmt.Sprintf("machine.RegVal(%s)", dumpReg(v.Val))
	case *FrameBase:
		return "machine.FrameBaseVal()"
	case *FloatConst:
		return fmt.Sprintf("machine.FloatConstVal(%g)", v.Val)
	case *CallResult:
		return d.callResult(v, depth)
	case *PredicateValue:
		return d.predicateValue(v, depth)
	case *WordValue:
		return d.call("machine.WordVal", depth, d.value(v.Parent, depth+1), dumpWordPart(v.Part))
	case *FarPointer:
		return d.farPointer(v, depth)
	case *SignExtendValue:
		return d.call(
			"machine.SignExtendVal",
			depth,
			d.value(v.Parent, depth+1),
			fmt.Sprintf("%d", v.FromBits),
			fmt.Sprintf("%d", v.ToBits),
		)
	case *StackWords:
		return d.stackWords(v, depth)
	case *ByteValue:
		return d.byteValue(v, depth)
	case *Cast:
		return d.call("machine.CastVal", depth, d.value(v.Value, depth+1), dumpType(v.To))
	case *Binary:
		return d.binary(v, depth)
	case *Load:
		return d.call("machine.LoadVal", depth, d.memoryAddress(v.Addr, depth+1))
	case *Address:
		return d.call("machine.AddressVal", depth, d.memoryAddress(v.Addr, depth+1))
	case *PhiValue:
		return d.phiValue(v, depth)
	default:
		return fmt.Sprintf("/* unsupported %T: %s */ nil", value, value)
	}
}

// binary returns Go source for a binary value, retaining producer provenance
// when the value came directly from an instruction.
func (d valueDumper) binary(v *Binary, depth int) string {
	if v.Producer == (Meta{}) {
		return d.call("machine.BinaryVal", depth, dumpValueOp(v.Op), d.value(v.LHS, depth+1), d.value(v.RHS, depth+1))
	}
	fields := []string{
		"Op: " + dumpValueOp(v.Op),
		"LHS: " + d.value(v.LHS, depth+1),
		"RHS: " + d.value(v.RHS, depth+1),
		"Producer: " + d.meta(v.Producer, depth+1),
	}
	return d.structLiteral("&machine.Binary", depth, fields)
}

// memoryAddress returns a Go composite literal for a MemoryAddress.
func (d valueDumper) memoryAddress(addr MemoryAddress, depth int) string {
	fields := make([]string, 0, 6)
	if addr.Seg != nil {
		fields = append(fields, "Seg: "+d.value(addr.Seg, depth+1))
	}
	if addr.Base != nil {
		fields = append(fields, "Base: "+d.value(addr.Base, depth+1))
	}
	if addr.Disp != 0 {
		fields = append(fields, "Disp: "+dumpIntHex(addr.Disp))
	}
	if addr.Width != 0 {
		fields = append(fields, fmt.Sprintf("Width: %d", addr.Width))
	}
	if addr.Index != nil {
		fields = append(fields, "Index: "+d.value(addr.Index, depth+1))
	}
	if addr.Origin != (Origin{}) {
		fields = append(fields, "Origin: "+d.origin(addr.Origin, depth+1))
	}
	return d.structLiteral("machine.MemoryAddress", depth, fields)
}

// callResult returns a Go composite literal for a CallResult.
func (d valueDumper) callResult(v *CallResult, depth int) string {
	fields := make([]string, 0, 3)
	if v.Target != nil {
		fields = append(fields, "Target: nil /* FIXME: "+v.Target.Name+" */")
	}
	if v.Type != nil {
		fields = append(fields, "Type: "+dumpType(v.Type))
	}
	if v.InstOff != 0 {
		fields = append(fields, fmt.Sprintf("InstOff: 0x%x", v.InstOff))
	}
	return d.structLiteral("&machine.CallResult", depth, fields)
}

// predicateValue returns a Go composite literal for a PredicateValue.
func (d valueDumper) predicateValue(v *PredicateValue, depth int) string {
	fields := []string{
		"Kind: " + dumpPredicateKind(v.Kind),
		fmt.Sprintf("Op: %q", v.Op),
	}
	if v.LHS != nil {
		fields = append(fields, "LHS: "+d.value(v.LHS, depth+1))
	}
	if v.RHS != nil {
		fields = append(fields, "RHS: "+d.value(v.RHS, depth+1))
	}
	return d.structLiteral("&machine.PredicateValue", depth, fields)
}

// farPointer returns a constructor or composite literal for a FarPointer.
func (d valueDumper) farPointer(v *FarPointer, depth int) string {
	if v.Segment == nil && v.Offset == nil {
		return d.call("machine.FarPointerVal", depth, d.value(v.Parent, depth+1), dumpFarPointerPart(v.Part))
	}
	fields := []string{
		"Parent: " + d.value(v.Parent, depth+1),
		"Part: " + dumpFarPointerPart(v.Part),
	}
	if v.Segment != nil {
		fields = append(fields, "Segment: "+d.value(v.Segment, depth+1))
	}
	if v.Offset != nil {
		fields = append(fields, "Offset: "+d.value(v.Offset, depth+1))
	}
	return d.structLiteral("&machine.FarPointer", depth, fields)
}

// stackWords returns a Go composite literal for StackWords.
func (d valueDumper) stackWords(v *StackWords, depth int) string {
	words := make([]string, 0, len(v.Words))
	for _, word := range v.Words {
		words = append(words, d.value(word, depth+2))
	}
	fields := []string{"Words: " + d.sliceLiteral("[]machine.Value", depth+1, words)}
	return d.structLiteral("&machine.StackWords", depth, fields)
}

// byteValue returns a constructor expression for a ByteValue.
func (d valueDumper) byteValue(v *ByteValue, depth int) string {
	if v.Value == nil {
		return d.call("machine.ByteVal", depth, d.value(v.Parent, depth+1), dumpBytePart(v.Part))
	}
	return d.call("machine.ByteWriteVal", depth, d.value(v.Parent, depth+1), dumpBytePart(v.Part), d.value(*v.Value, depth+1))
}

// phiValue returns a Go composite literal for a PhiValue.
func (d valueDumper) phiValue(v *PhiValue, depth int) string {
	fields := []string{fmt.Sprintf("Join: machine.BlockID(0x%x)", uint32(v.Join))}
	arms := make([]string, 0, len(v.Arms))
	for _, arm := range v.Arms {
		armFields := []string{"Value: " + d.value(arm.Value, depth+3)}
		if arm.Block != nil {
			armFields = append([]string{fmt.Sprintf("Block: &machine.Block{ID: machine.BlockID(0x%x)}", uint32(arm.Block.ID))}, armFields...)
		}
		arms = append(arms, d.structLiteral("machine.PhiArm", depth+2, armFields))
	}
	fields = append(fields, "Arms: "+d.sliceLiteral("[]machine.PhiArm", depth+1, arms))
	return d.structLiteral("&machine.PhiValue", depth, fields)
}

// originPtr returns a Go pointer expression for an Origin.
func (d valueDumper) originPtr(origin *Origin, depth int) string {
	if origin == nil {
		return "nil"
	}
	return "&" + d.origin(*origin, depth)
}

// origin returns a Go composite literal for an Origin.
func (d valueDumper) origin(origin Origin, depth int) string {
	fields := make([]string, 0, 2)
	if origin.InstOff != 0 {
		fields = append(fields, fmt.Sprintf("InstOff: 0x%x", origin.InstOff))
	}
	fields = append(fields, "Role: "+dumpOperandRole(origin.Role))
	return d.structLiteral("machine.Origin", depth, fields)
}

// meta returns a Go composite literal for machine instruction provenance.
func (d valueDumper) meta(meta Meta, depth int) string {
	fields := make([]string, 0, 4)
	if meta.BlockID != 0 {
		fields = append(fields, fmt.Sprintf("BlockID: machine.BlockID(0x%x)", uint32(meta.BlockID)))
	}
	if meta.InstOff != 0 {
		fields = append(fields, fmt.Sprintf("InstOff: 0x%x", meta.InstOff))
	}
	if meta.InstOp != 0 {
		fields = append(fields, fmt.Sprintf("InstOp: asm.Op(%d)", meta.InstOp))
	}
	if meta.InstLen != 0 {
		fields = append(fields, fmt.Sprintf("InstLen: %d", meta.InstLen))
	}
	return d.structLiteral("machine.Meta", depth, fields)
}

// call returns a formatted Go function call.
func (d valueDumper) call(name string, depth int, args ...string) string {
	if len(args) == 0 {
		return name + "()"
	}
	var sb strings.Builder
	sb.WriteString(name)
	sb.WriteString("(\n")
	for _, arg := range args {
		sb.WriteString(d.indent(depth + 1))
		sb.WriteString(arg)
		sb.WriteString(",\n")
	}
	sb.WriteString(d.indent(depth))
	sb.WriteByte(')')
	return sb.String()
}

// structLiteral returns a formatted Go composite literal.
func (d valueDumper) structLiteral(name string, depth int, fields []string) string {
	if len(fields) == 0 {
		return name + "{}"
	}
	var sb strings.Builder
	sb.WriteString(name)
	sb.WriteString("{\n")
	for _, field := range fields {
		sb.WriteString(d.indent(depth + 1))
		sb.WriteString(field)
		sb.WriteString(",\n")
	}
	sb.WriteString(d.indent(depth))
	sb.WriteByte('}')
	return sb.String()
}

// sliceLiteral returns a formatted Go slice literal.
func (d valueDumper) sliceLiteral(name string, depth int, values []string) string {
	if len(values) == 0 {
		return name + "{}"
	}
	var sb strings.Builder
	sb.WriteString(name)
	sb.WriteString("{\n")
	for _, value := range values {
		sb.WriteString(d.indent(depth + 1))
		sb.WriteString(value)
		sb.WriteString(",\n")
	}
	sb.WriteString(d.indent(depth))
	sb.WriteByte('}')
	return sb.String()
}

// indent returns the tab indentation for a nested dump.
func (d valueDumper) indent(depth int) string {
	return strings.Repeat("\t", depth)
}

// dumpReg returns the Go constant for a register.
func dumpReg(reg asm.Reg) string {
	switch reg {
	case asm.RegAX:
		return "asm.RegAX"
	case asm.RegBX:
		return "asm.RegBX"
	case asm.RegCX:
		return "asm.RegCX"
	case asm.RegDX:
		return "asm.RegDX"
	case asm.RegSI:
		return "asm.RegSI"
	case asm.RegDI:
		return "asm.RegDI"
	case asm.RegBP:
		return "asm.RegBP"
	case asm.RegSP:
		return "asm.RegSP"
	case asm.RegCS:
		return "asm.RegCS"
	case asm.RegDS:
		return "asm.RegDS"
	case asm.RegES:
		return "asm.RegES"
	case asm.RegSS:
		return "asm.RegSS"
	case asm.RegNone:
		return "asm.RegNone"
	default:
		return fmt.Sprintf("asm.Reg(%d)", reg)
	}
}

// dumpIntHex returns a Go hexadecimal integer literal.
func dumpIntHex(v int) string {
	if v < 0 {
		return fmt.Sprintf("-0x%x", -v)
	}
	return fmt.Sprintf("0x%x", v)
}

// dumpValueOp returns the Go constant for a ValueOp.
func dumpValueOp(op ValueOp) string {
	switch op {
	case ValueOpAdd:
		return "machine.ValueOpAdd"
	case ValueOpSub:
		return "machine.ValueOpSub"
	case ValueOpMul:
		return "machine.ValueOpMul"
	case ValueOpDiv:
		return "machine.ValueOpDiv"
	case ValueOpMod:
		return "machine.ValueOpMod"
	case ValueOpAnd:
		return "machine.ValueOpAnd"
	case ValueOpOr:
		return "machine.ValueOpOr"
	case ValueOpXor:
		return "machine.ValueOpXor"
	case ValueOpShl:
		return "machine.ValueOpShl"
	case ValueOpShr:
		return "machine.ValueOpShr"
	case ValueOpSar:
		return "machine.ValueOpSar"
	case ValueOpNeg:
		return "machine.ValueOpNeg"
	case ValueOpNot:
		return "machine.ValueOpNot"
	default:
		return fmt.Sprintf("machine.ValueOp(%d)", op)
	}
}

// dumpBytePart returns the Go constant for a BytePart.
func dumpBytePart(part BytePart) string {
	switch part {
	case ByteLow:
		return "machine.ByteLow"
	case ByteHigh:
		return "machine.ByteHigh"
	default:
		return fmt.Sprintf("machine.BytePart(%d)", part)
	}
}

// dumpWordPart returns the Go constant for a WordPart.
func dumpWordPart(part WordPart) string {
	switch part {
	case WordLow:
		return "machine.WordLow"
	case WordHigh:
		return "machine.WordHigh"
	case WordSignHigh:
		return "machine.WordSignHigh"
	default:
		return fmt.Sprintf("machine.WordPart(%d)", part)
	}
}

// dumpFarPointerPart returns the Go constant for a FarPointerPart.
func dumpFarPointerPart(part FarPointerPart) string {
	switch part {
	case FarPointerOffset:
		return "machine.FarPointerOffset"
	case FarPointerSegment:
		return "machine.FarPointerSegment"
	default:
		return fmt.Sprintf("machine.FarPointerPart(%d)", part)
	}
}

// dumpPredicateKind returns the Go constant for a PredicateKind.
func dumpPredicateKind(kind PredicateKind) string {
	switch kind {
	case PredicateUnknown:
		return "machine.PredicateUnknown"
	case PredicateCompare:
		return "machine.PredicateCompare"
	case PredicateFromFlags:
		return "machine.PredicateFromFlags"
	default:
		return fmt.Sprintf("machine.PredicateKind(%d)", kind)
	}
}

// dumpOperandRole returns the Go constant for an OperandRole.
func dumpOperandRole(role OperandRole) string {
	switch role {
	case OperandDst:
		return "machine.OperandDst"
	case OperandSrc:
		return "machine.OperandSrc"
	default:
		return fmt.Sprintf("machine.OperandRole(%d)", role)
	}
}

// dumpType returns the Go expression for common typeinfo.Type values.
func dumpType(typ typeinfo.Type) string {
	switch typ {
	case nil:
		return "nil"
	case typeinfo.U8:
		return "typeinfo.U8"
	case typeinfo.U16:
		return "typeinfo.U16"
	case typeinfo.U32:
		return "typeinfo.U32"
	case typeinfo.I16:
		return "typeinfo.I16"
	case typeinfo.I32:
		return "typeinfo.I32"
	case typeinfo.Double:
		return "typeinfo.Double"
	case typeinfo.LpStr:
		return "typeinfo.LpStr"
	default:
		return fmt.Sprintf("nil /* FIXME: %s */", typ)
	}
}
