package typeinfo

import (
	"fmt"
	"sort"
	"strings"
)

// StructKind distinguishes C aggregate types that share the same field shape.
type StructKind int

const (
	StructKindStruct StructKind = iota
	StructKindUnion
)

// Kind returns the high-level type kind for this aggregate kind.
func (k StructKind) Kind() Kind {
	switch k {
	case StructKindUnion:
		return KUnion
	default:
		return KStruct
	}
}

// String returns the C keyword for this aggregate kind.
func (k StructKind) String() string {
	switch k {
	case StructKindStruct:
		return "struct"
	case StructKindUnion:
		return "union"
	default:
		return fmt.Sprintf("StructKind(%d)", uint8(k))
	}
}

// StructField describes a field within a struct or union.
type StructField struct {
	Name     string
	Type     Type
	Offset   int
	Size     int
	End      int
	Bitfield *Bitfield
}

// IsBitfield reports whether the field is backed by bitfield metadata.
func (f StructField) IsBitfield() bool { return f.Bitfield != nil }

// Bitfield describes a bitfield member's storage and bit range.
type Bitfield struct {
	BaseType    Type
	StorageSize int
	BitOffset   int
	BitWidth    int
}

// StructFieldChunkKind identifies a normalized layout chunk.
type StructFieldChunkKind int

const (
	StructFieldChunkField StructFieldChunkKind = iota
	StructFieldChunkBitfield
)

// StructFieldChunk describes one occupied byte range in an aggregate layout.
type StructFieldChunk struct {
	Kind   StructFieldChunkKind
	Start  int
	End    int
	Type   Type
	Name   string
	Fields []StructField // for bitfield groups: all fields; for field: len==1
	order  int
}

// Size returns the byte width of the chunk.
func (c StructFieldChunk) Size() int { return c.End - c.Start }

// StructOverlapRegion describes a byte range where normalized chunks overlap.
type StructOverlapRegion struct {
	Start        int
	End          int
	ChunkIndexes []int
	Paths        [][]int
}

// Struct describes a named struct or union type.
type Struct struct {
	Name           string
	Typedef        string
	SKind          StructKind
	Size           int
	Fields         []StructField
	Chunks         []StructFieldChunk
	OverlapRegions []StructOverlapRegion

	fieldsByOffset map[int][]int
	fieldsByByte   [][]int // every field containing each byte
}

// Kind returns the high-level type kind for the struct or union.
func (s *Struct) Kind() Kind { return s.SKind.Kind() }

// Bytes returns the aggregate size in bytes.
func (s *Struct) Bytes() int { return s.Size }

// String returns the preferred display name for the aggregate.
func (s *Struct) String() string {
	if s.Typedef != "" {
		return s.Typedef
	}
	if s.Name != "" {
		return s.Name
	}
	return "<anonymous>"
}

// String returns the field name, including bit width for bitfields.
func (f StructField) String() string {
	if f.Bitfield == nil {
		return f.Name
	}
	return fmt.Sprintf("%s:%d", f.Name, f.Bitfield.BitWidth)
}

// FinalizeLayout derives field sizes, chunks, overlap regions, and offset lookup tables.
func (s *Struct) FinalizeLayout() {
	for i := range s.Fields {
		field := &s.Fields[i]
		if field.Size <= 0 {
			field.Size = fieldSize(*field)
		}
		if field.End <= field.Offset {
			field.End = field.Offset + field.Size
		}
	}
	s.Chunks = buildLayoutChunks(s.Fields)
	s.OverlapRegions = buildOverlapRegions(s.Chunks)
	s.fieldsByOffset = buildFieldsByOffset(s.Fields)
	s.fieldsByByte = buildFieldsByByte(s.Size, s.Fields)
}

// FieldsAtOffset returns the fields that begin at the given byte offset.
func (s *Struct) FieldsAtOffset(offset int) []StructField {
	indexes := s.fieldsByOffset[offset]
	fields := make([]StructField, 0, len(indexes))
	for _, idx := range indexes {
		if idx >= 0 && idx < len(s.Fields) {
			fields = append(fields, s.Fields[idx])
		}
	}
	return fields
}

type StructFieldMatch struct {
	Field *StructField
	Off   int // byte offset within Field
}

func (s *Struct) FieldsContainingOffset(offset int) []StructFieldMatch {
	if offset < 0 || offset >= len(s.fieldsByByte) {
		return nil
	}

	indexes := s.fieldsByByte[offset]
	out := make([]StructFieldMatch, 0, len(indexes))

	for _, idx := range indexes {
		field := &s.Fields[idx]

		out = append(out, StructFieldMatch{
			Field: field,
			Off:   offset - field.Offset,
		})
	}

	return out
}

// IsExternalWindowsStruct returns true for Windows API struct tag names like
// tagRECT, tagPOINT, tagMSG, etc. These are external types and should not be
// emitted in FormatAllStructsC output.
func (s *Struct) IsExternalWindowsStruct() bool {
	name := s.Name
	if len(name) > 3 && strings.HasPrefix(name, "tag") && name[3] >= 'A' && name[3] <= 'Z' {
		return true
	}

	switch strings.ToUpper(name) {
	case "TIMERINFO", "COMPLEX", "DOCINFO", "HELPWININFO", "MENUITEMTEMPLATE", "MENUITEMTEMPLATEHEADER", "_COMPLEXL", "_DIV_T", "_LDIV_T", "_COMPLEX", "_EXCEPTION", "_EXCEPTIONL", "_DISKFREE_T", "_FIND_T":
		return true
	}
	return false
}

// fieldSize returns the byte width occupied by a field.
func fieldSize(field StructField) int {
	if field.Bitfield != nil {
		return field.Bitfield.StorageSize
	}
	if field.Type == nil {
		return 0
	}
	return field.Type.Bytes()
}

// buildFieldsByOffset indexes fields by their starting byte offset.
func buildFieldsByOffset(fields []StructField) map[int][]int {
	if len(fields) == 0 {
		return nil
	}
	byOffset := make(map[int][]int)
	for i, field := range fields {
		byOffset[field.Offset] = append(byOffset[field.Offset], i)
	}
	return byOffset
}

// buildFieldsByByte indexes all bytes in the struct by the fields within them
func buildFieldsByByte(size int, fields []StructField) [][]int {
	byByte := make([][]int, size)

	for i := range fields {
		field := &fields[i]

		start := field.Offset
		end := field.End

		if start < 0 {
			start = 0
		}
		if end > size {
			end = size
		}

		for off := start; off < end; off++ {
			byByte[off] = append(byByte[off], i)
		}
	}

	return byByte
}

// buildLayoutChunks normalizes fields into sorted byte-range chunks.
func buildLayoutChunks(fields []StructField) []StructFieldChunk {
	var chunks []StructFieldChunk
	for i := 0; i < len(fields); i++ {
		field := fields[i]
		if field.Bitfield != nil {
			chunk, last := buildBitfieldLayoutChunk(fields, i)
			chunk.order = i
			chunks = append(chunks, chunk)
			i = last
			continue
		}
		chunks = append(chunks, StructFieldChunk{
			Kind:   StructFieldChunkField,
			Start:  field.Offset,
			End:    field.End,
			Type:   field.Type,
			Name:   field.Name,
			Fields: []StructField{field},
			order:  i,
		})
	}
	sort.SliceStable(chunks, func(i, j int) bool {
		if chunks[i].Start != chunks[j].Start {
			return chunks[i].Start < chunks[j].Start
		}
		if chunks[i].End != chunks[j].End {
			return chunks[i].End < chunks[j].End
		}
		return chunks[i].order < chunks[j].order
	})
	return chunks
}

// buildBitfieldLayoutChunk groups adjacent bitfields that share a storage slot.
func buildBitfieldLayoutChunk(fields []StructField, start int) (StructFieldChunk, int) {
	first := fields[start]
	chunk := StructFieldChunk{
		Kind:   StructFieldChunkBitfield,
		Start:  first.Offset,
		End:    first.End,
		Type:   first.Bitfield.BaseType,
		Fields: []StructField{first},
	}
	last := start
	for i := start + 1; i < len(fields); i++ {
		field := fields[i]
		if field.Bitfield == nil ||
			field.Offset != first.Offset ||
			field.Bitfield.BaseType != first.Bitfield.BaseType ||
			field.Bitfield.StorageSize != first.Bitfield.StorageSize {
			break
		}
		chunk.Fields = append(chunk.Fields, field)
		last = i
	}
	sort.SliceStable(chunk.Fields, func(i, j int) bool {
		a, b := chunk.Fields[i], chunk.Fields[j]
		if a.Bitfield.BitOffset != b.Bitfield.BitOffset {
			return a.Bitfield.BitOffset < b.Bitfield.BitOffset
		}
		return i < j
	})
	return chunk, last
}

// buildOverlapRegions finds byte ranges where normalized chunks overlap.
func buildOverlapRegions(chunks []StructFieldChunk) []StructOverlapRegion {
	var regions []StructOverlapRegion
	for i := 0; i < len(chunks); {
		regionEnd := chunks[i].End
		j := i + 1
		hasOverlap := false
		for j < len(chunks) && chunks[j].Start < regionEnd {
			hasOverlap = true
			if chunks[j].End > regionEnd {
				regionEnd = chunks[j].End
			}
			j++
		}
		for j < len(chunks) && chunks[j].Start < regionEnd {
			hasOverlap = true
			if chunks[j].End > regionEnd {
				regionEnd = chunks[j].End
			}
			j++
		}
		if !hasOverlap {
			i++
			continue
		}

		region := StructOverlapRegion{
			Start:        chunks[i].Start,
			End:          regionEnd,
			ChunkIndexes: make([]int, 0, j-i),
		}
		for k := i; k < j; k++ {
			if chunks[k].Start < regionEnd {
				region.ChunkIndexes = append(region.ChunkIndexes, k)
			}
		}
		region.Paths = overlapRegionPaths(chunks, region)
		if len(region.Paths) >= 2 {
			regions = append(regions, region)
		}
		i = j
	}
	return regions
}

// overlapRegionPaths groups overlapping chunks into field-list-order layout paths.
func overlapRegionPaths(chunks []StructFieldChunk, region StructOverlapRegion) [][]int {
	indexes := append([]int(nil), region.ChunkIndexes...)
	sort.SliceStable(indexes, func(i, j int) bool {
		return chunks[indexes[i]].order < chunks[indexes[j]].order
	})

	var paths [][]int
	var current []int
	currentEnd := region.Start
	for _, idx := range indexes {
		chunk := chunks[idx]
		if chunk.End <= chunk.Start {
			continue
		}
		if len(current) > 0 && chunk.Start == currentEnd {
			current = append(current, idx)
			currentEnd = chunk.End
			continue
		}
		if len(current) > 0 {
			paths = append(paths, current)
		}
		current = []int{idx}
		currentEnd = chunk.End
	}
	if len(current) > 0 {
		paths = append(paths, current)
	}
	return paths
}
