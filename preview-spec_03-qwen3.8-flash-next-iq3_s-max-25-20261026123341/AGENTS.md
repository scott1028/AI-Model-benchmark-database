# MCP Game PoC（箱庭版）

流程與步驟以 .spec/PLAYBOOK_v2.md 為準；進度記在 pipeline/progress.md。

## 座標與單位
- 單位是公尺。assets.json 的 size_m 一律用 Godot 軸向 [X, Y(上), Z]
- Blender 是 Z 朝上：Godot (X, Y, Z) = Blender (X, Z, -Y)
- 角色正面朝 Blender -Y，匯出後在 Godot 是朝 +Z
- world_layout.json 的座標是 Godot (x, z)：x 朝東、z 朝南

## Blender 規則
- 每個資產一個 collection，名稱 = asset id，在世界原點建模
- 匯出前該 collection 必須在 view layer 中可見（不可 exclude）
- 原點 bottom_center：底部 z=0、XY 置中
- 低模、平塗：只用 Principled BSDF 的 Base Color（linear 值），不用圖片貼圖
- collision 為 "col" 的資產，有碰撞的 mesh 物件名稱加 -col 後綴；其他物件名稱結尾不可撞到 Godot 匯入提示字（-noimp、-rigid、-vehicle、-wheel、-navmesh、-convcol、-colonly 等，底線也算）
- execute_blender_code 每次只做一小步；不刪專案外的檔案；不下載外部資產
- 模型匯出只用 PLAYBOOK 附錄 A0（匯出前準備）+ 附錄 A（標準 snippet）

## 每次匯出後必做
1. node tools/import.mjs
2. node tools/check_assets.mjs <id>

## 迴圈規則
- FAIL → 依 PLAYBOOK 附錄 B 的格式寫進 pipeline/log.md → 回 Blender 修
- 同一資產每輪最多 maxIterations 次，超過就停下來問使用者
- 不要手動修改 .glb 或 .import 檔

## Godot 規則
- 腳本放 game/scripts/，場景放 game/scenes/
- 輸入只用內建的 ui_left / ui_right / ui_up / ui_down / ui_accept（另加手把 A 跳躍），以及自訂的 boost（鍵盤 A、手把 X）/ brake（鍵盤 B、手把 B）；input map 定義在 game/project.godot
- 優先使用 Godot MCP 工具；做不到再直接編輯 .tscn，並記到 log 的「MCP 問題」
- game/scenes/world.tscn 只由 node tools/build_world.mjs 產生，不要手改；要改版面就改 world_layout.json
