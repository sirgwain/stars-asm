package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// AddressExpr is a normalized source address with residual byte displacement.
type AddressExpr struct {
	Base   Expr
	Offset int
	Terms  []ScaledTerm
	Deref  bool
}

// ScaledTerm is a dynamic address term measured in bytes.
type ScaledTerm struct {
	Expr  Expr
	Scale int
}

type machineAddressExpr struct {
	base   Expr
	offset int
	terms  []machineScaledTerm
}

type machineScaledTerm struct {
	value machine.Value
	scale int
}

// resolveAddressLValue resolves a machine memory access through typed semantic projection.
func (c *machineConverter) resolveAddressLValue(access machine.MemoryAccess, width int) (LValue, bool) {
	if addr, ok := c.addressExprFromMemory(access); ok {
		return c.consumeAddress(addr, width)
	}
	if addr, ok := c.addressExprFromFarPointerMemory(access); ok {
		return c.consumeAddress(addr, width)
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
	machineAddr, ok := c.machineAddressExpr(value)
	if !ok || machineAddr.base == nil {
		return nil, false
	}
	addr := c.semanticAddressExpr(machineAddr)
	return c.consumeAddressExpr(addr, 0)
}

// addressExprFromMemory decomposes direct local/global storage plus residual address terms.
func (c *machineConverter) addressExprFromMemory(access machine.MemoryAccess) (AddressExpr, bool) {
	if machine.ValueEquals(access.Seg, c.ctx.dsReg) && access.Base != nil {
		if addr, ok := c.machineAddressExpr(access.Base); ok && addr.base != nil {
			addr.offset += access.Disp
			if access.Index != nil {
				scale := access.Scale
				if scale == 0 {
					scale = 1
				}
				if indexAddr, ok := c.machineAddressExpr(access.Index); ok && indexAddr.base == nil {
					addr.offset += indexAddr.offset * scale
					for _, term := range indexAddr.terms {
						term.scale *= scale
						addr.terms = append(addr.terms, term)
					}
				} else {
					addr.terms = append(addr.terms, machineScaledTerm{value: access.Index, scale: scale})
				}
			}
			return c.semanticAddressExpr(addr), true
		}
	}
	addr, ok := c.directStorageAddress(access)
	if !ok {
		return AddressExpr{}, false
	}
	addr.offset += access.Disp
	if access.Index != nil {
		scale := access.Scale
		if scale == 0 {
			scale = 1
		}
		if indexAddr, ok := c.machineAddressExpr(access.Index); ok && indexAddr.base == nil {
			addr.offset += indexAddr.offset * scale
			for _, term := range indexAddr.terms {
				term.scale *= scale
				addr.terms = append(addr.terms, term)
			}
		} else {
			addr.terms = append(addr.terms, machineScaledTerm{value: access.Index, scale: scale})
		}
	}
	return c.semanticAddressExpr(addr), true
}

// addressExprFromFarPointerMemory decomposes memory reached through far pointer words.
func (c *machineConverter) addressExprFromFarPointerMemory(access machine.MemoryAccess) (AddressExpr, bool) {
	rootLoad, indexBytes, ok := splitFarPointerAddressBase(access.Base, access.Seg)
	if !ok {
		return AddressExpr{}, false
	}
	rootAccess := rootLoad.Access
	rootAccess.Width = 4
	root := c.convertMemoryLValue(rootAccess, rootAccess.Width)
	switch root.(type) {
	case *RawMemory, *Memory:
		return AddressExpr{}, false
	}
	addr := machineAddressExpr{base: root, offset: access.Disp}
	if indexBytes != nil {
		if indexAddr, ok := c.machineAddressExpr(indexBytes); ok && indexAddr.base == nil {
			addr.offset += indexAddr.offset
			addr.terms = append(addr.terms, indexAddr.terms...)
		} else {
			addr.terms = append(addr.terms, machineScaledTerm{value: indexBytes, scale: 1})
		}
	}
	out := c.semanticAddressExpr(addr)
	out.Deref = true
	return out, true
}

// resolveSemanticMemory resolves a lowered Memory node through typed semantic projection.
func (ctx *FuncContext) resolveSemanticMemory(mem *Memory) (LValue, bool) {
	if addr, ok := ctx.semanticAddressExprFromMemory(mem); ok {
		return (&machineConverter{ctx: ctx}).consumeAddress(addr, mem.Width)
	}
	return nil, false
}

// resolveSemanticFarPointer resolves a lowered whole far pointer to a typed address expression.
func (ctx *FuncContext) resolveSemanticFarPointer(ptr *FarPointer) (Expr, bool) {
	if ptr.Part != machine.FarPointerWhole {
		return nil, false
	}
	if expr, ok := ctx.resolveFarPointerParallelMerge(ptr); ok {
		return expr, true
	}
	if expr, ok := ctx.resolveFarPointerMergeOffset(ptr); ok {
		return expr, true
	}
	if expr, ok := ctx.resolveDataSegmentNearPointer(ptr); ok {
		return expr, true
	}
	if expr, ok := ctx.resolveDirectSemanticFarPointer(ptr); ok {
		return expr, true
	}
	root, residual, ok := splitSemanticFarPointerAddressBase(ptr.Offset, ptr.Segment)
	if !ok {
		return nil, false
	}
	if residual == nil && typeinfo.IsFarPointer(root.ExprType()) {
		return root, true
	}
	addr := AddressExpr{Base: root, Deref: true}
	if residual != nil {
		residualAddr, ok := semanticAddressExpr(residual)
		if !ok || residualAddr.Base != nil {
			return nil, false
		}
		addr.Offset += residualAddr.Offset
		addr.Terms = append(addr.Terms, residualAddr.Terms...)
	}
	target, ok := (&machineConverter{ctx: ctx}).consumeAddress(addr, 0)
	if !ok {
		if residual == nil || !typeinfo.IsFarPointer(root.ExprType()) {
			return nil, false
		}
		return &PointerOffset{Pointer: root, Offset: residual, TypeInfo: ptr.TypeInfo}, true
	}
	if typeinfo.IsPointer(target.ExprType()) {
		return target, true
	}
	return &AddressOf{
		Target:   target,
		TypeInfo: &typeinfo.Pointer{Elem: target.ExprType(), Class: typeinfo.PtrFar},
	}, true
}

// resolveFarPointerParallelMerge resolves paired segment and offset merges arm by arm.
func (ctx *FuncContext) resolveFarPointerParallelMerge(ptr *FarPointer) (Expr, bool) {
	segMerge, segOK := ptr.Segment.(*Merge)
	offMerge, offOK := ptr.Offset.(*Merge)
	if !segOK || !offOK || segMerge.Join != offMerge.Join || len(segMerge.Arms) != len(offMerge.Arms) {
		return nil, false
	}
	arms := make([]MergeArm, 0, len(offMerge.Arms))
	for i, offArm := range offMerge.Arms {
		segArm := segMerge.Arms[i]
		if segArm.Block != offArm.Block {
			return nil, false
		}
		next := *ptr
		next.Segment = segArm.Value
		next.Offset = offArm.Value
		resolved, ok := ctx.resolveSemanticFarPointer(&next)
		if !ok {
			return nil, false
		}
		arms = append(arms, MergeArm{Block: offArm.Block, Value: resolved})
	}
	return &Merge{TypeInfo: mergeType(arms), Join: offMerge.Join, Arms: arms}, true
}

// resolveFarPointerMergeOffset resolves each arm of a merged far-pointer offset.
func (ctx *FuncContext) resolveFarPointerMergeOffset(ptr *FarPointer) (Expr, bool) {
	merge, ok := ptr.Offset.(*Merge)
	if !ok {
		return nil, false
	}
	arms := make([]MergeArm, 0, len(merge.Arms))
	for _, arm := range merge.Arms {
		next := *ptr
		next.Offset = arm.Value
		resolved, ok := ctx.resolveSemanticFarPointer(&next)
		if !ok {
			return nil, false
		}
		arms = append(arms, MergeArm{Block: arm.Block, Value: resolved})
	}
	return &Merge{TypeInfo: mergeType(arms), Join: merge.Join, Arms: arms}, true
}

// resolveDataSegmentNearPointer resolves DS:near-pointer expressions to the near pointer.
func (ctx *FuncContext) resolveDataSegmentNearPointer(ptr *FarPointer) (Expr, bool) {
	if !exprMatchesMachineValue(ptr.Segment, ctx.dsReg) {
		return nil, false
	}
	root, residual, ok := splitDataSegmentNearPointerOffset(ptr.Offset)
	if !ok {
		return nil, false
	}
	if residual == nil {
		return root, true
	}
	return &PointerOffset{Pointer: root, Offset: residual, TypeInfo: root.ExprType()}, true
}

// splitDataSegmentNearPointerOffset finds a near pointer plus optional byte offset.
func splitDataSegmentNearPointerOffset(expr Expr) (Expr, Expr, bool) {
	if ptr, ok := expr.ExprType().(*typeinfo.Pointer); ok && ptr.Class == typeinfo.PtrNear {
		return expr, nil, true
	}
	if part, ok := expr.(*Part); ok && part.Width == 0 {
		if field, ok := resolveZeroWidthAggregatePart(part); ok {
			return field, nil, true
		}
		return part, nil, true
	}
	if array, ok := expr.ExprType().(*typeinfo.Array); ok && array.IsCStringArray() {
		return expr, nil, true
	}
	binary, ok := expr.(*Binary)
	if !ok || (binary.Op != OpAdd && binary.Op != OpSub) {
		return nil, nil, false
	}
	if root, residual, ok := splitDataSegmentNearPointerOffset(binary.LHS); ok {
		if binary.Op == OpSub {
			return root, subtractSemanticAddressTerms(residual, binary.RHS), true
		}
		return root, joinSemanticAddressTerms(residual, binary.RHS), true
	}
	if binary.Op == OpAdd {
		if root, residual, ok := splitDataSegmentNearPointerOffset(binary.RHS); ok {
			return root, joinSemanticAddressTerms(binary.LHS, residual), true
		}
	}
	return nil, nil, false
}

// resolveZeroWidthAggregatePart resolves a byte address into an aggregate field.
func resolveZeroWidthAggregatePart(part *Part) (Expr, bool) {
	strct, ok := part.Base.ExprType().(*typeinfo.Struct)
	if !ok {
		return nil, false
	}
	matches := strct.FieldsContainingOffset(part.ByteOff)
	if len(matches) != 1 || matches[0].Off != 0 || !isAggregateType(matches[0].Field.Type) {
		return nil, false
	}
	return &FieldAccess{Base: part.Base, Field: matches[0].Field}, true
}

// resolveDirectSemanticFarPointer resolves segment:offset expressions into static globals.
func (ctx *FuncContext) resolveDirectSemanticFarPointer(ptr *FarPointer) (Expr, bool) {
	segNum, ok := ptr.SegNum()
	if !ok {
		return nil, false
	}
	addr, ok := semanticAbsoluteAddressExpr(ptr.Offset)
	if !ok || addr.Base != nil || addr.Offset < 0 {
		return nil, false
	}
	if addr.Offset == 0 && segNum == 0 {
		return &Const{U64: 0, TypeInfo: ptr.TypeInfo}, true
	}
	if typeinfo.IsFunctionPointer(ptr.TypeInfo) {
		if f := ctx.sdb.GetFunctionByAddr(typeinfo.Addr{Seg: segNum, Off: uint32(addr.Offset)}); f != nil {
			return &FunctionRef{Function: f, TypeInfo: ptr.TypeInfo}, true
		}
	}
	global, globalOK := ctx.res.ResolveGlobal(segNum, uint32(addr.Offset), 0)

	if !globalOK {
		if ptrType, ok := ptr.TypeInfo.(*typeinfo.Pointer); ok && ptrType.IsCStringPointer() {
			if text, ok := ctx.res.ResolveLiteral(segNum, uint32(addr.Offset)); ok {
				return &StringLiteral{TypeInfo: ptr.TypeInfo, Text: text}, true
			}
		}

		return collapseConstSegmentFarPointerOffset(ptr)
	}
	base := &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: global.Global}}
	target, ok := (&machineConverter{ctx: ctx}).consumeAddressExpr(AddressExpr{
		Base:   base,
		Offset: global.FieldOff,
		Terms:  addr.Terms,
	}, 0)
	if !ok {
		if target, ok := resolveDirectGlobalByteArrayOffset(base, global.FieldOff, addr.Terms, ptr.TypeInfo); ok {
			return target, true
		}
		return nil, false
	}
	if typeinfo.IsCallCompatible(ptr.TypeInfo, target.ExprType()) {
		return target, true
	}
	lvalue, ok := target.(LValue)
	if !ok {
		return nil, false
	}
	return &AddressOf{
		Target:   lvalue,
		TypeInfo: ptr.TypeInfo,
	}, true
}

// resolveDirectGlobalByteArrayOffset resolves far pointers into known byte-array globals.
func resolveDirectGlobalByteArrayOffset(base Expr, fieldOff int, terms []ScaledTerm, ptrType typeinfo.Type) (Expr, bool) {
	array, ok := base.ExprType().(*typeinfo.Array)
	if !ok || array.Elem == nil || array.Elem.Bytes() != 1 || fieldOff != 0 || len(terms) != 1 {
		return nil, false
	}
	term, ok := semanticScaledTermExpr(terms[0])
	if !ok {
		return nil, false
	}
	target := &ArrayIndex{
		Base:     base,
		Index:    term,
		TypeInfo: array.Elem,
	}
	if typeinfo.IsCallCompatible(ptrType, target.ExprType()) {
		return target, true
	}
	return &AddressOf{Target: target, TypeInfo: ptrType}, true
}

// collapseConstSegmentFarPointerOffset keeps a static segment base with dynamic offset terms.
func collapseConstSegmentFarPointerOffset(ptr *FarPointer) (Expr, bool) {
	if _, ok := ptr.SegNum(); !ok {
		return nil, false
	}
	addr, ok := semanticAbsoluteAddressExpr(ptr.Offset)
	if !ok || addr.Base != nil || addr.Offset < 0 {
		return nil, false
	}
	residual, residualOK := semanticAddressResidualExpr(AddressExpr{Terms: addr.Terms})
	if !residualOK || residual == nil {
		return nil, false
	}
	return &PointerOffset{
		Pointer: &FarPointer{
			Part:     machine.FarPointerWhole,
			Segment:  ptr.Segment,
			Offset:   &Const{TypeInfo: typeinfo.U16, U64: uint64(uint16(addr.Offset))},
			TypeInfo: ptr.TypeInfo,
		},
		Offset:   residual,
		TypeInfo: ptr.TypeInfo,
	}, true
}

// resolveFarPointerWithRecoveredOffset rebuilds a far pointer whose offset was resolved too early.
func (ctx *FuncContext) resolveFarPointerWithRecoveredOffset(ptr *FarPointer) (Expr, bool) {
	if ptr.Part != machine.FarPointerWhole {
		return nil, false
	}
	if _, ok := ptr.SegNum(); !ok {
		return nil, false
	}
	offset, ok := semanticResolvedAddressOffsetExpr(ptr.Offset)
	if !ok {
		return nil, false
	}
	next := *ptr
	next.Offset = offset
	if resolved, ok := ctx.resolveSemanticFarPointer(&next); ok {
		return resolved, true
	}
	return collapseConstSegmentFarPointerOffset(&next)
}

// semanticResolvedAddressOffsetExpr rebuilds the offset word from a resolved global address.
func semanticResolvedAddressOffsetExpr(expr Expr) (Expr, bool) {
	switch e := expr.(type) {
	case *SymbolRef:
		root, ok := e.Path.(*symresolve.SymbolRoot)
		if !ok {
			return nil, false
		}
		global, ok := root.Symbol.(*typeinfo.GlobalVar)
		if !ok {
			return nil, false
		}
		return &Const{TypeInfo: typeinfo.U16, U64: uint64(uint16(global.Addr.Off))}, true
	case *ArrayIndex:
		base, ok := semanticResolvedAddressOffsetExpr(e.Base)
		if !ok {
			return nil, false
		}
		elemSize := 1
		if e.TypeInfo != nil && e.TypeInfo.Bytes() > 0 {
			elemSize = e.TypeInfo.Bytes()
		}
		index := e.Index
		if elemSize != 1 {
			index = &Binary{
				TypeInfo: typeinfo.U16,
				Op:       OpMul,
				LHS:      &Const{TypeInfo: typeinfo.U16, U64: uint64(elemSize)},
				RHS:      e.Index,
			}
		}
		return joinSemanticAddressTerms(base, index), true
	case *Part:
		base, ok := semanticResolvedAddressOffsetExpr(e.Base)
		if !ok {
			return nil, false
		}
		if e.ByteOff == 0 {
			return base, true
		}
		return joinSemanticAddressTerms(base, &Const{TypeInfo: typeinfo.U16, U64: uint64(e.ByteOff)}), true
	default:
		return nil, false
	}
}

// semanticAddressExprFromMemory decomposes lowered memory address expressions.
func (ctx *FuncContext) semanticAddressExprFromMemory(mem *Memory) (AddressExpr, bool) {
	if root, residual, ok := splitSemanticFarPointerAddressBase(mem.Base, mem.Seg); ok {
		addr := AddressExpr{Base: root, Offset: mem.Disp, Deref: true}
		if residual != nil {
			residualAddr, ok := semanticAddressExpr(residual)
			if !ok || residualAddr.Base != nil {
				return AddressExpr{}, false
			}
			addr.Offset += residualAddr.Offset
			addr.Terms = append(addr.Terms, residualAddr.Terms...)
		}
		return addr, true
	}

	if !semanticMemoryUsesDataSegment(mem.Seg) {
		return AddressExpr{}, false
	}
	offset := mem.Disp
	baseExpr := mem.Base
	if c, ok := baseExpr.(*Const); ok {
		offset += int(c.U64)
		baseExpr = nil
	}
	global, ok := ctx.res.ResolveGlobal(uint16(ctx.sdb.DGroupFrame), uint32(offset), mem.Width)
	if !ok {
		return AddressExpr{}, false
	}
	addr := AddressExpr{
		Base:   &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: global.Global}},
		Offset: global.FieldOff,
	}
	for _, expr := range []Expr{baseExpr, mem.Index} {
		if expr == nil {
			continue
		}
		exprAddr, ok := semanticAddressExpr(expr)
		if !ok || exprAddr.Base != nil {
			return AddressExpr{}, false
		}
		addr.Offset += exprAddr.Offset
		addr.Terms = append(addr.Terms, exprAddr.Terms...)
	}
	if mem.Scale != 0 && mem.Scale != 1 && len(addr.Terms) > 0 {
		addr.Terms[len(addr.Terms)-1].Scale *= mem.Scale
	}
	return addr, true
}

// semanticAddressExpr decomposes semantic arithmetic into residual address terms.
func semanticAddressExpr(expr Expr) (AddressExpr, bool) {
	expr = unwrapSemanticAddressWord(expr)
	switch e := expr.(type) {
	case nil:
		return AddressExpr{}, true
	case *Const:
		return AddressExpr{Offset: signedWordOffset(uint(e.U64))}, true
	case *Binary:
		return semanticBinaryAddressExpr(e)
	default:
		return AddressExpr{Terms: []ScaledTerm{{Expr: expr, Scale: 1}}}, true
	}
}

// semanticMemoryUsesDataSegment reports whether a lowered memory node uses DS.
func semanticMemoryUsesDataSegment(seg Expr) bool {
	return seg == nil || regExprEquals(seg, asm.RegDS)
}

// semanticAbsoluteAddressExpr decomposes segment offsets with unsigned constants.
func semanticAbsoluteAddressExpr(expr Expr) (AddressExpr, bool) {
	expr = unwrapSemanticAddressWord(expr)
	switch e := expr.(type) {
	case nil:
		return AddressExpr{}, true
	case *Const:
		return AddressExpr{Offset: int(uint16(e.U64))}, true
	case *Binary:
		return semanticAbsoluteBinaryAddressExpr(e)
	default:
		return AddressExpr{Terms: []ScaledTerm{{Expr: expr, Scale: 1}}}, true
	}
}

// semanticAbsoluteBinaryAddressExpr decomposes absolute segment offset arithmetic.
func semanticAbsoluteBinaryAddressExpr(expr *Binary) (AddressExpr, bool) {
	switch expr.Op {
	case OpAdd, OpSub:
		lhs, lhsOK := semanticAbsoluteAddressExpr(expr.LHS)
		rhs, rhsOK := semanticAbsoluteAddressExpr(expr.RHS)
		if !lhsOK || !rhsOK || (lhs.Base != nil && rhs.Base != nil) {
			return AddressExpr{}, false
		}
		if expr.Op == OpSub {
			rhs.Offset = -rhs.Offset
			for i := range rhs.Terms {
				rhs.Terms[i].Scale = -rhs.Terms[i].Scale
			}
		}
		return mergeSemanticAddressExpr(lhs, rhs), true
	case OpMul:
		if term, ok := semanticScaledTermFromMul(expr.LHS, expr.RHS); ok {
			return AddressExpr{Terms: []ScaledTerm{term}}, true
		}
	case OpShl:
		if c, ok := expr.RHS.(*Const); ok {
			return AddressExpr{Terms: []ScaledTerm{{Expr: expr.LHS, Scale: 1 << c.U64}}}, true
		}
	}
	return AddressExpr{Terms: []ScaledTerm{{Expr: expr, Scale: 1}}}, true
}

// semanticBinaryAddressExpr decomposes semantic binary arithmetic into address terms.
func semanticBinaryAddressExpr(expr *Binary) (AddressExpr, bool) {
	switch expr.Op {
	case OpAdd, OpSub:
		lhs, lhsOK := semanticAddressExpr(expr.LHS)
		rhs, rhsOK := semanticAddressExpr(expr.RHS)
		if !lhsOK || !rhsOK || (lhs.Base != nil && rhs.Base != nil) {
			return AddressExpr{}, false
		}
		if expr.Op == OpSub {
			rhs.Offset = -rhs.Offset
			for i := range rhs.Terms {
				rhs.Terms[i].Scale = -rhs.Terms[i].Scale
			}
		}
		return mergeSemanticAddressExpr(lhs, rhs), true
	case OpMul:
		if term, ok := semanticScaledTermFromMul(expr.LHS, expr.RHS); ok {
			return AddressExpr{Terms: []ScaledTerm{term}}, true
		}
	case OpShl:
		if c, ok := expr.RHS.(*Const); ok {
			return AddressExpr{Terms: []ScaledTerm{{Expr: expr.LHS, Scale: 1 << c.U64}}}, true
		}
	}
	return AddressExpr{Terms: []ScaledTerm{{Expr: expr, Scale: 1}}}, true
}

// mergeSemanticAddressExpr combines two semantic address decompositions.
func mergeSemanticAddressExpr(a, b AddressExpr) AddressExpr {
	out := AddressExpr{Base: a.Base, Offset: a.Offset + b.Offset}
	if out.Base == nil {
		out.Base = b.Base
	}
	out.Terms = append(out.Terms, a.Terms...)
	out.Terms = append(out.Terms, b.Terms...)
	return out
}

// semanticScaledTermFromMul extracts x*N or N*x from semantic multiplication.
func semanticScaledTermFromMul(a, b Expr) (ScaledTerm, bool) {
	if c, ok := a.(*Const); ok {
		return ScaledTerm{Expr: normalizeSemanticIndexExpr(b), Scale: int(c.U64)}, true
	}
	if c, ok := b.(*Const); ok {
		return ScaledTerm{Expr: normalizeSemanticIndexExpr(a), Scale: int(c.U64)}, true
	}
	return ScaledTerm{}, false
}

// normalizeSemanticIndexExpr folds residual word constants inside an index expression.
func normalizeSemanticIndexExpr(expr Expr) Expr {
	addr, ok := semanticAddressExpr(expr)
	if !ok || addr.Base != nil || len(addr.Terms) != 1 || addr.Terms[0].Scale != 1 || addr.Offset == 0 {
		return expr
	}
	if addr.Offset < 0 {
		return &Binary{TypeInfo: typeinfo.U16, Op: OpSub, LHS: addr.Terms[0].Expr, RHS: &Const{TypeInfo: typeinfo.U16, U64: uint64(-addr.Offset)}}
	}
	return &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: addr.Terms[0].Expr, RHS: &Const{TypeInfo: typeinfo.U16, U64: uint64(addr.Offset)}}
}

// semanticAddressResidualExpr rebuilds an address expression without a base.
func semanticAddressResidualExpr(addr AddressExpr) (Expr, bool) {
	if addr.Base != nil {
		return nil, false
	}
	var out Expr
	if addr.Offset != 0 {
		if addr.Offset < 0 {
			return nil, false
		}
		out = &Const{TypeInfo: typeinfo.U16, U64: uint64(addr.Offset)}
	}
	for _, term := range addr.Terms {
		termExpr, ok := semanticScaledTermExpr(term)
		if !ok {
			return nil, false
		}
		out = joinSemanticAddressTerms(out, termExpr)
	}
	return out, true
}

// semanticScaledTermExpr rebuilds one scaled residual address term.
func semanticScaledTermExpr(term ScaledTerm) (Expr, bool) {
	if term.Scale <= 0 {
		return nil, false
	}
	if term.Scale == 1 {
		return term.Expr, true
	}
	return &Binary{
		TypeInfo: typeinfo.U16,
		Op:       OpMul,
		LHS:      &Const{TypeInfo: typeinfo.U16, U64: uint64(term.Scale)},
		RHS:      term.Expr,
	}, true
}

// unwrapSemanticAddressWord removes low-word projections around address arithmetic.
func unwrapSemanticAddressWord(expr Expr) Expr {
	if word, ok := expr.(*Word); ok && word.Part == machine.WordLow {
		return word.Parent
	}
	if part, ok := expr.(*Part); ok && part.ByteOff == 0 && part.Width == 2 {
		return part.Base
	}
	return expr
}

// splitSemanticFarPointerAddressBase finds a semantic far-pointer root and residual offset bytes.
func splitSemanticFarPointerAddressBase(value Expr, seg Expr) (Expr, Expr, bool) {
	if root, ok := semanticFarPointerOffsetRoot(value, seg); ok {
		return root, nil, true
	}
	binary, ok := value.(*Binary)
	if !ok || (binary.Op != OpAdd && binary.Op != OpSub) {
		return nil, nil, false
	}
	if root, rest, ok := splitSemanticFarPointerAddressBase(binary.LHS, seg); ok {
		if binary.Op == OpSub {
			return root, subtractSemanticAddressTerms(rest, binary.RHS), true
		}
		return root, joinSemanticAddressTerms(rest, binary.RHS), true
	}
	if binary.Op == OpAdd {
		if root, rest, ok := splitSemanticFarPointerAddressBase(binary.RHS, seg); ok {
			return root, joinSemanticAddressTerms(binary.LHS, rest), true
		}
	}
	return nil, nil, false
}

// subtractSemanticAddressTerms rebuilds semantic residual byte subtraction.
func subtractSemanticAddressTerms(a, b Expr) Expr {
	if b == nil {
		return a
	}
	if a == nil {
		return &Binary{TypeInfo: typeinfo.U16, Op: OpSub, LHS: &Const{TypeInfo: typeinfo.U16, U64: 0}, RHS: b}
	}
	return &Binary{TypeInfo: typeinfo.U16, Op: OpSub, LHS: a, RHS: b}
}

// splitFarPointerAddressBase finds the loaded far-pointer root and residual offset bytes.
func splitFarPointerAddressBase(value machine.Value, seg machine.Value) (*machine.Load, machine.Value, bool) {
	if load, ok := farPointerOffsetRoot(value, seg); ok {
		return load, nil, true
	}
	binary, ok := value.(*machine.Binary)
	if !ok || (binary.Op != machine.ValueOpAdd && binary.Op != machine.ValueOpSub) {
		return nil, nil, false
	}
	if load, rest, ok := splitFarPointerAddressBase(binary.LHS, seg); ok {
		if binary.Op == machine.ValueOpSub {
			return load, subtractIndexTerms(rest, binary.RHS), true
		}
		return load, joinIndexTerms(rest, binary.RHS), true
	}
	if binary.Op == machine.ValueOpAdd {
		if load, rest, ok := splitFarPointerAddressBase(binary.RHS, seg); ok {
			return load, joinIndexTerms(binary.LHS, rest), true
		}
	}
	return nil, nil, false
}

// subtractIndexTerms rebuilds residual machine address subtraction.
func subtractIndexTerms(a, b machine.Value) machine.Value {
	if b == nil {
		return a
	}
	if a == nil {
		return machine.BinaryVal(machine.ValueOpSub, machine.ConstVal(0), b)
	}
	return machine.BinaryVal(machine.ValueOpSub, a, b)
}

// semanticFarPointerOffsetRoot matches low/high word projections of one semantic far pointer.
func semanticFarPointerOffsetRoot(value Expr, seg Expr) (Expr, bool) {
	if segPart, ok := seg.(*Part); ok && segPart.ByteOff == 2 && segPart.Width == 2 {
		if offPart, ok := value.(*Part); ok && offPart.ByteOff == 0 && offPart.Width == 2 && sameLValue(segPart.Base, offPart.Base) {
			return semanticFarPointerPartRoot(offPart.Base), true
		}
	}
	if segPart, ok := seg.(*Part); ok && segPart.Width == 2 {
		if offPart, ok := value.(*Part); ok && offPart.Width == 2 && segPart.ByteOff == offPart.ByteOff+2 && sameLValue(segPart.Base, offPart.Base) {
			return semanticFarPointerPartRootAtOffset(offPart.Base, offPart.ByteOff), true
		}
	}
	if segWord, ok := seg.(*Word); ok && segWord.Part == machine.WordHigh {
		if offWord, ok := value.(*Word); ok && offWord.Part == machine.WordLow && sameExpr(segWord.Parent, offWord.Parent) {
			return semanticFarPointerPartRoot(offWord.Parent), true
		}
	}
	if segParent, ok := farPointerPartParent(seg, machine.FarPointerSegment); ok {
		if offParent, ok := farPointerPartParent(value, machine.FarPointerOffset); ok && sameExpr(segParent, offParent) {
			return semanticFarPointerPartRoot(offParent), true
		}
	}
	return nil, false
}

// semanticFarPointerPartRoot returns the pointer represented by matching far pointer words.
func semanticFarPointerPartRoot(root Expr) Expr {
	if typeinfo.IsFarPointer(root.ExprType()) {
		return root
	}
	array, ok := root.ExprType().(*typeinfo.Array)
	if !ok || !typeinfo.IsFarPointer(array.Elem) {
		return root
	}
	return &ArrayIndex{
		Base:     root,
		Index:    &Const{TypeInfo: typeinfo.U16, U64: 0},
		TypeInfo: array.Elem,
	}
}

// semanticFarPointerPartRootAtOffset returns the pointer at a byte offset in a far pointer root.
func semanticFarPointerPartRootAtOffset(root Expr, byteOff int) Expr {
	if byteOff == 0 {
		return semanticFarPointerPartRoot(root)
	}
	array, ok := root.ExprType().(*typeinfo.Array)
	if !ok || !typeinfo.IsFarPointer(array.Elem) || array.Elem.Bytes() <= 0 {
		return root
	}
	if byteOff%array.Elem.Bytes() != 0 {
		return root
	}
	return &ArrayIndex{
		Base:     root,
		Index:    &Const{TypeInfo: typeinfo.U16, U64: uint64(byteOff / array.Elem.Bytes())},
		TypeInfo: array.Elem,
	}
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

// farPointerOffsetRoot matches the offset and segment words of one far pointer load.
func farPointerOffsetRoot(value machine.Value, seg machine.Value) (*machine.Load, bool) {
	segPtr, segPtrOK := seg.(*machine.FarPointer)
	offPtr, offPtrOK := value.(*machine.FarPointer)
	if segPtrOK && offPtrOK &&
		segPtr.Part == machine.FarPointerSegment &&
		offPtr.Part == machine.FarPointerOffset &&
		valueShapeEquals(segPtr.Parent, offPtr.Parent) {
		load, ok := offPtr.Parent.(*machine.Load)
		return load, ok
	}
	if rootSeg, ok := seg.(*machine.Load); ok {
		if load, ok := value.(*machine.Load); ok && farPointerWordStorage(load.Access, rootSeg.Access) {
			return load, true
		}
	}
	segWord, segOK := seg.(*machine.WordValue)
	offWord, offOK := value.(*machine.WordValue)
	if !segOK || !offOK || segWord.Part != machine.WordHigh || offWord.Part != machine.WordLow {
		return nil, false
	}
	if !valueShapeEquals(segWord.Parent, offWord.Parent) {
		return nil, false
	}
	load, ok := offWord.Parent.(*machine.Load)
	return load, ok
}

// directStorageAddress resolves the static base for a direct memory access.
func (c *machineConverter) directStorageAddress(access machine.MemoryAccess) (machineAddressExpr, bool) {
	if _, ok := access.Base.(*machine.FrameBase); ok {
		local, ok := c.ctx.res.ResolveLocal(c.ctx.fs, access.Origin.InstOff, access.Disp)
		if !ok {
			return machineAddressExpr{}, false
		}
		return machineAddressExpr{
			base:   &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: &local.Local}},
			offset: local.FieldOff - access.Disp,
		}, true
	}

	if machine.ValueEquals(access.Seg, c.ctx.dsReg) {
		withoutIndex := access
		withoutIndex.Index = nil
		withoutIndex.Scale = 0
		if baseConst, ok := withoutIndex.Base.(*machine.Const); ok {
			withoutIndex.Disp += int(baseConst.Val)
			withoutIndex.Base = nil
		}
		global, ok := c.ctx.resolveGlobal(withoutIndex)
		if ok {
			return machineAddressExpr{
				base:   &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: global.Global}},
				offset: global.FieldOff - withoutIndex.Disp,
			}, true
		}
	}

	return machineAddressExpr{}, false
}

// semanticAddressExpr lowers machine residual terms to semantic address terms.
func (c *machineConverter) semanticAddressExpr(addr machineAddressExpr) AddressExpr {
	terms := make([]ScaledTerm, 0, len(addr.terms))
	for _, term := range addr.terms {
		if term.value == nil {
			continue
		}
		terms = append(terms, ScaledTerm{Expr: c.convertIndexExpr(term.value), Scale: term.scale})
	}
	return AddressExpr{Base: addr.base, Offset: addr.offset, Terms: terms}
}

// convertIndexExpr lowers a machine value used as an array index with residual constants normalized.
func (c *machineConverter) convertIndexExpr(value machine.Value) Expr {
	addr, ok := c.machineAddressExpr(value)
	if !ok || addr.base != nil || len(addr.terms) != 1 || addr.terms[0].scale != 1 || addr.offset == 0 {
		return c.convertValue(value)
	}
	base := c.convertValue(addr.terms[0].value)
	if addr.offset < 0 {
		return &Binary{TypeInfo: typeinfo.U16, Op: OpSub, LHS: base, RHS: &Const{TypeInfo: typeinfo.U16, U64: uint64(-addr.offset)}}
	}
	return &Binary{TypeInfo: typeinfo.U16, Op: OpAdd, LHS: base, RHS: &Const{TypeInfo: typeinfo.U16, U64: uint64(addr.offset)}}
}

// machineAddressExpr decomposes machine arithmetic into a static base, byte offset, and scaled terms.
func (c *machineConverter) machineAddressExpr(value machine.Value) (machineAddressExpr, bool) {
	value = unwrapAddressWord(value)
	switch v := value.(type) {
	case nil:
		return machineAddressExpr{}, false
	case *machine.Const:
		if global, ok := c.ctx.res.ResolveGlobal(uint16(c.ctx.sdb.DGroupFrame), uint32(v.Val), 0); ok {
			return machineAddressExpr{base: &SymbolRef{Path: &symresolve.SymbolRoot{Symbol: global.Global}}, offset: global.FieldOff}, true
		}
		return machineAddressExpr{offset: signedWordOffset(v.Val)}, true
	case *machine.Address:
		path, ok := c.ctx.resolveMemoryPath(v.Access)
		if !ok {
			return machineAddressExpr{}, false
		}
		return machineAddressExpr{base: &SymbolRef{Path: path}}, true
	case *machine.Binary:
		return c.machineBinaryAddressExpr(v)
	default:
		return machineAddressExpr{terms: []machineScaledTerm{{value: value, scale: 1}}}, true
	}
}

// machineOffsetExpr decomposes residual byte arithmetic without resolving constants as symbols.
func (c *machineConverter) machineOffsetExpr(value machine.Value) (machineAddressExpr, bool) {
	value = unwrapAddressWord(value)
	switch v := value.(type) {
	case nil:
		return machineAddressExpr{}, false
	case *machine.Const:
		return machineAddressExpr{offset: signedWordOffset(v.Val)}, true
	case *machine.Binary:
		switch v.Op {
		case machine.ValueOpAdd, machine.ValueOpSub:
			lhs, lhsOK := c.machineOffsetExpr(v.LHS)
			rhs, rhsOK := c.machineOffsetExpr(v.RHS)
			if !lhsOK || !rhsOK {
				return machineAddressExpr{}, false
			}
			if v.Op == machine.ValueOpSub {
				rhs.offset = -rhs.offset
				for i := range rhs.terms {
					rhs.terms[i].scale = -rhs.terms[i].scale
				}
			}
			return mergeMachineAddressExpr(lhs, rhs), true
		case machine.ValueOpMul:
			if term, ok := machineScaledTermFromMul(v.LHS, v.RHS); ok {
				return machineAddressExpr{terms: []machineScaledTerm{term}}, true
			}
		case machine.ValueOpShl:
			if c, ok := v.RHS.(*machine.Const); ok {
				return machineAddressExpr{terms: []machineScaledTerm{{value: v.LHS, scale: 1 << c.Val}}}, true
			}
		}
	}
	return machineAddressExpr{terms: []machineScaledTerm{{value: value, scale: 1}}}, true
}

// machineBinaryAddressExpr decomposes binary machine arithmetic into address terms.
func (c *machineConverter) machineBinaryAddressExpr(v *machine.Binary) (machineAddressExpr, bool) {
	switch v.Op {
	case machine.ValueOpAdd, machine.ValueOpSub:
		lhs, lhsOK := c.machineAddressExpr(v.LHS)
		rhs, rhsOK := c.machineAddressExpr(v.RHS)

		lhs, rhs = preferConstDisplacementTerms(v.Op, v.LHS, lhs, v.RHS, rhs)
		if !lhsOK || !rhsOK || (lhs.base != nil && rhs.base != nil) {
			return machineAddressExpr{}, false
		}
		if v.Op == machine.ValueOpSub {
			rhs.offset = -rhs.offset
			for i := range rhs.terms {
				rhs.terms[i].scale = -rhs.terms[i].scale
			}
		}
		return mergeMachineAddressExpr(lhs, rhs), true
	case machine.ValueOpMul:
		if term, ok := machineScaledTermFromMul(v.LHS, v.RHS); ok {
			return machineAddressExpr{terms: []machineScaledTerm{term}}, true
		}
	case machine.ValueOpShl:
		if c, ok := v.RHS.(*machine.Const); ok {
			return machineAddressExpr{terms: []machineScaledTerm{{value: v.LHS, scale: 1 << c.Val}}}, true
		}
	}
	return machineAddressExpr{terms: []machineScaledTerm{{value: v, scale: 1}}}, true
}

// preferConstDisplacementTerms disambiguates constants that numerically overlap
// globals from constants used as byte displacements.
//
// A RHS constant in pointer/address arithmetic is a displacement:
//
//	pointer + 0x24c
//	pointer - 0x24c
//
// A LHS constant in addition is allowed to remain an absolute global base:
//
//	0x59a2 + 0xc0*i
//
// unless the RHS already provides a resolved base, in which case the LHS
// constant is the displacement:
//
//	4 + pointer
func preferConstDisplacementTerms(op machine.ValueOp, lhsValue machine.Value, lhs machineAddressExpr, rhsValue machine.Value, rhs machineAddressExpr) (machineAddressExpr, machineAddressExpr) {
	if c, ok := rhsValue.(*machine.Const); ok {
		rhs = machineAddressExpr{
			offset: signedWordOffset(c.Val),
		}
	}

	if c, ok := lhsValue.(*machine.Const); ok &&
		op == machine.ValueOpAdd &&
		rhs.base != nil {
		lhs = machineAddressExpr{
			offset: signedWordOffset(c.Val),
		}
	}

	return lhs, rhs
}

// mergeMachineAddressExpr combines two partial address decompositions.
func mergeMachineAddressExpr(a, b machineAddressExpr) machineAddressExpr {
	out := machineAddressExpr{base: a.base, offset: a.offset + b.offset}
	if out.base == nil {
		out.base = b.base
	}
	out.terms = append(out.terms, a.terms...)
	out.terms = append(out.terms, b.terms...)
	return out
}

// machineScaledTermFromMul extracts x*N or N*x as a byte-scaled term.
func machineScaledTermFromMul(a, b machine.Value) (machineScaledTerm, bool) {
	if c, ok := a.(*machine.Const); ok {
		return machineScaledTerm{value: b, scale: int(c.Val)}, true
	}
	if c, ok := b.(*machine.Const); ok {
		return machineScaledTerm{value: a, scale: int(c.Val)}, true
	}
	return machineScaledTerm{}, false
}

// arrayScaledIndex extracts the source index from a byte-scaled array offset.
func arrayScaledIndex(value machine.Value, elemSize int) (machine.Value, bool) {
	value = unwrapAddressWord(value)
	if elemSize == 1 {
		return value, true
	}
	binary, ok := value.(*machine.Binary)
	if !ok {
		return nil, false
	}
	switch binary.Op {
	case machine.ValueOpMul:
		if c, ok := binary.LHS.(*machine.Const); ok && int(c.Val) == elemSize {
			return binary.RHS, true
		}
		if c, ok := binary.RHS.(*machine.Const); ok && int(c.Val) == elemSize {
			return binary.LHS, true
		}
	case machine.ValueOpShl:
		if c, ok := binary.RHS.(*machine.Const); ok && elemSize == 1<<c.Val {
			return binary.LHS, true
		}
	}
	return nil, false
}

// unwrapAddressWord removes low-word projections around address arithmetic.
func unwrapAddressWord(value machine.Value) machine.Value {
	if word, ok := value.(*machine.WordValue); ok && word.Part == machine.WordLow {
		return word.Parent
	}
	return value
}

// signedWordOffset interprets word-sized arithmetic constants as signed residual offsets.
func signedWordOffset(v uint) int {
	if v <= 0xffff && v&0x8000 != 0 {
		return int(int16(uint16(v)))
	}
	return int(v)
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

	for {
		next, nextOffset, nextTerms, changed := consumeAddressStep(current, offset, terms, width)
		if !changed {
			break
		}
		current = next
		offset = nextOffset
		terms = nextTerms

		// A pointer-valued subobject produced while resolving this
		// memory address is pointer storage, not another implicit
		// dereference. Following it requires a separate machine load.
		if typeinfo.IsPointer(current.ExprType()) {
			break
		}
	}

	if offset == 0 && len(terms) == 0 {
		if ptr, ok := current.ExprType().(*typeinfo.Pointer); ok && addr.Deref && width > 0 && ptr.Elem != nil {

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

	if ptr, ok := current.ExprType().(*typeinfo.Pointer); ok && addr.Deref && len(terms) == 0 && ptr.Elem != nil {
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

// partialPointerPointee returns true if ptr is a pointer to a slice inside another pointer
func partialPointerPointee(ptr *typeinfo.Pointer, offset, width int) bool {
	if !typeinfo.IsPointer(ptr.Elem) {
		return false
	}

	elemWidth := ptr.Elem.Bytes()
	return offset >= 0 && width > 0 && elemWidth > width && offset+width <= elemWidth
}

// consumeAddressStep consumes one field, array index, or pointer projection.
func consumeAddressStep(current Expr, offset int, terms []ScaledTerm, width int) (Expr, int, []ScaledTerm, bool) {
	typ := current.ExprType()
	if ptr, ok := typ.(*typeinfo.Pointer); ok {
		if next, nextTerms, changed := consumeArrayTerm(current, typ, terms); changed {
			return next, offset, nextTerms, true
		}
		if next, nextOffset, changed := consumeStructField(current, ptr.Elem, offset, terms, width); changed {
			return next, nextOffset, terms, true
		}
		if next, nextOffset, changed := consumeArrayConstIndex(current, typ, offset, width, false); changed {
			return next, nextOffset, terms, true
		}
		return current, offset, terms, false
	}
	if next, nextOffset, changed := consumeStructField(current, typ, offset, terms, width); changed {
		return next, nextOffset, terms, true
	}
	if next, nextTerms, changed := consumeArrayTerm(current, typ, terms); changed {
		return next, offset, nextTerms, true
	}
	if next, nextOffset, changed := consumeArrayConstIndex(current, typ, offset, width, true); changed {
		return next, nextOffset, terms, true
	}
	return current, offset, terms, false
}

// consumeStructField consumes a constant offset into a struct field.
func consumeStructField(base Expr, typ typeinfo.Type, offset int, terms []ScaledTerm, width int) (Expr, int, bool) {
	strct, ok := typ.(*typeinfo.Struct)
	if !ok {
		return nil, 0, false
	}
	matches := strct.FieldsContainingOffset(offset)
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
func consumeArrayTerm(base Expr, typ typeinfo.Type, terms []ScaledTerm) (Expr, []ScaledTerm, bool) {
	elem := indexElementType(typ)
	if elem == nil || elem.Bytes() <= 0 {
		return nil, nil, false
	}
	for i, term := range terms {
		if term.Scale != elem.Bytes() {
			continue
		}
		nextTerms := append([]ScaledTerm(nil), terms[:i]...)
		nextTerms = append(nextTerms, terms[i+1:]...)
		return &ArrayIndex{Base: base, Index: term.Expr, TypeInfo: elem}, nextTerms, true
	}
	return nil, nil, false
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
