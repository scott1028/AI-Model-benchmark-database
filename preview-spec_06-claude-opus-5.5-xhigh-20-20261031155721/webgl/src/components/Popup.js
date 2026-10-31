// KGamePopupItem look-alike drawn on a 2D canvas, then shown as a WebGL quad.
import { POPUP_OPACITY } from '../constants.js';

const PADDING = 14;
const FONT_PX = 15;

export const drawPopupCanvas = (text, dpr) => {
  const canvas = document.createElement('canvas');
  const ctx = canvas.getContext('2d');
  const font = `600 ${FONT_PX * dpr}px system-ui, sans-serif`;
  ctx.font = font;
  const textW = ctx.measureText(text).width;
  const w = Math.ceil(textW + PADDING * 2 * dpr);
  const h = Math.ceil((FONT_PX + PADDING * 2) * dpr);
  canvas.width = w;
  canvas.height = h;
  const r = 8 * dpr;
  ctx.globalAlpha = POPUP_OPACITY;
  ctx.fillStyle = '#f4f4f2';
  ctx.strokeStyle = '#8a8f96';
  ctx.lineWidth = dpr;
  ctx.beginPath();
  ctx.roundRect(dpr / 2, dpr / 2, w - dpr, h - dpr, r);
  ctx.fill();
  ctx.stroke();
  ctx.globalAlpha = 1;
  ctx.font = font;
  ctx.fillStyle = '#20242a';
  ctx.textBaseline = 'middle';
  ctx.fillText(text, PADDING * dpr, h / 2);
  return canvas;
};
