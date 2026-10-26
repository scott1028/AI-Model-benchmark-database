# Coin Run PoC 完成報告（PLAYBOOK_v2）

日期：2026-10-26 ｜ 環境：Blender 5.2.2 LTS（blender-mcp addon 1.8）＋ Godot 4.7.2（godot-mcp）＋ opencode/agent-switch

## 1. DoD 勾選結果

- [v] 14 個資產 `check_assets` 全部 PASS（player / coin / crate / goal_flag / tree / bush / rock / wall_brick / house / mage_tower / temple_ruins / barrel / terrain / cloud）
- [v] `node tools/bot_test.mjs` PASS：`COIN 3/3` + `WIN`，0 個 `SCRIPT ERROR` / `ERROR:`
- [v] `node tools/physics_test.mjs` PASS：9 項檢查全 PASS
- [v] `run_project` → `get_debug_output` 錯誤與警告歸零；跟隨截圖與全景截圖依 Phase 9 清單檢查通過（`pipeline/screenshot.png`、`pipeline/screenshot_overview.png`）
- [v] 迴圈演練完成：coin 0.5→0.3 規格變更 → check FAIL → Blender 縮放 → PASS，log 有完整 iter 紀錄
- [v] `pipeline/REPORT.md`（本檔）

## 2. 每個資產的 check_assets 迭代次數

| 資產 | iter | 備註 |
|---|---|---|
| crate | 1 | 建模過程內修 2 次（板子定位、斜撐遮蓋），匯出前 self-fix |
| player | 1 | Blender 5.x 動畫 API 差異於建模內處理 |
| coin | 2 | iter 2 = Phase 10 迴圈演練（規格變更，非失誤） |
| goal_flag / tree / bush / rock / cloud | 各 1 | |
| barrel | 1 | 首版 416 tris 超 400 → 匯出前改鐵箍為無蓋圓筒 368 |
| wall_brick / house / mage_tower / temple_ruins | 各 1 | 824 / 572 / 752 / 548 tris |
| terrain | 1 | 匯出前 self-fix 2 次（裝飾 z 含 BASE、pebbles 密度）；29,988 tris ≤ 40,000 |

無任何資產用到超過 1 次「check FAIL → 回 Blender」的迭代（coin 的 iter 2 是刻意演練）。

## 3. MCP 問題與替代做法（詳見 log.md「MCP 問題」）

- godot-mcp `create_scene` 不接受 root 名稱（固定 "root"）→ 直接編輯 .tscn 改名。
- `add_node` 的 scenePath 帶 `res://` 會報錯 → 改不帶前綴的相對路徑；complex 屬性（shape/mesh/Transform/Resource/instance）一律直接編輯 .tscn（C-5 預期內）。
- `get_debug_output` 在 `run_project` 後立即呼叫回空 → 等 5–10 秒再讀。
- `save_scene` 對未開啟的場景報錯；`create_scene` 已存檔。
- blender `look` target 要用物件名而非 collection 名；view 需 3 數字向量。
- Blender 5.x：Action 無 `fcurves` → 用 `keyframe_new_interpolation_type='LINEAR'`。

## 4. physics_test 實測數字

| # | 檢查 | 實測 |
|---|---|---|
| 1 | 按住方向 1.5 s 極速 | 3.992（要求 3.8–4.05） |
| 2 | 放開 1 s 後 | 0.000000 |
| 3 | 0.1 s 空中 vs 地面 | 0.496 < 1.356×0.5 |
| 4 | 空中 4 m/s 按 1 s | 4.000（≤4.01，極速剛好不增快，C-13 設計驗證） |
| 5 | boost 2 s | 6.998（要求 6.7–7.05） |
| 6 | 煞車 vs 滑行（自 7 m/s） | 煞車 23 格 ≤30，< 滑行 44 格 ×0.7 |
| 7 | 最近 (-13.5,-2) 的樹葉粒子 | emitting=true（Tree04） |
| 8 | 同樹搖曳角 | 0.0374 rad > 0.005 |
| 9 | 最近 (14,-3) 木桶推 120 格 | 位移 0.526 m > 0.3 |

## 5. 遇到的坑（對照附錄 C；★=新出現、建議補進 C）

依附錄 C 預期內踩到/迴避成功：C-3（wheel_spin 1.042 s → 執行時改 length）、C-5（complex 屬性走 .tscn）、C-12（bush.gd `super()`）、C-13（step_velocity 空中阻力設計內建）、C-14（推桶衝量公式）、C-16（overview 陰影距離 128）、C-2/C-4/C-8（命名、A0、linear 色，未踩到）。

★ 新出現的坑：
1. **.tscn `instance=` 節點的 `transform=` 寫在 node header 括號內會被 Godot 靜默忽略**（所有 props 疊在origin、玩家卡死）。必須寫成下一行 `transform = Transform3D(...)`。文字解析的 route/flat 檢查抓不到，只有跑 Godot 才暴露。
2. `Environment.sky =` 需要 **Sky 資源**；直接塞 ProceduralSkyMaterial → 背景全黑、無報錯。
3. `Input.get_vector(neg_x, pos_x, neg_y, pos_y)`：`ui_up` 是 neg_y（第 3 參數）；放反 → 前後鍵反向，bot 模式不會暴露，只有按鍵驅動的場景測試抓到。
4. DirectionalLight3D 光線方向 = **-basis.z**；符號搞反時雲的投影補償把雲群推到無限遠。
5. Godot 4.7：CameraAttributesPractical DOF 是 `dof_blur_amount` + `dof_blur_near/far_distance/transition`（`dof_blur_far_amount` 已不存在）；`Particles3D` 不可用作註解型別；CPUParticles3D 無 `process_material`/`material`（用 CPU 參數 + `material_override`）；emission shape 列舉在 `ParticleProcessMaterial`。
6. .tscn 的 `Curve._data` tangent/interpolation 欄位必須是 INT（寫 `0.0` 直接 parse fail）。
7. `@export var x: NodePath` 在 .tscn 未指定時不會自動解析節點 → `_ready()` 需 fallback。
8. `extends Node3D` 的腳本掛到 `type="Node3D"` 沒問題，但改腳本基類（如 StaticBody3D）與 .tscn root 型別不一致時，每個 instance 都報一次錯，訊息不含節點路徑，難定位——腳本基類與 .tscn root 型別要一起改。
9. Godot `Vector3.FORWARD` = **(0, 0, -1)**（-Z 慣例）。測試裡拿它與自訂 +Z 速度同軸混用，check 4 會以「反向煞車」的姿態通過；跨模組自定 forward 要明寫 `Vector3(0, 0, 1)`。

## 6. 卡最久的步驟

Phase 8 bot_test 玩家「vel=(0,0,0) 原地不動」：先修了 3 條 parse error、再怀疑物理參數、最後靠逐格印 slide collision 路徑才發現是坑 #1（world.tscn 所有 instance transform 無效，建築碰撞疊在出生點）。約 4 個迭代都在這條上。

教訓：**手工生成的 .tscn，第一批 bot_test 應該先斷言關鍵節點的 runtime global_position**（build_world 的驗證目前只查文字座標）。

## 7. 建議的下一步

1. 把坑 #1/#2 補進 PLAYBOOK 附錄 C；build_world.mjs 輸出後加一步「Godot headless 載入 world.tscn、比對每個 instance 的 global_position」自動斷言。
2. physics_test 的場景檢查補一條「玩家從出生點按 ui_up 1 s 位移 > 2 m」，能更早抓到方向/阻塞類 bug。
3. `check_assets` 增加「同資產 glb 內 StaticBody 數量與名稱」檢查，防止 -col 命名誤用。
4. 若繼續發展玩法：把 bot 導航換成 NavigationAgent3D，順便驗 navmesh。

## 8. 加碼：Web 匯出（使用者指定）

- `game/export_presets.cfg`（Web preset、thread_support=false → 不需 COOP/COEP headers）
- 匯出產物在 preview 目錄根：`index.html` / `index.js` / `index.pck`（1.3 MB）/ `index.wasm`（39.5 MB）+ 圖示/works + `README.txt`（本地 `python3 -m http.server 8061` 遊玩說明）
- repo 根 `preview-spec_03-qwen3.8-flash-next-iq3_s-max-25-20261026123341.html` → redirect 至該 index.html

## 9. Git 軌跡（外層 benchmark repo，preview 目錄範圍）

依使用者要求，Phase 1–10 的 13 個 commit 已 squash 為單一 commit（原 hash：75aad6d/077044b/b7a9b47/a521e8d/016b942/d41d5e9/ccb1285/2ae7641/9a1e5b5/5f8c565/a4e20e9/83d5ab4/9286fb0，可在 reflog 追溯）。

## 10. 遊玩方式（Web 版）

preview 目錄根已附 Web 版（`index.html`）。因瀏覽器限制需走 HTTP：

```bash
cd preview-spec_03-qwen3.8-flash-next-iq3_s-max-25-20261026123341
python3 -m http.server 8061
# 開 http://127.0.0.1:8061/
```

repo 根的 `preview-spec_03-qwen3.8-flash-next-iq3_s-max-25-20261026123341.html` 會自動 redirect 過去。
