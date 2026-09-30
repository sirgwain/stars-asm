package cmd

import (
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/savefile"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
)

// TestFormatOrderRecord checks signed transfers and the zero-trimmed waypoint log format.
func TestFormatOrderRecord(t *testing.T) {
	d, err := newSaveDumper(testfixture.Stars(t))
	if err != nil {
		t.Fatal(err)
	}
	for _, tt := range []struct {
		name string
		rt   int
		data []byte
		want string
	}{
		{"planet transfer from Game.x1", savefile.RtLogCargoXfer8, []byte{97, 0, 2, 0, 0x21, 8, 0xe7}, "Colonists: object1 delta=-25kT object2 delta=+25kT raw=e7"},
		{"fleet transfer", savefile.RtLogCargoXfer8, []byte{2, 0, 97, 0, 0x12, 8, 25}, "Colonists: object1 delta=+25kT object2 delta=-25kT"},
		{"16 bit transfer", savefile.RtLogCargoXfer16, []byte{97, 0, 2, 0, 0x21, 9, 1, 0, 0x18, 0xfc}, "Colonists: object1 delta=-1000kT object2 delta=+1000kT"},
		{"32 bit transfer", savefile.RtLogCargoXfer32, []byte{97, 0, 2, 0, 0x21, 8, 0x60, 0x79, 0xfe, 0xff}, "Colonists: object1 delta=-100000kT object2 delta=+100000kT"},
		{"colonize from Game.x1", savefile.RtLogFleetOrderUpdate, []byte{2, 0, 1, 0, 0x20, 6, 0xc9, 5, 96, 0, 0x52, 0x11}, "position=(1568,1481) target=grobjPlanet id=96 warp=5 task=grTaskColonize"},
		{"transport task", savefile.RtOrderA, []byte{0x20, 6, 0xc9, 5, 96, 0, 0x51, 0x11, 0, 0, 0, 0, 0, 0, 25, 0x30, 0, 0}, "Colonists: action=iActionLoadExact quantity=25"},
		{"trimmed header", savefile.RtLogFleetOrderInsert, []byte{2}, "fleet=2 waypoint=0"},
	} {
		t.Run(tt.name, func(t *testing.T) {
			got, err := d.formatOrderRecord(tt.rt, tt.data)
			if err != nil || !strings.Contains(got, tt.want) {
				t.Fatalf("formatOrderRecord = %q, %v; want %q", got, err, tt.want)
			}
		})
	}
	for _, tt := range []struct {
		rt   int
		data []byte
	}{
		{savefile.RtLogCargoXfer8, []byte{97, 0, 2, 0, 0x21, 8}},
		{savefile.RtLogCargoXfer16, []byte{97, 0, 2, 0, 0x21, 8, 1}},
		{savefile.RtOrderA, make([]byte, 8)},
		{savefile.RtLogFleetOrderUpdate, make([]byte, 23)},
	} {
		if _, err := d.formatOrderRecord(tt.rt, tt.data); err == nil {
			t.Errorf("rt=%d data=%x: expected malformed record error", tt.rt, tt.data)
		}
	}
}
