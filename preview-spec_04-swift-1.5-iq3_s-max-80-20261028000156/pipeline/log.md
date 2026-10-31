# Log — Coin Run（PLAYBOOK v2.1）

## MCP 設定

啟動 agent 用的指令（agent-switch + opencode，照 `.spec/PLAYBOOK_BLENDER_GODOT_MCP_SETUP.md`）：

```bash
agent-switch opencode \
  --mcp-stdio "godot:npx -y @coding-solo/godot-mcp" \
  --mcp-env   "godot:GODOT_PATH=/home/scott/tools/bin/godot" \
  --mcp-stdio "blender:uvx blender-mcp" \
  --mcp-env   "blender:DISABLE_TELEMETRY=true"
```

權限政策：Godot MCP 工具與本專案檢查工具 → 自動允許；Blender `execute_blender_code` → **不自動允許**（每次詢問）。

實測（2026-10-28）：Blender 5.2.2 LTS、addon 1.8、protocol 15、`premium_generators: []`、polyhaven/sketchfab/polypizza 皆 off。

## 設計決策

| # | 決策 | 理由 |
|---|---|---|
| D01 | 中介格式用 GLB（glTF 2.0 二進位），一資產一檔，路徑 `godot/assets/<id>/<id>.glb` | Godot 4 原生匯入器對 glTF 的軸向/單位換算最穩定，尺寸可直接量測比對 |
| D02 | 所有資產原點＝接地面中心，模型局部 `Z >= 0` | 擺件時 `origin.y = 地面高度` 就不懸空/不陷入，檢查也最容易量化 |
| D03 | 軸向對映 Blender `(x,y,z)` → Godot `(x,z,-y)`；角色正面 Blender +Y → Godot -Z | glTF 預設 `+Y up` 的標準結果，寫成規則避免每次推 |
| D04 | 碰撞用 Blender 裡 `<id>_collision` 低面數網格 + 自訂屬性 `crcollider=1`，Godot 端由 WorldBuilder 轉成物理體 | 碰撞可控、可檢查；比 Godot 自動產生 concave 準確且便宜 |
| D05 | 機器人輪子動畫在 Blender 做（使用者指定）；樹/草叢的風擺動用 Godot 頂點著色器 | 輪子要「看得出在轉」且由速度驅動 → 骨骼/物件動畫；風擺動要全域、低開銷、強度可調 → shader |
| D06 | 規格檢查走兩端：headless Blender 讀 `.blend` 量測 + headless Godot 載入 `.glb` 量測 | 不依賴 MCP 執行期場景狀態（可重跑、可 CI 化），且滿足「兩端都符合 ±10%」 |
| D07 | 尺寸容差 `max(10% × 目標, 2 cm)` | PLAYBOOK 要求；2 cm 下限避免小資產因四捨五入誤 FAIL |
| D08 | 版面由 `godot/data/layout.json` + 執行期 `WorldBuilder` 產生 | Phase 6 要求版面可重現、調整不需逐一手動搬件 |
| D09 | 地面是正規資產 `terrain`，並輸出高度查詢資料（網格化高度表 JSON）給 Godot 擺件 | 起伏地面上要準確放置上百個物件 |
| D10 | bot / 物理測試輸出 JSON 到 `pipeline/checks/`，由 Python 判定 PASS/FAIL | 結果可判讀、可附進 REPORT |
| D11 | 測試用 headless Godot CLI，遊戲執行與錯誤觀察用 Godot MCP `run_project` + `get_debug_output` | headless 能拿到結構化輸出且不需顯示器；MCP 負責「實際執行遊戲」的驗收 |
| D12 | 顏色由規格檔的 palette 統一（箱庭低模平塗） | 風格一致、檢查可比對材料名稱 |
| D13 | 碰撞節點命名 `<id>_collision`，**不用** `<id>_col` | 實測：Godot 4.7 glTF 匯入器把 `_col` 名稱自動轉成 StaticBody3D + CollisionShape3D，**同時保留一份白色視覺網格** → 渲染與物理都偏離規格。`_collision` 不觸發該慣例，物理完全由我們的 WorldBuilder 依 `collision.mode` 產生並隱藏碰撞網格 |
| D14 | 匯出一律走 `tools/blender_export.py`（Blender MCP 內用 `runpy` 呼叫） | 匯出旗標集中一处，不會每顆資產各打一套而漂移；並自動跳過此 Blender 版本不支援的旗標 |
| D15 | 顏色檢查：Blender 端由 linear base color 編碼回 sRGB hex；Godot 端直接讀 `albedo_color` 的 raw 通道 | 實測 Godot 匯入後 `albedo_color` 已是 sRGB 數值，若再編碼會得到 `#C2A07C`（應為 `#8A5A33`）而誤判 |
| D16 | 面數與尺寸量測在兩端都排除碰撞網格 | 規格的面數/尺寸語意是「看得到的部分」；含碰撞網格會把 12 面盒子算成 24 面 |
| D17 | 碰撞分成兩類：規則形狀（box/cylinder/capsule/box_pair）在規格宣告參數、由 Godot `WorldBuilder` 建立；只有不規則外形（`shape: "mesh"`，目前是 rock）在 Blender 建 `<id>_collision` | 13 顆資產逐顆建模碰撞網格成本高、易錯、且對盒子沒益處；同時保留 mesh 碰撞路徑給不規則形狀（Phase 1.6 已驗證 extras 送達） |
| D18 | 兩端量測都用**真實頂點**遍歷，不用 `bound_box` / `Mesh.get_aabb()` | 旋轉過的節點會得到「AABB 的 AABB」：house 斜屋頂實測 4.70 m 被報成 9.19 m、shrine 5.0 被報成 8.63，全部誤判 FAIL |
| D19 | Godot 端量測在**第一幀之後**（`_process`）執行 | `_init` 裡 `global_transform` 還未更新（全為單位矩陣），bbox 塌回網格局部座標：rock 的 x 被報 0.75（實測 0.96）、origin 被報 -0.32（實測 0） |
| D20 | 冒煙測試場景用 `smoke_probe.gd` 在執行期載入全部 `.glb` 並在 30 幀後自走 `quit(0)` | Godot MCP `run_project` 是非同步的；場景會自己結束，`get_debug_output` 才收得到完整輸出與 errors |
| D21 | 地面與版面共用一個高度場模組 `tools/heightfield.py`（高度函數 + pad 壓平 + 道路遮罩 + 1 m 網格取樣），Blender 建模、版面產生器、Godot 端高度查詢都讀同一份數學 | 三處若各寫一套起伏公式，物件就會和地面對不起來；共用模組保證「所見即所踩」 |
| D22 | 版面由 `tools/gen_layout.py` 產生並在產生時就驗證 14 項（pad 平整、bot 路線淨空/沿路、高物遮擋、坡度、重疊、可推物體空間、閘門孔洞…） | 「版面可重現 + bot 路線不被擋」是附錄 D 的要求；先驗證再建模，避免摆好 100 件之後才發現走不過去 |
| D23 | 物件間距用「躯體半徑」（`PLACEMENT_SPACE`：樹 1.1 m）而不是視覺 bbox 半徑（樹 1.66 m） | 樹冠可以重疊（比較像林子），躯體/碰撞不可重疊；用 bbox 會把树挤到很遠、地圖變空 |
| D24 | 高物遮擋用碰撞輪廓半徑（`occ`）+ 視軸投影檢查，不算視覺 bbox | 等角 45° 鏡頭下擋住視線的是躯體輪廓；樹的視覺 bbox 3.3 m 會把整條路都判成被擋 |
| D25 | 地塊側裙的深度固定 3.2 m、地塊邊緣高度由牆環 pad 壓平在 y=0 | 牆要落在平地上；邊緣齊平也让「桌上模型」的輪廓乾淨，側裙深度足够画出三層土層 |
| D26 | 物理測試場景 `res://scenes/tests/physics_test.tscn` 用真的 `WorldBuilder` + 真的 `player.gd`，不做 mock 地面 | 量到的坡度/牆/木桶/草叢就是玩家實際會遇到的；測試與遊戲共用同一份 layout.json，測試不會「测試過遊戲壞」 |
| D27 | 物理門檻獨立成 `specs/physics_thresholds.json`（id / op / value / value_max / why），未回報的 id 一併判 FAIL | 門檻是規格的一部分，要能被審閱；漏測一項就 FAIL，防止測試腳本靜默少量 |
| D28 | 煞車語意＝「目標速度拉到 0」（`target = Vector3.ZERO`），不是「用更大的加速度」 | 原本邊衝邊煞會朝目標速度加速（實測煞停距離 9.15 m）；修正後 0.625 m，符合「有慣性但煞得下來」 |
| D29 | 推可動剛體用 `travel_demand`（控制意圖 × 目標速度）對 RigidBody3D 施加 `質量 × Δ速度 × GAIN` 的脈衝 | 貼著木桶時 `velocity` 已是 0，用它當推力等於沒推（實測位移 0 m）；用「想要去哪」推，位移 2.95 m，且摩擦/阻尼仍會把它停下來 |
| D30 | 移軸（tilt-shift）用自建 `canvas_item` 全螢幕 post（`hint_screen_texture`），清晰帶的螢幕 Y 由 GDScript 依机器人在畫面上的位置提供 | Godot 4.7 移除了 `Environment.dof_blur_*`，且 `canvas_item` 無法讀深度（實測 `hint_depth_texture is not supported in canvas_item shaders`）；只用螢幕帶反而更像微縮模型 |
| D31 | bot _waypoint 用「到達半徑 3 m 就推進下一個點」，而不是「下一個點比當前近就推進」 | 道路寬 3.8 m、路線點間距 ~8 m；「最近點」規則在越過點之後會反向振盪（實測在 (-14,-11) 來回 8 m 走不到金幣） |
| D32 | 風摆與碰撞反饋的振幅記在 GDScript meta（`shake_max` / `leaf_bursts`），shader 只讀參數 | headless 不執行 shader 頂點運算；振幅放在腳本層，物理測試才量得到「撞樹有搖、穿草叢有晃」 |
| D33 | 鏡頭需求（等角 45°、不翻滾、鏡頭相對輸入）做成 `make cam-check` 輸出 `CAM_REPORT` | 「45° 等角」不該只靠肉眼看截圖；pitch/方位/翻滾/輸入參照系變成可自動檢查的證據 |
| D34 | 地面顏色在 Godot 端用**烘焙色圖 + 覆蓋著色器**（`tools/gen_terrain_texture.py` → `data/terrain_surface.png` → `shaders/terrain_surface.gdshader` 覆蓋 terrain 網格 surface 0..4），不在 Blender 加貼圖 | Blender 端每 1 m 格只能給一個平塗材料 → 遊戲裡草坪變成 1 m 棋盤格（附錄 B「草坪不單調、不假」不合格）。色圖走同一規則（`heightfield.surface_material`）線性插值，鋪石/道路位置仍由版面決定；土層 5..7 保留原材料 |
| D35 | 色圖解析度 8 px/m、粒狀噪聲特徵尺寸 ≥ 2 m、鋪石縫只在 road/plaza 格、低對比（11%） | 4 px/m + 1 m 噪聲在遊戲鏡頭下產生莫爾；高對比細縫在小圖上變成「水漬」。先量化（PIL 取樣）再調參 |
| D36 | 覆蓋用的 `spatial` 著色器一律 `render_mode cull_disabled` | glTF 材料是雙面（Godot 匯入成 `cull_disabled`），自建 shader 預設 `cull_back`；地塊頂面不是單一流向 → 被剔除的面直接露出天空，在畫面上變成**格狀藍灰色斑塊**（實測該像素 (62,77,100)，關掉覆蓋後同一點變 (43,86,49) 正常草綠） |
| D37 | 雲影用**雲網格真的投影**（`cast_shadow` 遞規打開），不用假陰影貼片 | 對照實測：雲投射開/關差 5091 px（0.55% 畫幅），雲下亮度 108 vs 157＝**69% 的日照**，柔邊由 `light_angular_distance 12°` 給。先前寫的 `cloud_shadow.gdshader` 貼片在 strength 0/0.24/0.35/0.42/0.65 全部量不到差異（≤3%）→ 屬無效複雜度，已刪除 |
| D38 | 平行光改 `SHADOW_PARALLEL_4_SPLITS`（max 60 m、fade 0.85）＋環境光混入暖中性色（`ambient_light_color (0.62,0.60,0.55)`、sky_contribution 0.45） | 單張正交圖分 68 m → 陰影像素 ~1 m，樹影在草坪上變成 1 m 藍灰鋸齒斑；純藍天環境光讓陰影發藍（附錄 B 地面項目）。改後同區域藍灰像素由 9596 降到 7797 |
| D39 | 截圖五個 preset 的機位寫進 `main.gd`（`SHOT_DIR`/`SHOT_FRAMES`/`_apply_shot_preset`），action 机位選在**相機→機器人連線無高物**的空地上（(1.4,−4.4) 面向 +Z 推木桶5） | 附錄 B 要「機器人清楚可辨」與「推可推物」；實測 (10.6,13.4) 機位的視野被 4.2 m 樹冠吃掉機器人（HUD speed 0.1 表示正在推但看不到人）。用版面資料預算淨空（高物 >2 m 對視軸淨空 0.65 m、碰撞淨空 1.42 m） |
| D40 | WebGL 版用 `thread_support=false`（`web_nothreads_release` 範本） | 單執行緒版不需要 `Cross-Origin-Opener-Policy/Embedder-Policy` 表頭 → `python3 -m http.server` 就能跑，一般靜態代管也能跑；線程版要額外服務端表頭 + Service Worker |
| D41 | Web 平台渲染器固定 `gl_compatibility`（`project.godot` 的 `renderer/rendering_method.mobile`），桌面維持 Forward+ | WebGL2 不支援 Forward+。實測瀏覽器印出 `OpenGL ES 3.0 (WebGL 2.0 (OpenGL ES 3.0 Chromium)) - Compatibility`。代價：Compatibility 不吃 SSAO / Glow / 部分環境效果 → 網頁版畫面比桌面版平（附錄 B 的柔和度在瀏覽器打折），移軸 post 與地表色圖仍在（`hint_screen_texture` 於 4.x Compatibility 可用） |
| D42 | 匯出目標放在**Godot 專案外**的 `web/`（根目錄），根目录 `index.html` 只做導向 | 匯出產物含 PNG（splash/icon），放進 `godot/` 會被 Godot 掃描匯入、汙染專案；`web/` 39 MB 屬建置產物 → 進 `.gitignore`，`make web` 可重建。入口頁放最外層，`make serve` 開根目錄就能玩 |
| D43 | 網頁版用 `tools/web_smoke.mjs`（Node + Chrome DevTools Protocol + SwiftShader）驗收，不靠人工開瀏覽器 | 需要證據顯示「在瀏覽器裡真的跑得起來」。自帶靜態伺服器（`.wasm` 要 `application/wasm`）、收 console/exception、`--bot` 時把匯出殼層 `GODOT_CONFIG.args` 換成 `["--","--bot"]` 讓 `OS.get_cmdline_user_args()` 收到 bot 參數，並等 console 出現 `"event":"win"` 才算過 |
| D44 | 入口頁導向用 JS 依 `location.pathname` 推**候選網址並 probe**，不用 `meta refresh`、也不用寫死的相對路徑 | 站點掛在子路徑時，「目前路徑是目錄還是檔案」無法從網址字面判斷（伺服器不補尾端斜線 → 基準路徑差一層）。做法：`[目前+/web, 父層/web]` 兩種候選 `fetch` 探測（認 Godot 殼層裡的 `index.js` 字樣），命中才 `location.replace`；全部miss 就列出候選讓人手動點。`meta refresh` 與 `location.replace` 都在 JS 之前/之中觸發，會先把錯誤網址跑掉 |
| D45 | 入口頁放兩層：**仓库根**一份（導向 `<交付目錄>/web/index.html`）＋**交付目錄**一份（導向 `web/index.html`） | 使用者的 http server 根是仓库根（`preview-spec_04…/..`），網址不該要背交付目錄名；交付目錄名這輪被 externally 改過（加 `-80-`）→ 寫死子目錄名的網址會直接 404。兩頁都用「 pathname 推候選 + `fetch` 探測（認 Godot 殼層的 `index.js`）」，失敗就列候選連結 |
| D46 | 交付附上 `tools/serve.py`（預設 web 根＝仓库根、`.wasm` MIME、`--https` 自簽憑證、啟動時印出可用網址與環境警告） | 目標是 `http://<hostname>/<目錄>/index.html`。這形態會撞兩條環境規則：(1) 瀏覽器的 secure context — 純 HTTP 只有 localhost 是安全來源，Godot 對非安全來源直接拒絕啟動；(2) 以 www-data 跑的 server 進不了 `drwxr-x---` 的 `/home/scott` → 403。把偵測與解法寫在啟動訊息裡，比叫使用者試半天快 |
| D47 | 交付自訂 Web 殼層 `godot/web_shell.html`（非安全來源放行 Secure Context + AudioWorklet 靜音 stub），`export_presets.cfg` 設 `html/custom_html_shell` | 使用者要的是**純 HTTP** 的 `http://<hostname>/<目錄>/index.html`。官方殼層在非安全來源直接不啟動；放行後 Godot 音訊初始化又在 `ctx.audioWorklet.addModule()` 崩（`AudioWorklet` 是 `[SecureContext]`）。本交付是 nothreads 版（不需要 SharedArrayBuffer/COOP-COEP）且**無音效**，所以在殼層放行＋接靜音是最小改動，不動遊戲程式、不手改 `web/` 產物 |

## MCP 問題

| # | 現象 | 影響 | 替代做法 |
|---|---|---|---|
| M01 | godot-mcp（@coding-solo/godot-mcp）無「寫/改腳本檔」工具 | 無法用 MCP 建立 GDScript | 用檔案寫入建立 `scripts/*.gd`；建立/節點用 `create_scene`/`add_node`/`save_scene`，其餘寫 `.tscn` 文字檔 |
| M02 | godot-mcp 無截圖工具 | Phase 8 截圖 | 專案內寫 capture 腳本，用 headless/真實渲染視窗存 PNG（`make shot`） |
| M03 | `run_project` 只能指定場景，無法傳 player/bot 命令列參數 | 無法用 MCP 跑 bot 模式 | bot/物理測試用 `godot --headless --path godot --scene ... -- --bot`；MCP 只負責一般執行與 debug 输出 |
| M04 | `add_node` 有 `properties` 參數，但**只有純量屬性會套用**（實測 `light_energy`、`shadow_enabled` 成功；`rotation_degrees`、`translation` 無效），且 `create_scene` 把根節點命名為 `root` | 燈光的朝向、相機位置等需要 Transform 的設定無法經 MCP 完成 | 標量屬性用 MCP 設；Transform 與實例化由 `.tscn` 文字檔寫入，世界內容由 `data/layout.json` + `WorldBuilder` 產生 |
| M05 | Blender 5.2 的 glTF 匯出器不接受 `export_selected` / `export_empties`（`TypeError: keyword unrecognized`） | 直接寫死旗標會整次匯出失敗 | `tools/blender_export.py` 先用 `get_rna_type().properties` 讀出支援的旗標再傳；選取範圍用 `use_selection` |
| M06 | Blender MCP `look` 的 `mode` 沒有 `object`（只有 viewport/camera/angles/frames）；`mode:"viewport"` 的 `distance` 不會重新取景，自檢看到的永遠是整片網格 | 無法取景自檢外觀 | 用 `mode:"angles"` + `views:["front","right","top"]`（會框住 target），`views` 不接受 `side`（用 `left`/`right`） |
| M07 | Godot glTF 匯入器對節點名 `_col` 的內建慣例（自動 StaticBody3D + 保留白色視覺網格） | 違反規格的原點/材料/面數假設 | 改用 `_collision` 命名 + WorldBuilder 自建物理（見 D13） |
| M08 | Godot 4.7 `AnimationPlayer` 無 `anim_list` 屬性（改 `get_animation_list()`），audit 用到它會在 `_process` 拋錯、腳本不退出 → 檢查工具逾時 240 s | robot 檢查卡死 | 改用 `get_animation_list()`；量測失敗要有明確輸出而非無限跑 |
| M09 | `run_project` 立即回傳（背景執行），`get_debug_output` 回傳的是物件（直接印字串會得到 `[object Object]`） | 驗收時看不到輸出 | 讀 `output` / `errors` 兩個欄位；需要完整輸出時讓場景自己 `quit(0)`（D20） |
| M10 | godot-mcp 無「寫腳本 / 建立 GDScript / 掛腳本 / 截圖 / 批次擺件」工具 | 遊戲程式與截圖無法經 MCP 完成 | 檔案寫入 `.gd` / `.tscn`；截圖用專案內 capture 腳本（`make shot`）；組裝用 layout.json + WorldBuilder |
| M11 | Godot 4.7 移除 `Environment.dof_blur_*`（實測 `Invalid assignment of property or key 'dof_blur_far_enabled'`），且 `canvas_item` 著色器讀不到深度（`'hint_depth_texture' is not supported in canvas_item shaders`）； sampler 提示只認 `hint_screen_texture`（`source_screen_texture` 報 `Expected valid type hint after ':'`） | 需求裡的「移軸景深」沒有內建做法 | 自建 `shaders/tilt_shift.gdshader` + `scripts/tilt_shift.gd` 全螢幕 post，清晰帶位置由腳本給（見 D30） |
| M12 | Godot 4.7 API 更名：`CPUParticles3D.scale_min/max` → `scale_amount_min/max`（Vector3 也不行）、`ArrayMesh.get_surface_arrays()` → `surface_get_arrays()` | 落葉粒子與 rock 碰撞網格建立報錯 | 逐項改用 4.7 名稱；用 `ClassDB.class_get_property_list()` 查真實屬性名，不靠記憶 |
| M13 | 對「已在樹上的節點」呼叫 `set_script()` 不會啟動 `_process`（實測 `is_processing() == false`：撞樹已觸發 `hit()`（gust=0.3、bursts=3）但衰減/記錄都不跑） | 執行期掛載的風摆腳本等於沒動，物理測試量到 0 | WorldBuilder 掛載後補 `set_process(true)`，sway.gd 也加 `_ready()` 自查（見 D32） |
| M14 | Godot 4.7 著色器/網格 API 名稱與網路範例不一致，且 godot-mcp **沒有任何著色器或節點探查工具**（`create_scene`/`add_node` 不碰 `.gdshader`，也無法列舉列舉值）。實測陷阱：`QuadMesh` 的 orientation 只有 `FACE_X/FACE_Y/FACE_Z`（寫 `FACE_XZ` → 解析錯誤，水平面用 `FACE_Y`）；spatial 輸出是大寫 `ALBEDO/ROUGHNESS/SPECULAR/ALPHA`（`Albedo` → `Unknown identifier`、`COLOR` → `Constants cannot be modified`）；render mode 不認 `blend_alpha`、`shadows_unreceived`（透明混合用 `blend_mix`）；`set_surface_material_override` → 4.7 叫 `set_surface_override_material`；`ClassDB.class_get_enum_values()` 不存在 | 每次踩到都是一次完整匯入＋重跑（~1 分鐘），且 headless 的 dummy renderer 也會報 SHADER ERROR 混雜其他錯誤 | 用 `godot --headless --path godot --script res://tools/probe_*.gd` 寫一次性探查腳本：`ClassDB.class_get_property_list("QuadMesh")` 讀 `hint_string` 取得列舉值（`Face X,Face Y,Face Z`）、`class_get_method_list("MeshInstance3D")` 過濾 `material` 取得真名；用完即刪，不留在專案 |
| M15 | 工具產出的 PNG 預設匯入 `mipmaps/generate=false`（`godot/data/terrain_surface.png.import` 實測），地面色圖在遊戲鏡頭下小圖採樣 → 鋪石縫與粒狀噪聲變成閃爍鋸齒（看起來像水漬） | 地面細節在遠距離不可讀，附錄 B「道路清楚」打折 | 在該檔（不是 `.glb.import`，AGENTS §1 只禁 `.glb.import`）設 `mipmaps/generate=true` 後 `make import`；並把縫的對比降到 11%、噪聲特徵 ≥2 m（D35）。實測同區域藍灰像素 9596 → 7797 |
| M16 | 匯出 Web 失敗：`目標資料夾不存在或無法存取：「.../web」`（Godot 不會自行建立輸出目錄） | 每次 `make web` 都會踩到 | Makefile 的 `web` 目標先 `mkdir -p web` |
| M17 | 本版 godot-mcp **沒有匯出工具**（無 `export` / 無平台部署工具），也無法把命令列參數傳給網頁版（`?args=` 在 4.7 無效：`EngineConfig` 只讀 `index.html` 裡內嵌的 `GODOT_CONFIG.args`，不讀 URL 參數） | 期望「用 MCP 部署 + 用 URL 帶測試參數」做不到 | 匯出改用 CLI：`godot --headless --path godot --export-release "Web" <abs>/web/index.html`；測試參數由 `web_smoke.mjs` 的伺服器在回應 `index.html` 時把 `"args":[]` 換成 `["--","--bot"]` |

## 檢查與修正

| 時間 | 資產 | 第幾次 | 檢查結果 | 判斷 | 對 Blender 的修正請求 | 結果 |
|---|---|---|---|---|---|---|
| 2026-10-28 | probe_box | 1 | FAIL：`asset_audit.gd` 解析錯誤（`surface_get_array_index_len` 回傳未型別化，`:=` 推斷失敗）→ audit 無資料；檢查工具把「無資料」當 PASS | pipeline（工具 bug） | 無（模型端全項合格） | 修 audit 型別 + 強制「audit 無資料＝FAIL」→ 可量測 |
| 2026-10-28 | probe_box | 2 | FAIL：`collision_mesh`(godot) 找不到碰撞節點；顏色量測 `#C2A07C` ≠ 規格 `#8A5A33`；面數 24 含碰撞網格 | pipeline（量測/命名約定的問題，非建模） | 碰撞網格改名 `<id>_collision`（避開 Godot `_col` 慣例） | audit 修 sRGB 双重編碼 + 排除 `_collision` 後：12 面、`#8A5A33`、碰撞節點可见 |
| 2026-10-28 | probe_box | 3 | **PASS**（兩端尺寸 0.600m、原點 min_z≈0、12 面 ≤ 40、碰撞 `crcollider` extras 送達 Godot） | none | — | PASS，pipeline 正式打通 |
| 2026-10-28 | rock（冒煙資產） | 1 | FAIL：Godot 端 x 0.750（Blender 0.962）、z 0.626（0.696）、origin -0.323 | pipeline（audit 在 `_init` 內讀 `global_transform`，此時全是單位矩陣 → bbox 塌回局部座標；見 D19） | 不改模型（Blender 端全項合格） | 量測移到 `_process` 第一幀後 → 兩端逐物件對齊（rock_body 兩端同為 0.749/0.851/0.626） |
| 2026-10-28 | house | 1 | FAIL：Godot 端 x/y 9.19 m（Blender 4.70）；fantasy_shrine 同 8.63 vs 5.00 | pipeline（`Mesh.get_aabb()` 對旋轉節點是 AABB of AABB；斜屋頂轉了 45°；見 D18） | 不改模型 | 兩端都改用真實頂點遍歷 → PASS |
| 2026-10-28 | robot | 1 | FAIL：檢查工具逾時 240 s | pipeline（Godot 4.7 無 `AnimationPlayer.anim_list` → 腳本在 `_process` 拋錯不退出；M08） | 不改模型 | 改 `get_animation_list()` → PASS，且 Godot 收到 `wheel_roll`（0.833 s） |
| 2026-10-28 | 全部 14 項資產 | 1 | **PASS**：probe_box, rock, bush, tree, barrel, wooden_crate, coin, endpoint_flag, robot, brick_wall, house, fantasy_shrine, fantasy_arch, cloud（兩端尺寸/原點/面數/材料/動畫/碰撞全項合格） | none | — | 逐項結果在 `pipeline/checks/<id>.json` |
| 2026-10-28 | 規格修正（非 FAIL 迴圈） | — | 建模前自定的 `endpoint_flag` y=0.18 未含圓底座（實測 0.44）；`cloud` z=1.7 對 5 球堆疊過扁 | 規格估值問題 | 規格是唯一真相 → 在 `specs/` 修正並在 notes 記原因，不動模型 | flag y→0.46、cloud z→1.88（皆在 ±10% 檢查帶內） |
| 2026-10-28 | layout（版面） | 1 | FAIL 6 項：pads_flat 0.106、crate/barrel 坡度超限、barrel 與 bush 重疊、樹遮擋 bot 路線、tree1 出牆、prop_counts（tree 15/22、rock 4/16） | pipeline（驗證器算法問題，非版面問題） | 不改物件座標 | ① pad 平整度改為在 core 內縮 0.5 m 取樣（原來的 ±0.5 m 鄰域量到了 pad 外的混帶斜率）② `place_seeded` 原本只跟同批比較，改成連手工摆的 crate/barrel 一起比 ③ 出牆檢查改用切比雪夫距離（地圖是方形）④ 遮擋檢查改用碰撞輪廓半徑（D24）⑤ 间距改躯體半徑（D23）→ 全部通過 |
| 2026-10-28 | layout（版面） | 2 | **PASS** 14/14：pads_flat 0.000、路線沿路/淨空、無遮擋、無重疊、可推物 ≥1.5 m 淨空、拱門孔洞對準道路 | none | — | `tools/gen_layout.py --write` → `godot/data/layout.json`（seed 20261028 可重現）；props：house×2、shrine、arch、crate×8、barrel×6、tree×22、bush×34、rock×16；牆 44 段、雲 6 朵 |
| 2026-10-28 | terrain（地塊） | 1 | FAIL 預期：規格 z=3.20，實測 3.617（超出 ±0.32 容差） | 規格估值問題（建模後才知道：地塊邊緣被牆環壓平在 0，最高點 +0.417，z = 側裙 3.2 + 隆起 0.42） | 不改模型（x/y 44.00 準確、面數 5102 ≤ 6000、原點 0） | 規格 z→3.62 並把算法寫進 notes → PASS |
| 2026-10-28 | terrain（地塊） | 1 | **PASS**：Blender 與 Godot 兩端 x 44.00 / y 44.00 / z 3.617、min_z 0、XY 置中 0、5102 面 ≤ 6000、8 個材料名稱與顏色相符、heightfield 碰撞資料 45×45 @1 m（node_y −3.2，高度 −0.50..+0.42） | none | — | `make check ID=terrain`；`godot/data/heightmap.json` 就緒給 WorldBuilder |
| 2026-10-28 | terrain 外觀自檢 | 1 | 第一版草坪逐格隨機 → 像迷彩；廣場是 7×7 硬邊色塊 | 美術（附錄 C「不雜亂」） | 改 `surface_material`：草地用低頻 blob 噪聲（3–6 m 團塊）+ 微小抖動；廣場加 1.2 m 土色過渡帶；新增 `terrain_plaza` 石材 | 自檢 `look` three_quarter/top 通過，重新匯出並 PASS |
| 2026-10-28 | 世界組裝（`make world-check`） | 1 | FAIL：22 條 SCRIPT/SHADER ERROR（`CPUParticles3D.scale_min`、`Environment.dof_blur_far_enabled`、`ArrayMesh.get_surface_arrays`、sampler 提示 `source_screen_texture`） | pipeline（Godot 4.7 API/著色器語法，非資產問題） | 見 M11/M12：4.7 屬性與方法名稱、自建 tilt-shift post | 重跑 0 錯誤；`WORLD_BUILD` props 90、walls 44、clouds 6、coins 3、colliders 105、sway_sensors 34、missing []、prop_y −0.42..0.80 |
| 2026-10-28 | bot 測試 | 1 | FAIL：bot 從頭到尾 speed 0、座標恆在 spawn（10,13），只有 stuck 事件 | pipeline（bot 導航） | 起點就是 route[0] 且「下一點更近才推進」永不成立 → 起點 cursor=nearest+1、改用「到達半徑 3 m 推進」（D31） | 第 2 次開始會跑完全程 |
| 2026-10-28 | bot 測試 | 2 | FAIL：coins 只收到第 1 枚（`index: -1`）、之後在 (-14,-11) 來回振盪 8 m | pipeline（meta 掛錯節點 + 導航規則） | WorldBuilder 把 `coin_index` 同時寫進 Area3D（原本只在視覺節點）；waypoint 到達半徑 3 m（D31） | 第 3 次 **PASS**：coins [0,1,2] 依序、win 24.58 s、21 事件、0 錯誤、exit 0 |
| 2026-10-28 | 物理測試 | 1 | FAIL 6 項：brake_stop_m 9.148（>1.5）、barrel_push_m 0、tree_sway_m 0、leaf_bursts 0、bush_sway_m 0、wall_overflow_m 無上報 | 遊戲端實作 + 測試器上限 | 煞車語意（D28）、推力改用控制意圖（D29）、`set_process(true)`（M13）、草叢 sensor 要 `monitorable`（`monitoring=false` 才對）、runner `--quit-after` 6000→20000 | 第 2 次 **PASS 15/15**（top 3.99 / accel 0.667 / boost 6.38 / wheel 2.54 / brake 0.625 / coast 1.94 / jump 0.907 / lean 0.302 / barrel 2.95 / tree 0.296 / leaf 3 / bush 0.256 / slope 0.218 / climb 11.71 / wall 0.00） |
| 2026-10-28 | 互動鏈路除錯方法 | — | 只看 0/FAIL 無法分辨「沒撞到」與「撞到沒反應」 | — | probe 印 `PHYS_NOTE`（目標與玩家座標、sway 的 gust/shake/bursts/processing） | 實測玩家停在距樹幹 0.63 m（=0.31+0.32 膠囊+圓柱）→ 撞到但 `_process` 沒跑 → 收斂到 M13 |
| 2026-10-28 | Godot MCP 執行（`run_project` + `get_debug_output`） | 1 | 視窗模式 Vulkan Forward+ 執行正常、無 SCRIPT ERROR，但有 6 條 GDScript warning（未用變數/參數/訊號、`node` 變數遮蔽） | 程式整潔 | 移除未用訊號與變數、未用參數加底線、端點節點改名 `flag` | 重跑 `--world-check` → 0 錯誤 0 warning |
| 2026-10-28 | 鏡頭需求（`make cam-check`） | 1 | **PASS**：`CAM_REPORT pitch 45.02° azimuth -135° distance 20.05 m no_roll true camera_relative_input true follow_rate 5.5` | none | — | 第一次版 `no_roll` 判据用 `basis.y`（45° 俯視必然偏）→ 改查 `basis.x` 的 y 分量（水平才叫不翻滾） |
| 2026-10-28 | 版面工具 `tools/gen_layout.py` | 1 | FAIL：`KeyError: 'dims_m'` → 修掉後又 `KeyError: 'collision'` | pipeline（工具遍歷 `specs/*.json` 時把非資產規格檔 `physics_thresholds.json` 當成資產） | 不改模型（資產端無關） | 兩處迴圈都跳過沒有 `dims_m`/`collision` 的規格檔 → `make layout` PASS 14/14，色圖一併重生 |
| 2026-10-28 | 地面外觀（Godot 端，非資產 FAIL 迴圈） | 1 | 截圖判定不合格：草坪是 1 m 硬邊棋盤格（Blender 逐面平塗的直接結果）、鋪石/道路無細節 | 場景端（Godot 顯示），非建模 | 不回 Blender：新增 `tools/gen_terrain_texture.py` 烘色圖（共用 `heightfield.surface_material`，同一 seed）+ `shaders/terrain_surface.gdshader` 以 `VERTEX.xz` 取 UV，WorldBuilder 只覆蓋 surface 0..4（土層 5..7 保留） | `make world-check` 0 錯誤；截圖草坪連續柔和、鋪石縫清楚、棋盤格消失（`play-20261028005735.png`、`ground-20261028005737.png`） |
| 2026-10-28 | 地面外觀（Godot 端，非資產 FAIL 迴圈） | 2 | 新缺陷：草坪上出現**格狀藍灰色斑塊**（階梯狀邊界，1–5 格一組）。對照實驗：把色圖覆蓋關掉同一像素從 (62,77,100) 變回 (43,86,49)；色圖本身 0 個藍 dominant 像素 → 不是烘焙問題 | 場景端（自建著色器剔除模式） | 不回 Blender | `terrain_surface.gdshader` 加 `render_mode cull_disabled`（glTF 材料是雙面、自建 shader 預設 cull_back，被剔除的頂面直接露出天空）→ 藍灰斑块消失（`play-20261028010728.png`） |
| 2026-10-28 | 雲影（附錄 B 第 2 條） | 1 | 先做「雲下陰影貼片」著色器：strength 0.24/0.30/0.35/0.42/0.65 截圖差異 ≤3%，`strength = 0` 對照僅 25 px 變動 → 該機制實際上看不到 | 場景端（自己加的間接機制無效） | 不回 Blender | 改走**雲網格真的投影**（`_set_cast` 遞規開 `cast_shadow`），刪除 `cloud_shadow.gdshader` 與貼片程式碼。對照實測：投射開/關差 5091 px、雲下 108 vs 157＝**69% 日照**（`overview-20261028010706.png` vs `-010702.png`） |
| 2026-10-28 | 樹影 / 環境光 | 1 | 樹影在草坪上是 1 m 藍灰鋸齒斑、陰影區發藍 | 場景端（陰影地圖解析度 + 純藍天環境光） | 不回 Blender | 平行光 `SHADOW_PARALLEL_4_SPLITS`（max 60 m）、環境光混入暖中性 `(0.62,0.60,0.55)`、sky_contribution 0.45 → 藍灰像素 9596 → 7797，陰影中性化（D38） |
| 2026-10-28 | 截圖机位（action preset） | 1 | 不合格：機器人不在畫面內（被 4.2 m 樹冠遮住）、HUD speed 0.1 m/s（正在推桶但看不到）；試過 (17.5,13.5)（卡 tree13 碰撞）、(14.6,15.0)（速度 0.8） | 場景端（机位選點） | 不回 Blender | 用版面資料篩候選（高物 >2 m 對視軸淨空、碰撞淨空）→ 改 (1.4,−4.4) 面向 +Z 推木桶5，`SHOT_FRAMES` 78 → 機器人、木桶、終點旗、木箱同框（`action-20261028010736.png`） |
| 2026-10-28 | 純 HTTP 非本機網址要能玩 | 2 | (1) 官方殼層擋 Secure Context → 停在 splash；(2) 放行後 `TypeError: ... reading 'addModule'`（非安全來源無 AudioWorklet），Chrome 也拒構 `AudioWorkletNode`（`InvalidStateError`） | 瀏覽器 `[SecureContext]` 限制（非遊戲缺陷） | 不動 Godot 程式 | 自訂殼層（D47）濾掉 Secure Context（僅 nothreads）＋非安全來源把音訊接靜音；實測 `http://192.168.1.100:<port>/<目錄>/index.html` 0 錯誤、`--bot` 通關 72.4 s、YOU WIN 截圖 `web-http-bot.png` |
| 2026-10-28 | 仓库根入口頁在 `http://scott-265k:<port>/` 不導向 | 2 | 兩個外因：仓库根的 `index.html` 被外部刪除（` D` → 目錄列表頂替）；Chrome 主機名解析區分大小寫（小寫 `scott-265k` 解析失敗，`/etc/hosts` 是 `scott-265K`） | 環境（非頁面邏輯） | 不動頁面 | `git restore index.html` 取回；驗收改用手打 LAN IP → `http://192.168.1.100:<port>/` 導向並渲染 PASS |
| 2026-10-28 | 目標網址 `http://<hostname>/<目錄>/index.html` | 1 | 純 HTTP + 主機名時 Godot 網頁版不啟動：`Secure Context ... missing`（實測 `http://scott-265K:8201/<目錄>/index.html`；同一路徑 `http://localhost:8201/...` 與 `https://scott-265K:8202/...` 都 PASS） | 瀏覽器安全來源規則（非遊戲缺陷、非路徑問題） | 不動 Godot | `tools/serve.py --https` 自簽憑證（實測 https 版 `gl true`、0 錯誤）；或 localhost / `ssh -L` 隧道。`make serve` / `make serve-https`（本行的「無法」已被下一輪自訂殼層解除，見 D47） |
| 2026-10-28 | 入口頁導向（第二層：仓库根入口） | 1 | 使用者回報 `/preview-spec_04…/index.html` 無法訪問。實測：同位址用**現名**全 200、用**舊名** 404 → 真因是交付目錄被改名（`…-max-20261028000156` → `…-max-80-20261028000156`），與 base URL 無關（base URL 問題已在 W.9 修掉） | 部署/路徑（非遊戲程式） | 不動 Godot | 在仓库根加入口頁（D45），網址不再需要交付目錄名；`python3 -m http.server` 從仓库根實測 6 種網址全 PASS |
| 2026-10-28 | 入口頁導向（`index.html` → `web/index.html`） | 1 | 使用者回報開不到；重現：站點掛在子路徑且**未補尾端斜線**時 final URL 是 `http://host/web/index.html`（404），因為相對網址的基準是**上一層**目錄 | 場景端（入口頁網址推算），非遊戲程式 | 不動 Godot | 入口頁改 JS 推候選 + `fetch` probe（D44）；四種網址（無斜線／有斜線／`index.html`／根）全 PASS，`file://` 改顯示 HTTP 說明 |
| 2026-10-28 | `tools/web_smoke.mjs` 錯誤可判讀性 | 1 | FAIL：導向 404 時 verdict 只寫 `Failed to load resource ... 404`，看不出是哪個檔案 404 | 測試工具（Log entry 只取 `source + text`，丢了 `e.url`） | 不改遊戲 | 記錄時帶上 `e.url`；瀏覽器 `/favicon.ico` 的 404 歸為噪音（不影響判定） |
| 2026-10-28 | WebGL 版（`make web` + `make web-smoke`） | 1 | **PASS**：無頭 Chrome 載入 `web/index.html` → 引擎橫幅、canvas 1280×633、`WebGL 2.0 - Compatibility`、console 0 錯誤；`--bot` 收到 `COINRUN_BOT {"coins":3,"elapsed":72.4,"event":"win"}`、world `props 90 / walls 44 / coins 3` | none | 不動 Blender（Web 版用同一份資產與版面） | 截圖 `pipeline/screenshots/web-smoke.png`、`web-bot.png`；精簡版 pck 466 KB / wasm 39.5 MB |
| 2026-10-28 | WebGL 版鍵盤輸入 | 1 | 驗不到：CDP `Input.dispatchKeyEvent`（`rawKeyDown` ArrowRight）與頁面內合成 `KeyboardEvent('keydown')` 都送達 canvas（`dom_keys` 實測 19 次、`target=CANVAS focus=true`），HUD 仍 `SPEED 0.0 m/s` | 測試工具限制（無頭 Chrome 的按鍵不被 Godot 的 web 輸入鏈收用），**非已驗證可用** | 不改玩法程式碼 | 標為未驗證項目（progress W.8）；需在一般瀏覽器以真實按鍵確認。同場已驗證 bot 模式在瀏覽器內完整跑完，所以遊戲邏輯與物理在 WebGL 下正常 |
| 2026-10-28 | 附錄 B 五條逐項判定 | 1 | **PASS**（5/5，證據見下方「畫面驗收（附錄 B）」） | none | — | bot 24.58 s、physics 15/15、world-check 0 錯誤同時複核通過 |

| 2026-10-28 | coin（Phase 9 迴圈演練：規格→檢查→Blender） | 1 | 使用者回饋（Playbook 附錄 E 情境）：金幣 0.70 m 直立圓盤在等角鏡頭下擋住道路與終點旗視線，箱庭比例過大 → **規格縮至約 60%**（`dims_m` 0.70×0.12×0.70 → 0.42×0.07×0.42；收集盒 0.8×0.3×0.8 → 0.5×0.2×0.5） | 規格是唯一真相：先改 `specs/coin.json`，讓檢查工具去要求 Blender | 「把 `blender/coin.blend` 的視覺網格等比縮到 60%，原點仍在底部中心，材料/面數/動畫不動，套好修改器後依 `tools/blender_export.py` 匯出」 | 見下一列 |
| 2026-10-28 | coin | 2 | 預期 FAIL（改規格後、尚未重建模）：`make check ID=coin` 量到 Godot 端 x/z ≈ 0.70 m，超出規格 0.42 m 的容差帶（±10% → 0.378..0.462）→ 判 `modeling`、建議回 Blender | 檢查工具如預期抓住「規格已改、模型未改」 | 同第 1 次 | 回 Blender 重建模（下一步） |

| 2026-10-28 | coin | 3 | **PASS**：兩端 x 0.4200 / y 0.0810 / z 0.4200（目標 0.42 / 0.07 / 0.42，容差 ±10% 與 2 cm 底限）、`min_z 0`（原點仍在底部中心）、184 面 ≤ 240、材料 `coin_gold` `#E8B92C` 與 `coin_gold_dark` `#C8952C` 相符 | none | Blender MCP：根空物件在原點，逐子物件 `location *= 0.6`、`scale *= 0.6`（等比、不移動原點），`look` front/right/top 自檢（16 邊圓盤＋齒紋＋圖騰完整、0.42×0.08×0.42、中心 z 0.21）後用 `tools/blender_export.py` 匯出（11 物件、20916 bytes） | `make check ID=coin` PASS；`blender/coin.blend` 與 `godot/assets/coin/coin.glb` 一起更新 |
| 2026-10-28 | coin（遊戲端同步） | — | 需要同步的只有**從規格衍生的資料**：收集盒 0.8×0.3×0.8 → 0.5×0.2×0.5 | 版面是規格的下游客 | `make layout` 重新把 `specs/coin.json` 的 `collision`/`dims_m` 嵌進 `godot/data/layout.json`（WorldBuilder 執行期讀它），`make import` 讓 Godot 重新匯入新 glb | layout 內 coin 收集盒實測 `[0.5, 0.2, 0.5]`、dims `0.42/0.07/0.42` |
| 2026-10-28 | 縮小後回歸測試 | 1 | **PASS**：`make bot` → coins [0,1,2] 依序、win 24.68 s（縮小前 24.58 s，收集不受影響）；`make physics` → 15/15；`make world-check` → 0 錯誤 0 warning，coins 3、colliders 105、missing [] | none | — | 0.5 m 收集盒在 4 m/s、60 Hz（每步 0.067 m）下不會被穿過 |
| 2026-10-28 | 縮小後畫面 | 1 | **PASS**：金幣在等角鏡頭不再压住道路/終點旗視線；遊玩机位（9.5 m）下仍明顯可讀（`play-20261028011018.png` 左上硬幣清楚），全地塊机位下變成小黃點（箱庭比例合理） | none | — | 五個 preset 全重拍：`overview/play/action/road/ground-202610280110*.png` |

## 畫面驗收（附錄 B）

證據檔：`pipeline/screenshots/`（1280×720，Vulkan Forward+ / RTX PRO 6000，由 `tools/capture.py --preset ...` 產生）。

| 附錄 B 項目 | 判定 | 證據與量測 |
|---|---|---|
| 一般遊玩畫面：機器人清楚可辨、看得到周圍場景、陰影柔和、移軸迷你模型感 | **PASS** | `play-20261028010728.png`：`cam.distance 9.5`（清棋机位），機器人佔画面中央、四周道路/屋舍/旗子可讀；移軸 band 0.15、strength 0.8（`tilt_shift.gd`），樹影柔和（`light_angular_distance 12°`） |
| 整體畫面：整個地塊入鏡、構圖豐富、所有元素看得到、雲在空中且地上看得到雲影 | **PASS** | `overview-20261028010726.png`：`cam.distance 52`，44×44 地塊與牆環全入鏡、無大片空白；6 朵雲在天上（y 16.5–19.0）；雲影＝雲網格投影，實測雲下亮度 **108 vs 157（69%）**、覆蓋 5091 px |
| 地面：草坪不單調不假、道路清楚、看得出高低起伏 | **PASS** | `ground-20261028010731.png`、`road-20261028010734.png`：色圖 + 粒狀噪聲（≥2 m）取代 1 m 棋盤格；道路是連續土色帶並帶鋪石縫（8 px/m、mipmaps）；起伏由地塊側裙與道路下坡可見（高度 −0.504..+0.417、1 m 最大坡度 0.218） |
| 無倒立/躺平/懸空/陷入；高大物件不擋玩家常走區域 | **PASS** | `make world-check`：`prop_y_min −0.420`（皆為側裙/牆基底面，接地件 min_z≈0）、`prop_y_max 0.80`；`make layout` 14/14 含 `tall_props_not_shading_route`、`route_clear_of_blockers`、`no_prop_overlap` |
| 木箱造型與擺放和場景搭配 | **PASS** | `action-20261028010736.png`：3 個木箱在房子旁的路邊成組（版面 `CRATES` 規則：靠 pad、離路 ≥1.5 m、可推物淨空），與道路/草叢/木桶同框不突兀 |

## 物理門檻（Phase 8.2）

來源：`specs/physics_thresholds.json`（唯一真相，含逐項 `why`）；實測：`pipeline/checks/physics.json`。
量測場景 `res://scenes/tests/physics_test.tscn` 使用真的 WorldBuilder + 真的 `player.gd`（D26），未回報的 id 直接判 FAIL（D27）。

| id | 門檻 | 實測 | 理由（摘要） |
|---|---|---|---|
| top_speed | range 3.6..4.5 m/s | 3.99 | 需求表時速 4 m/s，留坡度與貼地餘量 |
| accel_time_s | max 1.2 s | 0.667 | accel 6.5 → 理論 0.62 s；要起步俐索但不能瞬時到速（有慣性） |
| boost_speed | range 5.8..7.4 m/s | 6.384 | boost 1.6×4 = 6.4 |
| wheel_rps | range 2.2..2.9 rev/s | 2.54 | 輪半徑 0.25 m → 4 m/s 應轉 2.55 rev/s（動畫與速度綁定、不打滑） |
| brake_stop_m | max 1.5 m | 0.625 | brake 12 m/s² → 理論 0.67 m；煞得下來但不會瞬間定住 |
| inertia_slide_m | range 1.0..3.5 m | 1.937 | 放手後摩擦力 4 m/s² → 理論 2.0 m；下限證明有慣性 |
| jump_height_m | range 0.6..1.6 m | 0.907 | jump 5.2、重力 15.68 → 0.86 m；要跳得過草叢等級障礙又不能飛 |
| lean_rad | range 0.15..0.55 rad | 0.302 | 設計 0.16 + boost 0.11 = 0.27 rad（需求：加速時身傾斜） |
| barrel_push_m | min 0.8 m | 2.947 | 需求：木桶可被推（推 3 秒要明顯位移，且不能一碰就飛） |
| tree_sway_m | min 0.25 m | 0.296 | 需求：撞樹會搖晃（撞擊 gust 0.30 m） |
| leaf_bursts | min 1 | 3 | 需求：撞樹掉葉 |
| bush_sway_m | min 0.2 m | 0.256 | 需求：可穿過草叢且穿過時會晃（brush gust 0.26 m） |
| climb_slope | min 0.18 m/m | 0.218 | 地形 1 m 網格最陡 0.218 m/m（12.3°）→ 機器人要爬得上全圖 |
| climb_progress_m | min 2.5 m | 11.714 | 爬坡要真的前進，不是頂在坡上 |
| wall_overflow_m | max 0.0 m | 0.0 | 需求：角色走不出去（boost 全速撞牆後不可越過 40×40 範圍） |

## 環境檢查

| 時間 | 項目 | 結果 |
|---|---|---|
| 2026-10-28 | Godot | 4.7.2 stable `/home/scott/tools/bin/godot` |
| 2026-10-28 | Blender | 5.2.2 LTS，執行中（`/home/scott/tools/blender/blender`） |
| 2026-10-28 | Node.js | v22.22.3（npx 可用） |
| 2026-10-28 | uv / uvx | uv 0.12.18 |
| 2026-10-28 | git | 2.53.0 |
| 2026-10-28 | 顯示器 | DISPLAY=:0（wayland/x11 可用，截圖可行） |
| 2026-10-28 | 按鍵綁定（程式確認） | `INPUT_VERIFY pass=true`：7 個 action 全部綁上方向鍵/dpad + Space/Enter/padA + A鍵/padX + B鍵/padB，且無多餘 action |
| 2026-10-28 | Godot MCP 唯讀工具 | `get_godot_version` → 4.7.2.stable；`get_project_info` → scenes 1 / scripts 3 / assets 1 |
| 2026-10-28 | Blender MCP 唯讀工具 | `get_addon_status` → addon 1.8、Blender 5.2.2 LTS、protocol 15、polyhaven/sketchfab/polypizza 與生成器皆 off；`get_scene_info`、`look` 可用 |
