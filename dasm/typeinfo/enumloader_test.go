package typeinfo

import "testing"

// TestParseHeaderEnumsChoosesTypedef checks that enums keep the original's
// 16-bit int where their values fit one and widen to 32 bits otherwise.
func TestParseHeaderEnumsChoosesTypedef(t *testing.T) {
	enums, err := parseHeaderEnums(`
typedef enum MdXfer { mdXferNone = -1, mdXferCargo = 0 } MdXfer;
typedef enum HullSlotType { hstEngine = 0x0001, hstPlanetary = 0x8000 } HullSlotType;
`)
	if err != nil {
		t.Fatalf("parseHeaderEnums() error = %v", err)
	}
	if enums[0].Typedef != I16 || enums[1].Typedef != U16 {
		t.Fatalf("typedefs = %v, %v; want int16_t, uint16_t", enums[0].Typedef, enums[1].Typedef)
	}

	wide, err := parseHeaderEnums(`
typedef enum Mixed { mixedNeg = -1, mixedBig = 0x8000 } Mixed;
typedef enum Bits { bitLow = 0x0001, bitHigh = 0x80000000 } Bits;
`)
	if err != nil {
		t.Fatalf("parseHeaderEnums() error = %v", err)
	}
	if wide[0].Typedef != I32 || wide[1].Typedef != U32 {
		t.Fatalf("typedefs = %v, %v; want int32_t, uint32_t", wide[0].Typedef, wide[1].Typedef)
	}

	if _, err := parseHeaderEnums(`typedef enum Bad { badNeg = -1, badBig = 0x80000000 } Bad;`); err == nil {
		t.Fatal("parseHeaderEnums() accepted values that fit no 32-bit int")
	}
}
