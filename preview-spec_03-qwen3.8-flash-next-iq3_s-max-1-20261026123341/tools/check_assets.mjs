import { spawnSync } from "node:child_process";
import fs from "node:fs";

const cfg = JSON.parse(fs.readFileSync("pipeline.config.json", "utf8"));
const specs = JSON.parse(fs.readFileSync("assets.json", "utf8")).assets;
const ids = process.argv.slice(2);
const targets = ids.length > 0 ? ids : Object.keys(specs);
for (const id of targets) {
  if (!specs[id]) {
    console.log(`UNKNOWN asset id: ${id}`);
    process.exit(1);
  }
}

const resPaths = targets.map((id) => `res://assets/${id}.glb`);
const r = spawnSync(
  cfg.godotPath,
  ["--headless", "--path", "game", "-s", "res://tools/inspect_assets.gd", "--", ...resPaths],
  { encoding: "utf8", timeout: 300000 }
);
if (r.error) {
  console.log(`CHECK_ASSETS: ${r.error.message}`);
  process.exit(1);
}
const inspectLine = `${r.stdout}`
  .split(/\r?\n/)
  .reverse()
  .find((l) => l.startsWith("INSPECT_JSON:"));
if (!inspectLine) {
  console.log("CHECK_ASSETS: no INSPECT_JSON output");
  console.error(r.stderr);
  process.exit(1);
}
const inspect = JSON.parse(inspectLine.slice("INSPECT_JSON:".length));

const AXIS = ["X 寬", "Y 高", "Z 深"];
const tol = cfg.sizeTolerance;
const axisTol = (specV) => Math.max(tol.relative * specV, tol.absoluteMin);
const f2 = (v) => Number(v).toFixed(2);

function checkSize(label, actual, specSize, id) {
  let ok = true;
  for (let a = 0; a < 3; a++) {
    const diff = Math.abs(actual[a] - specSize[a]);
    if (diff > axisTol(specSize[a])) {
      ok = false;
      const factor = specSize[a] / actual[a];
      console.log(
        `FAIL ${id}: ${label} ${AXIS[a]} ${f2(actual[a])}m，規格 ${f2(specSize[a])}m（差 ${f2(diff)}m > 容差 ${f2(axisTol(specSize[a]))}m）`
      );
      if (label === "Blender") {
        console.log(`FIX ${id}: ${AXIS[a]} ${f2(actual[a])}m，規格 ${f2(specSize[a])}m → 在 Blender 以原點為基準沿 ${"XYZ"[a]} 軸縮放 ${factor.toFixed(3)} 後 apply scale`);
      } else {
        console.log(`FIX ${id}: ${AXIS[a]} ${f2(actual[a])}m，規格 ${f2(specSize[a])}m → 在 Blender 沿 ${"XYZ"[a]} 軸縮放 ${factor.toFixed(3)} 後 apply scale`);
      }
    }
  }
  if (ok) console.log(`PASS ${id}: ${label} 尺寸 ${actual.map(f2).join("×")}`);
  return ok;
}

let passCount = 0;
let failCount = 0;
for (const id of targets) {
  const spec = specs[id];
  const insp = inspect[`res://assets/${id}.glb`];
  const statPath = `pipeline/blender_stats/${id}.json`;
  const stats = fs.existsSync(statPath) ? JSON.parse(fs.readFileSync(statPath, "utf8")) : null;
  const problems = [];

  const loaded = insp && !insp.error;
  if (!loaded) {
    problems.push("load");
    console.log(`FAIL ${id}: 載入 — ${insp ? insp.error : "inspect 查無此資產"}`);
    console.log(`FIX ${id}: 確認 game/assets/${id}.glb 已匯出且跑過 node tools/import.mjs`);
  } else {
    console.log(`PASS ${id}: 載入`);
  }

  let blenderOk = true;
  let godotOk = true;
  if (loaded) {
    if (!stats) {
      blenderOk = false;
      problems.push("stats");
      console.log(`FAIL ${id}: Blender 尺寸 — 缺少 ${statPath}（匯出時要跑附錄 A）`);
    } else {
      blenderOk = checkSize("Blender", stats.size_m, spec.size_m, id);
    }
    godotOk = checkSize("Godot", insp.aabb_size, spec.size_m, id);
    if (blenderOk && !godotOk) console.log(`PIPELINE: export/軸向問題，不是建模問題`);
    if (!blenderOk || !godotOk) problems.push("size");

    if (spec.origin === "bottom_center") {
      const [mnx, mny, mnz] = insp.aabb_min;
      const [sx, sy, sz] = insp.aabb_size;
      const dy = Math.abs(mny);
      const dx = Math.abs(mnx + sx / 2);
      const dz = Math.abs(mnz + sz / 2);
      if (dy <= 0.02 && dx <= 0.02 && dz <= 0.02) {
        console.log(`PASS ${id}: 原點 bottom_center`);
      } else {
        problems.push("origin");
        console.log(`FAIL ${id}: 原點 — y=${f2(mny)}（要 0）、X 中心偏移 ${f2(dx)}、Z 中心偏移 ${f2(dz)}`);
        console.log(`FIX ${id}: 在 Blender 把物件底部移到 z=0、XY 置中後 apply location`);
      }
    }

    if (stats) {
      if (stats.tris <= spec.max_tris) {
        console.log(`PASS ${id}: 面數 ${stats.tris} ≤ ${spec.max_tris}`);
      } else {
        problems.push("tris");
        console.log(`FAIL ${id}: 面數 ${stats.tris} > ${spec.max_tris}`);
        console.log(`FIX ${id}: 在 Blender 減少分段/subdivision 或簡化細節`);
      }
    }

    if (spec.collision === "col") {
      if (insp.has_collision) {
        console.log(`PASS ${id}: 碰撞`);
      } else {
        problems.push("collision");
        console.log(`FAIL ${id}: 碰撞 — 匯入後沒有 StaticBody3D`);
        console.log(`FIX ${id}: 在 Blender 加簡單外形 mesh 物件，名稱加 -col 後綴`);
      }
    }

    for (const anim of spec.anims) {
      if ((insp.anims || []).includes(anim)) {
        console.log(`PASS ${id}: 動畫 ${anim}`);
      } else {
        problems.push("anim");
        console.log(`FAIL ${id}: 動畫 ${anim} 不存在（找到：[${(insp.anims || []).join(", ")}]）`);
        console.log(`FIX ${id}: 在 Blender 確認 AnimationPlayer 的 action 名為 ${anim} 且隨 collection 一起匯出`);
      }
    }
  }

  if (problems.length === 0) passCount += 1;
  else failCount += 1;
}

console.log(`SUMMARY: ${passCount} pass, ${failCount} fail`);
if (failCount > 0) process.exit(1);
