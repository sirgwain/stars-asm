package ir

import "strings"

type AnalyzeResult struct {
	Untranslated        int `json:"untranslated,omitempty"`
	UntranslatedAssign  int `json:"untranslatedAssign,omitempty"`
	UntranslatedBitwise int `json:"untranslatedBitwise,omitempty"`
	UntranslatedBranch  int `json:"untranslatedBranch,omitempty"`
	UntranslatedPart    int `json:"untranslatedPart,omitempty"`
	UntranslatedScratch int `json:"untranslatedScratch,omitempty"`
}

func (fn *Func) Analyze() AnalyzeResult {
	var result AnalyzeResult

	for _, block := range fn.Blocks {
		for _, stmt := range block.Stmts {
			switch c := stmt.(type) {
			case *Comment:
				result.Untranslated++
				if strings.Contains(c.Text, "branch") {
					result.UntranslatedBranch++
				}
				if strings.Contains(c.Text, " = ") {
					result.UntranslatedAssign++
				}
				if strings.Contains(c.Text, "ss:") {
					result.UntranslatedScratch++
				}
				if strings.Contains(c.Text, ">>") {
					result.UntranslatedBitwise++
				}

			}
		}
	}

	return result
}
