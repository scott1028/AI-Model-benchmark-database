// Port of MineFieldItem + CellItem logic recovered from kmines.
// Pure state machine: no DOM, no WebGL. Addresses refer to Ghidra pseudocode.
import { BorderElement, Button, CellState, LONG_PRESS_MS, PlaceFlagOn } from '../constants.js';

const NO_POS = -1;

const createCell = () => ({ state: CellState.Released, hasMine: false, exploded: false, digit: 0 });

const isRevealedState = (state) => state === CellState.Revealed || state === CellState.Error;

// CellItem::press (inlined)
const pressCell = (cell) => {
  if (cell.state === CellState.Released) cell.state = CellState.Pressed;
};

// CellItem::undoPress (0x1187f0)
const undoPressCell = (cell) => {
  if (cell.state === CellState.Pressed) cell.state = CellState.Released;
};

// CellItem::release (0x118790)
const releaseCell = (cell, force = false) => {
  if (!force) {
    if (cell.state === CellState.Pressed) {
      cell.state = CellState.Revealed;
      cell.exploded = cell.hasMine;
    }
    return;
  }
  if (cell.state === CellState.Questioned || cell.state === CellState.Flagged) return;
  cell.exploded = cell.hasMine;
  if (!isRevealedState(cell.state)) cell.state = CellState.Revealed;
};

// CellItem::mark (0x117e70)
const markCell = (cell, useQuestionMarks) => {
  if (cell.state === CellState.Released) cell.state = CellState.Flagged;
  else if (cell.state === CellState.Flagged) cell.state = useQuestionMarks ? CellState.Questioned : CellState.Released;
  else if (cell.state === CellState.Questioned) cell.state = CellState.Released;
};

export class MineField {
  // getSettings(): current settings object; randomInt(n): integer in [0, n)
  constructor({ getSettings, randomInt, now = () => Date.now(), listener = {} }) {
    this.getSettings = getSettings;
    this.randomInt = randomInt;
    this.now = now;
    this.listener = listener;
    this.rows = 1;
    this.cols = 1;
    this.minesCount = 0;
    this.cells = [];
    this.borders = [];
    this.gameOver = false;
  }

  emit(name, ...args) {
    this.listener[name]?.(...args);
  }

  cellAt(row, col) {
    return this.cells[row * this.cols + col];
  }

  isInside(row, col) {
    return row >= 0 && row < this.rows && col >= 0 && col < this.cols;
  }

  // KMinesScene::startNewGame + MineFieldItem::initField (0x1206d0)
  initField(rows, cols, mines) {
    this.firstClick = true;
    this.gameOver = false;
    this.rows = rows;
    this.cols = cols;
    this.minesCount = Math.min(mines, rows * cols - 10);
    this.numUnrevealed = rows * cols;
    this.leftButtonPos = [NO_POS, NO_POS];
    this.midButtonPos = [NO_POS, NO_POS];
    this.touchPos = [NO_POS, NO_POS];
    this.touchStartMs = 0;
    this.emulatingMidButton = false;
    this.cells = Array.from({ length: rows * cols }, createCell);
    this.borders = getBorderLayout(rows, cols);
    this.flaggedMinesCount = 0;
    this.emit('flaggedMinesCountChanged', 0);
    this.emit('changed');
  }

  // KMines reset after a loss (inlined in 0x117ee0): mines stay, cells closed.
  resetField() {
    this.gameOver = false;
    this.numUnrevealed = this.rows * this.cols;
    for (const cell of this.cells) {
      cell.state = CellState.Released;
      cell.exploded = false;
    }
    this.flaggedMinesCount = 0;
    this.emit('flaggedMinesCountChanged', 0);
    this.emit('changed');
  }

  // 0x116240: NW, N, NE, W, E, SW, S, SE
  adjacentRowCols(row, col) {
    const out = [];
    for (let dr = -1; dr <= 1; dr++) {
      for (let dc = -1; dc <= 1; dc++) {
        if ((dr || dc) && this.isInside(row + dr, col + dc)) out.push([row + dr, col + dc]);
      }
    }
    return out;
  }

  // 0x119c40
  adjacentCells(row, col) {
    return this.adjacentRowCols(row, col).map(([r, c]) => this.cellAt(r, c));
  }

  // 0x11a0b0: first click and its neighbours never hold a mine.
  generateField(clickedIdx) {
    const clickedRow = Math.floor(clickedIdx / this.cols);
    const neighbours = this.adjacentCells(clickedRow, clickedIdx % this.cols);
    const minePositions = [];
    let toPlace = this.minesCount;
    while (toPlace !== 0) {
      const idx = this.randomInt(this.rows * this.cols);
      const cell = this.cells[idx];
      if (cell.hasMine || neighbours.includes(cell) || idx === clickedIdx) continue;
      cell.hasMine = true;
      minePositions.push(idx);
      toPlace--;
    }
    for (const idx of minePositions) {
      for (const cell of this.adjacentCells(Math.floor(idx / this.cols), idx % this.cols)) {
        if (!cell.hasMine) cell.digit++;
      }
    }
  }

  // 0x118690
  revealEmptySpace(row, col) {
    for (const [r, c] of this.adjacentRowCols(row, col)) {
      const cell = this.cellAt(r, c);
      const { state } = cell;
      if (state === CellState.Revealed || state === CellState.Questioned
        || state === CellState.Flagged || state === CellState.Error) continue;
      cell.state = CellState.Revealed;
      this.numUnrevealed--;
      if (cell.digit === 0) this.revealEmptySpace(r, c);
    }
  }

  // 0x1197f0: returns true when the game ended
  onItemRevealed(row, col) {
    this.numUnrevealed--;
    const revealed = this.cellAt(row, col);
    if (revealed.hasMine) {
      for (const cell of this.cells) {
        const isFlagged = cell.state === CellState.Flagged;
        if (cell.hasMine === isFlagged) continue;
        if (!isRevealedState(cell.state)) {
          cell.state = isFlagged && !cell.hasMine ? CellState.Error : CellState.Revealed;
        }
        this.numUnrevealed--;
      }
    } else if (revealed.digit === 0) {
      this.revealEmptySpace(row, col);
    }

    if (this.cells.some((cell) => cell.exploded)) {
      this.gameOver = true;
      this.emit('gameOver', false);
      return true;
    }
    if (this.numUnrevealed === this.minesCount) {
      const { useQuestionMarks } = this.getSettings();
      for (const cell of this.cells) {
        if (cell.state === CellState.Questioned) markCell(cell, useQuestionMarks);
        if (cell.state === CellState.Flagged || cell.state === CellState.Error) continue;
        if (cell.state !== CellState.Revealed) markCell(cell, useQuestionMarks);
      }
      this.gameOver = true;
      this.emit('flaggedMinesCountChanged', this.minesCount);
      this.emit('gameOver', true);
      return true;
    }
    return false;
  }

  // 0x119b00
  onCellRevealed(cell) {
    const idx = this.cells.indexOf(cell);
    if (idx === -1) return false;
    return this.onItemRevealed(Math.floor(idx / this.cols), idx % this.cols);
  }

  // 0x119a50
  handleFlag(cell) {
    const wasFlagged = cell.state === CellState.Flagged;
    markCell(cell, this.getSettings().useQuestionMarks);
    const isFlagged = cell.state === CellState.Flagged;
    if (isFlagged !== wasFlagged) {
      this.flaggedMinesCount += isFlagged ? 1 : -1;
      this.emit('flaggedMinesCountChanged', this.flaggedMinesCount);
    }
  }

  // Reveal neighbours once enough flags surround a revealed number.
  revealNeighbours(neighbours) {
    for (const cell of neighbours) {
      if (isRevealedState(cell.state)) continue;
      releaseCell(cell, true);
      if (isRevealedState(cell.state) && this.onCellRevealed(cell)) break;
    }
  }

  revealFirst(row, col) {
    this.firstClick = false;
    this.generateField(row * this.cols + col);
    this.emit('firstClickDone');
  }

  // 0x11f660
  mousePress(row, col, button, buttons) {
    if (!this.isInside(row, col)) return;
    const cell = this.cellAt(row, col);
    const { exploreWithLeftClickOnNumberCells, placeFlagOn } = this.getSettings();
    const isLeftHeld = (buttons & Button.Left) !== 0;
    this.emulatingMidButton = exploreWithLeftClickOnNumberCells
      ? isLeftHeld && isRevealedState(cell.state)
      : isLeftHeld && (buttons & Button.Right) !== 0;

    if (button === Button.Middle || this.emulatingMidButton) {
      undoPressCell(cell);
      for (const n of this.adjacentCells(row, col)) pressCell(n);
      this.midButtonPos = [row, col];
      this.leftButtonPos = [NO_POS, NO_POS];
    } else if (button === Button.Left) {
      pressCell(cell);
      this.leftButtonPos = [row, col];
    } else if (placeFlagOn === PlaceFlagOn.MousePress && button === Button.Right && !isLeftHeld) {
      this.handleFlag(cell);
    }
    this.emit('changed');
  }

  // 0x11fa60
  mouseRelease(row, col, button, buttons) {
    if (!this.isInside(row, col)) {
      this.cancelPresses();
      this.emit('changed');
      return;
    }
    const cell = this.cellAt(row, col);
    const { placeFlagOn } = this.getSettings();

    if (button === Button.Middle || this.emulatingMidButton) {
      this.midButtonPos = [NO_POS, NO_POS];
      const neighbours = this.adjacentCells(row, col);
      const flags = neighbours.filter((n) => n.state === CellState.Flagged).length;
      const mines = neighbours.filter((n) => n.hasMine).length;
      if (isRevealedState(cell.state) && flags === mines && flags !== 0) this.revealNeighbours(neighbours);
      else neighbours.forEach(undoPressCell);
    } else if (button === Button.Left && (buttons & Button.Right) === 0) {
      if (this.midButtonPos[0] !== NO_POS) {
        undoPressCell(cell);
      } else if (this.leftButtonPos[0] !== NO_POS) {
        if (!isRevealedState(cell.state)) {
          if (this.firstClick) this.revealFirst(row, col);
          releaseCell(cell);
          if (isRevealedState(cell.state)) this.onItemRevealed(row, col);
        }
        this.leftButtonPos = [NO_POS, NO_POS];
      }
    } else if (placeFlagOn === PlaceFlagOn.MouseRelease && button === Button.Right
      && (buttons & Button.Left) === 0) {
      this.handleFlag(cell);
    }
    this.emit('changed');
  }

  cancelPresses() {
    if (this.midButtonPos[0] !== NO_POS) {
      this.adjacentCells(...this.midButtonPos).forEach(undoPressCell);
      this.midButtonPos = [NO_POS, NO_POS];
      this.emulatingMidButton = false;
    }
    if (this.leftButtonPos[0] !== NO_POS) {
      undoPressCell(this.cellAt(...this.leftButtonPos));
      this.leftButtonPos = [NO_POS, NO_POS];
    }
  }

  // 0x11eb40
  mouseMove(row, col, buttons) {
    if (!this.isInside(row, col)) return;
    const [leftRow, leftCol] = this.leftButtonPos;
    const [midRow, midCol] = this.midButtonPos;
    if ((buttons & Button.Middle) === 0 && !this.emulatingMidButton) {
      if ((buttons & Button.Left) && leftRow !== NO_POS && (leftRow !== row || leftCol !== col)) {
        undoPressCell(this.cellAt(leftRow, leftCol));
        pressCell(this.cellAt(row, col));
        this.leftButtonPos = [row, col];
        this.emit('changed');
      }
    } else if (midRow !== NO_POS && (midRow !== row || midCol !== col)) {
      this.adjacentCells(midRow, midCol).forEach(undoPressCell);
      this.adjacentCells(row, col).forEach(pressCell);
      this.midButtonPos = [row, col];
      this.emit('changed');
    }
  }

  // 0x11ee50 TouchBegin
  touchBegin(row, col) {
    if (this.gameOver) return;
    if (!this.isInside(row, col)) {
      this.touchPos = [NO_POS, NO_POS];
      return;
    }
    this.touchPos = [row, col];
    this.touchStartMs = this.now();
    pressCell(this.cellAt(row, col));
    this.emit('changed');
  }

  // 0x11ee50 TouchEnd: short tap flags / chords, long press reveals
  touchEnd(row, col) {
    if (this.gameOver) return;
    const [touchRow, touchCol] = this.touchPos;
    if (!this.isInside(row, col) || row !== touchRow || col !== touchCol) {
      this.touchCancel();
      return;
    }
    const cell = this.cellAt(row, col);
    if (this.firstClick) {
      this.revealFirst(row, col);
      releaseCell(cell, true);
      if (isRevealedState(cell.state)) this.onItemRevealed(row, col);
    } else if (this.now() - this.touchStartMs < LONG_PRESS_MS) {
      if (isRevealedState(cell.state)) {
        const neighbours = this.adjacentCells(row, col);
        const flags = neighbours.filter((n) => n.state === CellState.Flagged).length;
        if (flags !== 0 && flags === cell.digit) this.revealNeighbours(neighbours);
      } else {
        undoPressCell(cell);
        this.handleFlag(cell);
      }
    } else if (isRevealedState(cell.state)) {
      undoPressCell(cell);
    } else {
      releaseCell(cell);
      if (isRevealedState(cell.state)) this.onCellRevealed(cell);
    }
    this.touchPos = [NO_POS, NO_POS];
    this.emit('changed');
  }

  touchCancel() {
    const [touchRow, touchCol] = this.touchPos;
    if (touchRow !== NO_POS) undoPressCell(this.cellAt(touchRow, touchCol));
    this.touchPos = [NO_POS, NO_POS];
    this.emit('changed');
  }
}

// Border ring for a (rows+2) x (cols+2) grid (0x1206d0).
const getBorderLayout = (rows, cols) => {
  const borders = [];
  for (let row = 0; row < rows + 2; row++) {
    for (let col = 0; col < cols + 2; col++) {
      const type = getBorderType(row, col, rows, cols);
      if (type !== null) borders.push({ row, col, type });
    }
  }
  return borders;
};

const getBorderType = (row, col, rows, cols) => {
  const isTop = row === 0;
  const isBottom = row === rows + 1;
  const isLeft = col === 0;
  const isRight = col === cols + 1;
  if (isTop) return isLeft ? BorderElement.CornerNW : isRight ? BorderElement.CornerNE : BorderElement.North;
  if (isBottom) return isLeft ? BorderElement.CornerSW : isRight ? BorderElement.CornerSE : BorderElement.South;
  if (isLeft) return BorderElement.West;
  if (isRight) return BorderElement.East;
  return null;
};
