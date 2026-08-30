package asm

import (
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

var starsExePath = "../../input/stars.exe"

func TestDecodeFunc(t *testing.T) {
	// open the NE part of the binary for call fixups, seg:off mapping, etc
	img, err := OpenNE(starsExePath)
	if err != nil {
		t.Fatalf("open exe/ne %s: %v", starsExePath, err)
	}

	tests := []struct {
		name string // description of this test case
		// Named input parameters for target function.
		ctx     *FuncContext
		want    string
		wantErr bool
	}{
		{
			name: "SetVCCheck",
			ctx: NewFuncContext(img,
				&typeinfo.Function{
					Name: "SetVCCheck",
					Addr: typeinfo.Addr{Seg: 0x10, Off: 0xb5bc},
					Len:  79,
				}),
			want: `
            PUSH      bp
            MOV       bp, sp
            SUB       sp, 0x0004
            PUSH      si
            PUSH      di
            CMP       [bp+0xa], 0x0000
            JNZ       L_b5ce
            JMP       L_b5d4
            MOV       ax, 0x0080
            JMP       L_b5d7
            MOV       ax, 0x0000
            MOV       cx, 0x0014
            MOV       bx, [bp+0x6]
            ADD       bx, cx
            MOV       cx, [bp+0x8]
            ADD       bx, cx
            MOV       cl, [bx]
            MOV       [bp-0x4], ax
            MOV       ax, cx
            AND       ax, 0x00ff
            AND       ax, 0x007f
            MOV       cx, [bp-0x4]
            OR        ax, cx
            MOV       cx, 0x0014
            MOV       bx, [bp+0x6]
            ADD       bx, cx
            MOV       cx, [bp+0x8]
            ADD       bx, cx
            MOV       [bx], al
            POP       di
            POP       si
            MOV       sp, bp
            POP       bp
            RETF`,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, gotErr := DecodeFunc(tt.ctx)
			if gotErr != nil {
				if !tt.wantErr {
					t.Errorf("DecodeFunc() failed: %v", gotErr)
				}
				return
			}
			if tt.wantErr {
				t.Fatal("DecodeFunc() succeeded unexpectedly")
			}
			var sb strings.Builder
			for _, inst := range got.Instrs {
				sb.WriteString(strings.TrimSpace(inst.String()))
				sb.WriteRune('\n')
			}
			var sbWant strings.Builder
			for inst := range strings.SplitSeq(tt.want, "\n") {
				sbWant.WriteString(strings.TrimSpace(inst))
				sbWant.WriteRune('\n')
			}
			if strings.TrimSpace(sb.String()) != strings.TrimSpace(sbWant.String()) {
				t.Errorf("DecodeFunc() = \n%v\n, want \n%v", strings.TrimSpace(sb.String()), strings.TrimSpace(sbWant.String()))
			}
		})
	}
}

func TestDecodeFuncJumpTable(t *testing.T) {
	img, err := OpenNE(starsExePath)
	if err != nil {
		t.Fatalf("open exe/ne %s: %v", starsExePath, err)
	}

	ctx := NewFuncContext(img,
		&typeinfo.Function{
			Name: "DoAiTurn",
			Addr: typeinfo.Addr{Seg: 0x12, Off: 0x0000},
			Len:  0x02ee,
		})
	got, err := DecodeFunc(ctx)
	if err != nil {
		t.Fatalf("DecodeFunc() failed: %v", err)
	}

	var lines []string
	for _, inst := range got.Instrs {
		if inst.Off >= 0x0220 && inst.Off <= 0x0239 {
			lines = append(lines, strings.TrimSpace(inst.String()))
		}
	}

	gotText := strings.Join(lines, "\n")
	want := strings.TrimSpace(`
SHL       ax, 0x0001
MOV       bx, ax
JMP       cs:[bx+0x229]
DW        L_01a5
DW        L_01d5
DW        L_01f5
DW        L_0205
DW        L_01b5
DW        L_01c5
DW        L_0215
DW        L_01e5
PUSH      [bp+0x6]`)
	if gotText != want {
		t.Fatalf("jump table decode =\n%s\nwant\n%s", gotText, want)
	}
	if strings.Contains(gotText, "MOVSW") {
		t.Fatal("jump table was decoded as instructions")
	}
}
