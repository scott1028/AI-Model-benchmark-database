# KMines — WebGL port

A browser port of the KDE game `kmines` (4.0.25123), reverse engineered from
`/usr/games/kmines`.  The whole minefield — tiles, frame, numbers, flags, mines,
explosions, error marks and the message popups — is drawn with **WebGL**; the
toolbar and status bar are the browser equivalent of the Qt window chrome.

## Run it

Serve the **parent** folder of this deliverable and open the entry page:

```bash
cd <parent-of-the-deliverable-folder>
python3 -m http.server 8137 --bind 127.0.0.1
# then open  http://127.0.0.1:8137/<deliverable-folder>/index.html
```

`index.html` in the deliverable root forwards to `webgl/index.html`.
No network access, no external assets: everything is generated at runtime.

## Controls (same rules as KMines)

| input | action |
|---|---|
| left click | dig a cell (drag while held to line up several cells) |
| right click | covered → flag → question mark → covered |
| middle click, or left+right together | explore a number cell when its flags match |
| touch: tap | flag |
| touch: press and hold ≥ 1 s | dig |
| Ctrl+N / P / Ctrl+H / Ctrl+Q / F1 | new game, pause, high scores, quit, help |

Settings (General / Theme / Custom Game), difficulty (Easy 9×9/10, Medium 16×16/40,
Hard 30×16/99, Custom 5..50) and the per-difficulty high scores are kept in
`localStorage` (`webgl-kmines.settings`, `webgl-kmines.highscores`).

## Files

| file | role (original counterpart) |
|---|---|
| `js/renderer.js` | WebGL quad batcher (`QGraphicsView` + `KGameRenderer` raster output) |
| `js/theme.js` | sprite ids of the themes, drawn procedurally (`themes/*.svgz` elements) |
| `js/atlas.js` | packs the sprites into one texture (`KGameRenderer` pixmap cache) |
| `js/field.js` | `MineFieldItem` + `CellItem`: states, mine generation, flood fill, chord, win/lose |
| `js/scene.js` | `KMinesScene`: cell-size formula, frame ring, centring, popups, pause |
| `js/game.js` | `KMinesMainWindow`: actions, clock, status bar, input rules, dialogs |
| `js/storage.js` | `Settings` (kminesrc) and the high-score store |
| `index.html`, `styles.css` | window chrome (`KXmlGuiWindow` toolbar + status bar + `report/src/kminesui.rc`) |

See `../report/README.md` for the reverse-engineering evidence behind each rule.
