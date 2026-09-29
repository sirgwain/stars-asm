#!/usr/bin/env bash
set -euo pipefail

# usage prints the new-game smoke test options.
usage() {
  cat <<'EOF'
Usage: newgame.sh [options] [game.def]

Generates a new game with the decompiled stars.exe (stars.exe -a game.def)
under Wine and checks that the game files were written.

The .def names its race files and output .xy relative to the working
directory. Race paths are relative to the .def's directory and are preserved
when copied into a scratch working directory. The .def's paths
are rewritten with backslashes: Stars! strips a file extension at the last
'.', unless a backslash follows it, so './x/y/tiny' would lose its whole name.

Options:
  --work DIR      Working directory (default dist/scaffold/newgame/run; recreated).
  --exe PATH      stars.exe to run (default dist/mingw-debug/bin/stars.exe).
  --timeout SECS  Seconds before the run is killed (default 60). An error
                  message box blocks until then.

Wine uses the default prefix; Stars.ini there must hold a registered serial,
since -a does nothing without one.
EOF
}

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
DEF="$ROOT/tests/scaffold/fixtures/newgame/tiny/game.def"
WORK="$ROOT/dist/scaffold/newgame/run"
EXE="$ROOT/dist/mingw-debug/bin/stars.exe"
TIMEOUT=60

while [ $# -gt 0 ]; do
  case "$1" in
    --work) WORK="$2"; shift 2 ;;
    --exe) EXE="$2"; shift 2 ;;
    --timeout) TIMEOUT="$2"; shift 2 ;;
    -h|--help) usage; exit 0 ;;
    -*) echo "unknown option: $1" >&2; usage >&2; exit 2 ;;
    *) DEF="$1"; shift ;;
  esac
done

[ -f "$DEF" ] || { echo "no .def file: $DEF" >&2; exit 2; }
[ -f "$EXE" ] || { echo "no stars.exe: $EXE (build with: cmake --build --preset mingw-debug)" >&2; exit 2; }
DEF_DIR="$(cd "$(dirname "$DEF")" && pwd)"
EXE="$(cd "$(dirname "$EXE")" && pwd)/$(basename "$EXE")"

rm -rf "$WORK"
mkdir -p "$WORK"
WORK="$(cd "$WORK" && pwd)"

# Stage race files and the output directory, and rewrite paths for Stars!.
OUT_BASE=""
: > "$WORK/game.def"
while IFS= read -r line || [ -n "$line" ]; do
  line="${line%$'\r'}"
  rel="${line#./}"
  case "$rel" in
    *.r[0-9]|*.r[0-9][0-9])
      mkdir -p "$WORK/$(dirname "$rel")"
      cp "$DEF_DIR/$rel" "$WORK/$rel"
      ;;
    *.xy)
      mkdir -p "$WORK/$(dirname "$rel")"
      OUT_BASE="${rel%.xy}"
      ;;
  esac
  case "$rel" in
    */*) printf '%s\n' "${rel//\//\\}" >> "$WORK/game.def" ;;
    *) printf '%s\n' "$line" >> "$WORK/game.def" ;;
  esac
done < "$DEF"
[ -n "$OUT_BASE" ] || { echo "$DEF names no output .xy file" >&2; exit 2; }

echo "generating: $DEF -> $WORK/$OUT_BASE.*"
status=0
(cd "$WORK" && perl -e 'alarm shift; exec @ARGV' "$TIMEOUT" wine "$EXE" -a game.def > wine.log 2>&1) || status=$?
if [ "$status" -eq 142 ]; then
  echo "FAIL: stars.exe did not exit within ${TIMEOUT}s (an error message box may be open)" >&2
  exit 1
elif [ "$status" -ne 0 ]; then
  echo "FAIL: wine exited with status $status (see $WORK/wine.log)" >&2
  exit 1
fi

OUT_DIR="$WORK/$(dirname "$OUT_BASE")"
NAME="$(basename "$OUT_BASE")"
ls -l "$OUT_DIR"
[ -f "$OUT_DIR/$NAME.xy" ] && [ -f "$OUT_DIR/$NAME.hst" ] || { echo "FAIL: $NAME.xy or $NAME.hst was not written" >&2; exit 1; }

echo "OK"
