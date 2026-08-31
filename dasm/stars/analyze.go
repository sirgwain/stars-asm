package stars

import (
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/stars/ir"
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/stars/sem"
	"github.com/sirgwain/stars-asm/dasm/stars/symresolve"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type FuncAnalysis struct {
	Decoded     *asm.DecodedFunc
	CFG         *machine.CFG
	Effects     machine.FuncEffects
	Sem         sem.Func
	SemAnalysis sem.AnalyzeResult
	Annotations *sem.Result
	IR          ir.Func
	IRAnalysis  ir.AnalyzeResult
}

// analyzeFunc decodes a function and derives its CFG, machine effects, and semantic effects.
func analyzeFunc(img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpOptions) (FuncAnalysis, error) {
	return analyzeFuncWithSemPassSnapshots(img, sdb, fs, opt, nil)
}

// analyzeFuncWithSemPassSnapshots decodes and analyzes a function, reporting semantic pass snapshots.
func analyzeFuncWithSemPassSnapshots(img *asm.ImageNE, sdb *typeinfo.SymbolDB, fs *typeinfo.Function, opt DumpOptions, onPass func(sem.PassSnapshot, *machine.FuncEffects) error) (FuncAnalysis, error) {
	// decode asm
	asmCtx := asm.NewFuncContext(img, fs)
	decoded, err := asm.DecodeFunc(asmCtx)
	if err != nil {
		return FuncAnalysis{}, err
	}

	// build the cfg graph
	res := symresolve.NewResolver(img, sdb)
	ctx := machine.NewFuncContext(img, sdb, res, fs)
	cfg, err := machine.BuildCFG(ctx, decoded.Instrs, ctx.ReturnsValue(), machine.CFGOptions{CollapseJumps: !opt.PreserveJumps})
	if err != nil {
		return FuncAnalysis{}, err
	}

	// extract machine effects
	effects := machine.Extract(ctx, cfg, machine.ExtractOptions{
		FromAddr: opt.FromAddr,
		ToAddr:   opt.ToAddr,
	})

	analysis := FuncAnalysis{
		Decoded: decoded,
		CFG:     cfg,
		Effects: *effects,
	}

	// run sem passes
	semCtx := sem.NewFuncContext(img, sdb, res, fs)
	semFunc, annotations, err := sem.Lower(semCtx, effects, func(snapshot sem.PassSnapshot) error {
		if onPass == nil {
			return nil
		}
		return onPass(snapshot, effects)
	})
	if err != nil {
		return FuncAnalysis{}, err
	}

	analysis.Annotations = annotations
	analysis.Sem = semFunc
	analysis.SemAnalysis = semFunc.Analyze(semCtx)

	// lower to IR
	irFunc := ir.Lower(semFunc, fs)
	analysis.IR = irFunc
	analysis.IRAnalysis = irFunc.Analyze()

	return analysis, nil
}
