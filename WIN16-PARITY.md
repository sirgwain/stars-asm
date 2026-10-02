# Win16 Parity Repairs

The native build reproduces some behavior of the original Stars! 2.7j that
depends on Win16 memory layout, uninitialized stack bytes, or the original
compiler and runtime. These repairs exist only so fixed-seed regression runs
(`tests/scaffold/REGRESSION.md`) can show that native and original match turn
for turn, which in turn shows the decompiler is faithful.

Once parity is no longer needed, the items under **Original bugs emulated**
can be reverted to the behavior the code evidently intended. Each entry lists
where the repair lives, what the original did, and what a revert should do.

Generated sources under `decompiled/` come from the decompiler passes named
below. Revert by changing the pass and regenerating, not by editing the C.

## Original bugs emulated (revert when parity is no longer needed)

### Negative-index reads return the Win16 neighbor

- **Pass:** `native-negative-index`
  (`dasm/stars/sem/processor_nativenegativeindex.go`, analysis in
  `dasm/stars/sem/negative_index.go`, audit with `stars-asm dasm negidx`).
- **Original:** a local that may be -1 (from functions marked
  `may_be_minus_one` in `dasm/input/overrides-semantics.json`) indexes an
  array without a check. Win16 read whatever its layout put just before
  the array.
- **Parity repair:** each unguarded read becomes `i != -1 ? path : <alias>`,
  where `<alias>` is the global or field the Win16 layout put before the array.
  Current sites:
  - `ai3.c` DoMacintiAiTurn: `rgshdef[iLatestMiner].cExist` with
    iLatestMiner -1 reads `vtimer.mdForce | vtimer.fAutoGenWhenIn << 16`.
  - `ai3.c` DoMacintiAiTurn and `ai.c` DoRobotoidAiTurn:
    `lpfl->rgcsh[iLatestDestroyer]` with -1 reads `lpfl->pt.y`.
- **Revert to:** drop the alias and handle -1 as "no such design" (count 0),
  or skip the condition, whichever each caller intends.
- Sites checked by hand that need no repair are listed in
  `NegativeIndexAudited`, each with the reason.

### Cybertron mine-laying order carries leftover stack bytes

- **Pass:** `native-cybertron`, `overlayCyberOrderFrame`
  (`dasm/stars/sem/processor_nativecybertron.go`).
- **Original:** DoCyberAiTurn's random mine-laying move builds a local
  `ORDER ord` without setting its task union, so `tlm.cTime` (also read as
  `tsell.iPlrX`, the laying countdown: 0 stops, 5 lays forever) is whatever
  the stack slot held. MSVC overlapped block-scoped `shdef`,
  `rgRecycleSBShdef`, and `ord` in the frame. After turn 80, the recycle
  clears zero that slot and the fleet stops laying on arrival.
- **Parity repair:** the three locals share one `rgbOrdFrame` byte array laid
  out like the Win16 frame (`shdef` +0, `ord` +0x84, `rgRecycleSBShdef` +0x86).
- **Revert to:** remove the overlay and set `ord.tlm.cTime = 5` and
  `ord.tlm.cTimeOld = 5`, like the AI's other LayMines orders (lay forever).
- **Not reproduced:** at turn 80 or earlier, with no design scrapped that
  turn, the slot holds residue from functions called before DoCyberAiTurn.
  In the original it is a stack address that shifts with the save path
  length (for example 0xa2e2), and native's frame is uninitialized. The
  regression report shows this as a `tlm.cTime` difference (oneai5, turn 80).

## Corruption guards (keep)

These stop native memory corruption where the original corrupted dead
storage harmlessly. They don't change game behavior and should stay.

- **Cybertron recycle writes for absent designs** (`native-cybertron`,
  `cyberRecycleIndex`): `rgRecycleShdef[iLatestDestroyer|iLatestCargo] = 0`
  with index -1 overwrote a byte of a dead `lppl` pointer in Win16. Native
  skips the store when the index is -1.
- **Macinti late-game splits** (`native-macinti`): turn>80 code copied from the
  16-entry ship logic marked entries 10–15 of the 10-entry
  `rgRecycleSBShdef`, writing into `l` and `fTonsOfMinerals`. SplitOutShdefs
  read all 16. Native uses a 16-entry `rgSplitShdef` scratch array. The stale
  bytes the original read between calls are not reproduced.

## Compiler and runtime behavior matched (keep unless parity is dropped)

These follow the original toolchain rather than a bug, and the game's
results depend on them.

- **qsort tie order:** `qsort` maps to `qsort16`
  (`dasm/stars/templates/assets/win16defines.h.templ`), rebuilt from the
  Win16 CRT, so elements with equal keys end up in the same order. Native libc
  qsort orders ties differently.
- **x87 precision:** floating arithmetic keeps the original's extended
  precision. Casts that round an x87 result to double or float are preserved
  (`simplifyFloatCast`/`simplifyFloatOperands` in
  `dasm/stars/sem/convert_native.go`).

## Known Win16 behavior not reproduced

- **Macinti armada strength** (`ai3.c` TargetMacArmada): `FPotentMacWarFleet`
  returns 0 without writing `*pcEquiv` for a weak fleet, and TargetMacArmada
  ignores the result and compares `cshWar` (BP-0x20) against the armada
  potency thresholds anyway. The original's value is whatever the fleet loop
  in DoMacintiAiTurn last left at that stack address. TargetMacArmada's own
  callees run below it and never touch it, but other calls from the loop do.
  The value chooses between returning and targeting, and it also sets how many
  `Random(10)` draws are made, so a wrong guess shifts the RNG stream for the
  rest of the turn. This is the cause of the oneai6 and smallai6 divergences
  (first seen at smallai6 t57, oneai6 t68). In a test build, setting
  `cshWar = 1000` (above every threshold) matched smallai6 through t150 and
  oneai6 through t84. Oneai6 then drifted at t85: the draw counts differed but
  the decisions didn't, and the wormhole jumps showed the shifted RNG.
  Making it `static` did not match either. **Revert to:** initialize `cshWar`
  to 0, or have FPotentMacWarFleet always store `cEquiv`.
- **Macinti mine-laying order** (`ai3.c` DoMacintiAiTurn): same unset task
  union as Cybertron. Its `tlm.cTime` slot (BP-0xba) overlaps the far-pointer
  segment of block local `lpplBest` and the tail of `shdef`, so the original
  value depends on a Win16 selector and cannot be reproduced deterministically.
- **PszFormatString** `vrgszUnits[-1]`: display text only. The original read a
  Win16 pointer.

## Regression harness tolerances

`stars-asm save compare` treats storage the game never reads as warnings,
not differences: order `fUnused`, task-union words past those the task reads,
`tlm.cTimeOld`, AIHIST freighter slots past `cFreighter`, and the reserved
CYBERINFO byte. See `tests/scaffold/REGRESSION.md`.
