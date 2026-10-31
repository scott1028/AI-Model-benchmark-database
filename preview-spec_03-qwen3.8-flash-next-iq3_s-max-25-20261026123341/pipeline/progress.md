# Coin Run pipeline 進度（PLAYBOOK_v2）

> 標記：`[v]` 已處理且通過、`[x]` 做了沒過、`[ ]` 未做。重啟後從未完成的步驟續跑。
> 本專案根目錄：preview-spec_03-qwen3.8-flash-next-iq3_s-max-25-20261026123341/
> 本環境 MCP 由 agent-switch 掛載（見 .spec/PLAYBOOK_BLENDER_GODOT_MCP_SETUP.md），MCP 已連線。

## Phase 0 環境檢查
- [v] 0.1 node v22.22.3、uv 0.12.18、git 2.53.0
- [v] 0.2 Godot 於 PATH：/home/scott/tools/bin/godot
- [v] 0.3 godot --version = 4.7.2（≥ 4.3）
- [v] 0.4 無缺件（9876 由 blender 佔用，正確）

## Phase 1 專案骨架
- [v] 1.1 pipeline.config.json
- [v] 1.2 assets.json（照抄）
- [v] 1.3 world_layout.json（照抄）
- [v] 1.4 .mcp.json、.claude/settings.json（照抄留檔；實際 MCP 由 agent-switch 掛載）
- [v] 1.5 .gitignore
- [v] 1.6 game/project.godot（features 4.7）
- [v] 1.7 game/tools/inspect_assets.gd（照抄）
- [v] 1.8 tools/check_env.mjs
- [v] 1.8 tools/import.mjs
- [v] 1.8 tools/check_assets.mjs
- [v] 1.8 tools/bot_test.mjs
- [v] 1.8 tools/physics_test.mjs
- [v] 1.8 tools/build_world.mjs（待 terrain_heights.json 後才可執行）
- [v] 1.9 CLAUDE.md（照抄）＋ AGENTS.md（同內容，opencode 規則檔）
- [v] 1.10-1 pipeline/progress.md、pipeline/log.md
- [v] 1.10-2 git init（外層 benchmark repo 已有 git → 沿用外層 repo，見 log）
- [v] 1.10-3 node tools/import.mjs（main.tscn 錯誤屬預期）
- [v] 1.10-4 git commit（已 squash 入單一 commit）

## Phase 2 連線驗證（人工安裝已在會話外完成 → 不 STOP，直接驗證）
- [v] 2-V1 node tools/check_env.mjs 全 OK
- [v] 2-V2 godot MCP get_godot_version 成功（4.7.2）
- [v] 2-V3 blender MCP get_addon_status 成功；get_scene_info 成功
- [v] 2-V4 全部通過

## Phase 3 冒煙測試（crate）
- [v] 3.1 Blender：刪預設 Cube、建 collection crate、附錄 D 建模
- [v] 3.2 look 自檢（angles、material shading）
- [v] 3.3 附錄 A0 → 附錄 A（ASSET=crate）
- [v] 3.4 node tools/import.mjs
- [v] 3.5 node tools/check_assets.mjs crate → PASS（iter 1）
- [v] 3.6 Godot MCP 建 test_crate.tscn + instance crate.glb（直接編輯 .tscn）
- [v] 3.7 run_project(test_crate) → get_debug_output 無錯誤 → stop_project
- [v] 3.8 git commit（已 squash 入單一 commit）

## Phase 4 角色與道具（player coin goal_flag barrel tree bush rock cloud）
- [v] 4-player 建模 + A0/A + import + check PASS（iter 1）
- [v] 4-coin 建模 + A0/A + import + check PASS（iter 1；凹面深度補償重做 1 次於匯出前）
- [v] 4-goal_flag 建模 + A0/A + import + check PASS（iter 1）
- [v] 4-barrel 建模 + A0/A + import + check PASS（iter 1；416 tris 超標→鐵箍改無蓋圓筒 368）
- [v] 4-tree 建模 + A0/A + import + check PASS（iter 1）
- [v] 4-bush 建模 + A0/A + import + check PASS（iter 1）
- [v] 4-rock 建模 + A0/A + import + check PASS（iter 1）
- [v] 4-cloud 建模 + A0/A + import + check PASS（iter 1）
- [v] 4.2 player 額外確認：robot_tire MeshInstance3D、wheel_spin 軌道=robot_tire（len 1.042 → C-3 於 player.gd 處理）
- [v] 4.3 git commit（已 squash 入單一 commit）

## Phase 5 建築（wall_brick house mage_tower temple_ruins）
- [v] 5-wall_brick 建模 + A0/A + import + check PASS（iter 1，824 tris）
- [v] 5-house 建模 + A0/A + import + check PASS（iter 1，572 tris）
- [v] 5-mage_tower 建模 + A0/A + import + check PASS（iter 1，752 tris）
- [v] 5-temple_ruins 建模 + A0/A + import + check PASS（iter 1，548 tris）
- [v] 5.2 git commit（已 squash 入單一 commit）

## Phase 6 地形
- [v] 6.1 Blender 高度場（min −0.351 / max 0.918，8 flat_pads 全 0）
- [v] 6.2 terrain_ground-col（含側面與底面，11 色 material）
- [v] 6.3 terrain_cobble 436 塊 / terrain_flora 1400 撮+14 群花 / terrain_pebbles（首版 16/公尺→改 4/公尺 133 顆）
- [v] 6.4 look 近看檢查（色塊柔和、廣場/道路清楚；地表物件 z 補 BASE 修正 1 次）
- [v] 6.5 pipeline/terrain_heights.json 寫出
- [v] 6.6 A0 → A（terrain）→ import → check PASS（29,988 tris ≤ 40,000）
- [v] 6.7 git commit（已 squash 入單一 commit）

## Phase 7 互動道具場景與世界
- [v] 7.1 tree.tscn / bush.tscn / barrel.tscn：MCP create_scene + add_node 骨架，其餘（instance/shape/Gradient/Curve/材質/腳本）直接編輯 .tscn
- [v] 7.2 wind.gd / sway.gd / tree.gd / bush.gd
- [v] 7.3 node tools/build_world.mjs → BUILD_WORLD: 107 props (67 placed + 40 walls)，路線/平整檢查通過
- [v] 7.4 node tools/import.mjs 無錯誤（main.tscn 尚未建立屬預期）
- [v] 7.5 git commit（已 squash 入單一 commit）

## Phase 8 主場景與腳本
- [v] 8.1 main.tscn 結構（附錄 F.3）
- [v] 8.2 main.gd / player.gd / coin.gd / goal.gd / clouds.gd
- [v] 8.3 node tools/import.mjs 無 UID 警告、無錯誤
- [v] 8.3+ node tools/bot_test.mjs → PASS（COIN 3/3 + WIN、0 errors；iter 4，見 log）
- [v] 8.4 git commit

## Phase 9 測試與畫面檢查
- [v] 9.1 game/tools/physics_test.gd 實作（6 靜態 + 3 場景檢查）
- [v] 9.2 node tools/bot_test.mjs → PASS
- [v] 9.3 node tools/physics_test.mjs → PASS（iter 2：ui_up 方向修正後 9 項全 PASS）
- [v] 9.4 run_project → debug 無錯誤（2 WARNING 已清）→ stop → screenshot.png 檢查 OK
- [v] 9.5 --overview 全景 → screenshot_overview.png（iter 2：sky/雲修正後 OK）
- [v] 9.6 兩張截圖清單檢查通過（移軸模糊、雲影、遠半邊樹影、無倒立/懸空/陷地）
- [v] 9.7 git commit（已 squash 入單一 commit）

## Phase 10 迴圈演練（coin 0.5 → 0.3）
- [v] 10.1 log 寫 Godot 回饋
- [v] 10.2 assets.json coin.size_m 改 [0.3, 0.3, 0.06]
- [v] 10.3 check_assets coin → FAIL（尺寸超出）+ 附錄 B 記錄
- [v] 10.4 Blender 縮放 → A0+A → import → check PASS（iter 2）
- [v] 10.5 main.tscn Coin shape 0.5 → 0.3；bot_test / physics_test 仍 PASS
- [v] 10.6 run_project 重看截圖（硬幣變小確認）、git commit（已 squash 入單一 commit）

## Phase 11 收尾
- [v] 11.1 pipeline/REPORT.md（含 physics_test 實測、坑對照附錄 C 標新）
- [v] 11.2 git commit（本次提交）
- [v] 11.3 回報使用者（最終回報於 session）

## 加碼（使用者指定）：Web 匯出
- [v] W.1 Web 匯出到 preview 目錄根（index.html/.pck/.wasm + README.txt；threads off）
- [v] W.2 repo 根放 redirect html（preview-spec_03-qwen3.8-flash-next-iq3_s-max-25-20261026123341.html）
