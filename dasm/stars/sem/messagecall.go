package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// messageCallArgumentType returns the message-specific whole-argument type for
// a SendMessage call whose message identifier is constant.
func messageCallArgumentType(sdb *typeinfo.SymbolDB, fn *typeinfo.Function, args []machine.Value, argIndex int) typeinfo.Type {
	if fn == nil || fn.Name != "SendMessage" || len(args) <= 1 {
		return nil
	}
	messageValue, ok := args[1].(*machine.Const)
	if !ok {
		return nil
	}
	message := sdb.GetMessage(int(messageValue.Val))
	if message == nil {
		return nil
	}
	switch argIndex {
	case 2:
		if message.WParam != nil {
			return message.WParam.Whole
		}
	case 3:
		if message.LParam != nil {
			return message.LParam.Whole
		}
	}
	return nil
}
