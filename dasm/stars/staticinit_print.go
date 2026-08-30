package stars

import (
	"fmt"
	"math"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// FormatInitializerC renders an initializer tree as a C initializer expression.
func FormatInitializerC(sdb *typeinfo.SymbolDB, init *Initializer) string {
	return formatInit(sdb, init)
}

func formatInit(sdb *typeinfo.SymbolDB, init *Initializer) string {
	if init == nil {
		return "0"
	}
	switch init.Kind {
	case InitZero:
		return "0"

	case InitScalar:
		return formatScalarInitializer(init.Type, init.Scalar)

	case InitString:
		return init.String

	case InitArray:
		return formatArrayInitializer(sdb, init)

	case InitStruct:
		return formatStructInitializer(sdb, init)

	case InitAddress:
		return formatAddressInitializer(init)

	default:
		return "0"
	}
}

// formatScalarInitializer renders a scalar value, applying enum substitution.
func formatScalarInitializer(typ typeinfo.Type, v any) string {
	if s, ok := tryFormatEnumScalar(typ, v); ok {
		return s
	}
	return formatRawScalar(v)
}

func tryFormatEnumScalar(typ typeinfo.Type, v any) (string, bool) {
	enumType, ok := typ.(*typeinfo.Enum)
	if !ok || enumType == nil {
		return "", false
	}
	val, ok := scalarToInt64(v)
	if !ok {
		return "", false
	}
	return formatEnumValue(enumType, int(val))
}

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
		if ev.Value <= 0 || ev.Value&(ev.Value-1) != 0 {
			continue
		}
		if remaining&ev.Value == ev.Value {
			parts = append(parts, ev.Name)
			remaining &^= ev.Value
		}
	}
	if remaining != 0 || len(parts) == 0 {
		return "", false
	}
	return strings.Join(parts, " | "), true
}

func scalarToInt64(v any) (int64, bool) {
	switch val := v.(type) {
	case int64:
		return val, true
	case uint64:
		if val <= math.MaxInt64 {
			return int64(val), true
		}
		return int64(val), true // accept wrap-around for enum lookup
	case bool:
		if val {
			return 1, true
		}
		return 0, true
	default:
		return 0, false
	}
}

func formatRawScalar(v any) string {
	switch val := v.(type) {
	case bool:
		if val {
			return "1"
		}
		return "0"
	case int64:
		return fmt.Sprintf("%d", val)
	case uint64:
		return fmt.Sprintf("%d", val)
	case float32:
		if math.IsInf(float64(val), 0) || math.IsNaN(float64(val)) {
			return fmt.Sprintf("0x%08xf", math.Float32bits(val))
		}
		s := fmt.Sprintf("%gf", val)
		return s
	case float64:
		if math.IsInf(val, 0) || math.IsNaN(val) {
			return fmt.Sprintf("0x%016x", math.Float64bits(val))
		}
		return fmt.Sprintf("%g", val)
	default:
		return "0"
	}
}

// isInitZeroValue reports whether an initializer is semantically zero.
func isInitZeroValue(init *Initializer) bool {
	if init == nil {
		return true
	}
	switch init.Kind {
	case InitZero:
		return true
	case InitScalar:
		switch v := init.Scalar.(type) {
		case bool:
			return !v
		case int64:
			return v == 0
		case uint64:
			return v == 0
		case float32:
			return v == 0
		case float64:
			return v == 0
		}
		return false
	case InitString:
		return init.String == `""`
	case InitArray:
		for _, e := range init.Elems {
			if !isInitZeroValue(e) {
				return false
			}
		}
		return true
	case InitStruct:
		for _, f := range init.Fields {
			if !isInitZeroValue(f.Value) {
				return false
			}
		}
		return true
	case InitAddress:
		return init.Addr == nil || (init.Addr.Symbol == nil && init.Addr.RawOff == 0)
	}
	return false
}

func formatArrayInitializer(sdb *typeinfo.SymbolDB, init *Initializer) string {
	// String arrays just emit the string literal.
	if init.Kind == InitString {
		return init.String
	}
	elems := init.Elems
	if len(elems) == 0 {
		return "{ 0 }"
	}

	// Trim trailing zeros if requested.
	for len(elems) > 0 && isInitZeroValue(elems[len(elems)-1]) {
		elems = elems[:len(elems)-1]
	}
	if len(elems) == 0 {
		return "{ 0 }"
	}

	var b strings.Builder
	b.WriteString("{ ")
	for i, e := range elems {
		if i > 0 {
			b.WriteString(", ")
		}
		b.WriteString(formatInit(sdb, e))
	}
	b.WriteString(" }")
	return b.String()
}

func formatStructInitializer(sdb *typeinfo.SymbolDB, init *Initializer) string {
	fields := init.Fields
	if len(fields) == 0 {
		return "{ 0 }"
	}

	var parts []string
	for _, f := range fields {
		if isInitZeroValue(f.Value) {
			continue
		}
		val := formatFieldValue(sdb, f)
		if f.Member != nil && f.Member.Name != "" {
			parts = append(parts, fmt.Sprintf(".%s = %s", f.Member.Name, val))
		} else {
			parts = append(parts, val)
		}
	}
	if len(parts) == 0 {
		return "{ 0 }"
	}

	// Decide single-line vs multi-line based on complexity.
	total := 0
	for _, p := range parts {
		total += len(p)
	}
	if total > 60 || len(parts) > 3 {
		var b strings.Builder
		b.WriteString("{\n")
		for _, p := range parts {
			b.WriteString("\t")
			b.WriteString(p)
			b.WriteString(",\n")
		}
		b.WriteString("}")
		return b.String()
	}
	return "{ " + strings.Join(parts, ", ") + " }"
}

func formatFieldValue(sdb *typeinfo.SymbolDB, f InitField) string {
	return formatInit(sdb, f.Value)
}

func formatAddressInitializer(init *Initializer) string {
	if init.Addr == nil {
		return "0"
	}
	a := init.Addr

	if a.Function != nil {
		return a.Function.Name
	}

	if a.Symbol == nil && a.RawSeg == 0 && a.RawOff == 0 {
		if typeinfo.IsArray(init.Type) || typeinfo.IsArrayPointer(init.Type) {
			return "{0}"
		}
		return "0"
	}
	if a.Symbol != nil && a.ExactBase {
		return "&" + a.Symbol.Name
	}
	if a.Symbol != nil {
		// Byte offset into symbol — raw fallback for now.
		return fmt.Sprintf("((void*)&%s + %d)", a.Symbol.Name, a.ByteOff)
	}
	// Unresolved — casted hex fallback.
	if a.RawSeg != 0 {
		return fmt.Sprintf("((void __far *)0x%04x%04xUL)", a.RawSeg, uint16(a.RawOff))
	}
	return fmt.Sprintf("((void *)0x%04x)", uint16(a.RawOff))
}
