package sem

import (
	"github.com/sirgwain/stars-asm/dasm/stars/machine"
)

// SemBlockProcessor processes semantic effects for one basic block.
type SemBlockProcessor interface {
	ProcessBlock(result *Result, f Func, b Block) (Block, bool)
}

// MachineBlockProcessor processes extracted machine effects for one basic block.
type MachineBlockProcessor interface {
	ProcessMachineBlock(result *Result, f machine.FuncEffects, b machine.BlockEffects) (machine.BlockEffects, bool)
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
	Index  int
	Name   string
	Func   Func
	Result *Result
}

// ProcessorSpecs returns the semantic processor pass order.
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
			Name:    "annotate-machine-storage",
			Purpose: "Annotate direct machine local and global storage references.",
			Machine: func(ctx *FuncContext) MachineBlockProcessor {
				return &annotateMachineStorageProcessor{ctx: ctx}
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
			Name:    "coalesce-copies",
			Purpose: "Coalesce adjacent contiguous machine copy effects into wider copies.",
			Machine: func(*FuncContext) MachineBlockProcessor {
				return &coalesceCopiesProcessor{}
			},
		},
	}
}

// ProcessorSpecs returns the semantic processor pass order.
func ProcessorSpecs() []Processor {
	return []Processor{
		{
			Name:    "resolve-storage",
			Purpose: "Resolve local and global storage references.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveStorageProcessor{ctx: ctx}
			},
		},
		{
			Name:    "elide-scratch-slots",
			Purpose: "Inline simple compiler scratch stack slots within a block.",
			Sem:     func(*FuncContext) SemBlockProcessor { return &elideScratchSlotsProcessor{} },
			Func:    func(*FuncContext) SemFuncProcessor { return &elideScratchSlotsProcessor{} },
		},
		{
			Name:    "resolve-storage",
			Purpose: "Resolve semantic memory references exposed by scratch-slot elision.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveStorageProcessor{ctx: ctx}
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
			Name:    "resolve-storage",
			Purpose: "Resolve storage references before deriving path-sensitive contexts.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveStorageProcessor{ctx: ctx}
			},
		},
		{
			Name:    "resolve-enums",
			Purpose: "Resolve enum constants using path-sensitive context.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveEnumsProcessor{ctx: ctx}
			},
		},
		{
			Name:    "establish-union-context",
			Purpose: "Establish path-sensitive union and enum contexts for the whole function.",
			Func: func(ctx *FuncContext) SemFuncProcessor {
				return &unionContextProcessor{ctx: ctx}
			},
		},
		{
			Name:    "resolve-storage",
			Purpose: "Resolve union-sensitive storage references.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveStorageProcessor{ctx: ctx}
			},
		},
		{
			Name:    "resolve-enums",
			Purpose: "Resolve enum constants using path-sensitive context.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &resolveEnumsProcessor{ctx: ctx}
			},
		},
		{
			Name:    "collapse-wide-stores",
			Purpose: "Collapse adjacent low/high word stores and matching wide word expressions.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &collapseWideStoresProcessor{ctx: ctx}
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
			Name:    "normalize-call-args",
			Purpose: "Normalize semantic call arguments before merge lowering.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &normalizeCallArgsProcessor{ctx: ctx}
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
			Name:    "collapse-wide-stores",
			Purpose: "Collapse wide stores exposed by coalesced merge temps.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &collapseWideStoresProcessor{ctx: ctx}
			},
		},
		{
			Name:    "collapse-call-results",
			Purpose: "Inline single-use call results into their consuming semantic effect.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &collapseCallResultsProcessor{ctx: ctx}
			},
		},
		{
			Name:    "normalize-call-args",
			Purpose: "Normalize semantically annotated call arguments.",
			Sem: func(ctx *FuncContext) SemBlockProcessor {
				return &normalizeCallArgsProcessor{ctx: ctx}
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
	machineEffects *machine.FuncEffects
	preSpecs       []PreProcessor
	specs          []Processor
	onPass         func(PassSnapshot) error
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

// Run applies semantic processors to the runner's function.
func (runner *ProcessorRunner) PreRun(result *Result) error {
	for _, spec := range runner.preSpecs {

		processor := spec.Machine(runner.ctx)
		for i := range runner.machineEffects.Blocks {
			next, changed := processor.ProcessMachineBlock(result, *runner.machineEffects, runner.machineEffects.Blocks[i])
			if changed {
				runner.machineEffects.Blocks[i] = next
			}
		}
	}
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
				Index:  i + 1,
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
	result := newResult()
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
func newResult() *Result {
	return &Result{
		Operands: make(map[machine.AnnotationKey]Annotation),
		Memory:   make(map[string]Annotation),
		Values:   make(map[string]Annotation),
	}
}
