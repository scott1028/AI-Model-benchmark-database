#!/usr/bin/env python3
"""Author + validate godot/data/layout.json (PLAYBOOK Phase 5-6, appendix D).

The layout is the single source of scene composition: WorldBuilder instantiates every
prop from it, tools/blender_terrain.py flattens pads and paints roads from it, and the
validator here proves the composition is playable before anything is built.

Coordinates are Godot space (x, z), y up (AGENTS.md §2). Seeded RNG => reproducible.

Usage: tools/gen_layout.py [--write] [--quiet]
Exit 0 when every validation check passes.
"""
import argparse
import json
import math
import random
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import heightfield as hf  # noqa: E402

SEED = 20261028
ROBOT_RADIUS = 0.31
CAM_VIEW = (-0.7071, -0.7071)  # isometric camera looks toward -X -Z; tall props shade that way

# roads connect: start plaza -> coin_2 -> coin_1 -> coin_0, plus endpoint / building approaches
ROADS = [
    [[10, 13], [2, 10], [-6, 8], [-12, 4], [-14, -3], [-14, -11], [-6, -11], [4, -10], [10.5, -9.5], [11.5, -10.5]],
    [[-6, 8], [-9, 15]],
    [[-2, 2], [0, -2], [2, -6], [4, -10]],
    [[-14, -3], [-15.5, 3.0]],
    [[2, 10], [-7, 10]],
    [[4, -10], [0, -17]],
]

# bot walks the road network: spawn -> coin_0 -> coin_1 -> coin_2 -> endpoint
BOT_ROUTE = [
    [10, 13], [2, 10], [-6, 8], [-9, 15], [-6, 8], [-12, 4], [-14, -3], [-14, -11],
    [-6, -11], [4, -10], [10.5, -9.5], [4, -10], [2, -6], [0, -2],
]

COINS = [
    {"index": 0, "x": -9.0, "z": 15.0, "y_offset": 0.15},
    {"index": 1, "x": -14.0, "z": -11.0, "y_offset": 0.15},
    {"index": 2, "x": 10.5, "z": -9.5, "y_offset": 0.15},
]

# buildings sit on their own pads (appendix C: flat bases, nothing floating or sunk).
# "gate": the road passes through the middle, so it is exempt from the keep-off-road rules.
BUILDINGS = [
    {"id": "house", "n": 1, "x": -12.0, "z": 14.0, "rot_deg": 0.0},
    {"id": "house", "n": 2, "x": -17.5, "z": 6.0, "rot_deg": 90.0},
    {"id": "fantasy_shrine", "n": 1, "x": 14.0, "z": -13.0, "rot_deg": 0.0},
    {"id": "fantasy_arch", "n": 1, "x": 0.0, "z": -17.0, "rot_deg": 0.0, "gate": True, "gap_half_m": 1.35},
]

# crates/barrels are placed by hand: believable spots (loading areas), flat pads, room to test
CRATES = [
    {"x": 7.0, "z": 15.8, "rot_deg": 12},
    {"x": 8.0, "z": 15.8, "rot_deg": -6},
    {"x": 7.5, "z": 15.2, "rot_deg": 24, "y_offset": 0.80},
    {"x": -15.0, "z": 11.0, "rot_deg": 90},
    {"x": -15.0, "z": 11.9, "rot_deg": 96, "y_offset": 0.80},
    {"x": -19.0, "z": 9.0, "rot_deg": 20},
    {"x": 11.0, "z": -11.4, "rot_deg": 45},
    {"x": -1.2, "z": -3.3, "rot_deg": 8},
]
BARRELS = [
    {"x": 12.6, "z": 15.0},
    {"x": 13.0, "z": 10.2},
    {"x": -15.2, "z": 17.0},
    {"x": 10.6, "z": -15.8},
    {"x": 1.4, "z": -1.0},
    {"x": 9.0, "z": -8.4},
]
CLOUDS = [
    # y is lifted well above the 3 m wall ring: from the isometric camera a cloud at ~11 m projects
    # onto the ground and reads like a bush; 16-19 m keeps it clearly in the sky (2026-10-28 截圖)
    {"x": -12.0, "z": -16.0, "y": 17.0, "scale": 1.0},
    {"x": 6.0, "z": 4.0, "y": 18.5, "scale": 0.8},
    {"x": 16.0, "z": 12.0, "y": 16.5, "scale": 0.7},
    {"x": -18.0, "z": 8.0, "y": 17.5, "scale": 0.9},
    {"x": 2.0, "z": -20.0, "y": 19.0, "scale": 1.1},
    {"x": -4.0, "z": 19.0, "y": 17.0, "scale": 0.75},
]

# scatter rules: clearances from roads / pads / bot route, max ground slope, spacing, how many.
# space_r is the *trunk / body* spacing radius (canopies may overlap; bodies may not).
PLACEMENT = {
    "tree": {"count": 22, "road_clear": 2.7, "pad_clear": 1.4, "route_clear": 1.4, "slope": 0.18, "space_r": 1.10, "tall": True},
    "bush": {"count": 34, "road_clear": 0.4, "pad_clear": 0.8, "route_clear": 0.0, "slope": 0.24, "space_r": 0.55},
    "rock": {"count": 16, "road_clear": 2.4, "pad_clear": 1.2, "route_clear": 1.2, "slope": 0.22, "space_r": 0.50},
}
SPAWN = {"x": 10.0, "z": 13.0, "facing_deg": 135}  # face the map centre (toward -X -Z)
ENDPOINT = {"x": 0.0, "z": -2.0}


def build_pads():
    pads = [
        {"id": "plaza_start", "center": [10.0, 13.0], "size": [7.0, 7.0], "kind": "plaza"},
        {"id": "pad_house_a", "center": [-12.0, 14.0], "size": [7.0, 7.0], "kind": "building"},
        {"id": "pad_house_b", "center": [-17.5, 6.0], "size": [7.0, 7.0], "kind": "building"},
        {"id": "pad_shrine", "center": [14.0, -13.0], "size": [8.0, 8.0], "kind": "building"},
        {"id": "pad_arch", "center": [0.0, -17.0], "size": [7.6, 3.4], "kind": "building"},
        {"id": "pad_endpoint", "center": [0.0, -2.0], "size": [3.4, 3.4], "kind": "plaza"},
    ]
    for c in COINS:
        pads.append({"id": "pad_coin_%d" % c["index"], "center": [c["x"], c["z"]], "size": [3.2, 3.2], "kind": "coin"})
    for pid, cx, cz, w, d in (("ring_north", 0, hf.WALL_HALF, 44.0, 3.6), ("ring_south", 0, -hf.WALL_HALF, 44.0, 3.6),
                              ("ring_east", hf.WALL_HALF, 0, 3.6, 44.0), ("ring_west", -hf.WALL_HALF, 0, 3.6, 44.0)):
        pads.append({"id": pid, "center": [cx, cz], "size": [w, d], "kind": "wall"})
    return pads


def spec_dims():
    """Per asset id: visual footprint radius, collision radius, occlusion radius, height."""
    out = {}
    for p in (hf.ROOT / "specs").glob("*.json"):
        s = json.loads(p.read_text(encoding="utf-8"))
        if "dims_m" not in s:
            continue  # not an asset spec (specs/physics_thresholds.json has no dims_m)
        d, col = s["dims_m"], s.get("collision", {})
        visual = max(d["x"], d["y"]) / 2.0
        col_r, occ = visual, visual
        shape = col.get("shape")
        if shape == "box":
            col_r = occ = max(col["size_m"][0], col["size_m"][1]) / 2.0
        elif shape in ("cylinder", "capsule"):
            col_r = occ = col["radius_m"]
        elif shape == "box_pair":
            col_r = occ = max(math.hypot(abs(b["offset_m"][0]) + b["size_m"][0] / 2.0,
                                         abs(b["offset_m"][1]) + b["size_m"][1] / 2.0) for b in col["boxes"])
        out[s["id"]] = {"radius": visual, "col_r": col_r, "occ": occ, "height": d["z"],
                        "mode": col.get("mode", "none")}
    return out


def inside_pad(x, z, pads, margin=0.0):
    for p in pads:
        hw, hd = p["size"][0] / 2.0 + margin, p["size"][1] / 2.0 + margin
        if abs(x - p["center"][0]) <= hw and abs(z - p["center"][1]) <= hd:
            return p
    return None


def slope_at(x, z, layout, step=0.5):
    """Max height change per metre in a small neighbourhood (how flat the ground is)."""
    worst = 0.0
    for dx, dz in ((step, 0), (-step, 0), (0, step), (0, -step)):
        worst = max(worst, abs(hf.terrain_height(x + dx, z + dz, layout) - hf.terrain_height(x - dx, z - dz, layout)) / (2 * step))
    return worst


def pad_flatness(layout):
    """Worst slope inside every pad core (0.5 m inset): this is what makes buildings sit flat."""
    worst = (0.0, None)
    for p in layout["pads"]:
        cx, cz = p["center"]
        w2 = max(0.0, p["size"][0] / 2.0 - 0.5)
        d2 = max(0.0, p["size"][1] / 2.0 - 0.5)
        for i in range(-4, 5):
            for j in range(-4, 5):
                s = slope_at(cx + (i / 4.0) * w2, cz + (j / 4.0) * d2, layout)
                if s > worst[0]:
                    worst = (s, p["id"])
    return worst


def view_shadow_hits_route(prop, route):
    """Is the prop between the camera and the route? Project route points onto the view axis."""
    hx = max(prop["occ"], 0.5) + 0.4
    reach = prop["height"] * 1.25
    px, pz = prop["x"], prop["z"]
    for seg in range(len(route) - 1):
        for t in [i / 12.0 for i in range(13)]:
            dx = route[seg][0] + (route[seg + 1][0] - route[seg][0]) * t - px
            dz = route[seg][1] + (route[seg + 1][1] - route[seg][1]) * t - pz
            along = dx * CAM_VIEW[0] + dz * CAM_VIEW[1]
            if along < 0.0 or along > reach:
                continue
            if abs(dx * -CAM_VIEW[1] + dz * CAM_VIEW[0]) <= hx:
                return ["%s at route (%.1f, %.1f)" % (prop["label"], px + dx, pz + dz)]
    return []


def place_seeded(layout, dims, rng):
    """Scatter trees/bushes/rocks, rejecting spots that break any placement rule."""
    hand = list(layout["props"])
    seeded = []
    for pid, rule in PLACEMENT.items():
        col_r, space_r = dims[pid]["col_r"], rule["space_r"]
        placed = tries = 0
        while placed < rule["count"] and tries < rule["count"] * 200:
            tries += 1
            x = rng.uniform(-hf.PLAY_HALF + 1.4, hf.PLAY_HALF - 1.4)
            z = rng.uniform(-hf.PLAY_HALF + 1.4, hf.PLAY_HALF - 1.4)
            if hf.road_dist(x, z, layout["roads"]) < rule["road_clear"] + col_r:
                continue
            if inside_pad(x, z, layout["pads"], rule["pad_clear"]):
                continue
            if hf.road_dist(x, z, [layout["routes"]["bot"]]) < rule["route_clear"] + col_r:
                continue
            if slope_at(x, z, layout) > rule["slope"]:
                continue
            cand = {"id": pid, "asset": pid, "n": placed + 1, "x": round(x, 2), "z": round(z, 2),
                    "rot_deg": round(rng.uniform(0, 360), 1),
                    "label": "%s%d" % (pid, placed + 1), "occ": dims[pid]["occ"], "height": dims[pid]["height"]}
            if rule.get("tall") and view_shadow_hits_route(cand, layout["routes"]["bot"]):
                continue
            if any(math.hypot(x - q["x"], z - q["z"]) < space_r + PLACEMENT_SPACE[q["id"]] for q in hand + seeded):
                continue
            seeded.append(cand)
            placed += 1
    return seeded


PLACEMENT_SPACE = {"tree": 1.10, "bush": 0.55, "rock": 0.50, "house": 2.4, "fantasy_shrine": 2.6,
                   "fantasy_arch": 2.7, "wooden_crate": 0.42, "barrel": 0.38, "endpoint_flag": 0.9, "coin": 0.5}


def validate(layout, dims):
    res = []
    route = layout["routes"]["bot"]
    props = layout["props"]
    blockers = [p for p in props if dims[p["id"]]["mode"] != "none" and not p.get("gate")]

    s, pid = pad_flatness(layout)
    res.append({"name": "pads_flat", "target": "<= %.2f m/m" % hf.MAX_PAD_SLOPE, "measured": "%.3f (%s)" % (s, pid),
                "ok": s <= hf.MAX_PAD_SLOPE + 1e-6})

    outside = [p for p in route if max(abs(p[0]), abs(p[1])) > hf.PLAY_HALF]
    res.append({"name": "route_inside_walls", "target": "<= %.1f m" % hf.PLAY_HALF,
                "measured": "ok" if not outside else str(outside), "ok": not outside})

    off_road = []
    for i in range(len(route) - 1):
        mx = (route[i][0] + route[i + 1][0]) / 2.0
        mz = (route[i][1] + route[i + 1][1]) / 2.0
        d = hf.road_dist(mx, mz, layout["roads"])
        if d > hf.ROAD_HALF_WIDTH + 0.6:
            off_road.append((i, round(mx, 1), round(mz, 1), round(d, 2)))
    res.append({"name": "route_follows_roads", "target": "segment midpoints on a road",
                "measured": "ok" if not off_road else str(off_road[:4]), "ok": not off_road})

    blocked = ["%s%d d=%.2f need %.2f" % (p["id"], p["n"], hf.road_dist(p["x"], p["z"], [route]), p["col_r"] + ROBOT_RADIUS + 0.25)
               for p in blockers
               if hf.road_dist(p["x"], p["z"], [route]) < p["col_r"] + ROBOT_RADIUS + 0.25]
    res.append({"name": "route_clear_of_blockers", "target": ">= collision radius + 0.56 m",
                "measured": "ok" if not blocked else "; ".join(blocked[:5]), "ok": not blocked})

    steep = ["%s%d slope %.3f > %.2f" % (p["id"], p["n"], slope_at(p["x"], p["z"], layout),
                                         PLACEMENT.get(p["id"], {"slope": 0.06})["slope"])
             for p in props
             if slope_at(p["x"], p["z"], layout) > PLACEMENT.get(p["id"], {"slope": 0.06})["slope"] + 1e-6]
    res.append({"name": "props_on_flat_enough_ground", "target": "per-type slope limit (buildings/props on pads 0.06)",
                "measured": "ok" if not steep else "; ".join(steep[:5]), "ok": not steep})

    on_road = ["%s%d d=%.2f" % (p["id"], p["n"], hf.road_dist(p["x"], p["z"], layout["roads"]))
               for p in blockers if hf.road_dist(p["x"], p["z"], layout["roads"]) < p["col_r"] + 0.35]
    res.append({"name": "blockers_off_roads", "target": ">= collision radius + 0.35 m (gates exempt)",
                "measured": "ok" if not on_road else "; ".join(on_road[:5]), "ok": not on_road})

    overlap = ["%s%d/%s%d" % (a["id"], a["n"], b["id"], b["n"])
               for i, a in enumerate(props) for b in props[i + 1:]
               if not (a.get("y_offset") or b.get("y_offset"))
               and math.hypot(a["x"] - b["x"], a["z"] - b["z"]) < PLACEMENT_SPACE[a["id"]] + PLACEMENT_SPACE[b["id"]] - 0.05]
    res.append({"name": "no_prop_overlap", "target": "bodies clear (canopies may overlap)",
                "measured": "ok" if not overlap else "; ".join(overlap[:5]), "ok": not overlap})

    shade = []
    for p in props:
        if dims[p["id"]]["height"] >= 3.0:
            shade += view_shadow_hits_route(dict(p, label="%s%d" % (p["id"], p["n"]), occ=dims[p["id"]]["occ"]), route)
    res.append({"name": "tall_props_not_shading_route", "target": "no route point behind a tall silhouette",
                "measured": "ok" if not shade else "; ".join(shade[:3]), "ok": not shade})

    landmarks = [dict(layout["spawn"], label="spawn"), dict(layout["endpoint"], label="endpoint", n=0),
                 *[dict(c, label="coin_%d" % c["index"]) for c in layout["coins"]]]
    bad = [lm["label"] for lm in landmarks if not inside_pad(lm["x"], lm["z"], layout["pads"])]
    res.append({"name": "landmarks_on_pads", "target": "spawn/coins/endpoint on a pad",
                "measured": "ok" if not bad else ",".join(bad), "ok": not bad})

    near = ["%s inside %s%d" % (lm["label"], p["id"], p["n"]) for lm in landmarks for p in blockers
            if math.hypot(lm["x"] - p["x"], lm["z"] - p["z"]) < p["col_r"] + 0.6]
    res.append({"name": "landmarks_reachable", "target": "no blocker within 0.6 m of spawn/coins/flag",
                "measured": "ok" if not near else "; ".join(near[:4]), "ok": not near})

    out_of_bounds = ["%s%d at %.1f/%.1f" % (p["id"], p["n"], p["x"], p["z"]) for p in props
                     if max(abs(p["x"]), abs(p["z"])) > hf.PLAY_HALF - 0.5]
    res.append({"name": "props_inside_walls", "target": "<= %.1f m" % (hf.PLAY_HALF - 0.5),
                "measured": "ok" if not out_of_bounds else "; ".join(out_of_bounds[:4]), "ok": not out_of_bounds})

    gaps = []
    for p in props:
        if not p.get("gate"):
            continue
        d = hf.road_dist(p["x"], p["z"], layout["roads"])
        if d > p["gap_half_m"]:
            gaps.append("%s road %.2f m off centre (gap +/-%.2f)" % (p["id"], d, p["gap_half_m"]))
    res.append({"name": "gate_road_passes_gap", "target": "road through the arch opening",
                "measured": "ok" if not gaps else "; ".join(gaps), "ok": not gaps})

    tight = ["barrel%d clear %.2f" % (p["n"], min(math.hypot(p["x"] - q["x"], p["z"] - q["z"]) - q["col_r"]
                                                  for q in blockers if q is not p))
             for p in props if dims[p["id"]]["mode"] == "rigid"
             and min(math.hypot(p["x"] - q["x"], p["z"] - q["z"]) - q["col_r"] for q in blockers if q is not p) < 1.5]
    res.append({"name": "pushables_have_room", "target": ">= 1.5 m clear around every barrel",
                "measured": "ok" if not tight else "; ".join(tight[:4]), "ok": not tight})

    counts = {}
    for p in props:
        counts[p["id"]] = counts.get(p["id"], 0) + 1
    res.append({"name": "prop_counts", "target": str({k: v["count"] for k, v in PLACEMENT.items()}),
                "measured": str({k: v for k, v in counts.items() if k in PLACEMENT}),
                "ok": all(counts.get(k, 0) == v["count"] for k, v in PLACEMENT.items())})
    return res


def build_layout():
    rng = random.Random(SEED)
    dims = spec_dims()
    layout = {
        "version": 1,
        "seed": SEED,
        "units": "metres, Godot axes (x right, z toward camera, y up)",
        "world": {"play_half": hf.PLAY_HALF, "wall_half": hf.WALL_HALF, "terrain_half": hf.TERRAIN_HALF},
        "heightmap": "res://data/heightmap.json",
        "player": {"asset": "robot", "speed_mps": 4.0, "spawn": SPAWN},
        "coins": [dict(c, asset="coin") for c in COINS],
        "endpoint": dict(ENDPOINT, asset="endpoint_flag"),
        "spawn": SPAWN,
        "pads": build_pads(),
        "roads": ROADS,
        "routes": {"bot": BOT_ROUTE},
        "fx": {
            "sway_assets": ["tree", "bush"],
            "sensor_assets": ["bush"],
            "wind": {
                "tree": {"strength_m": 0.07, "ref_height_m": 3.0},
                "bush": {"strength_m": 0.05, "ref_height_m": 0.9},
            },
            "coin_spin": True,
            "cloud_drift": True,
            "leaf_fall_mps": 1.2,
        },
        "walls": [],
        "clouds": [dict(c, asset="cloud", drift_mps=0.35) for c in CLOUDS],
        "props": [dict(b, asset=b["id"]) for b in BUILDINGS],
    }
    for i, c in enumerate(CRATES):
        layout["props"].append(dict(c, id="wooden_crate", n=i + 1, asset="wooden_crate"))
    for i, b in enumerate(BARRELS):
        layout["props"].append(dict(b, id="barrel", n=i + 1, asset="barrel"))
    layout["props"] += place_seeded(layout, dims, rng)

    # wall ring: 11 segments of 3.9 m spacing per side, centre line +/- wall_half
    for i in range(11):
        t = -19.5 + i * 3.9
        for side in (-1, 1):
            layout["walls"].append({"asset": "brick_wall", "x": t, "z": side * hf.WALL_HALF, "rot_deg": 0.0})
            layout["walls"].append({"asset": "brick_wall", "x": side * hf.WALL_HALF, "z": t, "rot_deg": 90.0})

    # attach spec metrics so validation (and WorldBuilder) can judge each entry
    for p in layout["props"] + layout["walls"]:
        key = p["asset"]
        p["radius"], p["col_r"], p["height"] = round(dims[key]["radius"], 3), round(dims[key]["col_r"], 3), dims[key]["height"]
        p["mode"] = dims[key]["mode"]
    # embed the per-asset collision/dims block so Godot needs only layout.json at runtime
    layout["assets"] = {}
    for p in (hf.ROOT / "specs").glob("*.json"):
        s = json.loads(p.read_text(encoding="utf-8"))
        if "dims_m" not in s or "collision" not in s:
            continue  # not an asset spec (specs/physics_thresholds.json)
        layout["assets"][s["id"]] = {"collision": s["collision"], "dims_m": s["dims_m"],
                                     "animations": s.get("animations", [])}
    return layout, dims


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    layout, dims = build_layout()
    results = validate(layout, dims)
    failures = [r for r in results if not r["ok"]]

    if args.write:
        hf.LAYOUT_PATH.parent.mkdir(parents=True, exist_ok=True)
        hf.LAYOUT_PATH.write_text(json.dumps(layout, indent=1, ensure_ascii=False) + "\n", encoding="utf-8")

    counts = {}
    for p in layout["props"]:
        counts[p["id"]] = counts.get(p["id"], 0) + 1
    if not args.quiet:
        for r in results:
            print("  [%s] %-32s %s" % ("v" if r["ok"] else "x", r["name"], r["measured"]))
        print("LAYOUT %s props=%s walls=%d clouds=%d pads=%d -> %s"
              % ("PASS" if not failures else "FAIL", json.dumps(counts), len(layout["walls"]),
                 len(layout["clouds"]), len(layout["pads"]), hf.LAYOUT_PATH.relative_to(hf.ROOT)))
    return 0 if not failures else 1


if __name__ == "__main__":
    sys.exit(main())
