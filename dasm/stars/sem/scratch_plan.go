package sem

import (
	"fmt"
	"maps"
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type scratchUseKey struct {
	point  scratchPoint
	region ScratchRange
}

// buildScratchRecoveryPlan chooses one disposition per object. Promotion is
// monotone; dependent aliases are reconsidered whenever storage must survive.
func buildScratchRecoveryPlan(f *Func, a *scratchAnalysis) map[scratchUseKey]Expr {
	a.recoverLifetimes()
	originals := map[scratchUseKey]Expr{}
	for _, access := range a.accesses {
		if access.kind != scratchWrite {
			originals[scratchUseKey{access.point, access.region}] = access.expr
		}
	}
	for _, def := range a.defs {
		o := def.object
		if o.disposition == scratchInline && !scratchExpressionPure(def.value) {
			o.disposition = scratchTemp
		}
	}
	names := map[string]bool{}
	for _, b := range f.Blocks {
		for _, effect := range b.Effects {
			walkEffect(effect, func(e Expr) {
				switch v := e.(type) {
				case *Temp:
					names[v.Name] = true
				case *Local:
					names[v.Name] = true
				case *SymbolRef:
					names[v.Path.String()] = true
				}
			})
		}
	}
	for _, o := range a.objects {
		name := fmt.Sprintf("t_scratch_m%x", -o.region.Offset)
		if o.region.Offset >= 0 {
			name = fmt.Sprintf("t_scratch_p%x", o.region.Offset)
		}
		baseName := name
		for suffix := 2; names[name]; suffix++ {
			name = fmt.Sprintf("%s_%d", baseName, suffix)
		}
		names[name] = true
		o.storage = &Temp{Name: name, TypeInfo: o.typ}
		if o.disposition == scratchLocal {
			local := &typeinfo.FunctionVar{Name: name, Type: o.typ, BPOffset: o.region.Offset}
			o.storage = &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: local}}
		}
	}
	for {
		changed := false
		values := map[scratchUseKey]Expr{}
		active := map[scratchUseKey]bool{}
		var resolve func(scratchUseKey, typeinfo.Type) Expr
		resolve = func(key scratchUseKey, typ typeinfo.Type) Expr {
			o := a.readObjects[key]
			if o.disposition == scratchUnresolved {
				return originals[key]
			}
			if o.disposition != scratchInline {
				return scratchStorageView(o, key.region, typ)
			}
			if value, ok := values[key]; ok {
				return value
			}
			if active[key] {
				o.disposition = scratchTemp
				changed = true
				return scratchStorageView(o, key.region, typ)
			}
			active[key] = true
			defer delete(active, key)
			state := a.reads[key.point]
			ids := state[key.region.Offset]
			var value Expr
			valid := len(ids) > 0
			ordered := make([]int, 0, len(ids))
			for id := range ids {
				ordered = append(ordered, id)
			}
			slices.Sort(ordered)
			for _, id := range ordered {
				if id < 0 {
					valid = false
					break
				}
				def := a.defs[id]
				if !def.region.Contains(key.region) {
					valid = false
					break
				}
				for off := key.region.Offset; off < key.region.End(); off++ {
					if !maps.Equal(ids, state[off]) {
						valid = false
						break
					}
				}
				if !valid {
					break
				}
				w := scratchAccessRewriter(func(expr Expr, kind scratchAccessKind, expected typeinfo.Type) Expr {
					off, size, _ := scratchSlotRangeExpr(expr)
					return resolve(scratchUseKey{def.point, ScratchRange{off, size}}, expected)
				})
				candidate, _ := w.rewriteExpr(def.value)
				if !scratchCanMove(f, a, candidate, id, key) {
					valid = false
					break
				}
				candidate, valid = scratchProjectValue(candidate, def.region, key.region, def.object.typ, typ)
				if !valid {
					break
				}
				if value != nil && (!sameExpr(value, candidate) || !typeinfo.Equals(value.ExprType(), candidate.ExprType())) {
					valid = false
					break
				}
				value = candidate
			}
			if !valid || o.disposition != scratchInline {
				if o.disposition == scratchInline {
					o.disposition = scratchTemp
					changed = true
				}
				return scratchStorageView(o, key.region, typ)
			}
			values[key] = value
			return value
		}
		for _, access := range a.accesses {
			if access.kind == scratchRead {
				resolve(scratchUseKey{access.point, access.region}, access.expr.ExprType())
			}
		}
		if !changed {
			return values
		}
	}
}

// scratchStorageView projects a physical lane from a recovered semantic object.
func scratchStorageView(o *scratchObject, r ScratchRange, typ typeinfo.Type) LValue {
	if r == o.region {
		return o.storage
	}
	return &Part{Base: o.storage, ByteOff: r.Offset - o.region.Offset, Width: r.Size, TypeInfo: typ}
}

// scratchProjectValue preserves store width before selecting a smaller read.
func scratchProjectValue(value Expr, stored, read ScratchRange, storageType, typ typeinfo.Type) (Expr, bool) {
	sourceType := value.ExprType()
	if sourceType == nil {
		return nil, false
	}
	if sourceType.Bytes() != stored.Size {
		// An x87 spill retains extended precision; a narrowing reload is a cast.
		if sourceType.Kind() == typeinfo.KFloat && storageType.Kind() == typeinfo.KFloat && storageType.Bytes() == stored.Size {
			value = &Cast{Value: value, To: storageType.String(), TypeInfo: storageType}
		} else if stored.Size <= 4 && typeinfo.IsIntLike(sourceType) {
			t := scratchTypeForWidth(stored.Size)
			value = &Cast{Value: value, To: t.String(), TypeInfo: t}
		} else {
			return nil, false
		}
	}
	if stored == read {
		return value, true
	}
	if c, ok := value.(*Const); ok && read.Size <= 4 {
		return &Const{TypeInfo: typ, U64: (c.U64 >> uint((read.Offset-stored.Offset)*8)) & ((uint64(1) << uint(read.Size*8)) - 1)}, true
	}
	if stored.Size == 4 && read.Size == 2 && (read.Offset == stored.Offset || read.Offset == stored.Offset+2) {
		part := machine.WordLow
		if read.Offset != stored.Offset {
			part = machine.WordHigh
		}
		return &Word{Parent: value, Part: part}, true
	}
	if read.Size == 1 && (stored.Size == 2 || stored.Size == 4) {
		offset := read.Offset - stored.Offset
		if stored.Size == 4 {
			part := machine.WordLow
			if offset >= 2 {
				part = machine.WordHigh
			}
			value = &Word{Parent: value, Part: part}
		}
		part := machine.ByteLow
		if offset%2 != 0 {
			part = machine.ByteHigh
		}
		return &Byte{Parent: value, Part: part, TypeInfo: typ}, true
	}
	return nil, false
}

// applyScratchRecoveryPlan rewrites uses and definitions only after all object
// dispositions have stabilized, preserving addressable objects as real locals.
func applyScratchRecoveryPlan(f *Func, a *scratchAnalysis, values map[scratchUseKey]Expr) bool {
	changed := false
	for _, o := range a.objects {
		if o.disposition == scratchLocal {
			f.RecoveredLocals = append(f.RecoveredLocals, o.storage.(*SymbolRef).Path.(*symresolve.SymbolRoot).Symbol.(*typeinfo.FunctionVar))
		}
	}
	for bi, b := range f.Blocks {
		effects := make([]Effect, 0, len(b.Effects))
		for ei, effect := range b.Effects {
			point := scratchPoint{bi, ei}
			if id, ok := a.writes[point]; ok && a.defs[id].object.disposition == scratchInline {
				changed = true
				continue
			}
			w := scratchAccessRewriter(func(expr Expr, kind scratchAccessKind, typ typeinfo.Type) Expr {
				off, size, _ := scratchSlotRangeExpr(expr)
				region := ScratchRange{off, size}
				o := a.readObjects[scratchUseKey{point, region}]
				if kind == scratchWrite {
					o = a.defs[a.writes[point]].object
				}
				if o.disposition == scratchUnresolved {
					return expr
				}
				if kind == scratchRead && o.disposition == scratchInline {
					return values[scratchUseKey{point, region}]
				}
				if ref, ok := expr.(*SymbolRef); ok && o.disposition == scratchLocal {
					switch ref.Path.(type) {
					case *symresolve.SymbolField, *symresolve.SymbolBitfield:
						return &SymbolRef{Path: replaceScratchRoot(ref.Path, o)}
					}
				}
				return scratchStorageView(o, region, expr.ExprType())
			})
			next, c := w.rewriteEffect(effect)
			changed = changed || c
			effects = append(effects, next)
		}
		f.Blocks[bi].Effects = effects
	}
	return changed
}

// replaceScratchRoot preserves existing field and bitfield paths on a local.
func replaceScratchRoot(path symresolve.SymbolPath, o *scratchObject) symresolve.SymbolPath {
	switch p := path.(type) {
	case *symresolve.SymbolScratch:
		root := o.storage.(*SymbolRef).Path
		if p.BPOffset == o.region.Offset && p.StorageSize == o.region.Size {
			return root
		}
		return &symresolve.SymbolOffset{Base: root, Offset: p.BPOffset - o.region.Offset, Result: p.TypeInfo}
	case *symresolve.SymbolOffset:
		next := *p
		next.Base = replaceScratchRoot(p.Base, o)
		return &next
	case *symresolve.SymbolField:
		next := *p
		next.Base = replaceScratchRoot(p.Base, o)
		return &next
	case *symresolve.SymbolBitfield:
		next := *p
		next.Base = replaceScratchRoot(p.Base, o)
		return &next
	default:
		panic("unexpected scratch path")
	}
}
