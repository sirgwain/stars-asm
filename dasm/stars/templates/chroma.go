package templates

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"strings"

	"github.com/alecthomas/chroma/v2"
	"github.com/alecthomas/chroma/v2/formatters"
	"github.com/alecthomas/chroma/v2/lexers"
	"github.com/alecthomas/chroma/v2/styles"
)

func printHighlightedC(w io.Writer, src string, showColor bool) {
	printHighlighted(w, "c", src, showColor)
}

func printHighlightedAsm(w io.Writer, src string, showColor bool) {
	printHighlighted(w, "nasm", src, showColor)
}

func printHighlighted(w io.Writer, lexerName, src string, showColor bool) {
	if !showColor {
		fmt.Fprint(w, src)
		return
	}
	colored, err := Highlight(lexerName, src)
	if err != nil {
		// Best-effort: if highlighting fails, still emit the plain text.
		fmt.Fprint(w, src)
		return
	}

	fmt.Fprint(w, colored)
}

func HighlightLines(lexerName string, lines []string, showColor bool) []string {
	if !showColor || len(lines) == 0 {
		return lines
	}
	return splitHighlighted(lexerName, strings.Join(lines, "\n"), lines)
}

func HighlightTextLines(lexerName, src string, showColor bool) []string {
	lines := strings.Split(strings.TrimRight(src, "\n"), "\n")
	if !showColor || src == "" {
		return lines
	}
	return splitHighlighted(lexerName, src, lines)
}

func splitHighlighted(lexerName, src string, fallback []string) []string {
	colored, err := Highlight(lexerName, src)
	if err != nil {
		return fallback
	}
	return strings.Split(strings.TrimRight(colored, "\n"), "\n")
}

func Highlight(lexerName, src string) (string, error) {
	lexer := lexers.Get(lexerName)
	if lexer == nil {
		lexer = lexers.Fallback
	}
	lexer = chroma.Coalesce(lexer)

	it, err := lexer.Tokenise(nil, src)
	if err != nil {
		return "", err
	}

	style := styles.Get("dracula")
	if style == nil {
		style = styles.Fallback
	}

	var f chroma.Formatter
	colorterm := strings.ToLower(os.Getenv("COLORTERM"))
	term := strings.ToLower(os.Getenv("TERM"))
	switch {
	case strings.Contains(colorterm, "truecolor") || strings.Contains(colorterm, "24bit"):
		f = formatters.TTY16m
	case strings.Contains(term, "256color"):
		f = formatters.TTY256
	default:
		f = formatters.TTY8
	}

	var out bytes.Buffer
	if err := f.Format(&out, style, it); err != nil {
		return "", err
	}
	return strings.TrimRight(out.String(), "\n"), nil
}
