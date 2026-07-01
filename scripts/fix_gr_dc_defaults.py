#!/usr/bin/env python3
"""T seed PrsVarLst faults on `CDC *dc=gr.dc` default args. Use NULL + body guard."""
from pathlib import Path
import re

ROOT = Path("/home/y4my4m/gits/Ezodus/Z")
SIG = re.compile(r"CDC \*dc=gr\.dc")
GUARD = "\tif (!dc)\n\t\tdc = gr.dc;\n"


def insert_guard(text: str, after_brace: int) -> str:
    i = after_brace
    while i < len(text) and text[i] in " \t\r\n":
        i += 1
    if i + 2 <= len(text) and text[i : i + 2] == "//":
        nl = text.find("\n", i)
        i = len(text) if nl == -1 else nl + 1
        while i < len(text) and text[i] in " \t\r\n":
            i += 1
    if re.match(r"if\s*\(\s*!dc\s*\)", text[i:].lstrip()):
        return text
    return text[:i] + GUARD + text[i:]


def fix_file(path: Path) -> int:
    text = path.read_bytes().decode("latin-1")
    if "CDC *dc=gr.dc" not in text:
        return 0

    count = len(SIG.findall(text))
    text = SIG.sub("CDC *dc=NULL", text)

    for m in reversed(list(re.finditer(r"CDC \*dc=NULL", text))):
        brace = text.find("{", m.end())
        if brace != -1:
            text = insert_guard(text, brace + 1)

    path.write_bytes(text.encode("latin-1"))
    return count


def main():
    total = 0
    for path in sorted(ROOT.rglob("*.ZC")):
        c = fix_file(path)
        if c:
            print(f"{path}: {c}")
            total += c
    print(f"total: {total}")


if __name__ == "__main__":
    main()
