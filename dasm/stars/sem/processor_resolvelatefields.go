package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type resolveLateFieldsProcessor struct {
	ctx *FuncContext
}

// ProcessBlock resolves ordinary declared fields hidden inside over-wide
// semantic Part accesses exposed by earlier scratch/address recovery.
func (p *resolveLateFieldsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	effects, changed := p.rewriter().rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// rewriter returns the semantic tree rewrite for late ordinary field recovery.
func (p *resolveLateFieldsProcessor) rewriter() *semRewriter {
	return &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			assign, ok := effect.(*Assign)
			if !ok {
				return effect, false, false
			}

			next := *assign
			changed := false
			if dst, ok := p.resolveNarrowWrite(assign.Dst, assign.Src); ok {
				next.Dst = dst
				changed = true
			}

			rewritten, childChanged := w.rewriteEffectChildren(&next)
			return rewritten, changed || childChanged, true
		},
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			// Mask/shift forms carry an exact physical bit range. Reuse the
			// late bitfield extractor, but project only ordinary byte-aligned
			// fields here; declared bitfields remain for the bitfield pass.
			if field, ok := p.resolveExtract(expr); ok {
				return field, true, true
			}

			switch value := expr.(type) {
			case *Byte:
				if value.Value == nil {
					if field, ok := p.resolveByteProjection(value); ok {
						return field, true, true
					}
				}

			case *SignExtend:
				if field, ok := p.resolveSignExtendParent(value); ok {
					next := *value
					next.Parent = field
					rewritten, _ := w.rewriteExprChildren(&next)
					return rewritten, true, true
				}
			}

			return expr, false, false
		},
		lvalue: func(w *semRewriter, value LValue) (LValue, bool, bool) {
			value, childChanged := w.rewriteLValueChildren(value)

			// A Part whose semantic type is already narrower than its physical
			// width carries enough information to select that exact field.
			part, ok := value.(*Part)
			if !ok || part.TypeInfo == nil {
				return value, childChanged, true
			}
			width := part.TypeInfo.Bytes()
			if width <= 0 || width >= part.Width {
				return value, childChanged, true
			}
			if field, ok := p.resolvePartRange(part, 0, width); ok {
				return field, true, true
			}
			return value, childChanged, true
		},
	}
}

// resolveExtract resolves a byte-aligned subrange selected from an over-wide
// Part by mask/shift operations, for example:
//
//	part[0x7a:2](shdefBuild.hul) & 0xff
//	(part[0x60:2](sel.fl) >> 8) & 0xff
func (p *resolveLateFieldsProcessor) resolveExtract(expr Expr) (LValue, bool) {
	range_, ok := semanticBitfieldExtract(expr)
	if !ok || range_.Access.BitOff%8 != 0 || range_.Access.BitWidth%8 != 0 {
		return nil, false
	}

	width := range_.Access.BitWidth / 8
	if width <= 0 || width >= range_.Access.StorageWidth {
		return nil, false
	}

	return p.resolveAddressRange(
		range_.Address,
		range_.Access.BitOff/8,
		width,
	)
}

// resolveByteProjection resolves LOBYTE/HIBYTE of an over-wide Part directly
// to the declared byte field covered by that physical access.
func (p *resolveLateFieldsProcessor) resolveByteProjection(value *Byte) (LValue, bool) {
	part, ok := unwrapSemanticLateFieldValue(value.Parent).(*Part)
	if !ok || part.Width <= 1 {
		return nil, false
	}

	byteOff := 0
	switch value.Part {
	case machine.ByteLow:
		byteOff = 0
	case machine.ByteHigh:
		byteOff = 1
	default:
		return nil, false
	}
	if byteOff >= part.Width {
		return nil, false
	}
	return p.resolvePartRange(part, byteOff, 1)
}

// resolveSignExtendParent resolves an explicitly sign-extended narrow physical
// subrange while preserving the SignExtend node itself. The explicit FromBits
// is the evidence that only the low portion of the over-wide Part is consumed.
func (p *resolveLateFieldsProcessor) resolveSignExtendParent(value *SignExtend) (LValue, bool) {
	if value.FromBits <= 0 || value.FromBits%8 != 0 {
		return nil, false
	}
	part, ok := unwrapSemanticLateFieldValue(value.Parent).(*Part)
	if !ok {
		return nil, false
	}
	width := value.FromBits / 8
	if width <= 0 || width >= part.Width {
		return nil, false
	}
	return p.resolvePartRange(part, 0, width)
}

// resolveNarrowWrite uses the assigned value width as evidence that an
// over-wide Part destination names a narrower declared field. This recovers
// forms such as:
//
//	part[0x19:2](vplr) = iMin
//	part[0x60:2](sel.fl) = lobyte(...)
func (p *resolveLateFieldsProcessor) resolveNarrowWrite(dst LValue, src Expr) (LValue, bool) {
	part, ok := dst.(*Part)
	if !ok || src == nil || src.ExprType() == nil {
		return nil, false
	}
	width := src.ExprType().Bytes()
	if width <= 0 || width >= part.Width {
		return nil, false
	}
	return p.resolvePartRange(part, 0, width)
}

// resolvePartRange projects one byte range inside a Part onto an exact ordinary
// declared field.
func (p *resolveLateFieldsProcessor) resolvePartRange(part *Part, byteOff, width int) (LValue, bool) {
	if part == nil || part.Base == nil || part.Width <= 0 || byteOff < 0 || width <= 0 || byteOff+width > part.Width {
		return nil, false
	}
	return p.resolveAddressRange(
		AddressExpr{Base: part.Base, Offset: part.ByteOff},
		byteOff,
		width,
	)
}

// resolveAddressRange projects an exact byte range and accepts only a declared
// non-bitfield field of the requested width. Residual Part/Deref projections
// are deliberately rejected so this pass cannot turn arbitrary physical
// storage into a guessed source-level field.
func (p *resolveLateFieldsProcessor) resolveAddressRange(address AddressExpr, byteOff, width int) (LValue, bool) {
	if width <= 0 || byteOff < 0 {
		return nil, false
	}

	address.Offset += byteOff
	converter := machineConverter{ctx: p.ctx}
	target, ok := converter.consumeAddress(address, width)
	if !ok || target == nil || target.ExprType() == nil || target.ExprType().Bytes() != width {
		return nil, false
	}

	field, ok := target.(*FieldAccess)
	if !ok || field.Field == nil || field.Field.Bitfield != nil {
		return nil, false
	}
	if isAggregateType(field.Field.Type) || typeinfo.IsPointer(field.Field.Type) {
		return nil, false
	}
	return field, true
}

// unwrapSemanticLateFieldValue removes representation-only wrappers that do
// not change which physical bytes are being selected.
func unwrapSemanticLateFieldValue(expr Expr) Expr {
	for {
		switch value := expr.(type) {
		case *Cast:
			expr = value.Value
		case *Word:
			if value.Part != machine.WordLow {
				return expr
			}
			expr = value.Parent
		default:
			return expr
		}
	}
}
