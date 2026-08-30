package stars

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func TestBuildGraphDataFromCFGFiltersRange(t *testing.T) {
	fn := &typeinfo.Function{Name: "Test"}
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	ctx := machine.NewFuncContext(nil, sdb, symresolve.NewResolver(img, sdb), fn)
	cfg, err := machine.BuildCFG(ctx, []asm.DecodedInst{
		{Off: 0x1000, Len: 2, Op: asm.OpJcc, Mnemonic: "JZ", Target: 0x1006},
		{Off: 0x1002, Len: 2, Mnemonic: "NOP"},
		{Off: 0x1004, Len: 2, Mnemonic: "NOP"},
		{Off: 0x1006, Len: 2, Op: asm.OpRET, Mnemonic: "RET"},
	}, false, machine.CFGOptions{})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}

	data, err := buildGraphViewDataFromCFG(cfg, fn, DumpOptions{
		FromAddr: 0x1003,
		ToAddr:   0x1006,
	}, nil)
	if err != nil {
		t.Fatalf("buildGraphDataFromCFG: %v", err)
	}

	if got, want := graphNodeIDs(data.Nodes), []string{"L_1002", "L_1006"}; !sameStrings(got, want) {
		t.Fatalf("nodes = %#v, want %#v", got, want)
	}
	if got, want := graphEdgePairs(data.Edges), []string{"L_1002->L_1006"}; !sameStrings(got, want) {
		t.Fatalf("edges = %#v, want %#v", got, want)
	}
	if got, want := len(data.SCCs), 2; got != want {
		t.Fatalf("SCC count = %d, want %d", got, want)
	}
}

func TestBuildGraphDataFromCFGHidesPrivateJumpTrampolines(t *testing.T) {
	fn := &typeinfo.Function{Name: "Test"}
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	ctx := machine.NewFuncContext(nil, sdb, symresolve.NewResolver(img, sdb), fn)
	cfg, err := machine.BuildCFG(ctx, []asm.DecodedInst{
		{Off: 0x1000, Len: 2, Op: asm.OpJcc, Mnemonic: "JGE", Target: 0x1010},
		{Off: 0x1002, Len: 2, Op: asm.OpJMP, Mnemonic: "JMP", Target: 0x1012},
		{Off: 0x1010, Len: 1, Op: asm.OpRET, Mnemonic: "RET"},
		{Off: 0x1012, Len: 1, Op: asm.OpRET, Mnemonic: "RET"},
	}, false, machine.CFGOptions{CollapseJumps: true})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}

	data, err := buildGraphViewDataFromCFG(cfg, fn, DumpOptions{}, nil)
	if err != nil {
		t.Fatalf("buildGraphDataFromCFG: %v", err)
	}

	if got, want := graphNodeIDs(data.Nodes), []string{"L_1000", "L_1010", "L_1012"}; !sameStrings(got, want) {
		t.Fatalf("nodes = %#v, want %#v", got, want)
	}
	if got, want := graphEdgePairs(data.Edges), []string{"L_1000->L_1010", "L_1000->L_1012"}; !sameStrings(got, want) {
		t.Fatalf("edges = %#v, want %#v", got, want)
	}
	if got, want := graphEdgeKinds(data.Edges), map[string]string{
		"L_1000->L_1010": "fallback",
		"L_1000->L_1012": "true",
	}; !sameStringMap(got, want) {
		t.Fatalf("edge kinds = %#v, want %#v", got, want)
	}
}

func TestBuildGraphDataFromCFGUsesCallOverrides(t *testing.T) {
	fn := &typeinfo.Function{Name: "Test"}
	img := &asm.ImageNE{}
	sdb := &typeinfo.SymbolDB{}
	ctx := machine.NewFuncContext(nil, sdb, symresolve.NewResolver(img, sdb), fn)
	cfg, err := machine.BuildCFG(ctx, []asm.DecodedInst{
		{
			Off:      0x1000,
			Len:      5,
			Op:       asm.OpCALLF,
			Mnemonic: "CALLF",
			Dst:      asm.Operand{Kind: asm.OKImm, Imm: 0},
			Src:      asm.Operand{Kind: asm.OKImm, Imm: 0xffff},
		},
		{Off: 0x1005, Len: 1, Op: asm.OpRET, Mnemonic: "RET"},
	}, false, machine.CFGOptions{})
	if err != nil {
		t.Fatalf("BuildCFG: %v", err)
	}
	cfg.Calls[0x1000] = &machine.InstCall{Target: &typeinfo.Function{
		Name: "DrawIcon",
		Ret:  signedInt16Type(),
		Params: []typeinfo.FunctionVar{
			{Type: typeinfo.U16},
			{Type: signedInt16Type()},
			{Type: signedInt16Type()},
			{Type: typeinfo.U16},
		},
	}}

	data, err := buildGraphViewDataFromCFG(cfg, fn, DumpOptions{}, nil)
	if err != nil {
		t.Fatalf("buildGraphDataFromCFG: %v", err)
	}
	if got, want := data.Nodes[0].Instructions[0], "CALLF     DrawIcon                  ; int16_t DrawIcon(uint16_t, int16_t, int16_t, uint16_t)"; got != want {
		t.Fatalf("call instruction = %q, want %q", got, want)
	}
}

func signedInt16Type() typeinfo.Type {
	return &typeinfo.Primitive{TypeKind: typeinfo.KInt, Name: "int16_t", Size: 2, Signed: true}
}

func graphNodeIDs(nodes []FuncGraphViewNode) []string {
	out := make([]string, 0, len(nodes))
	for _, node := range nodes {
		out = append(out, node.ID)
	}
	return out
}

func graphEdgePairs(edges []FuncGraphViewEdge) []string {
	out := make([]string, 0, len(edges))
	for _, edge := range edges {
		out = append(out, edge.Source+"->"+edge.Target)
	}
	return out
}

func graphEdgeKinds(edges []FuncGraphViewEdge) map[string]string {
	out := make(map[string]string, len(edges))
	for _, edge := range edges {
		out[edge.Source+"->"+edge.Target] = edge.Kind
	}
	return out
}

func sameStrings(a, b []string) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}

func sameStringMap(a, b map[string]string) bool {
	if len(a) != len(b) {
		return false
	}
	for key, val := range a {
		if b[key] != val {
			return false
		}
	}
	return true
}
