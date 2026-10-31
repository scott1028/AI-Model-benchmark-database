/* sprites.js — procedural sprite atlas.
 *
 * Key set is taken from the sprite keys found in /usr/games/kmines
 * (UTF-16 literals in .rodata) and from the layer ids of the installed
 * SVG themes in /usr/share/kmines/themes/*.svgz:
 *   cell_up cell_down mine flag question explosion hint error
 *   arabicOne arabicTwo arabicThree arabicFour arabicFive arabicSix
 *   arabicSeven arabicEight mainWidget
 *   border.edge.north/south/east/west
 *   border.outsideCorner.nw/ne/sw/se  border.insideCorner.nw/ne/sw/se
 * No files are loaded: every tile is drawn at runtime, so the port uses no
 * external art assets.
 */
(function () {
  'use strict';

  const TILE = 64;            // atlas tile size in px
  const COLS = 8;             // 8x8 tiles

  const KEYS = [
    'cell_up', 'cell_down', 'mine', 'flag', 'question', 'explosion', 'hint', 'error',
    'arabicOne', 'arabicTwo', 'arabicThree', 'arabicFour',
    'arabicFive', 'arabicSix', 'arabicSeven', 'arabicEight',
    'border.edge.north', 'border.edge.south', 'border.edge.east', 'border.edge.west',
    'border.outsideCorner.nw', 'border.outsideCorner.ne',
    'border.outsideCorner.sw', 'border.outsideCorner.se',
    'border.insideCorner.nw', 'border.insideCorner.ne',
    'border.insideCorner.sw', 'border.insideCorner.se',
    'mainWidget', 'panel', 'reload', 'blank'
  ];

  // Number colours follow the digit colours used by the KDE themes
  // (1 blue, 2 green, 3 red, 4 navy, 5 maroon, 6 teal, 7 dark, 8 gray).
  const NUM_COLORS = ['#1b5fd0', '#1a7f37', '#c62828', '#12247a',
                      '#8d1c1c', '#0e7490', '#222222', '#6b6b6b'];

  // "arabicOne".."arabicEight" → 1..8
  const WORD_DIGITS = {
    arabicOne: 1, arabicTwo: 2, arabicThree: 3, arabicFour: 4,
    arabicFive: 5, arabicSix: 6, arabicSeven: 7, arabicEight: 8
  };

  function keyIndex(k) {
    const i = KEYS.indexOf(k);
    return i < 0 ? KEYS.length - 1 : i;   // 'blank' fallback
  }

  function rounded(c, x, y, w, h, r) {
    c.beginPath();
    c.moveTo(x + r, y);
    c.arcTo(x + w, y, x + w, y + h, r);
    c.arcTo(x + w, y + h, x, y + h, r);
    c.arcTo(x, y + h, x, y, r);
    c.arcTo(x, y, x + w, y, r);
    c.closePath();
  }

  // Face colours of the reconstructed "clean blue"-like look.
  const FACE_UP = ['#8fb6e6', '#5f8fc9'];       // raised cell
  const FACE_DOWN = ['#cfe2f7', '#a9c8e8'];     // sunken / opened cell
  const PANEL = ['#dfe7f0', '#c3d0de'];

  function cellUp(c) {
    const g = c.createLinearGradient(0, 0, TILE, TILE);
    g.addColorStop(0, FACE_UP[0]); g.addColorStop(1, FACE_UP[1]);
    rounded(c, 3, 3, TILE - 6, TILE - 6, 9); c.fillStyle = g; c.fill();
    c.strokeStyle = 'rgba(255,255,255,.55)'; c.lineWidth = 3;
    c.beginPath(); c.moveTo(6, TILE - 8); c.lineTo(6, 6); c.lineTo(TILE - 8, 6); c.stroke();
    c.strokeStyle = 'rgba(20,45,80,.45)'; c.lineWidth = 3;
    c.beginPath(); c.moveTo(TILE - 6, 8); c.lineTo(TILE - 6, TILE - 6); c.lineTo(8, TILE - 6); c.stroke();
  }

  function cellDown(c) {
    const g = c.createLinearGradient(0, 0, TILE, TILE);
    g.addColorStop(0, FACE_DOWN[1]); g.addColorStop(1, FACE_DOWN[0]);
    rounded(c, 3, 3, TILE - 6, TILE - 6, 9); c.fillStyle = g; c.fill();
    c.strokeStyle = 'rgba(30,60,100,.30)'; c.lineWidth = 3;
    c.beginPath(); c.moveTo(7, TILE - 9); c.lineTo(7, 7); c.lineTo(TILE - 9, 7); c.stroke();
  }

  function mine(c) {
    c.fillStyle = 'rgba(0,0,0,0)'; c.fillRect(0, 0, TILE, TILE);
    c.save(); c.translate(TILE / 2, TILE / 2);
    c.strokeStyle = '#101418'; c.lineWidth = 5; c.lineCap = 'round';
    for (let i = 0; i < 8; i++) {
      c.save(); c.rotate(i * Math.PI / 4);
      c.beginPath(); c.moveTo(0, -10); c.lineTo(0, -25); c.stroke();
      c.restore();
    }
    c.fillStyle = '#141a20';
    c.beginPath(); c.arc(0, 0, 16, 0, Math.PI * 2); c.fill();
    c.fillStyle = 'rgba(255,255,255,.75)';
    c.beginPath(); c.arc(-5, -5, 4, 0, Math.PI * 2); c.fill();
    c.restore();
  }

  function flag(c) {
    c.save(); c.translate(TILE / 2, TILE / 2);
    c.fillStyle = '#8a5a2b';
    c.fillRect(-2, -4, 4, 22);                       // pole
    c.fillRect(-12, 16, 24, 6);                      // base
    const g = c.createLinearGradient(-18, -22, 16, -2);
    g.addColorStop(0, '#e53935'); g.addColorStop(1, '#b71c1c');
    c.fillStyle = g;
    c.beginPath();
    c.moveTo(0, -22); c.lineTo(18, -14); c.lineTo(0, -6);
    c.closePath(); c.fill();
    c.restore();
  }

  function question(c) {
    c.fillStyle = '#132a4d';
    c.font = 'bold 46px "DejaVu Sans", "Liberation Sans", sans-serif';
    c.textAlign = 'center'; c.textBaseline = 'middle';
    c.fillText('?', TILE / 2, TILE / 2 + 2);
  }

  function explosion(c) {
    c.save(); c.translate(TILE / 2, TILE / 2);
    const g = c.createRadialGradient(0, 0, 3, 0, 0, 28);
    g.addColorStop(0, '#fff3b0'); g.addColorStop(.45, '#ff9800'); g.addColorStop(1, '#c62828');
    c.fillStyle = g;
    c.beginPath();
    for (let i = 0; i < 12; i++) {
      const a = (i / 12) * Math.PI * 2;
      const r = i % 2 === 0 ? 28 : 15;
      c.lineTo(Math.cos(a) * r, Math.sin(a) * r);
    }
    c.closePath(); c.fill();
    c.restore();
  }

  function hint(c) {
    c.strokeStyle = 'rgba(255,215,0,.85)'; c.lineWidth = 5;
    rounded(c, 5, 5, TILE - 10, TILE - 10, 8); c.stroke();
  }

  function error(c) {
    c.strokeStyle = '#d32f2f'; c.lineWidth = 8; c.lineCap = 'round';
    c.beginPath();
    c.moveTo(14, 14); c.lineTo(TILE - 14, TILE - 14);
    c.moveTo(TILE - 14, 14); c.lineTo(14, TILE - 14);
    c.stroke();
  }

  function number(c, n) {
    c.fillStyle = NUM_COLORS[n - 1];
    c.font = 'bold 44px "DejaVu Sans", "Liberation Sans", sans-serif';
    c.textAlign = 'center'; c.textBaseline = 'middle';
    c.strokeStyle = 'rgba(255,255,255,.85)'; c.lineWidth = 5;
    c.strokeText(String(n), TILE / 2, TILE / 2 + 2);
    c.fillText(String(n), TILE / 2, TILE / 2 + 2);
  }

  // Border tiles: a 3D bevel strip, matching the bevelled frame the
  // border.* layers of the KDE themes provide.
  function bevel(c, dirs) {
    const g = c.createLinearGradient(0, 0, TILE, TILE);
    g.addColorStop(0, PANEL[0]); g.addColorStop(1, PANEL[1]);
    c.fillStyle = g; c.fillRect(0, 0, TILE, TILE);
    c.strokeStyle = 'rgba(255,255,255,.7)'; c.lineWidth = 4;
    c.beginPath();
    if (dirs.indexOf('n') >= 0) { c.moveTo(0, 3); c.lineTo(TILE, 3); }
    if (dirs.indexOf('w') >= 0) { c.moveTo(3, 0); c.lineTo(3, TILE); }
    c.stroke();
    c.strokeStyle = 'rgba(40,70,110,.55)'; c.lineWidth = 4;
    c.beginPath();
    if (dirs.indexOf('s') >= 0) { c.moveTo(0, TILE - 3); c.lineTo(TILE, TILE - 3); }
    if (dirs.indexOf('e') >= 0) { c.moveTo(TILE - 3, 0); c.lineTo(TILE - 3, TILE); }
    c.stroke();
  }

  function panelTile(c) {
    const g = c.createLinearGradient(0, 0, TILE, TILE);
    g.addColorStop(0, PANEL[0]); g.addColorStop(1, PANEL[1]);
    c.fillStyle = g; c.fillRect(0, 0, TILE, TILE);
    c.strokeStyle = 'rgba(255,255,255,.35)'; c.lineWidth = 2;
    c.beginPath(); c.moveTo(0, 2); c.lineTo(TILE, 2); c.stroke();
  }

  function reload(c) {
    c.strokeStyle = '#1a7f37'; c.lineWidth = 6; c.lineCap = 'round';
    c.save(); c.translate(TILE / 2, TILE / 2);
    c.beginPath(); c.arc(0, 0, 18, -0.4, Math.PI * 1.6); c.stroke();
    c.beginPath(); c.moveTo(14, -20); c.lineTo(22, -4); c.lineTo(4, -2); c.closePath();
    c.fillStyle = '#1a7f37'; c.fill();
    c.restore();
  }

  const DRAW = {
    'cell_up': cellUp, 'cell_down': cellDown, 'mine': mine, 'flag': flag,
    'question': question, 'explosion': explosion, 'hint': hint, 'error': error,
    'border.edge.north': (c) => bevel(c, 'n'),
    'border.edge.south': (c) => bevel(c, 's'),
    'border.edge.east': (c) => bevel(c, 'e'),
    'border.edge.west': (c) => bevel(c, 'w'),
    'border.outsideCorner.nw': (c) => bevel(c, 'nw'),
    'border.outsideCorner.ne': (c) => bevel(c, 'ne'),
    'border.outsideCorner.sw': (c) => bevel(c, 'sw'),
    'border.outsideCorner.se': (c) => bevel(c, 'se'),
    'border.insideCorner.nw': (c) => bevel(c, 'se'),
    'border.insideCorner.ne': (c) => bevel(c, 'sw'),
    'border.insideCorner.sw': (c) => bevel(c, 'ne'),
    'border.insideCorner.se': (c) => bevel(c, 'nw'),
    'mainWidget': panelTile, 'panel': panelTile, 'reload': reload
  };

  function build() {
    const cv = document.createElement('canvas');
    cv.width = TILE * COLS;
    cv.height = TILE * COLS;
    const c = cv.getContext('2d');
    c.clearRect(0, 0, cv.width, cv.height);
    KEYS.forEach((k, i) => {
      const col = i % COLS, row = (i / COLS) | 0;
      c.save();
      c.translate(col * TILE, row * TILE);
      if (k.indexOf('arabic') === 0) {
        number(c, WORD_DIGITS[k]);
      } else if (DRAW[k]) {
        DRAW[k](c);
      }
      c.restore();
    });
    return { canvas: cv, tile: TILE, cols: COLS, keys: KEYS, keyIndex: keyIndex };
  }

  window.KMinesSprites = { build: build, KEYS: KEYS, TILE: TILE, COLS: COLS, keyIndex: keyIndex };
})();
