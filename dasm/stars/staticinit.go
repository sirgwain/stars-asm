package stars

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// InitKind identifies the kind of a decoded static initializer node.
type InitKind uint8

const (
	InitUnknown InitKind = iota
	InitZero
	InitScalar
	InitString
	InitArray
	InitStruct
	InitAddress
)

// Initializer is a decoded static initializer tree node.
type Initializer struct {
	Kind   InitKind
	Type   typeinfo.Type
	Scalar any
	String string
	Elems  []*Initializer
	Fields []InitField
	Addr   *InitAddrTarget
}

// InitField holds a struct/union field initializer paired with its layout member.
type InitField struct {
	Member *typeinfo.StructField
	Value  *Initializer
}

// InitAddrTarget describes a static pointer target.
type InitAddrTarget struct {
	Symbol    *typeinfo.GlobalVar
	Function  *typeinfo.Function
	ByteOff   int
	RawSeg    uint16
	RawOff    uint32
	ExactBase bool
}

type initLocation struct {
	Seg uint16
	Off uint32
}

// LoadGlobalInitializer decodes the static initializer for sym from the NE image.
// Returns nil, false if the symbol has no recoverable initializer.
func LoadGlobalInitializer(img *asm.ImageNE, sdb *typeinfo.SymbolDB, g *typeinfo.GlobalVar) (*Initializer, bool) {
	if g.Type.Bytes() <= 0 {
		return nil, false
	}

	buf, ok := readGlobalBytes(img, g)
	if !ok {
		return nil, false
	}
	return decodeInitializer(img, sdb, initLocation{Seg: g.Addr.Seg, Off: g.Addr.Off}, g.Type, buf)
}

// decodeInitializer recursively decodes bytes into an Initializer tree for a global var.
func decodeInitializer(
	img *asm.ImageNE,
	sdb *typeinfo.SymbolDB,
	initAddr initLocation,
	typ typeinfo.Type,
	buf []byte,
) (*Initializer, bool) {

	switch v := typ.(type) {
	case *typeinfo.Primitive:
		return decodeScalarInitializer(v, typ.Bytes()*8, buf)
	case *typeinfo.Enum:
		return decodeEnumInitializer(v, buf)

	case *typeinfo.Pointer:
		return decodePointerInitializer(img, sdb, initAddr, v, buf)
	case *typeinfo.Array:
		return decodeArrayInitializer(img, sdb, initAddr, v, buf)

	case *typeinfo.Struct:
		return decodeStructInitializer(img, sdb, initAddr, v, buf)
	default:
		return nil, false
	}
}
