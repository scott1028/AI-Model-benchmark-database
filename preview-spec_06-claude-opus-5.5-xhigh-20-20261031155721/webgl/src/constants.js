// Values recovered from /usr/games/kmines (see ../../report/README.md).

// CellItem+0x28
export const CellState = Object.freeze({
  Released: 0,
  Pressed: 1,
  Revealed: 2,
  Questioned: 3,
  Flagged: 4,
  Error: 5,
  Hint: 6,
});

// BorderItem+0x28
export const BorderElement = Object.freeze({
  North: 0,
  South: 1,
  East: 2,
  West: 3,
  CornerNW: 4,
  CornerSW: 5,
  CornerNE: 6,
  CornerSE: 7,
});

// CellItem::fillNameHashes (0x118810)
export const STATE_SPRITES = Object.freeze({
  [CellState.Released]: ['cell_up'],
  [CellState.Pressed]: ['cell_down'],
  [CellState.Revealed]: ['cell_down'],
  [CellState.Questioned]: ['cell_up', 'question'],
  [CellState.Flagged]: ['cell_up', 'flag'],
  [CellState.Error]: ['cell_down', 'mine', 'error'],
  [CellState.Hint]: ['cell_up', 'hint'],
});

export const DIGIT_SPRITES = Object.freeze([
  null, 'arabicOne', 'arabicTwo', 'arabicThree', 'arabicFour',
  'arabicFive', 'arabicSix', 'arabicSeven', 'arabicEight',
]);

// BorderItem names (0x1206d0)
export const BORDER_SPRITES = Object.freeze({
  [BorderElement.North]: 'border.edge.north',
  [BorderElement.South]: 'border.edge.south',
  [BorderElement.East]: 'border.edge.east',
  [BorderElement.West]: 'border.edge.west',
  [BorderElement.CornerNE]: 'border.outsideCorner.ne',
  [BorderElement.CornerNW]: 'border.outsideCorner.nw',
  [BorderElement.CornerSW]: 'border.outsideCorner.sw',
  [BorderElement.CornerSE]: 'border.outsideCorner.se',
});

export const ALL_SPRITES = Object.freeze([
  'mainWidget', 'cell_up', 'cell_down', 'question', 'flag', 'mine', 'error',
  'hint', 'explosion', ...DIGIT_SPRITES.slice(1), ...Object.values(BORDER_SPRITES),
]);

// KMinesMainWindow::newGame (0x116660); level ids are KGameDifficulty values.
export const LEVELS = Object.freeze([
  { id: 'easy', label: 'Easy', rows: 9, cols: 9, mines: 10 },
  { id: 'medium', label: 'Medium', rows: 16, cols: 16, mines: 40 },
  { id: 'hard', label: 'Hard', rows: 16, cols: 30, mines: 99 },
  { id: 'custom', label: 'Custom' },
]);

// Qt::MouseButton values, used by MineField input methods.
export const Button = Object.freeze({ Left: 1, Right: 2, Middle: 4 });

export const PlaceFlagOn = Object.freeze({ MouseRelease: 0, MousePress: 1 });

// Settings::Settings (0x11e030)
export const DEFAULT_SETTINGS = Object.freeze({
  useQuestionMarks: true,
  allowKminesReset: true,
  disableScoreOnReset: false,
  exploreWithLeftClickOnNumberCells: false,
  placeFlagOn: PlaceFlagOn.MouseRelease,
  customWidth: 10,
  customHeight: 10,
  customMines: 20,
  theme: 'default',
});

export const CUSTOM_LIMITS = Object.freeze({ minSize: 5, maxSize: 50, minMines: 1 });

// Installed theme descriptors in /usr/share/kmines/themes/*.desktop
export const THEMES = Object.freeze([
  { id: 'default', name: 'Default', file: 'kmines_oxygen.svg', preview: 'default_preview.png' },
  { id: 'classic', name: 'Classic', file: 'kmines_classic.svg', preview: 'classic_preview.png' },
  { id: 'clean_blue', name: 'Clean Blue', file: 'kmines_clean_blue.svg', preview: 'clean_blue_preview.png' },
  { id: 'clean_blue_dark', name: 'Clean Blue Dark', file: 'kmines_clean_blue_dark.svg', preview: 'clean_blue_dark_preview.png' },
  { id: 'graveyard-mayhem', name: 'Graveyard Mayhem', file: 'graveyard-mayhem.svg', preview: 'graveyard-mayhem-preview.png' },
  { id: 'green', name: 'Gardens of Danger', file: 'kmines_green.svg', preview: 'green.png' },
]);

// KMinesScene popups (0x11a610): win/lose 4000 ms, opacity 0.9.
export const MESSAGE_TIMEOUT_MS = 4000;
export const POPUP_OPACITY = 0.9;
// MineFieldItem::sceneEvent (0x11ee50): long press threshold.
export const LONG_PRESS_MS = 1000;
export const HIGHSCORE_LIMIT = 10;
