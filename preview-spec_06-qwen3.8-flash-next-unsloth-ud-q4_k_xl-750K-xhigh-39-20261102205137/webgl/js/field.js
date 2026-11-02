// Port of MineFieldItem + CellItem (see report/README.md §3 and report/evidence/).
//
// Behaviour recovered from /usr/games/kmines and reproduced 1:1:
//   * CellState numbering as found in the vtable/state machine
//   * first click generates mines while avoiding the clicked cell + its 8 neighbours
//   * mine count is capped to width*height-10
//   * flag cycle Covered -> Flagged -> Marked(optional) -> Covered
//   * flood fill over cells with 0 surrounding mines
//   * chord/"explore" when the flags around a shown cell equal its number
//   * on loss: unflagged mines are shown, wrong flags get the "error" state;
//     on win: remaining marked cells are unmarked and covered cells get flagged

export const CellState = {
  Covered: 0,
  Pressed: 1,
  Shown: 2,
  Marked: 3,
  Flagged: 4,
  Boom: 5,
  Hint: 6,
};

export class Cell {
  constructor(row, col) {
    this.row = row;
    this.col = col;
    this.state = CellState.Covered;
    this.isMine = false;
    this.isBoom = false;            // the mine that was stepped on -> "explosion"
    this.surroundingMines = 0;
  }
}

export class MineField {
  constructor(settings) {
    this.settings = settings;
    this.width = 0;
    this.height = 0;
    this.minesTotal = 0;
    this.cells = [];
    this.hiddenCellsLeft = 0;
    this.flagsPlaced = 0;
    this.needGenerateMines = true;
    this.isGameOver = false;
    this.listeners = {};
  }

  on(signal, handler) {
    (this.listeners[signal] ||= []).push(handler);
  }

  emit(signal, ...args) {
    for (const handler of this.listeners[signal] || []) {
      handler(...args);
    }
  }

  setFieldSize(height, width, mines) {
    this.height = height;
    this.width = width;
    // updateFieldItems(): never fill the board, keep at least 10 safe cells
    this.minesTotal = Math.max(1, Math.min(mines, height * width - 10));
    this.cells = [];
    for (let row = 0; row < height; row++) {
      for (let col = 0; col < width; col++) {
        this.cells.push(new Cell(row, col));
      }
    }
    this.newGame();
  }

  newGame() {
    for (const cell of this.cells) {
      cell.state = CellState.Covered;
      cell.isMine = false;
      cell.isBoom = false;
      cell.surroundingMines = 0;
    }
    this.hiddenCellsLeft = this.width * this.height;
    this.flagsPlaced = 0;
    this.needGenerateMines = true;
    this.isGameOver = false;
    this.emit('flaggedMinesCountChanged', this.flagsPlaced);
  }

  // "kmines_reset" after a lost game (KMinesMainWindow::onGameOver, reset branch)
  resetField() {
    for (const cell of this.cells) {
      cell.state = CellState.Covered;
      cell.isBoom = false;
    }
    this.hiddenCellsLeft = this.width * this.height;
    this.flagsPlaced = 0;
    this.needGenerateMines = false;
    this.isGameOver = false;
    this.emit('flaggedMinesCountChanged', this.flagsPlaced);
  }

  getCell(row, col) {
    if (row < 0 || col < 0 || row >= this.height || col >= this.width) {
      return null;
    }
    return this.cells[row * this.width + col];
  }

  cellsAround(row, col) {
    const around = [];
    for (let r = row - 1; r <= row + 1; r++) {
      for (let c = col - 1; c <= col + 1; c++) {
        if (r === row && c === col) continue;
        const cell = this.getCell(r, c);
        if (cell) around.push(cell);
      }
    }
    return around;
  }

  generateMines(clicked) {
    const avoid = new Set(this.cellsAround(clicked.row, clicked.col));
    avoid.add(clicked);
    let toPlace = this.minesTotal;
    while (toPlace > 0) {
      const cell = this.cells[Math.floor(Math.random() * this.cells.length)];
      if (cell.isMine || avoid.has(cell)) continue;
      cell.isMine = true;
      toPlace--;
      for (const neighbour of this.cellsAround(cell.row, cell.col)) {
        if (!neighbour.isMine) neighbour.surroundingMines++;
      }
    }
    this.emit('firstClickDone');
  }

  // CellItem::switchState + the counter update in MineFieldItem::toggleCellState
  switchCellState(cell) {
    const wasFlagged = cell.state === CellState.Flagged;
    switch (cell.state) {
      case CellState.Marked:
        cell.state = CellState.Covered;
        break;
      case CellState.Flagged:
        cell.state = this.settings.useQuestionMarks ? CellState.Marked : CellState.Covered;
        break;
      case CellState.Covered:
        cell.state = CellState.Flagged;
        break;
      default:
        break;
    }
    const isFlagged = cell.state === CellState.Flagged;
    if (isFlagged !== wasFlagged) {
      this.flagsPlaced += isFlagged ? 1 : -1;
      this.emit('flaggedMinesCountChanged', this.flagsPlaced);
    }
  }

  // CellItem::reveal (FUN_00118790)
  revealCell(cell) {
    const state = cell.state;
    if (state === CellState.Marked || state === CellState.Flagged) return;
    cell.isBoom = cell.isMine;
    if (state === CellState.Pressed || (state !== CellState.Shown && state !== CellState.Boom)) {
      cell.state = CellState.Shown;
    }
  }

  // MineFieldItem::generateCellsAround (FUN_00118690)
  floodFrom(row, col) {
    for (const cell of this.cellsAround(row, col)) {
      if (cell.state === CellState.Shown || cell.state === CellState.Marked ||
          cell.state === CellState.Flagged || cell.state === CellState.Boom) continue;
      cell.state = CellState.Shown;
      this.hiddenCellsLeft--;
      if (cell.surroundingMines === 0) {
        this.floodFrom(cell.row, cell.col);
      }
    }
  }

  // MineFieldItem::checkField (FUN_001197f0); true = game finished
  checkField(row, col) {
    this.hiddenCellsLeft--;
    const cell = this.getCell(row, col);

    if (cell.isMine) {
      for (const other of this.cells) {
        const otherFlagged = other.state === CellState.Flagged;
        if (other.isMine === otherFlagged) continue;      // correctly flagged / irrelevant
        if (other.state !== CellState.Shown && other.state !== CellState.Boom) {
          other.state = otherFlagged ? CellState.Boom : CellState.Shown;
        }
        this.hiddenCellsLeft--;
      }
      this.isGameOver = true;
      this.emit('gameOver', false);
      return true;
    }

    if (cell.surroundingMines === 0) {
      this.floodFrom(row, col);
    }
    if (this.cells.some((other) => other.isBoom && other.state === CellState.Shown)) {
      this.isGameOver = true;
      this.emit('gameOver', false);
      return true;
    }
    if (this.hiddenCellsLeft === this.minesTotal) {
      for (const other of this.cells) {
        if (other.state === CellState.Marked) this.switchCellState(other);
        if (other.state === CellState.Flagged || other.state === CellState.Boom) continue;
        if (other.state !== CellState.Shown) this.switchCellState(other);
      }
      this.isGameOver = true;
      this.emit('flaggedMinesCountChanged', this.flagsPlaced);
      this.emit('gameOver', true);
      return true;
    }
    return false;
  }

  pressCell(row, col) {
    const cell = this.getCell(row, col);
    if (cell && cell.state === CellState.Covered) {
      cell.state = CellState.Pressed;
    }
    return cell;
  }

  unpressCell(row, col) {
    const cell = this.getCell(row, col);
    if (cell && cell.state === CellState.Pressed) {
      cell.state = CellState.Covered;
    }
    return cell;
  }

  previewAround(row, col) {
    for (const cell of this.cellsAround(row, col)) {
      if (cell.state === CellState.Covered) cell.state = CellState.Pressed;
    }
  }

  unpressAround(row, col) {
    for (const cell of this.cellsAround(row, col)) {
      if (cell.state === CellState.Pressed) cell.state = CellState.Covered;
    }
  }

  isShownCell(cell) {
    return cell !== null && (cell.state === CellState.Shown || cell.state === CellState.Boom);
  }

  // middle button / both buttons: "explore" around a shown number (FUN_0011fa60)
  explore(row, col) {
    const cell = this.getCell(row, col);
    if (!this.isShownCell(cell)) {
      this.unpressAround(row, col);
      return false;
    }
    const around = this.cellsAround(row, col);
    const flags = around.filter((other) => other.state === CellState.Flagged).length;
    if (flags === 0 || flags !== cell.surroundingMines) {
      this.unpressAround(row, col);
      return false;
    }
    for (const neighbour of around) {
      if (neighbour.state === CellState.Shown || neighbour.state === CellState.Boom) continue;
      this.revealCell(neighbour);
      if (this.isShownCell(neighbour) && this.checkField(neighbour.row, neighbour.col)) break;
    }
    return true;
  }

  // left button on a covered cell (FUN_0011fa60 / FUN_0011ee50)
  dig(row, col) {
    const cell = this.getCell(row, col);
    if (!cell || this.isShownCell(cell)) return;
    if (this.needGenerateMines) {
      this.needGenerateMines = false;
      this.generateMines(cell);
    }
    this.revealCell(cell);
    if (this.isShownCell(cell)) {
      this.checkField(row, col);
    }
  }
}
