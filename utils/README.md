# Win16 Signature Parser

`win16sigparser_main.go` parses Win16 header prototypes and Ghidra XML export files, then emits override JSON files used by this repo.

## What It Does

- Reads function prototypes from a header file (for example `WINDOWS.H` or `COMMDLG.H`).
- Reads exported symbols from one or more `*.exports` XML files.
- Emits override JSON with normalized return/argument types and calling convention.
- Can also emit `ordinal_<n>` aliases (useful for ordinal imports).

## Run

From the repo root:

```bash
go run ./cli/utils/win16sigparser_main.go \
  --windows-h ./cli/utils/WINDOWS.H \
  --gdi-exports ./dasm/input/gdi.exports \
  --kernel-exports ./dasm/input/kernel.exports \
  --user-exports ./dasm/input/user.exports \
  --out-gdi ./dasm/input/overrides-gdi.json \
  --out-kernel ./dasm/input/overrides-kernel.json \
  --out-user ./dasm/input/overrides-user.json
```

## COMMDLG Example

Use `COMMDLG.H` so parser can find common-dialog prototypes:

```bash
go run ./cli/utils/win16sigparser_main.go \
  --windows-h ./cli/utils/COMMDLG.H \
  --gdi-exports ./dasm/input/gdi.exports \
  --kernel-exports ./dasm/input/kernel.exports \
  --user-exports ./dasm/input/user.exports \
  --commdlg-exports ./dasm/input/commdlg.exports \
  --out-commdlg ./dasm/input/overrides-commdlg.json
```

## Useful Flags

- `--out <path>`: write one combined JSON containing all loaded modules.
- `--out-gdi`, `--out-kernel`, `--out-user`, `--out-commdlg`: write per-module files.
- `--emit-ordinal-aliases`: emit `ordinal_<n>` entries for each export ordinal.
- `--synthesize-missing`: if no prototype is found, synthesize a best-effort signature from `PURGE` bytes (`uint16_t` args, `int16_t` return, `pascal` callconv).

## Notes

- The parser is best-effort and does not run a full C preprocessor.
- If output is empty for a module, the header likely does not contain matching prototypes for those exports.
