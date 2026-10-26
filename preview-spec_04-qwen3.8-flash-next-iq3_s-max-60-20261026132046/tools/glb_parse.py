#!/usr/bin/env python3
"""Parse a GLB (no deps): node transforms -> world bbox, triangle count,
animation names, col_* node detection. Y-up coords (Blender: x, z=-y_up? we
convert back to Z-up by swapping: X=x, Y=-y_gltf? spec: gltf(x, y_up, -z forward)).
We report dims as (x, y_depth, z_height) in Blender convention: height = gltf.y, depth = gltf.z (negated).
"""
import json
import struct
import sys


def load_glb(path):
    with open(path, "rb") as f:
        magic, ver, total = struct.unpack("<III", f.read(12))
        assert magic == 0x46546C67, "not a GLB"
        j = None
        while True:
            hdr = f.read(8)
            if len(hdr) < 8:
                break
            clen, ctype = struct.unpack("<II", hdr)
            chunk = f.read(clen)
            if ctype in (0, 0x4E4F534A):  # JSON chunk
                j = json.loads(chunk.decode("utf-8"))
        return j


def mat4_from_node(node):
    if "matrix" in node:
        return node["matrix"]
    t = node.get("translation", [0, 0, 0])
    r = node.get("rotation", [0, 0, 0, 1])  # x,y,z,w
    s = node.get("scale", [1, 1, 1])
    x, y, z, w = r
    xx, yy, zz, xy, xz, yz, wx, wy, wz = (
        x * x, y * y, z * z, x * y, x * z, y * z, w * x, w * y, w * z)
    m = [
        (1 - 2 * (yy + zz)) * s[0], 2 * (xy + wz) * s[0], 2 * (xz - wy) * s[0], 0,
        2 * (xy - wz) * s[1], (1 - 2 * (xx + zz)) * s[1], 2 * (yz + wx) * s[1], 0,
        2 * (xz + wy) * s[2], 2 * (yz - wx) * s[2], (1 - 2 * (xx + yy)) * s[2], 0,
        t[0], t[1], t[2], 1,
    ]
    return m  # column-major like glTF


def mul(a, b):
    # column-major 4x4 multiply: result = a * b
    out = [0.0] * 16
    for c in range(4):
        for rr in range(4):
            s = 0.0
            for k in range(4):
                s += a[k * 4 + rr] * b[c * 4 + k]
            out[c * 4 + rr] = s
    return out


def xf(m, p):
    return [
        m[0] * p[0] + m[4] * p[1] + m[8] * p[2] + m[12],
        m[1] * p[0] + m[5] * p[1] + m[9] * p[2] + m[13],
        m[2] * p[0] + m[6] * p[1] + m[10] * p[2] + m[14],
    ]


def analyze(path):
    g = load_glb(path)
    acc = g.get("accessors", [])
    meshes = {m["name"]: m for m in g.get("meshes", []) if "name" in m}
    nodes = g.get("nodes", [])
    child_of = {}
    for i, n in enumerate(nodes):
        for c in n.get("children", []):
            child_of[c] = i
    roots = [i for i in range(len(nodes)) if i not in child_of]
    vmin = [1e9] * 3
    vmax = [-1e9] * 3
    tris = 0
    col_tris = 0
    col_nodes = []
    visual_nodes = []

    def walk(i, m):
        nonlocal tris, col_tris
        n = nodes[i]
        nm = n.get("name", "")
        m = mul(m, mat4_from_node(n))
        is_col = nm.startswith("col_")
        if is_col:
            col_nodes.append(nm)
        if "mesh" in n:
            if not is_col:
                visual_nodes.append(nm)
            mesh = g["meshes"][n["mesh"]]
            for pr in mesh.get("primitives", []):
                pa = acc[pr["attributes"]["POSITION"]]
                lo, hi = pa["min"], pa["max"]
                for cx in (lo, hi):
                    p = xf(m, cx)
                    for k in range(3):
                        vmin[k] = min(vmin[k], p[k])
                        vmax[k] = max(vmax[k], p[k])
                t = acc[pr["indices"]]["count"] // 3 if "indices" in pr else pa["count"] // 3
                if is_col:
                    col_tris += t
                else:
                    tris += t
        for c in n.get("children", []):
            walk(c, m)

    ident = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]
    for r in roots:
        walk(r, ident)

    # glTF Y-up -> report Blender-style Z-up: dims (x, depth, height)
    gx = [vmax[0] - vmin[0], vmax[1] - vmin[1], vmax[2] - vmin[2]]
    dims = [gx[0], gx[2], gx[1]]  # x, y_depth, z_height
    # bbox in Z-up: (x, z_gltf up as y? ) keep zmin = gltf.y min
    zmin_up = vmin[1]
    anims = [a.get("name", "") for a in g.get("animations", [])]
    return {
        "asset_dims_xyz_zup": [round(d, 4) for d in dims],
        "bbox_min_yup": [round(v, 4) for v in vmin],
        "bbox_max_yup": [round(v, 4) for v in vmax],
        "up_zmin": round(zmin_up, 4),
        "center_xy_zup": [round((vmin[0] + vmax[0]) / 2, 4), round((vmin[2] + vmax[2]) / 2, 4)],
        "tris": tris,
        "col_tris": col_tris,
        "col_nodes": col_nodes,
        "visual_nodes": visual_nodes,
        "animations": anims,
        "node_count": len(nodes),
    }


if __name__ == "__main__":
    print(json.dumps(analyze(sys.argv[1]), indent=2))
