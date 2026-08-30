package stars

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestDecodeInitializerEnumScalar(t *testing.T) {
	enumType := &typeinfo.Enum{
		Name: "HullSlotType",
		Values: []typeinfo.EnumValue{
			{Name: "hstNone", Value: 0},
			{Name: "hstEngine", Value: 1},
			{Name: "hstPlanetary", Value: 0x8000},
		},
	}

	init, ok := decodeInitializer(nil, nil, initLocation{}, enumType, []byte{0x00, 0x80})
	if !ok {
		t.Fatal("decodeInitializer() failed")
	}
	if init.Kind != InitScalar {
		t.Fatalf("init.Kind = %v, want %v", init.Kind, InitScalar)
	}
	if init.Type != enumType {
		t.Fatalf("init.Type = %T, want enum type", init.Type)
	}
	if got, want := init.Scalar, uint64(0x8000); got != want {
		t.Fatalf("init.Scalar = %v, want %v", got, want)
	}
	if got, want := FormatInitializerC(nil, init), "hstPlanetary"; got != want {
		t.Fatalf("FormatInitializerC() = %q, want %q", got, want)
	}
}

func TestDecodeInitializerEnumArray(t *testing.T) {
	enumType := &typeinfo.Enum{
		Name: "HullSlotType",
		Values: []typeinfo.EnumValue{
			{Name: "hstNone", Value: 0},
			{Name: "hstEngine", Value: 1},
			{Name: "hstScanner", Value: 2},
		},
	}
	arrayType := &typeinfo.Array{Elem: enumType, Count: 3}

	init, ok := decodeInitializer(nil, nil, initLocation{}, arrayType, []byte{0x01, 0x00, 0x02, 0x00, 0x00, 0x00})
	if !ok {
		t.Fatal("decodeInitializer() failed")
	}
	if got, want := FormatInitializerC(nil, init), "{ hstEngine, hstScanner }"; got != want {
		t.Fatalf("FormatInitializerC() = %q, want %q", got, want)
	}
}

func TestFormatInitializerEnumFlags(t *testing.T) {
	enumType := &typeinfo.Enum{
		Name:     "HullSlotType",
		EnumKind: typeinfo.EnumFlags,
		Values: []typeinfo.EnumValue{
			{Name: "hstNone", Value: 0},
			{Name: "hstEngine", Value: 1},
			{Name: "hstScanner", Value: 2},
			{Name: "hstWeapon", Value: 3},
		},
	}

	init := &Initializer{Kind: InitScalar, Type: enumType, Scalar: uint64(3)}
	if got, want := FormatInitializerC(nil, init), "hstWeapon"; got != want {
		t.Fatalf("FormatInitializerC() = %q, want exact enum value %q", got, want)
	}

	init.Scalar = uint64(5)
	if got, want := FormatInitializerC(nil, init), "hstEngine | 4"; got == want {
		t.Fatalf("FormatInitializerC() = %q, should not invent partial enum names", got)
	}
	if got, want := FormatInitializerC(nil, init), "5"; got != want {
		t.Fatalf("FormatInitializerC() = %q, want raw fallback %q", got, want)
	}
}
