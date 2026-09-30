# stars-asm

`stars-asm` is a reverse-engineering and decompilation toolkit for the original
`Stars! 2.7j` Win16 executable.

The project reads the original executable, extracts its CodeView NB09 debug
information, disassembles Win16 code, annotates machine instructions with known
symbols and types, and incrementally lifts instruction streams into higher-level
effects and structured C. The long-term goal is to produce modern Win32 C code
that is functionally equivalent to the original game code.

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
- Recovers structured conditionals, loops, and switches from IR, keeping labels
  and gotos for control flow that cannot be structured.
- Adapts recovered code for native Windows types, layouts, and API conventions.
- Regenerates resource scripts, headers, and assets from the original NE image.
- Renders assembly, CFG/effect dumps, graph views, structs, enums, globals, and
  generated source-oriented output.

The decompiler is intentionally being built in layers. Each layer keeps its own
responsibility narrow enough that the output can be inspected, tested, and
improved without hiding uncertainty too early.

## Architecture

The function analysis pipeline is:

```text
asm decoding → CFG and machine effects → sem machine preprocessing
             → semantic conversion and passes → C-like IR
             → structured regions → C rendering
```

The symbol database and executable image support analysis throughout the
pipeline. Semantic annotations also feed back into assembly and effect views.
Assembly, machine effects, semantic effects, and explicit-block IR can each be
rendered separately for inspection.

### `cmd`, `dasm/starsenv`, and `dasm/stars`

The `cmd` package exposes the CLI. `starsenv` loads the NB09 database, typed
symbols and overrides, NE image, and `.exports` tables for imported functions
from the input directory into a shared environment.

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

Semantic passes also adapt code for the native Windows build. These include
replacing recovered struct byte counts with `sizeof`, preserving Win16 file
record layouts, converting point and edit-selection arguments at Win32 API
boundaries, and recovering arrays from parameters accessed as adjacent words.

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

The explicit-block IR remains available through `dasm ir` for analysis and
feeds the structured control-flow recovery layer.

### `dasm/stars/region`

The `region` package rebuilds IR control flow as structured conditionals, loops,
and switches. It threads jumps, merges conditions, recovers switch chains, and
uses dominance and loop information to construct a tree of regions. Loops can
render as `while`, `do`/`while`, or `for`, with `break` and `continue` where
appropriate.

Control flow that cannot be structured retains labels and gotos. `dasm region`
renders this representation as C, and bulk C generation uses the same path.

### `dasm/stars/templates`

The `templates` package renders the recovered model and intermediate
representations. It produces human-readable dumps for assembly, CFGs, machine
and semantic effects, structs, enums, functions, globals, C-like IR, and
structured regions. Bulk source generation uses rendered region bodies for
module C files. C rendering uses `clang-format` and the repository's style file.

Templates are the presentation boundary. They should format what the analysis
knows without inventing analysis facts themselves.

### `dasm/stars/resources`

The `resources` package regenerates dialogs, menus, and accelerators as resource
script statements, and writes icons, cursors, bitmaps, and data as asset files.
Resource ID enums supply names for the generated `resource.h` header.
`dasm all --c` and `dasm all --all` write these outputs under `decompiled/res/`
by default, alongside the generated C sources.

### `dasm/stars/graphview`

The `graphview` package hosts the interactive Wails/Cytoscape graph viewer.
`stars` builds its graph data from function analysis, and `dasm graph --view`
launches the viewer for navigating blocks and inspecting recovered effects.

## Current Workflow

### CLI prerequisites

- Go 1.25.0 or newer, as declared in `go.mod`.
- `clang-format` on `PATH` for commands that render C and for the corresponding
  tests.
- The original `Stars! 2.7j` executable at `dasm/input/stars.exe`, with the
  supporting input files in that directory.

Run the commands below from the repository root. The CLI's `--input-path` flag
selects the directory loaded by `starsenv` (default: `./dasm/input`).

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
go run main.go dasm region -n DGetDistance
go run main.go dasm graph -p NthValidShdef --view
go run main.go dasm all --asm
go run main.go dasm all --c
```

`dasm sem --diff` writes per-pass dumps to `dist` and shows changed pass diffs.
Semantic analysis is available through `dasm sem --analyze`; bulk output also
includes analysis reports and generated union block facts.

`dasm all --c` generates structured C, headers, and resources. `--ir` writes
separate per-function IR dumps under `decompiled/ir/`; `--all` enables every
output format. Use `--out` to change the output directory from `./decompiled`.

### Development commands

```sh
make build             # Build dist/stars-asm
make test              # Run Go unit tests and refresh decompiler snapshots
make compile-analysis  # Write C/resource diagnostics without failing on errors
make compile-check     # Write diagnostics and fail on C/resource errors
```

Unit tests write snapshots under `dasm/stars/testdata/snapshots/`; review and
keep changes caused by intentional output updates. The compile targets require
MinGW-w64 and write `decompiled/compile-analysis.json` for the existing generated
sources. Regenerate the sources first when checking decompiler changes.

## Build and Run `stars.exe`

The CMake build compiles the generated C sources in `decompiled/` and embeds
the resources from `decompiled/res/`. It produces an x86-64 Windows GUI
executable with debug information, using MinGW-w64 and Wine to run it.

### Prerequisites

Make these tools available on `PATH`:

- CMake 3.23 or newer.
- Ninja.
- MinGW-w64's `x86_64-w64-mingw32-gcc`, `x86_64-w64-mingw32-windres`, and
  associated binutils.
- Wine, available as `wine`. CMake requires it during configuration.

### Build

Run from the repository root:

```sh
cmake --preset mingw-debug
cmake --build --preset mingw-debug
```

The executable is written to `dist/mingw-debug/bin/stars.exe`. Build files and
`compile_commands.json` are stored in `dist/mingw-debug/`.

CMake uses the existing generated sources. After changing the decompiler,
regenerate them with Go before rebuilding:

```sh
go run main.go dasm all --all
cmake --build --preset mingw-debug
```

### Run with Wine

From the repository root:

```sh
cmake --build --preset run-wine
```

This builds any pending changes and launches `stars.exe` with
`dist/mingw-debug/bin/` as its working directory. To launch the existing
executable directly, including any command-line arguments, use:

```sh
cd dist/mingw-debug/bin
wine stars.exe
```

The generated program is still under reconstruction. A successful build does
not establish that all game behavior works correctly under Wine.

### Game command-line arguments

The game parses these arguments in [`WinMain`](decompiled/stars.c), with startup
actions handled in [`FrameWndProc`](decompiled/mdi.c). These are arguments to
`stars.exe`, separate from the Go decompiler CLI described above.

```text
stars.exe [options] [game-file]
stars.exe -a definition-file
stars.exe -b batch-file [options]
```

Switch letters are case-insensitive and accept either `-` or `/` (`-g`, `/G`).
Simple switches can be grouped, such as `-gl`. Only literal spaces separate
arguments; the game parser does not handle quotes or escaped spaces. Use paths
without spaces and Windows path syntax under Wine (for example,
`'C:\games\demo.hst'` in a shell). Unknown switch characters are silently ignored;
there is no help switch, and `-h` means hot-seat mode.

| Argument | Effect |
| --- | --- |
| `game-file` | Sets the startup file and enables command-line startup. Typically a host file (`.hst`) for hosting or a player turn file (`.m1`, `.m2`, etc.) for playing. With `-a`, this is a new-game definition file. If multiple positional files are supplied, the last one wins. |
| `-g[N]` | Generates turns and exits. The count must immediately follow `g`: `-g10` requests ten turns. Omitting the count, or using zero, leaves the existing extra-turn count unchanged (normally one turn total). Positive counts are capped at 1,000. Without `-w` or `-t`, generation does not wait for outstanding player turns. |
| `-w` | Enables waiting. With `-g`, outstanding player turns lead into the host's automatic waiting mode. When opening a multiplayer player file normally, invokes the submit-and-wait-for-next-turn action. |
| `-t` | Try mode. With `-g`, generates only when no player turns are outstanding; otherwise exits, or skips to the next game in a batch. Without `-g`, attempts to open the startup game and then exits. Takes precedence over `-w` when both are set. |
| `-a` | Creates a new game from the supplied definition file, then exits. Requires a registered serial number. See the [new-game fixture](tests/scaffold/fixtures/newgame/tiny/game.def) and [scaffold runner](tests/scaffold/README.md) for an example. |
| `-b FILE` or `-bFILE` | Reads a batch-processing file and enables generation and command-line startup. See the format below. The filename is consumed immediately, so keep other switches separate from this argument. |
| `-v` | Validates the host game and player turns, writes a `.chk` report with player status and detected errors, then exits. This startup action takes precedence over new-game creation and turn generation. |
| `-l` | Enables logging to the game's `.log` file, including generation progress and load diagnostics. |
| `-h` | Enables hot-seat password handling: successfully entered passwords are not cached by the password dialog for subsequent use. |
| `-p PASSWORD` or `-pPASSWORD` | Supplies the remembered password and computes its password salt. Use at most 15 characters without spaces; the parser does not consume longer values as a single password. |
| `-x` | Requests `ExitWindows` when exiting, rather than just quitting the game. The actual effect depends on the Windows/Wine compatibility implementation. |
| `-c` | Sets command-line startup according to whether a filename has already been parsed. Usually redundant because a positional file or successful `-b` already enables it. Its effect depends on argument order. |
| `-d[fpm]` | Selects text dumps: `f` for fleets, `p` for planets, and `m` for the universe map; selectors are case-insensitive and can be combined (`-dfpm`). The startup dump path requires a player game. **Current reconstruction issue:** the selector loop in `stars.c` never terminates or checks the end of the string, so these switches are not currently safe to run. |

Examples, from a directory containing the game files:

```sh
wine stars.exe demo.m1           # Open player 1's turn
wine stars.exe -g demo.hst       # Generate one turn and exit
wine stars.exe -g10 -l demo.hst  # Generate ten turns with logging
wine stars.exe -g -t demo.hst    # Generate only if all turns are in
wine stars.exe -v demo.hst      # Write a validation report
wine stars.exe -a game.def      # Create a game from a definition
```

### Batch-processing file

`-b` takes a plain-text list of host game filenames, one per line. The extension
of the list itself is unrestricted. For example, `games.lst` can contain:

```text
C:\games\alpha\alpha.hst
C:\games\beta\beta.hst
C:\games\gamma\gamma.hst
```

Save it with **Windows CRLF line endings, including after the final entry**.
Both `FSetUpBatchProcessing` in [`stars.c`](decompiled/stars.c) and the next-entry
reader in [`mdi.c`](decompiled/mdi.c) copy through the character before LF and
unconditionally remove that character, assuming it is CR. An LF-only file or
an unterminated last line therefore loses the final character of a filename.

- Use one nonempty, unquoted path per line, with no comments, blank lines,
  switches, or surrounding whitespace. Each entire line is treated as a path.
- Relative paths resolve from the process working directory, not the list's
  directory. Windows absolute paths avoid that ambiguity.
- Keep paths within the 256-byte `szBase` buffer (at most 255 path bytes; the
  copied CR is replaced by NUL). Keep the list below 32 KiB: the loader stores
  its byte length in a signed 16-bit variable.
- Options apply to the whole run. The file contains no per-game passwords,
  turn counts, or commands. A new-game `.def` file for `-a` has a separate format.

On macOS/Linux, create a list with explicit CRLF endings and run it with Wine:

```sh
printf '%s\r\n' 'alpha.hst' 'beta.hst' 'gamma.hst' > games.lst
wine stars.exe -b games.lst -l
wine stars.exe -b games.lst -t -l
```

`-b` implies `-g`: normally it generates one turn for each listed game in order
and then exits. Adding `-t` skips games with outstanding turns, which is useful
for periodically invoking the batch from an external scheduler. `-w` instead
enters waiting mode at an incomplete game; it does not take the skip-to-next-game
branch.

In the current startup loop, `-gN` does **not** generate N turns for every entry:
the extra-turn counter is only consumed after the last batch entry is reached.
Avoid combining multi-turn generation with `-b` unless that final-game behavior
is intended. These details describe the checked-in reconstruction; batch
processing has not been established as working end to end by the scaffold test.

### Automatic development releases

The `Build and release latest stars.exe` GitHub Actions workflow builds the
checked-in C sources on pushes to the default branch. After a successful build,
it moves the `latest` tag to the built commit and replaces `stars.exe` in the
`Latest development build` prerelease. The release notes identify the commit
and workflow run. Each run also saves the executable as an artifact for 30 days.

The workflow uses the `mingw-debug` preset and can also be started manually on
the default branch from the Actions tab. Repository release immutability must
be disabled so the workflow can move the tag and replace the executable.

### Scaffold smoke tests

Run `make newgame` to generate the tiny test game with the compiled executable
under Wine. Fixtures and the runner live in [`tests/scaffold/`](tests/scaffold/README.md);
generated game files and logs go to `dist/scaffold/`. See that directory’s README
for prerequisites, custom fixtures, and the checks performed.

## Direction

The project produces structured C through explicit-block IR and region recovery,
with resources and compatibility adaptations for a native Windows build.
Ongoing work improves recovery, removes remaining translation gaps, and checks
the generated program's behavior against the original game. The goal is
behaviorally equivalent Win32 C that preserves the game logic while replacing
the old Win16 execution environment with maintainable source code.
