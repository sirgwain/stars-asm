package region

import (
	"math"
	"strconv"
	"strings"

	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// foldAssignments turns an if-else chain whose arms each only assign the same
// local a value the compiler merged into one assignment of a conditional
// expression, which is how the source's conditional expression compiles:
//
//	if (c) v = a; else if (d) v = b; else v = e;   becomes   v = c ? a : d ? b : e;
//
// The test runs first and exactly one arm is evaluated either way. C
// converts both arms of ?: to their common type before the assignment
// converts the result to v's type; for an integer v that is invisible,
// since integer conversions compose and an int converts to double exactly,
// so only integer and pointer locals are folded. Arms of 1 and 0 under a
// test that is itself 1 or 0 fold to the test: v = c or v = !c. locals maps
// each local's name to its type.
func foldAssignments(nodes []Node, locals map[string]typeinfo.Type) []Node {
	var out []Node
	for _, n := range nodes {
		switch n := n.(type) {
		case *If:
			n.Then = foldAssignments(n.Then, locals)
			n.Else = foldAssignments(n.Else, locals)
			if a, ok := foldIf(n, locals); ok {
				out = appendStmts(out, a)
				continue
			}
		case *Loop:
			n.Body = foldAssignments(n.Body, locals)
		case *Switch:
			for c := range n.Cases {
				n.Cases[c].Body = foldAssignments(n.Cases[c].Body, locals)
			}
		case *Basic:
			out = appendStmts(out, n.Stmts...)
			continue
		}
		out = append(out, n)
	}
	return out
}

// appendStmts appends stmts to nodes, extending a Basic that ends nodes.
func appendStmts(nodes []Node, stmts ...ir.Stmt) []Node {
	if len(nodes) > 0 {
		if b, ok := nodes[len(nodes)-1].(*Basic); ok {
			nodes[len(nodes)-1] = &Basic{Stmts: append(append([]ir.Stmt(nil), b.Stmts...), stmts...)}
			return nodes
		}
	}
	return append(nodes, &Basic{Stmts: stmts})
}

// maxFoldArms is the most values a folded chain of conditionals may choose
// between before it reads worse than the if-else chain it replaces.
const maxFoldArms = 4

// foldIf returns the single assignment n's arms fold into, or false when an
// arm does anything but assign one foldable local, the arms assign different
// ones, an arm updates the local rather than giving it a new value, or the
// result would choose between more than maxFoldArms values.
func foldIf(n *If, locals map[string]typeinfo.Type) (*ir.Assign, bool) {
	if len(n.Else) != 1 || len(n.Then) != 1 {
		return nil, false
	}
	then, ok := soleAssign(n.Then[0])
	if !ok {
		return nil, false
	}
	v, ok := then.Dst.(*ir.Var)
	if !ok || !foldableType(locals[v.Name]) {
		return nil, false
	}
	// Only a value the compiler merged came from a conditional expression;
	// separate stores on each path are the source's own if-else.
	merged := strings.HasPrefix(v.Name, "t_merge_")
	els, ok := soleAssign(n.Else[0])
	if !ok {
		inner, isIf := n.Else[0].(*If)
		if !isIf {
			return nil, false
		}
		if els, ok = foldIf(inner, locals); !ok {
			return nil, false
		}
	}
	if w, ok := els.Dst.(*ir.Var); !ok || w.Name != v.Name || condArms(then.Src)+condArms(els.Src) > maxFoldArms {
		return nil, false
	}
	// An arm that reads v updates it, as in v |= 8; each update reads
	// better as its own statement than as one arm of a conditional.
	if varRefs(then.Src, v.Name) > 0 || varRefs(els.Src, v.Name) > 0 || !(merged || then.Merge && els.Merge) {
		return nil, false
	}
	if isOne(then.Src) && isZero(els.Src) && boolValued(n.Cond) {
		return &ir.Assign{Dst: v, Src: n.Cond, Merge: true}, true
	}
	if isZero(then.Src) && isOne(els.Src) && boolValued(n.Cond) {
		return &ir.Assign{Dst: v, Src: Negate(n.Cond), Merge: true}, true
	}
	return &ir.Assign{Dst: v, Src: &ir.Cond{Cond: n.Cond, Then: then.Src, Else: els.Src}, Merge: true, Fits: then.Fits && els.Fits}, true
}

// condArms counts the values a chain of conditional expressions chooses
// between; any other expression is one value.
func condArms(e ir.Expr) int {
	if c, ok := e.(*ir.Cond); ok {
		return condArms(c.Then) + condArms(c.Else)
	}
	return 1
}

// soleAssign returns the assignment n consists of, or false.
func soleAssign(n Node) (*ir.Assign, bool) {
	b, ok := n.(*Basic)
	if !ok || len(b.Stmts) != 1 {
		return nil, false
	}
	a, ok := b.Stmts[0].(*ir.Assign)
	return a, ok
}

// foldableType reports whether a local of type typ may receive a folded
// conditional: an integer or a pointer, whose conversions from either arm
// are unaffected by the arms' common type.
func foldableType(typ typeinfo.Type) bool {
	switch t := typ.(type) {
	case *typeinfo.Primitive:
		return t.TypeKind == typeinfo.KInt
	case *typeinfo.Enum, *typeinfo.Pointer:
		return true
	}
	return false
}

// boolValued reports whether e is always 0 or 1: a comparison, a logical
// operator, or a logical not.
func boolValued(e ir.Expr) bool {
	switch e := e.(type) {
	case *ir.Binary:
		switch e.Op {
		case "==", "!=", "<", "<=", ">", ">=", "&&", "||":
			return true
		}
	case *ir.Unary:
		return e.Op == "!"
	}
	return false
}

// isOne reports whether e is the integer constant 1.
func isOne(e ir.Expr) bool {
	v, ok := constValue(e)
	return ok && v == 1
}

// isZero reports whether e is the integer constant 0.
func isZero(e ir.Expr) bool {
	v, ok := constValue(e)
	return ok && v == 0
}

// constValue returns the value of an integer constant as C reads its text,
// or false for anything else or a value outside int.
func constValue(e ir.Expr) (int64, bool) {
	c, ok := e.(*ir.IntConst)
	if !ok {
		return 0, false
	}
	v := int64(c.Value)
	if c.Text != "" && !strings.HasPrefix(c.Text, "'") {
		if parsed, err := strconv.ParseInt(c.Text, 0, 64); err == nil {
			v = parsed
		}
	}
	return v, v >= math.MinInt32 && v <= math.MaxInt32
}

// forwardFoldedTemps moves each merge temp assigned by a statement in nodes
// into its single use in body, in the statement or test right after it,
// when its value is a constant or a 0-or-1 test that fits the temp's type,
// or a value without side effects each of whose arms has the temp's C type
// and fits it, as lowering recorded, so the temp's conversion changes nothing,
// and every call in the use takes
// the temp as an argument, so nothing the use runs comes before the value.
// body is the whole function, where the temp's references are counted.
func forwardFoldedTemps(nodes, body []Node, locals map[string]typeinfo.Type) {
	for i, n := range nodes {
		switch n := n.(type) {
		case *If:
			forwardFoldedTemps(n.Then, body, locals)
			forwardFoldedTemps(n.Else, body, locals)
		case *Loop:
			forwardFoldedTemps(n.Body, body, locals)
		case *Switch:
			for c := range n.Cases {
				forwardFoldedTemps(n.Cases[c].Body, body, locals)
			}
		}
		b, ok := nodes[i].(*Basic)
		if !ok {
			continue
		}
		for k := 0; k < len(b.Stmts); k++ {
			def, ok := b.Stmts[k].(*ir.Assign)
			if !ok {
				continue
			}
			t, ok := def.Dst.(*ir.Var)
			if !ok || !strings.HasPrefix(t.Name, "t_merge_") || !(fitsInt(def.Src, locals[t.Name]) || def.Fits && !hasSideEffects(def.Src)) {
				continue
			}
			var use *ir.Expr
			switch {
			case k+1 < len(b.Stmts):
				use = stmtExpr(b.Stmts[k+1])
			case i+1 < len(nodes):
				if next, isIf := nodes[i+1].(*If); isIf {
					use = &next.Cond
				}
			}
			if use == nil || varRefs(*use, t.Name) != 1 || nodeVarRefs(body, t.Name) != 2 || !callsTake(*use, t.Name) {
				continue
			}
			*use = replaceVar(*use, t.Name, def.Src)
			b.Stmts = append(b.Stmts[:k:k], b.Stmts[k+1:]...)
			k--
		}
	}
}

// stmtExpr returns the expression a statement evaluates, which forwarding
// may rewrite in place, or nil.
func stmtExpr(s ir.Stmt) *ir.Expr {
	switch s := s.(type) {
	case *ir.Assign:
		return &s.Src
	case *ir.ExprStmt:
		return &s.Expr
	case *ir.Return:
		if s.Value != nil {
			return &s.Value
		}
	}
	return nil
}

// fitsInt reports whether every value e can take is a constant, or a 0 or
// 1 test, that fits the integer type typ, and typ promotes to or is int, so
// reading e where the temp was read gives the same value and C type. A
// FALSE/TRUE truth family is the integer type it is declared with.
func fitsInt(e ir.Expr, typ typeinfo.Type) bool {
	if enumType, ok := typ.(*typeinfo.Enum); ok && enumType.Truth && enumType.Storage != nil {
		typ = enumType.Storage
	}
	p, ok := typ.(*typeinfo.Primitive)
	if !ok || p.TypeKind != typeinfo.KInt || p.Native != typeinfo.NativeInt || p.Size > 4 || (p.Size == 4 && !p.Signed) {
		return false
	}
	bits := uint(p.Size * 8)
	lo, hi := int64(0), int64(1)<<bits-1
	if p.Signed {
		lo, hi = -(int64(1) << (bits - 1)), int64(1)<<(bits-1)-1
	}
	var fits func(ir.Expr) bool
	fits = func(e ir.Expr) bool {
		if c, ok := e.(*ir.Cond); ok {
			return !hasSideEffects(c.Cond) && fits(c.Then) && fits(c.Else)
		}
		if boolValued(e) {
			return !hasSideEffects(e)
		}
		v, ok := constValue(e)
		return ok && v >= lo && v <= hi
	}
	return fits(e)
}

// callsTake reports whether every call in e has the variable name among its
// arguments, so no call in e runs before name is read.
func callsTake(e ir.Expr, name string) bool {
	ok := true
	var walk func(ir.Expr)
	walk = func(e ir.Expr) {
		switch e := e.(type) {
		case *ir.Call:
			found := false
			for _, a := range e.Args {
				found = found || varRefs(a, name) > 0
				walk(a)
			}
			ok = ok && found
		case *ir.Unary:
			walk(e.X)
		case *ir.Binary:
			walk(e.LHS)
			walk(e.RHS)
		case *ir.Cond:
			walk(e.Cond)
			walk(e.Then)
			walk(e.Else)
		case *ir.Cast:
			walk(e.Value)
		case *ir.Index:
			walk(e.Base)
			walk(e.Index)
		case *ir.Field:
			walk(e.Base)
		case *ir.Macro:
			for _, a := range e.Args {
				walk(a)
			}
		case *ir.AddressOf:
			walk(e.Target)
		case *ir.Deref:
			walk(e.Pointer)
		case *ir.PointerOffset:
			walk(e.Pointer)
			walk(e.Offset)
		}
	}
	walk(e)
	return ok
}

// replaceVar returns e with each read of the variable name replaced by
// value.
func replaceVar(e ir.Expr, name string, value ir.Expr) ir.Expr {
	switch e := e.(type) {
	case *ir.Var:
		if e.Name == name {
			return value
		}
	case *ir.Unary:
		n := *e
		n.X = replaceVar(e.X, name, value)
		return &n
	case *ir.Binary:
		n := *e
		n.LHS, n.RHS = replaceVar(e.LHS, name, value), replaceVar(e.RHS, name, value)
		return &n
	case *ir.Cond:
		n := *e
		n.Cond, n.Then, n.Else = replaceVar(e.Cond, name, value), replaceVar(e.Then, name, value), replaceVar(e.Else, name, value)
		return &n
	case *ir.Cast:
		n := *e
		n.Value = replaceVar(e.Value, name, value)
		return &n
	case *ir.Index:
		n := *e
		n.Base, n.Index = replaceVar(e.Base, name, value), replaceVar(e.Index, name, value)
		return &n
	case *ir.Field:
		n := *e
		n.Base = replaceVar(e.Base, name, value)
		return &n
	case *ir.Call:
		n := *e
		n.Args = make([]ir.Expr, len(e.Args))
		for i, a := range e.Args {
			n.Args[i] = replaceVar(a, name, value)
		}
		return &n
	case *ir.Macro:
		n := *e
		n.Args = make([]ir.Expr, len(e.Args))
		for i, a := range e.Args {
			n.Args[i] = replaceVar(a, name, value)
		}
		return &n
	case *ir.AddressOf:
		n := *e
		n.Target = replaceVar(e.Target, name, value)
		return &n
	case *ir.Deref:
		n := *e
		n.Pointer = replaceVar(e.Pointer, name, value)
		return &n
	case *ir.PointerOffset:
		n := *e
		n.Pointer, n.Offset = replaceVar(e.Pointer, name, value), replaceVar(e.Offset, name, value)
		return &n
	}
	return e
}

// nodeVarRefs counts the reads and writes of the variable name in nodes,
// including nested statements, tests, and loop clauses.
func nodeVarRefs(nodes []Node, name string) int {
	count := 0
	stmt := func(s ir.Stmt) {
		switch s := s.(type) {
		case *ir.Assign:
			count += varRefs(s.Dst, name) + varRefs(s.Src, name)
		case *ir.ExprStmt:
			count += varRefs(s.Expr, name)
		case *ir.Return:
			count += varRefs(s.Value, name)
		}
	}
	walkNodes(nodes, func(n Node) {
		switch n := n.(type) {
		case *Basic:
			for _, s := range n.Stmts {
				stmt(s)
			}
		case *If:
			count += varRefs(n.Cond, name)
		case *Loop:
			count += varRefs(n.Cond, name)
			if n.Init != nil {
				stmt(n.Init)
			}
			if n.Post != nil {
				stmt(n.Post)
			}
		case *Switch:
			count += varRefs(n.Index, name)
		}
	})
	return count
}
