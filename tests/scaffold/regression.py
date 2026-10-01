#!/usr/bin/env python3
"""Stage, run, and compare fixed-seed Stars! AI regression scenarios."""

import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
from seed_exe import patch_seed

ROOT = Path(__file__).resolve().parents[2]
SCENARIOS = ("noai", "oneai1", "oneai2", "oneai3", "oneai4", "oneai5", "oneai6", "smallai4", "smallai6")
CHECKPOINTS = (0, 1, 10, 25, 50, 80, 100)
SAVE_NAME = re.compile(r"game\.(xy|hst|[mhx](?:[1-9]|1[0-6]))$", re.I)


def digest(path):
    """digest hashes an input or output for the run manifest."""
    return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(args):
    """prepare stages identical definitions and race data without overwriting runs."""
    work = args.work.resolve()
    if any(character.isspace() for character in str(work)):
        raise ValueError("work path must not contain whitespace; Stars! cannot quote filenames")
    if work.exists():
        raise ValueError(f"work directory already exists: {work}; choose a fresh directory")
    if not 0 <= args.seed <= 0xFFFFFFFF:
        raise ValueError("seed must fit uint32")
    exe = args.exe.resolve()
    original = None
    if args.engine == "dosbox":
        relative = work.relative_to(ROOT / "starsbox/c_drive")
        if any(not re.fullmatch(r"[A-Za-z0-9_]{1,8}", part) for part in relative.parts):
            raise ValueError("DOSBox work path must use 8-character DOS directory names")
        original = exe.read_bytes()
        patched = patch_seed(original, args.seed)
    else:
        cache = exe.parent.parent / "CMakeCache.txt"
        if f"STARS_TEST_SEED:STRING={args.seed}\n" not in cache.read_text():
            raise ValueError(f"build {exe} with -DSTARS_TEST_SEED={args.seed} first")
        if not exe.is_file():
            raise ValueError(f"missing executable: {exe}")
    work.mkdir(parents=True)
    if original is not None:
        exe = work / "seeded.exe"
        exe.write_bytes(patched)
    manifest = {"engine": args.engine, "seed": args.seed, "exe": str(exe),
                "exe_sha256": digest(exe), "scenarios": {}, "fixtures": {}}
    manifest["race_sha256"] = digest(ROOT / "tests/scaffold/fixtures/newgame/tiny/humanoid.r1")
    if original is not None:
        manifest["original_sha256"] = hashlib.sha256(original).hexdigest()
    for name in SCENARIOS:
        dest = work / name
        dest.mkdir()
        lines = (ROOT / f"tests/scaffold/fixtures/regression/{name}.def").read_text().splitlines()
        lines[1] = lines[1].rsplit(" ", 1)[0] + f" {args.seed}"
        manifest["fixtures"][name] = hashlib.sha256("\n".join(lines).encode("ascii")).hexdigest()
        game_dir = windows_path(dest, args.engine)
        lines[4] = game_dir + "\\human.r1"
        lines[-1] = game_dir + "\\game.xy"
        (dest / "game.def").write_bytes(("\r\n".join(lines) + "\r\n").encode("ascii"))
        shutil.copyfile(ROOT / "tests/scaffold/fixtures/newgame/tiny/humanoid.r1", dest / "human.r1")
        manifest["scenarios"][name] = {p.name: digest(p) for p in dest.iterdir()}
    (work / "run.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"Prepared {args.engine} run: {work}")


def host_turn(path):
    """host_turn validates the unencrypted host header and returns its turn."""
    data = path.read_bytes()
    if len(data) < 18 or data[:2] != b"\x10\x20" or data[2:6] != b"J3J3" or data[16] != 2:
        raise ValueError(f"invalid host header: {path}")
    return struct.unpack_from("<H", data, 12)[0]


def checked_files(directory, checksums, label):
    """checked_files verifies saved files against their recorded hashes."""
    for filename, checksum in checksums.items():
        if digest(directory / filename) != checksum:
            raise ValueError(f"{label} changed: {directory / filename}")


def windows_path(path, engine):
    """windows_path converts a host path to the selected engine's drive mapping."""
    if engine == "dosbox":
        return "C:\\" + str(path.relative_to(ROOT / "starsbox/c_drive")).replace("/", "\\")
    return "Z:" + str(path).replace("/", "\\")


def copy_files(source, destination, filenames):
    """copy_files copies the named checkpoint files into a directory."""
    for filename in filenames:
        shutil.copyfile(source / filename, destination / filename)


def start_from_baseline(args, manifest, name, directory, snapshots, has_data):
    """start_from_baseline imports a verified turn-zero checkpoint."""
    if has_data or args.resume:
        raise ValueError("--baseline requires a fresh scenario and cannot be combined with --resume")
    baseline = args.baseline.resolve()
    reference = json.loads((baseline / "run.json").read_text())
    if any(reference[key] != manifest[key] for key in ("seed", "fixtures", "race_sha256")):
        raise ValueError("baseline seed or fixtures differ from this run")
    source = baseline / name / "checkpoints/000"
    saved = json.loads((source / "checkpoint.json").read_text())
    checked_files(source, saved["files"], "baseline checkpoint file")
    if host_turn(source / "game.hst") != 0:
        raise ValueError("baseline must start at turn zero")
    checkpoint = snapshots / "000"
    checkpoint.mkdir()
    copy_files(source, checkpoint, saved["files"])
    copy_files(source, directory, saved["files"])
    saved["origin"] = "baseline"
    saved["source_checkpoint"] = str(source)
    (checkpoint / "checkpoint.json").write_text(json.dumps(saved, indent=2) + "\n")
    print(f"{name}: using reference creation files from {source}; native creation is not tested")


def restore_checkpoint(args, name, directory, snapshots):
    """restore_checkpoint verifies the latest checkpoint and preserves live files."""
    completed = [t for t in CHECKPOINTS if (snapshots / f"{t:03}" / "checkpoint.json").is_file()]
    if not completed or completed != list(CHECKPOINTS[:len(completed)]):
        raise ValueError(f"{name}: resume requires consecutive completed checkpoints starting at 000")
    previous = completed[-1]
    if previous >= args.through:
        print(f"{name}: already completed through turn {previous}")
        return completed
    checkpoint = snapshots / f"{previous:03}"
    saved = json.loads((checkpoint / "checkpoint.json").read_text())
    checked_files(checkpoint, saved["files"], "checkpoint file")
    if host_turn(checkpoint / "game.hst") != previous:
        raise ValueError(f"{name}: checkpoint host turn is incorrect")
    preserved = Path(tempfile.mkdtemp(prefix="before-resume-", dir=directory))
    for path in directory.iterdir():
        if SAVE_NAME.fullmatch(path.name):
            shutil.move(path, preserved / path.name)
        elif path.name.startswith("run-") and path.suffix == ".log":
            shutil.copyfile(path, preserved / path.name)
    copy_files(checkpoint, directory, saved["files"])
    print(f"{name}: restored turn {previous}; previous live files preserved in {preserved}")
    return completed


def launch_command(engine, exe, directory, turn, previous):
    """launch_command builds the creation or incremental generation command."""
    game_path = windows_path(directory, engine)
    flags = ["-a", game_path + "\\game.def"] if turn == 0 else [f"-g{turn - previous}", game_path + "\\game.hst"]
    if engine == "dosbox":
        dos_exe = windows_path(exe, engine)
        return ([str(ROOT / "starsbox/dosbox.bin"), "-noconsole", "-conf", "stars_dosbox_macos.conf",
                 "-c", "c:", "-c", f"cd {game_path}", "-c", f"win /n {dos_exe} -x {' '.join(flags)}", "-c", "exit"],
                ROOT / "starsbox")
    return ["wine", str(exe), *flags], directory


def capture_checkpoint(directory, snapshots, name, turn, command, exit_code):
    """capture_checkpoint validates generated saves and records their hashes."""
    saves = {p.name.lower(): p for p in directory.iterdir() if SAVE_NAME.fullmatch(p.name)}
    if not {"game.xy", "game.hst"} <= saves.keys():
        raise ValueError(f"{name}: missing universe or host file at turn {turn}; see run log")
    actual = host_turn(saves["game.hst"])
    if actual != turn:
        raise ValueError(f"{name}: expected turn {turn}, found {actual}; stopping before snapshot")
    checkpoint = snapshots / f"{turn:03}"
    checkpoint.mkdir()
    for filename, path in saves.items():
        shutil.copyfile(path, checkpoint / filename)
    report = {"turn": turn, "year": 2400 + turn, "command": command, "exit_code": exit_code,
              "files": {filename: digest(path) for filename, path in saves.items()}}
    (checkpoint / "checkpoint.json").write_text(json.dumps(report, indent=2) + "\n")
    print(f"Saved {checkpoint}", flush=True)


def run(args):
    """run executes matching launch boundaries and saves verified checkpoints."""
    work = args.work.resolve()
    manifest = json.loads((work / "run.json").read_text())
    exe = Path(manifest["exe"])
    if digest(exe) != manifest["exe_sha256"]:
        raise ValueError("executable changed since prepare; prepare a fresh run")
    for name in args.scenario or SCENARIOS:
        directory = work / name
        snapshots = directory / "checkpoints"
        has_data = (snapshots.exists() and any(snapshots.iterdir())) or any(SAVE_NAME.fullmatch(p.name) for p in directory.iterdir())
        if has_data and not args.resume:
            raise ValueError(f"{name} already has run data; prepare a fresh run")
        checked_files(directory, manifest["scenarios"][name], "fixture")
        snapshots.mkdir(exist_ok=True)
        completed = []
        if args.baseline:
            start_from_baseline(args, manifest, name, directory, snapshots, has_data)
            completed = [0]
        if args.resume:
            completed = restore_checkpoint(args, name, directory, snapshots)
            if completed[-1] >= args.through:
                continue
        previous = completed[-1] if completed else 0
        for turn in CHECKPOINTS:
            if turn in completed:
                continue
            if turn > args.through:
                break
            command, cwd = launch_command(manifest["engine"], exe, directory, turn, previous)
            print(f"{name}: turn {turn}: {' '.join(command)}", flush=True)
            with (directory / f"run-{turn:03}.log").open("w") as log:
                process = subprocess.run(command, cwd=cwd, stdout=log, stderr=subprocess.STDOUT,
                                         check=False, timeout=args.timeout)
            # FGenerateTurn sets vretExitValue=1 on successful command-line generation.
            expected_exit = 1 if manifest["engine"] == "native" and turn > 0 else 0
            if process.returncode != expected_exit:
                raise ValueError(f"{name}: exit {process.returncode}, expected {expected_exit}; see run-{turn:03}.log")
            if turn == 0:
                generated = {p.name.lower(): p for p in directory.iterdir()}
                if "game.m1" not in generated or "game.hst" not in generated:
                    raise ValueError(f"{name}: missing player turn or host file after game creation")
                player_file = generated["game.m1"]
                subprocess.run([str(args.cli.resolve()), "save", "update", str(player_file), "--ai", "maid"],
                               cwd=ROOT, check=True)
                subprocess.run([str(args.cli.resolve()), "save", "update", str(generated["game.hst"]),
                                "--ai", "maid", "--player", "1"], cwd=ROOT, check=True)
            capture_checkpoint(directory, snapshots, name, turn, command, process.returncode)
            previous = turn


def compare(args):
    """compare checks every checkpoint and reports missing or differing saves."""
    cli = args.cli.resolve()
    manifests = [json.loads((p / "run.json").read_text()) for p in (args.left, args.right)]
    if any(m["seed"] != manifests[0]["seed"] or m["fixtures"] != manifests[0]["fixtures"]
           or m["race_sha256"] != manifests[0]["race_sha256"] for m in manifests):
        raise ValueError("runs have different seeds or fixtures")
    results = []
    for name in args.scenario or SCENARIOS:
        for turn in CHECKPOINTS:
            if turn > args.through:
                break
            dirs = [p / name / "checkpoints" / f"{turn:03}" for p in (args.left, args.right)]
            if any(not d.is_dir() for d in dirs):
                results.append({"scenario": name, "turn": turn, "error": "missing checkpoint"})
                continue
            files = [{p.name.lower(): p for p in d.iterdir() if SAVE_NAME.fullmatch(p.name)} for d in dirs]
            if turn == 0 and any(json.loads((d / "checkpoint.json").read_text()).get("origin") == "baseline" for d in dirs):
                results.append({"scenario": name, "turn": turn, "skipped": "reference input; creation not tested"})
                continue
            for filename in sorted(files[0].keys() | files[1].keys() | {"game.xy", "game.hst"}):
                result = {"scenario": name, "turn": turn, "file": filename}
                if any(filename not in f for f in files):
                    result["error"] = "missing file"
                else:
                    proc = subprocess.run([str(cli), "save", "compare", str(files[0][filename]), str(files[1][filename])],
                                          cwd=ROOT, text=True, capture_output=True)
                    result["match"] = proc.returncode == 0
                    result["detail"] = proc.stdout + proc.stderr
                results.append(result)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(results, indent=2) + "\n")
    failures = sum(not r.get("match", False) and "skipped" not in r for r in results)
    matches = sum(r.get("match", False) for r in results)
    skipped = sum("skipped" in r for r in results)
    print(f"{matches} matches; {failures} differences/errors; {skipped} reference inputs skipped. Report: {args.report}")
    return bool(failures)


def main():
    """main dispatches regression staging, execution, and comparison."""
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="action", required=True)
    stage = sub.add_parser("prepare")
    stage.add_argument("--engine", choices=("native", "dosbox"), required=True)
    stage.add_argument("--work", type=Path, required=True)
    stage.add_argument("--exe", type=Path, required=True)
    stage.add_argument("--seed", type=int, default=12345)
    execute = sub.add_parser("run")
    execute.add_argument("--work", type=Path, required=True)
    execute.add_argument("--scenario", choices=SCENARIOS, action="append")
    execute.add_argument("--through", type=int, choices=CHECKPOINTS, default=100)
    execute.add_argument("--resume", action="store_true", help="restore the latest checkpoint and continue; preserve current saves")
    execute.add_argument("--baseline", type=Path, help="start from this run's creation checkpoint to test turn generation separately")
    execute.add_argument("--timeout", type=int, default=900, help="seconds per game launch")
    execute.add_argument("--cli", type=Path, default=ROOT / "dist/stars-asm", help="stars-asm CLI used to update the human player")
    diff = sub.add_parser("compare")
    diff.add_argument("left", type=Path)
    diff.add_argument("right", type=Path)
    diff.add_argument("--scenario", choices=SCENARIOS, action="append")
    diff.add_argument("--through", type=int, choices=CHECKPOINTS, default=100)
    diff.add_argument("--cli", type=Path, default=ROOT / "dist/stars-asm")
    diff.add_argument("--report", type=Path, default=ROOT / "tests/scaffold/fixtures/regression/regression-comparison.json")
    args = parser.parse_args()
    try:
        return {"prepare": prepare, "run": run, "compare": compare}[args.action](args) or 0
    except (ValueError, OSError, subprocess.SubprocessError, struct.error) as exc:
        print(f"FAIL: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
