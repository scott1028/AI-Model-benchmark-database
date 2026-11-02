// WebGL batched textured-quad renderer.
//
// Port-side equivalent of QGraphicsView + KGameRenderer: the original rasterises
// one SVG element per sprite id into a QPixmap and lets QGraphicsPixmapItem draw
// it; here the sprite ids are rasterised procedurally into a single texture atlas
// (theme.js + atlas.js) and every sprite becomes a quad in a batched draw.
// Popup messages use a second, dynamically rebuilt texture, which is what
// KGamePopupItem (a QGraphicsTextItem) is for in the original scene.

const FLOATS_PER_VERT = 8;          // x, y, u, v, r, g, b, a
const VERTS_PER_QUAD = 4;
const FLOATS_PER_QUAD = FLOATS_PER_VERT * VERTS_PER_QUAD;
const MAX_QUADS = 8192;

const VERTEX_SRC = `
attribute vec2 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;
uniform vec2 uViewport;
varying vec2 vTexCoord;
varying vec4 vColor;
void main() {
  gl_Position = vec4(aPosition.x / uViewport.x * 2.0 - 1.0,
                     1.0 - aPosition.y / uViewport.y * 2.0,
                     0.0, 1.0);
  vTexCoord = aTexCoord;
  vColor = aColor;
}`;

const FRAGMENT_SRC = `
precision mediump float;
varying vec2 vTexCoord;
varying vec4 vColor;
uniform sampler2D uAtlas;
void main() {
  gl_FragColor = texture2D(uAtlas, vTexCoord) * vColor;
}`;

const compileShader = (gl, type, source) => {
  const shader = gl.createShader(type);
  gl.shaderSource(shader, source);
  gl.compileShader(shader);
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    throw new Error(`shader compile failed: ${gl.getShaderInfoLog(shader)}`);
  }
  return shader;
};

const createProgram = (gl) => {
  const program = gl.createProgram();
  gl.attachShader(program, compileShader(gl, gl.VERTEX_SHADER, VERTEX_SRC));
  gl.attachShader(program, compileShader(gl, gl.FRAGMENT_SHADER, FRAGMENT_SRC));
  gl.linkProgram(program);
  if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
    throw new Error(`program link failed: ${gl.getProgramInfoLog(program)}`);
  }
  return program;
};

export class Renderer {
  constructor(canvas) {
    const gl = canvas.getContext('webgl', { alpha: false, antialias: false, depth: false });
    if (!gl) {
      throw new Error('WebGL is not available in this browser');
    }
    this.gl = gl;
    this.canvas = canvas;
    this.program = createProgram(gl);
    this.loc = {
      position: gl.getAttribLocation(this.program, 'aPosition'),
      texCoord: gl.getAttribLocation(this.program, 'aTexCoord'),
      color: gl.getAttribLocation(this.program, 'aColor'),
      viewport: gl.getUniformLocation(this.program, 'uViewport'),
      atlas: gl.getUniformLocation(this.program, 'uAtlas'),
    };
    gl.useProgram(this.program);
    gl.enable(gl.BLEND);
    gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
    gl.uniform1i(this.loc.atlas, 0);

    this.vertices = new Float32Array(MAX_QUADS * FLOATS_PER_QUAD);
    this.buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
    gl.bufferData(gl.ARRAY_BUFFER, this.vertices.byteLength, gl.DYNAMIC_DRAW);

    const indices = new Uint16Array(MAX_QUADS * 6);
    for (let i = 0; i < MAX_QUADS; i++) {
      const v = i * VERTS_PER_QUAD;
      const o = i * 6;
      indices[o] = v; indices[o + 1] = v + 1; indices[o + 2] = v + 2;
      indices[o + 3] = v; indices[o + 4] = v + 2; indices[o + 5] = v + 3;
    }
    this.indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexBuffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, indices, gl.STATIC_DRAW);

    this.quadCount = 0;
    this.currentTexture = null;
    this.frame = { drawCalls: 0, sprites: 0 };
  }

  createTexture(source) {
    const gl = this.gl;
    const texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, false);
    gl.pixelStorei(gl.UNPACK_PREMULTIPLY_ALPHA_WEBGL, false);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, source);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    return texture;
  }

  updateTexture(texture, source) {
    const gl = this.gl;
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, source);
  }

  resize(cssWidth, cssHeight, devicePixelRatio) {
    const width = Math.max(1, Math.round(cssWidth * devicePixelRatio));
    const height = Math.max(1, Math.round(cssHeight * devicePixelRatio));
    if (this.canvas.width !== width || this.canvas.height !== height) {
      this.canvas.width = width;
      this.canvas.height = height;
    }
    this.gl.viewport(0, 0, width, height);
    this.gl.useProgram(this.program);
    this.gl.uniform2f(this.loc.viewport, width, height);
    return { width, height };
  }

  begin(clearColor) {
    const gl = this.gl;
    this.quadCount = 0;
    this.currentTexture = null;
    this.frame = { drawCalls: 0, sprites: 0 };
    gl.clearColor(clearColor[0], clearColor[1], clearColor[2], 1);
    gl.clear(gl.COLOR_BUFFER_BIT);
  }

  // (x, y, w, h) in canvas px; rect = atlas uv; tint = [r,g,b,a] 0..1
  drawSprite(x, y, w, h, rect, texture, tint) {
    if (!rect || !texture) return;
    if (texture !== this.currentTexture || this.quadCount >= MAX_QUADS) {
      this.flush();
      this.currentTexture = texture;
    }
    const v = this.vertices;
    let o = this.quadCount * FLOATS_PER_QUAD;
    const r = tint ? tint[0] : 1;
    const g = tint ? tint[1] : 1;
    const b = tint ? tint[2] : 1;
    const a = tint && tint.length > 3 ? tint[3] : 1;
    const vert = (px, py, tu, tv) => {
      v[o] = px; v[o + 1] = py; v[o + 2] = tu; v[o + 3] = tv;
      v[o + 4] = r; v[o + 5] = g; v[o + 6] = b; v[o + 7] = a;
      o += FLOATS_PER_VERT;
    };
    vert(x, y, rect.u0, rect.v0);
    vert(x, y + h, rect.u0, rect.v1);
    vert(x + w, y + h, rect.u1, rect.v1);
    vert(x + w, y, rect.u1, rect.v0);
    this.quadCount++;
    this.frame.sprites++;
  }

  flush() {
    const gl = this.gl;
    if (this.quadCount === 0) {
      return;
    }
    gl.useProgram(this.program);
    gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
    gl.bufferSubData(gl.ARRAY_BUFFER, 0, this.vertices.subarray(0, this.quadCount * FLOATS_PER_QUAD));
    const stride = FLOATS_PER_VERT * 4;
    gl.enableVertexAttribArray(this.loc.position);
    gl.vertexAttribPointer(this.loc.position, 2, gl.FLOAT, false, stride, 0);
    gl.enableVertexAttribArray(this.loc.texCoord);
    gl.vertexAttribPointer(this.loc.texCoord, 2, gl.FLOAT, false, stride, 8);
    gl.enableVertexAttribArray(this.loc.color);
    gl.vertexAttribPointer(this.loc.color, 4, gl.FLOAT, false, stride, 16);
    gl.bindTexture(gl.TEXTURE_2D, this.currentTexture);
    gl.drawElements(gl.TRIANGLES, this.quadCount * 6, gl.UNSIGNED_SHORT, 0);
    this.frame.drawCalls++;
    this.quadCount = 0;
  }

  end() {
    this.flush();
  }
}
