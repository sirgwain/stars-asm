package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type symbolResolver struct {
	*FuncContext
}

func newSymbolResolver(ctx *FuncContext) *symbolResolver {
	return &symbolResolver{FuncContext: ctx}
}

// symbolFromValueTyped resolves symbols from values based on their type
// i.e. (0x59a2 + loword((0xc0 * load([bp+iplr]) for PLAYER* will resolve to rgplr
func (sr *symbolResolver) symbolFromValueTyped(value machine.Value, expected typeinfo.Type) (symresolve.SymbolPath, bool) {
	ds := sr.segFromRegister(asm.RegDS)
	switch v := value.(type) {

	case *machine.StackWords:
		if sym, ok := sr.symbolFromFarPointerWords(v, expected); ok {
			return sym, true
		}
		// StackWords stores the high word first and low word second.
		// Combine two constant 16-bit words into one 32-bit value.
		// looks like
		// words(0x0, 0x61) for a 32 bit value
		// or a wide arg
		// words(load(ds:[0x22bc]), load(ds:[0x22ba])) typ=COLORREF -> crButtonFace

		high, highOk := sr.symbolFromValueTyped(v.Words[0], expected)
		low, lowOk := sr.symbolFromValueTyped(v.Words[1], expected)

		if highOk && lowOk {
			cHigh, highConst := high.(*symresolve.SymbolConst)
			cLow, lowConst := low.(*symresolve.SymbolConst)
			if highConst && lowConst {
				val := ((cHigh.Const.Val & 0xffff) << 16) |
					(cLow.Const.Val & 0xffff)

				return &symresolve.SymbolConst{
					Const: machine.ConstVal(val),
					Typ:   expected,
				}, true
			}

			// check for shared global/local
			if symLow, ok := low.(*symresolve.SymbolOffset); ok {
				if symHigh, ok := high.(*symresolve.SymbolOffset); ok &&
					symresolve.Equals(symLow.Base, symHigh.Base) &&
					symHigh.Offset-symLow.Offset == 2 {
					return low, true
				}
			}
		}

	case *machine.Const:
		// A typed pointer constant may itself be a resolvable address.
		if typeinfo.IsNearPointer(expected) {
			if g, ok := sr.globalSymbol(ds, uint32(v.Val), 2); ok {
				return g, true
			}
		}
		return &symresolve.SymbolConst{Const: v, Typ: expected}, true
	case *machine.Load:
		if addr, ok := sr.addressFromMemory(v.Addr, expected); ok {
			if path, ok := addr.path(); ok {
				return path, true
			}
		}
		return sr.symbolFromValue(v)
	case *machine.Address:
		return sr.symbolFromValue(v)
	case *machine.Binary:
		if typeinfo.IsNearPointer(expected) {
			// A resolved pointer-valued term is authoritative. Constants beside
			// it are byte displacements, even when their numeric value happens
			// to fall inside a DGROUP global.
			if path, ok := sr.decomposePointerBase(2, v, 0); ok {
				return path, true
			}
			return sr.decompose(ds, 2, v, 0)
		}
	}

	return nil, false
}

func (sr *symbolResolver) symbolFromValue(value machine.Value) (symresolve.SymbolPath, bool) {
	switch v := value.(type) {

	case *machine.Load:
		base, ok := sr.memoryPath(v.Addr)
		if !ok {
			return nil, false
		}

		return base, true
	case *machine.Address:
		return sr.symbolFromAddressAddress(v.Addr)
	}
	return nil, false
}

// memoryPath resolves a machine memory access through the normalized address interface.
func (sr *symbolResolver) memoryPath(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {
	addr, ok := sr.addressFromMemory(mem, nil)
	if !ok {
		return nil, false
	}
	if path, ok := addr.path(); ok {
		return path, true
	}
	if !addr.deref {
		return nil, false
	}
	converter := &machineConverter{ctx: sr.FuncContext}
	semantic, ok := converter.semanticResolvedAddress(addr)
	if !ok {
		return nil, false
	}
	lvalue, ok := converter.consumeAddress(semantic, mem.Width)
	if !ok {
		return nil, false
	}
	return symbolPathForExpr(lvalue)
}

// exactMemoryPath resolves a memory access to its most specific typed path,
// including union selections.
func (sr *symbolResolver) exactMemoryPath(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {
	// The exact-path cases below consume Base and Disp only. A separate
	// machine index must remain on the normalized address projection path;
	// otherwise resolving the BP-relative root would silently discard it.
	if mem.Index != nil {
		return nil, false
	}

	if _, ok := mem.Base.(*machine.FrameBase); ok {
		// local bp+var
		l, ok := sr.res.ResolveLocal(sr.fs, sr.memoryInstOff(mem), mem.Disp)
		if !ok {
			return sr.symbolFromScratchMemoryAddress(mem, nil)
		}

		return sr.symbolFromVarAccess(&l.Local, l.FieldOff, mem.Width)
	}

	if ptr, ok := mem.Seg.(*machine.FarPointer); ok {
		// farseg(load(dword [bp+0x6])):[faroff(load(dword [bp+0x6]))+0x64]
		return sr.symbolFromFarPointer(ptr, mem.Base, mem.Disp, mem.Width)
	}

	if sym, ok := sr.symbolFromNativePointerMemory(mem); ok {
		return sym, true
	}
	if global, ok := sr.globalAccessFromMemory(mem); ok {
		return sr.symbolFromVarAccess(global.Global, global.FieldOff, mem.Width)
	}

	segReg, ok := mem.Seg.(*machine.Reg)
	if !ok {
		return nil, false
	}

	segNum := sr.segFromRegister(segReg.Val)
	if segNum != 0 {
		if mem.Base == nil {
			// some globals come as ds:[0x1234] where 0x1234 is the Disp
			// direct MOV ax, [0x1234] is like this
			if segReg.Val == asm.RegDS {
				if literal, ok := sr.res.ResolveLiteral(segNum, uint32(mem.Disp)); ok {
					return &symresolve.SymbolLiteral{Literal: literal, Typ: typeinfo.LpStr}, true
				}
			}
		}
		switch v := mem.Base.(type) {
		case *machine.Const:
			// some globals come as ds:[0x1234] or ds:[0x1234+0x2] where 0x1234 is a Const
			// copy coalescing is like this, as well as some [sel+0x2] style loads
			globalOff := uint32(v.Val + uint(mem.Disp))
			if segReg.Val == asm.RegDS {
				if literal, ok := sr.res.ResolveLiteral(segNum, globalOff); ok {
					return &symresolve.SymbolLiteral{Literal: literal, Typ: typeinfo.LpStr}, true
				}
			}
		case *machine.Load:
			// handle ds:[load([bp-pl])] or ds:[load([bp-pl])+0x2]
			if segReg.Val == asm.RegDS {
				if sym, ok := sr.symbolFromValue(mem.Base); ok {
					if !typeinfo.IsPointer(sym.Type()) {
						return nil, false
					}
					if path, ok := sr.symbolFromResolvedAccess(sym, mem.Disp, mem.Width); ok {
						return path, true
					}
					deref := &symresolve.SymbolDeref{
						Base: sym,
					}
					return &symresolve.SymbolOffset{
						Base:   deref,
						Offset: mem.Disp,
						Result: deref.Type(),
					}, true

				}
			}
		case *machine.Binary:
			return sr.decompose(segNum, mem.Width, v, mem.Disp)
		}

	}

	return nil, false
}

// memoryInstOff returns the instruction offset governing a memory access.
func (sr *symbolResolver) memoryInstOff(mem machine.MemoryAddress) uint32 {
	if mem.Origin.InstOff != 0 {
		return mem.Origin.InstOff
	}
	return sr.currentInstOff
}

// symbolFromAddressAddress resolves an address expression without treating it as
// a load from that address.
func (sr *symbolResolver) symbolFromAddressAddress(mem machine.MemoryAddress) (symresolve.SymbolPath, bool) {
	if _, ok := mem.Base.(*machine.FrameBase); ok {
		l, ok := sr.res.ResolveLocal(sr.fs, sr.memoryInstOff(mem), mem.Disp)
		if !ok {
			return sr.symbolFromScratchMemoryAddress(mem, nil)
		}
		return sr.symbolFromAddressedVar(&l.Local, l.FieldOff)
	}

	g, ok := sr.globalAccessFromMemory(mem)
	if !ok {
		return nil, false
	}
	return sr.symbolFromAddressedVar(g.Global, g.FieldOff)
}
