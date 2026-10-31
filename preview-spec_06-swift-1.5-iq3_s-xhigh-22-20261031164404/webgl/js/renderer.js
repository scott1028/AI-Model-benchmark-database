/* renderer.js — WebGL2 renderer for the reconstructed KMines scene.
 *
 * Mirrors the QGraphicsView scene of KMines: one tiled "mainWidget" panel,
 * a bevelled frame made of "border.edge.*" / "border.outsideCorner.*" sprites,
 * and the grid of cell sprites of MineFieldItem.
 */
(function () {
  'use strict';

  const VS = `#version 300 es
  in vec2 a_pos;
  in vec2 a_uv;
  in float a_tint;
  uniform vec2 u_view;
  out vec2 v_uv;
  out float v_tint;
  void main() {
    vec2 p = (a_pos / u_view) * 2.0 - 1.0;
    gl_Position = vec4(p.x, -p.y, 0.0, 1.0);
    v_uv = a_uv;
    v_tint = a_tint;
  }`;

  const FS = `#version 300 es
  precision mediump float;
  in vec2 v_uv;
  in float v_tint;
  uniform sampler2D u_atlas;
  out vec4 outColor;
  void main() {
    vec4 c = texture(u_atlas, v_uv);
    float b = 1.0 - v_tint;
    outColor = vec4(c.rgb * b, c.a);
  }`;

  class Renderer {
    constructor(canvas, atlas) {
      this.canvas = canvas;
      this.atlas = atlas;
      const gl = canvas.getContext('webgl2',
        { antialias: true, premultipliedAlpha: false, preserveDrawingBuffer: true });
      if (!gl) throw new Error('WebGL2 is not available in this browser');
      this.gl = gl;
      this.vert = this.compile(gl.VERTEX_SHADER, VS);
      this.frag = this.compile(gl.FRAGMENT_SHADER, FS);
      const p = gl.createProgram();
      gl.attachShader(p, this.vert); gl.attachShader(p, this.frag);
      gl.linkProgram(p);
      if (!gl.getProgramParameter(p, gl.LINK_STATUS)) {
        throw new Error('shader link failed: ' + gl.getProgramInfoLog(p));
      }
      this.program = p;
      this.aPos = gl.getAttribLocation(p, 'a_pos');
      this.aUv = gl.getAttribLocation(p, 'a_uv');
      this.aTint = gl.getAttribLocation(p, 'a_tint');
      this.uView = gl.getUniformLocation(p, 'u_view');
      this.uAtlas = gl.getUniformLocation(p, 'u_atlas');

      this.vao = gl.createVertexArray();
      gl.bindVertexArray(this.vao);
      this.vbo = gl.createBuffer();
      gl.bindBuffer(gl.ARRAY_BUFFER, this.vbo);
      const stride = 5 * 4;
      gl.enableVertexAttribArray(this.aPos);
      gl.vertexAttribPointer(this.aPos, 2, gl.FLOAT, false, stride, 0);
      gl.enableVertexAttribArray(this.aUv);
      gl.vertexAttribPointer(this.aUv, 2, gl.FLOAT, false, stride, 8);
      gl.enableVertexAttribArray(this.aTint);
      gl.vertexAttribPointer(this.aTint, 1, gl.FLOAT, false, stride, 16);
      gl.bindVertexArray(null);

      this.tex = gl.createTexture();
      gl.activeTexture(gl.TEXTURE0);
      gl.bindTexture(gl.TEXTURE_2D, this.tex);
      gl.pixelStorei(gl.UNPACK_PREMULTIPLY_ALPHA_WEBGL, false);
      gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, atlas.canvas);
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
      gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);

      gl.enable(gl.BLEND);
      gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

      this.data = new Float32Array(6 * 5 * 4096);
      this.count = 0;
      this.view = [1, 1];
    }

    compile(type, src) {
      const gl = this.gl;
      const s = gl.createShader(type);
      gl.shaderSource(s, src);
      gl.compileShader(s);
      if (!gl.getShaderParameter(s, gl.COMPILE_STATUS)) {
        throw new Error('shader compile failed: ' + gl.getShaderInfoLog(s));
      }
      return s;
    }

    resize(cssW, cssH, dpr) {
      const w = Math.max(1, Math.round(cssW * dpr));
      const h = Math.max(1, Math.round(cssH * dpr));
      if (this.canvas.width !== w || this.canvas.height !== h) {
        this.canvas.width = w;
        this.canvas.height = h;
      }
      this.view = [cssW, cssH];
      this.dpr = dpr;
    }

    begin() {
      const gl = this.gl;
      this.count = 0;
      gl.bindFramebuffer(gl.FRAMEBUFFER, null);
      gl.viewport(0, 0, this.canvas.width, this.canvas.height);
      gl.clearColor(0.11, 0.15, 0.21, 1);
      gl.clear(gl.COLOR_BUFFER_BIT);
    }

    ensure(n) {
      if (this.data.length < n) {
        const d = new Float32Array(n * 2);
        d.set(this.data);
        this.data = d;
      }
    }

    // x, y, w, h in CSS px; key = atlas key; tint = 0..1 darkening
    quad(x, y, w, h, key, tint = 0) {
      const a = this.atlas;
      const i = a.keyIndex(key);
      const col = i % a.cols, row = (i / a.cols) | 0;
      const T = a.tile, A = T * a.cols;
      const u0 = col / a.cols, u1 = (col + 1) / a.cols;
      const v0 = row / a.cols, v1 = (row + 1) / a.cols;
      this.ensure((this.count + 6) * 5);
      const d = this.data;
      let p = this.count * 5;
      const push = (px, py, uu, vv) => {
        d[p++] = px; d[p++] = py; d[p++] = uu; d[p++] = vv; d[p++] = tint;
      };
      push(x, y, u0, v0);
      push(x + w, y, u1, v0);
      push(x + w, y + h, u1, v1);
      push(x, y, u0, v0);
      push(x + w, y + h, u1, v1);
      push(x, y + h, u0, v1);
      this.count += 6;
    }

    // Tile a sprite across a rect (used for the panel background and edges).
    tileArea(x, y, w, h, key, tileSize, tint = 0) {
      for (let ty = 0; ty < h; ty += tileSize) {
        for (let tx = 0; tx < w; tx += tileSize) {
          const cw = Math.min(tileSize, w - tx);
          const ch = Math.min(tileSize, h - ty);
          this.quad(x + tx, y + ty, cw, ch, key, tint);
        }
      }
    }

    flush() {
      const gl = this.gl;
      if (this.count === 0) return;
      gl.useProgram(this.program);
      gl.bindVertexArray(this.vao);
      gl.bindBuffer(gl.ARRAY_BUFFER, this.vbo);
      gl.bufferData(gl.ARRAY_BUFFER, this.data.subarray(0, this.count * 5), gl.DYNAMIC_DRAW);
      gl.activeTexture(gl.TEXTURE0);
      gl.bindTexture(gl.TEXTURE_2D, this.tex);
      gl.uniform1i(this.uAtlas, 0);
      gl.uniform2f(this.uView, this.view[0], this.view[1]);
      gl.drawArrays(gl.TRIANGLES, 0, this.count);
      gl.bindVertexArray(null);
    }
  }

  window.KMinesRenderer = Renderer;
})();
