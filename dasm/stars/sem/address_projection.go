package sem

import (
	"fmt"

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

// BitfieldSignedness describes what an extraction proves about a declared
// bitfield's integer base type.
type BitfieldSignedness uint8

const (
	BitfieldSignednessUnknown BitfieldSignedness = iota
	BitfieldUnsigned
	BitfieldSigned
)

// BitfieldAccess describes one physical bit range within an integer storage unit.
type BitfieldAccess struct {
	StorageWidth int
	BitOff       int
	BitWidth     int
	Signedness   BitfieldSignedness
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
		if addr.offset != 0 || len(addr.terms) != 0 {
			c.ctx.log.Warn(
				"typed address projection using exact fallback with residual components",
				"instOff", fmt.Sprintf("%04x", c.ctx.currentInstOff),
				"memory", mem.String(),
				"exact", addr.exact.String(),
				"offset", fmt.Sprintf("0x%x", addr.offset),
				"terms", len(addr.terms),
			)
		}
		exact, ok := c.convertSymbolPath(addr.exact, addr.exact.Type())
		if ok {
			lvalue, ok := exact.(LValue)
			return lvalue, ok
		}
	}
	return nil, false
}

// resolveBitfieldLValue resolves a physical storage bit range through typed semantic projection.
func (c *machineConverter) resolveBitfieldLValue(mem machine.MemoryAddress, access BitfieldAccess) (LValue, bool) {
	resolved, ok := c.ctx.symbols.addressFromMemory(mem, nil)
	if !ok {
		return nil, false
	}
	if addr, ok := c.semanticBitfieldExactAddress(resolved); ok {
		if lvalue, ok := c.consumeBitfieldAddress(addr, access); ok {
			return lvalue, true
		}
	}
	resolved = physicalResolvedAddress(resolved)
	resolved.deref = resolved.deref || bitfieldExactPathHasDeref(resolved.exact)
	addr, ok := c.semanticResolvedAddress(resolved)
	if !ok {
		return nil, false
	}
	return c.consumeBitfieldAddress(addr, access)
}

// semanticBitfieldExactAddress preserves machine-proven nested pointer and
// index projections while returning terminal storage to a byte offset.
func (c *machineConverter) semanticBitfieldExactAddress(resolved resolvedAddress) (AddressExpr, bool) {
	path := resolved.exact
	offset := 0
	for path != nil {
		if _, isOffset := path.(*symresolve.SymbolOffset); !isOffset {
			typ, _ := typeinfo.UnwrapPointer(path.Type())
			if _, aggregate := typ.(*typeinfo.Struct); aggregate {
				if _, bareDeref := path.(*symresolve.SymbolDeref); bareDeref {
					return AddressExpr{}, false
				}
				base, ok := c.convertSymbolPath(path, path.Type())
				if !ok {
					return AddressExpr{}, false
				}
				return AddressExpr{Base: normalizeBitfieldAggregateBase(base), Offset: offset}, true
			}
		}
		switch current := path.(type) {
		case *symresolve.SymbolOffset:
			offset += current.Offset
			path = current.Base
		case *symresolve.SymbolField:
			if current.Field == nil {
				return AddressExpr{}, false
			}
			offset += current.Field.Offset
			path = current.Base
		default:
			return AddressExpr{}, false
		}
	}
	return AddressExpr{}, false
}

// normalizeBitfieldAggregateBase renders fields selected through explicit
// dereferences as equivalent source-level pointer field accesses.
func normalizeBitfieldAggregateBase(expr Expr) Expr {
	switch current := expr.(type) {
	case *FieldAccess:
		next := *current
		next.Base = normalizeBitfieldAggregateBase(current.Base)
		if deref, ok := next.Base.(*Deref); ok && deref.ByteOff == 0 {
			next.Base = deref.Pointer
		}
		return &next
	case *ArrayIndex:
		next := *current
		next.Base = normalizeBitfieldAggregateBase(current.Base)
		return &next
	default:
		return expr
	}
}

// bitfieldExactPathHasDeref reports whether an exact path already encoded a
// pointer crossing that the normalized address walker should perform instead.
func bitfieldExactPathHasDeref(path symresolve.SymbolPath) bool {
	for path != nil {
		switch current := path.(type) {
		case *symresolve.SymbolDeref:
			return true
		case *symresolve.SymbolOffset:
			path = current.Base
		case *symresolve.SymbolField:
			path = current.Base
		case *symresolve.SymbolTerm:
			path = current.Base
		default:
			return false
		}
	}
	return false
}

// resolveDeclaredBitfield returns the declared field selected by a physical bit range.
func resolveDeclaredBitfield(ctx *FuncContext, mem machine.MemoryAddress, access BitfieldAccess) (*typeinfo.StructField, bool) {
	lvalue, ok := (&machineConverter{ctx: ctx}).resolveBitfieldLValue(mem, access)
	if !ok {
		return nil, false
	}
	field, ok := lvalue.(*FieldAccess)
	if !ok || field.Field == nil || field.Field.Bitfield == nil {
		return nil, false
	}
	return field.Field, true
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

// consumeBitfieldAddress walks an address to the aggregate containing a declared bitfield.
func (c *machineConverter) consumeBitfieldAddress(addr AddressExpr, access BitfieldAccess) (LValue, bool) {
	current := addr.Base
	offset := addr.Offset
	terms := append([]ScaledTerm(nil), addr.Terms...)
	deref := addr.Deref

	for {
		if len(terms) == 0 {
			if field, ok := c.consumeStructBitfield(current, offset, access); ok {
				return field, true
			}
			if field, ok := c.consumeAmbiguousStructBitfield(current, offset, terms, access); ok {
				return field, true
			}
		}

		if deref && typeinfo.IsPointer(current.ExprType()) {
			next, nextOffset, nextTerms, changed := c.consumeDereferencedPointerStep(current, offset, terms, access.StorageWidth)
			if changed {
				current = next
				offset = nextOffset
				terms = nextTerms
				deref = false
				continue
			}
		}

		next, nextOffset, nextTerms, changed := c.consumeObjectAddressStep(current, offset, terms, access.StorageWidth)
		if !changed {
			return nil, false
		}
		current = next
		offset = nextOffset
		terms = nextTerms
	}
}

// consumeAmbiguousStructBitfield walks the selected aggregate branch, or the
// sole branch that eventually contains the requested bitfield when an
// overlapping union has no active member selection.
func (c *machineConverter) consumeAmbiguousStructBitfield(base Expr, offset int, terms []ScaledTerm, access BitfieldAccess) (LValue, bool) {
	typ := base.ExprType()
	if ptr, ok := typ.(*typeinfo.Pointer); ok {
		typ = ptr.Elem
	}
	strct, ok := typ.(*typeinfo.Struct)
	if !ok {
		return nil, false
	}
	matches := c.unionFieldMatches(base, strct, strct.FieldsContainingOffset(offset))
	if len(matches) == 0 {
		return nil, false
	}
	var out LValue
	for _, match := range matches {
		if match.Field.Bitfield != nil || !isAggregateType(match.Field.Type) {
			continue
		}
		field := bitfieldFieldAccess(base, match.Field)
		candidate, ok := c.consumeBitfieldAddress(
			AddressExpr{Base: field, Offset: match.Off, Terms: append([]ScaledTerm(nil), terms...)},
			access,
		)
		if !ok {
			continue
		}
		if out != nil {
			return nil, false
		}
		out = candidate
	}
	return out, out != nil
}

// consumeStructBitfield matches a physical storage bit range against one struct's fields.
func (c *machineConverter) consumeStructBitfield(base Expr, offset int, access BitfieldAccess) (LValue, bool) {
	typ := base.ExprType()
	if ptr, ok := typ.(*typeinfo.Pointer); ok {
		typ = ptr.Elem
	}
	strct, ok := typ.(*typeinfo.Struct)
	if !ok {
		return nil, false
	}
	matches := c.unionFieldMatches(base, strct, strct.FieldsContainingOffset(offset))
	var out *FieldAccess
	for _, match := range matches {
		bitfield := match.Field.Bitfield
		if bitfield == nil {
			continue
		}
		fieldBitOff := match.Off*8 + access.BitOff
		if access.BitOff+access.BitWidth > access.StorageWidth*8 ||
			match.Off+access.StorageWidth > bitfield.StorageSize ||
			fieldBitOff != bitfield.BitOffset ||
			access.BitWidth != bitfield.BitWidth ||
			!bitfieldSignednessMatches(bitfield.BaseType, access.Signedness) {
			continue
		}
		if out != nil {
			return nil, false
		}
		out = bitfieldFieldAccess(base, match.Field)
	}
	return out, out != nil
}

// bitfieldSignednessMatches reports whether known extraction signedness agrees
// with a declared primitive integer base type.
func bitfieldSignednessMatches(base typeinfo.Type, signedness BitfieldSignedness) bool {
	if signedness == BitfieldSignednessUnknown {
		return true
	}
	primitive, ok := base.(*typeinfo.Primitive)
	if !ok || primitive.TypeKind != typeinfo.KInt {
		return true
	}
	return primitive.Signed == (signedness == BitfieldSigned)
}

// bitfieldFieldAccess converts an explicit aggregate dereference into the
// equivalent pointer-based field access before selecting a field.
func bitfieldFieldAccess(base Expr, field *typeinfo.StructField) *FieldAccess {
	if deref, ok := base.(*Deref); ok && deref.ByteOff == 0 {
		base = deref.Pointer
	}
	return &FieldAccess{Base: base, Field: field}
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

	if strct, ok := ptr.Elem.(*typeinfo.Struct); ok {
		if field, fieldOff, ok := indexedFlexibleArrayFieldAtOffset(strct, offset, terms); ok {
			return &FieldAccess{Base: current, Field: field}, fieldOff, terms, true
		}
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

// indexedFlexibleArrayFieldAtOffset recognizes a trailing array member before
// pointer arithmetic folds its byte offset into an index of the parent struct.
func indexedFlexibleArrayFieldAtOffset(strct *typeinfo.Struct, offset int, terms []ScaledTerm) (*typeinfo.StructField, int, bool) {
	field, fieldOff, ok := zeroLengthArrayFieldAtOffset(strct, offset)
	if !ok {
		return nil, 0, false
	}
	array := field.Type.(*typeinfo.Array)
	if array.Elem == nil || array.Elem.Bytes() <= 0 {
		return nil, 0, false
	}
	for _, term := range terms {
		if term.Scale == array.Elem.Bytes() {
			return field, fieldOff, true
		}
	}
	return nil, 0, false
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
	if field, fieldOff, ok := indexedArrayFieldAtOffset(strct, offset, terms); ok {
		return &FieldAccess{Base: base, Field: field}, fieldOff, true
	}
	if field, fieldOff, ok := indexedArrayFieldAfterOffset(strct, offset, terms); ok {
		return &FieldAccess{Base: base, Field: field}, fieldOff, true
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

// indexedArrayFieldAtOffset selects an overlapping array field when a
// residual term has the array element's byte scale.
func indexedArrayFieldAtOffset(strct *typeinfo.Struct, offset int, terms []ScaledTerm) (*typeinfo.StructField, int, bool) {
	var match *typeinfo.StructField
	matchOff := 0
	for _, current := range strct.FieldsContainingOffset(offset) {
		array, ok := current.Field.Type.(*typeinfo.Array)
		if !ok || array.Elem == nil || array.Elem.Bytes() <= 0 {
			continue
		}
		indexed := false
		for _, term := range terms {
			if term.Scale == array.Elem.Bytes() {
				indexed = true
				break
			}
		}
		if !indexed {
			continue
		}
		if match != nil {
			return nil, 0, false
		}
		match = current.Field
		matchOff = current.Off
	}
	return match, matchOff, match != nil
}

// indexedArrayFieldAfterOffset recognizes a folded negative array index whose
// fixed address lands immediately before an array field.
func indexedArrayFieldAfterOffset(strct *typeinfo.Struct, offset int, terms []ScaledTerm) (*typeinfo.StructField, int, bool) {
	for _, current := range strct.FieldsContainingOffset(offset) {
		array, ok := current.Field.Type.(*typeinfo.Array)
		if !ok || array.Elem == nil {
			continue
		}
		for _, term := range terms {
			if term.Scale == array.Elem.Bytes() {
				return nil, 0, false
			}
		}
	}
	var match *typeinfo.StructField
	matchDistance := 0
	for i := range strct.Fields {
		field := &strct.Fields[i]
		array, ok := field.Type.(*typeinfo.Array)
		if !ok || array.Elem == nil || array.Elem.Bytes() <= 0 {
			continue
		}
		distance := field.Offset - offset
		if distance <= 0 || distance >= array.Bytes() {
			continue
		}
		indexed := false
		for _, term := range terms {
			if term.Scale == array.Elem.Bytes() {
				indexed = true
				break
			}
		}
		if !indexed {
			continue
		}
		if match != nil && distance == matchDistance {
			return nil, 0, false
		}
		if match == nil || distance < matchDistance {
			match = field
			matchDistance = distance
		}
	}
	if match == nil {
		return nil, 0, false
	}
	return match, -matchDistance, true
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
	if len(matches) <= 1 || c.ignoreUnionContext {
		return matches
	}
	var selection symresolve.UnionSelection
	var selected bool
	if unionCtx := c.ctx.unionContext(); unionCtx != nil {
		if path, ok := symbolPathForExpr(base); ok {
			selection, selected = unionCtx.SelectionFor(path, strct)
		} else if index, ok := base.(*ArrayIndex); ok {
			if root, ok := symbolPathForExpr(index.Base); ok {
				selection, selected = unionCtx.AllElementsSelectionFor(root, strct)
			}
		}
	}
	if !selected {
		rule, ok := c.ctx.sdb.UnionRules.UnionVariantForType(strct)
		if !ok || rule.DefaultMember == nil {
			return matches
		}
		for _, match := range matches {
			if match.Field == rule.DefaultMember {
				return []typeinfo.StructFieldMatch{match}
			}
		}
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
		if term.Scale%elem.Bytes() != 0 {
			nextTerms = append(nextTerms, term)
			continue
		}
		component := scaledArrayIndexTerm(term.Expr, term.Scale/elem.Bytes())
		index = joinSemanticAddressTerms(index, component)
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

// scaledArrayIndexTerm converts a byte-scaled address term into a logical
// source array-index component.
func scaledArrayIndexTerm(expr Expr, scale int) Expr {
	negative := scale < 0
	if negative {
		scale = -scale
	}
	if scale != 1 {
		expr = &Binary{
			TypeInfo: typeinfo.I16,
			Op:       OpMul,
			LHS:      expr,
			RHS:      &Const{TypeInfo: typeinfo.I16, U64: uint64(scale)},
		}
	}
	if negative {
		return &Unary{TypeInfo: typeinfo.I16, Op: OpNeg, X: expr}
	}
	return expr
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
