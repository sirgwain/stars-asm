package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// The global naming the listbox window class Stars creates listboxes with,
// the listbox style keeping the height it is given, and the CreateWindow
// argument holding the window style.
const (
	listboxClassGlobal  = "szListbox"
	lbsNoIntegralHeight = 0x0100
	createWindowStyle   = 2
)

// nativeListboxHeightProcessor creates Stars' runtime listboxes with
// LBS_NOINTEGRALHEIGHT for the native compile. Win16 listboxes trimmed
// themselves to a whole number of items only when created, so the height a
// pane later set with SetWindowPos stuck. Win32 listboxes trim themselves on
// every WM_SIZE. The planet pane resizes its listboxes while painting and
// resizes again when the height it reads back is short by a whole item, so
// under Win32 each trim uncovers the pane, which repaints and resizes the
// listbox again, forever. Keeping the exact height matches the Win16 steady
// state.
type nativeListboxHeightProcessor struct{}

// ProcessBlock adds LBS_NOINTEGRALHEIGHT to listboxes created in the block.
func (p *nativeListboxHeightProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	rewriter := &semRewriter{
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			next, changed := w.rewriteCallChildren(call)
			if !createsListbox(next) {
				return next, changed, true
			}
			style, ok := next.Args[createWindowStyle].(*Const)
			if !ok {
				panic(fmt.Sprintf("native-listbox-height: listbox style is not a constant at %v", meta))
			}
			if style.U64&lbsNoIntegralHeight != 0 {
				return next, changed, true
			}
			nextStyle := *style
			nextStyle.U64 |= lbsNoIntegralHeight
			args := append([]Expr(nil), next.Args...)
			args[createWindowStyle] = &nextStyle
			out := *next
			out.Args = args
			return &out, true, true
		},
	}
	effects, changed := rewriter.rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// createsListbox reports whether a call is CreateWindow of the listbox class.
func createsListbox(call *Call) bool {
	if call.Function == nil || call.Function.Name != "CreateWindow" || len(call.Args) <= createWindowStyle {
		return false
	}
	global, ok := globalOf(call.Args[0])
	return ok && global.Name == listboxClassGlobal
}
