package templates

import "github.com/charmbracelet/lipgloss"

// DumpOptions controls how function bytes are dumped.
type DumpOptions struct {
	// FromAddr, if non-zero, restricts output to start at this address.
	// If ToAddr is zero, output stops at the next label boundary (block end).
	FromAddr uint32
	// ToAddr, if non-zero, restricts output to end at this block/label.
	// Non-label addresses are treated as an exclusive cutoff.
	ToAddr uint32

	// ShowColor enables syntax highlighting in dump renderers that mix languages.
	ShowColor bool

	// PreserveJumps keeps direct jump trampoline blocks in rendered function dumps.
	PreserveJumps bool
}

// DumpStyles is the style set consumed by dump templates.
type DumpStyles struct {
	Bytes lipgloss.Style
}

type DumpAsmOptions struct {
	DumpOptions
	RawOperands bool
	ShowBytes   bool
	ShowOffsets bool
}

type DumpCFGOptions struct {
	DumpOptions
}

type DumpEffectsOptions struct {
	DumpOptions
	Config      bool
	ShowAsm     bool
	ShowOffsets bool
	// DiffState emits per-block machine state in/out diffs.
	DiffState bool
}

type DumpSemOptions struct {
	DumpOptions
	ShowAsm     bool
	ShowEffects bool
	ShowOffsets bool
	DiffDir     string
}
