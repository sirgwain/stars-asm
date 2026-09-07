// win16sigparser is a small helper that parses Win16 WINDOWS.H prototypes
// and emits stars-asm override.json entries for exported KERNEL/GDI/USER APIs.
//
// Usage:
//
//	go run ./win16sigparser \
//	  --windows-h /path/to/WINDOWS.H \
//	  --gdi-exports /path/to/gdi.exports \
//	  --kernel-exports /path/to/kernel.exports \
//	  --user-exports /path/to/user.exports \
//	  --commdlg-exports /path/to/commdlg.exports \
//	  --toolhelp-exports /path/to/toolhelp.exports \
//	  --out-gdi /path/to/overrides-gdi.json \
//	  --out-kernel /path/to/overrides-kernel.json \
//	  --out-user /path/to/overrides-user.json \
//	  --out-commdlg /path/to/overrides-commdlg.json \
//	  --out-toolhelp /path/to/overrides-toolhelp.json
//
// Notes:
// - This is a "best-effort" prototype parser (no full C preprocessor).
// - It intentionally focuses on Win16 API-style prototypes (ret + name + arglist;).
package main

import (
	"bytes"
	"encoding/json"
	"encoding/xml"
	"flag"
	"fmt"
	"io"
	"os"
	"regexp"
	"sort"
	"strings"
)

type OverrideFile struct {
	Functions []OverrideFunc `json:"functions"`
}
type OverrideFunc struct {
	Name     string        `json:"name"`
	Ret      string        `json:"ret"`
	Args     []OverrideArg `json:"args"`
	Callconv string        `json:"callconv"`
}
type OverrideArg struct {
	Name  string `json:"name"`
	CType string `json:"ctype"`
}

// ---- exports XML ----

type exportXML struct {
	Ordinal int    `xml:"ORDINAL,attr"`
	Name    string `xml:"NAME,attr"`
	Purge   int    `xml:"PURGE,attr"`
}

type exportEntry struct {
	Ordinal int
	Name    string
	Purge   int
}

func loadExports(path string) ([]exportEntry, error) {
	b, err := os.ReadFile(path)
	if err != nil {
		return nil, err
	}
	// The files are small; decode the whole thing.
	var lib struct {
		Exports []exportXML `xml:"EXPORT"`
	}
	if err := xml.Unmarshal(b, &lib); err != nil {
		return nil, err
	}
	out := make([]exportEntry, 0, len(lib.Exports))
	for _, e := range lib.Exports {
		if e.Ordinal <= 0 {
			continue
		}
		out = append(out, exportEntry(e))
	}
	return out, nil
}

// ---- windows.h prototype parsing ----

// remove /* */ comments and // comments (best effort).
var (
	reBlockComment = regexp.MustCompile(`(?s)/\*.*?\*/`)
	reLineComment  = regexp.MustCompile(`//.*`)
)

func stripComments(s string) string {
	s = reBlockComment.ReplaceAllString(s, "")
	s = reLineComment.ReplaceAllString(s, "")
	return s
}

func collapseLineContinuations(s string) string {
	// join backslash-newline
	return strings.ReplaceAll(s, "\\\n", "")
}

// We collect candidates by concatenating non-# lines until ';' is seen.
func collectProtoCandidates(s string) []string {
	var out []string
	var buf strings.Builder

	lines := strings.Split(s, "\n")
	for _, line := range lines {
		t := strings.TrimSpace(line)
		if t == "" {
			continue
		}
		if strings.HasPrefix(t, "#") {
			continue
		}
		buf.WriteByte(' ')
		buf.WriteString(t)
		if strings.Contains(t, ";") {
			whole := buf.String()
			parts := strings.Split(whole, ";")
			for _, p := range parts[:len(parts)-1] {
				p = strings.TrimSpace(p)
				if p == "" {
					continue
				}
				out = append(out, p+";")
			}
			buf.Reset()
			buf.WriteString(strings.TrimSpace(parts[len(parts)-1]))
		}
	}
	return out
}

// A naive proto matcher: "retstuff NAME(args);"
// Retstuff includes calling conv tokens; we extract later.
var reProto = regexp.MustCompile(`(?is)^(?P<ret>[\w\s\*\(\)]+?)\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*\((?P<args>.*?)\)\s*;$`)

type proto struct {
	Name    string
	RetRaw  string
	ArgsRaw string
}

func parseWindowsHProtos(path string) (map[string]proto, error) {
	b, err := os.ReadFile(path)
	if err != nil {
		return nil, err
	}
	s := string(b)
	s = stripComments(s)
	s = collapseLineContinuations(s)

	cands := collectProtoCandidates(s)
	out := map[string]proto{}

	for _, c := range cands {
		if strings.HasPrefix(strings.ToLower(strings.TrimSpace(c)), "typedef ") {
			continue
		}
		m := reProto.FindStringSubmatch(c)
		if m == nil {
			continue
		}
		ret := strings.TrimSpace(m[reProto.SubexpIndex("ret")])
		name := strings.TrimSpace(m[reProto.SubexpIndex("name")])
		args := strings.TrimSpace(m[reProto.SubexpIndex("args")])
		out[strings.ToUpper(name)] = proto{Name: name, RetRaw: ret, ArgsRaw: args}
	}
	return out, nil
}

// ---- type normalization ----

var mapSimpleType = map[string]string{
	"INT":   "int16_t",
	"SHORT": "int16_t",
	"UINT":  "uint16_t",
	"WORD":  "uint16_t",
	"DWORD": "uint32_t",
	"LONG":  "int32_t",
	"BOOL":  "int16_t",
	"BYTE":  "uint8_t",
	"CHAR":  "char",
	// Preserve Win16 override aliases instead of lowering to raw pointers.
	"LPSTR":   "LPSTR",
	"LPCSTR":  "LPCSTR",
	"LPVOID":  "LPVOID",
	"LPCVOID": "LPCVOID",
	"PSTR":    "PSTR",
	"PCSTR":   "PCSTR",
}

func isTypeToken(tok string) bool {
	if tok == "" {
		return false
	}
	up := strings.ToUpper(tok)
	switch up {
	case "FAR", "_FAR", "NEAR", "_NEAR", "HUGE", "_HUGE", "CONST", "VOLATILE", "SIGNED", "UNSIGNED":
		return true
	}
	if tok == strings.ToUpper(tok) {
		return true
	}
	if strings.HasPrefix(tok, "LP") || strings.HasPrefix(tok, "H") {
		return true
	}
	switch strings.ToLower(tok) {
	case "int", "short", "long", "char", "void":
		return true
	}
	return strings.HasSuffix(tok, "_t")
}

func normCType(tokens []string, stars int) string {
	hasConst := false
	hasFar := false
	hasNear := false
	var t2 []string
	for _, t := range tokens {
		up := strings.ToUpper(t)
		switch up {
		case "CONST":
			hasConst = true
			continue
		case "VOLATILE":
			continue
		case "FAR", "_FAR":
			hasFar = true
			continue
		case "NEAR", "_NEAR":
			hasNear = true
			continue
		}
		t2 = append(t2, t)
	}
	base := strings.Join(t2, " ")
	base = strings.TrimSpace(strings.Join(strings.Fields(base), " "))
	up := strings.ToUpper(base)

	// direct map
	if v, ok := mapSimpleType[up]; ok {
		base = v
	} else {
		// unsigned/signed combos
		if strings.HasPrefix(up, "UNSIGNED ") {
			rest := strings.TrimSpace(up[len("UNSIGNED "):])
			switch rest {
			case "INT", "SHORT":
				base = "uint16_t"
			case "LONG":
				base = "uint32_t"
			case "CHAR":
				base = "uint8_t"
			}
		} else if strings.HasPrefix(up, "SIGNED ") {
			rest := strings.TrimSpace(up[len("SIGNED "):])
			switch rest {
			case "INT", "SHORT":
				base = "int16_t"
			case "LONG":
				base = "int32_t"
			}
		}
	}

	// Prefer override aliases for plain char*/void* spellings.
	// This keeps generated signatures aligned with override type metadata.
	if stars == 1 {
		switch strings.ToUpper(base) {
		case "CHAR":
			if hasConst {
				return "LPCSTR"
			}
			return "LPSTR"
		case "VOID":
			if hasConst {
				return "LPCVOID"
			}
			return "LPVOID"
		}
	}

	if stars > 0 {
		qual := ""
		if hasFar {
			qual = " FAR"
		} else if hasNear {
			qual = " NEAR"
		}
		base = base + qual + strings.Repeat("*", stars)
	}
	return strings.TrimSpace(base)
}

func splitArgs(argstr string) []string {
	argstr = strings.TrimSpace(argstr)
	if argstr == "" || strings.EqualFold(argstr, "void") {
		return nil
	}
	var out []string
	depth := 0
	start := 0
	for i := 0; i < len(argstr); i++ {
		switch argstr[i] {
		case '(':
			depth++
		case ')':
			if depth > 0 {
				depth--
			}
		case ',':
			if depth == 0 {
				out = append(out, strings.TrimSpace(argstr[start:i]))
				start = i + 1
			}
		}
	}
	out = append(out, strings.TrimSpace(argstr[start:]))
	var out2 []string
	for _, a := range out {
		if a != "" {
			out2 = append(out2, a)
		}
	}
	return out2
}

var reFnPtr = regexp.MustCompile(`\(\s*\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)`)

func parseParam(p string, idx int) OverrideArg {
	p = strings.TrimSpace(p)
	if p == "..." {
		return OverrideArg{Name: fmt.Sprintf("arg%d", idx), CType: "..."}
	}

	// function pointer param: keep as-is for now
	if strings.Contains(p, "(") && strings.Contains(p, ")") && strings.Contains(p, "*") {
		if m := reFnPtr.FindStringSubmatch(p); m != nil {
			name := strings.ToLower(m[1])
			ctype := strings.TrimSpace(strings.ReplaceAll(p, m[1], ""))
			ctype = strings.Join(strings.Fields(ctype), " ")
			return OverrideArg{Name: name, CType: ctype}
		}
	}

	toks := strings.Fields(p)

	// unnamed single-token param (e.g., "HDC" or "int")
	if len(toks) == 1 {
		t := toks[0]
		base := strings.ReplaceAll(t, "*", "")
		if isTypeToken(base) {
			stars := strings.Count(t, "*")
			return OverrideArg{Name: fmt.Sprintf("arg%d", idx), CType: normCType([]string{base}, stars)}
		}
	}

	// general case: last token is name, unless it looks like a type
	last := toks[len(toks)-1]
	nameStars := 0
	for strings.HasPrefix(last, "*") {
		nameStars++
		last = strings.TrimPrefix(last, "*")
	}
	allTypeTokens := true
	for _, t := range toks {
		base := strings.ReplaceAll(t, "*", "")
		if !isTypeToken(base) {
			allTypeTokens = false
			break
		}
	}
	if allTypeTokens {
		// treat as unnamed type; consume all tokens so cases like "RECT FAR*"
		// don't collapse to "FAR*".
		stars := 0
		var typeTokens []string
		for _, t := range toks {
			if strings.Contains(t, "*") {
				stars += strings.Count(t, "*")
				t = strings.ReplaceAll(t, "*", "")
			}
			if t != "" {
				typeTokens = append(typeTokens, t)
			}
		}
		return OverrideArg{Name: fmt.Sprintf("arg%d", idx), CType: normCType(typeTokens, stars)}
	}

	name := strings.ToLower(last)
	toks = toks[:len(toks)-1]

	stars := nameStars
	var typeTokens []string
	for _, t := range toks {
		if strings.Contains(t, "*") {
			stars += strings.Count(t, "*")
			t = strings.ReplaceAll(t, "*", "")
			if t != "" {
				typeTokens = append(typeTokens, t)
			}
		} else {
			typeTokens = append(typeTokens, t)
		}
	}

	return OverrideArg{Name: name, CType: normCType(typeTokens, stars)}
}

func parseRetAndCallconv(retRaw string) (ret string, callconv string) {
	up := strings.ToUpper(retRaw)
	callconv = "cdecl"
	if strings.Contains(up, "PASCAL") || strings.Contains(up, "WINAPI") || strings.Contains(up, "APIENTRY") || strings.Contains(up, "CALLBACK") {
		callconv = "pascal"
	}

	// strip conv + far/near
	var toks []string
	for _, t := range strings.Fields(retRaw) {
		upT := strings.ToUpper(t)
		switch upT {
		case "FAR", "NEAR", "_FAR", "_NEAR", "PASCAL", "WINAPI", "APIENTRY", "CALLBACK":
			continue
		case "CONST", "VOLATILE":
			continue
		}
		toks = append(toks, t)
	}
	stars := 0
	var baseToks []string
	for _, t := range toks {
		if strings.Contains(t, "*") {
			stars += strings.Count(t, "*")
			t = strings.ReplaceAll(t, "*", "")
			if t != "" {
				baseToks = append(baseToks, t)
			}
		} else {
			baseToks = append(baseToks, t)
		}
	}
	ret = normCType(baseToks, stars)
	return ret, callconv
}

func synthesizeFromPurge(name string, purge int) OverrideFunc {
	argc := 0
	if purge > 0 {
		argc = purge / 2
	}
	args := make([]OverrideArg, 0, argc)
	for i := 1; i <= argc; i++ {
		args = append(args, OverrideArg{
			Name:  fmt.Sprintf("arg%d", i),
			CType: "uint16_t",
		})
	}
	return OverrideFunc{
		Name:     name,
		Ret:      "int16_t",
		Args:     args,
		Callconv: "pascal",
	}
}

func buildEntries(exports []exportEntry, protos map[string]proto, emitOrdinalAliases bool, synthesizeMissing bool) []OverrideFunc {
	sort.Slice(exports, func(i, j int) bool { return exports[i].Ordinal < exports[j].Ordinal })
	var out []OverrideFunc
	seen := map[string]bool{}
	add := func(f OverrideFunc) {
		up := strings.ToUpper(f.Name)
		if seen[up] {
			return
		}
		seen[up] = true
		out = append(out, f)
	}

	for _, e := range exports {
		name := strings.TrimSpace(e.Name)
		if name != "" {
			if pr, ok := protos[strings.ToUpper(name)]; ok {
				ret, cc := parseRetAndCallconv(pr.RetRaw)
				var args []OverrideArg
				for i, a := range splitArgs(pr.ArgsRaw) {
					args = append(args, parseParam(a, i+1))
				}
				add(OverrideFunc{
					Name:     pr.Name,
					Ret:      ret,
					Args:     args,
					Callconv: cc,
				})
			} else if synthesizeMissing {
				add(synthesizeFromPurge(name, e.Purge))
			}
		}

		if emitOrdinalAliases {
			ordName := fmt.Sprintf("ordinal_%d", e.Ordinal)
			if name != "" {
				if pr, ok := protos[strings.ToUpper(name)]; ok {
					ret, cc := parseRetAndCallconv(pr.RetRaw)
					var args []OverrideArg
					for i, a := range splitArgs(pr.ArgsRaw) {
						args = append(args, parseParam(a, i+1))
					}
					add(OverrideFunc{
						Name:     ordName,
						Ret:      ret,
						Args:     args,
						Callconv: cc,
					})
					continue
				}
			}
			if synthesizeMissing {
				f := synthesizeFromPurge(ordName, e.Purge)
				add(f)
			}
		}
	}
	return out
}

func writeOverrideFile(path string, funcs []OverrideFunc) error {
	sort.Slice(funcs, func(i, j int) bool { return funcs[i].Name < funcs[j].Name })
	out := OverrideFile{Functions: funcs}

	var buf bytes.Buffer
	enc := json.NewEncoder(&buf)
	enc.SetIndent("", "  ")
	if err := enc.Encode(out); err != nil {
		return err
	}
	return os.WriteFile(path, buf.Bytes(), 0644)
}

func main() {
	var (
		windowsH    = flag.String("windows-h", "", "Path to WINDOWS.H (Win16 SDK)")
		toolhelpH   = flag.String("toolhelp-h", "", "Optional path to TOOLHELP.H (merged with --windows-h prototypes)")
		gdiExp      = flag.String("gdi-exports", "", "Path to gdi.exports (XML)")
		kerExp      = flag.String("kernel-exports", "", "Path to kernel.exports (XML)")
		userExp     = flag.String("user-exports", "", "Path to user.exports (XML)")
		commdlgExp  = flag.String("commdlg-exports", "", "Path to commdlg.exports (XML)")
		toolhelpExp = flag.String("toolhelp-exports", "", "Path to toolhelp.exports (XML)")
		emitOrd     = flag.Bool("emit-ordinal-aliases", false, "Also emit ordinal_<n> aliases for each export")
		synthMiss   = flag.Bool("synthesize-missing", false, "Synthesize signatures from export PURGE bytes when no prototype exists")
		outPath     = flag.String("out", "", "Output JSON path for all modules combined")
		outGDI      = flag.String("out-gdi", "", "Output JSON path for GDI overrides")
		outKernel   = flag.String("out-kernel", "", "Output JSON path for KERNEL overrides")
		outUser     = flag.String("out-user", "", "Output JSON path for USER overrides")
		outCommdlg  = flag.String("out-commdlg", "", "Output JSON path for COMMDLG overrides")
		outToolhelp = flag.String("out-toolhelp", "", "Output JSON path for TOOLHELP overrides")
	)
	flag.Parse()

	if *windowsH == "" || *gdiExp == "" || *kerExp == "" || *userExp == "" {
		fmt.Fprintln(os.Stderr, "missing required args; see --help")
		os.Exit(2)
	}
	if *outPath == "" && *outGDI == "" && *outKernel == "" && *outUser == "" && *outCommdlg == "" && *outToolhelp == "" {
		fmt.Fprintln(os.Stderr, "missing output path; set --out or at least one --out-{gdi,kernel,user,commdlg,toolhelp}")
		os.Exit(2)
	}

	protos, err := parseWindowsHProtos(*windowsH)
	if err != nil {
		fmt.Fprintln(os.Stderr, "parse windows.h:", err)
		os.Exit(1)
	}
	if *toolhelpH != "" {
		toolhelpProtos, err := parseWindowsHProtos(*toolhelpH)
		if err != nil {
			fmt.Fprintln(os.Stderr, "parse toolhelp.h:", err)
			os.Exit(1)
		}
		for upName, pr := range toolhelpProtos {
			protos[upName] = pr
		}
	}
	gdi, err := loadExports(*gdiExp)
	if err != nil {
		fmt.Fprintln(os.Stderr, "parse gdi.exports:", err)
		os.Exit(1)
	}
	kernel, err := loadExports(*kerExp)
	if err != nil {
		fmt.Fprintln(os.Stderr, "parse kernel.exports:", err)
		os.Exit(1)
	}
	user, err := loadExports(*userExp)
	if err != nil {
		fmt.Fprintln(os.Stderr, "parse user.exports:", err)
		os.Exit(1)
	}
	commdlg := []exportEntry{}
	if *commdlgExp != "" {
		commdlg, err = loadExports(*commdlgExp)
		if err != nil {
			fmt.Fprintln(os.Stderr, "parse commdlg.exports:", err)
			os.Exit(1)
		}
	}
	toolhelp := []exportEntry{}
	if *toolhelpExp != "" {
		toolhelp, err = loadExports(*toolhelpExp)
		if err != nil {
			fmt.Fprintln(os.Stderr, "parse toolhelp.exports:", err)
			os.Exit(1)
		}
	}

	gdiFuncs := buildEntries(gdi, protos, *emitOrd, *synthMiss)
	kernelFuncs := buildEntries(kernel, protos, *emitOrd, *synthMiss)
	userFuncs := buildEntries(user, protos, *emitOrd, *synthMiss)
	commdlgFuncs := buildEntries(commdlg, protos, *emitOrd, *synthMiss)
	toolhelpFuncs := buildEntries(toolhelp, protos, *emitOrd, *synthMiss)

	var total int
	if *outGDI != "" {
		if err := writeOverrideFile(*outGDI, gdiFuncs); err != nil {
			fmt.Fprintln(os.Stderr, "write gdi:", err)
			os.Exit(1)
		}
		total += len(gdiFuncs)
		io.WriteString(os.Stderr, fmt.Sprintf("wrote %d functions to %s\n", len(gdiFuncs), *outGDI))
	}
	if *outKernel != "" {
		if err := writeOverrideFile(*outKernel, kernelFuncs); err != nil {
			fmt.Fprintln(os.Stderr, "write kernel:", err)
			os.Exit(1)
		}
		total += len(kernelFuncs)
		io.WriteString(os.Stderr, fmt.Sprintf("wrote %d functions to %s\n", len(kernelFuncs), *outKernel))
	}
	if *outUser != "" {
		if err := writeOverrideFile(*outUser, userFuncs); err != nil {
			fmt.Fprintln(os.Stderr, "write user:", err)
			os.Exit(1)
		}
		total += len(userFuncs)
		io.WriteString(os.Stderr, fmt.Sprintf("wrote %d functions to %s\n", len(userFuncs), *outUser))
	}
	if *outCommdlg != "" {
		if err := writeOverrideFile(*outCommdlg, commdlgFuncs); err != nil {
			fmt.Fprintln(os.Stderr, "write commdlg:", err)
			os.Exit(1)
		}
		total += len(commdlgFuncs)
		io.WriteString(os.Stderr, fmt.Sprintf("wrote %d functions to %s\n", len(commdlgFuncs), *outCommdlg))
	}
	if *outToolhelp != "" {
		if err := writeOverrideFile(*outToolhelp, toolhelpFuncs); err != nil {
			fmt.Fprintln(os.Stderr, "write toolhelp:", err)
			os.Exit(1)
		}
		total += len(toolhelpFuncs)
		io.WriteString(os.Stderr, fmt.Sprintf("wrote %d functions to %s\n", len(toolhelpFuncs), *outToolhelp))
	}

	if *outPath != "" {
		all := make([]OverrideFunc, 0, len(gdiFuncs)+len(kernelFuncs)+len(userFuncs)+len(commdlgFuncs)+len(toolhelpFuncs))
		all = append(all, gdiFuncs...)
		all = append(all, kernelFuncs...)
		all = append(all, userFuncs...)
		all = append(all, commdlgFuncs...)
		all = append(all, toolhelpFuncs...)
		if err := writeOverrideFile(*outPath, all); err != nil {
			fmt.Fprintln(os.Stderr, "write combined:", err)
			os.Exit(1)
		}
		total += len(all)
		io.WriteString(os.Stderr, fmt.Sprintf("wrote %d functions to %s\n", len(all), *outPath))
	}

	io.WriteString(os.Stderr, fmt.Sprintf("done (emitted %d total entries)\n", total))
}
