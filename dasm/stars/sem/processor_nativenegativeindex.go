package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// nativeNegativeIndexProcessor makes reads through an index that may be -1
// return what the original read: Win16 index -1 selected the bytes its
// layout placed before the array, which native layouts place elsewhere.
type nativeNegativeIndexProcessor struct {
	ctx *FuncContext
}

// NegativeIndexAudited lists sites the -1 dataflow reports that were checked
// by hand, keyed by function and index local, with why they need no repair.
var NegativeIndexAudited = map[string]map[string]string{
	"CheckWeapons": {"init": "init is -1 only for non-weapon slots, whose dxyPart stays -1 and skips the store"},
	"FLoadGame": {"iPlayer": "iPlayer is -1 only for the host file (dt 2); each read is behind dt != 2, " +
		"a non-hst extension, or cturn > 1, which a single-year host file never reaches"},
	"FTravelThroughMineFields": {"iType": "cFields counts unvisited field segments, so the scan always finds one " +
		"starting within the move and sets iType"},
	"PszFormatString": {"iMineral": "display text only; vrgszUnits[-1] was a Win16 pointer no native layout can reproduce"},
	"PszGetTaskName": {"iZip": "the inner switch sets iZip for every opOrd the outer switch admits",
		"icr": "icr is set whenever ids changes, and ids == idsAction returns first"},
	"SendBattleMessages": {"iplrStarbase": "fStarbaseDied implies a planet battle with an owned starbase, which sets iplrStarbase"},
}

// ProcessFunc guards each unaudited -1 read with its Win16 alias.
func (p *nativeNegativeIndexProcessor) ProcessFunc(_ *Result, f *Func) bool {
	facts := AnalyzeNegativeIndexes(f, p.ctx.fs)
	changed := false
	for _, site := range facts.Sites {
		if _, ok := NegativeIndexAudited[p.ctx.fs.Name][site.Index.Name]; ok {
			continue
		}
		bi := -1
		for i := range f.Blocks {
			if f.Blocks[i].ID == site.Block {
				bi = i
			}
		}
		effect := f.Blocks[bi].Effects[site.Effect]
		found := false
		w := semRewriter{
			lvalue: func(_ *semRewriter, v LValue) (LValue, bool, bool) {
				if accessPathContains(v, site.Access) {
					panic(fmt.Sprintf("native-negative-index: %s writes through %s, which may be -1", p.ctx.fs.Name, site.Index.Name))
				}
				return nil, false, false
			},
			expr: func(_ *semRewriter, e Expr) (Expr, bool, bool) {
				if addr, ok := e.(*AddressOf); ok && accessPathContains(addr.Target, site.Access) {
					panic(fmt.Sprintf("native-negative-index: %s takes an address through %s, which may be -1", p.ctx.fs.Name, site.Index.Name))
				}
				if !accessPathContains(e, site.Access) {
					return nil, false, false
				}
				found = true
				return &Cond{TypeInfo: e.ExprType(),
					Cond: &Compare{Op: CompareNE, LHS: site.Index, RHS: minusOne(site.Index.Type)},
					Then: e, Else: p.win16Alias(e, site)}, true, true
			},
		}
		next, _ := w.rewriteEffect(effect)
		if !found {
			panic(fmt.Sprintf("native-negative-index: %s: no read of %s in %s", p.ctx.fs.Name, FormatExpr(site.Access), site.Block))
		}
		f.Blocks[bi].Effects[site.Effect] = next
		changed = true
	}
	return changed
}

// accessPathContains reports whether expr selects storage through target by
// field and element selection alone.
func accessPathContains(expr Expr, target *ArrayIndex) bool {
	for {
		switch e := expr.(type) {
		case *ArrayIndex:
			if e == target {
				return true
			}
			expr = e.Base
		case *FieldAccess:
			expr = e.Base
		default:
			return false
		}
	}
}

// minusOne returns -1 in an integer type's width.
func minusOne(typ typeinfo.Type) *Const {
	return &Const{TypeInfo: typ, U64: uint64(1)<<(8*typ.Bytes()) - 1}
}

// win16Alias returns the value the original read for path with the site's
// index at -1: the bytes before the array in the Win16 data segment for a
// global array, or in the enclosing struct for an array field.
func (p *nativeNegativeIndexProcessor) win16Alias(path Expr, site NegativeIndexSite) Expr {
	sub := 0
	for expr := path; expr != Expr(site.Access); {
		field, ok := expr.(*FieldAccess)
		if !ok {
			panic(fmt.Sprintf("native-negative-index: %s: %s selects a variable element after the -1 index", p.ctx.fs.Name, FormatExpr(path)))
		}
		sub += field.Field.Offset
		expr = field.Base
	}
	width := path.ExprType().Bytes()
	offset := sub - site.Access.TypeInfo.Bytes()
	var parts []aliasPart
	switch base := site.Access.Base.(type) {
	case *Global:
		addr := base.Addr
		addr.Off = uint32(int(addr.Off) + offset)
		holder, at, ok := p.ctx.sdb.GetGlobalContaining(addr)
		if !ok {
			panic(fmt.Sprintf("native-negative-index: %s: no global holds %s[-1] at %s", p.ctx.fs.Name, base.Name, addr))
		}
		parts = p.aliasParts(&Global{GlobalVar: holder}, holder.Type, 0, at, width)
	case *FieldAccess:
		owner, _ := typeinfo.UnwrapPointer(base.Base.ExprType())
		at := base.Field.Offset + offset
		if at < 0 {
			panic(fmt.Sprintf("native-negative-index: %s: %s[-1] lies before its struct", p.ctx.fs.Name, FormatExpr(base)))
		}
		parts = p.aliasParts(base.Base, owner, 0, at, width)
	default:
		panic(fmt.Sprintf("native-negative-index: %s: unsupported array base %s", p.ctx.fs.Name, FormatExpr(site.Access.Base)))
	}
	return composeAlias(parts, path.ExprType())
}

// aliasPart is an integer piece of a Win16 alias at a byte shift from its start.
type aliasPart struct {
	value Expr
	shift int
}

// aliasParts returns the integer leaves of expr (of type typ, at byte off in
// the alias root) covering the alias bytes [start, start+width).
func (p *nativeNegativeIndexProcessor) aliasParts(expr Expr, typ typeinfo.Type, off, start, width int) []aliasPart {
	switch t := typ.(type) {
	case *typeinfo.Primitive:
		if t.TypeKind != typeinfo.KInt || off < start || off+t.Size > start+width {
			panic(fmt.Sprintf("native-negative-index: %s: alias partially covers %s", p.ctx.fs.Name, FormatExpr(expr)))
		}
		return []aliasPart{{value: expr, shift: off - start}}
	case *typeinfo.Struct:
		var parts []aliasPart
		for b := max(start-off, 0); b < min(start+width-off, t.Size); {
			var field *typeinfo.StructField
			for _, match := range t.FieldsContainingOffset(b) {
				if !match.Field.IsBitfield() {
					field = match.Field
					break
				}
			}
			if field == nil {
				panic(fmt.Sprintf("native-negative-index: %s: no plain field of %s at %d", p.ctx.fs.Name, t.Name, b))
			}
			parts = append(parts, p.aliasParts(&FieldAccess{Base: expr, Field: field}, field.Type, off+field.Offset, start, width)...)
			b = field.Offset + field.Size
		}
		return parts
	case *typeinfo.Array:
		size := t.Elem.Bytes()
		var parts []aliasPart
		for k := max(start-off, 0) / size; k < t.Count && off+k*size < start+width; k++ {
			element := &ArrayIndex{Base: expr, Index: &Const{TypeInfo: typeinfo.I16, U64: uint64(k)}, TypeInfo: t.Elem}
			parts = append(parts, p.aliasParts(element, t.Elem, off+k*size, start, width)...)
		}
		return parts
	}
	panic(fmt.Sprintf("native-negative-index: %s: alias covers %s, whose native layout differs", p.ctx.fs.Name, FormatExpr(expr)))
}

// composeAlias assembles alias parts into one value of typ, little endian.
func composeAlias(parts []aliasPart, typ typeinfo.Type) Expr {
	if len(parts) == 1 && parts[0].shift == 0 {
		return parts[0].value
	}
	wide := unsignedOfSize(typ.Bytes())
	var out Expr
	for _, part := range parts {
		var value Expr = castTo(castTo(part.value, unsignedOfSize(part.value.ExprType().Bytes())), wide)
		if part.shift != 0 {
			value = &Binary{TypeInfo: wide, Op: OpShl, LHS: value, RHS: &Const{TypeInfo: typeinfo.I16, U64: uint64(8 * part.shift)}}
		}
		if out == nil {
			out = value
		} else {
			out = &Binary{TypeInfo: wide, Op: OpOr, LHS: out, RHS: value}
		}
	}
	return castTo(out, typ)
}

// unsignedOfSize returns the unsigned integer type of a byte width.
func unsignedOfSize(size int) *typeinfo.Primitive {
	switch size {
	case 1:
		return typeinfo.U8
	case 2:
		return typeinfo.U16
	case 4:
		return typeinfo.U32
	}
	panic(fmt.Sprintf("native-negative-index: no unsigned type of %d bytes", size))
}
