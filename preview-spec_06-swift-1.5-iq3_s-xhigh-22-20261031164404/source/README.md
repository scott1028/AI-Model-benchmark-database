# Reconstructed source (kmines 4:25.12.3-0ubuntu1)

C++-style reconstruction of `/usr/games/kmines`, derived **only** from the installed
binary, its libraries, installed themes/docs, and runtime observation (gdb, offscreen
runs). No upstream sources consulted.

## Evidence grading

- **A (proven)**: visible verbatim in disassembly/decompiled pseudocode, in installed
  files, or observed at runtime.
- **B (high confidence)**: structural inference from call graphs, Qt metacall strings,
  config schemas, PLT call sites.
- **C (inferred)**: names, signatures, and member layout reconstructed to fit evidence;
  exact upstream spelling unknown (stripped binary).

## Files

| File | Class | Main evidence |
|---|---|---|
| `minesfield.cpp` | `MineField` (cell grid model) | FUN_001191b0 (ctor), FUN_001193b0 (setSize, `QHash<int, QList<MineFieldItem*>>`), FUN_00119c40 (neighbours), FUN_00119620 / FUN_001198b0 (accessors), FUN_00119290 (cell ctor) |
| `minesscene.cpp` | `KMinesScene` (game logic) | FUN_0011a610 (ctor), FUN_0011a0b0 (place mines), FUN_0011a8f0 (open cell), FUN_0011b270 (cascade open), FUN_00117930 (updateItem → `KGameRendererClient::setSpriteKey`), FUN_00117ee0 (game over + score), FUN_001206d0 (newField / reveal all), FUN_0011f660 (press), FUN_0011fa60 (release), FUN_0011eb40 (shine neighbours), FUN_0011ee50 (hover), FUN_00119f20 (start), FUN_00119570 (reveal one) |
| `minesfielditem.cpp` | `MineFieldItem` (view item) | FUN_001173a0 / FUN_001176b0 / FUN_00117510 (ctors with x,y,w,h), FUN_00118810 (static state→sprite-key table at vaddr 0x18810) |
| `mainwindow.cpp` | `KMinesMainWindow` | FUN_00121b50 (score dialogs), FUN_0011c300 / FUN_0011f320 / FUN_0011ff10 (status/popup messages), KGameScoreDialog / KGameHighScoreDialog / KGameRulesDialog PLT calls |
| `view.cpp` | `KMinesView` | FUN_00124500 (view ctor), FUN_001202b0 / FUN_00120550 (scene wiring), view-refresh / view-certificate pixmaps |
| `settings.cpp` | `Settings` (KConfig) | FUN_00113390..FUN_00116210 group; keys, defaults and groups proven (see `evidence/config-schema.md`) |
| `customgameconfig.cpp` | `CustomGameConfig` | FUN_001128c0..FUN_00113210 (UI), FUN_00112820 (validator bounds), FUN_001126a0/00112740 (apply) |
| `generaloptsconfig.cpp` | `GeneralOptsConfig` | FUN_00111a60 / FUN_00111580 / FUN_00111290 (UI), strings `Inside game`, `Use '?' marks` |
| `main.cpp` | bootstrap | FUN_00122d80 (main): KCrash, KDEApplication, KMinesMainWindow, KDBusService |

## Address mapping note

Ghidra/REA imagebase = 0x100000, so **REA address = objdump vaddr + 0x100000**
(e.g. Ghidra `FUN_0011a610` = objdump `0x1a610`). Function-level Ghidra exports
(830 functions) are in `../tmp/rea_decompile/functions/0x<REA_ADDR>.c`; functions
not present in that export were analysed from full disassembly
(`../tmp/kmines_dis.txt`) and raw `.rodata` (`../tmp/kmines_rodata.txt`).
