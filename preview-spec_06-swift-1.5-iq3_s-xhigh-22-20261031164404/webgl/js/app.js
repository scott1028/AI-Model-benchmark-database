/* app.js — reconstruction of KMinesScene + KMinesMainWindow + KMinesView.
 *
 * Signals/slots reproduced here (names taken from the Qt meta-object data of
 * /usr/games/kmines):
 *   MineFieldItem: firstClickDone, gameOver(won), flaggedMinesCountChanged
 *   KMinesScene:   minesCountChanged, gameOver, firstClickDone
 *   KMinesMainWindow: newGame, onGameOver, onFirstClick, onMinesCountChanged,
 *                     advanceTime, pauseGame, paused, showHighscores,
 *                     configureSettings, loadSettings
 */
(function () {
  'use strict';

  // Difficulty levels: the standard range (30..50) of KGameDifficulty plus the
  // "Custom" level (value 1000, key "Custom"; evidence: FUN_0011a610 ctor and
  // kminesrc "KgDifficulty/Level=Easy").
  // Board sizes per level: from the installed manual
  // /usr/share/doc/HTML/en/kmines/index.docbook (Easy 9x9/10, Medium 16x16/40,
  // Hard 30x16/99).
  const LEVELS = [
    { value: 30, key: 'Easy', title: 'Easy', width: 9, height: 9, mines: 10 },
    { value: 40, key: 'Medium', title: 'Medium', width: 16, height: 16, mines: 40 },
    { value: 50, key: 'Hard', title: 'Hard', width: 30, height: 16, mines: 99 },
    { value: 1000, key: 'Custom', title: 'Custom', width: 10, height: 10, mines: 20 }
  ];

  const RENDER_SIZE = 40;         // KGameRendererClient::renderSize of a cell
  const BORDER = RENDER_SIZE / 2; // bevelled frame thickness

  const DEFAULTS = {
    // defaults proven from the ItemBool ctor arguments (FUN_00115120..):
    UseQuestionMarks: false,
    AllowKminesReset: true,
    DisableScoreOnReset: true,
    ExploreWithLeftClickOnNumberCells: true,
    PlaceFlagOn: 0,            // 0 = MouseRelease, 1 = MousePress
    CustomWidth: 10,
    CustomHeight: 10,
    CustomMines: 20,
    Level: 30
  };

  const $ = (id) => document.getElementById(id);

  class Settings {
    constructor() {
      this.values = Object.assign({}, DEFAULTS);
      this.load();
    }
    // native: Settings (@0x1e2cc group "GeneralOptsConfig", @0x1e699 group
    // "CustomGameConfig"); key names and defaults come from the .ui/kcfg names.
    load() {
      try {
        const raw = localStorage.getItem('kmines-webgl/settings');
        if (raw) Object.assign(this.values, JSON.parse(raw));
      } catch (e) { /* first run */ }
      this.clampCustom();
    }
    save() {
      try { localStorage.setItem('kmines-webgl/settings', JSON.stringify(this.values)); }
      catch (e) { /* storage disabled */ }
    }
    clampCustom() {
      const min = 5, max = 50;             // ItemInt setMinValue(5) / setMaxValue(50)
      this.values.CustomWidth = Math.min(max, Math.max(min, this.values.CustomWidth | 0));
      this.values.CustomHeight = Math.min(max, Math.max(min, this.values.CustomHeight | 0));
      // Settings::updateMaxMines -> mines cannot exceed the field size
      const maxMines = Math.max(1, this.values.CustomWidth * this.values.CustomHeight - 1);
      this.values.CustomMines = Math.min(maxMines, Math.max(1, this.values.CustomMines | 0));
    }
    maxCustomMines() {
      return Math.max(1, this.values.CustomWidth * this.values.CustomHeight - 1);
    }
  }

  class HighScores {
    // native: KGameHighScoreDialog::addScore / initFromDifficulty, per level.
    add(levelKey, seconds) {
      const k = 'kmines-webgl/scores/' + levelKey;
      let list = [];
      try { list = JSON.parse(localStorage.getItem(k) || '[]'); } catch (e) { list = []; }
      list.push({ time: seconds, date: new Date().toISOString().slice(0, 10) });
      list.sort((a, b) => a.time - b.time);   // best = shortest time (docbook)
      list = list.slice(0, 10);
      try { localStorage.setItem(k, JSON.stringify(list)); } catch (e) { /* ignore */ }
      return list;
    }
    list(levelKey) {
      try { return JSON.parse(localStorage.getItem('kmines-webgl/scores/' + levelKey) || '[]'); }
      catch (e) { return []; }
    }
  }

  class KMinesScene {
    constructor(settings) {
      this.settings = settings;
      this.field = new MineFieldItem();
      this.field.useQuestionMarks = settings.values.UseQuestionMarks;
      // native KGameClock member @scene+0x60 (0x11b764). Time is set via
      // KGameClock slots: setTime(max(mines,499)) in updateMaxMines (0x11b690)
      // and setTime(5) on the first click (0x1ad46); status = timeString()
      // ("Time: %1"), score = seconds(). Model: a countdown clock under time
      // pressure (docbook), score = remaining seconds; start on first click,
      // stop on gameOver/newField.
      this.clock = { seconds: 0, running: false, timer: null };
      this.paused = false;
      this.level = this.levelByValue(settings.values.Level);
      this.scoreDisabledByReset = false;
      this.listeners = {};
      this.field.on('firstClickDone', () => this.emit('firstClickDone'));
      this.field.on('gameOver', (won) => this.emit('gameOver', won));
      this.field.on('flaggedMinesCountChanged', (n) => {
      if (n > (this.lastFlagCount || 0)) this.flagPenalty();   // setPenalty(5)
      this.lastFlagCount = n;
      this.emit('minesCountChanged', n);
    });
    }

    levelByValue(v) {
      return LEVELS.find((l) => l.value === v) || LEVELS[0];
    }

    // native: KMinesScene::newGame -> MineFieldItem::newField
    newGame() {
      const s = this.settings.values;
      this.level = this.levelByValue(s.Level);
      let w = this.level.width, h = this.level.height, m = this.level.mines;
      if (this.level.value === 1000) {
        this.settings.clampCustom();
        w = this.settings.values.CustomWidth;
        h = this.settings.values.CustomHeight;
        m = this.settings.values.CustomMines;
      }
      m = Math.min(m, w * h - 1);
      this.field.useQuestionMarks = this.settings.values.UseQuestionMarks;
      this.field.newField(w, h, m);
      this.stopClock();
      this.setClockLimits(m);        // setMaxSeconds(max(mines,499))
      this.clock.seconds = 0;
      this.paused = false;
      this.scoreDisabledByReset = false;
      this.lastFlagCount = 0;
      this.emit('firstClickDone');
      this.emit('minesCountChanged', 0);
      this.emit('timeChanged');
    }

    // native: KGameClock (CountUp) + KMinesMainWindow::advanceTime
    startClock() {
      if (this.clock.running || this.paused || this.field.gameOver) return;
      this.clock.running = true;
      this.clock.timer = setInterval(() => {
        this.clock.seconds++;
        this.emit('timeChanged');
      }, 1000);
    }

    // native: setMaxSeconds(max(mines,499)) (slot4 @0x11b690) + setPenalty(5)
    // (slot5): +5 s to the elapsed time for every flag placement
    setClockLimits(m) { this.clock.max = Math.max(m, 499); }
    flagPenalty() {
      if (this.clock.running) {
        this.clock.seconds += 5;
        this.emit('timeChanged');
      }
    }

    // native: KGameClock::setPenalty(5) — each flag placement adds 5 s
    flagPenalty() {
      if (this.clock.running) {
        this.clock.seconds += 5;
        this.emit('timeChanged');
      }
    }

    stopClock() {
      this.clock.running = false;
      if (this.clock.timer) { clearInterval(this.clock.timer); this.clock.timer = null; }
    }

    pauseGame() {
      if (this.field.gameOver || !this.clock.running) return;
      this.paused = true;
      this.stopClock();
      this.emit('paused', true);
    }

    resumeGame() {
      if (!this.paused) return;
      this.paused = false;
      this.startClock();
      this.emit('paused', false);
    }

    onFirstClick() {
      // native: first click -> setPenalty(5) + clock->start() (0x1ac2e/0x1ad46)
      this.startClock();
    }

    // score = clock->seconds() (elapsed incl. penalties); shorter = better
    scoreValue() { return this.clock.seconds; }



    timeString() {
      const secs = this.clock.seconds;
      const m = Math.floor(secs / 60);
      const s = secs % 60;
      return (m < 10 ? '0' : '') + m + ':' + (s < 10 ? '0' : '') + s;
    }



    on(signal, fn) { (this.listeners[signal] = this.listeners[signal] || []).push(fn); }
    emit(signal, payload) { (this.listeners[signal] || []).forEach((f) => f(payload)); }
  }

  class KMinesApp {
    constructor() {
      this.settings = new Settings();
      this.scores = new HighScores();
      this.scene = new KMinesScene(this.settings);
      this.cursor = null;          // keyboard cursor (row, col)
      this.dirty = true;
      this.popupTimer = null;
      this.atlas = window.KMinesSprites.build();
      this.renderer = new window.KMinesRenderer($('board'), this.atlas);
      this.bindUi();
      this.scene.on('minesCountChanged', () => { this.updateHud(); this.dirty = true; });
      this.scene.on('timeChanged', () => this.updateHud());
      this.scene.on('firstClickDone', () => { this.scene.onFirstClick(); this.dirty = true; });
      this.scene.on('gameOver', (won) => this.onGameOver(won));
      this.scene.on('paused', (p) => {
        // docbook: "the board disappears when the game is paused"
        $('board').style.visibility = p ? 'hidden' : 'visible';
        this.showMessage(p ? 'Game is paused.' : '', 1200);
        $('pauseButton').textContent = p ? 'Resume' : 'Pause';
        this.dirty = true;
      });
      this.scene.newGame();
      this.updateHud();
      this.layout();
      window.addEventListener('resize', () => { this.layout(); this.dirty = true; });
      this.frame();
    }

    bindUi() {
      const canvas = $('board');
      canvas.addEventListener('contextmenu', (e) => e.preventDefault());
      canvas.addEventListener('mousedown', (e) => this.onMouse(e, 'press'));
      canvas.addEventListener('mouseup', (e) => this.onMouse(e, 'release'));
      canvas.addEventListener('mouseleave', () => {
        this.scene.field.cells.forEach((c) => { c.pressed = false; });
        this.scene.field.shineOn = null;
        this.scene.field.shineFlagsOn = null;
        this.dirty = true;
      });
      canvas.addEventListener('mousemove', (e) => {
        const cell = this.cellAt(e);
        const f = this.scene.field;
        let changed = false;
        f.cells.forEach((c, i) => {
          const want = cell === i && !f.gameOver &&
            (e.buttons & 1) !== 0 && f.cells[i].state === KMinesState.Hidden;
          if (c.pressed !== want) { c.pressed = want; changed = true; }
        });
        if (changed) this.dirty = true;
      });
      canvas.tabIndex = 0;
      canvas.addEventListener('keydown', (e) => this.onKey(e));

      $('resetButton').addEventListener('click', () => this.newGame(true));
      $('pauseButton').addEventListener('click', () => {
        if (this.scene.paused) this.scene.resumeGame(); else this.scene.pauseGame();
      });
      $('scoresButton').addEventListener('click', () => this.showHighscores());
      $('settingsButton').addEventListener('click', () => this.openSettings());
      $('levelSelect').addEventListener('change', (e) => {
        this.settings.values.Level = parseInt(e.target.value, 10);
        this.settings.save();
        this.newGame(false);
      });
      $('settingsCancel').addEventListener('click', () => $('settingsDialog').close());
      $('scoreClose').addEventListener('click', () => $('scoreDialog').close());
      $('settingsForm').addEventListener('submit', (e) => { e.preventDefault(); this.applySettings(); });
      ['optCustomWidth', 'optCustomHeight'].forEach((id) => {
        $(id).addEventListener('input', () => this.syncCustomLimits());
      });
    }

    layout() {
      const f = this.scene.field;
      const w = f.width * RENDER_SIZE + 2 * BORDER;
      const h = f.height * RENDER_SIZE + 2 * BORDER;
      const dpr = Math.min(window.devicePixelRatio || 1, 2);
      const availW = Math.max(200, document.getElementById('viewPort').clientWidth);
      const scale = Math.min(1, availW / w);
      const canvas = $('board');
      canvas.style.width = Math.round(w * scale) + 'px';
      canvas.style.height = Math.round(h * scale) + 'px';
      this.cssScale = scale;
      this.renderer.resize(w, h, dpr * scale);
      this.boardW = w; this.boardH = h;
    }

    cellAt(evt) {
      const canvas = $('board');
      const r = canvas.getBoundingClientRect();
      const scale = this.boardW / r.width;
      const x = (evt.clientX - r.left) * scale - BORDER;
      const y = (evt.clientY - r.top) * scale - BORDER;
      const col = Math.floor(x / RENDER_SIZE);
      const row = Math.floor(y / RENDER_SIZE);
      const f = this.scene.field;
      if (!f.inside(row, col)) return -1;
      return f.index(row, col);
    }

    // native: MineFieldItem::mousePressEvent @0x1f660
    onMouse(evt, phase) {
      const f = this.scene.field;
      const i = this.cellAt(evt);
      if (i < 0) return;
      evt.preventDefault();
      const placeFlagOnPress = this.settings.values.PlaceFlagOn === 1;
      const cell = f.cells[i];

      if (phase === 'press') {
        if (evt.button === 1) {                     // middle: preview flags
          f.shineFlagsOn = [f.rowOf(i), f.colOf(i)];
          this.dirty = true;
          return;
        }
        if (evt.button === 2 && placeFlagOnPress) { f.toggleState(i); this.dirty = true; return; }
        if (evt.button === 0) {
          if (cell.state === KMinesState.Hidden) { cell.pressed = true; this.dirty = true; }
          if (this.settings.values.ExploreWithLeftClickOnNumberCells &&
              cell.state === KMinesState.Revealed) {
            f.shineOn = [f.rowOf(i), f.colOf(i)];
            this.dirty = true;
          }
        }
        return;
      }

      // release
      if (evt.button === 1) {                       // middle: explore / chord
        f.shineFlagsOn = null;
        f.explore(i);
        this.dirty = true;
        return;
      }
      if (evt.button === 2) {
        if (!placeFlagOnPress) f.toggleState(i);
        this.dirty = true;
        return;
      }
      if (evt.button === 0) {
        f.shineOn = null;
        cell.pressed = false;
        if (evt.shiftKey) { f.explore(i); this.dirty = true; return; }
        if (cell.state === KMinesState.Revealed &&
            this.settings.values.ExploreWithLeftClickOnNumberCells) {
          f.explore(i);
          this.dirty = true;
          return;
        }
        if (!f.firstClickDone) {
          f.placeMines(i);
          f.firstClickDone = true;
          f.emit('firstClickDone');
        }
        f.openField(i);
        this.cursor = [f.rowOf(i), f.colOf(i)];
        this.dirty = true;
      }
    }

    onKey(evt) {
      const f = this.scene.field;
      const k = evt.key;
      if (k === 'F2') { this.newGame(true); evt.preventDefault(); return; }
      if (k === 'p' || k === 'P') {
        this.scene.paused ? this.scene.resumeGame() : this.scene.pauseGame();
        evt.preventDefault(); return;
      }
      if (k === 'h' || k === 'H') { this.showHighscores(); evt.preventDefault(); return; }
      if (this.scene.paused && (k === ' ' || k === 'Enter')) {
        this.scene.resumeGame(); evt.preventDefault(); return;
      }
      if (f.gameOver) return;
      const d = { ArrowUp: [-1, 0], ArrowDown: [1, 0], ArrowLeft: [0, -1], ArrowRight: [0, 1] }[k];
      if (d) {
        if (!this.cursor) this.cursor = [0, 0];
        const r = Math.min(f.height - 1, Math.max(0, this.cursor[0] + d[0]));
        const c = Math.min(f.width - 1, Math.max(0, this.cursor[1] + d[1]));
        this.cursor = [r, c];
        this.dirty = true;
        evt.preventDefault();
        return;
      }
      if (!this.cursor) return;
      const i = f.index(this.cursor[0], this.cursor[1]);
      if (k === ' ' || k === 'Enter') {
        if (!f.firstClickDone) {
          f.placeMines(i);
          f.firstClickDone = true;
          f.emit('firstClickDone');
        }
        f.openField(i);
        this.dirty = true;
        evt.preventDefault();
      } else if (k === 'f' || k === 'F' || k === 'Insert') {
        f.toggleState(i);
        this.dirty = true;
        evt.preventDefault();
      }
    }

    newGame(confirmed) {
      const f = this.scene.field;
      if (confirmed && this.settings.values.AllowKminesReset &&
          f.firstClickDone && !f.gameOver) {
        // native: KMessageBox::questionTwoActions("Reset the Game?")
        if (!window.confirm('Reset the Game?')) return;
        if (this.settings.values.DisableScoreOnReset) this.scene.scoreDisabledByReset = true;
      }
      this.scene.newGame();
      $('pauseButton').textContent = 'Pause';
      this.layout();
      this.dirty = true;
    }

    // native: KMinesMainWindow::count / onMinesCountChanged / advanceTime
    updateHud() {
      const f = this.scene.field;
      $('timeLabel').textContent = 'Time: ' + this.scene.timeString();
      // native: label starts as "Mines: 0/0"; after first click "Mines: %1/%2"
      // with %1 = flags placed, %2 = total mines (translate strings at 0x221c3/0x12521)
      $('mineLabel').textContent = 'Mines: ' + (f.firstClickDone
        ? f.flagCount() + '/' + f.minesToPlace : '0/0');
      // (native status strings: "Mines: 0/0" then i18n("Mines: %1/%2"))
      $('levelLabel').textContent = this.scene.level.title;
      $('levelSelect').value = String(this.scene.level.value);
    }

    // native: KMinesScene::onGameOver -> KGamePopupItem::showMessage + score
    onGameOver(won) {
      this.scene.stopClock();
      this.showMessage(won ? 'Congratulations! You have won!' : 'You have lost.', 2000);
      if (won && !this.scene.scoreDisabledByReset) {
        // score = clock->seconds() (elapsed incl. penalty); shortest wins
        this.scores.add(this.scene.level.key, this.scene.scoreValue());
        this.showHighscores();
      }
      this.dirty = true;
    }

    showMessage(text, ms) {
      const el = $('popup');
      if (!text) { el.hidden = true; el.textContent = ''; return; }
      el.textContent = text;
      el.hidden = false;
      if (this.popupTimer) clearTimeout(this.popupTimer);
      if (ms) this.popupTimer = setTimeout(() => { el.hidden = true; }, ms);
    }

    showHighscores() {
      const list = this.scores.list(this.scene.level.key);
      const rows = list.map((e, n) =>
        '<tr><td>' + (n + 1) + '</td><td>' +
        String(Math.floor(e.time / 60)).padStart(2, '0') + ':' +
        String(e.time % 60).padStart(2, '0') + '</td><td>' + (e.date || '') + '</td></tr>');
      $('scoreBody').innerHTML = '<h3>' + this.scene.level.title + '</h3><table><thead><tr>' +
        '<th>#</th><th>Time</th><th>Date</th></tr></thead><tbody>' +
        (rows.join('') || '<tr><td colspan="3">No scores yet</td></tr>') + '</tbody></table>';
      $('scoreDialog').showModal();
    }

    openSettings() {
      const v = this.settings.values;
      $('optUseQuestionMarks').checked = !!v.UseQuestionMarks;
      $('optAllowKminesReset').checked = !!v.AllowKminesReset;
      $('optDisableScoreOnReset').checked = !!v.DisableScoreOnReset;
      $('optExploreWithLeftClickOnNumberCells').checked = !!v.ExploreWithLeftClickOnNumberCells;
      $('optPlaceFlagOn').value = String(v.PlaceFlagOn);
      $('optCustomWidth').value = v.CustomWidth;
      $('optCustomHeight').value = v.CustomHeight;
      $('optCustomMines').value = v.CustomMines;
      this.syncCustomLimits();
      $('settingsDialog').showModal();
    }

    syncCustomLimits() {
      const w = parseInt($('optCustomWidth').value, 10) || 5;
      const h = parseInt($('optCustomHeight').value, 10) || 5;
      const max = Math.max(1, w * h - 1);
      const m = $('optCustomMines');
      m.max = String(max);
      if (parseInt(m.value, 10) > max) m.value = String(max);
    }

    applySettings() {
      const v = this.settings.values;
      v.UseQuestionMarks = $('optUseQuestionMarks').checked;
      v.AllowKminesReset = $('optAllowKminesReset').checked;
      v.DisableScoreOnReset = $('optDisableScoreOnReset').checked;
      v.ExploreWithLeftClickOnNumberCells = $('optExploreWithLeftClickOnNumberCells').checked;
      v.PlaceFlagOn = parseInt($('optPlaceFlagOn').value, 10);
      v.CustomWidth = parseInt($('optCustomWidth').value, 10);
      v.CustomHeight = parseInt($('optCustomHeight').value, 10);
      v.CustomMines = parseInt($('optCustomMines').value, 10);
      this.settings.clampCustom();
      this.settings.save();
      $('settingsDialog').close();
      this.newGame(false);
    }

    // native: MineFieldItem::doLayout + KMinesScene::doLayout -> draw the scene
    draw() {
      const r = this.renderer;
      const f = this.scene.field;
      r.begin();
      // panel + bevelled frame (border.edge.* / border.outsideCorner.*)
      r.tileArea(0, 0, this.boardW, this.boardH, 'mainWidget', RENDER_SIZE);
      r.quad(0, 0, BORDER, BORDER, 'border.outsideCorner.nw');
      r.quad(this.boardW - BORDER, 0, BORDER, BORDER, 'border.outsideCorner.ne');
      r.quad(0, this.boardH - BORDER, BORDER, BORDER, 'border.outsideCorner.sw');
      r.quad(this.boardW - BORDER, this.boardH - BORDER, BORDER, BORDER, 'border.outsideCorner.se');
      r.tileArea(BORDER, 0, f.width * RENDER_SIZE, BORDER, 'border.edge.north', BORDER);
      r.tileArea(BORDER, this.boardH - BORDER, f.width * RENDER_SIZE, BORDER, 'border.edge.south', BORDER);
      r.tileArea(0, BORDER, BORDER, f.height * RENDER_SIZE, 'border.edge.west', BORDER);
      r.tileArea(this.boardW - BORDER, BORDER, BORDER, f.height * RENDER_SIZE, 'border.edge.east', BORDER);

      const shine = this.shineSet();
      for (let i = 0; i < f.cells.length; i++) {
        const x = BORDER + f.colOf(i) * RENDER_SIZE;
        const y = BORDER + f.rowOf(i) * RENDER_SIZE;
        const keys = f.spriteKeys(i);
        r.quad(x, y, RENDER_SIZE, RENDER_SIZE, keys.base);
        if (keys.number) r.quad(x, y, RENDER_SIZE, RENDER_SIZE, keys.number);
        if (keys.overlay) r.quad(x, y, RENDER_SIZE, RENDER_SIZE, keys.overlay);
        if (keys.cross) r.quad(x, y, RENDER_SIZE, RENDER_SIZE, keys.cross);
        if (shine.has(i)) r.quad(x, y, RENDER_SIZE, RENDER_SIZE, 'hint');
        if (this.cursor && this.cursor[0] === f.rowOf(i) && this.cursor[1] === f.colOf(i)) {
          r.quad(x, y, RENDER_SIZE, RENDER_SIZE, 'hint');
        }
      }
      r.flush();
    }

    shineSet() {
      const f = this.scene.field;
      const set = new Set();
      const src = f.shineFlagsOn || f.shineOn;
      if (!src || f.gameOver) return set;
      for (const n of f.neighboursOf(f.index(src[0], src[1]))) {
        set.add(n);
      }
      return set;
    }

    frame() {
      if (this.dirty) {
        this.dirty = false;
        this.draw();
      }
      requestAnimationFrame(() => this.frame());
    }
  }

  window.addEventListener('DOMContentLoaded', () => {
    try {
      window.kminesApp = new KMinesApp();
    } catch (e) {
      const el = document.getElementById('popup');
      el.hidden = false;
      el.textContent = 'Failed to start: ' + e.message;
      // eslint-disable-next-line no-console
      console.error(e);
    }
  });
})();
