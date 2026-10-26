# Log — Coin Run 箱庭版 (PLAYBOOK v2.1)

## 設計決策

- 2026-10-26 气氛：等角鏡頭不加 DOF（移軸景深）。理由：自動截圖/測試以畫面判讀為主，DOF 對小地圖收益低、易造成焦點誤判；氣氛改由暖色陽光+filmic+glow+天空漸層承擔。已於 7.5 標注。

- 2026-10-26 成品放在 benchmark repo 的 `preview-spec_04-*` 資料夾（跟 repo 既有命名慣例），不另建 git repo；外層已是 git repo，Phase 1.6 的 `git init` 跳過。使用者要求全部壓成一個 commit，故各 Phase 不再個別 commit，progress.md 照常打勾。
- 2026-10-26 自動化工具直接走 Blender addon 的 TCP 9876（探測過協定 v15 可用），这样檢查/匯出可由 CLI 重跑，不必每次经 agent 轉發；建模仍全部经 Blender MCP `execute_blender_code`。
- 2026-10-26 座標約定：Blender Z-up、正面 -Y；glTF 預設 +Y up；Godot 正面 -Z。原點統一「底面中心」。
- 2026-10-26 機器人用 RigidBody3D + 力/阻力模型（playbook 要求力向量+摩擦力+慣性；RigidBody 天然支持推木桶），CharacterBody 不做。
- 2026-10-26 版面对 `godot/data/layout.json` 驅動（WorldSpawner 執行期組裝），調整版面只改 JSON，滿足「不逐一手動搬上百個物件」。
- 2026-10-26 草叢無實體碰撞（Area3D，可穿過）；樹为 StaticBody 圓柱擋人、搖晃/落葉用動畫與粒子；木桶 RigidBody3D 可推；木箱 StaticBody 固定。
- 2026-10-26 測試輸出協定：stdout 行 `BOT_*` / `PHYS_*`，由 tools 解析比對 `specs/physics_tests.json` 門檻。
- 2026-10-26 截圖由遊戲內 `save_png` 寫到 pipeline/screenshots（有 DISPLAY=:0，遊戲視窗模式跑）。

## MCP 問題

- 2026-10-26 godot-mcp 只有 run/import/project 資訊類工具，沒有場景編輯工具 → Godot 端 tscn/gd/tres 由 agent 直接寫檔，執行與錯誤擷取仍用 MCP（run_project/get_debug_output）。
- 2026-10-26 blender-mcp `execute_code` 不回傳 print 輸出 → 工具用「Blender 內寫 JSON 檔、本機讀回」模式（tools/blender_client.run_code_capture）。

## 環境檢查

- 2026-10-26 Godot 4.7.2 stable（/home/scott/tools/bin/godot）、Blender 5.2.2 LTS + MCP addon 1.8（埠 9876 由 Blender 監聽，protocol 15）、node v22.22.3、uv 0.12.18、git 2.53.0、DISPLAY=:0。Phase 0 PASS。
- 2026-10-26 Blender MCP `get_addon_status`、Godot MCP `get_godot_version` 皆成功（Phase 2 人工步驟使用者已完成，MCP 已掛上，直接驗證通過）。

## 檢查與修正記錄

- 2026-10-26 Phase 9 迴圈演練（Godot 端回饋資產修改）：遊戲截圖裡直徑 0.6 m 的金幣在等角鏡頭下偏大、搶視覺且稍微擋到地面道路線條 → 回饋：金幣太大、擋視線。規格從 [0.6,0.6,0.12] 縮到約 60% = [0.36,0.36,0.07]。先改規格跑檢查（應 FAIL）驗證機制，再回 Blender 重建。

| 時間 | 資產 | 第幾次 | 檢查結果 | 判斷 | 對 Blender 的修正請求 | 結果 |
|---|---|---|---|---|---|---|
| 2026-10-26 | coin(P9-1) | 1 | FAIL: blender/glb/godot 三端皆 0.60×0.60×0.12 vs 新規格 0.36×0.36×0.07（工具建議 scale 0.60/0.60/0.586） | model（規格已縮，模型未跟） | 重建 coin：fit 目標改 [0.36,0.36,0.07]，重跑 build+export | 見下一列 |
| 2026-10-26 | coin(P9-2) | 1(重建後) | 第一次重跑 FAIL：glb 已 0.36 但 coin_blender.json 是舊統計（流程漏跑 stats） | pipeline（步驟順序） | 補跑 tools/blender_stats.py coin | PASS：blender/glb/godot 皆 0.36×0.36×0.07，tris 140/300 |
| 2026-10-26 | coin(P9-3) | — | 遊戲端同步：coin Area 收集半徑 1.2→1.0、bot 到站半徑 1.35→1.15 | 場景設定 | — | bot（3 幣+VICTORY）與物理 7/7 重跑仍 PASS；重拍截圖金幣比例正常；MCP run 0 錯誤 |
| 2026-10-26 | rock | 1 | FAIL: godot dim_y=0.900/dim_z=1.000 vs target y=1.0/z=0.9 | pipeline（檢查工具把 Godot Y-up dims 直接對 spec Z-up 軸序比對） | 無需回 Blender（Blender 端實測 1.2/1.0/0.9 正確） | 修 asset_check.py 軸序換算後 PASS |
- 2026-10-26 MCP 問題：godot-mcp `run_project` 可啟動遊戲（pgrep 確認 `godot -d --path ...` 存在），但初期 `get_debug_output` 回傳空、`stop_project` 找不到程序 → 遊戲內改寫結果標記檔（pipeline/checks/*.txt）+ 需要解析 stdout 的測試改用 CLI 啟動（tools/run_game.sh），原因記此。
- 2026-10-26 更正：Phase 8 再次試 godot-mcp `run_project`+`get_debug_output`（先 stop 再 run，等 ~8s 再取）→ **可正常拿到 stdout/stderr**（抓到 Robot.gd shadowing 警告並修復後歸零）。標記檔與 CLI 協定保留作為可重複測測定的基礎設施。
- 2026-10-26 Phase 3 commit 併入最終單一 commit（使用者要求全部壓成一個 commit）。progress 標 [x]3.6 = 本 Phase 未單獨 commit。
- 2026-10-26 Godot 4.7 API 差異：InputEventJoypadButton 用 `button_index`（非 `button`）；InputMap 用 `erase_action`；執行期綁鍵要 `ProjectSettings.set_setting("input/<action>", {deadzone, events})` 再 save 才會寫進 project.godot。
- 2026-10-26 asset_check 曾誤把 Godot Y-up dims 直接對 spec Z-up 比對 → rock 第一次 FAIL（pipeline 類），修工具後 PASS。
- 2026-10-26 Blender 5.2 layered action API：keyframe 存在 `act.layers[].strips[].channelbags[].fcurves`（`act.fcurves`/`layer.slots` 不可用）；修改插值要走這條路徑。已寫進 robot_03 步驟檔。
| 2026-10-26 | flag | 1 | FAIL dim_y 0.45 vs 0.6 | model（build 步驟 fit 目標與規格不一致） | 重建 fit [1.1,0.6,3.0] | PASS |
| 2026-10-26 | barrel | 1 | FAIL dims 6.74 | model（col_barrel 未 parent 到 AssetRoot，grid offset 位移不一致） | col 加入 parts 一起 fit | PASS(見下) |
| 2026-10-26 | coin/crate | 1 | rebuild 同步 fit API | — | — | PASS |
| 2026-10-26 | tree | 1 | FAIL anim 'Sway'（blend 內用唯一名 Sway_tree）+ bbox 空物件汙染 | pipeline（檢查工具不接受唯一動名；export 置中把 empty bound_box 算進去） | export/檢查改用 mesh-only bbox；檢查接受 <name> 前綴 | PASS |
| 2026-10-26 | bush | 1 | FAIL godot zmin 0.14 | pipeline（同上 export 置中污染） | 同上 | PASS |
| 2026-10-26 | tree | 1b | fit_bbox 巢狀 parent（SwayRoot）一次算不準 | pipeline | fit_bbox 改迭代收斂 | PASS |
