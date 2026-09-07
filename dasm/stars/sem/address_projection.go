package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// AddressExpr is a normalized source address with residual byte displacement.
type AddressExpr struct {
	Base   Expr
	Offset int
	Terms  []ScaledTerm

	// Deref indicates that the machine effective address was derived by
	// loading/following Base as a pointer.
	//
	// A pointer-typed Base with Deref == false denotes storage containing
	// the pointer itself. Its offsets refer to the pointer representation.
	//
	// A pointer-typed Base with Deref == true permits one projection through
	// the pointer into its pointee. That dereference is consumed and must not
	// implicitly propagate through pointer-valued subobjects.
	Deref bool
}

// ScaledTerm is a dynamic address term measured in bytes.
type ScaledTerm struct {
	Expr  Expr
	Scale int
}

// resolveAddressLValue resolves a machine memory address through typed semantic projection.
func (c *machineConverter) resolveAddressLValue(mem machine.MemoryAddress, width int, expected typeinfo.Type) (LValue, bool) {
	addr, ok := c.ctx.symbols.addressFromMemory(mem, expected)
	if !ok {
		return nil, false
	}
	semantic, ok := c.semanticResolvedAddress(addr)
	if !ok {
		return nil, false
	}
	if lvalue, ok := c.consumeAddress(semantic, width); ok {
		return lvalue, true
	}
	if addr.exact != nil {
		exact, ok := c.convertSymbolPath(addr.exact, addr.exact.Type())
		if ok {
			lvalue, ok := exact.(LValue)
			return lvalue, ok
		}
	}
	return nil, false
}

// resolveAddressValue resolves a machine address-valued expression through typed semantic projection.
func (c *machineConverter) resolveAddressValue(value machine.Value) (Expr, bool) {
	switch value.(type) {
	case *machine.Binary, *machine.Address:
	default:
		return nil, false
	}
	if address, ok := value.(*machine.Address); ok {
		if resolved, ok := c.ctx.symbols.addressFromMemory(address.Addr, nil); ok {
			addr, ok := c.semanticResolvedAddress(resolved)
			if !ok {
				return nil, false
			}
			projected, ok := c.consumeAddressProjection(addr, 0)
			if !ok {
				return nil, false
			}
			target, ok := projected.(LValue)
			if !ok {
				return nil, false
			}
			return &AddressOf{Target: target, TypeInfo: typeinfo.U16}, true
		}
	}
	if resolved, ok := c.ctx.symbols.addressFromValue(value, 0); ok && resolved.hasBase() {
		addr, ok := c.semanticResolvedAddress(resolved)
		if !ok {
			return nil, false
		}
		return c.consumeAddressProjection(addr, 0)
	}
	return nil, false
}

// semanticResolvedAddress builds a semantic address from resolver-owned symbolic data.
func (c *machineConverter) semanticResolvedAddress(addr resolvedAddress) (AddressExpr, bool) {
	var base Expr
	var ok bool
	if addr.base != nil {
		base, ok = c.convertSymbolPath(addr.base, addr.base.Type())
		if !ok {
			return AddressExpr{}, false
		}
	} else if addr.baseValue != nil {
		base = c.convertValue(addr.baseValue)
	}
	terms := make([]ScaledTerm, 0, len(addr.terms))
	for _, term := range addr.terms {
		if term.value == nil {
			continue
		}
		terms = append(terms, ScaledTerm{Expr: c.convertValue(term.value), Scale: term.scale})
	}
	return AddressExpr{Base: base, Offset: addr.offset, Terms: terms, Deref: addr.deref}, true
}

// joinSemanticAddressTerms rebuilds semantic residual byte terms.
func joinSemanticAddressTerms(a, b Expr) Expr {
	if a == nil {
		return b
	}
	if b == nil {
		return a
	}
	return &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: a, RHS: b}
}

// consumeAddress resolves residual address terms according to the base expression type.
func (c *machineConverter) consumeAddress(addr AddressExpr, width int) (LValue, bool) {
	expr, ok := c.consumeAddressExpr(addr, width)
	if !ok {
		return nil, false
	}
	lvalue, ok := expr.(LValue)
	return lvalue, ok
}

// consumeAddressExpr resolves residual address terms according to the base expression type.
func (c *machineConverter) consumeAddressExpr(addr AddressExpr, width int) (Expr, bool) {
	current := addr.Base
	offset := addr.Offset
	terms := append([]ScaledTerm(nil), addr.Terms...)

	// This means the machine address was actually formed through a
	// pointer value, rather than referring to storage containing one.
	deref := addr.Deref

	for {
		// Deref is a one-shot permission to interpret this pointer's
		// offsets/terms relative to its pointee.
		if deref {
			if _, ok := current.ExprType().(*typeinfo.Pointer); ok {
				next, nextOffset, nextTerms, changed := c.consumeDereferencedPointerStep(current, offset, terms, width)
				if changed {
					current = next
					offset = nextOffset
					terms = nextTerms

					// We crossed the pointer boundary. Do not implicitly
					// cross another pointer-valued field.
					deref = false
					continue
				}

				// We have an actual dereference, but there wasn't a
				// field/index projection to consume. Leave deref=true
				// for terminal handling (*p, p[offset], etc).
				break
			}
		}

		// Ordinary object-relative projection. This must NOT traverse
		// pointer types.
		next, nextOffset, nextTerms, changed := c.consumeObjectAddressStep(current, offset, terms, width)
		if !changed {
			break
		}

		current = next
		offset = nextOffset
		terms = nextTerms
	}
	if offset == 0 && len(terms) == 0 {
		if ptr, ok := current.ExprType().(*typeinfo.Pointer); ok && deref && width > 0 && ptr.Elem != nil {

			if ptr.Elem.Bytes() == width {
				if _, field := current.(*FieldAccess); !field {
					return &Deref{Pointer: current, Width: width, TypeInfo: ptr.Elem}, true
				}
			}

			// A partial load of a pointer value through another pointer:
			//
			//     char **ppszBeg
			//     load word [ppszBeg]
			//
			// represents LOWORD(*ppszBeg), not ppszBeg and not an
			// arbitrary partial load of the pointee object.
			if partialPointerPointee(ptr, 0, width) {
				if _, field := current.(*FieldAccess); !field {
					whole := &Deref{Pointer: current, Width: ptr.Elem.Bytes(), TypeInfo: ptr.Elem}
					return &Part{Base: whole, ByteOff: 0, Width: width, TypeInfo: intTypeForWidth(width)}, true
				}
			}
		}

		if base, ok := current.(LValue); ok && width > 0 && current.ExprType() != nil && current.ExprType().Bytes() > width {
			return &Part{Base: base, ByteOff: 0, Width: width, TypeInfo: intTypeForWidth(width)}, true
		}

		return current, true
	}

	if ptr, ok := current.ExprType().(*typeinfo.Pointer); ok && deref && len(terms) == 0 && ptr.Elem != nil {
		if _, field := current.(*FieldAccess); !field {
			if partialPointerPointee(ptr, offset, width) {
				whole := &Deref{Pointer: current, Width: ptr.Elem.Bytes(), TypeInfo: ptr.Elem}
				return &Part{Base: whole, ByteOff: offset, Width: width, TypeInfo: intTypeForWidth(width)}, true
			}

			return &Deref{Pointer: current, ByteOff: offset, Width: width, TypeInfo: derefType(current, width)}, true
		}
	}

	if base, ok := current.(LValue); ok && len(terms) == 0 && offset >= 0 {
		return &Part{Base: base, ByteOff: offset, Width: width, TypeInfo: intTypeForWidth(width)}, true
	}
	return nil, false
}

// consumeObjectAddressStep projects offsets and terms within a non-pointer object.
func (c *machineConverter) consumeObjectAddressStep(
	current Expr,
	offset int,
	terms []ScaledTerm,
	width int,
) (Expr, int, []ScaledTerm, bool) {
	typ := current.ExprType()

	if typeinfo.IsPointer(typ) {
		return current, offset, terms, false
	}

	if next, nextOffset, changed := c.consumeStructField(current, typ, offset, terms, width); changed {
		return next, nextOffset, terms, true
	}
	if next, nextOffset, nextTerms, changed := consumeArrayTerm(current, typ, offset, terms); changed {
		return next, nextOffset, nextTerms, true
	}
	if next, nextOffset, changed := consumeArrayConstIndex(current, typ, offset, width, true); changed {
		return next, nextOffset, terms, true
	}
	return current, offset, terms, false
}

// consumeDereferencedPointerStep projects one machine-proven pointer dereference.
func (c *machineConverter) consumeDereferencedPointerStep(
	current Expr,
	offset int,
	terms []ScaledTerm,
	width int,
) (Expr, int, []ScaledTerm, bool) {
	ptr, ok := current.ExprType().(*typeinfo.Pointer)
	if !ok {
		return current, offset, terms, false
	}

	if next, nextOffset, nextTerms, changed := consumeArrayTerm(current, ptr, offset, terms); changed {
		return next, nextOffset, nextTerms, true
	}

	if next, nextOffset, changed := c.consumeStructField(current, ptr.Elem, offset, terms, width); changed {
		return next, nextOffset, terms, true
	}

	if next, nextOffset, changed := consumeArrayConstIndex(current, ptr, offset, width, false); changed {
		return next, nextOffset, terms, true
	}

	return current, offset, terms, false
}

// consumeAddressProjection resolves an address-valued expression without
// descending into the target after its offset is consumed.
func (c *machineConverter) consumeAddressProjection(addr AddressExpr, width int) (Expr, bool) {
	current := addr.Base
	offset := addr.Offset
	terms := append([]ScaledTerm(nil), addr.Terms...)
	deref := addr.Deref

	for {
		// We've resolved the address itself. Do not descend into the
		// object at that address.
		if offset == 0 && len(terms) == 0 {
			return current, true
		}
		if !deref {
			if projected, ok := projectPointerAddress(current, offset, terms); ok {
				return projected, true
			}
		}

		// A machine-proven pointer dereference permits exactly one
		// projection through the pointer's pointee type.
		if deref && typeinfo.IsPointer(current.ExprType()) {
			next, nextOffset, nextTerms, changed := c.consumeDereferencedPointerStep(current, offset, terms, width)
			if changed {
				current = next
				offset = nextOffset
				terms = nextTerms
				deref = false
				continue
			}
		}

		// Otherwise we're only allowed to project within the current
		// object. This never implicitly follows a pointer.
		next, nextOffset, nextTerms, changed := c.consumeObjectAddressStep(current, offset, terms, width)
		if !changed {
			return c.consumeAddressExpr(addr, width)
		}

		current = next
		offset = nextOffset
		terms = nextTerms
	}
}

// projectPointerAddress converts residual byte offsets into source-level
// pointer steps when every component aligns with the pointee size.
func projectPointerAddress(pointer Expr, offset int, terms []ScaledTerm) (Expr, bool) {
	ptr, ok := pointer.ExprType().(*typeinfo.Pointer)
	if !ok || ptr.Elem == nil || ptr.Elem.Bytes() <= 0 {
		return nil, false
	}
	elemSize := ptr.Elem.Bytes()
	if offset%elemSize != 0 {
		return nil, false
	}
	var steps Expr
	if fixed := offset / elemSize; fixed != 0 {
		steps = appendSignedPointerStep(steps, &Const{TypeInfo: typeinfo.I16, U64: uint64(absInt(fixed))}, fixed < 0)
	}
	for _, term := range terms {
		if term.Scale%elemSize != 0 {
			return nil, false
		}
		scale := term.Scale / elemSize
		component := term.Expr
		magnitude := absInt(scale)
		if magnitude != 1 {
			component = &Binary{
				TypeInfo: typeinfo.I16,
				Op:       OpMul,
				LHS:      &Const{TypeInfo: typeinfo.I16, U64: uint64(magnitude)},
				RHS:      component,
			}
		}
		steps = appendSignedPointerStep(steps, component, scale < 0)
	}
	if steps == nil {
		return pointer, true
	}
	return &Binary{TypeInfo: pointer.ExprType(), Op: OpAdd, LHS: pointer, RHS: steps}, true
}

// appendSignedPointerStep adds or subtracts one logical pointer-step component.
func appendSignedPointerStep(steps Expr, component Expr, negative bool) Expr {
	if steps == nil {
		if negative {
			return &Unary{TypeInfo: typeinfo.I16, Op: OpNeg, X: component}
		}
		return component
	}
	op := OpAdd
	if negative {
		op = OpSub
	}
	return &Binary{TypeInfo: typeinfo.I16, Op: op, LHS: steps, RHS: component}
}

// absInt returns the non-negative magnitude of an integer.
func absInt(value int) int {
	if value < 0 {
		return -value
	}
	return value
}

// partialPointerPointee returns true if ptr is a pointer to a slice inside another pointer
func partialPointerPointee(ptr *typeinfo.Pointer, offset, width int) bool {
	if !typeinfo.IsPointer(ptr.Elem) {
		return false
	}

	elemWidth := ptr.Elem.Bytes()
	return offset >= 0 && width > 0 && elemWidth > width && offset+width <= elemWidth
}

// consumeStructField consumes a constant offset into a struct field.
func (c *machineConverter) consumeStructField(base Expr, typ typeinfo.Type, offset int, terms []ScaledTerm, width int) (Expr, int, bool) {
	strct, ok := typ.(*typeinfo.Struct)
	if !ok {
		return nil, 0, false
	}
	matches := strct.FieldsContainingOffset(offset)
	matches = c.unionFieldMatches(base, strct, matches)
	if len(matches) == 0 {
		field, fieldOff, ok := zeroLengthArrayFieldAtOffset(strct, offset)
		if !ok {
			return nil, 0, false
		}
		return &FieldAccess{Base: base, Field: field}, fieldOff, true
	}
	if len(matches) != 1 {
		if field, ok := exactFieldAccess(base, matches, width); ok {
			return field, 0, true
		}
		return nil, 0, false
	}
	match := matches[0]
	if match.Off != 0 {
		if !isAggregateType(match.Field.Type) && !typeinfo.IsPointer(match.Field.Type) && match.Off+width > match.Field.Type.Bytes() {
			return nil, 0, false
		}
		field := &FieldAccess{Base: base, Field: match.Field}
		return field, match.Off, true
	}
	if width == 0 {
		if match.Field.Offset != 0 && match.Field.Bitfield == nil {
			return &FieldAccess{Base: base, Field: match.Field}, 0, true
		}
		return nil, 0, false
	}
	if match.Field.Type.Bytes() != width &&
		!(isAggregateType(match.Field.Type) && (len(terms) > 0 || match.Field.Type.Bytes() > width)) &&
		!(typeinfo.IsPointer(match.Field.Type) && match.Field.Type.Bytes() > width) &&
		!(match.Field.Type.Bytes() > width) {
		return nil, 0, false
	}
	return &FieldAccess{Base: base, Field: match.Field}, 0, true
}

// isAggregateType reports whether a type contains addressable subobjects.
func isAggregateType(typ typeinfo.Type) bool {
	switch typ.Kind() {
	case typeinfo.KStruct, typeinfo.KUnion, typeinfo.KArray:
		return true
	default:
		return false
	}
}

// unionFieldMatches narrows overlapping fields using the current block's union context.
func (c *machineConverter) unionFieldMatches(base Expr, strct *typeinfo.Struct, matches []typeinfo.StructFieldMatch) []typeinfo.StructFieldMatch {
	if len(matches) <= 1 || c.ctx.unionContext() == nil {
		return matches
	}
	var selection symresolve.UnionSelection
	var selected bool
	if path, ok := symbolPathForExpr(base); ok {
		selection, selected = c.ctx.unionContext().SelectionFor(path, strct)
	} else if index, ok := base.(*ArrayIndex); ok {
		if root, ok := symbolPathForExpr(index.Base); ok {
			selection, selected = c.ctx.unionContext().AllElementsSelectionFor(root, strct)
		}
	}
	if !selected {
		return matches
	}
	for _, match := range matches {
		if match.Field == selection.Member {
			return []typeinfo.StructFieldMatch{match}
		}
	}
	return matches
}

// exactFieldAccess returns a unique exact non-bitfield field for an access.
func exactFieldAccess(base Expr, matches []typeinfo.StructFieldMatch, width int) (Expr, bool) {
	var out *FieldAccess
	for _, match := range matches {
		if match.Off != 0 || match.Field.Bitfield != nil || match.Field.Type.Bytes() != width {
			continue
		}
		if out != nil {
			return nil, false
		}
		out = &FieldAccess{Base: base, Field: match.Field}
	}
	return out, out != nil
}

// consumeArrayTerm consumes a scaled dynamic term into an array index.
func consumeArrayTerm(base Expr, typ typeinfo.Type, offset int, terms []ScaledTerm) (Expr, int, []ScaledTerm, bool) {
	elem := indexElementType(typ)
	if elem == nil || elem.Bytes() <= 0 {
		return nil, 0, nil, false
	}
	var index Expr
	nextTerms := make([]ScaledTerm, 0, len(terms))
	for _, term := range terms {
		if term.Scale != elem.Bytes() {
			nextTerms = append(nextTerms, term)
			continue
		}
		index = joinSemanticAddressTerms(index, term.Expr)
	}
	if index == nil {
		return nil, 0, nil, false
	}
	nextOffset := offset
	if offset%elem.Bytes() == 0 {
		index = offsetArrayIndex(index, offset/elem.Bytes())
		nextOffset = 0
	}
	return &ArrayIndex{Base: base, Index: index, TypeInfo: elem}, nextOffset, nextTerms, true
}

// offsetArrayIndex folds a constant element offset into an array index.
func offsetArrayIndex(index Expr, offset int) Expr {
	if offset == 0 {
		return index
	}
	op := OpAdd
	if offset < 0 {
		op = OpSub
		offset = -offset
	}
	return &Binary{
		TypeInfo: typeinfo.U16,
		Op:       op,
		LHS:      index,
		RHS:      &Const{TypeInfo: typeinfo.U16, U64: uint64(offset)},
	}
}

// consumeArrayConstIndex consumes a constant byte offset into an array index.
func consumeArrayConstIndex(base Expr, typ typeinfo.Type, offset int, width int, allowZero bool) (Expr, int, bool) {
	elem := indexElementType(typ)
	if elem == nil || elem.Bytes() <= 0 || offset < 0 {
		return nil, 0, false
	}
	if _, ok := typ.(*typeinfo.Pointer); ok && (offset < elem.Bytes() || offset%elem.Bytes() != 0) {
		return nil, 0, false
	}
	if offset == 0 {
		if _, ok := typ.(*typeinfo.Array); !ok {
			return nil, 0, false
		}
	}
	if offset == 0 && (!allowZero || width == 0 || typ.Bytes() == width || elem.Bytes() < width) {
		return nil, 0, false
	}
	index := offset / elem.Bytes()
	remainder := offset % elem.Bytes()
	if remainder != 0 && (width == 0 || remainder+width > elem.Bytes()) {
		return nil, 0, false
	}
	return &ArrayIndex{Base: base, Index: &Const{TypeInfo: typeinfo.U16, U64: uint64(index)}, TypeInfo: elem}, remainder, true
}

// zeroLengthArrayFieldAtOffset returns the flexible array field spanning offset.
func zeroLengthArrayFieldAtOffset(strct *typeinfo.Struct, offset int) (*typeinfo.StructField, int, bool) {
	for i := range strct.Fields {
		field := &strct.Fields[i]
		if !isZeroLengthArray(field.Type) || offset < field.Offset {
			continue
		}
		return field, offset - field.Offset, true
	}
	return nil, 0, false
}

// isZeroLengthArray reports whether typ is a flexible array marker.
func isZeroLengthArray(typ typeinfo.Type) bool {
	array, ok := typ.(*typeinfo.Array)
	return ok && array.Count == 0
}

// indexElementType returns the element type for semantic indexing.
func indexElementType(typ typeinfo.Type) typeinfo.Type {
	switch t := typ.(type) {
	case *typeinfo.Array:
		return t.Elem
	case *typeinfo.Pointer:
		return t.Elem
	default:
		return nil
	}
}
