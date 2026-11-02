// Packs the procedurally drawn sprites of one theme into a single texture atlas.
// Stand-in for KGameRenderer: same sprite ids in, uv rect out.

const PADDING = 4;

// Normalised atlas coordinate, sampled half a texel inside the packed sprite so
// neighbouring sprites never bleed into a quad edge (that would draw hairline
// seams between the cells of the board).
const texel = (pixels, half, size) => (pixels + half) / size;

const nextPowerOfTwo = (value) => {
  let size = 64;
  while (size < value) size *= 2;
  return size;
};

export class Atlas {
  constructor(drawers, spriteSize) {
    const ids = [...drawers.keys()];
    const perRow = Math.ceil(Math.sqrt(ids.length));
    const rows = Math.ceil(ids.length / perRow);
    const needed = Math.max(perRow, rows) * (spriteSize + PADDING) + PADDING;
    const size = nextPowerOfTwo(needed);

    const canvas = document.createElement('canvas');
    canvas.width = size;
    canvas.height = size;
    const ctx = canvas.getContext('2d');

    const rects = new Map();
    ids.forEach((id, index) => {
      const x = PADDING + (index % perRow) * (spriteSize + PADDING);
      const y = PADDING + Math.floor(index / perRow) * (spriteSize + PADDING);
      ctx.save();
      ctx.translate(x, y);
      ctx.beginPath();
      ctx.rect(0, 0, spriteSize, spriteSize);
      ctx.clip();
      drawers.get(id)(ctx, spriteSize);
      ctx.restore();
      rects.set(id, {
        u0: texel(x, 0.5, size),
        v0: texel(y, 0.5, size),
        u1: texel(x + spriteSize, -0.5, size),
        v1: texel(y + spriteSize, -0.5, size),
      });
    });

    this.canvas = canvas;
    this.rects = rects;
    this.spriteSize = spriteSize;
    this.size = size;
  }

  get(id) {
    return this.rects.get(id);
  }
}
