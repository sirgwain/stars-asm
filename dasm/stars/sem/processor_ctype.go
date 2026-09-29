package sem

import "fmt"

// ctypeTableGlobal is the MSC runtime's character class table. Its first
// entry is for EOF, so the class of character c is at _ctype[c + 1].
const ctypeTableGlobal = "_ctype"

// ctypeFunctions maps the class masks the MSC ctype.h macros test against the
// table to the runtime function testing the same classes. The Win32 runtime
// returns the same masked bits, so values used directly are unchanged.
var ctypeFunctions = map[uint64]string{
	0x1: "isupper",
	0x2: "islower",
	0x3: "isalpha",
	0x4: "isdigit",
}

// ctypeProcessor rewrites the MSC ctype.h macros, _ctype[c + 1] & mask, to
// calls of the matching runtime function. The table lives in the runtime's
// data, which the decompiled globals do not carry.
type ctypeProcessor struct {
	ctx *FuncContext
}

// ProcessBlock rewrites ctype macro tests in one semantic block.
func (p *ctypeProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	rewriter := &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			next, changed := w.rewriteExprChildren(expr)
			if call, ok := p.ctypeCall(next); ok {
				return call, true, true
			}
			return next, changed, true
		},
	}
	effects, changed := rewriter.rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// ctypeCall matches _ctype[c + 1] & mask and returns the runtime call testing
// the same classes of c.
func (p *ctypeProcessor) ctypeCall(expr Expr) (*Call, bool) {
	and, ok := expr.(*Binary)
	if !ok || and.Op != OpAnd {
		return nil, false
	}
	index, ok := and.LHS.(*ArrayIndex)
	if !ok {
		return nil, false
	}
	if global, ok := globalOf(index.Base); !ok || global.Name != ctypeTableGlobal {
		return nil, false
	}
	add, ok := index.Index.(*Binary)
	if !ok || add.Op != OpAdd {
		panic(fmt.Sprintf("ctype: %s index is not c + 1", ctypeTableGlobal))
	}
	if one, ok := add.RHS.(*Const); !ok || one.U64 != 1 {
		panic(fmt.Sprintf("ctype: %s index is not c + 1", ctypeTableGlobal))
	}
	mask, ok := and.RHS.(*Const)
	if !ok {
		panic(fmt.Sprintf("ctype: %s is not masked by a constant", ctypeTableGlobal))
	}
	name, ok := ctypeFunctions[mask.U64]
	if !ok {
		panic(fmt.Sprintf("ctype: no runtime function for %s mask 0x%x", ctypeTableGlobal, mask.U64))
	}
	fn := p.ctx.sdb.GetFunction(name)
	if fn == nil {
		panic(fmt.Sprintf("ctype: %s is not in the symbol db", name))
	}
	return &Call{Function: fn, Args: []Expr{add.LHS}}, true
}
