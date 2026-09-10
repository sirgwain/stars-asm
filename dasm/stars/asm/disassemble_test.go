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

// TestDecodeFuncJumpTable verifies decoding of the compiler's inline switch tables.
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

// TestDecodeFuncHighOffsetJumpTables verifies signed displacements preserve all
// table entries and make every case destination reachable in the real binary.
func TestDecodeFuncHighOffsetJumpTables(t *testing.T) {
	img, err := OpenNE(starsExePath)
	if err != nil {
		t.Fatalf("open exe/ne %s: %v", starsExePath, err)
	}

	tests := []struct {
		name    string
		fn      typeinfo.Function
		table   uint32
		targets []uint32
	}{
		{
			name:    "GetVCVal",
			fn:      typeinfo.Function{Name: "GetVCVal", Addr: typeinfo.Addr{Seg: 0x10, Off: 0xb710}, Len: 303},
			table:   0xb81f,
			targets: []uint32{0xb746, 0xb755, 0xb761, 0xb76d, 0xb77c, 0xb78b, 0xb78b, 0xb79a, 0xb7b8, 0xb7a9},
		},
		{
			name:  "PszFormatString",
			fn:    typeinfo.Function{Name: "PszFormatString", Addr: typeinfo.Addr{Seg: 7, Off: 0x85cc}, Len: 2459},
			table: 0x8ee1,
			targets: []uint32{
				0x8767, 0x8b11, 0x8c00, 0x8eba, 0x8791, 0x8eba, 0x8eba, 0x87d7, 0x8a21,
				0x8eba, 0x8c96, 0x8a33, 0x8eba, 0x8eba, 0x898e, 0x8eba, 0x8cfd, 0x8cfd,
				0x8eba, 0x8b0e, 0x8eba, 0x8850, 0x8eba, 0x8eba, 0x8eba, 0x8eba, 0x8eba,
				0x8eba, 0x8eba, 0x8eba, 0x8eba, 0x8eba, 0x8755, 0x864b, 0x8bec, 0x864b,
				0x87a9, 0x8b46, 0x8b5e, 0x87d7, 0x8a09, 0x8c2f, 0x8c7c, 0x8ad8, 0x8eba,
				0x864b, 0x8b26, 0x864b, 0x8ccf, 0x8cfd, 0x8628, 0x8eba, 0x864b, 0x8db5,
			},
		},
		{
			name:    "ExecuteReportClick/planet",
			fn:      typeinfo.Function{Name: "ExecuteReportClick", Addr: typeinfo.Addr{Seg: 0x22, Off: 0x7cd6}, Len: 2120},
			table:   0x8157,
			targets: []uint32{0x7d56, 0x7ddc, 0x7df9, 0x8171, 0x7df9, 0x8057, 0x8070, 0x8070, 0x7e1a, 0x7edf, 0x7edf, 0x7edf, 0x7e5e},
		},
		{
			name:    "ExecuteReportClick/fleet",
			fn:      typeinfo.Function{Name: "ExecuteReportClick", Addr: typeinfo.Addr{Seg: 0x22, Off: 0x7cd6}, Len: 2120},
			table:   0x82be,
			targets: []uint32{0x8235, 0x8235, 0x8235, 0x81e3, 0x81e3, 0x8273},
		},
		{
			name:  "FRunLogRecord",
			fn:    typeinfo.Function{Name: "FRunLogRecord", Addr: typeinfo.Addr{Seg: 0xa, Off: 0xa38c}, Len: 9238},
			table: 0xc738,
			targets: []uint32{
				0xa3a5, 0xad1d, 0xad1d, 0xbde1, 0xbf1c, 0xc155, 0xc721, 0xc721, 0xc721,
				0xc721, 0xc3f0, 0xc3f0, 0xc721, 0xc721, 0xc721, 0xc721, 0xc721, 0xc721,
				0xc721, 0xc721, 0xc721, 0xc721, 0xc721, 0xba30, 0xb819, 0xad1d, 0xc721,
				0xaa16, 0xc721, 0xa3a8, 0xc287, 0xc721, 0xc721, 0xc721, 0xc69b, 0xc50b,
				0xc65c, 0xb819, 0xa7dd, 0xc721, 0xc721, 0xc721, 0xc3b5, 0xa9bc, 0xa852,
				0xc721, 0xa807,
			},
		},
		{
			name:    "FTutorialEnabledShipBuilder",
			fn:      typeinfo.Function{Name: "FTutorialEnabledShipBuilder", Addr: typeinfo.Addr{Seg: 0x20, Off: 0x79f6}, Len: 2020},
			table:   0x816c,
			targets: []uint32{0x7ca7, 0x7ca4, 0x7ca4, 0x7ca4, 0x7ca4, 0x7ca4, 0x7ca4, 0x7d67, 0x7ca4, 0x7e24, 0x7ca4, 0x7ca4, 0x7ee6, 0x7ca4, 0x7f47, 0x7ca4, 0x8092},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := DecodeFunc(NewFuncContext(img, &tt.fn))
			if err != nil {
				t.Fatalf("DecodeFunc() failed: %v", err)
			}
			byOffset := make(map[uint32]DecodedInst, len(got.Instrs))
			for _, inst := range got.Instrs {
				byOffset[inst.Off] = inst
				if inst.Off >= tt.table && inst.Off < tt.table+uint32(len(tt.targets))*2 && inst.Op != OpDW {
					t.Errorf("table byte %#x decoded as code: %s", inst.Off, inst.String())
				}
			}
			for i, target := range tt.targets {
				off := tt.table + uint32(i)*2
				entry, ok := byOffset[off]
				if !ok || entry.Op != OpDW || entry.Target != int32(target) || entry.Len != 2 {
					t.Fatalf("table entry %#x = %+v, present=%v; want DW targeting %#x", off, entry, ok, target)
				}
				body, ok := byOffset[target]
				if !ok || body.Op == OpDW {
					t.Errorf("case destination %#x not decoded as code", target)
				}
			}
		})
	}
}
