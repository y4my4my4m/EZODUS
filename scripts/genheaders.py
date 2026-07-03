#!/usr/bin/env python3
"""Generate Z/HeadersGen.HH: extern headers for every global function and
variable AOT-compiled into HCRT.ZXE.

The EXODUS runtime JIT-compiles user HolyC against the AOT kernel image. The
JIT only knows symbol addresses (from the ZXE export table); argument lists
and default values must be re-declared at boot. T/ (TempleOS) ships a
generated KernelA.HH with thousands of extern headers; this script produces
the ZealOS equivalent from the sources listed in the FULL_PACKAGE include
graph. LoadImps (Z/FULL_PACKAGE.ZC) compiles the result at boot before
BootstrapImportSymbol patches the addresses in.

Usage: python3 scripts/genheaders.py  (from repo root)
"""
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
Z = os.path.join(ROOT, "Z")
OUT = os.path.join(Z, "HeadersGen.HH")

ENTRY = ["FULL_PACKAGE.ZC"]
# Headers already compiled at boot before HeadersGen.HH; skip anything
# (classes, functions) they declare.
PRELOADED = ["Kernel/KernelA.HH", "Kernel/KernelB.HH", "Compiler/CompilerA.HH"]

IDENT = r"[A-Za-z_][A-Za-z_0-9]*"


def read(path):
    with open(path, encoding="latin-1") as f:
        return f.read()


def strip_comments(text):
    """Remove // and /* */ comments, preserving strings/chars and newlines."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == '"' or c == "'":
            q = c
            out.append(c)
            i += 1
            while i < n and text[i] != q:
                if text[i] == "\\":
                    out.append(text[i : i + 2])
                    i += 2
                else:
                    out.append(text[i])
                    i += 1
            if i < n:
                out.append(q)
                i += 1
        elif text.startswith("//", i):
            while i < n and text[i] != "\n":
                i += 1
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append("\n" * text.count("\n", i, j))
            i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


def blank_strings(text):
    """Replace string/char literal contents with spaces (offsets preserved)."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == '"' or c == "'":
            j = i + 1
            while j < n and text[j] != c:
                step = 2 if text[j] == "\\" else 1
                for k in range(j, min(j + step, n)):
                    if out[k] != "\n":
                        out[k] = " "
                j += step
            i = j + 1
        else:
            i += 1
    return "".join(out)


def include_graph(entry_files):
    """Return the .ZC files reachable from the entry points, in include order."""
    seen, order = set(), []

    def visit(relpath, from_dir):
        for base in (from_dir, Z):
            path = os.path.normpath(os.path.join(base, relpath))
            if os.path.isfile(path):
                break
            if not os.path.splitext(path)[1]:
                path += ".ZC"
                if os.path.isfile(path):
                    break
        else:
            return
        rel = os.path.relpath(path, Z)
        if rel in seen:
            return
        seen.add(rel)
        text = strip_comments(read(path))
        if rel.endswith(".ZC"):
            order.append(rel)
        for m in re.finditer(r'#include\s+"([^"]+)"', text):
            visit(m.group(1), os.path.dirname(path))

    for e in entry_files:
        visit(e, Z)
    return order


def preloaded_names():
    """Class and function names already declared by the preloaded headers."""
    classes, funs = set(), set()
    for rel in PRELOADED:
        text = strip_comments(read(os.path.join(Z, rel)))
        for m in re.finditer(r"\bclass\s+(%s)" % IDENT, text):
            classes.add(m.group(1))
        for m in re.finditer(r"\b(%s)\s*\(" % IDENT, text):
            funs.add(m.group(1))
    return classes, funs


def top_level_statements(text):
    """Yield (statement, ends_with_brace) for brace-depth-0 statements.

    Skips preprocessor lines, asm{} and #exe{} blocks, and function bodies.
    """
    i, n = 0, len(text)
    stmt = []
    while i < n:
        c = text[i]
        if c == "#":
            # preprocessor: #exe{} may span lines/braces, others end at EOL
            m = re.match(r"#\s*(%s)" % IDENT, text[i:])
            kw = m.group(1) if m else ""
            if kw in ("exe", "if", "ifdef", "ifndef", "else", "endif",
                      "include", "define", "help_index", "help_file"):
                if kw == "exe":
                    j = text.find("{", i)
                    if j >= 0:
                        i = skip_braces(text, j)
                        continue
                while i < n and text[i] != "\n":
                    if text[i] == "\\" and i + 1 < n and text[i + 1] == "\n":
                        i += 1
                    i += 1
                continue
        if c == '"' or c == "'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            stmt.append(text[i : j + 1])
            i = j + 1
            continue
        if c == "{":
            yield "".join(stmt).strip(), True
            stmt = []
            i = skip_braces(text, i)
            # eat trailing ; of "class X {...};" so it doesn't open an
            # empty statement
            while i < n and text[i] in " \t\n;":
                i += 1
            continue
        if c == ";":
            s = "".join(stmt).strip()
            if s:
                yield s, False
            stmt = []
            i += 1
            continue
        stmt.append(c)
        i += 1


def skip_braces(text, i):
    """i points at '{'; return index past the matching '}'."""
    depth = 0
    n = len(text)
    while i < n:
        c = text[i]
        if c == '"' or c == "'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j + 1
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return n


def split_declarators(s):
    """Split 'a = x, *b, c[N]' at top-level commas."""
    parts, depth, cur = [], 0, []
    for c in s:
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == "," and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(c)
    if cur:
        parts.append("".join(cur))
    return parts


def strip_initializer(decl):
    depth = 0
    for i, c in enumerate(decl):
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        elif c == "=" and depth == 0:
            return decl[:i].rstrip()
    return decl.strip()


FUN_RE = re.compile(
    r"^(?:public\s+)?(?:interrupt\s+)?(%s)\s*(\**)\s*(%s)\s*\(" % (IDENT, IDENT)
)
SKIP_KW = ("extern", "import", "_extern", "_import", "_intern", "static",
           "class", "union", "asm", "if", "else", "for", "while", "switch",
           "return", "goto", "try", "catch", "do", "lock")


def main():
    files = include_graph(ENTRY)
    pre_classes, pre_funs = preloaded_names()
    seen_funs, seen_vars, seen_classes = set(), set(), set()
    fun_lines, var_lines, class_lines = [], [], []

    for rel in files:
        text = strip_comments(read(os.path.join(Z, rel)))
        for stmt, has_body in top_level_statements(text):
            stmt = re.sub(r"\s+", " ", stmt).strip()
            if not stmt:
                continue
            first = re.match(IDENT, stmt)
            first = first.group(0) if first else ""
            if first in ("class", "union") or stmt.startswith("public class") \
               or stmt.startswith("public union"):
                continue  # class bodies handled separately below
            if first in SKIP_KW or stmt.startswith("public extern") \
               or stmt.startswith("public _extern") \
               or stmt.startswith("public _intern") \
               or stmt.startswith("public import"):
                continue
            m = FUN_RE.match(stmt)
            if has_body and m and m.group(1) not in ("new", "delete"):
                name = m.group(3)
                if name in seen_funs or name in pre_funs:
                    continue
                seen_funs.add(name)
                pub = "public " if stmt.startswith("public") else ""
                sig = stmt[len("public"):].lstrip() if pub else stmt
                fun_lines.append("%sextern %s;" % (pub, sig))
            elif not has_body and "(" in stmt and stmt.count("(") and \
                    re.match(r"^(?:public\s+)?%s\s*\**\s*\(\s*\*" % IDENT, stmt):
                # global function pointer: U0 (*fp_x)(...)
                nm = re.search(r"\(\s*\*\s*(%s)" % IDENT, stmt)
                if nm and nm.group(1) not in seen_vars:
                    seen_vars.add(nm.group(1))
                    var_lines.append("extern %s;" % strip_initializer(stmt))
            elif not has_body and "(" not in stmt.split("=", 1)[0]:
                # plain global variable(s)
                tm = re.match(r"^(?:public\s+)?(%s)([ \t*&]+)(.*)$" % IDENT, stmt)
                if not tm or tm.group(1) in SKIP_KW:
                    continue
                base, rest = tm.group(1), tm.group(2) + tm.group(3)
                decls = []
                for d in split_declarators(rest):
                    d = strip_initializer(d)
                    nm = re.search(IDENT, d)
                    if not d or not nm:
                        continue
                    if nm.group(0) in seen_vars:
                        continue
                    seen_vars.add(nm.group(0))
                    decls.append(d)
                if decls:
                    var_lines.append("extern %s %s;" % (base, ", ".join(decls)))

        # top-level class/union definitions not already in preloaded headers;
        # scan a string-blanked copy so literals containing "class X {" (e.g.
        # CtrlsSlider's generated code) can't match
        blanked = blank_strings(text)
        for m in re.finditer(
            r"^(?:public\s+)?(class|union)\s+(%s)(\s*:\s*%s)?\s*\{" % (IDENT, IDENT),
            blanked, re.M,
        ):
            name = m.group(2)
            if name in pre_classes or name in seen_classes:
                continue
            seen_classes.add(name)
            start = blanked.index("{", m.start())
            end = skip_braces(blanked, start)
            class_lines.append(text[m.start() : end].strip() + ";")

    with open(OUT, "w", encoding="latin-1", newline="\n") as f:
        f.write("// Generated by scripts/genheaders.py -- do not edit.\n")
        f.write("// Extern headers for the AOT-compiled HCRT public API;\n")
        f.write("// compiled at boot by LoadImps (see FULL_PACKAGE.ZC).\n\n")
        for l in class_lines:
            f.write(l + "\n")
        f.write("\n")
        for l in var_lines:
            f.write(l + "\n")
        f.write("\n")
        for l in fun_lines:
            f.write(l + "\n")
    print("files scanned: %d" % len(files))
    print("classes: %d  vars: %d  funs: %d"
          % (len(class_lines), len(var_lines), len(fun_lines)))
    print("wrote %s" % OUT)


if __name__ == "__main__":
    main()
