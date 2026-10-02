package typeinfo

import (
	"bufio"
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"regexp"
	"strings"
)

type enumLoader struct {
}

// loadEnumsFromHeader parses a C header file for typedef enum definitions
// and loads them into the SymbolDB.
func (l *enumLoader) loadEnumsFromHeader(path string) ([]*Enum, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, fmt.Errorf("enum header %s: %w", path, err)
	}
	defer f.Close()

	data, err := io.ReadAll(f)
	if err != nil {
		return nil, fmt.Errorf("enum header read: %w", err)
	}
	eds, err := parseHeaderEnums(string(data))
	if err != nil {
		return nil, err
	}
	return eds, nil
}

// loadEnumConfig reads enum symbolic configuration from JSON.
func (l *enumLoader) loadEnumConfig(path string) (symbolicConfigJSON, error) {
	f, err := os.Open(path)
	if err != nil {
		return symbolicConfigJSON{}, fmt.Errorf("enum header %s: %w", path, err)
	}
	defer f.Close()

	data, err := io.ReadAll(f)
	if err != nil {
		return symbolicConfigJSON{}, fmt.Errorf("enum header read: %w", err)
	}

	var cfg symbolicConfigJSON
	dec := json.NewDecoder(bytes.NewReader(data))
	if err := dec.Decode(&cfg); err != nil {
		return symbolicConfigJSON{}, fmt.Errorf("symbolic values parse %s: %w", path, err)
	}
	return cfg, nil
}

// loadEnumRules loads enum use rules from JSON.
func (l *enumLoader) loadEnumRules(path string) ([]*EnumUseRule, error) {
	cfg, err := l.loadEnumConfig(path)
	if err != nil {
		return nil, err
	}
	uses := make([]*EnumUseRule, 0, len(cfg.Uses))
	for _, u := range cfg.Uses {
		r := parseUseRuleJSON(u)
		if r == nil {
			return nil, fmt.Errorf("failed to load enum use kind: %s, enum: %s, func: %s, struct: %s", u.Kind, u.Enum, u.Func, u.Struct)
		}
		uses = append(uses, r)
	}
	return uses, nil
}

// loadDependentEnumRules loads dependent enum path rules from JSON.
func (l *enumLoader) loadDependentEnumRules(path string, sdb *SymbolDB) ([]*DependentEnumRule, error) {
	cfg, err := l.loadEnumConfig(path)
	if err != nil {
		return nil, err
	}
	dependent := make([]*DependentEnumRule, 0, len(cfg.DependentEnums))
	for _, ruleJSON := range cfg.DependentEnums {
		rule, err := parseDependentEnumRuleJSON(ruleJSON, sdb)
		if err != nil {
			return nil, err
		}
		dependent = append(dependent, rule)
	}
	return dependent, nil
}

// loadMessageRules loads typed window-message payload rules from JSON.
func (l *enumLoader) loadMessageRules(path string, sdb *SymbolDB, resolver *typeResolver) ([]*MessageRule, error) {
	cfg, err := l.loadEnumConfig(path)
	if err != nil {
		return nil, err
	}
	messages := make([]*MessageRule, 0, len(cfg.Messages))
	for _, messageJSON := range cfg.Messages {
		message, err := parseMessageRuleJSON(messageJSON, sdb, resolver)
		if err != nil {
			return nil, err
		}
		messages = append(messages, message)
	}
	return messages, nil
}

var reTypedefEnum = regexp.MustCompile(`(?s)\btypedef\s+enum\b\s*(?:[A-Za-z_][A-Za-z0-9_]*\s*)?\{(.*?)\}\s*([A-Za-z_][A-Za-z0-9_]*)\s*;`)

func parseHeaderEnums(text string) ([]*Enum, error) {
	text = stripCComments(text)
	ms := reTypedefEnum.FindAllStringSubmatch(text, -1)
	if len(ms) == 0 {
		return nil, nil
	}
	result := make([]*Enum, 0, len(ms))
	for _, m := range ms {
		body := m[1]
		name := m[2]
		vals, err := parseEnumBody(body)
		if err != nil {
			return nil, fmt.Errorf("enum %s: %w", name, err)
		}
		typedef, err := enumTypedef(vals)
		if err != nil {
			return nil, fmt.Errorf("enum %s: %w", name, err)
		}
		result = append(result, &Enum{
			Name:     name,
			EnumKind: EnumExact,
			Values:   vals,
			Typedef:  typedef,
		})
	}
	return result, nil
}

func parseEnumBody(body string) ([]EnumValue, error) {
	parts := strings.Split(body, ",")
	vals := []EnumValue{}
	resolved := make(map[string]int)
	var prev int
	havePrev := false
	for _, p := range parts {
		p = strings.TrimSpace(p)
		if p == "" {
			continue
		}
		name, expr, hasEq := strings.Cut(p, "=")
		name = strings.TrimSpace(name)
		if name == "" {
			continue
		}
		var v int
		if hasEq {
			expr = strings.TrimSpace(expr)
			n, err := evalConstExpr(expr, func(name string) (int, error) {
				if v, ok := resolved[name]; ok {
					return v, nil
				}
				return 0, fmt.Errorf("unknown name %q", name)
			})
			if err != nil {
				return nil, fmt.Errorf("bad value for %s: %q: %w", name, expr, err)
			}
			v = n
		} else {
			if !havePrev {
				v = 0
			} else {
				v = prev + 1
			}
		}
		prev, havePrev = v, true
		resolved[name] = v
		vals = append(vals, EnumValue{Name: name, Value: v})
	}
	return vals, nil
}

// enumTypedef returns the integer type that holds every value of an enum:
// the 16-bit int the original used where every value fits one, otherwise
// 32 bits, signed when any value is negative.
func enumTypedef(values []EnumValue) (Type, error) {
	signed := false
	for _, v := range values {
		if v.Value < 0 {
			signed = true
		}
	}
	for _, typ := range []Type{U16, U32} {
		if signed {
			typ = map[Type]Type{U16: I16, U32: I32}[typ]
		}
		fits := true
		for _, v := range values {
			if signed && (v.Value < -1<<(8*typ.Bytes()-1) || v.Value >= 1<<(8*typ.Bytes()-1)) || !signed && v.Value >= 1<<(8*typ.Bytes()) {
				fits = false
				break
			}
		}
		if fits {
			return typ, nil
		}
	}
	for _, v := range values {
		if v.Value < -1<<31 || v.Value >= 1<<32 {
			return nil, fmt.Errorf("%s = %d does not fit 32 bits", v.Name, v.Value)
		}
	}
	return nil, fmt.Errorf("enum values do not fit a signed 32-bit int")
}

func stripCComments(s string) string {
	block := regexp.MustCompile(`(?s)/\*.*?\*/`)
	s = block.ReplaceAllString(s, "")
	var out strings.Builder
	sc := bufio.NewScanner(strings.NewReader(s))
	for sc.Scan() {
		line := sc.Text()
		if idx := strings.Index(line, "//"); idx >= 0 {
			line = line[:idx]
		}
		out.WriteString(line)
		out.WriteByte('\n')
	}
	return out.String()
}

type symbolicConfigJSON struct {
	FlagEnums       []string                `json:"flag_enums"`
	CharEnums       []string                `json:"char_enums"`
	Uses            []useRuleJSON           `json:"uses"`
	Messages        []messageRuleJSON       `json:"messages"`
	DependentEnums  []dependentEnumRuleJSON `json:"dependent_enums"`
	WindowClasses   []windowClassJSON       `json:"window_classes"`
	Windows         []windowRuleJSON        `json:"windows"`
	SentMessages    []sentMessagesJSON      `json:"sent_messages"`
	DialogControls  []dialogControlsJSON    `json:"dialog_controls"`
	BufferViews     []bufferViewJSON        `json:"buffer_views"`
	MessageHandlers []messageHandlerJSON    `json:"message_handlers"`
}

type useRuleJSON struct {
	Kind string `json:"kind"`

	Name string `json:"name"`

	Func  string `json:"func"`
	Param string `json:"param"`

	Struct string `json:"struct"`
	Field  string `json:"field"`

	Prefix  string   `json:"prefix"`
	Exclude []string `json:"exclude"`

	Enum string              `json:"enum"`
	When []argConstraintJSON `json:"when"`
}

type argConstraintJSON struct {
	Param  string `json:"param"`
	Value  int    `json:"value"`
	Global string `json:"global"`
	String string `json:"string"`
}

type messageRuleJSON struct {
	Message string              `json:"msg"`
	WParam  *messagePayloadJSON `json:"wparam"`
	LParam  *messagePayloadJSON `json:"lparam"`
	Result  *messageValueJSON   `json:"result"`
	Match   string              `json:"match"`
}

type messagePayloadJSON struct {
	Whole  *messageValueJSON `json:"whole"`
	Loword *messageValueJSON `json:"loword"`
	Hiword *messageValueJSON `json:"hiword"`
}

type messageValueJSON struct {
	Char     bool   `json:"char"`
	Enum     string `json:"enum"`
	CastType string `json:"cast_type"`
	Get      string `json:"get"`
	Win32    string `json:"win32"`
	HWnd     bool   `json:"hwnd"`
}

type dependentEnumRuleJSON struct {
	Type              string            `json:"type"`
	Func              string            `json:"func"`
	DiscriminatorEnum string            `json:"discriminator_enum"`
	Target            []string          `json:"target"`
	Discriminator     []string          `json:"discriminator"`
	EnumByValue       map[string]string `json:"enum_by_value"`
}

func parseUseRuleJSON(u useRuleJSON) *EnumUseRule {
	r := &EnumUseRule{
		Name:       u.Name,
		FuncName:   u.Func,
		ParamName:  u.Param,
		StructName: u.Struct,
		FieldName:  u.Field,
		EnumName:   u.Enum,
		Prefix:     u.Prefix,
		Exclude:    u.Exclude,
	}
	switch strings.ToLower(u.Kind) {
	case "param":
		r.Kind = UseParam
	case "field":
		r.Kind = UseField
	case "global":
		r.Kind = UseGlobal
	case "local":
		r.Kind = UseLocal
	case "call_result":
		r.Kind = UseCallResult
	default:
		return nil
	}
	if len(u.When) != 0 {
		r.WhenArgs = make([]ArgConstraint, 0, len(u.When))
		for _, when := range u.When {
			r.WhenArgs = append(r.WhenArgs, ArgConstraint{
				ParamName: when.Param,
				Value:     when.Value,
				Global:    when.Global,
				String:    when.String,
			})
		}
	}
	return r
}

// parseMessageRuleJSON resolves one message payload record to type metadata.
func parseMessageRuleJSON(cfg messageRuleJSON, sdb *SymbolDB, resolver *typeResolver) (*MessageRule, error) {
	var messageEnum *Enum
	var messageValue EnumValue
	for _, enum := range sdb.messageEnums() {
		if value, ok := enumValueByName(enum, cfg.Message); ok {
			messageEnum, messageValue = enum, value
			break
		}
	}
	if messageEnum == nil {
		return nil, fmt.Errorf("message %s not found in %s or a window class message enum", cfg.Message, MessageEnumName)
	}
	wparam, err := parseMessagePayloadJSON(cfg.Message, "wparam", cfg.WParam, sdb, resolver)
	if err != nil {
		return nil, err
	}
	lparam, err := parseMessagePayloadJSON(cfg.Message, "lparam", cfg.LParam, sdb, resolver)
	if err != nil {
		return nil, err
	}
	result, err := parseMessageValueJSON(cfg.Message, "result", cfg.Result, sdb, resolver)
	if err != nil {
		return nil, err
	}
	rule := &MessageRule{
		Enum:   messageEnum,
		Name:   messageValue.Name,
		Value:  messageValue.Value,
		WParam: wparam,
		LParam: lparam,
		Result: result,
	}
	if cfg.Match != "" {
		uint := resolver.getNamedType("UINT")
		if uint == nil {
			return nil, fmt.Errorf("message %s match %s: UINT type not registered", cfg.Message, cfg.Match)
		}
		rule.Match = &Function{
			Name:   cfg.Match,
			Module: OverrideModule,
			Ret:    I16,
			Params: []FunctionVar{{Name: "msg", Type: uint}},
		}
	}
	return rule, nil
}

// parseMessagePayloadJSON resolves the whole and word-part types for one
// message parameter.
func parseMessagePayloadJSON(message, parameter string, cfg *messagePayloadJSON, sdb *SymbolDB, resolver *typeResolver) (*MessagePayloadRule, error) {
	if cfg == nil {
		return nil, nil
	}
	whole, err := parseMessagePartJSON(message, parameter+".whole", cfg.Whole, sdb, resolver)
	if err != nil {
		return nil, err
	}
	loword, err := parseMessagePartJSON(message, parameter+".loword", cfg.Loword, sdb, resolver)
	if err != nil {
		return nil, err
	}
	hiword, err := parseMessagePartJSON(message, parameter+".hiword", cfg.Hiword, sdb, resolver)
	if err != nil {
		return nil, err
	}
	return &MessagePayloadRule{Whole: whole, Loword: loword, Hiword: hiword}, nil
}

// parseMessagePartJSON resolves one message parameter part's type and, when
// Win32 repacked it, the cracker function reading it.
func parseMessagePartJSON(message, path string, cfg *messageValueJSON, sdb *SymbolDB, resolver *typeResolver) (MessagePart, error) {
	if cfg != nil && cfg.Char {
		if cfg.Enum != "" || cfg.CastType != "" || cfg.Get != "" {
			return MessagePart{}, fmt.Errorf("message %s %s: a char part names no enum, cast_type or cracker", message, path)
		}
		return MessagePart{Char: true}, nil
	}
	typ, err := parseMessageValueJSON(message, path, cfg, sdb, resolver)
	if err != nil || cfg == nil {
		return MessagePart{Type: typ}, err
	}
	if (cfg.Get == "") != (cfg.Win32 == "") {
		return MessagePart{}, fmt.Errorf("message %s %s must specify both or neither of get and win32", message, path)
	}
	if cfg.Get == "" {
		if cfg.HWnd {
			return MessagePart{}, fmt.Errorf("message %s %s: hwnd requires a get cracker", message, path)
		}
		return MessagePart{Type: typ}, nil
	}
	wparam := resolver.getNamedType("WPARAM")
	lparam := resolver.getNamedType("LPARAM")
	hwnd := resolver.getNamedType("HWND")
	if wparam == nil || lparam == nil || hwnd == nil {
		return MessagePart{}, fmt.Errorf("message %s %s cracker %s: HWND, WPARAM and LPARAM types not registered", message, path, cfg.Get)
	}
	params := []FunctionVar{{Name: "wParam", Type: wparam}, {Name: "lParam", Type: lparam}}
	if cfg.HWnd {
		params = append([]FunctionVar{{Name: "hwnd", Type: hwnd}}, params...)
	}
	get := &Function{
		Name:   cfg.Get,
		Module: OverrideModule,
		Ret:    typ,
		Params: params,
		Macro:  true,
	}
	return MessagePart{Type: typ, Get: get, Win32: cfg.Win32}, nil
}

// parseMessageValueJSON resolves an enum or cast type for one message value.
func parseMessageValueJSON(message, path string, cfg *messageValueJSON, sdb *SymbolDB, resolver *typeResolver) (Type, error) {
	if cfg == nil {
		return nil, nil
	}
	if (cfg.Enum == "") == (cfg.CastType == "") {
		return nil, fmt.Errorf("message %s %s must specify exactly one of enum or cast_type", message, path)
	}
	if cfg.Enum != "" {
		typ := sdb.GetEnum(cfg.Enum)
		if typ == nil {
			return nil, fmt.Errorf("message %s %s enum %s not found", message, path, cfg.Enum)
		}
		return typ, nil
	}
	typ, err := (&overrideDB{sdb: sdb, typeResolver: resolver}).resolveNamedType("", cfg.CastType)
	if err != nil {
		return nil, fmt.Errorf("message %s %s cast type %s: %w", message, path, cfg.CastType, err)
	}
	return typ, nil
}

// parseDependentEnumRuleJSON resolves a dependent enum JSON record to typed rule data.
func parseDependentEnumRuleJSON(cfg dependentEnumRuleJSON, sdb *SymbolDB) (*DependentEnumRule, error) {
	if cfg.Func != "" {
		return parseFunctionDependentEnumRuleJSON(cfg, sdb)
	}
	strct := sdb.GetStruct(cfg.Type)
	if strct == nil {
		return nil, fmt.Errorf("dependent enum type %s not found", cfg.Type)
	}
	if len(cfg.Target) == 0 {
		return nil, fmt.Errorf("dependent enum %s has empty target path", cfg.Type)
	}
	if len(cfg.Discriminator) == 0 {
		return nil, fmt.Errorf("dependent enum %s has empty discriminator path", cfg.Type)
	}
	if _, ok := resolveStructFieldPathForEnum(strct, cfg.Target); !ok {
		return nil, fmt.Errorf("dependent enum %s target %s not found", cfg.Type, strings.Join(cfg.Target, "."))
	}
	discriminator, ok := resolveStructFieldPathForEnum(strct, cfg.Discriminator)
	if !ok {
		return nil, fmt.Errorf("dependent enum %s discriminator %s not found", cfg.Type, strings.Join(cfg.Discriminator, "."))
	}
	discriminatorEnum, ok := discriminator.Type.(*Enum)
	if !ok {
		return nil, fmt.Errorf("dependent enum %s discriminator %s is %s, not enum", cfg.Type, strings.Join(cfg.Discriminator, "."), discriminator.Type)
	}

	enumByValue := make(map[int]*Enum, len(cfg.EnumByValue))
	for valueName, enumName := range cfg.EnumByValue {
		value, ok := enumValueByName(discriminatorEnum, valueName)
		if !ok {
			return nil, fmt.Errorf("dependent enum %s discriminator value %s not found in %s", cfg.Type, valueName, discriminatorEnum.Name)
		}
		enumType := resolveDependentTargetEnum(sdb, enumName)
		if enumType == nil {
			return nil, fmt.Errorf("dependent enum %s target enum %s not found", cfg.Type, enumName)
		}
		enumByValue[value.Value] = enumType
	}

	return &DependentEnumRule{
		Type:          strct,
		Target:        append([]string(nil), cfg.Target...),
		Discriminator: append([]string(nil), cfg.Discriminator...),
		EnumByValue:   enumByValue,
	}, nil
}

// parseFunctionDependentEnumRuleJSON resolves a dependent enum rule rooted at
// a function's variables, such as a report column whose enum depends on the
// report type parameter.
func parseFunctionDependentEnumRuleJSON(cfg dependentEnumRuleJSON, sdb *SymbolDB) (*DependentEnumRule, error) {
	if cfg.Type != "" {
		return nil, fmt.Errorf("dependent enum for func %s must not also name type %s", cfg.Func, cfg.Type)
	}
	fn := sdb.GetFunction(cfg.Func)
	if fn == nil {
		return nil, fmt.Errorf("dependent enum func %s not found", cfg.Func)
	}
	for _, path := range [][]string{cfg.Target, cfg.Discriminator} {
		if len(path) == 0 {
			return nil, fmt.Errorf("dependent enum for func %s has an empty path", cfg.Func)
		}
		typ, ok := functionVarType(fn, path[0])
		if !ok {
			global := sdb.GetGlobal(path[0])
			if global == nil {
				return nil, fmt.Errorf("dependent enum for func %s: %s is not a param, local or global", cfg.Func, path[0])
			}
			typ = global.Type
		}
		if len(path) > 1 {
			strct, ok := namedStructTypeForDependent(typ)
			if !ok {
				return nil, fmt.Errorf("dependent enum for func %s: %s is not a struct", cfg.Func, path[0])
			}
			if _, ok := resolveStructFieldPathForEnum(strct, path[1:]); !ok {
				return nil, fmt.Errorf("dependent enum for func %s: path %s not found", cfg.Func, strings.Join(path, "."))
			}
		}
	}
	discriminatorEnum := sdb.GetEnum(cfg.DiscriminatorEnum)
	if discriminatorEnum == nil {
		return nil, fmt.Errorf("dependent enum for func %s: discriminator enum %q not found", cfg.Func, cfg.DiscriminatorEnum)
	}
	enumByValue := make(map[int]*Enum, len(cfg.EnumByValue))
	for valueName, enumName := range cfg.EnumByValue {
		value, ok := enumValueByName(discriminatorEnum, valueName)
		if !ok {
			return nil, fmt.Errorf("dependent enum for func %s: discriminator value %s not found in %s", cfg.Func, valueName, discriminatorEnum.Name)
		}
		enumType := resolveDependentTargetEnum(sdb, enumName)
		if enumType == nil {
			return nil, fmt.Errorf("dependent enum for func %s: target enum %s not found", cfg.Func, enumName)
		}
		enumByValue[value.Value] = enumType
	}
	return &DependentEnumRule{
		Func:              cfg.Func,
		DiscriminatorEnum: discriminatorEnum,
		Target:            append([]string(nil), cfg.Target...),
		Discriminator:     append([]string(nil), cfg.Discriminator...),
		EnumByValue:       enumByValue,
	}, nil
}

// functionVarType returns the type of fn's param or local named name.
func functionVarType(fn *Function, name string) (Type, bool) {
	for _, v := range fn.Params {
		if v.Name == name {
			return v.Type, true
		}
	}
	for _, v := range fn.Vars {
		if v.Name == name {
			return v.Type, true
		}
	}
	return nil, false
}

// resolveDependentTargetEnum returns the enum named or value-prefixed by name.
func resolveDependentTargetEnum(sdb *SymbolDB, name string) *Enum {
	if enumType := sdb.GetEnum(name); enumType != nil {
		return enumType
	}
	for _, enumType := range sdb.Enums {
		if enumUsesValuePrefix(enumType, name) {
			return enumType
		}
	}
	return nil
}

// enumUsesValuePrefix reports whether every named enum value uses prefix.
func enumUsesValuePrefix(enumType *Enum, prefix string) bool {
	if len(enumType.Values) == 0 {
		return false
	}
	for _, value := range enumType.Values {
		if !strings.HasPrefix(value.Name, prefix) {
			return false
		}
	}
	return true
}

// resolveStructFieldPathForEnum resolves a named field path rooted at a struct.
func resolveStructFieldPathForEnum(strct *Struct, path []string) (*StructField, bool) {
	var field *StructField
	current := strct
	for _, name := range path {
		field = structFieldByNameForDependent(current, name)
		if field == nil {
			return nil, false
		}
		next, ok := namedStructTypeForDependent(field.Type)
		if !ok {
			current = nil
			continue
		}
		current = next
	}
	return field, true
}

// structFieldByNameForDependent returns a field by name.
func structFieldByNameForDependent(strct *Struct, name string) *StructField {
	if strct == nil {
		return nil
	}
	for i := range strct.Fields {
		if strct.Fields[i].Name == name {
			return &strct.Fields[i]
		}
	}
	return nil
}

// namedStructTypeForDependent unwraps pointers and returns a struct type.
func namedStructTypeForDependent(typ Type) (*Struct, bool) {
	unwrapped, _ := UnwrapPointer(typ)
	strct, ok := unwrapped.(*Struct)
	return strct, ok
}
