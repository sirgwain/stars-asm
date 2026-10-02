package stars

import (
	"fmt"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"slices"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// NegativeIndexStage is the semantic pass whose output the -1 index audit
// analyzes: the IR the native-negative-index pass receives.
const NegativeIndexStage = "native-macinti"

// NegativeIndexAudit lists -1 index semantics derived for the program and
// the array accesses that may still index with -1.
type NegativeIndexAudit struct {
	// Returns are functions derived to return may_be_minus_one values.
	Returns []string
	// Stores maps functions to the pointer parameters derived to receive -1.
	Stores map[string][]string
	Sites  []NegativeIndexAuditSite
}

// NegativeIndexAuditSite is one array access that may index with -1.
type NegativeIndexAuditSite struct {
	Function string
	Block    machine.BlockID
	Index    string
	Access   string
	Effect   string
}

// AuditNegativeIndexes decompiles every function to NegativeIndexStage and
// runs the -1 dataflow, adding derived may_be_minus_one semantics to the
// symbol database until no function gains one, then reports the sites.
func AuditNegativeIndexes(img *asm.ImageNE, sdb *typeinfo.SymbolDB) (NegativeIndexAudit, error) {
	funcs := make([]*typeinfo.Function, 0, len(sdb.Functions))
	for _, f := range sdb.Functions {
		if !f.IsOverride() {
			funcs = append(funcs, f)
		}
	}
	slices.SortFunc(funcs, func(a, b *typeinfo.Function) int { return strings.Compare(a.Name, b.Name) })
	audit := NegativeIndexAudit{Stores: map[string][]string{}}
	writes := machine.NewWriteSummaries(img, sdb, symresolve.NewResolver(img, sdb))
	for {
		changed := false
		audit.Sites = nil
		for _, fn := range funcs {
			var facts sem.NegativeIndexFacts
			var found bool
			_, err := analyzeFuncWithSemPassSnapshots(img, sdb, fn, DumpOptions{}, writes, func(snapshot sem.PassSnapshot, _ *machine.FuncEffects) error {
				if snapshot.Name == NegativeIndexStage {
					facts = sem.AnalyzeNegativeIndexes(&snapshot.Func, fn)
					for _, site := range facts.Sites {
						block := snapshot.Func.Blocks[slices.IndexFunc(snapshot.Func.Blocks, func(b sem.Block) bool { return b.ID == site.Block })]
						audit.Sites = append(audit.Sites, NegativeIndexAuditSite{Function: fn.Name, Block: site.Block,
							Index: site.Index.Name, Access: sem.FormatExpr(site.Access), Effect: sem.FormatEffect(block.Effects[site.Effect])})
					}
					found = true
				}
				return nil
			})
			if err != nil {
				return NegativeIndexAudit{}, fmt.Errorf("%s: %w", fn.Name, err)
			}
			if !found {
				return NegativeIndexAudit{}, fmt.Errorf("%s: no %s pass snapshot", fn.Name, NegativeIndexStage)
			}
			if facts.ReturnsNeg && fn.RetSemantic == "" {
				fn.RetSemantic = typeinfo.SemanticMayBeMinusOne
				audit.Returns = append(audit.Returns, fn.Name)
				changed = true
			}
			for _, i := range facts.StoresNegTo {
				if fn.Params[i].Semantic == "" {
					fn.Params[i].Semantic = typeinfo.SemanticMayBeMinusOne
					audit.Stores[fn.Name] = append(audit.Stores[fn.Name], fn.Params[i].Name)
					changed = true
				}
			}
		}
		if !changed {
			break
		}
	}
	slices.Sort(audit.Returns)
	return audit, nil
}
