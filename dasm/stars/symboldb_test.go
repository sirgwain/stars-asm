package stars

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCreateWindowStyleEnumPreservesStackWidth verifies Win16 constant
// annotation keeps the original ABI width and declared type for
// CreateWindow's 32-bit style parameter.
func TestCreateWindowStyleEnumPreservesStackWidth(t *testing.T) {
	fx := testfixture.Stars(t)
	fn := fx.SDB.GetFunction("CreateWindow")
	if fn == nil {
		t.Fatal("missing CreateWindow")
	}
	if got, want := fn.ParamWords(), 15; got != want {
		t.Fatalf("CreateWindow param words = %d, want %d", got, want)
	}
	style, ok := fn.Params[2].Type.(*typeinfo.Enum)
	if !ok || style.Name != "WindowStyle" {
		t.Fatalf("CreateWindow arg3 type = %T %v, want WindowStyle constants", fn.Params[2].Type, fn.Params[2].Type)
	}
	if got, want := style.String(), "uint32_t"; got != want {
		t.Fatalf("CreateWindow arg3 declared type = %s, want %s", got, want)
	}
	if got, want := fn.Params[2].Type.Bytes(), 4; got != want {
		t.Fatalf("CreateWindow arg3 bytes = %d, want %d", got, want)
	}
	if !typeinfo.IsFarPointer(fn.Params[0].Type) || !typeinfo.IsFarPointer(fn.Params[1].Type) {
		t.Fatalf("CreateWindow string params should remain far pointers")
	}
}
