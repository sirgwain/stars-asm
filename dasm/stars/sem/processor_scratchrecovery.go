package sem

import "slices"

type scratchRecoveryProcessor struct{}

// ProcessFunc analyzes storage before choosing and applying semantic replacements.
func (p *scratchRecoveryProcessor) ProcessFunc(result *Result, f *Func) bool {
	a := analyzeScratchStorage(f)
	if len(a.objects) == 0 {
		return false
	}
	// Wide pairs are normalized on a private body before the data-flow analysis.
	// Only explicit covering accesses or matching projections justify a merge.
	body := *f
	body.Blocks = slices.Clone(f.Blocks)
	paired := recoverScratchWideDefinitions(&body, a)
	if paired {
		a = analyzeScratchStorage(&body)
	}
	a.solve(&body)
	replacements := buildScratchRecoveryPlan(&body, a)
	changed := applyScratchRecoveryPlan(&body, a, replacements)
	changed = inlineScratchAssignments(&body, a) || changed
	if changed || paired {
		*f = body
	}
	return changed || paired
}
