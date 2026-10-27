"""Standard asset export. Runs INSIDE the live Blender, called from Blender MCP:

  import runpy
  runpy.run_path("<ROOT>/tools/blender_export.py", init_globals={"ROOT": "<ROOT>", "ASSET_ID": "coin"})

Saves blender/<id>.blend and exports godot/assets/<id>/<id>.glb with fixed flags,
so export settings never drift between assets (AGENTS.md §4).
"""
import json
import os
from pathlib import Path

ROOT = Path(globals().get("ROOT") or os.environ.get("COINRUN_ROOT") or ".").resolve()  # noqa: F821
ASSET_ID = globals().get("ASSET_ID")  # noqa: F821

import bpy  # noqa: E402

blend_path = ROOT / "blender" / ("%s.blend" % ASSET_ID)
glb_dir = ROOT / "godot" / "assets" / ASSET_ID
glb_path = glb_dir / ("%s.glb" % ASSET_ID)
blend_path.parent.mkdir(parents=True, exist_ok=True)
glb_dir.mkdir(parents=True, exist_ok=True)

roots = [o for o in bpy.context.scene.objects if o.name == ASSET_ID or o.name == ASSET_ID + "_root"]
if not roots:
    roots = [o for o in bpy.context.selected_objects if o.parent is None]
if not roots:
    raise RuntimeError("no root object named %s and nothing selected" % ASSET_ID)

bpy.ops.object.select_all(action="DESELECT")


def select_tree(o):
    o.select_set(True)
    for c in o.children:
        select_tree(c)


for o in roots:
    select_tree(o)

bpy.ops.wm.save_as_mainfile(filepath=str(blend_path))

# Pass only flags this Blender version understands (Blender 5.x dropped export_empties).
wanted = {
    "export_format": "GLB",
    "export_yup": True,
    "export_apply": True,
    "export_animations": True,
    "export_materials": "EXPORT",
    "export_extras": True,
    "export_cameras": False,
    "export_lights": False,
    "export_image_format": "AUTO",
}
# selection-scoped export flag was renamed across Blender versions
sel_flag = "use_selection" if "use_selection" in {p.identifier for p in bpy.ops.export_scene.gltf.get_rna_type().properties} else "export_selected"
wanted[sel_flag] = True
supported = {p.identifier for p in bpy.ops.export_scene.gltf.get_rna_type().properties}
flags = {k: v for k, v in wanted.items() if k in supported}
skipped = sorted(set(wanted) - supported)
bpy.ops.export_scene.gltf(filepath=str(glb_path), **flags)

report = {
    "id": ASSET_ID,
    "blend": str(blend_path),
    "glb": str(glb_path),
    "glb_bytes": glb_path.stat().st_size if glb_path.exists() else 0,
    "exported_objects": len(bpy.context.selected_objects),
    "export_flags": sorted(flags),
    "export_flags_unsupported": skipped,
    "actions": [a.name for a in bpy.data.actions],
}
print("BLENDER_EXPORT " + json.dumps(report, ensure_ascii=False))
