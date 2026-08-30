package templates

import (
	"bytes"
	"fmt"
	"io"
	"strings"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type StructDetailView struct {
	Struct      *typeinfo.Struct
	Options     DumpOptions
	Kind        string
	TagName     string
	TypedefName string
	Size        int
	Body        string
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
				lines = append(lines, renderChunkLines(s.Chunks[idx], indent+"    ")...)
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

func renderChunkLines(chunk typeinfo.StructFieldChunk, indent string) []string {
	if chunk.Kind == typeinfo.StructFieldChunkBitfield {
		return renderBitfieldChunkLines(chunk, indent)
	}
	if len(chunk.Fields) == 0 {
		return nil
	}
	field := chunk.Fields[0]
	return []string{
		fmt.Sprintf("%s%s; /* +0x%04X (%d) */", indent, typeinfo.TypeDecl(chunk.Type, chunk.Name), field.Offset, field.Size),
	}
}

func renderBitfieldChunkLines(chunk typeinfo.StructFieldChunk, indent string) []string {
	fields := make([]typeinfo.StructField, 0, len(chunk.Fields))
	for _, field := range chunk.Fields {
		if field.Bitfield != nil {
			fields = append(fields, field)
		}
	}
	if len(fields) == 0 {
		return nil
	}

	lines := make([]string, 0, len(fields))
	for i, field := range fields {
		term := ","
		if i == len(fields)-1 {
			term = ";"
		}
		if i == 0 {
			lines = append(lines, fmt.Sprintf("%s%s %s : %d%s /* +0x%04X (%d) @bit%d */",
				indent,
				chunk.Type.String(),
				field.Name,
				field.Bitfield.BitWidth,
				term,
				chunk.Start,
				chunk.Size(),
				field.Bitfield.BitOffset,
			))
			continue
		}
		lines = append(lines, fmt.Sprintf("%s        %s : %d%s /* @bit%d */",
			indent,
			field.Name,
			field.Bitfield.BitWidth,
			term,
			field.Bitfield.BitOffset,
		))
	}
	return lines
}
