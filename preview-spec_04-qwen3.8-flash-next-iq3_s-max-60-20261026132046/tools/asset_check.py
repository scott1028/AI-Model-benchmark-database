#!/usr/bin/env python3
"""Asset spec gate: compares Blender-side stats, GLB, and Godot-post-import
measurements against specs/<name>.json. Writes pipeline/checks/<name>_check.json.

Usage: asset_check.py <asset> [--skip-godot]
Exit 0 = PASS, 1 = FAIL, 2 = usage/env error.
"""
import json
import math
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(__file__))
import glb_parse

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GODOT = os.environ.get("GODOT_BIN", "godot")


def tol_for(t):
    return max(0.10 * t, 0.02)


def close(a, b, tol):
    return abs(a - b) <= tol


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    name = sys.argv[1]
    skip_godot = "--skip-godot" in sys.argv

    spec_path = os.path.join(ROOT, "specs", f"{name}.json")
    if not os.path.exists(spec_path):
        print(f"missing spec: {spec_path}")
        return 2
    spec = json.load(open(spec_path))

    issues = []
    report = {"asset": name, "spec": spec_path, "stages": {}}

    def fail(stage, check, kind, detail, fix):
        issues.append({"stage": stage, "check": check, "kind": kind,
                       "detail": detail, "fix": fix})

    # --- stage 1: blender live stats ---
    bpath = os.path.join(ROOT, "pipeline", "checks", f"{name}_blender.json")
    if os.path.exists(bpath):
        b = json.load(open(bpath))
        report["stages"]["blender"] = b
        if not b.get("found"):
            fail("blender", "exists", "model", f"collection A_{name} missing in blend",
                 "create/rename collection A_" + name)
        else:
            for i, axis in enumerate("xyz"):
                t = spec["size"][i]
                if not close(b["dims"][i], t, tol_for(t)):
                    fail("blender", f"dim_{axis}", "model",
                         f"blender {axis}={b['dims'][i]} vs target {t} (tol {tol_for(t):.3f})",
                         f"scale {axis} by {t / max(b['dims'][i], 1e-6):.3f} in Blender")
            if b["tris"] > spec["max_tris"]:
                fail("blender", "tris", "model",
                     f"blender tris {b['tris']} > max {spec['max_tris']}",
                     "reduce subdivisions/segments in Blender")
            for a in spec.get("animations", []):
                if a not in b.get("animations", []) and (a + "_" + name) not in b.get("animations", []):
                    fail("blender", f"anim_{a}", "model",
                         f"action '{a}' not found on objects of A_{name}",
                         f"create keyframed action '{a}' in Blender")
    else:
        fail("blender", "stats_run", "pipeline", "no blender stats file",
             f"run tools/blender_stats.py {name}")

    # --- stage 2: GLB file ---
    glb_path = os.path.join(ROOT, "godot", "assets", f"{name}.glb")
    g = None
    if os.path.exists(glb_path):
        g = glb_parse.analyze(glb_path)
        report["stages"]["glb"] = g
        for i, axis in enumerate("xyz"):
            t = spec["size"][i]
            if not close(g["asset_dims_xyz_zup"][i], t, tol_for(t)):
                fail("glb", f"dim_{axis}", "model",
                     f"glb {axis}={g['asset_dims_xyz_zup'][i]} vs target {t}",
                     "fix in Blender, re-export (never edit glb)")
        if b and b.get("found"):
            for i, axis in enumerate("xyz"):
                if not close(g["asset_dims_xyz_zup"][i], b["dims"][i], max(0.01 * b["dims"][i], 0.005)):
                    fail("glb", f"axis_consistency_{axis}", "pipeline",
                         f"blender {axis}={b['dims'][i]} but glb {axis}={g['asset_dims_xyz_zup'][i]}",
                         "export axis/selection problem: export whole A_<name> collection, default +Y up")
        if g["tris"] > spec["max_tris"]:
            fail("glb", "tris", "model", f"glb tris {g['tris']} > max {spec['max_tris']}",
                 "reduce geometry in Blender")
        for a in spec.get("animations", []):
            if a not in g.get("animations", []):
                fail("glb", f"anim_{a}", "pipeline" if a in b.get("animations", []) else "model",
                     f"animation '{a}' missing in glb",
                     "export with export_animations=True" if a in b.get("animations", []) else f"create action '{a}' in Blender")
        if spec.get("collision_glb"):
            if not g.get("col_nodes"):
                fail("glb", "collision_glb", "model",
                     "spec requires col_* convex nodes, none exported",
                     "add col_* convex mesh in A_<name>")
    else:
        fail("glb", "export_run", "pipeline", f"missing {glb_path}",
             f"run tools/export_asset.py {name}")

    # --- stage 3: Godot post-import ---
    if not skip_godot:
        r = subprocess.run(
            [GODOT, "--headless", "--path", os.path.join(ROOT, "godot"),
             "-s", "res://tools/verify_asset.gd", "--", f"res://assets/{name}.glb"],
            capture_output=True, text=True, timeout=120)
        line = next((l for l in r.stdout.splitlines() if l.startswith("VERIFY_JSON ")), None)
        if not line:
            fail("godot", "verify_run", "pipeline",
                 f"verify_asset.gd failed: {r.stdout[-300:]} {r.stderr[-300:]}",
                 "run tools/godot_import.sh; check glb validity")
        else:
            v = json.loads(line[len("VERIFY_JSON "):])
            report["stages"]["godot"] = v
            if "error" in v:
                fail("godot", "load", "pipeline", str(v["error"]), "reimport project")
            else:
                godot_dims = [v["dims"][0], v["dims"][2], v["dims"][1]]  # Y-up -> spec Z-up order
                for i, axis in enumerate("xyz"):
                    t = spec["size"][i]
                    if not close(godot_dims[i], t, tol_for(t)):
                        fail("godot", f"dim_{axis}", "model" if not g or close(godot_dims[i], g["asset_dims_xyz_zup"][i], tol_for(t)) else "pipeline",
                             f"godot-imported {axis}={godot_dims[i]:.3f} vs target {t}",
                             "fix in Blender and re-export")
                if g and not close(v["dims"][1], g["asset_dims_xyz_zup"][2], 0.02):
                    fail("godot", "axis", "pipeline",
                         f"godot height {v['dims'][1]} != glb height {g['asset_dims_xyz_zup'][2]}",
                         "axis conversion broken; keep default +Y-up export")
                if v["bbox_min"][1] > -0.001 and not close(v["bbox_min"][1], 0.0, 0.03):
                    fail("godot", "origin_ground", "model",
                         f"imported zmin(bottom y)={v['bbox_min'][1]:.3f} != 0",
                         "set object origin at mesh bottom in Blender (origin=bottom_center)")
                cx = (v["bbox_min"][0] + v["bbox_max"][0]) / 2
                cz = (v["bbox_min"][2] + v["bbox_max"][2]) / 2
                xy_tol = spec.get("origin_xy_tol", 0.06)
                if not (close(cx, 0, xy_tol) and close(cz, 0, xy_tol)):
                    fail("godot", "origin_center_xy", "model",
                         f"imported center offset ({cx:.3f},{cz:.3f}) > tol {xy_tol}",
                         "center mesh around origin in Blender")
                for a in spec.get("animations", []):
                    if a not in v.get("animations", []):
                        fail("godot", f"anim_{a}", "pipeline",
                             f"'{a}' not in godot animation list {v.get('animations')}",
                             "check action is on exported node; glTF importer should keep AnimationPlayer")

    report["issues"] = issues
    report["pass"] = not issues
    out = os.path.join(ROOT, "pipeline", "checks", f"{name}_check.json")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    json.dump(report, open(out, "w"), indent=2)

    if issues:
        print(f"FAIL {name}:")
        for i in issues:
            print(f"  [{i['stage']}/{i['check']}] ({i['kind']}) {i['detail']}")
            print(f"    fix: {i['fix']}")
        return 1
    print(f"PASS {name}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
