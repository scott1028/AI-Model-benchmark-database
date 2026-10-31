// WebGL renderer: background, border ring, cells (+ overlays) and popup are
// textured quads. Mirrors the KGameRenderedItem stacking of the original.
import { BORDER_SPRITES, CellState, DIGIT_SPRITES, STATE_SPRITES } from '../constants.js';

const VERTEX_SHADER = `
attribute vec2 a_pos;
attribute vec2 a_uv;
uniform vec2 u_resolution;
varying vec2 v_uv;
void main() {
  vec2 clip = (a_pos / u_resolution) * 2.0 - 1.0;
  gl_Position = vec4(clip.x, -clip.y, 0.0, 1.0);
  v_uv = a_uv;
}`;

const FRAGMENT_SHADER = `
precision mediump float;
uniform sampler2D u_tex;
uniform float u_alpha;
varying vec2 v_uv;
void main() {
  gl_FragColor = texture2D(u_tex, v_uv) * u_alpha;
}`;

const compileShader = (gl, type, source) => {
  const shader = gl.createShader(type);
  gl.shaderSource(shader, source);
  gl.compileShader(shader);
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) throw new Error(gl.getShaderInfoLog(shader));
  return shader;
};

const createProgram = (gl) => {
  const program = gl.createProgram();
  gl.attachShader(program, compileShader(gl, gl.VERTEX_SHADER, VERTEX_SHADER));
  gl.attachShader(program, compileShader(gl, gl.FRAGMENT_SHADER, FRAGMENT_SHADER));
  gl.linkProgram(program);
  if (!gl.getProgramParameter(program, gl.LINK_STATUS)) throw new Error(gl.getProgramInfoLog(program));
  return program;
};

// Sprite keys stacked on one cell, bottom to top (CellItem::updatePixmap 0x117930).
export const getCellSpriteKeys = (cell) => {
  const keys = [...STATE_SPRITES[cell.state]];
  if (cell.state === CellState.Revealed) {
    if (cell.digit > 0) keys.push(DIGIT_SPRITES[cell.digit]);
    else if (cell.hasMine) {
      if (cell.exploded) keys.push('explosion');
      keys.push('mine');
    }
  }
  return keys;
};

export class Renderer {
  constructor(canvas) {
    const gl = canvas.getContext('webgl', { premultipliedAlpha: true, alpha: false, antialias: false })
      || canvas.getContext('experimental-webgl');
    if (!gl) throw new Error('WebGL is not available in this browser.');
    this.gl = gl;
    this.canvas = canvas;
    this.program = createProgram(gl);
    this.loc = {
      pos: gl.getAttribLocation(this.program, 'a_pos'),
      uv: gl.getAttribLocation(this.program, 'a_uv'),
      resolution: gl.getUniformLocation(this.program, 'u_resolution'),
      tex: gl.getUniformLocation(this.program, 'u_tex'),
      alpha: gl.getUniformLocation(this.program, 'u_alpha'),
    };
    this.buffer = gl.createBuffer();
    this.textures = { background: null, sheet: null, popup: null };
    this.sheet = { rects: new Map(), width: 1, height: 1 };
    gl.enable(gl.BLEND);
    gl.blendFunc(gl.ONE, gl.ONE_MINUS_SRC_ALPHA);
    gl.pixelStorei(gl.UNPACK_PREMULTIPLY_ALPHA_WEBGL, true);
  }

  uploadTexture(name, source) {
    const { gl } = this;
    if (!this.textures[name]) this.textures[name] = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, this.textures[name]);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, source);
  }

  setBackground(canvas) {
    this.hasBackground = canvas.width > 0 && canvas.height > 0;
    if (this.hasBackground) this.uploadTexture('background', canvas);
  }

  setSpriteSheet({ canvas, rects }) {
    this.sheet = { rects, width: Math.max(1, canvas.width), height: Math.max(1, canvas.height) };
    if (canvas.width > 0) this.uploadTexture('sheet', canvas);
  }

  setPopup(canvas) {
    this.popupSize = canvas ? { w: canvas.width, h: canvas.height } : null;
    if (canvas) this.uploadTexture('popup', canvas);
  }

  drawQuads(texture, vertices, alpha = 1) {
    if (!texture || vertices.length === 0) return;
    const { gl, loc } = this;
    gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.DYNAMIC_DRAW);
    gl.enableVertexAttribArray(loc.pos);
    gl.vertexAttribPointer(loc.pos, 2, gl.FLOAT, false, 16, 0);
    gl.enableVertexAttribArray(loc.uv);
    gl.vertexAttribPointer(loc.uv, 2, gl.FLOAT, false, 16, 8);
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.uniform1i(loc.tex, 0);
    gl.uniform1f(loc.alpha, alpha);
    gl.drawArrays(gl.TRIANGLES, 0, vertices.length / 4);
  }

  // Two triangles; uv in [u0,v0]-[u1,v1].
  pushQuad(out, x, y, w, h, u0 = 0, v0 = 0, u1 = 1, v1 = 1) {
    out.push(x, y, u0, v0, x + w, y, u1, v0, x, y + h, u0, v1,
      x, y + h, u0, v1, x + w, y, u1, v0, x + w, y + h, u1, v1);
  }

  pushSprite(out, key, x, y, size) {
    const rect = this.sheet.rects.get(key);
    if (!rect) return; // theme lacks this element (e.g. clean_blue has no borders)
    const { width, height } = this.sheet;
    this.pushQuad(out, x, y, size, size, rect.x / width, rect.y / height,
      (rect.x + rect.w) / width, (rect.y + rect.h) / height);
  }

  // view: { dpr, field, layout:{x,y,cellSize}, isFieldVisible, popup:{x,y,alpha}|null }
  render(view) {
    const { gl, canvas } = this;
    const { dpr, field, layout } = view;
    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(0, 0, 0, 1);
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.useProgram(this.program);
    gl.uniform2f(this.loc.resolution, canvas.width, canvas.height);

    if (this.hasBackground) {
      const bg = [];
      this.pushQuad(bg, 0, 0, canvas.width, canvas.height);
      this.drawQuads(this.textures.background, bg);
    }

    if (view.isFieldVisible && field.cells.length > 0) {
      const size = layout.cellSize * dpr;
      const ox = Math.round(layout.x * dpr);
      const oy = Math.round(layout.y * dpr);
      const quads = [];
      for (const b of field.borders) this.pushSprite(quads, BORDER_SPRITES[b.type], ox + b.col * size, oy + b.row * size, size);
      field.cells.forEach((cell, idx) => {
        const x = ox + ((idx % field.cols) + 1) * size;
        const y = oy + (Math.floor(idx / field.cols) + 1) * size;
        for (const key of getCellSpriteKeys(cell)) this.pushSprite(quads, key, x, y, size);
      });
      this.drawQuads(this.textures.sheet, quads);
    }

    if (view.popup && this.popupSize) {
      const p = [];
      this.pushQuad(p, Math.round(view.popup.x * dpr), Math.round(view.popup.y * dpr), this.popupSize.w, this.popupSize.h);
      this.drawQuads(this.textures.popup, p, view.popup.alpha);
    }
  }
}
