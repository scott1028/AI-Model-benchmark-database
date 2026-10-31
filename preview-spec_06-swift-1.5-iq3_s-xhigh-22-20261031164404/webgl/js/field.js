/* field.js — reconstruction of MineFieldItem (the board model of KMines).
 *
 * Every rule below is derived from the disassembly / decompiled pseudocode of
 * /usr/games/kmines; the matching native addresses are noted in comments.
 * See ../report/index.md and ../report/evidence/04-game-logic.md for the trail.
 */
(function () {
  'use strict';

  // Cell states. Numeric values follow the ints written into cell+0x28 by the
  // native code (newField writes 0, revealAll writes 1/2/3/4/5/7, ...).
  const State = {
    Hidden: 0,          // sprite "cell_up"
    Revealed: 1,        // sprite "cell_down"
    Exploding: 2,       // sprite "explosion" (the mine that ended the game)
    Flagged: 3,         // cell_up + "flag"
    Question: 4,        // cell_up + "question"
    FlagLost: 5,        // cell_down + "flag" + "error"  (flag on a non-mine)
    MineRevealed: 6,    // cell_down + "mine"             (revealed after a win)
    MineHidden: 7,      // "mine" on cell_up              (hidden mine, shown on loss)
    ExplosionWrong: 8   // cell_down + "flag" + "error"
  };

  // 8 neighbour offsets (native: the static list built by FUN_00116240, used by
  // MineFieldItem::getNeighbours @0x19c40).
  // "arabicOne".."arabicEight" sprite keys (word names, as in the binary).
  const NUMBER_WORDS = [null, 'One', 'Two', 'Three', 'Four',
                        'Five', 'Six', 'Seven', 'Eight'];

  const NEIGHBOURS = [
    [-1, -1], [-1, 0], [-1, 1],
    [0, -1], [0, 1],
    [1, -1], [1, 0], [1, 1]
  ];

  // Qt's QRandomGenerator::bounded(uint) picks a value with a multiply-shift
  // (evidence: FUN_0011a0b0 computes (u64)rand * (w*h) >> 32).
  function Rng(seed) {
    let s = (seed >>> 0) || 0x9e3779b9;
    this.next = function () {
      s ^= s << 13; s >>>= 0;
      s ^= s >> 17;
      s ^= s << 5; s >>>= 0;
      return s >>> 0;
    };
    this.bounded = function (n) {
      const r = this.next();
      return Math.floor((r * n) / 4294967296);   // (r * n) >> 32
    };
  }

  class MineFieldItem {
    constructor(opts) {
      this.width = 0;
      this.height = 0;
      this.minesToPlace = 0;
      this.cells = [];
      this.mineList = [];
      this.firstClickDone = false;      // native: field+0x88
      this.gameOver = false;
      this.gameWon = false;
      this.useQuestionMarks = true;
      this.shineOn = null;              // native: field+0x78/0x7c
      this.shineFlagsOn = null;         // native: field+0x80/0x84
      this.rng = new Rng((Date.now() & 0x7fffffff) ^ 0x5bd1e995);
      this.listeners = {};
    }

    on(signal, fn) {
      (this.listeners[signal] = this.listeners[signal] || []).push(fn);
    }

    emit(signal, payload) {
      (this.listeners[signal] || []).forEach((f) => f(payload));
    }

    index(row, col) { return row * this.width + col; }
    rowOf(i) { return (i / this.width) | 0; }
    colOf(i) { return i % this.width; }
    inside(row, col) {
      return row >= 0 && row < this.height && col >= 0 && col < this.width;
    }

    // native: MineFieldItem::newField @0x206d0 (writes height/width/mines and
    // clears every cell: state 0, isMine 0, minesAround 0; shine markers = -1)
    newField(width, height, mines) {
      this.width = width;
      this.height = height;
      this.minesToPlace = mines;
      this.cells = [];
      for (let i = 0; i < width * height; i++) {
        this.cells.push({ state: State.Hidden, isMine: false, minesAround: 0, pressed: false });
      }
      this.mineList = [];
      this.firstClickDone = false;
      this.gameOver = false;
      this.gameWon = false;
      this.shineOn = null;
      this.shineFlagsOn = null;
      this.emit('minesCountChanged', this.flagCount());
    }

    // native: MineFieldItem::placeMines(field, firstIndex) @0x1a0b0
    //   while (minesToPlace != 0) {
    //     idx = bounded(w*h)                     // uniform, multiply-shift
    //     if (cell[idx].isMine) continue;         // cell+0x2c
    //     if (idx == firstIndex) continue;        // first click is always safe
    //     cell[idx].isMine = true; mineList.append(idx);
    //   }
    //   for each mine: for each neighbour: if (!neighbour.isMine) neighbour.minesAround++
    placeMines(firstIndex) {
      let left = this.minesToPlace;
      const total = this.width * this.height;
      let guard = 0;
      while (left > 0 && guard++ < total * 200) {
        const idx = this.rng.bounded(total);
        const cell = this.cells[idx];
        if (cell.isMine) continue;
        if (idx === firstIndex) continue;
        cell.isMine = true;
        this.mineList.push(idx);
        left--;
      }
      for (const m of this.mineList) {
        for (const n of this.neighboursOf(m)) {
          if (!this.cells[n].isMine) this.cells[n].minesAround++;
        }
      }
    }

    neighboursOf(i) {
      const r = this.rowOf(i), c = this.colOf(i), out = [];
      for (const [dr, dc] of NEIGHBOURS) {
        if (this.inside(r + dr, c + dc)) out.push(this.index(r + dr, c + dc));
      }
      return out;
    }

    flagCount() {
      return this.cells.reduce((n, c) => n + (c.state === State.Flagged ? 1 : 0), 0);
    }

    // native: MineFieldItem::openField(cell, explore) @0x18790 + flood fill
    openField(i, explore = false) {
      const cell = this.cells[i];
      if (this.gameOver) return false;
      if (cell.state === State.Flagged || cell.state === State.Question) return false;
      if (cell.isMine) {
        cell.state = State.Exploding;
        this.endGame(false);
        return true;
      }
      if (cell.state === State.Revealed) return false;
      cell.state = State.Revealed;
      if (cell.minesAround === 0) {
        // "openRec": open the neighbours recursively
        const stack = this.neighboursOf(i);
        while (stack.length) {
          const n = stack.pop();
          const nc = this.cells[n];
          if (nc.state === State.Revealed || nc.isMine) continue;
          if (nc.state === State.Flagged || nc.state === State.Question) continue;
          nc.state = State.Revealed;
          if (nc.minesAround === 0) stack.push(...this.neighboursOf(n));
        }
      }
      this.checkWin();
      return true;
    }

    // native: mouseReleaseEvent @0x1fa60, middle-button/explore branch:
    //   flagsAround = count(neighbour.state == flagged)
    //   minesAround = count(neighbour.isMine)
    //   if (flagsAround == minesAround && flagsAround != 0) open every non-flagged neighbour
    explore(i) {
      const cell = this.cells[i];
      if (this.gameOver) return false;
      if (cell.state !== State.Revealed) return false;
      const ns = this.neighboursOf(i);
      const flags = ns.filter((n) => this.cells[n].state === State.Flagged).length;
      const mines = ns.filter((n) => this.cells[n].isMine).length;
      if (flags !== mines || flags === 0) return false;
      let acted = false;
      for (const n of ns) {
        if (this.cells[n].state === State.Flagged) continue;
        this.openField(n, true);
        acted = true;
        if (this.gameOver) break;
      }
      return acted;
    }

    // native: MineFieldItem::toggleState @0x19a50
    // hidden -> flagged -> (question, when "Use '?' marks" is on) -> hidden
    toggleState(i) {
      const cell = this.cells[i];
      if (this.gameOver) return false;
      if (!this.firstClickDone && cell.state === State.Hidden) {
        // right-click before the first click: mines get placed first, so the
        // flag is laid on the real field (mousePressEvent @0x1f660, 0x201f2)
        this.placeMines(i);
        this.firstClickDone = true;
        this.emit('firstClickDone');
      }
      if (cell.state === State.Hidden) cell.state = State.Flagged;
      else if (cell.state === State.Flagged) {
        cell.state = this.useQuestionMarks ? State.Question : State.Hidden;
      } else if (cell.state === State.Question) cell.state = State.Hidden;
      else return false;
      this.emit('flaggedMinesCountChanged', this.flagCount());
      return true;
    }

    isWin() {
      return this.cells.every((c) => c.isMine || c.state === State.Revealed);
    }

    checkWin() {
      if (this.gameOver || this.mineList.length === 0) return;
      if (!this.firstClickDone) return;
      if (this.isWin()) this.endGame(true);
    }

    // native: MineFieldItem::gameOver @0x206d0 (second body range) reveals the
    // mines, and KMinesScene::onGameOver @0x17ee0 shows the popup / score.
    endGame(won) {
      if (this.gameOver) return;
      this.gameOver = true;
      this.gameWon = won;
      if (won) {
        for (const m of this.mineList) {
          const c = this.cells[m];
          if (c.state !== State.Flagged) c.state = State.MineRevealed;
        }
      } else {
        for (let i = 0; i < this.cells.length; i++) {
          const c = this.cells[i];
          if (c.isMine && c.state !== State.Exploding) {
            c.state = c.state === State.Flagged ? State.Revealed : State.MineHidden;
          } else if (!c.isMine && c.state === State.Flagged) {
            c.state = State.FlagLost;
          }
        }
      }
      this.emit('gameOver', won);
    }

    // Sprite composition for one cell, mirroring MineFieldItem::updateItem
    // (@0x17930: the KGameRenderedItem sprite key from the state table, plus a
    // child item for the "arabic*" number sprite).
    spriteKeys(i) {
      const c = this.cells[i];
      const pressed = c.pressed && !this.gameOver;
      switch (c.state) {
        case State.Hidden:
          return { base: pressed ? 'cell_down' : 'cell_up', overlay: null, number: null };
        case State.Revealed:
          return {
            base: 'cell_down',
            overlay: null,
            number: c.minesAround > 0
              ? 'arabic' + NUMBER_WORDS[c.minesAround] : null
          };
        case State.Flagged:
          return { base: pressed ? 'cell_down' : 'cell_up', overlay: 'flag', number: null };
        case State.Question:
          return { base: pressed ? 'cell_down' : 'cell_up', overlay: 'question', number: null };
        case State.Exploding:
          return { base: 'cell_down', overlay: 'explosion', number: null };
        case State.MineHidden:
          return { base: 'cell_up', overlay: 'mine', number: null };
        case State.MineRevealed:
          return { base: 'cell_down', overlay: 'mine', number: null };
        case State.FlagLost:
        case State.ExplosionWrong:
          return { base: 'cell_down', overlay: 'flag', number: null, cross: 'error' };
        default:
          return { base: 'cell_up', overlay: null, number: null };
      }
    }
  }

  window.KMinesState = State;
  window.MineFieldItem = MineFieldItem;
})();
