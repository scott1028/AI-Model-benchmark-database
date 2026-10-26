# Coin Run 箱庭版 — 專案規則（給 agent）

本專案依 `.spec/PLAYBOOK_v2.1.md` 執行。本檔是執行契約：所有建模、匯出、檢查、Godot 端作業都照這裡的約定。

## 目錄結構

```
.spec/          playbook
pipeline/       progress.md / log.md / checks/ / screenshots/ / REPORT.md
specs/          資產規格 JSON（唯一真相）
tools/          自動化工具（環境檢查、匯入、規格檢查、bot/物理測試）
blender/        Blender 原始檔（coin_run.blend 由 Blender 自身存檔）
godot/          Godot 專案（assets/ 為 glb 匯出目標，勿手改 glb 與 .import）
```

## 單位與座標約定

- 1 Blender unit = 1 m；Blender 公制單位，godot 亦為公尺。
- Blender Z-up、角色正面朝 **-Y**；glTF 匯出用預設（+Y up），Godot 端角色正面為 **-Z**。換算：Godot(x, y, z) = Blender(x, z, -y)。
- 所有資產原點 = **底面中心**（世界 bbox 的 zmin = 0，xy 置中；容差見規格）。
- 地圖可活動區域 40×40 m，居中於世界原點；地面頂面約 z=0（Blender），起伏 ±0.6 m。

## Blender 建模與匯出規則

- 單一檔 `blender/coin_run.blend`；每個資產一個 collection：`A_<name>`（視覺）與需要碰撞時的 `A_<name>_COL`（`col_*` 凸包網格）。
- 材質：Principled BSDF，只設 base color（平塗）、roughness≈0.8、metallic=0；不使用貼圖。節點一律用 `type` 查，不用名稱。
- 建模只用 `execute_blender_code`，**每次呼叫只做一小步**（一個幾何區塊或一個材質批次）；不刪專案外檔案；不下載外部資產、不用外部資產庫/生成器。
- `primitive_*_add` 會加入 active collection，不是目標 collection：建立物件後一律手動 unlink/`col.objects.link()` 到 `A_<name>`（本專案踩過）。
- 匯出（由 `tools/export_asset.py` 執行，經 Blender TCP）：選取 `A_<name>` collection、`export_apply=True`、不含燈光/攝影機、動畫含在內、檔名 `godot/assets/<name>.glb`。
- 碰撞節點以 `col_` 前綴放在同 collection 內一起匯出；Godot 端若 importer 不吃 `col_`，碰撞改在遊戲端以規格中的 `collision` 欄位建primitive，並在 log 記錄。
- 動畫命名：`WheelSpin`（機器人輪子）、`Sway`（樹/草叢）。

## 匯出後必做檢查（每個資產）

1. Blender 端自檢：外觀截圖（TCP `get_viewport_screenshot`）+ `tools/blender_client.py` 量測（尺寸/面數/原點/動畫）→ `pipeline/checks/<name>_blender.json`。
2. `tools/godot_import.sh` 匯入 Godot。
3. `tools/asset_check.py <name>`：解析 glb + 跑 Godot headless `verify_asset.gd`（匯入後 AABB/動畫），與 `specs/<name>.json` 比對；尺寸容差 max(10%, 2cm)。
4. 全部 PASS 才算完成；FAIL → 依工具給的修正建議判斷是**建模問題**（回 Blender 修）或 **pipeline 問題**（修工具/設定），記 log 後重跑。同一資產每輪最多 3 次修正，超過 STOP 問使用者。
5. 不可手動修改 `godot/assets/*.glb` 或 `*.glb.import`；要改一律回 Blender 重匯。

## Godot 端規則

- 只動 `godot/` 專案內檔案；場景、腳本、tscn 由 agent 直接寫檔（Godot MCP 無場景編輯工具，原因記 log「MCP 問題」）。
- 輸入動作僅用使用者定案按鍵：`move_up/down/left/right`（方向鍵）、`accelerate`（鍵盤 A / 手把 X）、`brake`（鍵盤 B / 手把 B）、`jump`（Space / Enter / 手把 A）。不得新增其他按鍵。
- 執行遊戲優先用 Godot MCP（`run_project` + `get_debug_output`）；自動化測試用 headless/視窗模式 CLI，原因記 log。
- 版面一律由 `godot/data/layout.json` 驅動（`WorldSpawner` 重現版面），不逐一手動擺件。
- 測試輸出協定（stdout 行，工具解析）：
  - bot：`BOT_STAGE <n>`、`BOT_COIN <i>`、`BOT_VICTORY`、`BOT_DONE`
  - 物理：`PHYS <name> <metric>=<value>`
  - 任何腳本錯誤都算失敗。

## 進度與紀錄

- `pipeline/progress.md`：步驟完成打勾（`[v]` 通過、`[x]` 做了沒過、`[ ]` 未做）。
- `pipeline/log.md`：設計決策（1–2 行理由）、每次檢查與修正（時間/資產/第幾次/結果/判斷/修正請求/結果）、MCP 問題。
- 每個 Phase 完成後 `git commit`（本倉庫由使用者要求全部壓成一個 commit，見 log）。
