# Win16 Signature Parser

`win16sigparser_main.go` parses Win16 SDK header prototypes and the Ghidra XML
`*.exports` files in `dasm/input/`, then writes the `overrides-*.json`
function signatures the decompiler loads for KERNEL, GDI, USER, COMMDLG and
TOOLHELP imports.

## Win16 SDK headers

- `dasm/input/WINDOWS.H`: prototypes for KERNEL, GDI and USER. The decompiler
  also reads it for Win16 constant values (see below).
- `dasm/input/COMMDLG.H`: common dialog prototypes and constants.
- `utils/TOOLHELP.H`: TOOLHELP prototypes. Only this parser uses it.

The decompiler takes Win16 constant values (`WM_*`, `SWP_*`, `OFN_*`, ...)
from `WINDOWS.H` and `COMMDLG.H`. `dasm/input/win16defines.json` groups those
names into the families that `dasm/input/enums.json` rules refer to. It also
defines the application's own `WM_USER` messages.

## Run

From the repo root, regenerate the KERNEL, GDI, USER and TOOLHELP overrides:

```bash
go run ./utils \
  --windows-h ./dasm/input/WINDOWS.H \
  --toolhelp-h ./utils/TOOLHELP.H \
  --gdi-exports ./dasm/input/gdi.exports \
  --kernel-exports ./dasm/input/kernel.exports \
  --user-exports ./dasm/input/user.exports \
  --toolhelp-exports ./dasm/input/toolhelp.exports \
  --out-gdi ./dasm/input/overrides-gdi.json \
  --out-kernel ./dasm/input/overrides-kernel.json \
  --out-user ./dasm/input/overrides-user.json \
  --out-toolhelp ./dasm/input/overrides-toolhelp.json
```

## COMMDLG

Common dialog prototypes live in `COMMDLG.H`, so pass it as `--windows-h`:

```bash
go run ./utils \
  --windows-h ./dasm/input/COMMDLG.H \
  --gdi-exports ./dasm/input/gdi.exports \
  --kernel-exports ./dasm/input/kernel.exports \
  --user-exports ./dasm/input/user.exports \
  --commdlg-exports ./dasm/input/commdlg.exports \
  --out-commdlg ./dasm/input/overrides-commdlg.json
```

## Useful Flags

- `--out <path>`: write one combined JSON containing all loaded modules.
- `--out-gdi`, `--out-kernel`, `--out-user`, `--out-commdlg`, `--out-toolhelp`: write per-module files.
- `--toolhelp-h <path>`: merge TOOLHELP prototypes with `--windows-h`.
- `--emit-ordinal-aliases`: emit `ordinal_<n>` entries for each export ordinal.
- `--synthesize-missing`: if no prototype is found, synthesize a best-effort signature from `PURGE` bytes (`uint16_t` args, `int16_t` return, `pascal` callconv).

## Notes

- The parser is best-effort and does not run a full C preprocessor.
- If output is empty for a module, the header likely does not contain matching prototypes for those exports.
- The checked-in `dasm/input/overrides-*.json` files differ from a fresh run,
  so write to a scratch path and review the diff before replacing them.
