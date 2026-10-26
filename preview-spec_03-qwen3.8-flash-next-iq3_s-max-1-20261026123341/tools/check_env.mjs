import { spawnSync } from "node:child_process";
import net from "node:net";
import fs from "node:fs";

const cfg = JSON.parse(fs.readFileSync("pipeline.config.json", "utf8"));
let ok = true;

const gv = spawnSync(cfg.godotPath, ["--version"], { encoding: "utf8", timeout: 60000 });
if (gv.error || gv.status !== 0) {
  console.log(`NG godot: ${gv.error ? gv.error.message : `exit ${gv.status}`} (${cfg.godotPath})`);
  ok = false;
} else {
  console.log(`OK godot ${gv.stdout.trim()} (${cfg.godotPath})`);
}

const tcp = await new Promise((resolve) => {
  const sock = net.connect({ host: "127.0.0.1", port: cfg.blenderPort });
  sock.setTimeout(3000);
  sock.once("connect", () => { sock.destroy(); resolve(true); });
  sock.once("timeout", () => { sock.destroy(); resolve(false); });
  sock.once("error", () => resolve(false));
});
if (tcp) {
  // TCP only: port 9876 owned by another program also passes (appendix C-1).
  // Real validation is mcp__blender__get_scene_info succeeding.
  console.log(`OK blender MCP port ${cfg.blenderPort} listening (TCP only)`);
} else {
  console.log(`NG blender MCP port ${cfg.blenderPort} not listening`);
  ok = false;
}

process.exit(ok ? 0 : 1);
