package templates

import (
	"fmt"
	"io"
	"strings"
	"text/template"

	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type DumpEffectsView struct {
	Options     DumpEffectsOptions
	Styles      DumpStyles
	CFG         *machine.CFG
	Blocks      []DumpEffectsBlockView
	SourceFile  string
	SourceLines []typeinfo.SourceLine
	Annotations InstructionAnnotations
}

type DumpEffectsBlockView struct {
	DumpAsmBlockView
	ID      string
	Insts   string
	Offsets string
	Preds   []string
	Succs   []string
	Effects []string
}

func NewDumpEffectsView(effects *machine.FuncEffects, opt DumpEffectsOptions, function *typeinfo.Function, annotations *sem.Result) DumpEffectsView {
	view := DumpEffectsView{
		Options: opt,
		CFG:     effects.CFG,
	}
	if annotations != nil {
		view.Annotations = annotations
	}
	if function != nil {
		view.SourceFile = function.SourceFile.File
		view.SourceLines = function.SourceFile.Lines
	}

	effectsByBlock := make(map[machine.BlockID][]machine.Effect, len(effects.Blocks))
	for _, blockEffects := range effects.Blocks {
		effectsByBlock[blockEffects.Block] = blockEffects.Effects
	}

	for _, block := range effects.CFG.BlocksInRange(machine.BlockRange{
		FromAddr: opt.FromAddr,
		ToAddr:   opt.ToAddr,
	}) {
		view.Blocks = append(view.Blocks, DumpEffectsBlockView{
			DumpAsmBlockView: DumpAsmBlockView{
				Label:  block.Label,
				Off:    uint32(block.ID),
				Instrs: effects.CFG.BlockInstrs(block.ID),
			},
			ID:      block.ID.String(),
			Insts:   fmt.Sprintf("%d..%d", block.StartIdx, block.EndIdx-1),
			Offsets: fmt.Sprintf("0x%04x..0x%04x", uint32(block.ID), block.EndOff-1),
			Preds:   formatBlockIDs(effects.CFG.Predecessors(block.ID)),
			Succs:   formatBlockIDs(effects.CFG.Successors(block.ID)),
			Effects: formatEffects(effectsByBlock[block.ID], annotations),
		})
	}

	return view
}

func RenderDumpEffects(w io.Writer, view DumpEffectsView) error {
	t := template.New("dump_effects.templ").
		Funcs(template.FuncMap{
			"join":           strings.Join,
			"blockInstsView": newDumpAsmBlockInstsView,
			"renderAsmBlock": renderDumpEffectsAsmBlock,
			"highlightLines": HighlightLines,
			"highlightText":  HighlightTextLines,
			"renderBytes":    func(styles DumpStyles, b []byte) string { return styles.Bytes.Render(hexBytes(b)) },
			"asmCommentLine": asmCommentLine,
			"sourceComments": sourceComments,
		}).
		Funcs(asmTemplateFuncs())

	tmpl, err := t.ParseFS(templatesFS, "assets/dump_effects.templ", "assets/dump_asm_block_insts.templ")
	if err != nil {
		return err
	}

	return tmpl.Execute(w, view)
}

func renderDumpEffectsAsmBlock(root DumpEffectsView, block DumpEffectsBlockView) (string, error) {
	var buf strings.Builder
	if comments := sourceComments(root.SourceFile, root.SourceLines, block.Off); len(comments) > 0 {
		label := block.Label + ":"
		for _, comment := range comments {
			fmt.Fprintln(&buf, asmCommentLine(label, comment))
			label = ""
		}
	} else {
		fmt.Fprintf(&buf, "%s:\n", block.Label)
	}

	if err := RenderDumpAsmBlockInsts(&buf, newDumpAsmBlockInstsView(root, block)); err != nil {
		return "", err
	}
	return strings.TrimRight(buf.String(), "\n"), nil
}

func formatEffects(effects []machine.Effect, annotations *sem.Result) []string {
	if len(effects) == 0 {
		return nil
	}
	ctx := newEffectFormatContext(effects, annotations)
	lines := make([]string, 0, len(effects))
	for _, effect := range effects {
		lines = append(lines, ctx.formatEffect(effect))
	}
	return lines
}

type effectFormatContext struct {
	annotations    *sem.Result
	displayLoadIDs map[string]bool
}

// newEffectFormatContext computes which load identities are useful in a block dump.
func newEffectFormatContext(effects []machine.Effect, annotations *sem.Result) effectFormatContext {
	ctx := effectFormatContext{
		annotations:    annotations,
		displayLoadIDs: make(map[string]bool),
	}
	writes := make(map[string]uint32)
	loadIDs := make(map[string]map[machine.ValueID]struct{})
	for _, effect := range effects {
		switch e := effect.(type) {
		case machine.StoreEffect:
			ctx.noteValueLoads(e.Src, writes, loadIDs)
			writes[effectMemoryKey(e.Addr, e.Width)] = e.MetaInfo.InstOff
		case machine.CopyEffect:
			ctx.noteValueLoads(e.Dst, writes, loadIDs)
			ctx.noteValueLoads(e.Src, writes, loadIDs)
		case machine.CallEffect:
			for _, arg := range e.Args {
				ctx.noteValueLoads(arg, writes, loadIDs)
			}
			ctx.noteValueLoads(e.Result, writes, loadIDs)
		case machine.BranchEffect:
			ctx.noteValueLoads(e.Predicate, writes, loadIDs)
		case machine.ReturnEffect:
			ctx.noteValueLoads(e.Value, writes, loadIDs)
		}
	}
	for key, ids := range loadIDs {
		if len(ids) > 1 {
			ctx.displayLoadIDs[key] = true
		}
	}
	return ctx
}

// noteValueLoads records load identities and whether any load is stale after a write.
func (ctx effectFormatContext) noteValueLoads(value machine.Value, writes map[string]uint32, loadIDs map[string]map[machine.ValueID]struct{}) {
	switch v := value.(type) {
	case nil:
		return
	case *machine.PredicateValue:
		ctx.noteValueLoads(v.LHS, writes, loadIDs)
		ctx.noteValueLoads(v.RHS, writes, loadIDs)
	case *machine.WordValue:
		ctx.noteValueLoads(v.Parent, writes, loadIDs)
	case *machine.FarPointer:
		ctx.noteValueLoads(v.Parent, writes, loadIDs)
		ctx.noteValueLoads(v.Offset, writes, loadIDs)
		ctx.noteValueLoads(v.Segment, writes, loadIDs)
	case *machine.SignExtendValue:
		ctx.noteValueLoads(v.Parent, writes, loadIDs)
	case *machine.StackWords:
		for _, word := range v.Words {
			ctx.noteValueLoads(word, writes, loadIDs)
		}
	case *machine.ByteValue:
		ctx.noteValueLoads(v.Parent, writes, loadIDs)
		if v.Value != nil {
			ctx.noteValueLoads(*v.Value, writes, loadIDs)
		}
	case *machine.Cast:
		ctx.noteValueLoads(v.Value, writes, loadIDs)
	case *machine.Binary:
		ctx.noteValueLoads(v.LHS, writes, loadIDs)
		ctx.noteValueLoads(v.RHS, writes, loadIDs)
	case *machine.Load:
		if v.ID.IsZero() {
			return
		}
		key := effectMemoryKey(v.Access, v.Access.Width)
		if loadIDs[key] == nil {
			loadIDs[key] = make(map[machine.ValueID]struct{})
		}
		loadIDs[key][v.ID] = struct{}{}
		if writeOff, ok := writes[key]; ok && v.ID.InstOff < writeOff {
			ctx.displayLoadIDs[key] = true
		}
	case *machine.PhiValue:
		for _, arm := range v.Arms {
			ctx.noteValueLoads(arm.Value, writes, loadIDs)
		}
	}
}

func (ctx effectFormatContext) formatEffect(effect machine.Effect) string {
	if effect == nil {
		return "<nil effect>"
	}

	meta := effect.EffectMeta()
	prefix := fmt.Sprintf("%04x  ", meta.InstOff)

	switch e := effect.(type) {
	case machine.StoreEffect:
		return fmt.Sprintf("%sstore[%d] %s = %s", prefix, e.Width, ctx.formatMemoryAccess(e.Addr), ctx.formatValue(e.Src))
	case machine.CopyEffect:
		return fmt.Sprintf("%scopy[%#x] *%s = *%s", prefix, e.Width, ctx.formatValue(e.Dst), ctx.formatValue(e.Src))
	case machine.CallEffect:
		line := fmt.Sprintf("%scall %s(%s)", prefix, ctx.formatCallTarget(e), ctx.formatValues(e.Args))
		if e.Result != nil {
			line += fmt.Sprintf(" -> %s", ctx.formatValue(e.Result))
		}
		return line
	case machine.BranchEffect:
		return fmt.Sprintf("%sbranch %s ? %s : %s", prefix, ctx.formatValue(e.Predicate), e.TrueBlock, e.FalseBlock)
	case machine.JumpEffect:
		return fmt.Sprintf("%sgoto %s", prefix, e.To)
	case machine.ReturnEffect:
		if e.Value == nil {
			return prefix + "return"
		}
		return fmt.Sprintf("%sreturn %s", prefix, ctx.formatValue(e.Value))
	case machine.UnknownEffect:
		return fmt.Sprintf("%sunknown %s: %s", prefix, e.Inst.Mnemonic, e.Why)
	default:
		return fmt.Sprintf("%s%v", prefix, effect)
	}
}

func formatBlockIDs(ids []machine.BlockID) []string {
	if len(ids) == 0 {
		return nil
	}
	parts := make([]string, len(ids))
	for i, id := range ids {
		parts[i] = id.String()
	}
	return parts
}

func (ctx effectFormatContext) formatValues(values []machine.Value) string {
	if len(values) == 0 {
		return ""
	}
	parts := make([]string, len(values))
	for i, value := range values {
		parts[i] = ctx.formatValue(value)
	}
	return strings.Join(parts, ", ")
}

func (ctx effectFormatContext) formatValue(value machine.Value) string {
	if ctx.annotations != nil || len(ctx.displayLoadIDs) > 0 {
		if rendered, ok := ctx.formatValueWithIDs(value); ok {
			return rendered
		}
	}
	if ctx.annotations == nil {
		return fmt.Sprint(value)
	}
	return ctx.annotations.Value(value)
}

// formatValueWithIDs renders values whose load identity matters in effects dumps.
func (ctx effectFormatContext) formatValueWithIDs(value machine.Value) (string, bool) {
	switch v := value.(type) {
	case *machine.PredicateValue:
		if v.Kind == machine.PredicateCompare {
			return fmt.Sprintf("%s %s %s", ctx.formatValue(v.LHS), machine.JccCompareOp(v.Op), ctx.formatValue(v.RHS)), true
		}
		if ctx.annotations != nil {
			return ctx.annotations.Value(value), true
		}
		return fmt.Sprint(value), true
	case *machine.Binary:
		return fmt.Sprintf("(%s %s %s)", ctx.formatValue(v.LHS), v.Op, ctx.formatValue(v.RHS)), true
	case *machine.Load:
		if v.ID.IsZero() || !ctx.displayLoadIDs[effectMemoryKey(v.Access, v.Access.Width)] {
			return "", false
		}
		return fmt.Sprintf("load%s(%s)", v.ID, ctx.formatMemoryAccess(v.Access)), true
	default:
		return "", false
	}
}

func (ctx effectFormatContext) formatMemoryAccess(access machine.MemoryAccess) string {
	if ctx.annotations == nil {
		return access.String()
	}
	return ctx.annotations.MemoryAccess(access)
}

// effectMemoryKey returns a storage key for load identity display decisions.
func effectMemoryKey(access machine.MemoryAccess, width int) string {
	access.Origin = machine.Origin{}
	access.Width = width
	return access.String()
}

func formatCallTarget(target *typeinfo.Function) string {
	if target == nil {
		return "<nil>"
	}
	return target.Name
}

// formatCallTarget renders direct and indirect machine call targets.
func (ctx effectFormatContext) formatCallTarget(effect machine.CallEffect) string {
	if effect.Target != nil {
		return formatCallTarget(effect.Target)
	}
	if emptyCallTargetMemory(effect.MemoryAccess) {
		return "<nil>"
	}
	access := effect.MemoryAccess
	access.Width = 4
	return ctx.formatMemoryAccess(access)
}

// emptyCallTargetMemory reports whether a machine call has no indirect target memory.
func emptyCallTargetMemory(access machine.MemoryAccess) bool {
	return access.Seg == nil &&
		access.Base == nil &&
		access.Index == nil &&
		access.Disp == 0 &&
		access.Width == 0 &&
		access.Scale == 0 &&
		access.Origin == (machine.Origin{})
}
