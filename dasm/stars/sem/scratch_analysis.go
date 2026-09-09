package sem

import (
	"fmt"
	"maps"
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type scratchPoint struct{ block, effect int }
type scratchAccessKind uint8

const (
	scratchRead scratchAccessKind = iota
	scratchWrite
	scratchAddress
)

type scratchAccess struct {
	point    scratchPoint
	region   ScratchRange
	kind     scratchAccessKind
	expr     Expr
	typ      typeinfo.Type
	priority int
}
type scratchDef struct {
	object *scratchObject
	point  scratchPoint
	region ScratchRange
	value  Expr
}
type scratchDisposition uint8

const (
	scratchInline scratchDisposition = iota
	scratchTemp
	scratchLocal
	scratchUnresolved
)

type scratchObject struct {
	region      ScratchRange
	typ         typeinfo.Type
	priority    int
	disposition scratchDisposition
	storage     LValue
}

// scratchState keeps reaching definitions per byte, preserving intact lanes
// while invalidating any wider value intersected by a partial write. ID -1 is
// an undefined incoming byte; absence is the fixed-point bottom state.
type scratchState map[int]map[int]bool

type scratchAnalysis struct {
	readObjects  map[scratchUseKey]*scratchObject
	accesses     []scratchAccess
	objects      []*scratchObject
	defs         []scratchDef
	writes       map[scratchPoint]int
	reads        map[scratchPoint]scratchState
	exits        []scratchState
	predecessors [][]int
	entries      []bool
}

// scratchAccessRewriter distinguishes reads, stores, and address identity while
// using the ordinary semantic traversal for expression children.
func scratchAccessRewriter(visit func(Expr, scratchAccessKind, typeinfo.Type) Expr) *semRewriter {
	w := &semRewriter{}
	w.effect = func(w *semRewriter, effect Effect) (Effect, bool, bool) {
		call, ok := effect.(*CallEffect)
		if !ok {
			return effect, false, false
		}
		result, ok := call.Result.(LValue)
		if !ok {
			return effect, false, false
		}
		next := *call
		var callChanged, resultChanged bool
		next.Call, callChanged = w.rewriteCall(call.Call, call.MetaInfo)
		next.Result, resultChanged = w.rewriteLValue(result)
		return &next, callChanged || resultChanged, true
	}
	w.expr = func(w *semRewriter, expr Expr) (Expr, bool, bool) {
		if addr, ok := expr.(*AddressOf); ok {
			if part, ok := addr.Target.(*Part); ok && part.Width == 0 {
				if _, _, ok := scratchSlotRangeExpr(part.Base); ok {
					base := visit(part.Base, scratchAddress, part.Base.ExprType()).(LValue)
					nextPart, next := *part, *addr
					nextPart.Base = base
					next.Target = &nextPart
					return &next, base != part.Base, true
				}
			}
			if memory, ok := addr.Target.(*Memory); ok && memory.Width == 0 {
				if pointer, ok := addr.TypeInfo.(*typeinfo.Pointer); ok && pointer.Elem.Bytes() > 0 {
					target, next := *memory, *addr
					target.Width, target.TypeInfo = pointer.Elem.Bytes(), pointer.Elem
					if _, _, ok := scratchSlotRangeExpr(&target); ok {
						next.Target = visit(&target, scratchAddress, pointer.Elem).(LValue)
						return &next, true, true
					}
				}
			}
			if _, _, ok := scratchSlotRangeExpr(addr.Target); ok {
				typ := addr.Target.ExprType()
				if ptr, ok := addr.TypeInfo.(*typeinfo.Pointer); ok {
					typ = ptr.Elem
				}
				target := visit(addr.Target, scratchAddress, typ).(LValue)
				next := *addr
				next.Target = target
				return &next, target != addr.Target, true
			}
		}
		if _, _, ok := scratchSlotRangeExpr(expr); ok {
			next := visit(expr, scratchRead, expr.ExprType())
			return next, next != expr, true
		}
		return expr, false, false
	}
	w.lvalue = func(w *semRewriter, value LValue) (LValue, bool, bool) {
		if _, _, ok := scratchSlotRangeExpr(value); ok {
			next := visit(value, scratchWrite, value.ExprType()).(LValue)
			return next, next != value, true
		}
		if ref, ok := value.(*SymbolRef); ok {
			// Pointer crossings read the scratch pointer; they do not access
			// bytes at a field offset inside the pointer's stack slot.
			var expanded LValue
			switch path := ref.Path.(type) {
			case *symresolve.SymbolField:
				expanded = &FieldAccess{Base: &SymbolRef{Path: path.Base}, Field: path.Field}
			case *symresolve.SymbolBitfield:
				expanded = &FieldAccess{Base: &SymbolRef{Path: path.Base}, Field: path.Field}
			case *symresolve.SymbolDeref:
				expanded = &Deref{Pointer: &SymbolRef{Path: path.Base}, Width: path.Type().Bytes(), TypeInfo: path.Type()}
			case *symresolve.SymbolOffset:
				expanded = &Part{Base: &SymbolRef{Path: path.Base}, ByteOff: path.Offset, Width: path.Type().Bytes(), TypeInfo: path.Type()}
			}
			if expanded != nil {
				if next, changed := w.rewriteLValueChildren(expanded); changed {
					return next, true, true
				}
				return value, false, true
			}
		}
		return value, false, false
	}
	return w
}

// analyzeScratchStorage inventories accesses and partitions overlapping views.
func analyzeScratchStorage(f *Func) *scratchAnalysis {
	a := &scratchAnalysis{writes: map[scratchPoint]int{}, reads: map[scratchPoint]scratchState{}}
	for bi, b := range f.Blocks {
		for ei, e := range b.Effects {
			point := scratchPoint{bi, ei}
			w := scratchAccessRewriter(func(expr Expr, kind scratchAccessKind, typ typeinfo.Type) Expr {
				off, size, _ := scratchSlotRangeExpr(expr)
				if ref, ok := expr.(*SymbolRef); ok {
					root := scratchPathRoot(ref.Path)
					if root != ref.Path && root.TypeInfo.Bytes() == root.StorageSize {
						rootExpr := &SymbolRef{Path: root}
						a.accesses = append(a.accesses, scratchAccess{point, ScratchRange{root.BPOffset, root.StorageSize}, scratchAddress, rootExpr, root.TypeInfo, 4})
					}
				}
				priority := 1
				if kind == scratchAddress && typ != nil && typ.Bytes() > 0 {
					size = max(size, typ.Bytes())
					priority = 4
				}
				if assign, ok := e.(*Assign); ok {
					if kind == scratchWrite {
						typ = assign.Src.ExprType()
						priority = 2
					}
					if kind == scratchRead && expr == assign.Src {
						typ = assign.Dst.ExprType()
						priority = 3
					}
				}
				if call, ok := e.(*CallEffect); ok && kind == scratchWrite {
					typ = call.Call.ExprType()
					priority = 4
				}
				if kind == scratchWrite && (e.EffectMeta().InstOp == asm.OpFST || e.EffectMeta().InstOp == asm.OpFSTP) {
					switch size {
					case 4:
						typ = &typeinfo.Primitive{TypeKind: typeinfo.KFloat, Name: "float", Size: 4}
					case 8:
						typ = typeinfo.Double
					case 10:
						typ = typeinfo.F80
					}
					priority = 4
				}
				a.accesses = append(a.accesses, scratchAccess{point, ScratchRange{off, size}, kind, expr, typ, priority})
				return expr
			})
			w.call = func(w *semRewriter, c *Call, meta machine.Meta) (*Call, bool, bool) {
				// Expected argument types outrank width-only views, including pointees.
				for i, arg := range c.Args {
					if c.Function != nil && i < len(c.Params) {
						target := arg
						typ := c.Params[i].Type
						kind := scratchRead
						if addr, ok := arg.(*AddressOf); ok {
							target = addr.Target
							kind = scratchAddress
							if ptr, ok := typ.(*typeinfo.Pointer); ok {
								typ = ptr.Elem
							}
						}
						if off, size, ok := scratchSlotRangeExpr(target); ok {
							if kind == scratchAddress && typ != nil {
								size = max(size, typ.Bytes())
							}
							a.accesses = append(a.accesses, scratchAccess{point, ScratchRange{off, size}, kind, target, typ, 4})
						}
					}
				}
				return c, false, false
			}
			w.rewriteEffect(e)
			var dst, value Expr
			switch effect := e.(type) {
			case *Assign:
				dst, value = effect.Dst, effect.Src
			case *CallEffect:
				dst, value = effect.Result, effect.Call
			}
			if off, size, ok := scratchSlotRangeExpr(dst); ok {
				a.writes[point] = len(a.defs)
				a.defs = append(a.defs, scratchDef{point: point, region: ScratchRange{off, size}, value: value})
			}
		}
	}
	sorted := slices.Clone(a.accesses)
	slices.SortFunc(sorted, func(x, y scratchAccess) int { return x.region.Offset - y.region.Offset })
	for _, access := range sorted {
		r := access.region
		if len(a.objects) == 0 || !a.objects[len(a.objects)-1].region.Overlaps(r) {
			a.objects = append(a.objects, &scratchObject{region: r})
		} else {
			o := a.objects[len(a.objects)-1]
			o.region.Size = max(o.region.End(), r.End()) - o.region.Offset
		}
	}
	for _, access := range a.accesses {
		o := a.object(access.region)
		if access.kind == scratchAddress {
			o.disposition = scratchLocal
		}
		// Bitfield paths need the declared projection, not a whole-storage alias.
		if ref, ok := access.expr.(*SymbolRef); ok {
			if _, ok := ref.Path.(*symresolve.SymbolBitfield); ok {
				o.disposition = scratchLocal
			}
		}
		o.observeType(access)
	}
	for _, o := range a.objects {
		if o.typ == nil {
			o.typ = scratchTypeForWidth(o.region.Size)
		}
		switch o.typ.Kind() {
		case typeinfo.KStruct, typeinfo.KUnion, typeinfo.KArray:
			o.disposition = scratchLocal
		}
	}
	a.predecessors, a.entries = scratchPredecessors(f)
	return a
}

// observeType ranks full-object evidence, preferring explicit typed operations
// over inferred source types and neutral access widths. Lane views never narrow
// a wider object to the type of its first word.
func (o *scratchObject) observeType(access scratchAccess) {
	if access.region != o.region || access.typ == nil || access.typ.Bytes() != o.region.Size {
		return
	}
	priority := access.priority * 4
	switch access.typ.Kind() {
	case typeinfo.KPointer, typeinfo.KStruct, typeinfo.KUnion, typeinfo.KArray, typeinfo.KFloat:
		priority += 2
	}
	if priority > o.priority {
		o.typ, o.priority = access.typ, priority
	}
}

// object finds the physical object containing an inventoried access.
func (a *scratchAnalysis) object(r ScratchRange) *scratchObject {
	for _, o := range a.objects {
		if o.region.Contains(r) {
			return o
		}
	}
	panic(fmt.Sprintf("scratch access outside inventory: %+v", r))
}

// scratchPredecessors derives edges from semantic terminators and CFG fallthrough.
func scratchPredecessors(f *Func) ([][]int, []bool) {
	index := map[machine.BlockID]int{}
	for i, b := range f.Blocks {
		index[b.ID] = i
	}
	preds := make([][]int, len(f.Blocks))
	entries := make([]bool, len(f.Blocks))
	for i, b := range f.Blocks {
		var targets []machine.BlockID
		terminal := false
		if len(b.Effects) > 0 {
			switch e := b.Effects[len(b.Effects)-1].(type) {
			case *Branch:
				targets = []machine.BlockID{e.TrueBlock, e.FalseBlock}
				terminal = true
			case *Jump:
				targets = []machine.BlockID{e.To}
				terminal = true
			case *TableJump:
				targets = e.Targets
				terminal = true
			case *Return:
				terminal = true
			}
		}
		if !terminal {
			if f.CFG != nil {
				targets = f.CFG.Successors(b.ID)
			} else if i+1 < len(f.Blocks) {
				targets = []machine.BlockID{f.Blocks[i+1].ID}
			}
		}
		for _, target := range targets {
			if j, ok := index[target]; ok && !slices.Contains(preds[j], i) {
				preds[j] = append(preds[j], i)
			}
		}
	}
	// Seed every disconnected component, including unreachable cycles, with
	// undefined storage so missing definitions cannot be mistaken for constants.
	visited := make([]bool, len(f.Blocks))
	var mark func(int)
	mark = func(i int) {
		if visited[i] {
			return
		}
		visited[i] = true
		for j, p := range preds {
			if slices.Contains(p, i) {
				mark(j)
			}
		}
	}
	for i := range f.Blocks {
		if i == 0 || len(preds[i]) == 0 {
			entries[i] = true
			mark(i)
		}
	}
	for i := range f.Blocks {
		if !visited[i] {
			entries[i] = true
			mark(i)
		}
	}
	return preds, entries
}

// cloneScratchState copies definition sets for an independent transfer state.
func cloneScratchState(s scratchState) scratchState {
	out := scratchState{}
	for offset, defs := range s {
		out[offset] = maps.Clone(defs)
	}
	return out
}

// sameScratchState compares data-flow states without reflection.
func sameScratchState(a, b scratchState) bool {
	if len(a) != len(b) {
		return false
	}
	for offset, defs := range a {
		if !maps.Equal(defs, b[offset]) {
			return false
		}
	}
	return true
}

// transfer replaces only the bytes written by a definition.
func (a *scratchAnalysis) transfer(s scratchState, point scratchPoint) {
	if id, ok := a.writes[point]; ok {
		r := a.defs[id].region
		for off := r.Offset; off < r.End(); off++ {
			s[off] = map[int]bool{id: true}
		}
	}
}

// solve computes a monotone reaching-definition fixed point across joins and loops.
func (a *scratchAnalysis) solve(f *Func) {
	in := make([]scratchState, len(f.Blocks))
	out := make([]scratchState, len(f.Blocks))
	changed := true
	for changed {
		changed = false
		for bi, b := range f.Blocks {
			state := scratchState{}
			if a.entries[bi] {
				for _, o := range a.objects {
					for off := o.region.Offset; off < o.region.End(); off++ {
						state[off] = map[int]bool{-1: true}
					}
				}
			}
			for _, pred := range a.predecessors[bi] {
				for off, defs := range out[pred] {
					if state[off] == nil {
						state[off] = map[int]bool{}
					}
					maps.Copy(state[off], defs)
				}
			}
			in[bi] = cloneScratchState(state)
			for ei := range b.Effects {
				a.transfer(state, scratchPoint{bi, ei})
			}
			if !sameScratchState(state, out[bi]) {
				out[bi] = state
				changed = true
			}
		}
	}
	a.exits = out
	for bi, b := range f.Blocks {
		state := in[bi]
		for ei := range b.Effects {
			point := scratchPoint{bi, ei}
			a.reads[point] = cloneScratchState(state)
			a.transfer(state, point)
		}
	}
}
