// Headless Chrome check over CDP: redirect, console errors, WebGL rendering, input.
// Usage: node verify.mjs <url> <outDir> <chromeProfileDir>
import { spawn } from 'node:child_process';
import { mkdirSync, writeFileSync } from 'node:fs';
import { setTimeout as sleep } from 'node:timers/promises';

const [url, outDir, profileDir] = process.argv.slice(2);
mkdirSync(outDir, { recursive: true });
const port = 9333;
const chrome = spawn('google-chrome', [
  '--headless=new', `--remote-debugging-port=${port}`, `--user-data-dir=${profileDir}`,
  '--no-first-run', '--no-default-browser-check', '--use-angle=swiftshader', '--enable-unsafe-swiftshader',
  '--window-size=1100,800', 'about:blank',
], { stdio: 'ignore' });

const getJSON = async (path) => (await fetch(`http://127.0.0.1:${port}${path}`)).json();
let targets;
for (let i = 0; i < 50; i++) {
  try { targets = await getJSON('/json'); break; } catch { await sleep(200); }
}
const page = targets.find((t) => t.type === 'page');
const ws = new WebSocket(page.webSocketDebuggerUrl);
await new Promise((r) => ws.addEventListener('open', r, { once: true }));

let nextId = 1;
const pending = new Map();
const logs = [];
ws.addEventListener('message', (ev) => {
  const msg = JSON.parse(ev.data);
  if (msg.id && pending.has(msg.id)) {
    pending.get(msg.id)(msg);
    pending.delete(msg.id);
  } else if (msg.method === 'Runtime.consoleAPICalled') {
    logs.push({ type: msg.params.type, text: msg.params.args.map((a) => a.value ?? a.description).join(' ') });
  } else if (msg.method === 'Runtime.exceptionThrown') {
    logs.push({ type: 'exception', text: msg.params.exceptionDetails.exception?.description ?? msg.params.exceptionDetails.text });
  } else if (msg.method === 'Log.entryAdded') {
    logs.push({ type: `log-${msg.params.entry.level}`, text: `${msg.params.entry.text} ${msg.params.entry.url ?? ''}` });
  }
});
const send = (method, params = {}) => new Promise((resolve) => {
  const id = nextId++;
  pending.set(id, resolve);
  ws.send(JSON.stringify({ id, method, params }));
});
const evaluate = async (expression) =>
  (await send('Runtime.evaluate', { expression, returnByValue: true, awaitPromise: true })).result?.result?.value;
const screenshot = async (name) => {
  const res = await send('Page.captureScreenshot', { format: 'png' });
  writeFileSync(`${outDir}/${name}.png`, Buffer.from(res.result.data, 'base64'));
};
const mouse = async (type, x, y, button = 'left', buttons = 0) =>
  send('Input.dispatchMouseEvent', { type, x, y, button, buttons, clickCount: 1 });
const click = async (x, y, button = 'left') => {
  const mask = { left: 1, right: 2, middle: 4 }[button];
  await mouse('mousePressed', x, y, button, mask);
  await mouse('mouseReleased', x, y, button, 0);
};

await send('Runtime.enable');
await send('Log.enable');
await send('Page.enable');
await send('Emulation.setDeviceMetricsOverride', { width: 1100, height: 800, deviceScaleFactor: 1, mobile: false });
await send('Page.navigate', { url });
await sleep(1500);
for (let i = 0; i < 60; i++) {
  if (await evaluate("document.getElementById('load-status')?.textContent === ''")) break;
  await sleep(250);
}
await sleep(500);

const report = { url, finalUrl: await evaluate('location.href'), title: await evaluate('document.title') };
report.webgl = await evaluate("!!document.getElementById('scene').getContext('webgl')");
report.loadStatus = await evaluate("document.getElementById('load-status').textContent");
await screenshot('01-loaded');

// Easy 9x9 field: compute the layout like getFieldLayout().
const geo = await evaluate(`(() => { const r = document.getElementById('scene').getBoundingClientRect();
  return { left: r.left, top: r.top, width: r.width, height: r.height }; })()`);
const cs = Math.trunc(Math.min(geo.width / 11, geo.height / 11));
const fx = geo.left + geo.width / 2 - (cs * 11) / 2;
const fy = geo.top + geo.height / 2 - (cs * 11) / 2;
const cellCenter = (row, col) => [fx + (col + 1.5) * cs, fy + (row + 1.5) * cs];

// Sample pixels: background corner vs. a cell center must differ (sprites drawn).
const samplePixels = async () => evaluate(`(() => {
  const c = document.getElementById('scene'); const gl = c.getContext('webgl');
  const read = (x, y) => { const p = new Uint8Array(4); gl.readPixels(x, c.height - y - 1, 1, 1, gl.RGBA, gl.UNSIGNED_BYTE, p); return [...p]; };
  return { corner: read(2, 2), cell: read(${Math.round(cellCenter(0, 0)[0] - geo.left)}, ${Math.round(cellCenter(0, 0)[1] - geo.top)}) };
})()`);
report.pixelsBefore = await samplePixels();

await click(...cellCenter(4, 4));
await sleep(1600);
report.afterFirstClick = {
  mines: await evaluate("document.getElementById('mines-display').textContent"),
  time: await evaluate("document.getElementById('time-display').textContent"),
  pauseEnabled: await evaluate("!document.getElementById('action-pause').disabled"),
};
await screenshot('02-first-click');

// Flag a still-hidden corner cell via right click.
for (const [r, c] of [[0, 0], [0, 8], [8, 0], [8, 8], [0, 4], [4, 0]]) {
  await click(...cellCenter(r, c), 'right');
  await sleep(100);
  const mines = await evaluate("document.getElementById('mines-display').textContent");
  if (mines.startsWith('Mines: 1/')) { report.flaggedAt = [r, c]; break; }
}
report.minesAfterFlag = await evaluate("document.getElementById('mines-display').textContent");
await screenshot('03-flag');

// Pause hides the field.
await evaluate("document.getElementById('action-pause').click()");
await sleep(400);
await screenshot('04-paused');
await evaluate("document.getElementById('action-pause').click()");
await sleep(300);

// Other themes render without errors.
report.themes = {};
for (const theme of ['classic', 'clean_blue_dark', 'green', 'graveyard-mayhem', 'clean_blue', 'default']) {
  await evaluate(`(() => { const s = JSON.parse(localStorage.getItem('kmines.settings') || '{}');
    s.theme = '${theme}'; localStorage.setItem('kmines.settings', JSON.stringify(s)); })()`);
  await send('Page.reload');
  await sleep(1200);
  for (let i = 0; i < 80; i++) {
    if (await evaluate("document.getElementById('load-status')?.textContent === ''")) break;
    await sleep(250);
  }
  await sleep(400);
  report.themes[theme] = await evaluate("document.getElementById('load-status').textContent || 'ok'");
  await screenshot(`05-theme-${theme}`);
}

report.console = logs;
report.errors = logs.filter((l) => ['error', 'exception', 'log-error', 'assert'].includes(l.type));
writeFileSync(`${outDir}/report.json`, JSON.stringify(report, null, 2));
console.log(JSON.stringify(report, null, 2));
ws.close();
chrome.kill();
