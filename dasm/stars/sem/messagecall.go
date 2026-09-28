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
// calls with a constant message. Messages in the WM_USER range sent to a
// control of a known window class are named by that class's message enum,
// since listbox, combobox and edit messages share those numbers.
func (ctx *FuncContext) messageCallInfo(fn *typeinfo.Function, args []machine.Value) (messageCall, bool) {
	if fn == nil {
		return messageCall{}, false
	}
	var index int
	var class *typeinfo.WindowClass
	switch fn.Name {
	case "SendMessage", "PostMessage":
		if len(args) < 4 {
			return messageCall{}, false
		}
		index, class = 1, ctx.windowClass(args[0])
	case "SendDlgItemMessage":
		if len(args) < 5 {
			return messageCall{}, false
		}
		index, class = 2, ctx.dialogControlClass(args[0], args[1])
	default:
		return messageCall{}, false
	}
	value, ok := args[index].(*machine.Const)
	if !ok {
		return messageCall{}, false
	}
	wm := ctx.sdb.GetEnum(typeinfo.MessageEnumName)
	enum := wm
	if class != nil && int(value.Val) >= wm.GetValue("WM_USER").Value {
		enum = class.Messages
	}
	return messageCall{enum: enum, index: index, value: int(value.Val)}, true
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
