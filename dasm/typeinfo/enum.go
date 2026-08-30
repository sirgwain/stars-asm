package typeinfo

type Enum struct {
	Name     string
	EnumKind EnumKind
	Values   []EnumValue
	Size     int

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

func (e *Enum) String() string {
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
