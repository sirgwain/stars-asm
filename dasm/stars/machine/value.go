package machine

import (
	"fmt"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// ValueOp is machine/value arithmetic
type ValueOp uint8

const (
	ValueOpAdd ValueOp = iota
	ValueOpSub
	ValueOpMul
	ValueOpDiv
	ValueOpMod
	ValueOpAnd
	ValueOpOr
	ValueOpXor
	ValueOpShl
	ValueOpShr
	ValueOpSar
	ValueOpNeg
	ValueOpNot
)

func (op ValueOp) String() string {
	switch op {
	case ValueOpAdd:
		return "+"
	case ValueOpSub:
		return "-"
	case ValueOpMul:
		return "*"
	case ValueOpAnd:
		return "&"
	case ValueOpOr:
		return "|"
	case ValueOpXor:
		return "^"
	case ValueOpShl:
		return "<<"
	case ValueOpShr:
		return ">>"
	case ValueOpSar:
		return "sar"
	case ValueOpNeg:
		return "neg"
	case ValueOpNot:
		return "~"
	case ValueOpDiv:
		return "/"
	case ValueOpMod:
		return "%"
	default:
		return "op?"
	}
}

// BytePart describes a single byte part of a word value
type BytePart uint8

const (
	ByteLow BytePart = iota
	ByteHigh
)

// WordPart describes a projected word of a wider value.
type WordPart uint8

const (
	WordLow WordPart = iota
	WordHigh
	WordSignHigh
)

func (p BytePart) String() string {
	switch p {
	case ByteHigh:
		return "hibyte"
	default:
		return "lobyte"
	}
}

func (p WordPart) String() string {
	switch p {
	case WordHigh:
		return "hiword"
	case WordSignHigh:
		return "signhiword"
	default:
		return "loword"
	}
}

type Value interface {
	String() string
	value()
}

func UnknownVal(desc string) Value { return &Unknown{Desc: desc} }
func ConstVal(v uint) *Const       { return &Const{Val: v} }
func PredicateVal(kind PredicateKind, op string) Value {
	return &PredicateValue{Kind: kind, Op: op}
}
func RegVal(reg asm.Reg) Value { return &Reg{Val: reg} }

// FrameBaseVal returns the canonical BP-relative stack frame base.
func FrameBaseVal() Value { return &FrameBase{} }

// LoadVal returns a memory load value tagged with its operand origin.
func LoadVal(mem MemoryAccess) Value { return &Load{Access: mem, ID: valueIDFromOrigin(mem.Origin)} }

func AddressVal(mem MemoryAccess) Value { return &Address{Access: mem} }
func FloatConstVal(v float64) Value     { return &FloatConst{Val: v} }
func CastVal(v Value, to typeinfo.Type) Value {
	return &Cast{Value: v, To: to}
}
func WordVal(parent Value, part WordPart) Value {
	return &WordValue{Parent: parent, Part: part}
}

// FarPointerVal projects the offset or segment word from a known far pointer.
func FarPointerVal(parent Value, part FarPointerPart) Value {
	return &FarPointer{Parent: parent, Part: part}
}

// FarPointerWordsVal records a far pointer from its offset and segment words.
func FarPointerWordsVal(offset, segment Value) Value {
	return &FarPointer{Part: FarPointerWhole, Offset: offset, Segment: segment}
}

// SignExtendVal records an integer sign extension between machine widths.
func SignExtendVal(parent Value, fromBits, toBits int) Value {
	return &SignExtendValue{Parent: parent, FromBits: fromBits, ToBits: toBits}
}

// PredicateKind classifies branch predicates.
type PredicateKind uint8

const (
	PredicateUnknown PredicateKind = iota
	PredicateCompare
	PredicateFromFlags
)

type PredicateValue struct {
	Kind PredicateKind
	Op   string
	LHS  Value
	RHS  Value
}

func (v *PredicateValue) value() {}
func (p PredicateValue) String() string {
	if p.Kind == PredicateCompare {
		op := JccCompareOp(p.Op)
		return fmt.Sprintf("%s %s %s", p.LHS, op, p.RHS)
	}
	return p.Op
}

func JccCompareOp(mnemonic string) string {
	switch strings.ToUpper(mnemonic) {
	case "JZ", "JE":
		return "=="
	case "JNZ", "JNE":
		return "!="
	case "JL", "JNGE", "JC", "JB", "JNAE", "JS":
		return "<"
	case "JLE", "JNG", "JBE", "JNA":
		return "<="
	case "JG", "JNLE", "JA", "JNBE":
		return ">"
	case "JGE", "JNL", "JNC", "JAE", "JNB", "JNS":
		return ">="
	default:
		return mnemonic
	}
}
func BinaryVal(op ValueOp, lhs, rhs Value) Value {
	return &Binary{
		Op:  op,
		LHS: lhs,
		RHS: rhs,
	}
}

// ByteVal projects AL/AH-style byte reads from a full word value. If the word
// is already a byte write, reading the written byte returns that value and
// reading the other byte falls back to the original parent.
func ByteVal(parent Value, part BytePart) Value {
	if bv, ok := parent.(*ByteValue); ok && bv.Value != nil {
		if bv.Part == part {
			return *bv.Value
		}
		return ByteVal(bv.Parent, part)
	}
	if c, ok := parent.(*Const); ok {
		switch part {
		case ByteHigh:
			return ConstVal((c.Val >> 8) & 0xff)
		default:
			return ConstVal(c.Val & 0xff)
		}
	}
	return &ByteValue{Parent: parent, Part: part}
}

// ByteWriteVal models writes to AL/AH-style byte registers as a patched full
// word. The returned value is still the containing word, with constants folded
// when both the previous word and replacement byte are known.
func ByteWriteVal(parent Value, part BytePart, v Value) Value {
	if c, ok := v.(*Const); ok {
		v = ConstVal(c.Val & 0xff)
	}
	if parentConst, ok := parent.(*Const); ok {
		vConst, ok := v.(*Const)
		if !ok {
			return &ByteValue{Parent: parent, Part: part, Value: &v}
		}
		out := parentConst.Val & 0xffff
		switch part {
		case ByteHigh:
			out = (out & 0x00ff) | ((vConst.Val & 0xff) << 8)
		default:
			out = (out & 0xff00) | (vConst.Val & 0xff)
		}
		return ConstVal(out)
	}
	return &ByteValue{Parent: parent, Part: part, Value: &v}
}

type CallResult struct {
	Target  *typeinfo.Function
	Type    typeinfo.Type
	InstOff uint32
}

func (*CallResult) value() {}
func (v *CallResult) String() string {
	if v.Type == nil {
		return "callresult(void)"
	}
	return fmt.Sprintf("callresult(%s)", v.Type)
}

// WordValue ties AX/DX or pushed argument words back to a parent wide value.
type WordValue struct {
	Parent Value
	Part   WordPart
}

func (*WordValue) value() {}

func (v *WordValue) String() string {
	return fmt.Sprintf("%s(%s)", v.Part, v.Parent)
}

// FarPointerPart describes the word selected from a far pointer value.
type FarPointerPart uint8

const (
	FarPointerWhole FarPointerPart = iota
	FarPointerOffset
	FarPointerSegment
)

// String renders the far pointer projection name.
func (p FarPointerPart) String() string {
	switch p {
	case FarPointerSegment:
		return "farseg"
	case FarPointerOffset:
		return "faroff"
	default:
		return "farptr"
	}
}

// FarPointer identifies a source-level far pointer value or one projected word
// of that pointer.
type FarPointer struct {
	Parent  Value
	Part    FarPointerPart
	Offset  Value
	Segment Value
}

// value marks FarPointer as a machine value.
func (*FarPointer) value() {}

// String renders a far pointer word projection.
func (v *FarPointer) String() string {
	if v.Part == FarPointerWhole {
		return fmt.Sprintf("%s(%s, %s)", v.Part, v.Segment, v.Offset)
	}
	return fmt.Sprintf("%s(%s)", v.Part, v.Parent)
}

// SignExtendValue represents a signed widening operation such as CBW.
type SignExtendValue struct {
	Parent   Value
	FromBits int
	ToBits   int
}

func (*SignExtendValue) value() {}

func (v *SignExtendValue) String() string {
	return fmt.Sprintf("sext%dto%d(%s)", v.FromBits, v.ToBits, v.Parent)
}

// StackWords records a multi-word stack argument without resolving it to a
// source-level type or lvalue.
// i.e. words(0x25, 0x57a4) -> szWork global var in the ds
type StackWords struct {
	Words []Value
}

func (*StackWords) value() {}

func (v *StackWords) String() string {
	parts := make([]string, 0, len(v.Words))
	for _, word := range v.Words {
		parts = append(parts, word.String())
	}
	return "words(" + strings.Join(parts, ", ") + ")"
}

// ByteValue ties AL/AH-style byte operations to their containing word. When
// Value is nil it is a byte projection; otherwise it is the parent word with
// that byte replaced by Value.
type ByteValue struct {
	Parent Value
	Part   BytePart
	Value  *Value
}

func (*ByteValue) value() {}

func (v *ByteValue) String() string {
	if v.Value != nil {
		return fmt.Sprintf("set%s(%s, %s)", v.Part, v.Parent, *v.Value)
	}
	return fmt.Sprintf("%s(%s)", v.Part, v.Parent)
}

type Unknown struct {
	Desc string
}

func (*Unknown) value()           {}
func (v *Unknown) String() string { return "Unknown " + v.Desc }

type Const struct {
	Val    uint
	Origin *Origin
	Fixup  *asm.Fixup
}

func (*Const) value() {}
func (v *Const) WithOrigin(origin *Origin) *Const {
	v.Origin = origin
	return v
}
func (v *Const) WithFixup(fixup *asm.Fixup) *Const {
	if fixup == nil {
		return v
	}
	v.Fixup = fixup
	return v
}
func (v *Const) String() string { return fmt.Sprintf("0x%x", v.Val) }

type Reg struct {
	Val asm.Reg
}

func (*Reg) value()           {}
func (v *Reg) String() string { return v.Val.String() }

// FrameBase represents the canonical BP-relative stack frame base.
type FrameBase struct{}

// value marks FrameBase as a machine value.
func (*FrameBase) value() {}

// String renders the canonical frame base as bp.
func (*FrameBase) String() string { return "bp" }

type FloatConst struct {
	Val float64
}

func (*FloatConst) value()           {}
func (v *FloatConst) String() string { return fmt.Sprintf("%g", v.Val) }

type Cast struct {
	Value Value
	To    typeinfo.Type
}

func (*Cast) value() {}
func (v *Cast) String() string {
	return fmt.Sprintf("(%s)%s", v.To, v.Value)
}

type Binary struct {
	Op  ValueOp
	LHS Value
	RHS Value
}

func (*Binary) value() {}

func (v *Binary) String() string {
	return fmt.Sprintf("(%s %s %s)", v.LHS, v.Op, v.RHS)
}

type Load struct {
	Access MemoryAccess
	ID     ValueID
}

func (*Load) value() {}

func (v *Load) String() string {
	return fmt.Sprintf("load(%s)", v.Access)
}

// ValueID identifies a distinct symbolic value read from one instruction operand.
type ValueID struct {
	InstOff uint32
	Role    OperandRole
}

// IsZero reports whether id has no instruction operand identity.
func (id ValueID) IsZero() bool {
	return id == ValueID{}
}

// String renders a compact instruction operand identity.
func (id ValueID) String() string {
	if id.IsZero() {
		return ""
	}
	return fmt.Sprintf("@%04x/%s", id.InstOff, id.Role)
}

// valueIDFromOrigin converts an operand origin to a value identity.
func valueIDFromOrigin(origin Origin) ValueID {
	return ValueID{InstOff: origin.InstOff, Role: origin.Role}
}

// Address values are for pointers to vars
// this loads the offset address of rgprod into the ax register
// so it can be passed as an arg to InitPRoduction
// LEA   ax, [rgprod]   ; ax, [bp-264]
// PUSH  ax
// CALLF InitProduction ; void InitProduction(PROD *rgprod)
type Address struct {
	Access MemoryAccess
}

func (*Address) value() {}

func (v *Address) String() string {
	return fmt.Sprintf("addr(%s)", v.Access)
}

type MemoryAccess struct {
	Seg    Value // DS, SS, ES, etc
	Base   Value // BP, BX, or any symbolic value
	Disp   int
	Width  int
	Index  Value // optional dynamic byte offset added to Base+Disp, not a typed array index
	Scale  int   // optional multiplier for Index, 0/1 means unscaled
	Origin Origin
}

// Equals reports whether two memory accesses describe the same address.
func (a MemoryAccess) Equals(b MemoryAccess) bool {
	return a.Disp == b.Disp &&
		a.Width == b.Width &&
		a.Scale == b.Scale &&
		a.Origin == b.Origin &&
		ValueEquals(a.Seg, b.Seg) &&
		ValueEquals(a.Base, b.Base) &&
		ValueEquals(a.Index, b.Index)
}

func (a MemoryAccess) String() string {
	var sb strings.Builder
	if a.Width > 0 && a.Width != 2 {
		sb.WriteString(memoryWidthString(a.Width))
		sb.WriteByte(' ')
	}
	if a.Seg != nil {
		segText := a.Seg.String()
		_, frameBase := a.Base.(*FrameBase)
		if !(frameBase && segText == "ss") {
			sb.WriteString(segText)
			sb.WriteByte(':')
		}
	}

	sb.WriteByte('[')
	needSep := false
	if a.Base != nil {
		sb.WriteString(a.Base.String())
		needSep = true
	}
	if a.Index != nil {
		if needSep {
			sb.WriteByte('+')
		}
		sb.WriteString(a.Index.String())
		if a.Scale != 0 && a.Scale != 1 {
			fmt.Fprintf(&sb, "*%#x", a.Scale)
		}
		needSep = true
	}
	if a.Disp != 0 || !needSep {
		if needSep {
			fmt.Fprintf(&sb, "%+#x", a.Disp)
		} else {
			fmt.Fprintf(&sb, "0x%04x", uint16(a.Disp))
		}
	}
	sb.WriteByte(']')

	return sb.String()
}

// ValueEquals reports whether two symbolic machine values have the same shape.
func ValueEquals(a, b Value) bool {
	if a == nil || b == nil {
		return a == nil && b == nil
	}

	switch av := a.(type) {
	case *Unknown:
		bv, ok := b.(*Unknown)
		return ok && av.Desc == bv.Desc
	case *Const:
		bv, ok := b.(*Const)
		return ok && av.Val == bv.Val && originEquals(av.Origin, bv.Origin)
	case *Reg:
		bv, ok := b.(*Reg)
		return ok && av.Val == bv.Val
	case *FrameBase:
		_, ok := b.(*FrameBase)
		return ok
	case *FloatConst:
		bv, ok := b.(*FloatConst)
		return ok && av.Val == bv.Val
	case *CallResult:
		bv, ok := b.(*CallResult)
		return ok && callResultsEqual(av, bv)
	case *PredicateValue:
		bv, ok := b.(*PredicateValue)
		return ok && av.Kind == bv.Kind &&
			av.Op == bv.Op &&
			ValueEquals(av.LHS, bv.LHS) &&
			ValueEquals(av.RHS, bv.RHS)
	case *WordValue:
		bv, ok := b.(*WordValue)
		return ok && av.Part == bv.Part && ValueEquals(av.Parent, bv.Parent)
	case *FarPointer:
		bv, ok := b.(*FarPointer)
		return ok && av.Part == bv.Part &&
			ValueEquals(av.Parent, bv.Parent) &&
			ValueEquals(av.Offset, bv.Offset) &&
			ValueEquals(av.Segment, bv.Segment)
	case *SignExtendValue:
		bv, ok := b.(*SignExtendValue)
		return ok && av.FromBits == bv.FromBits &&
			av.ToBits == bv.ToBits &&
			ValueEquals(av.Parent, bv.Parent)
	case *StackWords:
		bv, ok := b.(*StackWords)
		return ok && valuesEqual(av.Words, bv.Words)
	case *ByteValue:
		bv, ok := b.(*ByteValue)
		return ok && byteValuesEqual(av, bv)
	case *Cast:
		bv, ok := b.(*Cast)
		return ok && typeinfo.Equals(av.To, bv.To) && ValueEquals(av.Value, bv.Value)
	case *Binary:
		bv, ok := b.(*Binary)
		return ok && av.Op == bv.Op &&
			ValueEquals(av.LHS, bv.LHS) &&
			ValueEquals(av.RHS, bv.RHS)
	case *Load:
		bv, ok := b.(*Load)
		return ok && av.ID == bv.ID && av.Access.Equals(bv.Access)
	case *Address:
		bv, ok := b.(*Address)
		return ok && av.Access.Equals(bv.Access)
	case *PhiValue:
		bv, ok := b.(*PhiValue)
		return ok && phiValuesEqual(av, bv)
	default:
		return false
	}
}

// valuesEqual reports whether two value slices have the same shape.
func valuesEqual(a, b []Value) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if !ValueEquals(a[i], b[i]) {
			return false
		}
	}
	return true
}

// originEquals reports whether two origins identify the same instruction operand.
func originEquals(a, b *Origin) bool {
	if a == nil || b == nil {
		return a == b
	}
	return a.InstOff == b.InstOff && a.Role == b.Role
}

// callResultsEqual reports whether two call results identify the same call output.
func callResultsEqual(a, b *CallResult) bool {
	return a.InstOff == b.InstOff &&
		functionTargetsEqual(a.Target, b.Target) &&
		typeinfo.Equals(a.Type, b.Type)
}

// functionTargetsEqual reports whether two function targets have the same identity.
func functionTargetsEqual(a, b *typeinfo.Function) bool {
	if a == nil || b == nil {
		return a == b
	}
	return a.Name == b.Name && a.Module == b.Module && a.Addr == b.Addr
}

// byteValuesEqual reports whether two byte projections or writes have the same shape.
func byteValuesEqual(a, b *ByteValue) bool {
	if a.Part != b.Part || !ValueEquals(a.Parent, b.Parent) {
		return false
	}
	if a.Value == nil || b.Value == nil {
		return a.Value == b.Value
	}
	return ValueEquals(*a.Value, *b.Value)
}

// phiValuesEqual reports whether two phi values have the same predecessor arms.
func phiValuesEqual(a, b *PhiValue) bool {
	if a.Join != b.Join || len(a.Arms) != len(b.Arms) {
		return false
	}
	for i, aArm := range a.Arms {
		bArm := b.Arms[i]
		if phiArmBlockID(aArm) != phiArmBlockID(bArm) || !ValueEquals(aArm.Value, bArm.Value) {
			return false
		}
	}
	return true
}

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
	case 10:
		return "tbyte"
	default:
		return fmt.Sprintf("%d-byte", width)
	}
}
