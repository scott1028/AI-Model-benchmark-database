# KMines — WebGL port

A browser re-implementation of the KDE Minesweeper game (`/usr/games/kmines`,
`4:25.12.3-0ubuntu1`), built strictly from the reverse-engineering evidence in
`../report/` and `../source/`. No external art, no external libraries:
plain ES5-style scripts, one WebGL2 renderer, all sprites drawn procedurally.

## Files

| File | Mirrors | Notes |
|---|---|---|
| `index.html` | `KMinesMainWindow` + `kminesui.rc` | head panel (Time/Mines labels + reset, mirroring the two imported status-bar labels), toolbar row (new/pause/highscores/settings/difficulty), board viewport, settings dialog (`GeneralOptsConfig` + `CustomGameConfig` pages), highscore dialog (`KGameHighScoreDialog`) |
| `js/field.js` | `MineFieldItem` + `MineField` | grid model, states, `placeMines` RNG `(r*n)>>32` (first click excludes only the clicked cell), flood fill, chord/explore, flag cycle |
| `js/app.js` | `KMinesScene` + `KMinesMainWindow` slots | levels Easy 9×9/10, Medium 16×16/40, Hard 30×16/99, Custom 1000 (5-50, defaults 10/10/20); settings with the proven keys/defaults; clock `Time: mm:ss`; messages "You have lost.", "Congratulations! You have won!", "Game is paused.", "Reset the Game?" |
| `js/renderer.js` | `KGameRenderer` + `QGraphicsView` | WebGL2 batched-sprite renderer (atlas texture, alpha blend, DPR-aware) |
| `js/sprites.js` | theme `*.svgz` layer ids | procedural atlas keyed by the binary's exact sprite keys (`cell_up`, `cell_down`, `mine`, `flag`, `question`, `explosion`, `hint`, `error`, `arabicOne..Eight`, `mainWidget`, `border.edge.*`, `border.outsideCorner.*`) |

## Rules reproduced (see `../report/index.md` for evidence)

- Left click opens; first click places mines **after** the click, excluding only
  that cell (matching the native loop at vaddr 0x1a0b0), then starts the clock.
- Zero-adjacency flood fill; numbers use `arabicOne..Eight` sprite keys.
- Right click cycles: none → flag → (question if "Use '?' marks") → none.
  Flag timing honors `PlaceFlagOn` (Released default / Pressed).
  Right-clicking before the first click places mines around that cell first.
- Middle click on an opened number, and left click on an opened number
  (`Explore with left click on number cells`, default on), opens all
  unflagged neighbours when the surrounding flag count equals the number.
- Hovering shines the cell and its orthogonal neighbours (native
  `setShineOnNeighbours`).
- Loss: explosion on the clicked mine, all mines revealed; win: all mines
  flagged/revealed, time recorded to the level's highscore list (localStorage,
  mirroring `KGameScore`).
- Pause stops the clock and shows "Game is paused."; mid-game reset asks
  "Reset the Game?" and can disable the score (`DisableScoreOnReset`, default on).

## How to run

Serve the parent folder and open the entry:

```sh
cd <this folder's parent>
python3 -m http.server 8137
# open http://127.0.0.1:8137/preview-wip/index.html
```

The root `preview-wip/index.html` redirects to `webgl/index.html`.

## Known deviations from native

- Sprite art is procedural (no external assets allowed) — visual style only.
- Highscores kept in `localStorage` per level (native: KConfig).
- No D-Bus single-instance, no KCrash, no KDE theme chooser.
