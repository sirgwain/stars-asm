package ir

type AnalyzeResult struct {
	Untranslated int `json:"untranslated,omitempty"`
}

func (fn *Func) Analyze() AnalyzeResult {
	var result AnalyzeResult

	for _, block := range fn.Blocks {
		for _, stmt := range block.Stmts {
			switch stmt.(type) {
			case *Comment:
				result.Untranslated = result.Untranslated + 1
			}
		}
	}

	return result
}
