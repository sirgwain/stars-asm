package stars

import (
	"testing"

	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestCreateWindowStyleEnumPreservesStackWidth verifies enum annotation keeps
// the original Win16 ABI width for CreateWindow's 32-bit style parameter.
func TestCreateWindowStyleEnumPreservesStackWidth(t *testing.T) {
	fx := testfixture.Stars(t)
	fn := fx.SDB.GetFunction("CreateWindow")
	if fn == nil {
		t.Fatal("missing CreateWindow")
	}
	if got, want := fn.ParamWords(), 15; got != want {
		t.Fatalf("CreateWindow param words = %d, want %d", got, want)
	}
	if got, want := fn.Params[2].Type.String(), "WindowStyle"; got != want {
		t.Fatalf("CreateWindow arg3 type = %s, want %s", got, want)
	}
	if got, want := fn.Params[2].Type.Bytes(), 4; got != want {
		t.Fatalf("CreateWindow arg3 bytes = %d, want %d", got, want)
	}
	if !typeinfo.IsFarPointer(fn.Params[0].Type) || !typeinfo.IsFarPointer(fn.Params[1].Type) {
		t.Fatalf("CreateWindow string params should remain far pointers")
	}
}
