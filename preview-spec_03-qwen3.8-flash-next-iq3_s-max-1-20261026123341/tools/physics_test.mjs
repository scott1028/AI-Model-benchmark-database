import { spawnSync } from "node:child_process";
import fs from "node:fs";

const cfg = JSON.parse(fs.readFileSync("pipeline.config.json", "utf8"));
const r = spawnSync(
  cfg.godotPath,
  ["--headless", "--path", "game", "--fixed-fps", "60", "-s", "res://tools/physics_test.gd"],
  { encoding: "utf8", timeout: 300000 }
);
if (r.error) {
  console.log(`PHYSICS: FAIL (${r.error.message})`);
  process.exit(1);
}
const lines = `${r.stdout}\n${r.stderr}`.split(/\r?\n/).filter((l) => l.trim() !== "");
for (const l of lines.filter((l) => /PASS|FAIL|TREE_HIT/.test(l))) console.log(l);

const scriptErrors = lines.filter((l) => l.includes("SCRIPT ERROR") || l.includes("ERROR:"));
for (const l of scriptErrors) console.log(l);
const flagged = lines.filter((l) => /FAIL|TREE_HIT/.test(l));
const pass = lines.some((l) => l.includes("PHYSICS_TEST: PASS")) && r.status === 0 && scriptErrors.length === 0;
console.log(`PHYSICS: ${pass ? "PASS" : "FAIL"} (${flagged.length} flagged lines, exit ${r.status})`);
if (!pass) process.exit(1);
