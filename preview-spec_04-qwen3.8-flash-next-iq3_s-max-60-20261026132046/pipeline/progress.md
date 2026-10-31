# Progress — Coin Run 箱庭版 (PLAYBOOK v2.1)

標記：`[v]` 通過、`[x]` 做了沒過、`[ ]` 未做

## Phase 0 環境檢查
- [v] 0.1 Godot ≥ 4.3 / Blender / Node ≥ 18 / uv / git / Godot 路徑
- [v] 0.2 缺件 STOP（無缺則跳過）

## Phase 1 專案骨架
- [v] 1.1 專案結構規劃
- [v] 1.2 專案規則檔 AGENTS.md + CLAUDE.md
- [v] 1.3 MCP 伺服器與權限設定（依 .spec/PLAYBOOK_BLENDER_GODOT_MCP_SETUP.md）
- [v] 1.4 Godot 專案 + 定案按鍵 + 程式確認綁定
- [v] 1.5 自動化工具：環境檢查 / Godot 匯入 / 規格比對 / bot 測試 / 物理測試
- [v] 1.6 progress.md、log.md、git、第一次 Godot 匯入、commit

## Phase 2 連線 MCP
- [v] 2.1 人工：Blender 裝 addon + Start MCP Server + 重啟 agent（使用者已完成）
- [v] 2.2 agent 驗證：環境檢查通過 + Godot MCP 唯讀工具 OK + Blender MCP 唯讀工具 OK

## Phase 3 冒煙測試
- [v] 3.1 選最簡單資產（rock）：寫規格
- [v] 3.2 Blender 建模
- [v] 3.3 Blender 端自檢外觀
- [v] 3.4 匯出 → Godot 匯入 → 規格檢查 PASS
- [v] 3.5 放進測試場景 → Godot MCP 執行無錯誤
- [x] 3.6 commit

## Phase 4 資產（規格→建模→自檢→自動檢查迴圈）
- [v] 4.0 rock（冒煙資產，已完成全迴圈）
- [v] 4.1 robot（含 WheelSpin 動畫）
- [v] 4.2 coin
- [v] 4.3 flag
- [v] 4.4 crate
- [v] 4.5 barrel（含碰撞）
- [v] 4.6 tree（含 Sway 動畫 + 葉）
- [v] 4.7 bush（含 Sway 動畫）
- [v] 4.8 wall
- [v] 4.9 house
- [v] 4.10 ancient tower
- [v] 4.11 ancient shrine
- [v] 4.12 cloud
- [x] 4.13 commit

## Phase 5 地面
- [v] 5.1 ground 規格
- [v] 5.2 ground 建模（厚度/土層/起伏/草坪/道路/點綴）
- [v] 5.3 自動檢查 PASS；物件可正確落在起伏地面
- [x] 5.4 commit

## Phase 6 世界組裝
- [v] 6.1 layout.json 版面（可重現、可调）
- [v] 6.2 WorldSpawner 組出地面/牆/元素/金幣/終點
- [x] 6.3 commit（併入最終單一 commit）

## Phase 7 遊戲邏輯、物理與氣氛
- [v] 7.1 收集 3 金幣 + 碰旗勝利
- [v] 7.2 角色操控：力向量 + 阻力/慣性模型（a0=12, k=3 → v∞=4 m/s）
- [v] 7.3 物理互動：加速傾斜、推木桶、撞樹搖晃掉葉、穿草叢晃
- [v] 7.4 鏡頭等角 45° 平滑跟隨（固定 offset (9,11,9) + lerp）
- [v] 7.5 氣氛：暖色陽光/軟陰影/glow、雲漂移、樹草微風（景深未開，見 log）
- [v] 7.6 bot 模式（--mode bot、虛擬按鍵、依序 waypoints→終點）
- [x] 7.7 commit（併入最終單一 commit）

## Phase 8 測試與畫面檢查
- [v] 8.1 bot 測試通過（10 stages、3 幣、VICTORY，見 checks/bot_test.log）
- [v] 8.2 物理測試通過（7/7，門檻 specs/physics_tests.json，見 checks/physics_check.json）
- [v] 8.3 Godot MCP 執行無錯誤（0 errors 0 warnings）
- [v] 8.4 截圖（遊玩 + 全覽）依附錄 B 檢查；修正 col_barrel 洋紅渲染後重拍
- [x] 8.5 commit（併入最終單一 commit）

## Phase 9 迴圈演練
- [v] 9.1 log 回饋：金幣太大 → 規格縮至 ~60%（0.36×0.36×0.07）
- [v] 9.2 自動檢查 FAIL（三端 0.6 vs 0.36）並記錄 → 檢查機制有效
- [v] 9.3 回 Blender 重建 → 匯出 → PASS（中途還抓到一个流程漏跑 stats 的 pipeline 問題）
- [v] 9.4 遊戲端同步（收集半徑、bot 到站半徑）；bot + 物理測試重跑仍 PASS
- [v] 9.5 重跑確認畫面（截圖重拍、MCP run 0 錯誤）
- [x] 9.6 commit（併入最終單一 commit）

## Phase 10 收尾
- [v] 10.1 pipeline/REPORT.md
- [v] 10.4 最終全量驗證：14 資產 asset_check 全 PASS、物理 7/7、bot VICTORY、MCP run 0 錯誤
- [v] 10.2 commit（單一 commit f42474b）
- [v] 10.3 回報使用者
