#!/usr/bin/env python3
"""Patch the verified Stars! 2.7j WinMain RNG call in a separate NE executable."""

import argparse
import hashlib
import json
from pathlib import Path
import struct


def patch_seed(data, seed):
    """patch_seed replaces the clock call and removes its loader relocation."""
    if not 0 <= seed <= 0xFFFFFFFF:
        raise ValueError("seed must fit uint32")
    b = bytearray(data)
    if len(b) < 64 or b[:2] != b"MZ":
        raise ValueError("expected an MZ/NE executable")
    ne = struct.unpack_from("<I", b, 0x3C)[0]
    if b[ne:ne + 2] != b"NE":
        raise ValueError("expected an NE executable")
    table = ne + struct.unpack_from("<H", b, ne + 0x22)[0]
    shift = struct.unpack_from("<H", b, ne + 0x32)[0] or 9
    sector, size, flags, _ = struct.unpack_from("<4H", b, table + 3 * 8)
    base = sector << shift
    site = base + 0x85
    expected = bytes.fromhex("9a ff ff 00 00 52 50 9a 5a 16 7a 00 83 c4 04")
    if b[site:site + len(expected)] != expected or not flags & 0x100:
        raise ValueError("WinMain 0004:0085 does not match the supported unpatched binary")
    rel = base + (size or 65536)
    count = struct.unpack_from("<H", b, rel)[0]
    entries = [bytes(b[rel + 2 + i * 8:rel + 10 + i * 8]) for i in range(count)]
    # Far imported ordinal fixup: source 0086, module 4 (KERNEL), ordinal 13.
    target = bytes.fromhex("03 01 86 00 04 00 0d 00")
    if entries.count(target) != 1:
        raise ValueError("expected exactly one GetTickCount relocation at 0004:0086")
    # The original ffff operand terminates this relocation chain. No other
    # call site is affected. The following Randomize2 relocation stays intact.
    entries.remove(target)
    struct.pack_into("<H", b, rel, count - 1)
    b[rel + 2:rel + 2 + count * 8] = b"".join(entries) + bytes(8)
    # Win16 uint32 argument: high word first, then low word (80186+ PUSH imm16).
    b[site:site + 7] = b"\x68" + struct.pack("<H", seed >> 16) + b"\x68" + struct.pack("<H", seed & 65535) + b"\x90"
    return bytes(b)


def main():
    """main writes a new patched executable and its provenance manifest."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--seed", type=int, default=12345)
    args = parser.parse_args()
    if args.source.resolve() == args.output.resolve() or args.output.exists():
        parser.error("output must be a new file; the original is never overwritten")
    source = args.source.read_bytes()
    try:
        patched = patch_seed(source, args.seed)
    except (ValueError, struct.error) as exc:
        parser.error(str(exc))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(patched)
    manifest = {"seed": args.seed, "source": str(args.source.resolve()),
                "source_sha256": hashlib.sha256(source).hexdigest(),
                "patched_sha256": hashlib.sha256(patched).hexdigest(),
                "site": "0004:0085", "removed_relocation": "0004:0086"}
    args.output.with_suffix(".json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"Patched {args.output}: startup seed {args.seed}")


if __name__ == "__main__":
    main()
