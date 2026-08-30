package asm

import "testing"

func TestDecodedInstHasFloatMemOperand(t *testing.T) {
	tests := []struct {
		name string
		b    []byte
		want bool
	}{
		{
			name: "fld m32fp",
			b:    []byte{0xD9, 0x06, 0x34, 0x12},
			want: true,
		},
		{
			name: "fcomp m64fp",
			b:    []byte{0xDC, 0x1E, 0x34, 0x12},
			want: true,
		},
		{
			name: "fld m80fp",
			b:    []byte{0xDB, 0x2E, 0x34, 0x12},
			want: true,
		},
		{
			name: "fild m32int",
			b:    []byte{0xDB, 0x06, 0x34, 0x12},
			want: false,
		},
		{
			name: "fistp m32int",
			b:    []byte{0xDB, 0x1E, 0x34, 0x12},
			want: false,
		},
		{
			name: "ordinary byte memory op",
			b:    []byte{0x8A, 0x06, 0x34, 0x12},
			want: false,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			inst := decode(tt.b, 0)
			if got := inst.HasFloatMemOperand(); got != tt.want {
				t.Fatalf("HasFloatMemOperand() = %v, want %v for %s", got, tt.want, inst.String())
			}
		})
	}
}
