# Repository Instructions

- This is a self-contained tool, not a defensive public API. Do not add nil-check-to-empty-default wrappers for internal constructors, render views, operands, or domain types.
- If required data is missing, validate it at the command or caller boundary and return a clear error there. Do not make constructors silently return empty views, zero values, placeholder operands, or helper functions that hide invalid calls.
- Tests should use realistic, fully initialized fixtures. Do not make production code accept nil/default internals just to satisfy weak tests.
- Do not add exported wrapper methods or functions that only call an unexported implementation for compatibility. If a function should be public, make the real function exported and call it directly.
- Do not add one-line trampoline helpers, exported or unexported, when they only exist to call another helper with one extra predicate. Fold the predicate into the real helper unless the split has multiple direct callers or materially improves clarity.
- Do not add `WithOptions` variants to avoid changing exported signatures. This repo is not preserving a public API surface; update the existing function signature and all call sites instead.
- Add golang style function header comments to all new functions
- Don't add tests to verify every little change. Add tests for functionality, update existing tests when functionality changes. Temporary tests while updating code to verify changes are fine, if they are removed when finished.
- Snapshots exist to make output changes visible. When an intentional code change affects snapshot output, update and keep the relevant snapshots with the change.
- Do not add reflection to do compares, write the compare functions
- Ignore pending/staged changes. If the repo has changes, do the correct thing, don't be cautious because there are pending changes.

## After Changes
After making changes to the repo, review the changes against the above rules to make sure no violations were introduced.

Run unit tests to automatically update snapshot files in [dasm/stars/testdata/snapshots]

## References

All struct definitions for this project live in [structs](decompiled/structs.h). When interpreting offsets and bitfields on globals or locals or function params, reference the structs.
