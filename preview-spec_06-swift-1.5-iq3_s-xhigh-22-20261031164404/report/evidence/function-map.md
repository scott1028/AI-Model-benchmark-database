# Function map (key addresses)

REA/Ghidra address = objdump vaddr + 0x100000. Exports: `tmp/rea_decompile/functions/0x<REA>.c`.
Confidence: A = read directly; B = call-graph inference; C = naming.

| Ghidra | vaddr | Reconstructed symbol | Confidence | Role |
|---|---|---|---|---|
| FUN_0011a610 | 0x1a610 | KMinesScene::KMinesScene | A/B | scene ctor: KGameRenderer("kmines"), KGameClock(…, 1), MineField, KGameScore, signal wiring |
| FUN_0011a0b0 | 0x1a0b0 | KMinesScene::placeMines | A | RNG loop `(rand()*(w*h))>>32`, skips ONLY clicked index, sets isMine, calls countMinesAround |
| FUN_0011a8f0 | 0x1a8f0 | KMinesScene::openCell | A/B | first-click → placeMines+clock start+firstClickDone; mine → explosion+gameOver; 0-adjacency → cascade |
| FUN_0011b270 | 0x1b270 | KMinesScene::openNeighbours | A/B | iterative flood fill (QQueue) of zero cells |
| FUN_00119c40 | 0x19c40 | MineField::neighbours | A | 8-neighbour gather, edges skipped |
| FUN_001191b0 | 0x191b0 | MineField ctor (model container) | A |
| FUN_00119290 | 0x19290 | MineFieldItem ctor (x,y,w,h) — model cell (56 B) | A (meta signature `MineFieldItem(int, int, int, int)`) |
| FUN_001193b0 | 0x193b0 | MineField::setSize | A (4-neighbour QHash, 1-based keys, emits minesCountChanged 0x1d439) |
| FUN_00119620 | 0x19620 | MineField::cellAt(x,y) | A | bounds check + row-major index |
| FUN_001198b0 | 0x198b0 | MineField::mineCount | B | counts isMine cells |
| FUN_00119a80 | 0x19a80 | MineField::addNeighbour | A | appends cell to per-cell neighbour list |
| FUN_00119b60 | 0x19b60 | MineFieldItem::incMinesAround | A | `+1` on cell+0x30 counter (call 0x19db5) |
| FUN_00119570 | 0x19570 | KMinesScene::revealCell | A/B | reveal as mine/explosion + updateItem |
| FUN_00119f20 | 0x19f20 | KMinesScene::start | B | scene start wrapper |
| FUN_00117ee0 | 0x17ee0 | KMinesScene::gameOver (+score) | A/B | clock stop, score value, DisableScoreOnReset check (0x17f35), emit gameOver/won |
| FUN_00117930 | 0x17930 | KMinesScene::updateItem | A | state lookup → KGameRendererClient::setSpriteKey (PLT 0xfa30) |
| FUN_00118810 | 0x18810 | state→spriteKey QHash (static init) | A | keys: hint, cell_up, error, mine, cell_down, flag, question, arabicOne..Eight (pool bytes 0x18810) |
| FUN_00117b20 / FUN_00117d80 | 0x17b20/0x17d80 | MineFieldItem paint/render | A/B | two renderPixmap calls (base+symbol), QImage compose |
| FUN_0011f660 | 0x1f660 | KMinesScene::mousePressEvent | A | gates: opened-number + ExploreWithLeftClickOnNumberCells (0x1fd3b); pending cell = this+0x118 (0x1fe1c); shine cell+neighbours |
| FUN_0011fa60 | 0x1fa60 | KMinesScene::mouseReleaseEvent | A | left: chord if opened number (flags==mines → openNeighbours, 0x20170-0x20198); PlaceFlagOn==Released path; right: marks cycle; middle: chord |
| FUN_0011eb40 | 0x1eb40 | KMinesScene::setShineOnNeighbours | A | 4 orthogonal neighbours shine (index±1 row-checked, index±width) |
| FUN_0011ee50 | 0x1ee50 | KMinesScene::hoverMoveEvent | A/B | un-shine old, shine cell under cursor (0x1f27e/0x1f2a9) |
| FUN_001206d0 | 0x206d0 | KMinesScene::newField | A/B | reset all cells, clock stopAndReset (0x2119b), reveal-all/gameOver paths |
| FUN_0011be70 | 0x1be70 | KMinesScene::setCustomField | B | custom w/h/mines |
| FUN_0011c300 / FUN_0011f320 | 0x1c300/0x1f320 | status/popup messages | B | i18n strings "You have lost." "Game is paused." "Reset the Game?" (0x222b6/0x222c5/0x221b3) |
| FUN_00121b50 | 0x21b50 | KMinesMainWindow::createGameScoreDialog | A/B | "Time: %1" score dialog (0x12b8e), KGameHighScoreDialog::addScoreDialog (0x12c9e), rules dialog logo 320px (0x1305f), difficulty addStandardLevelRange(30,50) (0x12f8c), Custom=1000 (0x12fb5) |
| FUN_00122d50 | 0x22d50 | KMinesMainWindow::pauseGame | A | KGameClock::pause (0x136c2) |
| FUN_00122d80 | 0x22d80 | main | A | KCrash::initialize, KDEApplication, KMinesMainWindow::show, KDBusService, exec |
| FUN_00113390..00116210 | 0x13390.. | Settings (KConfigSkeleton) | A | ItemBool/ItemEnum/ItemInt construction; keys+defaults proven |
| FUN_00114c40 | 0x14c40 | PlaceFlagOn ItemEnum | A | labels Released(0)/Pressed(1) |
| FUN_00111a60 / FUN_00111580 / FUN_00111290 | 0x11a60.. | GeneralOptsConfig page | A/B | "Inside game", "Use '?' marks", combo, checkboxes |
| FUN_001128c0 .. FUN_00113210 | 0x128c0.. | CustomGameConfig page | A | Width:/Height:/Mines: spinboxes + validators (0x112c5c-0x112e74) |
| FUN_001126a0 / FUN_00112740 | 0x126a0 | CustomGameConfig apply | A | setCustomWidth/Height/Mines |
| FUN_00124500 | 0x24500 | KMinesView::KMinesView | B | QGraphicsView config |
| FUN_001202b0 / FUN_00120550 | 0x202b0 | KMinesView::setScene / wheel zoom | B | fitInView/scale |
| FUN_0011d1b0 / FUN_0011d290 | 0x1d1b0 | QMetaSignal emitters | A | minesCountChanged / flaggedMinesCountChanged |
| FUN_0011d370 / 0011d430 / 0011d580 | 0x1d370 | save/load/read HighScores | B | KGameScore::save/load (0x12c4f…) |

## Qt metacall string evidence

See `inventory.md` (meta-object string blocks: class names, signals, slots,
constructor signatures verbatim in `.rodata`).

## Embedded kxmlgui resource (extracted, verbatim)

`tmp/kminesui_rc.xml` (from .rodata 0x23364): mainToolBar = `game_new`, `game_pause`;
empty MenuBar (menus come from KStandardAction/KXMLGUI defaults).
