# 04 — Game logic (recovered rules, evidence-graded)

Grades: **A** = read verbatim from disassembly/pseudocode, **B** = structural
inference (call graph, Qt meta strings, observed runtime), **C** = naming.

## Program map

```text
main (FUN_00122d80)
  KCrash::initialize → KDEApplication → KMinesMainWindow::show → KDBusService → exec

KMinesMainWindow (KXmlGuiWindow)
  members: KGameClock @+0x60, KGameScore @+0xc0, KMinesView, KGameDifficultyAction
  KGameDifficultyAction::addStandardLevelRange(30, 50)      (0x12f8c)
  KGameDifficultyLevel(1000, "Custom", i18n("Custom"), false) (0x1b670)
  KGameDifficultyAction::setLevelIcon("kmines")             (0x12fd0)
  toolbar (kminesui.rc): game_new, game_pause

KMinesScene (QGraphicsScene)
  KGameRenderer("kmines"); background = spritePixmap("mainWidget", QSize(...))
  KGameClock(this, CountUp); MineField; KGameScore
  signals: firstClickDone, gameOver, won, flaggedMinesCountChanged(int),
           minesCountChanged(int)
  slots:   newGame, onGameOver, advanceTime, onFirstClick, showHighscores,
           configureSettings, pauseGame, paused(bool), loadSettings, updateMaxMines

MineField / MineFieldItem
  cell struct = 56 bytes (0x38 imul at 0x11929d): state @+0x28, isMine @+0x2c,
  flag/question/opened/shine bytes @+0x2e..0x31, minesAround @+0x30,
  neighbour QHash @+0x20 keyed by padded 1-based index (setSize FUN_001193b0)
```

## Core rules

| # | Rule | Grade | Evidence |
|---|---|---|---|
| R1 | Mines are placed **after the first click**; only the clicked cell is excluded from placement | A | FUN_0011a0b0 (placeMines): loop `while (minesToPlace)`; `if (idx == clicked) continue` — no neighbour exclusion |
| R2 | Placement RNG: `idx = (uint64)rand() * (w*h) >> 32` (uniform multiply-shift) | A | 0x1ab2c-0x1ab38: `imul rbp, rax; shr rbp, 0x20` |
| R3 | Clock starts at first click (slot call 0x1ac2e), time in status = `KGameClock::timeString()` | A/B | imports: timeString, seconds; slot/vtable sites |
| R4 | Opening a mine → cell sprite `explosion`, all other mines revealed as `mine`, game over (lost) | A/B | FUN_00119570 reveal sites, "explosion"/"mine" keys, gameOver FUN_00117ee0 |
| R5 | `minesAround == 0` → flood fill cascade (iterative queue) | A/B | FUN_0011a8f0 openCell → FUN_0011b270 cascade; per-cell minesAround at +0x30 incremented via FUN_00119b60 |
| R6 | Opened numbers = sprite keys `arabicOne..arabicEight` (child sprite over `cell_down`) | A | FUN_00117930 updateItem → KGameRendererClient::setSpriteKey (PLT 0xfa30); keys UTF-16 |
| R7 | Right click cycles marks: none → flag → (question if UseQuestionMarks) → none | A/B | FUN_0011fa60 (0x2019c-0x201e8) `settings()->useQuestionMarks()` gate |
| R8 | Right click before first click places mines first, so flags are laid on the real field | A | FUN_0011fa60 0x201f2-0x2022d: placeMines with clicked cell excluded, on right press |
| R9 | Flag timing = setting `PlaceFlagOn`: 0 Released (default) / 1 Pressed | A | ItemEnum ctor FUN_00114c40, labels Released(0)/Pressed(1) |
| R10 | Middle-click chord: if flags around == number → open all unflagged neighbours | A | FUN_0011fa60 0x20170-0x20198 (count flags, cmp minesAround, FUN_0011b270) |
| R11 | Left-click on opened number cells explores (chord) when `ExploreWithLeftClickOnNumberCells` (default true) | A | FUN_0011f660 0x1fd3b gate; ItemBool default arg 1 |
| R12 | Pressing/hovering a closed cell shines it; pressing also shines the 4 orthogonal neighbours (updateItem per neighbour) | A | FUN_0011eb40 (setShineOnNeighbours: index±1 with row checks, index±width), FUN_0011ee50 hover |
| R13 | Win when every non-mine cell opened → signal `won`, popup "Congratulations! You have won!", score = elapsed seconds | A/B | FUN_00117ee0 (score = clock seconds via KGameScore::setValue); FUN_001206d0 win path |
| R14 | Loss popup "You have lost."; clock stops on game over | A | 0x222b6 string; clock stop site 0x17d1d |
| R15 | Pause: `game_pause` action; board hidden; status "Game is paused."; clock paused | A/B | meta slots pauseGame/paused(bool); 0x222c5 string; docbook "board disappears when paused" |
| R16 | New game while playing asks "Reset the Game?"; with DisableScoreOnReset (default true) the score of a reset game is disabled | A/B | 0x221b3 string; gameOver score-disable site (0x17f35 `KConfig("kmines").Group("General")` read "DisableScoreOnReset", default true) |
| R17 | Highscores: KGameScoreDialog / KGameHighScoreDialog (QQuickWidget-based) per difficulty; dialog title format `i18n("Time: %1").arg(score)` | A/B | PLT KGameScoreDialog/KGameHighScoreDialog + QQuickWidget::setGeometry 0x12c9e; 0x12b8e/0x12c1a |
| R18 | Difficulty change → new board with level size (Easy/Medium/Hard/Custom 1000); custom sizes 5..50, mines ≥1 and < w*h (max-mines update slot `updateMaxMines`) | A/B | meta slot `updateMaxMines`; KRestrictedNumeric validators at 0x112c5c-0x112e74 |
| R19 | Custom game page applies CustomWidth/Height/Mines via Settings, min 5/5/1, defaults 10/10/20, max 50 | A | ItemInt ctors + setMinValue(5) PLT 0xf0d0, setMaxValue(50) PLT 0xf430; validator cmp 1 |

## State → sprite key table (FUN_00118810 static QHash init, vaddr 0x18810)

Keys decoded from the pool bytes (proven A):
`hint, cell_up, error, mine, cell_down, flag, question` (+ number keys
`arabicOne..arabicEight` assembled as `"arabic" + {One..Eight}` in
FUN_00117b20/FUN_00117d80). Exact enum ordering of states = C.

## Scene event handler address map

| Function (Ghidra) | vaddr | Role |
|---|---|---|
| FUN_0011f660 | 0x1f660 | mousePressEvent (press handling, pending item at scene+0x118, shine) |
| FUN_0011fa60 | 0x1fa60 | mouseReleaseEvent (open/chord/flag cycle; PlaceFlagOn branches) |
| FUN_0011ee50 | 0x1ee50 | hoverMoveEvent (shine old/new) |
| FUN_0011eb40 | 0x1eb40 | setShineOnNeighbours |
| FUN_0011a8f0 | 0x1a8f0 | openCell (first-click path, cascade trigger, win check) |
| FUN_0011b270 | 0x1b270 | cascade open (queue flood fill) |
| FUN_0011a0b0 | 0x1a0b0 | placeMines (RNG + first-click exclusion) |
| FUN_00117930 | 0x17930 | updateItem → KGameRendererClient::setSpriteKey |
| FUN_00117ee0 | 0x17ee0 | onGameOver (clock stop, score, DisableScoreOnReset read) |
| FUN_001206d0 | 0x206d0 | newField / reveal-all (game over board) |
| FUN_00119570 | 0x19570 | revealCell (mine / explosion) |

Full export set: `tmp/rea_decompile/functions/` (Ghidra pseudocode, 830 fns) +
`tmp/kmines_dis.txt` (full disassembly).
