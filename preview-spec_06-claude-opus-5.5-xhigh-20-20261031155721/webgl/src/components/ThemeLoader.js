// Loads a kmines SVG theme and rasterizes elements by id, like KGameRenderer:
// each element's document-space bounds are stretched to the requested size.
const SVG_NS = 'http://www.w3.org/2000/svg';

const loadImage = (svgText) => new Promise((resolve, reject) => {
  const url = URL.createObjectURL(new Blob([svgText], { type: 'image/svg+xml' }));
  const img = new Image();
  img.onload = () => {
    URL.revokeObjectURL(url);
    resolve(img);
  };
  img.onerror = () => {
    URL.revokeObjectURL(url);
    reject(new Error('SVG rasterization failed'));
  };
  img.src = url;
});

const toMatrixAttr = (m) => `matrix(${m.a} ${m.b} ${m.c} ${m.d} ${m.e} ${m.f})`;

// Element bbox mapped into root user space (the theme's document coordinates).
const getDocumentBounds = (rootInverse, el) => {
  const box = el.getBBox();
  const m = rootInverse.multiply(el.getScreenCTM());
  const pts = [[box.x, box.y], [box.x + box.width, box.y], [box.x, box.y + box.height],
    [box.x + box.width, box.y + box.height]].map(([x, y]) => new DOMPoint(x, y).matrixTransform(m));
  const xs = pts.map((p) => p.x);
  const ys = pts.map((p) => p.y);
  const x = Math.min(...xs);
  const y = Math.min(...ys);
  return { x, y, width: Math.max(...xs) - x, height: Math.max(...ys) - y };
};

export class Theme {
  constructor(rootAttrs, defsMarkup, elements) {
    this.rootAttrs = rootAttrs;
    this.defsMarkup = defsMarkup;
    // id -> { bounds, parentTransform }
    this.elements = elements;
  }

  hasSprite(key) {
    return this.elements.has(key);
  }

  // One <svg> per call: every sprite gets a clipped slot of slotW x slotH.
  buildSheetMarkup(keys, slotW, slotH, columns) {
    const rows = Math.ceil(keys.length / columns);
    const width = slotW * columns;
    const height = slotH * rows;
    const slots = keys.map((key, i) => {
      const info = this.elements.get(key);
      const { x, y, width: w, height: h } = info.bounds;
      const sx = (i % columns) * slotW;
      const sy = Math.floor(i / columns) * slotH;
      return `<svg x="${sx}" y="${sy}" width="${slotW}" height="${slotH}" viewBox="${x} ${y} ${w} ${h}" `
        + `preserveAspectRatio="none" overflow="hidden"><use href="#${key}" xlink:href="#${key}" `
        + `transform="${info.parentTransform}"/></svg>`;
    }).join('');
    return `<svg ${this.rootAttrs} width="${width}" height="${height}" viewBox="0 0 ${width} ${height}">`
      + `<defs>${this.defsMarkup}</defs>${slots}</svg>`;
  }

  // Returns a canvas holding the sprites laid out on a grid, plus slot rects.
  async renderSheet(keys, slotW, slotH) {
    const present = keys.filter((k) => this.hasSprite(k));
    const columns = Math.max(1, Math.ceil(Math.sqrt(present.length)));
    const canvas = document.createElement('canvas');
    const rects = new Map();
    if (present.length === 0) return { canvas, rects };
    const img = await loadImage(this.buildSheetMarkup(present, slotW, slotH, columns));
    canvas.width = img.width;
    canvas.height = img.height;
    canvas.getContext('2d').drawImage(img, 0, 0);
    present.forEach((key, i) => {
      rects.set(key, { x: (i % columns) * slotW, y: Math.floor(i / columns) * slotH, w: slotW, h: slotH });
    });
    return { canvas, rects };
  }
}

const getRootAttributes = (svg) => {
  const skip = new Set(['width', 'height', 'viewBox', 'x', 'y', 'id', 'style', 'preserveAspectRatio']);
  const attrs = [...svg.attributes].filter((a) => !skip.has(a.name)).map((a) => `${a.name}="${a.value.replace(/"/g, '&quot;')}"`);
  if (!svg.hasAttribute('xmlns')) attrs.push(`xmlns="${SVG_NS}"`);
  if (!svg.hasAttribute('xmlns:xlink')) attrs.push('xmlns:xlink="http://www.w3.org/1999/xlink"');
  return attrs.join(' ');
};

// Fetch + measure. Measuring needs the SVG attached to the live document.
export const loadTheme = async (url, keys) => {
  const response = await fetch(url);
  if (!response.ok) throw new Error(`Theme fetch failed: ${url} (${response.status})`);
  const text = await response.text();
  const doc = new DOMParser().parseFromString(text, 'image/svg+xml');
  const parsed = doc.documentElement;
  if (parsed.nodeName !== 'svg') throw new Error(`Invalid SVG theme: ${url}`);

  const host = document.createElement('div');
  host.style.cssText = 'position:absolute;left:0;top:0;width:0;height:0;overflow:hidden;visibility:hidden;pointer-events:none';
  const svg = document.importNode(parsed, true);
  host.appendChild(svg);
  document.body.appendChild(host);
  try {
    const rootInverse = svg.getScreenCTM().inverse();
    const elements = new Map();
    for (const key of keys) {
      const el = svg.getElementById(key);
      if (!el || typeof el.getBBox !== 'function') continue;
      const bounds = getDocumentBounds(rootInverse, el);
      if (!(bounds.width > 0 && bounds.height > 0)) continue;
      const parent = el.parentNode;
      const parentMatrix = parent instanceof SVGGraphicsElement && parent !== svg
        ? rootInverse.multiply(parent.getScreenCTM())
        : new DOMMatrix();
      elements.set(key, { bounds, parentTransform: toMatrixAttr(parentMatrix) });
    }
    const defsMarkup = [...parsed.childNodes].map((n) => new XMLSerializer().serializeToString(n)).join('');
    return new Theme(getRootAttributes(parsed), defsMarkup, elements);
  } finally {
    host.remove();
  }
};
