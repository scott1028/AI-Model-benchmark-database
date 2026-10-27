#!/usr/bin/env python3
"""Bake the diorama top-surface colour map: godot/data/terrain_surface.png.

Why: the Blender mesh paints every 1 m cell with one flat material (per-face), so in-game the
lawn reads as a hard 1 m checkerboard (2026-10-28 screenshots, 附錄 B "草坪不單調、不假").
This tool writes the same palette onto a 1 m lattice and upscales it with bilinear filtering;
godot/shaders/terrain_surface.gdshader samples it linearly, so the cells blend smoothly while
roads and plaza edges stay where the layout put them.

Rule source: heightfield.surface_material + specs/terrain.json build.colors (single source,
same random seed as the mesh build), so mesh and texture never disagree.

Run: python3 tools/gen_terrain_texture.py [--px-per-m 4]
"""
import argparse
import json
import random
import sys
from pathlib import Path

import numpy as np
from PIL import Image

sys.path.insert(0, str(Path(__file__).resolve().parent))
import heightfield as hf  # noqa: E402

SPEC_PATH = hf.ROOT / "specs" / "terrain.json"
OUT_PATH = hf.ROOT / "godot" / "data" / "terrain_surface.png"

# material name (spec) -> build.colors key
COLOR_KEYS = {
    "terrain_grass_light": "grass_light",
    "terrain_grass_mid": "grass_mid",
    "terrain_grass_dark": "grass_dark",
    "terrain_road": "road",
    "terrain_plaza": "plaza",
}


def srgb_to_linear(hex_str):
    h = hex_str.lstrip("#")
    r, g, b = (int(h[i:i + 2], 16) / 255.0 for i in (0, 2, 4))
    f = lambda c: c / 12.92 if c <= 0.04045 else ((c + 0.055) / 1.055) ** 2.4
    return np.array([f(r), f(g), f(b)], dtype=np.float64)


def value_noise(n, scale, seed):
    """Smooth 0..1 noise on an n x n lattice, feature size ~scale metres (bilinear upscaled)."""
    rng = np.random.default_rng(seed)
    m = max(2, int(round(n / scale)))
    small = rng.random((m, m))
    img = Image.fromarray((small * 255).astype(np.uint8)).resize((n, n), Image.BILINEAR)
    return np.asarray(img, dtype=np.float64) / 255.0


def build_palette(layout, colors, n, rnd):
    """(n, n, 3) colours + (n, n) kind codes (0 grass, 1 road, 2 plaza)."""
    grid = np.zeros((n, n, 3), dtype=np.float64)
    kind = np.zeros((n, n), dtype=np.uint8)
    used = {}
    for j in range(n):
        for i in range(n):
            name = hf.surface_material(-hf.TERRAIN_HALF + i + 0.5, -hf.TERRAIN_HALF + j + 0.5, layout, rnd)
            used[name] = used.get(name, 0) + 1
            grid[j, i] = colors[name]
            kind[j, i] = 2 if name == "terrain_plaza" else (1 if name == "terrain_road" else 0)
    return grid, kind, used


def paving_joints(px_per_m, n_px, width):
    """Darken a thin border inside each 1 m paving cell, so plazas/roads read as laid stones.

    Kept low contrast: thin dark lines alias into speckle at the game camera's distance
    (2026-10-28 road screenshot read as puddles), so joints + mipmaps only.
    """
    px = np.arange(n_px)
    local = (px % px_per_m) / px_per_m
    edge = np.minimum(local, 1.0 - local)
    line = 1.0 - 0.11 * (edge < width)
    return np.outer(line, line)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--px-per-m", type=int, default=8, help="texture resolution (8 px per 1 m cell)")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    layout = hf.load_layout()
    spec = json.loads(SPEC_PATH.read_text(encoding="utf-8"))
    colors = {m: srgb_to_linear(spec["build"]["colors"][k]) for m, k in COLOR_KEYS.items()}

    half = spec["build"]["half_m"]
    step = spec["build"]["step_m"]
    n = int(round(2 * half / step))
    cells, kind, used = build_palette(layout, colors, n, random.Random(11))

    # 1 m lattice -> px_per_m pixels per cell, bilinear: cell edges become 1 m gradients
    img = Image.fromarray((np.clip(cells, 0.0, 1.0) * 255).astype(np.uint8))
    n_px = n * args.px_per_m
    up = np.asarray(img.resize((n_px, n_px), Image.BILINEAR), dtype=np.float64) / 255.0

    # grain: breaks the flat plastic look without inventing new palette entries.
    # Features stay >= 2 m: finer grain aliases into moiré at the game's texel/screen ratio.
    broad = value_noise(n_px, 6.0, 20261028)[:, :, None]
    fine = value_noise(n_px, 2.5, 777)[:, :, None]
    up = up * (1.0 + 0.055 * (broad - 0.5) + 0.022 * (fine - 0.5))

    # paving joints on road / plaza cells only (grass keeps its soft look)
    cell_of_px = np.arange(n_px) // args.px_per_m
    paved = kind[np.ix_(cell_of_px, cell_of_px)] > 0
    joints = paving_joints(args.px_per_m, n_px, 0.12)
    up = up * np.where(paved, joints, 1.0)[:, :, None]

    # Godot samples textures with v = 0 at the bottom row, so the row built for z = -22 must be the
    # LAST row of the PNG: without this flip the shader's v = (z + half) / size reads the map
    # mirrored in z, and road / plaza cells land on grass as stray patches (2026-10-28 截圖).
    up = np.flipud(up)

    OUT_PATH.parent.mkdir(parents=True, exist_ok=True)
    Image.fromarray((np.clip(up, 0.0, 1.0) * 255).astype(np.uint8)).save(OUT_PATH)

    if not args.quiet:
        print("TERRAIN_TEX %s %dx%d px (%d px/m) cells=%d palette=%s"
              % (OUT_PATH.relative_to(hf.ROOT), n_px, n_px, args.px_per_m, n,
                 json.dumps({k: v for k, v in sorted(used.items())})))
    return 0


if __name__ == "__main__":
    sys.exit(main())
