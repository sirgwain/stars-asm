package ir

import "strings"

type AnalyzeResult struct {
	NumStatements        int            `json:"numStatements,omitempty"`
	Untranslated         int            `json:"untranslated,omitempty"`
	UntranslatedAssign   int            `json:"untranslatedAssign,omitempty"`
	UntranslatedBitwise  int            `json:"untranslatedBitwise,omitempty"`
	UntranslatedBranch   int            `json:"untranslatedBranch,omitempty"`
	UntranslatedPart     int            `json:"untranslatedPart,omitempty"`
	UntranslatedScratch  int            `json:"untranslatedScratch,omitempty"`
	UntranslatedFailures map[string]int `json:"untranslatedFailures,omitempty"`
}

func (fn *Func) Analyze() AnalyzeResult {
	var result AnalyzeResult

	for _, block := range fn.Blocks {
		for _, stmt := range block.Stmts {
			switch stmt.(type) {
			case *Goto, *IfGoto, *TableJump:
			default:
				result.NumStatements++
			}

			switch c := stmt.(type) {
			case *Comment:
				result.Untranslated++
				if c.EffectKind == "branch" {
					result.UntranslatedBranch++
				}
				if c.EffectKind == "assign" {
					result.UntranslatedAssign++
				}
				if strings.Contains(c.Text, "ss:") {
					result.UntranslatedScratch++
				}
				if strings.Contains(c.Text, ">>") {
					result.UntranslatedBitwise++
				}
				part := false
				for _, failure := range c.Failures {
					if result.UntranslatedFailures == nil {
						result.UntranslatedFailures = make(map[string]int)
					}
					result.UntranslatedFailures[failure.Path+":"+failure.Kind]++
					part = part || failure.Kind == "part"
				}
				if part {
					result.UntranslatedPart++
				}

			}
		}
	}

	return result
}
