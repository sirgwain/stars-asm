package typeinfo

import "testing"

func TestParseHeaderEnumsChoosesSixteenBitTypedef(t *testing.T) {
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

	if _, err := parseHeaderEnums(`typedef enum Bad { badNeg = -1, badBig = 0x8000 } Bad;`); err == nil {
		t.Fatal("parseHeaderEnums() accepted values that fit neither int16_t nor uint16_t")
	}
}
