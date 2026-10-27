# Progress — Coin Run（PLAYBOOK v2.1）

標記：`[v]` 通過、`[x]` 做了沒過、`[ ]` 未做

## Phase 0 環境檢查

- [v] Godot ≥ 4.3（實測 4.7.2 @ /home/scott/tools/bin/godot）
- [v] Blender 可執行（5.2.2 LTS @ /home/scott/tools/blender/blender，執行中）
- [v] Node.js ≥ 18（22.22.3）、uv（0.12.18）、git（2.53.0）
- [v] Godot 執行檔位置已確認

## Phase 1 專案骨架

- [v] 1.1 專案結構規劃完成
- [v] 1.2 規則檔 `AGENTS.md` + `CLAUDE.md`
- [v] 1.3 MCP 伺服器與權限設定（Godot/檢查工具自動允許；Blender `execute_blender_code` 不自動允許）
- [v] 1.4 Godot 專案 + 定案按鍵，程式確認按鍵綁上（`make verify-input` → INPUT_VERIFY pass=true）
- [v] 1.5 自動化工具：env 檢查 / Godot 匯入 / 規格比對 / bot 測試 / 物理測試
      （`make env`、`make import`、`make check ID=`、`make bot`、`make physics`、`make shot`；
      bot/物理的 Godot 端場景在 Phase 7/8 建立，runner 已就緒）
- [v] 1.6 `pipeline/progress.md`、`pipeline/log.md`、`git init`、第一次 Godot 匯入（probe_box）、`git commit`

## Phase 2 連線 MCP（STOP，人工）

- [v] 2.0 告知人工步驟並停下（本機 MCP 已連線，人工步驟經確認不需執行；已停下等使用者回覆「繼續」）
- [v] 2.1 驗證：環境檢查通過（`make env` 全 [v]）
- [v] 2.2 Godot MCP 呼叫唯讀工具成功
- [v] 2.3 Blender MCP 呼叫唯讀工具成功（`get_addon_status`：addon 1.8 / Blender 5.2.2 / 生成器與外部庫皆 off）

## Phase 3 冒煙測試

- [v] 選一個最簡單資產（`rock`）
- [v] 寫規格
- [v] Blender 建模（Blender MCP）
- [v] 自檢外觀（`look` angles front/right/top）
- [v] 匯出 GLB
- [v] Godot 匯入
- [v] 規格檢查 PASS（第 3 次通過，前 2 次為 pipeline 問題）
- [v] 放進測試場景 `scenes/smoke.tscn`（13 個資產全部實例化）
- [v] Godot MCP 執行無錯誤（`run_project` + `get_debug_output` → errors=[]，SMOKE assets/SMOKE_OK）
- [v] git commit

## Phase 4 資產（先角色與道具，再建築）

- [v] robot（單輪機器人，輪子動畫 `wheel_roll` 已送達 Godot，0.833 s @30fps）
- [v] coin
- [v] endpoint_flag
- [v] wooden_crate
- [v] barrel
- [v] tree
- [v] bush
- [v] rock
- [v] brick_wall
- [v] house
- [v] fantasy 建築（`fantasy_shrine` 神龕 + `fantasy_arch` 拱門）
- [v] cloud
- [v] 每類做完 git commit

## Phase 5 地面

- [v] 依附錄 C 建立箱庭地塊資產（44×44 m、側裙三層土層 3.2 m、底面平整）
- [v] 地表起伏且可通行（`make physics`：climb_slope 0.218 m/m 實測爬過、climb_progress_m 11.71；bot 全程無卡住）
- [v] 建築/圍牆基座平整區（`gen_layout.py` 檢查 pads_flat 最大 0.000 m/m）
- [v] 草坪深淺變化（3 色 blobs）+ 道路連接主要區域（6 條道路、廣場鋪面）
- [v] 地面資產通過規格檢查（`make check ID=terrain` PASS，兩端 44.0/44.0/3.62、5102 面）
- [v] 其他物件可正確放置在起伏地面（`make world-check`：90 件摆件依 heightmap 落地，prop_y −0.42..0.80）

## Phase 6 世界組裝

- [v] 版面資料檔 `godot/data/layout.json`（`tools/gen_layout.py` 產生 + 14 項自動驗證，seed 20261028 可重現）
- [v] 地面 + 外牆圍住 40×40 m（44 段牆；`make physics` wall_overflow_m = 0.00 → 全速撞牆走不出去）
- [v] 所有場景元素、金幣 ×3、終點（樹22/草叢34/石16/木箱8/木桶6/房子2/神龕1/拱門1 + 雲6 + 金幣3 + 終點旗）
- [ ] 分區清楚、構圖豐富、留 runnable 空間 → 依附錄 B 在 8.5 截圖逐項判定
- [v] git commit

## Phase 7 遊戲邏輯、物理、氣氛

- [v] 收集 3 金幣 + 碰終點旗 = 勝利（`make bot` → win，24.58 s；未收齊碰旗顯示 NEED n MORE COIN(S) FIRST）
- [v] 力向量 + 摩擦力移動，慣性，4 m/s（top_speed 3.99、accel_time 0.667 s、inertia_slide 1.94 m）
- [v] 按鍵：boost(A/X)、brake(B/B)、jump(Space/Enter/A)（`make verify-input` 通過；boost_speed 6.38、brake_stop 0.625 m、jump 0.907 m）
- [v] 加速身傾斜（lean_rad 0.302 rad ≈ 17°）
- [v] 木桶可推動（barrel_push_m 2.95 m）
- [v] 撞樹搖晃 + 掉葉（tree_sway_m 0.296、leaf_bursts 3）
- [v] 穿草叢時草叢晃（bush_sway_m 0.256；草叢 collision mode = none，可穿過）
- [v] 等角 45° 平滑跟隨，方向鍵相對鏡頭（`make cam-check`：pitch 45.02°、方位 −135°、無翻滾、camera_relative_input = true）
- [v] 氣氛參數：暖阳光（#FFF0C9、角直徑 12° 軟陰影）+ SSAO + 雲漂移 0.35 m/s + 樹/草叢風摆 shader + 移軸（`shaders/tilt_shift.gdshader`）；畫面感受在 8.5 截圖判定
- [v] bot 模式（`-- --bot`，忽略輸入，依序 3 金幣 → 終點；`make bot` PASS）
- [v] git commit

## Phase 8 測試與畫面檢查

- [v] 8.1 bot 測試通過（coins [0,1,2] 依序、win 訊息、21 個事件、0 腳本錯誤、exit 0）
- [v] 8.2 物理測試通過（15/15，門檻與理由在 `specs/physics_thresholds.json` + log）
- [v] 8.3 Godot MCP 執行遊戲無錯誤（`run_project` + `get_debug_output` → Vulkan Forward+，errors 只有 6 條 GDScript warning，已全數清除）
- [v] 8.4 截圖：一般遊玩畫面 + 全地塊畫面（五個 preset：`play`/`overview`/`ground`/`road`/`action`，最新一組 `*-20261028010726..36.png`）
- [v] 8.5 依附錄 B 檢查並修正（資產問題回 Blender／場景問題修 Godot），重拍 → 5/5 PASS，逐項證據與修正表見 `pipeline/log.md`「畫面驗收（附錄 B）」
- [v] 8.6 git commit

## Phase 9 迴圈演練

- [v] 9.1 log 寫回饋：金幣太大擋視線，規格縮至約 60%（`specs/coin.json` 0.70×0.12×0.70 → 0.42×0.07×0.42，收集盒 0.8×0.3×0.8 → 0.5×0.2×0.5）
- [v] 9.2 改規格 → 自動檢查應 FAIL，並記錄（`make check ID=coin`：兩端 x/y/z 0.70/0.135/0.70 對目標 0.42/0.07/0.42 → `FAIL coin cause=modeling`）
- [v] 9.3 依建議回 Blender 調整 → 匯出 → 檢查 PASS（Blender MCP 等比 0.6、`look` 三視自檢、`tools/blender_export.py` 匯出 → 兩端 0.4200/0.0810/0.4200、min_z 0、184 面 → PASS）
- [v] 9.4 同步調整遊戲端相關設定；bot/物理測試仍通過（`make layout` 嵌入新收集盒 0.5×0.2×0.5、`make import`；bot PASS win 24.68 s、physics PASS 15/15、world-check 0 錯誤）
- [v] 9.5 重新執行確認畫面，git commit（五個 preset 重拍 `pipeline/screenshots/*-202610280110*.png`；金幣在遊玩机位可讀、不再壓住視線）

## Phase 10 收尾

- [v] 10.1 `pipeline/REPORT.md`（DoD 結果、逐資產迭代次數、MCP 問題與替代、物理門檻與實測、設計決策、問題與解法、最花時間的步驟、下一步）
- [v] 10.2 git commit
- [v] 10.3 回報使用者（附 REPORT 摘要）

## DoD 對照

- [v] 每個 3D 資產通過自動規格檢查（14 個資產 + terrain，Blender 端 + Godot 匯入後兩端比對）
- [v] bot 測試通過（3 金幣 → 終點，可判讀進度與勝利訊息，無腳本錯誤）
- [v] 物理測試通過（15 項可量測門檻全過）
- [v] Godot MCP 執行無錯誤；截圖依附錄 B 檢查過（8.4/8.5 已完成，逐項證據在 log「畫面驗收（附錄 B）」）
- [v] 完成一次迴圈演練（Phase 9：coin 規格縮至 60% → FAIL → Blender 重建 → PASS → bot/physics 複測 → 重拍）
- [v] `pipeline/REPORT.md` 已寫

## 追加需求：WebGL 版成品

- [v] W.1 Web 匯出組態：`godot/export_presets.cfg`（Web preset、`thread_support=false`→不需 COOP/COEP、`include_filter="*.json"` 帶進 `layout.json`/`heightmap.json`）
- [v] W.2 Web 渲染器切到 `gl_compatibility`（`project.godot` 的 `renderer/rendering_method.mobile`；桌面保持 Forward+）。實測瀏覽器印出 `OpenGL ES 3.0 (WebGL 2.0) - Compatibility`
- [v] W.3 `make web` 匯出到 `web/`（`index.html` + `index.wasm` 39.5 MB + `index.pck` 466 KB；pck 內確認有 `data/layout.json`、`data/heightmap.json`、`terrain_surface.png`、全部 `.glb` 與 shader）
- [v] W.4 最外層入口頁 `index.html`：meta refresh + `location.replace` 導向 `web/index.html`，並寫明要用 HTTP 開（`make serve`）
- [v] W.5 `make serve`（`python3 -m http.server`）實測：`/` 200 導向頁、`index.wasm` 200 `application/wasm`、`index.pck` 200
- [v] W.6 `tools/web_smoke.mjs` 無頭 Chrome（CDP + SwiftShader）驗證 `web/`：引擎啟動、canvas 1280×633、WebGL2 true、console 0 錯誤；截圖 `pipeline/screenshots/web-smoke.png`、`web-play.png`
- [v] W.7 瀏覽器內 bot 通關：`web_smoke.mjs --bot`（把 `GODOT_CONFIG.args` 改成 `["--","--bot"]`）→ console 收到 `COINRUN_BOT {"coins":3,"elapsed":72.4,"event":"win",...}`，world 組裝 `props 90 / walls 44 / coins 3`。`pipeline/screenshots/web-bot.png`
- [v] W.9 入口頁導向修正（使用者回報：從輸入目錄開 index 開不到 `web/index.html`，base URL 問題）
      - 原因：入口頁用相對網址 `web/index.html`。站點掛在子路徑**且伺服器不補尾端斜線**時（例 `http://host/preview-spec_04…`），基準路徑變成上一層 → 導到 `http://host/web/index.html` → 404（實測重現：`final http://127.0.0.1:8137/web/index.html`）
      - 解法：入口頁改用 JS 從 `location.pathname` 推候選（把目前路徑當目錄 / 當檔案兩種），`fetch` probe 哪個存在（認 Godot 殼層的 `index.js` 特徵）再 `location.replace`；拿不到就列出可點候選網址。`meta refresh` 會在 JS 之前跑掉，所以移除
      - 實測（`web_smoke --root` 從上層目錄當 web 根）：`/preview-spec_04…`（無斜線）、`/preview-spec_04…/`、`/preview-spec_04…/index.html`、`/` 四種**全 PASS**，final 都落在 `.../web/index.html`、`gl true`、0 錯誤
      - `file://` 直開：入口頁顯示「需要 HTTP（wasm 不能用 file:// 載入）+ `make serve`」，不再導向坏掉的頁面（截圖 `/tmp/shot_file_crop.png`）
      - 驗收工具順手修：`web_smoke.mjs` 的 Log 訊息帶上 `e.url`（原先 404 只看得到文字、查不到是哪个檔案），且瀏覽器預設抓 `/favicon.ico` 的 404 視為噪音

- [v] W.10 第二層入口頁（使用者：從 `preview-spec_04…/..` 啟動 http server，`/preview-spec_04…/index.html` 無法訪問）
      - 真因（實測重現）：**交付目錄被改名**了 —— 舊名 `preview-spec_04-swift-1.5-iq3_s-max-20261028000156` → 現名 `preview-spec_04-swift-1.5-iq3_s-max-80-20261028000156`。舊名網址由伺服器直接回 404（`File not found`），不是 base URL
      - 對策：在**仓库根**（= 使用者的 http server 根）加 `index.html` 入口頁 → 探測 `<交付目錄>/web/index.html` 後導向；找不到就列出候選網址讓人點。這樣網址不用帶交付目錄名
      - 實測（`python3 -m http.server` 從仓库根，逐一把網頁跑起來看 final URL）：`/`、`/index.html`、`/<新名>`、`/<新名>/`、`/<新名>/index.html`、`/<新名>/web/index.html` **6/6 PASS**（`gl true`、0 錯誤）；舊名網址如預期 404

- [v] W.11 目標網址 `http://<hostname>/preview-spec_04…/index.html`（使用者目標）
      - 查到的兩條真實門檻（實測矩陣，`tools/serve.py` + `web_smoke`）：
        1. `http://localhost:<port>/<目錄>/index.html` → **PASS**（`gl true`、0 錯誤）
        2. `http://<hostname>:<port>/<目錄>/index.html` → Godot 拒絕啟動：`The following features required to run Godot projects on the Web are missing: Secure Context`（瀏覽器規則：HTTP 只有 localhost/127.0.0.1 是安全來源）
        3. `https://<hostname>:<port>/<目錄>/index.html`（`--https` 自簽憑證）→ **PASS**（0 錯誤）
      - 新增 `tools/serve.py`：預設以**仓库根**為 web 根（目標網址的形態）、`.wasm` 給 `application/wasm`、印出精確網址、`--https` 自動開自簽憑證、port<1024 需 root 的提示、`/home/scott` 缺 other+x 会让 www-data server 403 的提示
      - 结论（**已被 W.12 取代**）：當時只能 `--https` / `localhost` / `ssh -L` 繞過；W.12 用自訂殼層放行 Secure Context 後，純 HTTP 直接可用

- [v] W.12 純 HTTP + 非 localhost 網址（使用者：「我要 http 可以訪問啊」）
      - 兩條真實門檻（都實測重現）：
        1. Godot 官方殼層：`Engine.getMissingFeatures` 在非安全來源回報 `Secure Context`，缺漏非空就**不啟動引擎**（停在 splash）
        2. 放行後仍死：`TypeError: Cannot read properties of undefined (reading 'addModule')` — `AudioWorklet` 是 `[SecureContext]`，HTTP 非本機來源時 `ctx.audioWorklet` 是 undefined，而 Godot 的音訊初始化**無條件**呼叫 `ctx.audioWorklet.addModule()`（Chrome 連 `new AudioWorkletNode()` 也直接 `InvalidStateError`）
      - 對策（自訂殼層 `godot/web_shell.html`，由匯出結果還原 `$GODOT_CONFIG` / `$GODOT_THREADS_ENABLED` 佔位符）：
        - 單執行緒版（`GODOT_THREADS_ENABLED=false`，本交付就是）把 `Secure Context` 從缺漏清單濾掉——該項實際是 SharedArrayBuffer / COOP-COEP 的需求，nothreads 版不需要
        - 非安全來源時把 `ctx.audioWorklet.addModule()` 與 `AudioWorkletNode` 接成**靜音 stub**（本遊戲無音效）；安全來源不受影響，照樣走原生 AudioWorklet
        - `export_presets.cfg`：`html/custom_html_shell="web_shell.html"` → 改殼層後 `make web` 重新匯出（不手改 `web/`）
      - 實測矩陣（`tools/serve.py` + `tools/web_smoke.mjs`，逐一把網頁跑起來）：
        - `http://localhost:<port>/<目錄>/index.html` → **PASS**（`gl true`、0 錯誤）
        - `http://192.168.1.100:<port>/<目錄>/index.html`（純 HTTP + LAN IP）→ **PASS**（`gl true`、0 錯誤，截圖 `pipeline/screenshots/web-http-host.png`）
        - `http://192.168.1.100:<port>/<目錄>/web/index.html --bot` → **bot 通關**（`{"coins":3,"elapsed":72.4,"event":"win"}`，0 錯誤，截圖 `web-http-bot.png` 的 YOU WIN）
        - `http://<LAN IP>:<port>/`（仓库根入口頁）→ 導向 `<目錄>/web/index.html` 並渲染 **PASS**
        - `https://<hostname>:<port>/<目錄>/index.html` → **PASS**（自簽憑證）
      - 順手修掉的驗收工具問題：`tools/serve.py --bot`（把 `"args":[]` 換成 `["--","--bot"]`）讓 bot 通關能在**任意網址**驗；`web_smoke` 把 Godot 退出時的 `at: ~PagedAllocator` 續行歸入 shutdown noise（不是遊戲錯誤）
      - 環境陷阱（實測）：Chrome 的主機名解析**區分大小寫** —— `http://scott-265K:…` 通、`http://scott-265k:…` 直接解析失敗（`/etc/hosts` 寫的是 `scott-265K`）→ 驗收用 LAN IP；仓库根的 `index.html` 曾被外部刪除（`git status` 顯示 ` D`）→ `git restore index.html` 取回

- [x] W.8 鍵盤輸入在**無頭 Chrome** 下驗不到：CDP `Input.dispatchKeyEvent` 與頁面內合成 `keydown` 都送到 canvas（實測 `dom_keys: ArrowRight keyCode=39 target=CANVAS focus=true`），但 HUD `SPEED 0.0 m/s` 不變 → 判定為**測試工具限制，未驗證項目**（不是已驗證可用）。真人真實按鍵屬 Godot 標準行為，需用一般瀏覽器手動確認
