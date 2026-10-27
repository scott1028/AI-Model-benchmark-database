#!/usr/bin/env python3
"""Asset spec check (AGENTS.md §5): Blender side + Godot imported side vs specs/<id>.json.

Usage: tools/check_asset.py --id <id> [--attempt N] [--skip-import]
Writes pipeline/checks/<id>.blender.json, <id>.godot.json, <id>.json. Exit 0 on PASS.
"""
import argparse
import json
import subprocess
import sys
from datetime import datetime
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GODOT = "/home/scott/tools/bin/godot"
BLENDER = "/home/scott/tools/blender/blender"
CHECKS = ROOT / "pipeline" / "checks"


def sh(cmd, timeout=240):
    p = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
    return p.returncode, p.stdout, p.stderr


def read_json(path):
    p = Path(path)
    if not p.exists():
        return None
    try:
        return json.loads(p.read_text(encoding="utf-8"))
    except json.JSONDecodeError as exc:
        return {"_parse_error": str(exc), "_raw": p.read_text(encoding="utf-8")[:2000]}


def hex_to_rgb(h):
    h = (h or "").lstrip("#")
    if len(h) != 6:
        return None
    return tuple(int(h[i:i + 2], 16) for i in (0, 2, 4))


def color_close(a, b, tol=12):
    ra, rb = hex_to_rgb(a), hex_to_rgb(b)
    if ra is None or rb is None:
        return True  # name check covers this; colour not comparable
    return all(abs(x - y) <= tol for x, y in zip(ra, rb))


def measure_axis(target, measured, tol_pct, tol_min_m):
    tol = max(abs(target) * tol_pct / 100.0, tol_min_m)
    delta = measured - target
    return {
        "target": round(target, 4),
        "measured": round(measured, 4),
        "delta": round(delta, 4),
        "delta_pct": round(100.0 * delta / target, 2) if target else None,
        "tol_m": round(tol, 4),
        "ok": abs(delta) <= tol,
    }


def check_dims(spec, dims, side, out):
    t = spec["dims_m"]
    tol_pct = spec.get("tolerance_pct", 10)
    tol_min = spec.get("tolerance_min_cm", 2) / 100.0
    for axis in ("x", "y", "z"):
        m = measure_axis(t[axis], dims[axis], tol_pct, tol_min)
        m.update({"name": "dim_%s" % axis, "side": side, "ok": m.pop("ok")})
        out.append(m)


def run_blender_audit(asset_id, attempt_tag):
    blend = ROOT / "blender" / ("%s.blend" % asset_id)
    out = CHECKS / ("%s.blender.json" % asset_id)
    if not blend.exists():
        return None, "blend file missing: %s" % blend
    code, so, se = sh([BLENDER, "--background", str(blend), "--python", str(ROOT / "tools" / "blender_audit.py"),
                       "--", "--id", asset_id, "--json-out", str(out)])
    if code != 0:
        return None, "blender audit failed: %s" % (se or so)[-800:]
    data = read_json(out)
    if data is None:
        return None, "blender audit produced no json"
    return data, None


def run_godot_import():
    code, so, se = sh([GODOT, "--headless", "--path", str(ROOT / "godot"), "--import"], timeout=600)
    if code != 0:
        return "godot import failed: %s" % (se or so)[-800:]
    return None


def run_godot_audit(asset_id):
    out = CHECKS / ("%s.godot.json" % asset_id)
    code, so, se = sh([GODOT, "--headless", "--path", str(ROOT / "godot"), "--script", "res://tools/asset_audit.gd",
                       "--", "--id", asset_id, "--json-out", str(out)])
    data = read_json(out)
    if data is None:
        line = next((ln for ln in (so + se).splitlines() if ln.startswith("ASSET_AUDIT")), None)
        if line:
            data = json.loads(line[len("ASSET_AUDIT "):])
    if data is None:
        return None, "godot audit produced no json: %s" % (se or so)[-800:]
    return data, None


def evaluate(spec, bl, gd, bl_err=None, gd_err=None):
    res = []
    tol_min = spec.get("tolerance_min_cm", 2) / 100.0
    sizes = spec["dims_m"]
    footprint_tol = max(tol_min, 0.05 * max(sizes["x"], sizes["y"]))

    if bl is None:
        res.append({"name": "blender_audit", "side": "blender", "target": "measurement",
                    "measured": (bl_err or "no data")[-300:], "ok": False})
    if gd is None:
        res.append({"name": "godot_audit", "side": "godot", "target": "measurement",
                    "measured": (gd_err or "no data")[-300:], "ok": False})

    for side, data, dims_key, min_key in (
        ("blender", bl, "dims_blender", "min_z"),
        ("godot", gd, "dims_blender_equivalent", "min_y"),
    ):
        if data is None:
            continue
        check_dims(spec, data[dims_key], side, res)
        origin = data["origin"]
        m = measure_axis(0.0, origin[min_key], 0, tol_min)
        m.update({"name": "origin_bottom", "side": side, "ok": m.pop("ok")})
        res.append(m)
        off = origin["xy_center_offset"]
        mag = max(abs(off[0]), abs(off[1]))
        res.append({"name": "origin_xy_center", "side": side, "target": 0.0, "measured": round(mag, 4),
                    "tol_m": round(footprint_tol, 4), "ok": mag <= footprint_tol})
        tris = data.get("tris_visual", data.get("tris"))
        res.append({"name": "tris", "side": side, "target": spec["tris_max"], "measured": tris,
                    "ok": tris is not None and tris <= spec["tris_max"]})
        names = {m["name"] for m in (data.get("materials") or [])}
        for want in spec.get("materials", []):
            res.append({"name": "material:" + want["name"], "side": side, "target": want["name"],
                        "measured": ",".join(sorted(names)) if want["name"] not in names else "ok",
                        "ok": want["name"] in names})
            if want["name"] in names:
                got = next((m["color"] for m in data["materials"] if m["name"] == want["name"]), None)
                res.append({"name": "material_color:" + want["name"], "side": side, "target": want.get("color"),
                            "measured": got, "ok": color_close(want.get("color"), got)})

    # collision (AGENTS.md §2): shape "mesh" → collider mesh in Blender/Godot;
    # primitive shapes (box/cylinder/capsule/box_pair) are declared in the spec and built
    # at runtime by WorldBuilder, so the spec must carry the parameters.
    col_spec = spec.get("collision", {})
    mode = col_spec.get("mode", "none")
    shape = col_spec.get("shape", "")
    if mode == "none":
        res.append({"name": "collision", "side": "spec", "target": "none", "measured": "no collider required", "ok": True})
    elif shape == "mesh":
        if bl is not None:
            col = bl.get("collision", {})
            res.append({"name": "collision_mesh", "side": "blender", "target": mode,
                        "measured": json.dumps(col, ensure_ascii=False), "ok": bool(col.get("present") and col.get("flag_ok"))})
        if gd is not None:
            res.append({"name": "collision_mesh", "side": "godot", "target": mode,
                        "measured": json.dumps({"nodes": gd.get("collision_nodes"), "meta_ok": gd.get("collision_meta_ok")},
                                               ensure_ascii=False),
                        "ok": bool(gd.get("collision_nodes")) and bool(gd.get("collision_meta_ok"))})
    elif shape == "heightfield":
        # terrain: the walkable surface is a height field; Godot builds HeightMapShape3D from the JSON
        need = ["size_m", "step_m", "heightmap"]
        missing = [k for k in need if k not in col_spec]
        res.append({"name": "collision_params", "side": "spec", "target": shape,
                    "measured": ("missing " + ",".join(missing)) if missing else "declared",
                    "ok": not missing})
        hm, hm_err = None, None
        if not missing:
            path = ROOT / "godot" / col_spec["heightmap"].replace("res://", "")
            if not path.exists():
                hm_err = "heightmap file missing: %s" % path
            else:
                try:
                    hm = json.loads(path.read_text(encoding="utf-8"))
                except json.JSONDecodeError as exc:
                    hm_err = "heightmap unreadable: %s" % exc
        if hm is None:
            res.append({"name": "collision_heightfield", "side": "godot", "target": col_spec.get("heightmap", "?"),
                        "measured": hm_err or "no data", "ok": False})
        else:
            want_n = int(round(col_spec["size_m"][0] / col_spec["step_m"])) + 1
            n = hm.get("size")
            rows = hm.get("rows") or []
            flat = [v for row in rows for v in row]
            ok_grid = (n == want_n and len(rows) == n and all(len(r) == n for r in rows))
            ok_vals = bool(flat) and all(isinstance(v, (int, float)) and v == v and abs(v) < 1000 for v in flat)
            res.append({"name": "collision_heightfield", "side": "godot", "target": "%dx%d grid, step %.2f m" % (want_n, want_n, col_spec["step_m"]),
                        "measured": "%dx%d, %d heights, node_y=%s, range %.2f..%.2f" % (
                            n, len(rows), len(flat), hm.get("node_y"),
                            min(flat) if flat else 0, max(flat) if flat else 0),
                        "ok": ok_grid and ok_vals and hm.get("node_y") is not None})
    else:
        need = {"box": ["size_m"], "cylinder": ["radius_m", "height_m"],
                "capsule": ["radius_m", "height_m"], "box_pair": ["boxes"]}.get(shape, [])
        missing = [k for k in need if k not in col_spec]
        res.append({"name": "collision_params", "side": "spec", "target": shape or mode,
                    "measured": ("missing " + ",".join(missing)) if missing else "declared (body built by WorldBuilder)",
                    "ok": not missing and bool(shape)})

    # animations
    want_anims = [a["name"] for a in spec.get("animations", [])]
    if bl is not None:
        have = {a["name"] for a in bl.get("animations", [])}
        for a in want_anims:
            res.append({"name": "anim(blender):" + a, "side": "blender", "target": a,
                        "measured": ",".join(sorted(have)), "ok": a in have})
    if gd is not None and want_anims:
        have = {str(a["name"]).lstrip(".") for a in gd.get("animations", [])}
        for a in want_anims:
            res.append({"name": "anim(godot):" + a, "side": "godot", "target": a,
                        "measured": ",".join(sorted(have)), "ok": a in have})

    failures = [r for r in res if not r.get("ok")]
    return res, failures


def classify(failures, bl, gd):
    if not failures:
        return "none", ""
    names = {f["name"] for f in failures}
    sides = {f["side"] for f in failures}
    if bl is None or gd is None:
        return "pipeline", "檢查工具拿不到測量結果（檔缺失或匯入失敗），先修 pipeline，不改模型"
    if "blender" in sides:
        return "modeling", "Blender 端測量已不符規格 → 回 Blender 依下列項目修模型"
    if sides == {"godot"}:
        return "pipeline", "Blender 端合格、Godot 端不合格 → 檢查匯出旗標/路徑/匯入器設定"
    return "modeling", "兩端都有偏差，以 Blender 端為主修模型"


def suggestions(failures, spec):
    out = []
    for f in failures:
        n = f["name"]
        if n in ("blender_audit", "godot_audit"):
            out.append("%s 量測失敗：%s → 先修 pipeline（Blender/Godot 匯入或審計腳本），不要改模型"
                       % (n, f["measured"]))
        elif n.startswith("dim_"):
            axis = n.split("_")[1]
            out.append("%s 尺寸 %.3fm，目標 %.3fm（容差 %.3fm）→ 在 Blender 以 origin 為基準 scale 補正 %.2f 倍"
                       % (axis, f["measured"], f["target"], f["tol_m"], (f["target"] / f["measured"]) if f["measured"] else 0))
        elif n == "origin_bottom":
            out.append("接地面不在原點（measured %.3f）→ 把網格沿 Z 平移使 min_z=0" % f["measured"])
        elif n == "origin_xy_center":
            out.append("原點未對齊足跡中心（偏移 %.3f）→ 平移網格使 XY 置中" % f["measured"])
        elif n == "tris":
            out.append("面數 %s > 上限 %s → 用 decimate / 手動簡化視覺網格（不動 `_collision`）" % (f["measured"], f["target"]))
        elif n.startswith("material:"):
            out.append("缺材料 %s → 在 Blender 建立同名材料並指定到物件" % n.split(":", 1)[1])
        elif n.startswith("material_color:"):
            out.append("材料 %s 顏色 %s != 規格 %s → 改 Principled Base Color"
                       % (n.split(":", 1)[1], f["measured"], f["target"]))
        elif n == "collision_params":
            out.append("碰撞參數不完整（shape=%s，%s）→ 在 specs/<id>.json 補 size_m / radius_m+height_m / boxes"
                       % (f["target"], f["measured"]))
        elif n == "collision_heightfield":
            out.append("高度場碰撞資料不可用（%s）→ 重跑 tools/blender_terrain.py 產生 godot/data/heightmap.json（size/step 要符合規格）"
                       % f["measured"])
        elif n == "collision_mesh":
            out.append("碰撞不符合 mode=%s → 建立/修正 <id>_collision（低面數、自訂屬性 crcollider=1、匯出保留 extras）"
                       % spec.get("collision", {}).get("mode"))
        elif n.startswith("anim("):
            out.append("動畫 %s 缺失 → Blender 建立該 action 並確認 export_animations=True" % n.split(":", 1)[1])
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--id", required=True)
    ap.add_argument("--attempt", type=int, default=1)
    ap.add_argument("--skip-import", action="store_true")
    args = ap.parse_args()

    CHECKS.mkdir(parents=True, exist_ok=True)
    spec_path = ROOT / "specs" / ("%s.json" % args.id)
    if not spec_path.exists():
        print("FAIL: no spec at %s (先寫規格再建模)" % spec_path)
        return 1
    spec = json.loads(spec_path.read_text(encoding="utf-8"))

    bl, bl_err = run_blender_audit(args.id, args.attempt)
    if bl_err:
        print("FAIL(blender audit): " + bl_err)
    if not args.skip_import:
        err = run_godot_import()
        if err:
            print("FAIL(godot import): " + err)
    gd, gd_err = run_godot_audit(args.id)
    if gd_err:
        print("FAIL(godot audit): " + gd_err)

    res, failures = evaluate(spec, bl, gd, bl_err, gd_err)
    cause, why = classify(failures, bl, gd)
    out = {
        "id": args.id,
        "pass": not failures,
        "attempt": args.attempt,
        "ts": datetime.now().isoformat(timespec="seconds"),
        "spec": str(spec_path.relative_to(ROOT)),
        "measurements": res,
        "failures": failures,
        "cause": cause,
        "cause_note": why,
        "suggested_fix": suggestions(failures, spec),
        "blender_audit_ok": bl is not None,
        "godot_audit_ok": gd is not None,
    }
    (CHECKS / ("%s.json" % args.id)).write_text(json.dumps(out, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")

    for f in failures:
        print("  [x] %-28s %-8s target=%s measured=%s" % (f["name"], f["side"], f.get("target"), f.get("measured")))
    print("%s %s cause=%s (pipeline/checks/%s.json)" % ("PASS" if out["pass"] else "FAIL", args.id, cause, args.id))
    return 0 if out["pass"] else 1


if __name__ == "__main__":
    sys.exit(main())
