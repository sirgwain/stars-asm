package stars

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestResolver_ResolveGlobal(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	tests := []struct {
		name         string
		seg          uint16
		off          uint32
		width        int
		want         string
		wantFieldOff int
	}{
		{
			name: "rgplr exact",
			seg:  0x25, off: 0x59a2, width: 2, want: "rgplr", wantFieldOff: 0,
		},
		{
			name: "rgplr second word still resolves containing global",
			seg:  0x25, off: 0x59a4, width: 2, want: "rgplr", wantFieldOff: 2,
		},
		{
			name: "lpPlanets low word",
			seg:  0x25, off: 0x00f6, width: 2, want: "lpPlanets", wantFieldOff: 0,
		},
		{
			name: "lpPlanets high word",
			seg:  0x25, off: 0x00f8, width: 2, want: "lpPlanets", wantFieldOff: 2,
		},
		{
			name: "sel+off (sel.fl)",
			seg:  0x25, off: 0x4972, width: 2, want: "sel", wantFieldOff: 0x1c,
		},
		{
			name: "GlobalPD+off",
			seg:  0x25, off: 0x0b80, width: 2, want: "GlobalPD", wantFieldOff: 0,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := res.ResolveGlobal(tt.seg, tt.off, tt.width)
			if !ok {
				t.Fatalf("ResolveGlobalContaining(%d, %#x, %d) ok = false", tt.seg, tt.off, tt.width)
			}
			if got.Global == nil || got.Global.Name != tt.want {
				t.Fatalf("global = %v, want %q", got, tt.want)
			}
			if got.FieldOff != tt.wantFieldOff {
				t.Fatalf("FieldOff = %d, want %d", got.FieldOff, tt.wantFieldOff)
			}
		})
	}
}

func TestResolver_ResolveLiteral(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	tests := []struct {
		name   string
		seg    uint16
		off    uint32
		want   string
		wantOk bool
	}{
		{
			name: "backup.%03d",
			seg:  0x25, off: 0x9f0, want: "\"backup.%03d\"", wantOk: true,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := res.ResolveLiteral(tt.seg, tt.off)
			if !ok {
				t.Fatalf("ResolveLiteral(%d, %#x) ok = false", tt.seg, tt.off)
			}
			if ok != tt.wantOk {
				t.Fatalf("ok = %v, wantOk %v", ok, tt.wantOk)
			}
			if ok && got != tt.want {
				t.Fatalf("got = %s, want %s", got, tt.want)
			}
		})
	}
}

func TestResolver_ResolveLocal(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	fn := fx.SDB.GetFunction("ChgPopFromPlanet")
	if fn == nil {
		t.Fatal("ChgPopFromPlanet not found")
	}

	tests := []struct {
		name         string
		off          uint32
		bpOff        int
		wantOK       bool
		want         string
		wantFieldOff int
	}{
		{
			name: "param lppl exact",
			off:  0x7082, bpOff: 6, wantOK: true, want: "lppl", wantFieldOff: 0,
		},
		{
			name: "param lppl high word",
			off:  0x7082, bpOff: 8, wantOK: true, want: "lppl", wantFieldOff: 2,
		},
		{
			name: "param fUpdate exact",
			off:  0x7082, bpOff: 10, wantOK: true, want: "fUpdate", wantFieldOff: 0,
		},
		{
			name: "root local lPopOld exact",
			off:  0x7082, bpOff: -32, wantOK: true, want: "lPopOld", wantFieldOff: 0,
		},
		{
			name: "root local lPopOld high word",
			off:  0x7082, bpOff: -30, wantOK: true, want: "lPopOld", wantFieldOff: 2,
		},
		{
			name:   "scoped local pctFull before block",
			off:    0x72B0,
			bpOff:  -40,
			wantOK: false,
		},
		{
			name: "scoped local pctFull at block start",
			off:  0x72B1, bpOff: -40, wantOK: true, want: "pctFull", wantFieldOff: 0,
		},
		{
			name: "scoped local pctRetard high word",
			off:  0x72B1, bpOff: -34, wantOK: true, want: "pctRetard", wantFieldOff: 2,
		},
		{
			name: "scoped local pctFull at final byte",
			off:  0x7461, bpOff: -40, wantOK: true, want: "pctFull", wantFieldOff: 0,
		},
		{
			name:   "scoped local pctFull at block end",
			off:    0x7462,
			bpOff:  -40,
			wantOK: false,
		},
		{
			name:   "unknown bp offset",
			off:    0x7082,
			bpOff:  -100,
			wantOK: false,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := res.ResolveLocal(fn, tt.off, tt.bpOff)
			if ok != tt.wantOK {
				t.Fatalf("ResolveLocal(%s, %#x, %d) ok = %v, want %v", fn.Name, tt.off, tt.bpOff, ok, tt.wantOK)
			}
			if !tt.wantOK {
				return
			}
			if got.Local.Name != tt.want {
				t.Fatalf("local = %v, want %q", got.Local.Name, tt.want)
			}
			if got.FieldOff != tt.wantFieldOff {
				t.Fatalf("FieldOff = %d, want %d", got.FieldOff, tt.wantFieldOff)
			}
		})
	}
}

func TestResolver_ResolveFunction(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	tests := []struct {
		name  string
		fixup asm.Fixup
		want  string
	}{
		{
			name:  "CALLF 0x000a:0x7096 -> CalcPlanetMaxPop",
			fixup: asm.Fixup{Target: asm.FixupTargetInternalRef, ResolvedSeg: 0x000a, ResolvedOff: 0x7096},
			want:  "CalcPlanetMaxPop",
		},
		{
			name:  "CALLF KERNEL:FREEPROCINSTANCE -> CalcPlanetMaxPop",
			fixup: asm.Fixup{Target: asm.FixupTargetImportOrdinal, ModuleName: "KERNEL", FuncName: "FREEPROCINSTANCE"},
			want:  "FreeProcInstance",
		},
		// TODO: still need to support resolving non-fixups, probably in a different function
		// {
		// 	name:  "CALLF [bx+0x6] -> CalcPlanetMaxPop",
		// 	fixup: asm.Fixup{},
		// 	want:  "ptile[i].pfn()",
		// },
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := res.ResolveFunction(&tt.fixup)
			if !ok {
				t.Fatalf("ResolveFunction(%v) ok = false", tt.fixup)
			}
			if got == nil || got.Name != tt.want {
				t.Fatalf("func = %v, want %q", got.Name, tt.want)
			}
		})
	}
}

func TestResolver_ResolveFloatLiteral(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	tests := []struct {
		name   string
		seg    int
		off    uint32
		bytes  int
		wantOK bool
		want   float64
	}{
		{
			name: "0x1e1e",
			seg:  0x25, off: 0x1e1e, bytes: 8, wantOK: true, want: .999,
		},
		{
			name: "0x1c9e",
			seg:  0x25, off: 0x1c9e, bytes: 4, wantOK: true, want: 1.0,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := res.ResolveFloatLiteral(tt.seg, tt.off, tt.bytes)
			if ok != tt.wantOK {
				t.Fatalf("ResolveFloatLiteral(%x, %04x) ok = %v, want %v", tt.seg, tt.off, ok, tt.wantOK)
			}
			if !tt.wantOK {
				return
			}
			if got != tt.want {
				t.Fatalf("local = %v, want %f", got, tt.want)
			}
		})
	}
}

func TestResolver_ResolveField(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	// helper variables
	plVar := &typeinfo.FunctionVar{Name: "pl", Type: fx.SDB.GetStruct("PLANET")}
	lpflVar := &typeinfo.FunctionVar{Name: "lpfl", Type: &typeinfo.Pointer{Elem: fx.SDB.GetStruct("FLEET"), Class: typeinfo.PtrFar}}
	selGlobalVar := fx.SDB.GetGlobal("sel")

	tests := []struct {
		name    string
		v       typeinfo.Var
		off     int
		wantOK  bool
		wantOff int
		want    string
	}{
		{
			name: "pl",
			v:    plVar, off: 0,
			wantOK: true, wantOff: 0, want: "pl.id",
		},
		{
			name: "pl+0x2",
			v:    plVar, off: 2,
			wantOK: true, wantOff: 0, want: "pl.iPlayer",
		},
		{
			name: "lpfl+0x64",
			v:    lpflVar, off: 0x64,
			wantOK: true, wantOff: 0, want: "lpfl->lpplord",
		},
		{
			name: "pl+0x38 - no field",
			v:    plVar, off: 0x38,
			wantOK: false,
		},
		{
			name: "sel+0xcc",
			v:    selGlobalVar, off: 0xcc,
			wantOK: true, wantOff: 0, want: "sel.pl.lpplprod",
		},
		{
			name: "sel+0xce",
			v:    selGlobalVar, off: 0xce,
			wantOK: true, wantOff: 2, want: "sel.pl.lpplprod",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, off, ok := res.ResolveField(tt.v, tt.off)
			if tt.wantOK != ok {
				t.Fatalf("TestResolver_ResolveField(%v, %#x) ok = %v, wantOk = %v", tt.v, tt.off, ok, tt.wantOK)
			}

			if off != tt.wantOff {
				t.Fatalf("TestResolver_ResolveField(%v, %#x) off = %d, wantOff = %d", tt.v, tt.off, tt.off, tt.wantOff)
			}

			if ok && got.CDecl() != tt.want {
				t.Fatalf("TestResolver_ResolveField(%v, %#x) got = %s, want = %s", tt.v, tt.off, got.CDecl(), tt.want)
			}
		})
	}
}

func TestResolver_ResolveFieldLoad(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)
	unionCtx := symresolve.NewUnionContext()

	// helper variables
	plVar := &typeinfo.FunctionVar{Name: "pl", Type: fx.SDB.GetStruct("PLANET")}
	partVar := &typeinfo.FunctionVar{Name: "part", Type: fx.SDB.GetStruct("PART")}
	vlpbdVCRGlobal := fx.SDB.GetGlobal("vlpbdVCR")
	selGlobalVar := fx.SDB.GetGlobal("sel")

	tests := []struct {
		name        string
		v           typeinfo.Var
		off         int
		accessWidth int
		wantOK      bool
		want        string
	}{
		{
			name: "pl",
			v:    plVar, off: 0, accessWidth: 2,
			wantOK: true, want: "pl.id",
		},
		{
			name: "pl 1 byte load",
			v:    plVar, off: 0, accessWidth: 1,
			wantOK: false,
		},
		{
			name: "pl+0x2",
			v:    plVar, off: 2, accessWidth: 2,
			wantOK: true, want: "pl.iPlayer",
		},
		{
			name: "part.hs.grhst",
			v:    partVar, off: 0, accessWidth: 2,
			wantOK: true, want: "part.hs.grhst",
		},
		{
			name: "part default union member",
			v:    partVar, off: 4, accessWidth: 4,
			wantOK: true, want: "part.pcom",
		},
		{
			name: "vlpbdVCR+0x3",
			v:    vlpbdVCRGlobal, off: 0x3, accessWidth: 1,
			wantOK: true, want: "vlpbdVCR->ctok",
		},
		{
			name: "sel+0x8c",
			v:    selGlobalVar, off: 0x7c, accessWidth: 1,
			wantOK: true, want: "sel.fl.iplan",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := res.ResolveFieldLoadInContext(tt.v, tt.off, tt.accessWidth, unionCtx)
			if tt.wantOK != ok {
				t.Fatalf("TestResolver_ResolveField(%v, %#x) ok = %v, wantOk = %v", tt.v, tt.off, ok, tt.wantOK)
			}

			if ok && got.CDecl() != tt.want {
				t.Fatalf("TestResolver_ResolveField(%v, %#x) got = %s, want = %s", tt.v, tt.off, got.CDecl(), tt.want)
			}
		})
	}
}

func TestResolver_ResolveFieldPathLoad(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	// helper paths
	plVar := &typeinfo.FunctionVar{Name: "pl", Type: fx.SDB.GetStruct("PLANET")}
	plRoot := &symresolve.SymbolRoot{Symbol: plVar}
	lpplVar := &typeinfo.FunctionVar{Name: "lppl", Type: &typeinfo.Pointer{Elem: fx.SDB.GetStruct("PLANET"), Class: typeinfo.PtrFar}}
	lpplRoot := &symresolve.SymbolRoot{Symbol: lpplVar}
	selGlobal := fx.SDB.GetGlobal("sel")
	lpplprod, _, ok := res.ResolveField(selGlobal, 0xcc)
	if !ok {
		t.Fatal("sel.pl.lpplprod not found")
	}

	tests := []struct {
		name        string
		base        symresolve.SymbolPath
		off         int
		accessWidth int
		wantOK      bool
		want        string
	}{
		{
			name: "pl path +0x2",
			base: plRoot, off: 2, accessWidth: 2,
			wantOK: true, want: "pl.iPlayer",
		},
		{
			name: "sel.pl.lpplprod +0x2",
			base: lpplprod, off: 2, accessWidth: 1,
			wantOK: true, want: "sel.pl.lpplprod->iprodMax",
		},
		{
			name: "sel.pl.lpplprod +0x3",
			base: lpplprod, off: 3, accessWidth: 1,
			wantOK: true, want: "sel.pl.lpplprod->iprodMac",
		},
		{
			name: "sel.pl.lpplprod 1 byte load over bitfields",
			base: lpplprod, off: 0, accessWidth: 1,
			wantOK: false,
		},
		{
			name: "lppl path +0x2c wide union field",
			base: lpplRoot, off: 0x2c, accessWidth: 4,
			wantOK: true, want: "lppl->lStarbase",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := res.ResolveFieldPathLoad(tt.base, tt.off, tt.accessWidth)
			if tt.wantOK != ok {
				t.Fatalf("TestResolver_ResolveFieldPathLoad(%v, %#x) ok = %v, wantOk = %v", tt.base.CDecl(), tt.off, ok, tt.wantOK)
			}

			if ok && got.CDecl() != tt.want {
				t.Fatalf("TestResolver_ResolveFieldPathLoad(%v, %#x) got = %s, want = %s", tt.base.CDecl(), tt.off, got.CDecl(), tt.want)
			}
		})
	}
}

func TestResolver_ResolveBitfieldLoad(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	gdGlobal := fx.SDB.GetGlobal("gd")
	lpplVar := &typeinfo.FunctionVar{Name: "lppl", Type: &typeinfo.Pointer{Elem: fx.SDB.GetStruct("PLANET"), Class: typeinfo.PtrFar}}

	tests := []struct {
		name         string
		v            typeinfo.Var
		off          int
		storageWidth int
		bitOff       int
		bitWidth     int
		wantOK       bool
		want         string
	}{
		{
			name: "gd+0x2 bit 10",
			v:    gdGlobal, off: 2, storageWidth: 2, bitOff: 10, bitWidth: 1,
			wantOK: true, want: "gd.fProgressTxt",
		},
		{
			name: "gd+0x2 bit 15",
			v:    gdGlobal, off: 2, storageWidth: 2, bitOff: 15, bitWidth: 1,
			wantOK: true, want: "gd.fToolbar",
		},
		{
			name: "gd+0x2 wrong width",
			v:    gdGlobal, off: 2, storageWidth: 1, bitOff: 10, bitWidth: 1,
			wantOK: false,
		},
		{
			name: "gd+0x2 no matching bit width",
			v:    gdGlobal, off: 2, storageWidth: 2, bitOff: 10, bitWidth: 2,
			wantOK: false,
		},
		{
			name: "lppl+0x18 cDefenses low word",
			v:    lpplVar, off: 0x18, storageWidth: 2, bitOff: 0, bitWidth: 12,
			wantOK: true, want: "lppl->cDefenses",
		},
		{
			name: "lppl+0x1a fNoResearch high word",
			v:    lpplVar, off: 0x1a, storageWidth: 2, bitOff: 7, bitWidth: 1,
			wantOK: true, want: "lppl->fNoResearch",
		},
		{
			name: "lppl+0x18 partial iScanner low word",
			v:    lpplVar, off: 0x18, storageWidth: 2, bitOff: 12, bitWidth: 4,
			wantOK: false,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := res.ResolveBitfieldLoad(tt.v, tt.off, tt.storageWidth, tt.bitOff, tt.bitWidth)
			if tt.wantOK != ok {
				t.Fatalf("TestResolver_ResolveBitfieldLoad(%v, %#x, %d, %d, %d) ok = %v, wantOk = %v", tt.v, tt.off, tt.storageWidth, tt.bitOff, tt.bitWidth, ok, tt.wantOK)
			}

			if ok && got.CDecl() != tt.want {
				t.Fatalf("TestResolver_ResolveBitfieldLoad(%v, %#x, %d, %d, %d) got = %s, want = %s", tt.v, tt.off, tt.storageWidth, tt.bitOff, tt.bitWidth, got.CDecl(), tt.want)
			}
		})
	}
}

func TestResolver_ResolveBitfieldPathLoad(t *testing.T) {
	fx := testfixture.Stars(t)
	res := symresolve.NewResolver(fx.Image, fx.SDB)

	gdRoot := &symresolve.SymbolRoot{Symbol: fx.SDB.GetGlobal("gd")}

	tests := []struct {
		name         string
		base         symresolve.SymbolPath
		off          int
		storageWidth int
		bitOff       int
		bitWidth     int
		wantOK       bool
		want         string
	}{
		{
			name: "gd path +0x2 bit 10",
			base: gdRoot, off: 2, storageWidth: 2, bitOff: 10, bitWidth: 1,
			wantOK: true, want: "gd.fProgressTxt",
		},
		{
			name: "gd path +0x2 wrong bit",
			base: gdRoot, off: 2, storageWidth: 2, bitOff: 9, bitWidth: 1,
			wantOK: true, want: "gd.fRptSafeDraw",
		},
		{
			name: "gd path +0x2 no matching mask width",
			base: gdRoot, off: 2, storageWidth: 2, bitOff: 9, bitWidth: 2,
			wantOK: false,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, ok := res.ResolveBitfieldPathLoad(tt.base, tt.off, tt.storageWidth, tt.bitOff, tt.bitWidth)
			if tt.wantOK != ok {
				t.Fatalf("TestResolver_ResolveBitfieldPathLoad(%v, %#x, %d, %d, %d) ok = %v, wantOk = %v", tt.base.CDecl(), tt.off, tt.storageWidth, tt.bitOff, tt.bitWidth, ok, tt.wantOK)
			}

			if ok && got.CDecl() != tt.want {
				t.Fatalf("TestResolver_ResolveBitfieldPathLoad(%v, %#x, %d, %d, %d) got = %s, want = %s", tt.base.CDecl(), tt.off, tt.storageWidth, tt.bitOff, tt.bitWidth, got.CDecl(), tt.want)
			}
		})
	}
}
