package machine_test

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

func TestDumpValueStackWords(t *testing.T) {
	value := &machine.StackWords{Words: []machine.Value{machine.ConstVal(0x0), machine.ConstVal(0x0)}}

	got := machine.DumpValue(value)
	want := `&machine.StackWords{
	Words: []machine.Value{
		machine.ConstVal(0x0),
		machine.ConstVal(0x0),
	},
}`
	if got != want {
		t.Fatalf("DumpValue() =\n%s\nwant\n%s", got, want)
	}
}

func TestDumpMemoryAddressWithNestedLoad(t *testing.T) {
	ipl := machine.LoadVal(machine.MemoryAddress{
		Base:  machine.FrameBaseVal(),
		Disp:  -0x20,
		Width: 2,
	})
	addr := machine.MemoryAddress{
		Seg: machine.LoadVal(machine.MemoryAddress{
			Seg:   machine.RegVal(asm.RegDS),
			Disp:  0x144e,
			Width: 2,
		}),
		Base: machine.BinaryVal(
			machine.ValueOpAdd,
			machine.LoadVal(machine.MemoryAddress{
				Seg:   machine.RegVal(asm.RegDS),
				Disp:  0x144c,
				Width: 2,
			}),
			machine.BinaryVal(machine.ValueOpMul, ipl, machine.ConstVal(0x4)),
		),
		Width: 2,
	}

	got := machine.DumpMemoryAddress(addr)
	want := `machine.MemoryAddress{
	Seg: machine.LoadVal(
		machine.MemoryAddress{
			Seg: machine.RegVal(asm.RegDS),
			Disp: 0x144e,
			Width: 2,
		},
	),
	Base: machine.BinaryVal(
		machine.ValueOpAdd,
		machine.LoadVal(
			machine.MemoryAddress{
				Seg: machine.RegVal(asm.RegDS),
				Disp: 0x144c,
				Width: 2,
			},
		),
		machine.BinaryVal(
			machine.ValueOpMul,
			machine.LoadVal(
				machine.MemoryAddress{
					Base: machine.FrameBaseVal(),
					Disp: -0x20,
					Width: 2,
				},
			),
			machine.ConstVal(0x4),
		),
	),
	Width: 2,
}`
	if got != want {
		t.Fatalf("DumpMemoryAddress() =\n%s\nwant\n%s", got, want)
	}
}
