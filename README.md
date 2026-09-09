# stars-asm

`stars-asm` is a reverse-engineering and decompilation toolkit for the original
`Stars! 2.7j` Win16 executable.

The project reads the original executable, extracts its CodeView NB09 debug
information, disassembles Win16 code, annotates machine instructions with known
symbols and types, and incrementally lifts instruction streams into higher-level
effects. The long-term goal is to produce modern Win32 C code that is
functionality equivalent to the original game code.

This is not a generic decompiler. It is a focused reconstruction tool for one
program and one toolchain lineage. That lets the code use domain knowledge from
the `Stars!` binary, its debug records, its memory model, and its compiler helper
patterns instead of treating every instruction as anonymous machine code.

## What It Does

At a high level, `stars-asm`:

- Loads the `stars.exe` NE image and locates the embedded NB09 debug database.
- Parses CodeView symbol and type records into a typed program model.
- Builds a symbol database containing functions, globals, structs, enums,
  publics, source ranges, locals, parameters, and block labels.
- Decodes reachable 16-bit x86 instructions for known functions.
- Builds control-flow graphs and strongly connected component information.
- Runs an abstract machine over basic blocks to recover calls, stores, branches,
  returns, stack arguments, register arguments, x87 values, and merged values at
  CFG joins.
- Annotates low-level memory/register effects with source-level symbols where
  the type and address information is strong enough.
- Runs semantic passes to recover typed expressions, scratch storage, union
  selections, enums, and explicit temporaries for merged values.
- Lowers semantic effects into C-like IR with explicit basic blocks and gotos,
  retaining diagnostics for effects that cannot yet be translated.
- Renders assembly, CFG/effect dumps, graph views, structs, enums, globals, and
  generated source-oriented output.

The decompiler is intentionally being built in layers. Each layer keeps its own
responsibility narrow enough that the output can be inspected, tested, and
improved without hiding uncertainty too early.

## Architecture

The function analysis pipeline is:

```text
asm decoding → CFG and machine effects → sem machine preprocessing
             → semantic conversion and passes → C-like IR → rendering
```

The symbol database and executable image support analysis throughout the
pipeline. Semantic annotations also feed back into assembly and effect views.

### `cmd`, `dasm/starsenv`, and `dasm/stars`

The `cmd` package exposes the CLI. `starsenv` loads the NB09 database, typed
symbols and overrides, NE image, and `.exports` tables for imported functions from the input
directory into a shared environment.

The `stars` package orchestrates function analysis, retaining decoded
instructions, the CFG, machine effects, semantic effects and annotations, and
IR together in `FuncAnalysis`. It also coordinates bulk output, analysis
reports, and recovery of global initializers from static executable data.

### `dasm/nb09`

The `nb09` package finds and parses the NB09 CodeView debug data embedded in the
original executable. It reads directory streams, module records, symbol streams,
type records, segment maps, public symbols, source modules, and line mappings.

This package is the raw debug-data layer. It preserves CodeView concepts closely
so later packages can decide how to interpret them.

### `dasm/typeinfo`

The `typeinfo` package converts NB09 records into the project’s typed program
model. Its `SymbolDB` is the central index for functions, globals, structs,
enums, publics, modules, source ranges, and source lines.

This layer resolves CodeView type records into Go structures representing C-like
types, including primitives, pointers, arrays, functions, structs, unions,
bitfields, and enum usage rules. It also applies project-specific overrides when
the debug data needs correction or clarification.

### `dasm/stars/asm`

The `asm` package owns NE image access and 16-bit x86 disassembly. It loads the
Win16 executable, maps segment/offset addresses, reads relocation fixups, decodes
instructions, identifies reachable code, and records instruction operands in a
structured form.

This is the lowest executable-code layer. It knows about bytes, segments,
fixups, opcodes, operands, jump targets, and imported/public call targets.

### `dasm/stars/machine`

The `machine` package lifts decoded instructions into machine effects. It
builds CFG blocks, computes liveness, walks strongly connected components,
merges predecessor state, widens loop-carried values to reach fixpoints, and
tracks an abstract machine state for registers, stack words, flags, x87 stack
values, calls, stores, branches, jumps, and returns.

This layer stays machine-oriented. Values such as loads, addresses, word/byte
projections, call results, predicates, and CFG merges are represented explicitly
so downstream passes can decide how much source-level meaning is justified.

### `dasm/stars/symresolve`

The `symresolve` package resolves concrete addresses and literals against the
symbol database and executable image. It bridges raw memory references to known
program entities when the address, segment, and type information line up. It
also defines symbolic access paths and supports field resolution using explicit
union context. Resolution of compound machine address expressions lives in
`sem`, which uses these shared lookup and path facilities.

This package is deliberately separate from the abstract machine so address and
symbol resolution can improve without making instruction transfer depend on
source-level rendering decisions.

### `dasm/stars/sem`

The `sem` package owns an ordered lowering pipeline with three stages:

1. Preprocess machine effects to recognize compiler helpers, normalize addresses,
   shifts, and call arguments, annotate storage, and combine copies, wide values,
   stores, and storage read-modify-write patterns.
2. Convert machine effects into semantic expressions and effects, resolving named
   variables, fields, indices, dereferences, offsets, and typed accesses.
3. Run semantic passes to recover scratch storage, propagate union context,
   resolve late addresses and bitfields, resolve enums and constant types, lower
   merges into temporaries on incoming CFG edges, materialize call results, and
   remove unreferenced empty blocks.

The pass order is defined in `sem/processor.go`. Per-pass snapshots support
inspection and diffs, while source annotations preserve connections to original
instruction operands and machine values.

This is where machine facts begin to look like source facts. Keeping semantic
lowering separate from extraction lets the machine layer remain conservative and
makes it easier to inspect unresolved or partially resolved values.

### `dasm/stars/ir`

The `ir` package lowers semantic effects into a separate C-like representation
with local declarations, assignments, calls, returns, explicit basic blocks,
conditional gotos, and table jumps. Unsupported effects retain comments and
lowering diagnostics, which are included in analysis reports.

This IR is deliberately low-level. Structured control-flow recovery belongs to
later work; current C output still exposes labels and gotos.

### `dasm/stars/templates`

The `templates` package renders the recovered model and intermediate
representations. It produces human-readable dumps for assembly, CFGs, machine
and semantic effects, structs, enums, functions, globals, and C-like IR. Bulk
source generation uses rendered IR bodies for module C files.

Templates are the presentation boundary. They should format what the analysis
knows without inventing analysis facts themselves.

### `dasm/stars/graphview`

The `graphview` package hosts the interactive Wails/Cytoscape graph viewer.
`stars` builds its graph data from function analysis, and `dasm graph --view`
launches the viewer for navigating blocks and inspecting recovered effects.

## Current Workflow

Disassembly commands load `dasm/input/stars.exe` and supporting input data through
`starsenv`. The shared function analysis path decodes instructions, builds the
CFG, extracts machine effects, runs semantic lowering and annotations, and
generates IR. Each command renders the requested view of that analysis.

Useful entry points include:

```sh
go run main.go nb09 modules
go run main.go dasm graph -p NthValidShdef
go run main.go dasm effects -n DGetDistance --asm
go run main.go dasm sem -n DGetDistance --asm --effects
go run main.go dasm sem -n DGetDistance --diff
go run main.go dasm ir -n DGetDistance
go run main.go dasm graph -p NthValidShdef --view
go run main.go dasm all --asm
```

`dasm sem --diff` writes per-pass dumps to `dist` and shows changed pass diffs.
Semantic analysis is available through `dasm sem --analyze`; bulk output also
includes analysis reports and generated union block facts.

## Direction

The project now produces C-like output through explicit-block IR and is moving
toward structured, modern C output. The target is not byte-for-byte recreation,
but behaviorally equivalent Win32 C that preserves the original game logic while replacing the
old Win16 execution environment with maintainable source code.
