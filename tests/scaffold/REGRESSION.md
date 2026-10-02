# Fixed-seed AI regression runs

This harness runs the original Win16 game in `starsbox/` and the reconstructed
native game under Wine. It keeps separate saves at creation and after 10, 50,
and 100 turns, then compares decrypted records.

## Scenarios

| Fixture | Size | Density | Seed | Players |
| --- | --- | --- | --- | --- |
| `fixtures/regression/small.def` | Small (1) | Normal (1) | 12345 | 1 human + 6 AI |
| `fixtures/regression/medium.def` | Medium (2) | Normal (1) | 12345 | 1 human + 6 AI |
| `fixtures/regression/huge.def` | Huge (4) | Packed (3) | 12345 | 1 human + 6 AI |

The human slot uses the existing `fixtures/newgame/tiny/humanoid.r1`. The first
slot must be a race file: the original definition parser accepts `#` AI entries
only in subsequent slots. After creation, the runner updates the human's `.m1`
player record to Maid AI (ID 7), so that slot takes basic actions during forced
generation. Do not use `-w` or `-t` for these runs.

The six AI entries are `#1 4` through `#6 4`: Robotoid, Turindrone, Automitron,
Rototill, Cybertron, and Macinti, all at Expert difficulty. Type and difficulty
zero would choose randomly, so neither is used. Victory conditions are disabled,
player positions use the same setting (1), and all seven game-option flags are
zero. Random events remain enabled to exercise more simulation behavior.

## Why both executables need a seed patch

The seed on line 2 of a `.def` initializes universe creation through `Randomize`.
A new turn-generation process normally calls `Randomize2(GetTickCount())` in
`WinMain`. Therefore a definition seed alone cannot reproduce later turns.

`seed_exe.py` patches a **separate copy** of the original executable:

- Verifies the instruction sequence at NE segment 4, offset `0085`.
- Replaces `CALLF GetTickCount; PUSH DX; PUSH AX` with `PUSH seed_high;
  PUSH seed_low; NOP`, preserving the following `Randomize2` call and stack size.
- Removes the matching import relocation at `0004:0086`. Otherwise Windows
  would overwrite the immediate operand while loading the executable.
- Leaves other relocation entries, instructions, and file offsets intact.
- Refuses to patch an unexpected executable or overwrite an existing output.

The instruction patch requires an 80186 or later, which the bundled DOSBox
supports. It is specific to the checked-in Stars! 2.7j layout. The `prepare`
command records executable hashes, the seed, and fixture hashes in `run.json`.
To use the patcher separately:

```sh
python3 tests/scaffold/seed_exe.py \
  starsbox/c_drive/STARS/stars.exe \
  starsbox/c_drive/seeded.exe --seed 12345
```

The native CMake option `STARS_TEST_SEED` replaces that same startup call in a
build-local `stars-seeded.c`. It leaves `decompiled/stars.c` intact and reapplies
on CMake reconfiguration after source changes. Ordinary builds use the clock.
Both seeds accept decimal uint32 values; the original algorithms use only the
low 12 bits for `Randomize` and low 14 bits for `Randomize2`.

## Build and stage

Run from the repository root. Both environments need a registered Stars! serial.
Python 3, Go, MinGW, CMake, Ninja, and Wine are needed; the original runner uses
`starsbox/dosbox.bin` and `starsbox/stars_dosbox_macos.conf` with its existing
`C:` mount at `starsbox/c_drive/`.

```sh
cmake --preset mingw-debug -B dist/regression-build -DSTARS_TEST_SEED=12345
cmake --build dist/regression-build
make build

python3 tests/scaffold/regression.py prepare \
  --engine native --seed 12345 \
  --exe dist/regression-build/bin/stars.exe \
  --work dist/scaffold/regression/native

python3 tests/scaffold/regression.py prepare \
  --engine dosbox --seed 12345 \
  --exe starsbox/c_drive/STARS/stars.exe \
  --work starsbox/c_drive/REGTEST
```

Choose **fresh output directories** for each run; staging never overwrites prior
results. DOSBox directory components must be at most eight letters, digits, or
underscores. The examples assume a Wine prefix with the usual `Z:` host-filesystem
mapping. Keep work paths free of spaces because the Stars! parser cannot quote
them.

Staging writes CRLF definitions and absolute Windows paths for the race and
output files. Launch commands also use absolute paths: Windows 3.1 can change
the working directory during startup, so `-a game.def` is unreliable through
`win /n`. The two staged definitions have different physical paths but identical
scenario settings and race bytes.

## Run and capture checkpoints

```sh
python3 tests/scaffold/regression.py run --work starsbox/c_drive/REGTEST
python3 tests/scaffold/regression.py run --work dist/scaffold/regression/native
```

Run only one DOSBox instance at a time: all original runs share the Windows 3.1
installation. Its window opens for each launch. The original runner adds `-x`
to exit Windows after each action and allow DOSBox to return to the shell.

For a quicker check, select one scenario and/or an earlier endpoint:

```sh
python3 tests/scaffold/regression.py run \
  --work starsbox/c_drive/REGTEST --scenario small --through 10 --timeout 900
```

The timeout is per launch (default 900 seconds), not for the entire suite. A
failed run stops immediately and preserves logs and files. By default, runs with
saves or completed checkpoints are not overwritten. To continue after a timeout
or an abbreviated run, use `--resume`: it verifies the latest checkpoint, moves
the current saves into a `before-resume-*` directory (also preserving logs),
restores the checkpoint, and repeats the next launch from that exact state.
A failure before any saves are written can be retried in place; a failure before
the first checkpoint that leaves partial saves requires a fresh staged run.

```sh
python3 tests/scaffold/regression.py run \
  --work starsbox/c_drive/REGTEST --scenario small --resume --timeout 900
```

Each scenario uses **four process launches**, in this exact order:

```text
-a   <absolute game.def>   -> checkpoint 000, year 2400
-g10 <absolute game.hst>   -> checkpoint 010, year 2410
-g40 <absolute game.hst>   -> checkpoint 050, year 2450
-g50 <absolute game.hst>   -> checkpoint 100, year 2500
```

The counts are incremental: `-g50` immediately after `-g10` would reach turn 60.
Keep launch boundaries identical across builds because the startup seed resets
on each launch. Do not replace the sequence with a single `-g100` run and expect
the same results. Logging switches can also change execution paths; the runner
uses the same simulation options on both sides.

Before saving a checkpoint, the runner checks the host file's actual turn. A
successful process exit without output is a failure. Native command-line turn
generation returns **1 on success** (`FGenerateTurn` sets `vretExitValue`); native
creation and the DOSBox wrapper return 0. The runner checks those expected codes
as well as the output files. At turn zero it runs
`dist/stars-asm save update game.m1 --ai maid` and
`dist/stars-asm save update game.hst --ai maid --player 1` before copying the checkpoint.
Use `--cli` on `regression.py run` if the CLI is elsewhere. Under each scenario:

```text
run-000.log                 emulator/Wine output for universe creation
run-010.log                 output for the first ten turns
run-050.log
run-100.log
checkpoints/000/game.*      all .xy, .hst, .mN, .hN, and .xN files present
checkpoints/000/checkpoint.json
checkpoints/010/...
checkpoints/050/...
checkpoints/100/...
```

Checkpoint manifests contain commands, years, and raw SHA-256 hashes. Backups
and diagnostic logs are not treated as game-state files.

## Compare

```sh
python3 tests/scaffold/regression.py compare \
  starsbox/c_drive/REGTEST dist/scaffold/regression/native
```

The command writes `tests/scaffold/fixtures/regression/regression-comparison.json` and returns nonzero
for any difference, invalid save, missing file, or missing checkpoint. Use
`--scenario small --through 10` when comparing abbreviated runs. For one file:

```sh
dist/stars-asm save compare original/game.hst native/game.hst
```

Raw hashes will differ because game IDs and file encryption salts include clock
values. `save compare` decrypts each file first, then excludes only:

- `RTBOF.lidGame` and `GAME.lid` (the clock-derived game identity).
- `RTBOF.lSaltTime` (the encryption salt).

It retains all other record bytes, ordering, version fields, flags, player data,
and simulation state. It handles the `.xy` file's raw packed star coordinates
separately from its encrypted `GAME` record. A mismatch reports the first
differing record with its type, file offsets, sizes, and decrypted bytes.

AI history (`rtAiData` in `.hN`) is decoded using the owning player's AI,
read from the companion `.mN` beside the history. Histories omit PLAYER records.
Cybertron histories decode as `CYBERINFO`; Robotoid, Turindrone, Automitron,
and Rototill histories decode as `AIHIST`. Without a companion turn file, the
history is reported only as a changed payload. Some storage is never read:
`AIHIST` freighter slots at or past `cFreighter` on both sides (stale heap data
moved by `ValidateStarbaseHistory`) and the reserved `CYBERINFO` byte. In fleet
orders (`rtOrderA`/`rtOrderB`), `fUnused` is never read and `fNoAutoTrack` is
cleared when fleets load. Task-union words past those the order's `grTask`
reads are stale too. Xfer reads all five, Patrol two, and LayMines and Give one;
`tlm.cTimeOld` is write-only. Changes in unused storage are listed as `unused ...`. If they are a file's only differences, `save compare`
exits 0 with `MATCH with warnings`. The regression report then records
`"match": true, "warning": true` and counts these files as warnings, not failures.
Its detail keeps only the summary line: native unused storage holds stale heap
and stack bytes that change between runs, so the values would make the
checked-in report shift. Run `save compare` on the files to see them.

This is strict record comparison, not a complete semantic interpretation. A
reported difference needs inspection: padding or environment-specific fields
can differ too. No additional bytes are silently discarded to make tests pass.
The patch makes simulation randomness reproducible, but it does not establish
that the reconstructed implementation is correct.

### Test turn generation independently of universe creation

If native creation is broken, a fresh native scenario can start from the
original's verified turn-zero files:

```sh
python3 tests/scaffold/regression.py run \
  --work dist/scaffold/regression/native --scenario small \
  --baseline starsbox/c_drive/REGTEST
```

This checks the seed and fixture hashes, copies the reference creation files,
then runs the same `-g10`, `-g40`, and `-g50` launches. The checkpoint manifest
marks turn zero as a reference input. Comparisons skip that checkpoint rather
than count it as successful native creation. Native turns are still compared
normally. Use `--resume` for a subsequent attempt from a completed checkpoint;
`--baseline` is only for a fresh scenario.

### Separate logic differences from inherited state

`crossfeed` copies saves from any directory and generates `--turns` turns with
the engine and executable of a prepared `--work` run. It writes them to
`<work>/<scenario>/xfeed/<from>_<to>/`, and `--expect` compares the result
against another directory of saves. Two tests classify a native difference:

```sh
# A. Logic: native, starting from the original's exact state.
python3 tests/scaffold/regression.py crossfeed --work starsbox/c_drive/native \
  --scenario oneai6 --input starsbox/c_drive/REGTEST/oneai6/checkpoints/025 \
  --turns 25 --expect starsbox/c_drive/REGTEST/oneai6/checkpoints/050

# B. Inertness: the original, starting from native's state.
python3 tests/scaffold/regression.py crossfeed --work starsbox/c_drive/REGTEST \
  --scenario oneai6 --input starsbox/c_drive/native/oneai6/checkpoints/025 \
  --turns 25 --expect starsbox/c_drive/REGTEST/oneai6/checkpoints/050
```

Test A isolates native turn generation over that span. If test B matches, the
stored difference has no effect on the original. The startup seed resets on
every launch. Match an original launch span (`--turns` from a checkpoint) when
comparing against its checkpoints. To narrow a span, crossfeed the same input
with both engines using a smaller `--turns`, then compare the two `xfeed`
outputs with `--expect`. An existing output is reused only when its input files
and executable match; otherwise remove it to rerun.

`bisect` runs both engines from the same input and binary-searches `-gK` for the
first divergent turn. A `-gK` launch reproduces the first K turns of a longer
launch from the same input. Each search needs about log2(turns) launches per engine:

```sh
python3 tests/scaffold/regression.py bisect --original starsbox/c_drive/REGTEST \
  --native starsbox/c_drive/native --scenario oneai6 \
  --input starsbox/c_drive/REGTEST/oneai6/checkpoints/025 --turns 25
```

Stars! keeps the last generated turn's inputs and AI order logs (`.xN`) in
`backup/`. Compare them between engines. Matching inputs with differing logs
place the divergence in that AI's decisions. `.xN` record 1 (`RTLOGHDR`) holds
the installation serial and environment fingerprint, so it always differs.

### Trace native RNG draws

Configure a separate build with `-DSTARS_TEST_TRACE=ON` and prepare a native run
from it. The linker wraps `Random` and `PctPlanetCapacity`
(`tests/scaffold/regression_trace.c`). The trace build behaves identically.
With `--trace`, `crossfeed` and `bisect` set `STARS_TRACE` and write
`trace.log` beside the output. Each `Random` line records the turn, player, AI flag,
range, result, caller address, and RNG seeds before the draw. Seeds allow replaying
the stream at any offset. `trace` resolves caller addresses to source lines:

```sh
cmake --preset mingw-debug -B dist/regression-trace -DSTARS_TEST_SEED=12345 -DSTARS_TEST_TRACE=ON
cmake --build dist/regression-trace
python3 tests/scaffold/regression.py prepare --engine native --seed 12345 \
  --exe dist/regression-trace/bin/stars.exe --work starsbox/c_drive/ntrace
python3 tests/scaffold/regression.py trace <dir>/trace.log \
  --exe dist/regression-trace/bin/stars.exe --turn 32 --player 1
```

The original cannot be traced. Compare its AI logs with the native trace,
then replay the native seeds at nearby offsets to locate an extra or missing draw.

For confidence in the harness, first run the original twice into fresh folders
and compare those runs. Matching original-to-original checkpoints establishes a
baseline before interpreting original-to-native failures.

## Harness tests

```sh
python3 -B -m unittest discover -s tests/scaffold -p 'test_*.py'
make test
```

The patch test checks both seed words and that only the intended instructions
and relocation table change. Go tests check salt/ID normalization, retention of
coordinate changes, and rejection of truncated universe files.
