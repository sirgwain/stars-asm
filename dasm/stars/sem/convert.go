package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertMachineToFunc converts extracted machine effects into initial semantic effects.
func convertMachineToFunc(ctx *FuncContext, result *Result, effects *machine.FuncEffects) *Func {
	converter := machineConverter{
		ctx:            ctx,
		result:         result,
		inlineCalls:    make(map[machineCallResultKey]*Call),
		inlineEligible: machineInlineCallResults(effects),
	}
	fn := Func{CFG: effects.CFG}
	fn.Blocks = make([]Block, 0, len(effects.Blocks))
	for _, block := range effects.Blocks {
		ctx.SetCurrentBlock(block.Block)
		fn.Blocks = append(fn.Blocks, Block{
			ID:      block.Block,
			Effects: converter.convertEffects(block.Effects),
		})
		ctx.ClearCurrentBlock()
	}
	return &fn
}

type machineConverter struct {
	ctx            *FuncContext
	result         *Result
	instOff        uint32
	memWrites      map[string]uint32
	tempByLoad     map[machine.ValueID]*Temp
	tempRequests   map[uint32][]*machine.Load
	noBitfields    bool
	inlineCalls    map[machineCallResultKey]*Call
	inlineEligible map[machineCallResultKey]bool
}

type machineCallResultKey struct {
	target  *typeinfo.Function
	instOff uint32
}

// convertEffects converts machine effects while preserving unresolved raw storage.
func (c *machineConverter) convertEffects(effects []machine.Effect) []Effect {
	c.memWrites = make(map[string]uint32)
	c.tempByLoad = make(map[machine.ValueID]*Temp)
	c.tempRequests = collectTempRequests(effects)
	out := make([]Effect, 0, len(effects)+len(c.tempRequests))
	for _, effect := range effects {
		out = append(out, c.convertTempAssignments(effect.EffectMeta().InstOff)...)
		if converted := c.convertEffect(effect); converted != nil {
			out = append(out, converted)
		}
	}
	return out
}

// convertEffect converts one machine effect into a semantic effect.
func (c *machineConverter) convertEffect(effect machine.Effect) Effect {
	c.instOff = effect.EffectMeta().InstOff
	c.ctx.currentInstOff = c.instOff
	switch e := effect.(type) {
	case machine.StoreEffect:
		if path, value, ok := c.ctx.symbols.symbolFromBitfieldStore(e.Addr, e.Src); ok {
			c.recordMemoryWrite(e.Addr, e.Width)
			src := c.convertValue(value)
			if constant, ok := src.(*Const); ok {
				constant.TypeInfo = path.Type()
			}
			return &Assign{
				MetaInfo: e.MetaInfo,
				Dst:      &SymbolRef{Path: path},
				Src:      src,
			}
		}
		dst := c.convertMemoryLValue(e.Addr, e.Width)
		src := c.convertValue(e.Src)
		if e.Width > 2 {
			src = c.convertValueTyped(e.Src, dst.ExprType())
		}
		if c.ctx.maskedStorageWrite(e.Addr, e.Src) {
			src = c.convertValueWithoutBitfields(e.Src)
		}
		assign := &Assign{
			MetaInfo: e.MetaInfo,
			Dst:      dst,
			Src:      src,
		}
		c.recordMemoryWrite(e.Addr, e.Width)
		return assign
	case machine.CopyEffect:
		dst := LValue(&RawValue{Value: e.Dst, TypeInfo: intTypeForWidth(e.Width)})
		if lvalue, ok := c.convertCopyAddress(e.Dst, e.Width); ok {
			dst = lvalue
		}
		src := c.convertValue(e.Src)
		if lvalue, ok := c.convertCopyAddress(e.Src, e.Width); ok {
			src = lvalue
		}
		assign := &Assign{
			MetaInfo: e.MetaInfo,
			Dst:      dst,
			Src:      src,
		}
		c.recordCopyWrite(e.Dst, e.Width)
		return assign
	case machine.CallEffect:
		target, fn := c.convertCallTarget(e)
		args := e.Args
		call := &Call{Function: fn, Target: target, Args: c.convertCallArgs(fn, args)}
		if callResult, ok := e.Result.(*machine.CallResult); ok {
			key := machineCallResultKey{target: callResult.Target, instOff: callResult.InstOff}
			if c.inlineEligible[key] {
				c.inlineCalls[key] = call
				return nil
			}
		}
		var result Expr
		if e.Result != nil {
			result = c.convertValue(e.Result)
		}
		return &CallEffect{MetaInfo: e.MetaInfo, Call: call, Result: result}
	case machine.BranchEffect:
		return &Branch{
			MetaInfo:   e.MetaInfo,
			Cond:       c.convertValue(e.Predicate),
			TrueBlock:  e.TrueBlock,
			FalseBlock: e.FalseBlock,
		}
	case machine.JumpEffect:
		return &Jump{MetaInfo: e.MetaInfo, To: e.To}
	case machine.ReturnEffect:
		var expected typeinfo.Type
		if c.ctx != nil && c.ctx.fs != nil {
			expected = c.ctx.fs.Ret
		}
		return &Return{MetaInfo: e.MetaInfo, Value: c.convertValueTyped(e.Value, expected)}
	default:
		return &RawEffect{Effect: effect, MetaInfo: effect.EffectMeta()}
	}
}

// convertCallTarget resolves direct and indirect machine call targets.
func (c *machineConverter) convertCallTarget(effect machine.CallEffect) (Expr, *typeinfo.Function) {
	if effect.Target != nil {
		return nil, effect.Target
	}
	if emptyCallMemoryAccess(effect.MemoryAccess) {
		return nil, nil
	}
	access := effect.MemoryAccess
	access.Width = 4
	target := c.convertMemoryLValue(access, access.Width)
	fn, _ := functionFromCallTargetType(target.ExprType(), access.Width)
	return target, fn
}

// functionFromCallTargetType resolves the callee signature for a direct
// function pointer or a loaded function-pointer storage slot.
func functionFromCallTargetType(typ typeinfo.Type, width int) (*typeinfo.Function, bool) {
	if fn, ok := typeinfo.GetFunctionPointerFunction(typ); ok {
		return fn, true
	}
	ptr, ok := typ.(*typeinfo.Pointer)
	if !ok || ptr.Elem == nil || ptr.Elem.Bytes() != width {
		return nil, false
	}
	return typeinfo.GetFunctionPointerFunction(ptr.Elem)
}

// emptyCallMemoryAccess reports whether an indirect call has no target memory.
func emptyCallMemoryAccess(mem machine.MemoryAddress) bool {
	return mem.Seg == nil &&
		mem.Base == nil &&
		mem.Index == nil &&
		mem.Disp == 0 &&
		mem.Width == 0 &&
		mem.Origin == (machine.Origin{})
}

// convertValues converts a slice of machine values into semantic expressions.
func (c *machineConverter) convertValues(values []machine.Value) []Expr {
	out := make([]Expr, len(values))
	for i, value := range values {
		out[i] = c.convertValue(value)
	}
	return out
}

// convertValue converts one machine value into a semantic expression.
func (c *machineConverter) convertValue(value machine.Value) Expr {
	if _, address := value.(*machine.Address); address {
		if resolved, ok := c.resolveAddressValue(value); ok {
			return resolved
		}
	}
	if !c.noBitfields {
		if path, ok := c.ctx.symbols.symbolFromBitfieldValue(value); ok {
			return &SymbolRef{Path: path}
		}
	}
	switch v := value.(type) {
	case nil:
		return nil
	case *machine.Reg:
		return &Register{Val: v.Val, SegNum: c.ctx.segFromRegister(v.Val)}
	case *machine.Const:
		return &Const{TypeInfo: typeinfo.U16, U64: uint64(v.Val), Origin: v.Origin, Fixup: v.Fixup}
	case *machine.FloatConst:
		return &FloatConst{TypeInfo: &typeinfo.Primitive{TypeKind: typeinfo.KFloat, Name: "float", Size: 4}, F64: v.Val}
	case *machine.CallResult:
		if call := c.inlineCalls[machineCallResultKey{target: v.Target, instOff: v.InstOff}]; call != nil {
			return call
		}
		return &CallResult{Function: v.Target, TypeInfo: v.Type, InstOff: v.InstOff}
	case *machine.WordValue:
		return &Word{Parent: c.convertValue(v.Parent), Part: v.Part}
	case *machine.FarPointer:
		return &FarPointer{
			Parent:  c.convertValue(v.Parent),
			Part:    v.Part,
			Offset:  c.convertValue(v.Offset),
			Segment: c.convertValue(v.Segment),
		}
	case *machine.SignExtendValue:
		return &SignExtend{Parent: c.convertValue(v.Parent), FromBits: v.FromBits, ToBits: v.ToBits, TypeInfo: intTypeForWidth(v.ToBits / 8)}
	case *machine.Binary:
		return &Binary{TypeInfo: typeinfo.U16, Op: convertOp(v.Op), LHS: c.convertValue(v.LHS), RHS: c.convertValue(v.RHS)}
	case *machine.ByteValue:
		return c.convertByte(v)
	case *machine.Cast:
		return &Cast{Value: c.convertValue(v.Value), To: v.To.String(), TypeInfo: v.To}
	case *machine.Load:
		if temp := c.tempByLoad[v.ID]; temp != nil && c.staleLoad(v) {
			return temp
		}
		return c.convertMemoryLValue(v.Addr, v.Addr.Width)
	case *machine.Address:
		return &AddressOf{Target: c.convertMemoryLValue(v.Addr, v.Addr.Width), TypeInfo: typeinfo.U16}
	case *machine.StackWords:
		words := &Words{Words: c.convertValues(v.Words)}
		if collapsed, ok := collapseWideWords(words); ok {
			return collapsed
		}
		return words
	case *machine.PredicateValue:
		return c.convertPredicate(v)
	case *machine.PhiValue:
		return c.convertPhi(v)
	default:
		return &RawValue{Value: value}
	}
}

// machineInlineCallResults identifies single-use call results confined to one block.
func machineInlineCallResults(effects *machine.FuncEffects) map[machineCallResultKey]bool {
	type usage struct {
		count int
		block machine.BlockID
		mixed bool
	}
	uses := make(map[machineCallResultKey]usage)
	for _, block := range effects.Blocks {
		walker := &machineRewriter{
			value: func(w *machineRewriter, value machine.Value) (machine.Value, bool, bool) {
				result, ok := value.(*machine.CallResult)
				if !ok {
					return value, false, false
				}
				key := machineCallResultKey{target: result.Target, instOff: result.InstOff}
				current := uses[key]
				if current.count > 0 && current.block != block.Block {
					current.mixed = true
				}
				current.count++
				current.block = block.Block
				uses[key] = current
				return value, false, false
			},
		}
		walker.rewriteMachineEffects(block.Effects)
	}
	inline := make(map[machineCallResultKey]bool)
	for key, usage := range uses {
		// One visit is the call-effect definition and one is the sole use.
		inline[key] = usage.count == 2 && !usage.mixed
	}
	return inline
}

// convertValueWithoutBitfields converts a machine value with bitfield extraction disabled.
func (c *machineConverter) convertValueWithoutBitfields(value machine.Value) Expr {
	prev := c.noBitfields
	c.noBitfields = true
	defer func() { c.noBitfields = prev }()
	return c.convertValue(value)
}

// convertByte converts a machine byte projection or replacement into semantic IR.
func (c *machineConverter) convertByte(v *machine.ByteValue) Expr {
	byteValue := &Byte{
		Parent:   c.convertValue(v.Parent),
		Part:     v.Part,
		TypeInfo: typeinfo.U8,
	}
	if v.Value != nil {
		byteValue.Value = c.convertValue(*v.Value)
		byteValue.TypeInfo = typeinfo.U16
	}
	return byteValue
}

// convertTempAssignments creates temp captures scheduled before instOff.
func (c *machineConverter) convertTempAssignments(instOff uint32) []Effect {
	loads := c.tempRequests[instOff]
	if len(loads) == 0 {
		return nil
	}
	assigns := make([]Effect, 0, len(loads))
	for _, load := range loads {
		storage := c.convertMemoryLValue(load.Addr, load.Addr.Width)
		temp := &Temp{
			Name:     tempName(load.ID),
			ID:       load.ID,
			Source:   storage,
			TypeInfo: storage.ExprType(),
		}
		if temp.TypeInfo == nil {
			temp.TypeInfo = intTypeForWidth(load.Addr.Width)
		}
		c.tempByLoad[load.ID] = temp
		assigns = append(assigns, &Assign{
			MetaInfo: machine.Meta{InstOff: load.ID.InstOff},
			Dst:      temp,
			Src:      storage,
		})
	}
	return assigns
}

// tempName returns the stable semantic temp name for a machine value identity.
func tempName(id machine.ValueID) string {
	return fmt.Sprintf("t_%04x", id.InstOff)
}

// collectTempRequests finds loads that must be captured before storage mutation.
func collectTempRequests(effects []machine.Effect) map[uint32][]*machine.Load {
	writes := make(map[string]uint32)
	requests := make(map[uint32][]*machine.Load)
	seen := make(map[machine.ValueID]bool)
	for _, effect := range effects {
		switch e := effect.(type) {
		case machine.StoreEffect:
			collectTempLoads(e.Src, writes, requests, seen)
			writes[memoryWriteKey(e.Addr, e.Width)] = e.MetaInfo.InstOff
		case machine.CopyEffect:
			collectTempLoads(e.Dst, writes, requests, seen)
			collectTempLoads(e.Src, writes, requests, seen)
		case machine.CallEffect:
			for _, arg := range e.Args {
				collectTempLoads(arg, writes, requests, seen)
			}
			collectTempLoads(e.Result, writes, requests, seen)
		case machine.BranchEffect:
			collectTempLoads(e.Predicate, writes, requests, seen)
		case machine.ReturnEffect:
			collectTempLoads(e.Value, writes, requests, seen)
		}
	}
	return requests
}

// collectTempLoads records stale loads and the write they must be captured before.
func collectTempLoads(value machine.Value, writes map[string]uint32, requests map[uint32][]*machine.Load, seen map[machine.ValueID]bool) {
	switch v := value.(type) {
	case nil:
		return
	case *machine.PredicateValue:
		collectTempLoads(v.LHS, writes, requests, seen)
		collectTempLoads(v.RHS, writes, requests, seen)
	case *machine.WordValue:
		collectTempLoads(v.Parent, writes, requests, seen)
	case *machine.FarPointer:
		collectTempLoads(v.Parent, writes, requests, seen)
		collectTempLoads(v.Offset, writes, requests, seen)
		collectTempLoads(v.Segment, writes, requests, seen)
	case *machine.SignExtendValue:
		collectTempLoads(v.Parent, writes, requests, seen)
	case *machine.StackWords:
		for _, word := range v.Words {
			collectTempLoads(word, writes, requests, seen)
		}
	case *machine.ByteValue:
		collectTempLoads(v.Parent, writes, requests, seen)
		if v.Value != nil {
			collectTempLoads(*v.Value, writes, requests, seen)
		}
	case *machine.Cast:
		collectTempLoads(v.Value, writes, requests, seen)
	case *machine.Binary:
		collectTempLoads(v.LHS, writes, requests, seen)
		collectTempLoads(v.RHS, writes, requests, seen)
	case *machine.Load:
		if v.ID.IsZero() || seen[v.ID] {
			return
		}
		writeOff, ok := writes[memoryWriteKey(v.Addr, v.Addr.Width)]
		if !ok || v.ID.InstOff >= writeOff {
			return
		}
		seen[v.ID] = true
		requests[writeOff] = append(requests[writeOff], v)
	case *machine.PhiValue:
		for _, arm := range v.Arms {
			collectTempLoads(arm.Value, writes, requests, seen)
		}
	}
}

// recordMemoryWrite records that storage has been updated by the current effect.
func (c *machineConverter) recordMemoryWrite(mem machine.MemoryAddress, width int) {
	if c.memWrites == nil {
		return
	}
	c.memWrites[memoryWriteKey(mem, width)] = c.instOff
}

// recordCopyWrite records the storage written by a copy effect.
func (c *machineConverter) recordCopyWrite(value machine.Value, width int) {
	addr, ok := value.(*machine.Address)
	if !ok {
		return
	}
	c.recordMemoryWrite(copyAddressMemoryAccess(addr.Addr, width), width)
}

// staleLoad reports whether a load predates a write to the same storage.
func (c *machineConverter) staleLoad(load *machine.Load) bool {
	if load == nil || load.ID.IsZero() || c.memWrites == nil {
		return false
	}
	writeOff, ok := c.memWrites[memoryWriteKey(load.Addr, load.Addr.Width)]
	return ok && load.ID.InstOff < writeOff
}

// memoryWriteKey returns a storage key for detecting reads invalidated by writes.
func memoryWriteKey(mem machine.MemoryAddress, width int) string {
	mem.Origin = machine.Origin{}
	mem.Width = width
	return mem.String()
}

// convertMemoryLValue converts a machine memory address into a semantic lvalue.
func (c *machineConverter) convertMemoryLValue(mem machine.MemoryAddress, width int) LValue {
	if lvalue, ok := c.resolveAddressLValue(mem, width, nil); ok {
		return lvalue
	}
	return unresolvedMemory(c.ctx, c.result, mem)
}

// derefType returns the semantic type for a memory address through pointer.
func derefType(pointer Expr, width int) typeinfo.Type {
	if ptr, ok := pointer.ExprType().(*typeinfo.Pointer); ok && ptr.Elem != nil && ptr.Elem.Bytes() == width {
		return ptr.Elem
	}
	return intTypeForWidth(width)
}

// unresolvedMemory converts a raw machine memory address to an unresolved semantic lvalue.
func unresolvedMemory(ctx *FuncContext, result *Result, mem machine.MemoryAddress) *Memory {
	converter := machineConverter{ctx: ctx, result: result}
	return &Memory{
		Seg:      converter.convertValue(mem.Seg),
		Base:     converter.convertValue(mem.Base),
		Disp:     mem.Disp,
		Width:    mem.Width,
		Index:    converter.convertValue(mem.Index),
		TypeInfo: intTypeForWidth(mem.Width),
	}
}

// convertPredicate converts a machine predicate to a semantic expression.
func (c machineConverter) convertPredicate(v *machine.PredicateValue) Expr {
	if v.Kind != machine.PredicateCompare {
		return &RawValue{Value: v}
	}
	return &Compare{Op: compareOp(v.Op), LHS: c.convertValue(v.LHS), RHS: c.convertValue(v.RHS)}
}

// convertPhi converts a machine phi into a semantic merge expression.
func (c *machineConverter) convertPhi(v *machine.PhiValue) Expr {
	arms := make([]MergeArm, 0, len(v.Arms))
	for _, arm := range v.Arms {
		if arm.Block == nil {
			continue
		}
		arms = append(arms, MergeArm{Block: arm.Block.ID, Value: c.convertValueInBlock(arm.Block.ID, arm.Value, nil)})
	}
	return &Merge{TypeInfo: mergeType(arms), Join: v.Join, Arms: arms}
}

// convertPhiTyped propagates the surrounding type into each merge arm.
func (c *machineConverter) convertPhiTyped(v *machine.PhiValue, expected typeinfo.Type) Expr {
	arms := make([]MergeArm, 0, len(v.Arms))
	for _, arm := range v.Arms {
		if arm.Block == nil {
			continue
		}
		arms = append(arms, MergeArm{Block: arm.Block.ID, Value: c.convertValueInBlock(arm.Block.ID, arm.Value, expected)})
	}
	return &Merge{TypeInfo: expected, Join: v.Join, Arms: arms}
}

// convertValueInBlock converts a predecessor-owned value using that block's
// configured symbol context, then restores the surrounding conversion state.
func (c *machineConverter) convertValueInBlock(block machine.BlockID, value machine.Value, expected typeinfo.Type) Expr {
	previousBlock := c.ctx.currentBlock
	previousInstOff := c.ctx.currentInstOff
	c.ctx.SetCurrentBlock(block)
	var expr Expr
	if expected != nil {
		expr = c.convertValueTyped(value, expected)
	} else {
		expr = c.convertValue(value)
	}
	if previousBlock != nil {
		c.ctx.SetCurrentBlock(*previousBlock)
	} else {
		c.ctx.currentBlock = nil
		c.ctx.currentUnionContext = nil
	}
	c.ctx.currentInstOff = previousInstOff
	return expr
}

// mergeType returns the result type shared by merge arms.
func mergeType(arms []MergeArm) typeinfo.Type {
	for _, arm := range arms {
		if arm.Value != nil && arm.Value.ExprType() != nil {
			return arm.Value.ExprType()
		}
	}
	return nil
}

// convertOp converts a machine value operation to a semantic operation.
func convertOp(op machine.ValueOp) Op {
	switch op {
	case machine.ValueOpAdd:
		return OpAdd
	case machine.ValueOpSub:
		return OpSub
	case machine.ValueOpMul:
		return OpMul
	case machine.ValueOpDiv:
		return OpDiv
	case machine.ValueOpMod:
		return OpMod
	case machine.ValueOpAnd:
		return OpAnd
	case machine.ValueOpOr:
		return OpOr
	case machine.ValueOpXor:
		return OpXor
	case machine.ValueOpShl:
		return OpShl
	case machine.ValueOpShr, machine.ValueOpSar:
		return OpShr
	case machine.ValueOpNeg:
		return OpNeg
	case machine.ValueOpNot:
		return OpNot
	default:
		return OpUnknown
	}
}

// compareOp converts a machine branch mnemonic to a semantic comparison.
func compareOp(op string) CompareOp {
	switch machine.JccCompareOp(op) {
	case "==":
		return CompareEQ
	case "!=":
		return CompareNE
	case "<":
		return CompareLT
	case "<=":
		return CompareLE
	case ">":
		return CompareGT
	case ">=":
		return CompareGE
	default:
		return CompareUnknown
	}
}
