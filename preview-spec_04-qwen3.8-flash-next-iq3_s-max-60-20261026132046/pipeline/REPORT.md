# Coin Run 箱庭版 — 完成報告

日期：2026-10-26 ・ 依 `.spec/PLAYBOOK_v2.1.md` 全流程執行（Phase 0–10）

## DoD 勾選結果

| DoD | 結果 |
|---|---|
| 13 個資產全部通過三階段自動檢查（Blender / glb / Godot 匯入後，±10%） | `[v]` 全 PASS（紀錄 `pipeline/checks/*_check.json`） |
| 地面可走、起伏、有材質變化 | `[v]` 44×44 島、±0.55 起伏、草地/道路/農地三區、土層側面；heights.json 驅動物件擺放 |
| 遊戲可操作（鍵盤），物理互動成立 | `[v]` 方向鍵 + A 加速 / B 煞車 / Space 跳；推桶、撞樹搖晃落葉、穿草叢晃動 |
| bot 測試通過（關卡可完成） | `[v]` `--mode bot`：10 stages、3 幣、BOT_VICTORY、exit 0 |
| 物理測試通過（有門檻與實測） | `[v]` 7/7 PASS，見下方實測表 |
| 透過 Godot MCP 實際執行遊戲沒有錯誤 | `[v]` run_project + get_debug_output：0 errors / 0 warnings |
| 截圖依附錄 B 檢查過 | `[v]` `pipeline/screenshots/gameplay_1.png` + `overview_1.png`（修掉 col_barrel 洋紅渲染後重拍） |
| 完成一次迴圈演練（Phase 9） | `[v]` 金幣 0.6→0.36：FAIL→修→PASS→遊戲同步→重測通過 |
| `pipeline/REPORT.md` 已寫 | `[v]` 本檔 |

## 每個資產的迭代次數（FAIL 迴圈）

| 資產 | 迭代 | 備註 |
|---|---|---|
| rock | 1 | 檢查工具軸序換算 bug（pipeline 類），修工具 |
| robot | 0 | 一次 PASS（WheelSpin 動畫一次到位） |
| coin | 1 + Phase9 2 輪 | P4 fit API 同步；P9 縮小迴圈（見下） |
| flag | 1 | fit 目標與規格不一致（model 類），重建 PASS |
| crate | 0 | 一次 PASS |
| barrel | 1 | col_barrel 未 parent 到 AssetRoot 致 bbox 6.74m（model 類） |
| tree | 2 | 動畫唯一名 + 空物件 bbox 污染（pipeline）；fit_bbox 巢狀 parent 改迭代收斂 |
| bush | 1 | 同 export 置中污染（pipeline），修工具 |
| wall / house / ancient_tower / ancient_shrine / cloud | 0 | 一次 PASS |
| ground | 0 | 一次 PASS；heights.json 同步行輸出 |

同一資產每輪 ≤3 次修正的上限從未觸發。

## 物理測試門檻與實測

設定：重力 18、驅動 a0=12、線性阻力 k=3 → 末端速度 4 m/s；跳躍 v=4.4（apex≈0.54m）；桶 9kg。

| 場景 | 實測 | 門檻 | 判定 |
|---|---|---|---|
| brake stop_distance | 0.331 m | [0.2, 1.8] | PASS |
| push barrel_speed | 2.241 m/s | [0.8, 5.0] | PASS |
| friction barrel_slide | 3.607 m | [0.1, 4.0] | PASS |
| jump apex | 0.492 m | [0.38, 0.80] | PASS |
| sway tree_sway | 0.377 rad | [0.05, 0.80] | PASS |
| leaves leaf_bursts | 1 | [1, 5] | PASS |
| bush pass_speed | 3.55 m/s | [1.2, 4.5] | PASS |

門檻理由寫在 `specs/physics_tests.json`；判定由 `tools/check_physics.py` 產出 `pipeline/checks/physics_check.json`。

## 主要設計決策（詳見 log.md）

- **單一版面真相**：全部擺件/道路/錢幣/旗幟在 `godot/data/layout.json`，`WorldSpawner.gd` 執行期重現；地面起伏由 Blender 輸出的 `heights.json` 提供，所有物件 y 由 `height_at(x,z)` 計算，不做手擺。
- **座標約定**：Blender Z-up、正面 -Y → glTF +Y up → Godot；實測 robot 正面匯入後是 **+Z**（verify_facing.gd），遊戲內以量測為準修正方向計算。
- **機器人物理**：RigidBody3D + 力/阻力模型（非 CharacterBody3D），天然滿足「力+摩擦+慣性」且能推桶；膠囊低摩擦、側向速度顯式衰減（arcade 滾動），角鎖定防翻覆、傾斜是視覺。
- **碰撞策略**：GLB 內 `col_barrel` 只作規格驗證用；Godot importer 不認得 `col_` 前綴 → 遊戲端依規格建 primitive（已記 log）；`col_barrel` 可見網格在遊戲端隱藏。
- **測試協定**：`--mode play|bot|phys|shots`；stdout 行 `BOT_*` / `PHYS <id> <metric>=<value>` 由工具解析；截圖由遊戲存 `pipeline/screenshots/`。

## MCP 問題與替代做法

1. godot-mcp 無場景編輯工具 → tscn/gd 由 agent 直接寫檔；執行與錯誤擷取仍用 MCP。
2. blender-mcp `execute_code` 不回傳 print → 改「Blender 內寫 JSON、本機讀回」（`blender_client.run_code_capture`）。
3. godot-mcp `get_debug_output` 初期回傳空 → 先做標記檔 + CLI stdout 協定；後期再測已可正常取得輸出（先 stop 再 run、等待数秒），協定保留作為可重複測試基礎設施。
4. 自動化測試（bot/phys/截圖）需解析 stdout/存檔 → 一律 CLI（`tools/run_game.sh`）。

## 遇到的問題與解法（Phase 6–8 重點）

- **地面對不起來（-0.53m 常數偏移）**：硬編 GLB 頂面高度假設錯誤 → `_ground_offset()` 以 heights max 對 GLB AABB 自動對齊。
- **機器人衝出島**：牆只旋轉了 mesh、collision box 未旋轉 → 改為旋轉整個 body。
- **驱动力被摩擦吃掉**（速度卡 0.55）：膠囊與地面摩擦 0.9 平均組合 → 機器人摩擦 0 + 側向 grip 衰減；地面摩擦降到 0.05（桶仍靠自身 damp+摩擦停下）。
- **跳躍無效**：`test_move` 0.1m 探測太短 + `_jump_base<=0` 判斷 bug 造成每 tick 重跳 → 0.3m 探測 + jump buffer + 布林旗標。
- **附錄 B 抓到洋紅桶子**：`col_barrel` 除錯 hull 以 Magenta 材質被當普通網格渲染 → 遊戲端隱藏，重截圖 magenta=0。
- **bot 撿不到第 3 幣**：到站半徑 1.35 > 收集判定 1.24 → 半徑配對調整（最終 1.15/1.0）。

## 花最多時間的步驟

Phase 6–7 的「物理調通」：地面對齊 → 牆碰撞 → 摩擦/驅動模型 → 跳躍判定，每一項都要靠 raycast probe / DBG 行逐段定位；其次數遠超過任何建模迭代。

## 建議的下一步

1. `get_debug_output` 已可用 → 把 bot/phys 改跑在 MCP 上，砍掉標記檔旁路。
2. 桶子 slide 3.6m 接近上限 4.0 → 若想要更「重」的手感，把桶的 linear_damp 調高或地面對桶單獨用高摩擦層。
3. 樹的 Sway 動畫目前遊戲端是程序搖晃，GLB 內 Sway 未使用 → 可改用 AnimationPlayer 播放以求風格一致。
4. 加領币/勝利的音效與粒子回饋（目前只有硬幣消失）。

## Web 版（發佈入口）

repo 根 `preview-spec_04-...html` redirect → 資料夾 `index.html` = Godot 4.7.2 Web 匯出（`thread_support=false`，靜態主機可直接跑，無需 COOP/COEP）。headless Chrome + swiftshader 經 CDP 實測 30s 後有首幀、HUD `coins 0/3` 正常。WebGL(Compatibility) 路徑下畫面比原生 Forward+ 偏亮，屬渲染路徑差異；原生畫面見 `pipeline/screenshots/`。

## 執行時間

- 開始：2026-10-26 **13:20:46**（= 資料夾名尾段 `20261026132046`，repo 慣例為開始時間戳）
- 單一 commit：14:19:00；補入口頁 amend 後最終 14:21
- **總計約 60 分鐘**（Phase 0–10 全程含 3 次資產 FAIL 迴圈、Phase 6–8 物理除錯與 Phase 9 演練）

## Commit

依使用者要求：**全部壓成一個 commit**（各 Phase 的 commit 步驟在 progress.md 標 `[x]`）。
