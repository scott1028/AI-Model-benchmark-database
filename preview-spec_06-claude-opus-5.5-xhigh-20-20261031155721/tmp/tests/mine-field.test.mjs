// Behavior tests for the ported MineField logic (dev by-product, not shipped).
import assert from 'node:assert/strict';
import { test } from 'node:test';
import { Button, CellState, DEFAULT_SETTINGS, PlaceFlagOn } from '../../webgl/src/constants.js';
import { MineField } from '../../webgl/src/components/MineField.js';

const createRandom = (seed) => {
  let x = seed >>> 0;
  return (n) => {
    x = (Math.imul(x, 1664525) + 1013904223) >>> 0;
    return Math.floor((x / 2 ** 32) * n);
  };
};

const createField = ({ settings = {}, seed = 1, now = () => 0 } = {}) => {
  const events = [];
  const merged = { ...DEFAULT_SETTINGS, ...settings };
  const field = new MineField({
    getSettings: () => merged,
    randomInt: createRandom(seed),
    now: () => now(),
    listener: {
      flaggedMinesCountChanged: (n) => events.push(['flags', n]),
      firstClickDone: () => events.push(['first']),
      gameOver: (won) => events.push(['over', won]),
    },
  });
  return { field, events };
};

const leftClick = (field, row, col) => {
  field.mousePress(row, col, Button.Left, Button.Left);
  field.mouseRelease(row, col, Button.Left, 0);
};

const rightClick = (field, row, col) => {
  field.mousePress(row, col, Button.Right, Button.Right);
  field.mouseRelease(row, col, Button.Right, 0);
};

const indexOf = (field, cell) => field.cells.indexOf(cell);
const posOf = (field, cell) => [Math.floor(indexOf(field, cell) / field.cols), indexOf(field, cell) % field.cols];

test('mines are clamped to rows*cols-10', () => {
  const { field } = createField();
  field.initField(5, 5, 100);
  assert.equal(field.minesCount, 15);
});

test('first click is safe for the cell and its neighbours, digits are correct', () => {
  for (let seed = 1; seed <= 30; seed++) {
    const { field, events } = createField({ seed });
    field.initField(16, 30, 99);
    leftClick(field, 7, 12);
    assert.deepEqual(events.filter((e) => e[0] === 'first'), [['first']]);
    assert.equal(field.cells.filter((c) => c.hasMine).length, 99);
    assert.equal(field.cellAt(7, 12).hasMine, false);
    for (const n of field.adjacentCells(7, 12)) assert.equal(n.hasMine, false);
    field.cells.forEach((cell, idx) => {
      if (cell.hasMine) return;
      const expected = field.adjacentCells(Math.floor(idx / 30), idx % 30).filter((n) => n.hasMine).length;
      assert.equal(cell.digit, expected);
    });
    // Clicked cell has digit 0, so the flood fill opened more than one cell.
    assert.equal(field.cellAt(7, 12).state, CellState.Revealed);
    assert.ok(field.cells.filter((c) => c.state === CellState.Revealed).length > 1);
  }
});

test('right click cycles flag -> question -> released and updates the counter', () => {
  const { field, events } = createField();
  field.initField(9, 9, 10);
  rightClick(field, 0, 0);
  assert.equal(field.cellAt(0, 0).state, CellState.Flagged);
  rightClick(field, 0, 0);
  assert.equal(field.cellAt(0, 0).state, CellState.Questioned);
  rightClick(field, 0, 0);
  assert.equal(field.cellAt(0, 0).state, CellState.Released);
  assert.deepEqual(events.filter((e) => e[0] === 'flags').map((e) => e[1]), [0, 1, 0]);
});

test('without question marks a flag goes straight back to released', () => {
  const { field } = createField({ settings: { useQuestionMarks: false } });
  field.initField(9, 9, 10);
  rightClick(field, 0, 0);
  rightClick(field, 0, 0);
  assert.equal(field.cellAt(0, 0).state, CellState.Released);
});

test('flag on mouse press when PlaceFlagOn=MousePress', () => {
  const { field } = createField({ settings: { placeFlagOn: PlaceFlagOn.MousePress } });
  field.initField(9, 9, 10);
  field.mousePress(2, 2, Button.Right, Button.Right);
  assert.equal(field.cellAt(2, 2).state, CellState.Flagged);
  field.mouseRelease(2, 2, Button.Right, 0);
  assert.equal(field.cellAt(2, 2).state, CellState.Flagged);
});

test('revealing a mine loses: mines shown, wrong flags become Error', () => {
  const { field, events } = createField({ seed: 7 });
  field.initField(9, 9, 10);
  leftClick(field, 4, 4);
  const mine = field.cells.find((c) => c.hasMine);
  const safeHidden = field.cells.find((c) => !c.hasMine && c.state === CellState.Released);
  rightClick(field, ...posOf(field, safeHidden));
  leftClick(field, ...posOf(field, mine));
  assert.deepEqual(events.at(-1), ['over', false]);
  assert.equal(field.gameOver, true);
  assert.equal(mine.exploded, true);
  assert.equal(safeHidden.state, CellState.Error);
  for (const c of field.cells.filter((x) => x.hasMine)) assert.equal(c.state, CellState.Revealed);
});

test('revealing every safe cell wins and auto-flags remaining mines', () => {
  const { field, events } = createField({ seed: 3 });
  field.initField(9, 9, 10);
  leftClick(field, 4, 4);
  for (const cell of field.cells) {
    if (!cell.hasMine && cell.state !== CellState.Revealed) leftClick(field, ...posOf(field, cell));
  }
  assert.deepEqual(events.at(-1), ['over', true]);
  assert.deepEqual(events.at(-2), ['flags', 10]);
  for (const c of field.cells.filter((x) => x.hasMine)) assert.equal(c.state, CellState.Flagged);
});

test('middle click on a satisfied number reveals its neighbours', () => {
  const { field } = createField({ seed: 11 });
  field.initField(9, 9, 10);
  leftClick(field, 4, 4);
  // Find a revealed number with hidden neighbours, flag its mines, then chord.
  const target = field.cells.find((c, idx) => c.state === CellState.Revealed && c.digit > 0
    && field.adjacentCells(Math.floor(idx / 9), idx % 9).some((n) => !n.hasMine && n.state === CellState.Released));
  assert.ok(target);
  const [row, col] = posOf(field, target);
  for (const n of field.adjacentCells(row, col)) if (n.hasMine) rightClick(field, ...posOf(field, n));
  field.mousePress(row, col, Button.Middle, Button.Middle);
  field.mouseRelease(row, col, Button.Middle, 0);
  for (const n of field.adjacentCells(row, col)) {
    if (!n.hasMine) assert.equal(n.state, CellState.Revealed);
  }
});

test('touch: short tap flags, long press reveals', () => {
  let t = 0;
  const { field } = createField({ seed: 5, now: () => t });
  field.initField(9, 9, 10);
  field.touchBegin(4, 4);
  field.touchEnd(4, 4); // first touch always reveals
  assert.equal(field.cellAt(4, 4).state, CellState.Revealed);
  const hidden = field.cells.find((c) => c.state === CellState.Released && !c.hasMine);
  const [row, col] = posOf(field, hidden);
  field.touchBegin(row, col);
  t += 200;
  field.touchEnd(row, col);
  assert.equal(hidden.state, CellState.Flagged);
  field.touchBegin(row, col);
  t += 200;
  field.touchEnd(row, col); // flagged -> questioned
  field.touchBegin(row, col);
  t += 200;
  field.touchEnd(row, col); // questioned -> released
  field.touchBegin(row, col);
  t += 1500;
  field.touchEnd(row, col);
  assert.equal(hidden.state, CellState.Revealed);
});

test('reset keeps the same mines and closes every cell', () => {
  const { field } = createField({ seed: 9 });
  field.initField(9, 9, 10);
  leftClick(field, 4, 4);
  const before = field.cells.map((c) => c.hasMine);
  const mine = field.cells.find((c) => c.hasMine);
  leftClick(field, ...posOf(field, mine));
  field.resetField();
  assert.deepEqual(field.cells.map((c) => c.hasMine), before);
  assert.ok(field.cells.every((c) => c.state === CellState.Released && !c.exploded));
  assert.equal(field.gameOver, false);
  assert.equal(field.firstClick, false);
});

test('border ring has 2*(rows+cols)+4 pieces with corners', () => {
  const { field } = createField();
  field.initField(16, 30, 99);
  assert.equal(field.borders.length, 2 * (16 + 30) + 4);
  const at = (r, c) => field.borders.find((b) => b.row === r && b.col === c).type;
  assert.deepEqual([at(0, 0), at(0, 31), at(17, 0), at(17, 31)], [4, 6, 5, 7]);
});
