package typeinfo

import (
	"fmt"
	"strconv"
	"strings"
	"unicode"
)

// constCastTypes are the integer types a constant expression may cast to.
// Casts are value-preserving here because constants are evaluated as int.
var constCastTypes = map[string]bool{
	"int": true, "short": true, "long": true, "unsigned": true,
	"BYTE": true, "WORD": true, "UINT": true, "DWORD": true, "LONG": true,
}

// evalConstExpr evaluates a C integer constant expression as written in enum
// bodies and Win16 SDK #defines: numbers with optional U/L suffixes, names
// resolved through lookup, parentheses, integer casts, MAKEINTRESOURCE, unary
// - and ~, and the binary +, -, <<, >>, & and | operators.
func evalConstExpr(expr string, lookup func(name string) (int, error)) (int, error) {
	p := constExprParser{tokens: tokenizeConstExpr(expr), lookup: lookup}
	if len(p.tokens) == 0 {
		return 0, fmt.Errorf("empty expression")
	}
	v, err := p.parseOr()
	if err != nil {
		return 0, err
	}
	if p.pos != len(p.tokens) {
		return 0, fmt.Errorf("unexpected %q", p.tokens[p.pos])
	}
	return v, nil
}

// tokenizeConstExpr splits a constant expression into identifiers, numbers,
// and operator tokens.
func tokenizeConstExpr(expr string) []string {
	var tokens []string
	runes := []rune(expr)
	for i := 0; i < len(runes); {
		r := runes[i]
		switch {
		case unicode.IsSpace(r):
			i++
		case unicode.IsLetter(r) || unicode.IsDigit(r) || r == '_':
			start := i
			for i < len(runes) && (unicode.IsLetter(runes[i]) || unicode.IsDigit(runes[i]) || runes[i] == '_') {
				i++
			}
			tokens = append(tokens, string(runes[start:i]))
		case (r == '<' || r == '>') && i+1 < len(runes) && runes[i+1] == r:
			tokens = append(tokens, string(runes[i:i+2]))
			i += 2
		default:
			tokens = append(tokens, string(r))
			i++
		}
	}
	return tokens
}

// constExprParser is a recursive-descent parser over constant expression
// tokens, following C operator precedence.
type constExprParser struct {
	tokens []string
	pos    int
	lookup func(name string) (int, error)
}

// peek returns the token at offset n from the current position, or "".
func (p *constExprParser) peek(n int) string {
	if p.pos+n >= len(p.tokens) {
		return ""
	}
	return p.tokens[p.pos+n]
}

// expect consumes tok or reports an error.
func (p *constExprParser) expect(tok string) error {
	if p.peek(0) != tok {
		return fmt.Errorf("expected %q, got %q", tok, p.peek(0))
	}
	p.pos++
	return nil
}

// parseOr parses a | chain.
func (p *constExprParser) parseOr() (int, error) {
	v, err := p.parseAnd()
	for err == nil && p.peek(0) == "|" {
		p.pos++
		var rhs int
		rhs, err = p.parseAnd()
		v |= rhs
	}
	return v, err
}

// parseAnd parses a & chain.
func (p *constExprParser) parseAnd() (int, error) {
	v, err := p.parseShift()
	for err == nil && p.peek(0) == "&" {
		p.pos++
		var rhs int
		rhs, err = p.parseShift()
		v &= rhs
	}
	return v, err
}

// parseShift parses a << or >> chain.
func (p *constExprParser) parseShift() (int, error) {
	v, err := p.parseAdd()
	for err == nil && (p.peek(0) == "<<" || p.peek(0) == ">>") {
		op := p.peek(0)
		p.pos++
		var rhs int
		rhs, err = p.parseAdd()
		if op == "<<" {
			v <<= rhs
		} else {
			v >>= rhs
		}
	}
	return v, err
}

// parseAdd parses a + or - chain.
func (p *constExprParser) parseAdd() (int, error) {
	v, err := p.parseUnary()
	for err == nil && (p.peek(0) == "+" || p.peek(0) == "-") {
		op := p.peek(0)
		p.pos++
		var rhs int
		rhs, err = p.parseUnary()
		if op == "+" {
			v += rhs
		} else {
			v -= rhs
		}
	}
	return v, err
}

// parseUnary parses unary operators, casts, and primary expressions.
func (p *constExprParser) parseUnary() (int, error) {
	switch p.peek(0) {
	case "-":
		p.pos++
		v, err := p.parseUnary()
		return -v, err
	case "~":
		p.pos++
		v, err := p.parseUnary()
		return ^v, err
	case "(":
		if constCastTypes[p.peek(1)] && p.peek(2) == ")" {
			p.pos += 3
			return p.parseUnary()
		}
		p.pos++
		v, err := p.parseOr()
		if err != nil {
			return 0, err
		}
		return v, p.expect(")")
	}
	return p.parsePrimary()
}

// parsePrimary parses a number, a name, or MAKEINTRESOURCE(expr).
func (p *constExprParser) parsePrimary() (int, error) {
	tok := p.peek(0)
	if tok == "" {
		return 0, fmt.Errorf("unexpected end of expression")
	}
	p.pos++
	if unicode.IsDigit(rune(tok[0])) {
		n, err := strconv.ParseInt(strings.TrimRight(tok, "uUlL"), 0, 64)
		if err != nil {
			return 0, fmt.Errorf("bad number %q", tok)
		}
		return int(n), nil
	}
	if tok == "MAKEINTRESOURCE" {
		if err := p.expect("("); err != nil {
			return 0, err
		}
		v, err := p.parseOr()
		if err != nil {
			return 0, err
		}
		return v, p.expect(")")
	}
	if !unicode.IsLetter(rune(tok[0])) && tok[0] != '_' {
		return 0, fmt.Errorf("unexpected %q", tok)
	}
	return p.lookup(tok)
}
