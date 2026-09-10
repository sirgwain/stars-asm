package sem

import (
	"log/slog"

	"github.com/sirgwain/stars-asm/dasm/log"
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// FuncContext carries symbol and image facts for semantic lowering.
type FuncContext struct {
	img     *asm.ImageNE
	sdb     *typeinfo.SymbolDB
	res     *symresolve.Resolver
	symbols *symbolResolver
	fs      *typeinfo.Function
	log     *slog.Logger
	dsReg   machine.Value
	csReg   machine.Value

	// for logging
	currentBlock   *machine.BlockID
	currentInstOff uint32
	fromAddr       uint32
	toAddr         uint32

	unionContexts          map[machine.BlockID]*symresolve.UnionContext
	currentUnionContext    *symresolve.UnionContext
	unionBlockPathFacts    map[machine.BlockID][]*typeinfo.UnionBlockPathFact
	configuredUnionBase    *symresolve.UnionContext
	configuredUnionByBlock map[machine.BlockID]*symresolve.UnionContext
}

// RecordedUnionBlockPathFacts returns configured and discovered union selections grouped by block.
func (ctx *FuncContext) RecordedUnionBlockPathFacts() map[machine.BlockID][]*typeinfo.UnionBlockPathFact {
	out := make(map[machine.BlockID][]*typeinfo.UnionBlockPathFact, len(ctx.unionBlockPathFacts))
	for block, facts := range ctx.unionBlockPathFacts {
		out[block] = append([]*typeinfo.UnionBlockPathFact(nil), facts...)
	}
	return out
}

// addUnionBlockPathFact records one distinct union selection used in a block.
func (ctx *FuncContext) addUnionBlockPathFact(fact *typeinfo.UnionBlockPathFact) bool {
	block := machine.BlockID(fact.BlockOff)
	for _, existing := range ctx.unionBlockPathFacts[block] {
		if existing.Root == fact.Root &&
			existing.AllElements == fact.AllElements &&
			existing.Type == fact.Type &&
			existing.Value.Value == fact.Value.Value {
			return false
		}
	}
	ctx.unionBlockPathFacts[block] = append(ctx.unionBlockPathFacts[block], fact)
	return true
}

// addUnionSelectionBlockFact records a replayable selection for one block.
func (ctx *FuncContext) addUnionSelectionBlockFact(block machine.BlockID, selection symresolve.UnionSelection) bool {
	if selection.Root == nil || selection.Rule == nil || selection.Rule.Type == nil || selection.Rule.Enum == nil {
		return false
	}
	root, offset, ok := symbolOffsetRoot(selection.Root)
	if !ok || offset != 0 {
		return false
	}
	if _, ok := root.(*symresolve.SymbolRoot); !ok {
		return false
	}
	return ctx.addUnionBlockPathFact(&typeinfo.UnionBlockPathFact{
		Func:        ctx.fs,
		BlockOff:    uint32(block),
		Root:        root.String(),
		AllElements: selection.AllElements,
		Type:        selection.Rule.Type,
		Path:        append([]string(nil), selection.Rule.Discriminator...),
		Enum:        selection.Rule.Enum,
		Value:       selection.Value,
		Rule:        selection.Rule,
	})
}

// unionFactRootPath gets the root symbol and field path for a union fact like
//
//	"func": "DoAutomitronAiTurn",
//	"root": "sel",
//	"root_path": ["sel", "fl", "lpplord", "rgord"],
func (ctx *FuncContext) unionFactRootPath(rootName string, fields []string) (symresolve.SymbolPath, bool) {
	root, ok := ctx.symbolRootByName(rootName)
	if !ok {
		return nil, false
	}
	if len(fields) == 0 {
		return root, true
	}
	return appendSymbolFieldPath(root, fields)
}

// SetUnionContexts installs block-entry union contexts for later passes.
func (ctx *FuncContext) SetUnionContexts(contexts map[machine.BlockID]*symresolve.UnionContext) {
	merged := make(map[machine.BlockID]*symresolve.UnionContext, len(contexts))
	for block, derived := range contexts {
		configured := ctx.configuredUnionBase
		if blockContext := ctx.configuredUnionByBlock[block]; blockContext != nil {
			configured = blockContext
		}
		merged[block] = symresolve.MergeUnionContexts(derived, configured)
	}
	ctx.unionContexts = merged
}

// SetCurrentBlock selects the union context for a block being processed.
func (ctx *FuncContext) SetCurrentBlock(id machine.BlockID) {
	ctx.currentBlock = &id
	ctx.currentUnionContext = ctx.configuredUnionBase
	if configured := ctx.configuredUnionByBlock[id]; configured != nil {
		ctx.currentUnionContext = configured
	}
	if ctx.unionContexts != nil {
		if derived := ctx.unionContexts[id]; derived != nil {
			ctx.currentUnionContext = derived
		}
	}
}

// ClearCurrentBlock clears the active block-specific union context.
func (ctx *FuncContext) ClearCurrentBlock() {
	ctx.currentBlock = nil
	ctx.currentInstOff = 0
	ctx.currentUnionContext = nil
}

// unionContext returns the active path-sensitive union context.
func (ctx *FuncContext) unionContext() *symresolve.UnionContext {
	return ctx.currentUnionContext
}

// custom lazy logger
func (ctx *FuncContext) LogValue() slog.Value {
	if ctx.currentBlock == nil {
		return slog.GroupValue(
			slog.String("func", ctx.fs.Name),
		)
	}
	return slog.GroupValue(
		slog.String("func", ctx.fs.Name),
		slog.String("block", ctx.currentBlock.String()),
	)
}

func (ctx *FuncContext) WithOptions(fromAddr, toAddr uint32) *FuncContext {
	ctx.fromAddr = fromAddr
	ctx.toAddr = toAddr
	return ctx
}

// NewFuncContext creates a function-scoped semantic lowering context.
func NewFuncContext(img *asm.ImageNE, sdb *typeinfo.SymbolDB, res *symresolve.Resolver, fs *typeinfo.Function) *FuncContext {

	ctx := &FuncContext{
		img:                 img,
		sdb:                 sdb,
		res:                 res,
		fs:                  fs,
		dsReg:               machine.RegVal(asm.RegDS),
		csReg:               machine.RegVal(asm.RegCS),
		unionBlockPathFacts: make(map[machine.BlockID][]*typeinfo.UnionBlockPathFact),
	}
	ctx.initializeConfiguredUnionContexts()

	handler := &semLogger{
		Handler: &log.LazyHandler{Handler: slog.Default().Handler()},
		include: ctx.shouldLogCurrentBlock,
	}
	log := slog.New(handler).With(slog.Any("e", ctx))
	ctx.log = log
	ctx.symbols = newSymbolResolver(ctx)

	return ctx
}

// initializeConfiguredUnionContexts builds contexts available before semantic processing begins.
func (ctx *FuncContext) initializeConfiguredUnionContexts() {
	// An active, non-nil context enables each union rule's default member;
	// configured function and block facts below override that default by path.
	ctx.configuredUnionBase = symresolve.NewUnionContext()
	ctx.configuredUnionByBlock = make(map[machine.BlockID]*symresolve.UnionContext)
	if ctx.sdb.UnionRules == nil {
		return
	}
	for _, fact := range ctx.sdb.UnionRules.FunctionFactsFor(ctx.fs) {
		root, ok := ctx.unionFactRootPath(fact.Root, fact.RootPath)
		if !ok {
			continue
		}
		if fact.AllElements {
			ctx.configuredUnionBase.AddAllElements(root, fact.Rule, fact.Value)
		} else {
			ctx.configuredUnionBase.Add(root, fact.Rule, fact.Value)
		}
	}
	for _, fact := range ctx.sdb.UnionRules.BlockPathFacts {
		if fact.Func != ctx.fs {
			continue
		}
		ctx.addUnionBlockPathFact(fact)
		root, ok := ctx.unionFactRootPath(fact.Root, fact.RootPath)
		if !ok {
			continue
		}
		block := machine.BlockID(fact.BlockOff)
		blockContext := ctx.configuredUnionByBlock[block]
		if blockContext == nil {
			blockContext = ctx.configuredUnionBase.Clone()
			ctx.configuredUnionByBlock[block] = blockContext
		}
		if fact.AllElements {
			blockContext.AddAllElements(root, fact.Rule, fact.Value)
		} else {
			blockContext.Add(root, fact.Rule, fact.Value)
		}
	}
}

// symbolRootByName returns a function or global symbolic root by name.
func (ctx *FuncContext) symbolRootByName(name string) (symresolve.SymbolPath, bool) {
	if root, ok := ctx.functionRootByName(name); ok {
		return root, true
	}
	global := ctx.sdb.GetGlobal(name)
	if global == nil {
		return nil, false
	}
	return &symresolve.SymbolRoot{Symbol: global}, true
}

// functionRootByName returns a function parameter or local root by name.
func (ctx *FuncContext) functionRootByName(name string) (symresolve.SymbolPath, bool) {
	for i := range ctx.fs.Params {
		if ctx.fs.Params[i].Name == name {
			return &symresolve.SymbolRoot{Symbol: &ctx.fs.Params[i]}, true
		}
	}
	for i := range ctx.fs.Vars {
		if ctx.fs.Vars[i].Name == name {
			return &symresolve.SymbolRoot{Symbol: &ctx.fs.Vars[i]}, true
		}
	}
	return nil, false
}

// segFromRegister returns the segment value for a given register based on the function context
func (ctx *FuncContext) segFromRegister(reg asm.Reg) uint16 {
	switch reg {
	case asm.RegDS:
		return uint16(ctx.sdb.DGroupFrame)
	case asm.RegCS:
		return ctx.fs.Addr.Seg
	}
	return 0
}

// sameResolvedStorage reports whether two memory accesses are structurally or
// symbolically the same storage.
func (ctx *FuncContext) sameResolvedStorage(a machine.MemoryAddress, b machine.MemoryAddress) bool {
	return ctx.symbols.sameResolvedStorage(a, b)
}

// maskedStorageWrite reports whether value preserves bits from the destination
// storage using resolver-aware memory equivalence.
func (ctx *FuncContext) maskedStorageWrite(mem machine.MemoryAddress, value machine.Value) bool {
	return maskedStorageWrite(mem, value, ctx.sameResolvedStorage)
}

// maskedStorageWrite reports whether value preserves bits from the
// destination storage using the supplied storage equivalence predicate.
func maskedStorageWrite(mem machine.MemoryAddress, value machine.Value, same func(machine.MemoryAddress, machine.MemoryAddress) bool) bool {
	_, _, _, ok := bitfieldStoreParts(mem, value, same)
	return ok
}

// contiguousMaskRange returns the bit offset and width for a contiguous mask.
func contiguousMaskRange(mask uint) (int, int, bool) {
	if mask == 0 {
		return 0, 0, false
	}
	bitOff := 0
	for mask&1 == 0 {
		bitOff++
		mask >>= 1
	}
	bitWidth, ok := lowBitMaskWidth(mask)
	return bitOff, bitWidth, ok
}

// bitMask returns a low-bit mask for a bit width.
func bitMask(width int) (uint, bool) {
	if width <= 0 || width >= 64 {
		return 0, false
	}
	return (uint(1) << width) - 1, true
}

// sameStorage reports whether two memory accesses describe the same storage.
func sameStorage(a, b machine.MemoryAddress) bool {
	return a.Disp == b.Disp &&
		a.Width == b.Width &&
		valueShapeEquals(a.Seg, b.Seg) &&
		valueShapeEquals(a.Base, b.Base) &&
		valueShapeEquals(a.Index, b.Index)
}

// valueShapeEquals compares machine values while ignoring instruction origins.
func valueShapeEquals(a, b machine.Value) bool {
	if a == nil || b == nil {
		return a == nil && b == nil
	}
	switch av := a.(type) {
	case *machine.Const:
		bv, ok := b.(*machine.Const)
		return ok && av.Val == bv.Val
	case *machine.Reg:
		bv, ok := b.(*machine.Reg)
		return ok && av.Val == bv.Val
	case *machine.FrameBase:
		_, ok := b.(*machine.FrameBase)
		return ok
	case *machine.Load:
		bv, ok := b.(*machine.Load)
		return ok && sameStorage(av.Addr, bv.Addr)
	case *machine.FarPointer:
		bv, ok := b.(*machine.FarPointer)
		return ok &&
			av.Part == bv.Part &&
			valueShapeEquals(av.Parent, bv.Parent) &&
			valueShapeEquals(av.Offset, bv.Offset) &&
			valueShapeEquals(av.Segment, bv.Segment)
	case *machine.Binary:
		bv, ok := b.(*machine.Binary)
		return ok &&
			av.Op == bv.Op &&
			valueShapeEquals(av.LHS, bv.LHS) &&
			valueShapeEquals(av.RHS, bv.RHS)
	case *machine.Address:
		bv, ok := b.(*machine.Address)
		return ok && sameStorage(av.Addr, bv.Addr)
	default:
		return machine.ValueEquals(a, b)
	}
}

// constOperand splits one constant and one non-constant machine value.
func constOperand(a, b machine.Value) (*machine.Const, machine.Value, bool) {
	if c, ok := a.(*machine.Const); ok {
		return c, b, true
	}
	if c, ok := b.(*machine.Const); ok {
		return c, a, true
	}
	return nil, nil, false
}

// lowBitMaskWidth returns the width of a contiguous low-bit mask.
func lowBitMaskWidth(mask uint) (int, bool) {
	if mask == 0 {
		return 0, false
	}
	width := 0
	for mask&1 == 1 {
		width++
		mask >>= 1
	}
	return width, mask == 0
}

// symbolPathForExpr returns the source-level path represented by an expression.
func symbolPathForExpr(expr Expr) (symresolve.SymbolPath, bool) {
	switch e := expr.(type) {
	case *Local:
		return &symresolve.SymbolRoot{Symbol: &e.FunctionVar}, true
	case *Global:
		return &symresolve.SymbolRoot{Symbol: e.GlobalVar}, true
	case *SymbolRef:
		return e.Path, true
	case *FieldAccess:
		base, ok := symbolPathForExpr(e.Base)
		if !ok {
			return nil, false
		}
		if e.Field.Bitfield != nil {
			return &symresolve.SymbolBitfield{Base: base, Field: e.Field}, true
		}
		return &symresolve.SymbolField{Base: base, Field: e.Field}, true
	case *ArrayIndex:
		base, ok := symbolPathForExpr(e.Base)
		if !ok || e.TypeInfo == nil {
			return nil, false
		}
		term := &symresolve.SymbolTerm{Base: base, Scale: e.TypeInfo.Bytes(), Result: e.TypeInfo}
		if index, ok := symbolPathForExpr(e.Index); ok {
			term.Index = index
		} else if index, ok := e.Index.(*Const); ok {
			term.IndexVal = machine.ConstVal(uint(index.U64))
		} else {
			return nil, false
		}
		return term, true
	case *Deref:
		base, ok := symbolPathForExpr(e.Pointer)
		if !ok {
			return nil, false
		}
		deref := symresolve.SymbolPath(&symresolve.SymbolDeref{Base: base})
		if e.ByteOff != 0 {
			return &symresolve.SymbolOffset{Base: deref, Offset: e.ByteOff, Result: deref.Type()}, true
		}
		return deref, true
	case *Part:
		base, ok := symbolPathForExpr(e.Base)
		if !ok {
			return nil, false
		}
		return &symresolve.SymbolOffset{Base: base, Offset: e.ByteOff, Result: e.Base.ExprType()}, true
	default:
		return nil, false
	}
}

// shouldLogCurrentBlock reports whether the currently processed block is in the
// requested debug range.
func (ctx *FuncContext) shouldLogCurrentBlock() bool {
	if ctx.fromAddr == 0 && ctx.toAddr == 0 {
		return true
	}
	if ctx.currentBlock == nil {
		return true
	}
	if ctx.fromAddr != 0 && uint32(*ctx.currentBlock) < ctx.fromAddr {
		return false
	}
	if ctx.fromAddr == 0 && uint32(*ctx.currentBlock) != ctx.toAddr {
		return false
	}
	if ctx.toAddr == 0 && uint32(*ctx.currentBlock) != ctx.fromAddr {
		return false
	}
	if ctx.toAddr != 0 && uint32(*ctx.currentBlock) >= ctx.toAddr {
		return false
	}
	return true
}
