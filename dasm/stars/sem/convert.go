package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// convertMachineToFunc converts extracted machine effects into initial semantic effects.
func convertMachineToFunc(ctx *FuncContext, result *Result, effects *machine.FuncEffects) *Func {
	converter := machineConverter{ctx: ctx, result: result}
	fn := Func{CFG: effects.CFG}
	fn.Blocks = make([]Block, 0, len(effects.Blocks))
	for _, block := range effects.Blocks {
		fn.Blocks = append(fn.Blocks, Block{
			ID:      block.Block,
			Effects: converter.convertEffects(block.Effects),
		})
	}
	return &fn
}

type machineConverter struct {
	ctx          *FuncContext
	result       *Result
	instOff      uint32
	memWrites    map[string]uint32
	tempByLoad   map[machine.ValueID]*Temp
	tempRequests map[uint32][]*machine.Load
	noBitfields  bool
}

// convertEffects converts machine effects while preserving unresolved raw storage.
func (c *machineConverter) convertEffects(effects []machine.Effect) []Effect {
	c.memWrites = make(map[string]uint32)
	c.tempByLoad = make(map[machine.ValueID]*Temp)
	c.tempRequests = collectTempRequests(effects)
	out := make([]Effect, 0, len(effects)+len(c.tempRequests))
	for _, effect := range effects {
		out = append(out, c.convertTempAssignments(effect.EffectMeta().InstOff)...)
		out = append(out, c.convertEffect(effect))
	}
	return out
}

// convertEffect converts one machine effect into a semantic effect.
func (c *machineConverter) convertEffect(effect machine.Effect) Effect {
	c.instOff = effect.EffectMeta().InstOff
	switch e := effect.(type) {
	case machine.StoreEffect:
		if dst, src, ok := c.ctx.resolveBitfieldStore(e.Addr, e.Src); ok {
			c.recordMemoryWrite(e.Addr, e.Width)
			return &Assign{
				MetaInfo: e.MetaInfo,
				Dst:      dst,
				Src:      src,
			}
		}
		src := c.convertValue(e.Src)
		if maskedStorageWrite(e.Addr, e.Src) {
			src = c.convertValueWithoutBitfields(e.Src)
		}
		assign := &Assign{
			MetaInfo: e.MetaInfo,
			Dst:      c.convertMemoryLValue(e.Addr, e.Width),
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
		if e.Target == nil && fn != nil {
			args = indirectCallArgs(fn, e.Args)
		}
		call := &Call{Function: fn, Target: target, Args: c.convertCallArgs(fn, args)}
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
		return &Return{MetaInfo: e.MetaInfo, Value: c.convertValue(e.Value)}
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
	fn, _ := typeinfo.GetFunctionPointerFunction(target.ExprType())
	return target, fn
}

// emptyCallMemoryAccess reports whether an indirect call has no target memory.
func emptyCallMemoryAccess(access machine.MemoryAccess) bool {
	return access.Seg == nil &&
		access.Base == nil &&
		access.Index == nil &&
		access.Disp == 0 &&
		access.Width == 0 &&
		access.Scale == 0 &&
		access.Origin == (machine.Origin{})
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
	if resolved, ok := c.resolveAddressValue(value); ok {
		return resolved
	}
	if !c.noBitfields {
		if resolved, ok := c.ctx.resolveBitfieldExtract(value); ok {
			return resolved
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
		return c.convertMemoryLValue(v.Access, v.Access.Width)
	case *machine.Address:
		return &AddressOf{Target: c.convertMemoryLValue(v.Access, v.Access.Width), TypeInfo: typeinfo.U16}
	case *machine.StackWords:
		return &Words{Words: c.convertValues(v.Words)}
	case *machine.PredicateValue:
		return c.convertPredicate(v)
	case *machine.PhiValue:
		return c.convertPhi(v)
	default:
		return &RawValue{Value: value}
	}
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
		storage := c.convertMemoryLValue(load.Access, load.Access.Width)
		temp := &Temp{
			Name:     tempName(load.ID),
			ID:       load.ID,
			Source:   storage,
			TypeInfo: storage.ExprType(),
		}
		if temp.TypeInfo == nil {
			temp.TypeInfo = intTypeForWidth(load.Access.Width)
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
		writeOff, ok := writes[memoryWriteKey(v.Access, v.Access.Width)]
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
func (c *machineConverter) recordMemoryWrite(access machine.MemoryAccess, width int) {
	if c.memWrites == nil {
		return
	}
	c.memWrites[memoryWriteKey(access, width)] = c.instOff
}

// recordCopyWrite records the storage written by a copy effect.
func (c *machineConverter) recordCopyWrite(value machine.Value, width int) {
	addr, ok := value.(*machine.Address)
	if !ok {
		return
	}
	c.recordMemoryWrite(copyAddressMemoryAccess(addr.Access, width), width)
}

// staleLoad reports whether a load predates a write to the same storage.
func (c *machineConverter) staleLoad(load *machine.Load) bool {
	if load == nil || load.ID.IsZero() || c.memWrites == nil {
		return false
	}
	writeOff, ok := c.memWrites[memoryWriteKey(load.Access, load.Access.Width)]
	return ok && load.ID.InstOff < writeOff
}

// memoryWriteKey returns a storage key for detecting reads invalidated by writes.
func memoryWriteKey(access machine.MemoryAccess, width int) string {
	access.Origin = machine.Origin{}
	access.Width = width
	return access.String()
}

// convertMemoryLValue converts a machine memory access into a semantic lvalue.
func (c *machineConverter) convertMemoryLValue(access machine.MemoryAccess, width int) LValue {
	if lvalue, ok := c.ctx.resolveMachineStorage(access, width); ok {
		return lvalue
	}
	if lvalue, ok := c.resolveAddressLValue(access, width); ok {
		return lvalue
	}
	if lvalue, ok := c.convertNearPointerMemoryLValue(access, width); ok {
		return lvalue
	}
	if lvalue, ok := c.convertFarPointerMemoryLValue(access, width); ok {
		return lvalue
	}
	return unresolvedMemory(c.ctx, c.result, access)
}

// derefType returns the semantic type for a memory access through pointer.
func derefType(pointer Expr, width int) typeinfo.Type {
	if ptr, ok := pointer.ExprType().(*typeinfo.Pointer); ok && ptr.Elem != nil && ptr.Elem.Bytes() == width {
		return ptr.Elem
	}
	return intTypeForWidth(width)
}

// unresolvedMemory converts a raw machine memory access to an unresolved semantic lvalue.
func unresolvedMemory(ctx *FuncContext, result *Result, access machine.MemoryAccess) *Memory {
	converter := machineConverter{ctx: ctx, result: result}
	return &Memory{
		Seg:      converter.convertValue(access.Seg),
		Base:     converter.convertValue(access.Base),
		Disp:     access.Disp,
		Width:    access.Width,
		Index:    converter.convertValue(access.Index),
		Scale:    access.Scale,
		TypeInfo: intTypeForWidth(access.Width),
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
func (c machineConverter) convertPhi(v *machine.PhiValue) Expr {
	arms := make([]MergeArm, 0, len(v.Arms))
	for _, arm := range v.Arms {
		if arm.Block == nil {
			continue
		}
		arms = append(arms, MergeArm{Block: arm.Block.ID, Value: c.convertValue(arm.Value)})
	}
	return &Merge{TypeInfo: mergeType(arms), Join: v.Join, Arms: arms}
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
