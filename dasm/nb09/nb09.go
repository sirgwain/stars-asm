package nb09

import (
	"fmt"
	"strings"
)

type SubsectionKind uint16

const (
	SST_MODULE       SubsectionKind = 0x0120
	SST_ALIGN_SYM    SubsectionKind = 0x0125
	SST_SRC_MODULE   SubsectionKind = 0x0127
	SST_LIBRARIES    SubsectionKind = 0x0128
	SST_GLOBAL_SYM   SubsectionKind = 0x0129
	SST_GLOBAL_PUB   SubsectionKind = 0x012A
	SST_GLOBAL_TYPES SubsectionKind = 0x012B
	SST_SEG_MAP      SubsectionKind = 0x012D
	SST_SEG_NAME     SubsectionKind = 0x012E
	SST_FILE_INDEX   SubsectionKind = 0x0133
	SST_STATIC_SYM   SubsectionKind = 0x0134
)

type SymRecType uint16

// Subset you referenced in nb09_parser.py (plus a couple “bookends”).
const (
	S_COMPILE   SymRecType = 0x0001
	S_REGISTER  SymRecType = 0x0002
	S_CONSTANT  SymRecType = 0x0003
	S_UDT       SymRecType = 0x0004
	S_SSEARCH   SymRecType = 0x0005
	S_END       SymRecType = 0x0006
	S_SKIP      SymRecType = 0x0007
	S_CVRESERVE SymRecType = 0x0008
	S_OBJNAME   SymRecType = 0x0009
	S_ENDARG    SymRecType = 0x000A
	S_COBOLUDT  SymRecType = 0x000B
	S_MANYREG   SymRecType = 0x000C
	S_RETURN    SymRecType = 0x000D
	S_ENTRYTHIS SymRecType = 0x000E

	S_BPREL16    SymRecType = 0x0100
	S_LDATA16    SymRecType = 0x0101
	S_GDATA16    SymRecType = 0x0102
	S_PUB16      SymRecType = 0x0103
	S_LPROC16    SymRecType = 0x0104
	S_GPROC16    SymRecType = 0x0105
	S_BLOCK16    SymRecType = 0x0107
	S_WITH16     SymRecType = 0x0108
	S_LABEL16    SymRecType = 0x0109
	S_CEXMODEL16 SymRecType = 0x010A
	S_VFTPATH16  SymRecType = 0x010B
	S_REGREL16   SymRecType = 0x010C

	S_BPREL32   SymRecType = 0x0200
	S_LDATA32   SymRecType = 0x0201
	S_GDATA32   SymRecType = 0x0202
	S_PUB32     SymRecType = 0x0203
	S_LPROC32   SymRecType = 0x0204
	S_GPROC32   SymRecType = 0x0205
	S_THUNK32   SymRecType = 0x0206
	S_BLOCK32   SymRecType = 0x0207
	S_VFTPATH32 SymRecType = 0x020B
	S_REGREL32  SymRecType = 0x020C
	S_LTHREAD32 SymRecType = 0x020D
	S_GTHREAD32 SymRecType = 0x020E

	S_LPROCMIPS SymRecType = 0x0300
	S_GPROCMIPS SymRecType = 0x0301

	S_PROCREF SymRecType = 0x0400
	S_DATAREF SymRecType = 0x0401
	S_ALIGN   SymRecType = 0x0402
)

// ---------- NB09 container ----------

// NB09DB is a convenient “fully parsed” database with all nb09 data
type NB09DB struct {
	Header NB09Header
	Dir    Directory

	// Raw subsections (the raw bytes for each subsection)
	subsections []RawSubsection

	// Optionally-parsed subsections:
	Modules     map[uint16]SstModule    // imod -> module
	AlignSyms   map[uint16]SymStream    // imod -> per-module sym stream
	SrcModules  map[uint16]SstSrcModule // imod -> line info
	Libraries   []string
	SegNames    SstSegName
	SegMap      SstSegMap
	FileIndex   SstFileIndex
	GlobalPub   HashSymSubsection
	GlobalSym   HashSymSubsection
	StaticSym   HashSymSubsection
	GlobalTypes TypeStream
}

func (db *NB09DB) ModuleName(imod uint16) string {
	if db.Modules == nil {
		return ""
	}
	if m, ok := db.Modules[imod]; ok {
		return m.Name
	}
	return ""
}

type NB09Header struct {
	Sig    [4]byte // "NB09"
	LfoDir uint32  // offset (relative to NB09 base) of directory header
}

type Directory struct {
	Header  DirHeader
	Entries []DirEntry
}

type DirHeader struct {
	CBDirHeader uint16 // usually 0x0010
	CBDirEntry  uint16 // usually 0x000C
	CDir        uint32 // number of entries
	LfoNextDir  uint32 // often 0
	Flags       uint32
}

type DirEntry struct {
	Subsection SubsectionKind // sst index (SST_*)
	IMod       uint16         // module index (1-based for module-specific subsections)
	Lfo        uint32         // offset (relative to NB09 base) of subsection data
	CB         uint32         // size of subsection data
}

type RawSubsection struct {
	Kind SubsectionKind
	IMod uint16
	Lfo  uint32
	Data []byte
}

// ---------- Common “hashsym” subsection header (GlobalSym/GlobalPub/StaticSym) ----------

type HashSymSubsection struct {
	SymHash    uint16
	AddrHash   uint16
	CBSymbol   uint32
	CBSymHash  uint32
	CBAddrHash uint32

	// The actual symbol record stream:
	Symbols SymStream

	// Raw hash tables (optional; many parsers ignore initially)
	SymHashBytes  []byte
	AddrHashBytes []byte
}

type SymStream struct {
	Records []SymRecord
}

type SymRecord struct {
	Offset uint32 // offset within the symbol blob
	RecLen uint16 // length of (rectyp + payload)
	RecTyp SymRecType
	Raw    []byte // payload bytes (len == RecLen-2)
	Parsed any    // one of the typed structs below, or nil for unknown
}

// ---------- Address helpers ----------

type Addr16 struct {
	Off uint16
	Seg uint16
}

// ---------- Symbol record payload structs (typed parse targets) ----------

type SData16 struct {
	IsGlobal bool // rectyp distinguishes local/global; keep it explicit
	Addr     Addr16
	TypInd   uint16
	Name     string // length-prefixed in these records
}

type SPub16 struct {
	Addr   Addr16
	TypInd uint16
	Name   string
}

type SPub32 struct {
	Off    uint32
	Seg    uint16
	TypInd uint16
	Name   string
}

type SProc16 struct {
	// CV “parent/next/end” are offsets in the containing symbol stream
	PParent uint32
	PEnd    uint32
	PNext   uint32

	ProcLen  uint16
	DbgStart uint16
	DbgEnd   uint16

	Addr   Addr16
	TypInd uint16
	Flags  uint8
	Name   string
}

// CVPACK reference records you resolve back into per-module sstAlignSym.
type SProcRef struct {
	Checksum uint32
	SymOff   uint32 // offset within that module's sstAlignSym stream
	IMod     uint16
	TypInd   uint16
}

type SDataRef struct {
	Checksum uint32
	SymOff   uint32
	IMod     uint16
	TypInd   uint16
}

// S_BPREL16: BP-relative local variable or parameter.
type SBpRel16 struct {
	Off    int16 // signed offset from BP (negative = local, positive = param)
	TypInd uint16
	Name   string
}

// S_REGISTER: variable that lives entirely in a register.
type SRegister struct {
	TypInd uint16
	Reg    uint16
	Name   string
}

// Often present, even if you don't use it yet.
type SUdt struct {
	TypInd uint16
	Name   string
}

type SObjName struct {
	Signature uint32 // “signature”/timestamp-ish in some toolchains
	Name      string
}

type SBlock16 struct {
	PParent uint32
	PEnd    uint32
	Len     uint16
	Addr    Addr16
	Name    string
}

type SLabel16 struct {
	Addr  Addr16
	Flags uint8
	Name  string
}

type SCompile struct {
	Flags   uint16 // compile flags (meaning varies by toolchain)
	Machine uint16 // often 0x0000 or target info; sometimes absent in older variants
	Version string // compiler/version string (NUL-terminated or length-prefixed depending on variant)
}

type SSSearch struct {
	Addr   Addr16
	SymOff uint16
}

// ---------- Procedure: proc + nested locals ----------

// Procedure groups a procedure symbol record with all the symbol records
// that appear inside its scope (between PROC and matching END).
type Procedure struct {
	// The proc record itself (SProc16 or SProc32).
	Proc SymRecord
	// All symbol records inside the proc scope, including nested blocks.
	// This includes S_BPREL16, S_REGISTER, S_BLOCK16, S_LABEL16, etc.
	Children []SymRecord
}

// ---------- Subsection: sstModule (0x0120) ----------

// SstModule represents one SST_MODULE subsection.
//
// Each module corresponds to one object file (.obj) or
// assembler source that was linked into the final EXE.
//
// imod (from the directory entry) is the module index
// used by other symbol records to reference this module.
type SstModule struct {
	// Overlay number.
	// Used in overlayed builds; almost always 0 in normal builds.
	Ovl uint16

	// Library index.
	// If this module came from a library (.lib), this identifies
	// which library. 0 or small values usually mean not from a lib.
	// For Stars, mostly ignored
	ILib uint16

	// Number of segment contributions (count of ModuleSegInfo entries).
	// This tells you how many segments this module contributed to.
	CSeg uint16

	// Debug style identifier.
	// This is a CodeView "style" constant indicating the debug format
	// variant. For NB09 this is typically a fixed magic number
	// (e.g. 0x5633 / 22083 decimal).
	// You generally do not need to interpret it.
	Style uint16

	// One entry per contributed segment (length = CSeg).
	SegInfo []ModuleSegInfo

	// Module name as a Pascal string (u8 length + bytes).
	// Example: ".\\mantold.asm", "race.obj"
	// This is the original object file name.
	Name string
}

// ModuleSegInfo describes one segment contribution
// from this module.
//
// A module can contribute to multiple segments
// (e.g. code + data).
type ModuleSegInfo struct {
	// Segment number in the final executable.
	// This corresponds to the segment indices you see in
	// CodeView and your disassembler (e.g. MEMORY_RACE, etc.).
	Seg uint16

	// Reserved / padding.
	// Usually zero in 16-bit CodeView.
	// Historically used for alignment or future expansion.
	Pad uint16

	// Offset within the segment where this module's
	// contribution begins.
	Off uint32

	// Size in bytes of this module's contribution
	// within that segment.
	CB uint32
}

// ---------- Subsection: sstSegName (0x012E) ----------
// NUL-terminated string table; directory references offsets into this blob.
type SstSegName struct {
	// Offset within the segname blob -> string.
	At map[uint32]string
}

// ---------- Subsection: sstSegMap (0x012D) ----------

type SstSegMap struct {
	CSeg    uint16
	CSegLog uint16
	Segs    []SegMapDesc // length == CSeg
}

type SegMapDesc struct {
	Flags      uint16
	Ovl        uint16
	Group      uint16
	Frame      uint16
	ISegName   uint16
	IClassName uint16
	Off        uint32
	CB         uint32
	// Implicit segment number is index+1 if you want it.
}

// ---------- Subsection: sstFileIndex (0x0133) ----------
// NOTE: Stars uses length-prefixed names; NameRef points to the length byte.
type SstFileIndex struct {
	CMod uint16
	CRef uint16

	ModStart []uint16 // len CMod
	RefCnt   []uint16 // len CMod
	NameRef  []uint32 // len CRef

	Names      []string            // expanded names (len CRef)
	ModToFiles map[uint16][]string // imod(1..CMod) -> list of file names
}

// ---------- Subsection: sstSrcModule (0x0127) ----------
// This can get detailed; start typed with raw “tables” and refine as needed.
type SstSrcModule struct {
	// Keep raw sections so you can evolve without breaking callers.
	Raw []byte

	CFile uint16
	CSeg  uint16

	// Per-file info
	Files []SrcModuleFile
}

type SrcModuleFile struct {
	Name string
	Segs []SrcModuleFileSeg
}

type SrcModuleFileSeg struct {
	Seg   uint16
	Start uint32
	End   uint32
	Lines []SrcModuleLine
}

type SrcModuleLine struct {
	Off     uint32
	LineNum uint16
}

// ---------- Types: sstGlobalTypes (0x012B) ----------

type TypeStream struct {
	Records []TypeRecord
	// Many toolchains use base typind 0x1000; keep this as computed metadata.
	BaseIndex uint16
}

type TypeRecord struct {
	Offset uint32 // offset within type blob
	RecLen uint16
	Leaf   TypeLeaf
	Raw    []byte // leaf-data
	Parsed any    // one of the LF_* structs below, or nil for unknown
}

type TypeLeaf uint16

// Leaves you referenced (plus a few commonly adjacent ones).
const (
	LF_MODIFIER  TypeLeaf = 0x0001
	LF_POINTER   TypeLeaf = 0x0002
	LF_ARRAY     TypeLeaf = 0x0003
	LF_CLASS     TypeLeaf = 0x0004
	LF_STRUCTURE TypeLeaf = 0x0005
	LF_PROCEDURE TypeLeaf = 0x0008

	LF_ARGLIST   TypeLeaf = 0x0201
	LF_FIELDLIST TypeLeaf = 0x0204
	LF_BITFIELD  TypeLeaf = 0x0206
)

// Numeric leaf encoding (TIS “numeric leaf” format).
type NumericLeaf struct {
	// If IsLeaf==false, Value is the immediate u16 (<0x8000).
	// If IsLeaf==true, Leaf is LF_* (>=0x8000) describing the encoding.
	IsLeaf bool
	Leaf   uint16
	Value  int64
	// Optional raw string for LF_VARSTRING if you choose to support it.
	Str string
}

// ---------- Type record payload structs ----------

type LFPointer struct {
	// TIS has an attribute bitfield (size/mode/const/volatile/etc).
	// Keep raw Attr; decode later as needed.
	UType uint16
	Attr  uint32
}

type LFModifier struct {
	UType uint16
	Mod   uint16
}

type LFArray struct {
	ElemType  uint16
	IndexType uint16
	Length    NumericLeaf
	Name      string
}

type LFProcedure struct {
	RetType    uint16
	CallType   uint8
	Reserved   uint8
	ParamCount uint16
	ArgList    uint16 // typind of an LF_ARGLIST record
}

type LFArgList struct {
	Count uint16
	Args  []uint16 // typinds
}

type LFBitfield struct {
	BaseType uint16
	Length   uint8
	Position uint8
}

type LFStruct struct {
	Count     uint16
	FieldList uint16
	Property  uint16
	Deriv     uint16
	VShape    uint16
	Size      NumericLeaf
	Name      string
}

// FIELDLIST sub-records are their own small tagged stream.
// Strongly type the common ones; keep raw for unknowns.
type FieldEntry struct {
	Leaf   FieldLeaf
	Raw    []byte
	Parsed any
}

type FieldLeaf uint16

const (
	LF_MEMBER FieldLeaf = 0x0406
)

type LFFieldList struct {
	Fields []FieldEntry
}

type LFMember struct {
	Attr   uint16
	Type   uint16
	Offset NumericLeaf
	Name   string
}

func (f FieldLeaf) String() string {
	switch f {
	case LF_MEMBER:
		return "LF_MEMBER"
	default:
		return fmt.Sprintf("FieldLeaf(0x%04x)", uint16(f))
	}
}

// ---------- String() methods for diagnostics ----------

func (k SubsectionKind) String() string {
	switch k {
	case SST_MODULE:
		return "sstModule"
	case SST_ALIGN_SYM:
		return "sstAlignSym"
	case SST_SRC_MODULE:
		return "sstSrcModule"
	case SST_LIBRARIES:
		return "sstLibraries"
	case SST_GLOBAL_SYM:
		return "sstGlobalSym"
	case SST_GLOBAL_PUB:
		return "sstGlobalPub"
	case SST_GLOBAL_TYPES:
		return "sstGlobalTypes"
	case SST_SEG_MAP:
		return "sstSegMap"
	case SST_SEG_NAME:
		return "sstSegName"
	case SST_FILE_INDEX:
		return "sstFileIndex"
	case SST_STATIC_SYM:
		return "sstStaticSym"
	default:
		return fmt.Sprintf("sst(0x%04X)", uint16(k))
	}
}

func (r SymRecType) String() string {
	switch r {
	case S_COMPILE:
		return "S_COMPILE"
	case S_REGISTER:
		return "S_REGISTER"
	case S_CONSTANT:
		return "S_CONSTANT"
	case S_UDT:
		return "S_UDT"
	case S_SSEARCH:
		return "S_SSEARCH"
	case S_END:
		return "S_END"
	case S_SKIP:
		return "S_SKIP"
	case S_CVRESERVE:
		return "S_CVRESERVE"
	case S_OBJNAME:
		return "S_OBJNAME"
	case S_ENDARG:
		return "S_ENDARG"
	case S_COBOLUDT:
		return "S_COBOLUDT"
	case S_MANYREG:
		return "S_MANYREG"
	case S_RETURN:
		return "S_RETURN"
	case S_ENTRYTHIS:
		return "S_ENTRYTHIS"
	case S_BPREL16:
		return "S_BPREL16"
	case S_LDATA16:
		return "S_LDATA16"
	case S_GDATA16:
		return "S_GDATA16"
	case S_PUB16:
		return "S_PUB16"
	case S_LPROC16:
		return "S_LPROC16"
	case S_GPROC16:
		return "S_GPROC16"
	case S_BLOCK16:
		return "S_BLOCK16"
	case S_WITH16:
		return "S_WITH16"
	case S_LABEL16:
		return "S_LABEL16"
	case S_CEXMODEL16:
		return "S_CEXMODEL16"
	case S_VFTPATH16:
		return "S_VFTPATH16"
	case S_REGREL16:
		return "S_REGREL16"
	case S_BPREL32:
		return "S_BPREL32"
	case S_LDATA32:
		return "S_LDATA32"
	case S_GDATA32:
		return "S_GDATA32"
	case S_PUB32:
		return "S_PUB32"
	case S_LPROC32:
		return "S_LPROC32"
	case S_GPROC32:
		return "S_GPROC32"
	case S_THUNK32:
		return "S_THUNK32"
	case S_BLOCK32:
		return "S_BLOCK32"
	case S_VFTPATH32:
		return "S_VFTPATH32"
	case S_REGREL32:
		return "S_REGREL32"
	case S_LTHREAD32:
		return "S_LTHREAD32"
	case S_GTHREAD32:
		return "S_GTHREAD32"
	case S_LPROCMIPS:
		return "S_LPROCMIPS"
	case S_GPROCMIPS:
		return "S_GPROCMIPS"
	case S_PROCREF:
		return "S_PROCREF"
	case S_DATAREF:
		return "S_DATAREF"
	case S_ALIGN:
		return "S_ALIGN"
	default:
		return fmt.Sprintf("S_(0x%04X)", uint16(r))
	}
}

func (l TypeLeaf) String() string {
	switch l {
	case LF_MODIFIER:
		return "LF_MODIFIER"
	case LF_POINTER:
		return "LF_POINTER"
	case LF_ARRAY:
		return "LF_ARRAY"
	case LF_CLASS:
		return "LF_CLASS"
	case LF_STRUCTURE:
		return "LF_STRUCTURE"
	case LF_PROCEDURE:
		return "LF_PROCEDURE"
	case LF_ARGLIST:
		return "LF_ARGLIST"
	case LF_FIELDLIST:
		return "LF_FIELDLIST"
	case LF_BITFIELD:
		return "LF_BITFIELD"
	default:
		return fmt.Sprintf("LF_(0x%04X)", uint16(l))
	}
}

func (s SData16) String() string {
	var b strings.Builder

	if s.Name != "" {
		fmt.Fprintf(&b, "%s ", s.Name)
	}

	scope := "local"
	if s.IsGlobal {
		scope = "global"
	}

	fmt.Fprintf(&b,
		"(SData16 %s addr=%0x typ=0x%04x)",
		scope,
		s.Addr,
		s.TypInd,
	)

	return b.String()
}

func (s SPub16) String() string {
	var b strings.Builder

	if s.Name != "" {
		fmt.Fprintf(&b, "%s ", s.Name)
	}

	fmt.Fprintf(&b,
		"(SPub16 addr=%0x typ=0x%04x)",
		s.Addr,
		s.TypInd,
	)

	return b.String()
}

func (s SPub32) String() string {
	var b strings.Builder

	if s.Name != "" {
		fmt.Fprintf(&b, "%s ", s.Name)
	}

	fmt.Fprintf(&b,
		"(SPub32 seg=0x%04x off=0x%08x typ=0x%04x)",
		s.Seg,
		s.Off,
		s.TypInd,
	)

	return b.String()
}

func (s SProc16) String() string {
	var b strings.Builder

	if s.Name != "" {
		fmt.Fprintf(&b, "%s ", s.Name)
	}

	fmt.Fprintf(&b,
		"(SProc16 addr=%0x len=0x%04x typ=0x%04x flags=0x%02x parent=0x%08x next=0x%08x end=0x%08x)",
		s.Addr,
		s.ProcLen,
		s.TypInd,
		s.Flags,
		s.PParent,
		s.PNext,
		s.PEnd,
	)

	return b.String()
}

func (s SProcRef) String() string {
	return fmt.Sprintf(
		"(SProcRef imod=%d symOff=0x%08x typ=0x%04x checksum=0x%08x)",
		s.IMod,
		s.SymOff,
		s.TypInd,
		s.Checksum,
	)
}

func (s SDataRef) String() string {
	return fmt.Sprintf(
		"(SDataRef imod=%d symOff=0x%08x typ=0x%04x checksum=0x%08x)",
		s.IMod,
		s.SymOff,
		s.TypInd,
		s.Checksum,
	)
}

func (s SBpRel16) String() string {
	var b strings.Builder

	if s.Name != "" {
		fmt.Fprintf(&b, "%s ", s.Name)
	}

	kind := "local"
	if s.Off > 0 {
		kind = "param"
	}

	fmt.Fprintf(&b,
		"(SBpRel16 %s bp%+d typ=0x%04x)",
		kind,
		s.Off,
		s.TypInd,
	)

	return b.String()
}

func (s SRegister) String() string {
	var b strings.Builder

	if s.Name != "" {
		fmt.Fprintf(&b, "%s ", s.Name)
	}

	fmt.Fprintf(&b,
		"(SRegister reg=%d typ=0x%04x)",
		s.Reg,
		s.TypInd,
	)

	return b.String()
}

func (s SUdt) String() string {
	if s.Name != "" {
		return fmt.Sprintf("%s (SUdt typ=0x%04x)", s.Name, s.TypInd)
	}
	return fmt.Sprintf("(SUdt typ=0x%04x)", s.TypInd)
}

func (s SObjName) String() string {
	if s.Name != "" {
		return fmt.Sprintf("%s (SObjName sig=0x%08x)", s.Name, s.Signature)
	}
	return fmt.Sprintf("(SObjName sig=0x%08x)", s.Signature)
}

func (s SBlock16) String() string {
	var b strings.Builder

	if s.Name != "" {
		fmt.Fprintf(&b, "%s ", s.Name)
	}

	fmt.Fprintf(&b,
		"(SBlock16 addr=%0x len=0x%04x parent=0x%08x end=0x%08x)",
		s.Addr,
		s.Len,
		s.PParent,
		s.PEnd,
	)

	return b.String()
}

func (s SLabel16) String() string {
	var b strings.Builder

	if s.Name != "" {
		fmt.Fprintf(&b, "%s ", s.Name)
	}

	fmt.Fprintf(&b,
		"(SLabel16 addr=%0x flags=0x%02x)",
		s.Addr,
		s.Flags,
	)

	return b.String()
}
