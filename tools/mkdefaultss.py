#!/usr/bin/env python3
"""
mkdefaultss.py - build DefaultSS444.c / DefaultSS555.c, the built-in image the
pico_shared menu screensaver bounces around when no artwork is on the SD card.

The source is a frame dumped by the host harness (hosttest/phx_host) in the
default, rotated orientation: a 320x240 PPM with the 208-pixel-wide game area
centred. The game area is box-filtered down to 160 rows.

    hosttest/phx_host ~/roms/arcade/PHOENIX 601 600 /tmp/ss
    tools/mkdefaultss.py /tmp/ss/frame_00600.ppm

Output format (both files): uint16 width, uint16 height (little-endian), then
width*height little-endian pixels, 0000RRRRGGGGBBBB (.444) or 0RRRRRGGGGGBBBBB
(.555), as an unsigned char array.
"""

import os
import sys

GAME_X, GAME_W, GAME_H = 56, 208, 240
OUT_H = 160


def read_ppm(path):
    with open(path, "rb") as f:
        data = f.read()
    parts = data.split(b"\n", 3)
    w, h = map(int, parts[1].split())
    return w, h, parts[3]


def scale(w, h, px):
    out_w = round(GAME_W * OUT_H / GAME_H)
    out = []
    for oy in range(OUT_H):
        y0, y1 = oy * GAME_H / OUT_H, (oy + 1) * GAME_H / OUT_H
        row = []
        for ox in range(out_w):
            x0, x1 = ox * GAME_W / out_w, (ox + 1) * GAME_W / out_w
            acc = [0.0, 0.0, 0.0]
            area = 0.0
            for sy in range(int(y0), min(int(y1 + 0.9999), GAME_H)):
                wy = min(sy + 1, y1) - max(sy, y0)
                for sx in range(int(x0), min(int(x1 + 0.9999), GAME_W)):
                    wx = min(sx + 1, x1) - max(sx, x0)
                    i = (sy * w + GAME_X + sx) * 3
                    for c in range(3):
                        acc[c] += px[i + c] * wx * wy
                    area += wx * wy
            row.append(tuple(min(255, int(v / area + 0.5)) for v in acc))
        out.append(row)
    return out_w, out


def write_c(path, name, w, h, rows, pack):
    data = [w & 0xFF, w >> 8, h & 0xFF, h >> 8]
    for row in rows:
        for r, g, b in row:
            v = pack(r, g, b)
            data += [v & 0xFF, v >> 8]
    with open(path, "w") as f:
        f.write("const unsigned char %s[] = {\n" % name)
        for i in range(0, len(data), 12):
            f.write("  " + ", ".join("0x%02x" % b for b in data[i:i + 12]) + ",\n")
        f.write("};\n")
        f.write("const unsigned int %s_len = %d;\n" % (name, len(data)))


def main():
    if len(sys.argv) != 2:
        print(__doc__)
        return 1
    w, h, px = read_ppm(sys.argv[1])
    if (w, h) != (320, 240):
        print("expected a 320x240 frame from phx_host")
        return 1
    out_w, rows = scale(w, h, px)
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    write_c(os.path.join(root, "DefaultSS444.c"), "DefaultSS160_444", out_w, OUT_H, rows,
            lambda r, g, b: ((r >> 4) << 8) | ((g >> 4) << 4) | (b >> 4))
    write_c(os.path.join(root, "DefaultSS555.c"), "DefaultSS160_555", out_w, OUT_H, rows,
            lambda r, g, b: ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3))
    print("wrote DefaultSS444.c and DefaultSS555.c (%dx%d)" % (out_w, OUT_H))
    return 0


if __name__ == "__main__":
    sys.exit(main())
