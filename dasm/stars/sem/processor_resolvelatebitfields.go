package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

type resolveLateBitfieldsProcessor struct {
	ctx *FuncContext
}

// ProcessBlock resolves bitfield extracts revealed by semantic scratch-slot substitution.
func (p *resolveLateBitfieldsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	effects, changed := p.rewriter().rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// rewriter returns the semantic tree rewrite for late bitfield resolution.
func (p *resolveLateBitfieldsProcessor) rewriter() *semRewriter {
	return &semRewriter{
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			if field, ok := p.resolve(expr); ok {
				return field, true, true
			}
			return expr, false, false
		},
	}
}

// resolve matches a semantic mask-and-shift expression against a declared bitfield.
func (p *resolveLateBitfieldsProcessor) resolve(expr Expr) (LValue, bool) {
	value := unwrapSemanticBitfieldValue(expr)
	and, ok := value.(*Binary)
	if !ok || and.Op != OpAnd {
		return nil, false
	}
	mask, source, ok := semanticConstOperand(and.LHS, and.RHS)
	if !ok {
		return nil, false
	}
	bitWidth, ok := lowBitMaskWidth(uint(mask.U64))
	if !ok {
		return nil, false
	}
	storage, bitOff, ok := shiftedSemanticStorage(source)
	if !ok {
		return nil, false
	}
	addr := AddressExpr{Base: storage.Pointer, Offset: storage.ByteOff, Deref: true}
	converter := machineConverter{ctx: p.ctx}
	if field, ok := converter.consumeBitfieldAddress(addr, storage.Width, bitOff, bitWidth); ok {
		return field, true
	}
	converter.ignoreUnionContext = true
	return converter.consumeBitfieldAddress(addr, storage.Width, bitOff, bitWidth)
}

// semanticConstOperand separates a constant mask from the other binary operand.
func semanticConstOperand(lhs, rhs Expr) (*Const, Expr, bool) {
	if constant, ok := lhs.(*Const); ok {
		return constant, rhs, true
	}
	if constant, ok := rhs.(*Const); ok {
		return constant, lhs, true
	}
	return nil, nil, false
}

// shiftedSemanticStorage returns a dereference and its accumulated right shift.
func shiftedSemanticStorage(expr Expr) (*Deref, int, bool) {
	value := unwrapSemanticBitfieldValue(expr)
	bitOff := 0
	for {
		shift, ok := value.(*Binary)
		if !ok || shift.Op != OpShr {
			break
		}
		amount, ok := shift.RHS.(*Const)
		if !ok {
			return nil, 0, false
		}
		bitOff += int(amount.U64)
		value = unwrapSemanticBitfieldValue(shift.LHS)
	}
	storage, ok := value.(*Deref)
	return storage, bitOff, ok
}

// unwrapSemanticBitfieldValue removes representation-only wrappers from a semantic bitfield value.
func unwrapSemanticBitfieldValue(expr Expr) Expr {
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
