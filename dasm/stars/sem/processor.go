package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// SemBlockProcessor processes semantic effects for one basic block.
type SemBlockProcessor interface {
	ProcessBlock(result *Result, f Func, b Block) (Block, bool)
}

// MachineBlockProcessor processes extracted machine effects for one basic block.
type MachineBlockProcessor interface {
	ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool)
}

// MachineFuncProcessor processes all extracted machine effects for a function.
type MachineFuncProcessor interface {
	ProcessMachineFunc(result *Result, f *machine.FuncEffects) bool
}

// SemFuncProcessor processes a whole semantic function.
type SemFuncProcessor interface {
	ProcessFunc(result *Result, f *Func) bool
}

// PreProcessor describes one named semantic pre-processor pass to modify machine blocks.
type PreProcessor struct {
	Name    string
	Purpose string
	Machine func(*FuncContext) MachineBlockProcessor
	Func    func(*FuncContext) MachineFuncProcessor
}

// Processor describes one named semantic processor pass.
type Processor struct {
	Name    string
	Purpose string
	Func    func(*FuncContext) SemFuncProcessor
	Sem     func(*FuncContext) SemBlockProcessor
}

// PassSnapshot captures the semantic function after one processor pass.
type PassSnapshot struct {
	Index   int
	Name    string
	Func    Func
	Result  *Result
	Effects *machine.FuncEffects
}

// PreProcessorSpecs returns the machine processor pass order.
func PreProcessorSpecs() []PreProcessor {
	return []PreProcessor{
		{
			Name:    "compiler-helpers",
			Purpose: "Lower pure compiler runtime helpers into structured machine values.",
			Machine: func(*FuncContext) MachineBlockProcessor {
				return &compilerHelpersProcessors{}
			},
		},
		{
			Name:    "normalize-assignment-addresses",
			Purpose: "Recover DGROUP addresses stored into typed near-pointer storage.",
			Machine: func(ctx *FuncContext) MachineBlockProcessor {
				return &normalizeAssignmentAddressesProcessor{ctx: ctx}
			},
		},
		{
			Name:    "normalize-shifts",
			Purpose: "Combine repeated shifts and convert non-bitwise left shifts to multiplications.",
			Machine: func(*FuncContext) MachineBlockProcessor {
				return &normalizeShiftsProcessor{}
			},
		},
		{
			Name:    "normalize-call-args",
			Purpose: "Normalize call arguments while machine word-pair structure is still available.",
			Machine: func(ctx *FuncContext) MachineBlockProcessor {
				return &normalizeCallArgsProcessor{ctx: ctx}
			},
		},
		{
			Name:    "annotate",
			Purpose: "Annotate direct machine local and global storage references.",
			Machine: func(ctx *FuncContext) MachineBlockProcessor {
				return &annotateProcessor{ctx: ctx}
			},
		},
		{
			Name:    "coalesce-copies",
			Purpose: "Coalesce adjacent contiguous machine copy effects into wider copies.",
			Machine: func(*FuncContext) MachineBlockProcessor {
				return &coalesceCopiesProcessor{}
			},
		},
		{
			Name:    "collapse-widevalues",
			Purpose: "Collapse wide values into 32 bit machine types.",
			Func: func(ctx *FuncContext) MachineFuncProcessor {
				return &collapseWideValues{ctx: ctx}
			},
		},
		{
			Name:    "collapse-widestores",
			Purpose: "Collapse adjacent contiguous machine store effects into wider stores.",
			Machine: func(ctx *FuncContext) MachineBlockProcessor {
				return &collapseWideStoresProcessor{ctx: ctx}
			},
		},
		{
			Name:    "collapse-storage-rmw",
			Purpose: "Collapse compiler scratch snapshots followed by split storage read-modify-write sequences.",
			Machine: func(ctx *FuncContext) MachineBlockProcessor {
				return &collapseStorageRMWProcessor{ctx: ctx}
			},
		},
		// {
		// 	Name:    "symbol-debug",
		// 	Purpose: "Attempt to resolve all symbols after machine pre-processing.",
		// 	Machine: func(ctx *FuncContext) MachineBlockProcessor {
		// 		return &symbolDebugPostMachineProcessor{ctx: ctx}
		// 	},
		// },
	}
}

// ProcessorSpecs returns the semantic processor pass order.
func ProcessorSpecs() []Processor {
	return []Processor{
		{
			Name:    "recover-scratch-storage",
			Purpose: "Recover scratch values and addressable objects across the function CFG.",
			Func:    func(*FuncContext) SemFuncProcessor { return &scratchRecoveryProcessor{} },
		},
		{
			Name:    "union-context",
			Purpose: "Resolve union context for dependent enums.",
			Func: func(ctx *FuncContext) SemFuncProcessor {
				return &unionContextProcessor{ctx: ctx}
			},
		},
		{
			Name:    "resolve-late-addresses",
			Purpose: "Project typed addresses exposed by semantic scratch substitution.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveLateAddressesProcessor{ctx: ctx}
			},
		},
		{
			Name:    "resolve-late-fields",
			Purpose: "Resolve ordinary declared fields hidden inside over-wide semantic accesses.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveLateFieldsProcessor{ctx: ctx}
			},
		},
		{
			Name:    "resolve-late-bitfields",
			Purpose: "Resolve declared bitfields exposed by semantic scratch-slot substitution.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveLateBitfieldsProcessor{ctx: ctx}
			},
		},
		{
			Name:    "resolve-enums",
			Purpose: "Resolve enum-typed constants and constrained call result types.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveEnumsProcessor{ctx: ctx}
			},
		},
		{
			Name:    "return-sink",
			Purpose: "Sink top-level return merge arms into predecessor blocks.",
			Func: func(*FuncContext) SemFuncProcessor {
				return &returnSinkProcessor{}
			},
		},
		{
			Name:    "lower-merges",
			Purpose: "Lower semantic merge expressions to typed temporaries on incoming CFG edges.",
			Func: func(*FuncContext) SemFuncProcessor {
				return &lowerMergesProcessor{}
			},
		},
		{
			Name:    "coalesce-wide-temps",
			Purpose: "Coalesce merge temps that hold low/high parts of the same wide value.",
			Func: func(ctx *FuncContext) SemFuncProcessor {
				return &coalesceWideTempsProcessor{ctx: ctx}
			},
		},
		{
			Name:    "materialize-call-results",
			Purpose: "Materialize non-inline call results that remain in later semantic expressions.",
			Func: func(*FuncContext) SemFuncProcessor {
				return &callResultProcessor{}
			},
		},
		{
			Name:    "resolve-const-types",
			Purpose: "Resolve consts to their semantic types.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveConstTypesProcessor{}
			},
		},
		{
			Name:    "remove-empty-blocks",
			Purpose: "Remove unreferenced empty semantic blocks.",
			Func: func(*FuncContext) SemFuncProcessor {
				return &removeEmptyBlocksProcessor{}
			},
		},
	}
}

// ProcessorRunner applies configured lowering processors to a function.
type ProcessorRunner struct {
	ctx            *FuncContext
	fn             *Func
	preSpecs       []PreProcessor
	specs          []Processor
	onPass         func(PassSnapshot) error
	machineEffects *machine.FuncEffects
}

// NewProcessorRunner creates a semantic processor runner.
func NewProcessorRunner(ctx *FuncContext, fn *Func) *ProcessorRunner {
	return &ProcessorRunner{ctx: ctx, fn: fn, preSpecs: PreProcessorSpecs(), specs: ProcessorSpecs()}
}

// WithMachineEffects gives the runner extracted machine effects to process before semantic passes.
func (runner *ProcessorRunner) WithMachineEffects(effects *machine.FuncEffects) *ProcessorRunner {
	runner.machineEffects = effects.Clone()
	return runner
}

// WithPassSnapshots installs a callback invoked after each semantic processor pass.
func (runner *ProcessorRunner) WithPassSnapshots(onPass func(PassSnapshot) error) *ProcessorRunner {
	runner.onPass = onPass
	return runner
}

// PreRun applies machine processors before semantic conversion.
func (runner *ProcessorRunner) PreRun(result *Result) error {
	for i, spec := range runner.preSpecs {
		if spec.Func != nil {
			processor := spec.Func(runner.ctx)
			processor.ProcessMachineFunc(result, runner.machineEffects)
		}
		if spec.Machine != nil {
			processor := spec.Machine(runner.ctx)
			for i := range runner.machineEffects.Blocks {
				runner.ctx.SetCurrentBlock(runner.machineEffects.Blocks[i].Block)

				next, changed := processor.ProcessMachineBlock(result, *runner.machineEffects, runner.machineEffects.Blocks[i])
				if changed {
					runner.machineEffects.Blocks[i] = next
				}
			}
		}

		if runner.onPass != nil {
			if err := runner.onPass(PassSnapshot{
				Index:   i + 1,
				Name:    spec.Name,
				Result:  result,
				Effects: runner.machineEffects,
			}); err != nil {
				return err
			}
		}

	}
	runner.ctx.ClearCurrentBlock()
	return nil
}

// Run applies semantic processors to the runner's function.
func (runner *ProcessorRunner) Run(result *Result) error {
	for i, spec := range runner.specs {

		if spec.Sem != nil {
			processor := spec.Sem(runner.ctx)
			for i := range runner.fn.Blocks {
				runner.ctx.SetCurrentBlock(runner.fn.Blocks[i].ID)
				next, changed := processor.ProcessBlock(result, *runner.fn, runner.fn.Blocks[i])
				if changed {
					runner.fn.Blocks[i] = next
				}
			}
			runner.ctx.ClearCurrentBlock()
		}
		if spec.Func != nil {
			processor := spec.Func(runner.ctx)
			processor.ProcessFunc(result, runner.fn)
		}
		if runner.onPass != nil {
			if err := runner.onPass(PassSnapshot{
				Index:  i + 1 + len(runner.preSpecs),
				Name:   spec.Name,
				Func:   *runner.fn,
				Result: result,
			}); err != nil {
				return err
			}
		}
	}
	return nil
}

// Lower processes machine effects and reports each semantic pass result.
func Lower(ctx *FuncContext, effects *machine.FuncEffects, onPass func(PassSnapshot) error) (Func, *Result, error) {
	result := newResult(ctx.fs)
	runner := NewProcessorRunner(ctx, nil).WithMachineEffects(effects).WithPassSnapshots(onPass)

	// preprocess the machine effects
	runner.PreRun(result)

	// convert preprocessed machine effects to sem effects
	runner.fn = convertMachineToFunc(ctx, result, runner.machineEffects)
	if err := runner.Run(result); err != nil {
		return Func{}, nil, err
	}

	return *runner.fn, result, nil
}

// newResult creates an empty semantic lowering and back-annotation result.
func newResult(function *typeinfo.Function) *Result {
	return &Result{
		Operands: make(map[machine.AnnotationKey]Annotation),
		Memory:   make(map[string][]Annotation),
		Values:   make(map[string]Annotation),
		function: function,
	}
}
