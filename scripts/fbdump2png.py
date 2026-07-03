#!/usr/bin/env python3
"""Convert an EZODUS_DUMP framebuffer dump (256*u64 bgr48 palette + 640x480
8bpp pixels) to PNG. Usage: fbdump2png.py DUMP PNG"""
import struct, sys, zlib

def main():
    data = open(sys.argv[1], 'rb').read()
    pal = struct.unpack('<256Q', data[:2048])
    px = data[2048:2048 + 640 * 480]
    rgb = []
    for c in pal:
        b = (c & 0xFFFF) >> 8
        g = ((c >> 16) & 0xFFFF) >> 8
        r = ((c >> 32) & 0xFFFF) >> 8
        rgb.append(bytes((r, g, b)))
    raw = b''.join(b'\x00' + b''.join(rgb[p] for p in px[y*640:(y+1)*640])
                   for y in range(480))
    def chunk(tag, payload):
        c = tag + payload
        return struct.pack('>I', len(payload)) + c + struct.pack('>I', zlib.crc32(c))
    png = (b'\x89PNG\r\n\x1a\n'
           + chunk(b'IHDR', struct.pack('>IIBBBBB', 640, 480, 8, 2, 0, 0, 0))
           + chunk(b'IDAT', zlib.compress(raw))
           + chunk(b'IEND', b''))
    open(sys.argv[2], 'wb').write(png)

main()
