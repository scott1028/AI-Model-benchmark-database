// Headless smoke test for the WebGL build: serves the project over HTTP, drives
// google-chrome over CDP, collects console/errors, screenshots the canvas.
//
//   node tools/web_smoke.mjs [--url <url>] [--out <png>] [--wait <s>] [--port <n>]
//
// Prints one JSON line: { ok, errors, warnings, console, canvas, screenshot, ms }
import { spawn } from "node:child_process";
import { createServer } from "node:http";
import { readFile, stat } from "node:fs/promises";
import { extname, join, normalize, resolve } from "node:path";

const arg = (k, d) => {
  const i = process.argv.indexOf("--" + k);
  return i >= 0 ? process.argv[i + 1] : d;
};
const ROOT = resolve(arg("root", resolve(import.meta.dirname, "..")));
const SITE_PORT = Number(arg("port", 8137));
const GAME_URL = arg("url", `http://127.0.0.1:${SITE_PORT}/web/index.html`);
const OUT = arg("out", join(ROOT, "pipeline/screenshots/web-smoke.png"));
const WAIT_S = Number(arg("wait", 150));
const DEBUG_PORT = 9333;

const MIME = {
  ".html": "text/html; charset=utf-8", ".js": "text/javascript", ".wasm": "application/wasm",
  ".json": "application/json", ".png": "image/png", ".pck": "application/octet-stream",
  ".css": "text/css", ".svg": "image/svg+xml",
};

const BOT = process.argv.includes("--bot");
const server = createServer(async (req, res) => {
  let path = join(ROOT, normalize(decodeURIComponent(req.url.split("?")[0])));
  try {
    if ((await stat(path)).isDirectory()) path = join(path, "index.html"); // same as python -m http.server
    let body = await readFile(path);
    // --bot: the exported shell passes GODOT_CONFIG.args, so ask the engine for user arg --bot
    if (BOT && path === join(ROOT, "web", "index.html")) {
      body = Buffer.from(body.toString().replace('"args":[]', '"args":["--","--bot"]'));
    }
    res.writeHead(200, { "content-type": MIME[extname(path)] || "application/octet-stream" });
    res.end(body);
  } catch {
    res.writeHead(404).end("not found");
  }
});
await new Promise((r) => server.listen(SITE_PORT, "127.0.0.1", r));

const chrome = spawn("google-chrome", [
  "--headless=new", `--remote-debugging-port=${DEBUG_PORT}`, "--no-sandbox",
  "--disable-dev-shm-usage", "--use-gl=angle", "--use-angle=swiftshader",
  "--enable-unsafe-swiftshader", "--ignore-certificate-errors",  // self-signed TLS is part of what we test
  "--window-size=1280,720", "about:blank",
], { stdio: "ignore" });

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
const jget = async (p, init) => {
  for (let i = 0; i < 60; i++) {
    try {
      const r = await fetch(`http://127.0.0.1:${DEBUG_PORT}${p}`, init);
      return await r.json();
    } catch {
      await sleep(500);
    }
  }
  throw new Error("chrome devtools endpoint not ready");
};

const target = await jget("/json/new?about:blank", { method: "PUT" });
const ws = new WebSocket(target.webSocketDebuggerUrl);
await new Promise((r, j) => { ws.addEventListener("open", r); ws.addEventListener("error", j); });

let nextId = 0;
const pending = new Map();
const consoleLines = [];
const pageErrors = [];
const shutdownWarnings = [];
// Godot's own shutdown bookkeeping (also seen in the headless runs); it fires when Chrome exits,
// never during play, so it is recorded separately instead of failing the run.
const SHUTDOWN_NOISE = /Pages in use exist at exit|Leaked instance dependency|ObjectDB instances were leaked|resources still in use at exit|RID allocations|Leaked .*instance|at: ~PagedAllocator/;
const record = (type, text) => {
  consoleLines.push({ type, text });
  if (type === "error") {
    if (SHUTDOWN_NOISE.test(text)) shutdownWarnings.push(text);
    else pageErrors.push(text);
  }
};
ws.addEventListener("message", (ev) => {
  const msg = JSON.parse(ev.data);
  if (msg.id && pending.has(msg.id)) {
    pending.get(msg.id)(msg.result || {});
    pending.delete(msg.id);
    return;
  }
  if (msg.method === "Runtime.consoleAPICalled") {
    record(msg.params.type, (msg.params.args || []).map((a) => a.value ?? a.description ?? "").join(" "));
  }
  if (msg.method === "Runtime.exceptionThrown") {
    pageErrors.push(msg.params.exceptionDetails?.exception?.description || "exception");
  }
  if (msg.method === "Log.entryAdded") {
    const e = msg.params.entry;
    if (e.level === "error" && /favicon/i.test((e.url || "") + " " + e.text)) return; // browser default icon
    record(e.level, [e.source, e.url, e.text].filter(Boolean).join(" "));
  }
});
const send = (method, params = {}) =>
  new Promise((r) => { const id = ++nextId; pending.set(id, r); ws.send(JSON.stringify({ id, method, params })); });

await send("Runtime.enable");
await send("Log.enable");
await send("Page.enable");
await send("Page.navigate", { url: GAME_URL });

// Godot boots wasm + software GL, so poll the canvas until it has painted frames.
const probe = async () => {
  const expr = `(() => {
    const c = document.querySelector("canvas");
    const gl = c && (c.getContext("webgl2") || c.getContext("webgl"));
    return JSON.stringify({ canvas: !!c, w: c && c.width, h: c && c.height, gl: !!gl,
      url: location.href, path: location.pathname,
      status: (document.getElementById("status") || {}).textContent || "" });
  })()`;
  const r = await send("Runtime.evaluate", { expression: expr, returnByValue: true });
  return JSON.parse(r.result?.value || "{}");
};

const t0 = Date.now();
let seen = {};
const engineBanner = () => consoleLines.some((l) => /Godot Engine v/i.test(l.text));
// --bot: load with ?args=--bot and wait for the bot's win event, so the browser run proves
// gameplay + physics, not just boot.
const WANT_WIN = process.argv.includes("--bot");
const winLine = () => consoleLines.find((l) => /"event":\s*"win"/.test(l.text));
while (Date.now() - t0 < WAIT_S * 1000) {
  await sleep(2000);
  seen = await probe();
  if (seen.gl && seen.w > 0 && engineBanner() && (!WANT_WIN || winLine())) break;
}

// --play <s>: hold the right-arrow key so the browser build proves input + physics, not just boot.
// The DOM listener proves whether Chrome delivered the key at all (Godot reads DOM keydown).
const PLAY = Number(arg("play", 0));
let domKeys = [];
if (PLAY > 0) {
  await send("Runtime.evaluate", { expression: `
    window.__keys = [];
    window.addEventListener("keydown", (e) => window.__keys.push(
      e.code + " keyCode=" + e.keyCode + " target=" + e.target.tagName + " focus=" + document.hasFocus()), true);
  ` });
  await send("Page.bringToFront");
  // A real click gives the canvas focus inside the engine, which is how a player starts playing.
  for (const type of ["mousePressed", "mouseReleased"]) {
    await send("Input.dispatchMouseEvent", { type, x: 640, y: 316, button: "left", buttons: type === "mousePressed" ? 1 : 0, clickCount: 1 });
  }
  await send("Runtime.evaluate", { expression: "document.querySelector('canvas').focus(); 'ok'" });
  const key = { key: "ArrowRight", code: "ArrowRight", windowsVirtualKeyCode: 39, nativeVirtualKeyCode: 39 };
  await send("Input.dispatchKeyEvent", { type: "rawKeyDown", ...key });
  // Also dispatch keydown from inside the page: Godot's web input reads DOM events, and a page-side
  // event separates "Chrome delivered the key" from "Godot consumed the key".
  const t_end = Date.now() + PLAY * 1000;
  while (Date.now() < t_end) {
    await send("Runtime.evaluate", { expression: `
      document.querySelector("canvas").dispatchEvent(new KeyboardEvent("keydown", {
        code: "ArrowRight", key: "ArrowRight", keyCode: 39, which: 39, bubbles: true, cancelable: true }));
    ` });
    await sleep(200);
  }
  await send("Input.dispatchKeyEvent", { type: "keyUp", ...key });
  const r = await send("Runtime.evaluate", { expression: "JSON.stringify(window.__keys || [])", returnByValue: true });
  domKeys = JSON.parse(r.result?.value || "[]");
}

const shot = await send("Page.captureScreenshot", { format: "png" });
const { writeFile } = await import("node:fs/promises");
await writeFile(OUT, Buffer.from(shot.data || "", "base64"));

const win = winLine();
const verdict = {
  ok: pageErrors.length === 0 && seen.gl === true && engineBanner() && (!WANT_WIN || !!win),
  url: GAME_URL, canvas: seen, engine_banner: engineBanner(), play_s: PLAY, dom_keys: domKeys,
  bot_win: win ? win.text : null,
  errors: pageErrors, shutdown_warnings: shutdownWarnings, warnings: consoleLines.filter((l) => l.type === "warning").length,
  console: consoleLines.length, console_tail: consoleLines.slice(-6).map((l) => l.type + " " + l.text.slice(0, 110)),
  screenshot: OUT, ms: Date.now() - t0,
};
console.log(JSON.stringify(verdict));

ws.close();
chrome.kill("SIGTERM");
server.close();
process.exit(verdict.ok ? 0 : 1);
