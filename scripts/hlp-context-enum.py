#!/usr/bin/env python3
"""Generate the HelpContextId enum from a WinHelp 3.1 .hlp file.

WinHelp's HELP_CONTEXT data is a [MAP] context number. The help file's
|CTXOMAP internal file maps each number to a topic offset, and |TTLBTREE maps
topic offsets to topic titles. This prints one enum member per context number,
named idh + the CamelCase title, with the title as a comment. A title shared by
several topics gets the context number as a suffix.

Usage:
    scripts/hlp-context-enum.py [HLP]           print the enum
    scripts/hlp-context-enum.py [HLP] --write   replace it in dasm/input/enums.h
"""

import argparse
import bisect
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_HLP = ROOT / "starsbox/c_drive/STARS/stars!.hlp"
ENUMS_H = ROOT / "dasm/input/enums.h"
HLP_MAGIC = 0x00035F3F


class HelpFile:
    """HelpFile reads the internal files of a WinHelp 3.1 help file."""

    def __init__(self, data):
        magic, directory, _, _ = struct.unpack_from("<IiiI", data, 0)
        if magic != HLP_MAGIC:
            raise ValueError(f"not a WinHelp file: magic {magic:#x}")
        self.data = data
        self.files = dict(self.leaf_entries(directory, self.directory_entry))

    def btree(self, offset):
        """btree returns the layout of the B+ tree stored in the internal
        file at offset, whose 9-byte file header precedes the tree header."""
        header = offset + 9
        page_size, = struct.unpack_from("<H", self.data, header + 4)
        root, _, _, levels, _ = struct.unpack_from("<hhhhi", self.data, header + 26)
        return {"page_size": page_size, "root": root, "levels": levels, "pages": header + 38}

    def leaf_entries(self, offset, read_entry):
        """leaf_entries returns every entry of the B+ tree in the internal
        file at offset, read in key order by read_entry(position)."""
        tree = self.btree(offset)
        page = tree["root"]
        for _ in range(tree["levels"] - 1):
            # an index page starts with the page of its leftmost child
            page, = struct.unpack_from("<h", self.data, tree["pages"] + page * tree["page_size"] + 4)
        entries = []
        while page != -1:
            start = tree["pages"] + page * tree["page_size"]
            _, count, _, next_page = struct.unpack_from("<HHhh", self.data, start)
            position = start + 8
            for _ in range(count):
                entry, position = read_entry(position)
                entries.append(entry)
            page = next_page
        return entries

    def directory_entry(self, position):
        """directory_entry reads an internal file name and its offset."""
        end = self.data.index(b"\0", position)
        offset, = struct.unpack_from("<i", self.data, end + 1)
        return (self.data[position:end].decode("latin-1"), offset), end + 5

    def title_entry(self, position):
        """title_entry reads a topic offset and its title."""
        topic, = struct.unpack_from("<i", self.data, position)
        end = self.data.index(b"\0", position + 4)
        return (topic, self.data[position + 4:end].decode("latin-1")), end + 1

    def contexts(self):
        """contexts returns (context number, title) for each [MAP] entry. A
        topic starting on a block boundary may be keyed differently in the
        title tree, so it takes the title of the nearest preceding topic."""
        offset = self.files["|CTXOMAP"]
        count, = struct.unpack_from("<H", self.data, offset + 9)
        mapped = [struct.unpack_from("<ii", self.data, offset + 11 + 8 * i) for i in range(count)]
        titles = dict(self.leaf_entries(self.files["|TTLBTREE"], self.title_entry))
        offsets = sorted(titles)
        result = []
        for number, topic in sorted(mapped):
            if topic in titles:
                result.append((number, titles[topic]))
                continue
            index = bisect.bisect_right(offsets, topic) - 1
            if index < 0:
                raise ValueError(f"context {number}: no title at or before topic offset {topic}")
            result.append((number, titles[offsets[index]]))
        return result


def camel(title):
    """camel returns a topic title as a CamelCase identifier fragment."""
    title = title.replace("'s", "s").replace("&", " and ").replace("%", " Pct ").replace("#", " Num ")
    words = re.findall(r"[A-Za-z0-9]+", title)
    return "".join(word[:1].upper() + word[1:] for word in words)[:48] or "Topic"


def enum_text(contexts):
    """enum_text renders the HelpContextId enum for contexts."""
    uses = {}
    for _, title in contexts:
        uses[camel(title)] = uses.get(camel(title), 0) + 1
    names = set()
    lines = []
    for number, title in contexts:
        name = "idh" + camel(title)
        if uses[camel(title)] > 1:
            name += str(number)
        if name[3:4].isdigit():
            name = "idh_" + name[3:]
        if name in names:
            raise ValueError(f"duplicate member {name}")
        names.add(name)
        lines.append(f"    {name} = {number}, // {title}")
    return (
        "// HelpContextId is a topic of stars!.hlp, as WinHelp's HELP_CONTEXT data\n"
        "// selects it: the [MAP] numbers of the help file's |CTXOMAP, named by topic\n"
        "// title. Many share their number with the dialog control they explain.\n"
        "// Generated by scripts/hlp-context-enum.py.\n"
        "typedef enum HelpContextId {\n" + "\n".join(lines) + "\n} HelpContextId;\n"
    )


def write_enum(text):
    """write_enum replaces the HelpContextId enum and its comment in enums.h."""
    source = ENUMS_H.read_text()
    match = re.search(r"(?:^//[^\n]*\n)*typedef enum HelpContextId \{.*?\} HelpContextId;\n", source, re.S | re.M)
    if match is None:
        raise ValueError(f"HelpContextId not found in {ENUMS_H}")
    ENUMS_H.write_text(source[:match.start()] + text + source[match.end():])


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("hlp", nargs="?", type=Path, default=DEFAULT_HLP, help="help file (default: %(default)s)")
    parser.add_argument("--write", action="store_true", help=f"replace the enum in {ENUMS_H.relative_to(ROOT)}")
    args = parser.parse_args()
    text = enum_text(HelpFile(args.hlp.read_bytes()).contexts())
    if args.write:
        write_enum(text)
    else:
        sys.stdout.write(text)


if __name__ == "__main__":
    main()
