package machine

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// EffectKind classifies raw machine/semantic effects emitted by instruction
// transfer. Effects are intentionally below sem.Event: they preserve addresses,
// values, and predicates without choosing final C lvalues/expressions.
type EffectKind uint8

const (
	EffectUnknown EffectKind = iota
	EffectStore
	EffectCopy
	EffectCall
	EffectBranch
	EffectJump
	EffectReturn
)

// Effect is the common interface for all raw effects.
type Effect interface {
	Kind() EffectKind
	EffectMeta() Meta
}

// Meta identifies where an effect came from.
type Meta struct {
	BlockID BlockID
	InstOff uint32
}

// StoreEffect records a memory write to a normalized machine address.
// Later lowering decides whether this is a local/global/field/index/bitfield.
type StoreEffect struct {
	MetaInfo Meta
	Addr     MemoryAccess
	Src      Value
	Width    int
}

func (e StoreEffect) Kind() EffectKind { return EffectStore }
func (e StoreEffect) EffectMeta() Meta { return e.MetaInfo }

type CopyEffect struct {
	MetaInfo Meta
	Dst      Value
	Src      Value
	Width    int
}

func (e CopyEffect) Kind() EffectKind { return EffectCopy }
func (e CopyEffect) EffectMeta() Meta { return e.MetaInfo }

// CallEffect records a CALLF and its decoded argument values. The Result value
// is also written into the ABI return registers by transfer.
type CallEffect struct {
	MetaInfo     Meta
	Target       *typeinfo.Function
	MemoryAccess MemoryAccess // for CALLF [bx+6] style func pointers
	Args         []Value
	Result       Value
}

func (e CallEffect) Kind() EffectKind { return EffectCall }
func (e CallEffect) EffectMeta() Meta { return e.MetaInfo }

// BranchEffect records a conditional terminator derived from machine flags.
type BranchEffect struct {
	MetaInfo   Meta
	Predicate  *PredicateValue
	TrueBlock  BlockID
	FalseBlock BlockID
}

func (e BranchEffect) Kind() EffectKind { return EffectBranch }
func (e BranchEffect) EffectMeta() Meta { return e.MetaInfo }

// JumpEffect records an unconditional direct jump terminator.
type JumpEffect struct {
	MetaInfo Meta
	To       BlockID
}

func (e JumpEffect) Kind() EffectKind { return EffectJump }
func (e JumpEffect) EffectMeta() Meta { return e.MetaInfo }

// ReturnEffect records a RET/RETF. Value is populated only when the function's
// source signature returns a value; void functions leave it empty even though
// the machine registers still contain incidental values.
type ReturnEffect struct {
	MetaInfo Meta
	Value    Value
}

func (e ReturnEffect) Kind() EffectKind { return EffectReturn }
func (e ReturnEffect) EffectMeta() Meta { return e.MetaInfo }

// UnknownEffect keeps coverage visible without forcing a fake semantic action.
type UnknownEffect struct {
	MetaInfo Meta
	Inst     asm.DecodedInst
	Why      string
}

func (e UnknownEffect) Kind() EffectKind { return EffectUnknown }
func (e UnknownEffect) EffectMeta() Meta { return e.MetaInfo }

// BlockEffects is the effect-level result for one basic block.
type BlockEffects struct {
	Block   BlockID
	Entry   *state
	Exit    *state
	Effects []Effect
}

// FuncEffects is the package output consumed by sem lowering.
type FuncEffects struct {
	CFG    *CFG
	Blocks []BlockEffects
}

// Clone returns a detached copy of the extracted machine effects.
func (f *FuncEffects) Clone() *FuncEffects {
	out := &FuncEffects{
		CFG:    f.CFG.Clone(),
		Blocks: make([]BlockEffects, len(f.Blocks)),
	}

	for i, block := range f.Blocks {
		out.Blocks[i] = block.clone()
	}

	return out
}

func (b BlockEffects) clone() BlockEffects {
	out := b

	if b.Entry != nil {
		out.Entry = b.Entry.clone()
	}
	if b.Exit != nil {
		out.Exit = b.Exit.clone()
	}

	out.Effects = append([]Effect(nil), b.Effects...)

	return out
}
