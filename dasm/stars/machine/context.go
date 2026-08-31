package machine

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// FuncContext is sem's analysis context. It carries only symbol/image facts
// needed for semantic extraction and intentionally excludes asm pipeline state.
type FuncContext struct {
	img   *asm.ImageNE
	sdb   *typeinfo.SymbolDB
	res   *symresolve.Resolver
	fs    *typeinfo.Function
	ssReg Value
	dsReg Value
	csReg Value
}

// NewFuncContext is transitional; prefer ProgramContext.NewSemFuncContext
// at real pipeline entry points so shared services have one owner.
func NewFuncContext(img *asm.ImageNE, sdb *typeinfo.SymbolDB, res *symresolve.Resolver, fs *typeinfo.Function) *FuncContext {
	ctx := &FuncContext{
		img:   img,
		sdb:   sdb,
		res:   res,
		fs:    fs,
		ssReg: RegVal(asm.RegSS),
		dsReg: RegVal(asm.RegDS),
		csReg: RegVal(asm.RegCS),
	}
	return ctx
}

func (ctx *FuncContext) ReturnsValue() bool {
	return ctx.fs.Ret.Kind() != typeinfo.KVoid
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
