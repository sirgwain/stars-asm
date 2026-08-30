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
- Renders assembly, CFG/effect dumps, graph views, structs, enums, globals, and
  generated source-oriented output.

The decompiler is intentionally being built in layers. Each layer keeps its own
responsibility narrow enough that the output can be inspected, tested, and
improved without hiding uncertainty too early.

## Architecture

### `dasm/nb09`

The `nb09` package finds and parses the NB09 CodeView debug data embedded in the
original executable. It reads directory streams, module records, symbol streams,
type records, segment maps, public symbols, source modules, and line mappings.

This package is the raw debug-data layer. It preserves CodeView concepts closely
so later packages can decide how to interpret them.

### `dasm/typeinfo` (`symboldb`)

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

The `machine` package lifts annotated instructions into machine effects. It
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
program entities when the address, segment, and type information line up.

This package is deliberately separate from the abstract machine so address and
symbol resolution can improve without making instruction transfer depend on
source-level rendering decisions.

### `dasm/stars/sem`

The `sem` package lowers machine effects toward source-level semantic effects
and records source facts for original instruction/effect annotations. It turns
machine memory facts into named variables, fields, indices, dereferences,
offsets, and typed accesses where possible.

This is where machine facts begin to look like source facts. Keeping semantic
lowering separate from extraction lets the machine layer remain conservative and
makes it easier to inspect unresolved or partially resolved values.

### `dasm/stars/templates`

The `templates` package renders the recovered model and intermediate
representations. It produces human-readable dumps for assembly, CFGs, effects,
structs, enums, functions, globals, and source-oriented output.

Templates are the presentation boundary. They should format what the analysis
knows without inventing analysis facts themselves.

## Current Workflow

Typical commands load `dasm/input/stars.exe`, build the symbol database, decode a
function or module, run machine extraction, optionally annotate effects, and
render the requested view.

Useful entry points include:

```sh
go run main.go nb09 modules
go run main.go dasm graph -p NthValidShdef
go run main.go dasm effects -n DGetDistance --asm
go run main.go dasm all --asm
```

## Direction

The project is moving from faithful annotated disassembly toward structured,
modern C output. The target is not byte-for-byte recreation, but behaviorally
equivalent Win32 C that preserves the original game logic while replacing the
old Win16 execution environment with maintainable source code.
