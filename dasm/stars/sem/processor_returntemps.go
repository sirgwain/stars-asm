package sem

import (
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
	"slices"
)

type returnTempsProcessor struct{ fs *typeinfo.Function }

// ProcessFunc sinks a return-only temp to its complete set of immediate edge
// definitions. The return type must preserve the temp's original conversion.
func (p *returnTempsProcessor) ProcessFunc(_ *Result, f *Func) bool {
	if p.fs.NativeDecl != "" {
		return false
	}
	changed := false
	for bi, block := range f.Blocks {
		if len(block.Effects) != 1 {
			continue
		}
		ret, ok := block.Effects[0].(*Return)
		if !ok {
			continue
		}
		temp, ok := ret.Value.(*Temp)
		if !ok || !typeinfo.Equals(temp.TypeInfo, p.fs.Ret) {
			continue
		}
		defs, ok := edgeDefinitions(f, block.ID, temp)
		refs := 0
		for _, b := range f.Blocks {
			refs += countTempRefs(b.Effects, temp)
		}
		if !ok || refs != len(defs)+1 {
			continue
		}
		valid := true
		for _, def := range defs {
			a, ok := f.Blocks[def.block].Effects[def.index].(*Assign)
			if !ok || !ForwardableValue(a.Src, temp.TypeInfo) {
				valid = false
				break
			}
		}
		if !valid {
			continue
		}
		for _, def := range defs {
			a := f.Blocks[def.block].Effects[def.index].(*Assign)
			next := *ret
			next.Value = a.Src
			f.Blocks[def.block].Effects = append(slices.Clone(f.Blocks[def.block].Effects[:def.index]), &next)
		}
		f.Blocks[bi].Effects = nil
		changed = true
	}
	return changed
}
