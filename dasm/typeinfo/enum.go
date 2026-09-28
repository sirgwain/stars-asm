package typeinfo

type Enum struct {
	Name     string
	EnumKind EnumKind
	Values   []EnumValue
	Size     int

	// Typedef is the integer type the enum name is declared as in the
	// generated enums.h, sized to the original 16-bit int. It is nil for
	// Win16 constant families, whose name is never declared and whose values
	// are emitted as #defines.
	Typedef Type

	// Storage is the declared type of the item this enum annotates. A use
	// whose storage differs in width from Typedef, such as a 1-byte field,
	// declares Storage instead of the enum name.
	Storage Type

	valuesByName map[string]EnumValue
}

// EnumKind controls how enum values are rendered.
type EnumKind uint8

const (
	EnumExact EnumKind = iota // exact-match: single value → name
	EnumFlags                 // bitset: value rendered as A|B|C
)

type EnumValue struct {
	Name  string
	Value int
}

func (e *Enum) Kind() Kind {
	return KInt
}
func (e *Enum) Bytes() int {
	if e.Size > 0 {
		return e.Size
	}
	return 2
}

// String returns the C type name for a declaration: the enum name where its
// typedef fits the annotated storage, otherwise the storage type.
func (e *Enum) String() string {
	if e.Typedef != nil && (e.Storage == nil || e.Storage.Bytes() == e.Typedef.Bytes()) {
		return e.Name
	}
	if e.Storage != nil {
		return e.Storage.String()
	}
	return e.Name
}

func (e *Enum) GetValue(name string) EnumValue {
	// lazy load enums
	if len(e.valuesByName) == 0 {
		e.valuesByName = map[string]EnumValue{}
		for _, v := range e.Values {
			e.valuesByName[v.Name] = v
		}
	}

	return e.valuesByName[name]
}

// EnumUseKind classifies where a symbolic interpretation rule applies.
type EnumUseKind uint8

const (
	UseParam      EnumUseKind = iota // function call argument
	UseField                         // struct/union field
	UseLocal                         // local var
	UseGlobal                        // global var
	UseCallResult                    // result of a call, selected by call identity/arguments
)

// EnumUseRule says where symbolic interpretation is valid and which enum to use.
type EnumUseRule struct {
	Kind EnumUseKind

	// global/local var matching
	Name string

	// Call-site matching (UseCallParam / UseCallResult)
	FuncName  string
	ParamName string

	// Field-site matching (UseField)
	StructName string
	FieldName  string

	// Which enum to use
	EnumName string

	// Call-result constraints (UseCallResult)
	WhenArgs []ArgConstraint
}

// DependentEnumRule maps one discriminator enum value to the enum type of a target path.
type DependentEnumRule struct {
	Type          *Struct
	Target        []string
	Discriminator []string
	EnumByValue   map[int]*Enum
}

// TargetEnumForValue returns the target enum selected by a discriminator value.
func (r *DependentEnumRule) TargetEnumForValue(value int) (*Enum, bool) {
	enumType := r.EnumByValue[value]
	return enumType, enumType != nil
}

// AppliesToType reports whether the rule is rooted at typ.
func (r *DependentEnumRule) AppliesToType(typ Type) bool {
	strct, ok := namedStructType(typ)
	if !ok {
		return false
	}
	return typeLookupName(strct) == typeLookupName(r.Type)
}

// ArgConstraint requires a named call argument to have a specific constant value.
type ArgConstraint struct {
	ParamName string
	Value     int
}

// MessageEnumName is the enum naming window message identifiers. Message
// payload rules and window procedure message parameters are keyed by it.
const MessageEnumName = "WMType"

// MessageRule describes the typed payload carried by one window message.
type MessageRule struct {
	// Enum is the message enum naming the message: window messages, or a
	// window class's control messages.
	Enum   *Enum
	Name   string
	Value  int
	WParam *MessagePayloadRule
	LParam *MessagePayloadRule
	// Result is the type a window procedure returns for this message, such
	// as the brush handle answering WM_CTLCOLOR.
	Result Type

	// Match is a predicate of the message parameter recognizing this
	// message natively, for a Win16 message Win32 replaced with others:
	// IS_WM_CTLCOLOR for the WM_CTLCOLOR* range. It is nil for messages
	// Win32 still sends.
	Match *Function
}

// MessagePayloadRule describes interpretations of a whole message parameter
// and its low and high words.
type MessagePayloadRule struct {
	Whole  MessagePart
	Loword MessagePart
	Hiword MessagePart
}

// MessagePart is one interpretation of a message parameter or word of it:
// its value type, and for a part Win32 moved elsewhere, the cracker that
// reads it with the Win32 packing.
type MessagePart struct {
	Type Type

	// Get is a GET_WM_* message cracker: a function of (wParam, lParam)
	// returning Type, defined in the generated headers. It is nil where the
	// Win32 packing matches Win16.
	Get *Function
}
