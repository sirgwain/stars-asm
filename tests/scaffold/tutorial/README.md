# Tutorial UI tests

The suite runs AutoHotkey **v2.0.28** and the rebuilt Stars! executable inside the same isolated Wine prefix. It drives normal Windows controls, menus, keyboard shortcuts and mouse gestures. The test build adds a hidden, read-only observation window; it does not complete tasks or modify game orders.

## Run

Install Wine, Python 3, CMake, Ninja and the x86_64 MinGW toolchain used by `mingw-debug`. The runner defaults `STARS_TUTORIAL_SERIAL` to `CV6JVUAX`. Set the environment variable to override it, then:

```sh
make tutorial
make tutorial-reject
```

`--download-ahk` fetches the official portable release and verifies its pinned SHA-256 before extracting it under `dist/tools/autohotkey`. To supply an existing interpreter:

```sh
python3 tests/scaffold/tutorial/run.py --ahk /path/to/AutoHotkey64.exe
```

The runner builds `dist/tutorial-build/bin/stars.exe` with `STARS_TEST_TUTORIAL=ON`. The option defaults to OFF for normal builds. `--exe PATH` accepts a prebuilt executable with that option enabled. Each run copies the executable and scripts into a fresh directory and uses its own Wine prefix, INI settings and game files. Wine prefixes are created under `/tmp/stars-tutorial-*`, outside the repository so Wine’s filesystem symlinks are not scanned by workspace tools. The absolute prefix path is printed at startup and recorded as `wine_prefix` in `metadata.json`. The original registration and game files are never modified.

Linux runs need an X11 display. For unattended runs:

```sh
xvfb-run -a -s '-screen 0 1600x1200x24' make tutorial
```

The virtual Wine desktop is 1600×1200. The game occupies 1280×960, and the tutor sits beside it so it cannot intercept scanner clicks. macOS Wine can execute the controls and gestures, but its GDI screen capture can return blank images. The suite logs that limitation instead of saving misleading screenshots or failing an otherwise valid walkthrough.

## Coverage and pass conditions

`run.py` extracts all 640 fragments (80 pages) from `dasm/input/strings_uncompressed.c`. It exports numeric IDs from `decompiled/res/resource.h`, and checks that every highlighted instruction assigned by `FTutorTaskDone` (a `TutorId` name or number, resolved through `decompiled/enums.h`) has a dispatcher action. Missing actions fail before launching Wine. `coverage.json` records that inventory.

`steps.ahk` maps highlighted instructions to actions for years 2400–2436: navigation, message filtering, exploration routes, production and templates, colonization and transport, research, ship and starbase design, fleet splitting/merging, reports, battle playback and the final score report. `actions.ahk` contains the reusable UI operations. It uses resource IDs and observed HWNDs for standard controls, and current layout rectangles for custom controls. No RC caption changes are needed. The source tutorial calls planet 7 “Moholdi”; the actual planet name is “Mohlodi”, which the action uses.

A full pass requires all 80 pages, every required task before each Generate, no panic auto-completion, the final completion message and departure from tutorial mode at turn 37. Most tutorial Generate operations load canned turn files, so the suite checks the live task state and records the orders **before** generation. The final generation runs the normal simulation. Early victory messages do not count as tutorial completion.

The separate `reject-generate` scenario tries Generate before the first task is complete, checks that the turn does not advance, and verifies that the tutorial remains usable afterward.

For incremental development:

```sh
python3 tests/scaffold/tutorial/run.py --until-year 2403 --timeout 180
```

An intentional early stop reports `partial`, produces a skipped JUnit case, and exits nonzero. It cannot be mistaken for full coverage. Unknown instructions, covered click targets, unexpected tutorial errors, stalls, crashes, syntax failures and timeouts also produce nonzero results.

For debugging later actions without replaying every earlier year, launch with `--keep-game-on-failure`. After fixing the action, use `--continue-run /absolute/path/to/the/original/run --keep-game-on-failure` to attach to that live game in its existing prefix recorded in `metadata.json`. The prefix must still exist; clearing `/tmp` prevents continuation. Runs created before prefix paths were recorded require a fresh run. A continuation can report failures or `partial`; it cannot report a full walkthrough pass. It requires the game to remain running, and does not reopen a saved game. Finish debugging with a fresh normal run. To close a retained game explicitly, run `WINEPREFIX=/absolute/prefix/path/from/metadata.json wineserver -k`.

## Artifacts

Each run is retained under `dist/scaffold/tutorial/<UTC timestamp>/`:

- `result.json` and `results.xml`: explicit outcome and failure location.
- `events.jsonl`: instruction text, action times, queue contents and diagnostics.
- `year-2400.txt`, etc.: live orders and state before each generation.
- `last-state.txt` and `windows.txt`: observer snapshot and window/control inventory on failure.
- `screenshots/`: BMP captures where supported by the Wine display driver.
- `wine.log` and `metadata.json`: interpreter diagnostics, versions and executable/source hashes.
- `game/coverage.json`: required instruction inventory.

By default, the runner closes only its own game and Wine server. The explicit debugging option retains a failed game. Prefixes contain registration data, remain in `/tmp` for debugging, and may be removed by the operating system; the manual GitHub workflow uploads diagnostic files only. The workflow uses the same default serial; optionally set the repository variable `STARS_TUTORIAL_SERIAL` to override it.

A populated instruction dispatcher is a coverage inventory, not proof that every gesture works on every Wine driver. `result.json` is the runtime authority; a full walkthrough is validated only when it reports `passed` with 80 pages at the final year.

Verified locally on 2026-10-01 with AutoHotkey 2.0.28 and Wine 11.0 on macOS: a fresh walkthrough passed all 80 pages in 346 UI actions, with 37 completed-year snapshots and the final completion message. After reducing input delays and mouse animation, the same 346-action walkthrough took 4 minutes 3 seconds, down from 14 minutes 25 seconds. Repeated component drags retain the Windows double-click interval because the designer ignores double-click messages. A separate fresh run passed premature-Generate rejection and recovery. The Linux workflow is provided for repeatable CI runs; it has not yet been dispatched.
