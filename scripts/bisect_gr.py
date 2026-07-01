#!/usr/bin/env python3
from pathlib import Path
import subprocess

FILES = [
    "GrInitA.ZC", "Gr.HH", "GrExterns.ZC", "GrGlobals.ZC", "GrTextBase.ZC", "GrAsm.ZC",
    "GrPalette.ZC", "GrDC.ZC", "GrInitB.ZC", "GrMath.ZC", "GrScreen.ZC", "GrBitMap.ZC",
    "GrPrimatives.ZC", "GrComposites.ZC", "ScreenCast.ZC", "SpriteNew.ZC", "GrSpritePlot.ZC",
    "SpriteMesh.ZC", "SpriteBitMap.ZC", "SpriteCode.ZC", "SpriteSideBar.ZC", "SpriteEd.ZC",
    "SpriteMain.ZC", "GrEnd.ZC",
]
MAKEGR = Path("/home/y4my4m/gits/Ezodus/Z/System/Gr/MakeGr.ZC")
ROOT = Path("/home/y4my4m/gits/Ezodus")


def write_makegr(n):
    lines = [
        "#exe {Cd(__DIR__);};",
        "#define GR_WIDTH 640",
        "#define GR_HEIGHT 480",
        "#define TEXT_ROWS 24",
        "#define TEXT_COLS 80",
    ]
    for f in FILES[:n]:
        lines.append(f'#include "{f}"')
    lines.append('#exe {Drv("Z"); Cd("/");};')
    MAKEGR.write_text("\n".join(lines) + "\n")


def test(n):
    write_makegr(n)
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
        return "ERROR"
    if "Except:" in out:
        return "EXCEPT"
    return "OK"


def main():
    for n in range(1, len(FILES) + 1):
        status = test(n)
        print(f"{n:2d} {FILES[n - 1]:20s} -> {status}")
        if status in ("FAULT", "ERROR"):
            break


if __name__ == "__main__":
    main()
