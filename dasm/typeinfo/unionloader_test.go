package typeinfo

import (
	"encoding/json"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/nb09"
)

// TestBlockMemberFactsLoadAndReplay checks direct selections against the real
// layouts, including independent regions, nested roots, and extension replay.
func TestBlockMemberFactsLoadAndReplay(t *testing.T) {
	db, err := nb09.Load("../input/stars.exe")
	if err != nil {
		t.Fatal(err)
	}
	sdb, err := Load("../input", db)
	if err != nil {
		t.Fatal(err)
	}
	base := BlockMemberFactJSON{
		Func: "FIsAiAttack", Root: "rglpshdef", AllElements: true,
		Type: "SHDEF", Block: "L_4ad0", Member: "lPower",
	}
	flags := base
	flags.Member = "wFlags"
	nested := BlockMemberFactJSON{
		Func: base.Func, Root: "lpfl", RootPath: []string{"lpflNext"},
		Type: "FLEET", Block: base.Block, Member: "lPower",
	}
	callResult := BlockMemberFactJSON{
		Func: "FCanKillTok", CallResult: "LpshdefFromTok", Type: "SHDEF", Block: "L_391e", Member: "lPower",
	}
	data, err := json.Marshal(unionConfigJSON{BlockMemberFacts: []BlockMemberFactJSON{base, flags, nested, callResult}})
	if err != nil {
		t.Fatal(err)
	}
	path := filepath.Join(t.TempDir(), "unions-members.json")
	if err := os.WriteFile(path, data, 0600); err != nil {
		t.Fatal(err)
	}
	loader := unionLoader{}
	rules, err := loader.loadUnionRules(path, sdb)
	if err != nil {
		t.Fatal(err)
	}
	if err := loader.appendUnionFacts(path, sdb, rules); err != nil {
		t.Fatal(err)
	}
	fn := sdb.GetFunction(base.Func)
	facts := rules.BlockMemberFactsFor(fn, 0x4ad0)
	if len(facts) != 3 || len(rules.BlockMemberFacts) != 4 {
		t.Fatalf("replay duplicated facts: %d indexed, %d total", len(facts), len(rules.BlockMemberFacts))
	}
	if facts[0].Region.Start != 0x87 || facts[1].Region.Start != 0x7b || facts[2].Member.Offset != 0x6c {
		t.Fatalf("wrong overlap regions or nested member: %+v", facts)
	}
	if len(rules.BlockMemberFactsFor(fn, 0x4af5)) != 0 {
		t.Fatal("direct fact expanded to an unconfigured block")
	}
	callFacts := rules.BlockMemberFactsFor(sdb.GetFunction(callResult.Func), 0x391e)
	if len(callFacts) != 1 || callFacts[0].CallResult != sdb.GetFunction(callResult.CallResult) || callFacts[0].Root != "" {
		t.Fatalf("call result facts did not replay: %+v", callFacts)
	}
	conflict := base
	conflict.Member = "lVisible"
	if err := loader.appendBlockMemberFact(conflict, sdb, rules); err == nil || !strings.Contains(err.Error(), "conflicting") {
		t.Fatalf("conflicting region choice error = %v", err)
	}

	tests := []struct {
		name   string
		change func(*BlockMemberFactJSON)
		want   string
	}{
		{"function", func(f *BlockMemberFactJSON) { f.Func = "MissingFunction" }, "function MissingFunction not found"},
		{"block syntax", func(f *BlockMemberFactJSON) { f.Block = "L_invalid" }, "invalid hexadecimal"},
		{"block range", func(f *BlockMemberFactJSON) { f.Block = "L_0001" }, "outside the function"},
		{"root", func(f *BlockMemberFactJSON) { f.Root = "missing" }, "root path missing not found"},
		{"root path", func(f *BlockMemberFactJSON) { f.RootPath = []string{"missing"} }, "not found"},
		{"type", func(f *BlockMemberFactJSON) { f.Type = "MissingType" }, "type MissingType not found"},
		{"collection type", func(f *BlockMemberFactJSON) { f.Type = "FLEET" }, "not FLEET"},
		{"element scope", func(f *BlockMemberFactJSON) { f.AllElements = false }, "not SHDEF"},
		{"member", func(f *BlockMemberFactJSON) { f.Member = "missing" }, "member SHDEF.missing not found"},
		{"ordinary field", func(f *BlockMemberFactJSON) { f.Member = "turn" }, "does not belong to an overlap region"},
		{"missing selector", func(f *BlockMemberFactJSON) { f.Root = "" }, "exactly one of root or call_result"},
		{"both selectors", func(f *BlockMemberFactJSON) { f.CallResult = "LpshdefFromTok" }, "exactly one of root or call_result"},
		{"unknown callee", func(f *BlockMemberFactJSON) { *f = callResult; f.CallResult = "MissingFunction" }, "call_result function MissingFunction not found"},
		{"callee return", func(f *BlockMemberFactJSON) { *f = callResult; f.CallResult = "LComputePower" }, "not SHDEF"},
		{"call result scope", func(f *BlockMemberFactJSON) { *f = callResult; f.AllElements = true }, "cannot use root_path or all_elements"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			cfg := base
			tt.change(&cfg)
			if err := loader.appendBlockMemberFact(cfg, sdb, emptyUnionRules()); err == nil || !strings.Contains(err.Error(), tt.want) {
				t.Fatalf("error = %v, want %q", err, tt.want)
			}
		})
	}
}
