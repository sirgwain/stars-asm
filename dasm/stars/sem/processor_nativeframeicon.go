package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// The frame window class and the icon resource Stars draws for it while
// minimized, quoted as a C string literal like the literals read from the
// image.
const (
	frameClassGlobal  = "szFrame"
	frameIconResource = `"StarsIco"`
)

// nativeFrameIconProcessor gives the frame window class the Stars icon for
// the native compile. Win16 Stars registers the frame with no class icon and
// supplies hiconStars itself when minimized, through WM_QUERYDRAGICON and by
// painting the iconic window. Win32 never asks for that icon: the taskbar and
// title bar use the class icon, falling back to the exe's first icon group,
// which PE resource sorting makes Bang1Ico. The processor rewrites
// wc.hIcon = 0 to wc.hIcon = LoadIcon(hInst, "StarsIco") in the block that
// registers the class named szFrame. The class is registered before
// FCreateStuff loads hiconStars, so it loads the resource itself; Win32
// shares the handle between both loads.
type nativeFrameIconProcessor struct {
	ctx *FuncContext
}

// ProcessBlock sets the class icon of a frame window class registered in the
// block.
func (p *nativeFrameIconProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	if !registersFrameClass(b) {
		return b, false
	}
	changed := false
	effects := append([]Effect(nil), b.Effects...)
	for i, effect := range effects {
		assign, ok := effect.(*Assign)
		if !ok {
			continue
		}
		if _, ok := fieldOwner(assign.Dst, "hIcon"); !ok {
			continue
		}
		if c, ok := assign.Src.(*Const); !ok || c.U64 != 0 {
			continue
		}
		next := *assign
		next.Src = p.loadFrameIcon()
		effects[i] = &next
		changed = true
	}
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// loadFrameIcon returns LoadIcon(hInst, "StarsIco").
func (p *nativeFrameIconProcessor) loadFrameIcon() *Call {
	loadIcon := p.ctx.sdb.GetFunction("LoadIcon")
	if loadIcon == nil {
		panic("native-frame-icon: LoadIcon is not in the symbol db")
	}
	hInst := p.ctx.sdb.GetGlobal("hInst")
	if hInst == nil {
		panic("native-frame-icon: global hInst is not in the symbol db")
	}
	name := &StringLiteral{TypeInfo: loadIcon.Params[1].Type, Text: frameIconResource}
	return &Call{Function: loadIcon, Args: []Expr{&Global{GlobalVar: hInst}, name}}
}

// registersFrameClass reports whether the block stores szFrame to a window
// class's lpszClassName.
func registersFrameClass(b Block) bool {
	for _, effect := range b.Effects {
		assign, ok := effect.(*Assign)
		if !ok {
			continue
		}
		if _, ok := fieldOwner(assign.Dst, "lpszClassName"); !ok {
			continue
		}
		if global, ok := globalOf(assign.Src); ok && global.Name == frameClassGlobal {
			return true
		}
	}
	return false
}

// globalOf returns the global variable expr names, as a global expression or
// a resolved symbol root.
func globalOf(expr Expr) (*typeinfo.GlobalVar, bool) {
	switch e := expr.(type) {
	case *Global:
		return e.GlobalVar, true
	case *SymbolRef:
		root, ok := e.Path.(*symresolve.SymbolRoot)
		if !ok {
			return nil, false
		}
		global, ok := root.Symbol.(*typeinfo.GlobalVar)
		return global, ok
	}
	return nil, false
}
