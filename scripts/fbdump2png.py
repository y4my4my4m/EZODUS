#!/usr/bin/env python3
"""Convert an EZODUS_DUMP framebuffer dump ("EZDF" + u32 w,h + 256*u64 bgr48
palette + WxH 8bpp pixels) to PNG. Usage: fbdump2png.py DUMP PNG"""
import struct, sys, zlib

def main():
    data = open(sys.argv[1], 'rb').read()
    if data[:4] == b'EZDF':
        w, h = struct.unpack('<II', data[4:12])
        data = data[12:]
    else:  # pre-header dumps
        w, h = 640, 480
    pal = struct.unpack('<256Q', data[:2048])
    px = data[2048:2048 + w * h]
    rgb = []
    for c in pal:
        b = (c & 0xFFFF) >> 8
        g = ((c >> 16) & 0xFFFF) >> 8
        r = ((c >> 32) & 0xFFFF) >> 8
        rgb.append(bytes((r, g, b)))
    raw = b''.join(b'\x00' + b''.join(rgb[p] for p in px[y*w:(y+1)*w])
                   for y in range(h))
    def chunk(tag, payload):
        c = tag + payload
        return struct.pack('>I', len(payload)) + c + struct.pack('>I', zlib.crc32(c))
    png = (b'\x89PNG\r\n\x1a\n'
           + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0))
           + chunk(b'IDAT', zlib.compress(raw))
           + chunk(b'IEND', b''))
    open(sys.argv[2], 'wb').write(png)

main()
