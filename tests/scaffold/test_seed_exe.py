"""Checks the binary patch against the repository's original executable."""
import struct
import unittest
from pathlib import Path
import sys

sys.dont_write_bytecode = True
from seed_exe import patch_seed


class SeedPatchTest(unittest.TestCase):
    """SeedPatchTest verifies instructions, relocations, and mismatch rejection."""

    def test_original_patch(self):
        """test_original_patch preserves other relocations and handles both seed words."""
        data = (Path(__file__).resolve().parents[2] / "dasm/input/stars.exe").read_bytes()
        ne = struct.unpack_from("<I", data, 0x3C)[0]
        table = ne + struct.unpack_from("<H", data, ne + 0x22)[0]
        sector, size = struct.unpack_from("<HH", data, table + 24)
        base = sector << (struct.unpack_from("<H", data, ne + 0x32)[0] or 9)
        rel = base + size
        count = struct.unpack_from("<H", data, rel)[0]
        patched = patch_seed(data, 0x12345678)
        self.assertEqual(len(data), len(patched))
        self.assertEqual(patched[base + 0x85:base + 0x8C], bytes.fromhex("68 34 12 68 78 56 90"))
        self.assertEqual(struct.unpack_from("<H", patched, rel)[0], count - 1)
        old = [data[rel + 2 + i * 8:rel + 10 + i * 8] for i in range(count)]
        old.remove(bytes.fromhex("03 01 86 00 04 00 0d 00"))
        self.assertEqual(patched[rel + 2:rel + 2 + 8 * (count - 1)], b"".join(old))
        restored = bytearray(patched)
        restored[base + 0x85:base + 0x8C] = data[base + 0x85:base + 0x8C]
        restored[rel:rel + 2 + count * 8] = data[rel:rel + 2 + count * 8]
        self.assertEqual(bytes(restored), data)
        with self.assertRaises(ValueError):
            patch_seed(patched, 12345)
        with self.assertRaises(ValueError):
            patch_seed(data, -1)


if __name__ == "__main__":
    unittest.main()
