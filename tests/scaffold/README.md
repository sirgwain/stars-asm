# Scaffold smoke tests

For Small, Medium, and Huge/Packed games with all six AI types, fixed seeds,
DOSBox/native runners, and comparisons at turns 0/10/50/100, see
[the regression guide](REGRESSION.md).

These tests exercise the compiled game under Wine. Go unit tests and decompiler
snapshots remain alongside their packages and run with `make test`.

## Layout

```text
tests/scaffold/
  newgame.sh                     New-game smoke test runner
  fixtures/newgame/tiny/
    game.def                     Fixed-seed, single-player tiny game
    humanoid.r1                  Race used by that game

dist/scaffold/newgame/run/       Recreated on each default run (ignored by Git)
  game.def                     Staged definition with Windows paths
  humanoid.r1                  Staged race
  generated/tiny.*             Generated game files
  wine.log                     Wine stdout and stderr
```

Keep fixture inputs in `fixtures/` and generated output in `dist/scaffold/`.
Each new-game scenario gets its own directory containing a `game.def` and its
race files. Race paths in the definition are relative to that fixture directory;
output paths are relative to the run directory. Use forward slashes and relative
paths in fixture definitions. The runner converts separators for Stars!.

## Run

Build the executable first:

```sh
cmake --preset mingw-debug
cmake --build --preset mingw-debug
make newgame
```

The runner requires Bash, Perl, and Wine on `PATH`. It uses Wine's default prefix
(or `WINEPREFIX` if set). That prefix's `Stars.ini` must contain a registered
serial; Stars! does not generate games with `-a` without one.

To run another scenario:

```sh
make newgame DEF=tests/scaffold/fixtures/newgame/my-scenario/game.def
```

To select an executable, timeout, or output directory:

```sh
tests/scaffold/newgame.sh --exe dist/mingw-debug/bin/stars.exe \
  --timeout 60 --work dist/scaffold/newgame/custom \
  tests/scaffold/fixtures/newgame/tiny/game.def
```

The work directory is deleted and recreated on every run. Use a dedicated
scratch directory. Defaults are resolved from the repository location, so the
script can also be invoked from another working directory.

## What passing means

The executable must exit successfully within the timeout and write both the
`.xy` universe file and `.hst` host file named by the definition. The runner
prints the output directory listing and retains `wine.log` for diagnosis.
An error dialog can leave the process blocked until the timeout.

This is a generation smoke test. It does not yet compare game contents with the
original executable or validate turn processing.
