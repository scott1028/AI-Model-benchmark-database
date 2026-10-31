# kmines 逆向報告（spec_06）

結論：`/usr/games/kmines`（KDE KMines 4:25.12.3，stripped ELF x86-64）的遊戲規則、輸入處理、設定項目、主題與介面行為，都從二進位與它安裝在系統上的檔案還原出來了。還原的 C++ 原始碼放在 `reconstructed-src/`，WebGL 移植版放在 `../webgl/`。只依靜態證據無法確定的少數行為，列在「未解與推論」。

## 1. 分析對象

| 項目 | 值 |
|---|---|
| 檔案 | `/usr/games/kmines`（166,928 bytes） |
| SHA-256 | `cc51df42feab795b5187631936a019ef35e47cdbac3a43a24928a6d17dff05ef` |
| GNU build-id | `15400df0baf38d248537a4f44d6ae691518ef071` |
| 套件 | `kmines 4:25.12.3-0ubuntu1 amd64`（`.note.package`） |
| 程式內版本字串 | `4.0.25123`（UTF-16 literal） |
| 相依 | Qt6 Widgets/Gui/Core、KDEGames6（KGameRenderer、KGameDifficulty、KGameClock、KGamePopupItem、KGameHighScoreDialog）、KF6 XmlGui/Config/I18n |
| 同套件安裝的檔案（有用到的） | `/usr/share/kmines/themes/*.desktop|*.svgz|*.png`、`/usr/share/doc/HTML/en/kmines/index.docbook`、`/usr/share/metainfo/org.kde.kmines.appdata.xml` |

## 2. 工具與環境

本次沒有下載或安裝任何新工具，全部使用本機已有的工具：

| 工具 | 版本 | 位置 | 用途 |
|---|---|---|---|
| REA（MCP） | 6.4.0 | `~/.nvm/versions/node/v22.22.3/bin/rea` | `open_binary`、`binary_overview`、`decompile_all`、`export_evidence_bundle` |
| Ghidra（REA 的 provider） | 12.1.4 | 由 REA 管理 | 反編譯 `.text` 內 175 個函式 |
| GNU objdump | 2.46 | `/usr/bin/objdump` | 讀立即值（預設值、範圍、逾時） |
| GNU strings、Python 3.12 | 系統內建 | — | UTF-8／UTF-16 字串、`.rodata` 位址解碼 |
| Google Chrome（headless） | 156.0.8078.79 | `/usr/bin/google-chrome` | 驗證 WebGL 版 |
| Node.js | v22.22.3 | nvm | 邏輯單元測試、CDP 驗證腳本 |

全程沒有使用 sudo、沒有修改系統檔案，也沒有執行 kmines 本身（避免在工作資料夾外產生 `kminesrc` 等設定檔）。

## 3. 方法

1. `open_binary` + `binary_overview`：1262 個 procedure（含 PLT／external），`.text` 範圍為 `0x10fbc0–0x121a79`（Ghidra image base `0x100000`）。
2. `decompile_all`（只取 `.text`）：175 個函式全部輸出到 `ghidra-decompiled/functions/<addr>.c`，索引在 `decompile-index.json`。
3. 字串：moc 字串表與 RTTI（`8CellItem`、`10BorderItem`、`13MineFieldItem`、`11KMinesScene`、`16KMinesMainWindow`、`16CustomGameConfig`、`17GeneralOptsConfig`）提供類別名與 signal／slot 名稱。UTF-16 `QStringLiteral` 提供 sprite key 與設定 key。
4. 用 objdump 追 `ki18n()` 呼叫點的 `lea` 參數，把每個 UI 字串對應到函式（例如 `0x11c325` → `"Congratulations! You have won!"`，`0x11c390` 分支 → `"You have lost."`）。
5. 從結構欄位的讀寫推出物件版面（見第 5 節），再逐一比對各函式的行為。
6. 匯出 REA 證據包：`evidence/rea-evidence-bundle.json`（3 筆記錄，0 個 unknown）。

## 4. 架構

```text
KMinesMainWindow (KXmlGuiWindow)            0x11a610 ctor
 ├─ KMinesView (QGraphicsView) ── resizeEvent → scene.resizeScene(w,h)
 ├─ KGameClock (FlexibleHourMinSec)          timeChanged → advanceTime
 ├─ status bar: "Mines: %1/%2" | "Time: %1" | difficulty combo
 └─ KMinesScene (QGraphicsScene)
     ├─ KGameRenderer(KGameThemeProvider("Theme"), themes/, default="default")
     ├─ MineFieldItem (QGraphicsObject)      game logic + mouse/touch
     │   ├─ CellItem × rows*cols  (KGameRenderedItem, overlays as children)
     │   └─ BorderItem × 2*(rows+cols)+4
     ├─ KGamePopupItem m_messageItem         win/lose, 4000 ms, opacity 0.9
     └─ KGamePopupItem m_gamePausedMessageItem  "Game is paused.", no timeout
```

Signal 流程：

```text
Time ─────────────────────────────────────────────────────────────►
User       | left release      |                 | reveal mine           |
MineField  | firstClickDone()  | flaggedMines..()| gameOver(false)       |
Scene      | → firstClickDone  | → minesCount..  | onGameOver: popup     |
MainWindow | onFirstClick:     | onMinesCount..: | onGameOver: clock     |
           |  clock resume,    |  "Mines: f/t"   |  pause, Reset? dialog |
           |  Pause enabled    |                 |                       |
```

## 5. 函式位址對照

| 位址 | 還原名稱 | 說明 |
|---|---|---|
| `0x10fc00` | `main` | KAboutData、作者／credits、KDBusService |
| `0x11a610` | `KMinesMainWindow::KMinesMainWindow` | 內嵌 KMinesScene／MineFieldItem 建構、動作、難度 |
| `0x116660` | `KMinesMainWindow::newGame` | 依難度呼叫 `startNewGame` |
| `0x117ee0` | `KMinesMainWindow::onGameOver` | Reset 對話框（內嵌 resetField）、高分 |
| `0x114110` | `onFirstClick` | 啟用暫停鈕、時鐘開始 |
| `0x114150` | `pauseGame(bool)` | |
| `0x1141a0` / `0x114400` | `onMinesCountChanged` / `advanceTime` | 狀態列文字 |
| `0x1142d0` | `showHighscores` | |
| `0x114570` | `configureSettings` | KConfigDialog 三頁 |
| `0x1144f0` | `loadSettings` | 重繪（不會開新局） |
| `0x118530` | `KMinesMainWindow::qt_static_metacall` | slot 索引 0–8 |
| `0x113fc0` / `0x114010` | `CustomGameConfig::updateMaxMines` | `max(1, w*h-10)` |
| `0x11e030` | `Settings::Settings` | kconfig_compiler 產物 |
| `0x11f4c0` | `Settings::self` | |
| `0x1206d0` | `KMinesScene::startNewGame` + `MineFieldItem::initField` | |
| `0x120020` | `KMinesScene::resizeScene` + `MineFieldItem::resizeToFitInRect` | |
| `0x11c300` | `KMinesScene::onGameOver` | 勝／負訊息 |
| `0x11f320` | `KMinesScene::setGamePaused` | |
| `0x11a0b0` | `MineFieldItem::generateField` | 佈雷 |
| `0x116240` | `MineFieldItem::adjacentRowColsFor` | |
| `0x119c40` | `MineFieldItem::adjacentItemsFor` | |
| `0x1196e0` | `MineFieldItem::adjustItemPositions` | |
| `0x118690` | `MineFieldItem::revealEmptySpace` | 遞迴展開 |
| `0x1197f0` | `MineFieldItem::onItemRevealed(int,int)` | 勝負判定 |
| `0x119b00` | `MineFieldItem::onItemRevealed(CellItem*)` | |
| `0x119a50` | `MineFieldItem::handleFlag` | |
| `0x11f660` / `0x11fa60` / `0x11eb40` | `mousePress/Release/MoveEvent` | |
| `0x1161e0` → `0x11ee50` | `sceneEvent` → 觸控處理 | TouchBegin/End/Cancel |
| `0x117930` | `CellItem::updatePixmap` | |
| `0x118790` | `CellItem::release(bool force)` | |
| `0x1187f0` | `CellItem::undoPress` | |
| `0x117e70` | `CellItem::mark` | 旗子／問號循環 |
| `0x118810` | `CellItem::fillNameHashes` | sprite key 表 |
| `0x111d60` `0x112a20` `0x116ca0` | `QHash::operator[]` 實例化 | state／digit／border 名稱表 |

物件欄位（offset 由 pseudocode 讀寫推出）：

- `CellItem`：`+0x28 state`、`+0x2c hasMine`、`+0x2d exploded`、`+0x30 digit`
- `BorderItem`：`+0x28 type`、`+0x2c row`、`+0x30 col`
- `MineFieldItem`：`+0x28 cells`、`+0x40 borders`、`+0x50 cellSize`、`+0x54 rows`、`+0x58 cols`、`+0x5c mines`、`+0x60 flagged`、`+0x64 touchPos`、`+0x70 touchStartMs`、`+0x78 leftButtonPos`、`+0x80 midButtonPos`、`+0x88 firstClick`、`+0x89 gameOver`、`+0x8a emulatingMidButton`、`+0x8c numUnrevealed`、`+0x90 renderer`
- `Settings`：`+0x18 UseQuestionMarks`、`+0x19 AllowKminesReset`、`+0x1a DisableScoreOnReset`、`+0x1b ExploreWithLeftClickOnNumberCells`、`+0x1c PlaceFlagOn`、`+0x20 CustomWidth`、`+0x24 CustomHeight`、`+0x28 CustomMines`

## 6. 還原出的遊戲規則（觀察）

- **難度**（`0x116660`）：Easy 9×9／10 雷、Medium 16×16／40、Hard 16 列 × 30 欄／99、Custom（讀 Settings）。難度範圍是 `addStandardLevelRange(30, 50)` 再加上值為 1000 的 `"Custom"`。
- **地雷上限**（`0x1206d0`）：`mines = min(mines, rows*cols-10)`。
- **佈雷時機**（`0x11fa60`、`0x11a0b0`）：第一次左鍵放開時才佈雷。用 `QRandomGenerator64::global()` 種子建立新的 `QRandomGenerator`，再以 `bounded(rows*cols)` 抽格子；首擊格與它的 8 個鄰格不會有雷，所以首擊一定會展開一片。之後每個雷的鄰格 digit +1。
- **格子狀態**：Released、Pressed、Revealed、Questioned、Flagged、Error、Hint。sprite 對應（`0x118810`）如下：
  - Released → `cell_up`
  - Pressed／Revealed → `cell_down`
  - Questioned → `cell_up`+`question`
  - Flagged → `cell_up`+`flag`
  - Error → `cell_down`+`mine`+`error`
  - Hint → `cell_up`+`hint`
- **Revealed 疊圖**（`0x117930`）：digit>0 疊 `arabicOne…Eight`；digit=0 且有雷時，爆炸格先疊 `explosion`，再疊 `mine`。
- **右鍵 mark**（`0x117e70`）：Released→Flagged→（UseQuestionMarks ? Questioned : Released），Questioned→Released。插旗時機由 `PlaceFlagOn` 決定：0 為放開時（預設），1 為按下時。
- **展開**（`0x118690`）：只會打開 Released／Pressed／Hint 的鄰格，遇到 digit=0 就遞迴。
- **輸了**（`0x1197f0`）：翻到雷時，所有「有雷但沒插旗」的格子翻成 Revealed，「插旗但沒雷」的格子變成 Error。只要有任何 exploded 格，就送出 `gameOver(false)`。
- **贏了**：未翻開格數 == 地雷數時，剩下的格子全部自動插旗，接著送出 `flaggedMinesCountChanged(mines)` 和 `gameOver(true)`。
- **中鍵／和弦**（`0x11f660`、`0x11fa60`）：中鍵，或左右鍵同按，或勾選「Explore with left click on number cells」時在已翻開格上按左鍵，都會把 3×3 鄰格壓下。放開時，若「鄰格旗數 == 鄰格雷數 且 ≠ 0」就 `release(force)` 所有鄰格，否則全部彈回。
- **拖曳**（`0x11eb40`）：按住左鍵移動時，壓下狀態跟著游標走；中鍵模式下，3×3 壓下區塊跟著移動。
- **觸控**（`0x11ee50`）：只接受單指，gameOver 時忽略。第一次觸控一定翻開；之後短按（<1000 ms）在未翻開格上切換旗子，在數字格上做和弦；長按則翻開。手指移到別格或場外會取消。
- **Reset**（`0x117ee0`）：輸了且 `AllowKminesReset`（預設 true）時，問「Reset the Game?」（Reset／Cancel）。選 Reset 會保留原本的雷與數字，所有格子回到 Released，時鐘繼續走，`m_gameCanScore = !DisableScoreOnReset`。
- **高分**：贏且 `m_gameCanScore` 為真時，開 `KGameHighScoreDialog(Name|Time)`，隱藏 Score 欄，以秒數比較（LessIsMore）。
- **暫停**：盤面 `setVisible(false)`，顯示「Game is paused.」（不會自動消失，點擊也不會關閉）。
- **時鐘**：開新局時 restart 並暫停，第一次點擊才開始，遊戲結束時暫停。
- **版面**（`0x120020`）：`cellSize = int(min(W/(cols+2), H/(rows+2)))`，盤面置中，外圈一格是邊框，背景是 `mainWidget` sprite 放大到整個場景。
- **邊框**（`0x1206d0`）：四角用 `border.outsideCorner.{nw,ne,sw,se}`，四邊用 `border.edge.{north,south,west,east}`。

## 7. 設定（`reconstructed-src/kmines.kcfg`）

| Key | 預設 | 範圍 |
|---|---|---|
| UseQuestionMarks | true | |
| AllowKminesReset | true | |
| DisableScoreOnReset | false | |
| ExploreWithLeftClickOnNumberCells | false | |
| PlaceFlagOn | MouseRelease (0) | MouseRelease / MousePress |
| CustomWidth | 10 | 5–50 |
| CustomHeight | 10 | 5–50 |
| CustomMines | 20 | 1–(w×h−10)（執行時更新上限） |

`kminesui.rc` 是直接從 `.rodata` 原樣取出的。

## 8. 未解與推論

| 項目 | 狀態 | 說明 |
|---|---|---|
| `KMinesScene+0x10`（`m_gameCanScore`）初始值 | 未解 | 全檔掃描所有寫入 `0x10(%reg)` 的指令，只有 Reset 路徑（`0x118453`）會寫它；靜態分析找不到開局時的初始化。WebGL 版的設計是每次開新局設為 true。 |
| 第一次啟動的預設難度 | 未解 | 由 libKDEGames 的 `KGameDifficulty` 決定，不在 kmines 檔案內。WebGL 版預設 Easy，之後記住上次選擇。 |
| 遊戲結束後的滑鼠輸入 | 觀察（保留） | 滑鼠處理函式不檢查 `m_gameOver`（只有觸控會檢查），二進位也沒有任何 `setEnabled`／`setAcceptedMouseButtons` 呼叫。所以輸了之後再翻其他格，會因為仍有 exploded 格而再次送出 `gameOver(false)`。WebGL 版照實保留。 |
| Hint 狀態 | 觀察 | 名稱表有 `hint`，但沒有任何程式把格子設為 6；Solver／Adviser 只出現在 credits，這個版本沒有提示功能。 |
| KGamePopupItem／KGameClock／高分對話框的外觀細節 | 推論 | 這些屬於 libKDEGames，不在 kmines 檔案內。WebGL 版依顯示文字與參數（置中、0.9 不透明度、4 秒）做近似實作。 |
| 還原的 C++ 能否編譯 | 未驗證 | 本機沒有 KF6／Qt6 開發標頭，無法編譯。這份原始碼用來閱讀與對照，不保證能直接 build。 |

## 9. WebGL 移植版（`../webgl/`）

| 模組 | 對應原始碼 |
|---|---|
| `src/components/MineField.js` | MineFieldItem + CellItem（純狀態機，逐函式對應上表位址） |
| `src/components/ThemeLoader.js` | KGameRenderer：讀 SVG，依元素 id 算出文件座標 bounds，拉伸到指定大小後光柵化成 sprite sheet |
| `src/components/Renderer.js` | QGraphicsView 繪製：背景、邊框、格子與疊圖、popup，全部是 WebGL textured quads |
| `src/components/Popup.js` | KGamePopupItem 近似 |
| `src/index.js` | KMinesMainWindow + KMinesScene：時鐘、暫停、Reset、高分、設定對話框、滑鼠抓取語意、觸控 |
| `src/constants.js`、`src/utils.js` | 列舉、sprite 表、預設值、版面計算 |
| `themes/` | 由 `/usr/share/kmines/themes/*.svgz` 解壓的 SVG 和 preview PNG（kmines 自己安裝的素材，沒有下載任何外部素材） |

設計取捨：

- 設定、難度、高分存在 `localStorage`，對應原本的 `kminesrc` 和 KGameHighScoreDialog。
- 瀏覽器會保留 Ctrl+N／Ctrl+H 給自己用，因此工具列按鈕才是主要入口。

## 10. 驗證

- `../tmp/tests/mine-field.test.mjs`（`node --test`）：11／11 通過。涵蓋地雷上限、首擊安全與 digit 正確（30 個種子）、旗子循環、MousePress 插旗、輸／贏、和弦、觸控短按／長按、Reset 保留雷、邊框配置。
- `../tmp/verify.mjs`：在上層資料夾啟動 `python3 -m http.server`，headless Chrome 開 `/preview-wip/index.html`，確認被導向 `/preview-wip/webgl/index.html`、WebGL context 可用、首擊後時鐘開始、右鍵後顯示「Mines: 1/10」、暫停時盤面隱藏、6 個主題都載入成功，console 沒有任何訊息或錯誤。
- `../tmp/verify-flow.mjs`：5×5 自訂局測試。1 雷時首擊即勝，會開高分對話框並寫入分數；15 雷時踩雷會出現 Reset 對話框，選 Reset 後恢復可玩；設定對話框可正常開關。console 無錯誤。
- 截圖：`verification/shots/*.png`。

## 11. 目錄

```text
report/
├─ README.md                 本報告
├─ reconstructed-src/        還原的 C++（main、mainwindow、scene、minefielditem、cellitem、borderitem、kmines.kcfg、kminesui.rc）
├─ ghidra-decompiled/        Ghidra 原始 pseudocode（175 個函式）+ index/journal
├─ evidence/                 REA 證據包
└─ verification/             截圖與驗證結果 JSON
```

reference01: `/usr/games/kmines`（sha256 `cc51df42…05ef`，build-id `15400df0…f071`）
reference02: `/usr/share/kmines/themes/*.desktop`、`*.svgz`
reference03: `/usr/share/doc/HTML/en/kmines/index.docbook`（KMines Handbook，安裝於本機）
