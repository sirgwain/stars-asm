package machine_test

import (
	"slices"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/testfixture"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// TestWriteSummaries verifies what calls of program and Win32 functions are
// summarized to store to: named globals, nothing, or anything.
func TestWriteSummaries(t *testing.T) {
	fx := testfixture.Stars(t)
	s := machine.NewWriteSummaries(fx.Image, fx.SDB, symresolve.NewResolver(fx.Image, fx.SDB))
	names := func(w machine.Writes) []string {
		var out []string
		for _, g := range w.Globals {
			out = append(out, g.Name)
		}
		slices.Sort(out)
		return out
	}

	for _, tc := range []struct {
		name    string
		any     bool
		globals []string
	}{
		// Random stores only its seeds, through the pure long helpers.
		{name: "Random", globals: []string{"lRandSeed1", "lRandSeed2"}},
		// Declared to write nothing.
		{name: "GetDlgItem"},
		// A Win32 function not declared so.
		{name: "SendMessage", any: true},
		// Stores through a pointer into its buffer.
		{name: "PszGetCompressedString", any: true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			fn := fx.SDB.GetFunction(tc.name)
			w := s.Of(fn)
			if w.Any != tc.any || !slices.Equal(names(w), tc.globals) {
				t.Fatalf("Of(%s) = any %v globals %v, want any %v globals %v", tc.name, w.Any, names(w), tc.any, tc.globals)
			}
		})
	}
	if w := s.Of(&typeinfo.Function{Name: "WNDPROC"}); !w.Any {
		t.Fatalf("a signature without a body writes %+v, want anything", w)
	}
}
