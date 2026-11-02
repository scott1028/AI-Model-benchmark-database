// Procedural theme sprites for the WebGL port.
//
// The original loads an SVG theme (for example
// /usr/share/kmines/themes/kmines_oxygen.svgz) and asks KGameRenderer to
// rasterise the element whose id equals the sprite key ("cell_up", "flag",
// "arabicFive", "border.edge.north", …) at the requested size.  Downloading
// assets is not allowed for this task, so every id the binary asks for is drawn
// here with Canvas2D into the atlas that the WebGL renderer then samples.
//
// Colours follow the values measured in the installed themes
// (see report/evidence/theme-*.txt):
//   default (kmines_oxygen): cell_up #ffffff/#e6e6e6/#808080/#666666,
//     cell_down #e6e6e6/#dddddd with a #ffccaa tint, error #ce0000,
//     hint #00be00, explosion #ffca57,
//     arabicOne..Eight #000081 #008100 #806600 #681957 #bc0000 #8c0000 #b35800 #323232
//   classic: #c0c0c0 tiles with a white/black 3D bevel.

// CellItem::updatePixmap: surrounding-mines count -> number sprite (FUN_00118810).
export const NUMBER_SPRITES = [
  'arabicOne', 'arabicTwo', 'arabicThree', 'arabicFour',
  'arabicFive', 'arabicSix', 'arabicSeven', 'arabicEight',
];

// CellState -> sprite keys drawn for a cell, base first then overlays.
export const STATE_SPRITES = [
  ['cell_up'],                     // 0 Covered
  ['cell_down'],                   // 1 Pressed (mouse held down)
  ['cell_down'],                   // 2 Shown
  ['cell_up', 'question'],         // 3 Marked
  ['cell_up', 'flag'],             // 4 Flagged
  ['cell_down', 'mine', 'error'],  // 5 Boom (wrong flag after a loss)
  ['cell_up', 'hint'],             // 6 Hint (sprite exists, unused by this build)
];

// Index == BorderItem type, in the order MineFieldItem::updateFieldItems uses.
export const BORDER_SPRITES = [
  'border.edge.south', 'border.edge.north', 'border.edge.east', 'border.edge.west',
  'border.outsideCorner.nw', 'border.outsideCorner.sw', 'border.outsideCorner.se',
  'border.outsideCorner.ne',
];

const THEMES = {
  default: {
    label: 'Default (oxygen)',
    canvasBg: '#4c4c50',
    tile: ['#dedede', '#b4b4b4'],
    tileBevelLight: '#ffffff',
    tileBevelDark: '#8c8c8c',
    tileOutline: '#6f6f6f',
    shown: '#f4efe9',
    shownEdge: '#cfc7bd',
    shownTint: '#ffccaa',
    panel: ['#9a9a9a', '#6f6f6f'],
    panelLight: '#c2c2c2',
    panelDark: '#3f3f3f',
    number: ['#000081', '#008100', '#806600', '#681957', '#bc0000', '#8c0000', '#b35800', '#323232'],
    flag: '#d40000',
    flagPole: '#2f2f2f',
    mine: '#141414',
    mineShine: '#ffffff',
    explosion: '#ffca57',
    error: '#ce0000',
    hint: '#00be00',
    question: '#4a4a4a',
  },
  classic: {
    label: 'Classic',
    canvasBg: '#6e6e6e',
    tile: ['#c0c0c0', '#c0c0c0'],
    tileBevelLight: '#ffffff',
    tileBevelDark: '#404040',
    tileOutline: '#000000',
    shown: '#ffffff',
    shownEdge: '#9a9a9a',
    shownTint: '#ffffff',
    panel: ['#c0c0c0', '#9a9a9a'],
    panelLight: '#ffffff',
    panelDark: '#4a4a4a',
    number: ['#0000ff', '#007b00', '#ff0000', '#00007b', '#7b0000', '#007b7b', '#000000', '#7b7b7b'],
    flag: '#ff0000',
    flagPole: '#000000',
    mine: '#000000',
    mineShine: '#c0c0c0',
    explosion: '#ff8000',
    error: '#ff0000',
    hint: '#008000',
    question: '#000000',
  },
  clean_blue: {
    label: 'Clean Blue',
    canvasBg: '#c8d6e4',
    tile: ['#f7fbff', '#bcd0e4'],
    tileBevelLight: '#ffffff',
    tileBevelDark: '#7d97b0',
    tileOutline: '#5b758d',
    shown: '#eaf1f8',
    shownEdge: '#b6c6d6',
    shownTint: '#9fc4e8',
    panel: ['#b9cadd', '#8ba4bf'],
    panelLight: '#eef4fa',
    panelDark: '#556b80',
    number: ['#1a4fb4', '#137a3c', '#8a5a00', '#6a2c91', '#c62828', '#8e1b1b', '#b35800', '#37474f'],
    flag: '#e53935',
    flagPole: '#37474f',
    mine: '#263238',
    mineShine: '#ffffff',
    explosion: '#ffb300',
    error: '#d32f2f',
    hint: '#2e7d32',
    question: '#455a64',
  },
  clean_blue_dark: {
    label: 'Clean Blue Dark',
    canvasBg: '#12171d',
    tile: ['#3b4756', '#232c37'],
    tileBevelLight: '#5b6b7d',
    tileBevelDark: '#131a22',
    tileOutline: '#0c1117',
    shown: '#1d242d',
    shownEdge: '#39434f',
    shownTint: '#4a6b8a',
    panel: ['#2f3a46', '#1e262f'],
    panelLight: '#4b5b6b',
    panelDark: '#0a0e12',
    number: ['#79b8ff', '#7ee08a', '#f2c14e', '#d9a3ff', '#ff7b72', '#ff9d9d', '#ffb066', '#cfd8dc'],
    flag: '#ff6b6b',
    flagPole: '#cfd8dc',
    mine: '#f2f5f7',
    mineShine: '#7f8c99',
    explosion: '#ffd54f',
    error: '#ff5252',
    hint: '#69f0ae',
    question: '#b0bec5',
  },
  green: {
    label: 'Gardens of Danger',
    canvasBg: '#2f4a25',
    tile: ['#93d16d', '#5d9440'],
    tileBevelLight: '#cbf2a6',
    tileBevelDark: '#2f5a20',
    tileOutline: '#22421a',
    shown: '#c39a6b',
    shownEdge: '#7d5734',
    shownTint: '#8a5f38',
    panel: ['#6aa84a', '#3c6a28'],
    panelLight: '#93d16d',
    panelDark: '#1f3a14',
    number: ['#1a237e', '#0d3d10', '#4e342e', '#4a148c', '#b71c1c', '#004d40', '#bf360c', '#212121'],
    flag: '#e53935',
    flagPole: '#33261a',
    mine: '#2b2b2b',
    mineShine: '#ffffff',
    explosion: '#ffca28',
    error: '#c62828',
    hint: '#2e7d32',
    question: '#33302b',
  },
  graveyard: {
    label: 'Graveyard Mayhem',
    canvasBg: '#17141c',
    tile: ['#4a4356', '#2c2836'],
    tileBevelLight: '#6f6780',
    tileBevelDark: '#171320',
    tileOutline: '#0e0b13',
    shown: '#2a2531',
    shownEdge: '#443c50',
    shownTint: '#6d5b8a',
    panel: ['#3a3244', '#241f2d'],
    panelLight: '#5f5570',
    panelDark: '#0d0a12',
    number: ['#9fa8da', '#a5d6a7', '#ffcc80', '#ce93d8', '#ef9a9a', '#80cbc4', '#ffab91', '#e0e0e0'],
    flag: '#b71c1c',
    flagPole: '#d7ccc8',
    mine: '#efebe9',
    mineShine: '#8d6e63',
    explosion: '#ff7043',
    error: '#e53935',
    hint: '#66bb6a',
    question: '#bcaaa4',
  },
};

export const THEME_NAMES = Object.keys(THEMES);
export const themeLabel = (name) => THEMES[name].label;
export const themePalette = (name) => THEMES[name] || THEMES.default;

const hexToRgb01 = (hex) => [
  parseInt(hex.slice(1, 3), 16) / 255,
  parseInt(hex.slice(3, 5), 16) / 255,
  parseInt(hex.slice(5, 7), 16) / 255,
];

export const themeClearColor = (name) => hexToRgb01(themePalette(name).canvasBg);

const fillRect = (ctx, x, y, w, h, color) => {
  ctx.fillStyle = color;
  ctx.fillRect(x, y, w, h);
};

const drawBevelledTile = (ctx, s, p) => {
  const b = Math.max(2, Math.round(s * 0.09));
  const grad = ctx.createLinearGradient(0, 0, 0, s);
  grad.addColorStop(0, p.tile[0]);
  grad.addColorStop(1, p.tile[1]);
  ctx.fillStyle = grad;
  ctx.fillRect(0, 0, s, s);
  fillRect(ctx, 0, 0, s, b, p.tileBevelLight);
  fillRect(ctx, 0, 0, b, s, p.tileBevelLight);
  fillRect(ctx, 0, s - b, s, b, p.tileBevelDark);
  fillRect(ctx, s - b, 0, b, s, p.tileBevelDark);
  ctx.strokeStyle = p.tileOutline;
  ctx.lineWidth = Math.max(1, s * 0.025);
  ctx.strokeRect(ctx.lineWidth / 2, ctx.lineWidth / 2, s - ctx.lineWidth, s - ctx.lineWidth);
};

const drawShownCell = (ctx, s, p) => {
  fillRect(ctx, 0, 0, s, s, p.shown);
  ctx.fillStyle = p.shownTint;
  ctx.globalAlpha = 0.16;
  ctx.fillRect(0, 0, s, s * 0.4);
  ctx.globalAlpha = 1;
  ctx.strokeStyle = p.shownEdge;
  ctx.lineWidth = Math.max(1, s * 0.05);
  ctx.strokeRect(ctx.lineWidth / 2, ctx.lineWidth / 2, s - ctx.lineWidth, s - ctx.lineWidth);
};

const drawFlag = (ctx, s, p) => {
  const poleW = Math.max(2, s * 0.07);
  ctx.fillStyle = p.flagPole;
  ctx.fillRect(s * 0.46 - poleW / 2, s * 0.18, poleW, s * 0.54);
  ctx.fillRect(s * 0.26, s * 0.72, s * 0.48, Math.max(2, s * 0.07));
  ctx.beginPath();
  ctx.moveTo(s * 0.24, s * 0.8);
  ctx.lineTo(s * 0.76, s * 0.8);
  ctx.lineTo(s * 0.68, s * 0.9);
  ctx.lineTo(s * 0.32, s * 0.9);
  ctx.closePath();
  ctx.fill();
  ctx.beginPath();
  ctx.moveTo(s * 0.5, s * 0.16);
  ctx.lineTo(s * 0.86, s * 0.32);
  ctx.lineTo(s * 0.5, s * 0.48);
  ctx.closePath();
  ctx.fillStyle = p.flag;
  ctx.fill();
};

const drawGlyph = (ctx, s, color, glyph, scale) => {
  ctx.fillStyle = color;
  ctx.font = `bold ${Math.round(s * scale)}px "DejaVu Sans", "Noto Sans", sans-serif`;
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  ctx.fillText(glyph, s / 2, s * 0.53);
};

const drawMine = (ctx, s, p) => {
  const cx = s * 0.5;
  const cy = s * 0.55;
  const r = s * 0.26;
  ctx.strokeStyle = p.mine;
  ctx.lineWidth = Math.max(1.5, s * 0.055);
  for (let i = 0; i < 8; i++) {
    const a = (Math.PI / 4) * i;
    ctx.beginPath();
    ctx.moveTo(cx + Math.cos(a) * r * 0.75, cy + Math.sin(a) * r * 0.75);
    ctx.lineTo(cx + Math.cos(a) * r * 1.6, cy + Math.sin(a) * r * 1.6);
    ctx.stroke();
  }
  ctx.fillStyle = p.mine;
  ctx.beginPath();
  ctx.arc(cx, cy, r, 0, Math.PI * 2);
  ctx.fill();
  ctx.fillStyle = p.mineShine;
  ctx.globalAlpha = 0.8;
  ctx.beginPath();
  ctx.arc(cx - r * 0.34, cy - r * 0.36, r * 0.24, 0, Math.PI * 2);
  ctx.fill();
  ctx.globalAlpha = 1;
};

const drawExplosion = (ctx, s, p) => {
  const cx = s * 0.5;
  const cy = s * 0.53;
  ctx.fillStyle = p.explosion;
  ctx.beginPath();
  for (let i = 0; i < 20; i++) {
    const a = (Math.PI / 10) * i;
    const rad = i % 2 === 0 ? s * 0.44 : s * 0.16;
    const x = cx + Math.cos(a) * rad;
    const y = cy + Math.sin(a) * rad;
    if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
  }
  ctx.closePath();
  ctx.fill();
  ctx.fillStyle = p.mineShine;
  ctx.globalAlpha = 0.55;
  ctx.beginPath();
  ctx.arc(cx, cy, s * 0.14, 0, Math.PI * 2);
  ctx.fill();
  ctx.globalAlpha = 1;
};

const drawCross = (ctx, s, p) => {
  ctx.strokeStyle = p.error;
  ctx.lineCap = 'round';
  ctx.lineWidth = Math.max(2, s * 0.13);
  const m = s * 0.26;
  ctx.beginPath();
  ctx.moveTo(m, m); ctx.lineTo(s - m, s - m);
  ctx.moveTo(s - m, m); ctx.lineTo(m, s - m);
  ctx.stroke();
};

const drawCheck = (ctx, s, p) => {
  ctx.strokeStyle = p.hint;
  ctx.lineCap = 'round';
  ctx.lineWidth = Math.max(2, s * 0.13);
  ctx.beginPath();
  ctx.moveTo(s * 0.24, s * 0.56);
  ctx.lineTo(s * 0.43, s * 0.75);
  ctx.lineTo(s * 0.78, s * 0.3);
  ctx.stroke();
};

// Border edges: a full plate with a highlight on the outer side and a dark lip
// facing the field, matching the "border.edge.*" elements of the SVG themes.
const drawBorderEdge = (ctx, s, p, side) => {
  const lw = Math.max(2, s * 0.1);
  const vertical = side === 'west' || side === 'east';
  const grad = vertical ? ctx.createLinearGradient(0, 0, s, 0) : ctx.createLinearGradient(0, 0, 0, s);
  grad.addColorStop(0, p.panel[0]);
  grad.addColorStop(1, p.panel[1]);
  ctx.fillStyle = grad;
  ctx.fillRect(0, 0, s, s);
  const rects = {
    north: { outer: [0, 0, s, lw], inner: [0, s - lw, s, lw] },
    south: { outer: [0, s - lw, s, lw], inner: [0, 0, s, lw] },
    west: { outer: [0, 0, lw, s], inner: [s - lw, 0, lw, s] },
    east: { outer: [s - lw, 0, lw, s], inner: [0, 0, lw, s] },
  }[side];
  fillRect(ctx, ...rects.outer, p.panelLight);
  fillRect(ctx, ...rects.inner, p.panelDark);
};

// Outside corners: quarter disc so the frame reads as one rounded plate.
const CORNER_GEOMETRY = {
  nw: { cx: 1, cy: 1, from: Math.PI, to: Math.PI * 1.5, lipRight: true, lipBottom: true },
  ne: { cx: 0, cy: 1, from: -Math.PI / 2, to: 0, lipRight: false, lipBottom: true },
  sw: { cx: 1, cy: 0, from: Math.PI / 2, to: Math.PI, lipRight: true, lipBottom: false },
  se: { cx: 0, cy: 0, from: 0, to: Math.PI / 2, lipRight: false, lipBottom: false },
};

const drawBorderCorner = (ctx, s, p, corner) => {
  const geo = CORNER_GEOMETRY[corner];
  const lw = Math.max(2, s * 0.1);
  const cx = geo.cx * s;
  const cy = geo.cy * s;
  const grad = ctx.createLinearGradient(0, 0, s, s);
  grad.addColorStop(0, p.panel[0]);
  grad.addColorStop(1, p.panel[1]);
  ctx.fillStyle = grad;
  ctx.beginPath();
  ctx.moveTo(cx, cy);
  ctx.arc(cx, cy, s, geo.from, geo.to);
  ctx.closePath();
  ctx.fill();

  ctx.strokeStyle = p.panelLight;
  ctx.lineWidth = lw;
  ctx.beginPath();
  ctx.arc(cx, cy, s - lw / 2, geo.from, geo.to);
  ctx.stroke();

  ctx.strokeStyle = p.panelDark;
  ctx.beginPath();
  if (corner === 'nw' || corner === 'sw') {
    ctx.moveTo(s - lw / 2, 0);
    ctx.lineTo(s - lw / 2, s);
  }
  if (corner === 'ne' || corner === 'se') {
    ctx.moveTo(lw / 2, 0);
    ctx.lineTo(lw / 2, s);
  }
  if (corner === 'nw' || corner === 'ne') {
    ctx.moveTo(0, s - lw / 2);
    ctx.lineTo(s, s - lw / 2);
  }
  if (corner === 'sw' || corner === 'se') {
    ctx.moveTo(0, lw / 2);
    ctx.lineTo(s, lw / 2);
  }
  ctx.stroke();
};

const drawMainWidget = (ctx, s, p) => {
  const grad = ctx.createLinearGradient(0, 0, 0, s);
  grad.addColorStop(0, p.canvasBg);
  grad.addColorStop(0.5, p.panel[1]);
  grad.addColorStop(1, p.canvasBg);
  ctx.fillStyle = grad;
  ctx.fillRect(0, 0, s, s);
  ctx.globalAlpha = 0.07;
  ctx.fillStyle = p.panelLight;
  for (let x = 0; x < s; x += 10) {
    ctx.fillRect(x, 0, 4, s);
  }
  ctx.globalAlpha = 1;
};

const buildSpriteDrawers = (palette) => {
  const drawers = new Map();
  drawers.set('cell_up', (ctx, s) => drawBevelledTile(ctx, s, palette));
  drawers.set('cell_down', (ctx, s) => drawShownCell(ctx, s, palette));
  drawers.set('flag', (ctx, s) => drawFlag(ctx, s, palette));
  drawers.set('question', (ctx, s) => drawGlyph(ctx, s, palette.question, '?', 0.78));
  drawers.set('mine', (ctx, s) => drawMine(ctx, s, palette));
  drawers.set('explosion', (ctx, s) => drawExplosion(ctx, s, palette));
  drawers.set('error', (ctx, s) => drawCross(ctx, s, palette));
  drawers.set('hint', (ctx, s) => drawCheck(ctx, s, palette));
  '12345678'.split('').forEach((glyph, index) => {
    drawers.set(NUMBER_SPRITES[index], (ctx, s) => drawGlyph(ctx, s, palette.number[index], glyph, 0.72));
  });
  drawers.set('border.edge.north', (ctx, s) => drawBorderEdge(ctx, s, palette, 'north'));
  drawers.set('border.edge.south', (ctx, s) => drawBorderEdge(ctx, s, palette, 'south'));
  drawers.set('border.edge.east', (ctx, s) => drawBorderEdge(ctx, s, palette, 'east'));
  drawers.set('border.edge.west', (ctx, s) => drawBorderEdge(ctx, s, palette, 'west'));
  drawers.set('border.outsideCorner.nw', (ctx, s) => drawBorderCorner(ctx, s, palette, 'nw'));
  drawers.set('border.outsideCorner.ne', (ctx, s) => drawBorderCorner(ctx, s, palette, 'ne'));
  drawers.set('border.outsideCorner.sw', (ctx, s) => drawBorderCorner(ctx, s, palette, 'sw'));
  drawers.set('border.outsideCorner.se', (ctx, s) => drawBorderCorner(ctx, s, palette, 'se'));
  drawers.set('mainWidget', (ctx, s) => drawMainWidget(ctx, s, palette));
  return drawers;
};

export const buildTheme = (name) => buildSpriteDrawers(themePalette(name));
