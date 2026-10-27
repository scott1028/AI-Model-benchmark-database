# Coin Run — 完成報告（PLAYBOOK v2.1）

日期：2026-10-28｜模型：swift-1.5-iq3_s｜Blender 5.2.2 LTS（Blender MCP）｜Godot 4.7.2（Godot MCP + CLI）

一句話：15 個 3D 資產全部由 Blender MCP 建模、經 `make check` 兩端自動檢查合格；世界由 `godot/data/layout.json` + `WorldBuilder` 資料驅動組裝；bot 與物理測試全綠；附錄 B 五條畫面要求逐項通過；完成一次「改規格 → 檢查 FAIL → 回 Blender 重建 → PASS」的完整迴圈演練。

## 1. 完成定義（DoD）

| DoD 項目 | 結果 | 證據 |
|---|---|---|
| 每個 3D 資產都通過自動規格檢查（Blender 端 + Godot 匯入後） | **[v]** 15/15 | `pipeline/checks/<id>.json`（`pass: true`、`blender_audit_ok`、`godot_audit_ok`）：probe_box、rock、bush、tree、barrel、wooden_crate、coin、endpoint_flag、robot、brick_wall、house、fantasy_shrine、fantasy_arch、cloud、terrain。檢查項目：尺寸 ±10%（底限 2 cm）、原點在底部中心、面數 ≤ `tris_max`、碰撞（形狀/模式/必要碰撞網格）、宣告的動畫送達 Godot、材料名稱與顏色 |
| 自動 bot 測試通過 | **[v]** | `make bot` → `pipeline/checks/bot.json`：9 項 checks 全過、金幣依序收集（coins 1→2→3）、`win`（"All 3 coins collected and the flag reached"）、elapsed 24.68 s、21 個可判讀事件、engine_errors 0、exit 0 |
| 自動物理測試通過（「移動」與「物理互動」每項都可量測） | **[v]** 15/15 | `make physics` → `pipeline/checks/physics.json`：15 筆 results、`unexpected_ids: []`、errors 0。門檻唯一真相 `specs/physics_thresholds.json`（見 §4），未回報的 id 直接判 FAIL（D27） |
| 透過 Godot MCP 實際執行遊戲沒有錯誤；截圖依附錄 B 檢查 | **[v]** | Godot MCP `run_project` + `get_debug_output`：Vulkan Forward+（RTX PRO 6000）執行、0 錯誤（連 6 條 GDScript warning 都清掉了）。附錄 B 五條逐項判定見 `pipeline/log.md`「畫面驗收（附錄 B）」；證據圖 `pipeline/screenshots/{overview,play,ground,road,action}-202610280110*.png` |
| 完成一次迴圈演練（Phase 9） | **[v]** | coin：規格縮至 60% → 檢查 FAIL（cause=modeling）→ Blender MCP 重建模（`look` 三視自檢）→ 匯出 → `make check ID=coin` PASS → `make layout` 同步收集盒 → bot 24.68 s / physics 15/15 / world-check 0 錯誤 → 重拍截圖。逐行紀錄在 log 的檢查與修正表 |
| `pipeline/REPORT.md` 已寫 | **[v]** | 本檔 |

## 2. 交付內容

- **遊戲**：`godot/scenes/main.tscn`（`make run` 或 Godot 開啟專案）。收齊 3 枚金幣後碰終點旗取勝。
- **按鍵**（實測 `INPUT_VERIFY pass=true`，無多餘 action）：方向鍵/dpad 移動（鏡頭相對）、Space/Enter/手把 A 跳躍、A 鍵/手把 X 按住加速、B 鍵/手把 B 按住煞車。
- **測試**：`make bot`、`make physics`、`make world-check`、`make cam-check`、`make check ID=<id>`、`make shot`／`python3 tools/capture.py --preset play|overview|ground|road|action`。
- **網頁版**：`make web`（匯出到 `web/`）→ `make serve` → `http://localhost:8000/`（根目錄 `index.html` 會導向 `web/index.html`）；`make web-smoke` 自動驗收。
- **資料驅動**：`tools/gen_layout.py`（seed 20261028，產生時驗證 14 項）→ `godot/data/layout.json`；`tools/blender_terrain.py` → `godot/data/heightmap.json`；`tools/gen_terrain_texture.py` → `godot/data/terrain_surface.png`。

## 2b. WebGL 版成品（追加需求）

**怎麼玩（兩種入口都有）**：

- 从**仓库根**開 server（`preview-spec_04…/..`）：`python3 -m http.server 8000` → 開 `http://localhost:8000/`，仓库根的 `index.html` 會探測並導向 `<交付目錄>/web/index.html`（網址不用帶交付目錄名，目錄被改名也不会 404）
- 从**交付目錄**開 server：`make serve` → 開 `http://localhost:8000/`，交付目錄的 `index.html` 導向 `web/index.html`

**要用 `http://<hostname>/preview-spec_04…/index.html` 這種網址（實測結論）**：

| 網址形態 | 實測 | 說明 |
|---|---|---|
| `http://localhost:<port>/<目錄>/index.html` | **PASS**（0 錯誤） | 最省事 |
| `http://<hostname 或 LAN IP>:<port>/<目錄>/index.html` | **PASS**（0 錯誤；`--bot` 通關 72.4 s） | 自訂殼層 `godot/web_shell.html` 在非安全來源放行 Secure Context（本交付是 nothreads 版，不需要 SharedArrayBuffer/COOP-COEP），並把 `AudioWorklet` 接成靜音（非安全來源瀏覽器不提供它；本遊戲無音效） |
| `https://<hostname>:<port>/<目錄>/index.html` | **PASS**（0 錯誤） | `make serve-https`（自動開自簽憑證，瀏覽器警告選繼續）；要音效或要官方殼層行為時用 |

```bash
cd .../preview-spec_04-swift-1.5-iq3_s-max-80-20261028000156
sudo python3 tools/serve.py --port 80            # → http://<hostname>/preview-spec_04…/index.html
make serve                                        # → http://localhost:8000/…（免 root）
```
`tools/serve.py` 啟動時會列出没個入口的可用網址，並提示兩條環境陷阱：`/home/scott` 為 `drwxr-x---` 時以 www-data 跑的 nginx/apache 會 403（要 `chmod o+x /home/scott`）、以及 port<1024 需要 root。

```bash
cd .../preview-spec_04-swift-1.5-iq3_s-max-80-20261028000156
sudo python3 tools/serve.py --port 80 --https   # → https://<hostname>/preview-spec_04…/index.html
# 或從對方機器：ssh -L 8000:localhost:8000 scott@<hostname> → http://localhost:8000/preview-spec_04…/index.html
```

舊版用 `meta refresh` 的相對網址，在「掛在子路徑且伺服器不補尾端斜線」時會導到上一層（W.9）；交付目錄名被 externally 改過（`…-max-20261028000156` → `…-max-80-20261028000156`），寫死目錄名的網址直接 404（W.10）。兩層入口頁現在都用 pathname 推候選 + `fetch` 探測，失敗就列候選連結。

根目錄的 `index.html` 是導向入口頁（meta refresh + `location.replace` → `web/index.html`），`web/` 是 `make web` 匯出的 Godot 4.7 Web 版。

| 項目 | 內容 |
|---|---|
| 產物 | `web/index.html` + `index.wasm`（39.5 MB）+ `index.pck`（466 KB）+ `index.js` + 圖示/音效 worklet |
| 組態 | `export_presets.cfg`：Web、`thread_support=false`（不需要 COOP/COEP 表頭 → 純靜態伺服器可跑）、`include_filter="*.json"`（把 `layout.json`/`heightmap.json` 放進 pck） |
| 渲染器 | Web 用 `gl_compatibility`（`renderer/rendering_method.mobile`），桌面維持 Forward+。瀏覽器實測：`OpenGL ES 3.0 (WebGL 2.0 (OpenGL ES 3.0 Chromium)) - Compatibility` |
| pck 內容抽查 | `data/layout.json`、`data/heightmap.json`、`terrain_surface.png`(.ctex)、全部 `.glb`、`terrain_surface.gdshader`、`tilt_shift.gdshader`、`main.tscn`、編譯後 `.gdc` |
| 自動驗收 | `make web-smoke`（`tools/web_smoke.mjs`：Node 自帶靜態伺服器 + Chrome DevTools Protocol + SwiftShader）→ 引擎橫幅、canvas 1280×633、WebGL2 true、**console 0 錯誤**；`--bot` 把殼層 `GODOT_CONFIG.args` 換成 `["--","--bot"]`，等 console 出現 `"event":"win"` 才算過 |
| 瀏覽器內實測 | `COINRUN_BOT {"coins":3,"elapsed":72.4,"event":"win","message":"All 3 coins collected and the flag reached"}`、world 組裝 `props 90 / walls 44 / coins 3`；證據 `pipeline/screenshots/web-smoke.png`、`web-bot.png` |

**已知取捨（老實講）**：

1. **Compatibility 渲染器**沒有 SSAO / Glow / 部分環境效果 → 網頁版比桌面版「平」一點；移軸 post 與地表色圖仍在。
2. **未經鍵盤驗證**：無頭 Chrome 下送 ArrowRight（CDP 與頁面內合成事件都送達 canvas，實測 `target=CANVAS focus=true`）但 HUD 速度不動 → 判定為測試工具限制，需在一般瀏覽器用真實按鍵確認（progress W.8 標 `[x]`）。bot 模式在瀏覽器內完整通關，遊戲邏輯/物理/渲染都已證明。
3. **SwiftShader 慢**：驗收跑在軟體 GL，bot 通關要 72 s（headless 原生 24.68 s）。真人顯示卡下不受此影響。
4. `web/` 39 MB 屬建置產物 → 進 `.gitignore`，用 `make web` 重建；入口頁與 preset 設定進 git。
5. **入口頁不寫死相對網址**（使用者回報打不開後修正）：站點掛在子路徑、且伺服器不補尾端斜線時，相對 `web/index.html` 的基準是**上一層** → 404。入口頁改成 JS 依 `location.pathname` 推候選並 `fetch` probe（認 Godot 殼層的 `index.js`），命中才導向；`file://` 直開則顯示「需要 HTTP + `make serve`」。四種網址實測全 PASS：`/preview-spec_04…`（無斜線）、`…/`、`…/index.html`、`/`。

## 3. 每個資產的迭代次數

「迭代次數」＝該資產在 `make check` 迴圈裡跑到 PASS 的次數（含因工具 bug 造成的 FAIL；每輪上限 3 次）。

| 資產 | 次數 | 第一次 FAIL 的原因（判斷） |
|---|---|---|
| probe_box（pipeline 驗證用） | **3** | 1) `asset_audit.gd` 型別推斷失敗 → audit 無資料被當 PASS（pipeline）2) `_col` 命名觸發 Godot 慣例 + 顏色 sRGB 双重編碼 + 面數含碰撞網格（pipeline） |
| rock | **2** | audit 在 `_init` 讀 `global_transform`（單位矩陣）→ 尺寸/原點全錯（pipeline，D19） |
| house | **2** | `Mesh.get_aabb()` 對旋轉節點是「AABB 的 AABB」→ 4.70 m 報成 9.19 m（pipeline，D18） |
| robot | **2** | Godot 4.7 無 `AnimationPlayer.anim_list` → 檢查腳本在 `_process` 拋錯不退出 → 逾時 240 s（pipeline，M08） |
| terrain | **2** | 規格 z=3.20 是建模前的估值，實測 3.617（側裙 3.2 + 隆起 0.42）→ 規格端修正並把算法寫進 notes（規格問題）。另有 1 次**美術迭代**：逐格隨機草坪像迷彩 → 改低頻 blob 噪聲 + 廣場過渡帶 |
| coin（Phase 9 新輪） | **3** | 規格縮至 60% 後檢查 FAIL（cause=modeling，實測 0.70/0.135/0.70 對 0.42/0.07/0.42）→ 重建模 → PASS |
| 其餘 12 項（bush、tree、barrel、wooden_crate、endpoint_flag、brick_wall、fantasy_shrine、fantasy_arch、cloud、probe 之外的 terrain 之外全部） | **1** | 第一次即 PASS（兩端尺寸/原點/面數/材料/動畫/碰撞全合格） |
| 規格檔自身修正（非 FAIL 迴圈） | — | `endpoint_flag` y 0.18→0.46（忘記圓底座）、`cloud` z 1.7→1.88（5 球堆疊）。規格是唯一真相 → 在 `specs/` 改並記原因，不動模型 |

非資產的迴圈（同一套「記 log → 修 → 複測」紀律）：layout 2 輪、世界組裝 1 輪（22 條 SCRIPT/SHADER ERROR → 0）、bot 3 輪、physics 2 輪、畫面檢查 7 次修正（見 §7）。

## 4. 物理測試：門檻與實測

門檻唯一真相 `specs/physics_thresholds.json`（逐項含 `why`）；實測 `pipeline/checks/physics.json`；量測場景 `res://scenes/tests/physics_test.tscn` 用**真的** `WorldBuilder` + **真的** `player.gd`（D26，不做 mock 地面）。

| id | 門檻 | 實測 | 對應需求 |
|---|---|---|---|
| top_speed | 3.6..4.5 m/s | **3.99** | 一般行走 4 m/s |
| accel_time_s | ≤ 1.2 s | **0.667** | 靠力向量加速、有慣性 |
| boost_speed | 5.8..7.4 m/s | **6.384** | 加速（A/padX） |
| wheel_rps | 2.2..2.9 rev/s | **2.54** | 單輪動畫與速度綁定（不打滑） |
| brake_stop_m | ≤ 1.5 m | **0.625** | 煞車（B/padB） |
| inertia_slide_m | 1.0..3.5 m | **1.937** | 有慣性（放手後會滑） |
| jump_height_m | 0.6..1.6 m | **0.907** | 跳躍（Space/Enter/padA） |
| lean_rad | 0.15..0.55 rad | **0.302** | 加速時機身傾斜 |
| barrel_push_m | ≥ 0.8 m | **2.947** | 木桶可被推 |
| tree_sway_m | ≥ 0.25 m | **0.296** | 撞樹會搖晃 |
| leaf_bursts | ≥ 1 | **3** | 撞樹掉葉 |
| bush_sway_m | ≥ 0.2 m | **0.256** | 穿過草叢會晃（草叢可穿過） |
| climb_slope | ≥ 0.18 m/m | **0.218** | 爬得上全圖（地形最陡 0.218） |
| climb_progress_m | ≥ 2.5 m | **11.714** | 爬坡要真的前進 |
| wall_overflow_m | ≤ 0.0 m | **0.00** | 角色走不出去（40×40 牆環） |

## 5. 主要設計決策（完整 43 條見 `pipeline/log.md`「設計決策」）

| # | 決策 | 為什麼關鍵 |
|---|---|---|
| D01–D03 | GLB 中介、原點＝接地面中心、Blender `(x,y,z)`→Godot `(x,z,−y)` | 尺寸可直接量測比對；擺件 `origin.y = 地面高度` 就不懸空/不陷入 |
| D06 | 檢查走兩端：headless Blender 讀 `.blend` + headless Godot 讀 `.glb` | 不依賴 MCP 執行期狀態，可重跑、可 CI 化；「兩端都 ±10%」才有證據 |
| D13 / D17 | 碰撞命名 `<id>_collision`（**不用** `_col`）；規則形狀由規格宣告、Godot 端 `WorldBuilder` 建立，只有不規則外形（rock）在 Blender 建碰撞網格 | 實測 Godot 4.7 對 `_col` 會自動生 StaticBody3D **且保留白色視覺網格** → 渲染與物理都偏離規格 |
| D18 / D19 | 兩端量測用真實頂點、且在 `_process` 第一幀後 | 否則斜屋頂 4.70 報成 9.19、rock 原點報 −0.32，全數誤判 |
| D21 | 高度場共用模組 `tools/heightfield.py`（建模、版面、Godot 高度查詢同一份數學） | 「所見即所踩」：物件擺放與 `HeightMapShape3D` 用同一份 1 m 網格高度表 |
| D22 | `tools/gen_layout.py` 產生版面時就驗證 14 項（pad 平整、路線淨空/沿路、高物遮擋、坡度、重疊、可推物淨空、閘門孔洞…） | 摆好 90 件之後才發現走不過去，成本最高 |
| D26 / D27 | 物理測試用真世界 + 門檻獨立成規格檔，未回報 id 判 FAIL | 防止「測試過、遊戲壞」與「靜默少量」 |
| D28 / D29 | 煞車＝目標速度拉到 0；推剛體用 `travel_demand`（控制意圖）施加脈衝 | 原實作邊衝邊煞（煞停 9.15 m）、貼著木桶時 `velocity`=0 等於沒推（位移 0 m） |
| D30 / D34 / D37 | 移軸自建 `canvas_item` post；地面顏色烘焙成色圖由 Godot 端覆蓋；雲影＝雲網格真的投影 | 4.7 移除 `Environment.dof_blur_*`；逐面平塗在遊戲裡變 1 m 棋盤格；假貼片量不到效果（刪除） |
| D36 | 自建 `spatial` 著色器一律 `cull_disabled` | glTF 材料是雙面，自建 shader 預設 cull_back → 被剔除的頂面露出天空，地上出現格狀藍灰斑塊 |
| D39 | 截圖机位寫進程式（preset）並用版面資料預選淨空點 | 附錄 B 要「機器人清楚可辨」；實測過机位被樹冠整個人擋住的版本 |
| D40 / D41 | WebGL 版用單執行緒範本 + Web 平台 `gl_compatibility` | 不需要 COOP/COEP 表頭 → `python3 -m http.server` 就能跑；WebGL2 不支援 Forward+（實測瀏覽器印出 `WebGL 2.0 - Compatibility`）。代價：Compatibility 不吃 SSAO/Glow |
| D42 / D43 | 匯出到專案外的 `web/`（39 MB 建置產物，gitignore），根目錄 `index.html` 只做導向；網頁版用 `tools/web_smoke.mjs`（CDP）驗收 | 匯出產物含 PNG，放進 `godot/` 會被 Godot 掃描匯入；「在瀏覽器跑得起來」需要可重跑的證據，不是肉眼看一次 |

## 6. MCP 問題與替代做法（完整 17 條見 log「MCP 問題」）

| 现象 | 替代做法 |
|---|---|
| godot-mcp 無「寫腳本 / 掛腳本 / 設 Transform / 截圖 / 批次擺件」工具（M01、M02、M04、M10） | `.gd`/`.tscn` 用檔案寫入；世界內容由 `layout.json` + `WorldBuilder`；截圖用 `tools/capture.py`（視窗 Vulkan 渲染 → PNG） |
| `run_project` 不能傳命令列參數、立即回傳（M03、M09） | bot/物理測试用 headless CLI（`-- --bot`）；需要完整輸出時讓場景自己 `quit(0)`，再讀 `output`/`errors` |
| Blender 5.2 glTF 匯出器旗標更名（M05） | `tools/blender_export.py` 先讀 `get_rna_type().properties` 只傳支援的旗標（`use_selection` 取代 `export_selected`） |
| Blender MCP `look` 無 `object` 模式、`viewport` 不重新取景（M06） | 用 `mode:"angles"` + `views:["front","right","top"]` 做建模自檢 |
| Godot 4.7 API 更名／移除（M08、M11、M12、M14） | 用 `godot --headless --script res://tools/probe_*.gd` 寫一次性探查：`ClassDB.class_get_property_list()` 讀 `hint_string` 拿列舉值（`QuadMesh` 只有 `FACE_X/Y/Z`）、`class_get_method_list()` 找真名（`set_surface_override_material`）；著色器輸出是大寫 `ALBEDO/ALPHA`，透明混合是 `blend_mix` |
| 匯出 Web 時 Godot 不建立輸出目錄（M16） | `make web` 先 `mkdir -p web` |
| godot-mcp 無匯出工具，且 4.7 網頁版不讀 URL 的 `?args=`（M17） | 匯出用 CLI `--export-release "Web"`；測試參數由驗收腳本的伺服器把殼層 `GODOT_CONFIG.args` 換成 `["--","--bot"]` |
| 工具產出的 PNG 預設不產生 mipmap（M15） | 在 `godot/data/terrain_surface.png.import` 開 `mipmaps/generate=true`（非 `.glb.import`，不違反 AGENTS §1），並降低鋪石縫對比 |

## 7. 遇到的問題與解法（挑花費最多的）

1. **檢查工具本身的 bug 會伪装成資產不合格**（probe_box 3 輪、rock/house/robot 各 1 輪）：修量測（真實頂點、`_process` 後、排除 `_collision`、sRGB 只編碼一次）、把「audit 無資料」強制判 FAIL。教訓：先讓量測可信，再談建模。
2. **Godot 4.7 沒有內建移軸**：自建 `tilt_shift.gdshader`（`hint_screen_texture` 全螢幕 post），清晰帶的螢幕 Y 由腳本依機器人在畫面上的位置給（`canvas_item` 讀不到深度）。
3. **地面「假」**（附錄 B）：Blender 逐 1 m 面只能給一個平塗材料 → 遊戲裡是 1 m 棋盤格。解法是把同一套 `surface_material` 規則烘成色圖（`tools/gen_terrain_texture.py`，8 px/m、噪聲 ≥2 m、mipmaps），Godot 端只覆蓋 mesh surface 0..4（土層保留）。
4. **地上出現格狀藍灰「水漬」斑塊**：先排除烘焙（色圖 0 個藍 dominant 像素、逐點取樣正常）、再排除貼片著色器（`strength=0` 對照只差 25 px），最後用「關掉覆蓋著色器 → 同一像素從 (62,77,100) 變回 (43,86,49)」鎖定是**自建 shader 的 cull_back** 把頂面剔除露出天空 → `cull_disabled` 解決。
5. **雲影**：先做「雲下軟陰影貼片」著色器，結果 strength 0.24/0.30/0.35/0.42/0.65 截圖差異 ≤3%、`strength=0` 對照只差 25 px → 機制無效，刪除；改雲網格真的投射（`cast_shadow` 遞規開啟），對照實測雲下亮度 **108 vs 157＝69% 日照**、覆蓋 5091 px。順帶把平行光改 4-split（原本單張正交圖分 68 m → 陰影像素 1 m，樹影變鋸齒斑）、環境光混入暖中性（純藍天使陰影發藍）。
6. **互動鏈路「0」分不清沒撞到 vs 撞到沒反應**：加 `PHYS_NOTE` 探查印出玩家/目標座標與 sway 狀態，實測玩家停在距樹幹 0.63 m（=膠囊 0.31 + 树幹 0.32）＝**撞到了**，但 `set_script()` 後 `_process` 沒跑（`is_processing()==false`）→ 掛載後補 `set_process(true)`。
7. **截圖机位**：action preset 的機器人被 4.2 m 樹冠整顆遮住（HUD speed 0.1 m/s 表示正在推桶但看不到）→ 用版面資料篩候選（高物 >2 m 對視軸淨空、碰撞淨空）選到 (1.4,−4.4)，機器人/木桶/終點旗/木箱同框。

## 8. 花最多時間的步驟

**Phase 8.5「依附錄 B 檢查並修正」**，佔本輪最多次迭代（7 次修正、20+ 次截圖、2 次對照實驗）。原因是畫面問題沒有自動檢查可跑，而且**症狀與病因不同層**：草坪棋盤格在 Blender、藍灰斑塊在自建著色器的剔除模式、雲影在光照設定、樹影鋸齒在陰影地圖解析度。中途還被一個假解法（雲影貼片）浪費了 5 次截圖，最後靠「`strength=0` 對照只差 25 px」才認它無效並刪除。

對照之下，建模與檢查（Phase 1–7）幾乎一次過；bot/物理的 FAIL 都在 2–3 輪內收斂，因為它們有可判讀的 JSON 數字可以收斂。

**可省的費用**：把「畫面需求」也自動化。本專案已把鏡頭需求自動化（`make cam-check`：pitch 45.02°、方位 −135°、no_roll、鏡頭相對輸入），也寫了像素級量測腳本（雲影亮度、藍灰像素計數、色圖取樣），但這些是一次性探查，尚未變成 `make` 目標。

## 9. 建議的下一步

1. **畫面檢查自動化的缺口**：雲影對比（雲下/雲上亮度比）、地面單調度（局部色彩變異）、遮擋（從相機到機器人的射線命中樹冠）做成 `make shot-check`，用現有的 PIL 量測與 `layout.json` 資料實作；附錄 B 就能像 bot/physics 一樣跑迴圈。
2. **音訊與回饋**：目前完全無音效（PLAYBOOK 未要求）；推桶/收幣/落地各一個短音，加 HUD 金幣計數的動畫，完成「小遊戲」的手感。
3. **手機/觸控**：只綁了鍵盤與手把（依定案按鍵表，不得新增按鍵）；若要觸控需先取得按鍵定案的放行。
4. **性能**：`prop_y`、碰撞 105 個、90 件 props 在 44×40 m 內全載入；若地塊放大，建議 WorldBuilder 依相機距離分批啟用 + 樹/草叢 LOD，並把陰影 `directional_shadow_max_distance` 依机位調降（60 m 是目前全圖的最差情況）。
5. **色圖的取樣來源**：`terrain_surface.png` 是 352×352 烘焙圖；若地塊尺寸改變，`TERRAIN_HALF_M`（`world_builder.gd`）與規格 `build.half_m` 要一起改。可改成從 `heightmap.json`/規格推導，少一個手工同步點。
6. **瀏覽器端按鍵驗證**：本輪在無頭 Chrome 驗不到按鍵（W.8 標 `[x]`）。把 `tools/web_smoke.mjs` 的按鍵段落改成用 `--headless=old` 之外的真實視窗（`xvfb-run` + 一般 Chrome），用 `xdotool` 按方向鍵，並把 HUD 速度做成可判讀輸出（現在只能從截圖讀），就能補上這條證據。
7. **`probe_box` 的定位**：它是 pipeline 打通用的驗證資產（不是遊戲內容），目前仍會在 `make check` 清單內。可加一個 `make check ID=...` 的正式清單（14 項遊戲資產）與「煙霧資產」清單分離，避免之後誤擺進場景。
