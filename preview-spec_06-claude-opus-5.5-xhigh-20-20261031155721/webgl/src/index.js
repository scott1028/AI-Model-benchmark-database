// App entry: KMinesMainWindow + KMinesScene behavior on top of the WebGL renderer.
import {
  ALL_SPRITES, Button, CUSTOM_LIMITS, DEFAULT_SETTINGS, HIGHSCORE_LIMIT, LEVELS,
  MESSAGE_TIMEOUT_MS, THEMES,
} from './constants.js';
import { MineField } from './components/MineField.js';
import { drawPopupCanvas } from './components/Popup.js';
import { Renderer } from './components/Renderer.js';
import { loadTheme } from './components/ThemeLoader.js';
import {
  formatClock, getCellAt, getFieldLayout, getRandomInt, isInsideRect, loadJSON, saveJSON,
} from './utils.js';

const STORAGE = { settings: 'kmines.settings', level: 'kmines.level', scores: 'kmines.highscores', name: 'kmines.playerName' };
const POPUP_FADE_MS = 200;
// DOM button index -> Qt::MouseButton
const QT_BUTTON = { 0: Button.Left, 1: Button.Middle, 2: Button.Right };

const $ = (id) => document.getElementById(id);
const ui = {
  canvas: $('scene'), view: $('view'), loadStatus: $('load-status'),
  mines: $('mines-display'), time: $('time-display'), difficulty: $('difficulty'),
  newGame: $('action-new'), pause: $('action-pause'), highscores: $('action-highscores'), settings: $('action-settings'),
  resetDialog: $('reset-dialog'), settingsDialog: $('settings-dialog'), settingsForm: $('settings-form'),
  themeList: $('theme-list'), scoresDialog: $('scores-dialog'), scoresLevel: $('scores-level'), scoresBody: $('scores-body'),
};

const app = {
  settings: { ...DEFAULT_SETTINGS, ...loadJSON(STORAGE.settings, {}) },
  levelId: loadJSON(STORAGE.level, 'easy'),
  canScore: true,
  isPaused: false,
  clock: { elapsedMs: 0, startedAt: null, lastShown: -1 },
  theme: null,
  themeId: null,
  sheetKey: '',
  renderToken: 0,
  layout: { cellSize: 1, x: 0, y: 0, width: 0, height: 0 },
  popup: null, // { kind, canvas, shownAt, hideAt, w, h }
  isGrabbing: false,
  frame: 0,
};

const renderer = new Renderer(ui.canvas);

const field = new MineField({
  getSettings: () => app.settings,
  randomInt: getRandomInt,
  listener: {
    flaggedMinesCountChanged: (count) => { ui.mines.textContent = `Mines: ${count}/${field.minesCount}`; },
    firstClickDone: () => onFirstClick(),
    gameOver: (won) => onGameOver(won),
    changed: () => requestRender(),
  },
});

// ---- clock (KGameClock) ----
const getClockSeconds = () => {
  const { elapsedMs, startedAt } = app.clock;
  return Math.floor((elapsedMs + (startedAt === null ? 0 : performance.now() - startedAt)) / 1000);
};
const restartClock = () => { app.clock = { elapsedMs: 0, startedAt: null, lastShown: -1 }; };
const pauseClock = () => {
  if (app.clock.startedAt === null) return;
  app.clock.elapsedMs += performance.now() - app.clock.startedAt;
  app.clock.startedAt = null;
};
const resumeClock = () => {
  if (app.clock.startedAt === null) app.clock.startedAt = performance.now();
};
setInterval(() => {
  const seconds = getClockSeconds();
  if (app.clock.startedAt !== null && seconds !== app.clock.lastShown) {
    app.clock.lastShown = seconds;
    ui.time.textContent = `Time: ${formatClock(seconds)}`; // advanceTime (0x114400)
  }
}, 200);

// ---- popups (KGamePopupItem) ----
const showPopup = (kind, text) => {
  const dpr = window.devicePixelRatio || 1;
  const canvas = drawPopupCanvas(text, dpr);
  renderer.setPopup(canvas);
  const now = performance.now();
  app.popup = { kind, w: canvas.width / dpr, h: canvas.height / dpr, shownAt: now,
    hideAt: kind === 'message' ? now + MESSAGE_TIMEOUT_MS : Infinity };
  requestRender();
};
const hidePopup = (kind) => {
  if (app.popup && (!kind || app.popup.kind === kind)) app.popup = null;
  requestRender();
};
const getPopupRect = () => {
  const { width, height } = ui.canvas.getBoundingClientRect();
  return { x: width / 2 - app.popup.w / 2, y: height / 2 - app.popup.h / 2, width: app.popup.w, height: app.popup.h };
};
const getPopupAlpha = (now) => {
  const { shownAt, hideAt } = app.popup;
  return Math.max(0, Math.min(1, (now - shownAt) / POPUP_FADE_MS, (hideAt - now) / POPUP_FADE_MS));
};

// ---- rendering ----
function requestRender() {
  if (!app.frame) app.frame = requestAnimationFrame(drawFrame);
}

function drawFrame(now) {
  app.frame = 0;
  if (app.popup && now >= app.popup.hideAt) app.popup = null;
  const popup = app.popup ? { ...getPopupRect(), alpha: getPopupAlpha(now) } : null;
  renderer.render({
    dpr: window.devicePixelRatio || 1,
    field,
    layout: app.layout,
    isFieldVisible: !app.isPaused,
    popup,
  });
  if (app.popup && Number.isFinite(app.popup.hideAt)) requestRender(); // keep fading / timing out
  else if (popup && popup.alpha < 1) requestRender();
}

// KMinesScene::resizeScene (0x120020): layout + re-rasterize theme at the new size.
const resizeScene = async () => {
  const dpr = window.devicePixelRatio || 1;
  const { width, height } = ui.view.getBoundingClientRect();
  ui.canvas.width = Math.max(1, Math.round(width * dpr));
  ui.canvas.height = Math.max(1, Math.round(height * dpr));
  app.layout = getFieldLayout(width, height, field.rows, field.cols);
  requestRender();
  if (!app.theme) return;

  const sheetKey = `${app.themeId}|${app.layout.cellSize}|${ui.canvas.width}x${ui.canvas.height}|${dpr}`;
  if (sheetKey === app.sheetKey) return;
  app.sheetKey = sheetKey;
  const token = ++app.renderToken;
  const cellPx = Math.round(app.layout.cellSize * dpr);
  const [background, sheet] = await Promise.all([
    app.theme.renderSheet(['mainWidget'], ui.canvas.width, ui.canvas.height),
    app.theme.renderSheet(ALL_SPRITES.filter((k) => k !== 'mainWidget'), cellPx, cellPx),
  ]);
  if (token !== app.renderToken) return; // a newer resize won
  renderer.setBackground(background.canvas);
  renderer.setSpriteSheet(sheet);
  requestRender();
};

// Resize callers do not await; report rasterization failures in the view.
const requestResize = () => {
  resizeScene().catch((err) => { ui.loadStatus.textContent = `Theme rendering failed: ${err.message}`; });
};

const applyTheme = async (themeId) => {
  const theme = THEMES.find((t) => t.id === themeId) ?? THEMES[0];
  ui.loadStatus.textContent = `Loading theme ${theme.name}…`;
  try {
    app.theme = await loadTheme(`themes/${theme.file}`, ALL_SPRITES);
    app.themeId = theme.id;
    app.sheetKey = '';
    await resizeScene();
    ui.loadStatus.textContent = '';
  } catch (err) {
    ui.loadStatus.textContent = `Theme failed to load: ${err.message}`;
    throw err;
  }
};

// ---- game flow (KMinesMainWindow slots) ----
const getLevel = () => LEVELS.find((l) => l.id === app.levelId) ?? LEVELS[0];

const setPaused = (paused) => {
  app.isPaused = paused;
  ui.pause.setAttribute('aria-pressed', String(paused));
  // KMinesScene::setGamePaused (0x11f320)
  if (paused) showPopup('pause', 'Game is paused.');
  else hidePopup('pause');
  if (paused) pauseClock();
  else resumeClock();
};

// newGame (0x116660)
const newGame = () => {
  restartClock();
  if (app.isPaused) setPaused(false);
  pauseClock();
  ui.pause.disabled = true;
  const level = getLevel();
  const rows = level.id === 'custom' ? app.settings.customHeight : level.rows;
  const cols = level.id === 'custom' ? app.settings.customWidth : level.cols;
  const mines = level.id === 'custom' ? app.settings.customMines : level.mines;
  hidePopup('message');
  app.canScore = true; // initial value is not written by the binary (see report)
  field.initField(rows, cols, mines);
  ui.time.textContent = 'Time: 00:00';
  requestResize();
};

// onFirstClick (0x114110)
function onFirstClick() {
  ui.pause.disabled = false;
  resumeClock();
}

// KMinesScene::onGameOver (0x11c300) then KMinesMainWindow::onGameOver (0x117ee0)
function onGameOver(won) {
  showPopup('message', won ? 'Congratulations! You have won!' : 'You have lost.');
  pauseClock();
  ui.pause.disabled = true;
  endGrab();
  if (!won) {
    if (app.settings.allowKminesReset && !ui.resetDialog.open) ui.resetDialog.showModal();
    return;
  }
  if (app.canScore) addHighScore(getClockSeconds());
}

ui.resetDialog.addEventListener('close', () => {
  if (ui.resetDialog.returnValue !== 'reset') return;
  field.resetField();
  hidePopup('message');
  resumeClock();
  ui.pause.disabled = false;
  app.canScore = !app.settings.disableScoreOnReset;
});

// ---- high scores (KGameHighScoreDialog, LessIsMore by seconds) ----
const loadScores = () => loadJSON(STORAGE.scores, {});

const renderScores = (levelId, editableIndex = -1) => {
  const list = loadScores()[levelId] ?? [];
  ui.scoresLevel.value = levelId;
  ui.scoresBody.replaceChildren(...Array.from({ length: HIGHSCORE_LIMIT }, (_, i) => {
    const tr = document.createElement('tr');
    const entry = list[i];
    const cells = [String(i + 1), entry?.name ?? '', entry ? formatClock(entry.seconds) : ''];
    cells.forEach((text, col) => {
      const td = document.createElement('td');
      if (i === editableIndex && col === 1) {
        const input = document.createElement('input');
        input.value = text;
        input.id = 'score-name';
        input.maxLength = 32;
        td.append(input);
      } else {
        td.textContent = text;
      }
      tr.append(td);
    });
    if (i === editableIndex) tr.className = 'new';
    return tr;
  }));
};

const addHighScore = (seconds) => {
  const scores = loadScores();
  const list = scores[app.levelId] ?? [];
  const rank = list.findIndex((e) => seconds < e.seconds);
  const index = rank === -1 ? list.length : rank;
  if (index >= HIGHSCORE_LIMIT) return; // addScore() returned 0: no dialog
  list.splice(index, 0, { name: loadJSON(STORAGE.name, 'Player'), seconds });
  scores[app.levelId] = list.slice(0, HIGHSCORE_LIMIT);
  saveJSON(STORAGE.scores, scores);
  renderScores(app.levelId, index);
  ui.scoresDialog.dataset.editIndex = String(index);
  ui.scoresDialog.showModal();
  $('score-name')?.select();
};

ui.scoresDialog.addEventListener('close', () => {
  const index = Number(ui.scoresDialog.dataset.editIndex ?? -1);
  const input = $('score-name');
  if (index >= 0 && input) {
    const name = input.value.trim() || 'Player';
    const scores = loadScores();
    if (scores[app.levelId]?.[index]) scores[app.levelId][index].name = name;
    saveJSON(STORAGE.scores, scores);
    saveJSON(STORAGE.name, name);
  }
  delete ui.scoresDialog.dataset.editIndex;
});
ui.scoresLevel.addEventListener('change', () => renderScores(ui.scoresLevel.value));

const showHighscores = () => {
  delete ui.scoresDialog.dataset.editIndex;
  renderScores(app.levelId);
  ui.scoresDialog.showModal();
};

// ---- settings dialog (KConfigDialog: General / Theme / Custom Game) ----
const settingsInputs = () => ui.settingsForm.elements;

const fillSettingsForm = (s) => {
  const el = settingsInputs();
  for (const key of ['useQuestionMarks', 'allowKminesReset', 'disableScoreOnReset', 'exploreWithLeftClickOnNumberCells']) {
    el[key].checked = s[key];
  }
  el.placeFlagOn.value = String(s.placeFlagOn);
  el.customWidth.value = s.customWidth;
  el.customHeight.value = s.customHeight;
  el.customMines.value = s.customMines;
  updateMaxMines();
  const radio = ui.themeList.querySelector(`input[value="${s.theme}"]`);
  if (radio) radio.checked = true;
};

// CustomGameConfig::updateMaxMines (0x113fc0)
function updateMaxMines() {
  const el = settingsInputs();
  const max = Number(el.customWidth.value) * Number(el.customHeight.value) - 10;
  el.customMines.max = String(max > 0 ? max : 1);
}

const readSettingsForm = () => {
  const el = settingsInputs();
  const clamp = (v, lo, hi) => Math.min(hi, Math.max(lo, Math.trunc(Number(v)) || lo));
  const customWidth = clamp(el.customWidth.value, CUSTOM_LIMITS.minSize, CUSTOM_LIMITS.maxSize);
  const customHeight = clamp(el.customHeight.value, CUSTOM_LIMITS.minSize, CUSTOM_LIMITS.maxSize);
  return {
    ...app.settings,
    useQuestionMarks: el.useQuestionMarks.checked,
    allowKminesReset: el.allowKminesReset.checked,
    disableScoreOnReset: el.disableScoreOnReset.checked,
    exploreWithLeftClickOnNumberCells: el.exploreWithLeftClickOnNumberCells.checked,
    placeFlagOn: Number(el.placeFlagOn.value),
    customWidth,
    customHeight,
    customMines: clamp(el.customMines.value, CUSTOM_LIMITS.minMines, Math.max(1, customWidth * customHeight - 10)),
  };
};

// Settings change only re-renders (loadSettings 0x1144f0); custom size applies on next game.
const applySettings = () => {
  app.settings = readSettingsForm();
  saveJSON(STORAGE.settings, app.settings);
  requestResize();
};

ui.themeList.replaceChildren(...THEMES.map((t) => {
  const li = document.createElement('li');
  li.innerHTML = `<label><input type="radio" name="theme" value="${t.id}"><img src="themes/${t.preview}" alt=""> ${t.name}</label>`;
  return li;
}));
ui.themeList.addEventListener('change', (e) => {
  // Theme selection applies immediately, as in KGameThemeSelector.
  app.settings = { ...app.settings, theme: e.target.value };
  saveJSON(STORAGE.settings, app.settings);
  applyTheme(e.target.value);
});

ui.settingsForm.addEventListener('input', (e) => {
  if (e.target.name === 'customWidth' || e.target.name === 'customHeight') updateMaxMines();
});
ui.settingsDialog.querySelectorAll('[role="tab"]').forEach((tab) => {
  tab.addEventListener('click', () => {
    ui.settingsDialog.querySelectorAll('[role="tab"]').forEach((t) => t.setAttribute('aria-selected', String(t === tab)));
    ui.settingsDialog.querySelectorAll('[data-page]').forEach((p) => { p.hidden = p.dataset.page !== tab.dataset.tab; });
  });
});
$('settings-apply').addEventListener('click', () => {
  if (ui.settingsForm.reportValidity()) applySettings();
});
$('settings-defaults').addEventListener('click', () => fillSettingsForm({ ...DEFAULT_SETTINGS, theme: app.settings.theme }));
ui.settingsDialog.addEventListener('close', () => {
  if (ui.settingsDialog.returnValue === 'ok') applySettings();
});

const configureSettings = () => {
  fillSettingsForm(app.settings);
  ui.settingsDialog.showModal();
};

// ---- input: mouse (QGraphicsScene grab semantics) and touch ----
const getLocalPoint = (e) => {
  const rect = ui.canvas.getBoundingClientRect();
  return [e.clientX - rect.left, e.clientY - rect.top];
};

function endGrab() {
  app.isGrabbing = false;
}

ui.canvas.addEventListener('mousedown', (e) => {
  e.preventDefault();
  const [px, py] = getLocalPoint(e);
  if (app.popup?.kind === 'message' && isInsideRect(getPopupRect(), px, py)) {
    hidePopup('message'); // KGamePopupItem hideOnMouseClick
    return;
  }
  if (app.isPaused) return; // hidden item gets no events
  if (!app.isGrabbing) {
    if (!isInsideRect(app.layout, px, py)) return;
    app.isGrabbing = true;
  }
  const [row, col] = getCellAt(app.layout, px, py);
  field.mousePress(row, col, QT_BUTTON[e.button], e.buttons);
});

window.addEventListener('mousemove', (e) => {
  if (!app.isGrabbing) return;
  const [px, py] = getLocalPoint(e);
  const [row, col] = getCellAt(app.layout, px, py);
  field.mouseMove(row, col, e.buttons);
});

window.addEventListener('mouseup', (e) => {
  if (!app.isGrabbing) return;
  const [px, py] = getLocalPoint(e);
  const [row, col] = getCellAt(app.layout, px, py);
  if (e.buttons === 0) endGrab();
  field.mouseRelease(row, col, QT_BUTTON[e.button], e.buttons);
});

ui.canvas.addEventListener('contextmenu', (e) => e.preventDefault());

const getTouchCell = (touch) => getCellAt(app.layout, ...getLocalPoint(touch));

ui.canvas.addEventListener('touchstart', (e) => {
  e.preventDefault();
  if (app.isPaused || e.touches.length !== 1) return;
  field.touchBegin(...getTouchCell(e.changedTouches[0]));
}, { passive: false });
ui.canvas.addEventListener('touchmove', (e) => {
  e.preventDefault();
  const [row, col] = getTouchCell(e.changedTouches[0]);
  if (!field.isInside(row, col)) field.touchCancel();
}, { passive: false });
ui.canvas.addEventListener('touchend', (e) => {
  e.preventDefault();
  if (app.isPaused || e.touches.length !== 0) return;
  field.touchEnd(...getTouchCell(e.changedTouches[0]));
}, { passive: false });
ui.canvas.addEventListener('touchcancel', () => field.touchCancel());

// ---- toolbar, difficulty, shortcuts ----
ui.difficulty.replaceChildren(...LEVELS.map((l) => new Option(l.label, l.id)));
ui.scoresLevel.replaceChildren(...LEVELS.map((l) => new Option(l.label, l.id)));
ui.difficulty.value = getLevel().id;
ui.difficulty.addEventListener('change', () => {
  app.levelId = ui.difficulty.value; // currentLevelChanged -> newGame
  saveJSON(STORAGE.level, app.levelId);
  newGame();
});
ui.newGame.addEventListener('click', newGame);
ui.pause.addEventListener('click', () => setPaused(!app.isPaused));
ui.highscores.addEventListener('click', showHighscores);
ui.settings.addEventListener('click', configureSettings);

window.addEventListener('keydown', (e) => {
  if (document.querySelector('dialog[open]') || e.target instanceof HTMLInputElement) return;
  const key = e.key.toLowerCase();
  if ((e.ctrlKey || e.metaKey) && key === 'n') { e.preventDefault(); newGame(); }
  else if ((e.ctrlKey || e.metaKey) && key === 'h') { e.preventDefault(); showHighscores(); }
  else if (!e.ctrlKey && !e.metaKey && !e.altKey && key === 'p' && !ui.pause.disabled) setPaused(!app.isPaused);
});

new ResizeObserver(requestResize).observe(ui.view);

newGame();
applyTheme(app.settings.theme).catch((err) => console.warn(err));
