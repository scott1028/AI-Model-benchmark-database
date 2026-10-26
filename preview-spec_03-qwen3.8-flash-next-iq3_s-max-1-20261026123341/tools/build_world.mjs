import fs from "node:fs";

const layout = JSON.parse(fs.readFileSync("world_layout.json", "utf8"));
const hm = JSON.parse(fs.readFileSync("pipeline/terrain_heights.json", "utf8"));

// kind → group node, scene path, route-check radius (null = no check)
const KIND = {
  wall_brick: { group: "Walls", scene: "res://assets/wall_brick.glb", radius: 2.0 },
  tree: { group: "Trees", scene: "res://scenes/tree.tscn", radius: 0.3 },
  bush: { group: "Bushes", scene: "res://scenes/bush.tscn", radius: null },
  rock: { group: "Rocks", scene: "res://assets/rock.glb", radius: 0.75 },
  barrel: { group: "Barrels", scene: "res://scenes/barrel.tscn", radius: 1.5 },
  crate: { group: "Props", scene: "res://assets/crate.glb", radius: 0.75 },
  house: { group: "Buildings", scene: "res://assets/house.glb", radius: 2.7 },
  mage_tower: { group: "Buildings", scene: "res://assets/mage_tower.glb", radius: 1.6 },
  temple_ruins: { group: "Buildings", scene: "res://assets/temple_ruins.glb", radius: 3.6 },
};
const SINK = { tree: 0.05, rock: 0.1, bush: 0.03 };

// bilinear sample of the terrain height map (Godot x, z → y)
const height = (x, z) => {
  const gx = Math.min(Math.max((x - hm.origin[0]) / hm.step, 0), hm.count - 1);
  const gz = Math.min(Math.max((z - hm.origin[1]) / hm.step, 0), hm.count - 1);
  const ix = Math.floor(gx), iz = Math.floor(gz);
  const ix1 = Math.min(ix + 1, hm.count - 1), iz1 = Math.min(iz + 1, hm.count - 1);
  const fx = gx - ix, fz = gz - iz;
  const h00 = hm.heights[iz][ix], h10 = hm.heights[iz][ix1];
  const h01 = hm.heights[iz1][ix], h11 = hm.heights[iz1][ix1];
  return (h00 * (1 - fx) + h10 * fx) * (1 - fz) + (h01 * (1 - fx) + h11 * fx) * fz;
};

const segDist = (px, pz, ax, az, bx, bz) => {
  const dx = bx - ax, dz = bz - az;
  const len2 = dx * dx + dz * dz;
  const t = len2 === 0 ? 0 : Math.min(Math.max(((px - ax) * dx + (pz - az) * dz) / len2, 0), 1);
  return Math.hypot(px - (ax + t * dx), pz - (az + t * dz));
};
const pathDist = (x, z) => {
  let best = Infinity;
  const pts = layout.bot_path;
  for (let i = 0; i < pts.length - 1; i++) {
    best = Math.min(best, segDist(x, z, pts[i][0], pts[i][1], pts[i + 1][0], pts[i + 1][1]));
  }
  return best;
};

// collect placed props
const placed = [];
for (const [groupKey, list] of Object.entries(layout.props)) {
  for (const p of list) {
    const [kind, x, z, yawDeg = 0, scale = 1] = p;
    if (!KIND[kind]) {
      console.log(`BUILD_WORLD: unknown kind "${kind}" in props.${groupKey}`);
      process.exit(1);
    }
    placed.push({ kind, x, z, yawDeg, scale, group: KIND[kind].group });
  }
}

// route check (walls excluded)
const blocked = placed
  .map((p) => ({ ...p, gap: pathDist(p.x, p.z) - KIND[p.kind].radius }))
  .filter((p) => KIND[p.kind].radius !== null && p.gap < 1.0);
if (blocked.length > 0) {
  console.log("BUILD_WORLD: FAIL 路線檢查 — 物件太靠近 bot_path（要 距離−半徑 ≥ 1.0）：");
  for (const p of blocked) {
    console.log(`  ${p.kind} (${p.x}, ${p.z}) 距離 ${pathDist(p.x, p.z).toFixed(2)}m − 半徑 ${KIND[p.kind].radius}m = ${p.gap.toFixed(2)}m`);
  }
  process.exit(1);
}

// flat check
const notFlat = layout.flat_pads
  .map(([x, z, r]) => ({ x, z, r, h: height(x, z) }))
  .filter((p) => Math.abs(p.h) > 0.03);
if (notFlat.length > 0) {
  console.log("BUILD_WORLD: FAIL 平整檢查 — flat_pads 高度偏離 0（要 |h| ≤ 0.03），請重匯地形：");
  for (const p of notFlat) console.log(`  (${p.x}, ${p.z}) r=${p.r} h=${p.h.toFixed(4)}`);
  process.exit(1);
}

// auto outer walls: every 4 m along ±20, centers -18..18
const walls = [];
const EDGE = 20;
for (let c = -18; c <= 18; c += 4) {
  walls.push({ kind: "wall_brick", x: c, z: -EDGE, yawDeg: 0, scale: 1, group: "Walls" }); // north
  walls.push({ kind: "wall_brick", x: c, z: EDGE, yawDeg: 0, scale: 1, group: "Walls" }); // south
  walls.push({ kind: "wall_brick", x: -EDGE, z: c, yawDeg: 90, scale: 1, group: "Walls" }); // west
  walls.push({ kind: "wall_brick", x: EDGE, z: c, yawDeg: 90, scale: 1, group: "Walls" }); // east
}

// groups in first-appearance order (placed props, then walls join an existing group)
const all = [...placed, ...walls];
const groupOrder = [];
for (const p of all) if (!groupOrder.includes(p.group)) groupOrder.push(p.group);

const pascal = (kind) =>
  kind
    .split("_")
    .map((w) => w[0].toUpperCase() + w.slice(1))
    .join("");

const extIds = new Map();
for (const p of all) {
  const scene = KIND[p.kind].scene;
  if (!extIds.has(scene)) extIds.set(scene, String(extIds.size + 1));
}

const n = (v) => {
  const s = v.toFixed(5);
  return s.replace(/\.?0+$/, "") || "0";
};
const transformLine = (x, z, yawDeg, scale, y) => {
  const a = (yawDeg * Math.PI) / 180;
  const c = Math.cos(a), s = Math.sin(a);
  const [bxx, bxz, bzxx, bzx] = [c * scale, s * scale, -s * scale, c * scale];
  return `Transform3D(${n(bxx)}, 0, ${n(bxz)}, 0, ${n(scale)}, 0, ${n(bzxx)}, 0, ${n(bzx)}, ${n(x)}, ${n(y)}, ${n(z)})`;
};

const lines = [];
lines.push(`[gd_scene load_steps=${extIds.size + 1} format=3]`);
lines.push("");
for (const [scene, id] of extIds) {
  lines.push(`[ext_resource type="PackedScene" path="${scene}" id="${id}"]`);
}
lines.push("");
lines.push(`[node name="World" type="Node3D"]`);
lines.push("");

const counters = {};
for (const group of groupOrder) {
  lines.push(`[node name="${group}" type="Node3D" parent="."]`);
  lines.push("");
  for (const p of all.filter((q) => q.group === group)) {
    counters[p.kind] = (counters[p.kind] || 0) + 1;
    const name = `${pascal(p.kind)}${String(counters[p.kind]).padStart(2, "0")}`;
    const y = height(p.x, p.z) - (SINK[p.kind] ?? 0);
    lines.push(
      `[node name="${name}" parent="${group}" instance=ExtResource("${extIds.get(KIND[p.kind].scene)}")]`,
      `transform = ${transformLine(p.x, p.z, p.yawDeg, p.scale, y)}`
    );
  }
  lines.push("");
}

fs.mkdirSync("game/scenes", { recursive: true });
fs.writeFileSync("game/scenes/world.tscn", lines.join("\n"));
console.log(
  `BUILD_WORLD: ${all.length} props (${placed.length} placed + 40 walls) on terrain → game/scenes/world.tscn`
);
