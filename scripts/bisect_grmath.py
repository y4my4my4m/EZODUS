#!/usr/bin/env python3
from pathlib import Path
import subprocess

ROOT = Path("/home/y4my4m/gits/Ezodus")
FULL = (ROOT / "Z/System/Gr/GrMath.ZC").read_bytes().split(b"\n")
FULL = [ln.decode("latin-1") for ln in FULL]
MAKEGR = ROOT / "Z/System/Gr/MakeGr.ZC"
HEADER = """#exe {Cd(__DIR__);};
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


def test_with_grmath_lines(n):
    partial = "\n".join(FULL[:n]) + "\n"
    (ROOT / "Z/System/Gr/_GrMathPart.ZC").write_text(partial)
    MAKEGR.write_text(HEADER + '#include "_GrMathPart.ZC"\n#exe {Drv("Z"); Cd("/");};\n')
    r = subprocess.run(
        ["./ezodus", "-c", "-t", "T", "-f", "HCRT_BOOTSTRAP.BIN", "BuildHCRT.ZC"],
        cwd=ROOT,
        capture_output=True,
        text=True,
        timeout=120,
    )
    out = r.stdout + r.stderr
    if "PrsVarLst" in out or "ENTERED FAULT" in out:
        return "FAULT"
    if "ERROR:" in out:
        for line in out.splitlines():
            if "ERROR:" in line:
                return "ERROR " + line.strip()[:100]
    return "OK"


def main():
    checkpoints = [50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 800, 900, len(FULL)]
    lo, hi = 1, len(FULL)
    last_ok = 0
    first_fault = len(FULL) + 1
    for n in checkpoints:
        if n > len(FULL):
            n = len(FULL)
        st = test_with_grmath_lines(n)
        print(f"{n:4d} {st}")
        if st == "FAULT":
            first_fault = min(first_fault, n)
        else:
            last_ok = max(last_ok, n)
    print(f"last_ok={last_ok} first_fault_at_or_before={first_fault}")
    # narrow
    if first_fault <= len(FULL):
        lo, hi = last_ok + 1, first_fault
        while lo < hi:
            mid = (lo + hi) // 2
            st = test_with_grmath_lines(mid)
            print(f"  {mid:4d} {st}")
            if st == "FAULT":
                hi = mid
            else:
                lo = mid + 1
        print(f"FAULT starts near line {lo}:")
        for i in range(max(1, lo - 5), min(len(FULL), lo + 5)):
            print(f"  {i:4d} {FULL[i-1][:100]}")


if __name__ == "__main__":
    main()
