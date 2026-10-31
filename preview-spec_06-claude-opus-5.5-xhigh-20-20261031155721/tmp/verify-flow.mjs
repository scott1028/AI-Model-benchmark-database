// Headless Chrome check over CDP: redirect, console errors, WebGL rendering, input.
// Usage: node verify.mjs <url> <outDir> <chromeProfileDir>
import { spawn } from 'node:child_process';
import { mkdirSync, writeFileSync } from 'node:fs';
import { setTimeout as sleep } from 'node:timers/promises';

const [url, outDir, profileDir] = process.argv.slice(2);
mkdirSync(outDir, { recursive: true });
const port = 9334;
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

const waitLoaded = async () => {
  await sleep(1000);
  for (let i = 0; i < 60; i++) {
    if (await evaluate("document.getElementById('load-status')?.textContent === ''")) break;
    await sleep(250);
  }
  await sleep(300);
};
const text = (id) => evaluate(`document.getElementById('${id}').textContent`);
const startCustom = async (mines) => {
  await evaluate(`localStorage.setItem('kmines.settings', JSON.stringify({ customWidth: 5, customHeight: 5, customMines: ${mines} }));
    localStorage.setItem('kmines.level', JSON.stringify('custom'));`);
  await send('Page.navigate', { url });
  await waitLoaded();
};
const geo = async () => evaluate(`(() => { const r = document.getElementById('scene').getBoundingClientRect();
  return { left: r.left, top: r.top, width: r.width, height: r.height }; })()`);
const cellCenterFor = (g) => {
  const cs = Math.trunc(Math.min(g.width / 7, g.height / 7));
  const fx = g.left + g.width / 2 - (cs * 7) / 2;
  const fy = g.top + g.height / 2 - (cs * 7) / 2;
  return (row, col) => [fx + (col + 1.5) * cs, fy + (row + 1.5) * cs];
};

const report = {};
// Win on first click: 5x5 with one mine.
await send('Page.navigate', { url });
await waitLoaded();
await startCustom(1);
let at = cellCenterFor(await geo());
await click(...at(2, 2));
await sleep(500);
report.win = {
  mines: await text('mines-display'),
  scoresOpen: await evaluate("document.getElementById('scores-dialog').open"),
  hasNameInput: await evaluate("!!document.getElementById('score-name')"),
};
await screenshot('10-win-highscore');
await evaluate("document.getElementById('score-name').value = 'Tester'; document.querySelector('#scores-dialog button[value=close]').click()");
await sleep(300);
report.win.saved = await evaluate("localStorage.getItem('kmines.highscores')");
await screenshot('11-win-board');

// Lose: 5x5 with 15 mines (max 25-10).
await startCustom(15);
at = cellCenterFor(await geo());
await click(...at(2, 2));
await sleep(300);
report.lose = { minesLabel: await text('mines-display') };
const ring = [[0,0],[0,1],[0,2],[0,3],[0,4],[1,0],[1,4],[2,0],[2,4],[3,0],[3,4],[4,0],[4,1],[4,2],[4,3],[4,4]];
for (const [r, c] of ring) {
  await click(...at(r, c));
  await sleep(150);
  if (await evaluate("document.getElementById('reset-dialog').open")) { report.lose.lostAt = [r, c]; break; }
}
report.lose.resetDialogOpen = await evaluate("document.getElementById('reset-dialog').open");
await screenshot('12-lost-reset-dialog');
await evaluate("document.querySelector('#reset-dialog button[value=reset]').click()");
await sleep(400);
report.lose.afterReset = {
  dialogOpen: await evaluate("document.getElementById('reset-dialog').open"),
  pauseEnabled: await evaluate("!document.getElementById('action-pause').disabled"),
  mines: await text('mines-display'),
};
await screenshot('13-after-reset');

// Settings dialog opens and closes.
await evaluate("document.getElementById('action-settings').click()");
await sleep(300);
report.settingsOpen = await evaluate("document.getElementById('settings-dialog').open");
await screenshot('14-settings');
await evaluate("document.querySelector('#settings-dialog button[value=cancel]').click()");

report.console = logs;
report.errors = logs.filter((l) => ['error', 'exception', 'log-error', 'assert'].includes(l.type));
writeFileSync(`${outDir}/report-flow.json`, JSON.stringify(report, null, 2));
console.log(JSON.stringify(report, null, 2));
ws.close();
chrome.kill();
