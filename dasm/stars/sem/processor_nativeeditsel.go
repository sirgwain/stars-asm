package sem

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// nativeEditSelProcessor repacks the EM_SETSEL messages Stars sends for the
// native compile. Win16 took the selection packed into lParam as
// MAKELONG(start, end), with wParam a no-scroll flag; Win32 takes the start
// in wParam and the end in lParam. The end keeps its sign, since -1 selects
// to the end of the text:
//
//	SendMessage(hwnd, EM_SETSEL, 0, lSel)
//	    → SendMessage(hwnd, EM_SETSEL, LOWORD(lSel), (int16_t)HIWORD(lSel))
//	SendDlgItemMessage(hwnd, id, EM_SETSEL, 0, MAKELONG(0, -1))
//	    → SendDlgItemMessage(hwnd, id, EM_SETSEL, 0, -1)
type nativeEditSelProcessor struct {
	ctx *FuncContext
}

// ProcessBlock repacks the EM_SETSEL sends in one block.
func (p *nativeEditSelProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	edit := p.ctx.sdb.GetWindowClass("edit")
	if edit == nil {
		panic("native-edit-sel: the edit window class is not configured")
	}
	emSetSel := edit.Messages.GetValue("EM_SETSEL").Value
	rewriter := &semRewriter{
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			next, changed := w.rewriteCallChildren(call)
			index, ok := sentMessageIndex(next)
			if !ok {
				return next, changed, true
			}
			msg, ok := next.Args[index].(*Const)
			if !ok {
				return next, changed, true
			}
			enum, ok := msg.TypeInfo.(*typeinfo.Enum)
			if !ok || enum.Name != edit.Messages.Name || int(msg.U64) != emSetSel {
				return next, changed, true
			}
			wParam, lParam := next.Args[index+1], next.Args[index+2]
			start, end, ok := splitSelection(lParam, wParam.ExprType(), lParam.ExprType())
			if !ok {
				panic(fmt.Sprintf("native-edit-sel: EM_SETSEL selection %T is neither a constant nor a variable at %v", lParam, meta))
			}
			args := append([]Expr(nil), next.Args...)
			args[index+1], args[index+2] = start, end
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

// sentMessageIndex returns the argument index of the message a
// SendMessage, PostMessage or SendDlgItemMessage call sends.
func sentMessageIndex(call *Call) (int, bool) {
	if call.Function == nil {
		return 0, false
	}
	index := 0
	switch call.Function.Name {
	case "SendMessage", "PostMessage":
		index = 1
	case "SendDlgItemMessage":
		index = 2
	default:
		return 0, false
	}
	return index, len(call.Args) > index+2
}

// splitSelection splits a Win16 MAKELONG(start, end) selection into the
// Win32 start and sign-extended end, typed as the wParam and lParam they
// are passed as. A constant splits into constants; a variable, which is
// read twice, into its LOWORD and HIWORD.
func splitSelection(sel Expr, wParam, lParam typeinfo.Type) (Expr, Expr, bool) {
	if cast, ok := sel.(*Cast); ok {
		sel = cast.Value
	}
	switch s := sel.(type) {
	case *Const:
		start := &Const{TypeInfo: wParam, U64: s.U64 & 0xffff}
		end := &Const{TypeInfo: lParam, U64: uint64(int64(int16(s.U64>>16))) & widthMask(lParam.Bytes())}
		return start, end, true
	case *Local, *Temp, *SymbolRef:
		start := &Word{Parent: s, Part: machine.WordLow}
		end := castTo(&Word{Parent: s, Part: machine.WordHigh}, typeinfo.I16)
		return start, end, true
	}
	return nil, nil, false
}

// widthMask returns the mask of a value bytes wide.
func widthMask(bytes int) uint64 {
	if bytes >= 8 {
		return ^uint64(0)
	}
	return 1<<(8*bytes) - 1
}
