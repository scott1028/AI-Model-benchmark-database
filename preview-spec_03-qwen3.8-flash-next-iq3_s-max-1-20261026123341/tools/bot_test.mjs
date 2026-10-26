import { spawnSync } from "node:child_process";
import fs from "node:fs";

const cfg = JSON.parse(fs.readFileSync("pipeline.config.json", "utf8"));
const r = spawnSync(
  cfg.godotPath,
  ["--headless", "--path", "game", "--fixed-fps", "60", "--quit-after", "1800", "--", "--bot"],
  { encoding: "utf8", timeout: 300000 }
);
if (r.error) {
  console.log(`BOT_TEST: ${r.error.message}`);
  process.exit(1);
}
const lines = `${r.stdout}\n${r.stderr}`.split(/\r?\n/).filter((l) => l.trim() !== "");
const relevant = lines.filter((l) => /COIN|WIN|NEED_COINS|SCRIPT ERROR|ERROR:/.test(l));
for (const l of relevant) console.log(l);

const gotCoins = lines.some((l) => l.includes("COIN 3/3"));
const gotWin = lines.some((l) => l.includes("WIN"));
const errLines = lines.filter((l) => l.includes("SCRIPT ERROR") || l.includes("ERROR:"));
const pass = gotCoins && gotWin && errLines.length === 0;

console.log(`BOT_TEST: ${pass ? "PASS" : "FAIL"} (COIN 3/3: ${gotCoins}, WIN: ${gotWin}, errors: ${errLines.length})`);
if (!pass) process.exit(1);
