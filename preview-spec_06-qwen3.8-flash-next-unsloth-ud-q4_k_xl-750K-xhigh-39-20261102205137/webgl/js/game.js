// App controller: the port's stand-in for KMinesMainWindow + KMinesScene wiring.
//
// Wires the WebGL canvas, the interaction rules recovered from the binary
// (FUN_0011f660 press, FUN_0011eb40 move, FUN_0011fa60 release, FUN_0011ee50
// touch), the difficulty levels, the clock, the status bar labels, the pause and
// reset popups and the high-score table.

import { MineField, CellState } from './field.js';
import { Scene } from './scene.js';
import { Renderer } from './renderer.js';
import { Atlas } from './atlas.js';
import { buildTheme, themeClearColor, themeLabel, themePalette, THEME_NAMES } from './theme.js';
import {
  DIFFICULTY, DIFFICULTY_ORDER, Mouse, addScore, clampSettings, customLimits, formatTime,
  loadScores, loadSettings, saveSettings,
} from './storage.js';

const SPRITE_PX = 64;
const LONG_PRESS_MS = 1000;         // the 1000 ms rule in MineFieldItem::sceneEvent

const difficultyOf = (id) => DIFFICULTY_ORDER.find((level) => level.id === id) || DIFFICULTY.Easy;

const fieldSizeFor = (settings, level) => (level === DIFFICULTY.Custom
  ? { height: settings.customHeight, width: settings.customWidth, mines: settings.customMines }
  : { height: level.height, width: level.width, mines: level.mines });

class Clock {
  constructor(onTick) {
    this.onTick = onTick;
    this.elapsedSeconds = 0;
    this.startedAt = 0;
    this.isRunning = false;
    this.hasStarted = false;
  }

  restart() {
    this.isRunning = false;
    this.hasStarted = false;
    this.elapsedSeconds = 0;
    this.onTick(0);
  }

  resume() {
    if (this.isRunning) return;
    this.isRunning = true;
    this.hasStarted = true;
    this.startedAt = performance.now() - this.elapsedSeconds * 1000;
  }

  pause() {
    if (!this.isRunning) return;
    this.isRunning = false;
    this.elapsedSeconds = Math.floor((performance.now() - this.startedAt) / 1000);
  }

  tick() {
    if (!this.isRunning) return;
    const seconds = Math.min(3599, Math.floor((performance.now() - this.startedAt) / 1000));
    if (seconds !== this.elapsedSeconds) {
      this.elapsedSeconds = seconds;
      this.onTick(seconds);
    }
  }

  seconds() {
    return this.elapsedSeconds;
  }
}

export class Game {
  constructor(root) {
    this.root = root;
    this.canvas = root.querySelector('#board');
    this.settings = loadSettings();
    this.canRecordScore = true;
    this.isPaused = false;
    this.lastGameWon = false;
    this.pressedCell = null;
    this.exploreCell = null;
    this.leftButtonHeld = false;
    this.rightButtonHeld = false;
    this.isTouchGesture = false;
    this.touchStartedAt = 0;
    this.atlases = new Map();

    this.renderer = new Renderer(this.canvas);
    this.field = new MineField(this.settings);
    this.scene = new Scene(this.field);
    this.scene.setRenderer(this.renderer);
    this.dialogs = new Dialogs(this);

    this.field.on('flaggedMinesCountChanged', (flags) => this.updateMinesLabel(flags));
    this.field.on('firstClickDone', () => this.onFirstClick());
    this.field.on('gameOver', (won) => this.onGameOver(won));

    this.clock = new Clock((seconds) => this.updateTimeLabel(seconds));

    this.bindHud();
    this.bindControls();
    this.bindPointer();
    this.bindKeyboard();
    this.applyTheme(this.settings.theme);
    this.startGame(difficultyOf(this.settings.difficultyId));
    this.resize();
    window.addEventListener('resize', () => this.resize());
    requestAnimationFrame(() => this.frame());
  }

  // ---- setup ----------------------------------------------------------

  atlasFor(themeName) {
    if (!this.atlases.has(themeName)) {
      const atlas = new Atlas(buildTheme(themeName), SPRITE_PX);
      this.atlases.set(themeName, { atlas, texture: this.renderer.createTexture(atlas.canvas) });
    }
    return this.atlases.get(themeName);
  }

  applyTheme(themeName) {
    const name = THEME_NAMES.includes(themeName) ? themeName : 'default';
    this.settings.theme = name;
    const { atlas, texture } = this.atlasFor(name);
    this.scene.setAtlas(atlas, texture, window.devicePixelRatio || 1);
    this.scene.setClearColor(themeClearColor(name));
    const palette = themePalette(name);
    this.root.style.setProperty('--panel', palette.panel[0]);
    this.root.style.setProperty('--canvas-bg', palette.canvasBg);
    saveSettings(this.settings);
  }

  resize() {
    const rect = this.canvas.parentElement.getBoundingClientRect();
    const dpr = window.devicePixelRatio || 1;
    this.scene.devicePixelRatio = dpr;
    this.renderer.resize(rect.width, rect.height, dpr);
    this.scene.updateScale(rect.width, rect.height);
  }

  frame() {
    this.clock.tick();
    this.scene.render();
    requestAnimationFrame(() => this.frame());
  }

  // ---- HUD ------------------------------------------------------------

  bindHud() {
    this.minesLabel = this.root.querySelector('#mines-label');
    this.timeLabel = this.root.querySelector('#time-label');
    this.difficultySelect = this.root.querySelector('#difficulty');
    this.pauseButton = this.root.querySelector('#pause-action');
    this.difficultySelect.innerHTML = DIFFICULTY_ORDER
      .map((level) => `<option value="${level.id}">${level.name}</option>`).join('');
  }

  bindControls() {
    this.root.querySelector('#new-action').addEventListener('click', () => this.newGame());
    this.pauseButton.addEventListener('click', () => this.togglePaused());
    this.root.querySelector('#highscores-action').addEventListener('click', () => this.showScores());
    this.root.querySelector('#settings-action').addEventListener('click', () => this.dialogs.open('settings'));
    this.root.querySelector('#help-action').addEventListener('click', () => this.dialogs.open('help'));
    this.difficultySelect.value = String(this.settings.difficultyId);
    this.difficultySelect.addEventListener('change', () => {
      this.settings.difficultyId = Number(this.difficultySelect.value);
      saveSettings(this.settings);
      this.newGame();
    });
  }

  bindKeyboard() {
    window.addEventListener('keydown', (event) => {
      if (event.target instanceof HTMLInputElement || event.target instanceof HTMLSelectElement) return;
      const key = event.key.toLowerCase();
      if (event.ctrlKey && key === 'n') { event.preventDefault(); this.newGame(); }
      else if (event.ctrlKey && key === 'h') { event.preventDefault(); this.showScores(); }
      else if (event.ctrlKey && key === 'q') { event.preventDefault(); this.quit(); }
      else if (key === 'p' && !event.ctrlKey) { event.preventDefault(); this.togglePaused(); }
      else if (key === 'f1') { event.preventDefault(); this.dialogs.open('help'); }
      else if (key === 'escape') { this.dialogs.close(); }
    });
  }

  updateMinesLabel(flagsPlaced) {
    this.minesLabel.textContent = `Mines: ${flagsPlaced}/${this.field.minesTotal}`;
  }

  updateTimeLabel(seconds) {
    this.timeLabel.textContent = `Time: ${formatTime(seconds)}`;
  }

  // ---- game flow (KMinesMainWindow slots) ------------------------------

  startGame(level) {
    const { height, width, mines } = fieldSizeFor(this.settings, level);
    this.settings.difficultyId = level.id;
    this.difficultySelect.value = String(level.id);
    this.canRecordScore = true;
    this.lastGameWon = false;
    this.isPaused = false;
    this.pressedCell = null;
    this.exploreCell = null;
    this.scene.showPaused(false);
    this.pauseButton.setAttribute('aria-pressed', 'false');
    this.pauseButton.disabled = false;
    this.field.setFieldSize(height, width, mines);
    // MineFieldItem::updateFieldItems() ends by recomputing the scale, so the new
    // board size is honoured without waiting for a window resize.
    this.resize();
    this.clock.restart();
    saveSettings(this.settings);
  }

  newGame() {
    this.dialogs.close();
    this.startGame(difficultyOf(this.settings.difficultyId));
  }

  onFirstClick() {
    if (!this.isPaused) this.clock.resume();
  }

  onGameOver(won) {
    this.clock.pause();
    this.lastGameWon = won;
    this.pauseButton.disabled = true;
    this.forgetPressedCells();
    if (won) {
      this.scene.showMessage('Congratulations! You have won!');
      if (this.canRecordScore) {
        const seconds = this.clock.seconds();
        const isNewEntry = addScore(difficultyOf(this.settings.difficultyId).id, seconds);
        this.showScores({ highlightSeconds: seconds, isNew: isNewEntry });
      }
      return;
    }
    this.scene.showMessage('You have lost.');
    if (this.settings.allowKminesReset) this.askReset();
  }

  forgetPressedCells() {
    for (const cell of this.field.cells) {
      if (cell.state === CellState.Pressed) cell.state = CellState.Covered;
    }
    this.pressedCell = null;
    this.exploreCell = null;
  }

  // KMessageBox::questionTwoActions("Reset the Game?", Reset / Cancel)
  askReset() {
    if (!this.settings.allowKminesReset || this.lastGameWon) return;
    this.dialogs.openConfirm({
      title: 'Reset the Game?',
      text: 'Clears the board and starts a new game. Mines already found stay cleared.',
      acceptLabel: 'Reset',
      onAccept: () => {
        this.canRecordScore = !this.settings.disableScoreOnReset;
        this.field.resetField();
        this.pauseButton.disabled = false;
        this.clock.restart();
        this.clock.resume();
      },
    });
  }

  togglePaused() {
    if (this.field.isGameOver) return;
    this.isPaused = !this.isPaused;
    this.pauseButton.setAttribute('aria-pressed', String(this.isPaused));
    this.scene.showPaused(this.isPaused);
    if (this.isPaused) {
      this.clock.pause();
      this.forgetPressedCells();
    } else if (this.clock.hasStarted) {
      this.clock.resume();
    }
  }

  showScores(info) {
    this.dialogs.open('scores', info);
  }

  quit() {
    this.field.isGameOver = true;
    this.clock.pause();
    this.pauseButton.disabled = true;
    this.scene.showMessage('Thanks for playing!');
  }

  // ---- pointer / touch (MineFieldItem mouse handlers) -------------------

  cellFromEvent(event) {
    const rect = this.canvas.getBoundingClientRect();
    const { row, col } = this.scene.cellAt(event.clientX - rect.left, event.clientY - rect.top);
    if (row < 0 || col < 0 || row >= this.field.height || col >= this.field.width) return null;
    return { row, col };
  }

  bindPointer() {
    this.canvas.addEventListener('contextmenu', (event) => event.preventDefault());

    this.canvas.addEventListener('pointerdown', (event) => {
      const at = this.cellFromEvent(event);
      this.leftButtonHeld = (event.buttons & 1) !== 0;
      this.rightButtonHeld = (event.buttons & 2) !== 0;
      if (!at || this.field.isGameOver || this.isPaused) {
        if (event.button === 1) event.preventDefault();
        return;
      }
      if (event.button === 1) event.preventDefault();       // no middle-click autoscroll
      try {
        this.canvas.setPointerCapture?.(event.pointerId);
      } catch (error) {
        // synthetic pointer events (tests, some embedded browsers) have no capture
      }
      this.isTouchGesture = event.pointerType === 'touch';
      if (this.isTouchGesture) this.touchStartedAt = performance.now();

      const cell = this.field.getCell(at.row, at.col);
      const wantsExplore = event.button === 1 ||
        (event.button === 0 && this.leftButtonHeld && this.rightButtonHeld) ||
        (event.button === 0 && this.settings.exploreWithLeftClickOnNumberCells &&
          this.field.isShownCell(cell));
      if (wantsExplore) {
        this.exploreCell = cell;
        this.field.previewAround(at.row, at.col);
        return;
      }
      if (event.button === 0) {
        this.pressedCell = this.field.pressCell(at.row, at.col);
      } else if (event.button === 2 && this.settings.placeFlagOn === Mouse.Press) {
        this.field.switchCellState(cell);                   // "Place flag when pressed"
      }
    });

    this.canvas.addEventListener('pointermove', (event) => {
      const at = this.cellFromEvent(event);
      if (!at || this.isTouchGesture || this.field.isGameOver || this.isPaused) return;

      if (this.exploreCell) {                               // drag the explore preview
        if (this.exploreCell.row !== at.row || this.exploreCell.col !== at.col) {
          this.field.unpressAround(this.exploreCell.row, this.exploreCell.col);
          this.field.previewAround(at.row, at.col);
          this.exploreCell = this.field.getCell(at.row, at.col);
        }
        return;
      }
      if (!this.leftButtonHeld || !this.pressedCell) return;
      if (this.pressedCell.row === at.row && this.pressedCell.col === at.col) return;
      if (this.pressedCell.state === CellState.Pressed) {
        this.field.unpressCell(this.pressedCell.row, this.pressedCell.col);
      }
      this.pressedCell = this.field.pressCell(at.row, at.col) || this.pressedCell;
    });

    const release = (event) => {
      const at = this.cellFromEvent(event);
      const cell = at ? this.field.getCell(at.row, at.col) : null;

      if (this.exploreCell) {
        const anchor = this.exploreCell;
        this.field.unpressAround(anchor.row, anchor.col);
        if (cell && !this.field.isGameOver) this.field.explore(cell.row, cell.col);
        this.exploreCell = null;
      } else if (cell && !this.field.isGameOver && !this.isPaused) {
        if (event.button === 0) {
          const isLongPress = performance.now() - this.touchStartedAt >= LONG_PRESS_MS;
          if (this.isTouchGesture && !isLongPress) {
            this.field.unpressCell(cell.row, cell.col);
            this.field.switchCellState(cell);               // touch tap flags
          } else {
            this.field.dig(cell.row, cell.col);             // click / long press digs
          }
        } else if (event.button === 2 && this.settings.placeFlagOn === Mouse.Release) {
          this.field.switchCellState(cell);                 // "Place flag when released"
        }
      }
      if (this.pressedCell && this.pressedCell.state === CellState.Pressed) {
        this.field.unpressCell(this.pressedCell.row, this.pressedCell.col);
      }
      this.pressedCell = null;
      this.isTouchGesture = false;
      this.leftButtonHeld = (event.buttons & 1) !== 0;
      this.rightButtonHeld = (event.buttons & 2) !== 0;
    };

    this.canvas.addEventListener('pointerup', release);
    this.canvas.addEventListener('pointercancel', () => this.forgetPressedCells());
  }

  // ---- used by Dialogs --------------------------------------------------

  commitSettings(settings) {
    this.settings = clampSettings(settings);
    this.field.settings = this.settings;
    saveSettings(this.settings);
  }

  currentScores() {
    return loadScores()[difficultyOf(this.settings.difficultyId).id] || [];
  }

  currentLevel() {
    return difficultyOf(this.settings.difficultyId);
  }

  useCustomDifficulty() {
    this.settings.difficultyId = DIFFICULTY.Custom.id;
    this.difficultySelect.value = String(DIFFICULTY.Custom.id);
    saveSettings(this.settings);
    this.startGame(DIFFICULTY.Custom);
  }
}

class Dialogs {
  constructor(game) {
    this.game = game;
    this.backdrop = document.getElementById('dialog-backdrop');
    this.title = document.getElementById('dialog-title');
    this.body = document.getElementById('dialog-body');
    this.acceptButton = document.getElementById('dialog-accept');
    this.closeButton = document.getElementById('dialog-close');
    this.onAccept = null;
    this.closeButton.addEventListener('click', () => this.close());
    this.acceptButton.addEventListener('click', () => {
      const handler = this.onAccept;
      this.close();
      if (handler) handler();
    });
    this.backdrop.addEventListener('click', (event) => {
      if (event.target === this.backdrop) this.close();
    });
  }

  close() {
    this.backdrop.hidden = true;
    this.body.replaceChildren();
    this.onAccept = null;
    this.acceptButton.hidden = true;
  }

  show(title, html, acceptLabel, onAccept) {
    this.title.textContent = title;
    this.body.innerHTML = html;
    this.onAccept = onAccept || null;
    this.acceptButton.hidden = !acceptLabel;
    this.acceptButton.textContent = acceptLabel || 'OK';
    this.backdrop.hidden = false;
  }

  open(kind, info) {
    if (kind === 'settings') {
      this.show('Configure KMines', this.settingsHtml(), 'Apply size and start Custom game',
        () => this.applySettings());
      this.wireSettings();
      return;
    }
    if (kind === 'scores') {
      this.show('High Scores', this.scoresHtml(info));
      return;
    }
    this.show('KMines Handbook', this.helpHtml());
  }

  openConfirm({ title, text, acceptLabel, onAccept }) {
    if (!this.game.settings.allowKminesReset || this.game.lastGameWon) return;
    this.show(title, `<p>${text}</p>`, acceptLabel, onAccept);
  }

  settingsHtml() {
    const s = this.game.settings;
    const checked = (value) => (value ? 'checked' : '');
    const selected = (value, current) => (value === current ? 'selected' : '');
    return `
      <div class="tabs">
        <section>
          <h3>General</h3>
          <label><input type="checkbox" id="opt-question" ${checked(s.useQuestionMarks)}> Use '?' marks</label>
          <label><input type="checkbox" id="opt-reset" ${checked(s.allowKminesReset)}> Allow KMines reset</label>
          <label><input type="checkbox" id="opt-noscore" ${checked(s.disableScoreOnReset)}> Disable score on reset</label>
          <label>
            <input type="checkbox" id="opt-explore" ${checked(s.exploreWithLeftClickOnNumberCells)}>
            Explore with left click on number cells
          </label>
          <label>Place flag when mouse button is:
            <select id="opt-flag-on">
              <option value="0" ${selected(Mouse.Release, s.placeFlagOn)}>Released</option>
              <option value="1" ${selected(Mouse.Press, s.placeFlagOn)}>Pressed</option>
            </select>
          </label>
        </section>
        <section>
          <h3>Theme</h3>
          <select id="opt-theme">
            ${THEME_NAMES.map((name) =>
              `<option value="${name}" ${selected(name, s.theme)}>${themeLabel(name)}</option>`).join('')}
          </select>
        </section>
        <section>
          <h3>Custom Game</h3>
          <label>Width:
            <input type="number" id="opt-width" min="${customLimits.width.min}"
              max="${customLimits.width.max}" value="${s.customWidth}"></label>
          <label>Height:
            <input type="number" id="opt-height" min="${customLimits.height.min}"
              max="${customLimits.height.max}" value="${s.customHeight}"></label>
          <label>Mines:
            <input type="number" id="opt-mines" min="${customLimits.minesMin}"
              max="${customLimits.maxMines(s.customWidth, s.customHeight)}" value="${s.customMines}"></label>
        </section>
      </div>
      <p class="hint">Options apply as soon as you change them; OK additionally starts a Custom
      game with the size above. The mine maximum is width x height minus 10, like KMines.</p>`;
  }

  readSettingsForm() {
    const q = (id) => this.body.querySelector(id);
    return {
      ...this.game.settings,
      useQuestionMarks: q('#opt-question').checked,
      allowKminesReset: q('#opt-reset').checked,
      disableScoreOnReset: q('#opt-noscore').checked,
      exploreWithLeftClickOnNumberCells: q('#opt-explore').checked,
      placeFlagOn: Number(q('#opt-flag-on').value),
      customWidth: Number(q('#opt-width').value),
      customHeight: Number(q('#opt-height').value),
      customMines: Number(q('#opt-mines').value),
    };
  }

  applySettings() {
    const widthInput = this.body.querySelector('#opt-width');
    const heightInput = this.body.querySelector('#opt-height');
    const minesInput = this.body.querySelector('#opt-mines');
    const max = customLimits.maxMines(Number(widthInput.value), Number(heightInput.value));
    if (Number(minesInput.value) > max) minesInput.value = String(max);
    this.game.commitSettings(this.readSettingsForm());
    this.game.useCustomDifficulty();
  }

  wireSettings() {
    const game = this.game;
    ['#opt-question', '#opt-reset', '#opt-noscore', '#opt-explore', '#opt-flag-on'].forEach((id) => {
      this.body.querySelector(id).addEventListener('change', () => game.commitSettings(this.readSettingsForm()));
    });
    this.body.querySelector('#opt-theme').addEventListener('change', (event) => {
      game.applyTheme(event.target.value);
    });

    const widthInput = this.body.querySelector('#opt-width');
    const heightInput = this.body.querySelector('#opt-height');
    const minesInput = this.body.querySelector('#opt-mines');
    const updateMaxMines = () => {                 // CustomGameConfig::updateMaxMines
      const max = customLimits.maxMines(Number(widthInput.value), Number(heightInput.value));
      minesInput.max = String(max);
      if (Number(minesInput.value) > max) minesInput.value = String(max);
    };
    [widthInput, heightInput].forEach((input) => input.addEventListener('input', updateMaxMines));
    minesInput.addEventListener('change', () => game.commitSettings(this.readSettingsForm()));
  }

  scoresHtml(info) {
    const level = this.game.currentLevel();
    const scores = this.game.currentScores();
    const rows = scores.length
      ? scores.map((entry, index) => `
          <tr class="${info && entry.seconds === info.highlightSeconds ? 'new-score' : ''}">
            <td>${index + 1}</td><td>${formatTime(entry.seconds)}</td><td>${entry.date}</td>
          </tr>`).join('')
      : '<tr><td colspan="3">No scores yet for this difficulty.</td></tr>';
    return `
      ${info?.isNew ? '<p class="new-score-banner">New best time!</p>' : ''}
      <table class="scores">
        <thead><tr><th>#</th><th>Score (${level.name})</th><th>Date</th></tr></thead>
        <tbody>${rows}</tbody>
      </table>`;
  }

  helpHtml() {
    return `
      <ul>
        <li>Left click digs a cell. The first click never hits a mine: its 3x3 area is kept clear.</li>
        <li>Right click cycles covered -&gt; flag -&gt; question mark -&gt; covered.</li>
        <li>Middle click, or left+right together, on a number cell digs its neighbours when the
          flags around it match the number.</li>
        <li>Hold the left button and drag to line up cells, release to dig.</li>
        <li>Touch: tap flags a cell, press and hold (1 s) digs it.</li>
        <li>Shortcuts: Ctrl+N new game, P pause, Ctrl+H high scores, Ctrl+Q quit, F1 this help.</li>
        <li>Pausing hides the board, like KMines does, so you cannot think against the clock.</li>
      </ul>`;
  }
}
