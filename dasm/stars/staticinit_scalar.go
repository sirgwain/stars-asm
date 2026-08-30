package stars

import (
	"encoding/binary"
	"math"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// decodeEnumInitializer decodes an enum as a scalar while preserving enum type
// information for later symbolic formatting.
func decodeEnumInitializer(typ *typeinfo.Enum, buf []byte) (*Initializer, bool) {
	if len(buf) < typ.Bytes() {
		return nil, false
	}
	return &Initializer{Kind: InitScalar, Type: typ, Scalar: uint64(binary.LittleEndian.Uint16(buf[:2]))}, true
}

// decodeScalarInitializer decodes a scalar (int/bool/float) from buf using typ.
func decodeScalarInitializer(typ *typeinfo.Primitive, bits int, buf []byte) (*Initializer, bool) {

	switch typ.Kind() {
	case typeinfo.KBool:
		if len(buf) < 1 {
			return nil, false
		}
		return &Initializer{Kind: InitScalar, Type: typ, Scalar: buf[0] != 0}, true

	case typeinfo.KInt:
		return decodeIntScalar(typ, bits, buf)

	case typeinfo.KFloat:
		return decodeFloatScalar(typ, buf)

	default:
		return nil, false
	}
}

func decodeIntScalar(typ *typeinfo.Primitive, bits int, buf []byte) (*Initializer, bool) {
	if bits == 0 {
		bits = typ.Bytes() * 8
	}
	sz := bits / 8
	if sz <= 0 || len(buf) < sz {
		return nil, false
	}
	b := buf[:sz]
	if typ.Signed {
		var v int64
		switch sz {
		case 1:
			v = int64(int8(b[0]))
		case 2:
			v = int64(int16(binary.LittleEndian.Uint16(b)))
		case 4:
			v = int64(int32(binary.LittleEndian.Uint32(b)))
		case 8:
			v = int64(binary.LittleEndian.Uint64(b))
		default:
			return nil, false
		}
		return &Initializer{Kind: InitScalar, Type: typ, Scalar: v}, true
	}
	var v uint64
	switch sz {
	case 1:
		v = uint64(b[0])
	case 2:
		v = uint64(binary.LittleEndian.Uint16(b))
	case 4:
		v = uint64(binary.LittleEndian.Uint32(b))
	case 8:
		v = binary.LittleEndian.Uint64(b)
	default:
		return nil, false
	}
	return &Initializer{Kind: InitScalar, Type: typ, Scalar: v}, true
}

func decodeFloatScalar(typ *typeinfo.Primitive, buf []byte) (*Initializer, bool) {
	switch typ.Bytes() {
	case 4:
		if len(buf) < 4 {
			return nil, false
		}
		v := math.Float32frombits(binary.LittleEndian.Uint32(buf[:4]))
		return &Initializer{Kind: InitScalar, Type: typ, Scalar: v}, true
	case 8:
		if len(buf) < 8 {
			return nil, false
		}
		v := math.Float64frombits(binary.LittleEndian.Uint64(buf[:8]))
		return &Initializer{Kind: InitScalar, Type: typ, Scalar: v}, true
	default:
		return nil, false
	}
}
