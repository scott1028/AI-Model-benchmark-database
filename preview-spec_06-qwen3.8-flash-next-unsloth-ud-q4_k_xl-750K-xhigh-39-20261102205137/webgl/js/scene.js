// Port of KMinesScene (+ the item placement done by MineFieldItem).
//
// Geometry comes from KMinesScene::updateScale (FUN_00120020 in the report):
//   cellSize = int(min(viewportW / (width + 2), viewportH / (height + 2)))
//   the field is centred in the viewport, cells sit at (col + 1, row + 1) cells,
//   border pieces occupy the outer ring, and the theme element "mainWidget"
//   (which KGameRenderer renders to the scene size and KGameScene paints as the
//   background brush) is tiled behind everything.

import { CellState } from './field.js';
import { STATE_SPRITES, NUMBER_SPRITES, BORDER_SPRITES } from './theme.js';

const POPUP_TIMEOUT_MS = 4000;      // KGamePopupItem::setMessageTimeout(4000)
const POPUP_OPACITY = 0.9;          // KGamePopupItem::setMessageOpacity(0.9)
const BACKGROUND_TILE_PX = 160;

class Popup {
  constructor() {
    this.text = '';
    this.shownAt = 0;
    this.sticky = false;
    this.opacity = 0;
  }

  show(text, nowMs, sticky) {
    this.text = text;
    this.shownAt = nowMs;
    this.sticky = !!sticky;
    this.opacity = POPUP_OPACITY;
  }

  hide() {
    this.text = '';
    this.opacity = 0;
  }

  update(nowMs) {
    if (!this.text || this.sticky) return;
    const elapsed = nowMs - this.shownAt;
    if (elapsed > POPUP_TIMEOUT_MS) {
      this.opacity = POPUP_OPACITY * Math.max(0, 1 - (elapsed - POPUP_TIMEOUT_MS) / 600);
      if (this.opacity <= 0.01) this.hide();
    }
  }
}

const paintPopup = (canvas, text, fontPx) => {
  const ctx = canvas.getContext('2d');
  ctx.font = `bold ${fontPx}px "DejaVu Sans", sans-serif`;
  const padding = Math.round(fontPx * 0.9);
  const textWidth = Math.ceil(ctx.measureText(text).width);
  const width = Math.max(64, textWidth + padding * 2);
  const height = Math.max(48, Math.round(fontPx * 2.4));
  if (canvas.width !== width || canvas.height !== height) {
    canvas.width = width;
    canvas.height = height;
  }
  ctx.clearRect(0, 0, width, height);
  const radius = Math.round(height * 0.25);
  ctx.beginPath();
  ctx.moveTo(radius, 0);
  ctx.arcTo(width, 0, width, height, radius);
  ctx.arcTo(width, height, 0, height, radius);
  ctx.arcTo(0, height, 0, 0, radius);
  ctx.arcTo(0, 0, width, 0, radius);
  ctx.closePath();
  ctx.fillStyle = 'rgba(18, 18, 20, 0.95)';
  ctx.fill();
  ctx.fillStyle = '#ffffff';
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  ctx.fillText(text, width / 2, height / 2 + 1);
  return { width, height };
};

export class Scene {
  constructor(field) {
    this.field = field;
    this.viewportWidth = 0;
    this.viewportHeight = 0;
    this.cellSize = 4;
    this.fieldX = 0;
    this.fieldY = 0;
    this.devicePixelRatio = 1;
    this.isPaused = false;
    this.messagePopup = new Popup();
    this.pausePopup = new Popup();
    this.popupCanvas = document.createElement('canvas');
    this.popupTexture = null;
    this.atlas = null;
    this.atlasTexture = null;
    this.clearColor = [0.24, 0.24, 0.26];
    this.fullRect = { u0: 0, v0: 0, u1: 1, v1: 1 };
  }

  setClearColor(rgb) {
    this.clearColor = rgb;
  }

  setRenderer(renderer) {
    this.renderer = renderer;
    this.popupTexture = renderer.createTexture(this.popupCanvas);
  }

  setAtlas(atlas, texture, devicePixelRatio) {
    this.atlas = atlas;
    this.atlasTexture = texture;
    this.devicePixelRatio = devicePixelRatio;
  }

  updateScale(viewportWidth, viewportHeight) {
    this.viewportWidth = viewportWidth;
    this.viewportHeight = viewportHeight;
    const columns = this.field.width + 2;
    const rows = this.field.height + 2;
    const size = Math.floor(Math.min(viewportWidth / columns, viewportHeight / rows));
    this.cellSize = Math.max(4, size);
    this.fieldX = Math.round((viewportWidth - columns * this.cellSize) / 2);
    this.fieldY = Math.round((viewportHeight - rows * this.cellSize) / 2);
  }

  showPaused(isPaused) {
    this.isPaused = isPaused;
    if (isPaused) {
      this.pausePopup.show('Game is paused.', performance.now(), true);
    } else {
      this.pausePopup.hide();
    }
  }

  showMessage(text) {
    this.messagePopup.show(text, performance.now(), false);
  }

  cellAt(canvasX, canvasY) {
    return {
      row: Math.floor((canvasY - this.fieldY) / this.cellSize) - 1,
      col: Math.floor((canvasX - this.fieldX) / this.cellSize) - 1,
    };
  }

  drawSprite(id, x, y, size, tint) {
    const rect = this.atlas.get(id);
    if (rect) this.renderer.drawSprite(x, y, size, size, rect, this.atlasTexture, tint);
  }

  drawBackground() {
    const scale = this.devicePixelRatio;
    const width = this.viewportWidth * scale;
    const height = this.viewportHeight * scale;
    const tile = BACKGROUND_TILE_PX * scale;
    const base = this.atlas.get('mainWidget');
    if (!base) return;
    for (let x = 0; x < width; x += tile) {
      for (let y = 0; y < height; y += tile) {
        this.renderer.drawSprite(x, y, Math.min(tile, width - x), Math.min(tile, height - y),
          base, this.atlasTexture);
      }
    }
  }

  drawBorder(fx, fy, size) {
    const { height, width } = this.field;
    // creation order in MineFieldItem::updateFieldItems: nw corner, north edge,
    // ne corner, west+east per row, sw corner, south edge, se corner
    this.drawSprite(BORDER_SPRITES[4], fx, fy, size);
    for (let col = 1; col <= width; col++) this.drawSprite(BORDER_SPRITES[1], fx + col * size, fy, size);
    this.drawSprite(BORDER_SPRITES[7], fx + (width + 1) * size, fy, size);
    for (let row = 1; row <= height; row++) {
      this.drawSprite(BORDER_SPRITES[3], fx, fy + row * size, size);
      this.drawSprite(BORDER_SPRITES[2], fx + (width + 1) * size, fy + row * size, size);
    }
    this.drawSprite(BORDER_SPRITES[5], fx, fy + (height + 1) * size, size);
    for (let col = 1; col <= width; col++) {
      this.drawSprite(BORDER_SPRITES[0], fx + col * size, fy + (height + 1) * size, size);
    }
    this.drawSprite(BORDER_SPRITES[6], fx + (width + 1) * size, fy + (height + 1) * size, size);
  }

  drawCell(cell, x, y, size) {
    const sprites = STATE_SPRITES[cell.state] || STATE_SPRITES[CellState.Covered];
    this.drawSprite(sprites[0], x, y, size);
    for (let i = 1; i < sprites.length; i++) {
      this.drawSprite(sprites[i], x, y, size);
    }
    if (cell.state === CellState.Shown) {
      if (cell.surroundingMines > 0) {
        this.drawSprite(NUMBER_SPRITES[cell.surroundingMines - 1], x, y, size);
      }
      if (cell.isMine) {
        if (cell.isBoom) this.drawSprite('explosion', x, y, size);
        this.drawSprite('mine', x, y, size);
      }
    }
  }

  drawPopup(popup) {
    popup.update(performance.now());
    if (!popup.text || popup.opacity <= 0) return;
    const scale = this.devicePixelRatio;
    const fontPx = Math.max(14, Math.min(30, Math.round(this.viewportHeight * 0.05)));
    const { width, height } = paintPopup(this.popupCanvas, popup.text, fontPx);
    this.renderer.updateTexture(this.popupTexture, this.popupCanvas);
    const w = width * scale;
    const h = height * scale;
    this.renderer.drawSprite((this.viewportWidth * scale - w) / 2,
      (this.viewportHeight * scale - h) / 2, w, h, this.fullRect, this.popupTexture,
      [1, 1, 1, popup.opacity]);
  }

  render() {
    if (!this.atlas || !this.renderer) return;
    const scale = this.devicePixelRatio;
    const size = this.cellSize * scale;
    const fx = this.fieldX * scale;
    const fy = this.fieldY * scale;
    this.renderer.begin(this.clearColor || [0.24, 0.24, 0.26]);
    this.drawBackground();
    if (!this.isPaused) {
      this.drawBorder(fx, fy, size);
      for (const cell of this.field.cells) {
        this.drawCell(cell, fx + (cell.col + 1) * size, fy + (cell.row + 1) * size, size);
      }
    }
    this.drawPopup(this.isPaused ? this.pausePopup : this.messagePopup);
    this.renderer.end();
  }
}
