package typeinfo

import "testing"

func TestFunctionType(t *testing.T) {
	f := Function{Ret: &Primitive{TypeKind: KVoid}}

	if f.Kind() != KFunc {
		t.Fatalf("Kind() = %v, want %v", f.Kind(), KFunc)
	}
	if f.Bytes() != 0 {
		t.Fatalf("Bytes() = %d, want 0", f.Bytes())
	}
}

func TestFunctionInScope(t *testing.T) {
	fn := Function{
		Name: "ChgPopFromPlanet",
		Addr: Addr{Seg: 0x0008, Off: 0x7082},
		Len:  1375,
		Scopes: []Scope{
			{ID: 0, ParentID: -1},
			{ID: 1, ParentID: 0, Depth: 1, Addr: Addr{Seg: 0x0008, Off: 0x72B1}, Len: 0x1B1},
		},
	}

	rootLocal := FunctionVar{Name: "lPopOld", BPOffset: -32, ScopeID: 0}
	blockLocal := FunctionVar{Name: "pctFull", BPOffset: -40, ScopeID: 1}

	tests := []struct {
		name string
		v    FunctionVar
		off  uint32
		want bool
	}{
		{name: "root before function", v: rootLocal, off: 0x7081, want: false},
		{name: "root at function start", v: rootLocal, off: 0x7082, want: true},
		{name: "root inside function", v: rootLocal, off: 0x72B1, want: true},
		{name: "root at final byte", v: rootLocal, off: 0x75E0, want: true},
		{name: "root at function end", v: rootLocal, off: 0x75E1, want: false},
		{name: "block before start", v: blockLocal, off: 0x72B0, want: false},
		{name: "block at start", v: blockLocal, off: 0x72B1, want: true},
		{name: "block inside", v: blockLocal, off: 0x7300, want: true},
		{name: "block at final byte", v: blockLocal, off: 0x7461, want: true},
		{name: "block at end", v: blockLocal, off: 0x7462, want: false},
		{name: "missing scope", v: FunctionVar{Name: "bad", ScopeID: 2}, off: 0x72B1, want: false},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := fn.InScope(tt.v, tt.off); got != tt.want {
				t.Fatalf("InScope(%s, 0x%04X) = %v, want %v", tt.v.Name, tt.off, got, tt.want)
			}
		})
	}
}
