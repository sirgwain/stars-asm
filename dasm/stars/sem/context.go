package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// FuncContext carries symbol and image facts for semantic lowering.
type FuncContext struct {
	img   *asm.ImageNE
	sdb   *typeinfo.SymbolDB
	res   *symresolve.Resolver
	fs    *typeinfo.Function
	dsReg machine.Value
	csReg machine.Value

	unionContexts       map[machine.BlockID]*symresolve.UnionContext
	currentUnionContext *symresolve.UnionContext
}

// SetUnionContexts installs block-entry union contexts for later passes.
func (ctx *FuncContext) SetUnionContexts(contexts map[machine.BlockID]*symresolve.UnionContext) {
	ctx.unionContexts = contexts
}

// SetCurrentBlock selects the union context for a block being processed.
func (ctx *FuncContext) SetCurrentBlock(id machine.BlockID) {
	ctx.currentUnionContext = nil
	if ctx.unionContexts != nil {
		ctx.currentUnionContext = ctx.unionContexts[id]
	}
}

// ClearCurrentBlock clears the active block-specific union context.
func (ctx *FuncContext) ClearCurrentBlock() {
	ctx.currentUnionContext = nil
}

// unionContext returns the active path-sensitive union context.
func (ctx *FuncContext) unionContext() *symresolve.UnionContext {
	return ctx.currentUnionContext
}

// NewFuncContext creates a function-scoped semantic lowering context.
func NewFuncContext(img *asm.ImageNE, sdb *typeinfo.SymbolDB, res *symresolve.Resolver, fs *typeinfo.Function) *FuncContext {
	return &FuncContext{
		img:   img,
		sdb:   sdb,
		res:   res,
		fs:    fs,
		dsReg: machine.ConstVal(sdb.DGroupFrame),
		csReg: machine.ConstVal(uint(fs.Addr.Seg)),
	}
}

// resolveMachineStorage resolves direct machine memory to a semantic lvalue.
func (ctx *FuncContext) resolveMachineStorage(mem machine.MemoryAccess, width int) (LValue, bool) {
	if mem.Index != nil {
		return nil, false
	}
	// if this isn't a bp+ local var, resolve it as a global
	if _, ok := mem.Base.(*machine.FrameBase); !ok {
		if global, ok := ctx.resolveGlobal(mem); ok {
			if lvalue, ok := ctx.resolveFieldStorage(global.Global, global.FieldOff, width); ok {
				return lvalue, true
			}
			return lvalueForGlobalAccess(global, width), true
		}
		return nil, false
	}

	off := mem.Origin.InstOff
	local, ok := ctx.res.ResolveLocal(ctx.fs, off, mem.Disp)
	if !ok {
		return nil, false
	}
	if lvalue, ok := ctx.resolveFieldStorage(&local.Local, local.FieldOff, width); ok {
		return lvalue, true
	}
	return lvalueForLocalAccess(local, width), true
}

// resolveFieldStorage resolves exact and partial loads of fields within a symbol.
func (ctx *FuncContext) resolveFieldStorage(v typeinfo.Var, fieldOff int, width int) (LValue, bool) {
	if typ := v.VarType(); typeinfo.IsPointer(typ) {
		if fieldOff+width > typ.Bytes() {
			return nil, false
		}
		root := symresolve.SymbolPath(&symresolve.SymbolRoot{Symbol: v})
		if fieldOff == 0 && width == typ.Bytes() {
			return &SymbolRef{Path: root}, true
		}
		part := &Part{
			Base:     &SymbolRef{Path: root},
			ByteOff:  fieldOff,
			Width:    width,
			TypeInfo: intTypeForWidth(width),
		}
		return part, true
	}
	if path, ok := ctx.res.ResolveFieldLoadInContext(v, fieldOff, width, ctx.unionContext()); ok {
		return &SymbolRef{Path: path}, true
	}
	path, partOff, ok := ctx.res.ResolveFieldInContext(v, fieldOff, ctx.unionContext())
	if !ok {
		return nil, false
	}
	if !typeinfo.IsPointer(path.Type()) || partOff+width > path.Type().Bytes() {
		return nil, false
	}
	part := &Part{
		Base:     &SymbolRef{Path: path},
		ByteOff:  partOff,
		Width:    width,
		TypeInfo: intTypeForWidth(width),
	}
	return part, true
}

// resolvePointerFieldLoad resolves a dereference offset through a typed pointer expression.
func (ctx *FuncContext) resolvePointerFieldLoad(pointer Expr, fieldOff int, width int) (LValue, bool) {
	path, ok := symbolPathForExpr(pointer)
	if ok {
		field, ok := ctx.res.ResolveFieldPathLoadInContext(path, fieldOff, width, ctx.unionContext())
		if !ok {
			return nil, false
		}
		return &SymbolRef{Path: field}, true
	}
	return exactExprFieldLoad(pointer, fieldOff, width)
}

// exactExprFieldLoad resolves an exact field access from a typed expression.
func exactExprFieldLoad(base Expr, fieldOff int, width int) (LValue, bool) {
	typ, _ := typeinfo.UnwrapPointer(base.ExprType())
	strct, ok := typ.(*typeinfo.Struct)
	if !ok {
		return nil, false
	}
	var out *FieldAccess
	for _, match := range strct.FieldsContainingOffset(fieldOff) {
		if match.Off != 0 || match.Field.Bitfield != nil || match.Field.Type.Bytes() != width {
			continue
		}
		if out != nil {
			return nil, false
		}
		out = &FieldAccess{Base: base, Field: match.Field}
	}
	if out == nil {
		return nil, false
	}
	return out, true
}

// resolveBitfieldExtract resolves a shifted and masked storage load to a bitfield.
func (ctx *FuncContext) resolveBitfieldExtract(value machine.Value) (Expr, bool) {
	load, bitOff, bitWidth, ok := bitfieldExtract(value)
	if !ok {
		return nil, false
	}
	path, ok := ctx.resolveBitfieldLoad(load.Access, bitOff, bitWidth)
	if !ok {
		return nil, false
	}
	return &SymbolRef{Path: path}, true
}

// resolveBitfieldStore resolves a masked storage write to a whole bitfield assignment.
func (ctx *FuncContext) resolveBitfieldStore(mem machine.MemoryAccess, value machine.Value) (LValue, Expr, bool) {
	load, bitOff, bitWidth, stored, ok := bitfieldStore(mem, value)
	if !ok {
		return nil, nil, false
	}
	path, ok := ctx.resolveBitfieldLoad(load.Access, bitOff, bitWidth)
	if !ok {
		return nil, nil, false
	}
	return &SymbolRef{Path: path}, &Const{TypeInfo: intTypeForWidth(load.Access.Width), U64: uint64(stored)}, true
}

// resolveBitfieldLoad resolves a bitfield path from direct machine storage.
func (ctx *FuncContext) resolveBitfieldLoad(mem machine.MemoryAccess, bitOff int, bitWidth int) (symresolve.SymbolPath, bool) {
	if path, ok := ctx.resolvePointerMemoryPath(mem); ok {
		field, ok := ctx.res.ResolveBitfieldPathLoad(path, mem.Disp, mem.Width, bitOff, bitWidth)
		return field, ok
	}
	if _, ok := mem.Base.(*machine.FrameBase); !ok {
		global, ok := ctx.resolveGlobal(mem)
		if !ok {
			return nil, false
		}
		root := &symresolve.SymbolRoot{Symbol: global.Global}
		path, ok := ctx.res.ResolveBitfieldPathLoadInContext(root, global.FieldOff, mem.Width, bitOff, bitWidth, ctx.unionContext())
		return path, ok
	}

	local, ok := ctx.res.ResolveLocal(ctx.fs, mem.Origin.InstOff, mem.Disp)
	if !ok {
		return nil, false
	}
	root := &symresolve.SymbolRoot{Symbol: &local.Local}
	path, ok := ctx.res.ResolveBitfieldPathLoadInContext(root, local.FieldOff, mem.Width, bitOff, bitWidth, ctx.unionContext())
	return path, ok
}

// resolvePointerMemoryPath returns the symbolic pointer used by indirect memory.
func (ctx *FuncContext) resolvePointerMemoryPath(mem machine.MemoryAccess) (symresolve.SymbolPath, bool) {
	if load, ok := nearPointerMemoryLoad(ctx.dsReg, mem); ok {
		return ctx.resolveMemoryPath(load.Access)
	}
	if load, ok := farPointerMemoryLoad(mem); ok {
		return ctx.resolveMemoryPath(load.Access)
	}
	return nil, false
}

// resolveMemoryPath returns the source-level path for direct local or global storage.
func (ctx *FuncContext) resolveMemoryPath(mem machine.MemoryAccess) (symresolve.SymbolPath, bool) {
	if _, ok := mem.Base.(*machine.FrameBase); !ok {
		global, ok := ctx.resolveGlobal(mem)
		if !ok {
			return nil, false
		}
		root := symresolve.SymbolPath(&symresolve.SymbolRoot{Symbol: global.Global})
		if global.FieldOff == 0 {
			return root, true
		}
		path, off, ok := ctx.res.ResolveFieldInContext(global.Global, global.FieldOff, ctx.unionContext())
		if !ok || off != 0 {
			return nil, false
		}
		return path, true
	}

	local, ok := ctx.res.ResolveLocal(ctx.fs, mem.Origin.InstOff, mem.Disp)
	if !ok {
		return nil, false
	}
	root := symresolve.SymbolPath(&symresolve.SymbolRoot{Symbol: &local.Local})
	if local.FieldOff == 0 {
		return root, true
	}
	path, off, ok := ctx.res.ResolveFieldInContext(&local.Local, local.FieldOff, ctx.unionContext())
	if !ok || off != 0 {
		return nil, false
	}
	return path, true
}

// bitfieldExtract returns the storage load, bit offset, and bit width for a bitfield expression.
func bitfieldExtract(value machine.Value) (*machine.Load, int, int, bool) {
	and, ok := value.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return nil, 0, 0, false
	}
	mask, source, ok := constOperand(and.LHS, and.RHS)
	if !ok {
		return nil, 0, 0, false
	}
	bitWidth, ok := lowBitMaskWidth(mask.Val)
	if !ok {
		return nil, 0, 0, false
	}
	load, bitOff, ok := shiftedLoad(source)
	if !ok {
		return nil, 0, 0, false
	}
	return load, bitOff, bitWidth, true
}

// bitfieldStore returns a whole bitfield write represented as load-and-mask-or-const.
func bitfieldStore(mem machine.MemoryAccess, value machine.Value) (*machine.Load, int, int, uint, bool) {
	source, set := value, uint(0)
	if or, ok := value.(*machine.Binary); ok && or.Op == machine.ValueOpOr {
		setConst, nextSource, ok := constOperand(or.LHS, or.RHS)
		if !ok {
			return nil, 0, 0, 0, false
		}
		set = setConst.Val
		source = nextSource
	}
	and, ok := source.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return nil, 0, 0, 0, false
	}
	keep, keptSource, ok := constOperand(and.LHS, and.RHS)
	if !ok {
		return nil, 0, 0, 0, false
	}
	load, ok := keptSource.(*machine.Load)
	if !ok || !sameStorage(mem, load.Access) {
		return nil, 0, 0, 0, false
	}
	fullMask, ok := bitMask(mem.Width * 8)
	if !ok {
		return nil, 0, 0, 0, false
	}
	changed := (^keep.Val) & fullMask
	bitOff, bitWidth, ok := contiguousMaskRange(changed)
	if !ok || set&^changed != 0 {
		return nil, 0, 0, 0, false
	}
	fieldMask, ok := bitMask(bitWidth)
	if !ok {
		return nil, 0, 0, 0, false
	}
	return load, bitOff, bitWidth, (set >> bitOff) & fieldMask, true
}

// maskedStorageWrite reports whether value preserves bits from the destination storage.
func maskedStorageWrite(mem machine.MemoryAccess, value machine.Value) bool {
	source := value
	if or, ok := value.(*machine.Binary); ok && or.Op == machine.ValueOpOr {
		_, nextSource, ok := constOperand(or.LHS, or.RHS)
		if !ok {
			return false
		}
		source = nextSource
	}
	and, ok := source.(*machine.Binary)
	if !ok || and.Op != machine.ValueOpAnd {
		return false
	}
	_, keptSource, ok := constOperand(and.LHS, and.RHS)
	if !ok {
		return false
	}
	load, ok := keptSource.(*machine.Load)
	return ok && sameStorage(mem, load.Access)
}

// shiftedLoad returns the load and right-shift amount for a bitfield source.
func shiftedLoad(value machine.Value) (*machine.Load, int, bool) {
	shift, ok := value.(*machine.Binary)
	if !ok || shift.Op != machine.ValueOpShr {
		load, ok := value.(*machine.Load)
		return load, 0, ok
	}
	amount, source, ok := constOperand(shift.LHS, shift.RHS)
	if !ok || source != shift.LHS {
		return nil, 0, false
	}
	load, ok := source.(*machine.Load)
	if !ok {
		return nil, 0, false
	}
	return load, int(amount.Val), true
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
func sameStorage(a, b machine.MemoryAccess) bool {
	return a.Disp == b.Disp &&
		a.Width == b.Width &&
		a.Scale == b.Scale &&
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
	case *machine.FrameBase:
		_, ok := b.(*machine.FrameBase)
		return ok
	case *machine.Load:
		bv, ok := b.(*machine.Load)
		return ok && sameStorage(av.Access, bv.Access)
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
		return ok && sameStorage(av.Access, bv.Access)
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
		return &symresolve.SymbolField{Base: base, Field: e.Field}, true
	default:
		return nil, false
	}
}

// resolveGlobal resolves direct machine memory to a global access.
func (ctx *FuncContext) resolveGlobal(mem machine.MemoryAccess) (symresolve.GlobalAccess, bool) {
	if mem.Index != nil {
		return symresolve.GlobalAccess{}, false
	}
	seg, ok := mem.Seg.(*machine.Const)
	if !ok {
		return symresolve.GlobalAccess{}, false
	}
	off := int64(mem.Disp)
	if mem.Base != nil {
		base, ok := mem.Base.(*machine.Const)
		if !ok {
			return symresolve.GlobalAccess{}, false
		}
		off += int64(base.Val)
	}
	if off < 0 || off > int64(^uint32(0)) {
		return symresolve.GlobalAccess{}, false
	}
	return ctx.res.ResolveGlobal(uint16(seg.Val), uint32(off), mem.Width)
}
