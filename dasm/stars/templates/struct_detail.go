package templates

import (
	"bytes"
	"fmt"
	"io"
	"sort"
	"strings"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type StructDetailView struct {
	Struct         *typeinfo.Struct
	Options        DumpOptions
	Kind           string
	TagName        string
	TypedefName    string
	Size           int
	Body           string
	DefinitionOnly bool
}

func NewStructDetailView(s *typeinfo.Struct, opt DumpOptions) StructDetailView {
	if len(s.Chunks) == 0 && len(s.Fields) > 0 {
		s.FinalizeLayout()
	}

	kind := "struct"
	if s.SKind == typeinfo.StructKindUnion {
		kind = "union"
	}
	typedefName := s.Typedef
	if typedefName == "" {
		typedefName = s.Name
	}

	return StructDetailView{
		Struct:      s,
		Options:     opt,
		Kind:        kind,
		TagName:     s.Name,
		TypedefName: typedefName,
		Size:        s.Size,
		Body:        renderStructBody(s, "    "),
	}
}

func RenderStructDetail(w io.Writer, view StructDetailView) error {
	t := template.New("struct_detail.templ").
		Funcs(template.FuncMap{
			"hex": func(v int) string { return fmt.Sprintf("%#x", v) },
		})

	tmpl, err := t.ParseFS(templatesFS, "assets/struct_detail.templ")
	if err != nil {
		return err
	}
	var buf bytes.Buffer
	if err := tmpl.Execute(&buf, view); err != nil {
		return err
	}

	formatted, err := formatCSource(buf.String())
	if err != nil {
		return err
	}

	printHighlightedC(w, formatted, view.Options.ShowColor)
	return nil
}

func renderStructBody(s *typeinfo.Struct, indent string) string {
	lines := renderStructBodyLines(s, indent)
	if len(lines) == 0 {
		return ""
	}
	return strings.Join(lines, "\n") + "\n"
}

func renderStructBodyLines(s *typeinfo.Struct, indent string) []string {
	regionByFirstChunk := make(map[int]typeinfo.StructOverlapRegion)
	coveredChunks := make(map[int]struct{})
	for _, region := range s.OverlapRegions {
		if len(region.ChunkIndexes) == 0 {
			continue
		}
		first := region.ChunkIndexes[0]
		regionByFirstChunk[first] = region
		for _, idx := range region.ChunkIndexes {
			coveredChunks[idx] = struct{}{}
		}
	}

	var lines []string
	for i, chunk := range s.Chunks {
		if region, ok := regionByFirstChunk[i]; ok {
			lines = append(lines, renderOverlapRegionLines(s, region, indent)...)
			continue
		}
		if _, ok := coveredChunks[i]; ok {
			continue
		}
		if s.SKind == typeinfo.StructKindUnion {
			lines = append(lines, renderUnionMemberLines(chunk, indent)...)
			continue
		}
		lines = append(lines, renderChunkLines(chunk, indent)...)
	}
	return lines
}

func renderOverlapRegionLines(s *typeinfo.Struct, region typeinfo.StructOverlapRegion, indent string) []string {
	if len(region.Paths) < 2 {
		var lines []string
		for _, idx := range region.ChunkIndexes {
			if idx >= 0 && idx < len(s.Chunks) {
				lines = append(lines, renderChunkLines(s.Chunks[idx], indent)...)
			}
		}
		return lines
	}

	lines := []string{indent + "union {"}
	for _, path := range region.Paths {
		if len(path) == 1 {
			idx := path[0]
			if idx >= 0 && idx < len(s.Chunks) {
				lines = append(lines, renderUnionMemberLines(s.Chunks[idx], indent+"    ")...)
			}
			continue
		}
		lines = append(lines, indent+"    struct {")
		for _, idx := range path {
			if idx >= 0 && idx < len(s.Chunks) {
				lines = append(lines, renderChunkLines(s.Chunks[idx], indent+"        ")...)
			}
		}
		lines = append(lines, indent+"    };")
	}
	lines = append(lines, indent+"};")
	return lines
}

// renderUnionMemberLines renders one chunk as a union member. Every union
// member starts at offset 0, bitfields included, so a group of bitfields is
// wrapped in an anonymous struct to keep each at its own bit offset.
func renderUnionMemberLines(chunk typeinfo.StructFieldChunk, indent string) []string {
	if chunk.Kind != typeinfo.StructFieldChunkBitfield {
		return renderChunkLines(chunk, indent)
	}
	var lines []string
	for _, group := range bitfieldGroups(chunk) {
		lines = append(lines, indent+"struct {")
		lines = append(lines, renderBitfieldGroupLines(chunk, group, indent+"    ")...)
		lines = append(lines, indent+"};")
	}
	return lines
}

func renderChunkLines(chunk typeinfo.StructFieldChunk, indent string) []string {
	if chunk.Kind == typeinfo.StructFieldChunkBitfield {
		groups := bitfieldGroups(chunk)
		if len(groups) == 1 {
			return renderBitfieldGroupLines(chunk, groups[0], indent)
		}
		// Overlapping bitfield layouts of one container are alternatives.
		lines := []string{indent + "union {"}
		lines = append(lines, renderUnionMemberLines(chunk, indent+"    ")...)
		return append(lines, indent+"};")
	}
	if len(chunk.Fields) == 0 {
		return nil
	}
	field := chunk.Fields[0]
	return []string{
		fmt.Sprintf("%s%s; /* +0x%04X (%d) */", indent, typeinfo.TypeDecl(chunk.Type, chunk.Name), field.Offset, field.Size),
	}
}

// bitfieldGroups splits a bitfield chunk's fields into groups whose bits do
// not overlap, in bit order. The debug info can describe one container with
// several overlapping layouts, such as a flags word read either as det and
// iPlrBmp or as reserved and fAi; each layout becomes its own group.
func bitfieldGroups(chunk typeinfo.StructFieldChunk) [][]typeinfo.StructField {
	var fields []typeinfo.StructField
	for _, field := range chunk.Fields {
		if field.Bitfield != nil {
			fields = append(fields, field)
		}
	}
	sort.SliceStable(fields, func(i, j int) bool { return fields[i].Bitfield.BitOffset < fields[j].Bitfield.BitOffset })

	var groups [][]typeinfo.StructField
	var ends []int
	for _, field := range fields {
		placed := false
		for i := range groups {
			if ends[i] <= field.Bitfield.BitOffset {
				groups[i] = append(groups[i], field)
				ends[i] = field.Bitfield.BitOffset + field.Bitfield.BitWidth
				placed = true
				break
			}
		}
		if !placed {
			groups = append(groups, []typeinfo.StructField{field})
			ends = append(ends, field.Bitfield.BitOffset+field.Bitfield.BitWidth)
		}
	}
	return groups
}

// renderBitfieldGroupLines declares one group of non-overlapping bitfields
// in a single container declaration, with unnamed padding bitfields holding
// each field at its bit offset.
func renderBitfieldGroupLines(chunk typeinfo.StructFieldChunk, fields []typeinfo.StructField, indent string) []string {
	type member struct {
		decl   string
		bitOff int
	}
	var members []member
	next := 0
	for _, field := range fields {
		if gap := field.Bitfield.BitOffset - next; gap > 0 {
			members = append(members, member{decl: fmt.Sprintf(": %d", gap), bitOff: next})
		}
		members = append(members, member{decl: fmt.Sprintf("%s : %d", field.Name, field.Bitfield.BitWidth), bitOff: field.Bitfield.BitOffset})
		next = field.Bitfield.BitOffset + field.Bitfield.BitWidth
	}

	lines := make([]string, 0, len(members))
	for i, m := range members {
		term := ","
		if i == len(members)-1 {
			term = ";"
		}
		if i == 0 {
			lines = append(lines, fmt.Sprintf("%s%s %s%s /* +0x%04X (%d) @bit%d */",
				indent, chunk.Type.String(), m.decl, term, chunk.Start, chunk.Size(), m.bitOff))
			continue
		}
		lines = append(lines, fmt.Sprintf("%s        %s%s /* @bit%d */", indent, m.decl, term, m.bitOff))
	}
	return lines
}
