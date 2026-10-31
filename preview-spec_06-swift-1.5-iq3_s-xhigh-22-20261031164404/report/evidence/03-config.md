# Configuration schema (proven)

Source: `KConfigSkeleton::Item*` constructor calls in `Settings::setup` region
(objdump vaddr 0x113390..0x116210; keys verified as ASCII `kcfg_*` strings + UTF-16
key strings in `.rodata`), plus the real config file written by an offscreen run.

## `kcfg_*` key strings (ASCII `.rodata`)

```
2202d GeneralOptsConfig      (group name)
2204e kcfg_UseQuestionMarks
22064 kcfg_AllowKminesReset
2207a kcfg_DisableScoreOnReset
220aa kcfg_PlaceFlagOn
22116 CustomGameConfig       (group name)
22132 kcfg_CustomWidth
2214b kcfg_CustomHeight
22165 kcfg_CustomMines
229e0 kcfg_ExploreWithLeftClickOnNumberCells
```

UTF-16 `.rodata` adds the KConfigSkeleton property labels + `MouseRelease`/`MousePress`
(PlaceFlagOn enum entry values) and the page object names:

```
2247c CustomMines   22494 "custom mines"   224ae CustomHeight  224c8 "custom height"
224e4 CustomWidth   224fc "custom width"   22526 PlaceFlagOn   2253e place_flag_on
225b2 MouseRelease  225cc MousePress
22740 ExploreWithLeftClickOnNumberCells   22788 explore_with_left_click_on_number_cells
227d8 DisableScoreOnReset  22800 disable_score_on_reset  22830 AllowKminesReset
22858 UseQuestionMarks
225e8 games-config-custom  22610 games-config-theme  22638 games-config-options
```

## ItemInt / ItemBool / ItemEnum call sites

| Key | Type | Default | Range | Call evidence |
|---|---|---|---|---|
| UseQuestionMarks | ItemBool | false (0) | — | FUN_00115120, PLT ItemBool 0xfb50, arg edx=0 |
| AllowKminesReset | ItemBool | true (1) | — | FUN_00115260, edx=1 |
| DisableScoreOnReset | ItemBool | true (1) | — | FUN_001154f0, edx=1 |
| ExploreWithLeftClickOnNumberCells | ItemBool | true (1) | — | FUN_00114f60, edx=1 |
| PlaceFlagOn | ItemEnum | 0 (Released) | 0=Released, 1=Pressed | FUN_00114c40 (labels `Released`/`Pressed` at 0x220f3/0x220fc; entry values `MouseRelease`/`MousePress`) |
| CustomWidth | ItemInt | 10 | min 5, max 50 | FUN_00114e90 + setMinValue(5) PLT 0xf0d0 / setMaxValue(50) PLT 0xf430 |
| CustomHeight | ItemInt | 10 | min 5, max 50 | FUN_001153c0 |
| CustomMines | ItemInt | 20 | min 1 | FUN_00115650, cmp 1 at validation |

No `ShowTips` string exists in this build → no tips item in this version (the
"General" group is used by KGameScore/KGameTheme, not by an app tips setting).

## Observed runtime config file (redirected `$HOME`, non-destructive)

`preview-wip/tmp/fakehome/.config/kminesrc` produced by one offscreen launch:

```ini
[KgDifficulty]
Level=Easy

[KgTheme]
Theme=themes/default.desktop
```

`KgDifficulty` / `KgTheme` = KGameDifficulty + KGameTheme KConfigXT groups (libKDEGames6).

## KGameScore high-score storage

High-score lists are `KGameScore` objects saved via `KGameScore::save()` into KConfig
group `General` (`KGameScore::saveHighScoreList` writes `kmines_0..kmines_9` style keys;
UTF-16 string `kmines_reset` at 0x225a0 is the KGameClock reset counter key).
Proven: `kmines_reset` string + KGameScore PLT calls (`save`, `load`, `setValue`,
`disable`, 0x12c4f-0x12c9e).
