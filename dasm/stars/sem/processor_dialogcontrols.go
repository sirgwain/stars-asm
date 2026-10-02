package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// dialogControlsProcessor names control ids by the control enum of the dialog
// they belong to. Dialogs reuse the same numbers for different controls, so a
// ControlId constant passed with a dialog window, or used in a function
// configured to handle a dialog, takes that dialog's own name when it has one.
type dialogControlsProcessor struct {
	ctx *FuncContext
	// controls is the control enum of the dialog the function handles, or
	// nil when it handles none.
	controls *typeinfo.Enum
	resolved bool
}

// ProcessBlock renames the control ids of one semantic block.
func (p *dialogControlsProcessor) ProcessBlock(result *Result, f Func, b Block) (Block, bool) {
	if !p.resolved {
		if dialog, ok := p.ctx.sdb.FunctionDialog(p.ctx.fs.Name); ok {
			p.controls = p.ctx.sdb.DialogControls[dialog]
		}
		p.resolved = true
	}
	effects, changed := p.rewriter(p.controls).rewriteEffects(b.Effects)
	if !changed {
		return b, false
	}
	b.Effects = effects
	return b, true
}

// rewriter returns a rewriter naming ControlId constants by controls, and the
// ids passed to a call with a dialog window by that dialog's controls.
func (p *dialogControlsProcessor) rewriter(controls *typeinfo.Enum) *semRewriter {
	return &semRewriter{
		effect: func(w *semRewriter, effect Effect) (Effect, bool, bool) {
			jump, ok := effect.(*TableJump)
			if !ok || controls == nil || jump.CaseEnum == controls {
				return effect, false, false
			}
			if enumType, _ := jump.SwitchedValue(); enumType == nil || enumType.Name != typeinfo.ControlEnumName {
				return effect, false, false
			}
			index, _ := w.rewriteExpr(jump.Index)
			next := *jump
			next.Index = index
			next.CaseEnum = controls
			return &next, true, true
		},
		expr: func(w *semRewriter, expr Expr) (Expr, bool, bool) {
			c, ok := expr.(*Const)
			if !ok {
				return expr, false, false
			}
			next, changed := renameControlConst(c, controls)
			return next, changed, true
		},
		call: func(w *semRewriter, call *Call, meta machine.Meta) (*Call, bool, bool) {
			dialog := p.callDialogControls(call)
			if dialog == nil {
				return call, false, false
			}
			// The call's ids belong to the dialog window it is passed.
			next, changed := p.rewriter(dialog).rewriteCallChildren(call)
			return next, changed, true
		},
	}
}

// callDialogControls returns the control enum of the dialog window passed as
// the first argument of a call that also takes control ids, such as
// GetDlgItem or CheckRadioButton, or nil when the window is not a known
// dialog.
func (p *dialogControlsProcessor) callDialogControls(call *Call) *typeinfo.Enum {
	if call.Function == nil || len(call.Params) == 0 || len(call.Args) == 0 {
		return nil
	}
	if named, ok := call.Params[0].Type.(*typeinfo.Primitive); !ok || named.Name != "HWND" {
		return nil
	}
	var rule *typeinfo.WindowRule
	switch hwnd := call.Args[0].(type) {
	case *Global:
		rule = p.ctx.sdb.GlobalWindow(hwnd.Name)
	case *Local:
		rule = p.ctx.sdb.FunctionVarWindow(p.ctx.fs.Name, hwnd.Name, p.ctx.isParam(&hwnd.FunctionVar))
	}
	if rule == nil || !rule.HasDialog {
		return nil
	}
	return p.ctx.sdb.DialogControls[rule.Dialog]
}

// renameControlConst retypes a ControlId constant to controls when controls
// names its value.
func renameControlConst(c *Const, controls *typeinfo.Enum) (Expr, bool) {
	enumType, ok := c.TypeInfo.(*typeinfo.Enum)
	if controls == nil || !ok || enumType.Name != typeinfo.ControlEnumName {
		return c, false
	}
	for _, member := range controls.Values {
		if member.Value == int(c.U64) {
			next := *c
			next.TypeInfo = typeinfo.EnumWithStorageSize(controls, enumType)
			return &next, true
		}
	}
	return c, false
}
