import { spawnSync } from "node:child_process";
import fs from "node:fs";

const cfg = JSON.parse(fs.readFileSync("pipeline.config.json", "utf8"));
const r = spawnSync(cfg.godotPath, ["--headless", "--path", "game", "--import"], { encoding: "utf8", timeout: 300000 });
if (r.error) {
  console.log(`IMPORT: ${r.error.message}`);
  process.exit(1);
}
for (const line of `${r.stdout}\n${r.stderr}`.split(/\r?\n/)) {
  if (/ERROR|WARNING/.test(line)) console.log(line);
}
if (r.status !== 0) {
  console.log(`IMPORT: FAIL (exit ${r.status})`);
  process.exit(1);
}
console.log("IMPORT: OK");
