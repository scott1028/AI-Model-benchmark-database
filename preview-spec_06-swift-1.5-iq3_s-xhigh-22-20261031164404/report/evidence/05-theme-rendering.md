# Theme assets (installed files)

Source: `/usr/share/kmines/themes/` (installed by the same package) — copied to
`preview-wip/tmp/themes/` for analysis (read-only; nothing modified).

## Installed themes

| .desktop | Display name | SVG file |
|---|---|---|
| `default.desktop` | **Default** ("KMines Oxygen theme", author Mark Taff) | `kmines_oxygen.svgz` |
| `classic.desktop` | Classic (author Mauricio Piacentini) | `kmines_classic.svgz` |
| `clean_blue.desktop` | Clean Blue | `kmines_clean_blue.svgz` |
| `clean_blue_dark.desktop` | Clean Blue Dark | `kmines_clean_blue_dark.svgz` |
| `green.desktop` | Green | `kmines_green.svgz` |
| `graveyard-mayhem.desktop` | Graveyard Mayhem | `graveyard-mayhem.svgz` |

Runtime config wrote `[KgTheme] Theme=themes/default.desktop` → **the default theme
is the Oxygen theme (`kmines_oxygen.svgz`)**.

## SVG layer ids (gzip-decompressed, `id=`/`inkscape:label=` extracted → `tmp/themes/layers.json`)

`kmines_oxygen.svgz` (default): `cell_up` (+`_plain/_color`), `cell_down` (+variants),
`arabicOne..arabicEight`, `mine`, `explosion`, `flag`, `question`, `error`,
`mainWidget`, `backgroundPanel`, `border.edge.{north,south,east,west}`,
`border.insideCorner.{nw,ne,sw,se}`, `border.outsideCorner.{nw,ne,sw,se}`,
`border.bay.{north,south,east,west}`, `smile`, `reload`, `checkmark`, `cross`, `logo`.

The sprite-key strings embedded in the binary (`kmines_utf16.txt`) are a subset:
`cell_up, cell_down, mine, explosion, flag, question, hint, error, arabicOne..Eight,
mainWidget, border.edge.*, border.outsideCorner.*, logo, settings, view-refresh,
themes, kmines, default` — i.e. exactly what KMines classes request from KGameRenderer.
(`smile`/`reload`/`checkmark`/`bay` layers exist in themes but no binary string
requests them → unused extra layers.)

Key match between binary strings and theme layers is 1:1 → **proven**.

## Rendering pipeline (proven)

- `KGameRenderer("kmines")` + KgTheme (`themes/default.desktop`)
- `KGameRenderer::renderPixmap(key, w, h)` / `spritePixmap(key, QSize)` (PLT 0xf590;
  observed runtime QSize 65×40, 345×23 in gdb probe)
- Cell = base (`cell_up`/`cell_down`) + symbol sprite composited (call sites 0x18092/0x1829a)
- `mainWidget` = central widget background; `border.*` = 9-patch board frame
- `logo` rendered 320px wide in rules dialog (FUN_00121b50 site 0x1305f)

## WebGL port mapping

`preview-wip/webgl/js/sprites.js` draws every requested key procedurally on a
Canvas2D atlas (no external assets allowed) using the **same key names**, so the
port's state→key table mirrors the original `stateKeyTable()` (FUN_00118810).
Visual style approximates the Oxygen default theme (glossy blue tiles).
