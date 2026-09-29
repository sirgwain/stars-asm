package region

import "github.com/sirgwain/stars-asm/dasm/stars/ir"

// inverseCompare maps each comparison operator to its logical negation.
var inverseCompare = map[string]string{
	"==": "!=",
	"!=": "==",
	"<":  ">=",
	">=": "<",
	">":  "<=",
	"<=": ">",
}

// Negate returns the logical negation of cond. It flips a comparison,
// applies De Morgan's laws to && and ||, and removes an existing ! instead of
// stacking another one.
func Negate(cond ir.Expr) ir.Expr {
	switch c := cond.(type) {
	case *ir.Binary:
		if op, ok := inverseCompare[c.Op]; ok {
			return &ir.Binary{Op: op, LHS: c.LHS, RHS: c.RHS}
		}
		switch c.Op {
		case "&&":
			return &ir.Binary{Op: "||", LHS: Negate(c.LHS), RHS: Negate(c.RHS)}
		case "||":
			return &ir.Binary{Op: "&&", LHS: Negate(c.LHS), RHS: Negate(c.RHS)}
		}
	case *ir.Unary:
		if c.Op == "!" {
			return c.X
		}
	}
	return &ir.Unary{Op: "!", X: cond}
}
