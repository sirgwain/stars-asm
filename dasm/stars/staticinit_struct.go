package stars

import (
	"encoding/binary"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// decodeStructInitializer decodes a struct or union from buf using Struct.
func decodeStructInitializer(
	img *asm.ImageNE,
	sdb *typeinfo.SymbolDB,
	loc initLocation,
	s *typeinfo.Struct,
	buf []byte,
) (*Initializer, bool) {

	// Union: conservative decode — first member only, or zero.
	if s.SKind == typeinfo.StructKindUnion {
		return decodeUnionInitializer(img, sdb, loc, s, buf)
	}

	var fields []InitField
	for i := range s.Chunks {
		chunk := &s.Chunks[i]
		switch chunk.Kind {
		case typeinfo.StructFieldChunkField:
			if len(chunk.Fields) == 0 {
				continue
			}
			m := &chunk.Fields[0]
			if m.Type == nil || m.Size <= 0 {
				continue
			}
			start := m.Offset
			end := m.Offset + m.Size
			if start < 0 || end > len(buf) {
				continue
			}
			child, ok := decodeInitializer(img, sdb, initLocation{Seg: loc.Seg, Off: loc.Off + uint32(m.Offset)}, m.Type, buf[start:end])
			if !ok {
				continue
			}
			mCopy := *m
			fields = append(fields, InitField{Member: &mCopy, Value: child})

		case typeinfo.StructFieldChunkBitfield:
			bfFields := decodeBitfieldGroup(*chunk, buf)
			fields = append(fields, bfFields...)
		}
	}

	return &Initializer{Kind: InitStruct, Type: s, Fields: fields}, true
}

// decodeUnionInitializer provides conservative union decoding (v1).
func decodeUnionInitializer(
	img *asm.ImageNE,
	sdb *typeinfo.SymbolDB,
	initAddr initLocation,
	s *typeinfo.Struct,
	buf []byte,
) (*Initializer, bool) {
	// All-zero union: return InitZero.
	allZero := true
	for _, b := range buf {
		if b != 0 {
			allZero = false
			break
		}
	}
	if allZero {
		return &Initializer{Kind: InitZero, Type: s}, true
	}

	// Non-zero: decode first layout member only.
	if len(s.Fields) == 0 {
		return &Initializer{Kind: InitStruct, Type: s}, true
	}
	m := &s.Fields[0]
	if m.Type == nil || m.Size <= 0 || m.Offset+m.Size > len(buf) {
		return &Initializer{Kind: InitStruct, Type: s}, true
	}
	start := m.Offset
	end := m.Offset + m.Size
	child, ok := decodeInitializer(img, sdb, initAddr, m.Type, buf[start:end])
	if !ok {
		return &Initializer{Kind: InitStruct, Type: s}, true
	}
	mCopy := *m
	return &Initializer{Kind: InitStruct, Type: s, Fields: []InitField{{Member: &mCopy, Value: child}}}, true
}

// decodeBitfieldGroup decodes all bitfield members in a bitfield-group chunk.
func decodeBitfieldGroup(chunk typeinfo.StructFieldChunk, buf []byte) []InitField {
	if len(chunk.Fields) == 0 {
		return nil
	}
	// All members share a common storage word starting at chunk.Start.
	start := chunk.Start
	storageSize := chunk.End - chunk.Start
	if start < 0 || start+storageSize > len(buf) || storageSize > 8 {
		return nil
	}

	// Read storage word as uint64 LE.
	storeBuf := buf[start : start+storageSize]
	var word uint64
	switch storageSize {
	case 1:
		word = uint64(storeBuf[0])
	case 2:
		word = uint64(binary.LittleEndian.Uint16(storeBuf))
	case 4:
		word = uint64(binary.LittleEndian.Uint32(storeBuf))
	case 8:
		word = binary.LittleEndian.Uint64(storeBuf)
	default:
		return nil
	}

	var fields []InitField
	for i := range chunk.Fields {
		m := &chunk.Fields[i]
		if m.Bitfield == nil {
			continue
		}
		bf := m.Bitfield
		if bf.BitWidth <= 0 {
			continue
		}
		mask := uint64((1 << bf.BitWidth) - 1)
		raw := (word >> bf.BitOffset) & mask

		var scalar any
		if p, ok := m.Type.(*typeinfo.Primitive); ok && p.Signed {
			// Sign-extend.
			signBit := uint64(1) << (bf.BitWidth - 1)
			if raw&signBit != 0 {
				signed := int64(raw | ^((signBit << 1) - 1))
				scalar = signed
			} else {
				scalar = int64(raw)
			}
		} else {
			scalar = raw
		}

		mCopy := *m
		fields = append(fields, InitField{
			Member: &mCopy,
			Value:  &Initializer{Kind: InitScalar, Type: m.Type, Scalar: scalar},
		})
	}
	return fields
}
