package machine

import (
	"slices"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// Writes describes the memory a call may store to outside its own stack
// frame, including through the functions it calls.
type Writes struct {
	// Any is set when the call may store to memory it does not name: through
	// a pointer, through a function pointer or an unknown instruction, or in a
	// Win32 or runtime function not declared to write nothing.
	Any bool
	// Globals are the globals the call stores to by address.
	Globals []*typeinfo.GlobalVar
}

// None reports whether the call stores to no memory outside its frame.
func (w Writes) None() bool {
	return !w.Any && len(w.Globals) == 0
}

// Union adds the writes of other to w.
func (w *Writes) Union(other Writes) {
	w.Any = w.Any || other.Any
	for _, g := range other.Globals {
		w.addGlobal(g)
	}
}

// addGlobal records a store to g.
func (w *Writes) addGlobal(g *typeinfo.GlobalVar) {
	if !slices.Contains(w.Globals, g) {
		w.Globals = append(w.Globals, g)
	}
}

// WriteSummaries computes, on demand, the Writes of each function in the
// program from its machine effects, and remembers them.
type WriteSummaries struct {
	img    *asm.ImageNE
	sdb    *typeinfo.SymbolDB
	res    symbolResolver
	known  map[*typeinfo.Function]Writes
	active map[*typeinfo.Function]bool
}

// NewWriteSummaries creates the write summaries of the program in img.
func NewWriteSummaries(img *asm.ImageNE, sdb *typeinfo.SymbolDB, res symbolResolver) *WriteSummaries {
	return &WriteSummaries{
		img:    img,
		sdb:    sdb,
		res:    res,
		known:  map[*typeinfo.Function]Writes{},
		active: map[*typeinfo.Function]bool{},
	}
}

// Of returns what a call of fn may store to. A function declared to write
// nothing writes nothing; any other function of the program is summarized
// from its stores and calls, and a Win32 function, or a signature that is
// not a function's body, may write anything. A function reached again
// while it is being summarized, through recursion, may write anything.
func (s *WriteSummaries) Of(fn *typeinfo.Function) Writes {
	if fn.WritesNone {
		return Writes{}
	}
	if fn.IsOverride() || s.sdb.GetFunctionByAddr(fn.Addr) != fn {
		return Writes{Any: true}
	}
	if w, ok := s.known[fn]; ok {
		return w
	}
	if s.active[fn] {
		return Writes{Any: true}
	}
	s.active[fn] = true
	w := s.summarize(fn)
	delete(s.active, fn)
	s.known[fn] = w
	return w
}

// summarize derives the writes of a function of the program from its
// machine effects.
func (s *WriteSummaries) summarize(fn *typeinfo.Function) Writes {
	decoded, err := asm.DecodeFunc(asm.NewFuncContext(s.img, fn))
	if err != nil {
		return Writes{Any: true}
	}
	ctx := NewFuncContext(s.img, s.sdb, s.res, fn)
	cfg, err := BuildCFG(ctx, decoded.Instrs, ctx.ReturnsValue(), CFGOptions{CollapseJumps: true})
	if err != nil {
		return Writes{Any: true}
	}
	effects := Extract(ctx, cfg, ExtractOptions{})
	var w Writes
	for _, block := range effects.Blocks {
		for _, effect := range block.Effects {
			switch e := effect.(type) {
			case StoreEffect:
				s.addStore(ctx, &w, e.Addr)
			case CopyEffect:
				addr, ok := e.Dst.(*Address)
				if !ok {
					w.Any = true
					continue
				}
				s.addStore(ctx, &w, addr.Addr)
			case CallEffect:
				if e.Target == nil {
					w.Any = true
					continue
				}
				w.Union(s.Of(e.Target))
			case UnknownEffect:
				w.Any = true
			}
			if w.Any {
				return w
			}
		}
	}
	return w
}

// addStore records a store to addr in w: nothing for the function's own
// frame, the global for a DS address, and any memory otherwise.
func (s *WriteSummaries) addStore(ctx *FuncContext, w *Writes, addr MemoryAddress) {
	if _, frame := addr.Base.(*FrameBase); frame {
		return
	}
	seg, ok := addr.Seg.(*Reg)
	if !ok || seg.Val != asm.RegDS || addr.Base != nil {
		w.Any = true
		return
	}
	g, _, ok := s.sdb.GetGlobalContaining(typeinfo.Addr{Seg: ctx.segFromRegister(asm.RegDS), Off: uint32(addr.Disp)})
	if !ok || addr.Index != nil && !globalHoldsIndexed(g, addr) {
		w.Any = true
		return
	}
	w.addGlobal(g)
}

// globalHoldsIndexed reports whether an indexed store into g stays within
// it: g is an array, which the index selects an element of.
func globalHoldsIndexed(g *typeinfo.GlobalVar, addr MemoryAddress) bool {
	_, array := g.Type.(*typeinfo.Array)
	return array && addr.Disp == int(g.Addr.Off)
}
