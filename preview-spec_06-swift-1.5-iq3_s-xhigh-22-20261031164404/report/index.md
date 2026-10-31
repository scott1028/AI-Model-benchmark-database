# Reverse-engineering report — `/usr/games/kmines`

Target: **KDE Minesweeper**, package `kmines 4:25.12.3-0ubuntu1`,
stripped x86-64 PIE ELF, SHA-256 `cc51df42feab795b5187631936a019ef35e47cdbac3a43a24928a6d17dff05ef`
(see `evidence/01-inventory.md`).

Scope rule honored: only the installed binary + installed files (themes, docs,
config) + runtime observation were used. No upstream/package source, no external
assets, no other `preview-*` outputs. No system modification; kmines was run
only sandboxed (offscreen, redirected `$HOME`) — `evidence/toolchain.md`.

## Deliverables

| Path | Content |
|---|---|
| `evidence/01-inventory.md` | binary facts, Qt meta strings, sprite keys, UI strings, kxmlgui rc, config, difficulty sizes |
| `evidence/02-method.md` | method + toolchain (versions, locations, non-destructive rules) |
| `evidence/03-config.md` | complete config schema (keys/groups/defaults, proven) |
| `evidence/04-game-logic.md` | recovered rules R1-R19 with addresses and grades |
| `evidence/05-theme-rendering.md` | theme assets, sprite-key ↔ SVG-layer proof, render pipeline |
| `evidence/function-map.md` | address → symbol table |
| `evidence/raw/` | verbatim evidence copies (UTF-16 strings, kminesui.rc, theme layers, observed kminesrc) |
| `../source/` | reconstructed C++-style source with inline evidence comments |
| `../webgl/` | WebGL2 port of the game (no external assets) |
| `../tmp/` | working artifacts: Ghidra export (830 fns), disassembly, strings, gdb probes, sandboxed HOME |

## Architecture (recovered)

```text
main: KCrash → KDEApplication → KMinesMainWindow.show → KDBusService → exec

KMinesMainWindow (KXmlGuiWindow)
 ├─ KGameDifficultyAction: addStandardLevelRange(30,50) + Custom(1000,"Custom")
 ├─ status labels: i18n("Time: %1")  i18n("Mines: %1/%2")  ["Time: 00:00","Mines: 0/0"]
 ├─ KGameScore + KGameScoreDialog/KGameHighScoreDialog (QQuickWidget-based)
 │     score = clock seconds (remaining), dialog title i18n("Time: %1")
 ├─ KMinesView (QGraphicsView: fitInView, wheel zoom, setTheme, viewportChanged)
 └─ KMinesScene (QGraphicsScene)
     ├─ KGameRenderer("kmines") + per-cell KGameRendererClient::setSpriteKey
     │     keys: cell_up cell_down mine explosion flag question hint error
     │           arabicOne..Eight mainWidget border.edge.* border.outsideCorner.*
     ├─ MineField: row-major 56-byte cells, neighbour QHash, minesAround @+0x30
     ├─ KGameClock: setTime(max(mines,499)) (08:19), setTime(5) first click,
     │     countdown under time pressure, pauseable (board hidden while paused)
     └─ settings: UseQuestionMarks, PlaceFlagOn(Released/Pressed),
                  ExploreWithLeftClickOnNumberCells, AllowKminesReset,
                  DisableScoreOnReset, CustomWidth/Height/Mines

toolbar (embedded kminesui.rc): game_new, game_pause
```

## Headline rules (full table in `evidence/04-game-logic.md`)

- First click is safe: mines are placed **after** it, excluding only the clicked
  cell, RNG `(rand()·(w·h))>>32` (proven, 0x1a0b0).
- Zero-adjacency flood fill; numbers `arabicOne..Eight`; chord on middle-click
  and (setting) left-click when flags around == number.
- Right-click flag/question cycle (marks setting); first right-click places the
  field first; flag timing follows `PlaceFlagOn` (default Released).
- Hover/press shine on cell + orthogonal neighbours.
- Countdown clock `max(mines,499)`s (08:19 standard), paused board disappears;
  score = remaining seconds; DisableScoreOnReset disables scores of reset games.
- Win "Congratulations! You have won!"; loss "You have lost." with explosion at
  the clicked mine and all mines revealed.
- Difficulty: Easy 9×9/10, Medium 16×16/40, Hard 30×16/99 (installed docbook),
  Custom 5-50 (defaults 10×10/20, min 1 mine).

## Confidence & gaps (honest)

1. Class/method **names** reconstructed from verbatim Qt meta strings + KDE APIs
   (binary stripped): signal/slot names proven; private helper names = C.
2. Ghidra slot-id → name mapping for KGameClock/KGameScore meta-calls = B.
3. Exact Easy/Medium/Hard size table is **not** a constant in the binary; sizes
   come from the installed official help (provenance: installed file).
4. state→sprite-key enum ordering = C (keys themselves proven).
5. Runtime `dqstr` QString decoding was unreliable in gdb (logs in `tmp/`);
   conclusions rest on `.rodata` + call-site analysis.
6. Theme art: sprite keys ↔ theme SVG layer ids match 1:1 (proven); the WebGL
   art is a procedural approximation, not the Oxygen SVG pixels.

## Verification (as the user would do it)

```sh
cd <parent of this folder>
python3 -m http.server 8141
# open http://127.0.0.1:8141/preview-wip/index.html
```

Headless Chrome (Playwright) results: entry redirects to `webgl/index.html`;
title OK; WebGL2 context active; board renders (62+ distinct canvas colors);
clicks work (clock runs, HUD updates); **zero console errors**
(`tmp/verify_kmines.py`, screenshot `tmp/verify_screenshot.png`).
