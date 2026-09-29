package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// messageCall describes a call that sends a constant message: the enum that
// names the message and the argument index of the message identifier.
type messageCall struct {
	enum  *typeinfo.Enum
	index int
	value int
}

// messageCallInfo recognizes SendMessage, PostMessage and SendDlgItemMessage
// calls with a constant message, named as messageEnum names it.
func (ctx *FuncContext) messageCallInfo(fn *typeinfo.Function, args []machine.Value) (messageCall, bool) {
	index, class, ok := ctx.messageTarget(fn, args)
	if !ok {
		return messageCall{}, false
	}
	value, ok := args[index].(*machine.Const)
	if !ok {
		return messageCall{}, false
	}
	return messageCall{enum: ctx.messageEnum(class, int(value.Val)), index: index, value: int(value.Val)}, true
}

// messageTarget returns the argument index of the message a SendMessage,
// PostMessage or SendDlgItemMessage call sends, and the window class of its
// target when known.
func (ctx *FuncContext) messageTarget(fn *typeinfo.Function, args []machine.Value) (int, *typeinfo.WindowClass, bool) {
	if fn == nil {
		return 0, nil, false
	}
	switch fn.Name {
	case "SendMessage", "PostMessage":
		if len(args) < 4 {
			return 0, nil, false
		}
		return 1, ctx.windowClass(args[0]), true
	case "SendDlgItemMessage":
		if len(args) < 5 {
			return 0, nil, false
		}
		return 2, ctx.dialogControlClass(args[0], args[1]), true
	}
	return 0, nil, false
}

// messageEnum returns the enum naming a message sent to a window of class.
// Messages in the WM_USER range sent to a control of a known window class are
// named by that class's message enum, since listbox, combobox and edit
// messages share those numbers; for a target of unknown class, the function's
// sent_messages rule names them.
func (ctx *FuncContext) messageEnum(class *typeinfo.WindowClass, value int) *typeinfo.Enum {
	wm := ctx.sdb.GetEnum(typeinfo.MessageEnumName)
	if value < wm.GetValue("WM_USER").Value {
		return wm
	}
	if class != nil {
		return class.Messages
	}
	if enum := ctx.sdb.SentMessageEnum(ctx.fs.Name, value); enum != nil {
		return enum
	}
	return wm
}

// messageMerge converts a message merged from constants, as a call sending
// cond ? LB_GETTEXT : CB_GETLBTEXT does, naming each arm's message.
func (c *machineConverter) messageMerge(fn *typeinfo.Function, args []machine.Value, index int, expected typeinfo.Type) (Expr, bool) {
	msgIndex, class, ok := c.ctx.messageTarget(fn, args)
	if !ok || index != msgIndex {
		return nil, false
	}
	phi, ok := args[index].(*machine.PhiValue)
	if !ok {
		return nil, false
	}
	arms := make([]MergeArm, 0, len(phi.Arms))
	for _, arm := range phi.Arms {
		if arm.Block == nil {
			continue
		}
		value, ok := arm.Value.(*machine.Const)
		if !ok {
			return nil, false
		}
		arms = append(arms, MergeArm{Block: arm.Block.ID, Value: &Const{TypeInfo: c.ctx.messageEnum(class, int(value.Val)), U64: uint64(value.Val)}})
	}
	return &Merge{TypeInfo: expected, Join: phi.Join, Arms: arms}, true
}

// messageCallArgumentType returns the message-specific whole-argument type
// for the wParam or lParam of a call sending a constant message.
func (ctx *FuncContext) messageCallArgumentType(fn *typeinfo.Function, args []machine.Value, argIndex int) typeinfo.Type {
	call, ok := ctx.messageCallInfo(fn, args)
	if !ok {
		return nil
	}
	message := ctx.sdb.GetMessage(call.enum, call.value)
	if message == nil {
		return nil
	}
	switch argIndex {
	case call.index + 1:
		if message.WParam != nil {
			return message.WParam.Whole.Type
		}
	case call.index + 2:
		if message.LParam != nil {
			return message.LParam.Whole.Type
		}
	}
	return nil
}
