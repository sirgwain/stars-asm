package typeinfo

import (
	"cmp"
	"fmt"
)

// Addr is a unified “address key” for 16-bit CodeView addresses.
// Off is uint32 so this can also support 32-bit symbol records later.
type Addr struct {
	Seg uint16
	Off uint32
}

func (a Addr) String() string { return fmt.Sprintf("%04x:%04x", a.Seg, uint16(a.Off)) }

func CompareAddr(a, b Addr) int {
	if c := cmp.Compare(a.Seg, b.Seg); c != 0 {
		return c
	}
	return cmp.Compare(a.Off, b.Off)
}
