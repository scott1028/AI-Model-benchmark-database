// Small helpers shared by the app.

// KGameClock::FlexibleHourMinSec: "MM:SS", "H:MM:SS" once an hour passed.
export const formatClock = (totalSeconds) => {
  const h = Math.floor(totalSeconds / 3600);
  const m = Math.floor((totalSeconds % 3600) / 60);
  const s = totalSeconds % 60;
  const mmss = `${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')}`;
  return h > 0 ? `${h}:${mmss}` : mmss;
};

export const loadJSON = (key, fallback) => {
  try {
    const raw = localStorage.getItem(key);
    return raw ? JSON.parse(raw) : fallback;
  } catch {
    return fallback;
  }
};

export const saveJSON = (key, value) => {
  try {
    localStorage.setItem(key, JSON.stringify(value));
  } catch {
    // Storage may be blocked (private mode); settings stay in memory.
  }
};

// KMinesScene::resizeScene (0x120020): integer cell size, field centered.
export const getFieldLayout = (width, height, rows, cols) => {
  const w = cols + 2;
  const h = rows + 2;
  const cellSize = Math.max(1, w / width > h / height ? Math.trunc(width / w) : Math.trunc(height / h));
  return {
    cellSize,
    x: width / 2 - (cellSize * w) / 2,
    y: height / 2 - (cellSize * h) / 2,
    width: cellSize * w,
    height: cellSize * h,
  };
};

// MineFieldItem mouse handlers: int(pos / cellSize) - 1 (border ring offset).
export const getCellAt = (layout, px, py) => [
  Math.trunc((py - layout.y) / layout.cellSize) - 1,
  Math.trunc((px - layout.x) / layout.cellSize) - 1,
];

export const isInsideRect = (rect, px, py) =>
  px >= rect.x && py >= rect.y && px < rect.x + rect.width && py < rect.y + rect.height;

export const getRandomInt = (n) => {
  const buf = new Uint32Array(1);
  crypto.getRandomValues(buf);
  // Same bounded() mapping as QRandomGenerator: (r * n) >> 32
  return Math.floor((buf[0] * n) / 2 ** 32);
};
