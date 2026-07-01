#!/usr/bin/env python3
"""Test which MakeGr include triggers PrsVarLst with full HCRT_TOS tail."""
from pathlib import Path
import subprocess

ROOT = Path("/home/y4my4m/gits/Ezodus")
MAKEGR = ROOT / "Z/System/Gr/MakeGr.ZC"

BASE = """#exe {Cd(__DIR__);};
#define GR_WIDTH 640
#define GR_HEIGHT 480
#define TEXT_ROWS 24
#define TEXT_COLS 80
#include "GrInitA.ZC"
#include "Gr.HH"
#include "GrExterns.ZC"
#include "GrGlobals.ZC"
#include "GrTextBase.ZC"
#include "GrAsm.ZC"
#include "GrPalette.ZC"
#include "GrDC.ZC"
#include "GrInitB.ZC"
"""

TESTS = {
    "base_only": [],
    "grmath": ["GrMath.ZC"],
    "grmath_part": ['"_GrMathPart.ZC"'],
    "screen": ["GrScreen.ZC"],
    "prim": ["GrPrimatives.ZC"],
    "initb_math_screen": ["GrMath.ZC", "GrScreen.ZC"],
    "skip_math_rest": ["GrScreen.ZC", "GrBitMap.ZC", "GrPrimatives.ZC"],
}


def run(label, includes):
    lines = [BASE]
    for inc in includes:
        if inc.startswith('"'):
            lines.append(f"#include {inc}")
        else:
            lines.append(f'#include "{inc}"')
    lines.append('#exe {Drv("Z"); Cd("/");};')
    MAKEGR.write_text("\n".join(lines) + "\n")
    r = subprocess.run(
        ["./ezodus", "-c", "-t", "T", "-f", "HCRT_BOOTSTRAP.BIN", "BuildHCRT.ZC"],
        cwd=ROOT,
        capture_output=True,
        text=True,
        timeout=120,
    )
    out = r.stdout + r.stderr
    if "PrsVarLst" in out or "ENTERED FAULT" in out:
        st = "FAULT"
    elif "ERROR:" in out:
        st = "ERROR"
    else:
        st = "OK/EXCEPT"
    print(f"{label:25s} -> {st}")


def main():
    # ensure part file = full grmath
    src = (ROOT / "Z/System/Gr/GrMath.ZC").read_bytes()
    (ROOT / "Z/System/Gr/_GrMathPart.ZC").write_bytes(src)
    for label, incs in TESTS.items():
        run(label, incs)


if __name__ == "__main__":
    main()
