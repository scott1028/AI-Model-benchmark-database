# 01 — Target inventory

## Binary

| Property | Value | Evidence |
|---|---|---|
| Path | `/usr/games/kmines` | — |
| Package | `kmines 4:25.12.3-0ubuntu1` | `dpkg -s kmines` |
| Type | ELF 64-bit LSB pie executable, x86-64, **stripped**, dynamically linked | `file` |
| SHA-256 | `cc51df42feab795b5187631936a019ef35e47cdbac3a43a24928a6d17dff05ef` | `sha256sum` |
| Size | 508,104 bytes | — |
| Ghidra/REA imagebase | 0x100000 (function addr = vaddr + 0x100000) | REA |

NEEDED: `libKDEGames6`, `libKFavIcons6`-style KF6 libs, `libKXmlGui6`, `libKConfigWidgets6`,
`libQt6Core/Gui/Widgets/Qml/QmlWidgets/Svg/SvgWidgets/DBus`, `libstdc++` (from `readelf -d`).

Key linked classes (demangled imports, `tmp/imports_demangled.txt`):
`KGameRenderer`, `KGameRendererClient`, `KGameTheme`, `KGameClock`, `KGameScore`,
`KGameDifficulty`, `KGameDifficultyAction`, `KGameDifficultyLevel`, `KGameRulesDialog`,
`KGameScoreDialog`, `KGameHighScoreDialog`, `KConfigSkeleton::Item{Bool,Int,Enum}`,
`KConfig`/`KConfigGroup`, `KXmlGuiWindow`, `KStandardGameAction`, `KToggleAction`,
`KStandardAction`, `KMessageWidget`, `KGamePopupItem`-style `showMessage`,
`QGraphicsScene`, `QGraphicsView`, `QGraphicsSceneMouseEvent`.

## Qt meta-object strings (verbatim in `.rodata`, proven)

```text
KMinesMainWindow
  newGame  showHighscores  configureSettings  updateMaxMines
  onFirstClick  advanceTime  onGameOver  paused(bool)  showHighscores  configureSettings
KMinesView
  signals: viewportChanged()
  slots : setScene(KMinesScene*)  setTheme(QString const&)  fitInView()
          fitToScreen()  zoomIn()  zoomOut()
KMinesScene
  signals: firstClickDone()  gameOver()  won()
           flaggedMinesCountChanged(int)  minesCountChanged(int)
  slots  : newGame()  onGameOver()  advanceTime()  onFirstClick()
           showHighscores()  configureSettings()  pauseGame()  paused(bool)
           loadSettings()  updateMaxMines()
MineFieldItem
  MineFieldItem(int, int, int, int)
Settings  CustomGameConfig  GeneralOptsConfig   (KConfigSkeleton classes)
KMinesMainWindow::KMinesMainWindow(QWidget*)
```

## Sprite keys requested by the binary (UTF-16 `.rodata`, proven)

`cell_up`, `cell_down`, `mine`, `flag`, `question`, `explosion`, `hint`, `error`,
`arabicOne` … `arabicEight`, `mainWidget`, `backgroundPanel`,
`border.edge.{north,south,east,west}`,
`border.outsideCorner.{nw,ne,sw,se}`,
`view-refresh`, `settings`, `themes`, `logo`, `kmines` (renderer theme object name),
`default`.

Full decode: `raw/kmines_utf16.txt`.

## UI / i18n strings (proven)

- `Time: %1`, `Time: 00:00`, `Mines: 0/0`, `Mines: %1/%2` (status bar format)
- `You have lost.`
- `Congratulations! You have won!`
- `Game is paused.`
- `Reset the Game?`
- `Inside game`, `Use '?' marks` (general options page labels)
- `Width:`, `Height:`, `Mines:` (custom game page labels)
- `Released`, `Pressed` (PlaceFlagOn enum labels)
- `Custom` (custom difficulty level label)
- debug: `Invalid cell coordinates (x: %1, y: %2)`,
  `No item found at given coordinates (x: %1, y: %2)`

## Embedded kxmlgui resource (recovered verbatim, `raw/kminesui_rc.xml`)

`mainToolBar`: `game_new`, `game_pause`; empty `<MenuBar>` (menus from KStandardAction defaults).

## Observed user config (`raw/kminesrc.observed`, real run)

```ini
[KgDifficulty]
Level=Easy

[KgTheme]
Theme=themes/default.desktop
```

## Difficulty board sizes

Binary proves `KGameDifficulty::addStandardLevelRange(30, 50)` (Easy/Medium/Hard are
libKDEGames6 standard levels 30/40/50), `KGameDifficultyLevel(1000, "Custom",
i18n("Custom"))` (0x1b670), and a config key "Custom". The per-level sizes come from
the installed official help `/usr/share/doc/HTML/en/kmines/index.docbook`:

| Level | Board | Mines |
|---|---|---|
| Easy | 9×9 | 10 |
| Medium | 16×16 | 40 |
| Hard | 30×16 | 99 |
| Custom | user, 5..50 (defaults 10×10, 20 mines, min 1) |
