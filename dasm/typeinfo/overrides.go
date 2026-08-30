package typeinfo

import (
	"bytes"
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

// overrideDB is used to override types from nb09 for
// improved output, such as windows types like HWND
type overrideDB struct {
	sdb          *SymbolDB
	typeResolver *typeResolver

	// override rules
	typeOverrideRules        []typeOverrideRule
	structFieldOverrideRules []structFieldOverrideRule
}

type typeOverrideRule struct {
	name   string // the variable name to override
	prefix string // the prefix to override (mutually exlusive with name)
	typ    Type   // the type to use
}

type structFieldOverrideRule struct {
	structName string // lowercased struct tag or typedef name; empty = any struct
	fieldName  string // lowercased exact field name
	prefix     string // original-case prefix (uses matchesTypePrefix)
	typ        Type
}

func newOverrideDB(sdb *SymbolDB, typeResolver *typeResolver) *overrideDB {
	db := &overrideDB{
		sdb:          sdb,
		typeResolver: typeResolver,
	}

	return db
}

// load loads types and functions from override files and adds them to the symboldb
func (o *overrideDB) load(inputDir string) error {
	if err := o.loadTypes(filepath.Join(inputDir, "overrides-types.json")); err != nil {
		return err
	}

	files, err := filepath.Glob(filepath.Join(inputDir, "override*.json"))
	if err != nil {
		return err
	}

	for _, f := range files {
		if err := o.loadFunctions(f); err != nil {
			return err
		}
	}
	for _, f := range files {
		if err := o.loadFunctionSemantics(f); err != nil {
			return err
		}
	}
	return nil
}

// ---- JSON loading ----

type overrideDBJSON struct {
	Functions            []functionsJSON           `json:"functions"`
	Globals              []globalsJSON             `json:"globals"`
	FunctionSemantics    []functionSemanticsJSON   `json:"function_semantics"`
	Types                []typesJSON               `json:"types"`
	TypeOverrideRules    []typeOverrideRulesJSON   `json:"type_override_rules"`
	StructFieldOverrides []structFieldOverrideJSON `json:"struct_overrides"`
}

type functionsJSON struct {
	Name     string           `json:"name"`
	Rename   string           `json:"rename"`
	CallConv string           `json:"callconv"`
	Ret      string           `json:"ret"`
	Params   []funcParamsJSON `json:"params"`
}

type globalsJSON struct {
	Name  string `json:"name"`
	CType string `json:"ctype"`
}

type funcParamsJSON struct {
	Name  string `json:"name"`
	CType string `json:"ctype"`
}

type functionSemanticsJSON struct {
	Name   string               `json:"name"`
	Params []paramSemanticsJSON `json:"params"`
}

type paramSemanticsJSON struct {
	Name     string `json:"name"`
	Semantic string `json:"semantic"`
}

type typesJSON struct {
	Name      string           `json:"name"`
	Canonical string           `json:"canonical"`
	Size      int              `json:"size"`
	Pointer   bool             `json:"pointer"`
	FuncPtr   *funcPtrTypeJSON `json:"funcptr,omitempty"`
}

type funcPtrTypeJSON struct {
	CallConv string           `json:"callconv"`
	Ret      string           `json:"ret"`
	Params   []funcParamsJSON `json:"params"`
	PtrClass string           `json:"ptr_class"`
}

type typeOverrideRulesJSON struct {
	Name   string `json:"name"`
	Prefix string `json:"prefix"`
	CType  string `json:"ctype"`
}

type structFieldOverrideJSON struct {
	Struct string `json:"struct"`
	Field  string `json:"field"`
	Prefix string `json:"prefix"`
	CType  string `json:"ctype"`
}

func (o *overrideDB) loadTypes(path string) error {
	b, err := os.ReadFile(path)
	if err != nil {
		return fmt.Errorf("overrides json read: %w", err)
	}
	var cfg overrideDBJSON
	dec := json.NewDecoder(bytes.NewReader(b))
	dec.DisallowUnknownFields()
	if err := dec.Decode(&cfg); err != nil {
		return fmt.Errorf("overrides json parse: %w", err)
	}

	// load additional types from json
	for _, t := range cfg.Types {
		name := strings.TrimSpace(t.Name)

		if t.FuncPtr != nil {
			ret, err := o.resolveNamedType("", stripModifers(t.FuncPtr.Ret))
			if err != nil {
				return err
			}
			params := make([]FunctionVar, 0, len(t.FuncPtr.Params))
			for _, arg := range t.FuncPtr.Params {
				typ, err := o.resolveNamedType("", stripModifers(arg.CType))
				if err != nil {
					return err
				}
				params = append(params, FunctionVar{Name: arg.Name, Type: typ})
			}
			ptrClass := PtrNear
			if t.FuncPtr.PtrClass == "far" {
				ptrClass = PtrFar
			}
			o.typeResolver.registerNamedType(t.Name, &Pointer{
				Class: ptrClass,
				Elem: &Function{
					Name:   t.Name,
					Conv:   CallConvFromString(t.FuncPtr.CallConv),
					Ret:    ret,
					Params: params,
				}})
			continue
		}

		// register simple types
		typ, err := o.resolveNamedType(name, strings.TrimSpace(stripModifers(t.Canonical)))
		if err != nil {
			return err
		}
		o.typeResolver.registerNamedType(name, typ)
	}

	// load type override rules from json, resolving ctypes to real types
	for _, rule := range cfg.TypeOverrideRules {
		typ, err := o.resolveNamedType("", strings.TrimSpace(stripModifers(rule.CType)))

		if err != nil {
			return fmt.Errorf("unable to resolve type for rule prefix: %s, name: %s, cType: %s %w", rule.Prefix, rule.Name, rule.CType, err)
		}
		o.typeOverrideRules = append(o.typeOverrideRules, typeOverrideRule{
			name:   rule.Name,
			prefix: rule.Prefix,
			typ:    typ,
		})
	}

	// load struct override rules from json, resolving ctypes to real types
	for _, rule := range cfg.StructFieldOverrides {
		typ, err := o.resolveNamedType("", strings.TrimSpace(stripModifers(rule.CType)))
		if err != nil {
			return fmt.Errorf("unable to resolve type for struct rule struct: %s, field: %s, prefix: %s, cType: %s %w", rule.Struct, rule.Field, rule.Prefix, rule.CType, err)
		}
		o.structFieldOverrideRules = append(o.structFieldOverrideRules, structFieldOverrideRule{
			structName: rule.Struct,
			fieldName:  rule.Field,
			prefix:     rule.Prefix,
			typ:        typ,
		})
	}

	return nil
}

// loadFunctions loads the functions block from an overrides.json file
// and
func (o *overrideDB) loadFunctions(path string) error {
	b, err := os.ReadFile(path)
	if err != nil {
		return fmt.Errorf("overrides json read: %w", err)
	}
	var cfg overrideDBJSON
	dec := json.NewDecoder(bytes.NewReader(b))
	dec.DisallowUnknownFields()
	if err := dec.Decode(&cfg); err != nil {
		return fmt.Errorf("overrides json parse: %w", err)
	}

	if err := o.loadGlobals(cfg.Globals); err != nil {
		return err
	}

	for _, f := range cfg.Functions {
		varargs := false
		ret, err := o.resolveNamedType("", f.Ret)
		if err != nil {
			ret = U16.WithName("uint16_t_" + f.Ret)
			// slog.Warn("unable to determine type for param", "functionName", f.Name, "ctype", f.Ret)
			// return fmt.Errorf("unable to determine return type for %s %s", f.Name, f.Ret)
		}

		params := make([]FunctionVar, 0, len(f.Params))
		for _, param := range f.Params {
			if param.CType == "..." {
				varargs = true
				continue
			}
			typ, err := o.resolveNamedType("", param.CType)
			if err != nil {
				// slog.Warn("unable to determine type for param", "functionName", f.Name, "ctype", param.CType, "name", param.Name)
				typ = U16.WithName("uint16_t_" + param.CType)
				// return fmt.Errorf("unable to determine type for param %s %s %s", f.Name, param.CType, param.Name)
			}

			params = append(params, FunctionVar{
				Name: param.Name,
				Type: typ,
			})
		}

		function := Function{
			Name:    f.Name,
			Conv:    CallConvFromString(f.CallConv),
			Module:  OverrideModule,
			Ret:     ret,
			Params:  params,
			VarArgs: varargs,
		}

		// if we have a public symbol, grab the address and rename the function
		if public := o.sdb.GetPublic(f.Name); public != nil {
			function.Addr = public.Addr
			if f.Rename != "" {
				function.Name = f.Rename
			}
		}

		o.sdb.AddFunction(&function)
	}

	return nil
}

// loadFunctionSemantics applies semantic parameter metadata from an overrides file.
func (o *overrideDB) loadFunctionSemantics(path string) error {
	b, err := os.ReadFile(path)
	if err != nil {
		return fmt.Errorf("overrides json read: %w", err)
	}
	var cfg overrideDBJSON
	dec := json.NewDecoder(bytes.NewReader(b))
	dec.DisallowUnknownFields()
	if err := dec.Decode(&cfg); err != nil {
		return fmt.Errorf("overrides json parse: %w", err)
	}
	for _, f := range cfg.FunctionSemantics {
		fn := o.sdb.GetFunction(f.Name)
		if fn == nil {
			continue
		}
		for _, param := range f.Params {
			idx := functionParamIndexByName(fn, param.Name)
			if idx < 0 {
				continue
			}
			fn.Params[idx].Semantic = param.Semantic
		}
	}
	return nil
}

// functionParamIndexByName returns the parameter index matching a name.
func functionParamIndexByName(fn *Function, name string) int {
	for i := range fn.Params {
		if strings.EqualFold(fn.Params[i].Name, name) {
			return i
		}
	}
	return -1
}

// loadGlobals loads globals overrides from overrides json
func (o *overrideDB) loadGlobals(globals []globalsJSON) error {
	for _, g := range globals {
		typ, err := o.resolveNamedType("", g.CType)
		if err != nil {
			return fmt.Errorf("unable to resolve type for global %s cType: %s %w", g.Name, g.CType, err)
		}

		global := GlobalVar{
			Name:   g.Name,
			Type:   typ,
			Module: GlobalsModule,
		}

		if public := o.sdb.GetPublic(g.Name); public != nil {
			global.Addr = public.Addr
		}

		o.sdb.AddGlobal(&global)
	}
	return nil
}

// resolveNamedType adds or replaces a C type override.
func (o *overrideDB) resolveNamedType(name, cType string) (Type, error) {
	cType = strings.TrimSpace(stripModifers(cType))
	// check for pointers
	if strings.Contains(cType, "*") {
		// pointer type
		ptrClass := PtrNear
		if strings.Contains(strings.ToLower(cType), "far *") || strings.Contains(strings.ToLower(cType), "far*") || strings.Contains(strings.ToLower(cType), "huge*") || strings.Contains(strings.ToLower(cType), "huge *") {
			ptrClass = PtrFar
		}
		typ := o.cTypeToType(name, strings.TrimSpace(
			strings.ReplaceAll(
				strings.ReplaceAll(
					strings.ReplaceAll(
						strings.ReplaceAll(
							strings.ReplaceAll(cType, "far *", ""),
							"_huge*", ""),
						"FAR *", ""),
					"FAR*", ""),
				"*", "")))
		if typ == nil {
			return nil, fmt.Errorf("failed to parse pointer %s %s", cType, name)
		}
		ptr := Pointer{
			Elem:  typ,
			Class: ptrClass,
		}
		return &ptr, nil
	}

	// check for arrays
	if strings.Contains(cType, "[") {
		base, dims, ok := splitArraySuffix(cType)
		if !ok {
			return nil, fmt.Errorf("failed to parse array %s %s", cType, name)
		}

		typ := o.cTypeToType(name, base)
		if typ == nil {
			return nil, fmt.Errorf("array basetype not found %s %s", cType, name)
		}

		// "int32_t[4][2]" -> []int{2,4}
		// TODO: only works with 1 dim...
		for _, dim := range dims {
			array := Array{
				Elem:  typ,
				Count: dim,
			}
			return &array, nil
		}
	}

	// convert simple types
	if typ := o.cTypeToType(name, cType); typ != nil {
		return typ, nil
	}

	return nil, fmt.Errorf("uanble to resolve type %s %s", cType, name)
}

// cTypeToType converts an override ctype to a primitive, struct, or enum
func (o *overrideDB) cTypeToType(name string, cType string) Type {
	if name == "" {
		// if we are resolving a generic char[32], the base type will resolve is an "int8_t"
		// but we want to preserve the char part
		name = cType
	}
	switch cType {
	case "int8_t", "char":
		return &Primitive{TypeKind: KInt, Name: name, Size: 1, Signed: true}
	case "uint8_t":
		return &Primitive{TypeKind: KInt, Name: name, Size: 1, Signed: false}
	case "int16_t":
		return &Primitive{TypeKind: KInt, Name: name, Size: 2, Signed: true}
	case "uint16_t":
		return &Primitive{TypeKind: KInt, Name: name, Size: 2, Signed: false}
	case "int32_t":
		return &Primitive{TypeKind: KInt, Name: name, Size: 4, Signed: true}
	case "uint32_t":
		return &Primitive{TypeKind: KInt, Name: name, Size: 4, Signed: false}
	case "double":
		return &Primitive{TypeKind: KFloat, Name: name, Size: 8, Signed: true}
	case "void":
		return &Primitive{TypeKind: KVoid, Name: name, Size: 0, Signed: false}
	}

	// see if we have a struct with this type
	s := o.typeResolver.getNamedType(cType)
	if s != nil {
		return s
	}
	return nil
}

// stripModifers removes const/volatile modifers. our type system doesn't care about it
func stripModifers(typeName string) string {
	return strings.ReplaceAll(strings.ReplaceAll(typeName, "const ", ""), "volatile ", "")
}

// splitArraySuffix parses fixed-size C array suffixes from right to left.
// Returns base element type text and dimensions in rightmost-first order.
// Example: "int32_t[4][2]" -> ("int32_t", []int{2,4}, true).
func splitArraySuffix(s string) (string, []int, bool) {
	cur := strings.TrimSpace(s)
	dims := make([]int, 0, 2)
	for strings.HasSuffix(cur, "]") {
		rb := len(cur) - 1
		lb := strings.LastIndex(cur[:rb], "[")
		if lb < 0 {
			return "", nil, false
		}
		nText := strings.TrimSpace(cur[lb+1 : rb])
		if nText == "" {
			return "", nil, false
		}
		n, err := strconv.Atoi(nText)
		if err != nil || n <= 0 {
			return "", nil, false
		}
		dims = append(dims, n)
		cur = strings.TrimSpace(cur[:lb])
	}
	if len(dims) == 0 || cur == "" {
		return "", nil, false
	}
	return cur, dims, true
}
